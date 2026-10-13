/*
ACTOR_STIMULUS.C

symbols in this file:
00024E70 0030:
	_actor_stimulus_clear (0000)
00024EA0 00d0:
	_actor_stimulus_combat (0000)
00024F70 0060:
	_actor_stimulus_surprise (0000)
00024FD0 0060:
	_actor_stimulus_suspicion (0000)
00025030 0070:
	_actor_stimulus_prop_sighted (0000)
000250A0 0080:
	_actor_stimulus_enter_combat_found_body (0000)
00025120 0040:
	_actor_stimulus_enter_combat_perceived_enemy (0000)
00025160 0080:
	_actor_stimulus_enter_combat_friend_in_combat (0000)
000251E0 0030:
	_actor_stimulus_bumped (0000)
00025210 0050:
	_actor_stimulus_environmental_noise (0000)
00025260 0150:
	_actor_stimulus_heard_shooting (0000)
000253B0 0020:
	_actor_stimulus_was_surprised (0000)
000253D0 0070:
	_actor_stimulus_maneuvering (0000)
00025440 0020:
	_actor_stimulus_vehicle_eviction (0000)
00025460 00d0:
	_actor_stimulus_abandon_stationary_facing (0000)
00025530 0320:
	_actor_stimulus_prop_acknowledged (0000)
00025850 0170:
	_actor_stimulus_damage (0000)
000259C0 0140:
	_actor_stimulus_weapon_impact (0000)
00025B00 0140:
	_actor_stimulus_weapon_detonation (0000)
00025C40 01a0:
	_actor_stimulus_prop_just_killed (0000)
00025DE0 0110:
	_actor_stimulus_prop_fleeing (0000)
00025EF0 0110:
	_actor_stimulus_noticed_danger_zone (0000)
002462D4 0038:
	??_C@_0DI@JCJNLAKP@?$CFs?5?$CFd?3?5surprise?5?$CFs?3?5?$CFs?5?$CFsexp?5?$CFs?5@ (0000)
0024630C 0007:
	??_C@_06NMPBKCIA@noncom?$AA@ (0000)
00246314 0006:
	??_C@_05HJNNLMDJ@front?$AA@ (0000)
0024631C 0006:
	??_C@_05ILIIOMHH@flank?$AA@ (0000)
00246324 0004:
	??_C@_03NJKJADM@not?$AA@ (0000)
00246328 0004:
	??_C@_03JEOKDCFP@far?$AA@ (0000)
0024632C 0006:
	??_C@_05LBOHBHFK@close?$AA@ (0000)
00246334 0013:
	??_C@_0BD@PCBBJBIG@unexp?9behind?9shoot?$AA@ (0000)
00246348 0012:
	??_C@_0BC@IGIPMMMB@unexp?9close?9shoot?$AA@ (0000)
0024635C 0008:
	??_C@_07JBPBHDIL@grenade?$AA@ (0000)
00246364 000c:
	??_C@_0M@EODEKNJA@enemy?9close?$AA@ (0000)
00246370 000c:
	??_C@_0M@POIHHAMP@enemy?9shoot?$AA@ (0000)
0024637C 0023:
	??_C@_0CD@LMNLJKDA@c?3?2halo?2SOURCE?2ai?2actor_stimulus@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "ai/ai_communication.h"
#include "ai/ai_debug.h"
#include "ai/actors.h"
#include "ai/actor_definitions.h"
#include "ai/actor_types.h"
#include "ai/props.h"

#include "game/cheats.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/players.h"

#include "main/console.h"

#include "memory/data.h"

#include "units/units.h"

#include <stddef.h>

/* ---------- constants */

enum
{
	_actor_mode_asleep = 1,
	_actor_mode_combat = 3,
};

enum
{
	_actor_combat_status_investigate = 2,
	_actor_combat_status_definite = 3,
	_actor_combat_status_certain = 4,
	_actor_combat_status_clear_los = 5,
};

enum
{
	_actor_stimulus_combat_friend = 1,
	_actor_stimulus_combat_body = 2,
	_actor_stimulus_combat_impact = 3,
	_actor_stimulus_combat_danger = 4,
	_actor_stimulus_combat_damage = 5,
	_actor_stimulus_combat_enemy = 6,
};

enum
{
	_actor_surprise_none = 0,
	_actor_surprise_unprepared_enemy_shooting = 1,
	_actor_surprise_unprepared_weapon_impact_close = 2,
	_actor_surprise_unprepared_enemy_close = 3,
	_actor_surprise_unprepared_grenade = 4,
	_actor_surprise_unprepared_damage = 5,
	_actor_surprise_unexpected_enemy_close_shooting = 6,
	_actor_surprise_unexpected_enemy_close_flanked_shooting = 7,
	NUMBER_OF_ACTOR_SURPRISE_TYPES,
};

enum
{
	_actor_panic_friend_fleeing = 2,
	_actor_panic_friend_same_type_killed = 3,
	_actor_panic_platoon_retreating = 6,
	_actor_panic_friend_leader_type_killed = 8,
};

enum
{
	_actor_definition_flags2_panic_in_groups_bit = 5,
};

enum
{
	_prop_facing_central = 2,
};

/* ---------- macros */

#define prop_acknowledged(prop) \
	((prop)->state >= _prop_state_becoming_unacknowledged && \
		(prop)->state <= _prop_state_acknowledged)

/* ---------- structures */

typedef char actor_stimulus_data_size_assert[
	sizeof(struct actor_stimulus_data) == 0x64 ? 1 : -1];
typedef char actor_stimuli_offset_assert[
	offsetof(struct actor_datum, stimuli) == 0x2EC ? 1 : -1];
typedef char actor_stimulus_vehicle_eviction_offset_assert[
	offsetof(struct actor_datum, stimuli.vehicle_eviction) == 0x2ED ? 1 : -1];
typedef char actor_stimulus_was_surprised_offset_assert[
	offsetof(struct actor_datum, stimuli.was_surprised) == 0x2F0 ? 1 : -1];
typedef char actor_stimulus_suspicion_combat_status_offset_assert[
	offsetof(struct actor_datum, stimuli.suspicion_combat_status) == 0x34A ? 1 : -1];
typedef char actor_stimulus_suspicion_timer_offset_assert[
	offsetof(struct actor_datum, stimuli.suspicion_timer) == 0x34C ? 1 : -1];
typedef char actor_stimulus_actor_mode_offset_assert[
	offsetof(struct actor_datum, state.mode) == 0x6A ? 1 : -1];
typedef char actor_stimulus_actor_unit_index_offset_assert[
	offsetof(struct actor_datum, meta.unit_index) == 0x18 ? 1 : -1];
typedef char actor_stimulus_panic_type_offset_assert[
	offsetof(struct actor_datum, stimuli.panic_type) == 0x308 ? 1 : -1];
typedef char actor_stimulus_panic_prop_index_offset_assert[
	offsetof(struct actor_datum, stimuli.panic_prop_index) == 0x30C ? 1 : -1];
typedef char actor_stimulus_direction_size_assert[
	sizeof(struct direction_specification) == 0x10 ? 1 : -1];
typedef char actor_stimulus_direction_prop_index_offset_assert[
	offsetof(struct direction_specification, prop_index) == 0x4 ? 1 : -1];
typedef char actor_stimulus_direction_point_offset_assert[
	offsetof(struct direction_specification, point) == 0x4 ? 1 : -1];
typedef char actor_stimulus_prop_unit_index_offset_assert[
	offsetof(struct prop_datum, unit_index) == 0x18 ? 1 : -1];
typedef char actor_stimulus_prop_enemy_offset_assert[
	offsetof(struct prop_datum, enemy) == 0x60 ? 1 : -1];
typedef char actor_stimulus_prop_dead_offset_assert[
	offsetof(struct prop_datum, dead) == 0x127 ? 1 : -1];

/* ---------- globals */

/* ---------- public code */

void actor_stimulus_clear(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	csmemset(&actor->stimuli, 0, sizeof(actor->stimuli));
	return;
}

static void actor_stimulus_combat(
	long actor_index,
	short transition_type,
	real_point3d const *guard_point,
	long guard_point_surface_index,
	real guard_distance,
	long guard_timer,
	real_vector3d const *transition_vector,
	long prop_index,
	long prop_look_timer,
	boolean prop_look_while_moving)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->state.mode >= _actor_mode_combat)
	{
		return;
	}
	if (transition_type < actor->stimuli.combat_transition)
	{
		return;
	}

	actor->stimuli.combat_transition = transition_type;
	if (!guard_point)
	{
		actor->stimuli.combat_transition_guard_at_point = FALSE;
	}
	else
	{
		actor->stimuli.combat_transition_guard_at_point = TRUE;
		actor->stimuli.combat_transition_guard_point = *guard_point;
		actor->stimuli.combat_transition_guard_point_surface_index =
			guard_point_surface_index;
		actor->stimuli.combat_transition_guard_point_distance = guard_distance;
	}

	if (!transition_vector)
	{
		actor->stimuli.combat_transition_has_vector = FALSE;
	}
	else
	{
		actor->stimuli.combat_transition_has_vector = TRUE;
		actor->stimuli.combat_transition_vector = *transition_vector;
	}

	actor->stimuli.combat_transition_prop_index = prop_index;
	actor->stimuli.combat_transition_prop_look_timer = prop_look_timer;
	actor->stimuli.combat_transition_prop_look_while_moving =
		prop_look_while_moving;
	actor->stimuli.combat_transition_guard_timer = guard_timer;

	return;
}

void actor_stimulus_surprise(
	long actor_index,
	short surprise_level,
	long prop_index,
	real_vector3d const *surprise_vector)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (surprise_level > actor->stimuli.surprise_level)
	{
		actor->stimuli.surprise_level = surprise_level;
		actor->stimuli.surprise_prop_index = prop_index;
		if (!surprise_vector)
		{
			actor->stimuli.surprise_has_vector = FALSE;
			return;
		}

		actor->stimuli.surprise_has_vector = TRUE;
		actor->stimuli.surprise_vector = *surprise_vector;
	}

	return;
}

void actor_stimulus_suspicion(
	long actor_index,
	short suspicion_combat_status,
	long suspicion_timer)
{
	struct actor_datum *actor = actor_get(actor_index);
	short current_status = actor->stimuli.suspicion_combat_status;

	if (current_status < suspicion_combat_status)
	{
		actor->stimuli.suspicion_combat_status = suspicion_combat_status;
		actor->stimuli.suspicion_timer = suspicion_timer;
	}
	else if (current_status == suspicion_combat_status)
	{
		actor->stimuli.suspicion_timer = MAX(
			actor->stimuli.suspicion_timer,
			suspicion_timer);
	}

	return;
}

void actor_stimulus_enter_combat_found_body(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);

	actor->emotions.corpse_ignore_time = game_time_get();
	actor_perception_find_prop_pathfinding_location(actor_index, prop_index);
	actor_stimulus_combat(
		actor_index,
		_actor_stimulus_combat_body,
		&prop->pathfinding_point,
		prop->pathfinding_surface_index,
		1.5f,
		90,
		NULL,
		prop_index,
		90,
		TRUE);

	return;
}

void actor_stimulus_enter_combat_perceived_enemy(
	long actor_index,
	long prop_index)
{
	struct prop_datum *prop = prop_get(prop_index);

	actor_stimulus_combat(
		actor_index,
		_actor_stimulus_combat_enemy,
		NULL,
		NONE,
		0.0f,
		90,
		&prop->actor_to_prop,
		prop_index,
		150,
		FALSE);

	return;
}

void actor_stimulus_enter_combat_friend_in_combat(
	long actor_index,
	long prop_index)
{
	struct prop_datum *prop = prop_get(prop_index);

	actor_stimulus_combat(
		actor_index,
		_actor_stimulus_combat_friend,
		NULL,
		NONE,
		0.0f,
		90,
		&prop->actor_to_prop,
		prop_index,
		150,
		FALSE);

	if (prop->actor_index != NONE)
	{
		struct actor_datum *friend_actor = actor_get(prop->actor_index);

		if (friend_actor->state.suspicion_combat_status > 0)
		{
			actor_stimulus_suspicion(
				actor_index,
				friend_actor->state.suspicion_combat_status,
				450);
		}
	}

	return;
}

void actor_stimulus_noticed_danger_zone(
	long actor_index,
	short danger_type,
	short danger_hostility,
	long danger_object_index,
	real_point3d const *position)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	long unit_index = actor->meta.unit_index;
	real_vector3d vector;
	real distance;

	if (unit_index == NONE)
	{
		return;
	}

	if (danger_type == _actor_danger_zone_projectile)
	{
		long communication_hostility = NONE;

		switch (danger_hostility)
		{
		case _actor_danger_hostility_enemy:
			communication_hostility = _comm_hostility_enemy;
			break;
		case _actor_danger_hostility_friend:
			communication_hostility = _comm_hostility_friend;
			break;
		case _actor_danger_hostility_self:
			communication_hostility = _comm_hostility_self;
			break;
		}

		ai_communication_event(
			_ai_communication_grenade_sighted,
			unit_index,
			NONE,
			communication_hostility,
			NONE,
			NONE,
			NULL);
	}

	distance = normalize3d(vector_from_points3d(
		&actor->input.position.head_position,
		position,
		&vector));
	if (actor->state.mode < _actor_mode_combat &&
		distance < definition->panic.surprise_distance &&
		danger_type == _actor_danger_zone_projectile)
	{
		actor_stimulus_surprise(
			actor_index,
			_actor_surprise_unprepared_grenade,
			NONE,
			&vector);
	}

	actor_stimulus_combat(
		actor_index,
		_actor_stimulus_combat_danger,
		NULL,
		NONE,
		0.0f,
		0,
		&vector,
		NONE,
		0,
		FALSE);

	return;
}

void actor_stimulus_weapon_impact(
	long actor_index,
	long object_index,
	real_point3d const *position,
	short count)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);

	if (actor->danger_zone.danger_type > _actor_danger_zone_none &&
		actor->danger_zone.object_index == object_index &&
		actor->danger_zone.acknowledgement_timer > 0)
	{
		ai_communication_event(
			_ai_communication_grenade_startle,
			actor->meta.unit_index,
			NONE,
			NONE,
			NONE,
			NONE,
			NULL);
	}
	else
	{
		real_vector3d surprise_vector;
		real distance = normalize3d(vector_from_points3d(
			&actor->input.position.head_position,
			position,
			&surprise_vector));

		if (fabs(distance) < _real_epsilon)
		{
			surprise_vector = actor->input.facing_vector;
		}

		if (actor->state.mode < _actor_mode_combat &&
			distance < definition->panic.surprise_distance)
		{
			actor_stimulus_surprise(
				actor_index,
				_actor_surprise_unprepared_weapon_impact_close,
				NONE,
				&surprise_vector);
		}

		actor_stimulus_combat(
			actor_index,
			_actor_stimulus_combat_impact,
			NULL,
			NONE,
			0.0f,
			90,
			&surprise_vector,
			NONE,
			0,
			FALSE);
	}

	{
		struct direction_specification direction;

		direction.type = _direction_specification_point;
		direction.point = *position;
		actor_look_secondary(
			actor_index,
			_secondary_look_weapon_impact,
			_secondary_look_priority_default,
			&direction);
	}

	return;
}

void actor_stimulus_weapon_detonation(
	long actor_index,
	long object_index,
	real_point3d const *position,
	short count)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);

	{
		real_vector3d surprise_vector;
		real distance = normalize3d(vector_from_points3d(
			&actor->input.position.head_position,
			position,
			&surprise_vector));

		if (fabs(distance) < _real_epsilon)
		{
			surprise_vector = actor->input.facing_vector;
		}

		if (actor->state.mode < _actor_mode_combat &&
			distance < definition->panic.surprise_distance)
		{
			actor_stimulus_surprise(
				actor_index,
				_actor_surprise_unprepared_grenade,
				NONE,
				&surprise_vector);
		}

		actor_stimulus_combat(
			actor_index,
			_actor_stimulus_combat_impact,
			NULL,
			NONE,
			0.0f,
			90,
			&surprise_vector,
			NONE,
			0,
			FALSE);
	}

	if (object_index != NONE &&
		game_team_is_enemy(
			(word)actor->meta.team_index,
			object_get(object_index)->object.owner_team_index))
	{
		actor_stimulus_suspicion(
			actor_index,
			_actor_combat_status_investigate,
			900);
	}

	{
		struct direction_specification direction;

		direction.type = _direction_specification_point;
		direction.point = *position;
		actor_look_secondary(
			actor_index,
			_secondary_look_detonation,
			_secondary_look_priority_default,
			&direction);
	}

	return;
}

void actor_stimulus_damage(
	long actor_index,
	long prop_index,
	real damage_fraction,
	real_vector3d const *damage_velocity)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = NULL;
	real_vector3d const *surprise_vector = NULL;
	boolean have_recoil_direction = FALSE;
	real_vector3d recoil_direction;
	struct direction_specification direction;

	if (prop_index != NONE)
	{
		prop = prop_get(prop_index);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\actor_stimulus.c",
			0x154,
			prop_acknowledged(prop));
		surprise_vector = &prop->actor_to_prop;
	}
	else if (damage_velocity)
	{
		real magnitude_squared = magnitude_squared3d(damage_velocity);

		if (magnitude_squared > 0.25f)
		{
			scale_vector3d(
				damage_velocity,
				-1.0f / square_root(magnitude_squared),
				&recoil_direction);
			surprise_vector = &recoil_direction;
			have_recoil_direction = TRUE;
		}
	}

	actor->stimuli.was_damaged = TRUE;
	if ((!prop || prop->enemy) && actor->state.mode < _actor_mode_combat)
	{
		actor_stimulus_surprise(
			actor_index,
			_actor_surprise_unprepared_damage,
			prop_index,
			surprise_vector);
		actor_stimulus_combat(
			actor_index,
			_actor_stimulus_combat_damage,
			NULL,
			NONE,
			0.0f,
			90,
			surprise_vector,
			prop_index,
			150,
			FALSE);
	}

	if (prop_index != NONE)
	{
		direction.type = _direction_specification_prop;
		direction.prop_index = prop_index;
	}
	else if (have_recoil_direction)
	{
		direction.type = _direction_specification_vector;
		direction.vector = recoil_direction;
	}
	else
	{
		return;
	}

	actor_look_secondary(
		actor_index,
		_secondary_look_damage,
		_secondary_look_priority_default,
		&direction);

	return;
}

void actor_stimulus_prop_just_killed(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);

	actor->state.been_in_combat = TRUE;
	if (!prop->enemy)
	{
		struct actor_definition *definition;
		long killer_prop_index;

		actor = actor_get(actor_index);
		definition = actor_definition_get(actor->meta.definition_index);
		killer_prop_index = actor_perception_find_killer_prop_index(
			actor_index,
			prop_index,
			TRUE);

		if (prop->type == definition->panic.panic_leader_type &&
			actor->stimuli.panic_type < _actor_panic_friend_leader_type_killed &&
			real_seed_random(get_global_random_seed_address()) <
				definition->panic.panic_chance_leader_type_killed)
		{
			actor->stimuli.panic_type = _actor_panic_friend_leader_type_killed;
			actor->stimuli.panic_prop_index = killer_prop_index;
		}

		if (prop->distance < 8.0f && killer_prop_index != NONE)
		{
			struct prop_datum *killer_prop = prop_get(killer_prop_index);

			if (killer_prop->enemy)
			{
				if (killer_prop->visibility > 0 &&
					killer_prop->quantized_facing <= _prop_facing_central)
				{
					real flee_chance =
						definition->panic.panic_chance_friend_killed;

					if ((TEST_FLAG(
							definition->flags2,
							_actor_definition_flags2_panic_in_groups_bit) &&
						game_time_get() >
							actor->emotions.flee_with_friends_disable_time &&
						actor_emotion_flee_with_friends(
							actor_index,
							&flee_chance)) ||
						real_seed_random(get_global_random_seed_address()) <
							flee_chance)
					{
						if (actor->stimuli.panic_type <
							_actor_panic_friend_same_type_killed)
						{
							actor->stimuli.panic_type =
								_actor_panic_friend_same_type_killed;
							actor->stimuli.panic_prop_index =
								killer_prop_index;
						}
					}
				}

				if (killer_prop->unopposable_enemy)
				{
					killer_prop->unopposable_casualties_inflicted++;
					killer_prop->unopposable_casualty_decay_timer = 750;
				}
			}
		}
	}

	return;
}

void actor_stimulus_prop_acknowledged(
	long actor_index,
	long prop_index,
	boolean reappearance,
	boolean expected_acknowledgement)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct prop_datum *prop = prop_get(prop_index);

	if (!prop->dead)
	{
		{
			struct direction_specification direction;

			direction.type = _direction_specification_prop;
			direction.prop_index = prop_index;
			actor_look_secondary(
				actor_index,
				_secondary_look_newly_acknowledged_prop,
				_secondary_look_priority_default,
				&direction);
		}

		if (prop->enemy)
		{
			short surprise_level = _actor_surprise_none;
			boolean flanked = dot_product3d(
				&prop->actor_to_prop,
				&actor->input.facing_vector) < 0.5f;
			short combat_status = actor->state.combat_status;

			if (!combat_status)
			{
				expected_acknowledgement = FALSE;
				if (actor->state.mode < _actor_mode_combat)
				{
					if (prop->shooting)
					{
						surprise_level =
							_actor_surprise_unprepared_enemy_shooting;
					}
					if (prop->distance < definition->panic.surprise_distance)
					{
						surprise_level = MAX(
							surprise_level,
							_actor_surprise_unprepared_enemy_close);
					}
				}
			}
			else if (combat_status >= _actor_combat_status_clear_los && !flanked)
			{
				expected_acknowledgement = TRUE;
			}

			if (!expected_acknowledgement &&
				prop->shooting &&
				prop->distance < definition->panic.surprise_distance)
			{
				if (flanked)
				{
					surprise_level = MAX(
						surprise_level,
						_actor_surprise_unexpected_enemy_close_flanked_shooting);
				}
				else
				{
					surprise_level = MAX(
						surprise_level,
						_actor_surprise_unexpected_enemy_close_shooting);
				}
			}

			if (ai_debug.print_surprise && prop->player)
			{
				char const *surprise_names[NUMBER_OF_ACTOR_SURPRISE_TYPES] =
				{
					"none",
					"enemy-shoot",
					"impact",
					"enemy-close",
					"grenade",
					"damage",
					"unexp-close-shoot",
					"unexp-behind-shoot",
				};

				console_printf(
					FALSE,
					"%s %d: surprise %s: %s %sexp %s %sshoot %s (%.1f%c%.1f)",
					actor_type_get_name((word)actor->meta.type),
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index),
					surprise_names[surprise_level],
					!combat_status ? "noncom" : "combat",
					expected_acknowledgement ? "" : "un",
					flanked ? "flank" : "front",
					prop->shooting ? "" : "not",
					prop->distance < definition->panic.surprise_distance ?
						"close" : "far",
					prop->distance,
					prop->distance < definition->panic.surprise_distance ?
						'<' : '>',
					definition->panic.surprise_distance);
			}

			if (surprise_level > _actor_surprise_none)
			{
				actor_stimulus_surprise(
					actor_index,
					surprise_level,
					prop_index,
					&prop->actor_to_prop);
			}

			if (actor->state.combat_status < _actor_combat_status_definite &&
				!expected_acknowledgement &&
				prop->visibility < 2 &&
				actor->meta.unit_index != NONE)
			{
				ai_communication_event(
					_ai_communication_unexpected_enemy,
					actor->meta.unit_index,
					prop->unit_index,
					_comm_hostility_enemy,
					NONE,
					NONE,
					NULL);
			}
		}
	}

	if (prop->player &&
		prop->enemy &&
		!prop->dead &&
		actor->meta.type != _actor_mounted_weapon &&
		cheat.medusa)
	{
		actor_kill(actor_index, FALSE, TRUE);
	}

	return;
}

void actor_stimulus_was_surprised(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->stimuli.was_surprised = TRUE;
	return;
}

void actor_stimulus_vehicle_eviction(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor->stimuli.vehicle_eviction = TRUE;
	return;
}

void actor_stimulus_bumped(
	long actor_index,
	long prop_index)
{
	struct direction_specification direction;

	direction.type = _direction_specification_prop;
	direction.prop_index = prop_index;
	actor_look_secondary(
		actor_index,
		_secondary_look_bumped_prop,
		_secondary_look_priority_default,
		&direction);

	return;
}

void actor_stimulus_environmental_noise(
	long actor_index,
	long object_index,
	real_point3d const *position,
	short count)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct direction_specification direction;

	if (actor->state.mode == _actor_mode_asleep)
	{
		return;
	}

	direction.type = _direction_specification_point;
	direction.point = *position;
	actor_look_secondary(
		actor_index,
		_secondary_look_environmental_noise,
		_secondary_look_priority_default,
		&direction);

	return;
}

void actor_stimulus_heard_shooting(
	long actor_index,
	long prop_index)
{
	struct prop_datum *prop = prop_get(prop_index);

	if (prop->enemy)
	{
		actor_stimulus_enter_combat_perceived_enemy(actor_index, prop_index);
	}
	else
	{
		struct unit_datum *unit = unit_get(prop->unit_index);
		long player_index;

		actor_stimulus_enter_combat_friend_in_combat(actor_index, prop_index);
		player_index = unit->unit.player_index;
		if (player_index != NONE)
		{
			struct player_datum *player = player_get(player_index);

			if (player->aim_assist_unit_index != NONE &&
				player->aim_assist_timestamp + 90 >= game_time_get())
			{
				struct actor_datum *actor = actor_get(actor_index);

				if (game_team_is_enemy(
					(word)actor->meta.team_index,
					(word)unit_get(player->aim_assist_unit_index)->object.owner_team_index))
				{
					actor_perception_create_orphan_from_friend(
						actor_index,
						player->aim_assist_unit_index,
						NONE,
						NONE);
				}
			}
		}
		else
		{
			if (unit->unit.actor_index != NONE &&
				actor_get(unit->unit.actor_index)->state.combat_status >=
					_actor_combat_status_certain)
			{
				actor_derive_target_information(actor_index, unit->unit.actor_index);
			}
		}
	}

	{
		struct direction_specification direction;

		direction.type = _direction_specification_prop;
		direction.prop_index = prop_index;
		actor_look_secondary(
			actor_index,
			_secondary_look_shooting_prop,
			_secondary_look_priority_default,
			&direction);
	}

	return;
}

void actor_stimulus_prop_sighted(
	long actor_index,
	long prop_index,
	boolean initial_acknowledgement)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop = prop_get(prop_index);

	if (!prop->dead &&
		actor->meta.unit_index != NONE &&
		prop->enemy)
	{
		ai_communication_event(
			initial_acknowledgement ?
				_ai_communication_sighted_enemy :
				_ai_communication_found_enemy,
			actor->meta.unit_index,
			prop->unit_index,
			_comm_hostility_enemy,
			NONE,
			NONE,
			NULL);
	}

	return;
}

void actor_stimulus_maneuvering(
	long actor_index,
	boolean advancing,
	boolean flee)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->meta.unit_index != NONE)
	{
		ai_communication_event(
			advancing ?
				_ai_communication_advance :
				_ai_communication_retreat,
			actor->meta.unit_index,
			NONE,
			NONE,
			NONE,
			NONE,
			NULL);
	}

	if (flee &&
		actor->stimuli.panic_type < _actor_panic_platoon_retreating)
	{
		actor->stimuli.panic_type = _actor_panic_platoon_retreating;
		actor->stimuli.panic_prop_index = NONE;
	}

	return;
}

void actor_stimulus_abandon_stationary_facing(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);

	if (actor->emotions.defensive_crouch)
	{
		if (definition->defensive.change_facing_stand_time > 0.0f)
		{
			actor->emotions.defensive_crouch = FALSE;
			actor->emotions.defensive_crouch_timer =
				(short)(definition->defensive.change_facing_stand_time *
					TICKS_PER_SECOND);
			{
				long prop_index = actor->target.target_prop_index;

				if (prop_index != NONE && prop_get(prop_index)->distance < 4.0f)
				{
					actor->emotions.perceived_danger = MAX(
						actor->emotions.perceived_danger,
						1.8f);
				}
			}
		}
	}

	return;
}

void actor_stimulus_prop_fleeing(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);
	struct prop_datum *prop = prop_get(prop_index);

	if (!prop->enemy &&
		TEST_FLAG(definition->flags2,
			_actor_definition_flags2_panic_in_groups_bit) &&
		game_time_get() > actor->emotions.flee_with_friends_disable_time)
	{
		real desire_to_flee = definition->panic.panic_chance_friend_killed;

		if (actor_emotion_flee_with_friends(actor_index, &desire_to_flee) ||
			real_seed_random(get_global_random_seed_address()) < desire_to_flee)
		{
			if (actor->stimuli.panic_type < _actor_panic_friend_same_type_killed)
			{
				long flee_prop_index = actor->target.target_prop_index;

				if (prop->actor_index != NONE)
				{
					struct actor_datum *fleeing_actor = actor_get(prop->actor_index);

					if (fleeing_actor->state.action == _actor_action_flee &&
						fleeing_actor->state.action_data.flee.flee_prop_index != NONE)
					{
						flee_prop_index = prop_get_active_by_unit_index(
							actor_index,
							prop_get(fleeing_actor->state.action_data.flee.flee_prop_index)->unit_index);
					}
					actor->stimuli.panic_type = _actor_panic_friend_fleeing;
					actor->stimuli.panic_prop_index = flee_prop_index;
				}
			}
		}
	}

	return;
}

/* ---------- private code */
