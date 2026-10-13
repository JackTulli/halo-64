/*
MASS_POINT_DATUM.H
*/

#ifndef __MASS_POINT_DATUM_H
#define __MASS_POINT_DATUM_H
#pragma once

/* ---------- headers */

#include "friction_datum.h"
#include "objects/objects.h"

/* ---------- structures */

struct mass_point_datum
{
	unsigned long flags;
	real_point3d position;
	real_vector3d forward;
	real_vector3d left;
	real_vector3d up;
	struct location location;
	real_vector3d radius;
	real_vector3d velocity;
	real_vector3d velocity_relative_to_ground;
	real_plane3d ground_plane;
	short ground_material_type;
	short pad0;
	real ground_depth;
	short water_material_type;
	short pad1;
	real water_depth;
	real normal_force_magnitude;
	real_vector3d normal_force;
	struct friction_datum ground_friction;
	real water_pressure_magnitude;
	real_vector3d water_pressure;
	struct friction_datum water_friction;
	struct friction_datum air_friction;
	real_vector3d powered_force;
	real_vector3d force;
	real_vector3d torque;
};

#endif // __MASS_POINT_DATUM_H
