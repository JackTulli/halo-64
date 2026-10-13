/*
RASTERIZER_XBOX_DECALS.C

symbols in this file:
0014A5F0 01b0:
	_D3DDevice_SetRenderState (0000)
0014A7A0 0050:
	_D3DDevice_SetTextureStageState (0000)
0014A7F0 0120:
	_rasterizer_decal_vertices_purge_proc (0000)
0014A910 0090:
	_rasterizer_decal_vertices_locked_proc (0000)
0014A9A0 0040:
	__rasterizer_decals_update_function_pointers (0000)
0014A9E0 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
0014AC00 0010:
	__rasterizer_decals_initialize_for_new_map (0000)
0014AC10 0040:
	__rasterizer_decals_dispose_from_old_map (0000)
0014AC50 0040:
	__rasterizer_decals_flush (0000)
0014AC90 0010:
	_rasterizer_decal_vertices_begin_update (0000)
0014ACA0 0010:
	_rasterizer_decal_vertices_end_update (0000)
0014ACB0 0070:
	__rasterizer_decal_vertices_new (0000)
0014AD20 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
0014AD80 0030:
	_IDirect3DDevice8_DrawPrimitive@16 (0000)
0014ADB0 0070:
	__rasterizer_decal_vertices_delete (0000)
0014AE20 0010:
	_IDirect3DDevice8_SetStreamSource@16 (0000)
0014AE30 0070:
	__rasterizer_decals_end (0000)
0014AEA0 0020:
	_IDirect3DDevice8_SetVertexData4ub@24 (0000)
0014AEC0 0010:
	_D3DVertexBuffer_Unlock@4 (0000)
0014AED0 0010:
	_IDirect3DVertexBuffer8_Release@4 (0000)
0014AEE0 0010:
	_IDirect3DVertexBuffer8_Register@8 (0000)
0014AEF0 0020:
	_IDirect3DVertexBuffer8_Lock@20 (0000)
0014AF10 0010:
	_IDirect3DVertexBuffer8_Unlock@4 (0000)
0014AF20 0110:
	__rasterizer_decals_initialize (0000)
0014B030 00b0:
	__rasterizer_decals_dispose (0000)
0014B0E0 00d0:
	__rasterizer_decal_vertices_lock (0000)
0014B1B0 0010:
	__rasterizer_decal_vertices_unlock (0000)
0014B1C0 02d0:
	__rasterizer_decals_begin (0000)
0014B490 0350:
	__rasterizer_decals_draw (0000)
0028DD18 0058:
	_D3DPRIMITIVETOVERTEXCOUNT (0000)
0028DD70 0012:
	??_C@_0BC@JENGFNKJ@decal_index?$CB?$DNNONE?$AA@ (0000)
0028DD88 005d:
	??_C@_0FN@KMDPMKOA@?$CD?$CD?$CD?5ERROR?5decals?3?5deleting?5perma@ (0000)
0028DDE8 005a:
	??_C@_0FK@BJHCLOEJ@?$CD?$CD?$CD?5ERROR?5decals?3?5deleting?5locke@ (0000)
0028DE44 000f:
	??_C@_0P@MKLHCCBN@decal_index?$CB?$DN0?$AA@ (0000)
0028DE54 000c:
	??_C@_0M@LJKLFPHM@decal_index?$AA@ (0000)
0028DE60 0029:
	??_C@_0CJ@FLPDNPAK@lruv_has_locked_proc?$CIlocal_verte@ (0000)
0028DE8C 0038:
	??_C@_0DI@DFPBIFJA@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0028DEC4 0013:
	??_C@_0BD@GMDLIMJM@local_vertex_cache?$AA@ (0000)
0028DED8 002a:
	??_C@_0CK@DKKOCKIP@cache_size?$CFsizeof?$CIstruct?5decal_v@ (0000)
0028DF04 0027:
	??_C@_0CH@FLDEKCGM@cache_size?$DOsizeof?$CIstruct?5decal_v@ (0000)
0028DF2C 0012:
	??_C@_0BC@NONEJICN@cache_index?$CB?$DNNONE?$AA@ (0000)
0028DF40 0013:
	??_C@_0BD@NBMEPDOH@decal?5vertex?5cache?$AA@ (0000)
0028DF54 001e:
	??_C@_0BO@OHOOIFNI@local_d3d_vertex_buffer?9?$DOData?$AA@ (0000)
0028DF74 000f:
	??_C@_0P@PKAEFIHO@decal?5vertices?$AA@ (0000)
0028DF84 0018:
	??_C@_0BI@PCGDDGMG@local_d3d_vertex_buffer?$AA@ (0000)
0028DF9C 0032:
	??_C@_0DC@BMDAFKAI@vertex_data_offset?$CFsizeof?$CIstruct@ (0000)
0028DFD0 0022:
	??_C@_0CC@GHCFEIDO@intensity?$DM?$DNPIXEL32_COMPONENT_MAS@ (0000)
0028DFF4 0031:
	??_C@_0DB@GLODBMEM@?$CD?$CD?$CD?5ERROR?5unsupported?5framebuffe@ (0000)
0030CEF4 0004:
	_last_decal_index_queried_by_lruv_cache (0000)
0045E8E8 0002:
	_local_layer (0000)
0045E8EC 0004:
	_rasterizer_decal_cached_bitmap_group_index (0000)
0045E8F0 0002:
	_rasterizer_decal_cached_bitmap_index (0000)
0045E8F4 0002:
	_local_framebuffer_blend_function (0000)
0045E8F8 0004:
	_local_d3d_vertex_buffer (0000)
0045E8FC 0004:
	_local_vertex_cache (0000)
0045E900 0001:
	_locked_decal_reported (0000)
0045E901 0001:
	_permanent_decal_reported (0000)
0045E902 0001:
	_local_filthy_decal_fog_hack_enabled (0000)
*/

/* ---------- headers */

#include "rasterizer/rasterizer_frame_statistics.h"
#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "effects/decal_definitions.h"
#include "effects/decals.h"
#include "math/integer_math.h"
#include "memory/data.h"
#include "memory/lruv_cache.h"
#include "rasterizer/rasterizer.h"
/* The XDK's stock D3DINLINE (static __forceinline) definitions supply both the
 * inline expansions used below and the out-of-line wrapper bodies January
 * retains in this object. Do not redefine D3DINLINE, take a wrapper's address
 * or hand-write a wrapper body: any of those changes the emitted ABI. */
#include <xtl.h>
#include "rasterizer/xbox/rasterizer_xbox.h"
#include "saved games/game_state.h"

/* ---------- constants */

enum
{
	NUMBER_OF_DECAL_LAYERS = 5
};

enum
{
	_decal_layer_primary = 0,
	_decal_layer_secondary,
	_decal_layer_light,
	_decal_layer_alpha_tested,
	_decal_layer_water
};

enum
{
	_decal_locked_bit,
	_decal_permanent_bit
};

enum
{
	_shader_framebuffer_blend_function_alpha_blend = 0,
	_shader_framebuffer_blend_function_multiply,
	_shader_framebuffer_blend_function_double_multiply,
	_shader_framebuffer_blend_function_add,
	_shader_framebuffer_blend_function_reverse_subtract,
	_shader_framebuffer_blend_function_min,
	_shader_framebuffer_blend_function_max,
	_shader_framebuffer_blend_function_alpha_multiply_add,
	NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS
};

enum
{
	_rasterizer_drawing_mode_normal = 0
};

enum
{
	_rasterizer_statistics_mode_geometry = 2
};

enum
{
	PIXEL32_COMPONENT_MASK = 0xff
};

enum
{
	RASTERIZER_STENCIL_MODE_REJECT = 2,
	RASTERIZER_STENCIL_MODE_WRITE_ALPHA_TESTED_DECAL = 4
};

enum
{
	MAXIMUM_DECALS_PER_MAP = 2048,
	DECAL_VERTEX_CACHE_PAGE_SIZE_BITS = 6,
	DECAL_VERTEX_CACHE_PAGE_COUNT = 2560,
	DECAL_VERTEX_CACHE_SIZE = DECAL_VERTEX_CACHE_PAGE_COUNT << DECAL_VERTEX_CACHE_PAGE_SIZE_BITS
};

/* ---------- macros */

#define DECAL_GET(index) ((struct decal_datum *)datum_get(global_decal_data, (index)))

/* ---------- structures */

struct rasterizer_decals_debug_options
{
	byte reserved0000[2];
	short statistics_mode;
	short drawing_mode;
	byte reserved0006[0xf];
	boolean draw_environment_decals;
	byte reserved0016[0x3e];
	long decal_zbias;
	byte reserved0058[9];
	boolean filthy_decal_fog_hack_enabled;
};

struct pixel_shader_definition
{
	unsigned long alpha_inputs[8];
	unsigned long final_combiner_inputs_abcd;
	unsigned long final_combiner_inputs_efg;
	unsigned long constant_0[8];
	unsigned long constant_1[8];
	unsigned long alpha_outputs[8];
	unsigned long rgb_inputs[8];
	unsigned long compare_mode;
	unsigned long final_combiner_constant_0;
	unsigned long final_combiner_constant_1;
	unsigned long rgb_outputs[8];
	unsigned long combiner_count;
	unsigned long texture_modes;
	unsigned long dot_mapping;
	unsigned long input_texture;
	unsigned long c0_mapping;
	unsigned long c1_mapping;
	unsigned long final_combiner_constants;
};

struct decal_vertex
{
	real_point3d position;
	unsigned long texcoord;
};

struct decal_datum
{
	short identifier;
	unsigned short flags;
	short cluster_index;
	short layer;
	real_point3d position;
	long creation_time;
	byte sequence_index;
	byte unused_was_frames_remaining;
	byte sprite_index;
	byte bitmap_index;
	real lifetime;
	real decay_time;
	pixel32 color;
	byte intensity;
	byte unused;
	short quad_count;
	long definition_index;
	long previous_decal_index;
	long next_decal_index;
};

typedef char verify_decal_datum_size[
	sizeof(struct decal_datum) == 0x38 ? 1 : -1];

/* Only the fields the rasterizer touches are recovered; the complete decal
   tag definition is not yet authenticated. */
struct decal_shader_definition
{
	byte reserved0000[4];
	short framebuffer_blend_function;
	byte reserved0006[0x16];
	struct tag_reference map;
};

struct decal_definition
{
	byte reserved0000[0xbc];
	struct decal_shader_definition shader;
};

typedef char verify_decal_definition_framebuffer_blend_function_offset[
	offsetof(
		struct decal_definition,
		shader.framebuffer_blend_function) == 0xc0 ? 1 : -1];
typedef char verify_decal_definition_map_index_offset[
	offsetof(
		struct decal_definition,
		shader.map.index) == 0xe4 ? 1 : -1];

/* ---------- prototypes */

static void rasterizer_decal_vertices_purge_proc(
	long decal_index);
static boolean rasterizer_decal_vertices_locked_proc(
	long decal_index);

/* ---------- globals */

extern struct data_array *global_decal_data;
extern D3DDevice *global_d3d_device;
static short local_layer = 0;
static long rasterizer_decal_cached_bitmap_group_index = 0;
static short rasterizer_decal_cached_bitmap_index = 0;
static short local_framebuffer_blend_function = 0;
static D3DVertexBuffer *local_d3d_vertex_buffer = NULL;
static struct lruv_cache *local_vertex_cache = NULL;
static boolean locked_decal_reported = FALSE;
static boolean permanent_decal_reported = FALSE;
static boolean local_filthy_decal_fog_hack_enabled = FALSE;
extern struct rasterizer_decals_debug_options rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;
extern struct pixel_shader_definition pixel_shader;

long last_decal_index_queried_by_lruv_cache = NONE;

/* ---------- public code */

void rasterizer_decal_vertices_end_update(
	void)
{
	return;
}

void *_rasterizer_decal_vertices_lock(
	long cache_index,
	long cache_size)
{
	byte *vertex_data = NULL;
	unsigned long vertex_data_offset;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		217,
		cache_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		218,
		local_vertex_cache);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		219,
		global_d3d_device);
	vertex_data_offset = (unsigned long)lruv_block_get_address(
		local_vertex_cache,
		cache_index);
	rasterizer_globals.current_lock_operation = _rasterizer_lock_decal_vertices;
	IDirect3DVertexBuffer8_Lock(
		local_d3d_vertex_buffer,
		vertex_data_offset,
		cache_size,
		&vertex_data,
		D3DLOCK_READONLY);
	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	return vertex_data;
}

void _rasterizer_decal_vertices_unlock(
	void)
{
	IDirect3DVertexBuffer8_Unlock(local_d3d_vertex_buffer);

	return;
}

void _rasterizer_decals_initialize_for_new_map(
	void)
{
	return;
}

void _rasterizer_decals_dispose_from_old_map(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		131,
		local_vertex_cache);
	decals_unlock(TRUE);
	lruv_flush(local_vertex_cache);

	return;
}

void rasterizer_decal_vertices_begin_update(
	void)
{
	lruv_idle(local_vertex_cache);

	return;
}

void _rasterizer_decals_update_function_pointers(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		116,
		local_vertex_cache);
	lruv_update_function_pointers(
		local_vertex_cache,
		rasterizer_decal_vertices_purge_proc,
		rasterizer_decal_vertices_locked_proc);

	return;
}

void _rasterizer_decals_initialize(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		89,
		global_d3d_device);
	local_d3d_vertex_buffer = match_malloc(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		91,
		sizeof(D3DVertexBuffer));
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		92,
		local_d3d_vertex_buffer);
	local_d3d_vertex_buffer->Common = 1;
	local_d3d_vertex_buffer->Data = (unsigned long)game_state_gpu_malloc(
		"decal vertices",
		NULL,
		DECAL_VERTEX_CACHE_SIZE);
	local_d3d_vertex_buffer->Lock = 0;
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		96,
		local_d3d_vertex_buffer->Data);
	IDirect3DVertexBuffer8_Register(
		local_d3d_vertex_buffer,
		NULL);
	local_vertex_cache = game_state_lruv_cache_new(
		"decal vertex cache",
		DECAL_VERTEX_CACHE_PAGE_COUNT,
		DECAL_VERTEX_CACHE_PAGE_SIZE_BITS,
		MAXIMUM_DECALS_PER_MAP,
		rasterizer_decal_vertices_purge_proc,
		rasterizer_decal_vertices_locked_proc);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		106,
		local_vertex_cache);

	return;
}

void _rasterizer_decals_dispose(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x99,
		local_vertex_cache);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x9A,
		local_d3d_vertex_buffer);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x9B,
		global_d3d_device);
	if (local_d3d_vertex_buffer)
	{
		IDirect3DVertexBuffer8_Release(local_d3d_vertex_buffer);
		local_d3d_vertex_buffer = NULL;
	}
	lruv_delete(local_vertex_cache);

	return;
}

long _rasterizer_decal_vertices_new(
	long cache_size)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		204,
		cache_size>sizeof(struct decal_vertex));
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		205,
		cache_size%sizeof(struct decal_vertex)==0);

	return lruv_block_new(local_vertex_cache, cache_size);
}

void _rasterizer_decal_vertices_delete(
	long cache_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x106,
		cache_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x107,
		local_vertex_cache);
	lruv_block_delete(local_vertex_cache, cache_index);

	return;
}

void _rasterizer_decals_begin(
	short layer)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		283,
		global_d3d_device);

	{
		short decal_layer_profiles[NUMBER_OF_DECAL_LAYERS] =
		{
			_rasterizer_profile_environment_decals_primary,
			_rasterizer_profile_environment_decals_secondary,
			_rasterizer_profile_environment_decals_light,
			_rasterizer_profile_environment_decals_alpha_tested,
			_rasterizer_profile_environment_decals_water
		};

		if (layer >= 0 && layer < NUMBER_OF_DECAL_LAYERS)
			rasterizer_profile_begin(decal_layer_profiles[layer]);
	}

	local_layer = layer;
	if (rasterizer_debug_options.drawing_mode != _rasterizer_drawing_mode_normal)
		return;
	if (!rasterizer_debug_options.draw_environment_decals)
		return;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		307,
		layer>=0 && layer<NUMBER_OF_DECAL_LAYERS);
	local_framebuffer_blend_function = NONE;
	rasterizer_decal_cached_bitmap_index = NONE;
	rasterizer_decal_cached_bitmap_group_index = NONE;
	local_filthy_decal_fog_hack_enabled = FALSE;
	rasterizer_set_texture(0, 0, 1, NONE, 0);
	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_ADDRESSU,
		D3DTADDRESS_CLAMP);
	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_ADDRESSV,
		D3DTADDRESS_CLAMP);
	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_MAGFILTER,
		D3DTEXF_LINEAR);
	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_MINFILTER,
		D3DTEXF_LINEAR);
	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_MIPFILTER,
		D3DTEXF_LINEAR);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_CULLMODE,
		D3DCULL_CCW);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ALPHABLENDENABLE,
		TRUE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZENABLE,
		TRUE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZWRITEENABLE,
		FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZFUNC,
		D3DCMP_LESSEQUAL);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZBIAS,
		rasterizer_debug_options.decal_zbias);
	if (layer == _decal_layer_alpha_tested)
	{
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHATESTENABLE,
			TRUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHAREF,
			0x7f);
		rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_WRITE_ALPHA_TESTED_DECAL);
	}
	else
	{
		if (rasterizer_debug_options.filthy_decal_fog_hack_enabled &&
			global_window_parameters.fog.atmospheric_maximum_density == 1.0f)
			local_filthy_decal_fog_hack_enabled = TRUE;
		if (local_filthy_decal_fog_hack_enabled)
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHATESTENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHAREF,
				0);
		}
		else
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHATESTENABLE,
				FALSE);
		}
	}
	rasterizer_set_vertex_shader_permutation(1, 10, 0);
	csmemset(&pixel_shader, 0, sizeof(pixel_shader));
	pixel_shader.texture_modes = 1;
	pixel_shader.rgb_outputs[0] = 0xc00;
	pixel_shader.alpha_outputs[1] = 0xc00;
	pixel_shader.rgb_outputs[1] = 0xc00;
	if (local_filthy_decal_fog_hack_enabled)
	{
		pixel_shader.combiner_count = 3;
		pixel_shader.constant_0[0] = 0x1000000;
		pixel_shader.alpha_inputs[2] = 0x1c151115;
		pixel_shader.alpha_outputs[2] = 0xc00;
	}
	else
	{
		pixel_shader.combiner_count = 2;
	}
	pixel_shader.final_combiner_inputs_abcd = 0xc;
	pixel_shader.final_combiner_inputs_efg = 0x1c00;
	IDirect3DDevice8_SetStreamSource(
		global_d3d_device,
		0,
		local_d3d_vertex_buffer,
		sizeof(struct decal_vertex));

	return;
}

void _rasterizer_decals_draw(
	short cluster_index)
{
	long decal_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		408,
		global_d3d_device);
	if (rasterizer_debug_options.drawing_mode != _rasterizer_drawing_mode_normal)
		return;
	if (!rasterizer_debug_options.draw_environment_decals)
		return;

	decal_index = decal_get_first_decal_index(cluster_index, local_layer);
	while (decal_index != NONE)
	{
		struct decal_datum *decal = DECAL_GET(decal_index);
		struct decal_definition *definition = decal_definition_get(decal->definition_index);
		struct decal_shader_definition *shader = &definition->shader;
		short framebuffer_blend_function = shader->framebuffer_blend_function;
		unsigned long vertex_data_offset;
		pixel32 color;
		unsigned long intensity;

		if (local_framebuffer_blend_function != framebuffer_blend_function)
		{
			local_framebuffer_blend_function = framebuffer_blend_function;
			if (framebuffer_blend_function == _shader_framebuffer_blend_function_multiply ||
				framebuffer_blend_function == _shader_framebuffer_blend_function_double_multiply)
			{
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_ALL);
			}
			else
			{
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
			}
			switch (local_framebuffer_blend_function)
			{
				case _shader_framebuffer_blend_function_add:
				case _shader_framebuffer_blend_function_reverse_subtract:
				case _shader_framebuffer_blend_function_max:
					pixel_shader.rgb_inputs[0] = 0x08040000;
					pixel_shader.rgb_inputs[1] = 0x340c0000;
					break;

				case _shader_framebuffer_blend_function_multiply:
				case _shader_framebuffer_blend_function_min:
					pixel_shader.rgb_inputs[0] = 0x28240820;
					pixel_shader.rgb_inputs[1] = 0x340c1420;
					pixel_shader.alpha_inputs[1] = 0x341c1420;
					break;

				case _shader_framebuffer_blend_function_double_multiply:
					pixel_shader.rgb_inputs[0] = 0xa8240820;
					pixel_shader.rgb_inputs[1] = 0x340c14a0;
					pixel_shader.alpha_inputs[1] = 0x341c14a0;
					break;

				case _shader_framebuffer_blend_function_alpha_blend:
					pixel_shader.rgb_inputs[0] = 0x08040000;
					pixel_shader.rgb_inputs[1] = 0x200c0000;
					pixel_shader.alpha_inputs[1] = 0x34180000;
					break;

				case _shader_framebuffer_blend_function_alpha_multiply_add:
					pixel_shader.rgb_inputs[0] = 0x08040000;
					pixel_shader.rgb_inputs[1] = 0x340c0000;
					pixel_shader.alpha_inputs[1] = 0x34180000;
					break;

				default:
					match_vassert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
						470,
						FALSE,
						"### ERROR unsupported framebuffer blend function");
					break;
			}
			rasterizer_set_framebuffer_blend_function(local_framebuffer_blend_function);
			rasterizer_set_pixel_shader(&pixel_shader);
			if (rasterizer_debug_options.statistics_mode == _rasterizer_statistics_mode_geometry)
				rasterizer_frame_statistics.decal_shader_change_count++;
		}

		if (rasterizer_decal_cached_bitmap_group_index != shader->map.index ||
			rasterizer_decal_cached_bitmap_index != (char)decal->bitmap_index)
		{
			rasterizer_decal_cached_bitmap_group_index = shader->map.index;
			rasterizer_decal_cached_bitmap_index = (char)decal->bitmap_index;
			rasterizer_set_texture(
				0,
				0,
				1,
				rasterizer_decal_cached_bitmap_group_index,
				rasterizer_decal_cached_bitmap_index);
			if (rasterizer_debug_options.statistics_mode == _rasterizer_statistics_mode_geometry)
				rasterizer_frame_statistics.decal_texture_change_count++;
		}

		vertex_data_offset = (unsigned long)lruv_block_get_address(local_vertex_cache, decal_index);
		color = decal->color;
		intensity = (decal->intensity * (color >> 24) + 127) >> 8;
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
			510,
			intensity<=PIXEL32_COMPONENT_MASK);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
			511,
			vertex_data_offset%sizeof(struct decal_vertex)==0);
		IDirect3DDevice8_SetVertexData4ub(
			global_d3d_device,
			9,
			color >> 16,
			color >> 8,
			color,
			PIXEL32_COMPONENT_MASK - intensity);
		IDirect3DDevice8_DrawPrimitive(
			global_d3d_device,
			D3DPT_QUADLIST,
			vertex_data_offset/sizeof(struct decal_vertex),
			decal->quad_count);
		if (rasterizer_debug_options.statistics_mode == _rasterizer_statistics_mode_geometry)
		{
			rasterizer_frame_statistics.decal_draw_count++;
			rasterizer_frame_statistics.decal_triangle_count += 2*decal->quad_count;
			rasterizer_frame_statistics.decal_vertex_count += 4*decal->quad_count;
		}

		decal_index = decal->next_decal_index;
	}

	return;
}

void _rasterizer_decals_end(
	void)
{
	if (local_layer == _decal_layer_alpha_tested)
		rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_REJECT);

	{
		short decal_layer_profiles[NUMBER_OF_DECAL_LAYERS] =
		{
			_rasterizer_profile_environment_decals_primary,
			_rasterizer_profile_environment_decals_secondary,
			_rasterizer_profile_environment_decals_light,
			_rasterizer_profile_environment_decals_alpha_tested,
			_rasterizer_profile_environment_decals_water
		};

		if (local_layer >= 0 && local_layer < NUMBER_OF_DECAL_LAYERS)
			rasterizer_profile_end(decal_layer_profiles[local_layer]);
	}

	return;
}

void _rasterizer_decals_flush(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		0x8E,
		local_vertex_cache);
	decals_unlock(FALSE);
	lruv_flush(local_vertex_cache);

	return;
}

/* ---------- private code */

static void rasterizer_decal_vertices_purge_proc(
	long decal_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		29,
		lruv_has_locked_proc(local_vertex_cache));
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		30,
		decal_index);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		31,
		decal_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		32,
		decal_index!=0);
	if (TEST_FLAG(DECAL_GET(decal_index)->flags, _decal_locked_bit) &&
		!locked_decal_reported)
	{
		error(
			2,
			"### ERROR decals: deleting locked decal (#%d, queried=#%d) in rasterizer -- tell Bernie!!",
			decal_index,
			last_decal_index_queried_by_lruv_cache);
		locked_decal_reported = TRUE;
	}
	if (TEST_FLAG(DECAL_GET(decal_index)->flags, _decal_permanent_bit) &&
		!permanent_decal_reported)
	{
		error(
			2,
			"### ERROR decals: deleting permanent decal (#%d, queried=#%d) in rasterizer -- tell Bernie!!",
			decal_index,
			last_decal_index_queried_by_lruv_cache);
		permanent_decal_reported = TRUE;
	}
	decal_delete(decal_index);

	return;
}

static boolean rasterizer_decal_vertices_locked_proc(
	long decal_index)
{
	struct decal_datum *decal;
	boolean locked;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		71,
		decal_index);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		72,
		decal_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_decals.c",
		73,
		decal_index!=0);
	decal = DECAL_GET(decal_index);
	locked = TEST_FLAG(decal->flags, _decal_locked_bit) ||
		TEST_FLAG(decal->flags, _decal_permanent_bit);
	last_decal_index_queried_by_lruv_cache = decal_index;

	return locked;
}
