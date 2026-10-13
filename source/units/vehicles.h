/*
VEHICLES.H

header included in hcex build.
*/

#ifndef __VEHICLES_H
#define __VEHICLES_H
#pragma once

/* ---------- headers */

#include "units.h"

/* ---------- constants */

enum vehicle_flags
{
	_vehicle_blurred_bit = 0,
	_vehicle_hovering_bit,
	_vehicle_control_crouch_bit,
	_vehicle_control_jump_bit,
	_vehicle_upending_bit,
	NUMBER_OF_VEHICLE_FLAGS,
};

/* ---------- macros */

#define vehicle_get(index) ((struct unit_datum *)object_get_and_verify_type((index), _object_mask_vehicle))
#define vehicle_try_and_get(index) ((struct unit_datum *)object_try_and_get_and_verify_type((index), _object_mask_vehicle))
#define vehicle_runtime_get(index) ((struct vehicle_runtime_datum *)object_get_and_verify_type((index), _object_mask_vehicle))

/* ---------- structures */

struct vehicle_runtime_datum;

/* ---------- prototypes/VEHICLES.C */

void vehicle_hover(
	long vehicle_index,
	boolean hover);

void vehicles_initialize(
	void);
void vehicles_initialize_for_new_map(
	void);
void vehicles_dispose_from_old_map(
	void);
void vehicles_dispose(
	void);
boolean vehicle_moving_near_any_player(
	void);
boolean vehicle_stuck(
	long vehicle_index,
	real_vector3d *direction);
void vehicle_delete(
	long vehicle_index);
long vehicle_find_pathfinding_surface_index(
	long vehicle_index,
	real_point3d *position);
void vehicle_accelerate(
	long vehicle_index,
	real_vector3d const *acceleration);

/* ---------- globals */

/* ---------- public code */

#endif // __VEHICLES_H
