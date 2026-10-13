/*
PLAYER_QUEUES_NEW.H
*/

#ifndef __PLAYER_QUEUES_NEW_H
#define __PLAYER_QUEUES_NEW_H
#pragma once

/* ---------- structures */

struct player_action_collection;
struct player_action;
struct server_update;

/* ---------- public code */

boolean update_server_new(
	void);
void update_server_delete(
	void);
void update_server_start(
	void);
void update_server_add_player(
	long player_index);
void update_server_next_update(
	void);
void update_server_build_server_update(
	long machine_index,
	struct server_update *update,
	long *update_number);
void update_server_handle_client_update(
	long machine_index,
	struct player_action *actions);

boolean update_client_new(
	void);
void update_client_delete(
	void);
void update_client_start(
	void);
void update_client_add_player(
	long player_index);
void update_client_queue(
	struct player_action const *action);
void update_client_queue_push(
	void);
boolean update_client_dequeue(
	struct player_action *actions);
long update_client_get_maximum_actions(
	void);
long update_client_get_maximum_possible_server_time(
	void);
void update_client_local_ticks(
	short ticks);
void update_client_build_client_update(
	struct player_action_collection *action_collection);
void update_client_handle_server_update(
	struct server_update *update,
	long update_number);

void update_queues_reset_and_fill_with_lies(
	void);
long player_new_queue(
	long player_index);

#endif /* __PLAYER_QUEUES_NEW_H */
