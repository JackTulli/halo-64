/*
AI_COMMUNICATION.H

header included in hcex build.
*/

#ifndef __AI_COMMUNICATION_H
#define __AI_COMMUNICATION_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- constants */

enum ai_communication_type
{
	_ai_communication_death = 0,
	_ai_communication_killing_spree,
	_ai_communication_hurt,
	_ai_communication_damage,
	_ai_communication_sighted_enemy,
	_ai_communication_found_enemy,
	_ai_communication_unexpected_enemy,
	_ai_communication_found_dead_friend,
	_ai_communication_allegiance_changed,
	_ai_communication_grenade_throwing,
	_ai_communication_grenade_startle,
	_ai_communication_grenade_sighted,
	_ai_communication_grenade_danger,
	_ai_communication_lost_contact,
	_ai_communication_blocked,
	_ai_communication_alert_noncombat,
	_ai_communication_search_start,
	_ai_communication_search_query,
	_ai_communication_search_report,
	_ai_communication_search_abandon,
	_ai_communication_search_group_abandon,
	_ai_communication_uncover_start,
	_ai_communication_advance,
	_ai_communication_retreat,
	_ai_communication_cover,
	_ai_communication_sighted_friend_player,
	_ai_communication_shooting,
	_ai_communication_shooting_vehicle,
	_ai_communication_shooting_berserk,
	_ai_communication_shooting_group,
	_ai_communication_shooting_traitor,
	_ai_communication_flee,
	_ai_communication_flee_leader_died,
	_ai_communication_flee_idle,
	_ai_communication_attempted_flee,
	_ai_communication_hiding_finished,
	_ai_communication_vehicle_entry,
	_ai_communication_vehicle_exit,
	_ai_communication_vehicle_woohoo,
	_ai_communication_vehicle_scared,
	_ai_communication_vehicle_falling,
	_ai_communication_surprise,
	_ai_communication_berserk,
	_ai_communication_melee,
	_ai_communication_dive,
	_ai_communication_uncover_exclamation,
	_ai_communication_falling_to_death,
	_ai_communication_leap,
	_ai_communication_postcombat_alone,
	_ai_communication_postcombat_unscathed,
	_ai_communication_postcombat_wounded,
	_ai_communication_postcombat_massacre,
	_ai_communication_postcombat_triumph,
	_ai_communication_postcombat_check_enemy,
	_ai_communication_postcombat_check_friend,
	_ai_communication_postcombat_shoot_corpse,
	_ai_communication_postcombat_celebrate,
	NUMBER_OF_AI_COMMUNICATION_TYPES,
};

enum ai_communication_hostility
{
	_comm_hostility_none = 0,
	_comm_hostility_self,
	_comm_hostility_friend,
	_comm_hostility_enemy,
	_comm_hostility_traitor,
	NUMBER_OF_AI_COMMUNICATION_HOSTILITIES,
};

/* ---------- macros */

#define ai_conversation_header_get(index) \
	((struct ai_conversation_datum_header *)datum_get(conversation_data, (index)))

/* ---------- structures */

struct ai_information_data;
struct ai_information_packet;

/* The independently mapped prefix of the 0x64-byte conversation datum. */
struct ai_conversation_datum_header
{
	short identifier;
	short scenario_conversation_index;
	boolean scripted;
	boolean any_line_spoken;
	boolean begun;
	boolean finished;
	boolean waiting_to_advance;
	boolean told_to_advance;
	byte reserved0A[2];
	long creation_time;
	long triggering_player_unit_index;
};

/* ---------- prototypes/AI_COMMUNICATION.C */

void ai_communication_initialize(
	void);
void ai_communication_initialize_for_new_map(
	void);
void ai_communication_dispose(
	void);
void ai_communication_dispose_from_old_map(
	void);
void ai_communication_packet_new(
	struct ai_information_packet *information);
void ai_communication_started(
	long unit_index,
	short priority,
	short vocalization_type,
	struct ai_information_packet *information);
void ai_communication_notify(
	long unit_index,
	short priority,
	short vocalization_type,
	struct ai_information_packet *information);
void ai_communication_finished(
	long unit_index,
	short priority,
	short vocalization_type,
	boolean reply_to_player,
	long preselected_reply_actor_index,
	struct ai_information_packet *information);
void actor_communication_update(
	long actor_index);
void ai_conversation_update(
	void);
void actor_handle_communication(
	long actor_index,
	long prop_index,
	struct ai_information_packet *information);
short actor_communication_team(
	long actor_index);
short ai_conversation_status(
	short scenario_conversation_index);
boolean ai_conversation(
	short scenario_conversation_index,
	boolean scripted);
long ai_conversation_new(
	short scenario_conversation_index,
	boolean scripted);
short ai_conversation_line(
	short scenario_conversation_index);
void ai_conversation_advance(
	short scenario_conversation_index);
void ai_conversation_finish(
	long conversation_index,
	boolean abort,
	boolean force);
void ai_conversation_stop(
	short scenario_conversation_index);
void ai_conversation_actor_deleted(
	long actor_index);
real ai_communication_get_player_rating(
	long unit_index,
	boolean test_line_of_sight,
	long *unit_index_reference,
	real *distance_reference);
short ai_communication_get_type_by_name(
	char const *name);
void ai_conversation_unit_died(
	long unit_index,
	boolean deleted);
void ai_communication_event(
	short communication_type,
	long subject_unit_index,
	long cause_unit_index,
	short hostility,
	short damage_type,
	short information_type,
	struct ai_information_data *information_data);

/* ---------- globals */

extern struct data_array *conversation_data;

/* ---------- public code */

#endif // __AI_COMMUNICATION_H
