/*
ACTOR_TYPE_CARRIER.C

symbols in this file:
00026000 0160:
	_carrier_decide_action (0000)
002463A0 000e:
	??_C@_0O@KOGDMDHG@flood?5carrier?$AA@ (0000)
002B6AE4 0020:
	_actor_type_carrier (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "actor_definitions.h"
#include "actor_types.h"
#include "actors.h"
#include "actions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void carrier_decide_action(
	long actor_index);

/* ---------- globals */

struct actor_type_definition actor_type_carrier =
{
	"flood carrier",
	16,
	0,
	2,
	2,
	1,
	FALSE,
	{ 0, 0 },
	NULL,
	carrier_decide_action,
	NULL,
	NULL
};

/* ---------- public code */

/* ---------- private code */

void carrier_decide_action(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor_definition_get(actor->meta.definition_index);
	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	actor_action_handle_surprise(actor_index, 4);
	if (!actor_action_deny_transition(actor_index))
	{
		actor_action_handle_berserking_from_attacking_mode(actor_index);
		actor_action_handle_berserking_from_damage(actor_index);
		actor_action_handle_berserking_from_proximity(actor_index);
		actor_action_handle_berserk_transition(actor_index, TRUE);
		actor_action_handle_combat_transition(actor_index);
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

	case _actor_action_obey:
		actor_action_handle_combat_status(
			actor_index,
			actor->state.action_data.obey.initiative,
			actor->state.action_data.obey.finished);
		break;
	}

	return;
}
