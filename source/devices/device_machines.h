/*
DEVICE_MACHINES.H
*/

#ifndef __DEVICE_MACHINES_H
#define __DEVICE_MACHINES_H
#pragma once

/* ---------- headers */

#include "device_definitions.h"
#include "devices.h"
#include "scenario/scenario_definitions.h"

/* ---------- macros */

#define machine_get(index) ((struct machine_datum *)object_get_and_verify_type((index), _object_mask_machine))
#define machine_try_and_get(index) ((struct machine_datum *)object_try_and_get_and_verify_type((index), _object_mask_machine))

/* ---------- structures */

struct scenario_machine_datum
{
	struct scenario_object_datum object;
	struct scenario_device_datum device;
	byte flags;
	byte pad[3];
};

/* ---------- prototypes/DEVICE_MACHINES.C */

void machines_initialize(
	void);
void machines_dispose(
	void);
void machines_initialize_for_new_map(
	void);
void machines_dispose_from_old_map(
	void);
void machine_place(
	long machine_index,
	struct scenario_machine_datum *scenario_machine);
boolean machine_new(
	long machine_index);
void machine_delete(
	long machine_index);
void machine_bumped(
	long machine_index,
	long biped_index);
void machine_try_to_open_with_damage(
	long machine_index);
boolean machine_update(
	long machine_index);

#endif // __DEVICE_MACHINES_H
