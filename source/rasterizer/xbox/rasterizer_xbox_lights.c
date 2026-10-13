/*
RASTERIZER_XBOX_LIGHTS.C

symbols in this file:
00158850 01b0:
	_D3DDevice_SetRenderState (0000)
00158A00 0050:
	_D3DDevice_SetTextureStageState (0000)
00158A50 01c0:
	_rasterizer_project_billboard (0000)
00158C10 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
00158E30 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
00158E90 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
00158EA0 0020:
	_IDirect3DDevice8_SetVertexData2f@16 (0000)
00158EC0 0030:
	_IDirect3DDevice8_SetVertexData4f@24 (0000)
00158EF0 0010:
	_IDirect3DDevice8_SetVertexData2s@16 (0000)
00158F00 0010:
	_IDirect3DDevice8_Begin@8 (0000)
00158F10 0010:
	_IDirect3DDevice8_End@4 (0000)
00158F20 0380:
	_rasterizer_sun_glow_copy_source (0000)
001592A0 0580:
	_rasterizer_sun_glow_convolve (0000)
00159820 0930:
	_rasterizer_sun_glow_draw (0000)
002911D0 0038:
	??_C@_0DI@FKFGJLFP@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
00291208 002e:
	??_C@_0CO@KOLMKNFB@?$CD?$CD?$CD?5ERROR?5rasterizer_sun_glow_co@ (0000)
00291238 007c:
	??_C@_0HM@HAPGHELG@IDirect3DDevice8_SetVertexData2f@ (0000)
002912B4 003d:
	??_C@_0DN@HPJBNCPC@IDirect3DDevice8_SetVertexData2s@ (0000)
002912F8 007c:
	??_C@_0HM@KNFENDHB@IDirect3DDevice8_SetVertexData2f@ (0000)
00291374 003d:
	??_C@_0DN@LEMNABFH@IDirect3DDevice8_SetVertexData2s@ (0000)
002913B8 007c:
	??_C@_0HM@GLDLNKPG@IDirect3DDevice8_SetVertexData2f@ (0000)
00291434 003d:
	??_C@_0DN@LFAPGLGA@IDirect3DDevice8_SetVertexData2s@ (0000)
00291478 007c:
	??_C@_0HM@LGJJHNDB@IDirect3DDevice8_SetVertexData2f@ (0000)
002914F4 003d:
	??_C@_0DN@HOFDLIMF@IDirect3DDevice8_SetVertexData2s@ (0000)
00291534 003d:
	??_C@_0DN@POLEKNHJ@IDirect3DDevice8_Begin?$CIglobal_d3@ (0000)
00291578 0045:
	??_C@_0EF@CFFLNGMH@secondary_target?$DO?$DN0?5?$CG?$CG?5secondary@ (0000)
002915C0 0041:
	??_C@_0EB@GKNGOOIJ@primary_target?$DO?$DN0?5?$CG?$CG?5primary_tar@ (0000)
00291604 002a:
	??_C@_0CK@FKCHNKBL@?$CD?$CD?$CD?5ERROR?5rasterizer_sun_glow_dr@ (0000)
00291630 005f:
	??_C@_0FP@CKNMNDPB@IDirect3DDevice8_SetVertexData2f@ (0000)
00291690 005f:
	??_C@_0FP@LJNHCKOE@IDirect3DDevice8_SetVertexData2f@ (0000)
002916F0 005f:
	??_C@_0FP@JAOANEPA@IDirect3DDevice8_SetVertexData2f@ (0000)
00291750 005f:
	??_C@_0FP@DOLCNOF@IDirect3DDevice8_SetVertexData2f@ (0000)
002917B0 0066:
	??_C@_0GG@GCMGACEI@IDirect3DDevice8_SetVertexData4f@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "game/game_globals.h"
#include "math/real_math.h"
#include "rasterizer/common/rasterizer_common.h"
#include "rasterizer/rasterizer_geometry.h"

#include <math.h>

/* The January translation unit retains the XDK's out-of-line D3D wrappers.
 * Keep the stock D3DINLINE definition: the real calls below make VC7 emit
 * the target's complete wrapper bodies. */
#include "rasterizer/rasterizer.h"
#include <xtl.h>

#include "rasterizer_xbox.h"

/* ---------- constants */

enum
{
	_rasterizer_target_sun_glow_primary = 4,
	_rasterizer_target_sun_glow_secondary,

	NUMBER_OF_RASTERIZER_TARGETS = 8
};

enum
{
	SUN_GLOW_CONVOLVE_PASS_COUNT = 4,
	SUN_GLOW_RAY_COUNT = 16
};

/* ---------- macros */

/* ---------- structures */

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

struct lens_flare_definition
{
	byte reserved00[0x10];
	real occlusion_radius;
};

struct rasterizer_lens_flare_submit_parameters
{
	struct lens_flare_definition *definition;
	real_point3d position;
	unsigned long compressed_direction;
	unsigned long compressed_up;
	unsigned long compressed_light_color;
	short light_identifier;
	short light_index;
	short lens_flare_index;
	byte compressed_window_index;
	byte compressed_light_scale;
	long internal__occlusion_pixels;
};

/* ---------- prototypes */

static boolean rasterizer_project_billboard(
	real_point3d const *point,
	real radius,
	real_point3d *projected_center,
	real_vector2d *projected_axes);

static void rasterizer_sun_glow_copy_source(
	short target,
	real_rectangle2d const *bounds);

static short rasterizer_sun_glow_convolve(
	short primary_target,
	short secondary_target,
	short pass_count);

/* ---------- globals */

extern struct rasterizer_window_begin_parameters global_window_parameters;
extern struct pixel_shader_definition pixel_shader;

/* ---------- public code */

void rasterizer_sun_glow_draw(
	struct rasterizer_lens_flare_submit_parameters const *parameters)
{
	real vertex_constants[5][4];
	real_point3d occlusion_point;
	short pass;
	real_rectangle2d screen_bounds;
	real brightness;
	real_rectangle2d bounds;
	real_vector2d projected_axes;
	real_point3d projected_center;
	real_vector3d direction;
	real intensity;
	short viewport_width;
	short viewport_height;
	real center_x;
	real center_y;
	real r;
	short blurred_target;
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		583,
		global_d3d_device);

	direction.i = parameters->position.x - global_window_parameters.camera.position.x;
	direction.j = parameters->position.y - global_window_parameters.camera.position.y;
	direction.k = parameters->position.z - global_window_parameters.camera.position.z;
	normalize3d(&direction);

	intensity =
		(direction.i * global_window_parameters.camera.forward.i +
		direction.j * global_window_parameters.camera.forward.j +
		direction.k * global_window_parameters.camera.forward.k -
		(real)cos(0.7853981634f)) /
		(1.0f - (real)cos(0.7853981634f));
	if (intensity < 0.0f)
	{
		brightness = 0.0f;
	}
	else if (intensity > 1.0f)
	{
		brightness = 1.0f;
	}
	else
	{
		brightness = intensity;
	}

	viewport_width = global_window_parameters.camera.viewport_bounds.x1 -
		global_window_parameters.camera.viewport_bounds.x0;
	viewport_height = global_window_parameters.camera.viewport_bounds.y1 -
		global_window_parameters.camera.viewport_bounds.y0;

	vertex_constants[0][0] = (1.0f / (real)viewport_width) * 2.0f;
	vertex_constants[0][1] = 0.0f;
	vertex_constants[0][2] = 0.0f;
	vertex_constants[0][3] = -1.0f - (1.0f / (real)viewport_width);
	vertex_constants[1][0] = 0.0f;
	vertex_constants[1][1] = (1.0f / (real)viewport_height) * -2.0f;
	vertex_constants[1][2] = 0.0f;
	vertex_constants[1][3] = (1.0f / (real)viewport_height) + 1.0f;
	vertex_constants[2][0] = 0.0f;
	vertex_constants[2][1] = 0.0f;
	vertex_constants[2][2] = 1.0f;
	vertex_constants[2][3] = 0.0f;
	vertex_constants[3][0] = 0.0f;
	vertex_constants[3][1] = 0.0f;
	vertex_constants[3][2] = 0.0f;
	vertex_constants[3][3] = 1.0f;
	vertex_constants[4][0] = 0.0f;
	vertex_constants[4][1] = 0.0f;
	vertex_constants[4][2] = 0.0f;
	vertex_constants[4][3] = 1.0f;
	IDirect3DDevice8_SetVertexShaderConstant(
		global_d3d_device,
		-68,
		vertex_constants,
		5);

	direction = uncompress_int32_to_real_vector3d(parameters->compressed_direction);
	occlusion_point.x = parameters->definition->occlusion_radius * direction.i +
		parameters->position.x;
	occlusion_point.y = parameters->definition->occlusion_radius * direction.j +
		parameters->position.y;
	occlusion_point.z = parameters->definition->occlusion_radius * direction.k +
		parameters->position.z;

	if (rasterizer_project_billboard(
		&occlusion_point,
		parameters->definition->occlusion_radius,
		&projected_center,
		&projected_axes))
	{
		center_x = (real)floor(projected_center.x + 0.5f);
		center_y = (real)floor(projected_center.y + 0.5f);

		bounds.x0 = center_x - 32.0f;
		bounds.y0 = center_y - 32.0f;
		bounds.x1 = center_x + 32.0f;
		bounds.y1 = center_y + 32.0f;

		screen_bounds.x0 = (real)global_window_parameters.camera.viewport_bounds.x0 + bounds.x0;
		screen_bounds.y0 = (real)global_window_parameters.camera.viewport_bounds.y0 + bounds.y0;
		screen_bounds.x1 = (real)global_window_parameters.camera.viewport_bounds.x0 + bounds.x1;
		screen_bounds.y1 = (real)global_window_parameters.camera.viewport_bounds.y0 + bounds.y1;

		if (screen_bounds.x0 < (real)global_window_parameters.camera.viewport_bounds.x1 &&
			screen_bounds.y0 < (real)global_window_parameters.camera.viewport_bounds.y1 &&
			screen_bounds.x1 > (real)global_window_parameters.camera.viewport_bounds.x0 &&
			screen_bounds.y1 > (real)global_window_parameters.camera.viewport_bounds.y0)
		{
			rasterizer_set_vertex_shader_permutation(0x38, 6, FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				D3DCULL_CCW);
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
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				0);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.combiner_count = 1;
			pixel_shader.final_combiner_constant_0 = 0;
			pixel_shader.final_combiner_inputs_efg = 0x1100;
			rasterizer_set_pixel_shader(&pixel_shader);

			IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				bounds.x0,
				bounds.y0);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				bounds.x1,
				bounds.y0);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				bounds.x1,
				bounds.y1);
			IDirect3DDevice8_SetVertexData2f(
				global_d3d_device,
				0,
				bounds.x0,
				bounds.y1);
			IDirect3DDevice8_End(global_d3d_device);

			rasterizer_set_vertex_shader_permutation(0x38, 6, FALSE);
			rasterizer_set_texture_direct(0, global_rasterizer_data->glow.index, 0);
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
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				0);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 1;
			pixel_shader.combiner_count = 1;
			pixel_shader.alpha_inputs[0] = 0x18200000;
			pixel_shader.alpha_outputs[0] = 0x200C0;
			pixel_shader.final_combiner_inputs_efg = 0x1C00;
			rasterizer_set_pixel_shader(&pixel_shader);

			IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 0);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				0,
				bounds.x0,
				bounds.y0,
				projected_center.z,
				1.0f);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 0);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				0,
				bounds.x1,
				bounds.y0,
				projected_center.z,
				1.0f);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 1);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				0,
				bounds.x1,
				bounds.y1,
				projected_center.z,
				1.0f);
			IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 1);
			IDirect3DDevice8_SetVertexData4f(
				global_d3d_device,
				0,
				bounds.x0,
				bounds.y1,
				projected_center.z,
				1.0f);
			IDirect3DDevice8_End(global_d3d_device);

			success = TRUE;
			rasterizer_sun_glow_copy_source(_rasterizer_target_sun_glow_primary, &screen_bounds);
			rasterizer_sun_glow_copy_source(_rasterizer_target_sun_glow_secondary, &screen_bounds);
			blurred_target = rasterizer_sun_glow_convolve(
				_rasterizer_target_sun_glow_primary,
				_rasterizer_target_sun_glow_secondary,
				SUN_GLOW_CONVOLVE_PASS_COUNT);

			rasterizer_set_vertex_shader_permutation(0x38, 6, FALSE);
			rasterizer_set_target_as_texture(0, blurred_target, FALSE);
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
			SetRenderStateSmart(
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_RED |
				D3DCOLORWRITEENABLE_GREEN |
				D3DCOLORWRITEENABLE_BLUE);
			SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
			SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_ONE);
			SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			SetRenderStateSmart(D3DRS_ALPHATESTENABLE, FALSE);
			SetRenderStateSmart(D3DRS_ZENABLE, FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				0);

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = 1;
			pixel_shader.combiner_count = 2;
			pixel_shader.constant_0[0] = 0xB0B080;
			pixel_shader.constant_1[0] = 0xFFFFFF;
			pixel_shader.alpha_inputs[0] = 0x48200000;
			pixel_shader.alpha_outputs[0] = 0xC0;
			pixel_shader.rgb_inputs[1] = 0x3C011C02;
			pixel_shader.rgb_outputs[1] = 0xC00;
			pixel_shader.final_combiner_inputs_abcd = 0xC080000;
			pixel_shader.final_combiner_inputs_efg = 0x1400;
			rasterizer_set_pixel_shader(&pixel_shader);

			for (pass = 0; pass < SUN_GLOW_RAY_COUNT; pass++)
			{
				r = ((real)pass * 0.0625f) * 80.0f - 4.0f;

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

				if (IDirect3DDevice8_SetVertexData4f(
					global_d3d_device,
					9,
					0.0f,
					0.0f,
					0.0f,
					brightness / (real)(pass + 1)) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SetVertexData4f(global_d3d_device, 9, 0.0f, 0.0f, 0.0f, brightness/(real)(pass + 1))");
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
					bounds.x0 - r,
					bounds.y0 - r) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, bounds.x0 - r, bounds.y0 - r)");
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
					bounds.x1 + r,
					bounds.y0 - r) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, bounds.x1 + r, bounds.y0 - r)");
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
					bounds.x1 + r,
					bounds.y1 + r) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, bounds.x1 + r, bounds.y1 + r)");
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
					bounds.x0 - r,
					bounds.y1 + r) >= 0 && success)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SetVertexData2f(global_d3d_device, VSDE_VERTEX, bounds.x0 - r, bounds.y1 + r)");
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

			if (!success)
				error(2, "### ERROR rasterizer_sun_glow_draw failed");
		}
	}

	return;
}

/* ---------- private code */

static boolean rasterizer_project_billboard(
	real_point3d const *point,
	real radius,
	real_point3d *projected_center,
	real_vector2d *projected_axes)
{
	real_point3d view_point;
	real clip_y;
	real clip_z;
	real projected_radius_x;
	real projected_radius_y;
	real inverse_w;
	short viewport_width;
	short viewport_height;
	boolean projected = FALSE;

	if (radius > 0.0f)
	{
		viewport_width = global_window_parameters.camera.viewport_bounds.x1 -
			global_window_parameters.camera.viewport_bounds.x0;
		viewport_height = global_window_parameters.camera.viewport_bounds.y1 -
			global_window_parameters.camera.viewport_bounds.y0;
		matrix4x3_transform_point(
			&global_window_parameters.frustum.world_to_view,
			point,
			&view_point);

		clip_y =
			global_window_parameters.frustum.projection_matrix[0][1] * view_point.x +
			global_window_parameters.frustum.projection_matrix[1][1] * view_point.y +
			global_window_parameters.frustum.projection_matrix[2][1] * view_point.z +
			global_window_parameters.frustum.projection_matrix[3][1];
		clip_z =
			global_window_parameters.frustum.projection_matrix[0][2] * view_point.x +
			global_window_parameters.frustum.projection_matrix[1][2] * view_point.y +
			global_window_parameters.frustum.projection_matrix[2][2] * view_point.z +
			global_window_parameters.frustum.projection_matrix[3][2];
		projected_radius_x = global_window_parameters.frustum.projection_matrix[0][0] * radius;
		projected_radius_y = global_window_parameters.frustum.projection_matrix[1][1] * radius;
		if (clip_z > 0.0f)
		{
			inverse_w = 1.0f / (
				global_window_parameters.frustum.projection_matrix[0][3] * view_point.x +
				global_window_parameters.frustum.projection_matrix[1][3] * view_point.y +
				global_window_parameters.frustum.projection_matrix[2][3] * view_point.z +
				global_window_parameters.frustum.projection_matrix[3][3]);
			projected_center->x = (((
				global_window_parameters.frustum.projection_matrix[0][0] * view_point.x +
				global_window_parameters.frustum.projection_matrix[1][0] * view_point.y +
				global_window_parameters.frustum.projection_matrix[2][0] * view_point.z +
				global_window_parameters.frustum.projection_matrix[3][0]) * inverse_w + 1.0f) *
				viewport_width - 1.0f) * 0.5f;
			projected_center->y = ((1.0f - clip_y * inverse_w) * viewport_height - 1.0f) * 0.5f;
			projected_center->z = MIN(1.0f, clip_z * inverse_w);
			projected_axes->i = viewport_width * inverse_w * projected_radius_x * 0.5f;
			projected_axes->j = viewport_height * inverse_w * projected_radius_y * 0.5f;
			projected = TRUE;
		}
	}

	return projected;
}

static void rasterizer_sun_glow_copy_source(
	short target,
	real_rectangle2d const *bounds)
{
	real vertex_constants[8][4];
	real scale = 1.0f / 64.0f;
	real mysterious_horizontal_offset = -1.0f / 32.0f;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		323,
		bounds);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		324,
		global_d3d_device);

	rasterizer_set_target_as_texture(0, 0, FALSE);
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
		D3DTEXF_POINT);

	rasterizer_set_texture_direct(1, global_rasterizer_data->glow.index, 0);
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
		D3DCULL_CCW);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALL);
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

	rasterizer_set_vertex_shader_permutation(0x26, 8, FALSE);

	vertex_constants[0][0] = bounds->x1 - bounds->x0;
	vertex_constants[0][1] = 0.0f;
	vertex_constants[0][2] = 0.0f;
	vertex_constants[0][3] = bounds->x0;
	vertex_constants[1][0] = 0.0f;
	vertex_constants[1][1] = bounds->y1 - bounds->y0;
	vertex_constants[1][2] = 0.0f;
	vertex_constants[1][3] = bounds->y0;
	vertex_constants[2][0] = 1.0f;
	vertex_constants[2][1] = 0.0f;
	vertex_constants[2][2] = 0.0f;
	vertex_constants[2][3] = 0.0f;
	vertex_constants[3][0] = 0.0f;
	vertex_constants[3][1] = 1.0f;
	vertex_constants[3][2] = 0.0f;
	vertex_constants[3][3] = 0.0f;
	vertex_constants[4][0] = 1.0f;
	vertex_constants[4][1] = 0.0f;
	vertex_constants[4][2] = 0.0f;
	vertex_constants[4][3] = 0.0f;
	vertex_constants[5][0] = 0.0f;
	vertex_constants[5][1] = 1.0f;
	vertex_constants[5][2] = 0.0f;
	vertex_constants[5][3] = 0.0f;
	vertex_constants[6][0] = 1.0f;
	vertex_constants[6][1] = 0.0f;
	vertex_constants[6][2] = 0.0f;
	vertex_constants[6][3] = 0.0f;
	vertex_constants[7][0] = 0.0f;
	vertex_constants[7][1] = 1.0f;
	vertex_constants[7][2] = 0.0f;
	vertex_constants[7][3] = 0.0f;
#ifdef HALO_LINUX
	/* the frame's texels, which the console's widescreen buffer squeezes */
	vertex_constants[0][0] *= halo_screen_texture_scale_x();
	vertex_constants[0][3] *= halo_screen_texture_scale_x();
#endif
	IDirect3DDevice8_SetVertexShaderConstant(
		global_d3d_device,
		-81,
		vertex_constants,
		8);

	csmemset(&pixel_shader, 0, sizeof(pixel_shader));
	pixel_shader.texture_modes = 0x21;
	pixel_shader.combiner_count = 1;
	pixel_shader.constant_0[0] = 0xC0000000;
	pixel_shader.alpha_inputs[0] = 0x19110000;
	pixel_shader.alpha_outputs[0] = 0x100C0;
	pixel_shader.final_combiner_inputs_abcd = 0x18;
	pixel_shader.final_combiner_inputs_efg = 0x1C00;
	rasterizer_set_pixel_shader(&pixel_shader);

	rasterizer_set_target(target, FALSE, FALSE, FALSE, FALSE);

	IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
	IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 0);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		0,
		scale - 1.0f + mysterious_horizontal_offset,
		scale + 1.0f);
	IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 0);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		0,
		scale + 1.0f + mysterious_horizontal_offset,
		scale + 1.0f);
	IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 1, 1);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		0,
		scale + 1.0f + mysterious_horizontal_offset,
		scale - 1.0f);
	IDirect3DDevice8_SetVertexData2s(global_d3d_device, 4, 0, 1);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		0,
		scale - 1.0f + mysterious_horizontal_offset,
		scale - 1.0f);
	IDirect3DDevice8_End(global_d3d_device);

	rasterizer_set_target(
		global_window_parameters.rasterizer_target,
		FALSE,
		FALSE,
		FALSE,
		TRUE);

	return;
}

static short rasterizer_sun_glow_convolve(
	short primary_target,
	short secondary_target,
	short pass_count)
{
	real vertex_constants[8][4];
	short source_target;
	short destination_target;
	short pass;
	short stage;
	boolean success;
	real scale = 1.0f / 64.0f;
	real mysterious_horizontal_offset = -1.0f / 32.0f;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		427,
		primary_target>=0 && primary_target<NUMBER_OF_RASTERIZER_TARGETS);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		428,
		secondary_target>=0 && secondary_target<NUMBER_OF_RASTERIZER_TARGETS);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_lights.c",
		429,
		global_d3d_device);

	if (pass_count > 0)
	{
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
			TRUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_SRCBLEND,
			D3DBLEND_DESTALPHA);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_DESTBLEND,
			D3DBLEND_ZERO);
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

		rasterizer_set_vertex_shader_permutation(0x26, 8, FALSE);

		vertex_constants[0][0] = 1.0f;
		vertex_constants[0][1] = 0.0f;
		vertex_constants[0][2] = 0.0f;
		vertex_constants[0][3] = -0.0078125f;
		vertex_constants[1][0] = 0.0f;
		vertex_constants[1][1] = 1.0f;
		vertex_constants[1][2] = 0.0f;
		vertex_constants[1][3] = -0.0078125f;
		vertex_constants[2][0] = 1.0f;
		vertex_constants[2][1] = 0.0f;
		vertex_constants[2][2] = 0.0f;
		vertex_constants[2][3] = 0.0078125f;
		vertex_constants[3][0] = 0.0f;
		vertex_constants[3][1] = 1.0f;
		vertex_constants[3][2] = 0.0f;
		vertex_constants[3][3] = 0.0078125f;
		vertex_constants[4][0] = 1.0f;
		vertex_constants[4][1] = 0.0f;
		vertex_constants[4][2] = 0.0f;
		vertex_constants[4][3] = -0.0078125f;
		vertex_constants[5][0] = 0.0f;
		vertex_constants[5][1] = 1.0f;
		vertex_constants[5][2] = 0.0f;
		vertex_constants[5][3] = 0.0078125f;
		vertex_constants[6][0] = 1.0f;
		vertex_constants[6][1] = 0.0f;
		vertex_constants[6][2] = 0.0f;
		vertex_constants[6][3] = 0.0078125f;
		vertex_constants[7][0] = 0.0f;
		vertex_constants[7][1] = 1.0f;
		vertex_constants[7][2] = 0.0f;
		vertex_constants[7][3] = -0.0078125f;
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-81,
			vertex_constants,
			8);

		success = TRUE;

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 0x8421;
		pixel_shader.combiner_count = 2;
		pixel_shader.constant_0[0] = 0xFF000000;
		pixel_shader.alpha_inputs[0] = 0x8A009A0;
		pixel_shader.alpha_outputs[0] = 0xC00;
		pixel_shader.rgb_inputs[0] = 0xAA00BA0;
		pixel_shader.rgb_outputs[0] = 0xC00;
		pixel_shader.rgb_inputs[1] = 0x1C110C11;
		pixel_shader.rgb_outputs[1] = 0xC00;
		pixel_shader.final_combiner_inputs_abcd = 0xC;
		rasterizer_set_pixel_shader(&pixel_shader);

		for (pass = 0; pass < pass_count; pass++)
		{
			source_target = (pass & 1) ? secondary_target : primary_target;
			destination_target = (pass & 1) ? primary_target : secondary_target;

			for (stage = 0; stage < 4; stage++)
			{
				rasterizer_set_target_as_texture(stage, source_target, FALSE);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					stage,
					D3DTSS_ADDRESSU,
					D3DTADDRESS_BORDER);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					stage,
					D3DTSS_ADDRESSV,
					D3DTADDRESS_BORDER);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					stage,
					D3DTSS_MAGFILTER,
					D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					stage,
					D3DTSS_MINFILTER,
					D3DTEXF_LINEAR);
				IDirect3DDevice8_SetTextureStageState(
					global_d3d_device,
					stage,
					D3DTSS_MIPFILTER,
					D3DTEXF_POINT);
			}

			rasterizer_set_target(destination_target, FALSE, FALSE, FALSE, FALSE);

			pixel_shader.constant_0[0] = (pass > 0 ? 0x7F : 0xFF) << 24;
			rasterizer_set_pixel_shader(&pixel_shader);

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
			FALSE,
			FALSE,
			FALSE,
			TRUE);

		if (!success)
			error(2, "### ERROR rasterizer_sun_glow_convolve failed");
	}

	return (pass_count & 1) ? secondary_target : primary_target;
}
