/*
PATH_OBSTACLE_AVOIDANCE.C

symbols in this file:
0004E6E0 0050:
	_obstacles_get_disc (0000)
0004E730 0030:
	_obstacle_from_disc (0000)
0004E760 0050:
	_path_get_step (0000)
0004E7B0 0060:
	_path_get_step_index (0000)
0004E810 0040:
	_heap_parent_index (0000)
0004E850 0010:
	_heap_left_index (0000)
0004E860 0010:
	_heap_right_index (0000)
0004E870 0060:
	_heap_cost (0000)
0004E8D0 00d0:
	_error_heap (0000)
0004E9A0 01b0:
	_heap_verify (0000)
0004EB50 0190:
	_heap_up (0000)
0004ECE0 02a0:
	_heap_down (0000)
0004EF80 0060:
	_heap_insert (0000)
0004EFE0 0070:
	_heap_remove (0000)
0004F050 0260:
	_render_debug_path (0000)
0004F2B0 0040:
	_valid_real_point2d (0000)
0004F2F0 0220:
	_path_add_step (0000)
0004F510 01e0:
	_path_new (0000)
0004F6F0 0200:
	_path_test_pill2d (0000)
0004F8F0 0330:
	_path_add_steps (0000)
0004FC20 0130:
	_path_iterate (0000)
0004FD50 0070:
	_path_find (0000)
0004FDC0 0550:
	_path_avoid_obstacles (0000)
00050310 00e0:
	_render_debug_obstacle_path (0000)
0024E4E0 005f:
	??_C@_0FP@EDEGFDAA@disc_index?$DO?$DN0?5?$CG?$CG?5disc_index?$DMobst@ (0000)
0024E540 0019:
	??_C@_0BJ@LMDHFJFI@c?3?2halo?2source?2ai?2path?4h?$AA@ (0000)
0024E560 0063:
	??_C@_0GD@BIALPJBH@step_index?$DO?$DN0?5?$CG?$CG?5step_index?$DMpath@ (0000)
0024E5C4 002c:
	??_C@_0CM@IMJGFEPB@c?3?2halo?2SOURCE?2ai?2path_obstacle_@ (0000)
0024E5F0 0063:
	??_C@_0GD@EOCGPJDJ@heap_index?$DO?$DN0?5?$CG?$CG?5heap_index?$DMpath@ (0000)
0024E654 000d:
	??_C@_0N@FHNAKGNC@heap_index?$DO0?$AA@ (0000)
0024E664 0010:
	??_C@_0BA@OJJDPBG@?$CF3d?4?5?$CF?412g?5?$CI?$CFx?$CJ?$AA@ (0000)
0024E674 0025:
	??_C@_0CF@NHJKJPIK@please?5give?5this?5debug?4txt?5to?5ch@ (0000)
0024E69C 0018:
	??_C@_0BI@GONMKDOC@heap_verify?5failed?5?$CI?$CFs?$CJ?$AA@ (0000)
0024E6B4 0013:
	??_C@_0BD@OPIGCKBC@heap_insert?5output?$AA@ (0000)
0024E6C8 0012:
	??_C@_0BC@FFIJKMBF@heap_insert?5input?$AA@ (0000)
0024E6DC 0013:
	??_C@_0BD@JEBDACBO@heap_remove?5output?$AA@ (0000)
0024E6F0 0012:
	??_C@_0BC@HBIKAONP@heap_remove?5input?$AA@ (0000)
0024E704 0005:
	??_C@_04PBNHMBFG@goal?$AA@ (0000)
0024E70C 0026:
	??_C@_0CG@LBHAPEOA@?$CFs?3?5assert_valid_real_point2d?$CI?$CFf@ (0000)
0024E734 0006:
	??_C@_05FAGFPHJG@start?$AA@ (0000)
0024E73C 0021:
	??_C@_0CB@CJEIPHLL@?$CFs?3?5assert_valid_real?$CI0x?$CF08X?5?$CFf?$CJ@ (0000)
0024E760 001d:
	??_C@_0BN@KOMMMEHH@stack_top?$DMMAXIMUM_DISC_COUNT?$AA@ (0000)
0024E780 0046:
	??_C@_0EG@PAHGCCFL@result?4disc_index?$DO?$DN0?5?$CG?$CG?5result?4d@ (0000)
0024E7C8 0042:
	??_C@_0EC@PIPIECEP@seed_disc_index?$DO?$DN0?5?$CG?$CG?5seed_disc_@ (0000)
0024E810 0052:
	??_C@_0FC@HKCAOOKO@path?9?$DOobstacles?9?$DOdisc_count?$DO?$DN0?5?$CG@ (0000)
0024E864 0017:
	??_C@_0BH@FKFIAHK@obstacle?5path?5failure?$CB?$AA@ (0000)
0024E880 0050:
	??_C@_0FA@MIKGHDND@?$CIobstacle_step_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIo@ (0000)
0024E8D0 0030:
	??_C@_0DA@BINHGJIJ@input_step_count?5?$DM?$DN?5MAXIMUM_SMOO@ (0000)
00319D08 2142:
	_debug_path (0000)
	_debug_obstacles (1538)
	_debug_obstacle_path_on_failure (2140)
	_debug_obstacle_path (2141)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"

#include "math/real_math.h"

#include "ai/path.h"
#include "ai/path_structure_bsp.h"
#include "input/input.h"
#include "main/console.h"
#include "physics/collision_bsp.h"
#include "physics/collision_bsp_definitions.h"
#include "render/render_debug.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "tag_files/tag_groups.h"

#include <stddef.h>

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct structure_bsp;

struct path_test_pill2d_result
{
	real distance;
	long surface_index;
	long edge_index;
	short disc_index;
	short obstacle_index;
};

typedef char path_test_pill2d_result_size_assert[
	sizeof(struct path_test_pill2d_result) == 0x10 ? 1 : -1];

/* ---------- prototypes */

static void error_heap(
	struct obstacle_path *path,
	short priority);
static boolean heap_verify(
	struct obstacle_path *path,
	char *message);
static void heap_up(
	struct obstacle_path *path,
	short heap_index);
static void heap_down(
	struct obstacle_path *path,
	short heap_index);
static boolean heap_insert(
	struct obstacle_path *path,
	short step_index);
static short heap_remove(
	struct obstacle_path *path);

static short path_add_step(
	struct obstacle_path *path,
	real_point2d const *point,
	long surface_index,
	short obstacle_index,
	byte obstacle_direction_index,
	real previous_distance,
	short previous_step_index);
static void path_new(
	struct obstacle_path *path,
	struct obstacles const *obstacles,
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real radius,
	real_point2d const *start,
	long start_surface_index,
	real_point2d const *goal,
	long goal_surface_index,
	boolean finishing_path,
	boolean ignore_optional);
static boolean path_test_pill2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *point,
	long surface_index,
	real_vector2d const *direction,
	real radius,
	real distance,
	boolean ignore_structure,
	boolean endpoint_is_goal,
	boolean ignore_optional,
	struct path_test_pill2d_result *result);
static void path_add_steps(
	struct obstacle_path *path,
	short step_index,
	short seed_disc_index);
static boolean path_iterate(
	struct obstacle_path *path);
static boolean path_find(
	struct obstacle_path *path,
	boolean ignore_broken_surfaces,
	struct obstacles const *obstacles,
	real radius,
	real_point2d const *start,
	long start_surface_index,
	real_point2d const *goal,
	long goal_surface_index,
	boolean finishing_path,
	boolean ignore_optional);

/* ---------- globals */

/* the failing search copied out by path_avoid_obstacles for
 * render_debug_obstacle_path to replay.
 * the names debug_path and debug_obstacles are descriptive, not recovered: no
 * first-party record names these two file statics (the feature is debug-only and
 * compiled out of every build whose symbols record statics). their storage, types
 * and January offsets (+0, +0x1538 of 0x00319D08) are January-proven. */
static struct obstacle_path debug_path;
static struct obstacles debug_obstacles;

boolean debug_obstacle_path_on_failure= FALSE;

boolean debug_obstacle_path= FALSE;


/* ---------- public code */

/* ---------- private code */

short obstacle_from_disc(
	struct obstacles const *obstacles,
	short disc_index)
{
	return disc_index!=NONE ? obstacles_get_disc(obstacles, disc_index)->obstacle_index : NONE;
}

struct obstacle_path_step *path_get_step(
	struct obstacle_path *path,
	short step_index)
{
	match_assert("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 40, step_index>=0 && step_index<path->step_count && path->step_count<=MAXIMUM_OBSTACLE_AVOIDANCE_STEPS);

	return &path->steps[step_index];
}

unsigned short path_get_step_index(
	struct obstacle_path *path,
	short heap_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
		0x31,
		heap_index>=0 && heap_index<path->heap_count && path->heap_count<=MAXIMUM_OBSTACLE_AVOIDANCE_STEPS);

	return path->heap[heap_index];
}

short heap_parent_index(
	short heap_index)
{
	match_assert("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 57, heap_index>0);

	return (short)((heap_index - 1) >> 1);
}

short heap_left_index(
	short heap_index)
{
	short result = 2 * heap_index + 1;

	return result;
}
short heap_right_index(
	short heap_index)
{
	short result = 2 * (heap_index + 1);

	return result;
}

real heap_cost(
	struct obstacle_path *path,
	short heap_index)
{
	short step_index = path_get_step_index(path, heap_index);

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
		0x28,
		step_index>=0 && step_index<path->step_count && path->step_count<=MAXIMUM_OBSTACLE_AVOIDANCE_STEPS);

	return path->steps[step_index].total_distance;
}

static void error_heap(
	struct obstacle_path *path,
	short priority)
{
	short heap_index;

	for (heap_index= 0; heap_index<path->heap_count; ++heap_index)
	{
		union
		{
			real value;
			long bits;
		} cost;

		cost.value= heap_cost(path, heap_index);
		error(priority, "%3d. %.12g (%x)", heap_index, cost.value, cost.bits);
	}

	return;
}

static boolean heap_verify(
	struct obstacle_path *path,
	char *message)
{
	short heap_index;

	for (heap_index= 1; heap_index<path->heap_count; ++heap_index)
	{
		if (heap_cost(path, heap_parent_index(heap_index))>heap_cost(path, heap_index))
		{
			error(_error_silent, "heap_verify failed (%s)", message);
			error(_error_silent, "please give this debug.txt to chucky");
			error_heap(path, _error_log);

			return FALSE;
		}
	}

	return TRUE;
}

static void heap_up(
	struct obstacle_path *path,
	short heap_index)
{
	while (heap_index>0)
	{
		short parent_index= heap_parent_index(heap_index);

		if (heap_cost(path, parent_index)>heap_cost(path, heap_index))
		{
			short step_index= path->heap[parent_index];

			path->heap[parent_index]= path->heap[heap_index];
			path->heap[heap_index]= step_index;
			heap_index= parent_index;
		}
		else
		{
			break;
		}
	}

	return;
}

static void heap_down(
	struct obstacle_path *path,
	short heap_index)
{
	if (heap_index<path->heap_count)
	{
		while (TRUE)
		{
			short smallest_index= heap_index;
			short left_index= heap_left_index(heap_index);
			short right_index= heap_right_index(heap_index);

			if (left_index<path->heap_count && heap_cost(path, heap_index)>heap_cost(path, left_index))
			{
				smallest_index= left_index;
			}
			if (right_index<path->heap_count && heap_cost(path, smallest_index)>heap_cost(path, right_index))
			{
				smallest_index= right_index;
			}

			if (smallest_index!=heap_index)
			{
				short step_index= path->heap[smallest_index];

				path->heap[smallest_index]= path->heap[heap_index];
				path->heap[heap_index]= step_index;
				heap_index= smallest_index;
			}
			else
			{
				break;
			}
		}
	}

	return;
}

static boolean heap_insert(
	struct obstacle_path *path,
	short step_index)
{
	boolean success= FALSE;

	heap_verify(path, "heap_insert input");

	if (path->heap_count<MAXIMUM_OBSTACLE_AVOIDANCE_STEPS)
	{
		short heap_index= path->heap_count;

		path->heap_count= heap_index+1;
		path_get_step(path, step_index); /* validates step_index */
		path->heap[heap_index]= step_index;
		heap_up(path, heap_index);
		heap_verify(path, "heap_insert output");
		success= TRUE;
	}

	return success;
}

static short heap_remove(
	struct obstacle_path *path)
{
	heap_verify(path, "heap_remove input");

	if (path->heap_count>0)
	{
		short step_index;

		path->heap_count-= 1;
		step_index= path->heap[0];
		path->heap[0]= path->heap[path->heap_count];
		heap_down(path, 0);
		heap_verify(path, "heap_remove output");

		return step_index;
	}

	return NONE;
}

void render_debug_path(
	struct obstacle_path *path)
{
	if (path->step_count>0)
	{
		struct collision_bsp *bsp= TAG_BLOCK_GET_ELEMENT(&path->structure->collision_bsp, 0, struct collision_bsp);
		long on_path[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_OBSTACLE_AVOIDANCE_STEPS)];
		short step_index;

		{
			real_point3d start_point;
			real_point3d goal_point;

			collision_surface_project_point2d(bsp, path->steps[0].surface_index, _z, TRUE, &path->steps[0].point, &start_point);
			collision_surface_project_point2d(bsp, path->goal_surface_index, _z, TRUE, &path->goal, &goal_point);
			render_debug_point(TRUE, &start_point, 0.125f, path->goal_step_index!=NONE ? global_real_argb_white : global_real_argb_yellow);
			render_debug_point(TRUE, &goal_point, 0.125f, path->goal_step_index!=NONE ? global_real_argb_white : global_real_argb_yellow);
		}

		step_index= path->goal_step_index;
		csmemset(on_path, 0, BIT_VECTOR_SIZE_IN_BYTES(path->step_count));
		while (step_index!=NONE)
		{
			struct obstacle_path_step *step= path_get_step(path, step_index);

			BIT_VECTOR_SET_FLAG(on_path, step_index, TRUE);
			step_index= step->previous_step_index;
		}

		for (step_index= 0; step_index<path->step_count; ++step_index)
		{
			struct obstacle_path_step *step= path_get_step(path, step_index);

			if (step->previous_step_index!=NONE)
			{
				struct obstacle_path_step *previous_step= path_get_step(path, step->previous_step_index);
				real_point3d previous_point;
				real_point3d point;

				collision_surface_project_point2d(bsp, previous_step->surface_index, _z, TRUE, &previous_step->point, &previous_point);
				collision_surface_project_point2d(bsp, step->surface_index, _z, TRUE, &step->point, &point);
				render_debug_line(TRUE, &previous_point, &point,
					BIT_VECTOR_TEST_FLAG(on_path, step_index) ? global_real_argb_white :
						(step->obstacle_direction_index ? global_real_argb_green : global_real_argb_red));
			}
		}
	}

	return;
}

static short path_add_step(
	struct obstacle_path *path,
	real_point2d const *point,
	long surface_index,
	short obstacle_index,
	byte obstacle_direction_index,
	real previous_distance,
	short previous_step_index)
{
	short step_index= NONE;

	if (path->step_count<MAXIMUM_OBSTACLE_AVOIDANCE_STEPS)
	{
		boolean blocks_goal= FALSE;
		real_vector2d direction;
		short chain_step_index;
		struct obstacle_path_step *step;
		real distance;

		vector_from_points2d(point, &path->goal, &direction);

		/* walk back along the steps that already wrap this obstacle */
		for (chain_step_index= previous_step_index; chain_step_index!=NONE; )
		{
			struct obstacle_path_step *chain_step= path_get_step(path, chain_step_index);

			if (chain_step->obstacle_index==obstacle_index)
			{
				if (chain_step->obstacle_direction_index!=obstacle_direction_index)
				{
					goto done;
				}
				chain_step_index= chain_step->previous_step_index;
			}
			else
			{
				if (obstacle_index==path->goal_obstacle_index && path->goal_obstacle_index!=NONE)
				{
					short opposite_step_index= chain_step->obstructed_goal_step_indices[!obstacle_direction_index];

					blocks_goal= TRUE;

					if (opposite_step_index!=NONE)
					{
						struct obstacle_path_step *opposite_step= path_get_step(path, opposite_step_index);
						struct obstacle_path_step *previous_step= path_get_step(path, previous_step_index);

						if (dot_product2d(&opposite_step->direction, &direction)>0.f &&
							cross_product2d(&opposite_step->direction, &previous_step->direction)*
								cross_product2d(&opposite_step->direction, &direction)<0.f)
						{
							goto done;
						}
					}

					if (chain_step->obstructed_goal_step_indices[obstacle_direction_index]==previous_step_index ||
						chain_step->obstructed_goal_step_indices[obstacle_direction_index]==NONE)
					{
						chain_step->obstructed_goal_step_indices[obstacle_direction_index]= path->step_count;
					}
				}

				break;
			}
		}

		step_index= path->step_count;
		path->step_count= step_index+1;
		step= path_get_step(path, step_index);
		step->point= *point;
		step->surface_index= surface_index;
		step->direction= direction;
		distance= normalize2d(&step->direction);
		step->distance= distance;
		step->total_distance= distance+previous_distance;
		step->obstacle_index= obstacle_index;
		step->obstacle_direction_index= obstacle_direction_index;
		step->previous_step_index= previous_step_index;
		csmemset(step->obstructed_goal_step_indices, NONE, sizeof(step->obstructed_goal_step_indices));

		if (blocks_goal && step->distance<path->best_goal_blocked_distance)
		{
			path->best_goal_blocked_distance= step->distance;
			path->best_goal_blocked_step_index= step_index;
		}

		match_vassert("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 420, heap_insert(path, step_index), NULL);
	}

done:
	return step_index;
}

static void path_new(
	struct obstacle_path *path,
	struct obstacles const *obstacles,
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real radius,
	real_point2d const *start,
	long start_surface_index,
	real_point2d const *goal,
	long goal_surface_index,
	boolean finishing_path,
	boolean ignore_optional)
{
	match_assert_valid_real("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 440, radius);
	match_assert_valid_real_point2d("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 441, start);
	match_assert_valid_real_point2d("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 442, goal);

	path->obstacles= obstacles;
	path->radius= radius;
	path->structure= structure;
	path->ignore_broken_surfaces= ignore_broken_surfaces;
	path->goal_found_exactly= FALSE;
	path->goal= *goal;
	path->goal_surface_index= goal_surface_index;
	path->goal_obstacle_index= obstacle_from_disc(obstacles, obstacles_test_circle(obstacles, NONE, goal, radius));
	path->finishing= finishing_path;
	path->goal_step_index= NONE;
	path->best_goal_blocked_distance= REAL_MAX;
	path->best_goal_blocked_step_index= NONE;
	path->ignore_optional= ignore_optional;
	path->step_count= 0;
	path->heap_count= 0;

	path_add_step(path, start, start_surface_index, NONE, 0, 0.f, NONE);

	return;
}

static boolean path_test_pill2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *point,
	long surface_index,
	real_vector2d const *direction,
	real radius,
	real distance,
	boolean ignore_structure,
	boolean endpoint_is_goal,
	boolean ignore_optional,
	struct path_test_pill2d_result *result)
{
	boolean hit;
	struct structure_test_ray2d_result ray_result;

	result->distance= distance;
	result->surface_index= NONE;
	result->edge_index= NONE;
	result->disc_index= NONE;
	result->obstacle_index= NONE;

	if (endpoint_is_goal)
	{
		result->distance= distance-radius;
	}

	if (!ignore_structure)
	{
		real_vector2d perpendicular;
		real_point2d left_point;
		real_point2d right_point;
		struct structure_test_ray2d_result left_result;
		struct structure_test_ray2d_result right_result;

		if (structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, direction, result->distance, &ray_result) &&
			result->distance>ray_result.distance)
		{
			result->distance= ray_result.distance;
			result->edge_index= ray_result.edge_index;
		}

		perpendicular2d(direction, &perpendicular);

		point_from_line2d(point, &perpendicular, radius, &left_point);
		structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, &perpendicular, radius, &left_result);
		if (structure_test_ray2d(structure, ignore_broken_surfaces, &left_point, left_result.surface_index, direction, result->distance, &left_result) &&
			result->distance>left_result.distance)
		{
			result->distance= left_result.distance;
			result->edge_index= left_result.edge_index;
		}

		point_from_line2d(point, &perpendicular, -radius, &right_point);
		structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, &perpendicular, radius, &right_result);
		if (structure_test_ray2d(structure, ignore_broken_surfaces, &right_point, right_result.surface_index, direction, result->distance, &right_result) &&
			result->distance>right_result.distance)
		{
			result->distance= right_result.distance;
			result->edge_index= right_result.edge_index;
		}
	}

	{
		struct obstacles_test_pill_result pill_result;

		if (obstacles_test_pill(obstacles, ignore_disc_index, point, direction, radius, result->distance, ignore_optional, &pill_result) &&
			result->distance>pill_result.distance)
		{
			result->distance= pill_result.distance;
			result->edge_index= NONE;
			result->disc_index= pill_result.disc_index;
			result->obstacle_index= pill_result.obstacle_index;
		}
	}

	if (result->edge_index==NONE && result->disc_index==NONE)
	{
		hit= FALSE;
		result->distance= distance;
	}
	else
	{
		hit= TRUE;
	}

	structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, direction, result->distance, &ray_result);
	result->surface_index= ray_result.surface_index;

	return hit;
}

static void path_add_steps(
	struct obstacle_path *path,
	short step_index,
	short seed_disc_index)
{
	struct obstacle_path_step *step= path_get_step(path, step_index);
	long visited[BIT_VECTOR_SIZE_IN_LONGS(MAXIMUM_DISC_COUNT)];
	short stack[MAXIMUM_DISC_COUNT];
	short stack_top;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
		594,
		path->obstacles->disc_count>=0 && path->obstacles->disc_count<=MAXIMUM_DISC_COUNT);
	csmemset(visited, 0, BIT_VECTOR_SIZE_IN_BYTES(path->obstacles->disc_count));

	match_assert(
		"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
		597,
		seed_disc_index>=0 && seed_disc_index<path->obstacles->disc_count);
	BIT_VECTOR_SET_FLAG(visited, seed_disc_index, TRUE);
	stack[0]= seed_disc_index;
	stack_top= 1;

	do
	{
		short disc_index= stack[--stack_top];
		short obstacle_index= obstacle_from_disc(path->obstacles, disc_index);
		real_vector2d directions[2];
		real tangent_distance;
		short side;

		obstacles_disc_tangents(path->obstacles, disc_index, &step->point, path->radius, &directions[0], &directions[1], &tangent_distance);
		if (tangent_distance<path->radius)
		{
			tangent_distance= path->radius;
		}

		for (side= 0; side<2; ++side)
		{
			real_vector2d const *direction= &directions[side];
			struct path_test_pill2d_result result;

			path_test_pill2d(path->structure, path->ignore_broken_surfaces, path->obstacles, disc_index,
				&step->point, step->surface_index, direction, path->radius, 2.f*path->radius+tangent_distance,
				FALSE, FALSE, path->ignore_optional, &result);

			if (result.disc_index!=NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
					625,
					result.disc_index>=0 && result.disc_index<path->obstacles->disc_count);

				if (!BIT_VECTOR_TEST_FLAG(visited, result.disc_index))
				{
					BIT_VECTOR_SET_FLAG(visited, result.disc_index, TRUE);
					match_assert("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 631, stack_top<MAXIMUM_DISC_COUNT);
					stack[stack_top++]= result.disc_index;
				}
			}

			if (result.distance>tangent_distance && result.obstacle_index!=obstacle_index)
			{
				real midpoint_distance= (result.distance+tangent_distance)*0.5f;
				struct structure_test_ray2d_result ray_result;
				real_point2d midpoint;

				structure_test_ray2d(path->structure, path->ignore_broken_surfaces, &step->point, step->surface_index, direction, midpoint_distance, &ray_result);
				point_from_line2d(&step->point, direction, midpoint_distance, &midpoint);
				path_add_step(path, &midpoint, ray_result.surface_index, obstacle_index, (byte)side,
					step->total_distance-step->distance+midpoint_distance, step_index);
			}
		}
	}
	while (stack_top>0);

	return;
}

static boolean path_iterate(
	struct obstacle_path *path)
{
	short step_index= heap_remove(path);

	if (step_index!=NONE)
	{
		struct obstacle_path_step *step= path_get_step(path, step_index);
		struct path_test_pill2d_result result;

		path_test_pill2d(path->structure, path->ignore_broken_surfaces, path->obstacles, NONE,
			&step->point, step->surface_index, &step->direction, path->radius, step->distance,
			step->previous_step_index==NONE, TRUE, path->ignore_optional, &result);

		if (result.edge_index==NONE)
		{
			if (result.disc_index==NONE)
			{
				if (result.surface_index==path->goal_surface_index ||
					structure_surfaces_are_equivalent(path->structure, &path->goal, path->goal_surface_index, result.surface_index))
				{
					real_point2d point;

					point_from_line2d(&step->point, &step->direction, result.distance, &point);
					path->goal_step_index= path_add_step(path, &point, result.surface_index, NONE, 0,
						step->total_distance-step->distance+result.distance, step_index);
				}
			}
			else
			{
				if (result.obstacle_index==path->goal_obstacle_index && step->distance<path->best_goal_blocked_distance)
				{
					path->best_goal_blocked_distance= step->distance;
					path->best_goal_blocked_step_index= step_index;
				}
				path_add_steps(path, step_index, result.disc_index);
			}
		}
	}

	return path->goal_step_index==NONE && path->heap_count>0;
}

static boolean path_find(
	struct obstacle_path *path,
	boolean ignore_broken_surfaces,
	struct obstacles const *obstacles,
	real radius,
	real_point2d const *start,
	long start_surface_index,
	real_point2d const *goal,
	long goal_surface_index,
	boolean finishing_path,
	boolean ignore_optional)
{
	path_new(path, obstacles, global_structure_bsp_get(), ignore_broken_surfaces, radius,
		start, start_surface_index, goal, goal_surface_index, finishing_path, ignore_optional);
	while (path_iterate(path))
	{
	}

	if (path->goal_step_index!=NONE)
	{
		path->goal_found_exactly= TRUE;
	}
	else
	{
		if (path->best_goal_blocked_step_index!=NONE)
		{
			path->goal_step_index= path->best_goal_blocked_step_index;
		}
	}

	return path->goal_step_index!=NONE;
}

boolean path_avoid_obstacles(
	struct path_state *state,
	short input_step_count,
	struct path_step const *input_steps,
	short *avoided_step_count,
	struct path_step *avoided_steps,
	boolean *steps_finish_path)
{
	struct collision_bsp *bsp= global_collision_bsp_get();
	real radius= MAX(state->input.pathfinding_radius, 0.2f);
	boolean result= TRUE;
	struct obstacles local_obstacles;
	struct obstacle_path local_obstacle_path;
	struct path_step temporary_steps[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	real_point3d last_step_end_point;
	long previous_surface_index;
	short step_index;

	if (state->debug && !state->debug->use_stored_obstacles)
	{
		state->debug->avoidance_path_count= 0;
		match_assert("c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c", 761, input_step_count <= MAXIMUM_SMOOTHED_PATH_STEPS);
	}

	for (step_index= 0; step_index<input_step_count; ++step_index)
	{
		struct obstacles *obstacles= &local_obstacles;
		struct obstacle_path *obstacle_path= &local_obstacle_path;
		boolean last_step= step_index==input_step_count-1;
		boolean finishing= last_step && *steps_finish_path;
		real_point3d const *start_point;
		long start_surface_index;
		real_point3d const *goal_point;
		long goal_surface_index;
		real_vector3d movement_direction;
		boolean compute_obstacles;
		boolean path_found;

		if (step_index>0)
		{
			start_point= &last_step_end_point;
			start_surface_index= previous_surface_index;
		}
		else
		{
			start_point= &state->input.start_point;
			start_surface_index= state->input.start_surface_index;
		}

		goal_point= &input_steps[step_index].point;
		goal_surface_index= input_steps[step_index].surface_index;
		vector_from_points3d(start_point, goal_point, &movement_direction);
		normalize3d(&movement_direction);

		compute_obstacles= TRUE;
		if (state->debug)
		{
			obstacles= &state->debug->avoidance_obstacles[step_index];
			obstacle_path= &state->debug->avoidance_paths[step_index];
			if (state->debug->use_stored_obstacles && step_index<state->debug->avoidance_path_count)
			{
				compute_obstacles= FALSE;
			}
		}

		if (compute_obstacles)
		{
			obstacles_new(obstacles);
			obstacles_get_discs_in_sphere(obstacles, start_point, 4.f, &movement_direction,
				state->input.ignore_source_object_index, state->input.ignore_target_object_index);
			if (state->input.attractor_valid)
			{
				obstacles_add_disc(obstacles, state->input.attractor_object_index, FLAG(_disc_optional_bit),
					&state->input.attractor_point, state->input.attractor_radius);
			}
			obstacles_recompute(obstacles, radius);
			if (state->debug && !state->debug->use_stored_obstacles)
			{
				state->debug->avoidance_path_count+= 1;
			}
		}

		/* the search is planar: only x and y of the 3d points are read */
		path_found= path_find(obstacle_path, state->input.ignore_broken_surfaces, obstacles, radius,
			(real_point2d const *)start_point, start_surface_index,
			(real_point2d const *)goal_point, goal_surface_index, finishing, FALSE);
		if (!path_found && obstacles->disc_optional_count>0)
		{
			path_found= path_find(obstacle_path, state->input.ignore_broken_surfaces, obstacles, radius,
				(real_point2d const *)start_point, start_surface_index,
				(real_point2d const *)goal_point, goal_surface_index, finishing, TRUE);
		}

		if (path_found)
		{
			boolean overflow= FALSE;
			short step_count= 0;
			short obstacle_step_index;
			short avoided_count;
			short i;

			if (obstacle_path->goal_found_exactly)
			{
				last_step_end_point= *goal_point;
				previous_surface_index= goal_surface_index;
			}
			else
			{
				struct obstacle_path_step *goal_step= path_get_step(obstacle_path, obstacle_path->goal_step_index);

				previous_surface_index= goal_step->surface_index;
				collision_surface_project_point2d(bsp, previous_surface_index, _z, TRUE, &goal_step->point, &last_step_end_point);
			}

			obstacle_step_index= obstacle_path->goal_step_index;
			while (obstacle_step_index!=0)
			{
				struct path_step *avoided_step= &temporary_steps[step_count++];
				struct obstacle_path_step *obstacle_step= path_get_step(obstacle_path, obstacle_step_index);

				avoided_step->surface_index= obstacle_step->surface_index;
				collision_surface_project_point2d(bsp, obstacle_step->surface_index, _z, TRUE, &obstacle_step->point, &avoided_step->point);
				obstacle_step_index= obstacle_step->previous_step_index;
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path_obstacle_avoidance.c",
					875,
					(obstacle_step_index >= 0) && (obstacle_step_index < obstacle_path->step_count));

				if (step_count>=MAXIMUM_OBSTACLE_AVOIDANCE_STEPS)
				{
					overflow= TRUE;
					break;
				}
			}

			avoided_count= *avoided_step_count;
			for (i= step_count-1; i>=0; --i)
			{
				if (avoided_count>=MAXIMUM_SMOOTHED_PATH_STEPS)
				{
					overflow= TRUE;
					break;
				}
				avoided_steps[avoided_count++]= temporary_steps[i];
			}
			*avoided_step_count= avoided_count;

			if (overflow)
			{
				*steps_finish_path= FALSE;
				break;
			}
		}
		else
		{
			if (debug_obstacle_path_on_failure)
			{
				error(_error_silent, "obstacle path failure!");
				debug_obstacle_path_on_failure= FALSE;
				debug_obstacle_path= TRUE;
				debug_obstacle_path_start_point= *start_point;
				debug_obstacle_path_start_surface_index= start_surface_index;
				debug_obstacle_path_goal_point= *goal_point;
				debug_obstacle_path_goal_surface_index= goal_surface_index;
				debug_obstacle_path_radius= radius;
				debug_obstacle_path_finishing= finishing;
				debug_ignore_broken_surfaces= state->input.ignore_broken_surfaces;
				debug_obstacles= *obstacles;
				debug_path= *obstacle_path;
			}

			result= FALSE;
			break;
		}
	}

	return result;
}

void render_debug_obstacle_path(
	void)
{
	if (debug_obstacle_path)
	{
		if (!console_is_active())
		{
			if (input_key_is_down(_key_r))
			{
				debug_obstacle_path_on_failure = TRUE;
				debug_obstacle_path = FALSE;
			}

			if (input_key_is_down(_key_g))
			{
				debug_obstacle_path_on_failure = FALSE;
				debug_obstacle_path = FALSE;
			}

			if (input_key_is_down(_key_n))
			{
				path_new(
					&debug_path,
					&debug_obstacles,
					global_structure_bsp_get(),
					debug_ignore_broken_surfaces,
					debug_obstacle_path_radius,
					(real_point2d const *)&debug_obstacle_path_start_point,
					debug_obstacle_path_start_surface_index,
					(real_point2d const *)&debug_obstacle_path_goal_point,
					debug_obstacle_path_goal_surface_index,
					debug_obstacle_path_finishing,
					FALSE);
			}

			if (input_key_is_down(_key_i))
			{
				path_iterate(&debug_path);
			}
		}

		render_debug_obstacles(&debug_obstacles, debug_obstacle_path_radius);
		render_debug_path(&debug_path);
	}

	return;
}
