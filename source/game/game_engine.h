/*
GAME_ENGINE.H

header included in hcex build.
*/

#ifndef __GAME_ENGINE_H
#define __GAME_ENGINE_H
#pragma once

#include "math/real_math.h"

/* ---------- constants */

enum
{
	_game_engine_allow_dynamic_lighting_bit = 0,
	_game_engine_allow_integrated_lights_bit,
	_game_engine_disable_infinite_grenades_bit,
};

enum
{
	_game_variant_draw_object_in_motion_sensor_bit = 0,
	_game_variant_unknown1_bit = 1,
	_game_variant_infinite_grenades_bit = 2,
	_game_variant_no_shields_bit,
	_game_variant_always_invisible_bit,
};

enum game_engine_type
{
	game_engine_none = 0,
	game_engine_ctf,
	game_engine_slayer,
	game_engine_oddball,
	game_engine_king,
	game_engine_race,
	game_engine_terminator,
	game_engine_stub,
	number_of_game_engines,
	first_usable_game_engine_index = game_engine_ctf,
	last_usable_game_engine_index = game_engine_race,
};

enum get_score_type
{
	_get_score_individual = 0,
	_get_score_team,
};

/* ---------- macros */

/* ---------- structures */

struct game_globals;
struct player_starting_location;
struct weapon_datum;
union real_argb_color;
union real_point2d;
union real_point3d;
union real_rgb_color;

struct universal_variant
{
	boolean teams;
	byte pad0;
	byte pad1;
	byte pad2;
	unsigned long flags;
	long goal_radar;
	boolean odd_man_out;
	byte pad4;
	byte pad5;
	byte pad6;
	long respawn_time_growth;
	long respawn_time;
	long suicide_penalty;
	long lives;
	real health;
	long score_to_win;
	long weapon_set;
	long vehicle_set;
};

struct ctf_variant
{
	boolean assault;
	boolean reset_on_capture;
	boolean flag_must_reset;
	boolean flag_at_home_to_score;
	long single_flag_time;
};

struct slayer_variant
{
	boolean no_death_bonus;
	boolean no_kill_penalty;
	boolean kill_in_order;
};

struct king_variant
{
	boolean moving_hill;
};

struct oddball_variant
{
	boolean random_start;
	boolean ball_spawn_delay;
	long speed_with_ball;
	long trait_with_ball;
	long trait_without_ball;
	long oddball_ball_type;
	long ball_spawn_count;
};

struct race_variant
{
	long race_type;
	long team_scoring;
};

union game_engine_variant
{
	struct ctf_variant ctf;
	struct slayer_variant slayer;
	struct king_variant king;
	struct oddball_variant oddball;
	struct race_variant race;
};

struct game_variant
{
	wchar_t human_readable_game_description[12];
	long game_engine_index;
	struct universal_variant universal_variant;
	union game_engine_variant game_engine_variant;
	word flags;
};

typedef char verify_game_variant_size[sizeof(struct game_variant) == 0x68 ? 1 : -1];

struct game_engine
{
	char const *name;
	long type;
	void (*dispose)(void);
	boolean (*initialize_for_new_map)(void);
	void (*dispose_from_old_map)(void);
	void (*player_added)(long player_index);
	void (*game_ending)(void);
	void (*game_starting)(void);
	void (*statistics_append)(long statistic);
	void (*handle_client_message)(void *message);
	void (*handle_server_message)(void *message);
	void (*unknown2C)(void);
	void (*post_rasterize_objects)(void);
	void (*player_update_each_tick)(
		long player_index);
	void (*objective_weapon_update)(
		long item_index,
		struct weapon_datum *weapon);
	boolean (*picking_up)(
		long weapon_index,
		long player_index);
	void (*weapon_dropped)(
		long weapon_index);
	void (*unknown44)(void);
	long (*get_player_score)(
		long player_index,
		enum get_score_type score_type);
	wchar_t *(*format_player_score)(
		long player_index,
		wchar_t *string);
	wchar_t *(*format_score_name)(
		wchar_t *string);
	wchar_t *(*format_team_name)(
		long team_index,
		wchar_t *string);
	boolean (*allow_pick_up)(
		long unit_index,
		long weapon_index);
	void (*player_damaged_player)(
		long damaging_player_index,
		long dead_player_index,
		boolean damage_type);
	void (*player_killed_player)(
		long killing_player_index,
		long killing_object_index,
		long dead_player_index,
		boolean friendly_fire);
	boolean (*format_message)(
		long player_index,
		long parameter1,
		long parameter2,
		wchar_t *message,
		long message_character_count);
	float (*starting_location_rating)(
		long player_index,
		struct player_starting_location const *starting_location);
	void (*prespawn_player_update)(
		long player_index);
	void (*player_update)(
		long player_index);
	void (*team_index_override)(void);
	boolean (*player_can_see_goal)(
		long player_index,
		long goal_index);
	boolean (*test_flag)(
		long flag);
	boolean (*test_trait)(
		long trait,
		long value);
	long (*did_player_win)(
		long player_index);
};

typedef char verify_game_engine_size[sizeof(struct game_engine) == 0x88 ? 1 : -1];
typedef char verify_game_engine_player_update_each_tick_offset[
	offsetof(struct game_engine, player_update_each_tick) == 0x34 ? 1 : -1];
typedef char verify_game_engine_objective_weapon_update_offset[
	offsetof(struct game_engine, objective_weapon_update) == 0x38 ? 1 : -1];
typedef char verify_game_engine_format_player_score_offset[
	offsetof(struct game_engine, format_player_score) == 0x4C ? 1 : -1];
typedef char verify_game_engine_format_score_name_offset[
	offsetof(struct game_engine, format_score_name) == 0x50 ? 1 : -1];
typedef char verify_game_engine_format_team_name_offset[
	offsetof(struct game_engine, format_team_name) == 0x54 ? 1 : -1];
typedef char verify_game_engine_format_message_offset[
	offsetof(struct game_engine, format_message) == 0x64 ? 1 : -1];
typedef char verify_game_engine_player_update_offset[
	offsetof(struct game_engine, player_update) == 0x70 ? 1 : -1];

/* ---------- prototypes/GAME_ENGINE.C */

void game_engine_playlist_initialize(
	void);

long game_globals_get_weapon(
	struct game_globals *game_globals,
	long weapon_list_index);

void game_engine_playlist_begin(
	void);

boolean game_engine_get_current_stage(
	struct game_variant *variant,
	char *map_name);

long list_index_to_weapon_definition_index(
	long weapon_list_index);

long weapon_definition_index_to_list_index(
	long weapon_definition_index);

void game_engine_state_message(
	long player_index,
	long state_message,
	long state_message_player_index);

void game_engine_player_depower_active_camo(
	long player_index);

long game_engine_get_team_score(
	long team_index);

long players_in_game(
	void);

float get_blink_alpha(
	void);

long game_engine_player_get_team_index(
	long player_index);

void game_engine_update_player_always_invis(
	long player_index);

boolean game_engine_player_has_flag(
	long player_index);

void game_show_score(
	long player_index,
	long score);

void get_postgame_hilite_colors(
	union real_argb_color *winner_color,
	union real_argb_color *normal_color,
	union real_argb_color *hilite_color);

boolean game_engine_running(
	void);

boolean game_engine_get_state_message(
	long player_index,
	wchar_t *message,
	long maximum_length);

boolean game_engine_force_single_screen(
	void);

void game_engine_dispose(
	void);

void game_engine_dispose_from_old_map(
	void);

void game_engine_game_ending(
	void);

void game_engine_game_starting(
	void);

void game_engine_statistics_append(
	long statistic);

void game_engine_handle_client_message(
	void *message);

void game_engine_handle_server_message(
	void *message);

void game_engine_post_rasterize_objects(
	void);

void game_engine_post_rasterize(
	void);

void game_engine_nonplayer_post_rasterize(
	void);

void game_engine_update_non_deterministic(
	float delta_seconds);

boolean match_game_type(
	long game_type,
	long count,
	short const *game_types);

void game_engine_initialize(
	struct game_variant *variant);

void game_engine_initialize_for_new_map(
	void);

float game_engine_get_distance_rating_for_spawn(
	long player_index,
	union real_point3d const *position);

void game_engine_variant_cleanup(
	struct game_variant *variant);

boolean game_engine_display_team_indicators(
	void);

boolean game_engine_can_score(
	void);

real game_engine_get_starting_location_rating(
	long player_index,
	struct player_starting_location const *starting_location);
boolean game_engine_should_spawn_player(
	long player_index);
void game_engine_postspawn_player_update(
	long player_index);

boolean game_engine_allow_pick_up(
	long unit_index,
	long weapon_index);

boolean game_engine_picking_up(
	long unit_index,
	long weapon_index);

boolean game_engine_test_flag(
	long flag);

boolean game_engine_test_trait(
	long trait,
	long value);

void game_engine_prespawn_player_update(
	long player_index);

long game_engine_did_player_win(
	long player_index);

struct game_variant *game_engine_get_variant(
	void);

struct game_variant *game_engine_get_variant_by_name(
	struct game_variant *variant,
	char const *name);

boolean game_engine_get_goal_in_use(
	short goal_index);

void game_engine_set_goal_position(
	short goal_index,
	union real_point3d const *position,
	float height,
	char const *name,
	long target_object_index,
	short team_index,
	long player_index);

boolean game_engine_has_teams(
	void);

boolean game_engine_allow_pause(
	void);

boolean game_engine_allow_dynamic_lighting(
	void);

boolean game_engine_infinite_grenades(
	long player_index);

boolean game_engine_has_shield(
	long player_index);

boolean game_engine_draw_object_in_motion_sensor(
	long unit_index);

boolean game_engine_hud_draw_motion_sensor(
	long player_index);

boolean game_engine_player_has_stealth_weapon(
	long player_index);

void game_engine_weapon_fired(
	long player_index);

short game_engine_player_get_custom_motion_sensor_positions(
	long player_index,
	union real_point2d *positions,
	byte *goal_indices,
	short maximum_count);

void game_engine_render_nav_points(
	long local_player_index);

union real_rgb_color *game_engine_player_get_change_color(
	union real_rgb_color *change_color,
	long player_index);

long find_netgame_flags(
	union real_point3d const *position,
	real radius,
	real height,
	short type,
	short team_index,
	long maximum_count,
	long *flag_indices);

long find_netgame_flag(
	union real_point3d const *position,
	real radius,
	real height,
	short type,
	short team_index);

boolean game_engine_should_end_game(
	void);

void game_engine_clear_goal_position(
	short goal_index);

long get_flag_definition_index(
	void);

long get_ball_definition_index(
	void);

void game_engine_override_map_name(
	char const *map_name);

void game_engine_override_game_variant(
	struct game_variant const *variant);

void game_engine_switch_to_postgame(
	void);

void game_engine_load_stage(
	char const *map_name);

void game_engine_end_game(
	void);

void ticks_to_unicode_time_string(
	long ticks,
	unsigned long character_count,
	wchar_t *string);

void game_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type);

boolean game_engine_player_is_out_of_lives(
	long player_index);

boolean game_engine_man_out(
	long player_index);

boolean game_engine_hud_draw_messages(
	long player_index);

boolean game_engine_force_autopickup(
	long unit_index,
	long weapon_index);

void game_engine_play_multiplayer_sound(
	long sound_index);

long game_engine_remap_object_definition(long definition_index);

long game_engine_remap_vehicle(long vehicle_definition_index);
long game_engine_remap_equipment(long equipment_definition_index);
long game_engine_remap_weapon(long weapon_definition_index);


boolean game_engine_allow_integrated_lights(
	long object_index);

void game_engine_player_killed(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire);

/* ---------- globals */

extern struct game_engine *game_engine;

/* ---------- public code */

real game_engine_get_damage_multiplier(
	long damaging_player_index,
	long damaged_player_index);

#endif // __GAME_ENGINE_H
