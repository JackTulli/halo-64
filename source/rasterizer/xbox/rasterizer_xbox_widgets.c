/*
RASTERIZER_XBOX_WIDGETS.C

symbols in this file:
00169EF0 01b0:
	_D3DDevice_SetRenderState (0000)
0016A0A0 0050:
	_D3DDevice_SetTextureStageState (0000)
0016A0F0 01c0:
	_rasterizer_widget_project_billboard (0000)
0016A2B0 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
0016A4D0 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
0016A530 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
0016A540 0010:
	_IDirect3DDevice8_BeginVisibilityTest@4 (0000)
0016A550 0010:
	_IDirect3DDevice8_EndVisibilityTest@8 (0000)
0016A560 0010:
	_IDirect3DDevice8_GetVisibilityTestResult@16 (0000)
0016A570 0020:
	_IDirect3DDevice8_SetVertexData2f@16 (0000)
0016A590 0030:
	_IDirect3DDevice8_SetVertexData4f@24 (0000)
0016A5C0 0010:
	_IDirect3DDevice8_SetVertexData2s@16 (0000)
0016A5D0 0010:
	_IDirect3DDevice8_SetVertexDataColor@12 (0000)
0016A5E0 0010:
	_IDirect3DDevice8_Begin@8 (0000)
0016A5F0 0010:
	_IDirect3DDevice8_End@4 (0000)
0016A600 0010:
	__rasterizer_widget_end (0000)
0016A610 00d0:
	__rasterizer_widget_get_occlusion_test_result (0000)
0016A6E0 0170:
	__rasterizer_widget_submit (0000)
0016A850 0480:
	__rasterizer_widget_begin (0000)
0016ACD0 00c0:
	__rasterizer_widget_set_texture (0000)
0016AD90 0040:
	__rasterizer_widget_set_tint_factor (0000)
0016ADD0 0040:
	__rasterizer_widget_set_zbuffer_enable (0000)
0016AE10 0210:
	__rasterizer_widget_draw_sprite2d (0000)
0016B020 0240:
	__rasterizer_widget_draw_sprite3d (0000)
0016B260 0280:
	__rasterizer_widget_submit_occlusion_test (0000)
0029CCB8 003d:
	??_C@_0DN@BOAOHHMH@?$CD?$CD?$CD?5ERROR?5rasterizer_widget_get_@ (0000)
0029CCF8 0026:
	??_C@_0CG@BMNNONEP@?$CIocclusion_test_result?$CG0x8000000@ (0000)
0029CD20 0039:
	??_C@_0DJ@CJFIOLEN@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0029CD5C 0022:
	??_C@_0CC@GCBGNNKG@?$CD?$CD?$CD?5ERROR?5unsupported?5widget?5typ@ (0000)
0029CD80 0045:
	??_C@_0EF@KIEIHHAH@fabs?$CIcos_theta?$CKcos_theta?5?$CL?5sin_t@ (0000)
0029CDC8 0039:
	??_C@_0DJ@DLLMHNIO@?$CD?$CD?$CD?5ERROR?5rasterizer_widget_subm@ (0000)
0029CE04 0004:
	__real@c6fffe00 (0000)
004662EA 0001:
	_warned_about_too_many_transparent_geometry_groups (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game_globals.h"
#include "math/real_math.h"
#include "rasterizer/common/rasterizer_common.h"
#include "rasterizer/rasterizer_geometry.h"
#include "rasterizer/rasterizer_transparent_geometry.h"

#include <xtl.h>
#include "rasterizer/xbox/rasterizer_xbox.h"
#include "rasterizer/rasterizer_widgets.h"
#include "rasterizer/rasterizer.h"

/* ---------- constants */

enum widget_type
{
	_widget_type_internal_sprite = 5,
	_widget_type_internal_occlusion_test,
	NUMBER_OF_INTERNAL_WIDGET_TYPES
};

enum widget_flags
{
	_widget_zbuffer_enable_bit = 0,
	_widget_zbuffer_write_enable_bit
};

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;
struct shader;
struct vertex_buffer;

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader const *shader;
	short shader_permutation_index;
	short pad12;
	short effect_type;
	byte reserved16[0x26];
	real_vector2d model_base_map_scale;
	long dynamic_triangle_buffer_index;
	void (*render_proc)(
		long object_index,
		long widget_index);
	long first_triangle_index;
	long triangle_count;
	long dynamic_vertex_buffer_index;
	struct vertex_buffer const *vertex_buffers;
	struct bitmap_data const *lightmap;
	void const *node_matrices;
	short node_matrix_count;
	word pad66;
	void const *lighting;
	void const *animation;
	real z_sort;
	real_point3d centroid;
	real_plane3d plane;
	long sorted_index;
	short previous_group_presorted_index;
	short next_group_presorted_index;
	long active_camouflage_transparent_source_object_index;
	byte reserved9C;
	boolean cortana_hack;
	byte reserved9E[2];
};

typedef char transparent_geometry_group_size_assert[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];


struct rasterizer_debug_options
{
	byte reserved00[0x44];
	boolean lens_flare_occlusion_enabled;
	boolean lens_flare_occlusion_debug;
	byte reserved46[0xE];
	unsigned long zbias;
	byte reserved58[0x10];
};

typedef char rasterizer_debug_options_size_assert[
	sizeof(struct rasterizer_debug_options) == 0x68 ? 1 : -1];
typedef char rasterizer_debug_options_zbias_offset_assert[
	offsetof(struct rasterizer_debug_options, zbias) == 0x54 ? 1 : -1];

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

static boolean rasterizer_widget_project_billboard(
	real_point3d const *point,
	real radius,
	real_point3d *projected_center,
	real_vector2d *projected_axes);

/* ---------- globals */

extern D3DDevice *global_d3d_device;
extern struct rasterizer_debug_options rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;
extern struct pixel_shader_definition pixel_shader;

static boolean warned_about_too_many_transparent_geometry_groups;

/* ---------- public code */

void _rasterizer_widget_end(
	void)
{
	return;
}

long _rasterizer_widget_get_occlusion_test_result(
	long occlusion_test_index)
{
	long occlusion_test_result = NONE;

	if (rasterizer_debug_options.lens_flare_occlusion_enabled)
	{
		boolean success;
		HRESULT result;
		ULONGLONG timestamp;

		result = IDirect3DDevice8_GetVisibilityTestResult(
			global_d3d_device,
			occlusion_test_index,
			(UINT *)&occlusion_test_result,
			&timestamp);
		if (result == D3DERR_TESTINCOMPLETE)
		{
			rasterizer_spin_begin(_rasterizer_profile_screen_effect);
			do
			{
				result = IDirect3DDevice8_GetVisibilityTestResult(
					global_d3d_device,
					occlusion_test_index,
					(UINT *)&occlusion_test_result,
					&timestamp);
			}
			while (result == D3DERR_TESTINCOMPLETE);
			rasterizer_spin_end();
		}

		if (result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"hr");
		}

		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
			574,
			(occlusion_test_result&0x80000000)==0);
		if (!success)
			error(
				_error_silent,
				"### ERROR rasterizer_widget_get_occlusion_test_result failed");
	}
	else
	{
		return 1;
	}

	return occlusion_test_result;
}

void _rasterizer_widget_submit(
	long object_index,
	long widget_index,
	real_point3d const *centroid,
	void (*render_proc)(
		long object_index,
		long widget_index))
{
	real_vector3d camera_to_centroid;
	real_plane3d null_plane;
	struct transparent_geometry_group *group;

	if (render_proc)
	{
		group = rasterizer_transparent_geometry_new_group();
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
			88,
			centroid);
		if (group)
		{
			camera_to_centroid.i = centroid->x - global_window_parameters.camera.position.x;
			camera_to_centroid.j = centroid->y - global_window_parameters.camera.position.y;
			camera_to_centroid.k = centroid->z - global_window_parameters.camera.position.z;
			group->geometry_flags = 0;
			group->object_index = 0;
			group->source_object_index = 0;
			group->shader = NULL;
			group->shader_permutation_index = 0;
			group->effect_type = 0;
			group->dynamic_triangle_buffer_index = NONE;
			group->render_proc = render_proc;
			group->first_triangle_index = object_index;
			group->triangle_count = widget_index;
			group->dynamic_vertex_buffer_index = NONE;
			group->vertex_buffers = NULL;
			group->lightmap = NULL;
			null_plane.n.i = 0.0f;
			null_plane.n.j = 0.0f;
			null_plane.n.k = 0.0f;
			null_plane.d = 0.0f;
			group->z_sort = -(
				global_window_parameters.camera.forward.i * camera_to_centroid.i +
				global_window_parameters.camera.forward.j * camera_to_centroid.j +
				global_window_parameters.camera.forward.k * camera_to_centroid.k);
			group->centroid = *centroid;
			group->plane = null_plane;
			group->model_base_map_scale.i = group->model_base_map_scale.j = 1.0f;
			group->next_group_presorted_index = group->previous_group_presorted_index = NONE;
			group->active_camouflage_transparent_source_object_index = 0;
			group->cortana_hack = FALSE;
			group->node_matrices = NULL;
			group->node_matrix_count = 0;
			group->lighting = NULL;
			group->animation = NULL;
		}
		else if (!warned_about_too_many_transparent_geometry_groups)
		{
			error(_error_silent, "### ERROR too many transparent geometry groups");
			warned_about_too_many_transparent_geometry_groups = TRUE;
		}
	}

	return;
}

void _rasterizer_widget_begin(
	short type,
	word flags)
{
	real vertex_constants[5][4];
	short viewport_width;
	short viewport_height;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		154,
		global_d3d_device);

	switch (type)
	{
		case _widget_type_internal_occlusion_test:
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				D3DCULL_CCW);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_COLORWRITEENABLE,
				rasterizer_debug_options.lens_flare_occlusion_debug ?
					D3DCOLORWRITEENABLE_RED |
					D3DCOLORWRITEENABLE_GREEN |
					D3DCOLORWRITEENABLE_BLUE :
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
				rasterizer_debug_options.lens_flare_occlusion_debug);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				rasterizer_debug_options.zbias);
			rasterizer_set_vertex_shader_permutation(
				0x38,
				_rasterizer_vertex_type_dynamic_unlit,
				0);
			viewport_width = global_window_parameters.camera.viewport_bounds.x1 -
				global_window_parameters.camera.viewport_bounds.x0;
			viewport_height = global_window_parameters.camera.viewport_bounds.y1 -
				global_window_parameters.camera.viewport_bounds.y0;
			vertex_constants[0][0] = 1.0f / viewport_width * 2.0f;
			vertex_constants[0][1] = 0.0f;
			vertex_constants[0][2] = 0.0f;
			vertex_constants[0][3] = -1.0f - 1.0f / viewport_width;
			vertex_constants[1][0] = 0.0f;
			vertex_constants[1][1] = 1.0f / viewport_height * -2.0f;
			vertex_constants[1][2] = 0.0f;
			vertex_constants[1][3] = 1.0f / viewport_height + 1.0f;
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
			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.combiner_count = PS_COMBINERCOUNT(1, 0);
			pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO,
				PS_REGISTER_ONE);
			rasterizer_set_pixel_shader(&pixel_shader);
			break;

		case _widget_type_internal_sprite:
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
				D3DBLEND_SRCALPHA);
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
				TEST_FLAG(flags, _widget_zbuffer_enable_bit));
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZFUNC,
				D3DCMP_LESSEQUAL);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				TEST_FLAG(flags, _widget_zbuffer_write_enable_bit));
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				0);
			rasterizer_widget_set_tint_factor(1.0f);
			rasterizer_set_vertex_shader_permutation(
				0x38,
				_rasterizer_vertex_type_dynamic_unlit,
				0);
			viewport_width = global_window_parameters.camera.viewport_bounds.x1 -
				global_window_parameters.camera.viewport_bounds.x0;
			viewport_height = global_window_parameters.camera.viewport_bounds.y1 -
				global_window_parameters.camera.viewport_bounds.y0;
			vertex_constants[0][0] = 1.0f / viewport_width * 2.0f;
			vertex_constants[0][1] = 0.0f;
			vertex_constants[0][2] = 0.0f;
			vertex_constants[0][3] = -1.0f - 1.0f / viewport_width;
			vertex_constants[1][0] = 0.0f;
			vertex_constants[1][1] = 1.0f / viewport_height * -2.0f;
			vertex_constants[1][2] = 0.0f;
			vertex_constants[1][3] = 1.0f / viewport_height + 1.0f;
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
			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = PS_TEXTUREMODES(
				PS_TEXTUREMODES_PROJECT2D,
				PS_TEXTUREMODES_NONE,
				PS_TEXTUREMODES_NONE,
				PS_TEXTUREMODES_NONE);
			pixel_shader.combiner_count = PS_COMBINERCOUNT(3, 0);
			pixel_shader.rgb_inputs[0] = PS_COMBINERINPUTS(
				PS_REGISTER_T0,
				PS_REGISTER_T0,
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO);
			pixel_shader.rgb_outputs[0] = PS_COMBINEROUTPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_DISCARD,
				PS_REGISTER_DISCARD,
				PS_COMBINEROUTPUT_IDENTITY);
			pixel_shader.rgb_inputs[1] = PS_COMBINERINPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_R0,
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO);
			pixel_shader.rgb_outputs[1] = PS_COMBINEROUTPUTS(
				PS_REGISTER_R1,
				PS_REGISTER_DISCARD,
				PS_REGISTER_DISCARD,
				PS_COMBINEROUTPUT_IDENTITY);
			pixel_shader.rgb_inputs[2] = PS_COMBINERINPUTS(
				PS_REGISTER_V0,
				PS_REGISTER_T0,
				PS_REGISTER_V0 | PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_V1 | PS_CHANNEL_ALPHA);
			pixel_shader.rgb_outputs[2] = PS_COMBINEROUTPUTS(
				PS_REGISTER_V0,
				PS_REGISTER_V1,
				PS_REGISTER_DISCARD,
				PS_COMBINEROUTPUT_IDENTITY);
			pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
				PS_REGISTER_V1,
				PS_REGISTER_EF_PROD,
				PS_REGISTER_ZERO,
				PS_REGISTER_V0);
			pixel_shader.final_combiner_inputs_efg = PS_COMBINERINPUTS(
				PS_REGISTER_R0,
				PS_REGISTER_R1,
				PS_REGISTER_V0 | PS_CHANNEL_ALPHA,
				0);
			rasterizer_set_pixel_shader(&pixel_shader);
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
				266,
				FALSE,
				"### ERROR unsupported widget type");
			break;
	}

	return;
}

boolean _rasterizer_widget_set_texture(
	short stage_index,
	long bitmap_group_index,
	short sequence_index)
{
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		285,
		global_d3d_device);
	if (bitmap_group_index != NONE)
		success = rasterizer_set_texture_non_blocking(
			stage_index,
			0,
			1,
			bitmap_group_index,
			sequence_index);
	else
		success = rasterizer_set_texture_direct_non_blocking(
			stage_index,
			global_rasterizer_data->glow.index,
			sequence_index);

	if (!success)
	{
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
			D3DTEXF_LINEAR);
	}

	return success;
}

void _rasterizer_widget_set_tint_factor(
	real tint_factor)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		318,
		global_d3d_device);
	IDirect3DDevice8_SetVertexData2f(
		global_d3d_device,
		10,
		tint_factor,
		0.0f);

	return;
}

void _rasterizer_widget_set_zbuffer_enable(
	boolean zbuffer_enable)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		334,
		global_d3d_device);
	D3DDevice_SetRenderState_ZEnable(zbuffer_enable);

	return;
}

void _rasterizer_widget_draw_sprite2d(
	real_point2d const *point,
	real radius,
	real_vector2d const *scale,
	real_vector2d const *texture_scale,
	real rotation,
	unsigned long color)
{
	real cos_theta;
	real sin_theta;
	real axis_x;
	real axis_y;
	real scale_x;
	real scale_y;
	long texture_u;
	long texture_v;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		356,
		point);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		357,
		global_d3d_device);

	if (radius > 0.0f)
	{
		if (rotation != 0.0f)
		{
			cos_theta = (real)cos(rotation);
			sin_theta = (real)sin(rotation);
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
				369,
				fabs(cos_theta*cos_theta + sin_theta*sin_theta - 1.0f)<_real_epsilon);
			axis_x = cos_theta - sin_theta;
			axis_y = sin_theta + cos_theta;
		}
		else
		{
			axis_x = axis_y = 1.0f;
		}

		if (scale)
		{
			scale_x = scale->i;
			scale_y = scale->j;
		}
		else
		{
			scale_x = scale_y = 1.0f;
		}

		texture_u = texture_scale ? (long)texture_scale->i : 1;
		texture_v = texture_scale ? (long)texture_scale->j : 1;

		IDirect3DDevice8_Begin(
			global_d3d_device,
			D3DPT_TRIANGLEFAN);
		IDirect3DDevice8_SetVertexDataColor(
			global_d3d_device,
			9,
			color);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			0,
			0);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			point->x - scale_x * axis_x,
			point->y - scale_y * axis_y);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			texture_u,
			0);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			point->x + scale_x * axis_y,
			point->y - scale_y * axis_x);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			texture_u,
			texture_v);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			point->x + scale_x * axis_x,
			point->y + scale_y * axis_y);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			0,
			texture_v);
		IDirect3DDevice8_SetVertexData2f(
			global_d3d_device,
			0,
			point->x - scale_x * axis_y,
			point->y + scale_y * axis_x);
		IDirect3DDevice8_End(global_d3d_device);
	}

	return;
}

void _rasterizer_widget_draw_sprite3d(
	real_point3d const *point,
	real radius,
	real_vector2d const *scale,
	real rotation,
	unsigned long color)
{
	real_point3d projected_center;
	real_vector2d projected_axes;
	real cos_theta;
	real sin_theta;
	real axis_x;
	real axis_y;
	real scale_x;
	real scale_y;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		425,
		point);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
		426,
		global_d3d_device);

	if (radius > 0.0f &&
		rasterizer_widget_project_billboard(
			point,
			radius,
			&projected_center,
			&projected_axes))
	{
		if (rotation != 0.0f)
		{
			cos_theta = (real)cos(DEGREES_TO_RADIANS(rotation));
			sin_theta = (real)sin(DEGREES_TO_RADIANS(rotation));
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_widgets.c",
				437,
				fabs(cos_theta*cos_theta + sin_theta*sin_theta - 1.0f)<_real_epsilon);
			axis_x = projected_axes.i * cos_theta - projected_axes.j * sin_theta;
			axis_y = projected_axes.i * sin_theta + projected_axes.j * cos_theta;
		}
		else
		{
			axis_x = projected_axes.i;
			axis_y = projected_axes.j;
		}

		if (scale)
		{
			scale_x = scale->i;
			scale_y = scale->j;
		}
		else
		{
			scale_x = scale_y = 1.0f;
		}

		IDirect3DDevice8_Begin(
			global_d3d_device,
			D3DPT_TRIANGLEFAN);
		IDirect3DDevice8_SetVertexDataColor(
			global_d3d_device,
			9,
			color);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			0,
			0);
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			0,
			projected_center.x - scale_x * axis_x,
			projected_center.y - scale_y * axis_y,
			projected_center.z,
			1.0f);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			1,
			0);
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			0,
			projected_center.x + scale_x * axis_y,
			projected_center.y - scale_y * axis_x,
			projected_center.z,
			1.0f);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			1,
			1);
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			0,
			projected_center.x + scale_x * axis_x,
			projected_center.y + scale_y * axis_y,
			projected_center.z,
			1.0f);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			0,
			1);
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			0,
			projected_center.x - scale_x * axis_y,
			projected_center.y + scale_y * axis_x,
			projected_center.z,
			1.0f);
		IDirect3DDevice8_End(global_d3d_device);
	}

	return;
}

long _rasterizer_widget_submit_occlusion_test(
	real_point3d const *point,
	real radius,
	long index)
{
	real_point3d projected_center;
	real_vector2d projected_axes;
	short x0;
	short y0;
	short x1;
	short y1;
	long pixel_count;
	HRESULT result;
	boolean success = TRUE;

	if (rasterizer_debug_options.lens_flare_occlusion_enabled)
	{
		if (rasterizer_widget_project_billboard(
			point,
			radius,
			&projected_center,
			&projected_axes))
		{
			projected_axes.i = MAX(1.0f, projected_axes.i);
			projected_axes.j = MAX(1.0f, projected_axes.j);

			x0 = (short)fast_ftol((real)floor(PIN(
				projected_center.x - projected_axes.i,
				-32767.0f,
				32767.0f)));
			y0 = (short)fast_ftol((real)floor(PIN(
				projected_center.y - projected_axes.j,
				-32767.0f,
				32767.0f)));
			x1 = (short)fast_ftol((real)floor(PIN(
				projected_center.x + projected_axes.i,
				-32767.0f,
				32767.0f)));
			y1 = (short)fast_ftol((real)floor(PIN(
				projected_center.y + projected_axes.j,
				-32767.0f,
				32767.0f)));

			pixel_count = MAX(0, (x1 - x0) * (y1 - y0));
			if (pixel_count > 0)
			{
				IDirect3DDevice8_BeginVisibilityTest(global_d3d_device);
				IDirect3DDevice8_Begin(
					global_d3d_device,
					D3DPT_TRIANGLEFAN);
				IDirect3DDevice8_SetVertexData4f(
					global_d3d_device,
					0,
					(real)x0,
					(real)y0,
					projected_center.z,
					1.0f);
				IDirect3DDevice8_SetVertexData4f(
					global_d3d_device,
					0,
					(real)x1,
					(real)y0,
					projected_center.z,
					1.0f);
				IDirect3DDevice8_SetVertexData4f(
					global_d3d_device,
					0,
					(real)x1,
					(real)y1,
					projected_center.z,
					1.0f);
				IDirect3DDevice8_SetVertexData4f(
					global_d3d_device,
					0,
					(real)x0,
					(real)y1,
					projected_center.z,
					1.0f);
				IDirect3DDevice8_End(global_d3d_device);

				result = IDirect3DDevice8_EndVisibilityTest(
					global_d3d_device,
					index);
				if (result < 0)
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_EndVisibilityTest(global_d3d_device, index)");
				}
			}
		}
		else
		{
			pixel_count = 0;
		}
	}
	else
	{
		pixel_count = 1;
	}

	if (!success)
		error(
			_error_silent,
			"### ERROR rasterizer_widget_submit_occlusion_test failed");

	return pixel_count;
}

/* ---------- private code */

static boolean rasterizer_widget_project_billboard(
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
		projected_radius_x =
			global_window_parameters.frustum.projection_matrix[0][0] * radius;
		projected_radius_y =
			global_window_parameters.frustum.projection_matrix[1][1] * radius;
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
				global_window_parameters.frustum.projection_matrix[3][0]) *
				inverse_w + 1.0f) * viewport_width - 1.0f) * 0.5f;
			projected_center->y =
				((1.0f - clip_y * inverse_w) * viewport_height - 1.0f) * 0.5f;
			projected_center->z = MIN(1.0f, clip_z * inverse_w);
			projected_axes->i =
				viewport_width * inverse_w * projected_radius_x * 0.5f;
			projected_axes->j =
				viewport_height * inverse_w * projected_radius_y * 0.5f;
			projected = TRUE;
		}
	}

	return projected;
}
