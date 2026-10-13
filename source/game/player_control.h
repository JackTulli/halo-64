#ifndef __PLAYER_CONTROL_H
#define __PLAYER_CONTROL_H
#pragma once

#include "networking/network_connection.h"

/* ---------- constants */

enum
{
	_player_control_action_bit,
	_player_control_jump_bit,
	_player_control_accept_bit,
	_player_control_back_bit,
	_player_control_primary_trigger_bit,
	_player_control_grenade_trigger_bit,
	_player_control_zoom_bit,
	_player_control_look_relative_up_bit,
	_player_control_look_relative_down_bit,
	_player_control_look_relative_left_bit,
	_player_control_look_relative_right_bit,
	_player_control_move_relative_forward_bit,
	_player_control_move_relative_backward_bit,
	_player_control_move_relative_right_bit,
	_player_control_move_relative_left_bit,
};

enum
{
	_player_control_camera_control_disabled_bit,
	_player_control_look_relative_all_directions_flags = 0x780,
	_player_control_move_relative_all_directions_flags = 0x7800,
};

enum
{
	_player_control_rotate_weapons_bit,
	_player_control_rotate_grenades_bit,
	_player_control_input_zoom_bit,
	_player_control_debug_rotate_units_bit,
	_player_control_debug_rotate_all_units_bit,
	_player_control_debug_ninja_rope_bit,
};

/* ---------- structures */

struct unit_camera;

struct player_control_unit_camera_info
{
	long unit_index;
	short seat_index;
	short pad6;
	struct unit_camera const *camera;
	real_point3d position;
};

struct player_control
{
	long unit_index;
	unsigned long control_flags;
	word inhibited_button_bit_vector;
	word reset_button_when_released_bit_vector;
	real_euler_angles2d desired_angles;
	real_vector2d throttle;
	real primary_trigger;
	short desired_weapon_index;
	short desired_grenade_index;
	short zoom_level;
	boolean use_autolevel;
	char autolevel_ticks;
	long target_object_index;
	real autoaim_level;
	real magnetism_level;
	real look_acceleration_time;
	real pitch_minimum;
	real pitch_maximum;
};

struct player_control_globals_data
{
	unsigned long action_flags;
	unsigned long action_test_flags;
	unsigned long suppressed_action_flags;
	unsigned long flags;
	struct player_control players[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
};

typedef char player_control_size_assert[
	sizeof(struct player_control) == 0x40 ? 1 : -1];
typedef char player_control_unit_index_offset_assert[
	offsetof(struct player_control, unit_index) == 0x0 ? 1 : -1];
typedef char player_control_inhibited_action_flags_offset_assert[
	offsetof(struct player_control, inhibited_button_bit_vector) == 0x8 ? 1 : -1];
typedef char player_control_desired_yaw_offset_assert[
	offsetof(struct player_control, desired_angles.yaw) == 0xC ? 1 : -1];
typedef char player_control_desired_pitch_offset_assert[
	offsetof(struct player_control, desired_angles.pitch) == 0x10 ? 1 : -1];
typedef char player_control_desired_weapon_index_offset_assert[
	offsetof(struct player_control, desired_weapon_index) == 0x20 ? 1 : -1];
typedef char player_control_desired_grenade_index_offset_assert[
	offsetof(struct player_control, desired_grenade_index) == 0x22 ? 1 : -1];
typedef char player_control_zoom_level_offset_assert[
	offsetof(struct player_control, zoom_level) == 0x24 ? 1 : -1];
typedef char player_control_target_object_index_offset_assert[
	offsetof(struct player_control, target_object_index) == 0x28 ? 1 : -1];
typedef char player_control_autoaim_level_offset_assert[
	offsetof(struct player_control, autoaim_level) == 0x2C ? 1 : -1];
typedef char player_control_magnetism_level_offset_assert[
	offsetof(struct player_control, magnetism_level) == 0x30 ? 1 : -1];
typedef char player_control_look_acceleration_time_offset_assert[
	offsetof(struct player_control, look_acceleration_time) == 0x34 ? 1 : -1];
typedef char player_control_pitch_minimum_offset_assert[
	offsetof(struct player_control, pitch_minimum) == 0x38 ? 1 : -1];
typedef char player_control_pitch_maximum_offset_assert[
	offsetof(struct player_control, pitch_maximum) == 0x3C ? 1 : -1];
typedef char player_control_unit_camera_info_size_assert[
	sizeof(struct player_control_unit_camera_info) == 0x18 ? 1 : -1];
typedef char player_control_globals_size_assert[
	sizeof(struct player_control_globals_data) == 0x110 ? 1 : -1];
typedef char player_control_globals_players_offset_assert[
	offsetof(struct player_control_globals_data, players) == 0x10 ? 1 : -1];
typedef char player_control_globals_action_test_flags_offset_assert[
	offsetof(struct player_control_globals_data, action_test_flags) == 0x4 ? 1 : -1];
typedef char player_control_globals_suppressed_action_flags_offset_assert[
	offsetof(struct player_control_globals_data, suppressed_action_flags) == 0x8 ? 1 : -1];
typedef char player_control_globals_flags_offset_assert[
	offsetof(struct player_control_globals_data, flags) == 0xC ? 1 : -1];

/* ---------- prototypes */

void player_control_initialize(
	void);
void player_control_dispose(
	void);
void player_control_dispose_from_old_map(
	void);
boolean player_control_camera_control_is_active(
	void);
struct player_control *player_control_get(
	short local_player_index);
void player_control_inhibit_buttons(
	short local_player_index,
	word action_flags,
	boolean persistent);
long player_control_get_target_object_index(
	short local_player_index);
real player_control_get_field_of_view(
	short local_player_index);
void player_control_get_unit_camera_info(
	short local_player_index,
	struct player_control_unit_camera_info *camera_info);
long player_control_get_unit_index(
	short local_player_index);
long player_control_get_desired_weapon(
	short local_player_index,
	long unit_index);
short player_control_get_zoom_level(
	short local_player_index);
real player_control_get_autoaim_level(
	short local_player_index);
void players_unzoom_all(
	void);
void player_control_unzoom(
	long unit_index);
real_euler_angles2d const *player_control_get_facing_angles(
	short local_player_index);
real_vector3d *player_control_get_facing_direction(
	short local_player_index,
	real_vector3d *facing_direction);
void player_control_set_desired_weapon(
	long unit_index,
	short desired_weapon_index);
void player_control_set_facing(
	short local_player_index,
	real_vector3d const *facing_direction);
void player_control_new_unit(
	short local_player_index,
	long unit_index);
void player_control_initialize_for_new_map(
	void);
void player_control_permanent_impulse(
	short local_player_index,
	real_euler_angles2d const *delta);
boolean scripted_player_control_set_camera_control(
	boolean camera_control);
void player_control_action_test_reset(
	void);
boolean player_control_action_test_accept(
	void);
boolean player_control_action_test_back(
	void);
boolean player_control_action_test_action(
	void);
boolean player_control_action_test_jump(
	void);
boolean player_control_action_test_primary_trigger(
	void);
boolean player_control_action_test_grenade_trigger(
	void);
boolean player_control_action_test_zoom(
	void);
boolean player_control_action_test_look_relative_left(
	void);
boolean player_control_action_test_look_relative_right(
	void);
boolean player_control_action_test_look_relative_up(
	void);
boolean player_control_action_test_look_relative_down(
	void);
boolean player_control_action_test_move_relative_all_directions(
	void);
boolean player_control_action_test_look_relative_all_directions(
	void);

/* ---------- globals */

extern real player_look_yaw_rate[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
extern real player_look_pitch_rate[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];

#endif // __PLAYER_CONTROL_H
