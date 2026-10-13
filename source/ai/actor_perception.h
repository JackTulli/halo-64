/*
ACTOR_PERCEPTION.H

header included in hcex build.
*/

#ifndef __ACTOR_PERCEPTION_H
#define __ACTOR_PERCEPTION_H
#pragma once

/* ---------- prototypes/ACTOR_PERCEPTION.C */

struct actor_position_data;
union real_point3d;

short actor_get_perception_knowledge(
	long actor_index,
	long prop_index);
short actor_visibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	union real_point3d const *target_position,
	char lighting,
	short line_of_sight,
	boolean use_maximum_distance,
	boolean target_is_player,
	short perception_knowledge);

void actor_situation_update_target_status(
	long actor_index);
void actor_situation_combat_status_update(
	long actor_index);
boolean actor_situation_try_new_target(
	long actor_index,
	long prop_index);
boolean actor_perception_friend_prop_is_attacking(
	long actor_index,
	long friend_prop_index,
	real_vector3d *attack_vector);
void actor_perception_unreachable(
	long actor_index,
	long prop_index,
	boolean unreachable);

#endif // __ACTOR_PERCEPTION_H
