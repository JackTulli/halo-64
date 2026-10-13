/*
PHYSICS_VARIABLES.H

header included in hcex build.
*/

#ifndef __PHYSICS_VARIABLES_H
#define __PHYSICS_VARIABLES_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct physics_variable_speed_parameters
{
	real positive_scale;
	real negative_scale;
	real acceleration;
	real deceleration;
};

/* ---------- prototypes/PHYSICS_VARIABLES.C */

void physics_variable_speed_update(
	real *speed,
	struct physics_variable_speed_parameters *parameters,
	real delta);

boolean physics_variable_speed_update_seek(
	real *speed,
	struct physics_variable_speed_parameters *parameters,
	real target,
	real delta);

boolean physics_variable_position_update_seek(
	real *position,
	real *limits,
	boolean wrap,
	real target,
	real delta);

void physics_variable_position_update(
	real *position,
	real *limits,
	boolean wrap,
	real delta);

void physics_variable_update(
	real *position,
	real *range,
	real *velocity,
	boolean update_velocity,
	real delta);

boolean physics_variable_update_seek(
	real *position,
	real *range,
	real *velocity,
	boolean update_velocity,
	real target,
	real delta);

/* ---------- globals */

/* ---------- public code */

#endif // __PHYSICS_VARIABLES_H
