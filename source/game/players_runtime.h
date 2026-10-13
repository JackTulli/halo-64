/*
PLAYERS_RUNTIME.H
*/

#ifndef __PLAYERS_RUNTIME_H
#define __PLAYERS_RUNTIME_H
#pragma once

/* ---------- structures */

struct player_starting_location;

/* ---------- prototypes/PLAYERS.C */

short player_get_starting_location_count(
	void);

struct player_starting_location *player_get_starting_location(
	short starting_location_index);

#endif // __PLAYERS_RUNTIME_H
