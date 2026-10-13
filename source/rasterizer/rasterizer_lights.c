/*
RASTERIZER_LIGHTS.C

symbols in this file:
00170840 0030:
	_screenshot_in_progress (0000)
00170870 0040:
	_lens_flare_parameters_get (0000)
001708B0 00f0:
	_lens_flare_occlusion_test_results_get (0000)
001709A0 0030:
	_rasterizer_lights_reset_for_new_map (0000)
001709D0 0130:
	_rasterizer_lights_begin_for_new_frame (0000)
00170B00 0010:
	_rasterizer_lights_begin (0000)
00170B10 0150:
	_rasterizer_light_submit (0000)
00170C60 0010:
	_rasterizer_lights_end (0000)
00170C70 0250:
	_lens_flare_evaluate_corona_rotation_function (0000)
00170EC0 0290:
	_rasterizer_lens_flare_submit (0000)
00171150 0190:
	_rasterizer_lens_flare_submit_for_cluster (0000)
001712E0 0190:
	_rasterizer_lens_flares_submit_occlusion_tests (0000)
00171470 08c0:
	_rasterizer_lens_flares_draw (0000)
0029E6A4 003f:
	??_C@_0DP@MMPMMAFC@lens_flare_index?$DO?$DN0?5?$CG?$CG?5lens_flar@ (0000)
0029E6E4 002e:
	??_C@_0CO@MGBADAON@c?3?2halo?2SOURCE?2rasterizer?2raster@ (0000)
0029E718 0063:
	??_C@_0GD@JPCGFHAO@lens_flare_parameters?9?$DOlight_ind@ (0000)
0029E780 0082:
	??_C@_0IC@IBFAKJOH@structure_lens_flare_index?$DO?$DN0?5?$CG?$CG@ (0000)
0029E804 0016:
	??_C@_0BG@JFBJFIEK@lens_flare_parameters?$AA@ (0000)
0029E81C 002e:
	??_C@_0CO@FCEOCGFK@?$CD?$CD?$CD?5ERROR?5too?5many?5lights?5submit@ (0000)
0029E84C 003f:
	??_C@_0DP@CJDDDBGK@parameters?9?$DOcolor?4blue?5?$DO?$DN0?40f?5?$CG?$CG@ (0000)
0029E88C 003f:
	??_C@_0DP@IFBCMKNP@parameters?9?$DOcolor?4green?$DO?$DN0?40f?5?$CG?$CG@ (0000)
0029E8CC 003d:
	??_C@_0DN@IFBOCEFD@parameters?9?$DOcolor?4red?5?$DO?$DN0?40f?5?$CG?$CG?5@ (0000)
0029E90C 003a:
	??_C@_0DK@NODNGFKL@?$CD?$CD?$CD?5ERROR?5unsupported?5lens?5flare@ (0000)
0029E948 0032:
	??_C@_0DC@BJJPKDMH@?$CD?$CD?$CD?5ERROR?5too?5many?5lens?5flares?5s@ (0000)
0029E980 004d:
	??_C@_0EN@OMGAKMEG@parameters?9?$DOlight_index?$DO?$DN0?5?$CG?$CG?5pa@ (0000)
0029E9D0 0065:
	??_C@_0GF@CKCJNIMD@structure_lens_flare_index?$DO?$DN0?5?$CG?$CG@ (0000)
0029EA38 005b:
	??_C@_0FL@ECFOOILG@parameters?9?$DOlens_flare_index?$DO?$DN0?5@ (0000)
0029EA98 006b:
	??_C@_0GL@HCLLBKGN@?$CIparameters?9?$DOcompressed_window_i@ (0000)
0029EB04 0017:
	??_C@_0BH@FDNIDAHL@parameters?9?$DOdefinition?$AA@ (0000)
0029EB1C 003c:
	??_C@_0DM@DJHNIOGP@?$CD?$CD?$CD?5ERROR?5unsupported?5lens?5flare@ (0000)
0029EB58 003b:
	??_C@_0DL@EPCOBKHO@animation_color?4blue?5?$DO?$DN0?40f?5?$CG?$CG?5a@ (0000)
0029EB94 003b:
	??_C@_0DL@CLPEFPIB@animation_color?4green?$DO?$DN0?40f?5?$CG?$CG?5a@ (0000)
0029EBD0 0039:
	??_C@_0DJ@NIJDHHFP@animation_color?4red?5?$DO?$DN0?40f?5?$CG?$CG?5an@ (0000)
0029EC0C 003b:
	??_C@_0DL@HIJLAENC@animation_color?4alpha?$DO?$DN0?40f?5?$CG?$CG?5a@ (0000)
0029EC48 0023:
	??_C@_0CD@LGKNCBJE@reflection?9?$DOanimation_period?$CB?$DN0?4@ (0000)
0029EC6C 0004:
	__real@42652ee1 (0000)
00466B80 51725:
	_bss_00466b80 (0000)
*/

/* ---------- headers */

#include "rasterizer/rasterizer_frame_statistics.h"
#include "cseries.h"
#include "errors.h"
#include "bitmaps/bitmaps.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "math/periodic_functions.h"
#include "objects/widgets/widget_types.h"
#include "render.h"
#include "scenario/scenario.h"
#include "tag_files/tag_groups.h"
#include "rasterizer_widgets.h"
#include "rasterizer.h"
#include "rasterizer_lights.h"
#include "rasterizer_geometry.h"
#include "rasterizer_geometry_compression.h"
#include "rasterizer_debug_options.h"
#include "objects/widgets/widget_types.h"
#include <xtl.h>
#include "rasterizer/xbox/rasterizer_xbox.h"
#ifdef HALO_LINUX
#include "main/main.h"
#endif

/* ---------- constants */

enum
{
	_rasterizer_target_render_primary = 0
};

enum
{
	_rasterizer_statistics_mode_none = 0,
	_rasterizer_statistics_mode_framerate,
	_rasterizer_statistics_mode_geometry
};

enum
{
	_lens_flare_first_person_weapon_flag = FLAG(7),
	_lens_flare_window_index_mask = ~_lens_flare_first_person_weapon_flag
};

enum
{
	_lens_flare_dynamic_light_flag = FLAG(15),
	_lens_flare_dynamic_light_index_mask = FLAG(15)-1
};

enum
{
	_lens_flare_corona_rotation_function_none = 0,
	_lens_flare_corona_rotation_function_eye_in_light_space,
	_lens_flare_corona_rotation_function_light_in_eye_space,
	_lens_flare_corona_rotation_function_eye_to_light_in_light_space,
	_lens_flare_corona_rotation_function_eye_to_light_in_eye_space,
	NUMBER_OF_LENS_FLARE_CORONA_ROTATION_FUNCTIONS
};

enum
{
	_lens_flare_occlusion_offset_direction_toward_viewer = 0,
	_lens_flare_occlusion_offset_direction_marker_forward,
	_lens_flare_occlusion_offset_direction_none,
	NUMBER_OF_LENS_FLARE_OCCLUSION_OFFSET_DIRECTIONS
};

enum
{
	_widget_type_internal_occlusion_test = 6
};

enum
{
	_lens_flare_reflection_scale_function_none = 0,
	_lens_flare_reflection_scale_function_light_direction,
	_lens_flare_reflection_scale_function_light_to_camera,
	_lens_flare_reflection_scale_function_camera_direction,
	NUMBER_OF_LENS_FLARE_REFLECTION_SCALE_FUNCTIONS
};

enum
{
	_lens_flare_reflection_rotate_from_center_of_screen_bit = 0,
	_lens_flare_reflection_radius_not_scaled_by_distance_bit,
	_lens_flare_reflection_radius_scaled_by_occlusion_bit,
	_lens_flare_reflection_zbuffer_bit,
	NUMBER_OF_LENS_FLARE_REFLECTION_FLAGS
};

enum
{
	_lens_flare_reflection_animation_color_interpolate_in_hsv_bit = 0,
	_lens_flare_reflection_animation_color_interpolate_along_farthest_hue_path_bit,
	NUMBER_OF_LENS_FLARE_REFLECTION_ANIMATION_FLAGS
};

enum
{
	_lens_flare_sun_bit = 0,
	NUMBER_OF_LENS_FLARE_DEFINITION_FLAGS
};

enum
{
	_periodic_function_one = 0,
	_periodic_function_zero
};

enum
{
	RASTERIZER_STENCIL_MODE_NONE = 0,
	RASTERIZER_STENCIL_MODE_REJECT = 2
};

/* ---------- macros */

#define LENS_FLARE_LIGHT_COLOR_ALPHA(color) ((color)>>24)
#define LENS_FLARE_LIGHT_COLOR_ALPHA_MASK 0xff000000

#define LENS_FLARE_DEFINITION_TAG 'lens'
#define lens_flare_definition_get(index) ((struct lens_flare_definition *)tag_get(LENS_FLARE_DEFINITION_TAG, (index)))

/* ---------- structures */

struct lens_flare_definition
{
	real falloff_angle;
	real cutoff_angle;
	real runtime_cosine_falloff_angle;
	real runtime_cosine_cutoff_angle;
	real occlusion_radius;
	short occlusion_offset_direction;
	word pad16;
	real near_fade_distance;
	real far_fade_distance;
	struct tag_reference primary_map;
	word flags;
	word pad32;
	byte reserved34[0x4C];
	short corona_rotation_function;
	word pad82;
	real corona_rotation_function_scale;
	byte reserved88[0x18];
	real_vector2d corona_radius_scale;
	byte reservedA8[0x1C];
	struct tag_block reflections;
	byte reservedD0[0x20];
};

struct lens_flare_reflection
{
	word flags;
	short type;
	short bitmap_index;
	word pad06;
	byte reserved08[0x14];
	real offset;
	real rotation_offset;
	byte reserved24[0x4];
	real radius_lower_bounds;
	real radius_upper_bounds;
	short radius_scale_function;
	word pad32;
	real brightness_lower_bounds;
	real brightness_upper_bounds;
	short brightness_scale_function;
	word pad3E;
	real_argb_color tint_color;
	real_argb_color animation_color_lower_bound;
	real_argb_color animation_color_upper_bound;
	word animation_flags;
	short animation_function;
	real animation_period;
	real animation_phase;
	byte reserved7C[0x4];
};

typedef char verify_lens_flare_definition_size[
	sizeof(struct lens_flare_definition) == 0xF0 ? 1 : -1];
typedef char verify_lens_flare_reflection_size[
	sizeof(struct lens_flare_reflection) == 0x80 ? 1 : -1];

struct structure_bsp
{
	byte reserved000[0x11C];
	struct tag_block lens_flares;
	struct tag_block lens_flare_markers;
	struct tag_block clusters;
};

struct structure_cluster
{
	byte reserved00[0x40];
	word first_lens_flare_marker_index;
	word lens_flare_marker_count;
	byte reserved44[0x24];
};

struct structure_lens_flare
{
	struct tag_reference lens_flare;
};

struct structure_lens_flare_marker
{
	real_point3d position;
	char i_direction;
	char j_direction;
	char k_direction;
	byte lens_flare_index;
};

struct lens_flare_occlusion_test_results
{
	short light_identifier;
	byte data[MAXIMUM_LENS_FLARES_PER_LIGHT][MAXIMUM_WINDOWS];
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

typedef char verify_structure_cluster_size[
	sizeof(struct structure_cluster) == 0x68 ? 1 : -1];
typedef char verify_structure_cluster_lens_flare_marker_count_offset[
	offsetof(
		struct structure_cluster,
		lens_flare_marker_count) == 0x42 ? 1 : -1];
typedef char verify_structure_lens_flare_marker_size[
	sizeof(struct structure_lens_flare_marker) == 0x10 ? 1 : -1];
typedef char verify_lens_flare_definition_far_fade_distance_offset[
	offsetof(
		struct lens_flare_definition,
		far_fade_distance) == 0x1C ? 1 : -1];
typedef char verify_rasterizer_light_submit_parameters_size[
	sizeof(struct rasterizer_light_submit_parameters) == 0x38 ? 1 : -1];
typedef char verify_rasterizer_lights_frame_statistics_dynamic_light_count_offset[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		dynamic_light_count) == 0x148 ? 1 : -1];
typedef char verify_rasterizer_lights_frame_statistics_lens_flare_count_offset[
	offsetof(
		struct rasterizer_frame_statistics_globals,
		lens_flare_count) == 0x14C ? 1 : -1];

/* ---------- prototypes */

static boolean screenshot_in_progress(
	void);
static struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters_get(
	short lens_flare_index);
static byte *lens_flare_occlusion_test_results_get(
	struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters);
static real lens_flare_evaluate_corona_rotation_function(
	short corona_rotation_function,
	struct rasterizer_lens_flare_submit_parameters const *lens_flare_parameters);

/* ---------- globals */

static struct lens_flare_occlusion_test_results local_lens_flare_occlusion_test_results[MAXIMUM_LIGHTS_PER_MAP] = {0};
static byte local_lens_flare_occlusion_test_results2[MAXIMUM_LENS_FLARE_MARKERS_PER_STRUCTURE+MAXIMUM_QUEUED_LENS_FLARES][MAXIMUM_WINDOWS];
static struct rasterizer_lens_flare_submit_parameters local_lens_flare_parameters[MAXIMUM_LENS_FLARES_PER_FRAME] = {0};
static long local_lens_flare_count = 0;
extern struct rasterizer_window_begin_parameters global_window_parameters;
extern short global_screenshot_count;
extern short global_screenshot_size;

/* ---------- private code */

static boolean screenshot_in_progress(
	void)
{
	return global_screenshot_count>1 || (global_screenshot_count==1 && global_screenshot_size>1);
}

static struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters_get(
	short lens_flare_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		67,
		lens_flare_index>=0 && lens_flare_index<local_lens_flare_count);

	return &local_lens_flare_parameters[lens_flare_index];
}

static byte *lens_flare_occlusion_test_results_get(
	struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters)
{
	word light_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		76,
		lens_flare_parameters);

	light_index= lens_flare_parameters->light_index;

	if (light_index & _lens_flare_dynamic_light_flag)
	{
		long window_index= lens_flare_parameters->compressed_window_index & _lens_flare_window_index_mask;
		long structure_lens_flare_index= ((light_index & _lens_flare_dynamic_light_index_mask)<<16) |
			lens_flare_parameters->lens_flare_index;

		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
			87,
			window_index>=0 && window_index<MAXIMUM_WINDOWS);
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
			88,
			structure_lens_flare_index>=0 && structure_lens_flare_index<(MAXIMUM_LENS_FLARE_MARKERS_PER_STRUCTURE+MAXIMUM_QUEUED_LENS_FLARES));

		return &local_lens_flare_occlusion_test_results2[structure_lens_flare_index][window_index];
	}

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		94,
		lens_flare_parameters->light_index>=0 && lens_flare_parameters->light_index<MAXIMUM_LIGHTS_PER_MAP);

	return &local_lens_flare_occlusion_test_results[lens_flare_parameters->light_index].
		data[lens_flare_parameters->lens_flare_index]
		[lens_flare_parameters->compressed_window_index & _lens_flare_window_index_mask];
}

static real lens_flare_evaluate_corona_rotation_function(
	short corona_rotation_function,
	struct rasterizer_lens_flare_submit_parameters const *lens_flare_parameters)
{
	real cosine = 1.0f;
	real sine = 0.0f;
	real rotation = 0.0f;
	real_vector3d direction;
	real_vector3d offset;

#line 118 "c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c"
	assert(lens_flare_parameters);

	direction = uncompress_int32_to_real_vector3d(lens_flare_parameters->compressed_direction);

	switch (corona_rotation_function)
	{
		case _lens_flare_corona_rotation_function_none:
			break;

		case _lens_flare_corona_rotation_function_eye_in_light_space:
		{
			real_vector3d plane;

			cross_product3d(&direction, &global_window_parameters.frustum.view_to_world.forward, &plane);
			cross_product3d(&plane, &direction, &plane);
			offset.i = global_window_parameters.camera.forward.i;
			offset.j = global_window_parameters.camera.forward.j;
			offset.k = global_window_parameters.camera.forward.k;
			sine = dot_product3d(&plane, &offset);
			cosine = -dot_product3d(&direction, &offset);
			break;
		}

		case _lens_flare_corona_rotation_function_light_in_eye_space:
			offset.i = -direction.i;
			offset.j = -direction.j;
			offset.k = -direction.k;
			sine = dot_product3d(&global_window_parameters.frustum.view_to_world.forward, &offset);
			cosine = -dot_product3d(&global_window_parameters.frustum.view_to_world.up, &offset);
			break;

		case _lens_flare_corona_rotation_function_eye_to_light_in_light_space:
		{
			real_vector3d plane;

			cross_product3d(&direction, &global_window_parameters.frustum.view_to_world.forward, &plane);
			cross_product3d(&plane, &direction, &plane);
			vector_from_points3d(
				&global_window_parameters.camera.position,
				&lens_flare_parameters->position,
				&offset);
			sine = dot_product3d(&plane, &offset);
			cosine = -dot_product3d(&direction, &offset);
			break;
		}

		case _lens_flare_corona_rotation_function_eye_to_light_in_eye_space:
			vector_from_points3d(
				&global_window_parameters.camera.position,
				&lens_flare_parameters->position,
				&offset);
			sine = dot_product3d(&global_window_parameters.frustum.view_to_world.forward, &offset);
			cosine = -dot_product3d(&global_window_parameters.frustum.view_to_world.up, &offset);
			break;

		default:
#line 151 "c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c"
			vassert(FALSE, "### ERROR unsupported lens flare corona rotation function");
			break;
	}

	if (corona_rotation_function != _lens_flare_corona_rotation_function_none && sine != 0.0f)
	{
		rotation = (real)atan2(sine, cosine)*(1.0f/(2.0f*_pi));
	}

	return rotation;
}

/* ---------- public code */

void rasterizer_lights_reset_for_new_map(
	void)
{
	memset(
		local_lens_flare_occlusion_test_results,
		0,
		sizeof(local_lens_flare_occlusion_test_results));
	memset(local_lens_flare_occlusion_test_results2, 0, sizeof(local_lens_flare_occlusion_test_results2));
	local_lens_flare_count = 0;

	return;
}

void rasterizer_lens_flare_submit(
	struct rasterizer_lens_flare_submit_parameters const *parameters)
{
	static boolean warned = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		266,
		parameters);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		267,
		parameters->definition);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		268,
		(parameters->compressed_window_index&_lens_flare_window_index_mask)==global_window_parameters.window_index);

	if (rasterizer_debug_options.lens_flares && !screenshot_in_progress() &&
		global_window_parameters.rasterizer_target==_rasterizer_target_render_primary)
	{
		if (local_lens_flare_count<MAXIMUM_LENS_FLARES_PER_FRAME)
		{
			real_vector3d camera_offset;
			real camera_distance;

			vector_from_points3d(
				&global_window_parameters.camera.position,
				&parameters->position,
				&camera_offset);
			camera_distance= dot_product3d(&global_window_parameters.camera.forward, &camera_offset);

			if ((parameters->definition->far_fade_distance==0.0f ||
				camera_distance<parameters->definition->far_fade_distance) &&
				(parameters->compressed_light_color&LENS_FLARE_LIGHT_COLOR_ALPHA_MASK)>0)
			{
				struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters=
					lens_flare_parameters_get((short)local_lens_flare_count++);

				memcpy(lens_flare_parameters, parameters, sizeof(*lens_flare_parameters));

				if (parameters->light_identifier==NONE)
				{
					if (parameters->light_index==NONE)
					{
						lens_flare_parameters->light_index= _lens_flare_dynamic_light_flag;

						match_assert(
							"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
							310,
							parameters->lens_flare_index>=0 && parameters->lens_flare_index<MAXIMUM_QUEUED_LENS_FLARES);
					}
					else
					{
						long structure_lens_flare_index= (parameters->light_index<<16)|parameters->lens_flare_index;

						match_assert(
							"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
							321,
							structure_lens_flare_index>=0 && structure_lens_flare_index<MAXIMUM_LENS_FLARE_MARKERS_PER_STRUCTURE);

						lens_flare_parameters->lens_flare_index= (short)(structure_lens_flare_index+MAXIMUM_QUEUED_LENS_FLARES);
						lens_flare_parameters->light_index= (short)((structure_lens_flare_index>>16)|(short)_lens_flare_dynamic_light_flag);
					}
				}
				else
				{
					struct lens_flare_occlusion_test_results *occlusion_test_results=
						&local_lens_flare_occlusion_test_results[lens_flare_parameters->light_index];

					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
						338,
						parameters->light_index>=0 && parameters->light_index<MAXIMUM_LIGHTS_PER_MAP);

					if (parameters->light_identifier!=occlusion_test_results->light_identifier)
					{
						memset(occlusion_test_results->data, 0, sizeof(occlusion_test_results->data));
						occlusion_test_results->light_identifier= lens_flare_parameters->light_identifier;
					}
				}

				if (rasterizer_debug_options.stats==_rasterizer_statistics_mode_geometry)
				{
					rasterizer_frame_statistics.lens_flare_count++;
				}
			}
		}
		else
		{
			if (!warned)
			{
				error(2, "### ERROR too many lens flares submitted to frame");
				warned= TRUE;
			}
		}
	}

	return;
}

void rasterizer_lights_begin_for_new_frame(
	void)
{
	rasterizer_profile_begin(_rasterizer_profile_lens_flare_occlusion_query);

	if (rasterizer_debug_options.lens_flares && !screenshot_in_progress())
	{
		short lens_flare_index;

		for (lens_flare_index= 0; lens_flare_index<local_lens_flare_count; lens_flare_index++)
		{
			struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters= lens_flare_parameters_get(lens_flare_index);
			byte *occlusion_test_result= lens_flare_occlusion_test_results_get(lens_flare_parameters);
			byte latest_visibility;

			if (lens_flare_parameters->internal__occlusion_pixels>0)
			{
				long visible_pixels= rasterizer_widget_get_occlusion_test_result(lens_flare_index);
				long occlusion_pixels= lens_flare_parameters->internal__occlusion_pixels;

				latest_visibility= (byte)MIN(255, (255*visible_pixels + (occlusion_pixels>>1))/occlusion_pixels);
			}
			else
			{
				latest_visibility= 0;
			}

			if (!latest_visibility)
			{
				*occlusion_test_result= 0;
			}
			else
			{
				byte previous_visibility= *occlusion_test_result;

#ifdef HALO_LINUX
				/* The native builds draw several frames per tick
				(port/linux/game/render_interpolation.c): move a quarter of the
				way up and half of the way down per 30 Hz tick, not per frame,
				at least a step a frame for as long as a tick's step would still
				move it. */
				real frame_ticks= main_get_seconds_elapsed()*TICKS_PER_SECOND;

				if (latest_visibility>previous_visibility)
				{
					long difference= latest_visibility - previous_visibility;

					if (difference>=4)
					{
						long step= (long)(difference*(1.f - (real)pow(0.75f, frame_ticks)) + 0.5f);

						*occlusion_test_result= (byte)(previous_visibility + PIN(step, 1, difference));
					}
				}
				else if (latest_visibility<previous_visibility)
				{
					long difference= previous_visibility - latest_visibility;
					long step= (long)(difference*(1.f - (real)pow(0.5f, frame_ticks)) + 0.5f);

					*occlusion_test_result= (byte)(previous_visibility - PIN(step, 1, difference));
				}
#else
				if (latest_visibility>previous_visibility)
				{
					*occlusion_test_result= (byte)((3*previous_visibility + latest_visibility)/4);
				}
				else if (latest_visibility<previous_visibility)
				{
					*occlusion_test_result= (byte)((previous_visibility + latest_visibility)/2);
				}
#endif
			}
		}

		local_lens_flare_count= 0;
	}

	rasterizer_profile_end(_rasterizer_profile_lens_flare_occlusion_query);

	return;
}

void rasterizer_lights_begin(
	void)
{
	rasterizer_lights.light_count = 0;

	return;
}

long rasterizer_light_submit(
	struct rasterizer_light_submit_parameters const *parameters)
{
	long light_index= NONE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		240,
		parameters);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		241,
		parameters->color.red >=0.0f && parameters->color.red <=1.0f);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		242,
		parameters->color.green>=0.0f && parameters->color.green<=1.0f);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
		243,
		parameters->color.blue >=0.0f && parameters->color.blue <=1.0f);

	if (rasterizer_lights.light_count<MAXIMUM_LIGHTS_PER_WINDOW)
	{
		light_index= rasterizer_lights.light_count++;
		rasterizer_lights.lights[light_index]= *parameters;

		if (rasterizer_debug_options.stats==_rasterizer_statistics_mode_geometry)
		{
			rasterizer_frame_statistics.dynamic_light_count++;
		}
	}
	else
	{
		error(2, "### ERROR too many lights submitted to window");
	}

	return light_index;
}

void rasterizer_lights_end(
	void)
{
	return;
}

void rasterizer_lens_flare_submit_for_cluster(
	short cluster_index)
{
	if (rasterizer_debug_options.lens_flares && !screenshot_in_progress())
	{
		struct structure_bsp *structure_bsp= global_structure_bsp_get();
		struct structure_cluster *cluster= TAG_BLOCK_GET_ELEMENT(&structure_bsp->clusters, cluster_index, struct structure_cluster);
		long lens_flare_marker_index;

		for (lens_flare_marker_index= 0; lens_flare_marker_index<cluster->lens_flare_marker_count; lens_flare_marker_index++)
		{
			long structure_lens_flare_marker_index= cluster->first_lens_flare_marker_index+lens_flare_marker_index;
			struct structure_lens_flare_marker *marker= TAG_BLOCK_GET_ELEMENT(&structure_bsp->lens_flare_markers, structure_lens_flare_marker_index, struct structure_lens_flare_marker);
			struct structure_lens_flare *structure_lens_flare= TAG_BLOCK_GET_ELEMENT(&structure_bsp->lens_flares, marker->lens_flare_index, struct structure_lens_flare);
			struct rasterizer_lens_flare_submit_parameters parameters;

			{
				real_vector3d direction;
				real_vector3d up;

				set_real_vector3d(
					&direction,
					marker->i_direction*(1.0f/127.0f),
					marker->j_direction*(1.0f/127.0f),
					marker->k_direction*(1.0f/127.0f));

				perpendicular3d(&direction, &up);
				normalize3d(&direction);
				normalize3d(&up);

				parameters.compressed_direction= compress_real_vector3d_to_int32_clamp(&direction);
				parameters.compressed_up= compress_real_vector3d_to_int32_clamp(&up);
			}

			parameters.definition= lens_flare_definition_get(structure_lens_flare->lens_flare.index);
			parameters.position= marker->position;
			parameters.compressed_light_color= NONE;
			parameters.light_identifier= NONE;
			parameters.compressed_light_scale= 0;
			parameters.light_index= (short)(structure_lens_flare_marker_index>>16);
			parameters.lens_flare_index= (short)structure_lens_flare_marker_index;
			parameters.compressed_window_index= (byte)render.window_index;

			rasterizer_lens_flare_submit(&parameters);
		}
	}

	return;
}

void rasterizer_lens_flares_submit_occlusion_tests(
	void)
{
	rasterizer_profile_begin(_rasterizer_profile_lens_flare_occlusion_submit);

	if (rasterizer_debug_options.lens_flares && !screenshot_in_progress() &&
		global_window_parameters.rasterizer_target == _rasterizer_target_render_primary &&
		local_lens_flare_count > 0)
	{
		short lens_flare_index;

		rasterizer_widget_begin(
			_widget_type_internal_occlusion_test,
			FLAG(_rasterizer_widget_z_enable_bit));

		for (lens_flare_index = 0; lens_flare_index < local_lens_flare_count; lens_flare_index++)
		{
			struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters =
				lens_flare_parameters_get(lens_flare_index);
			struct lens_flare_definition *definition = lens_flare_parameters->definition;
			real_vector3d direction = uncompress_int32_to_real_vector3d(lens_flare_parameters->compressed_direction);

			if ((lens_flare_parameters->compressed_window_index & _lens_flare_window_index_mask) ==
				global_window_parameters.window_index)
			{
				real occlusion_radius = definition->occlusion_radius;
				real_point3d occlusion_point;

				switch (definition->occlusion_offset_direction)
				{
				case _lens_flare_occlusion_offset_direction_toward_viewer:
					point_from_line3d(
						&lens_flare_parameters->position,
						&global_window_parameters.camera.forward,
						-definition->occlusion_radius,
						&occlusion_point);
					break;

				case _lens_flare_occlusion_offset_direction_marker_forward:
					point_from_line3d(
						&lens_flare_parameters->position,
						&direction,
						definition->occlusion_radius * 1.41421356f,
						&occlusion_point);
					break;

				case _lens_flare_occlusion_offset_direction_none:
					occlusion_point = lens_flare_parameters->position;
					break;

				default:
#line 482 "c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c"
					vassert(FALSE, "### ERROR unsupported lens flare occlusion offset direction");
					break;
				}

				lens_flare_parameters->internal__occlusion_pixels =
					rasterizer_widget_submit_occlusion_test(
						&occlusion_point,
						occlusion_radius,
						lens_flare_index);
			}
		}

		rasterizer_widget_end();
	}

	rasterizer_profile_end(_rasterizer_profile_lens_flare_occlusion_submit);

	return;
}

void rasterizer_lens_flares_draw(
	void)
{
	rasterizer_profile_begin(_rasterizer_profile_lens_flares);

	if (rasterizer_debug_options.lens_flares &&
		global_window_parameters.rasterizer_target == _rasterizer_target_render_primary &&
		local_lens_flare_count > 0)
	{
		short lens_flare_index;

		rasterizer_widget_begin(_widget_type_internal_sprite, 0);

		for (lens_flare_index = 0; lens_flare_index < local_lens_flare_count; lens_flare_index++)
		{
			struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters =
				lens_flare_parameters_get(lens_flare_index);
			byte *occlusion_test_result = lens_flare_occlusion_test_results_get(lens_flare_parameters);
			real_vector3d direction = uncompress_int32_to_real_vector3d(lens_flare_parameters->compressed_direction);

			if ((lens_flare_parameters->compressed_window_index & _lens_flare_window_index_mask) ==
				global_window_parameters.window_index)
			{
				struct lens_flare_definition *definition = lens_flare_parameters->definition;

				if (lens_flare_parameters->internal__occlusion_pixels > 0 &&
					LENS_FLARE_LIGHT_COLOR_ALPHA(lens_flare_parameters->compressed_light_color) > 0 &&
					definition->reflections.count > 0)
				{
					real_point3d position = lens_flare_parameters->position;
					real_vector3d camera_offset;
					real_vector3d mirror;
					real depth;
					real occlusion_fraction;
					real light_brightness;
					real corona_rotation;
					real screen_rotation;
					real cosine_scale;
					real cosine_offset;
					real scale_functions[NUMBER_OF_LENS_FLARE_REFLECTION_SCALE_FUNCTIONS];
					real light_scale;
					short reflection_index;

					vector_from_points3d(
						&global_window_parameters.camera.position,
						&position,
						&camera_offset);
					depth = dot_product3d(&global_window_parameters.camera.forward, &camera_offset);

					scale_vector3d(&global_window_parameters.camera.forward, depth, &mirror);
					subtract_vectors3d(&mirror, &camera_offset, &mirror);
					scale_vector3d(&mirror, 2.0f, &mirror);

					occlusion_fraction = *occlusion_test_result*(1.0f/255.0f);

					if (definition->far_fade_distance > 0.0f)
					{
						light_brightness = PIN(
							(depth-definition->far_fade_distance)/
								(definition->near_fade_distance-definition->far_fade_distance),
							0.0f,
							1.0f);
					}
					else
					{
						light_brightness = 1.0f;
					}

					light_brightness *= occlusion_fraction;
					light_brightness *= uncompress_int8_to_real(
						(byte)LENS_FLARE_LIGHT_COLOR_ALPHA(lens_flare_parameters->compressed_light_color));
					corona_rotation = lens_flare_evaluate_corona_rotation_function(
						definition->corona_rotation_function,
						lens_flare_parameters)*definition->corona_rotation_function_scale;
					screen_rotation = (real)atan2(
						dot_product3d(&global_window_parameters.frustum.view_to_world.forward, &camera_offset),
						dot_product3d(&global_window_parameters.frustum.view_to_world.left, &camera_offset))*(180.0f/((real)M_PI));

					cosine_scale = 1.0f/
						(definition->runtime_cosine_falloff_angle-definition->runtime_cosine_cutoff_angle);
					cosine_offset = -(definition->runtime_cosine_cutoff_angle*cosine_scale);

					normalize3d(&camera_offset);

					scale_functions[_lens_flare_reflection_scale_function_none] = 1.0f;
					scale_functions[_lens_flare_reflection_scale_function_light_direction] = PIN(
						-dot_product3d(&global_window_parameters.camera.forward, &direction)*cosine_scale+cosine_offset,
						0.0f,
						1.0f);
					scale_functions[_lens_flare_reflection_scale_function_light_to_camera] = PIN(
						-dot_product3d(&direction, &camera_offset)*cosine_scale+cosine_offset,
						0.0f,
						1.0f);
					scale_functions[_lens_flare_reflection_scale_function_camera_direction] = PIN(
						dot_product3d(&global_window_parameters.camera.forward, &camera_offset)*cosine_scale+cosine_offset,
						0.0f,
						1.0f);

					if (light_brightness > 0.0f)
					{
						light_scale = uncompress_int8_to_real(lens_flare_parameters->compressed_light_scale);

						for (reflection_index = 0; reflection_index < definition->reflections.count; reflection_index++)
						{
							struct lens_flare_reflection *reflection = TAG_BLOCK_GET_ELEMENT(
								&definition->reflections,
								reflection_index,
								struct lens_flare_reflection);
							real brightness_lower_bound = reflection->brightness_lower_bounds;
							real brightness = (brightness_lower_bound+
								(reflection->brightness_upper_bounds-brightness_lower_bound)*light_scale)*
								scale_functions[reflection->brightness_scale_function]*light_brightness;

							if (reflection_index == 0)
							{
								light_brightness = brightness;
							}

							if (brightness > 0.0f)
							{
								real radius_lower_bound = reflection->radius_lower_bounds;
								real radius = radius_lower_bound+
									(reflection->radius_upper_bounds-radius_lower_bound)*light_scale;
								real_argb_color color;
								real_vector2d radius_scale;
								real rotation;
								pixel32 pixel;
								real tint_factor;

								if (reflection->tint_color.alpha == 0.0f &&
									reflection->tint_color.red == 0.0f &&
									reflection->tint_color.green == 0.0f &&
									reflection->tint_color.blue == 0.0f)
								{
									pixel = (compress_real_to_int8(brightness)<<24) |
										(lens_flare_parameters->compressed_light_color & ~LENS_FLARE_LIGHT_COLOR_ALPHA_MASK);
									tint_factor = 1.0f;
								}
								else
								{
									color.rgb = reflection->tint_color.rgb;
									color.alpha = brightness;

									if (reflection->animation_function > _periodic_function_zero)
									{
										real_argb_color animation_color;
										real animation_time;

										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
											651,
											reflection->animation_period!=0.0f);

										animation_time = periodic_function_evaluate(
											reflection->animation_function,
											(global_frame_parameters.game_time_sec+reflection->animation_phase)/
												reflection->animation_period);

										rgb_colors_interpolate(
											&animation_color.rgb,
											reflection->animation_flags &
												(FLAG(_lens_flare_reflection_animation_color_interpolate_in_hsv_bit) |
												 FLAG(_lens_flare_reflection_animation_color_interpolate_along_farthest_hue_path_bit)),
											&reflection->animation_color_lower_bound.rgb,
											&reflection->animation_color_upper_bound.rgb,
											animation_time);
										scalars_interpolate(
											reflection->animation_color_lower_bound.alpha,
											reflection->animation_color_upper_bound.alpha,
											animation_time,
											&animation_color.alpha);

										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
											667,
											animation_color.alpha>=0.0f && animation_color.alpha<=1.0f);
										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
											668,
											animation_color.red >=0.0f && animation_color.red <=1.0f);
										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
											669,
											animation_color.green>=0.0f && animation_color.green<=1.0f);
										match_assert(
											"c:\\halo\\SOURCE\\rasterizer\\rasterizer_lights.c",
											670,
											animation_color.blue >=0.0f && animation_color.blue <=1.0f);

										color.alpha *= animation_color.alpha;
										color.red *= animation_color.red;
										color.green *= animation_color.green;
										color.blue *= animation_color.blue;
									}

									pixel = real_argb_color_to_pixel32(&color);
									tint_factor = reflection->tint_color.alpha;
								}

								if (reflection_index == 0)
								{
									rotation = corona_rotation+reflection->rotation_offset;
									radius_scale = definition->corona_radius_scale;
								}
								else
								{
									rotation = reflection->rotation_offset;
									radius_scale.i = radius_scale.j = 1.0f;
								}

								if (TEST_FLAG(reflection->flags, _lens_flare_reflection_rotate_from_center_of_screen_bit))
								{
									rotation += screen_rotation;
								}
								if (TEST_FLAG(reflection->flags, _lens_flare_reflection_radius_scaled_by_occlusion_bit))
								{
									radius = (occlusion_fraction+1.0f)*radius*0.5f;
								}
								if (TEST_FLAG(reflection->flags, _lens_flare_reflection_radius_not_scaled_by_distance_bit))
								{
									radius *= depth;
								}

								{
									real offset = reflection->offset;
									real_point3d point;

									point.x = offset*mirror.i + position.x;
									point.y = offset*mirror.j + position.y;
									point.z = offset*mirror.k + position.z;

									if (rasterizer_widget_set_texture(
										0,
										definition->primary_map.index,
										reflection->bitmap_index))
									{
										break;
									}

									rasterizer_widget_set_tint_factor(tint_factor);
									if (TEST_FLAG(reflection->flags, _lens_flare_reflection_zbuffer_bit) &&
										(lens_flare_parameters->compressed_window_index & _lens_flare_first_person_weapon_flag))
									{
										rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_REJECT);
									}
									else
									{
										rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_NONE);
									}
									rasterizer_widget_draw_sprite3d(
										&point,
										radius,
										&radius_scale,
										rotation*(_pi/180.0f),
										pixel);
								}
							}
						}
					}
				}
			}
		}

		rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_NONE);
		rasterizer_widget_end();

		if (rasterizer_debug_options.ray_of_buddha)
		{
			for (lens_flare_index = 0; lens_flare_index < local_lens_flare_count; lens_flare_index++)
			{
				struct rasterizer_lens_flare_submit_parameters *lens_flare_parameters =
					lens_flare_parameters_get(lens_flare_index);

				if (lens_flare_parameters->internal__occlusion_pixels > 0 &&
					(lens_flare_parameters->compressed_window_index & _lens_flare_window_index_mask) ==
						global_window_parameters.window_index &&
					(lens_flare_parameters->definition->occlusion_radius == 50.0f ||
						TEST_FLAG(lens_flare_parameters->definition->flags, _lens_flare_sun_bit)))
				{
					rasterizer_sun_glow_draw(lens_flare_parameters);
				}
			}
		}
	}

	rasterizer_profile_end(_rasterizer_profile_lens_flares);

	return;
}
