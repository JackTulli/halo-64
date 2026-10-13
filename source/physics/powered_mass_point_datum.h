/*
POWERED_MASS_POINT_DATUM.H
*/

#ifndef __POWERED_MASS_POINT_DATUM_H
#define __POWERED_MASS_POINT_DATUM_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- structures */

struct powered_mass_point_datum
{
	real ground_friction_velocity;
	real water_friction_velocity;
	real air_friction_velocity;
	real water_lift_ratio;
	real air_lift_ratio;
	real thrust_fraction;
	real antigrav_fraction;
	union
	{
		real_quaternion rotation;
		real rotation_values[4];
		real_vector2d rotation_pairs[2];
	};
	real_matrix4x3 rotation_matrix;
};

#endif // __POWERED_MASS_POINT_DATUM_H
