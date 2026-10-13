/*
GAME_STATISTICS_INTERNAL.H

Narrow cross-translation-unit interface owned by GAME_STATISTICS.C.
*/

#ifndef __GAME_STATISTICS_INTERNAL_H
#define __GAME_STATISTICS_INTERNAL_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- prototypes/GAME_STATISTICS.C */

void game_statistics_record_damage(
	long object_index,
	real damage,
	long owner_player_index,
	long owner_object_index,
	short owner_team_index);

#endif // __GAME_STATISTICS_INTERNAL_H
