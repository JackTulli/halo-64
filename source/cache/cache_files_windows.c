/*
CACHE_FILES_WINDOWS.C

symbols in this file:
001ABD40 0050:
	_cache_files_dispose (0000)
001ABD90 01a0:
	_cached_map_issue_async_request (0000)
001ABF30 0030:
	_cached_map_block_on_async_request (0000)
001ABF60 0040:
	_cache_request_get (0000)
001ABFA0 0060:
	_cache_request_next_free_index (0000)
001AC000 0080:
	_cache_requests_flush (0000)
001AC080 0010:
	_cache_files_precache_set_priority (0000)
001AC090 0010:
	_cache_files_precache_in_progress (0000)
001AC0A0 0030:
	_cache_files_precache_is_copying_map (0000)
001AC0D0 0030:
	_cache_files_precache_map_queue_end (0000)
001AC100 0040:
	_cached_map_file_get (0000)
001AC140 0040:
	_cached_map_file_invalidate (0000)
001AC180 0040:
	_cached_map_file_get_handle (0000)
001AC1C0 0050:
	_cached_map_file_get_size (0000)
001AC210 0020:
	_cached_map_file_get_path (0000)
001AC230 0010:
	_cache_file_windows_thread_wake (0000)
001AC240 0090:
	_cache_file_read_io_completion_routine@12 (0000)
001AC2D0 0070:
	_cache_file_blocking_io_completion_routine@12 (0000)
001AC340 0020:
	_cache_file_get_map_path (0000)
001AC360 0010:
	_IDirect3DVertexBuffer8_IsBusy@4 (0000)
001AC370 0010:
	_IDirect3DVertexBuffer8_BlockUntilNotBusy@4 (0000)
001AC380 0010:
	_IDirect3DVertexBuffer8_Register@8 (0000)
001AC390 0010:
	_IDirect3DIndexBuffer8_IsBusy@4 (0000)
001AC3A0 0010:
	_IDirect3DIndexBuffer8_BlockUntilNotBusy@4 (0000)
001AC3B0 0020:
	_cache_file_close (0000)
001AC3D0 0150:
	_cache_file_read (0000)
001AC520 0050:
	_cache_file_promote_read (0000)
001AC570 0090:
	_cache_file_read_header_from_dvd (0000)
001AC600 0020:
	_cached_map_issue_async_read (0000)
001AC620 0020:
	_cached_map_issue_async_write (0000)
001AC640 0060:
	_cache_file_block_until_not_busy (0000)
001AC6A0 0060:
	_tags_header_register_vertex_and_index_buffers (0000)
001AC700 00b0:
	_tags_header_deregister_vertex_and_index_buffers (0000)
001AC7B0 0070:
	_structure_bsp_header_register_vertex_buffers (0000)
001AC820 0070:
	_structure_bsp_header_deregister_vertex_buffers (0000)
001AC890 0060:
	_cached_map_files_delete (0000)
001AC8F0 00f0:
	_cache_files_verify_language (0000)
001AC9E0 00b0:
	_cache_files_precache_map_status (0000)
001ACA90 0070:
	_cached_map_file_set_modification_date (0000)
001ACB00 0190:
	_cached_map_file_read_header (0000)
001ACC90 0060:
	_cached_map_files_find_map (0000)
001ACCF0 0190:
	_cached_map_files_find_free_map (0000)
001ACE80 0130:
	_cache_file_windows_thread_proc (0000)
001ACFB0 0120:
	_cache_file_open (0000)
001AD0D0 02f0:
	_cache_files_open_cache_files (0000)
001AD3C0 0030:
	_cache_files_precache_map_loaded (0000)
001AD3F0 0130:
	_cache_files_precache_map_begin (0000)
001AD520 0060:
	_cache_files_precache_map_end (0000)
001AD580 0080:
	_cache_file_windows_thread_create (0000)
001AD600 0060:
	_cache_files_initialize (0000)
002A71F8 002d:
	??_C@_0CN@LGGFIMDL@cache_file_globals?4open_map_file@ (0000)
002A7228 002b:
	??_C@_0CL@CIGLGOJJ@c?3?2halo?2SOURCE?2cache?2cache_files@ (0000)
002A7254 0020:
	??_C@_0CA@OGKEMPOH@Read?1WriteFileEx?$CI?$CJ?5returned?5?$CD?$CFd?$AA@ (0000)
002A7274 0013:
	??_C@_0BD@PLJFCFLH@completion_routine?$AA@ (0000)
002A7288 0010:
	??_C@_0BA@LMKACIBF@completion_flag?$AA@ (0000)
002A7298 000b:
	??_C@_0L@GFMJGGB@overlapped?$AA@ (0000)
002A72A4 001b:
	??_C@_0BL@LMFIEMFC@file?$CB?$DNINVALID_HANDLE_VALUE?$AA@ (0000)
002A72C0 0017:
	??_C@_0BH@MFHBKAIA@async_request_function?$AA@ (0000)
002A72D8 0046:
	??_C@_0EG@KCENDOIE@request_index?$DO?$DN0?5?$CG?$CG?5request_inde@ (0000)
002A7320 002d:
	??_C@_0CN@BGBLHKIB@cache_file_globals?4open_map_file@ (0000)
002A7350 0024:
	??_C@_0CE@EPABAAJM@cache_file_globals?4copy_in_progr@ (0000)
002A7374 003f:
	??_C@_0DP@INCJNNMP@map_file_index?$DO?$DN0?5?$CG?$CG?5map_file_in@ (0000)
002A73B4 0011:
	??_C@_0BB@JJFBACKB@z?3?2cache?$CF03d?4map?$AA@ (0000)
002A73C8 0024:
	??_C@_0CE@EPGEPEPL@finished_request?9?$DOoverlapped?4hEv@ (0000)
002A73EC 002a:
	??_C@_0CK@GKGPAPGO@bytes_transferred?$DN?$DNfinished_requ@ (0000)
002A7418 001a:
	??_C@_0BK@GBNCJMGM@error_code?$DN?$DNERROR_SUCCESS?$AA@ (0000)
002A7434 0013:
	??_C@_0BD@BDBACLGP@overlapped?9?$DOhEvent?$AA@ (0000)
002A7448 0009:
	??_C@_08DCOLLFMA@?$CFs?$CFs?4map?$AA@ (0000)
002A7454 000a:
	??_C@_09GPIOEEJE@offset?$DO?$DN0?$AA@ (0000)
002A7460 001a:
	??_C@_0BK@OLOIOEMB@completion_flag_reference?$AA@ (0000)
002A747C 002c:
	??_C@_0CM@KBEGNHBI@?$CBIDirect3DIndexBuffer8_IsBusy?$CIin@ (0000)
002A74A8 002e:
	??_C@_0CO@CGLHMLAM@?$CBIDirect3DVertexBuffer8_IsBusy?$CIv@ (0000)
002A74D8 0015:
	??_C@_0BF@COEFPJKL@z?3?2last_language?4dat?$AA@ (0000)
002A74F0 002b:
	??_C@_0CL@NILOOFC@couldn?8t?5read?5header?5from?5cache?5@ (0000)
002A751C 003c:
	??_C@_0DM@LINBCMFH@cache_file_globals?4open_map_file@ (0000)
002A7558 0017:
	??_C@_0BH@BPIEKPLM@?$CBbest_request?9?$DOrunning?$AA@ (0000)
002A7570 0015:
	??_C@_0BF@DANPCANK@map_file_index?$CB?$DNNONE?$AA@ (0000)
002A7588 000e:
	??_C@_0O@OLMPCHGI@scenario_name?$AA@ (0000)
002A7598 002d:
	??_C@_0CN@LAKIIMED@couldn?8t?5open?5or?5create?5new?5cach@ (0000)
002A75C8 0026:
	??_C@_0CG@BLEGAMFF@setup?5for?5new?5cache?5file?5failed?5@ (0000)
002A75F0 0014:
	??_C@_0BE@KLDNFNMA@full?5path?5name?5?8?$CFs?8?$AA@ (0000)
002A7604 0022:
	??_C@_0CC@PCGPBKMC@couldn?8t?5find?5map?5?8?$CFs?8?5on?5the?5DV@ (0000)
002A7628 0020:
	??_C@_0CA@GMOAPCML@starting?5precaching?5of?5map?5?8?$CFs?8?$AA@ (0000)
002A7648 001a:
	??_C@_0BK@MIGLNFOL@cache_file_globals?4thread?$AA@ (0000)
002A7664 001f:
	??_C@_0BP@DJCANAAM@cache_file_globals?4sleep_event?$AA@ (0000)
002A7684 001c:
	??_C@_0BM@POLGJDBF@cache_file_globals?4requests?$AA@ (0000)
004CDFF8 307c:
	_cache_file_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "cache/cache_files.h"
#include "cache/cache_files_decompress_windows.h"
#include "cache/texture_cache.h"
#include "interface/ui_widget.h"
#include "tag_files/files.h"
#include "tag_files/tag_files.h"
#include "scenario/scenario_definitions.h"
#include "rasterizer/rasterizer.h"

#include <xtl.h>

/* ---------- constants */

enum
{
	MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS = 512,
	NUMBER_OF_CACHED_MAP_FILES = 6,
	CACHE_FILE_SECTOR_SIZE = 512,
	MAXIMUM_CACHE_FILES = 20,
	CACHE_FILE_THREAD_STACK_SIZE = 0x4000,

	SOLO_CACHE_FILE_MAXIMUM_SIZE = 0x11600000,
	MAIN_MENU_CACHE_FILE_MAXIMUM_SIZE = 0x02300000,
	MULTIPLAYER_CACHE_FILE_MAXIMUM_SIZE = 0x02F00000,
};

/* ---------- macros */

#define CACHE_FILE_BUILD_STRING "01.01.14.2342"

/* ---------- structures */

struct cache_file_tag_instance;

struct cache_file_tag_header
{
	struct cache_file_tag_instance *tag_instances;
	long scenario_tag_index;
	unsigned long checksum;
	long tag_count;
	long vertex_buffer_count;
	D3DVertexBuffer *vertex_buffers;
	long index_buffer_count;
	D3DIndexBuffer *index_buffers;
	unsigned long signature;
};

struct cache_file_structure_bsp_header
{
	void *base_address;
	long vertex_buffer_count;
	D3DVertexBuffer *vertex_buffers;
	long lightmap_vertex_buffer_count;
	D3DVertexBuffer *lightmap_vertex_buffers;
	unsigned long signature;
};

struct cache_file_header
{
	unsigned long header_signature;
	long version;
	long file_length;
	byte reservedC[4];
	long tag_data_offset;
	long tag_data_size;
	byte reserved18[8];
	char name[0x20];
	char build[0x20];
	short scenario_type;
	short pad62;
	unsigned long checksum;
	byte reserved68[0x794];
	unsigned long footer_signature;
};

struct cached_map_file
{
	HANDLE file;
	FILETIME last_modification_date;
	struct cache_file_header header;
};

struct cache_file_request
{
	OVERLAPPED overlapped;
	long size;
	void *buffer;
	boolean blocking;
	boolean pending;
	boolean running;
	byte pad1F;
};

typedef BOOL (WINAPI *cached_map_async_request_proc)(
	HANDLE file,
	void *buffer,
	unsigned long size,
	OVERLAPPED *overlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine);

struct cache_file_runtime_globals
{
	struct cached_map_file cached_map_files[NUMBER_OF_CACHED_MAP_FILES];
	boolean copy_in_progress;
	byte reserved3049;
	short copying_to_map_file_index;
	char copying_to_map_file_name[32];
	short open_map_file_index;
	short blocking_request_index;
	HANDLE sleep_event;
	HANDLE thread;
	struct cache_file_request *requests;
};

typedef char verify_cache_file_header_size[
	sizeof(struct cache_file_header) == 0x800 ? 1 : -1];
typedef char verify_cached_map_file_size[
	sizeof(struct cached_map_file) == 0x80C ? 1 : -1];
typedef char verify_cached_map_file_name_offset[
	offsetof(
		struct cached_map_file,
		header.name) == 0x2C ? 1 : -1];
typedef char verify_cache_file_request_size[
	sizeof(struct cache_file_request) == 0x20 ? 1 : -1];
typedef char verify_cache_file_request_blocking_offset[
	offsetof(
		struct cache_file_request,
		blocking) == 0x1C ? 1 : -1];
typedef char verify_cache_file_request_pending_offset[
	offsetof(
		struct cache_file_request,
		pending) == 0x1D ? 1 : -1];
typedef char verify_cache_file_copy_in_progress_offset[
	offsetof(
		struct cache_file_runtime_globals,
		copy_in_progress) == 0x3048 ? 1 : -1];
typedef char verify_cache_file_copying_map_index_offset[
	offsetof(
		struct cache_file_runtime_globals,
		copying_to_map_file_index) == 0x304A ? 1 : -1];
typedef char verify_cache_file_copying_map_name_offset[
	offsetof(
		struct cache_file_runtime_globals,
		copying_to_map_file_name) == 0x304C ? 1 : -1];
typedef char verify_cache_file_open_map_index_offset[
	offsetof(
		struct cache_file_runtime_globals,
		open_map_file_index) == 0x306C ? 1 : -1];
typedef char verify_cache_file_requests_offset[
	offsetof(
		struct cache_file_runtime_globals,
		requests) == 0x3078 ? 1 : -1];

/* ---------- prototypes */

static void cache_file_get_map_path(
	const char *map_name,
	char *path);
static boolean cache_file_read_header_from_dvd(
	const char *map_name,
	struct cache_file_header *header);
static short cached_map_files_find_free_map(
	long file_length,
	short scenario_type);
static long cached_map_file_get_size(
	short map_file_index);
static HANDLE cached_map_file_get_handle(
	short map_file_index);

static void CALLBACK cache_file_blocking_io_completion_routine(
	unsigned long error_code,
	unsigned long bytes_transferred,
	OVERLAPPED *overlapped);
static void cached_map_issue_async_request(
	cached_map_async_request_proc async_request_function,
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine);
static void cached_map_issue_async_read(
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine);
static void cached_map_issue_async_write(
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine);
static boolean cached_map_block_on_async_request(
	volatile boolean const *completion_flag);
static void cached_map_file_get_path(
	short map_file_index,
	char *path);
static void cached_map_file_set_modification_date(
	short map_file_index);
static void cached_map_file_read_header(
	short map_file_index);
static void cached_map_files_delete(
	short map_file_index);
static void cache_files_verify_language(
	void);
static void cache_files_open_cache_files(
	void);
static void CALLBACK cache_file_read_io_completion_routine(
	unsigned long error_code,
	unsigned long bytes_transferred,
	OVERLAPPED *overlapped);
static void cache_file_windows_thread_proc(
	void);
static void cache_file_windows_thread_create(
	void);
static struct cache_file_request *cache_request_get(
	short request_index);
static short cache_request_next_free_index(
	void);
static void cache_file_windows_thread_wake(
	void);
static void cache_requests_flush(
	void);
static struct cached_map_file *cached_map_file_get(
	short map_file_index);
static void cached_map_file_invalidate(
	short map_file_index);
static short cached_map_files_find_map(
	const char *map_name);

/* ---------- globals */

static struct cache_file_runtime_globals cache_file_globals;

/* ---------- public code */

void tags_header_register_vertex_and_index_buffers(
	struct cache_file_tag_header *header)
{
	short index;

	for (index = 0; index < header->vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->vertex_buffers[index];

		vertex_buffer->Common = D3DCOMMON_TYPE_VERTEXBUFFER | 1;
		IDirect3DVertexBuffer8_Register(vertex_buffer, NULL);
	}

	for (index = 0; index < header->index_buffer_count; index++)
	{
		D3DIndexBuffer *index_buffer = &header->index_buffers[index];

		index_buffer->Common = D3DCOMMON_TYPE_INDEXBUFFER | 1;
	}

	return;
}

void tags_header_deregister_vertex_and_index_buffers(
	struct cache_file_tag_header *header)
{
	short index;

	for (index = 0; index < header->vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->vertex_buffers[index];

		IDirect3DVertexBuffer8_BlockUntilNotBusy(vertex_buffer);
		match_assert(
			"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
			523,
			!IDirect3DVertexBuffer8_IsBusy(vertex_buffer));
	}

	for (index = 0; index < header->index_buffer_count; index++)
	{
		D3DIndexBuffer *index_buffer = &header->index_buffers[index];

		IDirect3DIndexBuffer8_BlockUntilNotBusy(index_buffer);
		match_assert(
			"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
			536,
			!IDirect3DIndexBuffer8_IsBusy(index_buffer));
	}

	return;
}

void structure_bsp_header_register_vertex_buffers(
	struct cache_file_structure_bsp_header *header)
{
	short index;

	for (index = 0; index < header->vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->vertex_buffers[index];

		vertex_buffer->Common = D3DCOMMON_TYPE_VERTEXBUFFER | 1;
		IDirect3DVertexBuffer8_Register(vertex_buffer, NULL);
	}

	for (index = 0; index < header->lightmap_vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->lightmap_vertex_buffers[index];

		vertex_buffer->Common = D3DCOMMON_TYPE_VERTEXBUFFER | 1;
		IDirect3DVertexBuffer8_Register(vertex_buffer, NULL);
	}

	return;
}

void structure_bsp_header_deregister_vertex_buffers(
	struct cache_file_structure_bsp_header *header)
{
	short index;

	rasterizer_globals.current_lock_operation = _rasterizer_lock_bsp_switch;

	for (index = 0; index < header->vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->vertex_buffers[index];

		IDirect3DVertexBuffer8_BlockUntilNotBusy(vertex_buffer);
	}

	for (index = 0; index < header->lightmap_vertex_buffer_count; index++)
	{
		D3DVertexBuffer *vertex_buffer = &header->lightmap_vertex_buffers[index];

		IDirect3DVertexBuffer8_BlockUntilNotBusy(vertex_buffer);
	}

	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	return;
}

void cache_files_dispose(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		207,
		cache_file_globals.open_map_file_index==NONE);
	match_free(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		209,
		cache_file_globals.requests);

	return;
}

void cache_files_precache_set_priority(
	boolean blocking)
{
	cache_copy_set_priority(blocking);

	return;
}

boolean cache_files_precache_in_progress(
	void)
{
	return cache_file_globals.copy_in_progress;
}

boolean cache_files_precache_is_copying_map(
	const char *map_name)
{
	if (cache_file_globals.copying_to_map_file_index != NONE &&
		strcmp(
			cache_file_globals.copying_to_map_file_name,
			tag_name_strip_path(map_name)) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

boolean cache_files_precache_map_loaded(
	const char *map_name)
{
	return cached_map_files_find_map(tag_name_strip_path(map_name)) != NONE;
}

boolean cache_files_precache_map_begin(
	const char *map_name,
	boolean copy_map)
{
	const char *cache_map_name = tag_name_strip_path(map_name);

	if (!cache_files_precache_map_loaded(map_name))
	{
		struct cache_file_header header;
		char path[256];

		if (cache_file_read_header_from_dvd(cache_map_name, &header))
		{
			long buffer_size = cache_copy_buffer_size(copy_map);
			void *buffer = texture_cache_steal_memory(buffer_size);
			short map_file_index = cached_map_files_find_free_map(
				header.file_length,
				header.scenario_type);
			struct cached_map_file *map_file = cached_map_file_get(map_file_index);

			memset(
				&map_file->header,
				0,
				sizeof(struct cache_file_header));
			cache_file_globals.copy_in_progress = TRUE;
			cache_file_globals.copying_to_map_file_index = map_file_index;
			strncpy(
				cache_file_globals.copying_to_map_file_name,
				cache_map_name,
				sizeof(cache_file_globals.copying_to_map_file_name) - 1);
			cache_file_globals.copying_to_map_file_name[
				sizeof(cache_file_globals.copying_to_map_file_name) - 1] = 0;
			cache_file_get_map_path(cache_map_name, path);
			error(_error_silent, "starting precaching of map '%s'", cache_map_name);
			cache_copy_begin(
				buffer,
				buffer_size,
				cached_map_file_get_handle(map_file_index),
				cached_map_file_get_size(map_file_index),
				path);
		}
		else
		{
			error(_error_silent, "couldn't find map '%s' on the DVD", cache_map_name);
			error(_error_silent, "full path name '%s'", map_name);
			if (copy_map)
			{
				display_error_damaged_media();
			}

			return FALSE;
		}
	}

	return TRUE;
}

void cache_files_initialize(
	void)
{
	cache_file_globals.open_map_file_index = NONE;
	cache_file_globals.requests = match_malloc(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		187,
		MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS * sizeof(struct cache_file_request));
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		188,
		cache_file_globals.requests);
	cache_file_windows_thread_create();
	cache_files_verify_language();
	cache_files_open_cache_files();
	cache_copy_initialize();

	return;
}

void cache_files_precache_map_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1031,
		cache_file_globals.copy_in_progress);
	cache_copy_end();
	texture_cache_return_memory();
	cached_map_file_set_modification_date(cache_file_globals.copying_to_map_file_index);
	cached_map_file_read_header(cache_file_globals.copying_to_map_file_index);
	cache_file_globals.copy_in_progress = FALSE;
	cache_file_globals.copying_to_map_file_index = NONE;

	return;
}

void cache_files_precache_map_queue_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1022,
		cache_file_globals.copy_in_progress);
	cache_copy_queue_end();

	return;
}

void cache_file_promote_read(
	short request_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		614,
		request_index>=0 && request_index<MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS);
	cache_file_globals.requests[request_index].blocking = TRUE;

	return;
}

void cache_file_block_until_not_busy(
	void)
{
	boolean busy;
	short request_index;

	do
	{
		SleepEx(0, TRUE);
		busy = FALSE;
		for (request_index = 0;
			request_index < MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS;
			request_index++)
		{
			match_assert(
				"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
				614,
				request_index>=0 && request_index<MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS);
			if (cache_file_globals.requests[request_index].pending)
			{
				busy = TRUE;
			}
		}
	}
	while (busy);

	return;
}

void cache_file_close(
	void)
{
	if (cache_file_globals.open_map_file_index != NONE)
	{
		cache_requests_flush();
		cache_file_globals.open_map_file_index = NONE;
	}

	return;
}

boolean cache_file_open(
	const char *scenario_name,
	struct cache_file_header *header)
{
	short map_file_index = cached_map_files_find_map(scenario_name);

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		220,
		scenario_name);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		221,
		header);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		223,
		cache_file_globals.open_map_file_index==NONE);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		224,
		map_file_index!=NONE);
	memset(
		cache_file_globals.requests,
		0,
		MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS * sizeof(struct cache_file_request));
	cache_file_globals.open_map_file_index = map_file_index;
	memcpy(
		header,
		&cached_map_file_get(map_file_index)->header,
		sizeof(struct cache_file_header));

	return TRUE;
}

short cache_file_read(
	long tag_index,
	long offset,
	long size,
	void *buffer,
	boolean *completion_flag_reference,
	boolean blocking)
{
	short request_index = cache_request_next_free_index();
	struct cache_file_request *request = cache_request_get(request_index);

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		269,
		cache_file_globals.open_map_file_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		272,
		buffer);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		273,
		completion_flag_reference);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		276,
		offset>=0);
	if (size & (CACHE_FILE_SECTOR_SIZE - 1))
	{
		size = (size | (CACHE_FILE_SECTOR_SIZE - 1)) + 1;
	}
	*completion_flag_reference = FALSE;
	memset(
		&request->overlapped,
		0,
		sizeof(OVERLAPPED));
	request->overlapped.hEvent = (HANDLE)completion_flag_reference;
	request->size = size;
	request->overlapped.OffsetHigh = 0;
	request->overlapped.Offset = offset;
	request->buffer = buffer;
	request->pending = TRUE;
	request->blocking = blocking;
	request->running = FALSE;
	cache_file_windows_thread_wake();

	return request_index;
}

short cache_files_precache_map_status(
	real *progress)
{
	short status;

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		984,
		cache_file_globals.copy_in_progress);
	switch (cache_copy_get_status(progress))
	{
		case _cache_copy_bad_file_failure:
		case _cache_copy_read_failure:
			status = _cached_map_file_failed;
			break;

		case _cache_copy_write_failure:
			cached_map_file_invalidate(cache_file_globals.copying_to_map_file_index);
			status = _cached_map_file_failed;
			break;

		case _cache_copy_in_progress:
			status = _cached_map_file_in_progress;
			break;

		case _cache_copy_finished:
			status = _cached_map_file_success;
			break;

		/* status is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			match_vassert("c:\\halo\\SOURCE\\cache\\cache_files_windows.c", 1013, FALSE, NULL);
			break;
	}

	return status;
}

/* ---------- private code */

static void cached_map_files_delete(
	short map_file_index)
{
	char path[256];

	while (++map_file_index < MAXIMUM_CACHE_FILES)
	{
		cached_map_file_get_path(map_file_index, path);
		DeleteFileA(path);
	}

	SetLastError(ERROR_SUCCESS);

	return;
}

static void cache_files_verify_language(
	void)
{
	struct file_reference reference;
	long file_language;
	long language = XGetLanguage();
	long last_language = NONE;

	if (file_reference_create_from_path(&reference, "z:\\last_language.dat", FALSE))
	{
		if (file_open(&reference, FLAG(_permission_read_bit)))
		{
			if (file_read(&reference, sizeof(long), &file_language))
			{
				last_language = file_language;
			}
			file_close(&reference);
		}
	}

	if (last_language != language)
	{
		short map_file_index;

		for (map_file_index = NONE;
			map_file_index < NUMBER_OF_CACHED_MAP_FILES;
			map_file_index++)
		{
			cached_map_files_delete(map_file_index);
		}
	}

	if (file_reference_create_from_path(&reference, "z:\\last_language.dat", FALSE))
	{
		if (file_create(&reference))
		{
			if (file_open(&reference, FLAG(_permission_write_bit)))
			{
				file_write(&reference, sizeof(long), &language);
				file_close(&reference);
			}
		}
	}

	return;
}

static void cache_files_open_cache_files(
	void)
{
	struct cache_file_header blank_header;
	struct cache_file_header dvd_header;
	char path[256];
	OVERLAPPED overlapped;
	boolean deleted_stale_cache_files = FALSE;
	volatile boolean completion_flag;
	boolean valid;
	short map_file_index;

	cached_map_files_delete(NUMBER_OF_CACHED_MAP_FILES);
	for (map_file_index = 0;
		map_file_index < NUMBER_OF_CACHED_MAP_FILES;
		map_file_index++)
	{
		struct cached_map_file *map_file = cached_map_file_get(map_file_index);
		long size;
		HANDLE file;

		valid = FALSE;
		cached_map_file_get_path(map_file_index, path);
		size = cached_map_file_get_size(map_file_index);
		file = CreateFileA(
			path,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
			NULL);
		if (file != INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS &&
				GetFileSize(file, NULL) == (unsigned long)size)
			{
				valid = TRUE;
			}
			else
			{
				if (!deleted_stale_cache_files)
				{
					cached_map_files_delete(map_file_index);
					deleted_stale_cache_files = TRUE;
				}

				completion_flag = FALSE;
				cached_map_issue_async_write(
					file,
					&overlapped,
					&blank_header,
					sizeof(struct cache_file_header),
					0,
					&completion_flag,
					cache_file_blocking_io_completion_routine);
				cached_map_block_on_async_request(&completion_flag);
				if (completion_flag &&
					SetFilePointer(file, size, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER &&
					SetEndOfFile(file))
				{
					valid = TRUE;
				}
				else
				{
					valid = FALSE;
					match_vassert(
						"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
						811,
						FALSE,
						csprintf(temporary, "setup for new cache file failed (#%d)", GetLastError()));
				}
			}
		}
		else
		{
			match_vassert(
				"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
				823,
				FALSE,
				csprintf(temporary, "couldn't open or create new cache file (#%d)", GetLastError()));
		}

		if (!valid && file != INVALID_HANDLE_VALUE)
		{
			CloseHandle(file);
			file = INVALID_HANDLE_VALUE;
		}

		map_file->file = file;
		if (valid)
		{
			char *cache_map_name = cached_map_file_get(map_file_index)->header.name;

			cached_map_file_read_header(map_file_index);
#ifndef HALO_LINUX
			/* (the native builds keep a copied map whatever build made it;
			the checksum below still has to match the original's) */
			if (strcmp(map_file->header.build, CACHE_FILE_BUILD_STRING) != 0)
			{
				valid = FALSE;
			}
#endif
			if (cache_file_read_header_from_dvd(cache_map_name, &dvd_header) &&
				map_file->header.checksum == dvd_header.checksum &&
				valid)
			{
				continue;
			}
		}

		memset(
			&map_file->header,
			0,
			sizeof(struct cache_file_header));
	}

	return;
}

static void CALLBACK cache_file_read_io_completion_routine(
	unsigned long error_code,
	unsigned long bytes_transferred,
	OVERLAPPED *overlapped)
{
	struct cache_file_request *finished_request = (struct cache_file_request *)overlapped;

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1387,
		error_code==ERROR_SUCCESS);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1388,
		bytes_transferred==finished_request->size);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1389,
		finished_request->overlapped.hEvent);
	*(volatile boolean *)finished_request->overlapped.hEvent = TRUE;
	finished_request->pending = FALSE;
	finished_request->running = FALSE;

	return;
}

static void cache_file_windows_thread_proc(
	void)
{
	while (TRUE)
	{
		while (WaitForSingleObjectEx(
			cache_file_globals.sleep_event,
			INFINITE,
			TRUE) == WAIT_IO_COMPLETION)
			;

		while (TRUE)
		{
			struct cache_file_request *best_request = NULL;
			short request_index;
			HANDLE file;

			for (request_index = 0;
				request_index < MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS;
				request_index++)
			{
				struct cache_file_request *request = cache_request_get(request_index);

				if (request->pending &&
					!request->running &&
					(!best_request ||
						(best_request->blocking > request->blocking &&
						best_request->overlapped.Offset > request->overlapped.Offset)))
				{
					best_request = request;
				}
			}

			if (!best_request)
			{
				break;
			}

			file = cached_map_file_get_handle(cache_file_globals.open_map_file_index);
			match_assert(
				"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
				1327,
				!best_request->running);
			best_request->running = TRUE;
			cached_map_issue_async_read(
				file,
				&best_request->overlapped,
				best_request->buffer,
				best_request->size,
				best_request->overlapped.Offset,
				(volatile boolean *)best_request->overlapped.hEvent,
				cache_file_read_io_completion_routine);
		}
	}

	return;
}

static void cache_file_windows_thread_create(
	void)
{
	cache_file_globals.sleep_event = CreateEventA(NULL, FALSE, FALSE, NULL);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1253,
		cache_file_globals.sleep_event);
	cache_file_globals.thread = CreateThread(
		NULL,
		CACHE_FILE_THREAD_STACK_SIZE,
		(LPTHREAD_START_ROUTINE)cache_file_windows_thread_proc,
		NULL,
		0,
		NULL);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1257,
		cache_file_globals.thread);

	return;
}

static void cache_file_get_map_path(
	const char *map_name,
	char *path)
{
	sprintf(path, "%s%s.map", cache_files_map_directory(), map_name);

	return;
}

static boolean cache_file_read_header_from_dvd(
	const char *map_name,
	struct cache_file_header *header)
{
	boolean result = FALSE;
	char path[256];
	HANDLE file;

	cache_file_get_map_path(map_name, path);
	file = CreateFileA(
		path,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		unsigned long bytes_read;

		if (ReadFile(
			file,
			header,
			sizeof(struct cache_file_header),
			&bytes_read,
			NULL) &&
			bytes_read == sizeof(struct cache_file_header) &&
			cache_file_header_verify(header, path, TRUE))
		{
			result = TRUE;
		}
		CloseHandle(file);
	}

	return result;
}

static short cached_map_files_find_free_map(
	long file_length,
	short scenario_type)
{
	short best_map_file_index = NONE;
	struct cached_map_file *best_map_file;
	short first_map_file_index;
	short last_map_file_index;
	short map_file_index;

	switch (scenario_type)
	{
		case _scenario_type_solo:
			first_map_file_index = 0;
			last_map_file_index = 1;
			break;

		case _scenario_type_multiplayer:
			first_map_file_index = 3;
			last_map_file_index = 5;
			break;

		case _scenario_type_main_menu:
			first_map_file_index = 2;
			last_map_file_index = 2;
			break;

		/* first_map_file_index and last_map_file_index are left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			match_vassert("c:\\halo\\SOURCE\\cache\\cache_files_windows.c", 1172, FALSE, NULL);
			break;
	}

	for (map_file_index = first_map_file_index;
		map_file_index <= last_map_file_index;
		map_file_index++)
	{
		if (cache_file_globals.open_map_file_index != map_file_index)
		{
			struct cached_map_file *map_file = cached_map_file_get(map_file_index);

			if (cached_map_file_get_size(map_file_index) > file_length)
			{
				if (best_map_file_index == NONE ||
					cached_map_file_get_size(map_file_index) < cached_map_file_get_size(best_map_file_index) ||
					CompareFileTime(
						&best_map_file->last_modification_date,
						&map_file->last_modification_date) > 0)
				{
					best_map_file_index = map_file_index;
					best_map_file = map_file;
				}
			}
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1200,
		cache_file_globals.open_map_file_index!=best_map_file_index);

	return best_map_file_index;
}

static long cached_map_file_get_size(
	short map_file_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1232,
		map_file_index>=0 && map_file_index<NUMBER_OF_CACHED_MAP_FILES);
	if (map_file_index <= 1)
	{
		return SOLO_CACHE_FILE_MAXIMUM_SIZE;
	}

	return (map_file_index <= 2)
		? MAIN_MENU_CACHE_FILE_MAXIMUM_SIZE
		: MULTIPLAYER_CACHE_FILE_MAXIMUM_SIZE;
}

static HANDLE cached_map_file_get_handle(
	short map_file_index)
{
	return cached_map_file_get(map_file_index)->file;
}

static void CALLBACK cache_file_blocking_io_completion_routine(
	unsigned long error_code,
	unsigned long bytes_transferred,
	OVERLAPPED *overlapped)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1405,
		error_code==ERROR_SUCCESS);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1406,
		overlapped->hEvent);
	*(volatile boolean *)overlapped->hEvent = TRUE;

	return;
}

static void cached_map_issue_async_request(
	cached_map_async_request_proc async_request_function,
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		390,
		async_request_function);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		391,
		file!=INVALID_HANDLE_VALUE);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		392,
		overlapped);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		393,
		buffer);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		394,
		completion_flag);
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		395,
		completion_routine);
	memset(
		overlapped,
		0,
		sizeof(OVERLAPPED));
	overlapped->Offset = offset;
	overlapped->OffsetHigh = 0;
	overlapped->hEvent = (HANDLE)completion_flag;
	while (TRUE)
	{
		unsigned long error_code;

		SleepEx(0, TRUE);
		SetLastError(ERROR_SUCCESS);
		if (async_request_function(
			file,
			buffer,
			size,
			overlapped,
			completion_routine))
		{
			break;
		}

		error_code = GetLastError();
		match_vassert(
			"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
			422,
			error_code==ERROR_NOT_ENOUGH_MEMORY || error_code==ERROR_NO_SYSTEM_RESOURCES || error_code==ERROR_INVALID_USER_BUFFER,
			csprintf(temporary, "Read/WriteFileEx() returned #%d", GetLastError()));
	}

	return;
}

static void cached_map_issue_async_read(
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine)
{
	cached_map_issue_async_request(
		ReadFileEx,
		file,
		overlapped,
		buffer,
		size,
		offset,
		completion_flag,
		completion_routine);

	return;
}

static void cached_map_issue_async_write(
	HANDLE file,
	OVERLAPPED *overlapped,
	void *buffer,
	long size,
	long offset,
	volatile boolean *completion_flag,
	LPOVERLAPPED_COMPLETION_ROUTINE completion_routine)
{
	cached_map_issue_async_request(
		(cached_map_async_request_proc)WriteFileEx,
		file,
		overlapped,
		buffer,
		size,
		offset,
		completion_flag,
		completion_routine);

	return;
}

static boolean cached_map_block_on_async_request(
	volatile boolean const *completion_flag)
{
	boolean completed;

	while (!*completion_flag)
	{
		if (SleepEx(5000, TRUE) != WAIT_IO_COMPLETION)
		{
			break;
		}
	}
	completed = *completion_flag;

	return completed;
}

static void cached_map_file_get_path(
	short map_file_index,
	char *path)
{
	sprintf(path, "z:\\cache%03d.map", map_file_index);

	return;
}

static void cached_map_file_set_modification_date(
	short map_file_index)
{
	struct cached_map_file *map_file = cached_map_file_get(map_file_index);
	SYSTEMTIME system_time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &map_file->last_modification_date);
	SetFileTime(map_file->file, &map_file->last_modification_date, NULL, NULL);

	return;
}

static void cached_map_file_read_header(
	short map_file_index)
{
	struct cached_map_file *map_file = cached_map_file_get(map_file_index);
	char path[256];
	OVERLAPPED overlapped;
	volatile boolean completion_flag;

	cached_map_file_get_path(map_file_index, path);
	GetFileTime(map_file->file, &map_file->last_modification_date, NULL, NULL);
	completion_flag = FALSE;
	cached_map_issue_async_read(
		map_file->file,
		&overlapped,
		&map_file->header,
		sizeof(struct cache_file_header),
		0,
		&completion_flag,
		cache_file_blocking_io_completion_routine);
	cached_map_block_on_async_request(&completion_flag);
	if (completion_flag)
	{
		if (!cache_file_header_verify(&map_file->header, path, FALSE))
		{
			memset(&map_file->header, 0, sizeof(struct cache_file_header));
			memset(&map_file->last_modification_date, 0, sizeof(FILETIME));
		}
	}
	else
	{
		match_vassert(
			"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
			1121,
			FALSE,
			csprintf(temporary, "couldn't read header from cache file (#%d)", GetLastError()));
		cached_map_file_invalidate(map_file_index);
	}

	return;
}

static struct cache_file_request *cache_request_get(
	short request_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		614,
		request_index>=0 && request_index<MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS);

	return cache_file_globals.requests + request_index;
}

static short cache_request_next_free_index(
	void)
{
	boolean out_of_requests = FALSE;

	while (TRUE)
	{
		short request_index;

		for (request_index = 0;
			request_index < MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS;
			request_index++)
		{
			if (!cache_request_get(request_index)->pending)
			{
				return request_index;
			}
		}

		/* every request slot is busy: remember that we blocked and scan again
		until the elevator thread retires one of them */
		if (!out_of_requests)
		{
			out_of_requests = TRUE;
		}
	}

	return NONE;
}

static void cache_file_windows_thread_wake(
	void)
{
	SetEvent(cache_file_globals.sleep_event);

	return;
}

static void cache_requests_flush(
	void)
{
	short request_index;

	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		651,
		cache_file_globals.open_map_file_index!=NONE);
	for (request_index = 0;
		request_index < MAXIMUM_SIMULTANEOUS_CACHE_REQUESTS;
		request_index++)
	{
		volatile struct cache_file_request *request = cache_request_get(request_index);

		while (request->pending)
			;
	}

	return;
}

static struct cached_map_file *cached_map_file_get(
	short map_file_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\cache\\cache_files_windows.c",
		1208,
		map_file_index>=0 && map_file_index<NUMBER_OF_CACHED_MAP_FILES);

	return cache_file_globals.cached_map_files + map_file_index;
}

static void cached_map_file_invalidate(
	short map_file_index)
{
	cached_map_file_get(map_file_index)->file = INVALID_HANDLE_VALUE;

	return;
}

static short cached_map_files_find_map(
	const char *map_name)
{
	short map_file_index;

	for (map_file_index = 0;
		map_file_index < NUMBER_OF_CACHED_MAP_FILES;
		map_file_index++)
	{
		if (_stricmp(
			map_name,
			cached_map_file_get(map_file_index)->header.name) == 0)
		{
			return map_file_index;
		}
	}

	return NONE;
}
