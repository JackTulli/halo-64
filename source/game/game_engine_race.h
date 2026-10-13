/*
GAME_ENGINE_RACE.H
*/

#ifndef __GAME_ENGINE_RACE_H
#define __GAME_ENGINE_RACE_H
#pragma once

/* ---------- headers */

#include "game/game_engine.h"

/* ---------- structures */

struct race_globals;

/* ---------- prototypes/GAME_ENGINE_RACE.C */

void race_engine_dispose(
	void);

boolean race_engine_initialize_for_new_map(
	void);

void race_engine_dispose_from_old_map(
	void);

void race_engine_player_added(
	long player_index);

void race_engine_game_ending(
	void);

void race_engine_game_starting(
	void);

void race_engine_statistics_append(
	long statistic);

void race_engine_handle_client_message(
	void *message);

void race_engine_handle_server_message(
	void *message);

void race_engine_pregame_post_rasterize(
	void);

void race_engine_post_rasterize(
	void);

void race_engine_player_update(
	long player_index);

void race_engine_weapon_update(
	long item_index,
	struct weapon_datum *weapon);

void race_engine_player_damaged_player(
	long killing_player_index,
	long dead_player_index,
	boolean friendly_fire);

void race_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire);

boolean race_engine_display_score(
	long player_index,
	long message,
	long message_player_index,
	wchar_t *buffer,
	long buffer_size);

void race_engine_prespawn_player_update(
	long player_index);

boolean race_goal_matches_player(
	long player_index,
	long goal_index);

long race_engine_get_score(
	long player_index,
	enum get_score_type score_type);

wchar_t *race_get_score_string(
	long player_index,
	wchar_t *string);

wchar_t *race_get_score_header_string(
	wchar_t *string);

wchar_t *race_get_team_score_string(
	long team_index,
	wchar_t *string);

long race_engine_did_player_win(
	long player_index);

void race_engine_update(
	void);

void race_flags_make_unique(
	void);

/* ---------- globals */

extern struct game_engine race_engine;
extern struct race_globals race_globals;

#endif // __GAME_ENGINE_RACE_H
