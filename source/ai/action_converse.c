/*
ACTION_CONVERSE.C

symbols in this file:
000028A0 00b0:
	_action_converse_setup (0000)
00002950 0010:
	_action_converse_begin (0000)
00002960 0100:
	_action_converse_perform (0000)
00002A60 0020:
	_action_converse_update (0000)
00002A80 0090:
	_action_converse_control (0000)
00002B10 0030:
	_action_converse_replace_prop (0000)
00002B40 0080:
	_actor_conversation_control (0000)
00002BC0 0030:
	_actor_conversation_end (0000)
00002BF0 0030:
	_action_converse_end (0000)
0024305C 0024:
	??_C@_0CE@NNMIMHKJ@c?3?2halo?2SOURCE?2ai?2action_convers@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"
#include "action_converse.h"

#include "actors.h"
#include "ai_communication.h"
#include "props.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/units.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct scenario_conversation_definition
{
	byte __unknown0[0x28];
	real run_to_distance;
	byte __unknown2c[0x48];
};

typedef char scenario_conversation_definition_size_assert[
	sizeof(struct scenario_conversation_definition) == 0x74 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

boolean action_converse_setup(
	long actor_index,
	long conversation_index,
	struct converse_state_data *state_data)
{
	struct ai_conversation_datum_header *conversation;
	struct scenario_conversation_definition *definition;

	actor_get(actor_index);
	conversation = ai_conversation_header_get(conversation_index);
	definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_conversations,
		conversation->scenario_conversation_index,
		struct scenario_conversation_definition);

	match_assert("c:\\halo\\SOURCE\\ai\\action_converse.c", 33, state_data);
	csmemset(state_data, 0, sizeof(*state_data));
	state_data->conversation_index = conversation_index;
	state_data->run_to_distance = definition->run_to_distance;
	state_data->run_to_unit_index = state_data->run_to_distance == 0.0f
		? NONE
		: conversation->triggering_player_unit_index;
	state_data->run_to_prop_index = NONE;
	state_data->in_range = FALSE;

	return TRUE;
}

void action_converse_begin(
	long actor_index)
{
	return;
}

boolean action_converse_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct converse_state_data *state_data = &actor->state.action_data.converse;

	if (actor->meta.timeslice)
	{
		if (state_data->run_to_prop_index == NONE &&
			state_data->run_to_unit_index != NONE)
		{
			state_data->run_to_prop_index = prop_get_base_by_unit_index(
				actor_index,
				state_data->run_to_unit_index,
				TRUE,
				TRUE);
		}

		if (state_data->run_to_prop_index == NONE)
		{
			state_data->failed = TRUE;
		}
		else
		{
			if (!state_data->in_range)
			{
				struct prop_datum *prop = prop_get(state_data->run_to_prop_index);

				if (prop->visibility >= _actor_perception_full &&
					prop->distance < state_data->run_to_distance)
				{
					state_data->in_range = TRUE;
				}
				else if (prop->distance < 0.7f)
				{
					state_data->in_range = TRUE;
				}
			}

			if (state_data->in_range)
			{
				actor_move_halt(actor_index);
			}
			else if (!actor_move_to_prop(
				actor_index,
				state_data->run_to_prop_index,
				state_data->run_to_distance))
			{
				state_data->failed = TRUE;
			}
		}
	}

	return state_data->failed;
}

void action_converse_update(
	long actor_index)
{
	actor_get(actor_index);

	return;
}

void action_converse_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct converse_state_data *state_data = &actor->state.action_data.converse;
	struct ai_conversation_datum_header *conversation = NULL;
	long prop_index = NONE;

	if (state_data->conversation_index != NONE)
		conversation = ai_conversation_header_get(state_data->conversation_index);

	if (state_data->run_to_prop_index != NONE)
	{
		prop_index = state_data->run_to_prop_index;
	}
	else if (conversation && conversation->triggering_player_unit_index != NONE)
	{
		prop_index = prop_get_active_by_unit_index(
			actor_index,
			conversation->triggering_player_unit_index);
	}

	actor->orders.look.idle_look_type = 1;

	if (prop_index != NONE)
	{
		actor->orders.look.primary_priority = _primary_priority_facing;
		actor->orders.look.primary_direction.type = _direction_specification_prop;
		actor->orders.look.primary_direction.prop_index = prop_index;
	}

	return;
}

void action_converse_replace_prop(
	long actor_index,
	long old_prop_index,
	long new_prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct converse_state_data *state_data = &actor->state.action_data.converse;

	if (state_data->run_to_prop_index == old_prop_index)
		state_data->run_to_prop_index = new_prop_index;

	return;
}

void actor_conversation_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->external_orders.conversation_index != NONE &&
		actor->external_orders.conversation_attention_unit_index != NONE)
	{
		struct direction_specification direction;
		long prop_index = prop_get_active_by_unit_index(
			actor_index,
			actor->external_orders.conversation_attention_unit_index);

		if (prop_index != NONE)
		{
			direction.type = _direction_specification_prop;
			direction.prop_index = prop_index;
		}
		else
		{
			direction.type = _direction_specification_point;
			unit_get_head_position(
				actor->external_orders.conversation_attention_unit_index,
				&direction.point);
		}

		actor_look_secondary(
			actor_index,
			_secondary_look_communicating_prop,
			_secondary_look_priority_turn_and_aim,
			&direction);
	}

	return;
}

void actor_conversation_end(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->external_orders.conversation_index != NONE)
	{
		ai_conversation_finish(
			actor->external_orders.conversation_index,
			FALSE,
			FALSE);
	}

	return;
}

void action_converse_end(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (actor->external_orders.conversation_index != NONE)
	{
		ai_conversation_finish(
			actor->external_orders.conversation_index,
			FALSE,
			FALSE);
	}

	return;
}

/* ---------- private code */
