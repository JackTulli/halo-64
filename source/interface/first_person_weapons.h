/*
FIRST_PERSON_WEAPONS.H

header included in hcex build.
*/

#ifndef __FIRST_PERSON_WEAPONS_H
#define __FIRST_PERSON_WEAPONS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct real_matrix4x3;
struct animation_state;
struct object_marker;

/* ---------- prototypes/FIRST_PERSON_WEAPONS.C */

short animation_update_render_only(
	long animation_graph_index,
	struct animation_state *state,
	long *sound_index);
void first_person_weapons_initialize(
	void);
void first_person_weapons_dispose(
	void);
void first_person_weapons_initialize_for_new_map(
	void);
void first_person_weapons_dispose_from_old_map(
	void);
void first_person_weapons_update(
	void);
long first_person_weapon_get_local_index(
	long weapon_index);
void first_person_weapon_draw(
	void);
short first_person_weapon_get_marker_by_name(
	long weapon_index,
	char const *name,
	struct object_marker *markers,
	short maximum_marker_count);
void first_person_weapon_render_update(
	void);
short first_person_weapon_get_marker_by_name_render(
	long weapon_index,
	char const *name,
	struct object_marker *markers,
	short maximum_marker_count);
void first_person_weapon_center_flashlight(
	long unit_index,
	real_point3d *position,
	real_vector3d *forward,
	real_vector3d *up);
boolean first_person_weapon_adjust_light(
	long weapon_index,
	char const *marker_name,
	real_point3d *position,
	real_vector3d *forward,
	real_vector3d *up);
void first_person_weapon_message_from_unit(
	long unit_index,
	short message_type);
void first_person_weapon_message_from_weapon(
	long weapon_index,
	short message_type);
struct real_matrix4x3 *first_person_weapon_get_node_matrix(
	short local_player_index,
	short node_index);

/* ---------- globals */

/* ---------- public code */

#endif // __FIRST_PERSON_WEAPONS_H
