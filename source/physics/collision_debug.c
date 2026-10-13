/*
COLLISION_DEBUG.C

symbols in this file:
00139530 1060:
	_collision_debug_render (0000)
0028B2A8 0060:
	_rdata_0028b2a8 (0000)
0028B308 000a:
	??_C@_09PCOGIILP@count?$DM?$DN14?$AA@ (0000)
0028B314 0031:
	??_C@_0DB@EBGLDBDO@plane?5?$CD?$CFd?$CFs?$HMnsurface?5?$CD?$CFd?$CFs?$CFs?$CFs?$CFs@ (0000)
0028B348 0004:
	__real@42652ee0 (0000)
0028B34C 0009:
	??_C@_08JNJLEJIF@?5negated?$AA@ (0000)
0028B358 000b:
	??_C@_0L@DMHIANBO@?5two?9sided?$AA@ (0000)
0028B364 000b:
	??_C@_0L@OFKNNBE@?5invisible?$AA@ (0000)
0028B370 000b:
	??_C@_0L@JOKAPIDA@?5climbable?$AA@ (0000)
0028B37C 000b:
	??_C@_0L@KCGACEBO@?5breakable?$AA@ (0000)
0028B388 000c:
	??_C@_0M@CIBLPKPA@phantom?5bsp?$AA@ (0000)
0028B394 0004:
	__real@3e162051 (0000)
0028B398 0004:
	__real@bd962051 (0000)
0028B3A0 0008:
	__real@401921fb60000000 (0000)
0028B3A8 0029:
	??_C@_0CJ@OKHLJDEM@c?3?2halo?2SOURCE?2physics?2collision@ (0000)
0030CD44 0010:
	_collision_debug_flag_front_facing_surfaces (0000)
	_collision_debug_flag_ignore_invisible_surfaces (0001)
	_collision_debug_flag_structure (0002)
	_collision_debug_flag_media (0003)
	_collision_debug_flag_objects (0004)
	_collision_debug_length (0008)
	_collision_debug_ignore_object_index (000c)
00456EC0 7122:
	_bss_00456ec0 (0000)
	_collision_debug_width (7118)
	_collision_debug_height (711c)
	_collision_debug_phantom_bsp (7120)
	_collision_debug_phantom_bsp_found (7121)
*/

/* ---------- headers */

#include "cseries.h"
#include "collision_debug.h"
#include "breakable_surfaces.h"
#include "collision_bsp.h"
#include "collision_bsp_definitions.h"
#include "collision_features.h"
#include "collisions.h"
#include "collision_usage.h"
#include "objects.h"
#include "tag_files/tag_files.h"
#include "collision_model_definitions.h"
#include "collision_models.h"
#include "game/game.h"
#include "game/game_globals.h"
#include "game/players.h"
#include "memory/data.h"
#include "render/render.h"
#include "render/render_cameras_internal.h"
#include "render/render_debug.h"
#include "scenario/scenario.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	MAXIMUM_COLLISION_DEBUG_RESULTS = 14,
	MAXIMUM_COLLISION_DEBUG_PATH_POINTS = MAXIMUM_COLLISION_DEBUG_RESULTS + 2,
	COLLISION_DEBUG_SPRAY_COLUMNS = 40,
	COLLISION_DEBUG_SPRAY_ROWS = 30,
	COLLISION_DEBUG_SPRAY_COUNT = COLLISION_DEBUG_SPRAY_COLUMNS * COLLISION_DEBUG_SPRAY_ROWS,
};

enum collision_surface_flags
{
	_collision_surface_two_sided_bit = 0,
	_collision_surface_invisible_bit,
	_collision_surface_climbable_bit,
	_collision_surface_breakable_bit,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

extern boolean collision_debug;
extern boolean collision_debug_features;
extern unsigned long collision_debug_flags;
extern real_point3d collision_debug_phantom_bsp_point;
extern boolean collision_debug_spray;

extern boolean collision_debug_flag_back_facing_surfaces;
extern boolean collision_debug_flag_ignore_breakable_surfaces;
extern boolean collision_debug_flag_ignore_two_sided_surfaces;
extern boolean collision_debug_flag_objects_bipeds;
extern boolean collision_debug_flag_objects_controls;
extern boolean collision_debug_flag_objects_equipment;
extern boolean collision_debug_flag_objects_light_fixtures;
extern boolean collision_debug_flag_objects_machines;
extern boolean collision_debug_flag_objects_placeholders;
extern boolean collision_debug_flag_objects_projectiles;
extern boolean collision_debug_flag_objects_scenery;
extern boolean collision_debug_flag_objects_vehicles;
extern boolean collision_debug_flag_objects_weapons;
extern boolean collision_debug_flag_skip_passthrough_bipeds;
extern boolean collision_debug_flag_try_to_keep_location_valid;
extern boolean collision_debug_flag_use_vehicle_physics;

static const real_point3d collision_debug_cube_vertices[NUMBER_OF_VERTICES_PER_CUBE] =
{
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f,  1.0f },
	{ -1.0f,  1.0f, -1.0f },
	{ -1.0f,  1.0f,  1.0f },
	{  1.0f, -1.0f, -1.0f },
	{  1.0f, -1.0f,  1.0f },
	{  1.0f,  1.0f, -1.0f },
	{  1.0f,  1.0f,  1.0f },
};

boolean collision_debug_flag_front_facing_surfaces = TRUE;
boolean collision_debug_flag_ignore_invisible_surfaces = TRUE;
boolean collision_debug_flag_structure = TRUE;
boolean collision_debug_flag_media = TRUE;
boolean collision_debug_flag_objects = TRUE;
real collision_debug_length = 100.0f;
long collision_debug_ignore_object_index = NONE;

static real_vector3d collision_debug_spray_normals[COLLISION_DEBUG_SPRAY_ROWS][COLLISION_DEBUG_SPRAY_COLUMNS] = { 0 };
static real_point3d collision_debug_spray_points[COLLISION_DEBUG_SPRAY_ROWS][COLLISION_DEBUG_SPRAY_COLUMNS] = { 0 };
static unsigned long collision_debug_spray_hit_flags[BIT_VECTOR_SIZE_IN_LONGS(COLLISION_DEBUG_SPRAY_COUNT)] = { 0 };
real collision_debug_width = 0.0f;
real collision_debug_height = 0.0f;
boolean collision_debug_phantom_bsp = FALSE;
boolean collision_debug_phantom_bsp_found = FALSE;

/* ---------- public code */

void collision_debug_render(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\physics\\collision_debug.c",
		76,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = _collision_user_debugging;

	if (collision_debug || collision_debug_spray || collision_debug_features || collision_debug_phantom_bsp)
	{
		unsigned long flags;
		real length;
		real_point3d point;
		real_vector3d vector;
		long ignore_object_index;

		flags = 0;
		SET_FLAG(flags, _collision_test_front_facing_surfaces_bit, collision_debug_flag_front_facing_surfaces);
		SET_FLAG(flags, _collision_test_back_facing_surfaces_bit, collision_debug_flag_back_facing_surfaces);
		SET_FLAG(flags, _collision_test_ignore_two_sided_surfaces_bit, collision_debug_flag_ignore_two_sided_surfaces);
		SET_FLAG(flags, _collision_test_ignore_invisible_surfaces_bit, collision_debug_flag_ignore_invisible_surfaces);
		SET_FLAG(flags, _collision_test_ignore_breakable_surfaces_bit, collision_debug_flag_ignore_breakable_surfaces);
		SET_FLAG(flags, _collision_test_structure_bit, collision_debug_flag_structure);
		SET_FLAG(flags, _collision_test_media_bit, collision_debug_flag_media);
		SET_FLAG(flags, _collision_test_objects_bit, collision_debug_flag_objects);
		SET_FLAG(flags, _collision_test_objects_bipeds_bit, collision_debug_flag_objects_bipeds);
		SET_FLAG(flags, _collision_test_objects_vehicles_bit, collision_debug_flag_objects_vehicles);
		SET_FLAG(flags, _collision_test_objects_weapons_bit, collision_debug_flag_objects_weapons);
		SET_FLAG(flags, _collision_test_objects_equipment_bit, collision_debug_flag_objects_equipment);
		SET_FLAG(flags, _collision_test_objects_projectiles_bit, collision_debug_flag_objects_projectiles);
		SET_FLAG(flags, _collision_test_objects_scenery_bit, collision_debug_flag_objects_scenery);
		SET_FLAG(flags, _collision_test_objects_machines_bit, collision_debug_flag_objects_machines);
		SET_FLAG(flags, _collision_test_objects_controls_bit, collision_debug_flag_objects_controls);
		SET_FLAG(flags, _collision_test_objects_light_fixtures_bit, collision_debug_flag_objects_light_fixtures);
		SET_FLAG(flags, _collision_test_objects_placeholders_bit, collision_debug_flag_objects_placeholders);
		SET_FLAG(flags, _collision_test_try_to_keep_location_valid_bit, collision_debug_flag_try_to_keep_location_valid);
		SET_FLAG(flags, _collision_test_skip_passthrough_bipeds_bit, collision_debug_flag_skip_passthrough_bipeds);
		SET_FLAG(flags, _collision_test_use_vehicle_physics_bit, collision_debug_flag_use_vehicle_physics);

		if (collision_debug_flags)
			flags = collision_debug_flags;

		if (!collision_debug_repeat)
		{
			collision_debug_ignore_object_index = local_player_get_player_index(render.local_player_index) == NONE
				? NONE
				: player_get(local_player_get_player_index(render.local_player_index))->unit_index;
			collision_debug_point = render.camera.position;
			collision_debug_vector = render.camera.forward;
		}

		length = (real)fabs(collision_debug_length);
		point = collision_debug_point;
		vector = collision_debug_vector;
		scale_vector3d(&vector, length, &vector);
		ignore_object_index = collision_debug_ignore_object_index;

		if (collision_debug_phantom_bsp)
		{
			struct collision_bsp_test_vector_result vector_result;

			if (collision_bsp_test_vector(
				flags,
				global_collision_bsp_get(),
				MAXIMUM_BREAKABLE_SURFACES_PER_MAP,
				breakable_surface_flags_get(),
				&point,
				&vector,
				1.0f,
				&vector_result))
			{
				real_point3d collision_point;
				struct collision_feature_list features;

				point_from_line3d(&point, &vector, vector_result.t, &collision_point);

				if (!collision_get_features_in_sphere(
					flags,
					&collision_point,
					0.01f,
					0.0f,
					0.01f,
					ignore_object_index,
					&features))
				{
					collision_debug_phantom_bsp_found = TRUE;
					collision_debug_phantom_bsp_point = collision_point;
				}
			}

			if (collision_debug_phantom_bsp_found)
			{
				real yaw;
				real pitch;
				real roll;
				real_matrix4x3 rotation_matrix;
				real_point3d rotated_points[8];
				short point_index;

				yaw = (real)fmod(game_time_get() * 1.0f * _pi / 30.f, _pi*2.f);
				pitch = (real)fmod(game_time_get() * -0.7f * _pi / 30.f, _pi*2.f);
				roll = (real)fmod(game_time_get() * 1.4f * _pi / 30.f, _pi*2.f);

				render_debug_string_at_point(
					TRUE,
					&collision_debug_phantom_bsp_point,
					"phantom bsp",
					global_real_argb_pink);
				matrix4x3_rotation_from_angles(&rotation_matrix, yaw, pitch, roll);

				for (point_index = 0; point_index < NUMBEROF(rotated_points); point_index++)
				{
					real_vector3d cube_vector;

					matrix4x3_transform_vector(
						&rotation_matrix,
						(real_vector3d const *)&collision_debug_cube_vertices[point_index],
						&cube_vector);
					point_from_line3d(&collision_debug_phantom_bsp_point, &cube_vector, 0.15f, &rotated_points[point_index]);
				}

				render_debug_line(TRUE, &rotated_points[0], &rotated_points[1], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[1], &rotated_points[3], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[3], &rotated_points[2], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[2], &rotated_points[0], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[4], &rotated_points[5], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[5], &rotated_points[7], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[7], &rotated_points[6], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[6], &rotated_points[4], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[0], &rotated_points[4], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[1], &rotated_points[5], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[2], &rotated_points[6], global_real_argb_pink);
				render_debug_line(TRUE, &rotated_points[3], &rotated_points[7], global_real_argb_pink);
			}
		}

		if (collision_debug)
		{
			if (collision_debug_length <= 0.0f)
			{
				add_vectors3d((real_vector3d const *)&point, &vector, (real_vector3d *)&point);

				if (collision_debug_width <= 0.0f)
				{
					if (collision_test_point(flags, &point, ignore_object_index))
						render_debug_point(TRUE, &point, 0.1f, global_real_argb_red);
					else
						render_debug_point(TRUE, &point, 0.1f, global_real_argb_green);
				}
				else
				{
					if (collision_test_sphere(&point, collision_debug_width, ignore_object_index))
						render_debug_sphere(TRUE, &point, collision_debug_width, global_real_argb_red);
					else
						render_debug_sphere(TRUE, &point, collision_debug_width, global_real_argb_green);
				}
			}
			else if (collision_debug_width <= 0.0f)
			{
				struct collision_result collision;

				if (collision_test_vector(flags, &point, &vector, ignore_object_index, &collision))
				{
					struct collision_bsp *bsp = NULL;
					real_matrix4x3 const *matrix = NULL;
					struct collision_surface const *surface = NULL;
					char textstring[2048];
#ifdef HALO_LINUX
					/* matrix points into it after the else if below, so it
					has to outlive that block (see object_reconnect_to_map) */
					struct collision_model_instance instance;
#endif

					render_debug_vector(TRUE, &point, &vector, collision.t, global_real_argb_red);
					render_debug_point(TRUE, &collision.point, 0.125f, global_real_argb_red);
					render_debug_vector(TRUE, &collision.point, &collision.plane.n, 0.25f, global_real_argb_red);

					if (collision.type == _collision_result_structure)
					{
						bsp = global_collision_bsp_get();
					}
					else if (collision.type == _collision_result_object && collision.node_index != NONE)
					{
#ifndef HALO_LINUX
						struct collision_model_instance instance;
#endif

						if (collision_model_instance_new(&instance, collision.object_index))
						{
							struct collision_node *node = TAG_BLOCK_GET_ELEMENT(
								&instance.model->nodes,
								collision.node_index,
								struct collision_node);
							bsp = TAG_BLOCK_GET_ELEMENT(&node->bsps, collision.bsp_index, struct collision_bsp);
							matrix = &instance.matrices[collision.node_index];
						}
					}

					if (bsp)
					{
						surface = TAG_BLOCK_GET_ELEMENT(&bsp->surfaces, collision.surface_index, struct collision_surface);
						render_debug_collision_surface(bsp, collision.surface_index, matrix, global_real_argb_red);

						_snprintf(
							textstring,
							sizeof(textstring),
							"plane #%d%s|nsurface #%d%s%s%s%s|n%s|n%f degrees",
							surface->plane_designator & LONG_MAX,
							surface->plane_designator & LONG_MIN ? " negated" : "",
							collision.surface_index,
							TEST_FLAG(collision.flags, _collision_surface_two_sided_bit) ? " two-sided" : "",
							TEST_FLAG(collision.flags, _collision_surface_invisible_bit) ? " invisible" : "",
							TEST_FLAG(collision.flags, _collision_surface_climbable_bit) ? " climbable" : "",
							TEST_FLAG(collision.flags, _collision_surface_breakable_bit) ? " breakable" : "",
							material_get_name(collision.material_type),
							arccosine(collision.plane.n.k) * 360.f / (_pi*2.f));
						render_debug_string(TRUE, textstring);
					}
				}
				else
				{
					render_debug_line(TRUE, &point, &collision.point, global_real_argb_green);
					render_debug_point(TRUE, &collision.point, 0.125f, global_real_argb_green);
				}
			}
			else
			{
				short count;
				short collision_index;
				real_point3d position = point;
				real_vector3d velocity = vector;
				real_point3d new_position;
				real_vector3d new_velocity;
				struct collision_plane collisions[MAXIMUM_COLLISION_DEBUG_PATH_POINTS];

				if (collision_debug_height <= 0.0f)
				{
					count = collision_move_sphere(
						flags,
						&position,
						&velocity,
						collision_debug_width,
						ignore_object_index,
						&new_position,
						&new_velocity,
						MAXIMUM_COLLISION_DEBUG_RESULTS,
						collisions);
				}
				else
				{
					count = collision_move_pill(
						flags,
						&position,
						&velocity,
						collision_debug_height,
						collision_debug_width,
						ignore_object_index,
						&new_position,
						&new_velocity,
						MAXIMUM_COLLISION_DEBUG_RESULTS,
						collisions);
				}

				render_debug_vector(TRUE, &position, &velocity, 1.0f, global_real_argb_blue);
				match_assert(
					"c:\\halo\\SOURCE\\physics\\collision_debug.c",
					297,
					count<=14);

				memmove(&collisions[1], &collisions[0], count * sizeof(collisions[0]));
				collisions[0].point = position;
				memset(&collisions[0].plane, 0, sizeof(collisions[0].plane));
				count++;
				collisions[count].point = new_position;
				memset(&collisions[count].plane, 0, sizeof(collisions[count].plane));
				count++;

				for (collision_index = 0; collision_index < count; collision_index++)
				{
					render_debug_point(TRUE, &collisions[collision_index].point, 0.0625f, global_real_argb_red);
					if (collision_index > 0)
						render_debug_line(TRUE, &collisions[collision_index - 1].point, &collisions[collision_index].point, global_real_argb_red);
					render_debug_vector(TRUE, &collisions[collision_index].point, &collisions[collision_index].plane.n, 0.125f, global_real_argb_red);
				}

				render_debug_vector(TRUE, &new_position, &new_velocity, 1.0f, global_real_argb_green);
			}
		}

		if (collision_debug_spray)
		{
			if (collision_debug_length <= 0.0f)
			{
				real_point3d o;
				real_point3d p;

				point_from_line3d(&render.camera.position, &render.camera.forward, 2.0f, &o);
				for (p.z = o.z - 0.125f; p.z <= o.z + 0.125f; p.z += 0.0625f)
				{
					for (p.y = o.y - 0.125f; p.y <= o.y + 0.125f; p.y += 0.0625f)
					{
						for (p.x = o.x - 0.125f; p.x <= o.x + 0.125f; p.x += 0.0625f)
						{
							render_debug_point(
								TRUE,
								&p,
								0.0625f,
								collision_test_point(flags, &p, NONE) ? global_real_argb_red : global_real_argb_green);
						}
					}
				}
			}
			else
			{
				if (!collision_debug_repeat)
				{
					real_vector3d i;
					real_vector3d j;
					real_vector3d k;
					short row;
					short column;

					{
						real_rectangle2d bounds;

						render_frustum_get_projection_bounds(&render.frustum, &bounds);
						set_real_vector3d(
							&i,
							(bounds.x1 - bounds.x0) * collision_debug_length / COLLISION_DEBUG_SPRAY_COLUMNS,
							0.0f,
							0.0f);
						set_real_vector3d(
							&j,
							0.0f,
							(bounds.y1 - bounds.y0) * collision_debug_length / COLLISION_DEBUG_SPRAY_ROWS,
							0.0f);
						set_real_vector3d(
							&k,
							bounds.x0 * collision_debug_length,
							bounds.y0 * collision_debug_length,
							-collision_debug_length);
					}

					matrix4x3_transform_vector(&render.frustum.view_to_world, &i, &i);
					matrix4x3_transform_vector(&render.frustum.view_to_world, &j, &j);
					matrix4x3_transform_vector(&render.frustum.view_to_world, &k, &k);

					for (row = 0; row < COLLISION_DEBUG_SPRAY_ROWS; row++)
					{
						for (column = 0; column < COLLISION_DEBUG_SPRAY_COLUMNS; column++)
						{
							struct collision_result collision;
							real_vector3d v;

							v.i = i.i * column + j.i * row + k.i;
							v.j = i.j * column + j.j * row + k.j;
							v.k = i.k * column + j.k * row + k.k;

							if (collision_test_vector(flags, &render.camera.position, &v, ignore_object_index, &collision))
							{
								BIT_VECTOR_SET_FLAG(collision_debug_spray_hit_flags, row * COLLISION_DEBUG_SPRAY_COLUMNS + column, TRUE);
								collision_debug_spray_points[row][column] = collision.point;
								collision_debug_spray_normals[row][column] = collision.plane.n;
							}
							else
							{
								BIT_VECTOR_SET_FLAG(collision_debug_spray_hit_flags, row * COLLISION_DEBUG_SPRAY_COLUMNS + column, FALSE);
							}
						}
					}
				}

				{
					short row;
					short column;

					for (row = 0; row < COLLISION_DEBUG_SPRAY_ROWS; row++)
					{
						for (column = 0; column < COLLISION_DEBUG_SPRAY_COLUMNS; column++)
						{
							if (BIT_VECTOR_TEST_FLAG(collision_debug_spray_hit_flags, row * COLLISION_DEBUG_SPRAY_COLUMNS + column))
							{
								render_debug_point(TRUE, &collision_debug_spray_points[row][column], 0.03125f, global_real_argb_red);
								render_debug_vector(
									TRUE,
									&collision_debug_spray_points[row][column],
									&collision_debug_spray_normals[row][column],
									0.0625f,
									global_real_argb_red);
							}
						}
					}
				}
			}
		}

		if (collision_debug_features)
		{
			real radius;
			real_point3d test_center;
			struct collision_feature_list features;

			point_from_line3d(&collision_debug_point, &collision_debug_vector, 0.5f, &test_center);
			test_center.z += collision_debug_height * 0.5f;
			radius = magnitude3d(&collision_debug_vector) * 0.5f
				+ collision_debug_height * 0.5f
				+ collision_debug_width;

			if (collision_get_features_in_sphere(
				flags,
				&test_center,
				radius,
				collision_debug_height,
				collision_debug_width,
				collision_debug_ignore_object_index,
				&features))
			{
				render_debug_collision_features(&features);
			}
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\physics\\collision_debug.c",
		443,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return;
}

/* ---------- private code */
