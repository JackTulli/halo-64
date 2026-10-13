#ifndef __GAME_ALLEGIANCE_H
#define __GAME_ALLEGIANCE_H
#pragma once

/* ---------- constants */

enum
{
	_allegiance_incident_accident = 0,
	_allegiance_incident_betrayal,
	_allegiance_incident_forgive
};

/* ---------- prototypes/GAME_ALLEGIANCE.C */

void game_allegiance_initialize(
	void);
void game_allegiance_dispose(
	void);
void game_allegiance_initialize_for_new_map(
	void);
void game_allegiance_dispose_from_old_map(
	void);
void game_allegiance_update(
	void);
void game_allegiance_create(
	short team1_index,
	boolean team1_suspicious,
	short team2_index,
	boolean team2_suspicious,
	short incident_threshold,
	short incident_decay_time,
	boolean requires_communication);
boolean game_team_is_enemy(
	short team_index0,
	short team_index1);
boolean game_team_is_ally(
	short our_team_index,
	short other_team_index);
short game_allegiance_get_incidents(
	short our_team_index,
	short other_team_index,
	short *incident_threshold);
void game_allegiance_provoke(
	short team_index0,
	short team_index1);
void game_allegiance_notify_change(
	short team1_index,
	short team2_index);
boolean game_allegiance_remove(
	short team1_index,
	short team2_index);
boolean game_allegiance_incident(
	short aggressor_team_index,
	short victim_team_index,
	short incident_type,
	boolean *notify_immediately);

/* ---------- globals */

extern char const *global_game_team_names[];

#endif // __GAME_ALLEGIANCE_H
