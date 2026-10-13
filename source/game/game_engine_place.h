/*
GAME_ENGINE_PLACE.H
*/

#ifndef __GAME_ENGINE_PLACE_H
#define __GAME_ENGINE_PLACE_H
#pragma once

/* ---------- headers */

#include "game/game_engine.h"

/* ---------- structures */

enum game_engine_place_flags
{
	_place_tied = 0,
	_place_all_tied,
	_place_two_groups,
	_place_team,
};

struct game_engine_place
{
	short flags;
	short place;
};

/* ---------- prototypes/GAME_ENGINE.C */

wchar_t *get_place_name(
	struct game_engine_place place);

struct game_engine_place game_engine_get_place(
	long player_index,
	enum get_score_type score_type);

#endif // __GAME_ENGINE_PLACE_H
