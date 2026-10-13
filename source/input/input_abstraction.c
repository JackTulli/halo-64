/*
INPUT_ABSTRACTION.C

symbols in this file:
000BDA30 0020:
	_input_abstraction_dispose (0000)
000BDA50 0010:
	_input_abstraction_reset_controller_detection_timer (0000)
000BDA60 0080:
	_input_abstraction_get_local_player_preferences (0000)
000BDAE0 00b0:
	_input_abstraction_update_local_player_preferences (0000)
000BDB90 0050:
	_input_abstraction_get_input_state (0000)
000BDBE0 0080:
	_input_abstraction_update_device_changes (0000)
000BDC60 00e0:
	_code_000bdc60 (0000)
000BDD40 0050:
	_code_000bdd40 (0000)
000BDD90 00a0:
	_input_abstraction_initialize (0000)
000BDE30 0950:
	_input_abstraction_update (0000)
0026F3A0 0014:
	_rdata_0026f3a0 (0000)
0026F3B4 000c:
	??_C@_0M@OAMDCHEH@preferences?$AA@ (0000)
0026F3C0 0041:
	??_C@_0EB@NBKADHOE@?$CIlocal_player_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIloca@ (0000)
0026F404 0029:
	??_C@_0CJ@KPJFDJEG@c?3?2halo?2SOURCE?2input?2input_abstr@ (0000)
0026F430 0041:
	??_C@_0EB@LOCNLGGH@invalid?5controller?5preferences?$DL?5@ (0000)
0026F474 003d:
	??_C@_0DN@PDKODJDH@?$CIcontroller_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIcontro@ (0000)
0026F4B4 0039:
	??_C@_0DJ@DKGJGJAM@stopping?5bink?5playback?5to?5due?5to@ (0000)
0026F4F0 004c:
	??_C@_0EM@DKLJMFDI@?$CIcontroller_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIcontro@ (0000)
0026F53C 0018:
	??_C@_0BI@DEJBIOHJ@unknown?5joystick?5preset?$AA@ (0000)
0026F558 0008:
	__real@3fc6571840000000 (0000)
0026F560 0008:
	__real@3ffa313e30a3879f (0000)
0026F568 0008:
	__real@3fe38c3550000000 (0000)
004535C0 00e0:
	_input_abstraction_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "bink/bink_playback.h"
#include "cseries/errors.h"
#include "cseries/cseries_windows.h"
#include "game/game_globals.h"
#include "game/player_control.h"
#include "game/players.h"
#include "input/input.h"
#include "input/input_abstraction.h"
#include "interface/player_ui.h"
#include "interface/ui_widget.h"
#include "interface/virtual_keyboard.h"
#include "math/real_math.h"
#include "networking/network_game_globals.h"
#include "scenario/scenario.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

/* ---------- constants */

enum
{
	_game_control_jump,
	_game_control_switch_grenades,
	_game_control_action,
	_game_control_switch_weapons,
	_game_control_melee,
	_game_control_flashlight,
	_game_control_grenade,
	_game_control_primary_trigger,
	_game_control_start,
	_game_control_back,
	_game_control_crouch,
	_game_control_zoom,
	NUMBER_OF_GAME_CONTROLS,
};

enum
{
	_joystick_controls_default,
	_joystick_controls_southpaw,
	_joystick_controls_legacy,
	_joystick_controls_legacy_southpaw,
	NUMBER_OF_JOYSTICK_CONTROLS,
};

enum
{
	_vehicle_type_human_plane = 3,
	_vehicle_type_alien_fighter = 5,
};

/* January promotes the rounded 45- and 10-degree single-precision constants
 * before subtracting them; the 35-degree window and its reciprocal are double. */
#define STICK_DIAGONAL_ANGLE 0.7853981852531433f
#define STICK_SECOND_QUADRANT_DIAGONAL_ANGLE 2.356194496154785f
#define RIGHT_STICK_DIAGONAL_SNAP_ANGLE ((double)0.1745329201221466f)
#define LEFT_STICK_DIAGONAL_SNAP_ANGLE ((double)STICK_DIAGONAL_ANGLE - RIGHT_STICK_DIAGONAL_SNAP_ANGLE)
#define STICK_DIAGONAL_BLEND_SCALE (1.0 / LEFT_STICK_DIAGONAL_SNAP_ANGLE)

/* ---------- macros */

/* ---------- structures */

struct game_input_state
{
	byte buttons[NUMBER_OF_GAME_CONTROLS];
	real forward_movement;
	real strafe;
	real yaw;
	real pitch;
};

/* Vehicle tags remain opaque in the shared header. This authenticated prefix
 * is sufficient for the aircraft-type check; it is not a complete tag layout. */
struct vehicle_definition
{
	struct unit_definition unit;
	unsigned long flags;
	short vehicle_type;
};

typedef char verify_input_vehicle_type_offset[
	offsetof(struct vehicle_definition, vehicle_type) == 0x2F4 ? 1 : -1];

struct input_abstraction_runtime_globals
{
	struct game_input_preferences player_control_preferences[MAXIMUM_GAMEPADS];
	struct game_input_state input_states[MAXIMUM_GAMEPADS];
	unsigned long device_enumeration_startup_timer;
	boolean controller_available[MAXIMUM_GAMEPADS];
	boolean initialized;
	unsigned long time_of_first_device_insertion;
};

typedef char verify_game_input_state_size[
	sizeof(struct game_input_state) == 0x1C ? 1 : -1];
typedef char verify_input_abstraction_input_states_offset[
	offsetof(struct input_abstraction_runtime_globals, input_states) == 0x60 ? 1 : -1];
typedef char verify_input_abstraction_device_timer_offset[
	offsetof(struct input_abstraction_runtime_globals, device_enumeration_startup_timer) == 0xD0 ? 1 : -1];
typedef char verify_input_abstraction_controller_available_offset[
	offsetof(struct input_abstraction_runtime_globals, controller_available) == 0xD4 ? 1 : -1];
typedef char verify_input_abstraction_initialized_offset[
	offsetof(struct input_abstraction_runtime_globals, initialized) == 0xD8 ? 1 : -1];
typedef char verify_input_abstraction_first_insertion_offset[
	offsetof(struct input_abstraction_runtime_globals, time_of_first_device_insertion) == 0xDC ? 1 : -1];
typedef char verify_input_abstraction_runtime_globals_size[
	sizeof(struct input_abstraction_runtime_globals) == 0xE0 ? 1 : -1];

/* ---------- prototypes */

static void set_default_game_input_preferences(
	struct game_input_preferences *preferences);
static boolean local_player_is_piloting_aircraft(
	short controller_index);

/* ---------- globals */

struct input_abstraction_runtime_globals input_abstraction_globals = {0};
static real const gamepad_axis_normalization_scale = 1.f / SHORT_MAX;
static real const stick_direction_angles[] =
{
	STICK_DIAGONAL_ANGLE,
	STICK_SECOND_QUADRANT_DIAGONAL_ANGLE,
	-STICK_DIAGONAL_ANGLE,
	-STICK_SECOND_QUADRANT_DIAGONAL_ANGLE,
};

/* ---------- public code */

void input_abstraction_initialize(
	void)
{
	long controller_index;

	csmemset(
		&input_abstraction_globals,
		0,
		offsetof(struct input_abstraction_runtime_globals, time_of_first_device_insertion));
	for (controller_index = 0; controller_index < MAXIMUM_GAMEPADS; controller_index++)
	{
		set_default_game_input_preferences(
			&input_abstraction_globals.player_control_preferences[controller_index]);
		input_abstraction_globals.controller_available[controller_index] =
			input_has_gamepad((short)controller_index);
	}
	input_abstraction_reset_controller_detection_timer();
	input_abstraction_globals.initialized = TRUE;

	return;
}

void input_abstraction_dispose(
	void)
{
	csmemset(
		&input_abstraction_globals,
		0,
		offsetof(struct input_abstraction_runtime_globals, time_of_first_device_insertion));

	return;
}

void input_abstraction_reset_controller_detection_timer(
	void)
{
	input_abstraction_globals.device_enumeration_startup_timer = system_milliseconds();

	return;
}

void input_abstraction_get_local_player_preferences(
	short local_player_index,
	struct game_input_preferences *preferences)
{
	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		495,
		(local_player_index>=0) && (local_player_index<MAXIMUM_GAMEPADS));
	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		496,
		preferences);

	csmemcpy(
		preferences,
		&input_abstraction_globals.player_control_preferences[local_player_index],
		sizeof(*preferences));

	return;
}

void input_abstraction_update_local_player_preferences(
	short controller_index,
	struct game_input_preferences const *preferences)
{
	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		507,
		(controller_index>=0) && (controller_index<MAXIMUM_GAMEPADS));
	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		508,
		preferences);
	match_vassert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		511,
		preferences->game_control_to_xbox_buttons[8] == _gamepad_binary_button_start &&
			preferences->game_control_to_xbox_buttons[9] == _gamepad_binary_button_back,
		"invalid controller preferences; can't remap start & back buttons");

	csmemcpy(
		&input_abstraction_globals.player_control_preferences[controller_index],
		preferences,
		sizeof(*preferences));

	return;
}

struct game_input_state *input_abstraction_get_input_state(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		521,
		(local_player_index>=0) && (local_player_index<MAXIMUM_GAMEPADS));

	return &input_abstraction_globals.input_states[local_player_index];
}

void input_abstraction_update_device_changes(
	unsigned long device_change_flags)
{
	if (input_abstraction_globals.initialized)
	{
		if (device_change_flags &&
			(system_milliseconds() - input_abstraction_globals.device_enumeration_startup_timer >= 2000 ||
			(input_abstraction_globals.time_of_first_device_insertion &&
			system_milliseconds() - input_abstraction_globals.time_of_first_device_insertion >= 2000)))
		{
			error(
				_error_silent,
				"stopping bink playback to due to change in input devices");
			bink_playback_stop();
		}

		if ((device_change_flags & 0xFFF000) &&
			!input_abstraction_globals.time_of_first_device_insertion)
		{
			input_abstraction_globals.time_of_first_device_insertion = system_milliseconds();
		}
	}

	return;
}

void input_abstraction_update(
	void)
{
	long controller_index;

	TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->player_control,
		0,
		struct game_globals_player_control);
	for (controller_index = 0; controller_index < MAXIMUM_GAMEPADS; controller_index++)
	{
		struct gamepad_state const *gamepad = input_get_gamepad_state((short)controller_index);

		if (gamepad)
		{
			struct game_input_state *state = &input_abstraction_globals.input_states[controller_index];
			real left_angle;
			real right_angle;
			real_point2d left_stick;
			real_point2d right_stick;
			long control_index;
			boolean invert_look;

			player_look_yaw_rate[controller_index] = input_abstraction_globals.player_control_preferences[controller_index].yaw_rate;
			player_look_pitch_rate[controller_index] = input_abstraction_globals.player_control_preferences[controller_index].pitch_rate;
			{
				real scale;

				left_angle = arctangent(gamepad->sticks[_gamepad_stick_left].y, gamepad->sticks[_gamepad_stick_left].x);
				scale = 1.0 / MAX(fabs(sine(left_angle)), fabs(cosine(left_angle)));
				left_stick.x = PIN(gamepad->sticks[_gamepad_stick_left].x * gamepad_axis_normalization_scale * scale, -1.f, 1.f);
				left_stick.y = PIN(gamepad->sticks[_gamepad_stick_left].y * gamepad_axis_normalization_scale * scale, -1.f, 1.f);
			}
			{
				real scale;

				right_angle = arctangent(gamepad->sticks[_gamepad_stick_right].y, gamepad->sticks[_gamepad_stick_right].x);
				scale = 1.0 / MAX(fabs(sine(right_angle)), fabs(cosine(right_angle)));
				right_stick.x = PIN(gamepad->sticks[_gamepad_stick_right].x * gamepad_axis_normalization_scale * scale, -1.f, 1.f);
				right_stick.y = PIN(gamepad->sticks[_gamepad_stick_right].y * gamepad_axis_normalization_scale * scale, -1.f, 1.f);
			}
			for (control_index = 0; control_index < NUMBER_OF_GAME_CONTROLS; control_index++)
			{
				state->buttons[control_index] =
					gamepad->buttons[input_abstraction_globals.player_control_preferences[controller_index].game_control_to_xbox_buttons[control_index]];
			}

			if (input_abstraction_globals.player_control_preferences[controller_index].joystick_controls == _joystick_controls_legacy ||
				input_abstraction_globals.player_control_preferences[controller_index].joystick_controls == _joystick_controls_legacy_southpaw)
			{
				short left_quadrant = (left_stick.x < 0.f ? 1 : 0) | (left_stick.y < 0.f ? 2 : 0);
				short right_quadrant = (right_stick.x < 0.f ? 1 : 0) | (right_stick.y < 0.f ? 2 : 0);
				real left_difference = left_angle - stick_direction_angles[left_quadrant];
				real right_difference = right_angle - stick_direction_angles[right_quadrant];
				real left_magnitude = square_root(left_stick.x * left_stick.x + left_stick.y * left_stick.y);
				real right_magnitude = square_root(right_stick.x * right_stick.x + right_stick.y * right_stick.y);

				if (fabs(left_difference) < LEFT_STICK_DIAGONAL_SNAP_ANGLE)
				{
					real absolute_angle = fabs(left_angle);

					if (absolute_angle < STICK_DIAGONAL_ANGLE ||
						absolute_angle > STICK_SECOND_QUADRANT_DIAGONAL_ANGLE)
					{
						left_stick.x = (left_stick.x < 0.f ? -1 : 1) * left_magnitude;
						left_stick.y = (left_stick.y < 0.f ? -1 : 1) * left_magnitude *
							(1.0 - fabs(left_difference) * STICK_DIAGONAL_BLEND_SCALE);
					}
					else
					{
						left_stick.y = (left_stick.y < 0.f ? -1 : 1) * left_magnitude;
						left_stick.x = (left_stick.x < 0.f ? -1 : 1) * left_magnitude *
							(1.0 - fabs(left_difference) * STICK_DIAGONAL_BLEND_SCALE);
					}
				}
				else
				{
					if (fabs(left_stick.x) > fabs(left_stick.y))
					{
						left_stick.x = (left_stick.x < 0.f ? -1 : 1) * left_magnitude;
						left_stick.y = 0.f;
					}
					else
					{
						left_stick.y = (left_stick.y < 0.f ? -1 : 1) * left_magnitude;
						left_stick.x = 0.f;
					}
				}

				if (fabs(right_difference) < RIGHT_STICK_DIAGONAL_SNAP_ANGLE)
				{
					real absolute_angle = fabs(right_angle);

					if (absolute_angle < STICK_DIAGONAL_ANGLE ||
						absolute_angle > STICK_SECOND_QUADRANT_DIAGONAL_ANGLE)
					{
						right_stick.x = (right_stick.x < 0.f ? -1 : 1) * right_magnitude;
						right_stick.y = (right_stick.y < 0.f ? -1 : 1) * right_magnitude *
							(1.0 - fabs(right_difference) * STICK_DIAGONAL_BLEND_SCALE);
					}
					else
					{
						right_stick.y = (right_stick.y < 0.f ? -1 : 1) * right_magnitude;
						right_stick.x = (right_stick.x < 0.f ? -1 : 1) * right_magnitude *
							(1.0 - fabs(right_difference) * STICK_DIAGONAL_BLEND_SCALE);
					}
				}
				else
				{
					if (fabs(right_stick.x) > fabs(right_stick.y))
					{
						right_stick.x = (right_stick.x < 0.f ? -1 : 1) * right_magnitude;
						right_stick.y = 0.f;
					}
					else
					{
						right_stick.y = (right_stick.y < 0.f ? -1 : 1) * right_magnitude;
						right_stick.x = 0.f;
					}
				}
			}

			invert_look = input_abstraction_globals.player_control_preferences[controller_index].invert_look;
			if (!invert_look && input_abstraction_globals.player_control_preferences[controller_index].invert_look_aircraft_control)
			{
				invert_look = local_player_is_piloting_aircraft((short)controller_index);
			}
			switch (input_abstraction_globals.player_control_preferences[controller_index].joystick_controls)
			{
				case _joystick_controls_default:
					if (gamepad->buttons[_gamepad_binary_button_dpad_left])
					{
						state->strafe = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
					{
						state->strafe = -1.f;
					}
					else
					{
						state->strafe = -left_stick.x;
					}
					if (gamepad->buttons[_gamepad_binary_button_dpad_up])
					{
						state->forward_movement = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
					{
						state->forward_movement = -1.f;
					}
					else
					{
						state->forward_movement = left_stick.y;
					}
					state->yaw = -right_stick.x;
					state->pitch = (invert_look ? -1.f : 1.f) * right_stick.y;
					break;
				case _joystick_controls_southpaw:
					if (gamepad->buttons[_gamepad_binary_button_dpad_left])
					{
						state->yaw = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
					{
						state->yaw = -1.f;
					}
					else
					{
						state->yaw = -left_stick.x;
					}
					if (gamepad->buttons[_gamepad_binary_button_dpad_up])
					{
						state->pitch = invert_look ? -1.f : 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
					{
						state->pitch = invert_look ? 1.f : -1.f;
					}
					else
					{
						state->pitch = (invert_look ? -1.f : 1.f) * left_stick.y;
					}
					state->forward_movement = right_stick.y;
					state->strafe = -right_stick.x;
					break;
				case _joystick_controls_legacy:
					if (gamepad->buttons[_gamepad_binary_button_dpad_left])
					{
						state->yaw = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
					{
						state->yaw = -1.f;
					}
					else
					{
						state->yaw = -left_stick.x;
					}
					if (gamepad->buttons[_gamepad_binary_button_dpad_up])
					{
						state->forward_movement = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
					{
						state->forward_movement = -1.f;
					}
					else
					{
						state->forward_movement = left_stick.y;
					}
					state->strafe = -right_stick.x;
					state->pitch = (invert_look ? -1.f : 1.f) * right_stick.y;
					break;
				case _joystick_controls_legacy_southpaw:
					if (gamepad->buttons[_gamepad_binary_button_dpad_left])
					{
						state->strafe = 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_right])
					{
						state->strafe = -1.f;
					}
					else
					{
						state->strafe = -left_stick.x;
					}
					if (gamepad->buttons[_gamepad_binary_button_dpad_up])
					{
						state->pitch = invert_look ? -1.f : 1.f;
					}
					else if (gamepad->buttons[_gamepad_binary_button_dpad_down])
					{
						state->pitch = invert_look ? 1.f : -1.f;
					}
					else
					{
						state->pitch = (invert_look ? -1.f : 1.f) * left_stick.y;
					}
					state->forward_movement = right_stick.y;
					state->yaw = -right_stick.x;
					break;
				default:
					error(_error_silent, "unknown joystick preset");
					break;
			}
			input_abstraction_globals.controller_available[controller_index] = TRUE;
		}
		else
		{
			if (input_abstraction_globals.controller_available[controller_index])
			{
				short error_controller = (short)controller_index;
				short error_code;
				boolean pause_game;
				boolean show_error = TRUE;

				if (main_menu_is_active())
				{
					long available_controllers = 0;
					long index = 0;

					do
					{
						if (input_abstraction_globals.controller_available[index])
						{
							available_controllers++;
						}
						index++;
					}
					while (index < MAXIMUM_GAMEPADS);
					pause_game = FALSE;
					error_code = _error_controller_unplugged;
					if (available_controllers >= 2)
					{
						if (player_ui_get_single_player_local_player_controller(0) != controller_index &&
							player_ui_get_single_player_local_player_controller(1) != controller_index &&
							player_ui_get_single_player_local_player_controller(2) != controller_index &&
							player_ui_get_single_player_local_player_controller(3) != controller_index &&
							!player_ui_local_player_wants_to_play_multiplayer((short)controller_index))
						{
							show_error = FALSE;
						}
					}
					else
					{
						if (player_ui_get_single_player_local_player_controller(0) != controller_index &&
							player_ui_get_single_player_local_player_controller(1) != controller_index &&
							player_ui_get_single_player_local_player_controller(2) != controller_index &&
							player_ui_get_single_player_local_player_controller(3) != controller_index &&
							!player_ui_local_player_wants_to_play_multiplayer((short)controller_index))
						{
							error_controller = NONE;
						}
					}
				}
				else if (global_network_game_client_get())
				{
					pause_game = FALSE;
					error_code = _error_controller_unplugged;
					show_error = local_player_exists(controller_index);
				}
				else
				{
					pause_game = TRUE;
					error_code = _error_controller_unplugged_start_to_continue;
					show_error = local_player_exists(controller_index);
				}
				if (show_error == TRUE)
				{
					if (virtual_keyboard_active())
					{
						virtual_keyboard_close();
					}
					display_error_deferred(error_code, error_controller, pause_game, pause_game);
				}
			}
			input_abstraction_globals.controller_available[controller_index] = FALSE;
		}
	}

	return;
}

/* ---------- private code */

static boolean local_player_is_piloting_aircraft(
	short controller_index)
{
	boolean piloting_aircraft = FALSE;
	long player_index;

	match_assert(
		"c:\\halo\\SOURCE\\input\\input_abstraction.c",
		569,
		(controller_index>=0) && (controller_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));
	player_index = local_player_get_player_index(controller_index);
	if (player_index != NONE)
	{
		struct player_datum *player = player_try_and_get(player_index);

		if (player)
		{
			struct unit_datum *unit = unit_try_and_get(player->unit_index);

			if (unit && unit->object.parent_object_index != NONE && unit->unit.parent_seat_index != NONE)
			{
				struct unit_datum *vehicle = vehicle_get(unit->object.parent_object_index);
				struct vehicle_definition *definition = vehicle_specific_definition_get(vehicle->definition_index);

				if (definition->vehicle_type == _vehicle_type_human_plane ||
					definition->vehicle_type == _vehicle_type_alien_fighter)
				{
					struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
						&definition->unit.unit.seats,
						unit->unit.parent_seat_index,
						struct unit_seat);

					if (TEST_FLAG(seat->flags, _unit_seat_driver_bit))
					{
						piloting_aircraft = TRUE;
					}
				}
			}
		}
	}

	return piloting_aircraft;
}

static void set_default_game_input_preferences(
	struct game_input_preferences *preferences)
{
	preferences->pitch_rate = 60.f;
	preferences->yaw_rate = 120.f;
	preferences->game_control_to_xbox_buttons[_game_control_jump] = _gamepad_analog_button_a;
	preferences->game_control_to_xbox_buttons[_game_control_switch_grenades] = _gamepad_analog_button_black;
	preferences->game_control_to_xbox_buttons[_game_control_action] = _gamepad_analog_button_x;
	preferences->game_control_to_xbox_buttons[_game_control_switch_weapons] = _gamepad_analog_button_y;
	preferences->game_control_to_xbox_buttons[_game_control_melee] = _gamepad_analog_button_b;
	preferences->game_control_to_xbox_buttons[_game_control_flashlight] = _gamepad_analog_button_white;
	preferences->game_control_to_xbox_buttons[_game_control_grenade] = _gamepad_analog_button_left_trigger;
	preferences->game_control_to_xbox_buttons[_game_control_primary_trigger] = _gamepad_analog_button_right_trigger;
	preferences->game_control_to_xbox_buttons[_game_control_start] = _gamepad_binary_button_start;
	preferences->game_control_to_xbox_buttons[_game_control_back] = _gamepad_binary_button_back;
	preferences->game_control_to_xbox_buttons[_game_control_crouch] = _gamepad_binary_button_left_thumb;
	preferences->game_control_to_xbox_buttons[_game_control_zoom] = _gamepad_binary_button_right_thumb;
	preferences->joystick_controls = _joystick_controls_default;
	preferences->invert_look = FALSE;
	preferences->invert_look_aircraft_control = FALSE;

	return;
}
