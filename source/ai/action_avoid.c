/*
ACTION_AVOID.C

symbols in this file:
00000EC0 0040:
	_action_avoid_setup (0000)
00000F00 0010:
	_action_avoid_begin (0000)
00000F10 0010:
	_action_avoid_end (0000)
00000F20 00c0:
	_action_avoid_perform (0000)
00000FE0 0010:
	_action_avoid_update (0000)
00000FF0 0090:
	_action_avoid_control (0000)
00242F3C 0021:
	??_C@_0CB@LDCPOHMO@c?3?2halo?2SOURCE?2ai?2action_avoid?4c@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"

#include "actors.h"

/* ---------- constants */

enum
{
	_action_avoid_primary_target_priority = 7,
	_action_avoid_primary_danger_priority = 5,
	_action_avoid_idle_look_type = 4,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

boolean action_avoid_setup(
	long actor_index,
	struct avoid_state_data *state_data)
{
	match_assert("c:\\halo\\SOURCE\\ai\\action_avoid.c", 30, state_data);
	memset(state_data, 0, sizeof(*state_data));
	return TRUE;
}

void
action_avoid_begin(
	long actor_index)
{
	return;
}

void
action_avoid_end(
	long actor_index)
{
	return;
}

boolean action_avoid_perform(
	long actor_index)
{
	boolean position_flags;
	long previous_owner_actor_index;
	short firing_position_index;
	struct firing_position candidate;
	struct firing_position_evaluation_context search;
	struct path_state workspace;
	struct actor_datum *actor = actor_get(actor_index);

	match_assert("c:\\halo\\SOURCE\\ai\\action_avoid.c", 55, !actor->meta.swarm);
	if (actor->meta.timeslice)
	{
		memset(&search, 0, sizeof(search));
		search.evaluation_mode = _firing_point_evaluation_mode_avoid;
		firing_position_index = actor_active_select_firing_position(
			actor_index,
			&search,
			&candidate,
			&previous_owner_actor_index,
			&workspace,
			&position_flags);
		actor_change_firing_position(
			actor_index,
			firing_position_index,
			&candidate,
			previous_owner_actor_index,
			&workspace,
			position_flags);
	}

	return actor->danger_zone.danger_type == 0;
}

void
action_avoid_update(
	long actor_index)
{
}

void
action_avoid_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->target.target_type >= _actor_target_uninspected_orphan)
	{
		actor->orders.combat.shoot_at_target = TRUE;
		actor->orders.look.primary_priority = _action_avoid_primary_target_priority;
	}
	else
	{
		actor->orders.look.primary_priority = _action_avoid_primary_danger_priority;
		if (actor->danger_zone.danger_type > 0)
		{
			actor->orders.look.primary_direction.type = _direction_specification_danger;
			goto finish_control;
		}
	}
	actor->orders.look.primary_direction.type = _direction_specification_target;

finish_control:
	actor->orders.look.idle_look_type = _action_avoid_idle_look_type;
	actor->orders.move.stationary_crouch = actor->emotions.defensive_crouch;
	actor->orders.move.moving_crouch = FALSE;
	actor->orders.move.panicked = FALSE;
	actor->orders.move.dive_into_cover = FALSE;
	actor->orders.move.emerge_from_cover = FALSE;
}

/* ---------- private code */
