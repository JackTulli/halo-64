/*
DEVICES.C

symbols in this file:
000846D0 0040:
	_devices_initialize (0000)
00084710 0010:
	_devices_dispose (0000)
00084720 0010:
	_devices_dispose_from_old_map (0000)
00084730 0050:
	_device_new (0000)
00084780 0200:
	_device_export_function_values (0000)
00084980 0160:
	_device_preprocess_node_orientations (0000)
00084AE0 0030:
	_device_get_position (0000)
00084B10 0030:
	_device_get_power (0000)
00084B40 0040:
	_device_set_never_appears_locked (0000)
00084B80 00e0:
	_device_group_set_actual_value (0000)
00084C60 0040:
	_device_one_sided_set (0000)
00084CA0 0040:
	_device_operates_automatically_set (0000)
00084CE0 0040:
	_device_group_change_only_once_more_set (0000)
00084D20 0020:
	_device_group_get_value (0000)
00084D40 0050:
	_device_touched (0000)
00084D90 0080:
	_device_can_change_position (0000)
00084E10 00b0:
	_device_effect_new (0000)
00084EC0 0070:
	_device_group_new (0000)
00084F30 0040:
	_device_group_delete (0000)
00084F70 00e0:
	_create_initial_device_groups (0000)
00085050 0020:
	_devices_initialize_for_new_map (0000)
00085070 0090:
	_device_delete (0000)
00085100 02e0:
	_device_update (0000)
000853E0 01b0:
	_device_render_debug (0000)
00085590 0120:
	_device_group_set_desired_value (0000)
000856B0 0040:
	_device_set_actual_position (0000)
000856F0 0120:
	_device_add_scenario_information (0000)
00085810 0080:
	_device_frontfacing (0000)
00085890 0040:
	_device_set_desired_position (0000)
000858D0 0050:
	_device_set_power (0000)
00259660 0013:
	??_C@_0BD@NKDNFDBG@device_groups_data?$AA@ (0000)
00259674 0021:
	??_C@_0CB@OENHPILN@c?3?2halo?2SOURCE?2devices?2devices?4c@ (0000)
00259698 000e:
	??_C@_0O@ICMOFGNG@device?5groups?$AA@ (0000)
002596A8 001b:
	??_C@_0BL@ILKEPPAG@no?5more?5free?5device?5groups?$AA@ (0000)
002596C4 001d:
	??_C@_0BN@DNCPCHGG@new_group_index?$DN?$DNgroup_index?$AA@ (0000)
002596E4 0019:
	??_C@_0BJ@GNCMHNPF@?$HMnposition?5?$CF?42f?1vel?5?$CF?42f?$AA@ (0000)
00259700 0019:
	??_C@_0BJ@JFHLBPID@?5?$CIgroup?5?$CFd?5desired?5?$CF?42f?$CJ?$AA@ (0000)
0025971C 0014:
	??_C@_0BE@JLADPBDO@power?5?$CF?42f?1vel?5?$CF?42f?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "devices.h"
#include "device_controls.h"
#include "device_definitions.h"
#include "device_machines.h"
#include "cache/cache_files.h"
#include "effects/effect_definitions.h"
#include "effects/effects.h"
#include "memory/data.h"
#include "models/model_animation_definitions.h"
#include "render/render_debug.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"

/* ---------- constants */

enum
{
	_device_position_reversed_bit = 0,
	_device_not_usable_bit = 1,
	_device_position_changed_bit = 2,
	_device_group_can_change_only_once_bit = 0,
	_device_group_changed_once_bit = 1,
	_device_group_runtime_bit = 2,
	_scenario_device_group_can_change_only_once_bit = 0,
};

enum
{
	_device_function_none = 0,
	_device_function_power,
	_device_function_change_in_power,
	_device_function_position,
	_device_function_change_in_position,
	_device_function_locked,
	_device_function_delay,
	NUMBER_OF_DEVICE_FUNCTION_MODES,
};

enum
{
	_device_animation_position = 0,
	_device_animation_power,
	NUMBER_OF_DEVICE_ANIMATIONS,
};

/* ---------- macros */

/* ---------- structures */

struct device_group_datum
{
	short identifier;
	word flags;
	real actual_value;
};

struct scenario_device_group
{
	char name[32];
	real initial_value;
	unsigned long flags;
	long unused[3];
};

struct animation_graph_device_animations
{
	long unused[21];
	struct tag_block animations;
};

typedef char device_group_datum_size_assert[
	sizeof(struct device_group_datum) == 0x8 ? 1 : -1];
typedef char device_group_datum_actual_value_offset_assert[
	offsetof(struct device_group_datum, actual_value) == 0x4 ? 1 : -1];
typedef char scenario_device_group_size_assert[
	sizeof(struct scenario_device_group) == 0x34 ? 1 : -1];
typedef char scenario_device_group_initial_value_offset_assert[
	offsetof(struct scenario_device_group, initial_value) == 0x20 ? 1 : -1];
typedef char scenario_device_group_flags_offset_assert[
	offsetof(struct scenario_device_group, flags) == 0x24 ? 1 : -1];
typedef char scenario_device_groups_offset_assert[
	offsetof(struct scenario, device_groups) == 0x288 ? 1 : -1];
typedef char animation_graph_device_animations_size_assert[
	sizeof(struct animation_graph_device_animations) == 0x60 ? 1 : -1];
typedef char animation_graph_device_animations_indices_offset_assert[
	offsetof(struct animation_graph_device_animations, animations) == 0x54 ? 1 : -1];
typedef char animation_graph_device_animations_offset_assert[
	offsetof(struct animation_graph, device_animations) == 0x30 ? 1 : -1];
typedef char animation_graph_animations_offset_assert[
	offsetof(struct animation_graph, animations) == 0x74 ? 1 : -1];
typedef char device_animation_size_assert[
	sizeof(struct animation) == 0xB4 ? 1 : -1];
typedef char device_animation_frame_count_offset_assert[
	offsetof(struct animation, frame_count) == 0x22 ? 1 : -1];

/* ---------- prototypes */

void device_group_set_actual_value(
	short group_index,
	real actual_value);
static short device_group_new(
	real initial_value,
	unsigned long flags);
static void create_initial_device_groups(
	void);
static void device_group_delete(
	short group_index);

/* ---------- globals */

/* ---------- public code */

void devices_initialize(
	void)
{
	device_groups_data = game_state_data_new(
		"device groups",
		1024,
		sizeof(struct device_group_datum));
	match_assert(
		"c:\\halo\\SOURCE\\devices\\devices.c",
		72,
		device_groups_data);

	return;
}

void devices_initialize_for_new_map(
	void)
{
	data_make_valid(device_groups_data);
	create_initial_device_groups();

	return;
}

void device_delete(
	long device_index)
{
	struct device_datum *device;

	device = device_get(device_index);
	device_group_delete(device->device.power_group_index);
	device_group_delete(device->device.position_group_index);

	return;
}

void devices_dispose(
	void)
{
	return;
}

void devices_dispose_from_old_map(
	void)
{
	data_make_invalid(device_groups_data);

	return;
}

boolean device_new(
	long device_index)
{
	struct device_datum *device;

	device = device_get(device_index);
	device_definition_get(device->definition_index);
	device->device.position_group_index = NONE;
	device->device.power_group_index = NONE;
	device->object.flags |= FLAG(_object_shadowless_bit);

	return TRUE;
}

void device_export_function_values(
	long device_index)
{
	short i;
	struct device_datum *device = device_get(device_index);
	struct device_definition *definition = device_definition_get(device->definition_index);

	for (i = 0; i < NUMBEROF(device->object.incoming_function_values); ++i)
	{
		if (definition->device.function_modes[i] != _device_function_none)
		{
			real value = 0.0f;

			switch (definition->device.function_modes[i])
			{
			case _device_function_power:
				value = device->device.power;
				break;
			case _device_function_change_in_power:
				if (device->device.power_velocity != 0.0f)
				{
					value = fabs(device->device.power_velocity) / definition->device.runtime_maximum_power_velocity;
				}
				break;
			case _device_function_position:
				value = device->device.position;
				break;
			case _device_function_change_in_position:
				if (device->device.position_velocity != 0.0f)
				{
					value = fabs(device->device.position_velocity) / definition->device.runtime_maximum_powered_position_velocity;
				}
				break;
			case _device_function_locked:
				if (device->device.power == 0.0f)
				{
					value = 1.0f;
				}
				if (device->object.type == _object_type_machine && device->device.position_group_index != NONE)
				{
					struct machine_datum *machine = machine_get(device_index);
					struct device_group_datum *group = datum_get(device_groups_data, machine->device.position_group_index);

					if (TEST_FLAG(machine->machine.flags, _machine_does_not_operate_automatically_bit) ||
						TEST_FLAG(machine->machine.flags, _machine_one_sided_bit))
					{
						value = 1.0f;
					}
					if (TEST_FLAG(group->flags, _device_group_can_change_only_once_bit) &&
						TEST_FLAG(group->flags, _device_group_changed_once_bit))
					{
						value = 1.0f;
					}
					if (machine->device.position == 1.0f ||
						TEST_FLAG(machine->machine.flags, _machine_never_appears_locked_bit))
					{
						value = 0.0f;
					}
				}
				break;
			case _device_function_delay:
				if (definition->device.runtime_delay_ticks <= 0.0f ||
					device->device.delay_ticks == definition->device.runtime_delay_ticks)
				{
					value = 0.0f;
				}
				else
				{
					value = device->device.delay_ticks / definition->device.runtime_delay_ticks;
				}
				break;
			}
			device->object.incoming_function_values[i] = value;
		}
	}

	return;
}

void device_preprocess_node_orientations(
	long device_index,
	struct real_orientation *node_orientations)
{
	struct device_datum *device = device_get(device_index);
	struct device_definition *definition = device_definition_get(device->definition_index);
	struct animation_graph *graph = animation_graph_definition_get(
		definition->object.animation_graph.index);

	if (graph->device_animations.count != 0)
	{
		struct animation_graph_device_animations *device_animations = TAG_BLOCK_GET_ELEMENT(
			&graph->device_animations,
			0,
			struct animation_graph_device_animations);

		if (device_animations != NULL)
		{
			if (device_animations->animations.count > _device_animation_position)
			{
				short animation_index = animation_graph_animation_index_get(
					&device_animations->animations)[_device_animation_position].animation_index;

				if (animation_index != NONE)
				{
					struct animation *animation = TAG_BLOCK_GET_ELEMENT(
						&graph->animations,
						animation_index,
						struct animation);
					real position = TEST_FLAG(device->device.flags, _device_position_reversed_bit)
						? 1.0f - device->device.position
						: device->device.position;
					real frame_index = position *
						(TEST_FLAG(definition->device.flags, _device_position_loops_bit)
							? (real)animation->frame_count
							: (real)(animation->frame_count - 1));

					if (TEST_FLAG(definition->device.flags, _device_position_animation_not_interpolated_bit))
					{
						overlay_animation_apply(animation, (short)frame_index, node_orientations);
					}
					else
					{
						overlay_animation_apply_continuous(animation, frame_index, node_orientations);
					}
				}
			}

			if (device_animations->animations.count > _device_animation_power)
			{
				short animation_index = animation_graph_animation_index_get(
					&device_animations->animations)[_device_animation_power].animation_index;

				if (animation_index != NONE)
				{
					struct animation *animation = TAG_BLOCK_GET_ELEMENT(
						&graph->animations,
						animation_index,
						struct animation);

					overlay_animation_apply_continuous(
						animation,
						animation->frame_count * device->device.power,
						node_orientations);
				}
			}
		}
	}

	return;
}

boolean device_update(
	long device_index)
{
	struct device_datum *device = device_get(device_index);
	struct device_definition *definition = device_definition_get(device->definition_index);
	boolean changed = FALSE;

	if (device->device.power_group_index != NONE)
	{
		struct device_group_datum *group = datum_get(
			device_groups_data,
			device->device.power_group_index);

		if (group->actual_value != device->device.power ||
			device->device.power_velocity != 0.0f)
		{
			real old_power = device->device.power;

			if (!accelerate_to_position(
				&device->device.power,
				&device->device.power_velocity,
				group->actual_value,
				definition->device.runtime_maximum_power_acceleration,
				definition->device.runtime_maximum_power_velocity,
				0.0f,
				1.0f,
				FALSE))
			{
				changed = TRUE;
			}

			if (old_power != device->device.power)
				device->device.flags |= FLAG(_device_position_changed_bit);
		}
	}

	if (device->device.position_group_index != NONE)
	{
		struct device_group_datum *group = datum_get(
			device_groups_data,
			device->device.position_group_index);

		if (group->actual_value != device->device.position ||
			device->device.position_velocity != 0.0f)
		{
			real maximum_acceleration =
				(1.0f - device->device.power) * definition->device.runtime_maximum_depowered_position_acceleration +
				device->device.power * definition->device.runtime_maximum_powered_position_acceleration;
			real maximum_velocity =
				(1.0f - device->device.power) * definition->device.runtime_maximum_depowered_position_velocity +
				device->device.power * definition->device.runtime_maximum_powered_position_velocity;
			boolean moving_forward = device->device.position_velocity > 0.0f;

			if (!(device->device.delay_ticks >= definition->device.runtime_delay_ticks) &&
				device->device.position == 0.0f &&
				!(group->actual_value < device->device.position))
			{
				if (++device->device.delay_ticks == 1)
				{
					device_effect_new(device_index, definition->device.delay_effect.index);
				}
			}
			else
			{
				real old_velocity = device->device.position_velocity;
				real old_position = device->device.position;

				if (fabs(device->device.position_velocity) > maximum_velocity)
				{
					device->device.position_velocity = moving_forward
						? maximum_velocity : -maximum_velocity;
				}

				if (accelerate_to_position(
					&device->device.position,
					&device->device.position_velocity,
					group->actual_value,
					maximum_acceleration,
					maximum_velocity,
					0.0f,
					1.0f,
					TEST_FLAG(definition->device.flags, _device_position_loops_bit)))
				{
					long effect_index = moving_forward
						? definition->device.positive_stop_effect.index
						: definition->device.negative_stop_effect.index;

					device_effect_new(device_index, effect_index);
				}
				else
				{
					if (device->device.position_velocity != 0.0f &&
						old_velocity * device->device.position_velocity <= 0.0f)
					{
						long effect_index = device->device.position_velocity > old_velocity
							? definition->device.positive_start_effect.index
							: definition->device.negative_start_effect.index;

						device_effect_new(device_index, effect_index);
					}
					changed = TRUE;
				}

				if (old_position != device->device.position)
					device->device.flags |= FLAG(_device_position_changed_bit);
			}
		}
		else
		{
			device->device.delay_ticks = 0;
		}
	}

	return changed;
}

real device_get_position(
	long device_index)
{
	if (device_index != NONE)
	{
		struct device_datum *device = device_get(device_index);

		return device->device.position;
	}

	return 0.0f;
}

real device_get_power(
	long device_index)
{
	if (device_index != NONE)
	{
		struct device_datum *device = device_get(device_index);

		return device->device.power;
	}

	return 0.0f;
}

void device_set_never_appears_locked(
	long device_index,
	boolean never_appears_locked)
{
	if (device_index != NONE)
	{
		struct machine_datum *machine = machine_try_and_get(device_index);

		if (machine != NULL)
		{
			if (never_appears_locked)
			{
				machine->machine.flags |= FLAG(_machine_never_appears_locked_bit);
			}
			else
			{
				machine->machine.flags &= ~FLAG(_machine_never_appears_locked_bit);
			}
		}
	}

	return;
}

void device_group_set_actual_value(
	short group_index,
	real actual_value)
{
	struct device_group_datum *group;
	struct device_datum *device;
	struct object_iterator iterator;

	if (actual_value < 0.0f)
		actual_value = 0.0f;
	else if (actual_value > 1.0f)
		actual_value = 1.0f;

	group = datum_get(device_groups_data, group_index);
	group->actual_value = actual_value;

	object_iterator_new(&iterator, _object_mask_device, 0);
	while ((device = object_iterator_next(&iterator)) != NULL)
	{
		if (device->device.power_group_index == group_index)
		{
			device->device.flags |= FLAG(_device_position_changed_bit);
			device->device.power = actual_value;
			device->device.power_velocity = 0.0f;
		}

		if (device->device.position_group_index == group_index)
		{
			device->device.flags |= FLAG(_device_position_changed_bit);
			device->device.position = actual_value;
			device->device.position_velocity = 0.0f;
		}
	}

	return;
}

void device_render_debug(
	long device_index)
{
	struct device_datum *device = device_get(device_index);

	if (debug_objects_devices)
	{
		char string[512];
		real_point3d origin;

		csstrcpy(string, "");
		sprintf(
			string+csstrlen(string),
			"power %.2f/vel %.2f",
			device->device.power,
			device->device.power_velocity);
		if (device->device.power_group_index != NONE)
		{
			struct device_group_datum *group = datum_get(
				device_groups_data,
				device->device.power_group_index);

			sprintf(
				string+csstrlen(string),
				" (group %d desired %.2f)",
				DATUM_INDEX_TO_ABSOLUTE_INDEX(device->device.power_group_index),
				group->actual_value);
		}

		sprintf(
			string+csstrlen(string),
			"|nposition %.2f/vel %.2f",
			device->device.position,
			device->device.position_velocity);
		if (device->device.position_group_index != NONE)
		{
			struct device_group_datum *group = datum_get(
				device_groups_data,
				device->device.position_group_index);

			sprintf(
				string+csstrlen(string),
				" (group %d desired %.2f)",
				DATUM_INDEX_TO_ABSOLUTE_INDEX(device->device.position_group_index),
				group->actual_value);
		}

		object_get_origin(device_index, &origin);
		/* Preserve January's inline schedule without owning point_from_line3d here. */
		origin.x = global_up3d->i*0.4f + origin.x;
		origin.y = global_up3d->j*0.4f + origin.y;
		origin.z = global_up3d->k*0.4f + origin.z;
		render_debug_string_at_point(FALSE, &origin, string, global_real_argb_white);
	}

	return;
}

boolean device_group_set_desired_value(
	short group_index,
	real desired_value)
{
	boolean changed = FALSE;

	if (desired_value < 0.0f)
		desired_value = 0.0f;
	else if (desired_value > 1.0f)
		desired_value = 1.0f;

	if (group_index != NONE)
	{
		struct device_group_datum *group = datum_get(device_groups_data, group_index);

		if (group->actual_value != desired_value &&
			(!TEST_FLAG(group->flags, _device_group_can_change_only_once_bit) ||
				!TEST_FLAG(group->flags, _device_group_changed_once_bit)))
		{
			struct object_iterator iterator;
			struct device_datum *device;

			group->actual_value = desired_value;
			group->flags |= FLAG(_device_group_changed_once_bit);
			changed = TRUE;

			object_iterator_new(&iterator, _object_mask_device, 0);
			while ((device = object_iterator_next(&iterator)) != NULL)
			{
				struct device_definition *definition =
					device_definition_get(device->definition_index);

				if (device->device.power_group_index == group_index)
				{
					long effect_index = desired_value > 0.0f
						? definition->device.repowered_effect.index
						: definition->device.depowered_effect.index;

					device_effect_new(iterator.index, effect_index);
				}
			}
		}
	}

	return changed;
}

void device_one_sided_set(
	long device_index,
	boolean one_sided)
{
	struct machine_datum *machine = machine_try_and_get(device_index);

	if (machine != NULL)
	{
		if (one_sided)
		{
			machine->machine.flags |= FLAG(_machine_one_sided_bit);
		}
		else
		{
			machine->machine.flags &= ~FLAG(_machine_one_sided_bit);
		}
	}

	return;
}

real device_group_get_value(
	short group_index)
{
	struct device_group_datum *group;

	group = datum_get(device_groups_data, group_index);

	return group->actual_value;
}

void device_group_change_only_once_more_set(
	long group_index,
	boolean change_only_once_more)
{
	if (group_index != NONE)
	{
		struct device_group_datum *group;

		group = datum_get(device_groups_data, group_index);
		if (change_only_once_more)
		{
			group->flags |= FLAG(_device_group_can_change_only_once_bit);
		}
		else
		{
			group->flags &= ~FLAG(_device_group_can_change_only_once_bit);
		}

		group->flags &= ~FLAG(_device_group_changed_once_bit);
	}

	return;
}

void device_operates_automatically_set(
	long device_index,
	boolean operates_automatically)
{
	struct machine_datum *machine = machine_try_and_get(device_index);

	if (machine != NULL)
	{
		SET_FLAG(
			machine->machine.flags,
			_machine_does_not_operate_automatically_bit,
			!operates_automatically);
	}

	return;
}

void device_touched(
	long device_index,
	long unit_index)
{
	struct device_datum *device = device_get(device_index);

	switch (device->object.type)
	{
		case _object_type_machine:
			machine_bumped(device_index, unit_index);
			break;

		case _object_type_control:
			control_touched(device_index, unit_index);
			break;
	}

	return;
}

boolean device_can_change_position(
	long device_index)
{
	struct device_datum *device = device_get(device_index);
	boolean can_change = FALSE;

	if (device->device.position_group_index != NONE)
	{
		struct device_group_datum *position_group = datum_get(
			device_groups_data,
			device->device.position_group_index);
		struct device_group_datum *power_group = datum_get(
			device_groups_data,
			device->device.power_group_index);

		can_change = TRUE;
		if (TEST_FLAG(position_group->flags, _device_group_can_change_only_once_bit) &&
			TEST_FLAG(position_group->flags, _device_group_changed_once_bit))
		{
			can_change = FALSE;
		}
		if (TEST_FLAG(device->device.flags, _device_not_usable_bit))
		{
			can_change = FALSE;
		}
		if (power_group->actual_value != 1.0f)
		{
			can_change = FALSE;
		}
	}

	return can_change;
}

void device_effect_new(
	long device_index,
	long effect_index)
{
	if (effect_index != NONE)
	{
		struct device_datum *device = device_get(device_index);
		long group_tag = tag_get_group_tag(effect_index);

		if (group_tag != EFFECT_DEFINITION_TAG)
		{
			match_dassert(
				"c:\\halo\\SOURCE\\devices\\devices.c",
				761,
				group_tag == SOUND_DEFINITION_TAG,
				NULL);

			if (group_tag == SOUND_DEFINITION_TAG)
			{
				object_impulse_sound_new(
					device_index,
					effect_index,
					NONE,
					global_origin3d,
					global_forward3d,
					1.0f);
			}
		}
		else
		{
			effect_new_from_object(
				effect_index,
				device_index,
				device_index,
				NONE,
				device->device.position,
				device->device.power,
				NULL,
				NULL);
		}
	}

	return;
}

void device_set_actual_position(
	long device_index,
	real position)
{
	if (device_index != NONE)
	{
		struct device_datum *device = device_get(device_index);

		if (device->device.position_group_index != NONE)
		{
			device_group_set_actual_value(
				device->device.position_group_index,
				position);
		}
	}

	return;
}

boolean device_set_desired_position(
	long device_index,
	real position)
{
	if (device_index != NONE)
	{
		struct device_datum *device = device_get(device_index);

		if (device->device.position_group_index != NONE)
		{
			return device_group_set_desired_value(
				device->device.position_group_index,
				position);
		}
	}

	return FALSE;
}

void device_set_power(
	long device_index,
	real power)
{
	if (device_index != NONE)
	{
		struct device_datum *device = device_get(device_index);

		device->device.flags |= FLAG(_device_position_changed_bit);
		device->device.power = power;
		device_group_set_desired_value(device->device.power_group_index, power);
	}

	return;
}

void device_add_scenario_information(
	long device_index,
	struct scenario_device_datum *scenario_device)
{
	struct device_datum *device = device_get(device_index);

	device_definition_get(device->definition_index);
	device->device.power_group_index = scenario_device->power_group_index != NONE
		? scenario_device->power_group_index
		: device_group_new(
			TEST_FLAG(scenario_device->flags, _scenario_device_initially_off_bit) ? 0.0f : 1.0f,
			FLAG(_device_group_runtime_bit));
	device->device.position_group_index = scenario_device->position_group_index != NONE
		? scenario_device->position_group_index
		: device_group_new(
			TEST_FLAG(scenario_device->flags, _scenario_device_initially_open_bit) ? 1.0f : 0.0f,
			FLAG(_device_group_runtime_bit) |
			(TEST_FLAG(scenario_device->flags, _scenario_device_changes_only_once_bit)
				? FLAG(_device_group_can_change_only_once_bit) : 0));

	{
		struct device_group_datum *group = datum_get(
			device_groups_data,
			device->device.power_group_index);

		device->device.power = group->actual_value;
	}
	{
		struct device_group_datum *group = datum_get(
			device_groups_data,
			device->device.position_group_index);

		device->device.position = group->actual_value;
	}

	/* January revalidates both assigned groups after copying their values. */
	datum_get(device_groups_data, device->device.power_group_index);
	datum_get(device_groups_data, device->device.position_group_index);

	if (TEST_FLAG(scenario_device->flags, _scenario_device_position_reversed_bit))
		device->device.flags |= FLAG(_device_position_reversed_bit);
	if (TEST_FLAG(scenario_device->flags, _scenario_device_not_usable_bit))
		device->device.flags |= FLAG(_device_not_usable_bit);

	return;
}

boolean device_frontfacing(
	long device_index,
	real_point3d const *position,
	real_vector3d const *facing)
{
	struct control_datum *control = control_try_and_get(device_index);
	struct object_marker front_marker;
	boolean frontfacing = TRUE;

	if (control != NULL &&
		!TEST_FLAG(control->control.flags, _control_usable_from_both_sides_bit) &&
		object_get_marker_by_name(device_index, "front", &front_marker, 1) == 1)
	{
		real facing_dot_front = dot_product3d(facing, &front_marker.matrix.forward);

		frontfacing = !(facing_dot_front > 0.0f);
	}

	return frontfacing;
}

/* ---------- private code */

static short device_group_new(
	real initial_value,
	unsigned long flags)
{
	short group_index = datum_new(device_groups_data);

	if (group_index != NONE)
	{
		struct device_group_datum *group = datum_get(device_groups_data, group_index);

		group->actual_value = initial_value;
		group->flags = flags;
	}
	else
	{
		match_vassert(
			"c:\\halo\\SOURCE\\devices\\devices.c",
			0x311,
			FALSE,
			"no more free device groups");
	}

	return group_index;
}

static void create_initial_device_groups(
	void)
{
	struct scenario *scenario = global_scenario_get();
	short group_index;

	for (group_index = 0; group_index < scenario->device_groups.count; group_index++)
	{
		struct scenario_device_group *definition = TAG_BLOCK_GET_ELEMENT(
			&scenario->device_groups,
			group_index,
			struct scenario_device_group);
		word flags = 0;
		short new_group_index;

		if (TEST_FLAG(definition->flags, _scenario_device_group_can_change_only_once_bit))
			flags |= FLAG(_device_group_can_change_only_once_bit);
		new_group_index = device_group_new(definition->initial_value, flags);
		match_assert(
			"c:\\halo\\SOURCE\\devices\\devices.c",
			0x339,
			new_group_index==group_index);
	}

	return;
}

static void device_group_delete(
	short group_index)
{
	if (group_index != NONE)
	{
		struct device_group_datum *group;

		group = datum_get(device_groups_data, group_index);
		if (TEST_FLAG(group->flags, _device_group_runtime_bit))
			datum_delete(device_groups_data, group_index);
	}

	return;
}
