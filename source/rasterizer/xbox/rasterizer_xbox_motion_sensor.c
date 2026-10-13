/*
RASTERIZER_XBOX_MOTION_SENSOR.C

symbols in this file:
0015D220 01b0:
	_code_0015d220 (D3DDevice_SetRenderState)
0015D3D0 0050:
	_code_0015d3d0 (D3DDevice_SetTextureStageState)
0015D420 0220:
	_code_0015d420 (IDirect3DDevice8_SetRenderState)
0015D640 0060:
	_code_0015d640 (IDirect3DDevice8_SetTextureStageState)
0015D6A0 0010:
	_code_0015d6a0 (IDirect3DDevice8_SetVertexShaderConstant)
0015D6B0 0020:
	_code_0015d6b0 (IDirect3DDevice8_SetVertexData2f)
0015D6D0 0030:
	_code_0015d6d0 (IDirect3DDevice8_SetVertexData4f)
0015D700 0010:
	_code_0015d700 (IDirect3DDevice8_SetVertexData2s)
0015D710 0010:
	_code_0015d710 (IDirect3DDevice8_Begin)
0015D720 0010:
	_code_0015d720 (IDirect3DDevice8_End)
0015D730 0280:
	__rasterizer_hud_motion_sensor_blip_begin (0000)
0015D9B0 0180:
	__rasterizer_hud_motion_sensor_blip_draw (0000)
0015DB30 0780:
	__rasterizer_hud_motion_sensor_blip_end (0000)
00291ECC 003f:
	??_C@_0DP@DPMIHOMG@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
00291F0C 0004:
	__real@bd000000 (0000)
00465E27 0001:
	_bss_00465e27 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_group.h"
#include "game/players.h"
#include "interface/interface.h"
#include "math/real_math.h"
#include "rasterizer/rasterizer.h"
#include <xtl.h>

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct motion_sensor_debug_options
{
	byte reserved00[0x23];
	boolean motion_sensor;
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

/* ---------- prototypes */

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index);

void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load);

void rasterizer_set_target(
	word target,
	boolean use_depth_buffer,
	boolean clear_color,
	boolean clear_depth,
	boolean clear_stencil);

void rasterizer_set_target_as_texture(
	short stage,
	long target,
	boolean filtered);

void rasterizer_set_texture_bitmap_data(
	short stage,
	struct bitmap_data const *bitmap);

void rasterizer_set_vertex_shader_permutation(
	short vertex_type,
	short permutation,
	boolean one_node);

void rasterizer_set_pixel_shader(
	struct pixel_shader_definition const *definition);

void SetRenderStateSmart(
	unsigned long state,
	unsigned long value);

/* ---------- globals */

extern void *global_d3d_device;
extern struct motion_sensor_debug_options rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;
extern struct pixel_shader_definition pixel_shader;

boolean bss_00465e27 = {0};

/* ---------- public code */

void _rasterizer_hud_motion_sensor_blip_begin(
	void)
{
	struct bitmap_data *blip_bitmap;
	struct bitmap_data *large_blip_bitmap;
	real vertex_constants[5][4];

	blip_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_motion_blip),
		0);
	large_blip_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_iface_map1),
		0);

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_motion_sensor.c",
		27,
		global_d3d_device);

	bss_00465e27 = FALSE;
	if (rasterizer_debug_options.motion_sensor &&
		_texture_cache_bitmap_get_hardware_format(blip_bitmap, FALSE, TRUE) &&
		_texture_cache_bitmap_get_hardware_format(large_blip_bitmap, FALSE, TRUE))
	{
		bss_00465e27 = TRUE;
		rasterizer_set_target(4, FALSE, FALSE, TRUE, FALSE);
		rasterizer_set_texture_bitmap_data(0, blip_bitmap);

		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_ADDRESSU,
			D3DTADDRESS_BORDER);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_ADDRESSV,
			D3DTADDRESS_BORDER);
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
			D3DTEXF_POINT);

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			0x901);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHABLENDENABLE,
			TRUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_SRCBLEND,
			D3DBLEND_ONE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_DESTBLEND,
			D3DBLEND_ONE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_BLENDOP,
			D3DBLENDOP_ADD);
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

		rasterizer_set_vertex_shader_permutation(4, 8, FALSE);

		vertex_constants[0][0] = 1.0f;
		vertex_constants[0][1] = 0.0f;
		vertex_constants[0][2] = 0.0f;
		vertex_constants[0][3] = 0.0f;
		vertex_constants[1][0] = 0.0f;
		vertex_constants[1][1] = 1.0f;
		vertex_constants[1][2] = 0.0f;
		vertex_constants[1][3] = 0.0f;
		vertex_constants[2][0] = 0.0f;
		vertex_constants[2][1] = 0.0f;
		vertex_constants[2][2] = 1.0f;
		vertex_constants[2][3] = 0.0f;
		vertex_constants[3][0] = 0.0f;
		vertex_constants[3][1] = 0.0f;
		vertex_constants[3][2] = 0.0f;
		vertex_constants[3][3] = 1.0f;
		vertex_constants[4][0] = 1.0f;
		vertex_constants[4][1] = 1.0f;
		vertex_constants[4][2] = 0.0f;
		vertex_constants[4][3] = 1.0f;
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-68,
			vertex_constants,
			5);

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 1;
		pixel_shader.combiner_count = 1;
		pixel_shader.final_combiner_inputs_abcd = 0x08040000;
		rasterizer_set_pixel_shader(&pixel_shader);
	}

	return;
}

void _rasterizer_hud_motion_sensor_blip_draw(
	real_point2d const *position,
	real intensity,
	real size,
	real_rgb_color const *color,
	boolean large_blip)
{
	struct bitmap_data *blip_bitmap;
	struct bitmap_data *large_blip_bitmap;
	real_point2d scaled_position;
	real radius;

	blip_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_motion_blip),
		0);
	large_blip_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_iface_map1),
		0);

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_motion_sensor.c",
		109,
		global_d3d_device);

	rasterizer_set_texture_bitmap_data(
		0,
		large_blip ? large_blip_bitmap : blip_bitmap);
	if (rasterizer_debug_options.motion_sensor && bss_00465e27)
	{
		radius = size * 0.0625f;
		scaled_position.x = position->x * -0.03125f;
		scaled_position.y = position->y * -0.03125f;

		IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			9,
			color->red * intensity,
			color->green * intensity,
			color->blue * intensity,
			1.0f);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 0);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			scaled_position.x - radius,
			scaled_position.y + radius);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 0);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			scaled_position.x + radius,
			scaled_position.y + radius);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 1);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			scaled_position.x + radius,
			scaled_position.y - radius);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 1);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			scaled_position.x - radius,
			scaled_position.y - radius);
		IDirect3DDevice8_End(global_d3d_device);
	}

	return;
}

void _rasterizer_hud_motion_sensor_blip_end(
	real_point2d const *center,
	real scale)
{
	struct bitmap_data *sweep_bitmap;
	struct bitmap_data *sweep_mask_bitmap;
	real vertex_constants[5][4];
	real half_scale;
	real high;
	real low;
	real radius;
	short height;
	short width;

	sweep_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_motion_sweep),
		0);
	sweep_mask_bitmap = bitmap_group_try_and_get_bitmap(
		interface_get_tag_index(_interface_bitmap_motion_sweep_mask),
		0);

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_motion_sensor.c",
		156,
		global_d3d_device);

	if (rasterizer_debug_options.motion_sensor)
	{
		if (bss_00465e27 &&
			_texture_cache_bitmap_get_hardware_format(sweep_bitmap, FALSE, TRUE) &&
			_texture_cache_bitmap_get_hardware_format(sweep_mask_bitmap, FALSE, TRUE))
		{
			rasterizer_set_texture_bitmap_data(0, sweep_bitmap);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_ADDRESSU,
				D3DTADDRESS_BORDER);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_ADDRESSV,
				D3DTADDRESS_BORDER);
			IDirect3DDevice8_SetTextureStageState(
				global_d3d_device,
				0,
				D3DTSS_BORDERCOLOR,
				0x46000000);
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
				D3DTEXF_POINT);

			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				0x901);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_RED |
				D3DCOLORWRITEENABLE_GREEN |
				D3DCOLORWRITEENABLE_BLUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHABLENDENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_SRCBLEND,
				D3DBLEND_ONE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_DESTBLEND,
				D3DBLEND_SRCALPHA);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_BLENDOP,
				D3DBLENDOP_ADD);
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
			rasterizer_set_vertex_shader_permutation(4, 8, FALSE);

			vertex_constants[0][0] = 1.0f;
			vertex_constants[0][1] = 0.0f;
			vertex_constants[0][2] = 0.0f;
			vertex_constants[0][3] = 0.0f;
			vertex_constants[1][0] = 0.0f;
			vertex_constants[1][1] = 1.0f;
			vertex_constants[1][2] = 0.0f;
			vertex_constants[1][3] = 0.0f;
			vertex_constants[2][0] = 0.0f;
			vertex_constants[2][1] = 0.0f;
			vertex_constants[2][2] = 1.0f;
			vertex_constants[2][3] = 0.0f;
			vertex_constants[3][0] = 0.0f;
			vertex_constants[3][1] = 0.0f;
			vertex_constants[3][2] = 0.0f;
			vertex_constants[3][3] = 1.0f;
			vertex_constants[4][0] = 1.0f;
			vertex_constants[4][1] = 1.0f;
			vertex_constants[4][2] = 0.0f;
			vertex_constants[4][3] = 1.0f;
			IDirect3DDevice8_SetVertexShaderConstant(
				global_d3d_device,
				-68,
				vertex_constants,
				5);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 1;
			pixel_shader.combiner_count = 1;
			pixel_shader.rgb_inputs[0] = 0x08040000;
			pixel_shader.rgb_outputs[0] = 0xC0;
			pixel_shader.alpha_inputs[0] = 0x18140000;
			pixel_shader.alpha_outputs[0] = 0xC0;
			pixel_shader.final_combiner_inputs_abcd = 0xC;
			pixel_shader.final_combiner_inputs_efg = 0x1C00;
			rasterizer_set_pixel_shader(&pixel_shader);

			IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				9,
				0.4588f,
				0.7294f,
				1.0f,
				1.0f);
			half_scale = scale * 0.5f;
			low = 0.5f - half_scale;
			high = 0.5f + half_scale;
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, high, low);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, -1.015625f, 1.046875f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, low, low);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, 1.046875f, 1.046875f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, low, high);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, 1.046875f, -1.015625f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, high, high);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, -1.015625f, -1.015625f);
			IDirect3DDevice8_End(global_d3d_device);

			rasterizer_set_texture_bitmap_data(0, sweep_mask_bitmap);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHABLENDENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_SRCBLEND,
				D3DBLEND_ZERO);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_DESTBLEND,
				D3DBLEND_SRCALPHA);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 1;
			pixel_shader.combiner_count = 1;
			pixel_shader.final_combiner_inputs_abcd = 8;
			pixel_shader.final_combiner_inputs_efg = 0x1800;
			rasterizer_set_pixel_shader(&pixel_shader);

			IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				9,
				0.4f,
				0.8f,
				0.4f,
				1.0f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, 1.0f, 0.0f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, -1.015625f, 1.046875f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, 0.0f, 0.0f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, 1.046875f, 1.046875f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, 0.0f, 1.0f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, 1.046875f, -1.015625f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 4, 1.0f, 1.0f);
			IDirect3DDevice8_SetVertexData2f(global_d3d_device, 0, -1.015625f, -1.015625f);
			IDirect3DDevice8_End(global_d3d_device);

			rasterizer_set_target(
				global_window_parameters.rasterizer_target,
				FALSE,
				FALSE,
				FALSE,
				TRUE);
			rasterizer_set_target_as_texture(0, 4, FALSE);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_MIPFILTER, D3DTEXF_POINT);
			IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_BORDERCOLOR, 0);
			IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_CULLMODE, 0x901);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_RED |
				D3DCOLORWRITEENABLE_GREEN |
				D3DCOLORWRITEENABLE_BLUE);
			SetRenderStateSmart(0x3B, TRUE);
			SetRenderStateSmart(0x3E, TRUE);
			SetRenderStateSmart(0x3F, 0x303);
			SetRenderStateSmart(0x4A, 0x8006);
			SetRenderStateSmart(0x3C, FALSE);
			SetRenderStateSmart(0x7B, FALSE);
			IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZBIAS, 0);
			rasterizer_set_vertex_shader_permutation(4, 8, FALSE);

			width = global_window_parameters.camera.viewport_bounds.x1 - global_window_parameters.camera.viewport_bounds.x0;
			height = global_window_parameters.camera.viewport_bounds.y1 - global_window_parameters.camera.viewport_bounds.y0;
			vertex_constants[0][0] = 2.0f / width;
			vertex_constants[0][1] = 0.0f;
			vertex_constants[0][2] = 0.0f;
			vertex_constants[0][3] = -1.0f - 1.0f / width;
			vertex_constants[1][0] = 0.0f;
			vertex_constants[1][1] = -2.0f / height;
			vertex_constants[1][2] = 0.0f;
			vertex_constants[1][3] = 1.0f + 1.0f / height;
			vertex_constants[2][0] = 0.0f;
			vertex_constants[2][1] = 0.0f;
			vertex_constants[2][2] = 0.0f;
			vertex_constants[2][3] = 0.5f;
			vertex_constants[3][0] = 0.0f;
			vertex_constants[3][1] = 0.0f;
			vertex_constants[3][2] = 0.0f;
			vertex_constants[3][3] = 1.0f;
			vertex_constants[4][0] = 1.0f;
			vertex_constants[4][1] = 1.0f;
			vertex_constants[4][2] = 0.0f;
			vertex_constants[4][3] = 1.0f;
			IDirect3DDevice8_SetVertexShaderConstant(
				global_d3d_device,
				-68,
				vertex_constants,
				5);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 1;
			pixel_shader.combiner_count = 1;
			pixel_shader.final_combiner_inputs_abcd = 8;
			pixel_shader.final_combiner_inputs_efg = 0x1800;
			rasterizer_set_pixel_shader(&pixel_shader);

			radius = local_player_count() > 1 ? 32.0f : 42.0f;
			IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 0);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				center->x - radius,
				center->y - radius);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 0);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				center->x + radius,
				center->y - radius);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 1);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				center->x + radius,
				center->y + radius);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 1);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				center->x - radius,
				center->y + radius);
			IDirect3DDevice8_End(global_d3d_device);
			return;
		}
	}

	if (rasterizer_debug_options.motion_sensor && bss_00465e27)
	{
		rasterizer_set_target(
			global_window_parameters.rasterizer_target,
			FALSE,
			FALSE,
			FALSE,
			TRUE);
	}

	return;
}

/* ---------- private code */
