/*
THREAD_WIN32.C

symbols in this file:
0006FC30 0030:
	_get_thread_from_pool (0000)
0006FC60 0040:
	_get_mutex_from_pool (0000)
0006FCA0 00f0:
	_create_thread (0000)
0006FD90 0050:
	_thread_has_exited (0000)
0006FDE0 0070:
	_dispose_thread (0000)
0006FE50 0090:
	_create_mutex (0000)
0006FEE0 0060:
	_take_mutex (0000)
0006FF40 0040:
	_release_mutex (0000)
0006FF80 0070:
	_dispose_mutex (0000)
00255AEC 0011:
	??_C@_0BB@GLLFGHGN@thread_reference?$AA@ (0000)
00255B00 0009:
	??_C@_08HFFPPCHF@function?$AA@ (0000)
00255B0C 0030:
	??_C@_0DA@KOBAFCDH@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
00255B3C 0019:
	??_C@_0BJ@MGKMABGD@thread_reference?9?$DOin_use?$AA@ (0000)
00255B58 000a:
	??_C@_09PJEOHGKG@mutex_?$CFld?$AA@ (0000)
00255B64 0010:
	??_C@_0BA@IKBDPBME@mutex_reference?$AA@ (0000)
00255B74 0018:
	??_C@_0BI@CFEHKCFO@mutex_reference?9?$DOin_use?$AA@ (0000)
0031C728 0608:
	_thread_globals (0000)
*/

/* ---------- headers */

#include "cseries.h"
#ifndef _X86_
#define _X86_
#endif
#include <excpt.h>
#include <windef.h>
#include <winbase.h>

#include "bungie_net/common/thread.h"

/* ---------- constants */

enum
{
	MAXIMUM_THREAD_REFERENCES = 32,
	MAXIMUM_MUTEX_REFERENCES = 32,
	MUTEX_NAME_LENGTH = 32,
	THREAD_STILL_ACTIVE = 0x103
};

/* ---------- macros */

/* ---------- structures */

struct thread_reference
{
	HANDLE handle;
	boolean in_use;
	byte __pad5[3];
};

struct mutex_reference
{
	HANDLE handle;
	char name[MUTEX_NAME_LENGTH];
	boolean in_use;
	byte __pad25[3];
};

struct thread_globals
{
	long mutex_index;
	byte __pad4[4];
	struct thread_reference thread_references[MAXIMUM_THREAD_REFERENCES];
	struct mutex_reference mutex_references[MAXIMUM_MUTEX_REFERENCES];
};

/* ---------- prototypes */

static struct thread_reference *get_thread_from_pool(
	void);
static struct mutex_reference *get_mutex_from_pool(
	void);

/* ---------- globals */

static struct thread_globals thread_globals = {0};

/* ---------- public code */

boolean create_thread(
	word flags,
	unsigned long (__stdcall *function)(void *),
	void *function_input,
	struct thread_reference **thread_reference)
{
	boolean success = FALSE;
	struct thread_reference *reference;
	unsigned long unused_thread_id;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0x6B, function);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0x6C, thread_reference);

	reference = get_thread_from_pool();
	if (reference && (reference->handle = CreateThread(
		NULL,
		0x4000,
		function,
		function_input,
		CREATE_SUSPENDED,
		&unused_thread_id))!=NULL)
	{
		long priority = THREAD_PRIORITY_NORMAL;

		if (TEST_FLAG(flags, 1))
		{
			priority = THREAD_PRIORITY_BELOW_NORMAL;
		}
		else if (TEST_FLAG(flags, 2))
		{
			priority = THREAD_PRIORITY_ABOVE_NORMAL;
		}

		if (SetThreadPriority(reference->handle, priority) && ResumeThread(reference->handle)!=-1)
		{
			success = TRUE;
		}
		else
		{
			CloseHandle(reference->handle);
			reference = NULL;
		}
	}

	*thread_reference = reference;
	return success;
}

boolean thread_has_exited(
	struct thread_reference *thread_reference)
{
	boolean result = FALSE;
	unsigned long exit_code;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0x98, thread_reference);

	if (GetExitCodeThread(thread_reference->handle, &exit_code) && exit_code!=THREAD_STILL_ACTIVE)
	{
		result = TRUE;
	}

	return result;
}

void dispose_thread(
	struct thread_reference *thread_reference)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xA8, thread_reference);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xA9, thread_reference->in_use);

	CloseHandle(thread_reference->handle);
	thread_reference->handle = NULL;
	thread_reference->in_use = FALSE;

	return;
}

boolean create_mutex(
	struct mutex_reference **mutex_reference)
{
	struct mutex_reference *reference;
	boolean success;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xB8, mutex_reference);

	success = FALSE;
	reference = get_mutex_from_pool();
	if (reference)
	{
		_snprintf(
			reference->name,
			NUMBEROF(reference->name),
			"mutex_%ld",
			thread_globals.mutex_index++);
		reference->handle = CreateMutexA(NULL, FALSE, reference->name);
		if (reference->handle)
			success = TRUE;
		else
			reference = NULL;
	}

	*mutex_reference = reference;

	return success;
}

boolean take_mutex(
	struct mutex_reference *mutex_reference,
	unsigned long timeout_ms)
{
	boolean result = FALSE;
	unsigned long wait_result;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xD3, mutex_reference);

	wait_result = WaitForSingleObject(mutex_reference->handle, timeout_ms);
	if (wait_result==WAIT_OBJECT_0 || wait_result==WAIT_ABANDONED)
	{
		result = TRUE;
	}

	return result;
}

void release_mutex(
	struct mutex_reference *mutex_reference)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xE6, mutex_reference);

	ReleaseMutex(mutex_reference->handle);

	return;
}

void dispose_mutex(
	struct mutex_reference *mutex_reference)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xF0, mutex_reference);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\thread_win32.c", 0xF1, mutex_reference->in_use);

	CloseHandle(mutex_reference->handle);
	mutex_reference->name[0] = 0;
	mutex_reference->handle = NULL;
	mutex_reference->in_use = FALSE;

	return;
}

/* ---------- private code */

static struct thread_reference *get_thread_from_pool(
	void)
{
	struct thread_reference *thread_reference = NULL;
	long thread_index;

	for (thread_index = 0; thread_index<MAXIMUM_THREAD_REFERENCES; thread_index++)
	{
		if (!thread_globals.thread_references[thread_index].in_use)
		{
			thread_reference = &thread_globals.thread_references[thread_index];
			thread_reference->handle = NULL;
			thread_reference->in_use = TRUE;
			break;
		}
	}

	return thread_reference;
}

static struct mutex_reference *get_mutex_from_pool(
	void)
{
	struct mutex_reference *mutex_reference = NULL;
	boolean *in_use;
	long mutex_index;

	mutex_index = 0;
	in_use = &thread_globals.mutex_references[0].in_use;
	do
	{
		if (!*in_use)
		{
			mutex_reference = &thread_globals.mutex_references[mutex_index];
			mutex_reference->name[0] = 0;
			mutex_reference->handle = NULL;
			mutex_reference->in_use = TRUE;
			break;
		}

		in_use += sizeof(struct mutex_reference);
		mutex_index++;
	}
	while ((long)in_use<(long)&thread_globals.mutex_references[MAXIMUM_MUTEX_REFERENCES].in_use);

	return mutex_reference;
}
