/*
GAME_ENGINE_RUNTIME.H
*/

#ifndef __GAME_ENGINE_RUNTIME_H
#define __GAME_ENGINE_RUNTIME_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "game/game_engine.h"

/* ---------- structures */

union real_point3d;


/* ---------- prototypes/GAME_ENGINE.C */

void game_engine_update(
	void);

void game_show_score_team(
	long team_index,
	long score);

void game_show_score_you_ally_enemy(
	long player_index,
	long you_score,
	long ally_score,
	long enemy_score,
	long other_player_index);

void game_engine_flag_reset(
	long weapon_index,
	union real_point3d const *position);

/* ---------- prototypes/GAME_ENGINE_CTF.C */

void ctf_state_message_update_warning(
	long team_index);

#endif // __GAME_ENGINE_RUNTIME_H
