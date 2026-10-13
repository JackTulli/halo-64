/*
AI_PROFILE.H

header included in hcex build.
*/

#ifndef __AI_PROFILE_H
#define __AI_PROFILE_H
#pragma once

#include "cseries/cseries.h"

/* ---------- constants */

enum
{
	AI_METER_HISTORY_TICKS = 60,
	NUMBER_OF_AI_METERS = 28,
};

enum
{
	_ai_meter_encounters = 0,
	_ai_meter_encounters_updated,
	_ai_meter_actors,
	_ai_meter_actors_updated,
	_ai_meter_actors_active,
	_ai_meter_units,
	_ai_meter_units_updated,
	_ai_meter_units_active,
	_ai_meter_props,
	_ai_meter_dead_props_acknowledged,
	_ai_meter_dead_props_orphaned,
	_ai_meter_dead_props_unacknowledged,
	_ai_meter_enemy_props_acknowledged,
	_ai_meter_enemy_props_orphaned,
	_ai_meter_enemy_props_unacknowledged,
	_ai_meter_friendly_props_acknowledged,
	_ai_meter_friendly_props_orphaned,
	_ai_meter_friendly_props_unacknowledged,
	_ai_meter_swarm_actors,
	_ai_meter_swarms,
	_ai_meter_swarm_components,
	_ai_meter_collisions,
	_ai_meter_line_of_sight,
	_ai_meter_line_of_fire,
	_ai_meter_path_flood,
	_ai_meter_path_find,
	_ai_meter_action_change,
	_ai_meter_firing_point,
};

/* ---------- macros */

/* ---------- structures */

struct ai_meter
{
	short accumulator;
	short current_value;
	real average;
	long history_sum;
	short history_next_index;
	short history_count;
	short history[AI_METER_HISTORY_TICKS];
};

struct ai_profile_globals
{
	boolean disabled;
	boolean move_actors_randomly;
	short render_spray;
	boolean show;
	boolean show_stats;
	boolean show_actors;
	boolean show_swarms;
	boolean show_paths;
	boolean show_line_of_sight;
	boolean show_prop_types;
	boolean show_sound_distance;
	struct ai_meter meters[NUMBER_OF_AI_METERS];
};

/* ---------- prototypes/AI_PROFILE.C */

void ai_profile_initialize(
	void);
void ai_profile_initialize_for_new_map(
	void);

void ai_profile_dispose(
	void);
void ai_profile_dispose_from_old_map(
	void);
void ai_profile_update(
	void);
void ai_profile_display(
	char *buffer);
void ai_profile_render(
	void);
short ai_profile_change_render_spray(
	void);

/* ---------- globals */

extern struct ai_profile_globals ai_profile;

/* ---------- public code */

#endif // __AI_PROFILE_H
