/*
HUD_UNIT.C

symbols in this file:
000C6590 0010:
	_unit_hud_outline_mapper_tick (0000)
000C65A0 0010:
	_unit_hud_shield_meter_mapper_tick (0000)
000C65B0 0010:
	_unit_hud_shield_meter_mapper_init (0000)
000C65C0 0040:
	_initialize_hud_state (0000)
000C6600 0070:
	_get_hud_state (0000)
000C6670 0040:
	_hud_initialize_unit_interface (0000)
000C66B0 00f0:
	_hud_initialize_unit_interface_for_new_map (0000)
000C67A0 0010:
	_hud_dispose_unit_interface_from_old_map (0000)
000C67B0 0010:
	_hud_dispose_unit_interface (0000)
000C67C0 0030:
	_scripted_hud_show_health (0000)
000C67F0 0030:
	_scripted_hud_blink_health (0000)
000C6820 0030:
	_scripted_hud_show_shield (0000)
000C6850 0030:
	_scripted_hud_blink_shield (0000)
000C6880 0030:
	_scripted_hud_show_motion_sensor (0000)
000C68B0 0030:
	_scripted_hud_blink_motion_sensor (0000)
000C68E0 0220:
	_hud_play_unit_sounds (0000)
000C6B00 0080:
	_hud_fix_unit_data (0000)
000C6B80 0220:
	_hud_update_unit_local_player (0000)
000C6DA0 02b0:
	_hud_render_damage_indicators (0000)
000C7050 0040:
	_hud_tick_shield (0000)
000C7090 0030:
	_hud_update_unit (0000)
000C70C0 0db0:
	_hud_render_unit_interface (0000)
00270268 0011:
	??_C@_0BB@MIFNILGL@unit_hud_globals?$AA@ (0000)
0027027C 0024:
	??_C@_0CE@ICNCJJA@c?3?2halo?2SOURCE?2interface?2hud_uni@ (0000)
002702A0 0013:
	??_C@_0BD@OOGPMJCA@hud?5unit?5interface?$AA@ (0000)
002702B4 0004:
	__real@3e400000 (0000)
002702B8 001d:
	??_C@_0BN@FNBCLDEB@new_local_player_index?$CB?$DNNONE?$AA@ (0000)
002702D8 001d:
	??_C@_0BN@NPOPPJAG@old_local_player_index?$CB?$DNNONE?$AA@ (0000)
002E4C68 0004:
	?overcharge_count@?1??hud_render_unit_interface@@9@9 (0000)
00453AC0 0004:
	_unit_hud_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cutscene/cinematics.h"
#include "effects/player_effects.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "cache/texture_cache.h"
#include "interface/hud_definitions.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/hud_draw.h"
#include "interface/hud.h"
#include "interface/motion_sensor.h"
#include "interface/hud_unit.h"
#include "interface/unit_hud_interface_definition.h"
#include "render/render.h"
#include "saved games/game_state.h"
#include "tag_files/tag_groups.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	_hud_panel_health_dont_show_bit = 0,
	_hud_panel_health_blink_bit,
	_hud_panel_shield_dont_show_bit,
	_hud_panel_shield_blink_bit,
	_hud_panel_motion_sensor_dont_show_bit,
	_hud_panel_motion_sensor_blink_bit
};

enum
{
	MAXIMUM_NUMBER_OF_LOCAL_PLAYERS = 4,
};

enum hud_damage_indicator_direction
{
	_hud_damage_indicator_top = 0,
	_hud_damage_indicator_left,
	_hud_damage_indicator_bottom,
	_hud_damage_indicator_right,

	NUMBER_OF_HUD_DAMAGE_INDICATOR_DIRECTIONS
};

/* ---------- macros */

/* ---------- structures */

struct unit_hud_state
{
	real last_shield_vitality;
	real last_body_vitality;
	real fade_time;
	long last_shield_hit_time;
	long last_shield_flash_time;
	long last_health_flash_time;
	long last_motion_sensor_flash_time;
	long last_unit_index;
	word auxilary_active_type_flags;
	short auxilary_flash_time[1];
	word sound_flags;
	byte pad26[2];
	long last_sound_handles[12];
};

struct unit_hud_globals
{
	struct unit_hud_state hud_states[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	long script_flags;
};

struct hud_scripted_globals
{
	boolean show_hud;
	boolean show_hud_help_text;
	byte reserved2[2];
};

typedef char unit_hud_state_auxilary_flash_time_offset_assert[
	offsetof(struct unit_hud_state, auxilary_flash_time) == 0x22 ? 1 : -1];
typedef char unit_hud_state_sound_flags_offset_assert[
	offsetof(struct unit_hud_state, sound_flags) == 0x24 ? 1 : -1];
typedef char unit_hud_state_last_sound_handles_offset_assert[
	offsetof(struct unit_hud_state, last_sound_handles) == 0x28 ? 1 : -1];
typedef char unit_hud_state_size_assert[
	sizeof(struct unit_hud_state) == 0x58 ? 1 : -1];
typedef char unit_hud_globals_script_flags_offset_assert[
	offsetof(struct unit_hud_globals, script_flags) == 0x160 ? 1 : -1];
typedef char unit_hud_globals_size_assert[
	sizeof(struct unit_hud_globals) == 0x164 ? 1 : -1];
typedef char hud_scripted_globals_size_assert[
	sizeof(struct hud_scripted_globals) == 0x4 ? 1 : -1];
typedef char hud_messaging_parameters_definition_size_assert[
	sizeof(struct hud_messaging_parameters_definition) == 0x120 ? 1 : -1];
typedef char hud_waypoint_definition_size_assert[
	sizeof(struct hud_waypoint_definition) == 0x9C ? 1 : -1];
typedef char hud_multiplayer_parameters_definition_size_assert[
	sizeof(struct hud_multiplayer_parameters_definition) == 0x104 ? 1 : -1];
typedef char hud_defaults_definition_size_assert[
	sizeof(struct hud_defaults_definition) == 0x50 ? 1 : -1];
typedef char hud_damage_indicators_definition_indicator_bitmap_offset_assert[
	offsetof(struct hud_damage_indicators_definition, indicator_bitmap) == 0x28 ? 1 : -1];
typedef char hud_damage_indicators_definition_size_assert[
	sizeof(struct hud_damage_indicators_definition) == 0x50 ? 1 : -1];
typedef char hud_globals_definition_damage_indicators_offset_assert[
	offsetof(struct hud_globals_definition, damage_indicators) == 0x310 ? 1 : -1];

/* ---------- prototypes */

static struct unit_hud_state *get_hud_state(
	short local_player_index);
static void hud_update_unit_local_player(
	short local_player_index);

/* ---------- globals */

static struct unit_hud_globals *unit_hud_globals = NULL;
extern struct hud_scripted_globals *hud_scripted_globals;

/* ---------- private code */

static void unit_hud_outline_mapper_tick(
	void)
{
	return;
}

static void unit_hud_shield_meter_mapper_tick(
	void)
{
	return;
}

/* ---------- public code */

void unit_hud_shield_meter_mapper_init(
	void)
{
	return;
}

/* ---------- private code */

static void initialize_hud_state(
	struct unit_hud_state *hud_state)
{
	csmemset(
		hud_state->auxilary_flash_time,
		NONE,
		sizeof(hud_state->auxilary_flash_time));
	hud_state->last_body_vitality = -1.0f;
	hud_state->last_shield_vitality = -1.0f;
	hud_state->last_health_flash_time = NONE;
	hud_state->last_motion_sensor_flash_time = NONE;
	hud_state->fade_time = -1.0f;
	hud_state->last_unit_index = NONE;

	return;
}

static struct unit_hud_state *get_hud_state(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x106,
		local_player_index>=0 &&
			local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x107,
		unit_hud_globals);

	return &unit_hud_globals->hud_states[local_player_index];
}

/* ---------- public code */

void hud_initialize_unit_interface(
	void)
{
	unit_hud_globals = game_state_malloc(
		"hud unit interface",
		NULL,
		sizeof(*unit_hud_globals));

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x110,
		unit_hud_globals);

	return;
}

void hud_initialize_unit_interface_for_new_map(
	void)
{
	short local_player_index;
	struct unit_hud_state *hud_state;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x11B,
		unit_hud_globals);
	csmemset(
		unit_hud_globals,
		0,
		sizeof(*unit_hud_globals));

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		hud_state = get_hud_state(local_player_index);
		initialize_hud_state(hud_state);
		hud_state->sound_flags = 0;
		csmemset(
			hud_state->last_sound_handles,
			NONE,
			sizeof(hud_state->last_sound_handles));
	}

	return;
}

void hud_dispose_unit_interface_from_old_map(
	void)
{
	return;
}

void hud_dispose_unit_interface(
	void)
{
	return;
}

void hud_fix_unit_data(
	short old_local_player_index,
	short new_local_player_index)
{
	struct unit_hud_state *old_hud_state;
	struct unit_hud_state *new_hud_state;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x1AB,
		old_local_player_index!=NONE);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x1AC,
		new_local_player_index!=NONE);
	old_hud_state = get_hud_state(old_local_player_index);
	new_hud_state = get_hud_state(new_local_player_index);
	*new_hud_state = *old_hud_state;

	return;
}

void scripted_hud_show_health(
	boolean show)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_health_dont_show_bit,
		!show);

	return;
}

void scripted_hud_blink_health(
	boolean blink)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_health_blink_bit,
		blink);

	return;
}

void scripted_hud_show_shield(
	boolean show)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_shield_dont_show_bit,
		!show);

	return;
}

void scripted_hud_blink_shield(
	boolean blink)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_shield_blink_bit,
		blink);

	return;
}

void scripted_hud_show_motion_sensor(
	boolean show)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_motion_sensor_dont_show_bit,
		!show);

	return;
}

void scripted_hud_blink_motion_sensor(
	boolean blink)
{
	SET_FLAG(
		unit_hud_globals->script_flags,
		_hud_panel_motion_sensor_blink_bit,
		blink);

	return;
}

void hud_play_unit_sounds(
	struct player_datum const *player,
	boolean show_hud)
{
	struct unit_hud_state *hud_state = get_hud_state(player->local_player_index);
	long unit_index = player->unit_index;

	if (unit_index == NONE)
		unit_index = hud_state->last_unit_index;

	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit == NULL)
			return;

		{
			struct unit_definition const *unit_definition =
				unit_definition_get(unit->definition_index);
			long active_hud_index = unit_definition_get_active_hud_index(
				unit_definition,
				local_player_count() > 1);

			if (active_hud_index == NONE)
				return;

			{
				struct unit_hud_interface_definition const *hud_definition =
					unit_hud_interface_definition_get(active_hud_index);
				unsigned long sound_flags = 0;

				if (TEST_FLAG(unit->object.flags, _object_on_media_bit) ||
					!(unit->object.body_vitality > 0.0f))
				{
					hud_state->last_unit_index = NONE;
				}
				else if (show_hud && !cinematic_in_progress())
				{
					if (hud_state->last_shield_vitality != -1.0f &&
						game_engine_has_shield(local_player_get_player_index(
							player->local_player_index)))
					{
						if (!TEST_FLAG(
							unit_hud_globals->script_flags,
							_hud_panel_shield_dont_show_bit))
						{
							sound_flags = TEST_FLAG(
								unit->object.damage_flags,
								_object_shield_charging_bit);
							SET_FLAG(
								sound_flags,
								_unit_hud_shield_damage,
								hud_state->last_shield_vitality >
									unit->object.shield_vitality);
							SET_FLAG(
								sound_flags,
								_unit_hud_shield_low,
								unit->object.shield_vitality < 0.25f &&
									unit->object.shield_vitality > 0.0f);
							SET_FLAG(
								sound_flags,
								_unit_hud_shield_empty,
								unit->object.shield_vitality == 0.0f);
						}
					}

					if (!TEST_FLAG(
						unit_hud_globals->script_flags,
						_hud_panel_health_dont_show_bit))
					{
						SET_FLAG(
							sound_flags,
							_unit_hud_health_low,
							unit->object.body_vitality < 0.25f);
						SET_FLAG(
							sound_flags,
							_unit_hud_health_empty,
							TEST_FLAG(unit->object.damage_flags, _object_dead_bit));
						SET_FLAG(
							sound_flags,
							_unit_hud_minor_damage,
							hud_state->last_body_vitality >
								unit->object.body_vitality &&
								hud_state->last_body_vitality -
									unit->object.body_vitality < 0.1875f);
						SET_FLAG(
							sound_flags,
							_unit_hud_major_damage,
							hud_state->last_body_vitality -
								unit->object.body_vitality >= 0.1875f);
					}
				}

				hud_play_sound(
					player->local_player_index,
					sound_flags,
					&hud_definition->warning_sounds,
					hud_state->last_sound_handles,
					&hud_state->sound_flags);
			}
		}
	}

	return;
}

/* ---------- private code */

static void hud_update_unit_local_player(
	short local_player_index)
{
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));

	if (local_player_get_player_index(local_player_index) != NONE)
	{
		long unit_index = player_get(
			local_player_get_player_index(local_player_index))->unit_index;

		if (unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(unit_index);
			struct unit_hud_state *hud_state = get_hud_state(local_player_index);

			if (hud_state->last_body_vitality == -1.0f)
				hud_state->last_body_vitality = unit->object.body_vitality;
			if (hud_state->last_shield_vitality == -1.0f)
				hud_state->last_shield_vitality = unit->object.shield_vitality;

			if (hud_state->last_shield_vitality > unit->object.shield_vitality)
			{
				if (hud_state->fade_time < 0.0f || hud_state->fade_time > 1.0f)
					hud_state->last_shield_hit_time = game_time_get();

				if (game_time_get() - hud_state->last_shield_hit_time < 15)
				{
					hud_state->fade_time = 0.0f;
				}
				else
				{
					hud_state->last_shield_vitality = unit->object.shield_vitality;
					hud_state->fade_time +=
						(real)(game_time_get() - hud_state->last_shield_hit_time) *
						(1.0f / TICKS_PER_SECOND);
					hud_state->last_shield_hit_time = game_time_get();
				}
			}
			else if (hud_state->last_shield_vitality < unit->object.shield_vitality)
			{
				hud_state->last_shield_vitality = unit->object.shield_vitality;
				hud_state->fade_time = -1.0f;
				hud_state->last_shield_hit_time = game_time_get();
			}
			else
			{
				hud_state->last_shield_vitality = unit->object.shield_vitality;
				if (hud_state->fade_time > 0.0f)
				{
					hud_state->fade_time +=
						(real)(game_time_get() - hud_state->last_shield_hit_time) *
						(1.0f / TICKS_PER_SECOND);
				}
				hud_state->last_shield_hit_time = game_time_get();
			}
		}
	}

	if (cinematic_in_progress())
	{
		long player_index = local_player_get_player_index(local_player_index);

		if (player_index != NONE)
		{
			hud_play_unit_sounds(
				player_get(player_index),
				hud_scripted_globals->show_hud);
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_unit.c", 0x201);

	return;
}

/* ---------- public code */

void hud_render_damage_indicators(
	short local_player_index)
{
	long unit_index;

	if (local_player_index == NONE)
		return;

	if (local_player_get_player_index(local_player_index) == NONE)
	{
		unit_index = NONE;
	}
	else
	{
		unit_index = player_get(
			local_player_get_player_index(local_player_index))->unit_index;
	}

	if (unit_try_and_get(unit_index))
	{
		struct hud_damage_indicators_definition *definition =
			&hud_globals->damage_indicators;
		real theta;
		real_point2d screen_position;
		real scale = hud_globals_get_scale(local_player_count() > 1);
		byte damage_indicators[NUMBER_OF_HUD_DAMAGE_INDICATOR_DIRECTIONS];
		short direction;

		player_effect_get_damage_indicators(
			local_player_index,
			damage_indicators);

		for (direction = 0;
			direction < NUMBER_OF_HUD_DAMAGE_INDICATOR_DIRECTIONS;
			direction++)
		{
			if (damage_indicators[direction] > 0 &&
				damage_indicators[direction] < TICKS_PER_SECOND)
			{
				long bitmap_group_index;
				short sequence_index;
				struct bitmap_data const *bitmap;
				real_rectangle2d const *clip;

				switch (direction)
				{
				case _hud_damage_indicator_top:
					theta = _pi;
					screen_position.y = (real)(
						render.camera.window_bounds.y0 +
						definition->top_offset);
					screen_position.x = (real)(
						(render.camera.viewport_bounds.x0 +
							render.camera.viewport_bounds.x1) / 2);
					break;

				case _hud_damage_indicator_left:
					theta = _pi / 2.0f;
					screen_position.x = (real)(
						render.camera.window_bounds.x0 +
						definition->left_offset);
					screen_position.y = (real)(
						(render.camera.viewport_bounds.y0 +
							render.camera.viewport_bounds.y1) / 2);
					break;

				case _hud_damage_indicator_bottom:
					theta = 0.0f;
					screen_position.y = (real)(
						render.camera.window_bounds.y1 -
						definition->bottom_offset);
					screen_position.x = (real)(
						(render.camera.viewport_bounds.x0 +
							render.camera.viewport_bounds.x1) / 2);
					break;

				case _hud_damage_indicator_right:
					theta = 3.0f * _pi / 2.0f;
					screen_position.x = (real)(
						render.camera.window_bounds.x1 -
						definition->right_offset);
					screen_position.y = (real)(
						(render.camera.viewport_bounds.y0 +
							render.camera.viewport_bounds.y1) / 2);
					break;

				/* theta and screen_position are left unassigned only by this default arm. Not reached unassigned: the
				 * arm's assertion failure calls system_exit, which does not return in January
				 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
				 * Source-policy approval pending (2026-09-27 audit). */
				default:
					match_assert(
						"c:\\halo\\SOURCE\\interface\\hud_unit.c",
						0x400,
						!"unreachable");
					break;
				}

				screen_position.x -= (real)render.camera.viewport_bounds.x0;
				screen_position.y -= (real)render.camera.viewport_bounds.y0;
				bitmap_group_index = definition->indicator_bitmap.index;
				if (local_player_count() > 1)
					sequence_index = definition->multiplayer_sequence_index;
				else
					sequence_index = definition->sequence_index;
				bitmap = NULL;
				clip = NULL;
				hud_retrieve_bitmap_and_bounding_rect(
					bitmap_group_index,
					sequence_index,
					0,
					&bitmap,
					&clip);

				if (bitmap &&
					_texture_cache_bitmap_get_hardware_format(
						(struct bitmap_data *)bitmap,
						FALSE,
						TRUE))
				{
					point2d corner;

					corner.x = (short)(long)screen_position.x;
					corner.y = (short)(long)screen_position.y;
					hud_draw_bitmap_direct(
						bitmap,
						_hud_anchor_center,
						&corner,
						clip,
						scale,
						theta,
						definition->color,
						FALSE);
				}
			}
		}
	}
	else
	{
		player_effect_clear_damage_indicators(local_player_index);
	}

	return;
}

/* ---------- public code */

void hud_tick_shield(
	long player_index,
	real amount)
{
	short local_player_index = player_get(player_index)->local_player_index;

	if (local_player_index != NONE)
		get_hud_state(local_player_index)->last_shield_vitality -= amount;

	return;
}

void hud_update_unit(
	void)
{
	short local_player_index = local_player_get_next(NONE);

	while (local_player_index != NONE)
	{
		hud_update_unit_local_player(local_player_index);
		local_player_index = local_player_get_next(local_player_index);
	}

	return;
}

/* ---------- public code */

void hud_render_unit_interface(
	struct player_datum *player)
{
	static long overcharge_count = 4;
	long return_eip = get_return_eip();
	long stack_buffer[STACK_BUFFER_LENGTH];

	csmemset(stack_buffer, 0x62, sizeof(stack_buffer));
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud_unit.c",
		0x209,
		player->local_player_index==render.local_player_index);

	unit_hud_shield_meter_mapper_tick();
	unit_hud_outline_mapper_tick();

	if (player->local_player_index == render.local_player_index &&
		player->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);
		struct unit_definition const *unit_definition =
			unit_definition_get(unit->definition_index);
		short local_player_index = player->local_player_index;
		long player_index = local_player_get_player_index(local_player_index);
		struct unit_hud_state *hud_state = get_hud_state(local_player_index);
		long unit_indices[18] = { player->unit_index };
		long unit_hud_indices[18] =
		{
			unit_definition_get_active_hud_index(
				unit_definition,
				local_player_count() > 1)
		};
		long unit_count = 1;
		unsigned long auxilary_flags;
		unsigned long aux_activated_when_disabled_flags;
		real auxilary_values[1];

		if (hud_state->last_unit_index == NONE)
			initialize_hud_state(get_hud_state(player->local_player_index));
		hud_state->last_unit_index = player->unit_index;

		if (unit->object.parent_object_index != NONE &&
			unit->unit.parent_seat_index != NONE)
		{
			long parent_unit_index = unit->object.parent_object_index;
			struct unit_datum *parent_unit = unit_get(parent_unit_index);
			struct unit_definition const *parent_unit_definition =
				unit_definition_get(parent_unit->definition_index);
			struct unit_seat const *seat = TAG_BLOCK_GET_ELEMENT(
				&parent_unit_definition->unit.seats,
				unit->unit.parent_seat_index,
				struct unit_seat);
			long parent_hud_index;

			get_hud_state(local_player_index);
			parent_hud_index = unit_definition_get_active_hud_index(
				parent_unit_definition,
				local_player_count() > 1);

			if (TEST_FLAG(seat->flags, _unit_seat_driver_bit))
			{
				long child_unit_index;

				if (parent_hud_index != NONE)
				{
					unit_indices[1] = parent_unit_index;
					unit_hud_indices[1] = parent_hud_index;
					unit_count = 2;
				}

				child_unit_index = parent_unit->object.first_child_object_index;
				while (child_unit_index != NONE && unit_count < NUMBEROF(unit_indices))
				{
					struct object_datum *child_object = object_get(child_unit_index);
					struct unit_datum *child_unit = unit_try_and_get(child_unit_index);

					if (child_unit &&
						child_unit->object.parent_object_index == parent_unit_index &&
						child_unit->unit.parent_seat_index != NONE)
					{
						unit_indices[unit_count] = child_unit_index;
						unit_hud_indices[unit_count] =
							unit_definition_get_seat_active_hud_index(
								parent_unit_definition,
								child_unit->unit.parent_seat_index,
								local_player_count() > 1);
						unit_count++;
					}

					child_unit_index = child_object->object.next_object_index;
				}
			}
		}

		auxilary_flags = unit->unit.integrated_light_power == 1.0f;
		aux_activated_when_disabled_flags =
			!TEST_FLAG(unit->unit.flags, _unit_integrated_light_on_bit) &&
			unit->unit.integrated_light_battery < 0.2f &&
			TEST_FLAG(unit->unit.control_flags, _unit_control_integrated_light_bit);
		auxilary_values[0] = unit->unit.integrated_light_battery;

		while (unit_count)
		{
			struct unit_datum *hud_unit;

			unit_count--;
			hud_unit = unit_try_and_get(unit_indices[unit_count]);

			if (hud_unit && unit_hud_indices[unit_count] != NONE)
			{
				struct unit_hud_interface_definition *hud_definition =
					unit_hud_interface_definition_get(unit_hud_indices[unit_count]);

				if (hud_definition->background.interface_bitmap.index != NONE)
				{
					short draw_flags = 0;

					SET_FLAG(
						draw_flags,
						_hud_draw_disabled_bit,
						TEST_FLAG(hud_unit->object.damage_flags, _object_dead_bit));
					SET_FLAG(
						draw_flags,
						_hud_draw_in_multiplayer_bit,
						local_player_count() > 1);
					hud_draw_static_element(
						local_player_index,
						&hud_definition->absolute_placement,
						&hud_definition->background,
						draw_flags,
						NONE);
				}

				if (game_engine_has_shield(player_index) &&
					!TEST_FLAG(
						unit_hud_globals->script_flags,
						_hud_panel_shield_dont_show_bit))
				{
					short draw_flags =
						hud_unit->object.shield_vitality < 0.25f ||
						TEST_FLAG(
							unit_hud_globals->script_flags,
							_hud_panel_shield_blink_bit);

					SET_FLAG(
						draw_flags,
						_hud_draw_disabled_bit,
						TEST_FLAG(hud_unit->object.damage_flags, _object_dead_bit));
					SET_FLAG(
						draw_flags,
						_hud_draw_in_multiplayer_bit,
						local_player_count() > 1);

					if (unit_count == 0)
					{
						if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
						{
							if (hud_state->last_shield_flash_time == NONE)
								hud_state->last_shield_flash_time = game_time_get();
						}
						else
						{
							hud_state->last_shield_flash_time = NONE;
						}
					}

					if (hud_definition->shield_meter.meter.meter_bitmap.index != NONE)
					{
						real reference_shield_vitality;
						short value_scale;
						struct meter_hud_element_definition overcharge_meter;

						game_engine_running();
						if (unit_count == 0)
							reference_shield_vitality = hud_state->last_shield_vitality;
						else
							reference_shield_vitality = hud_unit->object.shield_vitality;
						value_scale = hud_definition->shield_meter.meter.value_scale ?
							hud_definition->shield_meter.meter.value_scale :
							UNSIGNED_CHAR_MAX;
						overcharge_meter = hud_definition->shield_meter.meter;
						{
							pixel32 color[5] =
							{
								0,
								0x00FF0000,
								0x0000FF00,
								0x00FFFF00,
								0x007F00FF,
							};
							pixel32 *overcharge_color = color;
							long overcharge_index;

							for (overcharge_index = 0;
								overcharge_index <= overcharge_count;
								overcharge_color++, overcharge_index++)
							{
								boolean first_meter = overcharge_index == 0;
								real shield_vitality = PIN(
									hud_unit->object.shield_vitality - (real)overcharge_index,
									0.0f,
									1.0f);
								real last_shield_vitality = PIN(
									reference_shield_vitality - (real)overcharge_index,
									0.0f,
									1.0f);
								boolean shield_fading = last_shield_vitality > shield_vitality;
								real maximum_shield_vitality = shield_fading ?
									last_shield_vitality :
									shield_vitality;
								real reference_time;

								if (shield_vitality <= 0.0f &&
									maximum_shield_vitality <= 0.0f)
								{
									break;
								}

								overcharge_meter.min_color = *overcharge_color;
								overcharge_meter.max_color = *overcharge_color;
								reference_time = shield_fading ? hud_state->fade_time : -1.0f;
								hud_draw_meter(
									local_player_index,
									&hud_definition->absolute_placement,
									first_meter ?
										&hud_definition->shield_meter.meter :
										&overcharge_meter,
									(byte)PIN(
										fast_ftol((real)value_scale * shield_vitality),
										0,
										UNSIGNED_CHAR_MAX),
									(byte)PIN(
										fast_ftol((real)value_scale * maximum_shield_vitality),
										0,
										UNSIGNED_CHAR_MAX),
									draw_flags,
									reference_time,
									shield_vitality);
							}
						}
					}

					if (hud_definition->shield_meter.background.interface_bitmap.index != NONE)
					{
						hud_draw_static_element(
							local_player_index,
							&hud_definition->absolute_placement,
							&hud_definition->shield_meter.background,
							draw_flags,
							hud_state->last_shield_flash_time);
					}
				}

				if (!TEST_FLAG(
					unit_hud_globals->script_flags,
					_hud_panel_health_dont_show_bit))
				{
					short draw_flags =
						TEST_FLAG(
							hud_unit->object.damage_flags,
							_object_shield_depleted_bit) ||
						TEST_FLAG(
							unit_hud_globals->script_flags,
							_hud_panel_health_blink_bit);

					SET_FLAG(
						draw_flags,
						_hud_draw_disabled_bit,
						TEST_FLAG(hud_unit->object.damage_flags, _object_dead_bit));
					SET_FLAG(
						draw_flags,
						_hud_draw_in_multiplayer_bit,
						local_player_count() > 1);

					if (unit_count == 0)
					{
						if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
						{
							if (hud_state->last_health_flash_time == NONE)
								hud_state->last_health_flash_time = game_time_get();
						}
						else
						{
							hud_state->last_health_flash_time = NONE;
						}
					}

					if (hud_definition->health_meter.meter.meter_bitmap.index != NONE)
					{
						short value_scale = hud_definition->health_meter.meter.value_scale ?
							hud_definition->health_meter.meter.value_scale :
							8;
						struct meter_hud_element_definition health_meter =
							hud_definition->health_meter.meter;

						if (hud_unit->object.body_vitality >=
							hud_definition->health_meter.health_extras.max_cutoff)
						{
							health_meter.min_color = health_meter.max_color;
						}
						else if (hud_unit->object.body_vitality <=
							hud_definition->health_meter.health_extras.min_cutoff)
						{
							health_meter.max_color = health_meter.min_color;
						}
						else
						{
							health_meter.max_color =
								hud_definition->health_meter.health_extras.mid_color;
							health_meter.min_color = health_meter.max_color;
						}

						hud_draw_meter(
							local_player_index,
							&hud_definition->absolute_placement,
							&health_meter,
							(byte)PIN(
								fast_ftol_C(
									(real)value_scale * hud_unit->object.body_vitality),
								0,
								UNSIGNED_CHAR_MAX),
							(byte)PIN(
								fast_ftol_C(
									(real)value_scale * hud_unit->object.body_vitality),
								0,
								UNSIGNED_CHAR_MAX),
							draw_flags,
							-1.0f,
							hud_unit->object.body_vitality);
					}

					if (hud_definition->health_meter.background.interface_bitmap.index != NONE)
					{
						hud_draw_static_element(
							local_player_index,
							&hud_definition->absolute_placement,
							&hud_definition->health_meter.background,
							draw_flags,
							hud_state->last_health_flash_time);
					}

					hud_state->last_body_vitality = hud_unit->object.body_vitality;
				}

				if (unit_count == 0 &&
					!TEST_FLAG(
						unit_hud_globals->script_flags,
						_hud_panel_motion_sensor_dont_show_bit) &&
					game_engine_hud_draw_motion_sensor(player_index))
				{
					struct hud_absolute_placement_definition absolute_placement;
					short draw_flags = 0;
					point2d corner;

					absolute_placement.corner = _hud_anchor_bottom_left;
					SET_FLAG(
						draw_flags,
						_hud_draw_in_multiplayer_bit,
						local_player_count() > 1);
					SET_FLAG(
						draw_flags,
						_hud_draw_flashing_bit,
						TEST_FLAG(
							unit_hud_globals->script_flags,
							_hud_panel_motion_sensor_blink_bit));

					if (TEST_FLAG(draw_flags, _hud_draw_flashing_bit))
					{
						if (hud_state->last_motion_sensor_flash_time == NONE)
							hud_state->last_motion_sensor_flash_time = game_time_get();
					}
					else
					{
						hud_state->last_motion_sensor_flash_time = NONE;
					}

					if (hud_definition->motion_sensor.background.interface_bitmap.index != NONE)
					{
						hud_draw_static_element(
							local_player_index,
							&absolute_placement,
							&hud_definition->motion_sensor.background,
							draw_flags,
							NONE);
					}
					if (hud_definition->motion_sensor.foreground.interface_bitmap.index != NONE)
					{
						hud_draw_static_element(
							local_player_index,
							&absolute_placement,
							&hud_definition->motion_sensor.foreground,
							draw_flags,
							NONE);
					}

					hud_calculate_point(
						local_player_index,
						&absolute_placement,
						&hud_definition->blip_placement,
						NULL,
						local_player_count() > 1,
						0.0f,
						&corner);
					motion_sensor_draw_screen(
						local_player_index,
						local_player_count() > 1,
						&corner);
				}

				{
					struct auxilary_panel_definition *auxilary_panel = &hud_definition->auxilary_panel;
					word overlay_type_flags = game_engine_has_teams() != FALSE;
					short draw_flags = 0;
					short overlay_index;

					SET_FLAG(
						draw_flags,
						_hud_draw_in_multiplayer_bit,
						local_player_count() > 1);
					for (overlay_index = 0;
						overlay_index < auxilary_panel->auxilary_overlays.count;
						overlay_index++)
					{
						struct auxilary_overlay_definition *overlay = TAG_BLOCK_GET_ELEMENT(
							&auxilary_panel->auxilary_overlays,
							overlay_index,
							struct auxilary_overlay_definition);

						if (TEST_FLAG(overlay_type_flags, overlay->type))
						{
							if (TEST_FLAG(
								overlay->flags,
								_auxilary_overlay_use_team_color_bit))
							{
								overlay->static_element.colors.color =
									real_rgb_color_to_pixel32(
										&hud_unit->object.base_change_colors[0]) |
									0xFF000000;
							}

							hud_draw_static_element(
								local_player_index,
								&auxilary_panel->absolute_placement,
								&overlay->static_element,
								draw_flags,
								NONE);
						}
					}
				}

				{
					short meter_index;

					for (meter_index = 0;
						meter_index < hud_definition->auxilary_meters.count;
						meter_index++)
					{
						struct auxilary_meter_definition *meter = TAG_BLOCK_GET_ELEMENT(
							&hud_definition->auxilary_meters,
							meter_index,
							struct auxilary_meter_definition);

						if (TEST_FLAG(
								hud_state->auxilary_active_type_flags,
								meter->type) &&
							!TEST_FLAG(auxilary_flags, meter->type))
						{
							hud_state->auxilary_flash_time[meter->type] = NONE;
						}

						if (TEST_FLAG(auxilary_flags, meter->type))
						{
							long background_bitmap_index = verify_tag_reference(
								&meter->panel.background.interface_bitmap);
							long meter_bitmap_index = verify_tag_reference(
								&meter->panel.meter.meter_bitmap);
							short draw_flags = 0;

							SET_FLAG(
								draw_flags,
								_hud_draw_in_multiplayer_bit,
								local_player_count() > 1);
							SET_FLAG(
								draw_flags,
								_hud_draw_flashing_bit,
								auxilary_values[meter->type] <=
									meter->panel.aux_extras.min_cutoff);
							hud_state->auxilary_flash_time[meter->type] += game_time_get_elapsed();
							hud_state->auxilary_flash_time[meter->type] %= 2 * get_flash_duration(
								&meter->panel.background.colors);

							if (background_bitmap_index != NONE)
							{
								hud_draw_static_element(
									local_player_index,
									&hud_definition->absolute_placement,
									&meter->panel.background,
									draw_flags,
									game_time_get() - hud_state->auxilary_flash_time[meter->type]);
							}

							if (meter_bitmap_index != NONE)
							{
								real value_scale = (real)meter->panel.meter.value_scale;

								hud_draw_meter(
									local_player_index,
									&hud_definition->absolute_placement,
									&meter->panel.meter,
									(byte)PIN(
										fast_ftol(value_scale * auxilary_values[meter->type]),
										0,
										UNSIGNED_CHAR_MAX),
									(byte)PIN(
										fast_ftol(value_scale * auxilary_values[meter->type]),
										0,
										UNSIGNED_CHAR_MAX),
									draw_flags,
									-1.0f,
									auxilary_values[meter->type]);
							}
						}
						else if (TEST_FLAG(
							aux_activated_when_disabled_flags,
							meter->type) ||
							(hud_state->auxilary_flash_time[meter->type] != NONE &&
								hud_state->auxilary_flash_time[meter->type] <
									get_flash_duration(&meter->panel.background.colors)))
						{
							long background_bitmap_index = verify_tag_reference(
								&meter->panel.background.interface_bitmap);
							short draw_flags = 0;

							SET_FLAG(
								draw_flags,
								_hud_draw_in_multiplayer_bit,
								local_player_count() > 1);
							SET_FLAG(
								draw_flags,
								_hud_draw_flashing_bit,
								TRUE);
							hud_state->auxilary_flash_time[meter->type] += game_time_get_elapsed();

							if (background_bitmap_index != NONE)
							{
								hud_draw_static_element(
									local_player_index,
									&hud_definition->absolute_placement,
									&meter->panel.background,
									draw_flags,
									game_time_get() - hud_state->auxilary_flash_time[meter->type]);
							}
						}
						else
						{
							hud_state->auxilary_flash_time[meter->type] = NONE;
						}
					}
				}

				hud_state->auxilary_active_type_flags = (word)auxilary_flags;
			}
		}
	}

	match_assert_stack_frame("c:\\halo\\SOURCE\\interface\\hud_unit.c", 0x3C9);

	return;
}
