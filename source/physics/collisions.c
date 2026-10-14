/*
COLLISIONS.C

symbols in this file:
0013D2D0 0030:
	_collision_model_get_material_type (0000)
0013D300 0060:
	_collision_test_sphere (0000)
0013D360 0120:
	_object_test_point (0000)
0013D480 00b0:
	_collision_test_point (0000)
0013D530 0290:
	_object_test_vector (0000)
0013D7C0 06d0:
	_collision_test_vector (0000)
0013DE90 0190:
	_collision_test_vector_exit (0000)
0013E020 0170:
	_collision_test_pill (0000)
0013E190 00d0:
	_collision_test_pill_new (0000)
0013E260 0220:
	_object_get_features_in_sphere (0000)
0013E480 0200:
	_collision_get_features_in_sphere (0000)
0013E680 0040:
	_clip_position_to_plane (0000)
0013E6C0 0040:
	_clip_velocity_to_plane (0000)
0013E700 0080:
	_clip_position_to_line (0000)
0013E780 0050:
	_clip_velocity_to_line (0000)
0013E7D0 00a0:
	_collision_fix_pill_nudge_collision (0000)
0013E870 02a0:
	_collision_fix_pill (0000)
0013EB10 1290:
	_collision_move_point (0000)
0013FDA0 0180:
	_collision_move_pill (0000)
0013FF20 0040:
	_collision_move_sphere (0000)
0028BAB8 0008:
	__real@3fa0000000000000 (0000)
0028BAC0 0008:
	__real@3f30000000000000 (0000)
0028BAC8 0004:
	__real@3d000000 (0000)
0028BACC 0004:
	__real@3f441b7d (0000)
0028BAD0 0024:
	??_C@_0CE@BAGBGBHP@c?3?2halo?2SOURCE?2physics?2collision@ (0000)
0028BAF4 000d:
	??_C@_0N@LANGNGIH@new_velocity?$AA@ (0000)
0028BB04 000c:
	??_C@_0M@HGHPBCFN@?$CGclip_plane?$AA@ (0000)
0028BB10 000c:
	??_C@_0M@KHLCGDHC@?$CGclip_point?$AA@ (0000)
0028BB1C 0012:
	??_C@_0BC@GFMKCLPO@?$CGclip_line_vector?$AA@ (0000)
0028BB30 0011:
	??_C@_0BB@IPCKNOKM@?$CGclip_line_point?$AA@ (0000)
0028BB44 000d:
	??_C@_0N@HNOBANOD@clip_count?$DM3?$AA@ (0000)
0028BB54 002f:
	??_C@_0CP@BCIJNMNG@?$CFs?3?5assert_valid_real_plane3d?$CI?$CFf@ (0000)
0028BB84 0012:
	??_C@_0BC@KMBJNJFM@?$CGcollision?9?$DOplane?$AA@ (0000)
0028BB98 000a:
	??_C@_09MFIJNMKH@?$CGvelocity?$AA@ (0000)
0028BBA4 000a:
	??_C@_09KHKMDFEK@?$CGposition?$AA@ (0000)
0028BBB0 0012:
	??_C@_0BC@JCEJOAFB@?$CGclipped_velocity?$AA@ (0000)
0028BBC4 0012:
	??_C@_0BC@PAGMAJLM@?$CGclipped_position?$AA@ (0000)
0028BBD8 0028:
	??_C@_0CI@OLDCDOIO@collision_count?$DMmaximum_collisio@ (0000)
0028BC00 000d:
	??_C@_0N@MHPNJHFJ@old_velocity?$AA@ (0000)
0028BC10 000d:
	??_C@_0N@KFNIHOLE@old_position?$AA@ (0000)
0030CDE8 00cc:
	_data_0030cde8 (0000)
0045E000 001a:
	_bss_0045e000 (0000)
	_debug_collision_skip_objects (0018)
	_debug_collision_skip_vectors (0019)
*/

/* ---------- headers */

#define REAL_MATH_EXTERNAL_POINT_FROM_LINE3D
#include "cseries.h"
#include "collisions.h"

#include "breakable_surfaces.h"
#include "bsp3d.h"
#include "collision_bsp.h"
#ifdef HALO_N64
#include "collision_bsp_definitions.h"
#endif
#include "collision_features.h"
#include "collision_model_definitions.h"
#include "collision_models.h"
#include "collision_usage.h"
#include "physics.h"
#include "objects/object_types.h"
#include "objects/objects.h"
#include "game/game_globals.h"
#include "math/real_math_planes.h"
#include "scenario/fog_definitions.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "units/bipeds.h"

#undef REAL_MATH_EXTERNAL_POINT_FROM_LINE3D

/* ---------- constants */

/* ---------- macros */

/* Preserve January's in-TU scalar expansion without selecting the external
 * point_from_line3d COMDAT for this object. */
#define COLLISION_POINT_FROM_LINE3D(point, vector, distance, result) \
	do \
	{ \
		real_point3d *line_result = (result); \
		real line_distance = (distance); \
		real_vector3d const *line_vector = (vector); \
		real_point3d const *line_point = (point); \
		line_result->x = line_vector->i * line_distance + line_point->x; \
		line_result->y = line_vector->j * line_distance + line_point->y; \
		line_result->z = line_vector->k * line_distance + line_point->z; \
	} while (0)

/* ---------- structures */

struct collision_usage_times
{
	LARGE_INTEGER features;
	LARGE_INTEGER vector_objects;
	LARGE_INTEGER vector_structure;
};

/* ---------- prototypes */

boolean collision_bsp_test_pill_new(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	real *t,
	real_vector3d *normal);
boolean collision_bsp_test_pill(
	struct collision_bsp const *bsp,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	real maximum_t,
	struct collision_bsp_test_pill_result *result);
static boolean object_test_point(
	long object_index,
	unsigned long flags,
	real_point3d const *point,
	long ignore_object_index);
static boolean object_test_vector(
	long object_index,
	unsigned long flags,
	unsigned long bsp_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long ignore_object_index,
	struct collision_result *collision);
boolean collision_test_vector_exit(
	struct collision_result const *previous_collision,
	real_point3d const *point,
	real_vector3d const *vector,
	struct collision_result *exit_collision);
static void object_get_features_in_sphere(
	unsigned long flags,
	long object_index,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	long ignore_object_index,
	struct collision_feature_list *features);
static void collision_fix_pill_nudge_collision(
	unsigned long flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long ignore_object_index,
	struct collision_plane *collision);
/* ---------- globals */

static struct collision_usage_times collision_usage_times;
boolean debug_collision_skip_objects = FALSE;
#ifdef HALO_N64
/* set while a ray asks only whether anything is in the way (the bots' and
Covenant's sight: n64_bots.c, n64_firefight.c) */
int n64_collision_any_hit;
#endif
boolean debug_collision_skip_vectors = FALSE;

/* ---------- public code */

long collision_model_get_material_type(
	struct collision_model const *model,
	short material_index)
{
	long material_type;

	if (material_index != NONE)
	{
		material_type = TAG_BLOCK_GET_ELEMENT(
			&model->resistance.materials,
			material_index,
			struct damage_resistance_material)->material_type;
	}
	else
	{
		material_type = NONE;
	}

	return material_type;
}

boolean collision_test_sphere(
	real_point3d const *center,
	real radius,
	long ignore_object_index)
{
	boolean result;
	struct collision_bsp_test_sphere_result sphere_result;

	if (bsp3d_test_point(global_bsp3d_get(), 0, center) != NONE)
	{
		result = collision_bsp_test_sphere(
			global_collision_bsp_get(),
			MAXIMUM_BREAKABLE_SURFACES_PER_MAP,
			breakable_surface_flags_get(),
			center,
			radius,
			&sphere_result);
		if (!result)
		{
			return result;
		}
	}

	return TRUE;
}

boolean collision_test_point(
	unsigned long flags,
	real_point3d const *point,
	long ignore_object_index)
{
	unsigned long collision_flags = flags;
	long leaf_index;
	long reference_index;
	boolean test_objects;

	if (!(collision_flags & _collision_test_environment_flags))
	{
		goto no_collision;
	}

	leaf_index = bsp3d_test_point(global_bsp3d_get(), 0, point);
	test_objects = TEST_FLAG(collision_flags, _collision_test_objects_bit);
	if (debug_collision_skip_objects)
	{
		test_objects = FALSE;
	}

	if (leaf_index == NONE)
	{
		goto collision;
	}

	if (!test_objects)
	{
		goto no_collision;
	}

	{
		short cluster_index = TAG_BLOCK_GET_ELEMENT(
			&global_structure_bsp_get()->leaves,
			leaf_index & LONG_MAX,
			struct structure_leaf)->cluster_index;
		long object_index;

		for (object_index = cluster_get_first_collideable_object(&reference_index, cluster_index);
			object_index != NONE;
			object_index = cluster_get_next_collideable_object(&reference_index))
		{
			if (object_test_point(object_index, collision_flags, point, ignore_object_index))
			{
				goto collision;
			}
		}
	}

no_collision:
	return FALSE;

collision:
	return TRUE;
}

boolean collision_test_vector(
	unsigned long flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long ignore_object_index,
	struct collision_result *collision)
{
	boolean hit = FALSE;
	boolean test_environment = TEST_FLAG(flags, _collision_test_structure_bit) ||
		TEST_FLAG(flags, _collision_test_media_bit) ||
		TEST_FLAG(flags, _collision_test_objects_bit);

	if (debug_collision_skip_vectors)
	{
		test_environment = FALSE;
	}

#ifdef HALO_N64
	{
		/* (measuring, "debug_ray_callers": who casts rays, and how long) */
		extern volatile int n64_profile_zone;
		extern unsigned long n64_ray_callers[16][2];
		int n64_debug_flag(char const *name);
		/* (looked up once: asked for on every ray it cost more than the
		rest of this test) */
		static int ray_callers = -1;

		if (ray_callers < 0)
			ray_callers = n64_debug_flag("debug_ray_callers") != 0;
		if (ray_callers)
		{
			unsigned long caller = (unsigned long)__builtin_return_address(0);
			int i;

			for (i = 0; i < 16; i++)
			{
				if (n64_ray_callers[i][0] == caller || !n64_ray_callers[i][0])
				{
					n64_ray_callers[i][0] = caller;
					n64_ray_callers[i][1] += 1 + (n64_profile_zone ? 0x10000 : 0);
					break;
				}
			}
		}
	}
#endif

	collision->type = NONE;
	collision->start_location.leaf_index = NONE;
	collision->start_location.cluster_index = NONE;
	collision->location.leaf_index = NONE;
	collision->location.cluster_index = NONE;
	collision->t = 1.0f;

	if (test_environment)
	{
		struct structure_bsp *structure_bsp = global_structure_bsp_get();
		boolean test_objects = TEST_FLAG(flags, _collision_test_objects_bit);
		unsigned long bsp_flags;
		struct collision_bsp_test_vector_result bsp_result;

		if (debug_collision_skip_objects)
		{
			test_objects = FALSE;
		}

		if (!(flags & (FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_back_facing_surfaces_bit))))
		{
			flags |= FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_back_facing_surfaces_bit);
		}

		bsp_flags = 0;
		SET_FLAG(bsp_flags, _collision_test_front_facing_surfaces_bit, TEST_FLAG(flags, _collision_test_front_facing_surfaces_bit));
		SET_FLAG(bsp_flags, _collision_test_back_facing_surfaces_bit, TEST_FLAG(flags, _collision_test_back_facing_surfaces_bit));
		SET_FLAG(bsp_flags, _collision_test_ignore_two_sided_surfaces_bit, TEST_FLAG(flags, _collision_test_ignore_two_sided_surfaces_bit));
		SET_FLAG(bsp_flags, _collision_test_ignore_invisible_surfaces_bit, TEST_FLAG(flags, _collision_test_ignore_invisible_surfaces_bit));
		SET_FLAG(bsp_flags, _collision_test_ignore_breakable_surfaces_bit, TEST_FLAG(flags, _collision_test_ignore_breakable_surfaces_bit));

		collision_log_usage(_collision_function_vector_structure);
		collision_log_start_time(&collision_usage_times.vector_structure);
		if (collision_bsp_test_vector(
			bsp_flags,
			global_collision_bsp_get(),
			MAXIMUM_BREAKABLE_SURFACES_PER_MAP,
			breakable_surface_flags_get(),
			point,
			vector,
			REAL_MAX,
			&bsp_result) &&
			TEST_FLAG(flags, _collision_test_structure_bit))
		{
			long material_type;

			collision->t = bsp_result.t;
			collision->type = _collision_result_structure;
			collision->plane = *bsp_result.plane;
			if (bsp_result.plane_designator & LONG_MIN)
			{
				plane3d_negate(&collision->plane, &collision->plane);
			}

			if (bsp_result.material_index != NONE)
			{
				material_type = TAG_BLOCK_GET_ELEMENT(
					&structure_bsp->collision_materials,
					bsp_result.material_index,
					struct structure_collision_material)->runtime_physics_material_type;
			}
			else
			{
				material_type = NONE;
			}

			collision->material_type = (short)material_type;
			collision->surface_index = bsp_result.surface_index;
			collision->plane_designator = bsp_result.plane_designator;
			collision->flags = bsp_result.flags;
			collision->breakable_surface_index = bsp_result.breakable_surface_index;
			collision->material_index = bsp_result.material_index;
			hit = TRUE;
		}

		if (bsp_result.leaf_count > 0)
		{
			long leaf_index = bsp_result.leaf_indices[0];
			long cluster_index;

			collision->start_location.leaf_index = leaf_index;
			if (leaf_index == NONE)
			{
				cluster_index = NONE;
			}
			else
			{
				cluster_index = TAG_BLOCK_GET_ELEMENT(
					&global_structure_bsp_get()->leaves,
					leaf_index & LONG_MAX,
					struct structure_leaf)->cluster_index;
			}
			collision->start_location.cluster_index = cluster_index;

			leaf_index = bsp_result.leaf_indices[bsp_result.leaf_count - 1];
			collision->location.leaf_index = leaf_index;
			if (leaf_index == NONE)
			{
				cluster_index = NONE;
			}
			else
			{
				cluster_index = TAG_BLOCK_GET_ELEMENT(
					&global_structure_bsp_get()->leaves,
					leaf_index & LONG_MAX,
					struct structure_leaf)->cluster_index;
			}
			collision->location.cluster_index = cluster_index;
		}
		collision_log_end_time(
			_collision_function_vector_structure,
			collision_usage_times.vector_structure.QuadPart);

		if (TEST_FLAG(flags, _collision_test_media_bit) &&
			collision->location.cluster_index != NONE)
		{
			short fog_reference = TAG_BLOCK_GET_ELEMENT(
				&structure_bsp->clusters,
				collision->location.cluster_index,
				struct structure_cluster)->fog_reference;

			if (fog_reference != NONE && TEST_FLAG((word)fog_reference, 15))
			{
				struct structure_fog_plane const *fog_plane = TAG_BLOCK_GET_ELEMENT(
					&structure_bsp->fog_planes,
					fog_reference & SHORT_MAX,
					struct structure_fog_plane);

				if (fog_plane->runtime_material_type != NONE)
				{
					struct structure_fog_region const *fog_region = TAG_BLOCK_GET_ELEMENT(
						&structure_bsp->fog_regions,
						fog_plane->region_index,
						struct structure_fog_region);
					struct structure_fog_palette_entry const *fog_palette_entry = TAG_BLOCK_GET_ELEMENT(
						&structure_bsp->fog_palette,
						fog_region->fog_palette_index,
						struct structure_fog_palette_entry);
					struct fog_definition const *fog = fog_definition_get(fog_palette_entry->fog.index);
					real_plane3d plane;
					real point_distance;
					real vector_dot;

					plane = fog_plane->plane;
					plane.d -= fog->plane_distance;
					point_distance = plane3d_distance_to_point(&plane, point);
					vector_dot = dot_product3d(&plane.n, vector);

					if ((point_distance > 0.0f) != (vector_dot > 0.0f) &&
						fabs(point_distance) < fabs(vector_dot) &&
						fabs(vector_dot) >= _real_epsilon)
					{
						real t = -(point_distance / vector_dot);

						if (t < collision->t)
						{
							boolean negate = point_distance < 0.0f;
							long material_type;

							collision->t = t;
							collision->plane = plane;
							collision->type = _collision_result_media;
							if (negate)
							{
								plane3d_negate(&collision->plane, &collision->plane);
								material_type = _material_water;
							}
							else
							{
								material_type = fog_plane->runtime_material_type;
							}
							collision->material_type = (short)material_type;
							hit = TRUE;
						}
					}
				}
			}
		}

#ifdef HALO_N64
		/* (a line of sight asks only whether anything is in the way: with
		the level in the way, its objects need no test --
		n64_collision_any_hit, set around the AI's sight rays) */
		if (n64_collision_any_hit && hit)
			test_objects = FALSE;
#endif
		if (test_objects && bsp_result.leaf_count > 0)
		{
			long leaf_index;

			collision_log_usage(_collision_function_vector_objects);
			collision_log_start_time(&collision_usage_times.vector_objects);
			if (!(flags & _collision_test_objects_all_types_flags))
			{
				flags |= _collision_test_objects_all_types_flags;
			}

			structure_cluster_marker_begin();
			object_marker_begin();
			for (leaf_index = 0; leaf_index < bsp_result.leaf_count; leaf_index++)
			{
				long leaf = bsp_result.leaf_indices[leaf_index];
				long cluster_index;

				if (leaf == NONE)
				{
					cluster_index = NONE;
				}
				else
				{
					cluster_index = TAG_BLOCK_GET_ELEMENT(
						&global_structure_bsp_get()->leaves,
						leaf & LONG_MAX,
						struct structure_leaf)->cluster_index;
				}

				if (structure_cluster_mark(cluster_index))
				{
					long reference_index;
					long object_index;

					for (object_index = cluster_get_first_collideable_object(&reference_index, cluster_index);
						object_index != NONE;
						object_index = cluster_get_next_collideable_object(&reference_index))
					{
#ifdef HALO_N64
						/* (a type the ray doesn't test, by the object's header:
						object_test_vector would take nothing of it, children
						and all, and its datum -- read and marked -- is
						three or four cache misses) */
						if (!TEST_FLAG(flags, object_header_get(object_index)->type + _collision_test_objects_first_type_bit))
							continue;
#endif
						if (object_mark_function(object_index) &&
							object_test_vector(
								object_index,
								flags,
								bsp_flags,
								point,
								vector,
								ignore_object_index,
								collision))
						{
							hit = TRUE;
						}
					}
				}
			}
			object_marker_end();
			structure_cluster_marker_end();
			collision_log_end_time(
				_collision_function_vector_objects,
				collision_usage_times.vector_objects.QuadPart);
		}

		if (!hit)
		{
			collision->t = 1.0f;
		}

		COLLISION_POINT_FROM_LINE3D(point, vector, collision->t, &collision->point);
		if (TEST_FLAG(flags, _collision_test_try_to_keep_location_valid_bit) &&
			hit &&
			collision->location.leaf_index != NONE &&
			scenario_leaf_index_from_point(&collision->point) != collision->location.leaf_index)
		{
			point_from_line3d(&collision->point, &collision->plane.n, 0.00024414062f, &collision->point);
			scenario_location_from_point(&collision->location, &collision->point);
			if (collision->location.leaf_index == NONE)
			{
				real vector_dot = dot_product3d(vector, &collision->plane.n);
				real step = (vector_dot != 0.0f) ? 0.00024414062f / fabs(vector_dot) : 0.03125f;

				do
				{
					collision->t = MAX(collision->t - step, 0.0f);
					COLLISION_POINT_FROM_LINE3D(point, vector, collision->t, &collision->point);
					scenario_location_from_point(&collision->location, &collision->point);
					if (collision->t <= 0.0f)
					{
						break;
					}
				}
				while (collision->location.leaf_index == NONE);
			}
		}
	}
	else
	{
		collision->t = 1.0f;
		add_vectors3d(
			(real_vector3d const *)point,
			vector,
			(real_vector3d *)&collision->point);
		scenario_location_from_point(&collision->location, &collision->point);
	}

	return hit;
}

boolean collision_test_pill(
	unsigned long flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	long ignore_object_index,
	struct collision_result *collision)
{
	boolean hit = FALSE;
	struct collision_bsp_test_pill_result bsp_result;

	collision->type = NONE;
	collision->t = REAL_MAX;

	if (collision_bsp_test_pill(
		global_collision_bsp_get(),
		point,
		vector,
		radius,
		REAL_MAX,
		&bsp_result))
	{
		collision->t = bsp_result.t;
		if (flags & FLAG(_collision_test_structure_bit))
		{
			collision->plane = bsp_result.plane;
			collision->flags = 0;
			collision->breakable_surface_index = 0;
			collision->type = 2;
			collision->material_type = bsp_result.material_index;
			collision->surface_index = bsp_result.surface_index;
			collision->plane_designator = NONE;
			collision->material_index = bsp_result.material_index;
			hit = TRUE;
		}
	}

	if (bsp_result.leaf_count > 0)
	{
		long leaf_index = bsp_result.leaf_indices[0];
		long cluster_index;

		collision->start_location.leaf_index = leaf_index;
		if (leaf_index == NONE)
		{
			cluster_index = NONE;
		}
		else
		{
			cluster_index = TAG_BLOCK_GET_ELEMENT(
				&global_structure_bsp_get()->leaves,
				leaf_index & LONG_MAX,
				struct structure_leaf)->cluster_index;
		}
		collision->start_location.cluster_index = cluster_index;

		leaf_index = bsp_result.leaf_indices[bsp_result.leaf_count - 1];
		collision->location.leaf_index = leaf_index;
		if (leaf_index == NONE)
		{
			cluster_index = NONE;
		}
		else
		{
			cluster_index = TAG_BLOCK_GET_ELEMENT(
				&global_structure_bsp_get()->leaves,
				leaf_index & LONG_MAX,
				struct structure_leaf)->cluster_index;
		}
		collision->location.cluster_index = cluster_index;
	}

	if (!hit)
	{
		collision->t = 1.0f;
	}

	COLLISION_POINT_FROM_LINE3D(point, vector, collision->t, &collision->point);
	scenario_location_from_point(&collision->location, &collision->point);

	return hit;
}

boolean collision_test_vector_exit(
	struct collision_result const *previous_collision,
	real_point3d const *point,
	real_vector3d const *vector,
	struct collision_result *exit_collision)
{
	boolean found = FALSE;

	exit_collision->type = NONE;
	exit_collision->t = REAL_MAX;

	if (previous_collision->type == _collision_result_object)
	{
		real_point3d reverse_origin;
		real_vector3d reverse_vector;

		set_real_point3d(
			&reverse_origin,
			vector->i + point->x,
			(vector->j) + point->y,
			vector->k + point->z);
		negate_vector3d(vector, &reverse_vector);

		{
			struct collision_model_instance instance;

			if (collision_model_instance_new(&instance, previous_collision->object_index))
			{
				struct collision_model_test_vector_result result;

				if (collision_model_test_vector(
					&instance,
					FLAG(_collision_test_front_facing_surfaces_bit),
					&reverse_origin,
					&reverse_vector,
					&result))
				{
					exit_collision->type = _collision_result_object;
					exit_collision->t = 1.0f - result.bsp_result.t;
					matrix4x3_transform_plane(
						&instance.matrices[result.node_index],
						result.bsp_result.plane,
						&exit_collision->plane);
					if (result.bsp_result.plane_designator & LONG_MIN)
					{
						plane3d_negate(&exit_collision->plane, &exit_collision->plane);
					}

					exit_collision->material_type = collision_model_get_material_type(
						instance.model,
						result.bsp_result.material_index);
					exit_collision->object_index = previous_collision->object_index;
					exit_collision->region_index = result.region_index;
					exit_collision->node_index = result.node_index;
					exit_collision->bsp_index = result.bsp_index;
					exit_collision->surface_index = result.bsp_result.surface_index;
					exit_collision->plane_designator = result.bsp_result.plane_designator;
					exit_collision->flags = result.bsp_result.flags;
					exit_collision->breakable_surface_index = result.bsp_result.breakable_surface_index;
					exit_collision->material_index = result.bsp_result.material_index;
					found = TRUE;
				}
			}
		}
	}

	if (found)
	{
		COLLISION_POINT_FROM_LINE3D(point, vector, exit_collision->t, &exit_collision->point);
	}

	return found;
}

boolean collision_get_features_in_sphere(
	unsigned long flags,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	long ignore_object_index,
	struct collision_feature_list *features)
{
	unsigned long structure_flags;

	collision_features_new(features);
	structure_flags = flags & FLAG(_collision_test_structure_bit);
	if (structure_flags ||
		(flags & (FLAG(_collision_test_media_bit) | FLAG(_collision_test_objects_bit))))
	{
		struct structure_bsp const *structure_bsp = global_structure_bsp_get();
		struct collision_bsp const *collision_bsp = global_collision_bsp_get();
		boolean objects = TEST_FLAG(flags, _collision_test_objects_bit);
		struct collision_bsp_test_sphere_result result;

		if (debug_collision_skip_objects)
		{
			objects = FALSE;
		}

		collision_log_usage(_collision_function_vector_bounds_object);
		collision_log_start_time(&collision_usage_times.features);
		radius += 0.0625f;

		if (collision_bsp_test_sphere(
			collision_bsp,
			MAXIMUM_BREAKABLE_SURFACES_PER_MAP,
			breakable_surface_flags_get(),
			center,
			radius,
			&result) &&
			structure_flags)
		{
			collision_bsp_get_features_in_sphere(
				collision_bsp,
				&result,
				NULL,
				height,
				width,
				NONE,
				features);
		}

		if (objects && result.leaf_count > 0)
		{
			short leaf_reference_index;

			if (!(flags & _collision_test_objects_all_types_flags))
			{
				flags |= _collision_test_objects_all_types_flags;
			}

			structure_cluster_marker_begin();
			object_marker_begin();
			for (leaf_reference_index = 0;
				leaf_reference_index < result.leaf_count;
				leaf_reference_index++)
			{
				long leaf_index = result.leaf_indices[leaf_reference_index];
				struct structure_leaf const *leaf = TAG_BLOCK_GET_ELEMENT(
					&structure_bsp->leaves,
					leaf_index & LONG_MAX,
					struct structure_leaf);

				if (structure_cluster_mark(leaf->cluster_index))
				{
					long reference_index;
					long object_index;

					for (object_index = cluster_get_first_collideable_object(
							&reference_index,
							leaf->cluster_index);
						object_index != NONE;
						object_index = cluster_get_next_collideable_object(&reference_index))
					{
						if (object_mark_function(object_index))
						{
							object_get_features_in_sphere(
								flags,
								object_index,
								center,
								radius,
								height,
								width,
								ignore_object_index,
								features);
						}
					}
				}
			}
			object_marker_end();
			structure_cluster_marker_end();
		}

		collision_log_end_time(
			_collision_function_vector_bounds_object,
			collision_usage_times.features.QuadPart);
	}

	return features->count[_collision_feature_sphere] != 0 ||
		features->count[_collision_feature_cylinder] != 0 ||
		features->count[_collision_feature_prism] != 0;
}

boolean collision_fix_pill(
	unsigned long flags,
	real_point3d const *old_position,
	real distance,
	real height,
	real width,
	long ignore_object_index,
	real_point3d *new_position)
{
	/* Name, type and function scope: PC demo and HCEX PDBs.
	 * January confirms the 204-byte array and this function as its sole user.
	 * The declaration's precise block position is not independently attested.
	 */
	static real_vector3d offsets[17] =
	{
		{ { -1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, -1.0f, 0.0f } },
		{ { 0.0f, 1.0f, 0.0f } },
		{ { -0.70710677f, -0.70710677f, 0.0f } },
		{ { 0.70710677f, 0.70710677f, 0.0f } },
		{ { 0.70710677f, -0.70710677f, 0.0f } },
		{ { -0.70710677f, 0.70710677f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f } },
		{ { -0.70710677f, 0.0f, 0.70710677f } },
		{ { 0.70710677f, 0.0f, 0.70710677f } },
		{ { 0.0f, -0.70710677f, 0.70710677f } },
		{ { 0.0f, 0.70710677f, 0.70710677f } },
		{ { -0.57735026f, -0.57735026f, 0.57735026f } },
		{ { 0.57735026f, 0.57735026f, 0.57735026f } },
		{ { 0.57735026f, -0.57735026f, 0.57735026f } },
		{ { -0.57735026f, 0.57735026f, 0.57735026f } },
	};
	boolean result = FALSE;
	struct collision_feature_list features;
	struct collision_plane first_collision;

	match_assert(
		"c:\\halo\\SOURCE\\physics\\collisions.c",
		0x4F8,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] =
		_collision_user_bipeds;

	{
		real_point3d center;

		set_real_point3d(
			&center,
			old_position->x,
			old_position->y,
			height * 0.5f + old_position->z);
		collision_get_features_in_sphere(
			flags,
			&center,
			height * 0.5f + distance + width,
			height,
			width,
			ignore_object_index,
			&features);
	}

	if (!collision_features_test_point(&features, old_position, &first_collision) &&
		!collision_test_point(flags, old_position, ignore_object_index))
	{
		*new_position = *old_position;
		result = TRUE;
	}
	else
	{
		struct collision_plane collision;
		real_point3d fallback;
		boolean have_fallback;
		short offset_index;

		have_fallback = FALSE;
		for (offset_index = 0;
			offset_index < NUMBEROF(offsets);
			offset_index++)
		{
			real_vector3d const *offset = &offsets[offset_index];
			real_point3d candidate;

			candidate.x = offset->i * distance + old_position->x;
			candidate.y = offset->j * distance + old_position->y;
			candidate.z = offset->k * distance + old_position->z;

			if (!collision_features_test_point(&features, &candidate, &collision) &&
				!collision_test_point(flags, &candidate, ignore_object_index))
			{
				real_vector3d ground_vector;

				ground_vector.i = global_down3d->i * distance;
				ground_vector.j = global_down3d->j * distance;
				ground_vector.k = global_down3d->k * distance;
				if (collision_features_test_vector(
						&features,
						&candidate,
						&ground_vector,
						&collision) &&
					collision.plane.n.k > 0.76604444f)
				{
					collision_fix_pill_nudge_collision(
						flags,
						&candidate,
						&ground_vector,
						ignore_object_index,
						&collision);
					*new_position = collision.point;
					result = TRUE;
					break;
				}

				if (!have_fallback)
				{
					fallback = candidate;
					have_fallback = TRUE;
				}
			}
		}

		if (!result && have_fallback)
		{
			real_vector3d ground_vector;

			ground_vector.i = old_position->x - fallback.x;
			ground_vector.j = old_position->y - fallback.y;
			ground_vector.k = old_position->z - fallback.z;
			collision_features_test_vector(
				&features,
				&fallback,
				&ground_vector,
				&collision);
			collision_fix_pill_nudge_collision(
				flags,
				&fallback,
				&ground_vector,
				ignore_object_index,
				&collision);
			*new_position = collision.point;
			result = TRUE;
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\physics\\collisions.c",
		0x562,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return result;
}

boolean collision_test_pill_new(
	unsigned long flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real radius,
	long ignore_object_index,
	struct collision_result *collision)
{
	boolean result = FALSE;
	real t;
	real_vector3d normal;

	collision->type = NONE;
	collision->start_location.leaf_index = NONE;
	collision->start_location.cluster_index = NONE;
	collision->location.leaf_index = NONE;
	collision->location.cluster_index = NONE;
	collision->t = 1.0f;

	if (collision_bsp_test_pill_new(
		global_collision_bsp_get(),
		0,
		NULL,
		point,
		vector,
		radius,
		&t,
		&normal))
	{
		collision->t = t;
		collision->plane.n = normal;
		collision->type = 2;
		collision->plane.d = REAL_MAX;
		collision->material_type = NONE;
		collision->surface_index = NONE;
		collision->plane_designator = NONE;
		collision->flags = 0;
		collision->breakable_surface_index = 0;
		collision->material_index = NONE;
		result = TRUE;
	}

	COLLISION_POINT_FROM_LINE3D(point, vector, collision->t, &collision->point);
	collision->plane.n.i = 0.0f;
	collision->plane.n.j = 0.0f;
	collision->plane.n.k = 0.0f;

	return result;
}

short collision_move_sphere(
	unsigned long flags,
	real_point3d const *position,
	real_vector3d const *velocity,
	real radius,
	long ignore_object_index,
	real_point3d *clipped_position,
	real_vector3d *clipped_velocity,
	short maximum_collision_count,
	struct collision_plane *collisions)
{
	return collision_move_pill(
		flags,
		position,
		velocity,
		0.0f,
		radius,
		ignore_object_index,
		clipped_position,
		clipped_velocity,
		maximum_collision_count,
		collisions);
}

static void clip_position_to_plane(
	real_point3d const *position,
	real_plane3d const *plane,
	real_point3d *clipped_position)
{
	COLLISION_POINT_FROM_LINE3D(
		position,
		&plane->n,
		-plane3d_distance_to_point(plane, position),
		clipped_position);

	return;
}

static void clip_velocity_to_plane(
	real_vector3d const *velocity,
	real_plane3d const *plane,
	real_vector3d *clipped_velocity)
{
	real distance = -dot_product3d(velocity, &plane->n);

	clipped_velocity->i = plane->n.i * distance + velocity->i;
	clipped_velocity->j = plane->n.j * distance + velocity->j;
	clipped_velocity->k = plane->n.k * distance + velocity->k;

	return;
}

static void clip_position_to_line(
	real_point3d const *position,
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d *clipped_position)
{
	real_vector3d offset;

	vector_from_points3d(point, position, &offset);
	COLLISION_POINT_FROM_LINE3D(
		point,
		vector,
		dot_product3d(&offset, vector) / magnitude_squared3d(vector),
		clipped_position);

	return;
}

static void clip_velocity_to_line(
	real_vector3d const *velocity,
	real_point3d const *point,
	real_vector3d const *vector,
	real_vector3d *clipped_velocity)
{
	scale_vector3d(
		vector,
		dot_product3d(velocity, vector) / magnitude_squared3d(vector),
		clipped_velocity);

	return;
}

short collision_move_point(
	real_point3d const *old_position,
	real_vector3d const *old_velocity,
	struct collision_feature_list const *features,
	real_point3d *new_position,
	real_vector3d *new_velocity,
	short maximum_collision_count,
	struct collision_plane *collisions)
{
	short collision_count = 0;
	real_point3d position;
	real_vector3d velocity = *old_velocity;
	real_point3d clipped_position = *old_position;
	real_vector3d clipped_velocity = *old_velocity;
	short clip_count = 0;
	short clip_indices[3];
	real_plane3d clip_plane;
	real_point3d clip_line_point;
	real_vector3d clip_line_vector;
	real_point3d clip_point;

	match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3AD, old_position);
	match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3AE, old_velocity);
	match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3AF, features->count[_collision_feature_sphere]<=MAXIMUM_COLLISION_FEATURES_PER_TEST);
	match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3B0, features->count[_collision_feature_cylinder]<=MAXIMUM_COLLISION_FEATURES_PER_TEST);
	match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3B1, features->count[_collision_feature_prism]<=MAXIMUM_COLLISION_FEATURES_PER_TEST);

	while (!(fabs(clipped_velocity.i) < _real_epsilon &&
		fabs(clipped_velocity.j) < _real_epsilon &&
		fabs(clipped_velocity.k) < _real_epsilon))
	{
		struct collision_plane *collision = &collisions[collision_count];

		match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3BF, collision_count<maximum_collision_count);
		match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3C0, &clipped_position);
		match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3C1, &clipped_velocity);

		if (collision_features_test_vector(features, &clipped_position, &clipped_velocity, collision))
		{
			short new_clip_indices[3];
			short new_clip_count;

			collision_count++;
			position = collision->point;
			scale_vector3d(&velocity, 1.0f - collision->t, &velocity);

			match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3CC, &position);
			match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3CD, &velocity);
			match_vassert(
				"c:\\halo\\SOURCE\\physics\\collisions.c",
				0x3CE,
				valid_real_plane3d(&collision->plane),
				csprintf(
					temporary,
					"%s: assert_valid_real_plane3d(%f, %f, %f / %f)",
					"&collision->plane",
					collision->plane.n.i,
					collision->plane.n.j,
					collision->plane.n.k,
					collision->plane.d));
			match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3D1, clip_count<3);

			new_clip_count = 1;
			new_clip_indices[0] = collision_count - 1;
			clip_plane = collisions[new_clip_indices[0]].plane;
			clip_velocity_to_plane(&velocity, &clip_plane, &clipped_velocity);
			clip_position_to_plane(&position, &clip_plane, &clipped_position);

			if (clip_count > 0)
			{
				if (dot_product3d(&clipped_velocity, &collisions[clip_indices[0]].plane.n) < -_real_epsilon &&
					line_from_planes3d(
						&collisions[new_clip_indices[0]].plane,
						&collisions[clip_indices[0]].plane,
						&clip_line_point,
						&clip_line_vector))
				{
					match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3E1, &clip_line_point);
					match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3E2, &clip_line_vector);

					new_clip_count = 2;
					new_clip_indices[1] = clip_indices[0];
					clip_velocity_to_line(&velocity, &clip_line_point, &clip_line_vector, &clipped_velocity);
					clip_position_to_line(&position, &clip_line_point, &clip_line_vector, &clipped_position);

					if (clip_count > 1)
					{
						if (dot_product3d(&clipped_velocity, &collisions[clip_indices[1]].plane.n) < -_real_epsilon &&
							point_from_planes3d(
								&collisions[new_clip_indices[0]].plane,
								&collisions[new_clip_indices[1]].plane,
								&collisions[clip_indices[1]].plane,
								&clip_point))
						{
							match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3EE, &clip_point);

							new_clip_count = 3;
							new_clip_indices[2] = clip_indices[1];
							clipped_velocity.i = 0.0f;
							clipped_velocity.j = 0.0f;
							clipped_velocity.k = 0.0f;
							clipped_position = clip_point;
						}
					}
				}
				else if (clip_count > 1 &&
					dot_product3d(&clipped_velocity, &collisions[clip_indices[1]].plane.n) < -_real_epsilon &&
					line_from_planes3d(
						&collisions[new_clip_indices[0]].plane,
						&collisions[clip_indices[1]].plane,
						&clip_line_point,
						&clip_line_vector))
				{
					match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3FE, &clip_line_point);
					match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x3FF, &clip_line_vector);

					new_clip_count = 2;
					new_clip_indices[1] = clip_indices[1];
					clip_velocity_to_line(&velocity, &clip_line_point, &clip_line_vector, &clipped_velocity);
					clip_position_to_line(&position, &clip_line_point, &clip_line_vector, &clipped_position);
				}
			}

			match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x40A, &clipped_position);
			match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x40B, &clipped_velocity);

			clip_count = new_clip_count;
			csmemcpy(clip_indices, new_clip_indices, clip_count * sizeof(short));
		}
		else
		{
			clipped_position = collision->point;
			match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x418, &clipped_position);
			break;
		}

		if (collision_count >= maximum_collision_count)
		{
			break;
		}
	}

	*new_position = clipped_position;
	switch (clip_count)
	{
	case 0:
		*new_velocity = *old_velocity;
		break;

	case 1:
		match_vassert(
			"c:\\halo\\SOURCE\\physics\\collisions.c",
			0x428,
			valid_real_plane3d(&clip_plane),
			csprintf(
				temporary,
				"%s: assert_valid_real_plane3d(%f, %f, %f / %f)",
				"&clip_plane",
				clip_plane.n.i,
				clip_plane.n.j,
				clip_plane.n.k,
				clip_plane.d));
		clip_velocity_to_plane(old_velocity, &clip_plane, new_velocity);
		break;

	case 2:
		match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x42D, &clip_line_point);
		match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x42E, &clip_line_vector);
		clip_velocity_to_line(old_velocity, &clip_line_point, &clip_line_vector, new_velocity);
		break;

	case 3:
		match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x433, &clip_point);
		new_velocity->i = 0.0f;
		new_velocity->j = 0.0f;
		new_velocity->k = 0.0f;
		break;

	default:
		match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 0x438, !"unreachable");
	}

	match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x43B, new_position);
	match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x43C, new_velocity);

	if (clip_count > 1 && collision_count < maximum_collision_count)
	{
		struct collision_plane *collision = &collisions[collision_count++];
		real minimum_k;
		short steepest_clip_index;
		short clip_index;

		collision->t = collisions[clip_indices[clip_count - 1]].t;
		collision->point = collisions[clip_indices[clip_count - 1]].point;
		collision->object_index = NONE;
		collision->surface_index = NONE;
		collision->flags = 0;
		collision->breakable_surface_index = 0;
		collision->material_index = NONE;

		minimum_k = 0.0f;
		steepest_clip_index = NONE;
		for (clip_index = 0; clip_index < clip_count; clip_index++)
		{
			struct collision_plane const *clip_collision = &collisions[clip_indices[clip_index]];

			if (clip_collision->plane.n.k < minimum_k)
			{
				minimum_k = clip_collision->plane.n.k;
				steepest_clip_index = clip_index;
			}
		}

		if (clip_count == 2)
		{
			if (steepest_clip_index != NONE)
			{
				struct collision_plane const *steepest = &collisions[clip_indices[steepest_clip_index]];

				if (steepest_clip_index == 0)
				{
					cross_product3d(&clip_line_vector, &steepest->plane.n, &collision->plane.n);
				}
				else
				{
					cross_product3d(&steepest->plane.n, &clip_line_vector, &collision->plane.n);
				}
			}
			else
			{
				COLLISION_POINT_FROM_LINE3D(
					(real_point3d const *)global_up3d,
					&clip_line_vector,
					-(clip_line_vector.k / magnitude_squared3d(&clip_line_vector)),
					(real_point3d *)&collision->plane.n);
			}

			if (normalize3d(&collision->plane.n) != 0.0f)
			{
				collision->plane.d = dot_product3d((real_vector3d *)&clip_line_point, &collision->plane.n);
			}
			else
			{
				collision_count--;
			}
		}
		else
		{
			if (steepest_clip_index != NONE)
			{
				struct collision_plane const *steepest = &collisions[clip_indices[steepest_clip_index]];

				COLLISION_POINT_FROM_LINE3D(
					(real_point3d const *)global_up3d,
					&steepest->plane.n,
					-steepest->plane.n.k,
					(real_point3d *)&collision->plane.n);
				if (normalize3d(&collision->plane.n) != 0.0f)
				{
					collision->plane.d = dot_product3d((real_vector3d *)&clip_point, &collision->plane.n);
				}
				else
				{
					collision_count--;
				}
			}
			else
			{
				collision->plane.n = *global_up3d;
				collision->plane.d = dot_product3d((real_vector3d *)&clip_point, &collision->plane.n);
			}
		}
	}

	match_assert_valid_real_point3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x497, new_position);
	match_assert_valid_real_vector3d("c:\\halo\\SOURCE\\physics\\collisions.c", 0x498, new_velocity);

	return collision_count;
}

#ifdef HALO_N64
/* ---------- the level around each biped

A biped's move gathered the level's collision geometry around its pill from
scratch every tick -- a walk down the BSP and the edges of every surface near
it, about half of its physics -- for a pill that had moved a few centimetres.
Each biped keeps what a sphere a little larger than its pill's holds (the
vertices, edges and surfaces, and the clusters they are in) and, while the
sphere it needs stays inside that one, takes its features from the list:
vertices and edges by the walk's own sphere tests, surfaces by their bounding
spheres (a surface the pill can't reach may come along; it is never hit, as
the pill can't reach it). Objects move, so they are gathered fresh each tick
from the kept clusters. The list is the level's geometry: it is dropped with
the map, and when the BSP or its broken surfaces change. */

int n64_debug_flag(char const *name);
long game_time_get(void);

enum
{
	N64_PILL_CACHES = 12, /* four players and eight bots */
	N64_PILL_CACHE_VERTICES = 48,
	N64_PILL_CACHE_EDGES = 96,
	N64_PILL_CACHE_SURFACES = 64,
	N64_PILL_CACHE_CLUSTERS = 8,
	N64_PILL_CACHE_RETRY_TICKS = 30, /* after even a thin sphere held more */
};

/* the room around the pill's sphere: about three ticks of running, less
where the level is too busy for the list (halved each time it overflows,
and grown back while it has space) */
#define N64_PILL_CACHE_MARGIN 0.25f
#define N64_PILL_CACHE_MINIMUM_MARGIN 0.1f /* (thinner, it would refill every tick) */

struct n64_pill_cache
{
	long object_index;
	long last_used;
	long retry_time;
	struct collision_bsp const *bsp; /* NULL: empty */
	real_point3d center;
	real radius;
	real margin;
	short vertex_count;
	short edge_count;
	short surface_count;
	short cluster_count;
	long vertex_indices[N64_PILL_CACHE_VERTICES];
	long edge_indices[N64_PILL_CACHE_EDGES];
	long surface_indices[N64_PILL_CACHE_SURFACES];
	real_point3d surface_centers[N64_PILL_CACHE_SURFACES];
	real surface_radii[N64_PILL_CACHE_SURFACES];
	short cluster_indices[N64_PILL_CACHE_CLUSTERS];
	byte breakable_surface_flags[MAXIMUM_BREAKABLE_SURFACES_PER_MAP / 8];
};

static struct n64_pill_cache n64_pill_caches[N64_PILL_CACHES];
static boolean n64_pill_caches_off;

void n64_pill_caches_reset(
	void)
{
	short cache_index;

	for (cache_index = 0; cache_index < N64_PILL_CACHES; cache_index++)
	{
		n64_pill_caches[cache_index].object_index = NONE;
		n64_pill_caches[cache_index].last_used = LONG_MIN / 2;
		n64_pill_caches[cache_index].retry_time = LONG_MIN / 2;
		n64_pill_caches[cache_index].bsp = NULL;
		n64_pill_caches[cache_index].margin = N64_PILL_CACHE_MARGIN;
	}
	n64_pill_caches_off = n64_debug_flag("debug_no_pill_cache") != 0;

	return;
}

/* the biped's list, or another not used this tick or the last one */
static struct n64_pill_cache *n64_pill_cache_get(
	long object_index,
	long time)
{
	struct n64_pill_cache *oldest = NULL;
	short cache_index;

	for (cache_index = 0; cache_index < N64_PILL_CACHES; cache_index++)
	{
		if (n64_pill_caches[cache_index].object_index == object_index)
		{
			return &n64_pill_caches[cache_index];
		}
	}
	for (cache_index = 0; cache_index < N64_PILL_CACHES; cache_index++)
	{
		struct n64_pill_cache *cache = &n64_pill_caches[cache_index];

		/* (a time ahead of now is from before the game time went back) */
		if ((cache->last_used < time - 1 || cache->last_used > time) &&
			(!oldest || cache->last_used < oldest->last_used))
		{
			oldest = cache;
		}
	}
	if (oldest)
	{
		oldest->object_index = object_index;
		oldest->retry_time = LONG_MIN / 2;
		oldest->bsp = NULL;
		oldest->margin = N64_PILL_CACHE_MARGIN;
	}

	return oldest;
}

/* the list of what the sphere touches; FALSE when it held too much */
static boolean n64_pill_cache_fill(
	struct n64_pill_cache *cache,
	struct collision_bsp const *bsp,
	struct structure_bsp const *structure_bsp,
	byte const *breakable_surface_flags,
	real_point3d const *center,
	real radius)
{
	struct collision_bsp_test_sphere_result result;
	long index;

	cache->bsp = NULL;
	collision_bsp_test_sphere(
		bsp,
		MAXIMUM_BREAKABLE_SURFACES_PER_MAP,
		breakable_surface_flags,
		center,
		radius,
		&result);
	if (result.vertex_count > N64_PILL_CACHE_VERTICES ||
		result.edge_count > N64_PILL_CACHE_EDGES ||
		result.surface_count > N64_PILL_CACHE_SURFACES ||
		result.leaf_count >= 256)
	{
		return FALSE;
	}

	cache->cluster_count = 0;
	for (index = 0; index < result.leaf_count; index++)
	{
		short cluster_index = TAG_BLOCK_GET_ELEMENT(
			&structure_bsp->leaves,
			result.leaf_indices[index] & LONG_MAX,
			struct structure_leaf)->cluster_index;
		short known;

		for (known = 0; known < cache->cluster_count; known++)
		{
			if (cache->cluster_indices[known] == cluster_index)
				break;
		}
		if (known == cache->cluster_count)
		{
			if (cache->cluster_count == N64_PILL_CACHE_CLUSTERS)
				return FALSE;
			cache->cluster_indices[cache->cluster_count++] = cluster_index;
		}
	}

	cache->vertex_count = (short)result.vertex_count;
	csmemcpy(cache->vertex_indices, result.vertex_indices, result.vertex_count * sizeof(long));
	cache->edge_count = (short)result.edge_count;
	csmemcpy(cache->edge_indices, result.edge_indices, result.edge_count * sizeof(long));
	cache->surface_count = (short)result.surface_count;
	for (index = 0; index < result.surface_count; index++)
	{
		real_point3d points[MAXIMUM_VERTICES_PER_COLLISION_SURFACE];
		real_point3d minimum;
		real_point3d maximum;
		real_point3d *surface_center = &cache->surface_centers[index];
		real radius_squared = 0.f;
		short point_count = collision_surface_polygon(bsp, result.surface_indices[index], points);
		short point_index;

		cache->surface_indices[index] = result.surface_indices[index];
		minimum = maximum = points[0];
		for (point_index = 1; point_index < point_count; point_index++)
		{
			minimum.x = MIN(minimum.x, points[point_index].x);
			minimum.y = MIN(minimum.y, points[point_index].y);
			minimum.z = MIN(minimum.z, points[point_index].z);
			maximum.x = MAX(maximum.x, points[point_index].x);
			maximum.y = MAX(maximum.y, points[point_index].y);
			maximum.z = MAX(maximum.z, points[point_index].z);
		}
		surface_center->x = (minimum.x + maximum.x) * 0.5f;
		surface_center->y = (minimum.y + maximum.y) * 0.5f;
		surface_center->z = (minimum.z + maximum.z) * 0.5f;
		for (point_index = 0; point_index < point_count; point_index++)
		{
			radius_squared = MAX(radius_squared, distance_squared3d(surface_center, &points[point_index]));
		}
		/* (a hair more, for the rounding) */
		cache->surface_radii[index] = square_root(radius_squared) * 1.001f + 0.001f;
	}

	cache->center = *center;
	cache->radius = radius;
	csmemcpy(cache->breakable_surface_flags, breakable_surface_flags, sizeof(cache->breakable_surface_flags));
	cache->bsp = bsp;
	if (result.vertex_count * 2 <= N64_PILL_CACHE_VERTICES &&
		result.edge_count * 2 <= N64_PILL_CACHE_EDGES &&
		result.surface_count * 2 <= N64_PILL_CACHE_SURFACES)
	{
		cache->margin = MIN(cache->margin * 1.5f, N64_PILL_CACHE_MARGIN);
	}

	return TRUE;
}

/* collision_get_features_in_sphere for a biped's pill */
static boolean n64_pill_features_in_sphere(
	unsigned long flags,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	long ignore_object_index,
	struct collision_feature_list *features)
{
	struct collision_bsp const *bsp;
	struct structure_bsp const *structure_bsp;
	byte const *breakable_surface_flags;
	struct n64_pill_cache *cache;
	long time;
	real reach;
	real room;
	real radius_squared;
	boolean objects;
	short index;

	if (n64_pill_caches_off ||
		ignore_object_index == NONE ||
		!TEST_FLAG(flags, _collision_test_structure_bit) ||
		object_get_type(ignore_object_index) != _object_type_biped)
	{
		return collision_get_features_in_sphere(flags, center, radius, height, width, ignore_object_index, features);
	}

	time = game_time_get();
	cache = n64_pill_cache_get(ignore_object_index, time);
	if (cache && cache->retry_time > time + N64_PILL_CACHE_RETRY_TICKS)
	{
		cache->retry_time = LONG_MIN / 2;
	}
	if (!cache || cache->retry_time > time)
	{
		return collision_get_features_in_sphere(flags, center, radius, height, width, ignore_object_index, features);
	}
	cache->last_used = time;

	collision_features_new(features);
	bsp = global_collision_bsp_get();
	structure_bsp = global_structure_bsp_get();
	breakable_surface_flags = breakable_surface_flags_get();
	reach = radius + 0.0625f;

	room = cache->radius - reach;
	if (cache->bsp != bsp ||
		room < 0.f ||
		distance_squared3d(&cache->center, center) > room * room ||
		csmemcmp(cache->breakable_surface_flags, breakable_surface_flags, sizeof(cache->breakable_surface_flags)))
	{
		if (!n64_pill_cache_fill(
			cache,
			bsp,
			structure_bsp,
			breakable_surface_flags,
			center,
			reach + cache->margin))
		{
			cache->margin *= 0.5f;
			if (cache->margin < N64_PILL_CACHE_MINIMUM_MARGIN)
			{
				cache->margin = N64_PILL_CACHE_MARGIN;
				cache->retry_time = time + N64_PILL_CACHE_RETRY_TICKS;
			}
			return collision_get_features_in_sphere(flags, center, radius, height, width, ignore_object_index, features);
		}
	}

	/* the walk's tests, on the kept list (collision_surface_test_sphere) */
	radius = reach;
	radius_squared = radius * radius;
	for (index = 0; index < cache->vertex_count; index++)
	{
		struct collision_vertex const *vertex = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			cache->vertex_indices[index],
			struct collision_vertex);
		real delta_x = vertex->point.x - center->x;
		real delta_y = vertex->point.y - center->y;
		real delta_z = vertex->point.z - center->z;

		if (delta_x * delta_x + delta_y * delta_y + delta_z * delta_z <= radius_squared)
		{
			collision_features_from_vertex(bsp, cache->vertex_indices[index], NULL, height, width, NONE, features);
		}
	}
	for (index = 0; index < cache->edge_count; index++)
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			cache->edge_indices[index],
			struct collision_edge);
		struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[0],
			struct collision_vertex);
		struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			edge->vertex_indices[1],
			struct collision_vertex);
		real_vector3d edge_vector;

		vector_from_points3d(&vertex0->point, &vertex1->point, &edge_vector);
		if (fast_vector_intersects_sphere(&vertex0->point, &edge_vector, center, radius))
		{
			collision_features_from_edge(bsp, cache->edge_indices[index], NULL, height, width, NONE, features);
		}
	}
	for (index = 0; index < cache->surface_count; index++)
	{
		reach = radius + cache->surface_radii[index];
		if (distance_squared3d(&cache->surface_centers[index], center) <= reach * reach)
		{
			collision_features_from_surface(bsp, cache->surface_indices[index], NULL, height, width, NONE, features);
		}
	}

	/* the objects, fresh (collision_get_features_in_sphere) */
	objects = TEST_FLAG(flags, _collision_test_objects_bit);
	if (debug_collision_skip_objects)
	{
		objects = FALSE;
	}
	if (objects && cache->cluster_count > 0)
	{
		if (!(flags & _collision_test_objects_all_types_flags))
		{
			flags |= _collision_test_objects_all_types_flags;
		}

		object_marker_begin();
		for (index = 0; index < cache->cluster_count; index++)
		{
			long reference_index;
			long object_index;

			for (object_index = cluster_get_first_collideable_object(
					&reference_index,
					cache->cluster_indices[index]);
				object_index != NONE;
				object_index = cluster_get_next_collideable_object(&reference_index))
			{
				/* (an item lying about -- a weapon, equipment, garbage: no
				children -- of a type the pill doesn't collide with, by its
				header, its datum not read or marked) */
				struct object_header_datum const *header = object_header_get(object_index);

				if (TEST_FLAG(_object_mask_item, header->type) &&
					!TEST_FLAG(flags, header->type + _collision_test_objects_first_type_bit))
				{
					continue;
				}
				if (object_mark_function(object_index))
				{
					object_get_features_in_sphere(
						flags,
						object_index,
						center,
						radius,
						height,
						width,
						ignore_object_index,
						features);
				}
			}
		}
		object_marker_end();
	}

	return features->count[_collision_feature_sphere] != 0 ||
		features->count[_collision_feature_cylinder] != 0 ||
		features->count[_collision_feature_prism] != 0;
}
#endif

short collision_move_pill(
	unsigned long flags,
	real_point3d const *position,
	real_vector3d const *velocity,
	real height,
	real radius,
	long ignore_object_index,
	real_point3d *clipped_position,
	real_vector3d *clipped_velocity,
	short maximum_collision_count,
	struct collision_plane *collisions)
{
	short result = FALSE;
	struct collision_feature_list features;
	real_point3d center;

	match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 1214, global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = 7;

	center.x = position->x + velocity->i * 0.5f;
	center.y = position->y + velocity->j * 0.5f;
	center.z = height * 0.5f + (position->z + velocity->k * 0.5f);

#ifdef HALO_N64
	if (n64_pill_features_in_sphere(
#else
	if (collision_get_features_in_sphere(
#endif
		flags,
		&center,
		magnitude3d(velocity) * 0.5f + height * 0.5f + radius,
		height,
		radius,
		ignore_object_index,
		&features))
	{
		result = collision_move_point(
			position,
			velocity,
			&features,
			clipped_position,
			clipped_velocity,
			maximum_collision_count,
			collisions);
	}
	else
	{
		add_vectors3d(
			(real_vector3d const *)position,
			velocity,
			(real_vector3d *)clipped_position);
		*clipped_velocity = *velocity;
	}

	match_assert("c:\\halo\\SOURCE\\physics\\collisions.c", 1230, global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return result;
}

/* ---------- private code */

static boolean object_test_point(
	long object_index,
	unsigned long flags,
	real_point3d const *point,
	long ignore_object_index)
{
	do
	{
		struct object_datum const *object = object_get(object_index);

		if (object_index != ignore_object_index &&
			!(object->object.flags & FLAG(_object_invisible_bit)))
		{
			long object_type = object->object.type;

			if ((flags & FLAG(object_type + _collision_test_objects_first_type_bit)) &&
				point_in_sphere(point, &object->object.bounding_sphere_center, object->object.bounding_sphere_radius))
			{
				boolean hit;

				if (TEST_FLAG(_object_mask_vehicle, object_type) &&
					(flags & FLAG(_collision_test_use_vehicle_physics_bit)))
				{
					struct physics_instance instance;

					hit = physics_instance_new(&instance, object_index) &&
						physics_test_point(&instance, point);
				}
				else
				{
					struct collision_model_instance instance;

					hit = collision_model_instance_new(&instance, object_index) &&
						collision_model_test_point(&instance, point);
				}

				if (hit)
				{
					return TRUE;
				}

				if (object->object.first_child_object_index != NONE &&
					object_test_point(
						object->object.first_child_object_index,
						flags,
						point,
						ignore_object_index))
				{
					return TRUE;
				}
			}
		}

		object_index = object->object.next_object_index;
	}
	while (object_index != NONE);

	return FALSE;
}

static boolean object_test_vector(
	long object_index,
	unsigned long flags,
	unsigned long bsp_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long ignore_object_index,
	struct collision_result *collision)
{
	boolean hit = FALSE;

	do
	{
		struct object_datum const *object = object_get(object_index);

		if (object_index != ignore_object_index &&
			!TEST_FLAG(object->object.flags, _object_invisible_bit) &&
			TEST_FLAG(flags, object->object.type + _collision_test_objects_first_type_bit) &&
			fast_vector_intersects_sphere(
				point,
				vector,
				&object->object.bounding_sphere_center,
				(object->object.bounding_sphere_radius)))
		{
			if (TEST_FLAG(_object_mask_vehicle, object->object.type) &&
				TEST_FLAG(flags, _collision_test_use_vehicle_physics_bit))
			{
				struct physics_instance instance;
				struct physics_test_vector_result result;

				if (physics_instance_new(&instance, object_index) &&
					physics_test_vector(&instance, point, vector, &result) &&
					collision->t > result.t)
				{
					collision->type = _collision_result_object;
					collision->t = result.t;
					collision->plane = result.plane;
					collision->material_type = NONE;
					collision->object_index = object_index;
					collision->region_index = NONE;
					collision->node_index = NONE;
					collision->bsp_index = NONE;
					collision->surface_index = NONE;
					collision->plane_designator = NONE;
					collision->flags = 0;
					collision->breakable_surface_index = 0;
					collision->material_index = NONE;
					hit = TRUE;
				}
			}
			else
			{
				struct collision_model_instance instance;
				struct collision_model_test_vector_result result;

				if (collision_model_instance_new(&instance, object_index) &&
					collision_model_test_vector(&instance, bsp_flags, point, vector, &result) &&
					collision->t > result.bsp_result.t)
				{
					collision->type = _collision_result_object;
					collision->t = result.bsp_result.t;
					matrix4x3_transform_plane(
						&instance.matrices[result.node_index],
						result.bsp_result.plane,
						&collision->plane);
					if (result.bsp_result.plane_designator & LONG_MIN)
					{
						plane3d_negate(&collision->plane, &collision->plane);
					}

					collision->material_type = (short)collision_model_get_material_type(
						instance.model,
						result.bsp_result.material_index);
					collision->object_index = object_index;
					collision->region_index = result.region_index;
					collision->node_index = result.node_index;
					collision->bsp_index = result.bsp_index;
					collision->surface_index = result.bsp_result.surface_index;
					collision->plane_designator = result.bsp_result.plane_designator;
					collision->flags = result.bsp_result.flags;
					collision->breakable_surface_index = result.bsp_result.breakable_surface_index;
					collision->material_index = result.bsp_result.material_index;
					hit = TRUE;
				}
			}

			if (object->object.first_child_object_index != NONE &&
				object_test_vector(
					object->object.first_child_object_index,
					flags,
					bsp_flags,
					point,
					vector,
					ignore_object_index,
					collision))
			{
				hit = TRUE;
			}
		}

		object_index = object->object.next_object_index;
	}
	while (object_index != NONE);

	return hit;
}

static void object_get_features_in_sphere(
	unsigned long flags,
	long object_index,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	long ignore_object_index,
	struct collision_feature_list *features)
{
	do
	{
		struct object_datum *object = object_get(object_index);

		if (object_index != ignore_object_index &&
			!TEST_FLAG(object->object.flags, _object_invisible_bit) &&
			!TEST_FLAG(object->object.flags, _object_no_collisions_bit) &&
			(!TEST_FLAG(object->object.damage_flags, _object_dead_bit) ||
				object->object.type != _object_type_biped))
		{
			real reach = object->object.bounding_sphere_radius;

			reach += radius;
			if (point_in_sphere(
				center,
				&object->object.bounding_sphere_center,
				reach))
			{
				if (TEST_FLAG(flags, object->object.type + _collision_test_objects_first_type_bit))
				{
					switch (object->object.type)
					{
					case _object_type_biped:
						{
							struct biped_datum *biped = (struct biped_datum *)object;

							if ((!TEST_FLAG(flags, _collision_test_skip_passthrough_bipeds_bit) ||
								!TEST_FLAG(biped->biped.flags, _biped_movement_passes_through_bipeds_bit)) &&
								(object->object.parent_object_index == NONE ||
									biped->unit.parent_seat_index == NONE))
							{
								real_point3d base;
								real pill_height;
								real pill_width;

								biped_get_physics_pill(
									object_index,
									&base,
									&pill_height,
									&pill_width);
								base.z += pill_height;
								collision_features_from_point(
									&base,
									pill_height + height,
									pill_width + width,
									object_index,
									NONE,
									0,
									(byte)NONE,
									NONE,
									features);
							}
						}
						break;

					case _object_type_vehicle:
					case _object_type_scenery:
					case _object_type_machine:
					case _object_type_control:
						if (TEST_FLAG(_object_mask_vehicle, object->object.type) &&
							TEST_FLAG(flags, _collision_test_use_vehicle_physics_bit))
						{
							struct physics_instance instance;

							if (physics_instance_new(&instance, object_index))
							{
								physics_get_features_in_sphere(
									&instance,
									center,
									radius,
									height,
									width,
									features);
							}
						}
						else
						{
							struct collision_model_instance instance;

							if (collision_model_instance_new(&instance, object_index))
							{
								collision_model_get_features_in_sphere(
									&instance,
									center,
									radius,
									height,
									width,
									features);
							}
						}
						break;
					}
				}

				if (object->object.first_child_object_index != NONE)
				{
					object_get_features_in_sphere(
						flags,
						object->object.first_child_object_index,
						center,
						radius,
						height,
						width,
						ignore_object_index,
						features);
				}
			}
		}

		object_index = object->object.next_object_index;
	}
	while (object_index != NONE);

	return;
}

static void collision_fix_pill_nudge_collision(
	unsigned long flags,
	real_point3d const *point,
	real_vector3d const *vector,
	long ignore_object_index,
	struct collision_plane *collision)
{
	if (collision->t > 0.0f)
	{
		real_point3d *contact = &collision->point;

		do
		{
			real t;

			if (!collision_test_point(flags, contact, ignore_object_index))
			{
				break;
			}

			t = collision->t - 0.03125f;
			collision->t = t;
			contact->x = vector->i * t + point->x;
			contact->y = vector->j * t + point->y;
			contact->z = vector->k * t + point->z;
		}
		while (collision->t > 0.0f);
	}

	if (collision->t <= 0.0f)
	{
		collision->point = *point;
	}

	return;
}
