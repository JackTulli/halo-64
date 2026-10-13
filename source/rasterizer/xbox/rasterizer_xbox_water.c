/*
RASTERIZER_XBOX_WATER.C

symbols in this file:
001688F0 01b0:
	_D3DDevice_SetRenderState (0000)
00168AA0 0050:
	_D3DDevice_SetTextureStageState (0000)
00168AF0 0020:
	_rasterizer_water_set_visibility_for_frame (0000)
00168B10 0010:
	_rasterizer_water_set_visibility_for_window (0000)
00168B20 0010:
	_rasterizer_water_get_visibility_for_window (0000)
00168B30 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
00168D50 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
00168DB0 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
00168DC0 0020:
	_IDirect3DDevice8_SetVertexData2f@16 (0000)
00168DE0 0010:
	_IDirect3DDevice8_SetVertexData2s@16 (0000)
00168DF0 0010:
	_IDirect3DDevice8_Begin@8 (0000)
00168E00 0010:
	_IDirect3DDevice8_End@4 (0000)
00168E10 0820:
	_rasterizer_water_build_bumpmap (0000)
00169630 08c0:
	_rasterizer_water_draw (0000)
0029CBA0 0030:
	??_C@_0DA@GMAFCPCK@?$CD?$CD?$CD?5ERROR?5rasterizer_water_build@ (0000)
0029CBD0 0043:
	??_C@_0ED@IHGINMLN@ripples?$FL2?$FN?4contibution_factor?5?$CL?5@ (0000)
0029CC18 0043:
	??_C@_0ED@IIOIOBNN@ripples?$FL0?$FN?4contibution_factor?5?$CL?5@ (0000)
0029CC5C 0024:
	??_C@_0CE@CFGOBBBI@ripples?$FLripple_index?$FN?4map_repeat@ (0000)
0029CC80 0037:
	??_C@_0DH@OMMLHAPF@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
004662E8 0002:
	_water_needs_update_flag (0000)
	_water_visible_for_window_flag (0001)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game_globals.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmaps_inlines.h"
#include "math/real_math.h"
#include "rasterizer/common/rasterizer_common.h"
#include "rasterizer/rasterizer.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"
#include "tag_files/tag_groups.h"

#include <math.h>
/* The January object retains out-of-line copies of the D3D inline wrappers.
 * The stock XDK definition of D3DINLINE (static __forceinline) reproduces all
 * nine of them; the real calls below are what instantiates them. */
#include <xtl.h>
#include "rasterizer_xbox.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "rasterizer_xbox_water.h"

/* ---------- constants */

enum
{
	NUMBER_OF_WATER_RIPPLES = 4,
	WATER_BUMPMAP_RESOLUTION = 128,
	_rasterizer_target_water_bumpmap = 6
};

enum
{
	_shader_type_transparent_water = 7
};

enum
{
	_vertex_shader_transparent_water_opacity = 0x14,
	_vertex_shader_transparent_water_reflection = 0x17,
	_vertex_shader_transparent_water_bumpmap = 0x26
};

enum
{
	_shader_transparent_water_base_map_alpha_modulates_reflection_bit,
	_shader_transparent_water_base_map_color_modulates_background_bit,
	_shader_transparent_water_atmospheric_fog_bit,
	_shader_transparent_water_draw_before_fog_bit
};

enum
{
	_rasterizer_geometry_no_queue_bit = 1,
	_rasterizer_geometry_sky_bit = 4
};

/* ---------- macros */

#define SHADER_GET_TRANSPARENT_WATER(shader) \
	((struct shader_transparent_water_definition *)shader_get_and_verify_type( \
		(shader), _shader_type_transparent_water))

/* ---------- structures */

struct rasterizer_water_debug_options
{
	byte reserved00[0x1E];
	boolean water;
};

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	byte reserved04[0x8];
	struct shader *shader;
	short shader_permutation_index;
	byte reserved12[0x6E];
	real_plane3d plane;
};

struct water_ripple
{
	byte reserved00[0x4];
	real contibution_factor;
	byte reserved08[0x20];
	real animation_angle;
	real animation_velocity;
	real_point2d map_offset;
	short map_repeats;
	short map_index;
	byte reserved3C[0x10];
};

/* the shader_transparent_water tag definition, from the shader header onwards */
struct shader_transparent_water_definition
{
	struct shader shader;
	unsigned short flags;
	short type;
	byte reserved2C[0x20];
	struct tag_reference base_map;
	byte reserved5C[0x10];
	real_argb_color view_perpendicular_tint_color;
	real_argb_color view_parallel_tint_color;
	byte reserved8C[0x10];
	struct tag_reference reflection_map;
	byte reservedAC[0x10];
	real ripple_animation_angle;
	real ripple_animation_velocity;
	real ripple_scale;
	struct tag_reference ripple_maps;
	short ripple_mipmap_levels;
	short pad0DA;
	real ripple_mipmap_fade_factor;
	real ripple_mipmap_lod_bias;
	byte reserved0E4[0x40];
	struct tag_block ripples;
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

typedef char verify_water_ripple_size[
	sizeof(struct water_ripple) == 0x4C ? 1 : -1];
typedef char verify_water_ripple_animation_angle_offset[
	offsetof(struct water_ripple, animation_angle) == 0x28 ? 1 : -1];
typedef char verify_water_ripple_map_repeats_offset[
	offsetof(struct water_ripple, map_repeats) == 0x38 ? 1 : -1];
typedef char verify_water_definition_flags_offset[
	offsetof(struct shader_transparent_water_definition, flags) == 0x28 ? 1 : -1];
typedef char verify_water_definition_base_map_offset[
	offsetof(struct shader_transparent_water_definition, base_map) == 0x4C ? 1 : -1];
typedef char verify_water_definition_perpendicular_tint_offset[
	offsetof(struct shader_transparent_water_definition, view_perpendicular_tint_color) == 0x6C ? 1 : -1];
typedef char verify_water_definition_parallel_tint_offset[
	offsetof(struct shader_transparent_water_definition, view_parallel_tint_color) == 0x7C ? 1 : -1];
typedef char verify_water_definition_reflection_map_offset[
	offsetof(struct shader_transparent_water_definition, reflection_map) == 0x9C ? 1 : -1];
typedef char verify_water_definition_animation_angle_offset[
	offsetof(struct shader_transparent_water_definition, ripple_animation_angle) == 0xBC ? 1 : -1];
typedef char verify_water_definition_ripple_maps_offset[
	offsetof(struct shader_transparent_water_definition, ripple_maps) == 0xC8 ? 1 : -1];
typedef char verify_water_definition_lod_bias_offset[
	offsetof(struct shader_transparent_water_definition, ripple_mipmap_lod_bias) == 0xE0 ? 1 : -1];
typedef char verify_water_geometry_group_plane_offset[
	offsetof(struct transparent_geometry_group, plane) == 0x80 ? 1 : -1];
typedef char verify_water_definition_mipmap_levels_offset[
	offsetof(struct shader_transparent_water_definition, ripple_mipmap_levels) == 0xD8 ? 1 : -1];
typedef char verify_water_definition_ripples_offset[
	offsetof(struct shader_transparent_water_definition, ripples) == 0x124 ? 1 : -1];
typedef char verify_pixel_shader_definition_size[
	sizeof(struct pixel_shader_definition) == 0xF0 ? 1 : -1];

/* ---------- globals */

boolean water_needs_update_flag = FALSE;
boolean water_visible_for_window_flag = FALSE;

extern struct rasterizer_water_debug_options rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

void rasterizer_water_set_visibility_for_frame(
	boolean visibility)
{
	water_needs_update_flag = !visibility;
	water_visible_for_window_flag = visibility;

	return;
}

void rasterizer_water_set_visibility_for_window(
	boolean visibility)
{
	water_visible_for_window_flag = visibility;

	return;
}

boolean rasterizer_water_get_visibility_for_window(
	void)
{
	return water_visible_for_window_flag;
}

void rasterizer_water_build_bumpmap(
	struct shader *shader)
{
	struct shader_transparent_water_definition *water;
	struct water_ripple ripples[NUMBER_OF_WATER_RIPPLES];
	real texture_transforms[NUMBER_OF_WATER_RIPPLES][8];
	short mipmap_level_count;
	short ripple_index;
	short mipmap_index;
	real cosine;
	real sine;
	real scale;
	real mysterious_horizontal_offset;
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
		47,
		shader);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
		48,
		global_d3d_device);
	if (rasterizer_debug_options.water)
	{
		water = SHADER_GET_TRANSPARENT_WATER(shader);
		mipmap_level_count = MIN(water->ripple_mipmap_levels, NUMBER_OF_WATER_RIPPLES);

		for (ripple_index = 0; ripple_index < NUMBER_OF_WATER_RIPPLES; ripple_index++)
		{
			if (ripple_index < water->ripples.count)
			{
				ripples[ripple_index] = *TAG_BLOCK_GET_ELEMENT(
					&water->ripples,
					ripple_index,
					struct water_ripple);
			}
			else
			{
				csmemset(&ripples[ripple_index], 0, sizeof(struct water_ripple));
				ripples[ripple_index].map_repeats = 1;
			}
		}

		if (ripples[0].contibution_factor == 0.0f &&
			ripples[1].contibution_factor == 0.0f)
			ripples[1].contibution_factor = 1.0f;
		if (ripples[2].contibution_factor == 0.0f &&
			ripples[3].contibution_factor == 0.0f)
			ripples[3].contibution_factor = 1.0f;

		for (ripple_index = 0; ripple_index < NUMBER_OF_WATER_RIPPLES; ripple_index++)
		{
			rasterizer_set_texture(
				ripple_index,
				0,
				3,
				ripple_index < water->ripples.count ? water->ripple_maps.index : NONE,
				ripples[ripple_index].map_index);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				ripple_index,
				D3DTSS_ADDRESSU,
				D3DTADDRESS_WRAP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				ripple_index,
				D3DTSS_ADDRESSV,
				D3DTADDRESS_WRAP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				ripple_index,
				D3DTSS_MAGFILTER,
				D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				ripple_index,
				D3DTSS_MINFILTER,
				D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				ripple_index,
				D3DTSS_MIPFILTER,
				D3DTEXF_LINEAR);
		}

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			D3DCULL_CCW);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHABLENDENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHATESTENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZBIAS,
			0);
		rasterizer_set_vertex_shader_permutation(
			_vertex_shader_transparent_water_bumpmap,
			8,
			0);

		for (ripple_index = 0; ripple_index < NUMBER_OF_WATER_RIPPLES; ripple_index++)
		{
			cosine = (real)cos(ripples[ripple_index].animation_angle);
			sine = (real)sin(ripples[ripple_index].animation_angle);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
				124,
				ripples[ripple_index].map_repeats>0);
			texture_transforms[ripple_index][0] = (real)ripples[ripple_index].map_repeats;
			texture_transforms[ripple_index][1] = 0.0f;
			texture_transforms[ripple_index][2] = 0.0f;
			texture_transforms[ripple_index][3] =
				global_frame_parameters.game_time_sec * ripples[ripple_index].animation_velocity * cosine +
				ripples[ripple_index].map_offset.x;
			texture_transforms[ripple_index][4] = 0.0f;
			texture_transforms[ripple_index][5] = (real)ripples[ripple_index].map_repeats;
			texture_transforms[ripple_index][6] = 0.0f;
			texture_transforms[ripple_index][7] =
				global_frame_parameters.game_time_sec * ripples[ripple_index].animation_velocity * sine +
				ripples[ripple_index].map_offset.y;
		}
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-81,
			texture_transforms,
			8);

		success = TRUE;
		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 0x8421;
		pixel_shader.combiner_count = 0x11004;
		pixel_shader.rgb_inputs[0] = 0x31481149;
		pixel_shader.rgb_outputs[0] = 0xC00;
		pixel_shader.rgb_inputs[1] = 0x314A114B;
		pixel_shader.rgb_outputs[1] = 0xD00;
		pixel_shader.rgb_inputs[2] = 0x31CC11CD;
		pixel_shader.rgb_outputs[2] = 0x30C00;
		pixel_shader.rgb_inputs[3] = 0xCC20A020;
		pixel_shader.rgb_outputs[3] = 0xC00;
		pixel_shader.final_combiner_inputs_abcd = 0x310C0100;
		pixel_shader.final_combiner_inputs_efg = 0;
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
			159,
			ripples[0].contibution_factor + ripples[1].contibution_factor>0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
			160,
			ripples[2].contibution_factor + ripples[3].contibution_factor>0.0f);
		pixel_shader.constant_0[0] = real_alpha_to_pixel32(
			ripples[0].contibution_factor /
			(ripples[0].contibution_factor + ripples[1].contibution_factor));
		pixel_shader.constant_0[1] = real_alpha_to_pixel32(
			ripples[2].contibution_factor /
			(ripples[2].contibution_factor + ripples[3].contibution_factor));
		pixel_shader.constant_0[2] = real_alpha_to_pixel32(
			(ripples[0].contibution_factor + ripples[1].contibution_factor) /
			(ripples[0].contibution_factor + ripples[1].contibution_factor +
			ripples[2].contibution_factor + ripples[3].contibution_factor));
		rasterizer_set_pixel_shader(&pixel_shader);
		rasterizer_set_stencil_mode(0);

		for (mipmap_index = 0; mipmap_index < mipmap_level_count; mipmap_index++)
		{
			if (water->ripple_mipmap_levels > 1)
				pixel_shader.final_combiner_constant_0 = real_alpha_to_pixel32(
					(real)mipmap_index / (real)(water->ripple_mipmap_levels - 1) *
					water->ripple_mipmap_fade_factor) | 0x8080FF;
			else
				pixel_shader.final_combiner_constant_0 = 0x7F7FFF;
			rasterizer_set_pixel_shader(&pixel_shader);
			rasterizer_set_target(
				_rasterizer_target_water_bumpmap,
				mipmap_index,
				0,
				FALSE,
				FALSE);

			scale = 1.0f / (real)(WATER_BUMPMAP_RESOLUTION >> mipmap_index);
			mysterious_horizontal_offset = scale * -2.0f;

			if (IDirect3DDevice8_Begin(
				global_d3d_device,
				D3DPT_TRIANGLEFAN) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN)");
			}

			if (IDirect3DDevice8_SetVertexData2s(
				global_d3d_device,
				4,
				0,
				0) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 0)");
			}
			if (IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				scale - 1.0f + mysterious_horizontal_offset,
				scale + 1.0f) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, scale - 1.0f + mysterious_horizontal_offset, scale + 1.0f)");
			}

			if (IDirect3DDevice8_SetVertexData2s(
				global_d3d_device,
				4,
				1,
				0) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 0)");
			}
			if (IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				scale + 1.0f + mysterious_horizontal_offset,
				scale + 1.0f) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, scale + 1.0f + mysterious_horizontal_offset, scale + 1.0f)");
			}

			if (IDirect3DDevice8_SetVertexData2s(
				global_d3d_device,
				4,
				1,
				1) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 1)");
			}
			if (IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				scale + 1.0f + mysterious_horizontal_offset,
				scale - 1.0f) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, scale + 1.0f + mysterious_horizontal_offset, scale - 1.0f)");
			}

			if (IDirect3DDevice8_SetVertexData2s(
				global_d3d_device,
				4,
				0,
				1) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 1)");
			}
			if (IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				scale - 1.0f + mysterious_horizontal_offset,
				scale - 1.0f) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, scale - 1.0f + mysterious_horizontal_offset, scale - 1.0f)");
			}

			if (IDirect3DDevice8_End(global_d3d_device) >= 0 && success)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_End(global_d3d_device)");
			}
		}

		rasterizer_set_target(
			global_window_parameters.rasterizer_target,
			0,
			0,
			FALSE,
			TRUE);
		rasterizer_set_stencil_mode(2);

		if (!success)
			error(2, "### ERROR rasterizer_water_build_bumpmap failed");
	}

	return;
}

void rasterizer_water_draw(
	struct transparent_geometry_group const *group)
{
	struct shader_transparent_water_definition *water;
	real_rgb_color tint_color;
	short vertex_type;
	short permutation_index;
	boolean write_depth;
	boolean draw_depth_only;
	real reflection_amount;
	union
	{
		real value;
		unsigned long bits;
	} mipmap_lod_bias;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
		238,
		group);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_water.c",
		239,
		global_d3d_device);
	if (rasterizer_debug_options.water)
	{
		water = SHADER_GET_TRANSPARENT_WATER(group->shader);
		permutation_index = shader_get_vertex_shader_permutation(group->shader);
		vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
		draw_depth_only = TEST_FLAG(water->flags, _shader_transparent_water_draw_before_fog_bit) &&
			!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit) &&
			!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_sky_bit);
		if (draw_depth_only)
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				D3DCULL_NONE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_COLORWRITEENABLE,
				0);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHABLENDENABLE,
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHATESTENABLE,
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZFUNC,
				D3DCMP_LESSEQUAL);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				TRUE);
			rasterizer_set_vertex_shader_permutation(
				_vertex_shader_transparent_water_opacity,
				vertex_type,
				permutation_index);
			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.combiner_count = 1;
			rasterizer_set_pixel_shader(&pixel_shader);
			rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
		}
		else
		{
			write_depth = !TEST_FLAG(group->geometry_flags, _rasterizer_geometry_sky_bit) &&
				!TEST_FLAG(water->flags, _shader_transparent_water_draw_before_fog_bit);
			if (water_needs_update_flag)
			{
				rasterizer_water_build_bumpmap(group->shader);
				water_needs_update_flag = FALSE;
			}

			if (TEST_FLAG(water->flags, _shader_transparent_water_base_map_alpha_modulates_reflection_bit))
			{
				rasterizer_set_texture(
					0,
					0,
					1,
					water->base_map.index,
					group->shader_permutation_index);
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
				rasterizer_set_texture_direct(
					1,
					global_rasterizer_data->vector_normalization.index,
					0);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_ADDRESSU,
					D3DTADDRESS_CLAMP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_ADDRESSV,
					D3DTADDRESS_CLAMP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_ADDRESSW,
					D3DTADDRESS_CLAMP);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_MAGFILTER,
					D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_MINFILTER,
					D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					1,
					D3DTSS_MIPFILTER,
					D3DTEXF_LINEAR);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_CULLMODE,
					D3DCULL_NONE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_ALPHA);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHABLENDENABLE,
					FALSE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ALPHATESTENABLE,
					FALSE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZENABLE,
					TRUE);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZFUNC,
					D3DCMP_LESSEQUAL);
				IDirect3DDevice8_SetRenderState(
					global_d3d_device,
					D3DRS_ZWRITEENABLE,
					write_depth);
				rasterizer_set_vertex_shader_permutation(
					_vertex_shader_transparent_water_opacity,
					vertex_type,
					permutation_index);
				csmemset(&pixel_shader, 0, sizeof(pixel_shader));
				pixel_shader.texture_modes = 0x61;
				pixel_shader.combiner_count = 2;
				pixel_shader.constant_0[0] = real_alpha_to_pixel32(
					water->view_perpendicular_tint_color.alpha);
				pixel_shader.constant_1[0] = real_alpha_to_pixel32(
					water->view_parallel_tint_color.alpha);
				pixel_shader.alpha_inputs[0] = 0x29120911;
				pixel_shader.alpha_outputs[0] = 0xC00;
				pixel_shader.alpha_inputs[1] = 0x1C180000;
				pixel_shader.alpha_outputs[1] = 0xC0;
				pixel_shader.final_combiner_inputs_abcd = 0;
				pixel_shader.final_combiner_inputs_efg = 0x1C00;
				rasterizer_set_pixel_shader(&pixel_shader);
				rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
			}

			if (TEST_FLAG(water->flags, _shader_transparent_water_base_map_color_modulates_background_bit))
			{
				rasterizer_set_texture(
					0,
					0,
					1,
					water->base_map.index,
					group->shader_permutation_index);
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
					D3DCULL_NONE);
				SetRenderStateSmart(
					D3DRS_COLORWRITEENABLE,
					D3DCOLORWRITEENABLE_RED |
					D3DCOLORWRITEENABLE_GREEN |
					D3DCOLORWRITEENABLE_BLUE);
				SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
				SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_ZERO);
				SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
				SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
				SetRenderStateSmart(D3DRS_ZENABLE, TRUE);
				SetRenderStateSmart(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				SetRenderStateSmart(D3DRS_ZWRITEENABLE, write_depth);
				rasterizer_set_vertex_shader_permutation(
					_vertex_shader_transparent_water_opacity,
					vertex_type,
					permutation_index);
				csmemset(&pixel_shader, 0, sizeof(pixel_shader));
				pixel_shader.texture_modes = 1;
				pixel_shader.combiner_count = 1;
				pixel_shader.final_combiner_inputs_abcd =
					(TEST_FLAG(water->flags, _shader_transparent_water_atmospheric_fog_bit) ? 0x13 : 0) << 24 |
					0x200800;
				rasterizer_set_pixel_shader(&pixel_shader);
				rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
			}

			rasterizer_set_target_as_texture(
				0,
				_rasterizer_target_water_bumpmap,
				MIN(water->ripple_mipmap_levels, NUMBER_OF_WATER_RIPPLES));
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_ADDRESSU,
				D3DTADDRESS_WRAP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_ADDRESSV,
				D3DTADDRESS_WRAP);
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
			rasterizer_set_texture(
				3,
				2,
				0,
				water->reflection_map.index,
				group->shader_permutation_index);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_ADDRESSU,
				D3DTADDRESS_CLAMP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_ADDRESSV,
				D3DTADDRESS_CLAMP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_ADDRESSW,
				D3DTADDRESS_CLAMP);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_MAGFILTER,
				D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_MINFILTER,
				D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				3,
				D3DTSS_MIPFILTER,
				D3DTEXF_LINEAR);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				D3DCULL_NONE);
			SetRenderStateSmart(
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_RED |
				D3DCOLORWRITEENABLE_GREEN |
				D3DCOLORWRITEENABLE_BLUE);
			SetRenderStateSmart(
				D3DRS_ALPHABLENDENABLE,
				!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_sky_bit));
			SetRenderStateSmart(
				D3DRS_SRCBLEND,
				TEST_FLAG(water->flags, _shader_transparent_water_base_map_alpha_modulates_reflection_bit)
					? D3DBLEND_DESTALPHA
					: D3DBLEND_ONE);
			SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_ONE);
			SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
			SetRenderStateSmart(D3DRS_ZENABLE, TRUE);
			SetRenderStateSmart(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			SetRenderStateSmart(D3DRS_ZWRITEENABLE, write_depth);
			rasterizer_set_vertex_shader_permutation(
				_vertex_shader_transparent_water_reflection,
				vertex_type,
				permutation_index);

			{
				real vertex_constants[3][4] =
				{
					{
						water->ripple_scale,
						water->ripple_scale,
						(real)cos(water->ripple_animation_angle) *
							water->ripple_animation_velocity * global_frame_parameters.game_time_sec,
						(real)sin(water->ripple_animation_angle) *
							water->ripple_animation_velocity * global_frame_parameters.game_time_sec
					},
					{ 0.0f, 0.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 0.0f }
				};

				IDirect3DDevice8_SetVertexShaderConstant(
					global_d3d_device,
					-84,
					vertex_constants,
					3);
			}

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 0x64621;
			pixel_shader.dot_mapping = 0x111;
			if (TEST_FLAG(water->flags, _shader_transparent_water_atmospheric_fog_bit))
			{
				pixel_shader.combiner_count = 4;
				pixel_shader.rgb_inputs[0] = 0x0B0B0120;
				pixel_shader.rgb_outputs[0] = 0xCD;
				pixel_shader.rgb_inputs[1] = 0x0C0C0000;
				pixel_shader.rgb_outputs[1] = 0xC0;
				pixel_shader.rgb_inputs[2] = 0x0C0C0000;
				pixel_shader.rgb_outputs[2] = 0xC0;
				pixel_shader.rgb_inputs[3] = 0x2D0C0D0B;
				pixel_shader.rgb_outputs[3] = 0xC00;
				pixel_shader.final_combiner_inputs_abcd = 0x330C0000;
			}
			else
			{
				pixel_shader.combiner_count = 2;
				pixel_shader.rgb_inputs[0] = 0x0B0B0120;
				pixel_shader.rgb_outputs[0] = 0xCD;
				pixel_shader.rgb_inputs[1] = 0x0C0C0000;
				pixel_shader.rgb_outputs[1] = 0xC0;
				pixel_shader.final_combiner_inputs_abcd = 0x2D0F0B00;
				pixel_shader.final_combiner_inputs_efg = 0x0C0C0000;
			}

			if (magnitude3d(&group->plane.n) > 0.0f)
			{
				struct render_camera const *camera = &global_window_parameters.camera;

				reflection_amount = PIN(
					-dot_product3d(&camera->forward, &group->plane.n),
					0.0f,
					1.0f);
				tint_color.red = (1.0f - reflection_amount) * water->view_parallel_tint_color.red +
					reflection_amount * water->view_perpendicular_tint_color.red;
				tint_color.green = (1.0f - reflection_amount) * water->view_parallel_tint_color.green +
					reflection_amount * water->view_perpendicular_tint_color.green;
				tint_color.blue = (1.0f - reflection_amount) * water->view_parallel_tint_color.blue +
					reflection_amount * water->view_perpendicular_tint_color.blue;
				pixel_shader.constant_0[0] = real_rgb_color_to_pixel32(&tint_color);
			}
			else
			{
				pixel_shader.constant_0[0] = 0xFFFFFF;
			}
			rasterizer_set_pixel_shader(&pixel_shader);

			mipmap_lod_bias.value = -water->ripple_mipmap_lod_bias;
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_MIPMAPLODBIAS,
				mipmap_lod_bias.bits);
			rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_MIPMAPLODBIAS,
				0);
		}
	}

	return;
}

/* ---------- private code */
