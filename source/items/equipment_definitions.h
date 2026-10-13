/*
EQUIPMENT_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __EQUIPMENT_DEFINITIONS_H
#define __EQUIPMENT_DEFINITIONS_H
#pragma once

#include "items/item_definitions.h"

/* ---------- constants */

enum
{
	EQUIPMENT_DEFINITION_TAG = 'eqip',
};

enum equipment_powerup_type
{
	_equipment_powerup_none = 0,
	_equipment_powerup_double_speed,
	_equipment_powerup_overshield,
	_equipment_powerup_active_camouflage,
	_equipment_powerup_full_spectrum_vision,
	_equipment_powerup_health,
	_equipment_powerup_grenade,
	NUMBER_OF_EQUIPMENT_POWERUP_TYPES,
};

/* ---------- macros */

#define equipment_definition_get(index) ((struct equipment_definition *)tag_get(EQUIPMENT_DEFINITION_TAG, (index)))

/* ---------- structures */

struct _equipment_definition
{
	short powerup_type;
	short grenade_type;
	real powerup_time;
	struct tag_reference pickup_sound;
};

struct equipment_definition
{
	struct _object_definition object;
	struct _item_definition item;
	struct _equipment_definition equipment;
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __EQUIPMENT_DEFINITIONS_H
