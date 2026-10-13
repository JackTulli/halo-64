/*
DEVICE_CONTROLS.H
*/

#ifndef __DEVICE_CONTROLS_H
#define __DEVICE_CONTROLS_H
#pragma once

/* ---------- headers */

#include "device_definitions.h"
#include "devices.h"

/* ---------- constants */

enum
{
	CONTROL_DEFINITION_TAG = 'ctrl',
};

enum control_datum_flags
{
	_control_usable_from_both_sides_bit = 0,
	NUMBER_OF_CONTROL_DATUM_FLAGS,
};

/* ---------- macros */

#define control_definition_get(index) ((struct control_definition *)tag_get(CONTROL_DEFINITION_TAG, (index)))
#define control_get(index) ((struct control_datum *)object_get_and_verify_type((index), _object_mask_control))
#define control_try_and_get(index) ((struct control_datum *)object_try_and_get_and_verify_type((index), _object_mask_control))

/* ---------- structures */

struct scenario_control_datum;

struct _control_definition
{
	short type;
	short triggers_when;
	real call_value;
	byte reserved8[0x50];
	struct tag_reference on_effect;
	struct tag_reference off_effect;
	struct tag_reference denied_effect;
	byte reserved88[0xC];
};

struct control_definition
{
	struct _object_definition object;
	struct _device_definition device;
	struct _control_definition control;
};

typedef char verify_control_definition_control_offset[
	offsetof(struct control_definition, control) == 0x290 ? 1 : -1];
typedef char verify_control_definition_on_effect_index_offset[
	offsetof(struct control_definition, control.on_effect.index) == 0x2F4 ? 1 : -1];
typedef char verify_control_definition_off_effect_index_offset[
	offsetof(struct control_definition, control.off_effect.index) == 0x304 ? 1 : -1];
typedef char verify_control_definition_denied_effect_index_offset[
	offsetof(struct control_definition, control.denied_effect.index) == 0x314 ? 1 : -1];

struct _control_datum
{
	unsigned long flags;
	short custom_name_index;
};

struct control_datum
{
	long definition_index;
	struct _object_datum object;
	struct _device_datum device;
	struct _control_datum control;
};

/* ---------- prototypes/DEVICE_CONTROLS.C */

void controls_initialize(
	void);
void controls_dispose(
	void);
void controls_initialize_for_new_map(
	void);
void controls_dispose_from_old_map(
	void);
boolean control_new(
	long control_index);
void control_delete(
	long control_index);
boolean control_update(
	long control_index);
void control_place(
	long control_index,
	struct scenario_control_datum *scenario_control);
void control_touched(
	long control_index,
	long unit_index);
void control_destroyed(
	long control_index);

#endif // __DEVICE_CONTROLS_H
