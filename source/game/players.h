/*
PLAYERS.H

header included in hcex build.
*/

#ifndef __PLAYERS_H
#define __PLAYERS_H
#pragma once

/* ---------- headers */

#include "game/game.h"

/* ---------- constants */

enum
{
	_player_powerup_active_camouflage = 0,
	_player_powerup_full_spectrum_vision,
	NUMBER_OF_PLAYER_POWERUPS,
	MAXIMUM_LOCAL_PLAYERS = 4,
	_local_player_triggered_switch_none = NONE,
};

enum player_action_result
{
	_player_action_result_reload = 0,
	_player_action_result_pickup_powerup,
	_player_action_result_pickup_weapon,
	_player_action_result_exit_vehicle,
	_player_action_result_swap_for_grenades,
	_player_action_result_swap_for_powerup,
	_player_action_result_swap_for_weapon,
	_player_action_result_add_weapon_to_inventory,
	_player_action_result_enter_vehicle,
	_player_action_result_evict_from_vehicle,
	_player_action_result_touch_device,
	_player_action_result_flip_vehicle,
	NUMBER_OF_PLAYER_ACTION_RESULTS,
};


/* ---------- macros */

#define player_get(index)			((struct player_datum*)datum_get(player_data, index))
#define player_try_and_get(index)	((struct player_datum*)datum_try_and_get(player_data, index))

/* ---------- structures */

struct player_action
{
	unsigned long control_flags;
	real_euler_angles2d desired_facing;
	real_vector2d throttle;
	real primary_trigger;
	short desired_weapon_index;
	short desired_grenade_index;
	short desired_zoom_level;
	short pad;
};

typedef char player_action_size_assert[
	sizeof(struct player_action) == 0x20 ? 1 : -1];
typedef char player_action_desired_facing_yaw_offset_assert[
	offsetof(struct player_action, desired_facing.yaw) == 0x4 ? 1 : -1];
typedef char player_action_desired_facing_pitch_offset_assert[
	offsetof(struct player_action, desired_facing.pitch) == 0x8 ? 1 : -1];

struct network_player
{
	wchar_t name[12];
	short primary_color_index;
	short icon_index;
	char machine_index;
	char controller_index;
	char team_index;
	char player_list_index;
};

struct player_datum
{
	short identifier;
	short local_player_index;
	wchar_t name[12];
	long squad_index;
	long team_index;
	long action_object_index;
	short action_result;
	short action_seat_index;
	long respawn_timer;
	long respawn_penalty;
	long unit_index;
	long dead_unit_index;
	short cluster_index;
	boolean swapped_weapons;
	byte pad0;
	long aim_assist_unit_index;
	long aim_assist_timestamp;
	struct network_player network_player_data;
	short powerup_durations[NUMBER_OF_PLAYER_POWERUPS];
	real speed_multiplier;
	long unknown70;
	long state_message;
	long state_message_player_index;
	long unknown7c;
	union
	{
		byte unknown80[4];
		long target_hold_time;
	};
	long death_time;
	long multiplayer_special;
	struct game_statistics statistics;
	long telefrag_timeout;
	long quit_out_of_game_time;
	boolean is_blocking_teleporter;
	boolean quit_out_of_game;
	byte pad_d2[2];
};

struct players_globals
{
	long unknown0;
	long local_players[MAXIMUM_LOCAL_PLAYERS];
	long dead_units[MAXIMUM_LOCAL_PLAYERS];
	short local_player_count;
	short double_speed_ticks;
	boolean all_dead;
	boolean input_disabled;
	short pending_teleport_starting_location_index;
	short respawn_failure;
	boolean respawn_failed;
	union
	{
		byte bsp_switch_state;
		struct
		{
			char local_player_triggered_switch : 4;
			byte bsp_check_recursive_switch_ticks : 4;
		};
	};
	unsigned long combined_pvs[16];
	unsigned long combined_pvs_local[16];
};

typedef char players_globals_local_player_count_offset_assert[
	offsetof(struct players_globals, local_player_count) == 0x24 ? 1 : -1];
typedef char players_globals_respawn_failure_offset_assert[
	offsetof(struct players_globals, respawn_failure) == 0x2C ? 1 : -1];
typedef char players_globals_all_dead_offset_assert[
	offsetof(struct players_globals, all_dead) == 0x28 ? 1 : -1];
typedef char players_globals_input_disabled_offset_assert[
	offsetof(struct players_globals, input_disabled) == 0x29 ? 1 : -1];
typedef char players_globals_combined_pvs_offset_assert[
	offsetof(struct players_globals, combined_pvs) == 0x30 ? 1 : -1];
typedef char players_globals_combined_pvs_local_offset_assert[
	offsetof(struct players_globals, combined_pvs_local) == 0x70 ? 1 : -1];
typedef char players_globals_size_assert[
	sizeof(struct players_globals) == 0xB0 ? 1 : -1];

typedef char player_datum_team_index_offset_assert[
	offsetof(struct player_datum, team_index) == 0x20 ? 1 : -1];
typedef char player_datum_target_hold_time_offset_assert[
	offsetof(struct player_datum, target_hold_time) == 0x80 ? 1 : -1];
typedef char player_datum_statistics_offset_assert[
	offsetof(struct player_datum, statistics) == 0x8C ? 1 : -1];
typedef char player_datum_telefrag_timeout_offset_assert[
	offsetof(struct player_datum, telefrag_timeout) == 0xC8 ? 1 : -1];
typedef char player_datum_quit_out_of_game_time_offset_assert[
	offsetof(struct player_datum, quit_out_of_game_time) == 0xCC ? 1 : -1];
typedef char player_datum_is_blocking_teleporter_offset_assert[
	offsetof(struct player_datum, is_blocking_teleporter) == 0xD0 ? 1 : -1];
typedef char player_datum_quit_out_of_game_offset_assert[
	offsetof(struct player_datum, quit_out_of_game) == 0xD1 ? 1 : -1];
typedef char player_datum_size_assert[
	sizeof(struct player_datum) == 0xD4 ? 1 : -1];

/* ---------- prototypes/PLAYER_CONTROL.C */

void player_input_enable(
	boolean enable);

boolean player_input_enabled(
	void);

void player_control_unzoom(long unit_index);

/* ---------- prototypes/PLAYERS.C */

void players_initialize(
	void);
void players_initialize_for_new_map(
	void);

long *machine_get_player_list(
	long machine_index);

long player_new(
	long machine_index,
	long player_index,
	short local_player_index,
	struct network_player const *network_player);

boolean local_player_exists(
	long local_player_index);
long local_player_get_player_index(
	short local_player_index);
void players_set_local_player_unit(
	short local_player_index,
	long unit_index);

short local_player_get_next(
	short local_player_index);

short local_player_count(
	void);

short players_get_respawn_failure(
	void);

boolean players_respawn_coop(
	void);

boolean players_are_all_dead(
	void);
boolean any_player_is_in_the_air(
	void);
boolean any_player_is_dead(
	void);

long player_index_from_unit_index(long unit_index);

unsigned long const *players_get_combined_pvs_local(
	void);

unsigned long const *players_get_combined_pvs(
	void);

void player_control_fix_for_loaded_game_state(void);

void player_handle_powerup_minor(
	long player_index,
	short powerup_index,
	short duration);

void player_add_equipment(
	long unit_index,
	short starting_profile_index,
	boolean reset_equipment);

void players_update_before_game(
	void);
void players_update_after_game(
	void);
void players_debug_render(
	void);

/* ---------- globals */

extern struct data_array *player_data;
extern struct players_globals *players_globals;

/* ---------- public code */

#endif // __PLAYERS_H
