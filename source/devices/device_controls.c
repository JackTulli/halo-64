/*
DEVICE_CONTROLS.C

symbols in this file:
00083D10 0010:
	_controls_initialize (0000)
00083D20 0010:
	_controls_dispose (0000)
00083D30 0010:
	_controls_initialize_for_new_map (0000)
00083D40 0010:
	_controls_dispose_from_old_map (0000)
00083D50 0070:
	_control_place (0000)
00083DC0 0030:
	_control_new (0000)
00083DF0 0010:
	_control_delete (0000)
00083E00 0030:
	_control_update (0000)
00083E30 0130:
	_code_00083e30 (0000)
00083F60 0040:
	_control_touched (0000)
00083FA0 0040:
	_control_destroyed (0000)
00259634 0029:
	??_C@_0CJ@LFHDKHDO@c?3?2halo?2SOURCE?2devices?2device_co@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "device_controls.h"
#include "memory/data.h"
#include "scenario/scenario_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct scenario_control_datum
{
	struct scenario_object_datum object;
	struct scenario_device_datum device;
	word flags;
	short unused;
	short custom_name_index;
};

struct device_group_datum
{
	short identifier;
	word pad;
	real actual_value;
};

/* ---------- prototypes */

static void code_00083e30(
	long control_index);

/* ---------- globals */

/* ---------- public code */

void controls_initialize(
	void)
{
	return;
}

void controls_dispose(
	void)
{
	return;
}

void controls_initialize_for_new_map(
	void)
{
	return;
}

void controls_dispose_from_old_map(
	void)
{
	return;
}

boolean control_new(
	long control_index)
{
	struct control_datum *control = control_get(control_index);

	control_definition_get(control->definition_index);

	return TRUE;
}

void control_delete(
	long control_index)
{
	return;
}

boolean control_update(
	long control_index)
{
	struct control_datum *control = control_get(control_index);

	control_definition_get(control->definition_index);

	return TRUE;
}

void control_place(
	long control_index,
	struct scenario_control_datum *scenario_control)
{
	struct control_datum *control = control_get(control_index);

	control_definition_get(control->definition_index);
	device_add_scenario_information(control_index, &scenario_control->device);
	if (TEST_FLAG(scenario_control->flags, 0))
		control->control.flags |= FLAG(0);
	if (TEST_FLAG(scenario_control->flags, 4))
		control->control.flags |= FLAG(1);
	control->control.custom_name_index = scenario_control->custom_name_index - 1;

	return;
}

void control_touched(
	long control_index,
	long unit_index)
{
	struct control_datum *control= control_get(control_index);
	struct control_definition *definition= control_definition_get(control->definition_index);

	if (definition->control.triggers_when == 0)
		code_00083e30(control_index);

	return;
}

void control_destroyed(
	long control_index)
{
	struct control_datum *control= control_get(control_index);
	struct control_definition *definition= control_definition_get(control->definition_index);

	if (definition->control.triggers_when == 1)
		code_00083e30(control_index);

	return;
}

/* ---------- private code */

static void code_00083e30(
	long control_index)
{
	struct control_datum *control= control_get(control_index);
	struct control_definition *definition= control_definition_get(control->definition_index);

	if (control->device.position_group_index != NONE)
	{
		struct device_group_datum *group= datum_get(
			device_groups_data,
			control->device.position_group_index);
		real desired_value;

		switch (definition->control.type)
		{
		case 0:
			desired_value= group->actual_value > 0.5f ? 0.0f : 1.0f;
			break;
		case 1:
			desired_value= 1.0f;
			break;
		case 2:
			desired_value= 0.0f;
			break;
		case 3:
			desired_value= definition->control.call_value;
			break;
		/* desired_value is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			display_assert(
				NULL,
				"c:\\halo\\SOURCE\\devices\\device_controls.c",
				138,
				TRUE);
			system_exit(NONE);
			break;
		}

		if (device_group_set_desired_value(
			control->device.position_group_index,
			desired_value))
		{
			long effect_index;

			if (desired_value > 0.5f)
				effect_index= definition->control.on_effect.index;
			else
				effect_index= definition->control.off_effect.index;

			device_effect_new(control_index, effect_index);
		}
		else
		{
			device_effect_new(control_index, definition->control.denied_effect.index);
		}
	}

	return;
}
