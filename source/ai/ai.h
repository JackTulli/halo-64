/*
AI.H

header included in hcex build.
*/

#ifndef __AI_H
#define __AI_H
#pragma once

/* ---------- constants */

enum
{
	_ai_line_of_sight_clear = 0,
	_ai_line_of_sight_occluded,
	_ai_line_of_sight_from_cover,
	_ai_line_of_sight_to_cover,
	_ai_line_of_sight_obstructed,
	NUMBER_OF_AI_LINE_OF_SIGHTS,
};

enum
{
	_ai_line_of_sight_normal = 0,
	_ai_line_of_sight_expand_source,
	_ai_line_of_sight_expand_target,
	NUMBER_OF_AI_LINE_OF_SIGHT_MODES,
};

/* ---------- macros */

/* ---------- structures */

struct ai_unit_information_look_data
{
	long unit_index;
};

struct ai_object_information_look_data
{
	long object_index;
};

struct ai_information_look_data
{
	union
	{
		struct ai_unit_information_look_data unit;
		struct ai_object_information_look_data object;
	};
};

struct ai_allegiance_information_data
{
	short team1_index;
	short team2_index;
	boolean broken;
};

struct ai_combat_stimulus_information_data
{
	word pad;
};

struct ai_target_knowledge_information_data
{
	long prop_index;
};

struct ai_information_data
{
	union
	{
		struct ai_allegiance_information_data allegiance;
		struct ai_combat_stimulus_information_data combat_stimulus;
		struct ai_target_knowledge_information_data target_knowledge;
	};
};


struct ai_information_packet
{
	long target_unit_index;
	short communication_type;
	short dialogue_type_index;
	short damage_category;
	boolean updated_dialogue_timers;
	short look_priority;
	short look_type;
	struct ai_information_look_data look_data;
	short information_type;
	struct ai_information_data information_data;
};

/* ---------- prototypes/AI.C */

void ai_initialize(
	void);
void ai_initialize_for_new_map(
	void);
void ai_place(
	void);
void ai_update(
	void);
void ai_disconnect_from_structure_bsp(
	void);
void ai_reconnect_to_structure_bsp(
	void);
void ai_globals_dialogue_triggers_enabled(
	boolean enabled);
void ai_update_team_status(
	void);
long ai_get_race_from_team_index(
	short team_index);
void ai_get_major_upgrade_chance(
	short major_upgrade,
	boolean *upgrade_major,
	boolean *random,
	real *chance);
boolean ai_consider_major_upgrade(
	long encounter_index,
	short squad_index,
	real chance);

boolean ai_enemies_attacking_player(
	void);
boolean ai_enemies_can_see_player(
	void);
boolean ai_try_vehicle_eviction(
	long actor_index,
	long entering_unit_index,
	boolean immediate);
boolean ai_test_ballistic_line_of_fire(
	long actor_index,
	union real_point3d const *origin,
	real ticks,
	union real_vector3d const *velocity,
	real gravity,
	long ignore_object_index,
	boolean in_vehicle);
boolean ai_test_line_of_fire(
	long actor_index,
	long ignore_unit_index,
	union real_point3d const *origin,
	union real_vector3d const *vector,
	long *blocking_prop_index_reference);
short ai_test_line_of_sight(
	union real_point3d const *point0,
	short cluster0,
	union real_point3d const *point1,
	short cluster1,
	short mode,
	boolean test_line_of_fire,
	long ignore_object_index,
	boolean ignore_vehicles);
void ai_handle_editing(
	long encounter_index);
void ai_handle_spatial_effect(
	long object_index,
	union real_point3d const *position,
	short effect_type,
	short volume,
	short count);

boolean ai_release_inactive_swarms(
	char *result_description,
	boolean *more_to_release,
	byte *working_memory,
	short working_memory_size);
void ai_find_inactive_encounters(
	byte *working_memory,
	short working_memory_size);
boolean ai_release_inactive_encounters(
	char *result_description,
	boolean *more_to_release,
	byte *working_memory,
	short working_memory_size);

void ai_handle_damage(
	long unit_index,
	long shooter_object_index,
	short damage_category,
	real fraction,
	union real_vector3d *damage_velocity,
	boolean delayed);
void ai_handle_enter_vehicle(
	long unit_index,
	long vehicle_index);
void ai_handle_exit_vehicle(
	long unit_index,
	long vehicle_index);
/* ---------- globals */

void ai_index_to_string(
	long ai_index,
	struct scenario *scenario,
	char *buffer,
	long size);

struct damage_data;

boolean ai_adjust_damage(
	long actor_index,
	struct damage_data *damage,
	real *total_damage);

#endif // __AI_H
