/*
ACTION_ALERT.C

symbols in this file:
00000600 0080:
	_action_alert_setup (0000)
00000680 0010:
	_action_alert_begin (0000)
00000690 0050:
	_action_alert_control (0000)
000006E0 0030:
	_action_alert_flush_position_indices (0000)
00000710 0030:
	_action_alert_flush_structure_indices (0000)
00000740 0030:
	_vector_from_points3d (0000)
00000770 0030:
	_magnitude_squared3d (0000)
000007A0 0040:
	_distance_squared3d (0000)
000007E0 0020:
	_real_random_range (0000)
00000800 0150:
	_action_alert_update (0000)
00000950 0310:
	_action_alert_next_position (0000)
00000C60 0260:
	_action_alert_perform (0000)
00242EEC 000b:
	??_C@_0L@ILHIFHGP@state_data?$AA@ (0000)
00242EF8 0021:
	??_C@_0CB@JGDDPGCE@c?3?2halo?2SOURCE?2ai?2action_alert?4c@ (0000)
00242F1C 0004:
	__real@3e800000 (0000)
00242F20 0013:
	??_C@_0BD@KDCHGMEK@?$CBactor?9?$DOmeta?4swarm?$AA@ (0000)
00242F34 0004:
	__real@41f00000 (0000)
00242F38 0004:
	__real@3f000000 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"
#include "math/real_math.h"

#include "actor_definitions.h"
#include "actors.h"
#include "ai_scenario_definitions.h"
#include "props.h"
#include "game/game.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum move_position_order
{
	_move_position_order_none = 0,
	_move_position_order_repeat,
	_move_position_order_loop,
	_move_position_order_loop_back_and_forth,
	_move_position_order_loop_randomly,
	_move_position_order_random,
	NUMBER_OF_MOVE_POSITION_ORDERS,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static short action_alert_next_position(
	long actor_index,
	short move_position_order,
	short current_position_index,
	boolean *direction_increasing);

/* ---------- globals */

/* ---------- public code */

boolean action_alert_setup(
	long actor_index,
	long move_position_order,
	short initial_move_position_index,
	struct alert_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);

	match_assert("c:\\halo\\SOURCE\\ai\\action_alert.c", 35, state_data);
	csmemset(state_data, 0, sizeof(*state_data));
	if (actor->meta.swarm)
		move_position_order = _move_position_order_none;
	state_data->move_position_order = move_position_order;
	state_data->target_move_position_index = NONE;
	state_data->pending_move_position_index = initial_move_position_index;
	state_data->move_position_increasing = TRUE;
	state_data->wait_ticks = 0;
	state_data->must_play_animation = FALSE;

	return TRUE;
}

void action_alert_begin(
	long actor_index)
{
	return;
}

void action_alert_flush_position_indices(
	long actor_index)
{
	struct alert_state_data *state_data =
		&actor_get(actor_index)->state.action_data.alert;

	state_data->target_move_position_index = NONE;
	state_data->pending_move_position_index = NONE;

	return;
}

void action_alert_flush_structure_indices(
	long actor_index)
{
	struct alert_state_data *state_data =
		&actor_get(actor_index)->state.action_data.alert;

	state_data->target_position_definition.cluster_index = NONE;
	state_data->target_position_definition.surface_index = NONE;

	return;
}

void action_alert_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition =
		actor_definition_get(actor->meta.definition_index);

	actor->orders.look.idle_look_type = 1;
	if (TEST_FLAG(definition->flags,
		_actor_definition_crouch_when_noncombat_bit))
	{
		actor->orders.move.stationary_crouch = TRUE;
		actor->orders.move.moving_crouch = TRUE;
	}

	return;
}

void action_alert_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct alert_state_data *state_data = &actor->state.action_data.alert;

	if (!actor->meta.dormant && state_data->target_move_position_index != NONE)
	{
		real distance_squared = distance_squared3d(
			&actor->input.position.body_position,
			&state_data->target_position_definition.position);

		match_assert("c:\\halo\\SOURCE\\ai\\action_alert.c", 174, !actor->meta.swarm);
		if (actor_path_at_destination(actor_index) || distance_squared < 0.25f)
		{
			if (state_data->wait_ticks > 0)
				state_data->wait_ticks--;

			if (state_data->must_play_animation)
			{
				short animation_reference_index =
					state_data->target_position_definition.animation_reference_index;

				if (animation_reference_index != NONE)
				{
					struct ai_animation_reference_definition *animation_reference =
						TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->ai_animation_references,
							animation_reference_index,
							struct ai_animation_reference_definition);
					long animation_graph_index = animation_reference->animation_graph.index;

					if (animation_graph_index == NONE)
					{
						struct unit_datum *unit = unit_get(actor->meta.unit_index);
						struct unit_definition *definition =
							unit_definition_get(unit->definition_index);

						animation_graph_index = definition->object.animation_graph.index;
					}

					unit_start_user_animation(
						actor->meta.unit_index,
						animation_graph_index,
						animation_reference->animation_name,
						TRUE);
				}

				state_data->must_play_animation = FALSE;
			}
		}
	}

	return;
}

/* ---------- private code */

static short action_alert_next_position(
	long actor_index,
	short move_position_order,
	short current_position_index,
	boolean *direction_increasing)
{
	struct actor_datum *actor = actor_get(actor_index);
	short result = NONE;

	if (actor->input.vehicle_passenger ||
		move_position_order == _move_position_order_none)
	{
		result = NONE;
	}
	else if (actor->meta.encounter_index != NONE)
	{
		struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
			&encounter->squads,
			actor->meta.squad_index,
			struct squad_definition);

		match_assert("c:\\halo\\SOURCE\\ai\\action_alert.c", 275, !actor->meta.swarm);
		if (move_position_order == _move_position_order_repeat &&
			current_position_index != NONE)
		{
			result = current_position_index;
		}
		else
		{
			struct tag_block *move_positions;
			unsigned long unavailable_positions[BIT_VECTOR_SIZE_IN_LONGS(32)];
			boolean any_position_available = FALSE;
			short position_index;

			csmemset(
				unavailable_positions,
				0,
				sizeof(unavailable_positions));
			position_index = 0;
			move_positions = &squad->move_positions;
			while (position_index < move_positions->count)
			{
				struct move_position_definition *move_position =
					TAG_BLOCK_GET_ELEMENT(
						move_positions,
						position_index,
						struct move_position_definition);
				boolean position_available = TRUE;
				struct prop_iterator iterator;
				struct prop_datum *prop;

				if (position_index == current_position_index)
					position_available = FALSE;

				if (current_position_index != NONE &&
					distance_squared3d(
						&actor->input.position.body_position,
						&move_position->position) < 0.25f)
				{
					position_available = FALSE;
				}

				if (move_position->sequence_id &&
					move_position->sequence_id != actor->state.noncombat_sequence_id)
				{
					position_available = FALSE;
				}

				prop_iterator_new(&iterator, actor_index);
				for (prop = prop_iterator_next(&iterator);
					prop;
					prop = prop_iterator_next(&iterator))
				{
					if (prop->state >= _prop_state_becoming_unacknowledged &&
						prop->state <= _prop_state_acknowledged &&
						distance_squared3d(
							&prop->body_position,
							&move_position->position) < 0.25f)
					{
						position_available = FALSE;
						break;
					}
				}

				if (position_available)
					any_position_available = TRUE;
				else
					BIT_VECTOR_SET_FLAG(unavailable_positions, position_index, TRUE);

				position_index++;
			}

			if (!any_position_available)
			{
				result = NONE;
			}
			else
			{
				if (move_position_order == _move_position_order_random)
				{
					result = choose_random_array_element(
						squad->move_positions.address,
						sizeof(struct move_position_definition),
						move_positions->count,
						offsetof(struct move_position_definition, weight),
						unavailable_positions);
				}
				else
				{
					short next_position_index;

					if (current_position_index < 0 ||
						current_position_index >= move_positions->count)
						next_position_index = 0;
					else
						next_position_index = current_position_index;

					do
					{
						boolean increasing = TRUE;

						switch (move_position_order)
						{
						case _move_position_order_loop:
							increasing = TRUE;
							break;
						case _move_position_order_loop_back_and_forth:
							if (next_position_index == 0)
								increasing = TRUE;
							else if (next_position_index == move_positions->count - 1)
								increasing = FALSE;
							else if (direction_increasing)
								increasing = *direction_increasing;
							else
								increasing = TRUE;
							break;
						case _move_position_order_loop_randomly:
							increasing = TEST_FLAG(game_time_get(), 0);
							break;
						}

						if (direction_increasing)
							*direction_increasing = increasing;

						if (increasing)
						{
							next_position_index++;
							if (next_position_index >= move_positions->count)
								next_position_index = 0;
						}
						else
						{
							next_position_index--;
							if (next_position_index < 0)
								next_position_index = (short)(move_positions->count - 1);
						}
					}
					while (BIT_VECTOR_TEST_FLAG(unavailable_positions, next_position_index));

					result = next_position_index;
				}
			}
		}
	}

	return result;
}

/* ---------- public code */

boolean action_alert_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct alert_state_data *state_data = &actor->state.action_data.alert;

	if (state_data->move_position_order != _move_position_order_none &&
		state_data->pending_move_position_index == NONE)
	{
		boolean reached_position = TRUE;

		match_assert("c:\\halo\\SOURCE\\ai\\action_alert.c", 71, !actor->meta.swarm);
		if (state_data->target_move_position_index != NONE &&
			actor_path_has_path(actor_index))
		{
			real distance_squared = distance_squared3d(
				&actor->input.position.body_position,
				&state_data->target_position_definition.position);
			real tolerance = actor_destination_tolerance(actor_index);

			tolerance = MAX(tolerance, 0.5f);
			if (distance_squared > tolerance * tolerance)
				reached_position = FALSE;
		}

		if (reached_position)
		{
			if (state_data->wait_ticks > 0 ||
				state_data->must_play_animation ||
				unit_get(actor->meta.unit_index)->unit.animation.state ==
					_unit_state_user_animation)
			{
				reached_position = FALSE;
			}
		}

		if (reached_position)
		{
			state_data->pending_move_position_index = action_alert_next_position(
				actor_index,
				state_data->move_position_order,
				state_data->target_move_position_index,
				&state_data->move_position_increasing);
		}
	}

	if (actor->meta.timeslice &&
		!actor->meta.dormant &&
		state_data->pending_move_position_index != NONE)
	{
		boolean moved = FALSE;

		match_assert("c:\\halo\\SOURCE\\ai\\action_alert.c", 120, !actor->meta.swarm);
		if (actor->meta.encounter_index != NONE)
		{
			struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
				struct encounter_definition);
			struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
				&encounter->squads,
				actor->meta.squad_index,
				struct squad_definition);

			if (state_data->pending_move_position_index >= 0 &&
				state_data->pending_move_position_index < squad->move_positions.count)
			{
				struct move_position_definition *move_position =
					TAG_BLOCK_GET_ELEMENT(
						&squad->move_positions,
						state_data->pending_move_position_index,
						struct move_position_definition);
				real wait_time = real_random_range(
					move_position->time_lower_bound,
					move_position->time_upper_bound);

				state_data->target_move_position_index =
					state_data->pending_move_position_index;
				state_data->pending_move_position_index = NONE;
				state_data->target_position_definition = *move_position;
				state_data->wait_ticks = (short)(wait_time * TICKS_PER_SECOND);
				state_data->must_play_animation = TRUE;
				moved = actor_move_to_move_position(
					actor_index,
					state_data->target_move_position_index);
			}
		}

		if (!moved)
		{
			state_data->target_move_position_index =
				state_data->pending_move_position_index;
			state_data->pending_move_position_index = NONE;
			state_data->wait_ticks = 0;
			state_data->must_play_animation = FALSE;
		}
	}

	return FALSE;
}
