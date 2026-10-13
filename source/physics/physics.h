/*
PHYSICS.H

header included in hcex build.
*/

#ifndef __PHYSICS_H
#define __PHYSICS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct collision_feature_list;
struct mass_point_datum;
struct physics_definition;
struct powered_mass_point_datum;

struct physics_instance
{
	long object_index;
	const struct physics_definition *physics;
	real_matrix4x3 world_matrix;
};

struct physics_test_vector_result
{
	real t;
	real_plane3d plane;
};

/* ---------- prototypes/PHYSICS.C */

boolean physics_instance_new(struct physics_instance *instance, long object_index);
real pin_fraction(real value, real begin, real end);
boolean physics_get_features_in_sphere(
	struct physics_instance const *instance,
	real_point3d const *center,
	real radius,
	real height,
	real width,
	struct collision_feature_list *features);
boolean physics_test_point(
	struct physics_instance const *instance,
	real_point3d const *point);
boolean physics_test_vector(
	struct physics_instance const *instance,
	real_point3d const *point,
	real_vector3d const *vector,
	struct physics_test_vector_result *result);
void physics_compute_new(
	struct physics_instance const *instance,
	struct powered_mass_point_datum const *powered_mass_points,
	struct mass_point_datum *mass_points,
	real_vector3d *total_force,
	real_vector3d *total_torque);
void physics_update(
	long object_index,
	struct powered_mass_point_datum *powered_mass_points,
	struct mass_point_datum *mass_points,
	real_vector3d const *magic_force,
	real_vector3d const *magic_torque);

void render_debug_physics(struct physics_instance *instance);


/* ---------- globals */

extern real global_gravity;
extern real global_water_density;
extern real global_air_density;
extern real global_physics_collision_depth;
extern real_plane3d depths_of_hell;

/* ---------- public code */

#endif // __PHYSICS_H
