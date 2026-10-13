/*
ACTOR_TYPE_MOUNTED_WEAPON.C

symbols in this file:
000289C0 00d0:
	_mounted_weapon_decide_action (0000)
00246540 000f:
	??_C@_0P@NIDPBLDH@mounted_weapon?$AA@ (0000)
002B6C24 0020:
	_actor_type_mounted_weapon (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actors.h"
#include "actor_types.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void mounted_weapon_decide_action(long actor_index);

/* ---------- globals */

struct actor_type_definition actor_type_mounted_weapon =
{
	"mounted_weapon",
	0,
	2,
	2,
	2,
	0,
	FALSE,
	{ 0, 0 },
	NULL,
	mounted_weapon_decide_action,
	NULL,
	NULL
};

/* ---------- public code */

/* ---------- private code */

void mounted_weapon_decide_action(long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	actor_action_handle_initial_action(actor_index);
	actor_action_handle_pending_command_list(actor_index);
	if (!actor_action_deny_transition(actor_index))
	{
		actor_action_handle_combat_transition(actor_index);
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
		break;
	}
}
