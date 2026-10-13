/*
DEVICES.H

header included in hcex build.
*/

#ifndef __DEVICES_H
#define __DEVICES_H
#pragma once

/* ---------- headers */

#include "objects/objects.h"

/* ---------- constants */

enum
{
	_scenario_device_initially_open_bit = 0,
	_scenario_device_initially_off_bit,
	_scenario_device_changes_only_once_bit,
	_scenario_device_position_reversed_bit,
	_scenario_device_not_usable_bit,
	SCENARIO_DEVICE_DATUM_FLAGS,
};

enum
{
	_machine_does_not_operate_automatically_bit = 0,
	_machine_one_sided_bit,
	_machine_never_appears_locked_bit,
	_machine_opened_by_melee_attack_bit,
	NUMBER_OF_MACHINE_DATUM_FLAGS,
};

/* ---------- macros */

#define device_get(index) ((struct device_datum *)object_get_and_verify_type((index), _object_mask_device))

/* ---------- structures */

struct _device_datum
{
	unsigned long flags;
	short power_group_index;
	real power;
	real power_velocity;
	short position_group_index;
	real position;
	real position_velocity;
	short delay_ticks;
};

struct scenario_device_datum
{
	short power_group_index;
	short position_group_index;
	unsigned long flags;
};

struct device_datum
{
	long definition_index;
	struct _object_datum object;
	struct _device_datum device;
};

struct _machine_datum
{
	unsigned long flags;
	long door_open_ticks;
	real_point3d elevator_position;
};

struct machine_datum
{
	long definition_index;
	struct _object_datum object;
	struct _device_datum device;
	struct _machine_datum machine;
};

/* ---------- prototypes/DEVICES.C */

void devices_dispose(
	void);
void device_add_scenario_information(
	long object_index,
	struct scenario_device_datum *scenario_device);
void device_export_function_values(
	long device_index);
void device_render_debug(
	long device_index);
real device_get_position(
	long device_index);
real device_get_power(
	long device_index);
void device_set_never_appears_locked(
	long device_index,
	boolean never_locked);
void device_one_sided_set(
	long device_index,
	boolean one_sided);
void device_operates_automatically_set(
	long device_index,
	boolean automatic);
void device_set_actual_position(
	long device_index,
	real position);
boolean device_group_set_desired_value(
	short group_index,
	real desired_value);
boolean device_can_change_position(
	long device_index);
boolean device_frontfacing(
	long device_index,
	real_point3d const *position,
	real_vector3d const *facing);
void device_group_change_only_once_more_set(
	long device_group_index,
	boolean change_only_once_more);
real device_group_get_value(
	short device_group_index);
void device_touched(
	long device_index,
	long unit_index);
void device_effect_new(
	long device_index,
	long effect_index);
void device_preprocess_node_orientations(
	long device_index,
	struct real_orientation *node_orientations);
boolean device_update(
	long device_index);

extern struct data_array *device_groups_data;

/* ---------- globals */

extern boolean debug_objects_devices;

/* ---------- public code */

#endif // __DEVICES_H
