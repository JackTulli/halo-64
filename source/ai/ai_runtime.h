/*
AI_RUNTIME.H
*/

#ifndef __AI_RUNTIME_H
#define __AI_RUNTIME_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- prototypes/AI.C */

void ai_dispose(
	void);
void ai_dispose_from_old_map(
	void);
void ai_globals_ai_active(
	boolean enabled);
void ai_globals_grenades_enabled(
	boolean enabled);
boolean ai_enemies_endanger_player(
	boolean must_be_attacking);
long ai_get_responsible_unit(
	long object_index,
	boolean responsible_for_weapon_fire);
void ai_handle_bump(
	long unit_index,
	long object_index,
	union real_vector3d const *old_velocity);
void ai_handle_death(
	long unit_index,
	long killer_object_index,
	short damage_category);
boolean ai_handle_killing_spree(
	long unit_index,
	short killing_spree_count);
void ai_handle_allegiance_broken_notification(
	short team1_index,
	short team2_index,
	boolean broken);
void ai_handle_allegiance_status_changed(
	short team1_index,
	short team2_index,
	boolean currently_broken,
	boolean permanently_broken);
void ai_create_mounted_weapons_for_unit(
	long unit_index);
void ai_handle_deleted_object(
	long object_index);
void ai_handle_unit_effect(
	long unit_index,
	short effect_type,
	short volume);
void ai_flush_spatial_effects(
	void);
void ai_erase(
	long encounter_index,
	long platoon_index,
	long squad_index,
	boolean immediate);

#endif // __AI_RUNTIME_H
