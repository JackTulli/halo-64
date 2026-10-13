/*
WEAPON_DATUM_FLAGS.H
*/

#ifndef __WEAPON_DATUM_FLAGS_H
#define __WEAPON_DATUM_FLAGS_H
#pragma once

/* ---------- constants */

enum weapon_datum_flag
{
	_weapon_overheated_bit = 0,
	_weapon_overheated_exit_bit,
	_weapon_overheat_recoil_bit,
	_weapon_needs_to_reload_bit,
	_weapon_integrated_light_on_bit,
	_weapon_multiplayer_inventory_bit,
	_weapon_multiplayer_bit,
	NUMBER_OF_WEAPON_DATUM_FLAGS,
};

#endif // __WEAPON_DATUM_FLAGS_H
