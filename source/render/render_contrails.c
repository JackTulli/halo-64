/*
RENDER_CONTRAILS.C

symbols in this file:
001777D0 0090:
	_contrail_fade (0000)
00177860 07a0:
	_code_00177860 (0000)
00178000 00b0:
	_code_00178000 (0000)
001780B0 0010:
	_render_contrails_ground_mapped (0000)
001780C0 0010:
	_render_contrails_media_mapped (0000)
001780D0 0010:
	_render_contrails_normal (0000)
0029FC28 002d:
	??_C@_0CN@ILEHFPFH@contrail?5?$CFs?5uses?5an?5unsupported?5@ (0000)
0029FC58 0016:
	??_C@_0BG@PBIEBPJJ@triangles?5?$CG?$CG?5vertices?$AA@ (0000)
0029FC70 0029:
	??_C@_0CJ@KBEDIECC@c?3?2halo?2SOURCE?2render?2render_con@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "effects/contrail_definitions.h"
#include "effects/contrails.h"
#include "bitmaps/bitmap_group.h"
#include "cseries/errors.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "math/periodic_functions.h"
#include "objects/object_definitions.h"
#include "objects/objects.h"
#include "rasterizer/rasterizer.h"
#include "render.h"
#include "tag_files/tag_files.h"

/* ---------- constants */

enum
{
	_contrail_render_mode_media_mapped = 1 << 2,
	_contrail_render_mode_ground_mapped = 1 << 3,
	_contrail_render_mode_normal = ~(_contrail_render_mode_media_mapped | _contrail_render_mode_ground_mapped),
};

enum
{
	_contrail_point_transitioning_bit = 1,
};

/* ---------- macros */

/* ---------- structures */

struct contrail_vertex
{
	real_point3d point;
	real_point2d texture;
	pixel32 color;
};

typedef char verify_contrail_vertex_size[
	sizeof(struct contrail_vertex) == 0x18 ? 1 : -1];

/* ---------- prototypes */

void render_contrails(
	unsigned long render_mode_flags);
void render_contrail(
	struct contrail_datum *contrail,
	struct contrail_definition *definition,
	short instance_index);

struct bitmap_data *bitmap_group_get_bitmap_from_sequence(
	long bitmap_group_index,
	short sequence_index,
	short frame_index);
void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load);

real contrail_fade(
	struct contrail_definition *definition,
	short fade_mode,
	real_point3d const *world_point,
	real_vector3d const *world_normal);

/* ---------- globals */

extern boolean render_contrails_enabled;

/* ---------- public code */

real contrail_fade(
	struct contrail_definition *definition,
	short fade_mode,
	real_point3d const *world_point,
	real_vector3d const *world_normal)
{
	real result = 1.f;

	if (fade_mode)
	{
		real_vector3d to_camera;

		vector_from_points3d(world_point, &render.camera.position, &to_camera);
		result = fabs(dot_product3d(world_normal, &to_camera) / magnitude3d(&to_camera));

		if (definition->flags & (1 << _contrail_fades_slowly_bit))
			result = transition_function_evaluate(2, result);
		if (fade_mode == 2)
			result = 1.f - result;
	}

	return result;
}

void render_contrail(
	struct contrail_datum *contrail,
	struct contrail_definition *definition,
	short instance_index)
{
	struct bitmap_data *bitmap;
	short segment_count;
	short triangle_count;
	short vertex_count;
	long triangle_buffer_index;
	long vertex_buffer_index;

	bitmap = bitmap_group_get_bitmap_from_sequence(
		definition->bitmap.index,
		contrail->sequence_index,
		contrail->frame_index);
	rasterizer_globals.current_lock_operation = _rasterizer_lock_contrail;
	if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
	{
		segment_count = contrail->contrail_point_counts[instance_index] - 1;
		triangle_count = segment_count + segment_count;
		vertex_count = triangle_count + 2;
		triangle_buffer_index = rasterizer_dynamic_triangles_new(triangle_count);
		vertex_buffer_index = rasterizer_dynamic_vertices_new(6, vertex_count);
		if (triangle_buffer_index != NONE && vertex_buffer_index != NONE)
		{
			short *triangles;
			struct contrail_vertex *vertices;
			struct contrail_vertex *first_vertex;
			struct contrail_shader *shader;
			boolean apply_orientation;
			real texture_u_step;
			real texture_offset_v;
			real texture_v_far;
			real_point3d average_position;
			struct contrail_point_datum *previous_point;
			real texture_u;
			long point_index;

			triangles = rasterizer_dynamic_triangles_lock(triangle_buffer_index);
			vertices = rasterizer_dynamic_vertices_lock(vertex_buffer_index);
			average_position = *global_origin3d;
			shader = &definition->shader;
			apply_orientation = shader->framebuffer_fade_mode != 0;
			match_assert(
				"c:\\halo\\SOURCE\\render\\render_contrails.c",
				136,
				triangles && vertices);

			texture_u = contrail->texture_offset_u;
			texture_u_step = definition->texture_repeats_u;
			if (definition->scale_flags & (1 << _contrail_texture_repeats_u_bit))
				texture_u_step *= contrail->density;
			texture_u_step = -texture_u_step;
			texture_offset_v = contrail->texture_offset_v;
			texture_v_far = definition->texture_repeats_v;
			if (definition->scale_flags & (1 << _contrail_texture_repeats_v_bit))
				texture_v_far *= contrail->density;
			texture_v_far += texture_offset_v;
			previous_point = NULL;
			point_index = contrail->first_contrail_point_indices[instance_index];

			while (point_index != NONE)
			{
				struct contrail_point_datum *point;
				struct contrail_point_state *state;
				real color_scale;
				real width;
				real_argb_color color;
				real half_width;
				real_vector3d orientation;
				real_vector3d facing_normal;

				point = datum_get(contrail_point_data, point_index);
				state = TAG_BLOCK_GET_ELEMENT(
					&definition->states,
					point->state_index,
					struct contrail_point_state);
				color_scale = 1.f;
				if (state->scale_flags & (1 << _contrail_state_color_bit))
					color_scale = point->density;
				width = state->width;
				if (state->scale_flags & (1 << _contrail_state_width_bit))
					width *= point->density;
				half_width = width;
				color.alpha = (state->color_upper_bound.alpha - state->color_lower_bound.alpha) * color_scale + state->color_lower_bound.alpha;
				color.red = (state->color_upper_bound.red - state->color_lower_bound.red) * color_scale + state->color_lower_bound.red;
				color.green = (state->color_upper_bound.green - state->color_lower_bound.green) * color_scale + state->color_lower_bound.green;
				color.blue = (state->color_upper_bound.blue - state->color_lower_bound.blue) * color_scale + state->color_lower_bound.blue;

				if (point->flags & (1 << _contrail_point_transitioning_bit))
				{
					struct contrail_point_state *next_state;
					real transition;
					real next_color_scale;
					real next_width;
					real_argb_color next_color;

					next_state = TAG_BLOCK_GET_ELEMENT(
						&definition->states,
						point->state_index + 1,
						struct contrail_point_state);
					transition = point->time;
					next_color_scale = 1.f;
					if (next_state->scale_flags & (1 << _contrail_state_color_bit))
						next_color_scale = point->density;
					next_width = next_state->width;
					if (next_state->scale_flags & (1 << _contrail_state_width_bit))
						next_width *= point->density;
					next_color.alpha = (next_state->color_upper_bound.alpha - next_state->color_lower_bound.alpha) * next_color_scale + next_state->color_lower_bound.alpha;
					next_color.red = (next_state->color_upper_bound.red - next_state->color_lower_bound.red) * next_color_scale + next_state->color_lower_bound.red;
					next_color.green = (next_state->color_upper_bound.green - next_state->color_lower_bound.green) * next_color_scale + next_state->color_lower_bound.green;
					next_color.blue = (next_state->color_upper_bound.blue - next_state->color_lower_bound.blue) * next_color_scale + next_state->color_lower_bound.blue;
					half_width = (next_width - width) * transition + width;
					color.alpha = (next_color.alpha - color.alpha) * transition + color.alpha;
					color.red = (next_color.red - color.red) * transition + color.red;
					color.green = (next_color.green - color.green) * transition + color.green;
					color.blue = (next_color.blue - color.blue) * transition + color.blue;
				}

				if (contrail->object_index != NONE)
				{
					struct object_datum *object = object_get(contrail->object_index);
					struct object_definition *object_definition = object_definition_get(object->definition_index);
					struct object_attachment_definition *attachment = TAG_BLOCK_GET_ELEMENT(
						&object_definition->object.attachments,
						contrail->attachment_index,
						struct object_attachment_definition);
					short change_color_index = attachment->change_color_reference - 1;

					if (change_color_index != NONE)
					{
						real_rgb_color const *change_color = &object->object.outgoing_change_colors[change_color_index];
						color.red *= change_color->red;
						color.green *= change_color->green;
						color.blue *= change_color->blue;
					}
				}

				half_width *= 0.5f;
				vertices[0].texture.x = texture_u;
				vertices[0].texture.y = texture_v_far;
				vertices[1].texture.x = texture_u;
				vertices[1].texture.y = texture_offset_v;
				average_position.x += point->position.x;
				average_position.y += point->position.y;
				average_position.z += point->position.z;

				switch (definition->render_type)
				{
				case _contrail_render_type_vertical:
					vertices[0].point.x = point->position.x;
					vertices[0].point.y = point->position.y;
					vertices[0].point.z = point->position.z - half_width;
					vertices[1].point.x = point->position.x;
					vertices[1].point.y = point->position.y;
					vertices[1].point.z = point->position.z + half_width;
					if (apply_orientation)
					{
						struct contrail_point_datum *next_point;
						if (previous_point)
						{
							orientation.i = previous_point->position.y - point->position.y;
							orientation.j = point->position.x - previous_point->position.x;
						}
						else
						{
							next_point = datum_get(contrail_point_data, point->next_contrail_point_index);
							orientation.i = point->position.y - next_point->position.y;
							orientation.j = next_point->position.x - point->position.x;
						}
						orientation.k = 0.f;
						normalize3d(&orientation);
					}
					break;

				case _contrail_render_type_horizontal:
				case _contrail_render_type_media:
				{
					real_vector2d perpendicular;
					struct contrail_point_datum *next_point;
					if (previous_point)
					{
						perpendicular.i = previous_point->position.y - point->position.y;
						perpendicular.j = point->position.x - previous_point->position.x;
					}
					else
					{
						next_point = datum_get(contrail_point_data, point->next_contrail_point_index);
						perpendicular.i = point->position.y - next_point->position.y;
						perpendicular.j = next_point->position.x - point->position.x;
					}
					normalize2d(&perpendicular);
					vertices[0].point.x = point->position.x - perpendicular.i * half_width;
					vertices[0].point.y = point->position.y - perpendicular.j * half_width;
					vertices[0].point.z = point->position.z;
					vertices[1].point.x = point->position.x + perpendicular.i * half_width;
					vertices[1].point.y = point->position.y + perpendicular.j * half_width;
					vertices[1].point.z = point->position.z;
					if (apply_orientation)
						orientation = *global_up3d;
					break;
				}

				case _contrail_render_type_viewer:
				{
					real_vector3d tangent;
					real eye_i;
					real eye_j;
					real eye_k;
					struct contrail_point_datum *next_point;
					if (previous_point)
					{
						eye_i = render.camera.position.x - previous_point->position.x;
						eye_j = render.camera.position.y - previous_point->position.y;
						eye_k = render.camera.position.z - previous_point->position.z;
						tangent.i = point->position.x - previous_point->position.x;
						tangent.j = point->position.y - previous_point->position.y;
						tangent.k = point->position.z - previous_point->position.z;
					}
					else
					{
						next_point = datum_get(contrail_point_data, point->next_contrail_point_index);
						eye_i = render.camera.position.x - point->position.x;
						eye_j = render.camera.position.y - point->position.y;
						eye_k = render.camera.position.z - point->position.z;
						tangent.i = next_point->position.x - point->position.x;
						tangent.j = next_point->position.y - point->position.y;
						tangent.k = next_point->position.z - point->position.z;
					}
					facing_normal.i = tangent.k * eye_j - tangent.j * eye_k;
					facing_normal.j = tangent.i * eye_k - tangent.k * eye_i;
					facing_normal.k = tangent.j * eye_i - tangent.i * eye_j;
					normalize3d(&facing_normal);
					vertices[0].point.x = point->position.x - facing_normal.i * half_width;
					vertices[0].point.y = point->position.y - facing_normal.j * half_width;
					vertices[0].point.z = point->position.z - facing_normal.k * half_width;
					vertices[1].point.x = point->position.x + facing_normal.i * half_width;
					vertices[1].point.y = point->position.y + facing_normal.j * half_width;
					vertices[1].point.z = point->position.z + facing_normal.k * half_width;
					if (apply_orientation)
					{
						orientation.i = facing_normal.k * tangent.j - facing_normal.j * tangent.k;
						orientation.j = facing_normal.i * tangent.k - facing_normal.k * tangent.i;
						orientation.k = facing_normal.j * tangent.i - facing_normal.i * tangent.j;
						normalize3d(&orientation);
					}
					break;
				}

				case _contrail_render_type_ground:
				default:
					error(
						_error_silent,
						"contrail %s uses an unsupported render type.",
						tag_get_name(contrail->definition_index));
					/*
					 * Original bug preserved: this return bypasses the buffer
					 * unlock/delete calls and leaves current_lock_operation set.
					 * A non-matching safety fix would route this arm through the
					 * common cleanup at the end of the function.
					 */
					return;
				}

				color.alpha *= contrail_fade(
					definition,
					shader->framebuffer_fade_mode,
					&point->position,
					&orientation);
				color.alpha = PIN(color.alpha, 0.f, 1.f);
				vertices[0].color = vertices[1].color = real_argb_color_to_pixel32(&color);
				texture_u += texture_u_step;
				previous_point = point;
				vertices += 2;
				point_index = point->next_contrail_point_index;
			}

			vertices -= vertex_count;
			first_vertex = vertices;
			if (!(definition->flags & (1 << _contrail_first_point_unfaded_bit)))
			{
				((byte *)&first_vertex[0].color)[3] = 0;
				((byte *)&first_vertex[1].color)[3] = 0;
			}
			if (!(definition->flags & (1 << _contrail_last_point_unfaded_bit)))
			{
				((byte *)&vertices[vertex_count - 1].color)[3] = 0;
				((byte *)&vertices[vertex_count - 2].color)[3] = 0;
			}
			if (segment_count > 0)
			{
				short segment_index;
				for (segment_index = 0; segment_index < segment_count; segment_index++)
				{
					triangles[0] = 2 * segment_index;
					triangles[1] = 2 * segment_index + 1;
					triangles[2] = 2 * segment_index + 2;
					triangles += 3;
					triangles[0] = 2 * segment_index + 2;
					triangles[1] = 2 * segment_index + 1;
					triangles[2] = 2 * segment_index + 3;
					triangles += 3;
				}
			}
			{
				real one_over_point_count = 1.f / contrail->contrail_point_counts[instance_index];
				average_position.x *= one_over_point_count;
				average_position.y *= one_over_point_count;
				average_position.z *= one_over_point_count;
			}
			rasterizer_dynamic_triangles_unlock(triangle_buffer_index);
			rasterizer_dynamic_vertices_unlock(vertex_buffer_index);
			rasterizer_dynamic_unlit_geometry_draw(
				(struct shader const *)shader,
				bitmap,
				0,
				triangle_buffer_index,
				vertex_buffer_index,
				triangle_count,
				&average_position,
				0);
			rasterizer_dynamic_triangles_delete(triangle_buffer_index);
			rasterizer_dynamic_vertices_delete(vertex_buffer_index);
		}
	}
	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	return;
}

void render_contrails(
	unsigned long render_mode_flags)
{
	if (render_contrails_enabled)
	{
		long contrail_index;

		for (contrail_index = data_next_index(contrail_data, NONE);
			contrail_index != NONE;
			contrail_index = data_next_index(contrail_data, contrail_index))
		{
			short instance_index;
			struct contrail_datum *contrail = datum_get(contrail_data, contrail_index);
			struct contrail_definition *definition = contrail_definition_get(
				contrail->definition_index);

			for (instance_index = 0; instance_index < 4; instance_index++)
			{
				if (((1 << definition->render_type) & render_mode_flags) &&
					contrail->contrail_point_counts[instance_index] >= 2)
				{
					render_contrail(contrail, definition, instance_index);
				}
			}
		}
	}

	return;
}

void render_contrails_ground_mapped(
	void)
{
	render_contrails(_contrail_render_mode_ground_mapped);
	return;
}

void render_contrails_media_mapped(
	void)
{
	render_contrails(_contrail_render_mode_media_mapped);
	return;
}

void render_contrails_normal(
	void)
{
	render_contrails(_contrail_render_mode_normal);
	return;
}

/* ---------- private code */
