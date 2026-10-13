/*
NETWORK_SERVER_MANAGER_INTERNAL.H

Private networking declarations shared by the server manager and its message
handler.  Keep these out of the widely included public manager header: the
January compiler is sensitive to declaration position even in unrelated code.
*/

#ifndef __NETWORK_SERVER_MANAGER_INTERNAL_H
#define __NETWORK_SERVER_MANAGER_INTERNAL_H
#pragma once

/* ---------- headers */

#include "cseries.h"
#include "networking/network_server_manager.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct message_client_game_update;
struct network_connection;
struct network_game;
struct network_game_server;
struct network_game_server_client_machine;
struct network_machine;
struct network_player;

/* ---------- prototypes/NETWORK_SERVER_MANAGER.C */

word network_game_server_get_state(
	struct network_game_server *server,
	short *substate);
boolean network_game_server_game_is_open(
	struct network_game_server *server);
boolean network_game_server_game_is_valid(
	struct network_game_server *server);
boolean network_game_server_client_machine_is_joined_to_game(
	struct network_game_server *server,
	struct network_game_server_client_machine *machine);
boolean network_game_server_accept_client_machine_into_game(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine);
boolean network_game_server_add_player_to_game(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	struct network_player *player);
boolean network_game_server_remove_player_from_game(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	struct network_player *player);
boolean network_game_server_adjust_machine_settings(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	struct network_machine *machine_settings);
void network_game_server_client_machine_game_loading_complete(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine);
void network_game_server_client_machine_is_precached(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	char const *map_name);
void network_game_server_handle_client_update_packet(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	struct message_client_game_update *game_update);
boolean network_game_server_switch_machine_from_postgame_to_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine);
void network_game_server_queue_player_for_addition(
	struct network_game_server *server,
	struct network_player *player);
struct network_machine *network_game_server_get_client_machine(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	long *machine_index);
struct network_connection *network_game_server_get_connection(
	struct network_game_server *server);
struct network_connection *network_game_server_get_client_connection(
	struct network_game_server_client_machine *client_machine);
struct network_connection *network_game_server_get_machine_connection(
	struct network_game_server *server,
	struct network_machine *machine);
struct network_game_server_client_machine *network_game_server_get_client_machine_at_index(
	struct network_game_server *server,
	long index);
struct network_game_server_client_machine *network_game_server_get_client_machine_at_address(
	struct network_game_server *server,
	unsigned long address);
struct network_game *network_game_server_get_game(
	struct network_game_server *server);
boolean network_game_server_remove_machine_from_game(
	struct network_game_server *server,
	struct network_machine *machine);
void network_game_server_update_countdown(
	struct network_game_server *server,
	short countdown_event);
/* ---------- globals */

/* ---------- public code */

#endif // __NETWORK_SERVER_MANAGER_INTERNAL_H
