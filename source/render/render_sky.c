/*
RENDER_SKY.C

symbols in this file:
0017C290 04d0:
	_render_sky (0000)
002A007C 0004:
	__real@3f7fc000 (0000)
002A0080 0004:
	__real@447ff800 (0000)
002A0088 0048:
	??_C@_0EI@POBIDGNL@?$CBrender?4visible_sky_model?5?$HM?$HM?5sce@ (0000)
002A00D0 0023:
	??_C@_0CD@OBGDMKCP@c?3?2halo?2SOURCE?2render?2render_sky@ (0000)
004C04F8 0020:
	_bss_004c04f8 (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "render.h"
#include "models/model_animation_definitions.h"
#include "models/model_definitions.h"
#include "models/models.h"
#include "objects/objects.h"
#include "scenario/scenario.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct sky
{
	struct tag_reference model;
	struct tag_reference animation_graph;
	byte pad20[0x8C];
	struct tag_block render_model_regions;
	struct tag_block animations;
	struct tag_block lights;
};

struct sky_render_model_region
{
	byte unused[0x24];
};

struct sky_animation
{
	short animation_index;
	word pad2;
	real period;
	byte unused8[0x1C];
};

struct sky_light
{
	struct tag_reference lens_flare;
	char marker_name[TAG_STRING_LENGTH + 1];
	byte pad31[0x37];
	real_euler_angles2d direction;
	byte pad70[4];
};

typedef char verify_sky_animations_offset[offsetof(struct sky, animations) == 0xB8 ? 1 : -1];
typedef char verify_sky_lights_offset[offsetof(struct sky, lights) == 0xC4 ? 1 : -1];
typedef char verify_sky_animation_size[sizeof(struct sky_animation) == 0x24 ? 1 : -1];
typedef char verify_sky_light_size[sizeof(struct sky_light) == 0x74 ? 1 : -1];
typedef char verify_sky_render_lighting_size[sizeof(struct render_lighting) == 0x74 ? 1 : -1];

/* ---------- prototypes */

void lights_queue_lens_flare(
	long lens_flare_index,
	real_point3d const *position,
	real_vector3d const *forward,
	real_vector3d const *up,
	real_rgb_color const *color,
	real scale);
void rasterizer_models_begin(
	boolean sky_mode);
void rasterizer_models_end(
	void);
/* ---------- globals */

real render_sky_globals[MAXIMUM_SKIES_PER_SCENARIO] = {0.f};

/* ---------- public code */

void render_sky(
	void)
{
	real_matrix4x3 node_matrices[MAXIMUM_NODES_PER_ANIMATION];
	struct real_orientation node_orientations[MAXIMUM_NODES_PER_ANIMATION];
	struct render_lighting render_model_lighting;
	real region_scales[MAXIMUM_SKIES_PER_SCENARIO];
	real_matrix4x3 view_matrix;
	struct object_marker light_marker;
	struct sky *sky;
	struct model *model;
	struct animation_graph *animation_graph;
	short i;

	if (render.visible_sky_model)
	{
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_sky.c",
			0x26,
			!render.visible_sky_model || scenario_get_sky(render.visible_sky_index));

		if (render.visible_sky_model)
		{
			sky = scenario_get_sky(render.visible_sky_index);
			model = model_definition_get(sky->model.index);
			model_get_node_orientations(model, node_orientations);

			if (sky->animation_graph.index != NONE)
			{
				animation_graph = animation_graph_definition_get(sky->animation_graph.index);
				for (i = 0; i < sky->animations.count; i++)
				{
					struct sky_animation *sky_animation = TAG_BLOCK_GET_ELEMENT(
						&sky->animations,
						i,
						struct sky_animation);

					if (sky_animation->animation_index >= 0 &&
						sky_animation->animation_index < animation_graph->animations.count &&
						sky_animation->period != 0.f)
					{
						struct animation *animation = TAG_BLOCK_GET_ELEMENT(
							&animation_graph->animations,
							i,
							struct animation);

						if (animation->node_count == model->nodes.count)
						{
							real phase = (real)fmod(
								(double)(render.time_delta_since_tick_sec / sky_animation->period + render_sky_globals[i]),
								1.0);
							render_sky_globals[i] = phase;
							overlay_animation_apply_continuous(
								animation,
								animation->frame_count * phase,
								node_orientations);
						}
					}
				}
			}

			model_node_matrices_from_orientations(
				model,
				node_matrices,
				node_orientations,
				global_origin3d,
				global_forward3d,
				global_up3d);

			for (i = 0; i < sky->render_model_regions.count; i++)
			{
				TAG_BLOCK_GET_ELEMENT(
					&sky->render_model_regions,
					i,
					struct sky_render_model_region);
				region_scales[i] = 1.f;
			}

			for (i = 0; i < sky->lights.count; i++)
			{
				struct sky_light *light = TAG_BLOCK_GET_ELEMENT(
					&sky->lights,
					i,
					struct sky_light);

				if (light->lens_flare.index != NONE)
				{
					real_vector3d direction;

					if (csstrlen(light->marker_name) != 0)
					{
						if (model_get_marker_by_name(
							sky->model.index,
							light->marker_name,
							NULL,
							NULL,
							NONE,
							node_matrices,
							FALSE,
							&light_marker,
							1) == 0)
						{
							goto next_light;
						}

						vector_from_points3d(
							&render.camera.position,
							&light_marker.matrix.position,
							&direction);
						normalize3d(&direction);
					}
					else
					{
						vector3d_from_euler_angles2d(&direction, &light->direction);
					}

					{
						point_from_line3d(
							&render.camera.position,
							&direction,
							1023.875f,
							&light_marker.matrix.position);
						negate_vector3d(&direction, &light_marker.matrix.forward);
						perpendicular3d(&light_marker.matrix.forward, &light_marker.matrix.up);
						normalize3d(&light_marker.matrix.up);
						lights_queue_lens_flare(
							light->lens_flare.index,
							&light_marker.matrix.position,
							&light_marker.matrix.forward,
							&light_marker.matrix.up,
							global_real_rgb_white,
							1.f);
					}
				}

			next_light:
				;
			}

			view_matrix = *global_identity4x3;
			view_matrix.position.x = render.camera.position.x * 0.9990234375f;
			view_matrix.position.y = render.camera.position.y * 0.9990234375f;
			view_matrix.position.z = render.camera.position.z * 0.9990234375f;
			{
				long node_count = model->nodes.count;

				view_matrix.scale = 1.f / 1024.f;
				for (i = 0; i < node_count; i++)
				{
					matrix4x3_multiply(
						&view_matrix,
						&node_matrices[i],
						&node_matrices[i]);
					node_count = model->nodes.count;
				}
			}

			rasterizer_models_begin(TRUE);
			csmemset(&render_model_lighting, 0, sizeof(render_model_lighting));
			render_model_lighting.ambient_color = *global_real_rgb_white;
			render_model(
				sky->model.index,
				0.f,
				node_matrices,
				0,
				0,
				region_scales,
				&render_model_lighting,
				&render.camera.position,
				0,
				0,
				0,
				0,
				TRUE);
			rasterizer_models_end();
		}
	}

	return;
}

/* ---------- private code */
