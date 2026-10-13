/*
GAME_ENGINE_KING.H
*/

#ifndef __GAME_ENGINE_KING_H
#define __GAME_ENGINE_KING_H
#pragma once

/* ---------- headers */

#include "game/game_engine.h"

/* ---------- constants */

enum king_message
{
	king_message_enemy_on_the_hill = 0x1E,
	king_message_ally_on_the_hill,
	king_message_you_are_on_the_hill,
};

enum king_hill_state
{
	king_hill_uncontrolled = 0,
	king_hill_controlled,
	king_hill_controlled_red,
	king_hill_controlled_blue,
	king_hill_contested,
};

/* ---------- structures */

struct model_vertex_uncompressed;
struct render_animation;
struct render_lighting;

struct king_globals
{
#ifdef HALO_LINUX
	/* port: score and score_tick are indexed by team (free for all gives
	every player a team), on_the_hill by absolute player index, so all three
	follow the session player limit (halo_port_limits.h) */
	long score[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
	long score_tick[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
	boolean on_the_hill[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
#else
	long score[16];
	long score_tick[16];
	boolean on_the_hill[16];
#endif
	long hill_point_count;
	real_point3d hill_points[12];
	real_point2d convex_hull[12];
	real_point3d hill_center;
	long hill_state;
	long hill_controlled_count;
	long hill_previous_controller;
	real hill_top;
	real hill_bottom;
	long hill_id;
	long hill_timer;
};

/* January's layout; the port's per-player arrays are larger */
#ifndef HALO_LINUX
typedef char verify_king_globals_size[
	sizeof(struct king_globals) == 0x1AC ? 1 : -1];
typedef char verify_king_globals_on_the_hill_offset[
	offsetof(struct king_globals, on_the_hill) == 0x80 ? 1 : -1];
typedef char verify_king_globals_convex_hull_offset[
	offsetof(struct king_globals, convex_hull) == 0x124 ? 1 : -1];
typedef char verify_king_globals_hill_id_offset[
	offsetof(struct king_globals, hill_id) == 0x1A4 ? 1 : -1];
#endif

/* ---------- prototypes/GAME_ENGINE_KING.C */

void king_engine_dispose(
	void);

boolean king_engine_initialize_for_new_map(
	void);

void king_engine_dispose_from_old_map(
	void);

void king_engine_player_added(
	long player_index);

void king_engine_game_ending(
	void);

void king_engine_game_starting(
	void);

void king_engine_statistics_append(
	long statistic);

void king_engine_handle_client_message(
	void *message);

void king_engine_handle_server_message(
	void *message);

void king_engine_pregame_post_rasterize(
	void);

void king_engine_post_rasterize(
	void);

void king_engine_player_update(
	long player_index);

void king_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type);

void king_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire);

boolean king_engine_display_score(
	long player_index,
	long message,
	long message_player_index,
	wchar_t *buffer,
	long buffer_size);

void king_engine_prespawn_player_update(
	long player_index);

long king_get_score(
	long player_index,
	enum get_score_type score_type);

wchar_t *king_get_score_string(
	long player_index,
	wchar_t *buffer);

wchar_t *king_get_score_header_string(
	wchar_t *buffer);

wchar_t *king_get_team_score_string(
	long team_index,
	wchar_t *buffer);

boolean king_engine_goal_matches_player(
	long player_index,
	long goal_index);

void king_engine_update(
	void);

void render_dynamic_quad_initialize(
	void);

void render_dynamic_quad(
	struct model_vertex_uncompressed *vertices,
	long shader_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation,
	real u_scale,
	real v_scale);

/* ---------- globals */

extern struct game_engine king_engine;
extern struct king_globals king_globals;

#endif // __GAME_ENGINE_KING_H
