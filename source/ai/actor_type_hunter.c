/*
ACTOR_TYPE_HUNTER.C

symbols in this file:
00027120 0160:
	_hunter_decide_action (0000)
00246444 0007:
	??_C@_06BAGPFCCJ@hunter?$AA@ (0000)
002B6BA4 0020:
	_actor_type_hunter (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"
#include "actors.h"
#include "actor_types.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void hunter_decide_action(
	long actor_index);

/* ---------- globals */

struct actor_type_definition actor_type_hunter =
{
	"hunter",
	4,
	0,
	1,
	1,
	1,
	FALSE,
	{ 0, 0 },
	NULL,
	hunter_decide_action,
	NULL,
	NULL
};

/* ---------- public code */

/* ---------- private code */

void hunter_decide_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	if (!actor_action_deny_transition(actor_index))
	{
		actor_action_handle_berserking_from_damage(actor_index);
		actor_action_handle_berserking_from_attached_projectiles(actor_index);
		actor_action_handle_berserk_transition(actor_index, 3);
		actor_action_handle_combat_transition(actor_index);
		actor_action_handle_danger_avoidance(actor_index);
	}

	switch (actor->state.action)
	{
	case _actor_action_fight:
	case _actor_action_flee:
	case _actor_action_guard:
	case _actor_action_charge:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
		{
			actor_action_handle_combat_failure(actor_index);
			return;
		}
		break;

	case _actor_action_uncover:
	case _actor_action_search:
	case _actor_action_wait:
		if (!actor_action_handle_combat_status(actor_index, TRUE, FALSE))
		{
			actor_action_handle_exit_pursuit(actor_index);
			return;
		}
		break;

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
		{
			actor_action_handle_combat_status(actor_index, TRUE, TRUE);
		}
		break;
	}

	return;
}
