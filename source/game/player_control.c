/*
PLAYER_CONTROL.C

symbols in this file:
000A56F0 0050:
	_player_control_get (0000)
000A5740 0020:
	_player_control_initialize (0000)
000A5760 0010:
	_player_control_dispose (0000)
000A5770 0010:
	_player_control_dispose_from_old_map (0000)
000A5780 0020:
	_player_control_camera_control_is_active (0000)
000A57A0 0030:
	_scripted_player_control_set_camera_control (0000)
000A57D0 0060:
	_player_control_inhibit_buttons (0000)
000A5830 00f0:
	_evaluate_piecewise_linear_function (0000)
000A5920 0010:
	_player_action_clear (0000)
000A5930 0060:
	_player_control_get_aiming_unit_index (0000)
000A5990 0070:
	_player_control_get_target_object_index (0000)
000A5A00 00b0:
	_player_control_get_field_of_view (0000)
000A5AB0 0130:
	_player_control_get_unit_camera_info (0000)
000A5BE0 0050:
	_player_control_get_unit_index (0000)
000A5C30 0080:
	_player_control_get_desired_weapon (0000)
000A5CB0 0050:
	_player_control_get_autoaim_level (0000)
000A5D00 0040:
	_unit_get_local_player_index (0000)
000A5D40 0050:
	_players_unzoom_all (0000)
000A5D90 0050:
	_player_control_unzoom (0000)
000A5DE0 0020:
	_player_control_get_zoom_level (0000)
000A5E00 0020:
	_player_control_action_test_reset (0000)
000A5E20 0020:
	_player_control_action_test_accept (0000)
000A5E40 0020:
	_player_control_action_test_back (0000)
000A5E60 0020:
	_player_control_action_test_action (0000)
000A5E80 0010:
	_player_control_action_test_jump (0000)
000A5E90 0010:
	_player_control_action_test_primary_trigger (0000)
000A5EA0 0010:
	_player_control_action_test_grenade_trigger (0000)
000A5EB0 0010:
	_player_control_action_test_zoom (0000)
000A5EC0 0020:
	_player_control_action_test_move_relative_all_directions (0000)
000A5EE0 0020:
	_player_control_action_test_look_relative_all_directions (0000)
000A5F00 0010:
	_player_control_action_test_look_relative_left (0000)
000A5F10 0010:
	_player_control_action_test_look_relative_right (0000)
000A5F20 0010:
	_player_control_action_test_look_relative_up (0000)
000A5F30 0010:
	_player_control_action_test_look_relative_down (0000)
000A5F40 0200:
	_player_control_action_test_check_reset_input_blob (0000)
000A6140 0040:
	_signed_angular_difference (0000)
000A6180 0050:
	_limit2d (0000)
000A61D0 0040:
	_interpolate_scalar (0000)
000A6210 0120:
	_player_control_set_facing (0000)
000A6330 00f0:
	_player_control_new_unit (0000)
000A6420 0d80:
	_get_local_player_input_blob (0000)
000A71A0 00e0:
	_player_control_get_facing_angles (0000)
000A7280 0030:
	_player_control_get_facing_direction (0000)
000A72B0 0050:
	_player_control_set_desired_weapon (0000)
000A7300 0630:
	_player_control_modify_desired_angles (0000)
000A7930 0090:
	_player_control_initialize_for_new_map (0000)
000A79C0 0620:
	_handle_one_player_input (0000)
000A7FE0 0080:
	_player_control_update (0000)
000A8060 0040:
	_player_control_permanent_impulse (0000)
0025C614 0016:
	??_C@_0BG@KLOMMKO@player_control_update?$AA@ (0000)
0025C62C 000c:
	_MOUSE_YAW_SCALE (0000)
	_MOUSE_PITCH_SCALE (0004)
0025C638 0025:
	??_C@_0CF@CDJDDDIC@c?3?2halo?2SOURCE?2game?2player_contr@ (0000)
0025C660 0017:
	??_C@_0BH@GDMLDNGJ@player?5control?5globals?$AA@ (0000)
0025C678 0046:
	??_C@_0EG@FFIEHMMD@function?5?$CG?$CG?5low_index?$DO?$DN0?5?$CG?$CG?5low_@ (0000)
0025C6C0 0004:
	__real@3f9c61aa (0000)
0025C6C4 000c:
	??_C@_0M@KHNJLLND@camera_info?$AA@ (0000)
0025C6D0 0004:
	__real@c0490fdb (0000)
0025C6D4 0023:
	??_C@_0CD@PHGHILFD@player_control?9?$DOdesired_angles?4y@ (0000)
0025C6F8 0025:
	??_C@_0CF@JIDICGJ@player_control?9?$DOdesired_angles?4p@ (0000)
0025C720 0017:
	??_C@_0BH@BJMHFMPG@input?9?$DOprimary_trigger?$AA@ (0000)
0025C738 0004:
	__real@3f75dcc7 (0000)
0025C73C 0004:
	__real@3d567750 (0000)
0025C740 0004:
	__real@bd567750 (0000)
0025C744 0004:
	__real@3dd67750 (0000)
0025C748 0004:
	__real@bdd67750 (0000)
0025C74C 0027:
	??_C@_0CH@OIFLGJJD@constants?9?$DOlook_acceleration_tim@ (0000)
0025C774 0021:
	??_C@_0CB@KGNPCEEG@constants?9?$DOlook_function?4count?$DO1@ (0000)
0025C798 002e:
	??_C@_0CO@HHADHJPE@valid_euler_angles2d?$CI?$CGplayer?9?$DOde@ (0000)
0025C7C8 0004:
	__real@bfbf0243 (0000)
0025C7CC 0004:
	__real@3fbf0243 (0000)
0025C7D0 0004:
	__real@3c490fdb (0000)
0025C7D4 0004:
	__real@bc490fdb (0000)
0025C7D8 0004:
	__real@3da3d70a (0000)
0025C7DC 001d:
	??_C@_0BN@JFNDAOJI@player?9?$DOdesired_angles?4pitch?$AA@ (0000)
0025C800 0008:
	__real@3fe45f3060000000 (0000)
0025C808 001a:
	??_C@_0BK@GKNPAGDI@action?4desired_facing?4yaw?$AA@ (0000)
0025C824 001c:
	??_C@_0BM@BMIDFCCA@action?4desired_facing?4pitch?$AA@ (0000)
0025C840 0018:
	??_C@_0BI@OPEALALJ@player?9?$DOprimary_trigger?$AA@ (0000)
0025C858 001b:
	??_C@_0BL@MOACFICP@player?9?$DOdesired_angles?4yaw?$AA@ (0000)
0025C874 0006:
	??_C@_05NCGEDJPM@delta?$AA@ (0000)
002DE808 0600:
	_player_autoaim_flag (0000)
	_player_magnetism_flag (0001)
	_controls_swapped (0002)
	_player_look_zoomed_scale (0004)
0043EE30 002b:
	_player_control_globals (0000)
	_debug_input_target (0004)
	_player_look_yaw_rate (0008)
	_player_look_pitch_rate (0018)
	_controls_enable_crouch (0028)
	_controls_enable_doubled_spin (0029)
	_controls_swap_doubled_spin_state (002a)
*/

/* ---------- headers */

#include "cseries.h"
#ifdef HALO_N64
#include "../../port/n64/src/n64.h"
#endif
#define limit2d limit2d_inline
#include "game/game.h"
#undef limit2d
#include "game/player_control.h"
#include "game/player_control_runtime.h"
#include "players.h"
#include "player_queues_new.h"

#include "game/aim_assist.h"
#include "game/game_globals.h"
#include "camera/director.h"
#include "cseries/profile.h"
#include "cutscene/cinematics.h"
#include "input/input.h"
#include "input/input_abstraction.h"
#include "interface/player_ui.h"
#include "items/weapons.h"
#include "main/main.h"
#include "objects/objects.h"
#include "physics/collision_usage.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "units/bipeds.h"
#include "units/unit_definitions.h"
#include "units/units.h"
#include "units/vehicle_definitions.h"
#include "units/vehicles.h"

#include "real_math.h"

/* ---------- constants */

enum control_button
{
	_button_jump = 0,
	_button_switch_grenade,
	_button_action_reload,
	_button_switch_weapon,
	_button_melee_attack,
	_button_flashlight,
	_button_throw_grenade,
	_button_fire,
	_button_start,
	_button_back,
	_button_crouch,
	_button_scope_zoom,
	NUMBER_OF_ACTION_CONTROL_BUTTONS,
};

enum mouse_button
{
	_mouse_button_left = 0,
	_mouse_button_middle,
	_mouse_button_right,
	NUMBER_OF_MOUSE_BUTTONS = 4,
};

real const MOUSE_YAW_SCALE = 0.0031415927f;
real const MOUSE_PITCH_SCALE = 0.0031415927f;
static real const ANALOG_BUTTON_SCALE = 1.f / 255.f;

/* ---------- macros */

#define valid_euler_angles2d(angles) ( \
	valid_real((angles)->pitch) && \
	(angles)->pitch <= DEGREES_TO_RADIANS(85.5f) && \
	(angles)->pitch >= -DEGREES_TO_RADIANS(85.5f) && \
	valid_real((angles)->yaw) && \
	(angles)->yaw <= _pi * 2.f && \
	(angles)->yaw >= 0.f)

/* ---------- structures */

struct input_blob
{
	real_vector2d throttle;
	real primary_trigger;
	real_euler_angles2d facing_delta;
	boolean accept;
	boolean back;
	byte pad16[2];
	unsigned long unit_control_flags;
	unsigned long player_control_flags;
};

struct game_input_state
{
	byte buttons[12];
	real forward_movement;
	real strafe;
	real yaw;
	real pitch;
};

struct mouse_state
{
	long x;
	long y;
	long wheel;
	byte buttons[NUMBER_OF_MOUSE_BUTTONS];
};

typedef char input_blob_size_assert[
	sizeof(struct input_blob) == sizeof(struct player_action) ? 1 : -1];
typedef char game_input_state_size_assert[
	sizeof(struct game_input_state) == 0x1C ? 1 : -1];
typedef char mouse_state_size_assert[
	sizeof(struct mouse_state) == 0x10 ? 1 : -1];
typedef char mouse_state_buttons_offset_assert[
	offsetof(struct mouse_state, buttons) == 0xC ? 1 : -1];

/* ---------- prototypes */

void player_aiming_vector_from_facing(
	long player_index,
	real_vector3d *facing_direction,
	real_euler_angles2d const *facing_angles);

static void player_control_modify_desired_angles(
	short local_player_index,
	real delta_yaw,
	real delta_pitch);
static void player_action_clear(
	struct input_blob *input);
static void player_control_action_test_check_reset_input_blob(
	struct input_blob *input);
static void get_local_player_input_blob(
	short local_player_index,
	real time_delta_sec,
	struct input_blob *input);
static void handle_one_player_input(
	short local_player_index,
	real time_delta_sec);

/* ---------- globals */

boolean player_autoaim_flag = TRUE;
boolean player_magnetism_flag = TRUE;
boolean controls_swapped = TRUE;
real player_look_zoomed_scale = 0.5f;

static struct profile_section player_control_update_section = {"player_control_update", NONE, TRUE};

static struct player_control_globals_data *player_control_globals;
short debug_input_target = 0;
real player_look_yaw_rate[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS] = {0};
real player_look_pitch_rate[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS] = {0};
boolean controls_enable_crouch = FALSE;
boolean controls_enable_doubled_spin = FALSE;
boolean controls_swap_doubled_spin_state = FALSE;
#ifdef HALO_LINUX
/* The native builds read input once a frame and draw several frames per
30 Hz tick (port/linux/game/render_interpolation.c). The pitch autolevel and
limits were stepped once per input update, a tick on the Xbox, so they step
by the ticks the frame lasted; an impulse applied from a tick steps by one. */
static real player_control_angle_step_ticks = 1.f;
static real player_control_autolevel_time[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
#endif

/* ---------- public code */

short unit_get_local_player_index(
	long unit_index)
{
	long player_index = unit_get(unit_index)->unit.player_index;

	if (player_index!=NONE)
		return player_get(player_index)->local_player_index;

	return NONE;
}


void player_control_initialize(
	void)
{
	player_control_globals = game_state_malloc(
		"player control globals",
		NULL,
		sizeof(*player_control_globals));
	return;
}

struct player_control *player_control_get(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0xB1,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	return &player_control_globals->players[local_player_index];
}

void player_control_dispose(
	void)
{
	return;
}

void player_control_dispose_from_old_map(
	void)
{
	return;
}

boolean player_control_camera_control_is_active(
	void)
{
	return (boolean)(!TEST_FLAG(
		player_control_globals->flags,
		_player_control_camera_control_disabled_bit) &&
		!game_time_get_paused());
}

boolean scripted_player_control_set_camera_control(
	boolean camera_control)
{
	SET_FLAG(
		player_control_globals->flags,
		_player_control_camera_control_disabled_bit,
		!camera_control);
	return camera_control;
}

void player_control_inhibit_buttons(
	short local_player_index,
	word action_flags,
	boolean persistent)
{
	struct player_control *control = player_control_get(local_player_index);

	control->inhibited_button_bit_vector |= action_flags;
	if (persistent)
	{
		control->reset_button_when_released_bit_vector |= action_flags;
	}
	return;
}

long player_control_get_target_object_index(
	short local_player_index)
{
	struct player_control *control = player_control_get(local_player_index);

	if (object_try_and_get(control->target_object_index))
	{
		return control->target_object_index;
	}
	return NONE;
}

real player_control_get_field_of_view(
	short local_player_index)
{
	struct player_control *control = player_control_get(local_player_index);
	real field_of_view = DEGREES_TO_RADIANS(70.f);

	if (control->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(control->unit_index);
		struct unit_definition *definition = unit_definition_get(unit->definition_index);
		long weapon_index = unit_inventory_get_weapon(
			control->unit_index,
			unit->unit.current_weapon_index);

		if (weapon_index != NONE)
		{
			field_of_view = weapon_get_field_of_view(
				weapon_index,
				definition->unit.camera_field_of_view,
				control->zoom_level);
		}
		else
		{
			field_of_view = definition->unit.camera_field_of_view;
		}
	}
	return field_of_view;
}

void player_control_get_unit_camera_info(
	short local_player_index,
	struct player_control_unit_camera_info *camera_info)
{
	struct player_control *control;
	struct unit_datum *unit;

	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0x402, camera_info);
	camera_info->camera = NULL;
	control = player_control_get(local_player_index);
	camera_info->unit_index = control->unit_index;
	camera_info->seat_index = NONE;

	if (camera_info->unit_index != NONE)
	{
		unit = unit_get(camera_info->unit_index);
		unit_get_camera_position(camera_info->unit_index, &camera_info->position);

		if (unit->object.parent_object_index != NONE)
		{
			struct unit_datum *vehicle = vehicle_try_and_get(
				unit->object.parent_object_index);

			if (vehicle)
			{
				struct unit_definition *definition = vehicle_definition_get(
					vehicle->definition_index);
				struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
					&definition->unit.seats,
					unit->unit.parent_seat_index,
					struct unit_seat);

				camera_info->unit_index = unit->object.parent_object_index;
				camera_info->camera = &seat->camera;
				camera_info->seat_index = unit->unit.parent_seat_index;
				unit = unit_get(camera_info->unit_index);
			}
		}

		if (camera_info->seat_index == NONE)
		{
			camera_info->camera = &unit_definition_get(unit->definition_index)->unit.camera;
		}
	}
	return;
}

real evaluate_piecewise_linear_function(
	short count,
	real const *function,
	real input)
{
	boolean negate = TRUE;
	short low_index;
	short high_index;
	real result;

	if (!(input < 0.f))
	{
		negate = FALSE;
	}
	input = PIN(fabs(input) * (count - 1), 0.0, (real)count - 1.f);
	low_index = PIN((short)input, 0, count - 1);
	high_index = MIN(low_index + 1, count - 1);
	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0x14B,
		function && low_index>=0 && low_index<=high_index && high_index<count);
	result = (function[high_index] - function[low_index]) *
		(input - low_index) + function[low_index];
	if (negate)
	{
		result = -result;
	}
	return result;
}

static void player_action_clear(
	struct input_blob *input)
{
	csmemset(
		input,
		0,
		sizeof(*input));

	return;
}

static void handle_one_player_input(
	short local_player_index,
	real time_delta_sec)
{
	struct player_control *player = player_control_get(local_player_index);
	struct game_globals_player_control *constants = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->player_control,
		0,
		struct game_globals_player_control);
	struct input_blob input;
	long current_weapon_index;

	csmemset(
		&input,
		0xFA,
		sizeof(input));
	get_local_player_input_blob(
		local_player_index,
		time_delta_sec,
		&input);
	if (local_player_get_player_index(local_player_index) != NONE)
	{
		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x2CE,
			player->desired_angles.pitch);
		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x2CF,
			player->desired_angles.yaw);
	}

	if (director_inhibited_input(local_player_index))
	{
		csmemset(&input, 0, sizeof(input));
	}

	if (game_connection() == _game_connection_local)
	{
		if (TEST_FLAG(input.player_control_flags, _player_control_debug_rotate_units_bit) ||
			TEST_FLAG(input.player_control_flags, _player_control_debug_rotate_all_units_bit))
		{
			long unit_index = TEST_FLAG(
				input.player_control_flags,
				_player_control_debug_rotate_all_units_bit)
				? units_debug_get_next_unit(player->unit_index)
				: units_debug_get_closest_unit(player->unit_index);

			if (unit_index != NONE)
			{
				players_set_local_player_unit(local_player_index, unit_index);
			}
		}
		if (TEST_FLAG(input.player_control_flags, _player_control_debug_ninja_rope_bit) &&
			player->unit_index != NONE)
		{
			unit_debug_ninja_rope(player->unit_index);
		}
	}

	if (player->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);
		short must_be_readied_weapon;

		unit_definition_get(unit->definition_index);
		current_weapon_index = unit_inventory_get_weapon(
			player->unit_index,
			unit->unit.current_weapon_index);
		if (player->desired_weapon_index == NONE ||
			unit_inventory_get_weapon(
				player->unit_index,
				player->desired_weapon_index) == NONE)
		{
			player->desired_weapon_index = unit->unit.desired_weapon_index;
		}

		if (TEST_FLAG(input.player_control_flags, _player_control_rotate_weapons_bit) ||
			unit_inventory_get_weapon(
				player->unit_index,
				player->desired_weapon_index) == NONE ||
			player->desired_weapon_index == NONE)
		{
			player->desired_weapon_index = unit_inventory_next_weapon(
				player->unit_index,
				player->desired_weapon_index,
				TEST_FLAG(input.player_control_flags, _player_control_rotate_weapons_bit));
			player->zoom_level = NONE;
		}

		must_be_readied_weapon = unit_inventory_get_must_be_readied_weapon(
			player->unit_index);
		if (must_be_readied_weapon != NONE &&
			player->desired_weapon_index != must_be_readied_weapon)
		{
			player->desired_weapon_index = must_be_readied_weapon;
			player->zoom_level = NONE;
		}

		if (player->desired_grenade_index == NONE ||
			!unit_get_grenade_count(
				player->unit_index,
				player->desired_grenade_index))
		{
			player->desired_grenade_index = unit->unit.desired_grenade_index;
		}
		if (TEST_FLAG(input.player_control_flags, _player_control_rotate_grenades_bit) ||
			!unit_get_grenade_count(
				player->unit_index,
				player->desired_grenade_index) ||
			player->desired_grenade_index == NONE)
		{
			player->desired_grenade_index = unit_inventory_next_grenade(
				player->unit_index,
				player->desired_grenade_index,
				1);
		}

		if (TEST_FLAG(input.player_control_flags, _player_control_input_zoom_bit) &&
			player_control_camera_control_is_active() &&
			current_weapon_index != NONE &&
			!cinematic_in_progress())
		{
			player->zoom_level = weapon_rotate_zoom_level(
				current_weapon_index,
				player->zoom_level);
		}

#ifdef HALO_N64
		if (n64_golden_active(local_player_index))
		{
			/* Held aim uses authored Halo scope levels and releases on R-up.
			   It re-enters the scope after reload while R remains held. */
			player->zoom_level = n64_golden_zoom(local_player_index, current_weapon_index);
		}
#endif
		if (!director_inhibited_facing(local_player_index))
		{
#ifdef HALO_LINUX
			player_control_angle_step_ticks = time_delta_sec * TICKS_PER_SECOND;
#endif
			player_control_modify_desired_angles(
				local_player_index,
				input.facing_delta.yaw,
				input.facing_delta.pitch);
#ifdef HALO_LINUX
			player_control_angle_step_ticks = 1.f;
#endif
		}

		if (unit->object.parent_object_index == NONE)
		{
			if (
#ifdef HALO_N64
				!n64_golden_active(local_player_index) &&
#endif
				player_ui_autolevel_enabled(local_player_index) &&
				fabs(player->throttle.i) > 0.5 &&
				input.facing_delta.pitch < 0.0001f &&
				player->magnetism_level < 0.0001f)
			{
#ifdef HALO_LINUX
				/* count ticks, not frames (see player_control_angle_step_ticks) */
				long ticks;

				player_control_autolevel_time[local_player_index] += time_delta_sec * TICKS_PER_SECOND;
				ticks = (long)player_control_autolevel_time[local_player_index];
				player_control_autolevel_time[local_player_index] -= (real)ticks;
				player->autolevel_ticks = (char)PIN(
					player->autolevel_ticks + ticks,
					0,
					127);
#else
				player->autolevel_ticks = (char)PIN(
					player->autolevel_ticks + 1,
					0,
					127);
#endif
				player->use_autolevel =
					player->autolevel_ticks > constants->minimum_autolevel_enabled_ticks;
			}
			else
			{
#ifdef HALO_LINUX
				player_control_autolevel_time[local_player_index] = 0.f;
#endif
				player->autolevel_ticks = 0;
				player->use_autolevel = FALSE;
			}
		}
		else
		{
			player->use_autolevel = FALSE;
		}
	}

	player->control_flags = input.unit_control_flags;
	player->throttle = input.throttle;
	player->primary_trigger = input.primary_trigger;
	match_assert_valid_real(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0x351,
		player->primary_trigger);

	if (local_player_get_player_index(local_player_index) != NONE)
	{
		struct player_action action;

		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x35D,
			player->desired_angles.pitch);
		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x35E,
			player->desired_angles.yaw);
		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x35F,
			player->primary_trigger);

		action.desired_facing = player->desired_angles;
		action.control_flags = player->control_flags;
		action.desired_weapon_index = player->desired_weapon_index;
		action.desired_grenade_index = player->desired_grenade_index;
		action.desired_zoom_level = player->zoom_level;
		action.throttle = player->throttle;
		action.primary_trigger = player->primary_trigger;

		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x369,
			action.desired_facing.pitch);
		match_assert_valid_real(
			"c:\\halo\\SOURCE\\game\\player_control.c",
			0x36A,
			action.desired_facing.yaw);
		update_client_queue(&action);
	}

	return;
}

boolean player_control_update(
	real time_delta_sec)
{
	short local_player_index;

	profile_enter(player_control_update_section);
	collision_log_begin_period(2);
	update_client_queue_push();
	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		handle_one_player_input(local_player_index, time_delta_sec);
	}
	collision_log_end_period();
	profile_exit(player_control_update_section);

	return FALSE;
}

long player_control_get_aiming_unit_index(
	short local_player_index)
{
	struct player_control *control = player_control_get(local_player_index);

	return unit_get_aiming_unit_index(control->unit_index);
}

long player_control_get_unit_index(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0xB1,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	return player_control_globals->players[local_player_index].unit_index;
}

short player_control_get_zoom_level(
	short local_player_index)
{
	short zoom_level = NONE;

	if (local_player_index != NONE)
	{
		zoom_level = player_control_get(local_player_index)->zoom_level;
	}
	return zoom_level;
}

real player_control_get_autoaim_level(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0xB1,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	return player_control_globals->players[local_player_index].autoaim_level;
}

long player_control_get_desired_weapon(
	short local_player_index,
	long unit_index)
{
	struct player_control *control = player_control_get(local_player_index);
	long weapon_index = NONE;

	if (control->unit_index != unit_index ||
		(weapon_index = unit_inventory_get_weapon(
			unit_index,
			control->desired_weapon_index)) == NONE)
	{
		weapon_index = unit_inventory_get_weapon(
			unit_index,
			unit_get(unit_index)->unit.current_weapon_index);
	}
	return weapon_index;
}

void players_unzoom_all(
	void)
{
	short local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		player_control_get(local_player_index)->zoom_level = NONE;
	}
	return;
}

void player_control_unzoom(
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	long player_index = unit->unit.player_index;

	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->local_player_index != NONE)
		{
			player_control_get(player->local_player_index)->zoom_level = NONE;
		}
	}
	return;
}

real_euler_angles2d const *player_control_get_facing_angles(
	short local_player_index)
{
	struct player_control *player = player_control_get(local_player_index);

	match_assert(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0x3C0,
		valid_euler_angles2d(&player->desired_angles));
	return &player->desired_angles;
}

real_vector3d *player_control_get_facing_direction(
	short local_player_index,
	real_vector3d *facing_direction)
{
	player_aiming_vector_from_facing(
		local_player_get_player_index(local_player_index),
		facing_direction,
		player_control_get_facing_angles(local_player_index));
	return facing_direction;
}

void player_control_set_desired_weapon(
	long unit_index,
	short desired_weapon_index)
{
	struct unit_datum *unit = unit_get(unit_index);

	if (unit->unit.player_index != NONE)
	{
		struct player_datum *player = player_get(unit->unit.player_index);

		if (player->local_player_index != NONE)
		{
			player_control_get(player->local_player_index)->desired_weapon_index =
				desired_weapon_index;
		}
	}
	return;
}

boolean limit2d(
	real_vector2d *vector,
	real length)
{
	real dot = vector->i * vector->i + vector->j * vector->j;

	if (dot > length * length)
	{
		scale_vector2d(
			vector,
			length / square_root(dot),
			vector);

		return TRUE;
	}

	return FALSE;
}

void player_control_set_facing(
	short local_player_index,
	real_vector3d const *facing_direction)
{
	struct player_control *player_control = player_control_get(local_player_index);

	euler_angles2d_from_vector3d(
		&player_control->desired_angles,
		facing_direction);
	match_assert_valid_real(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0xBB,
		player_control->desired_angles.pitch);
	match_assert_valid_real(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0xBC,
		player_control->desired_angles.yaw);
	if (player_control->desired_angles.yaw < 0.f)
	{
		player_control->desired_angles.yaw += _pi * 2.f;
	}
	return;
}

void player_control_new_unit(
	short local_player_index,
	long unit_index)
{
	struct player_control *control = player_control_get(local_player_index);

	csmemset(control, 0, sizeof(*control));
	control->unit_index = unit_index;
	control->desired_weapon_index = NONE;
	control->desired_grenade_index = NONE;
	control->zoom_level = NONE;
	control->use_autolevel = FALSE;
	control->target_object_index = NONE;
	control->pitch_maximum = DEGREES_TO_RADIANS(85.5f);
	control->pitch_minimum = -DEGREES_TO_RADIANS(85.5f);
	control->inhibited_button_bit_vector = 0;
	control->reset_button_when_released_bit_vector = 0;

	if (unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(unit_index);

		euler_angles2d_from_vector3d(
			&control->desired_angles,
			&unit->unit.desired_facing_vector);
		if (control->desired_angles.yaw < 0.f)
		{
			control->desired_angles.yaw += _pi * 2.f;
		}
		control->desired_weapon_index = unit->unit.desired_weapon_index;
		control->desired_grenade_index = unit->unit.desired_grenade_index;
		control->zoom_level = unit->unit.desired_zoom_level;
	}
	return;
}

static void get_local_player_input_blob(
	short local_player_index,
	real time_delta_sec,
	struct input_blob *input)
{
	long player_index = local_player_get_player_index(local_player_index);

	player_action_clear(input);
	if (player_index != NONE)
	{
		struct player_control *control = player_control_get(local_player_index);
		struct player_datum *player = player_get(player_index);
		short gamepad_index = player->local_player_index;
		boolean is_primary_player = gamepad_index == debug_input_target;

		if (gamepad_index != NONE && input_has_gamepad(gamepad_index))
		{
			struct game_globals_player_control *constants = TAG_BLOCK_GET_ELEMENT(
				&scenario_get_game_globals()->player_control,
				0,
				struct game_globals_player_control);
			struct gamepad_state const *gamepad = input_get_gamepad_state(gamepad_index);
			struct game_input_state *input_state = input_abstraction_get_input_state(gamepad_index);
			real look_yaw_rate = 0.f;
			real look_pitch_rate = 0.f;

			if (player->unit_index != NONE)
			{
				struct unit_datum *unit = unit_get(player->unit_index);

				look_yaw_rate = DEGREES_TO_RADIANS(
					player_look_yaw_rate[local_player_index]) *
					(1.f / TICKS_PER_SECOND);
				look_pitch_rate = DEGREES_TO_RADIANS(
					player_look_pitch_rate[local_player_index]) *
					(1.f / TICKS_PER_SECOND);
				if (unit->object.parent_object_index != NONE &&
					unit->unit.parent_seat_index != NONE)
				{
					struct unit_datum *parent_unit = unit_get(
						unit->object.parent_object_index);
					struct unit_definition *parent_definition = unit_definition_get(
						parent_unit->definition_index);
					struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
						&parent_definition->unit.seats,
						unit->unit.parent_seat_index,
						struct unit_seat);

					if (seat->yaw_rate > 0.f)
					{
						look_yaw_rate = DEGREES_TO_RADIANS(seat->yaw_rate) *
							(1.f / TICKS_PER_SECOND);
					}
					if (seat->pitch_rate > 0.f)
					{
						look_pitch_rate = DEGREES_TO_RADIANS(seat->pitch_rate) *
							(1.f / TICKS_PER_SECOND);
					}
				}
			}
#ifdef HALO_N64
			{
				/* each player's look speeds (the menu's CONTROLS: the stick's
				or the C buttons', by the scheme; port/n64/src/xdk_input.c) */
				extern void n64_look_speed(int local_player_index, float *yaw, float *pitch);
				float yaw_speed, pitch_speed;

				n64_look_speed(local_player_index, &yaw_speed, &pitch_speed);
				look_yaw_rate *= yaw_speed;
				look_pitch_rate *= pitch_speed;
			}
#endif

			input->throttle.i = input_state->forward_movement;
			input->throttle.j = input_state->strafe;
			{
				real absolute_pitch = fabs(input_state->pitch);
				real absolute_yaw = fabs(input_state->yaw);
				real look_scale = 1.f;
				real clamped_yaw;
				real clamped_pitch;

				if (absolute_pitch > 0.1f && absolute_yaw > 0.1f)
				{
					if (absolute_pitch > absolute_yaw)
					{
						absolute_yaw /= absolute_pitch;
						absolute_pitch = 1.f;
					}
					else
					{
						absolute_pitch /= absolute_yaw;
						absolute_yaw = 1.f;
					}
					look_scale = square_root(
						absolute_pitch * absolute_pitch +
						absolute_yaw * absolute_yaw);
				}
				clamped_yaw = PIN(input_state->yaw * look_scale, -1.f, 1.f);
				clamped_pitch = PIN(input_state->pitch * look_scale, -1.f, 1.f);

#ifdef HALO_N64
				if (player_control_camera_control_is_active() && n64_golden_active(local_player_index))
				{
					n64_golden_update(local_player_index, time_delta_sec, &input->facing_delta.yaw, &input->facing_delta.pitch,
						&input->throttle.i, &input->throttle.j);
				}
				else
#endif
				if (player_control_camera_control_is_active())
				{
					long doubled_spin;
					real yaw_spin_scale;
					real pitch_spin_scale;
					real_euler_angles2d look_delta;

					if (input_state->buttons[_button_scope_zoom] &&
						controls_enable_doubled_spin)
					{
						doubled_spin = !controls_swap_doubled_spin_state;
					}
					else
					{
						doubled_spin = controls_swap_doubled_spin_state;
					}
					pitch_spin_scale = (real)(doubled_spin + 1);
					if (!input_state->buttons[_button_scope_zoom] ||
						!controls_enable_doubled_spin)
					{
						doubled_spin = controls_swap_doubled_spin_state;
					}
					else
					{
						doubled_spin = !controls_swap_doubled_spin_state;
					}
					yaw_spin_scale = (real)(doubled_spin + 1);
					match_assert(
						"c:\\halo\\SOURCE\\game\\player_control.c",
						0x1C8,
						constants->look_function.count>1);
					look_delta.yaw = evaluate_piecewise_linear_function(
						constants->look_function.count,
						constants->look_function.address,
						clamped_yaw) * yaw_spin_scale * look_yaw_rate;
					look_delta.pitch = evaluate_piecewise_linear_function(
						constants->look_function.count,
						constants->look_function.address,
						clamped_pitch) * pitch_spin_scale * look_pitch_rate;

					if (player->unit_index != NONE && control->zoom_level != NONE)
					{
						real inverse_zoom = 1.f / unit_get_zoom_magnification(
							player->unit_index,
							control->zoom_level);

						look_delta.yaw *= inverse_zoom;
						look_delta.pitch *= inverse_zoom;
					}
					if (player->unit_index != NONE)
					{
						struct game_globals_player_information *player_information =
							TAG_BLOCK_GET_ELEMENT(
								&scenario_get_game_globals()->player_information,
								0,
								struct game_globals_player_information);
						struct unit_datum *unit = unit_get(player->unit_index);
						real stun_scale = 1.f - unit->unit.body_stun *
							player_information->stun_turning_penalty;

						look_delta.yaw *= stun_scale;
						look_delta.pitch *= stun_scale;
					}

					match_assert(
						"c:\\halo\\SOURCE\\game\\player_control.c",
						0x1E3,
						constants->look_acceleration_time>0.0f);
					if (fabs(clamped_yaw) >= constants->look_pegging_threshold)
					{
						real acceleration = PIN(
							control->look_acceleration_time /
								constants->look_acceleration_time,
							0.f,
							1.f);

						look_delta.yaw *= (constants->look_acceleration_scale - 1.f) *
							acceleration + 1.f;
						control->look_acceleration_time += time_delta_sec;
					}
					else
					{
						control->look_acceleration_time = 0.f;
					}

					{
						real_euler_angles2d target_angular_position;
						real_euler_angles2d target_angular_velocity;

						control->target_object_index = local_player_aim_assist(
							local_player_index,
							&control->autoaim_level,
							&control->magnetism_level,
							&target_angular_position,
							&target_angular_velocity);
						if (player_magnetism_flag && control->magnetism_level > 0.f &&
							(fabs(clamped_yaw) > _real_epsilon ||
							fabs(clamped_pitch) > _real_epsilon ||
							fabs(input->throttle.i) > _real_epsilon ||
							fabs(input->throttle.j) > _real_epsilon))
						{
							real game_speed = game_time_get_speed();
#ifdef HALO_N64
							/* the N64's stronger aim assist (aim_assist.c): the aim
							slows more over a target (friction 0.6 -> 0.75) and is
							carried along with it more (adhesion 0.7 -> 0.95) */
							real input_scale = 1.f - control->magnetism_level *
								PIN(constants->magnetism_friction * 1.25f, 0.f, 0.8f);
							real magnetism_scale = control->magnetism_level *
								PIN(constants->magnetism_adhesion * 1.35f, 0.f, 1.f);
#else
							real input_scale = 1.f - control->magnetism_level *
								PIN(constants->magnetism_friction, 0.f, 1.f);
							real magnetism_scale = control->magnetism_level *
								PIN(constants->magnetism_adhesion, 0.f, 1.f);
#endif

							if (game_players_are_double_speed())
							{
								game_speed *= 0.5f;
							}
							target_angular_velocity.yaw *= game_speed;
							target_angular_velocity.pitch *= game_speed;
							target_angular_velocity.yaw = PIN(
								target_angular_velocity.yaw,
								-DEGREES_TO_RADIANS(6.f),
								DEGREES_TO_RADIANS(6.f));
							target_angular_velocity.pitch = PIN(
								target_angular_velocity.pitch,
								-DEGREES_TO_RADIANS(3.f),
								DEGREES_TO_RADIANS(3.f));
							look_delta.yaw = target_angular_velocity.yaw * magnetism_scale +
								look_delta.yaw * input_scale;
							look_delta.pitch = target_angular_velocity.pitch * magnetism_scale +
								look_delta.pitch * input_scale;
						}
					}

					{
						real facing_scale = time_delta_sec * TICKS_PER_SECOND;

						input->facing_delta.yaw = facing_scale * look_delta.yaw;
						input->facing_delta.pitch = facing_scale * look_delta.pitch;
					}
#ifdef HALO_LINUX
					{
						/* direct mouse aim (port/linux/src/xinput_sdl.c) */
						extern int halo_linux_mouse_look(short gamepad_index, real *yaw, real *pitch);
						real mouse_yaw;
						real mouse_pitch;

						if (halo_linux_mouse_look(gamepad_index, &mouse_yaw, &mouse_pitch))
						{
							if (player->unit_index != NONE && control->zoom_level != NONE)
							{
								real inverse_zoom = 1.f / unit_get_zoom_magnification(
									player->unit_index,
									control->zoom_level);

								mouse_yaw *= inverse_zoom;
								mouse_pitch *= inverse_zoom;
							}
							input->facing_delta.yaw += mouse_yaw;
							input->facing_delta.pitch += mouse_pitch;
						}
					}
#endif
				}
				else
				{
					input->facing_delta.yaw = 0.f;
					input->facing_delta.pitch = 0.f;
				}
			}

			{
				byte effective_buttons[NUMBER_OF_ACTION_CONTROL_BUTTONS] = {0};
				word buttons_to_reset =
					control->inhibited_button_bit_vector &
					control->reset_button_when_released_bit_vector;
				long button_index;

				if (buttons_to_reset)
				{
					for (button_index = 0;
						button_index < NUMBER_OF_ACTION_CONTROL_BUTTONS;
						button_index++)
					{
						if (TEST_FLAG(buttons_to_reset, button_index) &&
							!input_state->buttons[button_index])
						{
							SET_FLAG(
								control->inhibited_button_bit_vector,
								button_index,
								FALSE);
							SET_FLAG(
								control->reset_button_when_released_bit_vector,
								button_index,
								FALSE);
						}
					}
				}
				for (button_index = 0;
					button_index < NUMBER_OF_ACTION_CONTROL_BUTTONS;
					button_index++)
				{
					if (!TEST_FLAG(
						control->inhibited_button_bit_vector,
						button_index))
					{
						effective_buttons[button_index] =
							input_state->buttons[button_index];
					}
				}

				if (player->unit_index != NONE)
				{
					struct biped_datum *biped = biped_try_and_get(player->unit_index);

					if (biped &&
						(controls_enable_crouch ||
						TEST_FLAG(biped->biped.flags, _biped_airborne_bit) ||
						magnitude_squared2d(&input->throttle) < 0.98f * 0.98f))
					{
						SET_FLAG(
							input->unit_control_flags,
							_unit_control_crouch_modifier_bit,
							effective_buttons[_button_crouch]);
					}
				}

				input->primary_trigger =
					(real)input_state->buttons[_button_fire] * ANALOG_BUTTON_SCALE;
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_weapon_primary_trigger_bit,
					effective_buttons[_button_fire]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_throw_grenade_bit,
					effective_buttons[_button_throw_grenade]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_weapon_secondary_trigger_bit,
					effective_buttons[_button_throw_grenade]);
				SET_FLAG(
					input->player_control_flags,
					_player_control_input_zoom_bit,
					effective_buttons[_button_scope_zoom] == TRUE);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_action_bit,
					effective_buttons[_button_action_reload]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_swap_weapons_bit,
					effective_buttons[_button_action_reload] >=
						constants->minimum_weapon_swap_ticks);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_integrated_light_bit,
					effective_buttons[_button_flashlight]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_jump_bit,
					effective_buttons[_button_jump]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_use_equipment_bit,
					effective_buttons[_button_melee_attack]);
				SET_FLAG(
					input->player_control_flags,
					_player_control_rotate_weapons_bit,
					effective_buttons[_button_switch_weapon] == TRUE);
				SET_FLAG(
					input->player_control_flags,
					_player_control_rotate_grenades_bit,
					effective_buttons[_button_switch_grenade] == TRUE);

				if (!TEST_FLAG(
					control->inhibited_button_bit_vector,
					_button_back))
				{
					input->back = gamepad->buttons[_gamepad_analog_button_b];
				}
				if (!TEST_FLAG(
					control->inhibited_button_bit_vector,
					_button_action_reload))
				{
					input->accept = gamepad->buttons[_gamepad_analog_button_a];
				}
			}
		}
		else
		{
			struct mouse_state const *mouse = input_get_mouse_state();

			if (mouse && is_primary_player)
			{
				mouse = input_get_mouse_state();
				input->throttle.i =
					(real)(!!input_key_is_down(_key_w) - !!input_key_is_down(_key_s));
				input->throttle.j =
					(real)(!!input_key_is_down(_key_a) - !!input_key_is_down(_key_d));
				if (player_control_camera_control_is_active())
				{
					input->facing_delta.yaw = -(real)mouse->x * MOUSE_YAW_SCALE;
					input->facing_delta.pitch = -(real)mouse->y * MOUSE_PITCH_SCALE;
				}
				else
				{
					input->facing_delta.yaw = 0.f;
					input->facing_delta.pitch = 0.f;
				}

				SET_FLAG(
					input->unit_control_flags,
					_unit_control_look_dont_turn_bit,
					input_key_is_down(_key_shift));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_force_alert_bit,
					input_key_is_down(_key_alt));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_crouch_modifier_bit,
					input_key_is_down(_key_control));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_jump_bit,
					input_key_is_down(_key_space));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_action_bit,
					input_key_is_down(_key_q));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_integrated_light_bit,
					input_key_is_down(_key_x));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_weapon_reload_bit,
					input_key_is_down(_key_r));
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_weapon_primary_trigger_bit,
					mouse->buttons[_mouse_button_left]);
				SET_FLAG(
					input->unit_control_flags,
					_unit_control_throw_grenade_bit,
					mouse->buttons[_mouse_button_right]);
				SET_FLAG(
					input->player_control_flags,
					_player_control_input_zoom_bit,
					input_key_is_down(_key_z) == TRUE);
				SET_FLAG(
					input->player_control_flags,
					_player_control_rotate_weapons_bit,
					input_key_is_down(_key_e) == TRUE);
				input->primary_trigger = TEST_FLAG(
					input->unit_control_flags,
					_unit_control_weapon_primary_trigger_bit) ? 1.f : 0.f;
			}
		}

		SET_FLAG(
			input->player_control_flags,
			_player_control_debug_rotate_units_bit,
			input_key_is_down(_key_backslash) == TRUE);
		SET_FLAG(
			input->player_control_flags,
			_player_control_debug_rotate_all_units_bit,
			input_key_is_down(_key_right_bracket) == TRUE);
		SET_FLAG(
			input->player_control_flags,
			_player_control_debug_ninja_rope_bit,
			input_key_is_down(_key_left_bracket) == TRUE);
		SET_FLAG(
			input->unit_control_flags,
			_unit_control_user_animation1_bit,
			input_key_is_down(_key_1) == TRUE);
		SET_FLAG(
			input->unit_control_flags,
			_unit_control_user_animation2_bit,
			input_key_is_down(_key_2) == TRUE);

		if (magnitude_squared2d(&input->throttle) > 1.f)
		{
			real scale = 1.f / square_root(magnitude_squared2d(&input->throttle));

			input->throttle.i *= scale;
			input->throttle.j *= scale;
		}
	}

	player_control_action_test_check_reset_input_blob(input);
	match_assert_valid_real(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0x2B6,
		input->primary_trigger);

	return;
}

void player_control_action_test_reset(
	void)
{
	struct player_control_globals_data *globals = player_control_globals;

	globals->action_flags = 0;
	globals->action_test_flags = 0;
	return;
}

boolean player_control_action_test_accept(
	void)
{
	struct player_control_globals_data *globals = player_control_globals;

	globals->action_test_flags |= FLAG(_player_control_accept_bit);
	globals->suppressed_action_flags |= FLAG(_player_control_accept_bit);
	return TEST_FLAG(
		globals->action_flags,
		_player_control_accept_bit);
}

boolean player_control_action_test_back(
	void)
{
	struct player_control_globals_data *globals = player_control_globals;

	globals->action_test_flags |= FLAG(_player_control_back_bit);
	globals->suppressed_action_flags |= FLAG(_player_control_back_bit);
	return TEST_FLAG(
		globals->action_flags,
		_player_control_back_bit);
}

boolean player_control_action_test_action(
	void)
{
	struct player_control_globals_data *globals = player_control_globals;

	globals->action_test_flags |= FLAG(_player_control_action_bit);
	globals->suppressed_action_flags |= FLAG(_player_control_action_bit);
	return TEST_FLAG(
		globals->action_flags,
		_player_control_action_bit);
}

boolean player_control_action_test_jump(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_jump_bit);
}

boolean player_control_action_test_primary_trigger(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_primary_trigger_bit);
}

boolean player_control_action_test_grenade_trigger(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_grenade_trigger_bit);
}

boolean player_control_action_test_zoom(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_zoom_bit);
}

boolean player_control_action_test_look_relative_left(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_look_relative_left_bit);
}

boolean player_control_action_test_look_relative_right(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_look_relative_right_bit);
}

boolean player_control_action_test_look_relative_up(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_look_relative_up_bit);
}

boolean player_control_action_test_look_relative_down(
	void)
{
	return TEST_FLAG(
		player_control_globals->action_flags,
		_player_control_look_relative_down_bit);
}

boolean player_control_action_test_move_relative_all_directions(
	void)
{
	return !(~player_control_globals->action_flags &
		_player_control_move_relative_all_directions_flags);
}

boolean player_control_action_test_look_relative_all_directions(
	void)
{
	return !(~player_control_globals->action_flags &
		_player_control_look_relative_all_directions_flags);
}

static void player_control_action_test_check_reset_input_blob(
	struct input_blob *input)
{
	struct player_control_globals_data *globals;

	if (input->accept && cinematic_can_be_skipped())
	{
		main_skip_cinematic();
	}

	globals = player_control_globals;
	if (TEST_FLAG(input->unit_control_flags, _unit_control_action_bit))
	{
		globals->action_flags |= FLAG(_player_control_action_bit);
	}
	if (TEST_FLAG(input->unit_control_flags, _unit_control_jump_bit))
	{
		globals->action_flags |= FLAG(_player_control_jump_bit);
	}
	if (input->accept)
	{
		globals->action_flags |= FLAG(_player_control_accept_bit);
	}
	if (input->back)
	{
		globals->action_flags |= FLAG(_player_control_back_bit);
	}
	if (input->primary_trigger > 0.f)
	{
		globals->action_flags |= FLAG(_player_control_primary_trigger_bit);
	}
	if (TEST_FLAG(input->unit_control_flags, _unit_control_throw_grenade_bit))
	{
		globals->action_flags |= FLAG(_player_control_grenade_trigger_bit);
	}
	if (TEST_FLAG(input->player_control_flags, _player_control_input_zoom_bit))
	{
		globals->action_flags |= FLAG(_player_control_zoom_bit);
	}

	if (input->facing_delta.pitch > 0.f)
	{
		globals->action_flags |= FLAG(_player_control_look_relative_up_bit);
	}
	else if (input->facing_delta.pitch < 0.f)
	{
		globals->action_flags |= FLAG(_player_control_look_relative_down_bit);
	}
	if (input->facing_delta.yaw > 0.f)
	{
		globals->action_flags |= FLAG(_player_control_look_relative_left_bit);
	}
	else if (input->facing_delta.yaw < 0.f)
	{
		globals->action_flags |= FLAG(_player_control_look_relative_right_bit);
	}
	if (input->throttle.i > 0.f)
	{
		globals->action_flags |= FLAG(_player_control_move_relative_forward_bit);
	}
	else if (input->throttle.i < 0.f)
	{
		globals->action_flags |= FLAG(_player_control_move_relative_backward_bit);
	}
	if (input->throttle.j > 0.f)
	{
		globals->action_flags |= FLAG(_player_control_move_relative_right_bit);
	}
	else if (input->throttle.j < 0.f)
	{
		globals->action_flags |= FLAG(_player_control_move_relative_left_bit);
	}

	if (!TEST_FLAG(globals->action_test_flags, _player_control_action_bit))
	{
		if (!TEST_FLAG(globals->suppressed_action_flags, _player_control_action_bit))
		{
			goto check_accept;
		}
		SET_FLAG(
			globals->suppressed_action_flags,
			_player_control_action_bit,
			TEST_FLAG(input->unit_control_flags, _unit_control_action_bit));
	}
	SET_FLAG(input->unit_control_flags, _unit_control_action_bit, FALSE);

check_accept:
	if (!controls_swapped)
	{
		if (!TEST_FLAG(globals->action_test_flags, _player_control_accept_bit))
		{
			if (!TEST_FLAG(globals->suppressed_action_flags, _player_control_accept_bit))
			{
				goto check_back_unswapped;
			}
			SET_FLAG(
				globals->suppressed_action_flags,
				_player_control_accept_bit,
				TEST_FLAG(input->unit_control_flags, _unit_control_action_bit));
		}
		SET_FLAG(input->unit_control_flags, _unit_control_action_bit, FALSE);

	check_back_unswapped:
		if (!TEST_FLAG(globals->action_test_flags, _player_control_back_bit))
		{
			if (!TEST_FLAG(globals->suppressed_action_flags, _player_control_back_bit))
			{
				return;
			}
			SET_FLAG(
				globals->suppressed_action_flags,
				_player_control_back_bit,
				TEST_FLAG(input->player_control_flags, _player_control_rotate_weapons_bit));
		}
		SET_FLAG(input->player_control_flags, _player_control_rotate_weapons_bit, FALSE);
	}
	else
	{
		if (!TEST_FLAG(globals->action_test_flags, _player_control_accept_bit))
		{
			if (!TEST_FLAG(globals->suppressed_action_flags, _player_control_accept_bit))
			{
				goto check_back_swapped;
			}
			SET_FLAG(
				globals->suppressed_action_flags,
				_player_control_accept_bit,
				TEST_FLAG(input->unit_control_flags, _unit_control_jump_bit));
		}
		SET_FLAG(input->unit_control_flags, _unit_control_jump_bit, FALSE);

	check_back_swapped:
		if (!TEST_FLAG(globals->action_test_flags, _player_control_back_bit))
		{
			if (!TEST_FLAG(globals->suppressed_action_flags, _player_control_accept_bit))
			{
				return;
			}
			SET_FLAG(
				globals->suppressed_action_flags,
				_player_control_accept_bit,
				TEST_FLAG(input->player_control_flags, _player_control_rotate_grenades_bit));
		}
		SET_FLAG(input->player_control_flags, _player_control_rotate_grenades_bit, FALSE);
	}

	return;
}

void player_control_initialize_for_new_map(
	void)
{
	short local_player_index;

	player_control_globals->action_test_flags = 0;
	player_control_globals->action_flags = 0;
	player_control_globals->suppressed_action_flags = 0;
	player_control_globals->flags = 0;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		struct game_globals_player_control *constants = TAG_BLOCK_GET_ELEMENT(
			&scenario_get_game_globals()->player_control,
			0,
			struct game_globals_player_control);

		player_control_new_unit(local_player_index, NONE);
		if (player_look_pitch_rate[local_player_index] == 0.f)
		{
			player_look_pitch_rate[local_player_index] =
				constants->look_default_pitch_rate;
		}
		if (player_look_yaw_rate[local_player_index] == 0.f)
		{
			player_look_yaw_rate[local_player_index] =
				constants->look_default_yaw_rate;
		}
	}
	return;
}

void player_control_permanent_impulse(
	short local_player_index,
	real_euler_angles2d const *delta)
{
	match_assert("c:\\halo\\SOURCE\\game\\player_control.c", 0x467, delta);
	player_control_modify_desired_angles(
		local_player_index,
		delta->yaw,
		delta->pitch);
	return;
}

/* ---------- private code */

static void player_control_modify_desired_angles(
	short local_player_index,
	real delta_yaw,
	real delta_pitch)
{
	struct player_control *player = player_control_get(local_player_index);
	struct game_globals_player_control *constants = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->player_control,
		0,
		struct game_globals_player_control);
	struct player_control_unit_camera_info camera_info;
	real pitch_minimum = -DEGREES_TO_RADIANS(85.5f);
	real pitch_maximum = DEGREES_TO_RADIANS(85.5f);

	match_assert(
		"c:\\halo\\SOURCE\\game\\player_control.c",
		0x494,
		valid_euler_angles2d(&player->desired_angles));
	player_control_get_unit_camera_info(local_player_index, &camera_info);

	player->desired_angles.yaw += delta_yaw;
	if (camera_info.seat_index != NONE)
	{
		struct unit_datum *unit = unit_get(camera_info.unit_index);
		struct unit_definition *definition = unit_definition_get(unit->definition_index);
		struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
			&definition->unit.seats,
			camera_info.seat_index,
			struct unit_seat);

		if (seat->yaw_minimum != 0.f || seat->yaw_maximum != 0.f)
		{
			struct object_marker marker;
			real_euler_angles2d marker_angles;
			real yaw_minimum;
			real yaw_maximum;
			real arc;
			real to_maximum;
			real to_minimum;

			object_get_marker_by_name(
				camera_info.unit_index,
				seat->marker_name,
				&marker,
				1);
			euler_angles2d_from_vector3d(&marker_angles, &marker.matrix.forward);
			yaw_minimum = marker_angles.yaw + seat->yaw_minimum;
			yaw_maximum = marker_angles.yaw + seat->yaw_maximum;
			arc = signed_angular_difference(yaw_minimum, yaw_maximum);
			to_maximum = signed_angular_difference(player->desired_angles.yaw, yaw_maximum);
			to_minimum = signed_angular_difference(yaw_minimum, player->desired_angles.yaw);
			if (arc < 0.f)
			{
				arc += _pi * 2.f;
			}

			if (!(to_maximum >= 0.f && to_maximum < arc) &&
				!(to_minimum >= 0.f && to_minimum < arc))
			{
				if (fabs(to_minimum) < fabs(to_maximum))
				{
					player->desired_angles.yaw = yaw_minimum;
				}
				else
				{
					player->desired_angles.yaw = yaw_maximum;
				}
			}
		}
	}

	while (player->desired_angles.yaw < 0.f)
	{
		player->desired_angles.yaw += _pi * 2.f;
	}
	while (player->desired_angles.yaw > _pi * 2.f)
	{
		player->desired_angles.yaw -= _pi * 2.f;
	}

	if (camera_info.camera)
	{
		struct unit_datum *unit = unit_get(camera_info.unit_index);
		real pitch_autolevel = camera_info.camera->pitch_autolevel;

		if (camera_info.camera->pitch_maximum != 0.f ||
			camera_info.camera->pitch_minimum != 0.f)
		{
			pitch_minimum = camera_info.camera->pitch_minimum;
			pitch_maximum = camera_info.camera->pitch_maximum;

			if (camera_info.seat_index != NONE && unit->object.up.k > 0.2f)
			{
				real_euler_angles2d look_angles;
				real_vector3d look_vector;
				real tilt;

				look_angles.yaw = player->desired_angles.yaw;
				look_angles.pitch = 0.f;
				vector3d_from_euler_angles2d(&look_vector, &look_angles);
				tilt = _pi / 2.f - angle_between_vectors3d(&look_vector, &unit->object.up);
				pitch_minimum -= tilt;
				pitch_maximum -= tilt;
				pitch_autolevel -= tilt;
			}

			pitch_minimum = PIN(
				pitch_minimum,
				-DEGREES_TO_RADIANS(85.5f),
				DEGREES_TO_RADIANS(85.5f));
			pitch_maximum = PIN(
				pitch_maximum,
				-DEGREES_TO_RADIANS(85.5f),
				DEGREES_TO_RADIANS(85.5f));
		}

		if (pitch_autolevel != 0.f || player->use_autolevel)
		{
			real error = fabs(player->desired_angles.pitch - pitch_autolevel) * 0.63661975f;

			match_assert_valid_real(
				"c:\\halo\\SOURCE\\game\\player_control.c",
				0x4F2,
				player->desired_angles.pitch);
#ifdef HALO_LINUX
			error *= player_control_angle_step_ticks;
#endif
			if (pitch_autolevel != 0.f)
			{
				interpolate_scalar(
					&player->desired_angles.pitch,
					pitch_autolevel,
					error * 0.08f * magnitude3d(&unit->object.translational_velocity));
			}
			else
			{
				interpolate_scalar(
					&player->desired_angles.pitch,
					pitch_autolevel,
					error * constants->look_autolevel_scale *
						magnitude3d(&unit->object.translational_velocity));
			}
			match_assert_valid_real(
				"c:\\halo\\SOURCE\\game\\player_control.c",
				0x4FD,
				player->desired_angles.pitch);
		}
	}

#ifdef HALO_LINUX
	interpolate_scalar(&player->pitch_minimum, pitch_minimum, _pi / 256.f * player_control_angle_step_ticks);
	interpolate_scalar(&player->pitch_maximum, pitch_maximum, _pi / 256.f * player_control_angle_step_ticks);
#else
	interpolate_scalar(&player->pitch_minimum, pitch_minimum, _pi / 256.f);
	interpolate_scalar(&player->pitch_maximum, pitch_maximum, _pi / 256.f);
#endif

	player->desired_angles.pitch += delta_pitch;
	player->desired_angles.pitch = PIN(
		player->desired_angles.pitch,
		player->pitch_minimum,
		player->pitch_maximum);

	return;
}
