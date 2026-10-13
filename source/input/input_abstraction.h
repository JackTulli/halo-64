/*
INPUT_ABSTRACTION.H

header included in hcex build.
*/

#ifndef __INPUT_ABSTRACTION_H
#define __INPUT_ABSTRACTION_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct game_input_preferences
{
	real yaw_rate;
	real pitch_rate;
	byte game_control_to_xbox_buttons[12];
	short joystick_controls;
	boolean invert_look;
	boolean invert_look_aircraft_control;
};

struct game_input_state;

typedef char verify_game_input_preferences_size[
	sizeof(struct game_input_preferences) == 0x18 ? 1 : -1];

/* ---------- prototypes/INPUT_ABSTRACTION.C */

void input_abstraction_initialize(
	void);
void input_abstraction_update(
	void);
void input_abstraction_update_device_changes(
	unsigned long device_change_flags);
void input_abstraction_reset_controller_detection_timer(
	void);
void input_abstraction_get_local_player_preferences(
	short local_player_index,
	struct game_input_preferences *preferences);
void input_abstraction_update_local_player_preferences(
	short controller_index,
	struct game_input_preferences const *preferences);
struct game_input_state *input_abstraction_get_input_state(
	short local_player_index);

/* ---------- globals */

/* ---------- public code */

#endif // __INPUT_ABSTRACTION_H
