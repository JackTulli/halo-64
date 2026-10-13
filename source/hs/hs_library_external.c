/*
HS_LIBRARY_EXTERNAL.C

symbols in this file:
000B8960 0010:
	_hs_not (0000)
000B8970 0020:
	_hs_print (0000)
000B8990 0060:
	_hs_players (0000)
000B89F0 00b0:
	_hs_trigger_volume_test_objects (0000)
000B8AA0 0070:
	_hs_unit_can_see_object (0000)
000B8B10 0080:
	_hs_objects_can_see_object (0000)
000B8B90 0050:
	_hs_unit_can_see_flag (0000)
000B8BE0 00a0:
	_hs_objects_can_see_flag (0000)
000B8C80 00b0:
	_code_000b8c80 (0000)
000B8D30 0050:
	_hs_object_create (0000)
000B8D80 0040:
	_hs_object_destroy (0000)
000B8DC0 0030:
	_hs_object_destroy_by_name (0000)
000B8DF0 00c0:
	_hs_object_destroy_all (0000)
000B8EB0 0080:
	_code_000b8eb0 (0000)
000B8F30 0020:
	_hs_object_create_containing (0000)
000B8F50 0020:
	_hs_object_destroy_containing (0000)
000B8F70 0040:
	_hs_object_list_get_element (0000)
000B8FB0 0070:
	_hs_object_set_shield (0000)
000B9020 00c0:
	_hs_object_set_permutation (0000)
000B90E0 0040:
	_hs_objects_predict (0000)
000B9120 0060:
	_hs_objects_delete_by_definition (0000)
000B9180 0070:
	_hs_effect_new (0000)
000B91F0 0070:
	_hs_effect_new_from_object_marker (0000)
000B9260 0070:
	_hs_damage_new (0000)
000B92D0 0060:
	_hs_damage_object (0000)
000B9330 0080:
	_code_000b9330 (0000)
000B93B0 0020:
	_hs_sound_get_gain (0000)
000B93D0 0020:
	_hs_sound_set_gain (0000)
000B93F0 00a0:
	_hs_trigger_volume_test_objects_all (0000)
000B9490 0020:
	_hs_trigger_volume_test_objects_any (0000)
000B94B0 0030:
	_hs_object_create_anew (0000)
000B94E0 0020:
	_hs_object_create_anew_containing (0000)
000B9500 0290:
	_code_000b9500 (0000)
000B9790 0020:
	_hs_object_teleport (0000)
000B97B0 0020:
	_hs_object_set_facing (0000)
000B97D0 0080:
	_hs_teleport_players_not_in_trigger_volume (0000)
0026E798 002d:
	??_C@_0CN@IIANAKMD@WARNING?3?5object_create?5?9?5?8?$CFs?8?5al@ (0000)
0026E7C8 005f:
	??_C@_0FP@BIAHJJOL@?$CD?$CD?$CD?5ERROR?5a?5script?5tried?5to?5dele@ (0000)
0026E828 0028:
	??_C@_0CI@GDKOMGNN@c?3?2halo?2SOURCE?2hs?2hs_library_ext@ (0000)
0026E850 001e:
	??_C@_0BO@NDHEDPHG@the?5sound?5?8?$CFs?8?5does?5not?5exist?$AA@ (0000)
0026E870 0010:
	??_C@_0BA@NMABAOHF@?$CGflag?9?$DOposition?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "main/console.h"
#include "memory/data.h"
#include "models/model_definitions.h"
#include "objects/damage.h"
#include "objects/objects.h"
#include "effects/effects.h"
#include "game/player_control.h"
#include "game/players.h"
#include "items/items.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "sound/sound_definitions.h"
#include "units/units.h"

/* ---------- constants */

/* ---------- macros */

#define hs_sound_definition_get(index) \
	((struct hs_sound_definition *)tag_get(SOUND_DEFINITION_TAG, (index)))
#define hs_looping_sound_definition_get(index) \
	((struct hs_looping_sound_definition *)tag_get(LOOPING_SOUND_DEFINITION_TAG, (index)))
#define hs_item_datum_from_object(object) \
	((struct item_datum *)(object))

/* ---------- structures */

struct scenario_cutscene_flag
{
	long runtime_unused;
	char name[TAG_STRING_LENGTH];
	real_point3d position;
	real_euler_angles2d facing;
	byte unused[0x24];
};

struct hs_sound_definition
{
	byte unused00[0x28];
	real gain;
};

struct hs_looping_sound_track
{
	long unknown0;
	real gain;
	byte unused08[0x98];
};

struct hs_looping_sound_definition
{
	byte unused00[0x3C];
	struct tag_block tracks;
};

/* ---------- prototypes */

long object_index_from_name_index(
	short name_index);
long object_list_new(
	void);
void object_list_add(
	long object_list_index,
	long object_index);
long object_list_get_first(
	long object_list_index,
	long *reference_index);
long object_list_get_next(
	long object_list_index,
	long *reference_index);
boolean hs_trigger_volume_test_objects(
	short trigger_volume_index,
	long object_list_index,
	boolean all);
void hs_object_create(
	short object_name_index);
void hs_object_create_anew(
	short object_name_index);
void hs_object_destroy(
	long object_index);
void terminal_printf(
	union real_argb_color const *color,
	char const *format,
	...);
void area_of_effect_cause_damage(
	struct damage_data *damage,
	long unlucky_object_index);
boolean player_teleport(
	long player_index,
	long source_unit_index,
	real_point3d const *position);
void unit_exit_seat_end(
	long unit_index);

/* ---------- globals */

extern unsigned long hs_debug_data[];

/* ---------- public code */

boolean hs_not(
	boolean value)
{
	return !value;
}

void hs_print(
	char const *message)
{
	/* BUG (preserved for exact matching): the printed string is passed as the format
	 * (January 0x4b8970 +0x0c pushes it as terminal_printf's format), so a '%' in the
	 * text, from a scenario script or typed at this build's console, reads arguments
	 * that were never passed. A corrected build should print it through "%s".
	 * Source-policy approval pending (2026-09-27 audit). */
	terminal_printf(global_real_argb_green, message);

	return;
}

long hs_players(
	void)
{
	long object_list_index;
	long player_index;

	object_list_index = object_list_new();
	for (player_index = data_next_index(player_data, NONE);
		player_index != NONE;
		player_index = data_next_index(player_data, player_index))
	{
		struct player_datum *player;

		player = player_get(player_index);
		if (player->unit_index != NONE)
			object_list_add(object_list_index, player->unit_index);
	}

	return object_list_index;
}

boolean hs_trigger_volume_test_objects(
	short trigger_volume_index,
	long object_list_index,
	boolean all)
{
	long reference_index;
	long object_index;
	boolean result;

	result = all;
	object_index = object_list_get_first(object_list_index, &reference_index);
	while (object_index != NONE)
	{
		if (scenario_trigger_volume_test_object(
			trigger_volume_index,
			object_index))
		{
			if (!result)
			{
				result = TRUE;
				break;
			}
		}
		else if (result)
		{
			result = FALSE;
			break;
		}

		object_index = object_list_get_next(
			object_list_index,
			&reference_index);
	}

	BIT_VECTOR_SET_FLAG(hs_debug_data, trigger_volume_index, result);

	return result;
}

boolean hs_unit_can_see_object(
	long unit_index,
	long object_index,
	real degrees)
{
	boolean result;

	result = FALSE;
	if (object_index != NONE)
	{
		real_point3d target_point;

		if (unit_try_and_get(object_index))
			unit_get_head_position(object_index, &target_point);
		else
			target_point = object_get(object_index)->object.bounding_sphere_center;

		result = unit_can_see_point(
			unit_index,
			&target_point,
			DEGREES_TO_RADIANS(degrees));
	}

	return result;
}

boolean hs_objects_can_see_object(
	long object_list_index,
	long object_index,
	real degrees)
{
	long reference_index;
	long unit_index;
	boolean result;

	result = FALSE;
	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		if (unit_try_and_get(unit_index) &&
			hs_unit_can_see_object(unit_index, object_index, degrees))
		{
			result = TRUE;
			break;
		}

		unit_index = object_list_get_next(
			object_list_index,
			&reference_index);
	}

	return result;
}

boolean hs_unit_can_see_flag(
	long unit_index,
	short cutscene_flag_index,
	real degrees)
{
	boolean result;

	result = FALSE;
	if (cutscene_flag_index)
	{
		result = unit_can_see_point(
			unit_index,
			&TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->cutscene_flags,
				cutscene_flag_index,
				struct scenario_cutscene_flag)->position,
			DEGREES_TO_RADIANS(degrees));
	}

	return result;
}

boolean hs_objects_can_see_flag(
	long object_list_index,
	short cutscene_flag_index,
	real degrees)
{
	long reference_index;
	long unit_index;

	unit_index = object_list_get_first(object_list_index, &reference_index);
	while (unit_index != NONE)
	{
		if (unit_try_and_get(unit_index) &&
			hs_unit_can_see_flag(unit_index, cutscene_flag_index, degrees))
		{
			return TRUE;
		}

		unit_index = object_list_get_next(
			object_list_index,
			&reference_index);
	}

	return FALSE;
}

boolean code_000b8c80(
	long object_index)
{
	struct object_datum *object;
	boolean result;

	object = object_get(object_index);
	result = player_index_from_unit_index(object_index) != NONE;
	if (!result)
	{
		long child_object_index;

		child_object_index = object->object.first_child_object_index;
		while (child_object_index != NONE)
		{
			struct object_datum *child_object;

			child_object = object_get(child_object_index);
			if (code_000b8c80(child_object_index))
			{
				result = TRUE;
				break;
			}

			child_object_index = child_object->object.next_object_index;
		}
	}

	if (!result)
	{
		long parent_object_index;

		parent_object_index = object->object.parent_object_index;
		while (parent_object_index != NONE)
		{
			struct object_datum *parent_object;

			parent_object = object_get(parent_object_index);
			if (player_index_from_unit_index(parent_object_index) != NONE)
			{
				result = TRUE;
				break;
			}

			parent_object_index = parent_object->object.parent_object_index;
		}
	}

	if (!result &&
		TEST_FLAG(_object_mask_item, object->object.type) &&
		TEST_FLAG(
			hs_item_datum_from_object(object)->item.flags,
			_item_belongs_to_player_bit))
	{
		result = TRUE;
	}

	return result;
}

void hs_object_create(
	short object_name_index)
{
	if (object_name_index != NONE)
	{
		long object_index;

		object_index = object_index_from_name_index(object_name_index);
		if (object_index != NONE)
		{
			struct scenario_object_name *object_name;

			object_name = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->object_names,
				object_name_index,
				struct scenario_object_name);
			error(
				_error_silent,
				"WARNING: object_create - '%s' already exists",
				object_name->name);
		}
		else
			object_new_by_name(object_name_index);
	}

	return;
}

void hs_object_destroy(
	long object_index)
{
	if (object_index != NONE)
	{
		if (!code_000b8c80(object_index))
		{
			object_delete(object_index);
			return;
		}

		error(
			_error_silent,
			"### ERROR a script tried to delete the player (or the horse he rode in on, or his six-shooter)");
	}

	return;
}

void hs_object_destroy_by_name(
	short object_name_index)
{
	if (object_name_index != NONE)
	{
		long object_index;

		object_index = object_index_from_name_index(object_name_index);
		if (object_index != NONE)
			hs_object_destroy(object_index);
	}

	return;
}

void hs_object_destroy_all(
	void)
{
	{
		struct data_iterator iterator;
		struct player_datum *player;

		data_iterator_new(&iterator, player_data);
		for (player = data_iterator_next(&iterator);
			player != NULL;
			player = data_iterator_next(&iterator))
		{
			if (player->unit_index != NONE &&
				object_get_ultimate_parent(player->unit_index) !=
					player->unit_index)
			{
				unit_exit_seat_end(player->unit_index);
			}
		}
	}

	{
		struct object_iterator iterator;
		struct object_datum *object;

		object_iterator_new(&iterator, _object_mask_all, 0);
		for (object = object_iterator_next(&iterator);
			object != NULL;
			object = object_iterator_next(&iterator))
		{
			if (object->object.parent_object_index == NONE &&
				!code_000b8c80(iterator.index))
			{
				object_delete(iterator.index);
			}
		}
	}

	return;
}

static void code_000b8eb0(
	char const *name_string,
	void (*iterator)(short object_name_index))
{
	struct scenario *scenario;
	struct tag_block *object_names;
	short object_name_index;

	scenario = global_scenario_get();
	match_assert(
		"c:\\halo\\SOURCE\\hs\\hs_library_external.c",
		0x197,
		iterator);
	object_names = &scenario->object_names;
	for (object_name_index = 0;
		object_name_index < object_names->count;
		object_name_index++)
	{
		struct scenario_object_name *object_name;

		object_name = TAG_BLOCK_GET_ELEMENT(
			object_names,
			object_name_index,
			struct scenario_object_name);
		if (strstr(object_name->name, name_string))
			iterator(object_name_index);
	}

	return;
}

void hs_object_create_containing(
	char const *name_string)
{
	code_000b8eb0(name_string, hs_object_create);

	return;
}

void hs_object_destroy_containing(
	char const *name_string)
{
	code_000b8eb0(name_string, hs_object_destroy_by_name);

	return;
}

long hs_object_list_get_element(
	long object_list_index,
	short element_index)
{
	long reference_index;
	long object_index;

	object_index = object_list_get_first(object_list_index, &reference_index);
	while (element_index > 0 && object_index != NONE)
	{
		object_index = object_list_get_next(object_list_index, &reference_index);
		element_index--;
	}

	return object_index;
}

void hs_object_set_shield(
	long object_index,
	real shield_vitality)
{
	if (object_index != NONE)
	{
		struct object_datum *object;

		object = object_get(object_index);
		if (shield_vitality < 0.f)
			shield_vitality = 0.f;
		else if (shield_vitality > 1.f)
			shield_vitality = 1.f;
		object->object.shield_vitality =
			object->object.maximum_shield_vitality * shield_vitality;
	}

	return;
}

void hs_object_set_permutation(
	long object_index,
	char const *region_name,
	char *permutation_name)
{
	if (object_index != NONE)
	{
		struct object_datum *object;
		struct object_definition *object_definition;
		short desired_region_index;

		object = object_get(object_index);
		object_definition = object_definition_get(object->definition_index);
		desired_region_index = NONE;
		if (strcmp(region_name, ""))
		{
			if (object_definition->object.model.index != NONE)
			{
				struct model *model;
				short region_index;

				model = model_definition_get(
					object_definition->object.model.index);
				for (region_index = 0;
					region_index < model->regions.count;
					region_index++)
				{
					struct model_region *region;

					region = TAG_BLOCK_GET_ELEMENT(
						&model->regions,
						region_index,
						struct model_region);
					if (!_stricmp(region->name, region_name))
					{
						desired_region_index = region_index;
						break;
					}
				}
			}
		}
		object_permute_region(
			object_index,
			permutation_name,
			desired_region_index,
			TRUE);
	}

	return;
}

void hs_objects_predict(
	long object_list_index)
{
	long reference_index;
	long object_index;

	object_index = object_list_get_first(object_list_index, &reference_index);
	while (object_index != NONE)
	{
		object_predict(object_index);
		object_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void hs_objects_delete_by_definition(
	long definition_index)
{
	struct object_iterator iterator;
	struct object_datum *object;

	object_iterator_new(&iterator, _object_mask_all, 0);
	while ((object = object_iterator_next(&iterator)) != NULL)
	{
		if (object->definition_index == definition_index)
			object_delete(iterator.index);
	}
	objects_memory_compact();

	return;
}

void hs_effect_new(
	long effect_definition_index,
	short cutscene_flag_index)
{
	struct scenario_cutscene_flag *cutscene_flag;
	real_vector3d forward;

	cutscene_flag = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->cutscene_flags,
		cutscene_flag_index,
		struct scenario_cutscene_flag);
	vector3d_from_euler_angles2d(&forward, &cutscene_flag->facing);
	effect_new_unattached_from_markers(
		effect_definition_index,
		NONE,
		global_zero_vector3d,
		1,
		NULL,
		&cutscene_flag->position,
		&forward,
		1.f,
		1.f,
		NULL,
		NULL,
		TRUE);

	return;
}

void hs_effect_new_from_object_marker(
	long effect_definition_index,
	long object_index,
	char const *marker_name)
{
	if (effect_definition_index != NONE)
	{
		if (object_index != NONE)
		{
			struct object_marker marker;

			if (object_get_marker_by_name(
				object_index,
				marker_name,
				&marker,
				1))
			{
				effect_new_attached_from_markers(
					effect_definition_index,
					NONE,
					object_index,
					marker.node_index,
					1,
					&marker_name,
					&marker.matrix.position,
					&marker.matrix.forward,
					1.f,
					1.f,
					NULL,
					NULL);
			}
		}
	}

	return;
}

void hs_damage_new(
	long damage_effect_index,
	short cutscene_flag_index)
{
	struct scenario_cutscene_flag *cutscene_flag;
	struct damage_data damage;

	cutscene_flag = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->cutscene_flags,
		cutscene_flag_index,
		struct scenario_cutscene_flag);
	damage_data_new(&damage, damage_effect_index);
	damage.origin = damage.epicenter = cutscene_flag->position;
	scenario_location_from_point(&damage.location, &cutscene_flag->position);
	area_of_effect_cause_damage(&damage, NONE);

	return;
}

void hs_damage_object(
	long damage_effect_index,
	long object_index)
{
	if (object_index != NONE)
	{
		struct damage_data damage;

		damage_data_new(&damage, damage_effect_index);
		object_get_origin(object_index, &damage.origin);
		damage.epicenter = damage.origin;
		scenario_location_from_point(&damage.location, &damage.origin);
		object_cause_damage(
			&damage,
			object_index,
			NONE,
			NONE,
			NONE,
			NULL);
	}

	return;
}

static real *code_000b9330(
	char const *tag_name)
{
	long sound_index;
	struct hs_sound_definition *sound;
	struct hs_looping_sound_definition *looping_sound;
	struct hs_looping_sound_track *track;

	sound_index = tag_loaded(SOUND_DEFINITION_TAG, tag_name);
	if (sound_index != NONE)
	{
		sound = hs_sound_definition_get(sound_index);
		return &sound->gain;
	}

	sound_index = tag_loaded(LOOPING_SOUND_DEFINITION_TAG, tag_name);
	if (sound_index != NONE)
	{
		looping_sound = hs_looping_sound_definition_get(sound_index);
		if (looping_sound->tracks.count > 0)
		{
			track = TAG_BLOCK_GET_ELEMENT(
				&looping_sound->tracks,
				0,
				struct hs_looping_sound_track);
			return &track->gain;
		}
	}

	console_printf(FALSE, "the sound '%s' does not exist", tag_name);
	return NULL;
}

real hs_sound_get_gain(
	char const *tag_name)
{
	real *gain_reference;

	gain_reference = code_000b9330(tag_name);
	if (gain_reference)
		return *gain_reference;

	return 0.f;
}

void hs_sound_set_gain(
	char const *tag_name,
	real gain)
{
	real *gain_reference;

	gain_reference = code_000b9330(tag_name);
	if (gain_reference)
		*gain_reference = gain;

	return;
}

boolean hs_trigger_volume_test_objects_all(
	short trigger_volume_index,
	long object_list_index)
{
	return hs_trigger_volume_test_objects(
		trigger_volume_index,
		object_list_index,
		TRUE);
}

boolean hs_trigger_volume_test_objects_any(
	short trigger_volume_index,
	long object_list_index)
{
	return hs_trigger_volume_test_objects(
		trigger_volume_index,
		object_list_index,
		FALSE);
}

void hs_object_create_anew(
	short object_name_index)
{
	if (object_name_index != NONE)
	{
		long object_index;

		object_index = object_index_from_name_index(object_name_index);
		if (object_index != NONE)
			hs_object_destroy(object_index);

		hs_object_create(object_name_index);
	}

	return;
}

void hs_object_create_anew_containing(
	char const *name_string)
{
	code_000b8eb0(name_string, hs_object_create_anew);

	return;
}

static void code_000b9500(
	long object_index,
	short cutscene_flag_index,
	boolean set_position,
	boolean set_facing)
{
	if (object_index != NONE)
	{
		struct object_datum *object;
		struct scenario_cutscene_flag *flag;
		struct player_datum *player;
		struct unit_datum *unit;
		real_vector3d forward;

		object = object_get(object_index);
		flag = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->cutscene_flags,
			cutscene_flag_index,
			struct scenario_cutscene_flag);
		player = NULL;

		match_assert_valid_real_point3d(
			"c:\\halo\\SOURCE\\hs\\hs_library_external.c",
			0x1CC,
			&flag->position);

		if (set_position && object->object.parent_object_index != NONE)
		{
			unit = unit_try_and_get(object_index);
			if (unit)
				unit_exit_seat_end(object_index);
			else
				object_detach(object_index);
		}

		vector3d_from_euler_angles2d(&forward, &flag->facing);
		match_assert_valid_real_normal3d(
			"c:\\halo\\SOURCE\\hs\\hs_library_external.c",
			0x1DF,
			&forward);

		object_reset(object_index);
		unit = unit_try_and_get(object_index);
		if (unit)
		{
			long player_index;
			real_vector3d unit_forward;

			player_index = player_index_from_unit_index(object_index);
			if (unit->object.parent_object_index != NONE)
			{
				real_matrix4x3 inverse_matrix;

				matrix4x3_inverse(
					object_get_node_matrix(
						unit->object.parent_object_index,
						unit->object.parent_node_index),
					&inverse_matrix);
				matrix4x3_transform_normal(
					&inverse_matrix,
					&forward,
					&unit_forward);
			}
			else
				unit_forward = forward;

			if (set_facing)
			{
				unit->unit.desired_facing_vector = forward;
				unit->unit.desired_aiming_vector = forward;
				unit->unit.desired_looking_vector = forward;
			}

			if (player_index != NONE)
			{
				player = player_get(player_index);
				if (set_position)
					player_teleport(player_index, NONE, &flag->position);

				if (set_facing && player->local_player_index != NONE)
					player_control_set_facing(
						player->local_player_index,
						&unit_forward);
			}
		}

		object_set_position(
			object_index,
			set_position && !player ? &flag->position : NULL,
			set_facing && !player ? &forward : NULL,
			NULL);
	}

	return;
}

void hs_object_teleport(
	long object_index,
	short cutscene_flag_index)
{
	code_000b9500(object_index, cutscene_flag_index, TRUE, TRUE);

	return;
}

void hs_object_set_facing(
	long object_index,
	short cutscene_flag_index)
{
	code_000b9500(object_index, cutscene_flag_index, FALSE, TRUE);

	return;
}

void hs_teleport_players_not_in_trigger_volume(
	short trigger_volume_index,
	short cutscene_flag_index)
{
	long player_index;

	for (player_index = data_next_index(player_data, NONE);
		player_index != NONE;
		player_index = data_next_index(player_data, player_index))
	{
		struct player_datum *player;

		player = player_get(player_index);
		if (player->unit_index != NONE &&
			!scenario_trigger_volume_test_object(
				trigger_volume_index,
				player->unit_index))
		{
			code_000b9500(
				player->unit_index,
				cutscene_flag_index,
				TRUE,
				TRUE);
		}
	}

	return;
}

/* ---------- private code */
