/*
RENDER_SPRITE.C

symbols in this file:
0017C760 0130:
	_code_0017c760 (0000)
0017C890 0070:
	_code_0017c890 (0000)
0017C900 0090:
	_build_sprite_prepare_for_window (0000)
0017C990 0180:
	_code_0017c990 (0000)
0017CB10 00a0:
	_build_sprites_begin (0000)
0017CBB0 0130:
	_build_sprites_end (0000)
0017CCE0 01e0:
	_code_0017cce0 (0000)
0017CEC0 0070:
	_build_sprite_compute_vertex_fade (0000)
0017CF30 0610:
	_build_sprite (0000)
0017D540 0280:
	_build_sprite_rotational (0000)
002A00F4 0004:
	_one_over_full_circle (0000)
002A00F8 001b:
	??_C@_0BL@MGMOMNLP@perpendicular?5to?5direction?$AA@ (0000)
002A0114 0016:
	??_C@_0BG@KINAGGNG@parallel?5to?5direction?$AA@ (0000)
002A012C 000e:
	??_C@_0O@GDBPHPAN@screen?5facing?$AA@ (0000)
002A013C 0019:
	??_C@_0BJ@NOODAMK@?$CBuntransformed_direction?$AA@ (0000)
002A0158 0016:
	??_C@_0BG@PMLDPFAJ@transformed_direction?$AA@ (0000)
002A0170 0013:
	??_C@_0BD@IDBJDPKL@transformed_origin?$AA@ (0000)
002A0184 0015:
	??_C@_0BF@ILAHHFNF@untransformed_origin?$AA@ (0000)
002A019C 0026:
	??_C@_0CG@EFAPFCGE@c?3?2halo?2SOURCE?2render?2render_spr@ (0000)
002A01C4 0022:
	??_C@_0CC@MKHFOGBB@?5?5?5coverage?3?5?$CF?41f?5big?5sprites?3?5?$CF@ (0000)
002A01E8 0031:
	??_C@_0DB@IJJJGION@build_sprite?5failed?5to?5allocate?5@ (0000)
002A021C 0010:
	??_C@_0BA@BMIFFLPI@group?9?$DOvertices?$AA@ (0000)
002A022C 003d:
	??_C@_0DN@EKFLFNFF@a?5build_sprites_begin?5call?5can?5a@ (0000)
002A026C 002c:
	??_C@_0CM@LNKKNFPA@?$CBTEST_FLAG?$CIflags?0?5_build_sprites@ (0000)
002A0298 0041:
	??_C@_0EB@NNMLIIBJ@?$CD?$CD?$CD?5ERROR?5sprites?5rendered?5with?5@ (0000)
002A02DC 0031:
	??_C@_0DB@ILMICBDJ@TEST_FLAG?$CIdata?9?$DOflags?0?5_build_sp@ (0000)
002A0310 003b:
	??_C@_0DL@OIGDNGND@build_sprite?5only?5supports?5norma@ (0000)
002A034C 0024:
	??_C@_0CE@EJDAIAFL@build_sprite?5sprite?5count?5exceed@ (0000)
002A0370 004c:
	??_C@_0EM@DGAJPPIN@the?5bitmap?5group?5?$CFs?5sequence?5?$CFd?5@ (0000)
002A03C0 0068:
	??_C@_0GI@GFCEGJMK@mode?$DN?$DN_build_sprite_normal?5?$HM?$HM?5?$CIu@ (0000)
002A0428 0004:
	__real@3ecf817a (0000)
002A042C 001f:
	??_C@_0BP@HLIODOFJ@untransformed_axis_of_rotation?$AA@ (0000)
0030E778 0018:
	_data_0030e778 (0000)
	_global_sprite_render_orientations_enum (000c)
004C0518 0001:
	_bss_004c0518 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "bitmaps/bitmap_group.h"
#include "cseries/errors.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "math/real_math.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "render/render_cameras_internal.h"
#include "render/render_debug.h"
#include "render/render_sprite.h"
#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum build_sprites_internal_flags
{
	_build_sprites_valid_bit = NUMBER_OF_BUILD_SPRITES_FLAGS,
};

enum
{
	MAXIMUM_BUILD_SPRITE_GROUPS = 8,
};

enum
{
	_shader_effect_uses_nonlinear_tint_bit = 1,
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
	_rasterizer_geometry_parts_define_local_nodes_bit,
	NUMBER_OF_RASTERIZER_GEOMETRY_FLAGS
};

/* ---------- macros */

/* ---------- structures */

struct build_sprite_vertex
{
	real_point3d point;
	real_point2d texture_coordinates;
	pixel32 color;
};

typedef char build_sprite_vertex_size_assert[
	sizeof(struct build_sprite_vertex) == 0x18 ? 1 : -1];

struct build_sprite_globals_data
{
	boolean initialized;
	boolean debug_flag;
	word pad02;
	real screen_coverage;
	short big_sprite_count;
	word pad0A;
	real screen_area_scale;
	real_vector3d viewer_space_world_up;
	real_vector3d viewer_space_world_forward;
};

typedef char build_sprite_globals_data_size_assert[
	sizeof(struct build_sprite_globals_data) == 0x28 ? 1 : -1];

/* ---------- prototypes */

void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load);

static void build_sprite_transform_origin_and_direction(
	struct build_sprite_data const *data,
	unsigned long flags,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_direction,
	real_point3d *transformed_origin,
	real_vector3d *transformed_direction);
static void build_sprite_compute_scale(
	struct build_sprite_data const *data,
	short mode,
	unsigned long flags,
	real_point3d const *origin,
	struct bitmap_data const *bitmap,
	real *scale);
static short build_sprite_get_group(
	struct build_sprite_data *data,
	struct bitmap_data *bitmap);
static void build_sprite_compute_basis(
	struct build_sprite_data const *data,
	short mode,
	unsigned long flags,
	real_point3d const *origin,
	real_vector3d const *transformed_direction,
	real_matrix4x3 *basis);

/* ---------- globals */

extern boolean debug_sprites;
extern struct build_sprite_globals_data build_sprite_globals;

static char *sprite_render_orientation_names[NUMBER_OF_BUILD_SPRITE_ORIENTATIONS] =
{
	"screen facing",
	"parallel to direction",
	"perpendicular to direction",
};

struct tag_enum_definition global_sprite_render_orientations_enum =
{
	NUMBER_OF_BUILD_SPRITE_ORIENTATIONS,
	sprite_render_orientation_names,
	NULL,
};

real const one_over_full_circle = 1.f / (2.f*_pi);

/* ---------- public code */

void build_sprites_begin(
	struct build_sprite_data *data,
	short maximum_sprite_count,
	long bitmap_group_index,
	struct shader_effect_definition const *shader,
	unsigned long flags)
{
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 335, shader);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_sprite.c",
		336,
		!TEST_FLAG(flags, _build_sprites_valid_bit));

	data->bitmap_group_index = bitmap_group_index;
	data->flags = flags;
	data->shader = shader;
	data->group_count = 0;
	data->sprite_count = 0;
	data->maximum_sprite_count = maximum_sprite_count;
	data->centroid = *global_origin3d;
	SET_FLAG(data->flags, _build_sprites_valid_bit, TRUE);
	return;
}

void build_sprites_end(
	struct build_sprite_data *data)
{
	real one_over_sprite_count = 1.f / data->sprite_count;
	short group_index;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_sprite.c",
		358,
		TEST_FLAG(data->flags, _build_sprites_valid_bit));

	data->centroid.x *= one_over_sprite_count;
	data->centroid.y *= one_over_sprite_count;
	data->centroid.z *= one_over_sprite_count;
	matrix4x3_transform_point(&render.frustum.view_to_world, &data->centroid, &data->centroid);

	for (group_index = 0; group_index < data->group_count; group_index++)
	{
		struct build_sprite_group *group = &data->groups[group_index];

		if (group->sprite_count)
		{
			rasterizer_dynamic_vertices_unlock(group->vertex_buffer_index);
			if (TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
			{
				match_vassert(
					"c:\\halo\\SOURCE\\render\\render_sprite.c",
					377,
					FALSE,
					"### ERROR sprites rendered with screen geometry -- tell Bernie!!");
				rasterizer_dynamic_screen_geometry_draw(
					0,
					-NUMBER_OF_VERTICES_PER_QUADRILATERAL,
					group->vertex_buffer_index,
					2 * group->sprite_count);
			}
			else
			{
				unsigned long geometry_flags = FLAG(_rasterizer_geometry_viewspace_bit);

				SET_FLAG(
					geometry_flags,
					_rasterizer_geometry_first_person_bit,
					TEST_FLAG(data->flags, _build_sprites_first_person_bit));
				rasterizer_dynamic_unlit_geometry_draw(
					(struct shader const *)data->shader,
					group->bitmap,
					NULL,
					-NUMBER_OF_VERTICES_PER_QUADRILATERAL,
					group->vertex_buffer_index,
					2 * group->sprite_count,
					&data->centroid,
					geometry_flags);
			}
			rasterizer_dynamic_vertices_delete(group->vertex_buffer_index);
		}
	}

	SET_FLAG(data->flags, _build_sprites_valid_bit, FALSE);
	return;
}

void build_sprite_prepare_for_window(void)
{
	char string[512];

	if (debug_sprites)
	{
		sprintf(
			string,
			"   coverage: %.1f big sprites: %d",
			build_sprite_globals.screen_coverage,
			build_sprite_globals.big_sprite_count);
		render_debug_string(FALSE, string);
	}

	build_sprite_globals.screen_coverage = 0.f;
	build_sprite_globals.big_sprite_count = 0;
	matrix4x3_transform_normal(
		&render.frustum.world_to_view,
		global_up3d,
		&build_sprite_globals.viewer_space_world_up);
	matrix4x3_transform_normal(
		&render.frustum.world_to_view,
		global_left3d,
		&build_sprite_globals.viewer_space_world_forward);
	return;
}

real build_sprite_compute_vertex_fade(
	short fade_mode,
	real_point3d const *viewer_space_point,
	real_vector3d const *viewer_space_normal)
{
	real fade = 1.f;

	if (fade_mode)
	{
		fade = fabs(dot_product3d(viewer_space_normal, (real_vector3d const *)viewer_space_point) /
			magnitude3d((real_vector3d const *)viewer_space_point));

		if (fade_mode == 2)
			fade = 1.f - fade;
	}

	return fade;
}

void build_sprite(
	struct build_sprite_data *data,
	short mode,
	short sequence_index,
	short sprite_index,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_direction,
	real rotation,
	real scale,
	real_argb_color const *color,
	real fade,
	unsigned long flags)
{
	struct bitmap_group *bitmap_group = bitmap_group_get(data->bitmap_group_index);

	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 418, untransformed_origin);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_sprite.c",
		419,
		mode==_build_sprite_normal ||
			(untransformed_direction && magnitude_squared3d(untransformed_direction)));

	if (!color)
		color = global_real_argb_white;

	if (data->sprite_count<data->maximum_sprite_count)
	{
		if (sequence_index>=0 && sequence_index<bitmap_group->sequences.count)
		{
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group->sequences,
			sequence_index,
			struct bitmap_group_sequence);

		if (sequence->first_bitmap_index!=NONE &&
			sprite_index>=0 && sprite_index<sequence->sprites.count)
		{
			struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
				&sequence->sprites,
				sprite_index,
				struct bitmap_group_sprite);
			struct bitmap_data *bitmap;
			short group_index;

			match_vassert(
				"c:\\halo\\SOURCE\\render\\render_sprite.c",
				435,
				sprite->bitmap_index!=NONE,
				csprintf(
					temporary,
					"the bitmap group %s sequence %d sprite %d references bitmap -1 (tell matt).",
					tag_get_name(data->bitmap_group_index),
					sequence_index,
					sprite_index));

			bitmap = TAG_BLOCK_GET_ELEMENT(
				&bitmap_group->bitmaps,
				sprite->bitmap_index,
				struct bitmap_data);
			group_index = build_sprite_get_group(data, bitmap);
			if (group_index!=NONE)
			{
				struct build_sprite_group *group = &data->groups[group_index];

				if (group->sprite_count<data->maximum_sprite_count)
				{
					short vertex_index = NUMBER_OF_VERTICES_PER_QUADRILATERAL*group->sprite_count;
					real rotation_sine = 0.f;
					real rotation_cosine = 1.f;
					real_rectangle3d bounds = *global_null_rectangle3d;
					real_point3d transformed_origin;
					real_vector3d transformed_direction;
					real_matrix4x3 basis;
					pixel32 pixel;
					real alpha;
					short vertex;

					if (rotation!=0.f)
					{
						rotation_sine = sine(rotation);
						rotation_cosine = cosine(rotation);
					}

					build_sprite_transform_origin_and_direction(
						data,
						flags,
						untransformed_origin,
						untransformed_direction,
						&transformed_origin,
						&transformed_direction);
					build_sprite_compute_basis(
						data,
						mode,
						flags,
						&transformed_origin,
						&transformed_direction,
						&basis);
					build_sprite_compute_scale(data, mode, flags, &transformed_origin, bitmap, &scale);

					if (data->shader &&
						data->shader->framebuffer_fade_mode &&
						mode!=_build_sprite_normal)
					{
						cross_product3d(&basis.forward, &basis.left, &basis.up);
						fade *= build_sprite_compute_vertex_fade(
							data->shader->framebuffer_fade_mode,
							&transformed_origin,
							&basis.up);
					}

					pixel = real_argb_color_to_pixel32(color);
					if (data->shader &&
						data->shader->framebuffer_blend_function &&
						!TEST_FLAG(
							data->shader->flags,
							_shader_effect_uses_nonlinear_tint_bit))
					{
						alpha = fade*255.f;
					}
					else
					{
						alpha = (real)(pixel>>24)*fade;
					}
					pixel = (pixel & 0x00ffffff) | ((unsigned long)(byte)alpha<<24);

					for (vertex = 0; vertex<NUMBER_OF_VERTICES_PER_QUADRILATERAL; vertex++)
					{
						real u = (((vertex>>1)^vertex)&1)
							? sprite->bounds.x1
							: sprite->bounds.x0;
						real v = (vertex&2) ? sprite->bounds.y0 : sprite->bounds.y1;
						real offset_x = u - (sprite->bounds.x0 + sprite->registration_point.x);
						real offset_y = (sprite->registration_point.y + sprite->bounds.y0) - v;
						real x = offset_x*rotation_cosine - offset_y*rotation_sine;
						real y = offset_y*rotation_cosine + offset_x*rotation_sine;

						if (TEST_FLAG(flags, _build_sprite_u_mirror_bit))
							x = -x;
						if (TEST_FLAG(flags, _build_sprite_v_mirror_bit))
							y = -y;

						if (TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
						{
							struct dynamic_screen_vertex *screen_vertex =
								&((struct dynamic_screen_vertex *)group->vertices)[vertex_index];

							screen_vertex->position.x = x*scale + transformed_origin.x;
							screen_vertex->position.y = y*scale + transformed_origin.y;
							screen_vertex->texture_coordinates.x = u;
							screen_vertex->texture_coordinates.y = v;
							screen_vertex->color = pixel;
						}
						else
						{
							struct build_sprite_vertex *sprite_vertex =
								&((struct build_sprite_vertex *)group->vertices)[vertex_index];
							real_point3d point;

							point.x = (basis.forward.i*x + basis.left.i*y)*scale +
								transformed_origin.x;
							point.y = (basis.forward.j*x + basis.left.j*y)*scale +
								transformed_origin.y;
							point.z = (basis.forward.k*x + basis.left.k*y)*scale +
								transformed_origin.z;
							if (point.x<bounds.x0)
								bounds.x0 = point.x;
							if (point.x>bounds.x1)
								bounds.x1 = point.x;
							if (point.y<bounds.y0)
								bounds.y0 = point.y;
							if (point.y>bounds.y1)
								bounds.y1 = point.y;
							if (point.z<bounds.z0)
								bounds.z0 = point.z;
							if (point.z>bounds.z1)
								bounds.z1 = point.z;
							sprite_vertex->point = point;
							sprite_vertex->texture_coordinates.x = u;
							sprite_vertex->texture_coordinates.y = v;
							sprite_vertex->color = pixel;
						}
						vertex_index++;
					}

					data->centroid.x += transformed_origin.x;
					data->centroid.y += transformed_origin.y;
					data->centroid.z += transformed_origin.z;
					group->sprite_count++;
					data->sprite_count++;

					if (!TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
					{
						real coverage = render_frustum_cube_view_fraction(&render.frustum, &bounds);

						build_sprite_globals.screen_coverage += coverage;
						if (coverage>0.5f && build_sprite_globals.big_sprite_count++>10)
						{
							group->sprite_count--;
							data->sprite_count--;
						}

						if (debug_sprites)
						{
							struct build_sprite_vertex *vertices =
								(struct build_sprite_vertex *)group->vertices;
							real_point3d point0;
							real_point3d point1;
							real_point3d point2;
							real_point3d point3;

							matrix4x3_transform_point(
								&render.frustum.view_to_world,
								&vertices[vertex_index - 4].point,
								&point0);
							matrix4x3_transform_point(
								&render.frustum.view_to_world,
								&vertices[vertex_index - 3].point,
								&point1);
							matrix4x3_transform_point(
								&render.frustum.view_to_world,
								&vertices[vertex_index - 2].point,
								&point2);
							matrix4x3_transform_point(
								&render.frustum.view_to_world,
								&vertices[vertex_index - 1].point,
								&point3);
							rasterizer_debug_line(&point0, &point1, global_real_argb_white);
							rasterizer_debug_line(&point0, &point2, global_real_argb_white);
							rasterizer_debug_line(&point2, &point3, global_real_argb_white);
							rasterizer_debug_line(&point3, &point1, global_real_argb_white);
						}
					}
				}
			}
		}
		}
	}
	else
	{
		error(_error_silent, "build_sprite sprite count exceeded.");
	}
	return;
}

void build_sprite_rotational(
	struct build_sprite_data *data,
	unsigned long flags,
	short sequence_index,
	short sprite_index,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_axis_of_rotation,
	real rotation,
	real scale,
	real_argb_color const *color,
	real fade)
{
	real_point3d transformed_origin;
	real_vector3d transformed_axis_of_rotation;
	real const quarter_circle = _pi/2;
	real fraction;
	real angle;
	real sprite_rotation;

	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 646, data);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 647, untransformed_origin);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_sprite.c",
		648,
		untransformed_axis_of_rotation);

	if (!color)
		color = global_real_argb_white;

	build_sprite_transform_origin_and_direction(
		data,
		flags & FLAG(_build_sprite_rotational_viewer_space_bit),
		untransformed_origin,
		untransformed_axis_of_rotation,
		&transformed_origin,
		&transformed_axis_of_rotation);

	angle = angle_between_vectors3d(
		(real_vector3d const *)&transformed_origin,
		&transformed_axis_of_rotation) - quarter_circle;
	fraction = angle*angle/(quarter_circle*quarter_circle);
	fraction = PIN(fraction, 0.f, 1.f);

	if (fraction>0.05f)
	{
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group_get(data->bitmap_group_index)->sequences,
			sequence_index+1,
			struct bitmap_group_sequence);
		short sprite_count = (short)sequence->sprites.count;
		unsigned long edge_flags = FLAG(_build_sprite_viewer_space_bit);
		short edge_sprite_index;

		if (TEST_FLAG(flags, _build_sprite_rotational_sideways_rotation_animates_bit))
		{
			edge_sprite_index = (short)(fmod(
				sprite_count*one_over_full_circle*rotation + 0.5f,
				(real)sprite_count) + sprite_index);
			sprite_rotation = 0.f;
			if (angle<0.f)
				edge_sprite_index = sprite_count-sprite_index;
		}
		else
		{
			edge_sprite_index = sprite_index;
			sprite_rotation = rotation;
			if (angle<0.f)
				SET_FLAG(edge_flags, _build_sprite_u_mirror_bit, TRUE);
		}

		build_sprite(
			data,
			_build_sprite_normal,
			sequence_index+1,
			edge_sprite_index,
			&transformed_origin,
			NULL,
			sprite_rotation,
			scale,
			color,
			fraction*fade,
			edge_flags);
	}

	fraction = 1.f-fraction;
	if (fraction>0.05f)
	{
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group_get(data->bitmap_group_index)->sequences,
			sequence_index,
			struct bitmap_group_sequence);
		short sprite_count = (short)sequence->sprites.count;

		build_sprite(
			data,
			_build_sprite_normal,
			sequence_index,
			(short)fmod(
				sprite_count*one_over_full_circle*rotation + 0.5f,
				(real)sprite_count),
			&transformed_origin,
			NULL,
			arctangent(
				transformed_axis_of_rotation.j,
				transformed_axis_of_rotation.i),
			scale,
			color,
			fraction*fade,
			FLAG(_build_sprite_viewer_space_bit));
	}
	return;
}

/* ---------- private code */

static void build_sprite_transform_origin_and_direction(
	struct build_sprite_data const *data,
	unsigned long flags,
	real_point3d const *untransformed_origin,
	real_vector3d const *untransformed_direction,
	real_point3d *transformed_origin,
	real_vector3d *transformed_direction)
{
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 74, data);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 75, untransformed_origin);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 76, transformed_origin);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 77, transformed_direction);

	if (TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
	{
		real_vector3d unused_view_vector;

		render_camera_screen_to_view(
			&render.camera,
			&render.frustum,
			(real_point2d const *)untransformed_origin,
			&unused_view_vector);
		match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 84, !untransformed_direction);
	}
	else if (TEST_FLAG(flags, _build_sprite_viewer_space_bit))
	{
		*transformed_origin = *untransformed_origin;
		if (untransformed_direction)
			*transformed_direction = *untransformed_direction;
	}
	else
	{
		matrix4x3_transform_point(
			&render.frustum.world_to_view,
			untransformed_origin,
			transformed_origin);
		if (untransformed_direction)
			matrix4x3_transform_normal(
				&render.frustum.world_to_view,
				untransformed_direction,
				transformed_direction);
	}
	return;
}

static void build_sprite_compute_scale(
	struct build_sprite_data const *data,
	short mode,
	unsigned long flags,
	real_point3d const *origin,
	struct bitmap_data const *bitmap,
	real *scale)
{
	if (TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
	{
		if (*scale == 0.f)
			*scale = 1.f;
	}
	else if (mode == _build_sprite_normal && *scale == 0.f)
	{
		*scale = -(origin->z / render.frustum.projection_world_to_screen.i);
	}

	*scale = bitmap->width * *scale;
	return;
}

static short build_sprite_get_group(
	struct build_sprite_data *data,
	struct bitmap_data *bitmap)
{
	/* Name, type and function scope from the 2003 PC demo PDB and the HCEX PDB (static local
	   unsigned char warned). Neither PDB records the block: placing it at the top of the function
	   is unattested. January corroborates: its one-byte .bss is referenced only here. */
	static boolean warned;
	short group_index;

	for (group_index = 0; group_index < data->group_count; group_index++)
	{
		if (data->groups[group_index].bitmap == bitmap)
			break;
	}

	match_vassert(
		"c:\\halo\\SOURCE\\render\\render_sprite.c",
		275,
		group_index < data->group_count ||
			data->group_count < MAXIMUM_BUILD_SPRITE_GROUPS,
		csprintf(
			temporary,
			"a build_sprites_begin call can accomodate at most %d bitmaps",
			MAXIMUM_BUILD_SPRITE_GROUPS));

	if (group_index < data->group_count ||
		data->group_count < MAXIMUM_BUILD_SPRITE_GROUPS)
	{
		if (group_index >= data->group_count)
		{
			struct build_sprite_group *group = &data->groups[group_index];

			data->group_count++;
			group->bitmap = bitmap;
			if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
			{
				rasterizer_globals.current_lock_operation = _rasterizer_lock_sprite;
				group->vertex_buffer_index = rasterizer_dynamic_vertices_new(
					TEST_FLAG(data->flags, _build_sprites_screen_space_bit) ? 8 : 6,
					NUMBER_OF_VERTICES_PER_QUADRILATERAL*data->maximum_sprite_count);
				if (group->vertex_buffer_index != NONE)
				{
					group->vertices = rasterizer_dynamic_vertices_lock(
						group->vertex_buffer_index);
					match_assert(
						"c:\\halo\\SOURCE\\render\\render_sprite.c",
						294,
						group->vertices);
				}
				else
				{
					if (!warned)
					{
						error(
							_error_silent,
							"build_sprite failed to allocate dynamic vertices");
						warned = TRUE;
					}
					group->vertices = NULL;
				}
				rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
			}
			else
			{
				group->vertices = NULL;
			}
			group->sprite_count = 0;
		}
	}
	else
	{
		group_index = NONE;
	}

	if (group_index != NONE && !data->groups[group_index].vertices)
	{
		group_index = NONE;
	}

	return group_index;
}

static void build_sprite_compute_basis(
	struct build_sprite_data const *data,
	short mode,
	unsigned long flags,
	real_point3d const *origin,
	real_vector3d const *transformed_direction,
	real_matrix4x3 *basis)
{
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 116, data);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 117, origin);
	match_assert("c:\\halo\\SOURCE\\render\\render_sprite.c", 118, basis);

	if (TEST_FLAG(data->flags, _build_sprites_screen_space_bit))
	{
		match_vassert(
			"c:\\halo\\SOURCE\\render\\render_sprite.c",
			122,
			mode == _build_sprite_normal,
			"build_sprite only supports normal sprites in screen space.");
	}
	else if (mode == _build_sprite_normal)
	{
		basis->forward.i = 1.f;
		basis->forward.j = 0.f;
		basis->forward.k = 0.f;
		basis->left.i = 0.f;
		basis->left.j = 1.f;
		basis->left.k = 0.f;
	}
	else if (mode == _build_sprite_parallel)
	{
		basis->forward = *transformed_direction;
		normalize3d(&basis->forward);
		cross_product3d((real_vector3d const *)origin, &basis->forward, &basis->left);
		normalize3d(&basis->left);
	}
	else if (mode == _build_sprite_perpendicular)
	{
		real_vector3d const *reference = &build_sprite_globals.viewer_space_world_up;
		real dot = dot_product3d(transformed_direction, reference);

		if (dot*dot > magnitude_squared3d(transformed_direction)*0.99f)
			reference = &build_sprite_globals.viewer_space_world_forward;

		cross_product3d(transformed_direction, reference, &basis->forward);
		normalize3d(&basis->forward);
		basis->left = basis->forward;
		basis->up = *transformed_direction;
		normalize3d(&basis->up);
		rotate_vector_about_axis(&basis->left, &basis->up, -1.f, 0.f);
	}
	else
	{
		match_vassert("c:\\halo\\SOURCE\\render\\render_sprite.c", 172, FALSE, NULL);
	}
	return;
}
