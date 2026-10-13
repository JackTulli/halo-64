/*
WEATHER_PARTICLE_SYSTEMS.C

symbols in this file:
000924D0 0040:
	_weather_particle_system_get (0000)
00092510 0050:
	_weather_particle_system_get_type (0000)
00092560 0030:
	_weather_particle_systems_initialize (0000)
00092590 0070:
	_weather_particle_systems_initialize_for_new_map (0000)
00092600 0020:
	_weather_particle_systems_dispose_from_old_map (0000)
00092620 0020:
	_weather_particle_systems_dispose (0000)
00092640 0030:
	_weather_particle_system_type_delete_particle (0000)
00092670 00a0:
	_weather_particle_system_wrap_point (0000)
00092710 0160:
	_weather_particle_system_new (0000)
00092870 0110:
	_weather_particle_system_delete (0000)
00092980 02c0:
	_weather_particle_system_new_particle (0000)
00092C40 0040:
	_weather_particle_system_box_offset_from_point3d (0000)
00092C80 02b0:
	_weather_particle_update_physics (0000)
00092F30 0100:
	_weather_particle_system_build_clipping_planes (0000)
00093030 0040:
	_weather_particle_system_transform_clip_planes_to_box (0000)
00093070 00b0:
	_weather_polyhedra_find (0000)
00093120 0130:
	_weather_particle_system_update_particle_count (0000)
00093250 0240:
	_weather_particle_system_update (0000)
00093490 0690:
	_weather_particle_system_render (0000)
00093B20 00e0:
	_weather_particle_systems_render (0000)
0025AAFC 0004:
	_one_over_char_max (0000)
0025AB00 0032:
	??_C@_0DC@LLJNAPDG@c?3?2halo?2SOURCE?2effects?2weather_p@ (0000)
0025AB34 003d:
	??_C@_0DN@CIHCKGAO@type_index?$DO?$DN0?5?$CG?$CG?5type_index?$DMdefi@ (0000)
0025AB74 0033:
	??_C@_0DD@FLIDKJLH@couldn?8t?5allocate?5weather?5partic@ (0000)
0025ABA8 0012:
	??_C@_0BC@EPMBJEML@weather?5particles?$AA@ (0000)
0025ABBC 001f:
	??_C@_0BP@EMHAEIEA@system?9?$DOdefinition_index?$DN?$DNNONE?$AA@ (0000)
0025ABDC 0024:
	??_C@_0CE@MLAKBDEP@too?5many?5weather?5polyhedra?5visib@ (0000)
0025AC00 003b:
	??_C@_0DL@JLJDEDI@box_count?$DMMAXIMUM_NUMBER_OF_VISI@ (0000)
002DDDAE 0001:
	_weather (0000)
0043D590 0274:
	_weather_particle_system_globals (0000)
*/

/* ---------- headers */

#include "effects/weather_particle_systems.h"

#include <math.h>

#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "cseries/errors.h"
#include "effects/weather_particle_definitions.h"
#include "math/real_math.h"
#include "networking/network_connection.h"
#include "objects/objects.h"
#include "physics/point_physics.h"
#include "render/render.h"
#include "render/render_cameras_internal.h"
#include "render/render_sprite.h"
#include "scenario/scenario.h"
#include "shaders/shader_definitions.h"
#include "structures/structure_bsp_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MAXIMUM_NUMBER_OF_WEATHER_PARTICLES = 512,
	MAXIMUM_NUMBER_OF_WEATHER_PARTICLE_TYPES = 8,
	MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_POLYHEDRA = 8,
	MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_PARTICLE_BOXES = 26,
	NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES = 5,
};

enum
{
	_weather_particle_type_interpolate_colors_in_hsv_bit = 0,
	_weather_particle_type_along_long_hue_path_bit,
	_weather_particle_type_random_rotation_bit,
	NUMBER_OF_WEATHER_PARTICLE_TYPE_FLAGS,
};

enum
{
	_weather_particle_direction_from_velocity = 0,
	_weather_particle_direction_from_acceleration,
	NUMBER_OF_WEATHER_PARTICLE_DIRECTION_SOURCES,
};

enum
{
	_weather_particle_clipping_plane_left = 0,
	_weather_particle_clipping_plane_right,
	_weather_particle_clipping_plane_top,
	_weather_particle_clipping_plane_bottom,
	_weather_particle_clipping_plane_far,
};

/* ---------- macros */

/* ---------- structures */

/*
 * January's tag-definition consumer layout.  The 0x25C stride and every field
 * used below are independently corroborated by the HCEA symbols build.
 */
struct weather_particle_type_definition
{
	char name[32];
	unsigned long flags;
	real distance_fadein_start;
	real distance_fadein_end;
	real distance_fadeout_start;
	real distance_fadeout_end;
	real height_fadein_start;
	real height_fadein_end;
	real height_fadeout_start;
	real height_fadeout_end;
	long unused[24];
	real particle_count_lower_bound;
	real particle_count_upper_bound;
	struct tag_reference physics;
	long unused2[4];
	real acceleration_lower_bound;
	real acceleration_upper_bound;
	real acceleration_turning_rate;
	real acceleration_change_rate;
	long unused3[8];
	real radius_lower_bound;
	real radius_upper_bound;
	real animation_rate_lower_bound;
	real animation_rate_upper_bound;
	real rotation_rate_lower_bound;
	real rotation_rate_upper_bound;
	long unused4[8];
	real_argb_color color_lower_bound;
	real_argb_color color_upper_bound;
	real runtime_one_over_sprite_width;
	long unused5[15];
	struct tag_reference bitmap;
	short render_mode;
	short render_direction_source;
	struct shader_effect_definition shader;
};

typedef char weather_particle_type_definition_size_assert[
	sizeof(struct weather_particle_type_definition) == 0x25C ? 1 : -1];

struct weather_particle_type
{
	real density;
	real box_width;
	short particle_count;
	word pad0A;
	long first_particle_index;
};

struct weather_particle_system
{
	long definition_index;
	real time;
	real time_delta_sec;
	real scale;
	struct location location;
	short weather_palette_index;
	boolean under_water;
	byte pad1B;
	struct weather_particle_type types[MAXIMUM_NUMBER_OF_WEATHER_PARTICLE_TYPES];
};

struct weather_particle
{
	short identifier;
	word pad02;
	real_point3d position;
	real_vector3d velocity;
	real_vector3d acceleration;
	short sequence_index;
	word pad2A;
	real sprite_index;
	real rotation;
	real_argb_color color;
	real radius;
	real rotation_rate;
	real animation_rate;
	long next_particle_index;
};

struct weather_particle_system_globals
{
	short active_system_count;
	word pad02;
	struct weather_particle_system systems[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
};

typedef char weather_particle_type_size_assert[
	sizeof(struct weather_particle_type) == 0x10 ? 1 : -1];
typedef char weather_particle_system_size_assert[
	sizeof(struct weather_particle_system) == 0x9C ? 1 : -1];
typedef char weather_particle_size_assert[
	sizeof(struct weather_particle) == 0x54 ? 1 : -1];
typedef char weather_particle_system_globals_size_assert[
	sizeof(struct weather_particle_system_globals) == 0x274 ? 1 : -1];

/*
 * These three tag-block element layouts are private here because their owner
 * headers do not publish them yet.  Their names, sizes, and accessed offsets
 * are corroborated by HCEA and sibling January consumers.
 */
struct structure_weather_palette_entry
{
	char name[32];
	struct tag_reference particle_system;
	word pad30;
	short runtime_particle_system_global_function_index;
	char particle_system_global_function_name[32];
	long particle_system_unused[11];
	struct tag_reference wind;
	real_vector3d wind_direction;
	real wind_magnitude;
	word padA0;
	short wind_global_function_index;
	char wind_global_function_name[32];
	long wind_unused[11];
};

struct structure_weather_polyhedron
{
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
	long unused;
	struct tag_block planes;
};

typedef char structure_weather_palette_entry_size_assert[
	sizeof(struct structure_weather_palette_entry) == 0xF0 ? 1 : -1];
typedef char structure_weather_polyhedron_size_assert[
	sizeof(struct structure_weather_polyhedron) == 0x20 ? 1 : -1];
/* ---------- prototypes */

static struct weather_particle_system *weather_particle_system_get(
	short local_player_index);
static struct weather_particle_type *weather_particle_system_get_type(
	struct weather_particle_system *system,
	short type_index);
static void weather_particle_system_type_delete_particle(
	struct weather_particle_type *type);
static void weather_particle_system_wrap_point(
	real box_width,
	real_point3d const *point,
	real_point3d *result);
static long weather_particle_system_new_particle(
	short local_player_index,
	short type_index);
static void weather_particle_system_box_offset_from_point3d(
	real box_width,
	real_point3d const *point,
	real_point3d *offset);
#ifdef HALO_LINUX
/* A frame was a tick on the Xbox; the native builds draw several frames per
tick (port/linux/game/render_interpolation.c). Weather particles turn their
acceleration and drift once an update: turn once per 30 Hz tick, and drift by
the frame's share of a tick. This is the ticks the current update crosses. */
static long weather_particle_update_ticks = 1;

#endif
static void weather_particle_update_physics(
	short local_player_index,
	short type_index,
	short particle_index);
static void weather_particle_system_build_clipping_planes(
	real_plane3d *planes,
	real far_clip_distance);
static void weather_particle_system_transform_clip_planes_to_box(
	real *plane_distances,
	real_plane3d const *planes,
	real_point3d const *box_position);
static short weather_polyhedra_find(
	short *polyhedron_indices,
	real maximum_distance);
static void weather_particle_system_update_particle_count(
	short local_player_index,
	short type_index,
	real scale);
static void weather_particle_system_update(
	short local_player_index);
static void weather_particle_system_render(
	short local_player_index);

/* ---------- globals */

real const one_over_char_max = 1.f/255.f;

boolean weather = TRUE;

static struct weather_particle_system_globals weather_particle_system_globals = { 0 };

/* ---------- public code */

void weather_particle_systems_initialize(
	void)
{
	weather_particle_data = data_new("weather particles", MAXIMUM_NUMBER_OF_WEATHER_PARTICLES, sizeof(struct weather_particle));
	if (!weather_particle_data)
		error(_error_immediate, "couldn't allocate weather particle system globals.");

	return;
}

void weather_particle_systems_initialize_for_new_map(
	void)
{
	short local_player_index;

	for (local_player_index = 0; local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; local_player_index++)
	{
		match_assert("c:\\halo\\SOURCE\\effects\\weather_particle_systems.c", 91, local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
		weather_particle_system_globals.systems[local_player_index].definition_index = NONE;
	}

	weather_particle_system_globals.active_system_count = 0;
	data_make_valid(weather_particle_data);

	return;
}

void weather_particle_systems_dispose_from_old_map(
	void)
{
	if (weather_particle_data->valid)
		data_make_invalid(weather_particle_data);

	return;
}

void weather_particle_systems_dispose(
	void)
{
	if (weather_particle_data)
	{
		data_dispose(weather_particle_data);
		weather_particle_data = NULL;
	}

	return;
}

void weather_particle_system_new(
	short local_player_index,
	long definition_index,
	real scale)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(definition_index);
	short type_index;

	match_assert("c:\\halo\\SOURCE\\effects\\weather_particle_systems.c", 177, system->definition_index==NONE);

	system->scale = scale;
	system->definition_index = definition_index;
	system->time = 0.f;
	system->time_delta_sec = 0.f;
	weather_particle_system_globals.active_system_count+= 1;

	for (type_index = 0; type_index<definition->particle_types.count; type_index++)
	{
		struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);
		struct weather_particle_type_definition *type_definition = TAG_BLOCK_GET_ELEMENT(
			&definition->particle_types,
			type_index,
			struct weather_particle_type_definition);

		type->first_particle_index = NONE;
		type->particle_count = 0;
		type->density = real_local_random_range(
			type_definition->particle_count_lower_bound,
			type_definition->particle_count_upper_bound);
		type->box_width = type_definition->distance_fadeout_end;
	}

	return;
}

static void weather_particle_system_build_clipping_planes(
	real_plane3d *planes,
	real far_clip_distance)
{
	real_plane3d *far_plane = &planes[_weather_particle_clipping_plane_far];
	real_point3d far_point;

	far_point.x = render.camera.forward.i*far_clip_distance + render.camera.position.x;
	far_point.y = render.camera.forward.j*far_clip_distance + render.camera.position.y;
	far_point.z = render.camera.forward.k*far_clip_distance + render.camera.position.z;
	far_plane->n = render.camera.forward;
	far_plane->d = far_plane->n.i*far_point.x + far_plane->n.j*far_point.y + far_plane->n.k*far_point.z;

	planes[_weather_particle_clipping_plane_left] = render.frustum.world_planes[0];
	planes[_weather_particle_clipping_plane_right] = render.frustum.world_planes[1];
	planes[_weather_particle_clipping_plane_top] = render.frustum.world_planes[2];
	planes[_weather_particle_clipping_plane_bottom] = render.frustum.world_planes[3];

	return;
}

static void weather_particle_system_transform_clip_planes_to_box(
	real *plane_distances,
	real_plane3d const *planes,
	real_point3d const *box_position)
{
	short plane_index;

	for (plane_index = 0; plane_index<NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES; plane_index++)
	{
		plane_distances[plane_index] =
			planes[plane_index].n.i*box_position->x +
			planes[plane_index].n.j*box_position->y +
			planes[plane_index].n.k*box_position->z;
	}

	return;
}

static short weather_polyhedra_find(
	short *polyhedron_indices,
	real maximum_distance)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	short polyhedron_count = 0;
	short polyhedron_index;

	for (polyhedron_index = 0; polyhedron_index<structure_bsp->weather_polyhedra.count; polyhedron_index++)
	{
		struct structure_weather_polyhedron *polyhedron = TAG_BLOCK_GET_ELEMENT(
			&structure_bsp->weather_polyhedra,
			polyhedron_index,
			struct structure_weather_polyhedron);

		if (distance_squared3d(&render.camera.position, &polyhedron->bounding_sphere_center)<
			(maximum_distance + polyhedron->bounding_sphere_radius)*(maximum_distance + polyhedron->bounding_sphere_radius))
		{
			if (polyhedron_count<MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_POLYHEDRA)
			{
				polyhedron_indices[polyhedron_count++] = polyhedron_index;
			}
			else
			{
				error(_error_silent, "too many weather polyhedra visible.");
			}
		}
	}

	return polyhedron_count;
}

static void weather_particle_system_update_particle_count(
	short local_player_index,
	short type_index,
	real scale)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
	struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);
	long target_particle_count = (long)(type->density*type->box_width*type->box_width*type->box_width*scale/weather_particle_system_globals.active_system_count);

	target_particle_count = MAX(0, target_particle_count);

	while (type->particle_count<target_particle_count)
	{
		if (weather_particle_system_new_particle(local_player_index, type_index)==NONE)
		{
			break;
		}
	}

	while (type->particle_count>target_particle_count)
	{
		weather_particle_system_type_delete_particle(type);
	}

	return;
}

static void weather_particle_system_update(
	short local_player_index)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
	short type_index;

	system->time_delta_sec = render.time_delta_since_tick_sec;
	system->time+= system->time_delta_sec;
#ifdef HALO_LINUX
	{
		static real leftover_ticks[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];

		leftover_ticks[local_player_index]+= system->time_delta_sec*TICKS_PER_SECOND;
		weather_particle_update_ticks = (long)leftover_ticks[local_player_index];
		leftover_ticks[local_player_index]-= (real)weather_particle_update_ticks;
	}
#endif

	for (type_index = 0; type_index<definition->particle_types.count; type_index++)
	{
		struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);
		struct weather_particle_type_definition *type_definition = TAG_BLOCK_GET_ELEMENT(
			&definition->particle_types,
			type_index,
			struct weather_particle_type_definition);
		struct bitmap_group *bitmap_group = bitmap_group_get(type_definition->bitmap.index);
		struct weather_particle *particle;
		real height = render.camera.position.z;
		real scale = system->scale;
		long particle_index;

		scale*= PIN(
			(height - type_definition->height_fadein_start)/(type_definition->height_fadein_end - type_definition->height_fadein_start),
			0.f, 1.f);
		scale*= 1.f - PIN(
			(height - type_definition->height_fadeout_start)/(type_definition->height_fadeout_end - type_definition->height_fadeout_start),
			0.f, 1.f);
		weather_particle_system_update_particle_count(local_player_index, type_index, scale);

		for (particle_index = type->first_particle_index; particle_index!=NONE; particle_index = particle->next_particle_index)
		{
			particle = datum_get(weather_particle_data, particle_index);
			particle->sprite_index = (real)fmod(
				particle->sprite_index+= particle->animation_rate*system->time_delta_sec,
				TAG_BLOCK_GET_ELEMENT(&bitmap_group->sequences, particle->sequence_index, struct bitmap_group_sequence)->sprites.count);
			particle->rotation+= (particle_index&1 ? -1 : 1)*particle->rotation_rate*system->time_delta_sec;
			weather_particle_update_physics(local_player_index, type_index, particle_index);
		}
	}

	return;
}

static void weather_particle_system_render(
	short local_player_index)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	short type_index;

	weather_particle_system_update(local_player_index);

	for (type_index = 0; type_index<definition->particle_types.count; type_index++)
	{
		struct weather_particle_type *type = &system->types[type_index];
		struct weather_particle_type_definition *type_definition = TAG_BLOCK_GET_ELEMENT(
			&definition->particle_types,
			type_index,
			struct weather_particle_type_definition);

		if (type->particle_count)
		{
			real_plane3d clipping_planes[NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES];
			real_point3d box_positions[MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_PARTICLE_BOXES];
			real box_plane_distances[MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_PARTICLE_BOXES][NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES];
			short visible_polyhedron_indices[MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_POLYHEDRA];
			short visible_polyhedron_count = weather_polyhedra_find(visible_polyhedron_indices, type->box_width);
			struct build_sprite_data sprite_data;
			struct weather_particle *particle;
			real box_offsets[3];
			real_rectangle3d bounds;
			short box_count;
			short i, j, k;
			long particle_index;

			weather_particle_system_build_clipping_planes(clipping_planes, type->box_width);
			weather_particle_system_box_offset_from_point3d(type->box_width, &render.camera.position, &box_positions[0]);
			weather_particle_system_transform_clip_planes_to_box(box_plane_distances[0], clipping_planes, &box_positions[0]);

			bounds.x0 = box_positions[0].x;
			bounds.x1 = box_positions[0].x + type->box_width;
			bounds.y0 = box_positions[0].y;
			bounds.y1 = box_positions[0].y + type->box_width;
			bounds.z0 = box_positions[0].z;
			bounds.z1 = box_positions[0].z + type->box_width;

			box_offsets[0] = -type->box_width;
			box_offsets[1] = 0.f;
			box_offsets[2] = type->box_width;

			box_count = 1;
			for (i = 0; i<3; i++)
			{
				for (j = 0; j<3; j++)
				{
					for (k = 0; k<3; k++)
					{
						if (i!=1 || j!=1 || k!=1)
						{
							real_rectangle3d box_bounds;

							box_bounds.x0 = bounds.x0 + box_offsets[i];
							box_bounds.x1 = bounds.x1 + box_offsets[i];
							box_bounds.y0 = bounds.y0 + box_offsets[j];
							box_bounds.y1 = bounds.y1 + box_offsets[j];
							box_bounds.z0 = bounds.z0 + box_offsets[k];
							box_bounds.z1 = bounds.z1 + box_offsets[k];

							if (render_frustum_cube_visible(&render.frustum, &box_bounds, TRUE))
							{
								match_assert("c:\\halo\\SOURCE\\effects\\weather_particle_systems.c", 673, box_count<MAXIMUM_NUMBER_OF_VISIBLE_WEATHER_PARTICLE_BOXES);
								box_positions[box_count].x = box_bounds.x0;
								box_positions[box_count].y = box_bounds.y0;
								box_positions[box_count].z = box_bounds.z0;
								weather_particle_system_transform_clip_planes_to_box(box_plane_distances[box_count], clipping_planes, &box_positions[box_count]);
								box_count++;
							}
						}
					}
				}
			}

			build_sprites_begin(&sprite_data, type->particle_count, type_definition->bitmap.index, &type_definition->shader, 0);

			for (particle_index = type->first_particle_index; particle_index!=NONE; particle_index = particle->next_particle_index)
			{
				real particle_plane_distances[NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES];
				short plane_index;
				short box_index;

				particle = datum_get(weather_particle_data, particle_index);

				for (plane_index = 0; plane_index<NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES; plane_index++)
				{
					particle_plane_distances[plane_index] = plane3d_distance_to_point(&clipping_planes[plane_index], &particle->position);
				}

				for (box_index = 0; box_index<box_count; box_index++)
				{
					boolean visible = TRUE;

					for (plane_index = 0; plane_index<NUMBER_OF_WEATHER_PARTICLE_CLIPPING_PLANES && visible; plane_index++)
					{
						visible = box_plane_distances[box_index][plane_index] + particle_plane_distances[plane_index]<0.f;
					}

					if (visible)
					{
						break;
					}
				}

				if (box_index<box_count)
				{
					real far_clip_distance = MIN(type_definition->distance_fadeout_end, type->box_width);
					real_point3d position;
					real_vector3d camera_to_position;
					real depth;

					position.x = box_positions[box_index].x + particle->position.x;
					position.y = box_positions[box_index].y + particle->position.y;
					position.z = box_positions[box_index].z + particle->position.z;
					depth = dot_product3d(vector_from_points3d(&render.camera.position, &position, &camera_to_position), &render.camera.forward);

					if (depth>type_definition->distance_fadein_start && depth<far_clip_distance)
					{
						real fade_in = PIN(
							(depth - type_definition->distance_fadein_start)/(type_definition->distance_fadein_end - type_definition->distance_fadein_start),
							0.f, 1.f);
						real fade_out = 1.f - PIN(
							(depth - type_definition->distance_fadeout_start)/(far_clip_distance - type_definition->distance_fadeout_start),
							0.f, 1.f);
						short polyhedron_index;

						for (polyhedron_index = 0; polyhedron_index<visible_polyhedron_count; polyhedron_index++)
						{
							struct structure_weather_polyhedron *polyhedron = TAG_BLOCK_GET_ELEMENT(
								&structure_bsp->weather_polyhedra,
								visible_polyhedron_indices[polyhedron_index],
								struct structure_weather_polyhedron);
							short polyhedron_plane_index;

							for (polyhedron_plane_index = 0; polyhedron_plane_index<polyhedron->planes.count; polyhedron_plane_index++)
							{
								real_plane3d *plane = TAG_BLOCK_GET_ELEMENT(&polyhedron->planes, polyhedron_plane_index, real_plane3d);

								if (plane3d_distance_to_point(plane, &position)<0.f)
								{
									break;
								}
							}

							if (polyhedron_plane_index==polyhedron->planes.count)
							{
								break;
							}
						}

						if (polyhedron_index==visible_polyhedron_count)
						{
							real_vector3d const *direction = type_definition->render_direction_source==_weather_particle_direction_from_acceleration ?
								&particle->acceleration : &particle->velocity;

							if (type_definition->render_mode!=_build_sprite_normal && magnitude_squared3d(direction)==0.f)
							{
								direction = global_up3d;
							}

							build_sprite(
								&sprite_data,
								type_definition->render_mode,
								particle->sequence_index,
								(short)particle->sprite_index,
								&position,
								direction,
								particle->rotation,
								2.f*particle->radius*type_definition->runtime_one_over_sprite_width,
								&particle->color,
								fade_in*fade_out,
								0);
						}
					}
				}
			}

			build_sprites_end(&sprite_data);
		}
	}

	return;
}

void weather_particle_systems_render(
	void)
{
	if (render_particle_systems_enabled && weather)
	{
		if (render.local_player_index!=NONE)
		{
			struct weather_particle_system *system = weather_particle_system_get(render.local_player_index);
			long definition_index = NONE;

			system->location.cluster_index = render.cluster_index;
			system->location.leaf_index = render.leaf_index;
			system->under_water = scenario_location_underwater(&system->location, &render.camera.position, &system->weather_palette_index);
			if (system->weather_palette_index!=NONE)
			{
				struct structure_weather_palette_entry *weather_palette_entry = TAG_BLOCK_GET_ELEMENT(
					&global_structure_bsp_get()->weather_palette,
					system->weather_palette_index,
					struct structure_weather_palette_entry);

				definition_index = weather_palette_entry->particle_system.index;
			}

			if (system->definition_index!=definition_index)
			{
				if (system->definition_index!=NONE)
				{
					weather_particle_system_delete(render.local_player_index);
				}
				if (definition_index!=NONE)
				{
					weather_particle_system_new(render.local_player_index, definition_index, 1.f);
				}
			}

			if (system->definition_index!=NONE)
			{
				weather_particle_system_render(render.local_player_index);
			}
		}
	}

	return;
}

void weather_particle_system_delete(
	short local_player_index)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
	short type_index;

	for (type_index = 0; type_index<definition->particle_types.count; type_index++)
	{
		struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);

		while (type->first_particle_index!=NONE)
		{
			weather_particle_system_type_delete_particle(type);
		}
	}

	weather_particle_system_globals.active_system_count-= 1;
	system->definition_index = NONE;

	return;
}

/* ---------- private code */

static struct weather_particle_system *weather_particle_system_get(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\effects\\weather_particle_systems.c", 91, local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);

	return &weather_particle_system_globals.systems[local_player_index];
}

static struct weather_particle_type *weather_particle_system_get_type(
	struct weather_particle_system *system,
	short type_index)
{
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);

	match_assert("c:\\halo\\SOURCE\\effects\\weather_particle_systems.c", 102, type_index>=0 && type_index<definition->particle_types.count);

	return &system->types[type_index];
}

static void weather_particle_system_type_delete_particle(
	struct weather_particle_type *type)
{
	struct weather_particle *particle = datum_get(weather_particle_data, type->first_particle_index);
	long next_particle_index = particle->next_particle_index;

	datum_delete(weather_particle_data, type->first_particle_index);
	type->particle_count-= 1;
	type->first_particle_index = next_particle_index;

	return;
}

static void weather_particle_system_wrap_point(
	real box_width,
	real_point3d const *point,
	real_point3d *result)
{
	real offset;

	offset = point->x<0.f ? box_width : 0.f;
	result->x = (real)fmod(point->x, box_width) + offset;
	offset = point->y<0.f ? box_width : 0.f;
	result->y = (real)fmod(point->y, box_width) + offset;
	offset = point->z<0.f ? box_width : 0.f;
	result->z = (real)fmod(point->z, box_width) + offset;

	return;
}

static long weather_particle_system_new_particle(
	short local_player_index,
	short type_index)
{
	long particle_index = datum_new(weather_particle_data);

	if (particle_index!=NONE)
	{
		struct weather_particle_system *system = weather_particle_system_get(local_player_index);
		struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
		struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);
		struct weather_particle_type_definition *type_definition = TAG_BLOCK_GET_ELEMENT(
			&definition->particle_types,
			type_index,
			struct weather_particle_type_definition);
		struct weather_particle *particle = datum_get(weather_particle_data, particle_index);
		struct bitmap_group *bitmap_group = bitmap_group_get(type_definition->bitmap.index);
		struct bitmap_group_sequence *sequence;
		real color_interpolation;

		particle->position.x = real_local_random_range(0.f, type->box_width);
		particle->position.y = real_local_random_range(0.f, type->box_width);
		particle->position.z = real_local_random_range(0.f, type->box_width);
		particle->velocity.i = particle->velocity.j = particle->velocity.k = 0.f;
		local_random_direction3d(&particle->acceleration);
		scale_vector3d(
			&particle->acceleration,
			real_local_random_range(type_definition->acceleration_lower_bound, type_definition->acceleration_upper_bound),
			&particle->acceleration);
		particle->radius = real_local_random_range(type_definition->radius_lower_bound, type_definition->radius_upper_bound);
		particle->animation_rate = real_local_random_range(type_definition->animation_rate_lower_bound, type_definition->animation_rate_upper_bound);
		particle->rotation_rate = real_local_random_range(type_definition->rotation_rate_lower_bound, type_definition->rotation_rate_upper_bound);
		if (TEST_FLAG(type_definition->flags, _weather_particle_type_random_rotation_bit))
		{
			particle->rotation = real_local_random_range(0.f, _pi*2.f);
		}
		else
		{
			particle->rotation = 0.f;
		}
		particle->sequence_index = local_random_range(0, bitmap_group->sequences.count);
		sequence = TAG_BLOCK_GET_ELEMENT(&bitmap_group->sequences, particle->sequence_index, struct bitmap_group_sequence);
		particle->sprite_index = real_local_random_range(0.f, (real)sequence->sprites.count);
		color_interpolation = real_local_random();
		rgb_colors_interpolate(
			&particle->color.rgb,
			type_definition->flags,
			&type_definition->color_lower_bound.rgb,
			&type_definition->color_upper_bound.rgb,
			color_interpolation);
		particle->color.alpha = real_local_random_range(type_definition->color_lower_bound.alpha, type_definition->color_upper_bound.alpha);

		particle->next_particle_index = type->first_particle_index;
		type->particle_count+= 1;
		type->first_particle_index = particle_index;
	}

	return particle_index;
}

static void weather_particle_system_box_offset_from_point3d(
	real box_width,
	real_point3d const *point,
	real_point3d *offset)
{
	weather_particle_system_wrap_point(box_width, point, offset);
	offset->x = point->x - offset->x;
	offset->y = point->y - offset->y;
	offset->z = point->z - offset->z;

	return;
}

static void weather_particle_update_physics(
	short local_player_index,
	short type_index,
	short particle_index)
{
	struct weather_particle_system *system = weather_particle_system_get(local_player_index);
	struct weather_particle_system_definition *definition = weather_particle_system_definition_get(system->definition_index);
	struct weather_particle_type *type = weather_particle_system_get_type(system, type_index);
	struct weather_particle_type_definition *type_definition = TAG_BLOCK_GET_ELEMENT(
		&definition->particle_types,
		type_index,
		struct weather_particle_type_definition);
	struct weather_particle *particle = datum_get(weather_particle_data, particle_index);
	unsigned long flags;
	real_vector3d random_direction;

#ifdef HALO_LINUX
	if ((type_definition->acceleration_lower_bound!=0.f || type_definition->acceleration_upper_bound!=0.f) &&
		weather_particle_update_ticks<=0)
	{
		point_from_line3d((real_point3d *)&particle->velocity, &particle->acceleration, system->time_delta_sec, (real_point3d *)&particle->velocity);
	}
	else
#endif
	if (type_definition->acceleration_lower_bound!=0.f || type_definition->acceleration_upper_bound!=0.f)
	{
		real magnitude = normalize3d(&particle->acceleration);
		real inverse_turning_rate = 1.f - type_definition->acceleration_turning_rate;

		magnitude+= real_local_random_range(-type_definition->acceleration_change_rate, type_definition->acceleration_change_rate);
		magnitude = PIN(magnitude, type_definition->acceleration_lower_bound, type_definition->acceleration_upper_bound);
		local_random_direction3d(&random_direction);
		particle->acceleration.i = random_direction.i*type_definition->acceleration_turning_rate + inverse_turning_rate*particle->acceleration.i;
		particle->acceleration.j = random_direction.j*type_definition->acceleration_turning_rate + inverse_turning_rate*particle->acceleration.j;
		particle->acceleration.k = random_direction.k*type_definition->acceleration_turning_rate + inverse_turning_rate*particle->acceleration.k;
		scale_vector3d(&particle->acceleration, magnitude, &particle->acceleration);
		point_from_line3d((real_point3d *)&particle->velocity, &particle->acceleration, system->time_delta_sec, (real_point3d *)&particle->velocity);
	}

	{
		unsigned long seed = particle_index;

		flags = system->under_water ?
			FLAG(_point_physics_ignore_position_bit)|FLAG(_point_physics_ignore_position_under_water_bit)|FLAG(_point_physics_force_no_collisions_bit) :
			FLAG(_point_physics_ignore_position_bit)|FLAG(_point_physics_force_no_collisions_bit);
		point_physics_update(
			flags,
			point_physics_definition_get(type_definition->physics.index),
			&system->location,
			system->weather_palette_index,
			&particle->position,
			&particle->velocity,
			NULL,
			NULL,
			NULL,
			particle->radius,
			system->time_delta_sec);
#ifdef HALO_LINUX
		point_from_line3d(&particle->position, seed_random_direction3d(&seed, &random_direction), 0.001f*system->time_delta_sec*TICKS_PER_SECOND, &particle->position);
#else
		point_from_line3d(&particle->position, seed_random_direction3d(&seed, &random_direction), 0.001f, &particle->position);
#endif
	}
	weather_particle_system_wrap_point(type->box_width, &particle->position, &particle->position);

	return;
}
