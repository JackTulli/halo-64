/*
ACTOR_TYPE_ELITE.C

symbols in this file:
00026360 02b0:
	_elite_decide_action (0000)
002463B8 0006:
	??_C@_05EHJKLNBO@elite?$AA@ (0000)
002B6B24 0020:
	_actor_type_elite (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "actions.h"
#include "actor_definitions.h"
#include "actors.h"
#include "actor_types.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void elite_decide_action(
	long actor_index);

/* ---------- globals */

struct actor_type_definition actor_type_elite =
{
	"elite",
	4,
	0,
	1,
	1,
	1,
	FALSE,
	{ 0, 0 },
	NULL,
	elite_decide_action,
	NULL,
	NULL
};

/* ---------- public code */

/* ---------- private code */

void elite_decide_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);

	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	actor_action_handle_surprise(actor_index, 4);
	if (!actor_action_deny_transition(actor_index))
	{
		actor_action_handle_berserking_from_attacking_mode(actor_index);
		actor_action_handle_berserking_from_damage(actor_index);
		actor_action_handle_berserking_from_proximity(actor_index);
		actor_action_handle_berserking_from_attached_projectiles(actor_index);
		actor_action_handle_berserk_transition(actor_index, TRUE);
		if (!actor->emotions.berserk)
		{
			actor_action_handle_panic_from_attached_projectiles(actor_index);
			actor_action_handle_panic_from_attached_melee_attackers(actor_index);
			actor_action_handle_panic_transition(actor_index, 9, FALSE, 11);
		}
		actor_action_handle_combat_transition(actor_index);
		actor_action_handle_active_cover_seeking(actor_index, FALSE, FALSE);
		actor_action_handle_vehicle_entry(actor_index);
		actor_action_handle_vehicle_exit(actor_index);
		actor_action_handle_grenade_throwing(actor_index);
		actor_action_handle_danger_avoidance(actor_index);
	}

	switch (actor->state.action)
	{
	case _actor_action_fight:
	case _actor_action_charge:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
		{
			if (!actor_action_handle_combat_failure(actor_index))
				actor_action_handle_evasion(actor_index);

			return;
		}
		break;

	case _actor_action_guard:
		if (actor->state.action_data.guard.cower &&
			!actor->state.action_data.guard.cower_panicked &&
			!actor->state.action_data.guard.cower_from_retreat)
		{
			real shield_fraction = actor->state.combat_status >= 4
				? definition->defensive.shield_fraction_emerge_attack
				: definition->defensive.shield_fraction_emerge_pursue;

			if (actor->input.shield_vitality < shield_fraction)
			{
				actor->state.action_data.guard.cower = TRUE;
				actor->state.action_data.guard.cower_ticks = 30;
			}
			else
			{
				actor->state.action_data.guard.cower = FALSE;
				actor->state.action_data.guard.cower_ticks = 0;
			}
		}
		actor_action_handle_combat_status(
			actor_index,
			actor_action_can_stop_guarding(actor_index, _actor_action_fight, _actor_action_guard),
			FALSE);
		return;

	case _actor_action_flee:
		if (actor->state.action_data.flee.unable_to_flee)
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		else
			actor_action_handle_done_fleeing(actor_index);
		return;

	case _actor_action_uncover:
	case _actor_action_search:
	case _actor_action_wait:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
			actor_action_handle_exit_pursuit(actor_index);
		return;

	case _actor_action_vehicle:
		if (actor->state.action_data.vehicle.vehicle_entry_done)
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		else if (actor->state.action_data.vehicle.vehicle_entry_failed)
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		return;

	case _actor_action_obey:
		actor_action_handle_combat_status(
			actor_index,
			actor->state.action_data.obey.initiative,
			actor->state.action_data.obey.finished);
		return;

	case _actor_action_converse:
		actor_action_handle_combat_status(
			actor_index,
			actor_action_can_stop_conversing(actor_index),
			actor->state.action_data.converse.failed || actor->external_orders.conversation_index == NONE);
		return;

	case _actor_action_avoid:
		if (actor->danger_zone.danger_type == 0)
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		break;
	}

	return;
}
