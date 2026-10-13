/*
BINK_PLAYBACK.C

symbols in this file:
001B53F0 0040:
	_code_001b53f0 (0000)
001B5430 0020:
	_bink_playback_active (0000)
001B5450 0020:
	_bink_playback_ui_rendering_inhibited (0000)
001B5470 0010:
	_bink_playback_in_progress (0000)
001B5480 00f0:
	_code_001b5480 (0000)
001B5570 0030:
	_code_001b5570 (0000)
001B55A0 01f0:
	_code_001b55a0 (0000)
001B5790 00c0:
	_code_001b5790 (0000)
001B5850 0010:
	_code_001b5850 (0000)
001B5860 0390:
	_code_001b5860 (0000)
001B5BF0 0010:
	_code_001b5bf0 (0000)
001B5C00 0050:
	_code_001b5c00 (0000)
001B5C50 0010:
	_code_001b5c50 (0000)
001B5C60 0020:
	_code_001b5c60 (0000)
001B5C80 0010:
	_code_001b5c80 (0000)
001B5C90 0030:
	_bink_playback_initialize (0000)
001B5CC0 0060:
	_code_001b5cc0 (0000)
001B5D20 0070:
	_code_001b5d20 (0000)
001B5D90 0070:
	_bink_playback_stop (0000)
001B5E00 0030:
	_bink_playback_dispose (0000)
001B5E30 00a0:
	_bink_playback_update__internal (0000)
001B5ED0 0060:
	_bink_playback_render (0000)
001B5F30 0430:
	_bink_playback_start (0000)
001B6360 0010:
	_bink_playback_update (0000)
002AA374 001e:
	??_C@_0BO@MFHOKAAA@bink_globals?4memory_pool_base?$AA@ (0000)
002AA394 002d:
	??_C@_0CN@BECKFBGA@bink_globals?4memory_pool_size?$DO?$DNs@ (0000)
002AA3C4 0010:
	??_C@_0BA@FPPHECDF@size_in_bytes?$DO0?$AA@ (0000)
002AA3D8 0047:
	??_C@_0EH@BGOHEHFB@alignment_in_bytes?$DO0?5?$CG?$CG?5?$CIalignme@ (0000)
002AA420 0024:
	??_C@_0CE@CFKHDMNJ@c?3?2halo?2SOURCE?2bink?2bink_playbac@ (0000)
002AA448 0040:
	??_C@_0EA@OJNDLELI@bink_globals?4memory_pool_offset?5@ (0000)
002AA488 002f:
	??_C@_0CP@EKHFIEHP@?$CD?$CD?$CD?5FATAL_ERROR?5bink?5needs?5more?5@ (0000)
002AA4B8 0053:
	??_C@_0FD@JEIGGOCE@?$CD?$CD?$CD?5FATAL_ERROR?5bink?5needs?5more?5@ (0000)
002AA50C 002a:
	??_C@_0CK@MEIMCONA@?$CB?$CCbink?5memory?5allocation?5should?5@ (0000)
002AA538 003a:
	??_C@_0DK@JIJLOKKD@?$CD?$CD?$CD?5FATAL_ERROR?5bink?5just?5confus@ (0000)
002AA574 003a:
	??_C@_0DK@JKNNFEPK@?$CD?$CD?$CD?5FATAL_ERROR?5bink?5just?5confus@ (0000)
002AA5B0 002d:
	??_C@_0CN@HECFNEKJ@SkippedFrames?$DN?$HMt?$CFd?5?$CI?$CFd?$CJ?$HMnSkipped@ (0000)
002AA5E0 00c5:
	??_C@_0MF@OOMIEAIL@FramesTime?$DN?$HMt?$CF?402f?$HMnFrameVideoDe@ (0000)
002AA6A8 0040:
	??_C@_0EA@EKIBIHMI@we?5released?5the?5texture?5cache?5bu@ (0000)
002AA6E8 005a:
	??_C@_0FK@BKFMFKML@?$CD?$CD?$CD?5ERROR?5bink?5failed?5to?5fucking@ (0000)
002AA744 0028:
	??_C@_0CI@EPJBNEGJ@?$CD?$CD?$CD?5ERROR?5failed?5to?5open?5bink?5fi@ (0000)
002AA76C 0012:
	??_C@_0BC@DLLDJJJE@end?5event_manager?$AA@ (0000)
002AA780 0014:
	??_C@_0BE@DCEEPBKH@begin?5event_manager?$AA@ (0000)
002AA794 0015:
	??_C@_0BF@DNOFFDBE@end?5XPhysicalProtect?$AA@ (0000)
002AA7AC 0017:
	??_C@_0BH@JFDFIOME@begin?5XPhysicalProtect?$AA@ (0000)
002AA7C4 000d:
	??_C@_0N@POAPBJK@end?5BinkOpen?$AA@ (0000)
002AA7D4 0022:
	??_C@_0CC@FBMLCGMA@?$CD?$CD?$CD?5ERROR?5no?5DirectSound?5for?5bin@ (0000)
002AA7F8 001c:
	??_C@_0BM@OJHAKIIN@end?5BinkSoundUseDirectSound?$AA@ (0000)
002AA814 001e:
	??_C@_0BO@ONDJOKIK@begin?5BinkSoundUseDirectSound?$AA@ (0000)
002AA834 000f:
	??_C@_0P@MDOKKFIJ@begin?5BinkOpen?$AA@ (0000)
002AA844 0035:
	??_C@_0DF@PFMDENHE@0?5?$DN?$DN?5?$CIbink_globals?4memory_pool_s@ (0000)
002AA87C 002b:
	??_C@_0CL@NNCIPKIH@there?5is?5already?5a?5bink?5movie?5be@ (0000)
0031693C 0005:
	_data_0031693c (0000)
004D2AF0 0164:
	_bss_004d2af0 (0000)
	_debug_bink (0084)
	_bink_globals (0088)
*/

/* ---------- headers */

#include "cseries.h"
#include "bink_playback.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "cache/cache_files.h"
#include "cache/texture_cache.h"
#include "cseries/errors.h"
#include "input/input.h"
#include "interface/attract_mode.h"
#include "interface/event_manager.h"
#include "interface/interface.h"
#include "interface/ui_widget.h"
#include "main/main.h"
#include "main/main_internal.h"
#include "math/integer_math.h"
#include "rasterizer/rasterizer.h"
#include "sound/sound_dsound.h"
#include "text/draw_string.h"
#include "text/text_group.h"
#include <xtl.h>

/* ---------- constants */

enum
{
	MAXIMUM_NUMBER_OF_BINK_POINTER_BLOCKS= 16,
	BINK_POINTER_BLOCK_GUARD_SIZE= 0x3000,

	CPU_PAGE_SIZE= 4096,

	/* the texture cache memory we steal for bink (16Mb when the caller wants us to eat up memory like a goddamn beaver) */
	BINK_MEMORY_POOL_SIZE= 4*1024*1024,
	BINK_MEMORY_POOL_SIZE_GODDAMN_BEAVER= 16*1024*1024,
	BINK_IO_SIZE_GODDAMN_BEAVER= 12*1024*1024,

	BINK_FRAME_BUFFER_ALIGNMENT= 128,

	/* Bink SDK surface types, BinkOpen and BinkCopyToBuffer flags (bink.h) */
	BINKSURFACE32= 3,
	BINKIOSIZE= 0x01000000,
	BINKCOPYALL= 0x80000000
};

/* the hand-built linear X8R8G8B8 texture we decompress into: D3DCOMMON_TYPE_TEXTURE with
   one reference and a single-level 2d linear format word (see rasterizer_xbox.c) */
enum
{
	BINK_TEXTURE_COMMON= D3DCOMMON_TYPE_TEXTURE|1,
	BINK_TEXTURE_FORMAT= (1<<D3DFORMAT_MIPMAP_SHIFT)|(D3DFMT_LIN_X8R8G8B8<<D3DFORMAT_FORMAT_SHIFT)|
		(2<<D3DFORMAT_DIMENSION_SHIFT)|D3DFORMAT_BORDERSOURCE_COLOR|D3DFORMAT_DMACHANNEL_A
};

/* bitmap_group.h does not name the bitmap types, formats or flags; these spellings are
   the ones bitmaps.c and rasterizer_xbox.c use */
enum
{
	_bitmap_type_2d= 0
};

enum
{
	_bitmap_format_x8r8g8b8= 10
};

enum
{
	_bitmap_linear_bit= 4
};

/* rasterizer.h does not name the framebuffer blend functions; these spellings are the
   ones rasterizer_xbox_dynavobgeom.c uses */
enum
{
	_shader_framebuffer_blend_function_alpha_blend= 0,
	_shader_framebuffer_blend_function_multiply,
	_shader_framebuffer_blend_function_double_multiply,
	_shader_framebuffer_blend_function_add,
	_shader_framebuffer_blend_function_reverse_subtract,
	_shader_framebuffer_blend_function_min,
	_shader_framebuffer_blend_function_max,
	_shader_framebuffer_blend_function_alpha_multiply_add,
	NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS
};

/* ---------- macros */

/* ---------- structures */

/* Bink SDK (RAD Game Tools) handle; the vendored bink.h is not in the tree, so the
   SDK's own field names are kept for the members this file touches. */
typedef struct BINK
{
	unsigned long Width;
	unsigned long Height;
	unsigned long Frames;
	unsigned long FrameNum;
	unsigned long LastFrameNum;
} *HBINK;

typedef struct BINKSUMMARY
{
	unsigned long Width;
	unsigned long Height;
	unsigned long TotalTime;
	unsigned long FileFrameRate;
	unsigned long FileFrameRateDiv;
	unsigned long FrameRate;
	unsigned long FrameRateDiv;
	unsigned long TotalOpenTime;
	unsigned long TotalFrames;
	unsigned long TotalPlayedFrames;
	unsigned long SkippedFrames;
	unsigned long SkippedBlits;
	unsigned long SoundSkips;
	unsigned long TotalBlitTime;
	unsigned long TotalReadTime;
	unsigned long TotalVideoDecompTime;
	unsigned long TotalAudioDecompTime;
	unsigned long TotalIdleReadTime;
	unsigned long TotalBackReadTime;
	unsigned long TotalReadSpeed;
	unsigned long SlowestFrameTime;
	unsigned long Slowest2FrameTime;
	unsigned long SlowestFrameNum;
	unsigned long Slowest2FrameNum;
	unsigned long AverageDataRate;
	unsigned long AverageFrameSize;
	unsigned long HighestMemAmount;
	unsigned long TotalIOMemory;
	unsigned long HighestIOUsed;
	unsigned long Highest1SecRate;
	unsigned long Highest1SecFrame;
} BINKSUMMARY;

typedef struct BINKREALTIME
{
	unsigned long FrameNum;
	unsigned long FrameRate;
	unsigned long FrameRateDiv;
	unsigned long Frames;
	unsigned long FramesTime;
	unsigned long FramesVideoDecompTime;
	unsigned long FramesAudioDecompTime;
	unsigned long FramesReadTime;
	unsigned long FramesIdleReadTime;
	unsigned long FramesThreadReadTime;
	unsigned long FramesBlitTime;
	unsigned long ReadBufferSize;
	unsigned long ReadBufferUsed;
	unsigned long FramesDataRate;
} BINKREALTIME;

struct bink_playback_globals
{
	boolean initialized;
	boolean needs_decode;
	byte pad_0002[2];
	unsigned long flags;
	HBINK bink;
	short width;
	short height;
	boolean texture_locked;
	byte pad_0011[3];
	long surface_pitch;
	unsigned long surface_type;
	unsigned long bytes_per_pixel;
	D3DTexture *texture;

	/* frame statistics for debug_bink (BinkGetSummary deltas) */
	long total_skipped_frames;
	long total_skipped_blits;
	long frame_count_at_last_summary;
	long rendered_frame_count;
	long skipped_frames;
	long skipped_blits;
	long frames_since_last_summary;

	struct rasterizer_dynamic_screen_geometry_parameters screen_geometry;
	byte *memory_pool_base;
	long memory_pool_offset;
	long memory_pool_size;
};

typedef void *(__stdcall *rad_memory_allocate_proc)(
	unsigned long size);
typedef void (__stdcall *rad_memory_free_proc)(
	void *memory);
typedef void *(__stdcall *bink_sound_system_open_proc)(
	unsigned long param);

typedef char bink_playback_globals_size_assert[
	sizeof(struct bink_playback_globals) == 0xD8 ? 1 : -1];
typedef char bink_playback_globals_flags_offset_assert[
	offsetof(struct bink_playback_globals, flags) == 4 ? 1 : -1];
typedef char bink_playback_globals_bink_offset_assert[
	offsetof(struct bink_playback_globals, bink) == 8 ? 1 : -1];
typedef char bink_playback_globals_needs_decode_offset_assert[
	offsetof(struct bink_playback_globals, needs_decode) == 1 ? 1 : -1];
typedef char bink_playback_globals_texture_offset_assert[
	offsetof(struct bink_playback_globals, texture) == 0x20 ? 1 : -1];
typedef char bink_playback_globals_rendered_frame_count_offset_assert[
	offsetof(struct bink_playback_globals, rendered_frame_count) == 0x30 ? 1 : -1];
typedef char bink_playback_globals_screen_geometry_offset_assert[
	offsetof(struct bink_playback_globals, screen_geometry) == 0x40 ? 1 : -1];
typedef char bink_playback_globals_memory_pool_base_offset_assert[
	offsetof(struct bink_playback_globals, memory_pool_base) == 0xCC ? 1 : -1];
typedef char bink_summary_size_assert[
	sizeof(BINKSUMMARY) == 0x7C ? 1 : -1];
typedef char bink_summary_skipped_frames_offset_assert[
	offsetof(BINKSUMMARY, SkippedFrames) == 0x28 ? 1 : -1];
typedef char bink_summary_skipped_blits_offset_assert[
	offsetof(BINKSUMMARY, SkippedBlits) == 0x2C ? 1 : -1];
typedef char bink_realtime_size_assert[
	sizeof(BINKREALTIME) == 0x38 ? 1 : -1];
typedef char bink_realtime_frames_offset_assert[
	offsetof(BINKREALTIME, Frames) == 0x0C ? 1 : -1];

/* ---------- prototypes */

/* Bink SDK (RAD Game Tools) entry points used by this file; RADEXPLINK is __stdcall. */
void __stdcall RADSetMemory(
	rad_memory_allocate_proc allocate,
	rad_memory_free_proc release);
void __stdcall BinkClose(
	HBINK bink);
long __stdcall BinkDoFrame(
	HBINK bink);
void __stdcall BinkNextFrame(
	HBINK bink);
long __stdcall BinkWait(
	HBINK bink);
long __stdcall BinkCopyToBuffer(
	HBINK bink,
	void *destination,
	long destination_pitch,
	unsigned long destination_height,
	unsigned long destination_x,
	unsigned long destination_y,
	unsigned long flags);
void * __stdcall BinkOpenDirectSound(
	unsigned long param);
long __stdcall BinkSetSoundSystem(
	bink_sound_system_open_proc open,
	unsigned long param);
void __stdcall BinkSetIOSize(
	unsigned long io_size);
HBINK __stdcall BinkOpen(
	const char *name,
	unsigned long flags);
void __stdcall BinkGetSummary(
	HBINK bink,
	BINKSUMMARY *summary);
void __stdcall BinkGetRealtime(
	HBINK bink,
	BINKREALTIME *realtime,
	unsigned long frame_count);

static void bink_get_memory_available(
	const char *string);
static void *bink_alloc_permanent(
	unsigned long size_in_bytes,
	unsigned long alignment_in_bytes);
static boolean is_all_bink_memory_free(
	void);
static void * __stdcall bink_alloc(
	unsigned long size_in_bytes);
static void __stdcall bink_free(
	void *memory);
void code_001b5850(
	void);
static void bink_draw_frame(
	void);
static boolean bink_query_analog_controller_buttons(
	void);
static void bink_free_texture_cache(
	void);
static void bink_decompress_video_frame(
	void);
static void bink_playback_update__internal(
	void);

/* ---------- globals */

static unsigned long bink_available_memory_kilobytes= NONE;
static boolean bink_saved_frame_rate_throttle= TRUE;

/* January .bss +0x00: the allocation table handed to bink */
static void *bink_pointer_blocks[MAXIMUM_NUMBER_OF_BINK_POINTER_BLOCKS] = { 0 };
/* January .bss +0x40: the bitmap the rasterizer draws the movie frame from */
static struct bitmap_data bink_bitmap = { 0 };
/* January .bss +0x70: the hand-built linear texture over the frame buffer */
static D3DBaseTexture bink_texture = { 0 };
boolean debug_bink = FALSE;
struct bink_playback_globals bink_globals = { 0 };
/* January .bss +0x160: entries used in bink_pointer_blocks */
static long bink_pointer_block_count = 0;

/* ---------- public code */

boolean bink_playback_active(
	void)
{
	return bink_globals.bink && bink_globals.initialized;
}

boolean bink_playback_ui_rendering_inhibited(
	void)
{
	return bink_globals.initialized &&
		TEST_FLAG(bink_globals.flags, _bink_playback_dont_render_ui_bit);
}

boolean bink_playback_in_progress(
	void)
{
	return bink_globals.bink != NULL;
}

void bink_playback_initialize(
	void)
{
	csmemset(&bink_globals, 0, sizeof(bink_globals));
	RADSetMemory(bink_alloc, bink_free);
	bink_globals.initialized = TRUE;

	return;
}

void bink_playback_stop(
	void)
{
	boolean saved_frame_rate_throttle;

	if (!bink_globals.initialized)
		return;

	if (TEST_FLAG(bink_globals.flags, _bink_playback_prevent_events_to_ui_bit))
		event_manager_suppress(FALSE);

	if (bink_globals.bink)
	{
		BinkClose(bink_globals.bink);
		bink_globals.bink = NULL;
	}

	bink_free_texture_cache();

	if (TEST_FLAG(bink_globals.flags, _bink_playback_return_to_main_menu_when_finished_bit))
		main_menu_load();

	saved_frame_rate_throttle = bink_saved_frame_rate_throttle;
	bink_globals.flags = 0;
	global_frame_rate_throttle = saved_frame_rate_throttle;
	attract_mode_reset_timer();

	return;
}

void bink_playback_dispose(
	void)
{
	if (bink_globals.initialized)
	{
		bink_playback_stop();
		csmemset(&bink_globals, 0, sizeof(bink_globals));
	}

	return;
}

void bink_playback_render(
	void)
{
	if (!bink_globals.initialized || !bink_globals.bink)
		return;

	if (global_frame_rate_throttle)
		bink_globals.needs_decode = TRUE;
	else if (!bink_globals.needs_decode)
		goto skip_decode;

	bink_decompress_video_frame();
	bink_globals.needs_decode = FALSE;

skip_decode:
	bink_draw_frame();
	bink_globals.rendered_frame_count++;

	if (!global_frame_rate_throttle)
		bink_playback_update__internal();

	return;
}

void bink_playback_start(
	const char *full_pathname,
	unsigned long flags)
{
	bink_get_memory_available("begin bink_playback_start");

	if (!bink_globals.initialized)
		return;
	if (cache_files_precache_in_progress())
		return;
#ifdef HALO_XBOX
	/* no movie file (attract_mode.c has said so): nothing to play, and no
	memory to steal for it */
	if (!full_pathname)
		return;
	/* the console build's 12 MB texture cache cannot lend the 16 MB pool */
	flags&= ~FLAG(_bink_playback_eat_up_memory_like_a_goddamn_beaver_bit);
#endif

	match_dassert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		414,
		bink_globals.bink==NULL,
		"there is already a bink movie being played");

	bink_globals.memory_pool_size= TEST_FLAG(flags, _bink_playback_eat_up_memory_like_a_goddamn_beaver_bit) ?
		BINK_MEMORY_POOL_SIZE_GODDAMN_BEAVER : BINK_MEMORY_POOL_SIZE;
	bink_globals.memory_pool_offset= 0;
	bink_globals.memory_pool_base= texture_cache_steal_memory(bink_globals.memory_pool_size);
	match_assert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		424,
		0 == (bink_globals.memory_pool_size % CPU_PAGE_SIZE));
	XPhysicalProtect(bink_globals.memory_pool_base, bink_globals.memory_pool_size, PAGE_READONLY);
	if (bink_globals.memory_pool_base)
	{
		bink_get_memory_available("begin BinkOpen");
		{
			struct IDirectSound *dsound= dsound_get();
			boolean sound_initialized= FALSE;

			if (dsound)
			{
				bink_get_memory_available("begin BinkSoundUseDirectSound");
				if (BinkSetSoundSystem(BinkOpenDirectSound, (unsigned long)dsound))
				{
					sound_initialized= TRUE;
				}
				bink_get_memory_available("end BinkSoundUseDirectSound");
			}
			if (!sound_initialized)
			{
				error(_error_silent, "### ERROR no DirectSound for bink");
			}
		}
		if (TEST_FLAG(flags, _bink_playback_eat_up_memory_like_a_goddamn_beaver_bit))
		{
			BinkSetIOSize(BINK_IO_SIZE_GODDAMN_BEAVER);
			bink_globals.bink= BinkOpen(full_pathname, BINKIOSIZE);
		}
		else
		{
			bink_globals.bink= BinkOpen(full_pathname, 0);
		}
		bink_get_memory_available("end BinkOpen");

		if (bink_globals.bink)
		{
			long frame_buffer_size= (bink_globals.bink->Height*bink_globals.bink->Width*sizeof(pixel32)+CPU_PAGE_SIZE-1)&~(CPU_PAGE_SIZE-1);
			byte *frame_buffer= bink_alloc_permanent(frame_buffer_size, BINK_FRAME_BUFFER_ALIGNMENT);
			long byte_index;

			bink_get_memory_available("begin XPhysicalProtect");
			XPhysicalProtect(frame_buffer, frame_buffer_size, PAGE_READWRITE|PAGE_WRITECOMBINE);
			bink_get_memory_available("end XPhysicalProtect");
			for (byte_index= 0; byte_index<frame_buffer_size; byte_index++)
			{
				frame_buffer[byte_index]= (byte)(rand()>>8);
			}

			bink_globals.width= (short)bink_globals.bink->Width;
			bink_globals.height= (short)bink_globals.bink->Height;
			bink_globals.surface_pitch= 0;
			bink_globals.surface_type= BINKSURFACE32;
			bink_globals.bytes_per_pixel= sizeof(pixel32);
			bink_globals.texture_locked= FALSE;
			bink_globals.texture= (D3DTexture *)&bink_texture;

			bink_texture.Common= BINK_TEXTURE_COMMON;
			bink_texture.Data= 0;
			bink_texture.Lock= 0;
			bink_texture.Size= ((bink_globals.width*sizeof(pixel32)/D3DTEXTURE_PITCH_ALIGNMENT-1)<<D3DSIZE_PITCH_SHIFT)|
				((bink_globals.height-1)<<D3DSIZE_HEIGHT_SHIFT)|(bink_globals.width-1);
			bink_texture.Format= BINK_TEXTURE_FORMAT;
			IDirect3DBaseTexture8_Register(&bink_texture, frame_buffer);

			csmemset(&bink_globals.screen_geometry, 0, sizeof(bink_globals.screen_geometry));
			bink_globals.screen_geometry.map_texture_scale[0].i= 1.f;
			bink_globals.screen_geometry.map_texture_scale[0].j= 1.f;
			bink_globals.screen_geometry.map_scale[0].i= 1.f;
			bink_globals.screen_geometry.map_scale[0].j= 1.f;
			bink_globals.screen_geometry.meter_parameters= NULL;
			bink_globals.screen_geometry.point_sampled= FALSE;
			bink_globals.screen_geometry.framebuffer_blend_function= _shader_framebuffer_blend_function_alpha_multiply_add;

			bink_bitmap.signature= BITMAP_GROUP_TAG;
			bink_bitmap.width= bink_globals.width;
			bink_bitmap.height= bink_globals.height;
			bink_bitmap.depth= 1;
			bink_bitmap.type= _bitmap_type_2d;
			bink_bitmap.format= _bitmap_format_x8r8g8b8;
			bink_bitmap.flags= FLAG(_bitmap_linear_bit);
			bink_bitmap.pixels_size= bitmap_format_get_bits_per_pixel(bink_bitmap.format)*bink_bitmap.height*bink_bitmap.width/8;
			bink_bitmap.tag_index= NONE;
			bink_bitmap.cache_block_index= NONE;
			bink_bitmap.hardware_format= bink_globals.texture;
			bink_bitmap.base_address= (void *)NONE;
			bink_globals.screen_geometry.map[0]= &bink_bitmap;

			bink_globals.flags= flags;
			if (TEST_FLAG(flags, _bink_playback_prevent_events_to_ui_bit))
			{
				bink_get_memory_available("begin event_manager");
				event_manager_flush();
				event_manager_suppress(TRUE);
				bink_get_memory_available("end event_manager");
			}

			bink_decompress_video_frame();

			bink_saved_frame_rate_throttle= global_frame_rate_throttle;
			bink_globals.total_skipped_frames= 0;
			bink_globals.total_skipped_blits= 0;
			bink_globals.frame_count_at_last_summary= 0;
			bink_globals.rendered_frame_count= 0;
			bink_globals.skipped_frames= 0;
			bink_globals.skipped_blits= 0;
			bink_globals.frames_since_last_summary= 0;
			global_frame_rate_throttle= FALSE;
		}
		else
		{
			error(_error_silent, "### ERROR failed to open bink file '%s'", full_pathname);
			bink_free_texture_cache();
		}
	}
	else
	{
		error(_error_immediate, "### ERROR bink failed to fucking steal some fucking memory from the fucking texture cache");
		bink_free_texture_cache();
	}

	return;
}

void bink_playback_update(
	void)
{
	if (global_frame_rate_throttle)
		bink_playback_update__internal();

	return;
}

/* ---------- private code */

void code_001b5850(
	void)
{
	return;
}

static void bink_draw_frame(
	void)
{
	struct dynamic_screen_vertex vertices[4];
	rectangle2d screen_bounds;
	short vertex_index;

	if (TEST_FLAG(bink_globals.flags, _bink_playback_full_screen_bit))
	{
		screen_bounds= rasterizer_globals.reserved04.screen_bounds;
	}
	else
	{
		short screen_width= rasterizer_globals.reserved04.screen_bounds.x1-
			rasterizer_globals.reserved04.screen_bounds.x0;
		short screen_height= rasterizer_globals.reserved04.screen_bounds.y1-
			rasterizer_globals.reserved04.screen_bounds.y0;

		screen_bounds.x0= (short)((screen_width-bink_globals.width)/2);
		screen_bounds.x1= (short)((screen_width+bink_globals.width)/2);
		screen_bounds.y0= (short)((screen_height-bink_globals.height)/2);
		screen_bounds.y1= (short)((screen_height+bink_globals.height)/2);
	}

	for (vertex_index= 0; vertex_index<4; vertex_index++)
	{
		real u= ((vertex_index+1)&2) ? (real)bink_globals.width : 0.0f;
		real v= vertex_index>1 ? (real)bink_globals.height : 0.0f;
		real x= ((vertex_index+1)&2) ? (real)screen_bounds.x1 : (real)screen_bounds.x0;
		real y= vertex_index>1 ? (real)screen_bounds.y1 : (real)screen_bounds.y0;

		vertices[vertex_index].position.x= x;
		vertices[vertex_index].position.y= y;
		vertices[vertex_index].texture_coordinates.x= u;
		vertices[vertex_index].texture_coordinates.y= v;
		vertices[vertex_index].color= (pixel32)NONE;
	}

	rasterizer_psuedo_dynamic_screen_quad_draw(&bink_globals.screen_geometry, vertices);

	if (debug_bink)
	{
		char string[4096];
		BINKSUMMARY summary;
		BINKREALTIME realtime;
		short tab_stops[6]= { 250, 0, 0, 0, 0, 0 };
		point2d cursor= { 0, 0 };
		rectangle2d bounds;
		real reciprocal_frame_count;

		BinkGetRealtime(bink_globals.bink, &realtime, 0);
		reciprocal_frame_count= 1.0f/(real)realtime.Frames;
		sprintf(
			string,
			"FramesTime=|t%.02f|nFrameVideoDecompTime=|t%.02f|n"
			"FrameAudioDecompTime=|t%.02f|nFrameReadTime=|t%.02f|n"
			"FrameIdleReadTime=|t%.02f|nFrameThreadReadTime=|t%.02f|n"
			"FramesBlitTime=|t%.02f|n|nFrames=|t%d",
			(real)realtime.FramesTime*reciprocal_frame_count,
			(real)realtime.FramesVideoDecompTime*reciprocal_frame_count,
			(real)realtime.FramesAudioDecompTime*reciprocal_frame_count,
			(real)realtime.FramesReadTime*reciprocal_frame_count,
			(real)realtime.FramesIdleReadTime*reciprocal_frame_count,
			(real)realtime.FramesThreadReadTime*reciprocal_frame_count,
			(real)realtime.FramesBlitTime*reciprocal_frame_count,
			realtime.Frames);

		bounds= rasterizer_globals.reserved04.frame_bounds;
		offset_rectangle2d(&bounds, 0, 32);
		interface_set_bitmap_text_draw_mode(
			_interface_font_terminal,
			_text_style_plain,
			0,
			0,
			_interface_color_table_dialog,
			0);
		draw_string_set_color(global_real_argb_green);
		draw_string_set_tab_stops(tab_stops, 1);
		rasterizer_draw_string(&bounds, NULL, &cursor, -4, string);

		bounds.y0= (short)(cursor.y+31);
		if (bink_globals.rendered_frame_count-bink_globals.frame_count_at_last_summary>=29)
		{
			BinkGetSummary(bink_globals.bink, &summary);
			bink_globals.skipped_frames= summary.SkippedFrames-bink_globals.total_skipped_frames;
			bink_globals.skipped_blits= summary.SkippedBlits-bink_globals.total_skipped_blits;
			bink_globals.frames_since_last_summary=
				bink_globals.rendered_frame_count-bink_globals.frame_count_at_last_summary;
			bink_globals.total_skipped_frames= summary.SkippedFrames;
			bink_globals.total_skipped_blits= summary.SkippedBlits;
			bink_globals.frame_count_at_last_summary= bink_globals.rendered_frame_count;
		}

		sprintf(
			string,
			"SkippedFrames=|t%d (%d)|nSkippedBlits=|t%d|n",
			bink_globals.skipped_frames,
			bink_globals.frames_since_last_summary,
			bink_globals.skipped_blits);
		draw_string_set_color(global_real_argb_red);
		rasterizer_draw_string(&bounds, NULL, &cursor, -4, string);
	}

	return;
}

static void bink_free_texture_cache(
	void)
{
	if (bink_globals.initialized && bink_globals.memory_pool_base)
	{
		match_vassert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			665,
			is_all_bink_memory_free(),
			"we released the texture cache but we still had memory allocated");
		texture_cache_return_memory();
		bink_globals.memory_pool_base= NULL;
		bink_globals.texture= NULL;
		bink_globals.memory_pool_size= 0;
	}

	return;
}

static void bink_get_memory_available(
	const char *string)
{
	MEMORYSTATUS status;

	csmemset(&status, 0, sizeof(status));
	status.dwLength= sizeof(status);
	GlobalMemoryStatus(&status);
	bink_available_memory_kilobytes= status.dwAvailPhys/1024;

	return;
}

static void *bink_alloc_permanent(
	unsigned long size_in_bytes,
	unsigned long alignment_in_bytes)
{
	byte *address= bink_globals.memory_pool_base + (bink_globals.memory_pool_size-size_in_bytes);

	if (alignment_in_bytes && ((unsigned long)address&(alignment_in_bytes-1)))
	{
		match_assert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			691,
			alignment_in_bytes>0 && (alignment_in_bytes&(alignment_in_bytes-1))==0);
		size_in_bytes+= (alignment_in_bytes-(unsigned long)address)&(alignment_in_bytes-1);
		address-= (alignment_in_bytes-(unsigned long)address)&(alignment_in_bytes-1);
	}

	match_assert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		697,
		size_in_bytes>0);
	match_assert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		698,
		bink_globals.memory_pool_size>=size_in_bytes);
	match_assert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		699,
		bink_globals.memory_pool_base);
	bink_globals.memory_pool_size-= size_in_bytes;

	return address;
}

static void * __stdcall bink_alloc(
	unsigned long size_in_bytes)
{
	void *result= NULL;
	boolean can_allocate;

	bink_get_memory_available("begin bink_alloc");

	if (bink_pointer_block_count>0 && !bink_pointer_blocks[0])
	{
		match_vassert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			735,
			is_all_bink_memory_free(),
			"### FATAL_ERROR bink just confused the hell out of me (1)");
		if (is_all_bink_memory_free())
		{
			bink_pointer_block_count= 0;
			bink_globals.memory_pool_offset= 0;
		}
	}

	can_allocate= bink_globals.memory_pool_offset+size_in_bytes<=bink_globals.memory_pool_size &&
		bink_pointer_block_count<MAXIMUM_NUMBER_OF_BINK_POINTER_BLOCKS &&
		bink_globals.memory_pool_base;

	if (!can_allocate)
	{
		match_assert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			759,
			!"bink memory allocation should not fail");
		/* January emits a site-local int3 here; the intrinsic form sinks into the
		   epilogue, so the original text was an inline-assembly breakpoint. */
#ifdef HALO_LINUX
		__builtin_trap();
#else
		__asm { int 3 }
#endif
	}
	else
	{
		result= bink_globals.memory_pool_base+bink_globals.memory_pool_offset;
		bink_globals.memory_pool_offset+= size_in_bytes;
		XPhysicalProtect(result, size_in_bytes, PAGE_READWRITE);
		match_vassert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			786,
			bink_globals.memory_pool_offset<=bink_globals.memory_pool_size,
			csprintf(temporary, "### FATAL_ERROR bink needs more memory (requested %d bytes over the %d-byte limit)",
				bink_globals.memory_pool_offset-bink_globals.memory_pool_size, bink_globals.memory_pool_size));
		match_vassert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			787,
			bink_pointer_block_count<MAXIMUM_NUMBER_OF_BINK_POINTER_BLOCKS,
			"### FATAL_ERROR bink needs more pointer blocks");
		bink_pointer_blocks[bink_pointer_block_count++]= result;

		bink_get_memory_available("end bink_alloc");

		match_assert(
			"c:\\halo\\SOURCE\\bink\\bink_playback.c",
			799,
			bink_globals.memory_pool_offset < bink_globals.memory_pool_size);
		bink_globals.memory_pool_offset+= BINK_POINTER_BLOCK_GUARD_SIZE;
		if (bink_globals.memory_pool_offset>bink_globals.memory_pool_size)
		{
			bink_globals.memory_pool_offset= bink_globals.memory_pool_size;
		}
	}

	return result;
}

static void __stdcall bink_free(
	void *memory)
{
	long block_index;

	bink_get_memory_available("begin bink_free");

	for (block_index= 0; block_index<bink_pointer_block_count; block_index++)
	{
		if (bink_pointer_blocks[block_index]==memory)
		{
			bink_pointer_blocks[block_index]= NULL;
			break;
		}
	}
	match_dassert(
		"c:\\halo\\SOURCE\\bink\\bink_playback.c",
		825,
		block_index<bink_pointer_block_count,
		"### FATAL_ERROR bink just confused the hell out of me (2)");

	bink_get_memory_available("end bink_free");

	return;
}

static boolean is_all_bink_memory_free(
	void)
{
	boolean result= TRUE;
	short block_index;

	for (block_index= 0; block_index<bink_pointer_block_count; block_index++)
	{
		if (bink_pointer_blocks[block_index])
		{
			result= FALSE;
		}
	}

	return result;
}

static void bink_decompress_video_frame(
	void)
{
	D3DLOCKED_RECT locked_rect;

	BinkDoFrame(bink_globals.bink);
	BinkNextFrame(bink_globals.bink);

	rasterizer_globals.current_lock_operation= _rasterizer_lock_bink;
	IDirect3DTexture8_LockRect(bink_globals.texture, 0, &locked_rect, NULL, 0);
	BinkCopyToBuffer(bink_globals.bink, locked_rect.pBits, locked_rect.Pitch, bink_globals.bink->Height,
		0, 0, bink_globals.surface_type|BINKCOPYALL);
	IDirect3DTexture8_UnlockRect(bink_globals.texture, 0);
	rasterizer_globals.current_lock_operation= _rasterizer_lock_none;

	return;
}

static void bink_playback_update__internal(
	void)
{
	if (bink_globals.initialized && bink_globals.bink)
	{
		if (global_frame_rate_throttle)
		{
			bink_globals.needs_decode= BinkWait(bink_globals.bink)==0;
		}
		else
		{
			while (BinkWait(bink_globals.bink))
			{
			}
			bink_globals.needs_decode= TRUE;
		}

		if (TEST_FLAG(bink_globals.flags, _bink_playback_button_click_stops_movie_bit))
		{
			boolean allow_skipping= TEST_FLAG(bink_globals.flags, _bink_playback_dont_allow_skipping_if_filesystem_check_thread_is_active_bit) ?
				!filesystem_check_thread_is_active() : TRUE;

			if (allow_skipping && bink_query_analog_controller_buttons())
			{
				bink_playback_stop();
				return;
			}
		}

		if ((!bink_globals.bink || bink_globals.bink->FrameNum==bink_globals.bink->Frames-1) &&
			!TEST_FLAG(bink_globals.flags, _bink_playback_loop_bit))
		{
			bink_playback_stop();
		}
	}

	return;
}

static boolean bink_query_analog_controller_buttons(
	void)
{
	boolean button_pressed= FALSE;
	short gamepad_index;

	for (gamepad_index= 0; gamepad_index<MAXIMUM_GAMEPADS && !button_pressed; gamepad_index++)
	{
		const struct gamepad_state *gamepad= input_get_gamepad_state(gamepad_index);

		if (gamepad)
		{
			if (gamepad->buttons[_gamepad_binary_button_start]==1 ||
				gamepad->buttons[_gamepad_binary_button_back]==1)
			{
				button_pressed= TRUE;
			}
			else
			{
				short button_index;

				for (button_index= FIRST_GAMEPAD_ANALOG_BUTTON; button_index<NUMBER_OF_GAMEPAD_ANALOG_BUTTONS; button_index++)
				{
					if (gamepad->buttons[button_index]==1)
					{
						button_pressed= TRUE;
						break;
					}
				}
			}
		}
	}

	return button_pressed;
}
