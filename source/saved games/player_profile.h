/*
PLAYER_PROFILE.H

header included in hcex build.
*/

#ifndef __PLAYER_PROFILE_H
#define __PLAYER_PROFILE_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

enum
{
	MAXIMUM_PLAYER_PROFILE_NAME_LENGTH = 12,
	NUMBER_OF_SINGLE_PLAYER_LEVELS = 10,
	NUMBER_OF_DEFAULT_PROFILES = 2,
	NUMBER_OF_LOOK_SENSITIVITY_SETTINGS = 10
};

enum
{
	_player_profile_default_profile_bit = 0
};

/* January button mappings; the swap names are reconstructed from the
   actual button table, not later-game preset ordinals. */
enum
{
	_button_preset_standard = 0,
	_button_preset_swap_triggers,
	_button_preset_swap_a_and_left_trigger,
	_button_preset_swap_b_and_left_trigger,
	_button_preset_swap_b_and_right_thumb,
	NUMBER_OF_BUTTON_PRESETS
};

enum
{
	_joystick_preset_standard = 0,
	_joystick_preset_south_paw,
	_joystick_preset_legacy,
	_joystick_preset_legacy_south_paw,
	NUMBER_OF_JOYSTICK_PRESETS
};

/* ---------- macros */

/* ---------- structures */

struct player_profile_controller_settings
{
	byte button_preset;
	byte joystick_preset;
	byte look_sensitivity;
	boolean invert_look;
	boolean vibration_disabled;
	boolean flight_stick_aircraft_controls;
	boolean autocenter;
	boolean ingame_help_disabled;
};

struct player_profile
{
	wchar_t player_name[MAXIMUM_PLAYER_PROFILE_NAME_LENGTH];
	short primary_color_index;
	word flags;
	byte single_player_map_flags[NUMBER_OF_SINGLE_PLAYER_LEVELS];
	short last_single_player_map_played;
	struct player_profile_controller_settings controller_settings;
};

/* ---------- prototypes/PLAYER_PROFILE.C */

void player_profiles_initialize(
	void);
void player_profiles_dispose(
	void);
long player_profile_get_random_color(
	void);
long player_profile_get_random_good_color(
	void);
boolean player_profile_get_enclosing_directory_path(
	long profile,
	char *full_path);
boolean player_profile_get(
	long player_profile_index,
	struct player_profile *profile);
void player_profile_save(
	long player_profile_index,
	struct player_profile *profile);
void player_profile_get_highest_completed_solo_level(
	struct player_profile *profile,
	short *level,
	short *difficulty);
word player_profile_number_of_available_primary_colors(
	void);
real_rgb_color player_profile_get_rgb_color(
	long color_index);
void player_profile_save_level_completed(
	short local_player_index);
void player_profiles_enumerate_available_to_local_player_index(
	short local_player_index,
	word *number_of_profiles,
	long *player_profile_indices,
	boolean include_default_profiles);

void player_profile_delete(
	long player_profile_index);
boolean player_profile_get_from_path(
	char *full_path,
	struct player_profile *profile);
long player_profile_new(
	short local_player_index,
	wchar_t *name);
real_argb_color player_profile_get_argb_color(
	long color_index);
void player_profile_save_last_level_played(
	short local_player_index);

/* ---------- globals */

/* ---------- public code */

#endif // __PLAYER_PROFILE_H
