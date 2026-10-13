/*
XDK_KERNEL.C

The Win32/XAPI kernel functions the game calls, over libdragon (n64.h):
files, time, events, memory and the debug output.

Files: reads come from the cartridge (DragonFS): "d:\maps\x" is
"rom:/maps/x". The game also opens files to write (its logs, d:\core);
those get a handle that swallows the data, since the cartridge cannot be
written. There are no threads on the N64 build: the game's own worker
threads belong to subsystems it leaves out (the cache's copy thread, sound),
so CreateThread reports itself and returns a handle that never runs.
*/

#include "platform.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *memalign(size_t alignment, size_t size);

/* ---------- logging */

void platform_log(const char *format, ...)
{
	char line[512];
	va_list arguments;

	va_start(arguments, format);
	vsnprintf(line, sizeof(line), format, arguments);
	va_end(arguments);
	n64_log(line);
}

void platform_unimplemented(const char *name)
{
	static const char *reported[64];
	static int reported_count;
	int i;

	for (i = 0; i < reported_count; i++)
	{
		if (reported[i] == name)
			return;
	}
	if (reported_count < 64)
		reported[reported_count++] = name;
	platform_log("unimplemented: %s", name);
}

void __stdcall OutputDebugStringA(const char *text)
{
	char line[512];
	size_t length;

	strncpy(line, text, sizeof(line) - 1);
	line[sizeof(line) - 1] = 0;
	length = strlen(line);
	while (length && (line[length - 1] == '\n' || line[length - 1] == '\r'))
		line[--length] = 0;
	n64_log(line);
}

/* ---------- errors */

static unsigned long last_error;

unsigned long __stdcall GetLastError(void)
{
	return last_error;
}

void __stdcall SetLastError(unsigned long error)
{
	last_error = error;
}

/* ---------- handles */

enum
{
	_handle_free,
	_handle_file,
	_handle_null_file,
	_handle_event,
	_handle_thread,
};

#define MAXIMUM_HANDLES 32

struct handle
{
	long type;
	long file;
	unsigned long position;
	unsigned long size;
	int signaled;
	int manual_reset;
};

static struct handle handles[MAXIMUM_HANDLES];

static struct handle *handle_new(long type)
{
	int i;

	for (i = 0; i < MAXIMUM_HANDLES; i++)
	{
		if (handles[i].type == _handle_free)
		{
			memset(&handles[i], 0, sizeof(handles[i]));
			handles[i].type = type;
			return &handles[i];
		}
	}
	n64_fatal("out of Win32 handles");
}

static struct handle *handle_get(void *pointer)
{
	struct handle *handle = (struct handle *)pointer;

	if (handle < handles || handle >= handles + MAXIMUM_HANDLES || handle->type == _handle_free)
		return NULL;
	return handle;
}

int __stdcall CloseHandle(void *object)
{
	struct handle *handle = handle_get(object);

	if (!handle)
	{
		last_error = ERROR_INVALID_HANDLE;
		return FALSE;
	}
	if (handle->type == _handle_file)
		n64_file_close(handle->file);
	handle->type = _handle_free;
	return TRUE;
}

/* ---------- files */

/* "d:\maps\wizard.map" -> "rom:/maps/wizard.map"; FALSE for paths that
cannot be on the cartridge */
static int rom_path(const char *path, char *buffer, size_t size)
{
	size_t length;
	const char *cursor = path;

	if (isalpha((unsigned char)cursor[0]) && cursor[1] == ':')
		cursor += 2;
	while (*cursor == '\\' || *cursor == '/')
		cursor++;
	length = snprintf(buffer, size, "rom:/%s", cursor);
	if (length >= size)
		return FALSE;
	for (char *c = buffer + 5; *c; c++)
	{
		if (*c == '\\')
			*c = '/';
		else
			*c = (char)tolower((unsigned char)*c);
	}
	return TRUE;
}

void *__stdcall CreateFileA(const char *path, unsigned long access, unsigned long share, void *security,
	unsigned long disposition, unsigned long flags, void *template_file)
{
	struct handle *handle;
	char translated[256];

	(void)share;
	(void)security;
	(void)flags;
	(void)template_file;
	if (access & GENERIC_WRITE)
	{
		/* the cartridge is read-only: writes vanish */
		handle = handle_new(_handle_null_file);
		return handle;
	}
	if (!rom_path(path, translated, sizeof(translated)))
	{
		last_error = ERROR_FILE_NOT_FOUND;
		return INVALID_HANDLE_VALUE;
	}
	{
		long file = n64_file_open(translated);

		if (file < 0)
		{
			last_error = ERROR_FILE_NOT_FOUND;
			return INVALID_HANDLE_VALUE;
		}
		handle = handle_new(_handle_file);
		handle->file = file;
		handle->size = n64_file_size(file);
	}
	(void)disposition;
	return handle;
}

int __stdcall ReadFile(void *file, void *buffer, unsigned long size, unsigned long *read, struct _OVERLAPPED *overlapped)
{
	struct handle *handle = handle_get(file);
	long count = 0;

	(void)overlapped;
	if (read)
		*read = 0;
	if (!handle || (handle->type != _handle_file && handle->type != _handle_null_file))
	{
		last_error = ERROR_INVALID_HANDLE;
		return FALSE;
	}
	if (handle->type == _handle_file)
	{
		n64_file_seek(handle->file, handle->position);
		count = n64_file_read(handle->file, buffer, size);
		if (count < 0)
			count = 0;
		handle->position += count;
	}
	if (read)
		*read = count;
	return TRUE;
}

int __stdcall WriteFile(void *file, const void *buffer, unsigned long size, unsigned long *written, struct _OVERLAPPED *overlapped)
{
	struct handle *handle = handle_get(file);

	(void)buffer;
	(void)overlapped;
	if (!handle)
	{
		last_error = ERROR_INVALID_HANDLE;
		return FALSE;
	}
	handle->position += size;
	if (handle->position > handle->size)
		handle->size = handle->position;
	if (written)
		*written = size;
	return TRUE;
}

unsigned long __stdcall SetFilePointer(void *file, long distance, long *distance_high, unsigned long method)
{
	struct handle *handle = handle_get(file);
	long position;

	if (!handle)
	{
		last_error = ERROR_INVALID_HANDLE;
		return 0xFFFFFFFF;
	}
	if (distance_high)
		*distance_high = 0;
	switch (method)
	{
	case FILE_BEGIN: position = distance; break;
	case FILE_CURRENT: position = (long)handle->position + distance; break;
	default: position = (long)handle->size + distance; break;
	}
	if (position < 0)
		position = 0;
	handle->position = position;
	return position;
}

unsigned long __stdcall GetFileSize(void *file, unsigned long *size_high)
{
	struct handle *handle = handle_get(file);

	if (size_high)
		*size_high = 0;
	return handle ? handle->size : 0xFFFFFFFF;
}

unsigned long __stdcall GetFileAttributesA(const char *path)
{
	char translated[256];
	long file;

	if (!rom_path(path, translated, sizeof(translated)))
		return 0xFFFFFFFF;
	file = n64_file_open(translated);
	if (file < 0)
	{
		last_error = ERROR_FILE_NOT_FOUND;
		return 0xFFFFFFFF;
	}
	n64_file_close(file);
	return FILE_ATTRIBUTE_NORMAL;
}

void *__stdcall FindFirstFileA(const char *pattern, struct _WIN32_FIND_DATAA *data)
{
	(void)pattern;
	(void)data;
	last_error = ERROR_FILE_NOT_FOUND;
	return INVALID_HANDLE_VALUE;
}

int __stdcall FindNextFileA(void *find, struct _WIN32_FIND_DATAA *data)
{
	(void)find;
	(void)data;
	last_error = ERROR_NO_MORE_FILES;
	return FALSE;
}

/* ---------- events and threads */

void *__stdcall CreateEventA(void *security, int manual_reset, int initial_state, const char *name)
{
	struct handle *handle = handle_new(_handle_event);

	(void)security;
	(void)name;
	handle->manual_reset = manual_reset;
	handle->signaled = initial_state;
	return handle;
}

int __stdcall SetEvent(void *event)
{
	struct handle *handle = handle_get(event);

	if (!handle)
		return FALSE;
	handle->signaled = TRUE;
	return TRUE;
}

unsigned long __stdcall WaitForSingleObject(void *object, unsigned long milliseconds)
{
	struct handle *handle = handle_get(object);

	(void)milliseconds;
	if (!handle)
		return WAIT_FAILED;
	if (handle->type == _handle_event)
	{
		if (!handle->signaled)
			return WAIT_TIMEOUT;
		if (!handle->manual_reset)
			handle->signaled = FALSE;
	}
	return WAIT_OBJECT_0;
}

void *__stdcall CreateThread(void *security, unsigned long stack_size,
	unsigned long (__stdcall *start)(void *), void *parameter, unsigned long flags, unsigned long *thread_id)
{
	(void)security;
	(void)stack_size;
	(void)start;
	(void)parameter;
	(void)flags;
	PLATFORM_UNIMPLEMENTED();
	if (thread_id)
		*thread_id = 1;
	return handle_new(_handle_thread);
}

unsigned long __stdcall ResumeThread(void *thread)
{
	(void)thread;
	return 0;
}

int __stdcall SetThreadPriority(void *thread, int priority)
{
	(void)thread;
	(void)priority;
	return TRUE;
}

int __stdcall SwitchToThread(void)
{
	return FALSE;
}

void __stdcall Sleep(unsigned long milliseconds)
{
	n64_wait_microseconds(milliseconds * 1000);
}

/* ---------- time */

int __stdcall QueryPerformanceCounter(union _LARGE_INTEGER *counter)
{
	counter->QuadPart = (long long)n64_ticks64();
	return TRUE;
}

int __stdcall QueryPerformanceFrequency(union _LARGE_INTEGER *frequency)
{
	frequency->QuadPart = N64_TICKS_PER_SECOND;
	return TRUE;
}

unsigned long __stdcall GetTickCount(void)
{
	return (unsigned long)(n64_microseconds() / 1000);
}

void __stdcall GetSystemTime(struct _SYSTEMTIME *time)
{
	unsigned long long seconds = n64_microseconds() / 1000000;

	memset(time, 0, sizeof(*time));
	time->wYear = 2001;
	time->wMonth = 11;
	time->wDay = 15;
	time->wHour = (WORD)(seconds / 3600 % 24);
	time->wMinute = (WORD)(seconds / 60 % 60);
	time->wSecond = (WORD)(seconds % 60);
}

/* ---------- memory */

void *__stdcall XPhysicalAlloc(unsigned long size, unsigned long address, unsigned long alignment, unsigned long protect)
{
	void *result;

	(void)address;
	(void)protect;
	if (alignment < 16)
		alignment = 16;
	result = memalign(alignment, size);
	if (!result)
		platform_log("XPhysicalAlloc(%lu) failed", size);
	return result;
}

unsigned long __stdcall XQueryMemoryProtect(void *address)
{
	(void)address;
	return PAGE_READWRITE;
}

int __stdcall VirtualProtect(void *address, unsigned long size, unsigned long protect, unsigned long *old_protect)
{
	(void)address;
	(void)size;
	(void)protect;
	if (old_protect)
		*old_protect = PAGE_READWRITE;
	return TRUE;
}

void *__stdcall GlobalAlloc(unsigned int flags, unsigned long size)
{
	void *result = malloc(size ? size : 1);

	if (result && (flags & GMEM_ZEROINIT))
		memset(result, 0, size);
	return result;
}

void *__stdcall GlobalReAlloc(void *memory, unsigned long size, unsigned int flags)
{
	(void)flags;
	return realloc(memory, size ? size : 1);
}

void *__stdcall LocalFree(void *memory)
{
	free(memory);
	return NULL;
}

void __stdcall GlobalMemoryStatus(struct _MEMORYSTATUS *status)
{
	memset(status, 0, sizeof(*status));
	status->dwLength = sizeof(*status);
	status->dwTotalPhys = 8 * 1024 * 1024;
	status->dwAvailPhys = n64_heap_free();
}

/* ---------- the XAPI and the debug monitor */

unsigned long __stdcall XGetLaunchInfo(unsigned long *type, struct _LAUNCH_DATA *data)
{
	(void)data;
	*type = 0;
	return ERROR_FILE_NOT_FOUND;
}

HRESULT __stdcall DmWalkLoadedModules(PDM_WALK_MODULES *walk, PDMN_MODLOAD module)
{
	(void)walk;
	(void)module;
	return XBDM_ENDOFLIST;
}

HRESULT __stdcall DmWalkModuleSections(PDM_WALK_MODSECT *walk, const char *module_name, PDMN_SECTIONLOAD section)
{
	(void)walk;
	(void)module_name;
	(void)section;
	return XBDM_ENDOFLIST;
}

HRESULT __stdcall DmCloseModuleSections(PDM_WALK_MODSECT walk)
{
	(void)walk;
	return S_OK;
}
