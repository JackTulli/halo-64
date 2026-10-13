/*
PATH_STRUCTURE_BSP.C

symbols in this file:
00051D80 01e0:
	_structure_test_ray2d (0000)
00051F60 0080:
	_structure_surfaces_are_equivalent (0000)
00051FE0 0070:
	_clip_empty_interval_by_solid_interval (0000)
00052050 0450:
	_structure_test_line2d (0000)
000524A0 0060:
	_structure_surface_index_from_point (0000)
00052500 0270:
	_structure_test_pill2d (0000)
0024EC0C 0020:
	_quantized_pathfinding_surface_widths (0000)
0024EC2C 0020:
	_quantized_pathfinding_surface_heights (0000)
0024EC4C 0027:
	??_C@_0CH@JLKIDLBA@c?3?2halo?2SOURCE?2ai?2path_structure@ (0000)
0024EC78 0008:
	__real@3fa99999a0000000 (0000)
0024EC80 0004:
	__real@3c000000 (0000)
0024EC88 0048:
	??_C@_0EI@IFBLLPMB@surface_index?$DO?$DN0?5?$CG?$CG?5surface_inde@ (0000)
0024ECD0 0007:
	??_C@_06KJFADLGN@result?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "path_structure_bsp.h"

#include "math/real_math.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_bsp.h"
#include "physics/collision_bsp_definitions.h"
#include "structures/structure_bsp_definitions.h"

/* ---------- constants */

enum
{
	_pathfinding_surface_walkable_bit = 6,
	_pathfinding_surface_breakable_bit = 7,
};

enum
{
	_collision_surface_breakable_bit = 3,
};

enum
{
	_path_test_pill_endpoint_near_wall_ok_bit = 0,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

static real const quantized_pathfinding_surface_widths[8] =
{
	0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.5f, 2.0f, 4.0f
};

static real const quantized_pathfinding_surface_heights[8] =
{
	0.25f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 4.0f, 8.0f
};

/* ---------- public code */

boolean structure_test_ray2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *point,
	long surface_index,
	real_vector2d const *direction,
	real distance,
	struct structure_test_ray2d_result *result)
{
	struct collision_surface_test_line2d_result surface_result;
	struct collision_bsp const *bsp;
	byte *breakable_surface_flags;
	byte const *pathfinding_surfaces;

	bsp = TAG_BLOCK_GET_ELEMENT(&structure->collision_bsp, 0, struct collision_bsp);
	breakable_surface_flags = breakable_surface_flags_get();
	pathfinding_surfaces = structure->pathfinding_surfaces.address;

	collision_surface_test_line2d(
		bsp,
		surface_index,
		_z,
		TRUE,
		point,
		direction,
		&surface_result);

	while (TRUE)
	{
		long next_surface_index = NONE;

		if (distance < surface_result.enter_t &&
			pathfinding_surfaces[surface_result.enter_surface_index])
		{
			boolean surface_passable = TRUE;

			if (!ignore_broken_surfaces &&
				TEST_FLAG(
					pathfinding_surfaces[surface_result.enter_surface_index],
					_pathfinding_surface_breakable_bit))
			{
				struct collision_surface const *collision_surface;

				collision_surface = TAG_BLOCK_GET_ELEMENT(
					&bsp->surfaces,
					surface_result.enter_surface_index,
					struct collision_surface);
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path_structure_bsp.c",
					105,
					TEST_FLAG(collision_surface->flags, _collision_surface_breakable_bit));
				surface_passable = BIT_VECTOR_TEST_FLAG(
					(long *)breakable_surface_flags,
					collision_surface->breakable_surface_index);
			}

			if (surface_passable)
			{
				next_surface_index = surface_result.enter_surface_index;
			}
		}

		if (next_surface_index == NONE &&
			distance > surface_result.exit_t &&
			pathfinding_surfaces[surface_result.exit_surface_index])
		{
			boolean surface_passable = TRUE;

			if (!ignore_broken_surfaces &&
				TEST_FLAG(
					pathfinding_surfaces[surface_result.exit_surface_index],
					_pathfinding_surface_breakable_bit))
			{
				struct collision_surface const *collision_surface;

				collision_surface = TAG_BLOCK_GET_ELEMENT(
					&bsp->surfaces,
					surface_result.exit_surface_index,
					struct collision_surface);
				match_assert(
					"c:\\halo\\SOURCE\\ai\\path_structure_bsp.c",
					126,
					TEST_FLAG(collision_surface->flags, _collision_surface_breakable_bit));
				surface_passable = BIT_VECTOR_TEST_FLAG(
					(long *)breakable_surface_flags,
					collision_surface->breakable_surface_index);
			}

			if (surface_passable)
			{
				next_surface_index = surface_result.exit_surface_index;
			}
		}

		if (next_surface_index == NONE)
		{
			break;
		}

		surface_index = next_surface_index;
		collision_surface_test_line2d(
			bsp,
			surface_index,
			_z,
			TRUE,
			point,
			direction,
			&surface_result);
	}

	if (distance < surface_result.enter_t)
	{
		result->distance = surface_result.enter_t;
		result->surface_index = surface_index;
		result->edge_index = surface_result.enter_edge_index;
		return TRUE;
	}

	if (distance > surface_result.exit_t)
	{
		result->distance = surface_result.exit_t;
		result->surface_index = surface_index;
		result->edge_index = surface_result.exit_edge_index;
		return TRUE;
	}

	result->distance = distance;
	result->surface_index = surface_index;
	result->edge_index = NONE;

	return FALSE;
}

boolean structure_surfaces_are_equivalent(
	struct structure_bsp const *structure,
	real_point2d const *destination_point,
	long destination_surface_index,
	long test_surface_index)
{
	struct collision_bsp const *bsp;
	real_point3d destination_point3d;
	real_point3d test_point3d;
	boolean result;

	bsp = TAG_BLOCK_GET_ELEMENT(&structure->collision_bsp, 0, struct collision_bsp);
	result = FALSE;

	if (destination_surface_index != NONE && test_surface_index != NONE)
	{
		collision_surface_project_point2d(
			bsp,
			destination_surface_index,
			_z,
			TRUE,
			destination_point,
			&destination_point3d);
		collision_surface_project_point2d(
			bsp,
			test_surface_index,
			_z,
			TRUE,
			destination_point,
			&test_point3d);
		result = fabs(destination_point3d.z - test_point3d.z) < 0.05f;
	}

	return result;
}

boolean structure_test_line2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *p0,
	long p0_surface_index,
	real_point2d const *p1,
	long p1_surface_index,
	struct path_collision_result *result)
{
	struct collision_bsp const *bsp = TAG_BLOCK_GET_ELEMENT(&structure->collision_bsp, 0, struct collision_bsp);
	byte const *pathfinding_surfaces = structure->pathfinding_surfaces.address;
	long const *breakable_surface_flags = (long const *)breakable_surface_flags_get();
	long surface_index = p0_surface_index;
	boolean recursed = FALSE;
	real_vector2d p0p1;

	match_assert("c:\\halo\\SOURCE\\ai\\path_structure_bsp.c", 217, result);

	vector_from_points2d(p0, p1, &p0p1);

	while (TRUE)
	{
		struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(&bsp->surfaces, surface_index, struct collision_surface);
		long edge_index = surface->first_edge_index;
		real_point3d point_in_surface = *global_origin3d;
		short edge_count = 0;
		boolean crossed_any = FALSE;
		boolean reached_target = FALSE;

		while (TRUE)
		{
			struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(&bsp->edges, edge_index, struct collision_edge);
			boolean on_right_side = surface_index == edge->surface_indices[1];
			struct collision_vertex const *vertex0 = TAG_BLOCK_GET_ELEMENT(
				&bsp->vertices,
				edge->vertex_indices[!on_right_side],
				struct collision_vertex);
			struct collision_vertex const *vertex1 = TAG_BLOCK_GET_ELEMENT(
				&bsp->vertices,
				edge->vertex_indices[on_right_side],
				struct collision_vertex);
			real_vector2d e0e1;
			real_vector2d e0p1;
			real_vector2d p0e0;
			real_vector2d p0e1;

			vector_from_points2d((real_point2d const *)&vertex0->point, (real_point2d const *)&vertex1->point, &e0e1);
			vector_from_points2d((real_point2d const *)&vertex0->point, p1, &e0p1);
			vector_from_points2d(p0, (real_point2d const *)&vertex0->point, &p0e0);
			vector_from_points2d(p0, (real_point2d const *)&vertex1->point, &p0e1);

			if (edge->surface_indices[!on_right_side] == p1_surface_index)
			{
				reached_target = TRUE;
			}

			point_in_surface.x += vertex0->point.x;
			point_in_surface.y += vertex0->point.y;
			point_in_surface.z += vertex0->point.z;
			edge_count++;

			if (cross_product2d(&e0e1, &e0p1) > 0.0f)
			{
				crossed_any = TRUE;
				if (cross_product2d(&p0p1, &p0e0) > 0.0f &&
					cross_product2d(&p0e1, &p0p1) > 0.0f)
				{
					long next_surface_index = edge->surface_indices[!on_right_side];
					boolean passable = TEST_FLAG(
						pathfinding_surfaces[next_surface_index],
						_pathfinding_surface_walkable_bit);

					if (!ignore_broken_surfaces &&
						passable &&
						TEST_FLAG(
							pathfinding_surfaces[next_surface_index],
							_pathfinding_surface_breakable_bit))
					{
						struct collision_surface const *collision_surface = TAG_BLOCK_GET_ELEMENT(
							&bsp->surfaces,
							next_surface_index,
							struct collision_surface);

						match_assert(
							"c:\\halo\\SOURCE\\ai\\path_structure_bsp.c",
							274,
							TEST_FLAG(collision_surface->flags, _collision_surface_breakable_bit));
						passable = BIT_VECTOR_TEST_FLAG(
							breakable_surface_flags,
							collision_surface->breakable_surface_index);
					}

					if (passable)
					{
						surface_index = next_surface_index;
						break;
					}
					else
					{
						real t = (cross_product2d(&e0e1, &p0e0) - magnitude2d(&e0e1) * (1.0f / 128.0f)) /
							cross_product2d(&e0e1, &p0p1);
						real_point2d p2d;

						point_from_line2d(p0, &p0p1, t, &p2d);
						collision_surface_project_point2d(
							bsp,
							surface_index,
							_z,
							TRUE,
							&p2d,
							&result->point);
						result->surface_index = surface_index;
						result->edge_index = edge_index;
						result->collision = TRUE;
						result->t = t;
						return TRUE;
					}
				}
			}

			edge_index = edge->edge_indices[on_right_side];
			if (edge_index == surface->first_edge_index)
			{
				if (crossed_any)
				{
					struct path_collision_result p0_result;

					match_assert(
						"c:\\halo\\SOURCE\\ai\\path_structure_bsp.c",
						316,
						surface_index>=0 && surface_index<structure->pathfinding_surfaces.count);
					point_in_surface.x /= edge_count;
					point_in_surface.y /= edge_count;

					if (!recursed &&
						pathfinding_surfaces[surface_index] &&
						!structure_test_line2d(
							structure,
							ignore_broken_surfaces,
							(real_point2d const *)&point_in_surface,
							surface_index,
							p0,
							NONE,
							&p0_result))
					{
						recursed = TRUE;
						surface_index = p0_result.surface_index;
						break;
					}
					else
					{
						collision_surface_project_point2d(
							bsp,
							p0_surface_index,
							_z,
							TRUE,
							p0,
							&result->point);
						result->surface_index = NONE;
						result->edge_index = NONE;
						result->collision = TRUE;
						result->t = 0.0f;
						return TRUE;
					}
				}
				else
				{
					if (surface_index == p1_surface_index || reached_target || p1_surface_index == NONE)
					{
						collision_surface_project_point2d(
							bsp,
							surface_index,
							_z,
							TRUE,
							p1,
							&result->point);
						result->surface_index = surface_index;
						result->edge_index = NONE;
						result->collision = FALSE;
						result->t = 1.0f;
						return FALSE;
					}
					else
					{
						collision_surface_project_point2d(
							bsp,
							p0_surface_index,
							_z,
							TRUE,
							p0,
							&result->point);
						result->surface_index = NONE;
						result->edge_index = NONE;
						result->collision = TRUE;
						result->t = 0.0f;
						return TRUE;
					}
				}
			}
		}
	}
}

boolean clip_empty_interval_by_solid_interval(
	real *empty_t0,
	real *empty_t1,
	real solid_t0,
	real solid_t1)
{
	real clipped_solid_t0;
	real clipped_solid_t1;

	if (*empty_t0 > solid_t1)
		clipped_solid_t1 = *empty_t0;
	else
		clipped_solid_t1 = solid_t1;
	solid_t1 = clipped_solid_t1;

	clipped_solid_t0 = MIN(solid_t0, *empty_t1);

	if (*empty_t1 - solid_t1 > clipped_solid_t0 - *empty_t0)
		*empty_t0 = solid_t1;
	else
		*empty_t1 = clipped_solid_t0;

	return *empty_t0 > *empty_t1;
}

long structure_surface_index_from_point(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *known_point,
	long known_surface_index,
	real_point2d *point)
{
	struct path_collision_result result;

	if (known_surface_index != NONE)
	{
		structure_test_line2d(
			structure,
			ignore_broken_surfaces,
			known_point,
			known_surface_index,
			point,
			NONE,
			&result);

		point->x = result.point.x;
		point->y = result.point.y;

		if (result.surface_index == NONE)
			return known_surface_index;

		return result.surface_index;
	}

	return NONE;
}

boolean structure_test_pill2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *p0,
	long p0_surface_index,
	real_point2d const *p1,
	long p1_surface_index,
	real radius,
	unsigned long flags,
	struct path_collision_result *result)
{
	boolean collision = FALSE;
	real_vector2d direction;
	real_vector2d perpendicular;

	vector_from_points2d(p0, p1, &direction);
	set_real_vector2d(&perpendicular, -direction.j, direction.i);
	if (normalize2d(&perpendicular) > 0.0f)
	{
		real_point2d left_p0;
		real_point2d left_p1;
		real_point2d right_p0;
		real_point2d right_p1;
		long left_p0_surface_index;
		long left_p1_surface_index;
		long right_p0_surface_index;
		long right_p1_surface_index;
		struct path_collision_result left_result;
		struct path_collision_result right_result;
		struct path_collision_result endpoint_result;
		struct path_collision_result *best_result;

		left_p0_surface_index = structure_surface_index_from_point(
			structure,
			ignore_broken_surfaces,
			p0,
			p0_surface_index,
			point_from_line2d(p0, &perpendicular, radius, &left_p0));
		left_p1_surface_index = structure_surface_index_from_point(
			structure,
			ignore_broken_surfaces,
			p1,
			p1_surface_index,
			point_from_line2d(p1, &perpendicular, radius, &left_p1));
		right_p0_surface_index = structure_surface_index_from_point(
			structure,
			ignore_broken_surfaces,
			p0,
			p0_surface_index,
			point_from_line2d(p0, &perpendicular, -radius, &right_p0));
		right_p1_surface_index = structure_surface_index_from_point(
			structure,
			ignore_broken_surfaces,
			p1,
			p1_surface_index,
			point_from_line2d(p1, &perpendicular, -radius, &right_p1));

		if (left_p0_surface_index == NONE)
		{
			left_result.collision = FALSE;
		}
		else if (structure_test_line2d(
				structure,
				ignore_broken_surfaces,
				&left_p0,
				left_p0_surface_index,
				&left_p1,
				left_p1_surface_index,
				&left_result) &&
			left_result.surface_index != NONE &&
			!TEST_FLAG(flags, _path_test_pill_endpoint_near_wall_ok_bit) &&
			!structure_test_line2d(
				structure,
				ignore_broken_surfaces,
				(real_point2d const *)&left_result.point,
				left_result.surface_index,
				p1,
				p1_surface_index,
				&endpoint_result))
		{
			left_result.collision = FALSE;
		}

		if (right_p0_surface_index == NONE)
		{
			right_result.collision = FALSE;
		}
		else if (structure_test_line2d(
				structure,
				ignore_broken_surfaces,
				&right_p0,
				right_p0_surface_index,
				&right_p1,
				right_p1_surface_index,
				&right_result) &&
			right_result.surface_index != NONE &&
			!TEST_FLAG(flags, _path_test_pill_endpoint_near_wall_ok_bit) &&
			!structure_test_line2d(
				structure,
				ignore_broken_surfaces,
				(real_point2d const *)&right_result.point,
				right_result.surface_index,
				p1,
				p1_surface_index,
				&endpoint_result))
		{
			right_result.collision = FALSE;
		}

		if (left_result.collision && right_result.collision)
		{
			if (left_result.t < right_result.t)
			{
				best_result = &left_result;
			}
			else
			{
				best_result = &right_result;
			}
		}
		else if (left_result.collision)
		{
			best_result = &left_result;
		}
		else if (right_result.collision)
		{
			best_result = &right_result;
		}
		else
		{
			best_result = NULL;
		}

		/* BUG (preserved for exact matching): a side whose p0 surface index is NONE gets only
		 * its collision flag, so when p0_surface_index is NONE the copy below returns
		 * left_result's point, surface_index, edge_index and t unassigned (January 0x452500
		 * writes only the byte [ebp-0x4c] at +0x189 and copies seven dwords from [ebp-0x4c]
		 * at +0x24f..+0x25a), and the function returns FALSE. actor_move_try_evasion_vector
		 * passes the actor's pathfinding surface as p0 and, on a FALSE return, reads
		 * result->point.z to accept or reject the evasion point; actor_find_pathfinding_location
		 * leaves that surface NONE for a non-flying actor when no ground surface is found or
		 * when the actor's vehicle is not a ground vehicle. A runtime occurrence was not
		 * traced. A corrected build should fill both side results before selecting.
		 * Source-policy approval pending (2026-09-27 audit). */
		if (!best_result ||
			distance_squared2d((real_point2d const *)&best_result->point, p1) < radius * radius)
		{
			*result = left_result;
		}
		else
		{
			*result = *best_result;
			collision = TRUE;
		}
	}

	return collision;
}

/* ---------- private code */
