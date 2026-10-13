/*
PATH_STRUCTURE_BSP.H
*/

#ifndef __PATH_STRUCTURE_BSP_H
#define __PATH_STRUCTURE_BSP_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- structures */

struct structure_bsp;

struct path_collision_result
{
	boolean collision;
	real_point3d point;
	long surface_index;
	long edge_index;
	real t;
};

struct structure_test_ray2d_result
{
	real distance;
	long surface_index;
	long edge_index;
};

typedef char path_collision_result_size_assert[
	sizeof(struct path_collision_result) == 0x1C ? 1 : -1];
typedef char path_collision_result_point_offset_assert[
	offsetof(struct path_collision_result, point) == 0x04 ? 1 : -1];
typedef char path_collision_result_t_offset_assert[
	offsetof(struct path_collision_result, t) == 0x18 ? 1 : -1];

/* ---------- public code */

boolean structure_test_ray2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *point,
	long surface_index,
	real_vector2d const *direction,
	real distance,
	struct structure_test_ray2d_result *result);

boolean structure_surfaces_are_equivalent(
	struct structure_bsp const *structure,
	real_point2d const *destination_point,
	long destination_surface_index,
	long test_surface_index);

boolean structure_test_line2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *p0,
	long p0_surface_index,
	real_point2d const *p1,
	long p1_surface_index,
	struct path_collision_result *result);

boolean structure_test_pill2d(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *p0,
	long p0_surface_index,
	real_point2d const *p1,
	long p1_surface_index,
	real radius,
	unsigned long flags,
	struct path_collision_result *result);

long structure_surface_index_from_point(
	struct structure_bsp const *structure,
	boolean ignore_broken_surfaces,
	real_point2d const *known_point,
	long known_surface_index,
	real_point2d *point);

#endif // __PATH_STRUCTURE_BSP_H
