/*
EQUIPMENT.H

header included in hcex build.
*/

#ifndef __EQUIPMENT_H
#define __EQUIPMENT_H
#pragma once

#include "math/real_math.h"
#include "items/items.h"
#include "objects/object_types.h"
#include "scenario/scenario_definitions.h"

/* ---------- constants */

enum
{
	_scenario_equipment_levitate_bit = 0,
	_scenario_equipment_does_not_orient_to_ground_bit = 2,
};

enum
{
	_equipment_orient_to_ground_bit = 5,
};

/* ---------- macros */

#define equipment_get(index) ((struct item_datum *)object_get_and_verify_type((index), _object_mask_equipment))
#define equipment_try_and_get(index) ((struct item_datum *)object_try_and_get_and_verify_type((index), _object_mask_equipment))

/* ---------- structures */

struct scenario_equipment_datum
{
	struct scenario_object_datum object;
};

/* ---------- prototypes/EQUIPMENT.C */

void equipment_place(long equipment_index, struct scenario_equipment_datum *scenario_equipment);
void equipment_definition_handle_pickup(long equipment_definition_index);
void equipment_handle_pickup(long equipment_index);

/* ---------- globals */

/* ---------- public code */

#endif // __EQUIPMENT_H
