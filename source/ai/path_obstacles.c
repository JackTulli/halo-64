/*
PATH_OBSTACLES.C

symbols in this file:
000503F0 0020:
	_obstacles_new (0000)
00050410 0020:
	_set_real_point2d (0000)
00050430 0030:
	_rotate_vector2d (0000)
00050460 0090:
	_project_point3d (0000)
000504F0 0040:
	_point_in_circle (0000)
00050530 0050:
	_point_in_sphere (0000)
00050580 0090:
	_circle_intersect_ray (0000)
00050610 0080:
	_circle_tangents (0000)
00050690 00d0:
	_obstacles_add_disc (0000)
00050760 0320:
	_obstacles_get_discs_in_sphere (0000)
00050A80 00a0:
	_obstacles_test_circle (0000)
00050B20 00f0:
	_obstacles_test_pill (0000)
00050C10 00e0:
	_obstacles_disc_tangents (0000)
00050CF0 0230:
	_obstacles_disc_neighborhood (0000)
00050F20 00b0:
	_obstacles_recompute (0000)
00050FD0 01c0:
	_render_debug_obstacles (0000)
0024E900 000b:
	??_C@_0L@LKENDBEE@?$HO?$CIsign?$CG?$HO1?$CJ?$AA@ (0000)
0024E90C 0021:
	??_C@_0CB@FEGCJDLB@projection?$DO?$DN_x?5?$CG?$CG?5projection?$DM?$DN_z@ (0000)
0024E930 0014:
	??_C@_0BE@DKJODJEH@?4?4?2math?2real_math?4h?$AA@ (0000)
0024E948 0051:
	??_C@_0FB@IMJPHNIA@obstacles?9?$DOobstacle_count?$DO?$DN0?5?$CG?$CG?5@ (0000)
0024E9A0 0046:
	??_C@_0EG@PILJPJIJ@obstacles?9?$DOdisc_count?$DO?$DN0?5?$CG?$CG?5obst@ (0000)
0024E9E8 0023:
	??_C@_0CD@MNFANEEP@c?3?2halo?2SOURCE?2ai?2path_obstacles@ (0000)
0024EA0C 000d:
	??_C@_0N@IDCAPIB@matt?5is?5bad?4?$AA@ (0000)
0024EA1C 0004:
	__real@3b800000 (0000)
0024EA20 003c:
	??_C@_0DM@HAPAFMPD@seed_disc_index?$DO?$DN0?5?$CG?$CG?5seed_disc_@ (0000)
0024EA5C 0004:
	__real@c0000000 (0000)
0024EA60 004a:
	??_C@_0EK@GINAEHH@disc?9?$DOobstacle_index?$DO?$DN0?5?$CG?$CG?5disc?9@ (0000)
002B7578 0800:
	_data_002b7578 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "path.h"
#include "devices/device_definitions.h"
#include "devices/devices.h"
#include "objects/objects.h"
#include "physics/collision_model_definitions.h"
#include "physics/collisions.h"
#include "render/render_debug.h"

/* ---------- constants */

/* ---------- macros */

#define path_obstacles_device_datum_from_object(object) \
	((struct device_datum const *)(object))

/* ---------- structures */

/* ---------- prototypes */

static boolean circle_intersect_ray(
	real_point2d const *center,
	real radius,
	real_point2d const *point,
	real_vector2d const *direction,
	real *distance);
static void circle_tangents(
	real_vector2d const *direction,
	real distance,
	real radius,
	real_vector2d *right_direction,
	real_vector2d *left_direction,
	real *tangent_distance);

/* ---------- globals */

real_argb_color global_obstacle_colors[MAXIMUM_DISC_COUNT] =
{
	{ { 1.f, 1.f, 1.f, 0.5f } },
	{ { 1.f, 1.f, 1.f, 0.f } },
	{ { 1.f, 1.f, 0.5f, 1.f } },
	{ { 1.f, 1.f, 0.5f, 0.5f } },
	{ { 1.f, 1.f, 0.5f, 0.f } },
	{ { 1.f, 1.f, 0.f, 1.f } },
	{ { 1.f, 1.f, 0.f, 0.5f } },
	{ { 1.f, 1.f, 0.f, 0.f } },
	{ { 1.f, 0.5f, 1.f, 1.f } },
	{ { 1.f, 0.5f, 1.f, 0.5f } },
	{ { 1.f, 0.5f, 1.f, 0.f } },
	{ { 1.f, 0.5f, 0.5f, 1.f } },
	{ { 1.f, 0.5f, 0.5f, 0.5f } },
	{ { 1.f, 0.5f, 0.5f, 0.f } },
	{ { 1.f, 0.5f, 0.f, 1.f } },
	{ { 1.f, 0.5f, 0.f, 0.5f } },
	{ { 1.f, 0.5f, 0.f, 0.f } },
	{ { 1.f, 0.f, 1.f, 1.f } },
	{ { 1.f, 0.f, 1.f, 0.5f } },
	{ { 1.f, 0.f, 1.f, 0.f } },
	{ { 1.f, 0.f, 0.5f, 1.f } },
	{ { 1.f, 0.f, 0.5f, 0.5f } },
	{ { 1.f, 0.f, 0.5f, 0.f } },
	{ { 1.f, 0.f, 0.f, 1.f } },
	{ { 1.f, 0.f, 0.f, 0.5f } }
};

/* ---------- public code */

void obstacles_new(
	struct obstacles *obstacles)
{
	obstacles->obstacle_count = 0;
	obstacles->disc_count = 0;
	obstacles->disc_optional_count = 0;
	return;
}

boolean obstacles_add_disc(
	struct obstacles *obstacles,
	long object_index,
	short flags,
	real_point3d const *center,
	real radius)
{
	struct obstacle_disc *disc;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
		104,
		obstacles->disc_count>=0 && obstacles->disc_count<=MAXIMUM_DISC_COUNT);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
		105,
		obstacles->obstacle_count>=0 && obstacles->obstacle_count<=obstacles->disc_count);
	if (obstacles->disc_count == MAXIMUM_DISC_COUNT)
	{
		return FALSE;
	}

	disc = &obstacles->discs[obstacles->disc_count++];
	if (TEST_FLAG(flags, _disc_optional_bit))
	{
		obstacles->disc_optional_count++;
	}
	disc->flags = flags;
	disc->object_index = object_index;
	disc->obstacle_index = NONE;
	project_point3d(center, _z, TRUE, &disc->center);
	disc->radius = radius;
	disc->height = center->z;

	return TRUE;
}

void obstacles_get_discs_in_sphere(
	struct obstacles *obstacles,
	real_point3d const *center,
	real radius,
	real_vector3d const *movement_direction,
	long ignore_source_object_index,
	long ignore_target_object_index)
{
	long object_indices[256];
	real_matrix4x3 world_matrix;
	real_point3d transformed_center;
	short object_count;
	short object_number;

	match_vassert(
		"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
		0x98,
		ignore_source_object_index != NONE,
		"matt is bad.");

	object_count = objects_in_sphere(
		_object_mask_biped,
		_object_mask_biped | _object_mask_sightblocking,
		&object_get(ignore_source_object_index)->object.location,
		center,
		radius,
		object_indices,
		NUMBEROF(object_indices));
	for (object_number = 0; object_number < object_count; object_number++)
	{
		long object_index = object_indices[object_number];
		struct object_datum *object = object_get(object_index);

		if (object_index != ignore_source_object_index &&
			object_index != ignore_target_object_index &&
			!TEST_FLAG(object->object.flags, _object_invisible_bit) &&
			(object->object.type != _object_type_biped ||
				!TEST_FLAG(object->object.damage_flags, _object_dead_bit)))
		{
			boolean object_is_pathfinding_obstacle = TRUE;

			if (object->object.type == _object_type_machine)
			{
				struct machine_definition const *machine_definition =
					machine_definition_get(object->definition_index);

				object_is_pathfinding_obstacle =
					TEST_FLAG(machine_definition->machine.flags, _machine_is_pathfinding_obstacle_bit) &&
					(!TEST_FLAG(machine_definition->machine.flags, _machine_is_not_pathfinding_obstacle_when_open_bit) ||
						path_obstacles_device_datum_from_object(object)->device.position != 1.f);
			}

			if (object_is_pathfinding_obstacle)
			{
				real bounding_radius = object->object.bounding_sphere_radius;

				bounding_radius += radius;

				if (point_in_sphere(
					center,
					&object->object.bounding_sphere_center,
					bounding_radius))
				{
					struct object_definition const *object_definition =
						object_definition_get(object->definition_index);
					struct collision_model const *collision_model =
						collision_model_definition_get(object_definition->object.collision_model.index);

					if (!TEST_FLAG(object_definition->object.flags, _object_not_pathfinding_obstacle_bit) &&
						collision_model->pathfinding_spheres.count > 0)
					{
						short pathfinding_sphere_index;

						object_get_world_matrix(object_index, &world_matrix);
						for (pathfinding_sphere_index = 0;
							pathfinding_sphere_index < collision_model->pathfinding_spheres.count;
							pathfinding_sphere_index++)
						{
							struct pathfinding_sphere const *pathfinding_sphere = TAG_BLOCK_GET_ELEMENT(
								&collision_model->pathfinding_spheres,
								pathfinding_sphere_index,
								struct pathfinding_sphere);
							word node_index = pathfinding_sphere->node_index;
							real transformed_radius;

							if (node_index != (word)NONE)
							{
								real_matrix4x3 const *node_matrix = object_get_node_matrix(object_index, node_index);

								matrix4x3_transform_point(
									node_matrix,
									&pathfinding_sphere->center,
									&transformed_center);
								transformed_radius = pathfinding_sphere->radius * node_matrix->scale;
							}
							else
							{
								matrix4x3_transform_point(
									&world_matrix,
									&pathfinding_sphere->center,
									&transformed_center);
								transformed_radius = world_matrix.scale * pathfinding_sphere->radius;
							}

							if (transformed_center.z + transformed_radius + 0.5f < center->z &&
								movement_direction->k > -0.2f)
							{
								continue;
							}
							if (transformed_center.z - transformed_radius - 0.5f > center->z &&
								movement_direction->k < 0.2f)
							{
								continue;
							}

							{
								real_vector3d offset;
								real total_radius;

								vector_from_points3d(center, &transformed_center, &offset);
								total_radius = transformed_radius + radius;
								if ((offset.i * offset.i +
									offset.j * offset.j +
									offset.k * offset.k * 4.f) <= total_radius * total_radius)
								{
									short flags = 0;

									if (object->object.type == _object_type_biped &&
										dot_product3d(&offset, movement_direction) > 0.f &&
										dot_product3d(&object->object.translational_velocity, movement_direction) > 0.06666667f)
									{
										SET_FLAG(flags, _disc_optional_bit, TRUE);
									}

									obstacles_add_disc(
										obstacles,
										object_index,
										flags,
										&transformed_center,
										transformed_radius);
								}
							}
						}
					}
				}
			}
		}
	}

	return;
}

short obstacles_test_circle(
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *center,
	real radius)
{
	short disc_index;

	for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
	{
		if (disc_index != ignore_disc_index)
		{
			struct obstacle_disc const *disc = obstacles_get_disc(obstacles, disc_index);

			if (point_in_circle(center, &disc->center, disc->radius + radius))
			{
				return disc_index;
			}
		}
	}

	return NONE;
}

boolean obstacles_test_pill(
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *point,
	real_vector2d const *direction,
	real radius,
	real distance,
	boolean ignore_optional,
	struct obstacles_test_pill_result *result)
{
	short disc_index;

	result->distance = distance;
	result->disc_index = NONE;
	result->obstacle_index = NONE;
	for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
	{
		if (disc_index != ignore_disc_index)
		{
			struct obstacle_disc const *disc = obstacles_get_disc(obstacles, disc_index);

			if (!ignore_optional || !TEST_FLAG(disc->flags, _disc_optional_bit))
			{
				real disc_distance;

				if (circle_intersect_ray(&disc->center, disc->radius + radius,
					point, direction, &disc_distance) && result->distance > disc_distance)
				{
					result->distance = disc_distance;
					result->disc_index = disc_index;
					result->obstacle_index = disc->obstacle_index;
				}
			}
		}
	}

	return result->disc_index != NONE;
}

void obstacles_disc_tangents(
	struct obstacles const *obstacles,
	short disc_index,
	real_point2d const *point,
	real radius,
	real_vector2d *right_direction,
	real_vector2d *left_direction,
	real *tangent_distance)
{
	struct obstacle_disc const *disc = obstacles_get_disc(obstacles, disc_index);
	real_vector2d direction;
	real distance = normalize2d(vector_from_points2d(point, &disc->center, &direction));

	circle_tangents(
		&direction,
		distance,
		radius + disc->radius + 0.00390625f,
		right_direction,
		left_direction,
		tangent_distance);
	return;
}

void obstacles_disc_neighborhood(
	struct obstacles const *obstacles,
	real radius,
	short seed_disc_index,
	byte *disc_flags)
{
	short disc_stack[MAXIMUM_DISC_COUNT];
	short stack_top = 0;
	unsigned long *disc_flag_words = (unsigned long *)disc_flags;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
		0x183,
		obstacles->disc_count>=0 && obstacles->disc_count<=MAXIMUM_DISC_COUNT);
	csmemset(disc_flag_words, 0, BIT_VECTOR_SIZE_IN_BYTES(obstacles->disc_count));

	if (seed_disc_index != NONE)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
			0x18C,
			seed_disc_index>=0 && seed_disc_index<obstacles->disc_count);

		BIT_VECTOR_SET_FLAG(disc_flag_words, seed_disc_index, TRUE);
		disc_stack[stack_top++] = seed_disc_index;

		while (stack_top > 0)
		{
			short current_disc_index = disc_stack[--stack_top];
			struct obstacle_disc const *current_disc = obstacles_get_disc(obstacles, current_disc_index);
			short disc_index;

			for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
			{
				if (!BIT_VECTOR_TEST_FLAG(disc_flag_words, disc_index))
				{
					struct obstacle_disc const *disc = obstacles_get_disc(obstacles, disc_index);

					if (point_in_circle(
						&current_disc->center,
						&disc->center,
						(disc->radius + radius) + (current_disc->radius + radius)))
					{
						BIT_VECTOR_SET_FLAG(disc_flag_words, disc_index, TRUE);
						match_assert(
							"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
							0x1A5,
							stack_top<MAXIMUM_DISC_COUNT);
						disc_stack[stack_top++] = disc_index;
					}
				}
			}
		}
	}

	return;
}

void obstacles_recompute(
	struct obstacles *obstacles,
	real radius)
{
	unsigned long disc_flags[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_DISC_COUNT)];
	short disc_index;

	obstacles->obstacle_count = 0;
	for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
		obstacles->discs[disc_index].obstacle_index = NONE;

	for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
	{
		if (obstacles->discs[disc_index].obstacle_index == NONE)
		{
			short obstacle_index = obstacles->obstacle_count++;
			short neighborhood_disc_index;

			obstacles_disc_neighborhood(obstacles, radius, disc_index, (byte *)disc_flags);
			for (neighborhood_disc_index = 0;
				neighborhood_disc_index < obstacles->disc_count;
				neighborhood_disc_index++)
			{
				if (BIT_VECTOR_TEST_FLAG(disc_flags, neighborhood_disc_index))
					obstacles->discs[neighborhood_disc_index].obstacle_index = obstacle_index;
			}
		}
	}

	return;
}

void render_debug_obstacles(
	struct obstacles const *obstacles,
	real radius)
{
	struct collision_result collision;
	real_vector3d vector;
	real_point3d point;
	short disc_index;

	for (disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
	{
		struct obstacle_disc const *disc = obstacles_get_disc(obstacles, disc_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\path_obstacles.c",
			0x211,
			disc->obstacle_index>=0 && disc->obstacle_index<obstacles->obstacle_count);

		set_real_point3d(&point, disc->center.x, disc->center.y, disc->height);
		set_real_vector3d(&vector, 0.f, 0.f, (radius + disc->radius) * -2.f);

		if (collision_test_vector(
			FLAG(_collision_test_front_facing_surfaces_bit) |
				FLAG(_collision_test_structure_bit),
			&point,
			&vector,
			NONE,
			&collision))
		{
			render_debug_circle(
				TRUE,
				&collision.plane,
				_z,
				TRUE,
				&disc->center,
				disc->radius,
				&global_obstacle_colors[disc->obstacle_index],
				0.015625f);
			if (radius > 0.f)
			{
				render_debug_circle(
					TRUE,
					&collision.plane,
					_z,
					TRUE,
					&disc->center,
					disc->radius + radius,
					&global_obstacle_colors[disc->obstacle_index],
					0.015625f);
			}
		}
		else
		{
			render_debug_sphere(
				TRUE,
				&point,
				disc->radius,
				&global_obstacle_colors[disc->obstacle_index]);
			if (radius > 0.f)
			{
				render_debug_sphere(
					TRUE,
					&point,
					disc->radius + radius,
					&global_obstacle_colors[disc->obstacle_index]);
			}
		}
	}

	return;
}

/* ---------- private code */

static boolean circle_intersect_ray(
	real_point2d const *center,
	real radius,
	real_point2d const *point,
	real_vector2d const *direction,
	real *distance)
{
	real_vector2d offset;
	real projection = dot_product2d(direction, vector_from_points2d(point, center, &offset));

	if (projection > 0.0f)
	{
		real distance_squared_minus_radius_squared = magnitude_squared2d(&offset) - radius * radius;

		if (distance_squared_minus_radius_squared <= 0.0f)
		{
			*distance = 0.0f;
			return TRUE;
		}
		else
		{
			real discriminant = projection * projection - distance_squared_minus_radius_squared;

			if (discriminant >= 0.0f)
			{
				*distance = projection - square_root(discriminant);
				return TRUE;
			}
		}
	}

	return FALSE;
}

static void circle_tangents(
	real_vector2d const *direction,
	real distance,
	real radius,
	real_vector2d *right_direction,
	real_vector2d *left_direction,
	real *tangent_distance)
{
	real sine = radius / distance;
	real cosine;

	sine = MIN(sine, 1.0f);
	cosine = square_root(1.0f - sine * sine);
	rotate_vector2d(direction, -sine, cosine, right_direction);
	rotate_vector2d(direction, sine, cosine, left_direction);
	*tangent_distance = cosine * distance;
	return;
}
