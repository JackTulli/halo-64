/*
MAIN_RUNTIME.H
*/

#ifndef __MAIN_RUNTIME_H
#define __MAIN_RUNTIME_H
#pragma once

/* ---------- headers */

#include "math/integer_math.h"

/* ---------- prototypes/MAIN.C */

boolean gamepad_button_is_down(
	short button_index);
short main_get_window_count(
	void);

void compute_window_bounds(
	long player_index,
	long num_players,
	rectangle2d *pixel_bounds,
	rectangle2d *safe_frame_bounds);
void main_loop_of_death(
	void);

#endif // __MAIN_RUNTIME_H
