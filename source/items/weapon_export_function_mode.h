/*
WEAPON_EXPORT_FUNCTION_MODE.H
*/

#ifndef __WEAPON_EXPORT_FUNCTION_MODE_H
#define __WEAPON_EXPORT_FUNCTION_MODE_H
#pragma once

/* ---------- constants */

enum weapon_export_function_mode
{
	_weapon_function_none = 0,
	_weapon_function_heat,
	_weapon_function_primary_ammunition,
	_weapon_function_secondary_ammunition,
	_weapon_function_primary_rate_of_fire,
	_weapon_function_secondary_rate_of_fire,
	_weapon_function_ready,
	_weapon_function_primary_ejection_port,
	_weapon_function_secondary_ejection_port,
	_weapon_function_overheated,
	_weapon_function_primary_charged,
	_weapon_function_secondary_charged,
	_weapon_function_illumination,
	_weapon_function_age,
	_weapon_function_integrated_light,
	_weapon_function_primary_firing,
	_weapon_function_secondary_firing,
	NUMBER_OF_WEAPON_FUNCTION_MODES,
};

#endif // __WEAPON_EXPORT_FUNCTION_MODE_H
