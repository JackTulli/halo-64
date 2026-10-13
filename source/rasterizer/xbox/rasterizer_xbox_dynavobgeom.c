/*
RASTERIZER_XBOX_DYNAVOBGEOM.C

symbols in this file:
0014E840 01b0:
	_D3DDevice_SetRenderState (0000)
0014E9F0 0050:
	_D3DDevice_SetTextureStageState (0000)
0014EA40 0010:
	__rasterizer_hud_begin (0000)
0014EA50 0010:
	__rasterizer_hud_end (0000)
0014EA60 0010:
	__rasterizer_dynamic_lit_geometry_draw (0000)
0014EA70 0100:
	__rasterizer_dynamic_screen_geometry_add_multitexture_params_to_base (0000)
0014EB70 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
0014ED90 0030:
	__rasterizer_dynamic_screen_geometry_draw (0000)
0014EDC0 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
0014EE20 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
0014EE30 0020:
	_IDirect3DDevice8_SetVertexData2f@16 (0000)
0014EE50 0010:
	_IDirect3DDevice8_SetVertexDataColor@12 (0000)
0014EE60 0010:
	_IDirect3DDevice8_Begin@8 (0000)
0014EE70 0010:
	_IDirect3DDevice8_End@4 (0000)
0014EE80 0270:
	__rasterizer_dynamic_unlit_geometry_draw (0000)
0014F0F0 0040:
	_submit_screen_vertex (0000)
0014F130 0b90:
	__rasterizer_psuedo_dynamic_screen_quad_draw (0000)
0028FBDC 0010:
	??_C@_0BA@KKBHHCGF@multitex_params?$AA@ (0000)
0028FBEC 0005:
	??_C@_04BHIIPFEC@base?$AA@ (0000)
0028FBF4 003d:
	??_C@_0DN@KLLFPFFH@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0028FC34 003e:
	??_C@_0DO@GMEFDNIN@_rasterizer_dynamic_screen_geome@ (0000)
0028FC74 002f:
	??_C@_0CP@PIDBCAKH@?$CD?$CD?$CD?5ERROR?5too?5many?5transparent?5g@ (0000)
0028FCA4 0009:
	??_C@_08JOJIKHG@centroid?$AA@ (0000)
0028FCB0 0027:
	??_C@_0CH@DLHDBKLI@shader?9?$DObase?4type?$DN?$DN_shader_type_@ (0000)
0028FCD8 0069:
	??_C@_0GJ@OKAOIPGM@?$CBTEST_FLAG?$CIgeometry_flags?0?5_rast@ (0000)
0028FD44 0016:
	??_C@_0BG@MMAGMAFI@meter?9?$DOgradient?$DN?$DN1?40f?$AA@ (0000)
0028FD5C 0013:
	??_C@_0BD@BMLFIAIE@meter?9?$DOtint_mode_2?$AA@ (0000)
0028FD70 0035:
	??_C@_0DF@MDAHAMMD@?$CBparameters?9?$DOmap?$FL1?$FN?5?$HM?$HM?5?$CBparamete@ (0000)
0028FDA8 002a:
	??_C@_0CK@NCHEDINN@?$CBparameters?9?$DOmap?$FL2?$FN?5?$HM?$HM?5parameter@ (0000)
0028FDD4 0013:
	??_C@_0BD@LBFDNODJ@parameters?9?$DOmap?$FL0?$FN?$AA@ (0000)
00465A16 0001:
	?warned@?1??_rasterizer_dynamic_unlit_geometry_draw@@9@9 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "math/real_math.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmaps_inlines.h"
#include "rasterizer.h"
#include "rasterizer/rasterizer_debug_options.h"
#include "rasterizer/rasterizer_frame_statistics.h"
#include "rasterizer/rasterizer_model_types.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "render/render_cameras.h"
#include "shaders/shader_definitions.h"
#include "rasterizer_xbox_pixel_shader.h"

/* The January translation unit retains the XDK's out-of-line D3D wrappers.
 * Keep the stock D3DINLINE definition: the real calls below make VC7 emit
 * the target's complete wrapper bodies. */
#include <xtl.h>

#include "rasterizer_xbox.h"
#include "rasterizer_xbox_dynavobgeom.h"
#include "rasterizer_xbox_pixel_shader.h"

/* ---------- constants */

enum
{
	_rasterizer_geometry_viewspace_bit = 5,
};

enum
{
	_shader_type_effect = 1,
};

enum
{
	_render_model_effect_type_none = 0,
};

enum
{
	_shader_effect_sort_bias_bit = 0,
};

enum
{
	_rasterizer_statistics_mode_geometry = 2,
};

enum
{
	_rasterizer_target_render_primary = 0,
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
	NUMBER_OF_SCREEN_GEOMETRY_MAPS = 3,
	SCREEN_GEOMETRY_TRANSFORM_CONSTANT = -68,
	SCREEN_GEOMETRY_MAP_CONSTANT = -63,
	SCREEN_GEOMETRY_VERTEX_SHADER_INDEX = 4,
	SCREEN_GEOMETRY_VERTEX_TYPE = 8,
	SCREEN_GEOMETRY_VERTEX_SHADER_PERMUTATION = 1,
};

enum
{
	_screen_geometry_vertex_register_position = 0,
	_screen_geometry_vertex_register_texcoord = 4,
	_screen_geometry_vertex_register_color = 9,
};

/* ---------- macros */

#define SHADER_EFFECT_SORT_BIAS 0.25f

#define SHADER_GET_EFFECT(shader) \
	((struct shader_effect_definition *)shader_get_and_verify_type( \
		(struct shader *)(shader), _shader_type_effect))

/* ---------- structures */

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader *shader;
	short shader_permutation_index;
	short pad12;
	struct render_model_effect effect;
	real_vector2d model_base_map_scale;
	long dynamic_triangle_buffer_index;
	struct triangle_buffer const *triangle_buffer;
	long first_triangle_index;
	long triangle_count;
	long dynamic_vertex_buffer_index;
	struct vertex_buffer const *vertex_buffer;
	struct bitmap_data const *lightmap;
	real_matrix4x3 const *node_matrices;
	short node_matrix_count;
	short pad66;
	struct render_lighting const *lighting;
	struct render_animation const *animation;
	real z_sort;
	real_point3d centroid;
	real_plane3d plane;
	long sorted_index;
	short previous_group_presorted_index;
	short next_group_presorted_index;
	long active_camouflage_transparent_source_object_index;
	boolean sort_last;
	boolean cortana_hack;
	byte pad9E[2];
};

typedef char transparent_geometry_group_size_assert[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];
typedef char transparent_geometry_group_model_base_map_scale_offset_assert[
	offsetof(struct transparent_geometry_group, model_base_map_scale) == 0x3C ? 1 : -1];
typedef char transparent_geometry_group_z_sort_offset_assert[
	offsetof(struct transparent_geometry_group, z_sort) == 0x70 ? 1 : -1];
typedef char transparent_geometry_group_plane_offset_assert[
	offsetof(struct transparent_geometry_group, plane) == 0x80 ? 1 : -1];
typedef char transparent_geometry_group_cortana_hack_offset_assert[
	offsetof(struct transparent_geometry_group, cortana_hack) == 0x9D ? 1 : -1];

struct rasterizer_meter_parameters
{
	pixel32 gradient_min_color;
	pixel32 gradient_max_color;
	pixel32 background_color;
	pixel32 flash_color;
	boolean flash_color_is_negative;
	boolean tint_mode_2;
	byte pad12[2];
	pixel32 tint_color;
	real gradient;
};

typedef char rasterizer_dynamic_geometry_pixel_shader_size_assert[
	sizeof(struct pixel_shader_definition) == 0xF0 ? 1 : -1];

typedef char rasterizer_dynamic_geometry_viewport_bounds_offset_assert[
	offsetof(struct rasterizer_window_begin_parameters, camera.viewport_bounds) == 0x34 ? 1 : -1];

typedef char rasterizer_dynamic_geometry_camera_offset_assert[
	offsetof(struct rasterizer_window_begin_parameters, camera) == 0x8 ? 1 : -1];
/* ---------- prototypes */

static void submit_screen_vertex(
	struct dynamic_screen_vertex const *vertex);

/* ---------- globals */

extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

void _rasterizer_hud_begin(
	void)
{
	rasterizer_profile_begin(_rasterizer_profile_hud);
	return;
}

void _rasterizer_hud_end(
	void)
{
	rasterizer_profile_end(_rasterizer_profile_hud);
	return;
}

void _rasterizer_psuedo_dynamic_screen_quad_draw(
	struct rasterizer_dynamic_screen_geometry_parameters *parameters,
	struct dynamic_screen_vertex *vertices)
{
	struct rasterizer_meter_parameters const *meter;
	real_rgb_color const *tint;
	short map_index;
	short combiner_index;
	long vertex_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		756,
		global_d3d_device);

	if (!rasterizer_debug_options.dynamic_screen_geometry)
	{
		return;
	}
	if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
	{
		return;
	}

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		761,
		parameters);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		763,
		parameters->map[0]);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		765,
		!parameters->map[2] || parameters->map[1]);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		767,
		!parameters->map[1] || !parameters->meter_parameters);

	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_CULLMODE, D3DCULL_NONE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHABLENDENABLE, TRUE);
	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHATESTENABLE, FALSE);
	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZENABLE, FALSE);
	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZBIAS, 0);

	rasterizer_set_framebuffer_blend_function(parameters->framebuffer_blend_function);

	{
		real vertex_constants[20];
		real map_constants[24];
		real_vector2d normalized_offset;
		short width;
		short height;

		width =
			global_window_parameters.camera.viewport_bounds.x1 -
			global_window_parameters.camera.viewport_bounds.x0;
		height =
			global_window_parameters.camera.viewport_bounds.y1 -
			global_window_parameters.camera.viewport_bounds.y0;

		normalized_offset.i = parameters->offset ? 2.0f * parameters->offset->i / width : 0.0f;
		normalized_offset.j = parameters->offset ? -2.0f * parameters->offset->j / height : 0.0f;

		vertex_constants[0] = 2.0f * (1.0f / width);
		vertex_constants[1] = 0.0f;
		vertex_constants[2] = 0.0f;
		vertex_constants[3] = normalized_offset.i - (1.0f / width + 1.0f);
		vertex_constants[4] = 0.0f;
		vertex_constants[5] = -2.0f * (1.0f / height);
		vertex_constants[6] = 0.0f;
		vertex_constants[7] = 1.0f / height + normalized_offset.j + 1.0f;
		vertex_constants[8] = 0.0f;
		vertex_constants[9] = 0.0f;
		vertex_constants[10] = 0.0f;
		vertex_constants[11] = 0.5f;
		vertex_constants[12] = 0.0f;
		vertex_constants[13] = 0.0f;
		vertex_constants[14] = 0.0f;
		vertex_constants[15] = 1.0f;
		vertex_constants[16] = parameters->map_texture_scale[0].i;
		vertex_constants[17] = parameters->map_texture_scale[0].j;
		vertex_constants[18] = 0.0f;
		vertex_constants[19] = 1.0f;

		map_constants[0] = parameters->map_texture_scale[1].i;
		map_constants[1] = parameters->map_texture_scale[1].j;
		map_constants[2] = parameters->map_texture_scale[2].i;
		map_constants[3] = parameters->map_texture_scale[2].j;
		map_constants[4] = parameters->map_anchor_screen[0] ? 1.0f : 0.0f;
		map_constants[5] = parameters->map_anchor_screen[0] ? 0.0f : 1.0f;
		map_constants[6] = parameters->map_anchor_screen[1] ? 1.0f : 0.0f;
		map_constants[7] = parameters->map_anchor_screen[1] ? 0.0f : 1.0f;
		map_constants[8] = parameters->map_anchor_screen[2] ? 1.0f : 0.0f;
		map_constants[9] = parameters->map_anchor_screen[2] ? 0.0f : 1.0f;
		map_constants[10] = parameters->map_offset[0] ? parameters->map_offset[0]->x : 0.0f;
		map_constants[11] = parameters->map_offset[0] ? parameters->map_offset[0]->y : 0.0f;
		map_constants[12] = parameters->map_offset[1] ? parameters->map_offset[1]->x : 0.0f;
		map_constants[13] = parameters->map_offset[1] ? parameters->map_offset[1]->y : 0.0f;
		map_constants[14] = parameters->map_offset[2] ? parameters->map_offset[2]->x : 0.0f;
		map_constants[15] = parameters->map_offset[2] ? parameters->map_offset[2]->y : 0.0f;
		map_constants[16] = parameters->map_scale[0].i;
		map_constants[17] = parameters->map_scale[0].j;
		map_constants[18] = parameters->map_scale[1].i;
		map_constants[19] = parameters->map_scale[1].j;
		map_constants[20] = parameters->map_scale[2].i;
		map_constants[21] = parameters->map_scale[2].j;
		map_constants[22] = 0.0f;
		map_constants[23] = 0.0f;

		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			SCREEN_GEOMETRY_TRANSFORM_CONSTANT,
			vertex_constants,
			5);
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			SCREEN_GEOMETRY_MAP_CONSTANT,
			map_constants,
			6);
	}

	for (map_index = 0; map_index < NUMBER_OF_SCREEN_GEOMETRY_MAPS; map_index++)
	{
		if (!parameters->map[map_index])
		{
			break;
		}

		rasterizer_set_texture_bitmap_data(map_index, parameters->map[map_index]);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			map_index,
			D3DTSS_ADDRESSU,
			parameters->map_wrapped[map_index] ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			map_index,
			D3DTSS_ADDRESSV,
			parameters->map_wrapped[map_index] ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			map_index,
			D3DTSS_MAGFILTER,
			parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			map_index,
			D3DTSS_MINFILTER,
			parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			map_index,
			D3DTSS_MIPFILTER,
			parameters->point_sampled ? D3DTEXF_POINT : D3DTEXF_LINEAR);
	}

	rasterizer_set_vertex_shader_permutation(
		SCREEN_GEOMETRY_VERTEX_SHADER_INDEX,
		SCREEN_GEOMETRY_VERTEX_TYPE,
		SCREEN_GEOMETRY_VERTEX_SHADER_PERMUTATION);

	meter = parameters->meter_parameters;
	if (meter)
	{
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
			886,
			meter->tint_mode_2);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
			887,
			meter->gradient==1.0f);

		IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHABLENDENABLE, TRUE);
		IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_SRCBLEND, D3DBLEND_CONSTANTCOLOR);
		IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_BLENDCOLOR, meter->tint_color);
		IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_BLENDOP, D3DBLENDOP_ADD);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_ALPHAKILL,
			D3DTALPHAKILL_ENABLE);

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 1;
		pixel_shader.combiner_count = 0x00011104;

		pixel_shader.constant_0[0] = meter->gradient_min_color;
		pixel_shader.constant_1[0] =
			real_alpha_to_pixel32(1.0f / MAX(meter->gradient * 8.0f, 1.0f)) |
			(meter->gradient_max_color & 0x00FFFFFF);
		pixel_shader.alpha_inputs[0] = 0x12081208;
		pixel_shader.rgb_inputs[0] = 0x1120E820;
		pixel_shader.alpha_outputs[0] = 0x00020C00;
		pixel_shader.rgb_outputs[0] = 0x00020C00;

		pixel_shader.constant_0[1] = meter->gradient_min_color;
		pixel_shader.constant_1[1] = meter->gradient_max_color;
		pixel_shader.alpha_inputs[1] = 0x6C200000;
		pixel_shader.alpha_outputs[1] = 0x000000C0;
		pixel_shader.rgb_inputs[1] = 0x3C011C02;
		pixel_shader.rgb_outputs[1] = 0x00000C00;

		pixel_shader.constant_0[2] = meter->gradient_min_color;
		pixel_shader.constant_1[2] = meter->flash_color;
		pixel_shader.alpha_inputs[2] = 0x0820B220;
		pixel_shader.alpha_outputs[2] = 0x00000C00;
		pixel_shader.rgb_inputs[2] =
			0x0C201C00 | (meter->flash_color_is_negative ? 0x000000E2 : 0x00000002);
		pixel_shader.rgb_outputs[2] = 0x00000C00;

		pixel_shader.constant_0[3] = meter->background_color;
		pixel_shader.constant_1[3] = meter->tint_color;
		pixel_shader.alpha_inputs[3] = 0x12201120;
		pixel_shader.alpha_outputs[3] = 0x00004C00;
		pixel_shader.rgb_inputs[3] = 0x0C200120;
		pixel_shader.rgb_outputs[3] = 0x00004C00;

		pixel_shader.final_combiner_inputs_abcd = 0x0C180000;
		pixel_shader.final_combiner_inputs_efg = 0x00001C00;
	}
	else if (parameters->map[0])
	{
		real_argb_color map_color[NUMBER_OF_SCREEN_GEOMETRY_MAPS];

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes =
			(((((parameters->map[2] ? 1 : 0) << 5) | (parameters->map[1] ? 1 : 0)) << 5) |
			(parameters->map[0] ? 1 : 0));

		tint = parameters->map_tint[0] ? parameters->map_tint[0] : global_real_rgb_white;
		map_color[0].rgb = *tint;
		tint = parameters->map_tint[1] ? parameters->map_tint[1] : global_real_rgb_white;
		map_color[1].rgb = *tint;
		tint = parameters->map_tint[2] ? parameters->map_tint[2] : global_real_rgb_white;
		map_color[2].rgb = *tint;
		map_color[0].alpha = parameters->map_fade[0] ? *parameters->map_fade[0] : 1.0f;
		map_color[1].alpha = parameters->map_fade[1] ? *parameters->map_fade[1] : 1.0f;
		map_color[2].alpha = parameters->map_fade[2] ? *parameters->map_fade[2] : 1.0f;

		pixel_shader.constant_0[0] = real_argb_color_to_pixel32(&map_color[0]);
		pixel_shader.constant_1[0] = real_argb_color_to_pixel32(&map_color[1]);
		pixel_shader.constant_0[1] = real_argb_color_to_pixel32(&map_color[2]);
		pixel_shader.constant_0[4] = real_argb_color_to_pixel32(&parameters->plasma_fade);
		pixel_shader.constant_0[5] = real_argb_color_to_pixel32(&parameters->plasma_fade);
		pixel_shader.constant_0[6] = real_argb_color_to_pixel32(&parameters->plasma_fade);
		pixel_shader.constant_0[7] = real_argb_color_to_pixel32(&parameters->plasma_fade);

		pixel_shader.rgb_outputs[0] = 0x00000089;
		pixel_shader.alpha_outputs[0] = 0x00000089;
		pixel_shader.rgb_inputs[0] = 0x08010902;
		pixel_shader.alpha_inputs[0] = 0x18111912;
		pixel_shader.rgb_inputs[1] = 0x0A010804;
		pixel_shader.rgb_outputs[1] = 0x000000AC;
		pixel_shader.alpha_inputs[1] = 0x1A111814;
		pixel_shader.alpha_outputs[1] = 0x000000AC;

		combiner_index = 2;
		if (parameters->map[1])
		{
			switch (parameters->map0_to_1_blend_function)
			{
				case _shader_framebuffer_blend_function_alpha_blend:
					pixel_shader.rgb_inputs[2] = 0x0C200920;
					pixel_shader.alpha_inputs[2] = 0x1C201920;
					pixel_shader.alpha_outputs[2] = 0x00000C00;
					pixel_shader.rgb_outputs[2] = 0x00000C00;
					break;

				case _shader_framebuffer_blend_function_multiply:
					pixel_shader.rgb_inputs[2] = 0x0C090000;
					pixel_shader.rgb_outputs[2] = 0x000000C0;
					pixel_shader.alpha_inputs[2] = 0x1C190000;
					pixel_shader.alpha_outputs[2] = 0x000000C0;
					break;

				case _shader_framebuffer_blend_function_double_multiply:
					pixel_shader.rgb_inputs[2] = 0x0C20E920;
					pixel_shader.alpha_inputs[2] = 0x1C20F920;
					pixel_shader.alpha_outputs[2] = 0x00000C00;
					pixel_shader.rgb_outputs[2] = 0x00000C00;
					break;

				case _shader_framebuffer_blend_function_add:
					pixel_shader.rgb_inputs[2] = 0x0C090000;
					pixel_shader.rgb_outputs[2] = 0x000100C0;
					pixel_shader.alpha_inputs[2] = 0x1C190000;
					pixel_shader.alpha_outputs[2] = 0x000100C0;
					break;

				case _shader_framebuffer_blend_function_reverse_subtract:
					pixel_shader.rgb_inputs[2] = 0x0C090000;
					pixel_shader.rgb_outputs[2] = 0x000020C0;
					pixel_shader.alpha_inputs[2] = 0x1C190000;
					pixel_shader.alpha_outputs[2] = 0x000000C0;
					break;

				case _shader_framebuffer_blend_function_min:
					pixel_shader.alpha_inputs[2] = 0x0820A920;
					pixel_shader.rgb_inputs[2] = 0x1920B820;
					pixel_shader.alpha_inputs[3] = 0x1C1C0C0C;
					pixel_shader.alpha_outputs[3] = 0x00024C00;
					pixel_shader.rgb_inputs[3] = 0x00000000;
					pixel_shader.rgb_outputs[3] = 0x00000000;
					pixel_shader.alpha_inputs[4] = 0x00005C5C;
					pixel_shader.alpha_outputs[4] = 0x00004D00;
					pixel_shader.rgb_inputs[4] = 0x00000000;
					pixel_shader.rgb_outputs[4] = 0x00000000;
					combiner_index = 5;
					pixel_shader.alpha_inputs[5] = 0x00000000;
					pixel_shader.alpha_outputs[5] = 0x00000C00;
					pixel_shader.rgb_inputs[5] = 0x1CA01DA0;
					pixel_shader.rgb_outputs[5] = 0x00000C00;
					pixel_shader.alpha_outputs[2] = 0x00000C00;
					pixel_shader.rgb_outputs[2] = 0x00000C00;
					break;
			}

			combiner_index++;
		}

		if (parameters->map[2])
		{
			switch (parameters->map1_to_2_blend_function)
			{
				case _shader_framebuffer_blend_function_alpha_blend:
					if (parameters->map0_to_1_blend_function ==
						_shader_framebuffer_blend_function_min)
					{
						pixel_shader.rgb_inputs[combiner_index] = 0x0C010A00 |
							(parameters->doing_plasma_effect ? 0x00000004 : 0x00000020);
					}
					else
					{
						pixel_shader.rgb_inputs[combiner_index] = 0x0C200A20;
					}
					pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
					pixel_shader.alpha_inputs[combiner_index] = 0x1C201A20;
					pixel_shader.alpha_outputs[combiner_index] = 0x00000C00;
					break;

				case _shader_framebuffer_blend_function_multiply:
					pixel_shader.rgb_inputs[combiner_index] = 0x0C0A0000;
					pixel_shader.rgb_outputs[combiner_index] = 0x000000C0;
					pixel_shader.alpha_inputs[combiner_index] = 0x1C1A0000;
					pixel_shader.alpha_outputs[combiner_index] = 0x000000C0;
					break;

				case _shader_framebuffer_blend_function_double_multiply:
					pixel_shader.rgb_inputs[combiner_index] = 0x0C20EA20;
					pixel_shader.rgb_outputs[combiner_index] = 0x00000C00;
					pixel_shader.alpha_inputs[combiner_index] = 0x1C20FA20;
					pixel_shader.alpha_outputs[combiner_index] = 0x00000C00;
					break;

				case _shader_framebuffer_blend_function_add:
					pixel_shader.rgb_inputs[combiner_index] = 0x0C0A0000;
					pixel_shader.rgb_outputs[combiner_index] = 0x000100C0;
					pixel_shader.alpha_inputs[combiner_index] = 0x1C1A0000;
					pixel_shader.alpha_outputs[combiner_index] = 0x000100C0;
					break;

				case _shader_framebuffer_blend_function_reverse_subtract:
					pixel_shader.rgb_inputs[combiner_index] = 0x0C0A0000;
					pixel_shader.rgb_outputs[combiner_index] = 0x000020C0;
					pixel_shader.alpha_inputs[combiner_index] = 0x1C1A0000;
					pixel_shader.alpha_outputs[combiner_index] = 0x000000C0;
					break;
			}

			combiner_index++;
		}

		pixel_shader.combiner_count = combiner_index | 0x00011100;
		pixel_shader.final_combiner_inputs_abcd = 0x0000000C;
		pixel_shader.final_combiner_inputs_efg = 0x00001C00;
	}

	rasterizer_set_pixel_shader(&pixel_shader);

	IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_CULLMODE, D3DCULL_CCW);
	rasterizer_set_vertex_shader_permutation(
		SCREEN_GEOMETRY_VERTEX_SHADER_INDEX,
		SCREEN_GEOMETRY_VERTEX_TYPE,
		SCREEN_GEOMETRY_VERTEX_SHADER_PERMUTATION);

	IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
	for (vertex_index = 0; vertex_index < NUMBER_OF_VERTICES_PER_QUADRILATERAL; vertex_index++)
	{
		submit_screen_vertex(&vertices[vertex_index]);
	}
	IDirect3DDevice8_End(global_d3d_device);

	IDirect3DDevice8_SetTextureStageState(
		global_d3d_device,
		0,
		D3DTSS_ALPHAKILL,
		D3DTALPHAKILL_DISABLE);

	return;
}

void _rasterizer_dynamic_unlit_geometry_draw(
	struct shader const *shader,
	struct bitmap_data const *primary_map,
	struct render_animation const *animation,
	long dynamic_triangle_buffer_index,
	long dynamic_vertex_buffer_index,
	long triangle_count,
	real_point3d const *centroid,
	unsigned long geometry_flags)
{
	struct transparent_geometry_group *group;
	real_vector3d forward;
	real_plane3d zero_plane;
	static boolean warned = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		38,
		global_d3d_device);

	if (!rasterizer_debug_options.dynamic_unlit_geometry)
	{
		return;
	}

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		42,
		!TEST_FLAG(geometry_flags, _rasterizer_geometry_viewspace_bit) ||
			shader->base.type==_shader_type_effect);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		44,
		shader->base.type==_shader_type_effect);

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		52,
		centroid);

	vector_from_points3d(&global_window_parameters.camera.position, centroid, &forward);

	group = rasterizer_transparent_geometry_new_group();
	if (group)
	{
		zero_plane.n.i = 0.0f;
		zero_plane.n.j = 0.0f;
		zero_plane.n.k = 0.0f;
		zero_plane.d = 0.0f;

		group->geometry_flags = geometry_flags;
		group->object_index = 0;
		group->source_object_index = 0;
		group->shader = (struct shader *)shader;
		group->shader_permutation_index = 0;
		group->effect.type = _render_model_effect_type_none;
		group->dynamic_triangle_buffer_index = dynamic_triangle_buffer_index;
		group->triangle_buffer = NULL;
		group->first_triangle_index = 0;
		group->triangle_count = triangle_count;
		group->dynamic_vertex_buffer_index = dynamic_vertex_buffer_index;
		group->vertex_buffer = NULL;
		group->lightmap = primary_map;
		group->z_sort = -dot_product3d(&global_window_parameters.camera.forward, &forward);
		group->centroid = *centroid;
		group->plane = zero_plane;
		group->model_base_map_scale.i = group->model_base_map_scale.j = 1.0f;
		group->previous_group_presorted_index = NONE;
		group->next_group_presorted_index = NONE;
		group->active_camouflage_transparent_source_object_index = 0;
		group->cortana_hack = FALSE;

		if (shader->base.type == _shader_type_effect &&
			TEST_FLAG(SHADER_GET_EFFECT(shader)->flags, _shader_effect_sort_bias_bit))
		{
			group->z_sort += SHADER_EFFECT_SORT_BIAS;
		}

		group->node_matrices = NULL;
		group->node_matrix_count = 0;
		group->lighting = NULL;
		group->animation = NULL;

		if (rasterizer_debug_options.stats == _rasterizer_statistics_mode_geometry)
		{
			rasterizer_frame_statistics.dynamic_unlit_draw_count++;
			rasterizer_frame_statistics.dynamic_unlit_triangle_count += triangle_count;
			rasterizer_frame_statistics.largest_dynamic_unlit_triangle_count = MAX(
				triangle_count,
				rasterizer_frame_statistics.largest_dynamic_unlit_triangle_count);
			rasterizer_frame_statistics.dynamic_unlit_vertex_count +=
				rasterizer_frame_statistics_count_dynamic_vertices(
					dynamic_triangle_buffer_index,
					0,
					triangle_count);
		}
	}
	else if (!warned)
	{
		error(
			_error_silent,
			"### ERROR too many transparent geometry groups");
		warned = TRUE;
	}

	return;
}

void _rasterizer_dynamic_lit_geometry_draw(
	void const *vertices,
	void const *parameters)
{
	return;
}

void _rasterizer_dynamic_screen_geometry_draw(
	long primitive_type,
	long vertex_type,
	long dynamic_vertex_buffer_index,
	long vertex_count)
{
	match_vassert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		255,
		FALSE,
		"_rasterizer_dynamic_screen_geometry_draw not supported no mo'");

	return;
}

void _rasterizer_dynamic_screen_geometry_add_multitexture_params_to_base(
	struct rasterizer_dynamic_screen_geometry_parameters *base,
	struct rasterizer_dynamic_screen_geometry_parameters const *multitex_params)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		220,
		base);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_dynavobgeom.c",
		221,
		multitex_params);

	base->offset = multitex_params->offset;
	base->map_anchor_screen[1] = multitex_params->map_anchor_screen[1];
	base->map_anchor_screen[2] = multitex_params->map_anchor_screen[2];
	base->map[1] = multitex_params->map[1];
	base->map[2] = multitex_params->map[2];
	base->map_wrapped[1] = multitex_params->map_wrapped[1];
	base->map_wrapped[2] = multitex_params->map_wrapped[2];
	base->map_offset[1] = multitex_params->map_offset[1];
	base->map_offset[2] = multitex_params->map_offset[2];
	base->map_scale[1] = multitex_params->map_scale[1];
	base->map_scale[2] = multitex_params->map_scale[2];
	base->map_texture_scale[1] = multitex_params->map_texture_scale[1];
	base->map_texture_scale[2] = multitex_params->map_texture_scale[2];
	base->map_tint[1] = multitex_params->map_tint[1];
	base->map_tint[2] = multitex_params->map_tint[2];
	base->map_fade[1] = multitex_params->map_fade[1];
	base->map_fade[2] = multitex_params->map_fade[2];
	base->map0_to_1_blend_function = multitex_params->map0_to_1_blend_function;
	base->map1_to_2_blend_function = multitex_params->map1_to_2_blend_function;

	return;
}

/* ---------- private code */

static void submit_screen_vertex(
	struct dynamic_screen_vertex const *vertex)
{
	IDirect3DDevice8_SetVertexDataColor(
		global_d3d_device,
		_screen_geometry_vertex_register_color,
		vertex->color);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		_screen_geometry_vertex_register_texcoord,
		vertex->texture_coordinates.x,
		vertex->texture_coordinates.y);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		_screen_geometry_vertex_register_position,
		vertex->position.x,
		vertex->position.y);

	return;
}
