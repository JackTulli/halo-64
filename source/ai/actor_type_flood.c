/*
ACTOR_TYPE_FLOOD.C

symbols in this file:
00026810 0170:
	_flood_decide_action (0000)
00026980 0510:
	_actor_type_flood_desire_shamble (0000)
002463CC 0006:
	??_C@_05ONANONLM@flood?$AA@ (0000)
002463D4 0004:
	__real@41f80000 (0000)
002463D8 0032:
	??_C@_0DC@EGHABMGG@actor?9?$DOemotions?4crouch_switching@ (0000)
0024640C 0004:
	__real@bfb33333 (0000)
00246410 0004:
	__real@3fb33333 (0000)
00246414 0025:
	??_C@_0CF@GOCGFILI@c?3?2halo?2SOURCE?2ai?2actor_type_flo@ (0000)
002B6B64 0020:
	_actor_type_flood (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "actor_definitions.h"
#include "actor_types.h"
#include "actors.h"
#include "actions.h"
#include "props.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	_actor_mode_combat = 3,
};

enum
{
	_actor_combat_status_clear_los = 5,
};

enum
{
	_actor_movement_switching_always_run = 0,
	_actor_movement_switching_always_crouch,
	_actor_movement_switching_switch_types,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void flood_decide_action(
	long actor_index);

/* ---------- globals */

struct actor_type_definition actor_type_flood =
{
	"flood",
	8,
	0,
	0,
	0,
	1,
	FALSE,
	{ 0, 0 },
	NULL,
	flood_decide_action,
	NULL,
	NULL
};

/* ---------- public code */

/* ---------- private code */

void flood_decide_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor_definition_get(actor->meta.definition_index);
	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	actor_action_handle_surprise(actor_index, 4);
	if (!actor_action_deny_transition(actor_index))
	{
		actor_action_handle_berserking_from_damage(actor_index);
		actor_action_handle_berserk_transition(actor_index, 3);
		actor_action_handle_combat_transition(actor_index);
		actor_action_handle_danger_avoidance(actor_index);
	}

	switch (actor->state.action)
	{
	case _actor_action_fight:
	case _actor_action_charge:
		if (actor_action_handle_combat_status(actor_index, TRUE, FALSE) ||
			actor_action_handle_combat_failure(actor_index))
		{
			return;
		}
		actor_action_handle_evasion(actor_index);
		return;

	case _actor_action_flee:
		if (actor->state.action_data.flee.unable_to_flee)
		{
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		}
		else
		{
			actor_action_handle_done_fleeing(actor_index);
		}
		return;

	case _actor_action_guard:
		actor_action_handle_combat_status(
			actor_index,
			actor_action_can_stop_guarding(actor_index, _actor_action_fight, _actor_action_guard),
			FALSE);
		return;

	case _actor_action_uncover:
	case _actor_action_search:
	case _actor_action_wait:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
		{
			actor_action_handle_exit_pursuit(actor_index);
		}
		return;

	case _actor_action_obey:
		actor_action_handle_combat_status(
			actor_index,
			actor->state.action_data.obey.initiative,
			actor->state.action_data.obey.finished);
		return;

	case _actor_action_avoid:
		if (actor->danger_zone.danger_type == 0)
		{
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		}
		return;
	}

	return;
}

boolean actor_type_flood_desire_shamble(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_variant_definition const *variant =
		actor_variant_definition_get(actor->meta.variant_definition_index);
	struct actor_variant_definition const *firing_variant =
		actor_combat_get_firing_variant_definition(actor_index);
	boolean result = FALSE;
	boolean crouch_switching = FALSE;

	if (!unit_is_busy(actor->meta.unit_index) && !actor_path_has_path(actor_index))
	{
		result = TRUE;
	}
	else if (actor->state.mode < _actor_mode_combat)
	{
		result = TRUE;
	}
	else if (actor->state.combat_status < _actor_combat_status_clear_los)
	{
		result = FALSE;
	}
	else
	{
		struct unit_datum *unit = unit_get(actor->meta.unit_index);
		struct prop_datum *target_prop = actor->target.target_prop_index == NONE
			? NULL
			: prop_get(actor->target.target_prop_index);

		if (unit->unit.animation.state == _unit_state_hard_ping && !actor->emotions.berserk)
		{
			result = TRUE;
		}
		else if (target_prop &&
			target_prop->distance > firing_variant->ranged_combat.maximum_firing_range)
		{
			result = FALSE;
		}
		else if (actor->emotions.berserk && target_prop &&
			target_prop->distance > firing_variant->ranged_combat.berserk_firing_range_upper_bound)
		{
			result = FALSE;
		}
		else if (TEST_FLAG(unit->object.damage_flags, _object_melee_attack_inhibited_bit))
		{
			result = TRUE;
		}
		else if (actor->emotions.berserk ||
			(actor->state.action == _actor_action_charge &&
				(actor->state.action_data.charge.goal == _charge_goal_melee ||
					actor->state.action_data.charge.goal == _charge_goal_melee_leaping)))
		{
			result = FALSE;
		}
		else if (!actor_has_ranged_weapon(actor_index) || actor->input.underwater)
		{
			result = FALSE;
		}
		else if (variant->movement_switching.movement_type == _actor_movement_switching_always_run)
		{
			result = FALSE;
		}
		else if (variant->movement_switching.movement_type == _actor_movement_switching_always_crouch)
		{
			result = TRUE;
		}
		else if (target_prop &&
			target_prop->distance < firing_variant->ranged_combat.combat_range_upper_bound)
		{
			result = TRUE;
		}
		else
		{
			crouch_switching = TRUE;
		}
	}

	if (crouch_switching)
	{
		boolean value_changed = FALSE;

		if (actor->emotions.crouch_switching_active)
		{
			boolean change_value = FALSE;

			if (actor->emotions.crouch_switching_stay_timer > 0)
			{
				actor->emotions.crouch_switching_stay_timer--;
			}
			else if (TEST_FLAG(
				variant->flags,
				_actor_variant_definition_movement_switching_try_to_stay_with_friends_bit) &&
				actor->situation.close_friends > 0)
			{
				struct prop_iterator iterator;
				struct prop_datum *prop;
				struct prop_datum *target_prop;
				short backward_count, middle_count, forward_count;

				backward_count = middle_count = forward_count = 0;

				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_type_flood.c",
					201,
					actor->target.target_prop_index != NONE);
				target_prop = prop_get(actor->target.target_prop_index);

				prop_iterator_new(&iterator, actor_index);
				while ((prop = prop_iterator_next(&iterator)) != NULL)
				{
					if (prop->state >= _prop_state_becoming_unacknowledged &&
						prop->state <= _prop_state_acknowledged &&
						!prop->enemy && !prop->dead && prop->distance < 15.0f &&
						prop->actor_index != NONE)
					{
						struct actor_datum *friend_actor = actor_get(prop->actor_index);

						if (friend_actor->emotions.crouch_switching_active)
						{
							real_vector3d friend_vector;
							real projection;

							vector_from_points3d(
								&actor->input.position.body_position,
								&friend_actor->input.position.body_position,
								&friend_vector);
							projection = dot_product3d(&friend_vector, &target_prop->actor_to_prop);
							if (projection > 1.4f)
							{
								forward_count++;
							}
							else if (projection < -1.4f)
							{
								backward_count++;
							}
							else
							{
								middle_count++;
							}
						}
					}
				}

				if (actor->emotions.crouch_switching_current_value)
				{
					if (backward_count == 0 && forward_count > middle_count)
					{
						change_value = TRUE;
					}
				}
				else
				{
					if (forward_count == 0 && backward_count > middle_count)
					{
						change_value = TRUE;
					}
				}
			}

			if (!change_value)
			{
				match_assert(
					"c:\\halo\\SOURCE\\ai\\actor_type_flood.c",
					256,
					actor->emotions.crouch_switching_change_timer > 0);
				if (--actor->emotions.crouch_switching_change_timer == 0)
				{
					change_value = TRUE;
				}
			}

			if (change_value)
			{
				actor->emotions.crouch_switching_current_value =
					!actor->emotions.crouch_switching_current_value;
				value_changed = TRUE;
			}
		}
		else
		{
			real crouch_chance = variant->movement_switching.initial_crouch_chance;
			real random_value;

			if (actor->situation.area_friends > 0)
			{
				short crouching_count, standing_count;
				struct prop_iterator iterator;
				struct prop_datum *prop;
				real adjustment;

				crouching_count = standing_count = 0;

				prop_iterator_new(&iterator, actor_index);
				while ((prop = prop_iterator_next(&iterator)) != NULL)
				{
					if (prop->state >= _prop_state_becoming_unacknowledged &&
						prop->state <= _prop_state_acknowledged &&
						!prop->enemy && !prop->dead && prop->actor_index != NONE)
					{
						struct actor_datum *friend_actor = actor_get(prop->actor_index);

						if (friend_actor->meta.type == actor->meta.type &&
							friend_actor->state.combat_status >= _actor_combat_status_clear_los)
						{
							if (friend_actor->emotions.defensive_crouch)
							{
								crouching_count++;
							}
							else
							{
								standing_count++;
							}
						}
					}
				}

				adjustment = crouching_count * (1.0f - crouch_chance);
				adjustment += standing_count * (-crouch_chance);
				crouch_chance -= 0.5f * adjustment;
			}

			random_value = real_seed_random(get_global_random_seed_address());
			actor->emotions.crouch_switching_active = TRUE;
			actor->emotions.crouch_switching_current_value = random_value < crouch_chance;
			value_changed = TRUE;
		}

		if (value_changed)
		{
			real change_time;

			if (actor->emotions.crouch_switching_current_value)
			{
				change_time = real_random_range(
					variant->movement_switching.crouch_time_lower_bound,
					variant->movement_switching.crouch_time_upper_bound);
			}
			else
			{
				change_time = real_random_range(
					variant->movement_switching.run_time_lower_bound,
					variant->movement_switching.run_time_upper_bound);
			}
			actor->emotions.crouch_switching_change_timer = (short)MAX(change_time * 30.0f, 31.0f);
			actor->emotions.crouch_switching_stay_timer = 30;
		}

		result = actor->emotions.crouch_switching_current_value;
	}
	else
	{
		actor->emotions.crouch_switching_active = FALSE;
	}

	return result;
}
