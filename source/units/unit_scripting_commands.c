/*
UNIT_SCRIPTING_COMMANDS.C

symbols in this file:
00197370 0040:
	_unit_scripting_set_maximum_vitality (0000)
001973B0 0080:
	_units_scripting_set_maximum_vitality (0000)
00197430 0120:
	_unit_scripting_set_current_vitality (0000)
00197550 0050:
	_units_scripting_set_current_vitality (0000)
001975A0 0040:
	_unit_scripting_get_health (0000)
001975E0 0040:
	_unit_scripting_get_shield (0000)
00197620 0040:
	_unit_scripting_get_grenade_count (0000)
00197660 0070:
	_unit_scripting_impervious (0000)
001976D0 0080:
	_unit_scripting_start_user_animation_list (0000)
00197750 0030:
	_unit_scripting_has_weapon (0000)
00197780 0060:
	_unit_scripting_has_weapon_readied (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "hs/object_lists.h"
#include "items/weapons.h"
#include "objects/damage.h"
#include "units.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void unit_scripting_set_current_vitality(
	long unit_index,
	real body_vitality,
	real shield_vitality);

/* ---------- globals */

/* ---------- public code */

void unit_scripting_set_maximum_vitality(
	long unit_index,
	real body_vitality,
	real shield_vitality)
{
	if (unit_index != NONE)
	{
		struct object_datum const *object;
		object = object_get(unit_index);
		if (!TEST_FLAG(object->object.damage_flags, _object_dead_bit))
			object_initialize_vitality(unit_index, &body_vitality, &shield_vitality);
	}
	return;
}

void units_scripting_set_maximum_vitality(
	long object_list_index,
	real body_vitality,
	real shield_vitality)
{
	long reference_index;
	long unit_index;
	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		unit_scripting_set_maximum_vitality(unit_index, body_vitality, shield_vitality);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}
	return;
}

void unit_scripting_set_current_vitality(
	long unit_index,
	real body_vitality,
	real shield_vitality)
{
	if (unit_index != NONE)
	{
		struct object_datum *object = object_get(unit_index);

		if (!TEST_FLAG(object->object.damage_flags, _object_dead_bit))
		{
			real new_shield_vitality;
			real new_body_vitality;

			if (object->object.maximum_shield_vitality <= 0.f)
			{
				new_shield_vitality = 0.f;
			}
			else
			{
				if (shield_vitality >= object->object.maximum_shield_vitality)
					new_shield_vitality = 1.f;
				else
					new_shield_vitality = shield_vitality / object->object.maximum_shield_vitality;
			}

			if (object->object.maximum_body_vitality <= 0.f)
			{
				new_body_vitality = 0.f;
			}
			else
			{
				if (body_vitality >= object->object.maximum_body_vitality)
					new_body_vitality = 1.f;
				else
					new_body_vitality = body_vitality / object->object.maximum_body_vitality;
			}

			if (object->object.shield_vitality > 0.f && new_shield_vitality <= 0.f)
				object_deplete_shield(unit_index);
			object->object.shield_vitality = new_shield_vitality;

			if (object->object.body_vitality > 0.f && new_body_vitality <= 0.f)
				object_deplete_body(unit_index);
			object->object.body_vitality = new_body_vitality;
		}
	}

	return;
}

void units_scripting_set_current_vitality(
	long object_list_index,
	real body_vitality,
	real shield_vitality)
{
	long reference_index;
	long unit_index;
	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		unit_scripting_set_current_vitality(unit_index, body_vitality, shield_vitality);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}
	return;
}

real unit_scripting_get_health(
	long unit_index)
{
	struct object_datum const *object;
	real result;

	object = object_try_and_get(unit_index);
	result = -1.f;
	if (object)
	{
		if (TEST_FLAG(object->object.damage_flags, _object_dead_bit))
			result = 0.f;
		else
			result = object->object.body_vitality;
	}

	return result;
}

real unit_scripting_get_shield(
	long unit_index)
{
	struct object_datum const *object;
	real result;

	object = object_try_and_get(unit_index);
	result = -1.f;
	if (object)
	{
		if (TEST_FLAG(object->object.damage_flags, _object_dead_bit))
			result = 0.f;
		else
			result = object->object.shield_vitality;
	}

	return result;
}

short unit_scripting_get_grenade_count(
	long unit_index)
{
	struct unit_datum const *unit;
	short grenade_count;
	short grenade_index;
	unit = unit_try_and_get(unit_index);
	grenade_count = 0;
	if (unit)
	{
		for (grenade_index = 0; grenade_index < NUMBEROF(unit->unit.grenade_counts); grenade_index++)
			grenade_count += unit->unit.grenade_counts[grenade_index];
	}
	return grenade_count;
}

void unit_scripting_impervious(
	long object_list_index,
	boolean impervious)
{
	long reference_index;
	long unit_index;
	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		struct unit_datum *unit;
		unit = unit_try_and_get(unit_index);
		if (unit)
			SET_FLAG(unit->unit.flags, _unit_impervious_bit, impervious);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}
	return;
}

boolean unit_scripting_start_user_animation_list(
	long object_list_index,
	long animation_graph_index,
	char const *animation_name,
	boolean interpolate)
{
	boolean result;
	long reference_index;
	long unit_index;
	result = TRUE;
	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		if (unit_try_and_get(unit_index))
			result = result && unit_start_user_animation(unit_index, animation_graph_index, animation_name, interpolate);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}
	return result;
}

boolean unit_scripting_has_weapon(
	long unit_index,
	long weapon_definition_index)
{
	boolean result;
	result = FALSE;
	if (unit_index != NONE && weapon_definition_index != NONE)
		result = unit_has_weapon_definition_index(unit_index, weapon_definition_index);
	return result;
}

boolean unit_scripting_has_weapon_readied(
	long unit_index,
	long weapon_definition_index)
{
	boolean result;
	result = FALSE;
	if (unit_index != NONE && weapon_definition_index != NONE)
	{
		struct unit_datum const *unit;
		long weapon_index;
		unit = unit_get(unit_index);
		weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
		if (weapon_index != NONE)
			result = weapon_get(weapon_index)->definition_index == weapon_definition_index;
	}
	return result;
}

/* ---------- private code */
