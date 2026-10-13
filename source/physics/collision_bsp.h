/*
COLLISION_BSP.H

header included in hcex build.
*/

#ifndef __COLLISION_BSP_H
#define __COLLISION_BSP_H
#pragma once

#include "bsp2d.h"
#include "bsp3d.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct collision_bsp_test_vector_result
{
	real t;
	const real_plane3d *plane;
	long surface_index;
	long plane_designator;
	byte flags;
	byte breakable_surface_index;
	short material_index;
	long leaf_count;
	long leaf_indices[256];
};

struct collision_model_test_vector_result
{
	short node_index;
	short region_index;
	short bsp_index;
	struct collision_bsp_test_vector_result bsp_result;
};

struct collision_bsp_test_pill_result
{
	real t;
	real_plane3d plane;
	long surface_index;
	byte flags;
	byte breakable_surface_index;
	short material_index;
	long leaf_count;
	long leaf_indices[256];
};

struct collision_model_test_pill_result
{
	short node_index;
	short region_index;
	short bsp_index;
	struct collision_bsp_test_pill_result bsp_result;
};

struct collision_surface_test_line2d_result
{
	real enter_t;
	long enter_edge_index;
	long enter_surface_index;
	real exit_t;
	long exit_edge_index;
	long exit_surface_index;
};

typedef char collision_surface_test_line2d_result_size_assert[
	sizeof(struct collision_surface_test_line2d_result) == 0x18 ? 1 : -1];
typedef char collision_surface_test_line2d_result_exit_t_offset_assert[
	offsetof(struct collision_surface_test_line2d_result, exit_t) == 0x0C ? 1 : -1];

struct collision_bsp_test_sphere_result
{
	long surface_count;
	long surface_indices[256];
	long edge_count;
	long edge_indices[256];
	long vertex_count;
	long vertex_indices[256];
	long leaf_count;
	long leaf_indices[256];
};

/* ---------- prototypes/COLLISION_BSP.C */

short collision_surface_edge_count(
	struct collision_bsp const *bsp,
	long surface_index);
short collision_surface_polygon(
	struct collision_bsp const *bsp,
	long surface_index,
	real_point3d *points);
real collision_edge_length(
	struct collision_bsp const *bsp,
	long edge_index);
real collision_surface_perimeter(
	struct collision_bsp const *bsp,
	long surface_index);
real collision_surface_area(
	struct collision_bsp const *bsp,
	long surface_index);
real_point3d *collision_surface_project_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_point3d *result);
boolean collision_surface_find_closest_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_point2d *result);
boolean collision_surface_test_point2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point);
boolean collision_surface_test_line2d(
	struct collision_bsp const *bsp,
	long surface_index,
	short projection,
	boolean sign,
	real_point2d const *point,
	real_vector2d const *direction,
	struct collision_surface_test_line2d_result *result);
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
boolean collision_bsp_test_sphere(
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *center,
	real radius,
	struct collision_bsp_test_sphere_result *result);
boolean collision_bsp_test_vector(
	unsigned long flags,
	struct collision_bsp const *bsp,
	short breakable_surface_count,
	byte const *breakable_surface_flags,
	real_point3d const *point,
	real_vector3d const *vector,
	real maximum_t,
	struct collision_bsp_test_vector_result *result);

void render_debug_collision_surface(
	struct collision_bsp *bsp,
	long surface_index,
	real_matrix4x3 const *matrix,
	real_argb_color const *color);

/* ---------- globals */

/* ---------- public code */

#endif // __COLLISION_BSP_H
