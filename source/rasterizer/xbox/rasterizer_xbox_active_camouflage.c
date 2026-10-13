/*
RASTERIZER_XBOX_ACTIVE_CAMOUFLAGE.C

symbols in this file:
001488C0 0070:
	_real_alpha_to_pixel32 (0000)
00148930 01b0:
	_D3DDevice_SetRenderState (0000)
00148AE0 0050:
	_D3DDevice_SetTextureStageState (0000)
00148B30 0020:
	_rasterizer_active_camouflage_set_visibility (0000)
00148B50 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
00148D70 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
00148DD0 0010:
	_IDirect3DDevice8_SetVertexShaderConstant@16 (0000)
00148DE0 0010:
	_IDirect3DDevice8_SetVertexData2s@16 (0000)
00148DF0 0010:
	_IDirect3DDevice8_Begin@8 (0000)
00148E00 0010:
	_IDirect3DDevice8_End@4 (0000)
00148E10 0340:
	_rasterizer_active_camouflage_cache_primary_render_target (0000)
00149150 07e0:
	_rasterizer_active_camouflage_draw (0000)
0028DA50 004e:
	??_C@_0EO@HDJOIPE@global_window_parameters?4rasteri@ (0000)
0028DAA0 0043:
	??_C@_0ED@HJIAGIIM@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0028DAE8 0045:
	??_C@_0EF@JPIBDDAJ@?$CBTEST_FLAG?$CIgroup?9?$DOgeometry_flags@ (0000)
0028DB30 002a:
	??_C@_0CK@KJCGLLHP@local_active_camouflage_debug_ca@ (0000)
0028DB5C 001e:
	??_C@_0BO@PEAJKHII@group?9?$DOeffect?4intensity?$DM?$DN1?40f?$AA@ (0000)
0028DB7C 001d:
	??_C@_0BN@DOLIELKA@group?9?$DOeffect?4intensity?$DO0?40f?$AA@ (0000)
0028DBA0 0040:
	??_C@_0EA@OILAHNPH@group?9?$DOeffect?4type?$DN?$DN_render_mode@ (0000)
0028DBE0 000e:
	??_C@_0O@KHNLKNGC@group?9?$DOshader?$AA@ (0000)
0045E8E0 0006:
	_local_active_camouflage_visibility_flag (0000)
	_local_active_camouflage_debug_cached_flag (0001)
	_local_active_camouflage_debug_cache_count (0004)
*/

/* ---------- headers */

#include "cseries.h"
#include "bitmaps/bitmaps_inlines.h"
#include "game/game_globals.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_active_camouflage.h"
#include "rasterizer/rasterizer_geometry.h"
#include "rasterizer/rasterizer_model_types.h"
#include "rasterizer/rasterizer_models.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "rasterizer/common/rasterizer_common.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"

#include <xtl.h>

#include "rasterizer/xbox/rasterizer_xbox.h"
#include "rasterizer/xbox/rasterizer_xbox_internal.h"
#include "rasterizer/rasterizer_debug_options.h"
#include "rasterizer/xbox/rasterizer_xbox_pixel_shader.h"

/* ---------- constants */

/* no shared header publishes the geometry-flag, shader-type, model-flag or model-effect
enumerations yet; compatible TU-local copies live in rasterizer_xbox_models.c,
rasterizer_xbox_transparent_geometry.c, rasterizer_transparent_geometry.c and models.c */

enum
{
	_rasterizer_target_render_primary = 0,
	_rasterizer_target_render_secondary
};

enum
{
	_rasterizer_geometry_no_sort_bit = 0,
	_rasterizer_geometry_no_queue_bit,
	_rasterizer_geometry_no_fog_bit,
	_rasterizer_geometry_no_zbuffer_bit,
	_rasterizer_geometry_sky_bit,
	_rasterizer_geometry_viewspace_bit,
	_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit,
	_rasterizer_geometry_first_person_bit,
	_rasterizer_geometry_parts_define_local_nodes_bit
};

enum
{
	_shader_type_screen = 0,
	_shader_type_effect,
	_shader_type_decal,
	_shader_type_environment,
	_shader_type_model,
	_shader_type_transparent_generic,
	_shader_type_transparent_chicago,
	_shader_type_transparent_water,
	_shader_type_transparent_glass,
	_shader_type_transparent_meter,
	_shader_type_transparent_plasma
};

enum
{
	_shader_model_detail_after_reflection_bit = 0,
	_shader_model_two_sided_bit,
	_shader_model_not_alpha_tested_bit,
	_shader_model_alpha_blended_decal_bit,
	_shader_model_true_atmospheric_fog_bit,
	_shader_model_nocull_two_sided_bit
};

enum
{
	_render_model_effect_type_none = 0,
	_render_model_effect_type_active_camouflage,
	_render_model_effect_type_modifier
};

enum
{
	_active_camouflage_tint_edge_density_bit = 0
};

/* descriptive names: January's vertex shader name table has not been recovered */
enum
{
	_rasterizer_vertex_shader_active_camouflage_model = 13,
	_rasterizer_vertex_shader_active_camouflage = 64
};

enum
{
	RASTERIZER_STENCIL_MODE_NONE = 0,
	RASTERIZER_STENCIL_MODE_REJECT = 2,
};

enum
{
	ACTIVE_CAMOUFLAGE_SCREEN_WIDTH = 320,
	ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT = 240
};

/* ---------- macros */

/* ---------- structures */

/* no shared header declares these yet; compatible TU-local copies also live in
rasterizer_xbox_models.c, rasterizer_xbox_transparent_geometry.c,
rasterizer_xbox_environment.c, rasterizer_xbox_environment_fog.c,
rasterizer_xbox_dynavobgeom.c, rasterizer_xbox_water.c, rasterizer_xbox_widgets.c,
rasterizer_xbox_shadows.c, rasterizer_transparent_geometry.c, models.c and shaders.c */

struct shader_model_properties
{
	word flags;
	short type;
	byte reserved04[0xC];
	real translucency;
	byte reserved14[0x10];
	short change_color_source;
	byte reserved26[0x1E];
	word self_illumination_flags;
	short pad46;
	short self_illumination_color_source;
	short self_illumination_animation_function;
	real self_illumination_animation_period;
	real_rgb_color self_illumination_animation_color_lower_bound;
	real_rgb_color self_illumination_animation_color_upper_bound;
	byte reserved68[0xC];
	real map_u_scale;
	real map_v_scale;
	struct tag_reference base_map;
	byte reserved8C[8];
	struct tag_reference multipurpose_map;
	byte reservedA4[8];
	short detail_function;
	short detail_mask;
	real detail_map_scale;
	struct tag_reference detail_map;
	real detail_map_v_scale;
	byte reservedC8[0xC];
	struct shader_texture_animation texture_animation;
	byte reserved10C[8];
	real reflection_falloff_distance;
	real reflection_cutoff_distance;
	real perpendicular_brightness;
	real_rgb_color perpendicular_tint_color;
	real parallel_brightness;
	real_rgb_color parallel_tint_color;
	struct tag_reference reflection_cube_map;
};

struct shader_model_definition
{
	struct shader shader;
	struct shader_model_properties model;
};

struct transparent_geometry_group
{
	unsigned long geometry_flags;
	long object_index;
	long source_object_index;
	struct shader *shader;
	short shader_permutation_index;
	word pad12;
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
	word pad66;
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

typedef char verify_shader_model_texture_animation_offset[
	offsetof(struct shader_model_definition, model.texture_animation) == 0xFC ? 1 : -1];
typedef char verify_transparent_geometry_group_size[
	sizeof(struct transparent_geometry_group) == 0xA0 ? 1 : -1];
typedef char verify_transparent_geometry_group_animation_offset[
	offsetof(struct transparent_geometry_group, animation) == 0x6C ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

extern struct rasterizer_window_begin_parameters global_window_parameters;

boolean local_active_camouflage_visibility_flag = FALSE;
boolean local_active_camouflage_debug_cached_flag = FALSE;
short local_active_camouflage_debug_cache_count = 0;

/* ---------- public code */

void rasterizer_active_camouflage_set_visibility(
	boolean visibility)
{
	local_active_camouflage_visibility_flag = visibility;

	if (!visibility)
	{
		local_active_camouflage_debug_cache_count = 0;
		local_active_camouflage_debug_cached_flag = FALSE;
	}

	return;
}

void rasterizer_active_camouflage_cache_primary_render_target(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		41,
		global_d3d_device);

	if (rasterizer_debug_options.active_camouflage &&
		local_active_camouflage_visibility_flag)
	{
		real screen_projection[5][4];
		rectangle2d bounds;

		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
			46,
			global_window_parameters.rasterizer_target==_rasterizer_target_render_primary);

		rasterizer_set_target_as_texture(
			0,
			_rasterizer_target_render_primary,
			0);
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
			4,
			_rasterizer_vertex_type_dynamic_screen,
			FALSE);

		screen_projection[0][0] =
			2.0f / (real)ACTIVE_CAMOUFLAGE_SCREEN_WIDTH;
		screen_projection[0][1] = 0.0f;
		screen_projection[0][2] = 0.0f;
		screen_projection[0][3] =
			-1.0f - 1.0f / (real)ACTIVE_CAMOUFLAGE_SCREEN_WIDTH;
		screen_projection[1][0] = 0.0f;
		screen_projection[1][1] =
			-2.0f / (real)ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT;
		screen_projection[1][2] = 0.0f;
		screen_projection[1][3] =
			1.0f + 1.0f / (real)ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT;
		screen_projection[2][0] = 0.0f;
		screen_projection[2][1] = 0.0f;
		screen_projection[2][2] = 0.0f;
		screen_projection[2][3] = 0.5f;
		screen_projection[3][0] = 0.0f;
		screen_projection[3][1] = 0.0f;
		screen_projection[3][2] = 0.0f;
		screen_projection[3][3] = 1.0f;
		screen_projection[4][0] = 1.0f;
		screen_projection[4][1] = 1.0f;
		screen_projection[4][2] = 0.0f;
		screen_projection[4][3] = 1.0f;
		IDirect3DDevice8_SetVertexShaderConstant(
			global_d3d_device,
			-68,
			screen_projection,
			5);

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = PS_TEXTUREMODES(
			PS_TEXTUREMODES_PROJECT2D,
			PS_TEXTUREMODES_NONE,
			PS_TEXTUREMODES_NONE,
			PS_TEXTUREMODES_NONE);
		pixel_shader.combiner_count = PS_COMBINERCOUNT(1, 0);
		pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO,
			PS_REGISTER_T0);
		rasterizer_set_pixel_shader(&pixel_shader);

		rasterizer_set_target(
			_rasterizer_target_render_secondary,
			0,
			0,
			FALSE,
			FALSE);
		rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_NONE);

		IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			global_window_parameters.camera.viewport_bounds.x0,
			global_window_parameters.camera.viewport_bounds.y1);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			0,
			0,
			0);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			global_window_parameters.camera.viewport_bounds.x1,
			global_window_parameters.camera.viewport_bounds.y1);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			0,
			ACTIVE_CAMOUFLAGE_SCREEN_WIDTH,
			0);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			global_window_parameters.camera.viewport_bounds.x1,
			global_window_parameters.camera.viewport_bounds.y0);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			0,
			ACTIVE_CAMOUFLAGE_SCREEN_WIDTH,
			ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			4,
			global_window_parameters.camera.viewport_bounds.x0,
			global_window_parameters.camera.viewport_bounds.y0);
		IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			0,
			0,
			ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT);
		IDirect3DDevice8_End(global_d3d_device);

		rasterizer_set_target(
			global_window_parameters.rasterizer_target,
			0,
			0,
			FALSE,
			TRUE);
		rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_REJECT);

		bounds.x0 = 512;
		bounds.x1 = 640;
		bounds.y0 = (local_active_camouflage_debug_cache_count + 1) * 96;
		bounds.y1 = (local_active_camouflage_debug_cache_count + 2) * 96;
		rasterizer_secondary_render_target_debug(&bounds);

		local_active_camouflage_debug_cache_count++;
		if (!rasterizer_debug_options.active_camouflage_multipass)
		{
			local_active_camouflage_visibility_flag = FALSE;
		}
		local_active_camouflage_debug_cached_flag = TRUE;
	}

	return;
}

void rasterizer_active_camouflage_draw(
	struct transparent_geometry_group *group)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		152,
		group);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		153,
		group->shader);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		154,
		group->effect.type==_render_model_effect_type_active_camouflage);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		155,
		group->effect.intensity>0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		156,
		group->effect.intensity<=1.0f);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
		157,
		global_d3d_device);

	if (rasterizer_debug_options.active_camouflage &&
		global_window_parameters.rasterizer_target==_rasterizer_target_render_primary)
	{
		struct shader_model_definition *model;

		model = (struct shader_model_definition *)shader_get_and_verify_type(
			group->shader,
			_shader_type_model);

		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
			164,
			local_active_camouflage_debug_cached_flag);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_active_camouflage.c",
			165,
			!TEST_FLAG(group->geometry_flags, _rasterizer_geometry_no_queue_bit));

		if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
		{
			rasterizer_set_frustum_z(
				rasterizer_globals.first_person_weapon_near_clip_distance,
				rasterizer_globals.first_person_weapon_far_clip_distance);
		}

		if (group->effect.intensity==1.0f)
		{
			/* fully cloaked: lay down depth only, alpha tested against the base map */
			rasterizer_set_texture(
				0,
				0,
				1,
				model->model.base_map.index,
				group->shader_permutation_index);
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

			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_CULLMODE,
				TEST_FLAG(model->model.flags, _shader_model_two_sided_bit) ?
					D3DCULL_NONE : D3DCULL_CCW);
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
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHAREF,
				127);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZFUNC,
				D3DCMP_LESSEQUAL);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZBIAS,
				0);

			rasterizer_set_vertex_shader_permutation(
				_rasterizer_vertex_shader_active_camouflage_model,
				rasterizer_transparent_geometry_get_primary_vertex_type(group),
				0);

			{
				real_vector4d vertex_constants[3] =
				{
					{model->model.detail_map_scale, model->model.detail_map_v_scale*model->model.detail_map_scale, 1.0f, 1.0f},
					{1.0f, 0.0f, 0.0f, 0.0f},
					{0.0f, 1.0f, 0.0f, 0.0f}
				};

				shader_texture_animation_evaluate(
					&model->model.texture_animation,
					group->animation,
					model->model.map_u_scale*group->model_base_map_scale.i,
					model->model.map_v_scale*group->model_base_map_scale.j,
					0.0f,
					0.0f,
					0.0f,
					global_frame_parameters.game_time_sec,
					&vertex_constants[1],
					&vertex_constants[2]);
				IDirect3DDevice8_SetVertexShaderConstant(
					global_d3d_device,
					-84,
					vertex_constants,
					NUMBEROF(vertex_constants));
			}

			csmemset(&pixel_shader, 0, sizeof(pixel_shader));
			pixel_shader.texture_modes = PS_TEXTUREMODES(
				PS_TEXTUREMODES_PROJECT2D,
				PS_TEXTUREMODES_NONE,
				PS_TEXTUREMODES_NONE,
				PS_TEXTUREMODES_NONE);
			pixel_shader.combiner_count = PS_COMBINERCOUNT(1, 0);
			pixel_shader.final_combiner_inputs_efg = PS_COMBINERINPUTS(
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO,
				PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
				0);
			rasterizer_set_pixel_shader(&pixel_shader);

			rasterizer_transparent_geometry_group_draw__internal(group, FALSE);
		}
		else
		{
			/* partially cloaked: draw the model normally underneath the distortion */
			struct rasterizer_model_begin_parameters parameters;

			parameters.geometry_flags = group->geometry_flags&FLAG(_rasterizer_geometry_first_person_bit);
			parameters.skinning.node_matrices = group->node_matrices;
			parameters.skinning.node_matrix_count = group->node_matrix_count;
			parameters.centroid = group->centroid;
			csmemset(&parameters.effect, 0, sizeof(parameters.effect));
			parameters.base_map_scale = group->model_base_map_scale;
			if (group->lighting)
			{
				csmemcpy(&parameters.lighting, group->lighting, sizeof(parameters.lighting));
			}
			else
			{
				csmemset(&parameters.lighting, 0, sizeof(parameters.lighting));
			}
			if (group->animation)
			{
				parameters.animation = *group->animation;
			}
			else
			{
				csmemset(&parameters.animation, 0, sizeof(parameters.animation));
			}

			rasterizer_profile_enable(FALSE);
			rasterizer_models_begin(FALSE);
			rasterizer_model_begin(&parameters, TRUE);
			rasterizer_model_draw(
				group->shader,
				group->shader_permutation_index,
				group->triangle_buffer,
				group->dynamic_triangle_buffer_index,
				group->triangle_count,
				group->vertex_buffer,
				group->dynamic_vertex_buffer_index);
			rasterizer_model_end();
			rasterizer_models_end();
			rasterizer_profile_enable(TRUE);
		}

		rasterizer_set_texture_direct(
			0,
			global_rasterizer_data->active_camouflage_distortion.index,
			0);
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

		rasterizer_set_target_as_texture(
			2,
			_rasterizer_target_render_secondary,
			0);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			2,
			D3DTSS_ADDRESSU,
			D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			2,
			D3DTSS_ADDRESSV,
			D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			2,
			D3DTSS_MAGFILTER,
			D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			2,
			D3DTSS_MINFILTER,
			D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			2,
			D3DTSS_MIPFILTER,
			D3DTEXF_POINT);

		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			TEST_FLAG(model->model.flags, _shader_model_two_sided_bit) ?
				D3DCULL_NONE : D3DCULL_CCW);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE);
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
			D3DRS_ZWRITEENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZFUNC,
			D3DCMP_EQUAL);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZBIAS,
			0);

		if (group->effect.intensity<1.0f)
		{
			SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, TRUE);
			SetRenderStateSmart(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			SetRenderStateSmart(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			SetRenderStateSmart(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		}
		else
		{
			SetRenderStateSmart(D3DRS_ALPHABLENDENABLE, FALSE);
		}

		rasterizer_set_vertex_shader_permutation(
			_rasterizer_vertex_shader_active_camouflage,
			rasterizer_transparent_geometry_get_primary_vertex_type(group),
			0);

		{
			real_vector4d vertex_constants[3];
			real_rgb_color tint_color;
			real distance_falloff;
			real refraction_amount;

			/* blend the normal and hyper-stealth parameters by the effect parameter */
			refraction_amount = (1.0f - group->effect.parameter)*global_rasterizer_data->active_camouflage_refraction_amount +
				group->effect.parameter*global_rasterizer_data->active_camouflage_hyper_stealth_refraction_amount;
			distance_falloff = (1.0f - group->effect.parameter)*global_rasterizer_data->active_camouflage_distance_falloff +
				group->effect.parameter*global_rasterizer_data->active_camouflage_hyper_stealth_distance_falloff;
			tint_color.red = (1.0f - group->effect.parameter)*global_rasterizer_data->active_camouflage_tint_color.red +
				group->effect.parameter*global_rasterizer_data->active_camouflage_hyper_stealth_tint_color.red;
			tint_color.green = (1.0f - group->effect.parameter)*global_rasterizer_data->active_camouflage_tint_color.green +
				group->effect.parameter*global_rasterizer_data->active_camouflage_hyper_stealth_tint_color.green;
			tint_color.blue = (1.0f - group->effect.parameter)*global_rasterizer_data->active_camouflage_tint_color.blue +
				group->effect.parameter*global_rasterizer_data->active_camouflage_hyper_stealth_tint_color.blue;

			vertex_constants[0].i = refraction_amount*group->effect.intensity;
			vertex_constants[0].j = distance_falloff;
			vertex_constants[0].k = (real)ACTIVE_CAMOUFLAGE_SCREEN_WIDTH;
			vertex_constants[0].l = (real)ACTIVE_CAMOUFLAGE_SCREEN_HEIGHT;
			vertex_constants[1].i = 0.0f;
			vertex_constants[1].j = 0.0f;
			vertex_constants[1].k = 0.0f;
			vertex_constants[1].l = 0.0f;
			vertex_constants[2].i = tint_color.red;
			vertex_constants[2].j = tint_color.green;
			vertex_constants[2].k = tint_color.blue;
			vertex_constants[2].l = 0.0f;
			IDirect3DDevice8_SetVertexShaderConstant(
				global_d3d_device,
				-84,
				vertex_constants,
				NUMBEROF(vertex_constants));
		}

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = PS_TEXTUREMODES(
			PS_TEXTUREMODES_CUBEMAP,
			PS_TEXTUREMODES_DOTPRODUCT,
			PS_TEXTUREMODES_DOT_ST,
			PS_TEXTUREMODES_NONE);
		pixel_shader.input_texture = PS_INPUTTEXTURE(0, 0, 0, 0);
		pixel_shader.dot_mapping = PS_DOTMAPPING(
			0,
			PS_DOTMAPPING_MINUS1_TO_1_D3D,
			PS_DOTMAPPING_MINUS1_TO_1_D3D,
			PS_DOTMAPPING_ZERO_TO_ONE);
		pixel_shader.combiner_count = PS_COMBINERCOUNT(2, 0);
		pixel_shader.rgb_inputs[0] = TEST_FLAG(
			global_rasterizer_data->active_camouflage_flags,
			_active_camouflage_tint_edge_density_bit) ?
			PS_COMBINERINPUTS(
				PS_REGISTER_T0 | PS_CHANNEL_ALPHA | PS_INPUTMAPPING_UNSIGNED_INVERT,
				PS_REGISTER_ONE,
				PS_REGISTER_T0 | PS_CHANNEL_ALPHA,
				PS_REGISTER_V0) :
			PS_COMBINERINPUTS(
				PS_REGISTER_V0,
				PS_REGISTER_ONE,
				PS_REGISTER_ZERO,
				PS_REGISTER_ZERO);
		pixel_shader.rgb_outputs[0] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			0);
		pixel_shader.rgb_inputs[1] = PS_COMBINERINPUTS(
			PS_REGISTER_V0 | PS_CHANNEL_ALPHA | PS_INPUTMAPPING_UNSIGNED_INVERT,
			PS_REGISTER_ONE,
			PS_REGISTER_V0 | PS_CHANNEL_ALPHA,
			PS_REGISTER_R0);
		pixel_shader.rgb_outputs[1] = PS_COMBINEROUTPUTS(
			PS_REGISTER_DISCARD,
			PS_REGISTER_DISCARD,
			PS_REGISTER_R0,
			0);
		pixel_shader.final_combiner_constant_0 = real_alpha_to_pixel32(group->effect.intensity);
		pixel_shader.final_combiner_inputs_abcd = PS_COMBINERINPUTS(
			PS_REGISTER_T2,
			PS_REGISTER_R0,
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO);
		pixel_shader.final_combiner_inputs_efg = PS_COMBINERINPUTS(
			PS_REGISTER_ZERO,
			PS_REGISTER_ZERO,
			PS_REGISTER_C0 | PS_CHANNEL_ALPHA,
			0);
		rasterizer_set_pixel_shader(&pixel_shader);

		rasterizer_transparent_geometry_group_draw__internal(group, FALSE);

		if (TEST_FLAG(group->geometry_flags, _rasterizer_geometry_first_person_bit))
		{
			rasterizer_set_frustum_z(0.0f, 0.0f);
		}
	}

	return;
}

/* ---------- private code */
