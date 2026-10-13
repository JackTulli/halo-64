/*
ACTION_SEARCH.C

symbols in this file:
00007D60 0080:
	_action_search_setup_target (0000)
00007DE0 0100:
	_action_search_setup_pursuit (0000)
00007EE0 0070:
	_action_search_setup_undirected (0000)
00007F50 0180:
	_action_search_update (0000)
000080D0 0030:
	_action_search_flush_position_indices (0000)
00008100 0030:
	_action_search_flush_structure_indices (0000)
00008130 0150:
	_action_search_control (0000)
00008280 0090:
	_action_search_begin (0000)
00008310 0350:
	_action_search_perform (0000)
00243944 0022:
	??_C@_0CC@PIOOIGM@c?3?2halo?2SOURCE?2ai?2action_search?4@ (0000)
00243968 0004:
	__real@3f23d70b (0000)
0024396C 0004:
	__real@40c80000 (0000)
00243970 0004:
	__real@3efae147 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "actions.h"

#include "actor_definitions.h"
#include "actors.h"
#include "ai.h"
#include "ai_communication.h"
#include "ai_scenario_definitions.h"
#include "encounters.h"
#include "props.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/units.h"
#include "math/real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

long actor_target_unit_index(
	long actor_index);

boolean actors_searching_same_position(
	long actor_index,
	long other_actor_index);

/* ---------- public code */

boolean action_search_setup_target(
	long actor_index,
	boolean must_charge,
	struct search_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean success;

	success = FALSE;
	match_assert("c:\\halo\\SOURCE\\ai\\action_search.c", 33, state_data);
	csmemset(state_data, 0, sizeof(struct search_state_data));
	if (!actor->input.vehicle_passenger)
	{
		state_data->pursuit_location.type = 0;
		state_data->charging = must_charge;
		actor->state.searching = TRUE;
		success = TRUE;
	}
	return success;
}

boolean action_search_setup_pursuit(
	long actor_index,
	short firing_position_index,
	boolean tenacious,
	struct search_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean success;

	success = FALSE;
	match_assert("c:\\halo\\SOURCE\\ai\\action_search.c", 56, state_data);
	csmemset(state_data, 0, sizeof(struct search_state_data));
	if (!actor->input.vehicle_passenger &&
		!actor->meta.swarm &&
		actor->meta.encounter_index != NONE &&
		firing_position_index != NONE)
	{
		struct encounter_definition *encounter;
		struct firing_position_definition *firing_position;

		encounter = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
			struct encounter_definition);
		firing_position = TAG_BLOCK_GET_ELEMENT(
			&encounter->firing_positions,
			firing_position_index,
			struct firing_position_definition);

		state_data->tenacious = tenacious;
		state_data->pursuit_location.firing_position_index = firing_position_index;
		state_data->pursuit_location.type = 1;
		state_data->pursuit_location.position = firing_position->position;
		state_data->pursuit_location.surface_index = firing_position->surface_index;
		state_data->pursuit_location.cluster_index = firing_position->cluster_index;
		success = TRUE;
		actor->state.searching = success;
	}
	return success;
}

boolean action_search_setup_undirected(
	long actor_index,
	boolean tenacious,
	struct search_state_data *state_data)
{
	struct actor_datum *actor = actor_get(actor_index);
	boolean success;

	success = FALSE;
	match_assert("c:\\halo\\SOURCE\\ai\\action_search.c", 87, state_data);
	csmemset(state_data, 0, sizeof(struct search_state_data));
	if (actor->meta.swarm)
	{
		state_data->pursuit_location.type = 2;
		actor->state.searching = TRUE;
		success = TRUE;
	}
	return success;
}

void action_search_update(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct search_state_data *state_data = &actor->state.action_data.search;
	struct actor_definition *definition;

	if (state_data->search_done)
	{
		return;
	}
	definition = actor_definition_get(actor->meta.definition_index);

	if (definition->defensive.defensive_crouch_type == 4)
	{
		state_data->sneaking = TRUE;
	}
	else
	{
		state_data->sneaking = FALSE;
		if ((definition->flags&FLAG(_actor_definition_sneak_uncovering_target_bit)) &&
			state_data->pursuit_location.type == 0 &&
			actor->target.target_type == 5 &&
			prop_get(actor->target.target_prop_index)->quantized_distance <= 2)
		{
			state_data->sneaking = TRUE;
		}
	}

	if (state_data->at_destination)
	{
		if (state_data->search_remaining_time > 0)
		{
			state_data->search_remaining_time -= 1;
		}
		if (state_data->search_remaining_time == 0)
		{
			state_data->search_done = TRUE;
		}
		if (actor->meta.unit_index != NONE)
		{
			if (state_data->pursuit_location.type == 0)
			{
				if (!actor->firing_positions.pursuit_communicated_lost_contact &&
					(state_data->search_done ||
						state_data->search_remaining_time + 3*TICKS_PER_SECOND <
							state_data->search_desired_time))
				{
					ai_communication_event(
						13,
						actor->meta.unit_index,
						actor_target_unit_index(actor_index),
						NONE,
						NONE,
						NONE,
						NULL);
					actor->firing_positions.pursuit_communicated_lost_contact = TRUE;
				}
			}
			else if (state_data->search_remaining_time == 0)
			{
				ai_communication_event(
					18,
					actor->meta.unit_index,
					actor_target_unit_index(actor_index),
					NONE,
					NONE,
					NONE,
					NULL);
			}
		}
	}
	else
	{
		if (!actor->control.moving && !actor->meta.swarm)
		{
			if (++state_data->search_failure_timer > 4*TICKS_PER_SECOND)
			{
				state_data->search_failed = TRUE;
				state_data->search_done = TRUE;
			}
		}
	}
	return;
}

void action_search_flush_position_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct search_state_data *state_data = &actor->state.action_data.search;

	if (state_data->pursuit_location.type == 1)
	{
		state_data->pursuit_location.firing_position_index = NONE;
		state_data->search_done = TRUE;
	}
	return;
}

void action_search_flush_structure_indices(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct search_state_data *state_data = &actor->state.action_data.search;

	state_data->pursuit_location.cluster_index = NONE;
	state_data->pursuit_location.surface_index = NONE;
	return;
}

void action_search_control(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	struct search_state_data *state_data = &actor->state.action_data.search;

	if (actor->control.moving)
	{
		actor->orders.look.primary_priority = 3;
		actor->orders.look.primary_direction.type = 0;
	}
	else
	{
		long minimum_search_time = MAX(state_data->search_desired_time/3, 3*TICKS_PER_SECOND);

		if (state_data->search_desired_time - state_data->search_remaining_time <
			minimum_search_time)
		{
			if (state_data->pursuit_location.type == 0)
			{
				actor->orders.look.primary_priority = 3;
				actor->orders.look.primary_direction.type = 2;
			}
			else if (state_data->pursuit_location.type == 1)
			{
				actor->orders.look.primary_priority = 3;
				actor->orders.look.primary_direction.type = 3;
				actor->orders.look.primary_direction.point = state_data->pursuit_location.position;
			}
			else
			{
				actor->orders.look.primary_priority = 1;
			}
		}
		else
		{
			actor->orders.look.primary_priority = 1;
		}
	}
	actor->orders.look.idle_look_type = 3;

	if (state_data->pursuit_location.type == 0)
	{
		if (definition->flags&FLAG(_actor_definition_shoot_at_targets_last_location_bit))
		{
			actor->orders.combat.shoot_at_target = actor->target.target_type >= 5;
		}
		else
		{
			actor->orders.combat.shoot_at_target = actor->target.target_type >= 6;
		}
	}

	actor->orders.move.stationary_crouch = state_data->sneaking;
	actor->orders.move.moving_crouch = state_data->sneaking;
	actor->orders.move.panicked = FALSE;
	actor->orders.move.dive_into_cover = FALSE;
	actor->orders.move.emerge_from_cover = TRUE;
	return;
}

void action_search_begin(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct actor_definition *definition = actor_definition_get(actor->meta.definition_index);
	struct search_state_data *state_data = &actor->state.action_data.search;
	long search_time;

	if (state_data->pursuit_location.type == 0)
	{
		search_time = (long)(real_random_range(
			definition->pursuit.target_location_time_lower_bound,
			definition->pursuit.target_location_time_upper_bound)*TICKS_PER_SECOND);
	}
	else
	{
		search_time = (long)(real_random_range(
			definition->pursuit.pursuit_position_time_lower_bound,
			definition->pursuit.pursuit_position_time_upper_bound)*TICKS_PER_SECOND);
	}
	state_data->search_desired_time = search_time;
	state_data->search_remaining_time = search_time;
	return;
}

boolean action_search_perform(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct search_state_data *state_data = &actor->state.action_data.search;
	struct prop_datum *prop;

	if (!actor->meta.swarm && actor->meta.timeslice && !state_data->search_done)
	{
		state_data->at_destination = TRUE;
		if (state_data->pursuit_location.type == 0 &&
			actor->target.target_prop_index != NONE)
		{
			real radius;

			prop = prop_get(actor->target.target_prop_index);
			radius = 1.7f;
			if (prop->line_of_sight)
			{
				radius = 0.7f;
			}
			state_data->at_destination = distance_squared3d(
				&prop->body_position,
				&actor->input.position.body_position) < radius*radius;
		}
		else if (state_data->pursuit_location.type == 1 &&
			state_data->pursuit_location.firing_position_index != NONE)
		{
			real distance = distance_squared3d(
				&state_data->pursuit_location.position,
				&actor->input.position.body_position);

			if (distance < 0.7f*0.7f)
			{
				state_data->at_destination = TRUE;
			}
			else if (distance > 2.5f*2.5f)
			{
				state_data->at_destination = FALSE;
			}
			else
			{
				real_point3d calculated_head_position;

				unit_estimate_position(
					actor->meta.unit_index,
					1,
					&state_data->pursuit_location.position,
					NULL,
					NULL,
					&calculated_head_position);
				state_data->at_destination = ai_test_line_of_sight(
					&actor->input.position.head_position,
					actor->input.position.body_location.cluster_index,
					&calculated_head_position,
					state_data->pursuit_location.cluster_index,
					0,
					0,
					NONE,
					actor->input.vehicle_index != NONE) == 0;
			}
		}

		if (!state_data->at_destination && !state_data->charging)
		{
			struct prop_iterator iterator;
			struct prop_datum *prop;
			short searching_actor_count = 0;
			short nearby_searching_actor_count = 0;

			prop_iterator_new(&iterator, actor_index);
			for (prop = prop_iterator_next(&iterator); prop;
				prop = prop_iterator_next(&iterator))
			{
				if (prop->state >= _prop_state_becoming_unacknowledged &&
					prop->state <= _prop_state_acknowledged &&
					!prop->enemy &&
					!prop->dead &&
					prop->actor_index != NONE &&
					actors_searching_same_position(actor_index, prop->actor_index))
				{
					struct actor_datum *other_actor = actor_get(prop->actor_index);

					searching_actor_count += 1;
					if (!other_actor->meta.swarm &&
						!other_actor->control.moving &&
						distance_squared3d(
							&actor->input.position.body_position,
							&other_actor->input.position.body_position) < 0.8f*0.8f)
					{
						nearby_searching_actor_count += 1;
					}
				}
			}
			if (!state_data->tenacious &&
				searching_actor_count >=
					(short)((state_data->pursuit_location.type == 1) ? 2 : 4))
			{
				long last_perceived_time;

				state_data->search_done = TRUE;
				last_perceived_time = NONE;
				if (actor->target.target_prop_index != NONE)
				{
					prop = prop_get(actor->target.target_prop_index);
					last_perceived_time = prop->last_perceived_time;
				}
				if (actor->meta.encounter_index != NONE)
				{
					encounter_mark_examined_pursuit_position(
						actor->meta.encounter_index,
						actor_index,
						state_data->pursuit_location.firing_position_index,
						last_perceived_time);
				}
			}
			else if (nearby_searching_actor_count > 0)
			{
				state_data->at_destination = TRUE;
			}
		}

		if (state_data->at_destination)
		{
			actor_move_halt(actor_index);
		}
		else if (state_data->pursuit_location.type == 0)
		{
			if (!actor_move_to_prop(actor_index, actor->target.target_prop_index, 2.5f))
			{
				state_data->search_done = TRUE;
				state_data->search_failed = TRUE;
			}
		}
		else if (state_data->pursuit_location.type == 1)
		{
			actor->firing_positions.current_position_index = NONE;
			if (!actor_move_to_firing_position(
				actor_index,
				state_data->pursuit_location.firing_position_index,
				NULL))
			{
				state_data->search_done = TRUE;
				state_data->search_failed = TRUE;
			}
		}
		else
		{
			actor_move_halt(actor_index);
		}
	}
	return state_data->search_done;
}

/* ---------- private code */
