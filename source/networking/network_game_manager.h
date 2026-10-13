/*
NETWORK_GAME_MANAGER.H

header included in hcex build.
*/

#ifndef __NETWORK_GAME_MANAGER_H
#define __NETWORK_GAME_MANAGER_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct network_game;
struct network_machine;
struct network_player;

/* ---------- prototypes/EXAMPLE.C */

void network_game_invalidate_player(
	struct network_player *player);
void network_game_end_and_load_ui(
	struct network_game *game);
void network_game_dump(
	struct network_game *game,
	char const *prefix);
boolean network_player_is_valid(
	struct network_player *player);
boolean network_game_add_machine(
	struct network_game *game,
	struct network_machine *machine);
boolean network_game_update_machine(
	struct network_game *game,
	struct network_machine *machine);
void xbox_set_machine_name(
	char const *machine_name);
void network_game_generate_local_machine_name(
	wchar_t *machine_name);
boolean network_game_spawn_player(
	struct network_player *player);
boolean network_game_player_is_valid(
	struct network_player *player,
	struct network_game *game);
void network_game_reset_for_next_round(
	struct network_game *game,
	boolean unload_game_objects);
boolean network_game_add_player(
	struct network_game *game,
	struct network_player *player);
void network_game_invalidate_machine(
	struct network_game *game,
	word machine_index);
void network_game_invalidate(
	struct network_game *game);
boolean network_game_update_player(
	struct network_game *game,
	struct network_player *player);
boolean network_game_remove_player(
	struct network_game *game,
	struct network_player *player);
boolean network_game_remove_machine(
	struct network_game *game,
	struct network_machine *machine);
boolean network_game_create_game_objects(
	struct network_game *game);

/* ---------- globals */

/* ---------- public code */

#endif // __NETWORK_GAME_MANAGER_H
