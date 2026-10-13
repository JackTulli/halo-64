/*
VEHICLE_DATUM.H

Private runtime vehicle datum ownership shared by vehicles.c and physics.c.
*/

#ifndef __VEHICLE_DATUM_H
#define __VEHICLE_DATUM_H
#pragma once

/* ---------- headers */

#include "objects/objects.h"
#include "units/units.h"

/* ---------- macros */

#define vehicle_datum_get(index) ((struct vehicle_datum *)object_get_and_verify_type((index), _object_mask_vehicle))
#define vehicle_datum_try_and_get(index) ((struct vehicle_datum *)object_try_and_get_and_verify_type((index), _object_mask_vehicle))

/* ---------- structures */

struct _vehicle_datum
{
	word flags;
	short stop_time;
	byte airborne_ticks;
	byte upending_type;
	byte upending_ticks;
	byte on_ground_ticks;
	real speed;
	real slide;
	real turn;
	real wheel;
	real left_tread;
	real right_tread;
	real hover;
	real thrust;
	byte suspension[8];
	real_point3d hover_position;
	union
	{
		struct
		{
			real_vector3d collision_force;
			real_vector3d collision_torque;
		};
		struct
		{
			real_quaternion collision_accumulator_head;
			real_point2d collision_accumulator_tail;
		};
	};
	unsigned long stuck_mass_point_flags;
};

struct vehicle_datum
{
	long definition_index;
	struct _object_datum object;
	struct _unit_datum unit;
	struct _vehicle_datum vehicle;
};

typedef char vehicle_datum_vehicle_offset_assert[
	offsetof(struct vehicle_datum, vehicle) == 0x424 ? 1 : -1];
typedef char vehicle_datum_size_assert[
	sizeof(struct vehicle_datum) == 0x47C ? 1 : -1];

#endif // __VEHICLE_DATUM_H
