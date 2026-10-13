/*
STRUCTURE_VISIBILITY.H
*/

#ifndef __STRUCTURE_VISIBILITY_H
#define __STRUCTURE_VISIBILITY_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- structures */

struct render_camera;
struct render_frustum;

struct render_mirror
{
	real_plane3d plane;
	real index_of_refraction;
	real depth;
	short cluster_index;
	word pad;
};

/* ---------- globals */

extern boolean structures_use_pvs_for_vs;
extern boolean debug_portals;

/* ---------- prototypes/STRUCTURE_VISIBILITY.C */

void debug_pvs(
	boolean on);
void structure_visibility_find_camera(
	struct render_camera const *camera);
short structure_visibility_find_objects(
	long *result_indices,
	short maximum_count,
	long (*cluster_get_first)(long *iterator, short cluster_index),
	long (*cluster_get_next)(long *iterator),
	void (*get_bounding_sphere)(long object_index, real_point3d *center, real *radius),
	boolean (*unmarked)(long object_index),
	long (*mark)(long object_index));
boolean structure_visibility_find_mirror(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	struct render_mirror *result);
short structure_visibility_build_surfaces(
	long *surface_indices,
	short maximum_count,
	real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	real_rectangle3d const *bounding_box,
	short bounding_surface_count,
	real_plane3d const *bounding_surfaces,
	short cluster_count,
	short const *cluster_indices);
void structure_visibility_compute(
	void);

#endif /* __STRUCTURE_VISIBILITY_H */
