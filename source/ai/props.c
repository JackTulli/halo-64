/*
PROPS.C

symbols in this file:
00052770 0040:
	_props_initialize (0000)
000527B0 0010:
	_props_dispose (0000)
000527C0 0010:
	_props_initialize_for_new_map (0000)
000527D0 0010:
	_props_dispose_from_old_map (0000)
000527E0 0260:
	_prop_add (0000)
00052A40 0030:
	_prop_new_blank (0000)
00052A70 0140:
	_prop_remove (0000)
00052BB0 0030:
	_prop_iterator_new (0000)
00052BE0 0030:
	_prop_iterator_next (0000)
00052C10 0220:
	_prop_new_unacknowledged (0000)
00052E30 00e0:
	_prop_setup_orphan (0000)
00052F10 00d0:
	_prop_orphan_transition (0000)
00052FE0 00f0:
	_prop_orphan_from_friend (0000)
000530D0 0020:
	_prop_orphan_update_information (0000)
000530F0 0030:
	_prop_delete (0000)
00053120 0090:
	_prop_get_active_by_unit_index (0000)
000531B0 0190:
	_prop_get_base_by_unit_index (0000)
0024ECD8 000a:
	??_C@_09OMCEPKNN@prop_data?$AA@ (0000)
0024ECE4 001a:
	??_C@_0BK@JLBOKELK@c?3?2halo?2SOURCE?2ai?2props?4c?$AA@ (0000)
0024ED00 002d:
	??_C@_0CN@BBGHFIGI@prop_unit?9?$DOobject?4type?5?$DN?$DN?5_objec@ (0000)
0024ED30 0025:
	??_C@_0CF@FLGLPGLE@unit_index?5?$CB?$DN?5actor?9?$DOmeta?4unit_i@ (0000)
0024ED58 0045:
	??_C@_0EF@KDOAGHLG@AI?5knowledge?5database?5?$CI?$CFd?5entrie@ (0000)
0024EDA0 00b7:
	??_C@_0LH@KLCHODAH@?$CB?$CI?$CIactor?9?$DOcontrol?4idle_minor_act@ (0000)
0024EE58 00b7:
	??_C@_0LH@OAMKKKBK@?$CB?$CI?$CIactor?9?$DOcontrol?4idle_major_act@ (0000)
0024EF10 00d9:
	??_C@_0NJ@EMHCADI@?$CB?$CI?$CIactor?9?$DOcontrol?4secondary_look@ (0000)
0024EFEC 002e:
	??_C@_0CO@LOLEJOIH@actor?9?$DOtarget?4target_prop_index?5@ (0000)
0024F01C 0020:
	??_C@_0CA@LGONIAIN@prop?9?$DOparent_prop_index?5?$DN?$DN?5NONE?$AA@ (0000)
0024F03C 0027:
	??_C@_0CH@DGNOKALK@parent_prop?9?$DOorphan_prop_index?5?$DN@ (0000)
0024F064 002e:
	??_C@_0CO@FIFBOHNC@parent_prop?9?$DOowner_actor_index?5?$DN@ (0000)
002B7D78 0004:
	_last_prop_data_full_warn_time (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "props.h"

#include "actors.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "memory/data.h"
#include "saved games/game_state.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

boolean game_team_is_enemy(short team_index0, short team_index1);
boolean game_team_is_ally(short team_index0, short team_index1);
boolean game_team_ally_status_changed(short team_index0, short team_index1);
void prop_position_refresh(
	long actor_index,
	long prop_index,
	struct actor_position_data *position,
	boolean force,
	boolean update_status);
void prop_status_refresh(
	long actor_index,
	long prop_index,
	struct actor_position_data *position);
/* ---------- globals */

long last_prop_data_full_warn_time = NONE;

/* ---------- public code */

void props_initialize(
	void)
{
	prop_data = game_state_data_new("prop", 768, sizeof(struct prop_datum));

	match_assert("c:\\halo\\SOURCE\\ai\\props.c", 0x24, prop_data);

	return;
}

void props_dispose(
	void)
{
	return;
}

void props_initialize_for_new_map(
	void)
{
	data_make_valid(prop_data);

	return;
}

void props_dispose_from_old_map(
	void)
{
	data_make_invalid(prop_data);

	return;
}

/* ---------- private code */

static void prop_add(
	long actor_index,
	long unit_index,
	long prop_index)
{
	if (prop_index == NONE)
	{
		long game_time = game_time_get();

		if (last_prop_data_full_warn_time == NONE ||
			game_time >= last_prop_data_full_warn_time + TICKS_PER_SECOND * 30)
		{
			error(
				_error_silent,
				"AI knowledge database (%d entries) is full (warns once every 30 sec)",
				768);
			last_prop_data_full_warn_time = game_time;
		}
	}
	else
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct prop_datum *prop = prop_get(prop_index);

		prop->owner_actor_index = actor_index;
		prop->unit_effect = NONE;
		prop->ticks_since_damage = NONE;
		prop->unit_index = unit_index;
		prop->currently_damaging_me = FALSE;
		prop->damage_inflicted_on_me = 0.0f;
		prop->ticks_since_definitely_located = NONE;
		prop->definitely_located = FALSE;
		prop->definite_knowledge_source_actor = NONE;
		prop->last_perceived_time = NONE;
		prop->last_visible_time = NONE;
		prop->orphan_corpse_cheated = FALSE;
		prop->actor_index = NONE;
		prop->orphan_prop_index = NONE;
		prop->required_ticks = 0;
		prop->last_unreachable_time = NONE;

		if (unit_index != NONE)
		{
			struct unit_datum *prop_unit = unit_get(unit_index);
			struct unit_definition *unit_definition =
				unit_definition_get(prop_unit->definition_index);

			match_assert(
				"c:\\halo\\SOURCE\\ai\\props.c",
				0xe8,
				unit_index != actor->meta.unit_index);
			match_assert(
				"c:\\halo\\SOURCE\\ai\\props.c",
				0xe9,
				prop_unit->object.type == _object_type_biped);

			prop->team_index = prop_unit->object.owner_team_index;
			prop->enemy = game_team_is_enemy(actor->meta.team_index, prop->team_index);
			prop->ally = game_team_is_ally(actor->meta.team_index, prop->team_index);
			prop->ally_status_changed =
				game_team_ally_status_changed(actor->meta.team_index, prop->team_index);
			prop->dead = TEST_FLAG(prop_unit->object.damage_flags, _object_dead_bit);
			prop->suicide_radius = unit_definition->unit.ai_danger_radius;
			prop->really_dead =
				prop->dead && prop_unit->unit.feign_death_timer == 0;
			prop->dead_ticks = prop->dead ? 1000 : 0;
			prop->player = prop_unit->object.owner_player_index != NONE;

			if (prop_unit->unit.swarm_actor_index != NONE)
			{
				prop->swarm = TRUE;
				prop->actor_index = prop_unit->unit.swarm_actor_index;
				prop->swarm_unit_selected_time = game_time_get();
			}
			else
			{
				prop->actor_index = prop_unit->unit.actor_index;
			}

			if (prop->player)
			{
				prop->type = 6;
			}
			else
			{
				if (prop->actor_index != NONE)
				{
					struct actor_datum *prop_actor = actor_get(prop->actor_index);

					prop->type = prop_actor->meta.type;
				}
				else
				{
					prop->type = NONE;
				}
			}
		}

		prop->next_prop_index = actor->meta.first_prop_index;
		actor->meta.first_prop_index = prop_index;
	}

	return;
}

long prop_new_blank(
	long actor_index)
{
	long prop_index = datum_new(prop_data);

	prop_add(actor_index, NONE, prop_index);

	return prop_index;
}

static void prop_remove(
	long actor_index,
	long prop_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	struct prop_datum *prop;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\props.c",
		0x19b,
		actor->target.target_prop_index != prop_index);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\props.c",
		0x19e,
		!((actor->control.secondary_look_type != _secondary_look_none) &&
		  (actor->control.secondary_look_direction.type == _direction_specification_prop) &&
		  (actor->control.secondary_look_direction.prop_index == prop_index)));
	match_assert(
		"c:\\halo\\SOURCE\\ai\\props.c",
		0x1a1,
		!((actor->control.idle_major_active) &&
		  (actor->control.idle_major_direction.type == _direction_specification_prop) &&
		  (actor->control.idle_major_direction.prop_index == prop_index)));
	match_assert(
		"c:\\halo\\SOURCE\\ai\\props.c",
		0x1a4,
		!((actor->control.idle_minor_active) &&
		  (actor->control.idle_minor_direction.type == _direction_specification_prop) &&
		  (actor->control.idle_minor_direction.prop_index == prop_index)));

	prop = prop_get(actor->meta.first_prop_index);
	if (actor->meta.first_prop_index != prop_index)
	{
		long *previous_next_prop_index;

		do
		{
			previous_next_prop_index = &prop->next_prop_index;
			prop = prop_get(prop->next_prop_index);
		}
		while (*previous_next_prop_index != prop_index);

		*previous_next_prop_index = prop->next_prop_index;
	}
	else
	{
		actor->meta.first_prop_index = prop->next_prop_index;
	}

	return;
}

void prop_iterator_new(
	struct prop_iterator *iterator,
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	iterator->next_index = actor->meta.first_prop_index;

	return;
}

struct prop_datum *prop_iterator_next(
	struct prop_iterator *iterator)
{
	struct prop_datum *prop = NULL;

	iterator->index = iterator->next_index;
	if (iterator->index != NONE)
	{
		prop = prop_get(iterator->index);
		iterator->next_index = prop->next_prop_index;
	}

	return prop;
}

long prop_new_unacknowledged(
	long actor_index,
	long unit_index,
	boolean enemy)
{
	long prop_index;
	long worst_prop_index = NONE;
	long worst_required_prop_index = NONE;
	real worst_distance = REAL_MAX;
	real worst_required_distance = REAL_MAX;
	short required_prop_count = 0;
	short required_prop_maximum;
	struct prop_iterator iterator;
	struct prop_datum *prop;

	prop_iterator_new(&iterator, actor_index);
	while ((prop = prop_iterator_next(&iterator)) != NULL)
	{
		if (prop->state >= _prop_state_uninspected_orphan &&
			prop->state <= _prop_state_inspected_orphan)
		{
			continue;
		}
		if (prop->orphan_prop_index != NONE)
		{
			continue;
		}

		{
			boolean replace = FALSE;
			boolean desire_prop = actor_perception_desire_prop(
				actor_index,
				NONE,
				prop->unit_index,
				prop->actor_index,
				prop->in_use,
				prop->player,
				prop->enemy,
				prop->dead,
				prop->dead_ticks,
				prop->suicide_radius,
				prop->distance * prop->distance,
				prop->required_ticks,
				&replace);

			if (!desire_prop)
			{
				if (prop->distance < worst_distance)
				{
					worst_prop_index = iterator.index;
					worst_distance = prop->distance;
				}
			}
			else if (prop->enemy == enemy)
			{
				required_prop_count++;

				if (replace && prop->distance < worst_required_distance)
				{
					worst_required_prop_index = iterator.index;
					worst_required_distance = prop->distance;
				}
			}
		}
	}

	if (worst_prop_index != NONE)
	{
		prop_index = worst_prop_index;
	}
	else
	{
		prop_index = NONE;
		required_prop_maximum = enemy ? 6 : 4;
		if (worst_required_prop_index != NONE &&
			required_prop_count >= required_prop_maximum)
		{
			prop_index = worst_required_prop_index;
		}
	}

	if (prop_index == NONE)
	{
		prop_index = datum_new(prop_data);
	}
	else
	{
		short identifier;

		prop = prop_get(prop_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x9e,
			prop->orphan_prop_index == NONE);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x9f,
			prop->parent_prop_index == NONE);

		actor_switch_props(actor_index, prop_index, NONE);
		prop_remove(actor_index, prop_index);
		identifier = prop->identifier;
		memset(prop, 0, sizeof(*prop));
		prop->identifier = identifier;
	}

	prop_add(actor_index, unit_index, prop_index);

	return prop_index;
}

static void prop_setup_orphan(
	long actor_index,
	long orphan_prop_index,
	long source_prop_index)
{
	struct prop_datum *source_prop = prop_get(source_prop_index);
	struct prop_datum *prop = prop_get(orphan_prop_index);
	short identifier;
	long owner_actor_index;
	long next_prop_index;
	long preserved_parent_prop_index;

	identifier = prop->identifier;
	owner_actor_index = prop->owner_actor_index;
	next_prop_index = prop->next_prop_index;
	preserved_parent_prop_index = prop->parent_prop_index;

	memcpy(prop, source_prop, sizeof(*prop));
	prop->owner_actor_index = owner_actor_index;
	prop->next_prop_index = next_prop_index;
	prop->identifier = identifier;
	prop->parent_prop_index = preserved_parent_prop_index;
	prop->state = _prop_state_uninspected_orphan;
	prop->orphan_lifespan_ticks = TICKS_PER_SECOND * 30;
	prop->orphan_inspection_ticks = 0;
	prop->tried_to_uncover = FALSE;
	prop->tried_to_search = FALSE;
	prop->abandoned_search = FALSE;
	vector_from_points3d(
		&prop->last_perceived_body_position,
		&prop->body_position,
		&prop->orphan_hint_vector);
	prop->velocity = *global_zero_vector3d;
	prop->quantized_speed = 0;

	return;
}

long prop_orphan_transition(
	long actor_index,
	long parent_prop_index)
{
	long prop_index = datum_new(prop_data);

	prop_add(actor_index, NONE, prop_index);
	if (prop_index != NONE)
	{
		struct prop_datum *parent_prop = prop_get(parent_prop_index);
		struct prop_datum *prop = prop_get(prop_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x155,
			parent_prop->owner_actor_index == actor_index);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x156,
			parent_prop->orphan_prop_index == NONE);

		prop_setup_orphan(actor_index, prop_index, parent_prop_index);
		parent_prop->orphan_prop_index = prop_index;
		prop->parent_prop_index = parent_prop_index;
	}

	return prop_index;
}

long prop_orphan_from_friend(
	long actor_index,
	long unacknowledged_prop_index,
	long friend_acknowledged_prop_index)
{
	long prop_index = datum_new(prop_data);

	prop_add(actor_index, NONE, prop_index);
	if (prop_index != NONE)
	{
		struct prop_datum *parent_prop = prop_get(unacknowledged_prop_index);
		struct prop_datum *prop = prop_get(prop_index);
		struct prop_datum *friend_prop = prop_get(friend_acknowledged_prop_index);

		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x16d,
			parent_prop->owner_actor_index == actor_index);
		match_assert(
			"c:\\halo\\SOURCE\\ai\\props.c",
			0x16e,
			parent_prop->orphan_prop_index == NONE);

		prop_setup_orphan(actor_index, prop_index, friend_acknowledged_prop_index);
		parent_prop->orphan_prop_index = prop_index;
		prop->parent_prop_index = unacknowledged_prop_index;

		if (friend_prop->state >= _prop_state_uninspected_orphan &&
			friend_prop->state <= _prop_state_inspected_orphan)
		{
			prop->state = friend_prop->state;
		}
	}

	return prop_index;
}

void prop_orphan_update_information(
	long actor_index,
	long orphan_prop_index,
	long friend_acknowledged_prop_index)
{
	prop_setup_orphan(actor_index, orphan_prop_index, friend_acknowledged_prop_index);

	return;
}

void prop_delete(
	long actor_index,
	long prop_index)
{
	prop_remove(actor_index, prop_index);
	datum_delete(prop_data, prop_index);

	return;
}

long prop_get_active_by_unit_index(
	long actor_index,
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	long target_actor_index = unit->unit.swarm_actor_index != NONE ?
		unit->unit.swarm_actor_index :
		unit->unit.actor_index;
	struct prop_iterator iterator;
	struct prop_datum *prop;
	long result = NONE;

	prop_iterator_new(&iterator, actor_index);
	while ((prop = prop_iterator_next(&iterator)) != NULL)
	{
		if (prop->state >= _prop_state_unacknowledged &&
			prop->state <= _prop_state_becoming_acknowledged)
		{
			continue;
		}
		if (prop->unit_index == unit_index ||
			(prop->swarm &&
			 prop->actor_index != NONE &&
			 prop->actor_index == target_actor_index))
		{
			break;
		}
	}

	if (prop != NULL)
	{
		result = iterator.index;
	}

	return result;
}

long prop_get_base_by_unit_index(
	long actor_index,
	long unit_index,
	boolean create_if_missing,
	boolean update_status)
{
	long result = NONE;

	if (unit_index != NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct unit_datum *unit = unit_get(unit_index);
		long target_actor_index;

		if (unit->unit.swarm_actor_index != NONE)
		{
			target_actor_index = unit->unit.swarm_actor_index;
		}
		else
		{
			target_actor_index = unit->unit.actor_index;
		}

		if (unit->object.type == _object_type_biped &&
			target_actor_index != actor_index)
		{
			struct prop_iterator iterator;
			struct prop_datum *prop;

			prop_iterator_new(&iterator, actor_index);
			while ((prop = prop_iterator_next(&iterator)) != NULL)
			{
				if (prop->unit_index == unit_index ||
					(prop->swarm &&
					 prop->actor_index != NONE &&
					 prop->actor_index == target_actor_index))
				{
					break;
				}
			}

			if (prop != NULL)
			{
				result = prop->orphan_prop_index == NONE ?
					iterator.index :
					prop->orphan_prop_index;
			}

			if (result == NONE && create_if_missing && actor->meta.active)
			{
				boolean enemy =
					game_team_is_enemy(actor->meta.team_index, unit->object.owner_team_index);
				result = prop_new_unacknowledged(actor_index, unit_index, enemy);
				if (result != NONE)
				{
					struct actor_position_data position;
					struct prop_datum *new_prop = prop_get(result);

					prop_position_refresh(
						actor_index,
						result,
						&position,
						FALSE,
						update_status);
					new_prop->required_ticks = TICKS_PER_SECOND;
					new_prop->delay_requirement_decision = TRUE;

					if (update_status)
					{
						prop_status_refresh(actor_index, result, &position);
						if (new_prop->perception >= 2)
						{
							boolean expected_acknowledgement =
								actor_expected_acknowledgement(actor_index, result);

							new_prop->state = _prop_state_acknowledged;
							actor_perception_acknowledge(
								actor_index,
								result,
								FALSE,
								expected_acknowledgement);
						}
					}
				}
			}
		}
	}

	return result;
}
