/*
CONNECTED_GEOMETRY.H

header included in hcex build.
*/

#ifndef __CONNECTED_GEOMETRY_H
#define __CONNECTED_GEOMETRY_H
#pragma once

/* ---------- headers */

#include "memory/array.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct connected_geometry_edge
{
	struct dynamic_array triangle_indices;
	long point_indices[2];
	long unused[2];
};

struct connected_geometry_triangle
{
	long edge_designators[3];
	long coplanar_group_index;
	long unused[2];
};

struct connected_geometry
{
	struct dynamic_array points;
	struct dynamic_array edges;
	struct dynamic_array triangles;
};

/* ---------- prototypes/CONNECTED_GEOMETRY.C */

void connected_geometry_new(
	struct connected_geometry *geometry);
void connected_geometry_delete(
	struct connected_geometry *geometry);
long connected_geometry_add_triangle(
	struct connected_geometry *geometry,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2,
	boolean report_duplicates);
long connected_geometry_group_coplanar(
	struct connected_geometry *geometry);

/* ---------- globals */

/* ---------- public code */

#endif // __CONNECTED_GEOMETRY_H
