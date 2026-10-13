/*
PLAYER_EFFECTS.H

header included in hcex build.
*/

#ifndef __PLAYER_EFFECTS_H
#define __PLAYER_EFFECTS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

struct screen_flash_definition;
struct render_screen_flash;
struct damage_data;

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/PLAYER_EFFECTS.C */

/* ---------- globals */

/* ---------- public code */

void player_effect_initialize(
	void);
void player_effect_dispose(
	void);
void player_effect_initialize_for_new_map(
	void);
void player_effect_dispose_from_old_map(
	void);
void player_effect_add_continuous_effect(
	short local_player_index,
	long definition_index,
	real distance);
void player_effect_update(
	void);
void player_effect_continuous_refresh(
	long definition_index,
	real_point3d const *position);
void player_effect_screen_fade_in(
	real red,
	real green,
	real blue,
	short ticks);
void player_effect_screen_fade_out(
	real red,
	real green,
	real blue,
	short ticks);
void player_effect_get_damage_indicators(
	short local_player_index,
	byte *damage_indicators);
void player_effect_clear_damage_indicators(
	short local_player_index);
void player_effect_get_screen_flash(
	short local_player_index,
	struct render_screen_flash *screen_flash);
void player_effect_get_camera_effect_matrix(
	short local_player_index,
	real_matrix4x3 *matrix);
void player_effect_start(
	long player_index,
	struct damage_data const *damage,
	real_vector3d const *direction,
	real scale,
	real total_damage);
void scripted_player_effect_set_translation(
	real horizontal,
	real vertical,
	real depth);
void scripted_player_effect_set_rotation(
	real yaw,
	real pitch,
	real roll);
void scripted_player_effect_set_rumble(
	real left_motor,
	real right_motor);
void scripted_player_effect_start(
	real maximum_intensity,
	real attack_time);
void scripted_player_effect_stop(
	real duration);
void player_effect_screen_flash(
	long player_index,
	struct screen_flash_definition const *screen_flash,
	real scale);
void player_telefrag_effect_stop(
	long player_index);
void player_telefrag_effect_start(
	long player_index,
	real intensity);

#endif // __PLAYER_EFFECTS_H
