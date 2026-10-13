/*
PATH_SMOOTHING.C

symbols in this file:
00051190 0080:
	_surface_is_walkable (0000)
00051210 0150:
	_find_tangent_point (0000)
00051360 0120:
	_find_avoidance_point (0000)
00051480 0220:
	_choose_turning_point (0000)
000516A0 0380:
	_find_turning_point (0000)
00051A20 0360:
	_path_smooth (0000)
0024EAB0 0046:
	??_C@_0EG@DGPBJLIB@TEST_FLAG?$CIcollision_surface?9?$DOfla@ (0000)
0024EAF8 0023:
	??_C@_0CD@IKIHDGOB@c?3?2halo?2SOURCE?2ai?2path_smoothing@ (0000)
0024EB20 006d:
	??_C@_0GN@DGMBNALI@collision_edge?9?$DOvertex_indices?$FL0@ (0000)
0024EB90 0024:
	??_C@_0CE@JGDDFPAJ@clockwise?$DN?$DNTRUE?5?$HM?$HM?5clockwise?$DN?$DNFA@ (0000)
0024EBB4 0012:
	??_C@_0BC@GBNEHDPD@steps_finish_path?$AA@ (0000)
0024EBC8 000f:
	??_C@_0P@LBJDKFJI@smoothed_steps?$AA@ (0000)
0024EBD8 0014:
	??_C@_0BE@FBKMNIEA@smoothed_step_count?$AA@ (0000)
0024EBEC 000a:
	??_C@_09LCGCKCI@raw_steps?$AA@ (0000)
0024EBF8 0013:
	??_C@_0BD@DLANHJEI@raw_step_count?5?$DO?50?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "ai/path.h"
#include "ai/path_structure_bsp.h"
#include "math/real_math.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_bsp.h"
#include "physics/collision_bsp_definitions.h"
#include "structures/structure_bsp_definitions.h"

/* ---------- constants */

enum
{
	_collision_surface_breakable_bit = 3,
	_pathfinding_surface_walkable_bit = 6,
	_pathfinding_surface_breakable_bit = 7,
	_path_test_pill_endpoint_near_wall_ok_bit = 0,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

static boolean surface_is_walkable(
	byte const *pathfinding_surfaces,
	struct collision_bsp const *bsp,
	byte const *breakable_surface_flags,
	long surface_index,
	boolean ignore_broken_surfaces)
{
	byte pathfinding_surface_flags;
	boolean walkable;
	struct collision_surface const *collision_surface;
	byte breakable_surface_index;

	pathfinding_surface_flags = pathfinding_surfaces[surface_index];
	walkable = TEST_FLAG(pathfinding_surface_flags, _pathfinding_surface_walkable_bit);

	if (!ignore_broken_surfaces &&
		walkable &&
		TEST_FLAG(pathfinding_surface_flags, _pathfinding_surface_breakable_bit))
	{
		collision_surface = TAG_BLOCK_GET_ELEMENT(
			&bsp->surfaces,
			surface_index,
			struct collision_surface);
#line 482 "c:\\halo\\SOURCE\\ai\\path_smoothing.c"
		match_assert(__FILE__, __LINE__, TEST_FLAG(collision_surface->flags, _collision_surface_breakable_bit));
		breakable_surface_index = collision_surface->breakable_surface_index;
		walkable = BIT_VECTOR_TEST_FLAG(
			(long const *)breakable_surface_flags,
			breakable_surface_index);
	}

	return walkable;
}

static void find_tangent_point(
	real_point2d const *point,
	real_point2d const *center,
	real radius,
	boolean clockwise,
	real_point2d *tangent_point)
{
	real_vector2d center_to_point;
	real distance_squared;
	real inverse_distance_squared;
	real tangent_length;

	vector_from_points2d(center, point, &center_to_point);
	distance_squared = magnitude_squared2d(&center_to_point);
	inverse_distance_squared = radius / distance_squared;
	tangent_length = distance_squared - radius*radius;

	if (tangent_length > 0.0f)
	{
		real_point2d tangent_points[2];
		real_vector2d tangent_vectors[2];

		tangent_length = square_root(tangent_length);

		set_real_point2d(
			&tangent_points[0],
			(center_to_point.i*radius + center_to_point.j*tangent_length)*inverse_distance_squared + center->x,
			(center_to_point.j*radius - center_to_point.i*tangent_length)*inverse_distance_squared + center->y);
		set_real_point2d(
			&tangent_points[1],
			(center_to_point.i*radius - center_to_point.j*tangent_length)*inverse_distance_squared + center->x,
			(center_to_point.j*radius + center_to_point.i*tangent_length)*inverse_distance_squared + center->y);

		vector_from_points2d(point, &tangent_points[0], &tangent_vectors[0]);
		vector_from_points2d(point, &tangent_points[1], &tangent_vectors[1]);

		*tangent_point = tangent_points[(cross_product2d(&tangent_vectors[0], &tangent_vectors[1]) > 0.0f) != clockwise];
	}
	else
	{
		real_vector2d radius_vector;

		vector_from_points2d(center, point, &radius_vector);

		if (normalize2d(&radius_vector) == 0.0f)
			radius_vector = *global_left2d;

		point_from_line2d(center, &radius_vector, radius, tangent_point);
	}

	return;
}

static void find_avoidance_point(
	real_point2d const *tangent_points,
	real_point2d const *center,
	real_point2d const *start_point,
	real radius,
	real_point2d *avoidance_point)
{
	real_vector2d center_to_tangent[2];
	real cross;
	real scale;
	real magnitude;
	real_vector2d direction;

	vector_from_points2d(center, &tangent_points[0], &center_to_tangent[0]);
	vector_from_points2d(center, &tangent_points[1], &center_to_tangent[1]);
	cross = cross_product2d(&center_to_tangent[0], &center_to_tangent[1]);

	if (!(fabs(cross) < _real_epsilon))
	{
		real_vector2d avoidance;

		scale = radius*radius / cross;
		avoidance.j = (center_to_tangent[0].i - center_to_tangent[1].i)*scale + center->y;
		avoidance.i = center->x - (center_to_tangent[0].j - center_to_tangent[1].j)*scale;
		avoidance_point->x = avoidance.i;
		avoidance_point->y = avoidance.j;

		direction.i = avoidance_point->x - center->x;
		direction.j = avoidance_point->y - center->y;
		if (!(direction.i*direction.i + direction.j*direction.j > radius*radius*4.0f))
			return;
	}

	direction.i = tangent_points[0].x - start_point->x;
	direction.j = tangent_points[0].y - start_point->y;
	magnitude = (real)sqrt(direction.i*direction.i + direction.j*direction.j);
	if (!(_real_epsilon > fabs(magnitude - 0.0f)))
	{
		scale = 1.0f / magnitude;
		direction.i = direction.i*scale;
		direction.j = direction.j*scale;
	}
	else
	{
		magnitude = 0.0f;
	}
	if (magnitude == 0.0f)
		direction = *global_left2d;

	avoidance_point->x = direction.i*radius + tangent_points[0].x;
	avoidance_point->y = direction.j*radius + tangent_points[0].y;

	return;
}

static boolean choose_turning_point(
	real_point2d const *start_point,
	real_point2d const *clockwise_turning_point,
	real_point2d const *counterclockwise_turning_point,
	real_point2d const *unobstructed_path_point,
	real_point2d const *obstructed_path_point,
	real_point2d *result)
{
	real_vector2d start_to_clockwise;
	real_vector2d clockwise_to_unobstructed;
	real_vector2d clockwise_to_obstructed;
	real_vector2d start_to_counterclockwise;
	real_vector2d counterclockwise_to_unobstructed;
	real_vector2d counterclockwise_to_obstructed;
	real clockwise_turn;
	real counterclockwise_turn;

	normalize2d(vector_from_points2d(
		clockwise_turning_point,
		start_point,
		&start_to_clockwise));

	normalize2d(vector_from_points2d(
		clockwise_turning_point,
		unobstructed_path_point,
		&clockwise_to_unobstructed));

	normalize2d(vector_from_points2d(
		clockwise_turning_point,
		obstructed_path_point,
		&clockwise_to_obstructed));

	normalize2d(vector_from_points2d(
		counterclockwise_turning_point,
		start_point,
		&start_to_counterclockwise));

	normalize2d(vector_from_points2d(
		counterclockwise_turning_point,
		unobstructed_path_point,
		&counterclockwise_to_unobstructed));

	normalize2d(vector_from_points2d(
		counterclockwise_turning_point,
		obstructed_path_point,
		&counterclockwise_to_obstructed));

	clockwise_turn =
		signed_angle_between_vectors2d(&clockwise_to_unobstructed, &clockwise_to_obstructed) +
		signed_angle_between_vectors2d(&start_to_clockwise, &clockwise_to_unobstructed);
	counterclockwise_turn =
		signed_angle_between_vectors2d(&counterclockwise_to_unobstructed, &counterclockwise_to_obstructed);

	if (clockwise_turn >
		-(counterclockwise_turn +
			signed_angle_between_vectors2d(
				&start_to_counterclockwise,
				&counterclockwise_to_unobstructed)))
	{
		*result = *clockwise_turning_point;
		return TRUE;
	}

	*result = *counterclockwise_turning_point;

	return FALSE;
}

static boolean find_turning_point(
	struct structure_bsp const *structure,
	real_point2d const *point,
	real radius,
	long first_edge_index,
	boolean clockwise,
	boolean ignore_broken_surfaces,
	real_point2d *result)
{
	struct collision_bsp const *bsp;
	byte const *pathfinding_surfaces;
	byte const *breakable_surface_flags;
	struct collision_edge const *collision_edge;
	struct collision_vertex const *vertex_a;
	struct collision_vertex const *vertex_b;
	real edge_dx;
	real edge_dy;
	real_vector2d edge_direction;
	real_point2d positive_point;
	real_point2d negative_point;
	real_vector2d vertex_to_positive;
	real_vector2d vertex_to_negative;
	boolean side_flag;
	boolean valid;
	long side_test;
	long xor_flag;
	boolean matches_end;
	long edge_index;
	boolean refined_side;
	long starting_vertex_index;
	long loop_reference_vertex_index;
	long next_vertex_index;
	long chain_start_edge_index;
	long candidate_surface_index;

	bsp = TAG_BLOCK_GET_ELEMENT(
		&structure->collision_bsp,
		0,
		struct collision_bsp);
	pathfinding_surfaces = structure->pathfinding_surfaces.address;
	breakable_surface_flags = breakable_surface_flags_get();
	starting_vertex_index = NONE;
	loop_reference_vertex_index = NONE;
#line 511 "c:\\halo\\SOURCE\\ai\\path_smoothing.c"
	match_assert("c:\\halo\\SOURCE\\ai\\path_smoothing.c", 0x1ff, clockwise==TRUE || clockwise==FALSE);

	edge_index = first_edge_index;
	while (TRUE)
	{
		collision_edge = TAG_BLOCK_GET_ELEMENT(
			&bsp->edges,
			edge_index,
			struct collision_edge);
		side_flag = surface_is_walkable(
			pathfinding_surfaces,
			bsp,
			breakable_surface_flags,
			collision_edge->surface_indices[0],
			ignore_broken_surfaces);

		vertex_a = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			collision_edge->vertex_indices[side_flag],
			struct collision_vertex);
		vertex_b = TAG_BLOCK_GET_ELEMENT(
			&bsp->vertices,
			collision_edge->vertex_indices[!side_flag],
			struct collision_vertex);

		edge_dx = vertex_b->point.x - vertex_a->point.x;
		edge_dy = vertex_b->point.y - vertex_a->point.y;
		set_real_vector2d(&edge_direction, edge_dy, -edge_dx);

		valid = FALSE;
		normalize2d(&edge_direction);

		point_from_line2d(point, &edge_direction, radius, &positive_point);
		point_from_line2d(point, &edge_direction, -radius, &negative_point);

		vertex_to_positive.i = vertex_a->point.x - positive_point.x;
		vertex_to_positive.j = vertex_a->point.y - positive_point.y;
		vertex_to_negative.i = vertex_a->point.x - negative_point.x;
		vertex_to_negative.j = vertex_a->point.y - negative_point.y;

		side_test =
			vertex_to_positive.j*edge_dy +
			vertex_to_positive.i*edge_dx < 0.0f;
		if (side_test == clockwise)
		{
			if (vertex_to_positive.i*edge_dy -
				vertex_to_positive.j*edge_dx < 0.0f)
			{
				valid = TRUE;
			}
		}

		if (vertex_to_negative.i*edge_dy -
			vertex_to_negative.j*edge_dx < 0.0f)
		{
			valid = TRUE;
		}

		if (starting_vertex_index == NONE)
			valid = TRUE;

		xor_flag = valid != side_flag;
		next_vertex_index = collision_edge->vertex_indices[xor_flag == clockwise];

		if (next_vertex_index == loop_reference_vertex_index)
		{
			vertex_a = TAG_BLOCK_GET_ELEMENT(
				&bsp->vertices,
				next_vertex_index,
				struct collision_vertex);
			result->x = vertex_a->point.x;
			result->y = vertex_a->point.y;
			return TRUE;
		}

		if (next_vertex_index == starting_vertex_index)
			return FALSE;

		if (starting_vertex_index == NONE)
			starting_vertex_index = next_vertex_index;

		chain_start_edge_index = edge_index;
		while (TRUE)
		{
			matches_end = next_vertex_index == collision_edge->vertex_indices[1];
			candidate_surface_index = collision_edge->surface_indices[!matches_end];
			refined_side = surface_is_walkable(
				pathfinding_surfaces,
				bsp,
				breakable_surface_flags,
				candidate_surface_index,
				ignore_broken_surfaces);

			if (refined_side == clockwise)
				break;

			edge_index = collision_edge->edge_indices[!matches_end];
			collision_edge = TAG_BLOCK_GET_ELEMENT(
				&bsp->edges,
				edge_index,
				struct collision_edge);
			if (edge_index == chain_start_edge_index)
				return FALSE;

#line 631 "c:\\halo\\SOURCE\\ai\\path_smoothing.c"
			match_assert("c:\\halo\\SOURCE\\ai\\path_smoothing.c", 0x277, collision_edge->vertex_indices[0]==next_vertex_index || collision_edge->vertex_indices[1]==next_vertex_index);
		}

		loop_reference_vertex_index = next_vertex_index;
	}

	return FALSE;
}

#line 27 "c:\\halo\\SOURCE\\ai\\path_smoothing.c"
void path_smooth(
	struct path_state *state,
	short raw_step_count,
	struct path_step const *raw_steps,
	short *smoothed_step_count,
	struct path_step *smoothed_steps,
	boolean *steps_finish_path)
{
	real_point2d current_position;
	long current_surface_index;
	short smoothed_count;
	short step_index;
	short collision_step_index;
	long collision_edge_index;
	boolean collision_active;
	boolean found_clockwise;
	boolean found_counterclockwise;
	boolean chose_clockwise;
	boolean smoothed_path_finishes;
	short i;
	struct path_collision_result collision_result;
	struct path_step *smoothed_step;
	real_point2d clockwise_turning_point;
	real_point2d counterclockwise_turning_point;
	real_point2d chosen_center;
	real_point2d tangent_points[2];
	real_point2d avoidance_point;
	real_point2d known_point;

#line 33 "c:\\halo\\SOURCE\\ai\\path_smoothing.c"
	assert(raw_step_count > 0);
	assert(raw_steps);
	assert(smoothed_step_count);
	assert(smoothed_steps);
	assert(steps_finish_path);

	if (raw_step_count > 1)
	{
		smoothed_count = 0;
		step_index = 1;
		smoothed_path_finishes = FALSE;
		current_position.x = state->input.start_point.x;
		current_position.y = state->input.start_point.y;
		current_surface_index = state->input.start_surface_index;

		while (TRUE)
		{
			collision_step_index = NONE;
			collision_edge_index = NONE;
			collision_active = FALSE;

#ifdef HALO_N64
			/* what the scan below ends with is the first step of the blocked
			run reaching the last step (and its edge), or none if the last is
			clear: found from the end, stopping at the first clear step --
			the same answer for one test where the path is clear, not one for
			every step left (a pill test walks the surfaces between) */
			for (i = raw_step_count - 1; i >= step_index; i--)
			{
				if (!structure_test_pill2d(
					state->structure,
					state->input.ignore_broken_surfaces,
					&current_position,
					current_surface_index,
					(real_point2d const *)&raw_steps[i].point,
					raw_steps[i].surface_index,
					0.3f,
					FLAG(_path_test_pill_endpoint_near_wall_ok_bit),
					&collision_result))
				{
					break;
				}
				collision_edge_index = collision_result.edge_index;
				collision_step_index = i;
				collision_active = TRUE;
			}
#else
			for (i = step_index; i < raw_step_count; i++)
			{
				if (structure_test_pill2d(
					state->structure,
					state->input.ignore_broken_surfaces,
					&current_position,
					current_surface_index,
					(real_point2d const *)&raw_steps[i].point,
					raw_steps[i].surface_index,
					0.3f,
					FLAG(_path_test_pill_endpoint_near_wall_ok_bit),
					&collision_result))
				{
					if (!collision_active)
					{
						collision_edge_index = collision_result.edge_index;
						collision_step_index = i;
						collision_active = TRUE;
					}
				}
				else if (collision_active)
				{
					collision_step_index = NONE;
					collision_edge_index = NONE;
					collision_active = FALSE;
				}
			}
#endif

			if (!collision_active || collision_edge_index == NONE)
				break;

			found_clockwise = find_turning_point(
				state->structure,
				&current_position,
				0.3f,
				collision_edge_index,
				TRUE,
				state->input.ignore_broken_surfaces,
				&clockwise_turning_point);
			found_counterclockwise = find_turning_point(
				state->structure,
				&current_position,
				0.3f,
				collision_edge_index,
				FALSE,
				state->input.ignore_broken_surfaces,
				&counterclockwise_turning_point);
			if (!found_counterclockwise || !found_clockwise)
				goto bail_out;

			chose_clockwise = choose_turning_point(
				&current_position,
				&clockwise_turning_point,
				&counterclockwise_turning_point,
				(real_point2d const *)&raw_steps[collision_step_index - 1].point,
				(real_point2d const *)&raw_steps[collision_step_index].point,
				&chosen_center);

			find_tangent_point(
				&current_position,
				&chosen_center,
				0.35f,
				chose_clockwise,
				&tangent_points[0]);
			find_tangent_point(
				(real_point2d const *)&raw_steps[collision_step_index].point,
				&chosen_center,
				0.35f,
				!chose_clockwise,
				&tangent_points[1]);
			find_avoidance_point(
				&tangent_points[0],
				&chosen_center,
				&current_position,
				0.35f,
				&avoidance_point);

			known_point = current_position;
			current_position = avoidance_point;
			current_surface_index = structure_surface_index_from_point(
				state->structure,
				state->input.ignore_broken_surfaces,
				&known_point,
				current_surface_index,
				&current_position);

			smoothed_step = &smoothed_steps[smoothed_count];
			smoothed_count++;
			collision_surface_project_point2d(
				TAG_BLOCK_GET_ELEMENT(
					&state->structure->collision_bsp,
					0,
					struct collision_bsp),
				current_surface_index,
				_z,
				TRUE,
				&current_position,
				&smoothed_step->point);
			smoothed_step->surface_index = current_surface_index;

			if (smoothed_count >= 4)
				goto bail_out;

			step_index = collision_step_index;
		}

		smoothed_steps[smoothed_count] = raw_steps[raw_step_count - 1];
		smoothed_count++;
		smoothed_path_finishes = TRUE;

	bail_out:
		*smoothed_step_count = smoothed_count;
		if (!smoothed_path_finishes)
			*steps_finish_path = FALSE;

	}
	else
	{
		*smoothed_step_count = 1;
		*smoothed_steps = *raw_steps;
	}

	return;
}
