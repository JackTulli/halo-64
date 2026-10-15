/*
HUD.C

symbols in this file:
000BF660 0030:
	_code_000bf660 (0000)
000BF690 0050:
	_hud_initialize (0000)
000BF6E0 0020:
	_hud_dispose (0000)
000BF700 0080:
	_hud_initialize_for_new_map (0000)
000BF780 0020:
	_hud_dispose_from_old_map (0000)
000BF7A0 0020:
	_scripted_show_hud (0000)
000BF7C0 0020:
	_scripted_show_hud_help_text (0000)
000BF7E0 0060:
	_hud_update (0000)
000BF840 0030:
	_code_000bf840 (0000)
000BF870 06d0:
	_code_000bf870 (0000)
000BFF40 0050:
	_hud_get_item_string (0000)
000BFF90 0020:
	_hud_picked_up_grenade (0000)
000BFFB0 0030:
	_hud_picked_up_ammunition (0000)
000BFFE0 0020:
	_hud_picked_up_weapon (0000)
000C0000 0020:
	_hud_picked_up_powerup (0000)
000C0020 00d0:
	_code_000c0020 (0000)
000C00F0 0060:
	_hud_load (0000)
000C0150 00e0:
	_hud_autosave (0000)
000C0230 0160:
	_hud_draw_friendly_indicator (0000)
000C0390 00a0:
	_hud_draw_players (0000)
000C0430 0370:
	_code_000c0430 (0000)
000C07A0 0140:
	_hud_draw_screen (0000)
0026FC1C 0015:
	??_C@_0BF@NENKDAEK@hud_scripted_globals?$AA@ (0000)
0026FC34 001f:
	??_C@_0BP@ILJLOMEO@c?3?2halo?2SOURCE?2interface?2hud?4c?$AA@ (0000)
0026FC54 0015:
	??_C@_0BF@MENHAMGO@hud?5scripted?5globals?$AA@ (0000)
0026FC6C 0036:
	??_C@_0DG@OKALAJAB@interface_get_tag_index?$CI_interfa@ (0000)
0026FCA4 0013:
	??_C@_0BD@EAHBLILM@ACTIVE?9CAMOUFLAGE?5?$AA@ (0000)
0026FCB8 0016:
	??_C@_0BG@GFCGCBJE@FULL?9SPECTRUM?5VISION?5?$AA@ (0000)
0026FCD0 0005:
	??_C@_04OPODHAPJ@?$CFs?$HMn?$AA@ (0000)
0026FCD8 0037:
	??_C@_0DH@GKJDMEOF@?$CFs?$HMntotal?5?$CFd?1?$CFd?$HMnloaded?5?$CFd?1?$CFd?$HMnh@ (0000)
0026FD10 0008:
	??_C@_07IFCAFJHP@?$CFd?5?$CFs?$HMn?$AA@ (0000)
0026FD18 001a:
	??_C@_0BK@NJIPBDGF@?$CFs?5?$CIpress?5WHITE?5to?5use?$CJ?$HMn?$AA@ (0000)
002E4C34 0004:
	_default_string (0000)
00453AAC 0008:
	_hud_globals (0000)
	_hud_scripted_globals (0004)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "bitmaps/bitmap_group.h"
#include "camera/director.h"
#include "cache/texture_cache.h"
#include "cutscene/cinematics.h"
#include "devices/device_controls.h"
#include "game/game_engine.h"
#include "game/game_globals.h"
#include "game/player_control.h"
#include "game/players.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_messaging.h"
#include "interface/hud_unit.h"
#include "interface/hud_weapon.h"
#include "interface/interface.h"
#include "interface/motion_sensor.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "memory/data.h"
#include "objects/object_definitions.h"
#include "objects/objects.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"
#include "render/render_camera_projection.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "tag_files/tag_files.h"
#include "text/draw_string.h"
#include "text/text_group.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	NUMBER_OF_TEMPORARY_HUD_RETICLE_POINTS = 16
};

enum player_respawn_failure
{
	_player_respawn_failure_none = 0,
	_player_respawn_failure_combat,
	_player_respawn_failure_enemies,
	_player_respawn_failure_moving,
	_player_respawn_failure_vehicle,
	NUMBER_OF_PLAYER_RESPAWN_FAILURES,
};

enum hud_state_message_type
{
	_hud_message_pickup = 0,
	_hud_message_swap_powerup,
	_hud_message_touch_device,
	_hud_message_custom_device,
	_hud_message_swap_weapon,
	_hud_message_remind_to_switch_weapons,
	_hud_message_enter_vehicle,
	_hud_message_exit_vehicle,
	_hud_message_flip_vehicle,
	_hud_message_respawn_failed_moving,
	_hud_message_respawn_failed_unsafe,
	_hud_message_respawn_failed_combat,
	_hud_message_respawn_failed_vehicle,
	NUMBER_OF_HUD_STATE_MESSAGE_TYPES,
};

/* ---------- macros */

#define hud_globals_definition_get(index) \
	((struct hud_globals_definition *)tag_get('hudg', (index)))
#define weapon_hud_interface_definition_get(index) \
	((struct weapon_hud_interface_definition *)tag_get('wphi', (index)))

/* ---------- structures */

struct hud_scripted_globals
{
	boolean show_hud;
	boolean show_hud_help_text;
	byte reserved2[2];
};

struct icon_hud_element_definition
{
	short sequence_index;
	short width_offset;
	point2d offset;
	unsigned long color;
	char frame_rate;
	byte flags;
	short text_index;
};

struct weapon_hud_interface_definition
{
	byte reserved000[0x13C];
	struct icon_hud_element_definition messaging_icon;
	byte reserved14C[0x30];
};

struct weapon_interface_magazine_state
{
	boolean reloading;
	boolean can_fire;
	short rounds_loaded;
	short rounds_loaded_maximum;
	short rounds_remaining;
	short rounds_remaining_maximum;
};

struct weapon_interface_state
{
	real heat;
	real age;
	boolean overheated;
	byte pad09;
	short magazine_count;
	struct weapon_interface_magazine_state magazines[2];
};

typedef char hud_scripted_globals_size_assert[
	sizeof(struct hud_scripted_globals) == 0x4 ? 1 : -1];
typedef char hud_scripted_globals_show_hud_help_text_offset_assert[
	offsetof(struct hud_scripted_globals, show_hud_help_text) == 0x1 ? 1 : -1];
typedef char hud_globals_hud_item_messages_index_offset_assert[
	offsetof(struct hud_globals_definition, messaging.hud_item_messages.index) == 0xA0 ? 1 : -1];
typedef char hud_globals_loading_begin_index_offset_assert[
	offsetof(struct hud_globals_definition, loading_begin_index) == 0x3D8 ? 1 : -1];
typedef char hud_globals_checkpoint_sound_index_offset_assert[
	offsetof(struct hud_globals_definition, checkpoint_sound.index) == 0x3EC ? 1 : -1];
typedef char icon_hud_element_definition_size_assert[
	sizeof(struct icon_hud_element_definition) == 0x10 ? 1 : -1];
typedef char weapon_hud_interface_definition_messaging_icon_offset_assert[
	offsetof(struct weapon_hud_interface_definition, messaging_icon) == 0x13C ? 1 : -1];
typedef char weapon_interface_state_size_assert[
	sizeof(struct weapon_interface_state) == 0x20 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

struct hud_globals_definition *hud_globals = NULL;
struct hud_scripted_globals *hud_scripted_globals = NULL;
extern boolean temporary_hud;

wchar_t const *default_string = L"";

/* ---------- public code */

static boolean weapon_state_is_depleted(
	struct weapon_interface_state const *state)
{
	return (state->magazines[0].rounds_loaded_maximum &&
		!state->magazines[0].rounds_loaded &&
		!state->magazines[0].rounds_remaining) ||
		state->age == 1.0f;
}

void hud_initialize(
	void)
{
	hud_scripted_globals = game_state_malloc(
		"hud scripted globals",
		NULL,
		sizeof(*hud_scripted_globals));
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud.c",
		87,
		hud_scripted_globals);
	hud_messaging_initialize();
	hud_initialize_unit_interface();
	hud_initialize_weapon_interface();
	hud_initialize_nav_points();
	motion_sensor_initialize();
	return;
}

void hud_dispose(
	void)
{
	motion_sensor_dispose();
	hud_dispose_nav_points();
	hud_dispose_weapon_interface();
	hud_dispose_unit_interface();
	hud_messaging_dispose();
	return;
}

void hud_initialize_for_new_map(
	void)
{
	csmemset(hud_scripted_globals, 0, sizeof(*hud_scripted_globals));
	hud_scripted_globals->show_hud = TRUE;
	match_assert(
		"c:\\halo\\SOURCE\\interface\\hud.c",
		113,
		interface_get_tag_index(_interface_hud_globals)!=NONE);
	hud_globals = hud_globals_definition_get(
		interface_get_tag_index(_interface_hud_globals));
	hud_messaging_initialize_for_new_map();
	hud_initialize_unit_interface_for_new_map();
	hud_initialize_weapon_interface_for_new_map();
	hud_initialize_nav_points_for_new_map();
	motion_sensor_initialize_for_new_map();
	return;
}

void hud_dispose_from_old_map(
	void)
{
	motion_sensor_dispose_from_old_map();
	hud_dispose_nav_points_from_old_map();
	hud_dispose_weapon_interface_from_old_map();
	hud_dispose_unit_interface_from_old_map();
	hud_messaging_dispose_from_old_map();
	return;
}

boolean scripted_show_hud(
	boolean show)
{
	hud_scripted_globals->show_hud = show;
	return hud_scripted_globals->show_hud;
}

boolean scripted_show_hud_help_text(
	boolean show)
{
	hud_scripted_globals->show_hud_help_text = show;
	return hud_scripted_globals->show_hud_help_text;
}

void hud_update(
	void)
{
	short local_player_index;
	long player_index;

	hud_update_weapon();
	hud_update_unit();
	hud_update_nav_points();
	hud_messaging_globals_update();
#ifdef HALO_N64
	/* the motion sensor's history and scan, once a tick (hud_draw_screen
	asks too, but only at the HUD's recordings: motion_sensor_tick) */
	if (!game_time_get_paused() && local_player_count() > 0)
		motion_sensor_tick();
#endif
	if (game_engine_force_single_screen())
	{
		for (local_player_index = 0;
			local_player_index < MAXIMUM_LOCAL_PLAYERS;
			local_player_index++)
		{
			player_index = local_player_get_player_index(local_player_index);
			if (player_index != NONE)
			{
				hud_play_unit_sounds(
					datum_get(player_data, player_index),
					FALSE);
			}
		}
	}
	return;
}

static long get_object_icon_text_index(
	long object_index)
{
	if (object_index == NONE)
		return NONE;

	return object_definition_get(
		object_get(object_index)->definition_index)->object.icon_text_index;
}

#ifdef HALO_N64
/* State-message templates contain Xbox button/weapon icons. The N64 text
backend cannot compose those icon fragments. Draw one complete N64 prompt,
but only after the real interaction scan has selected an eligible weapon. */
extern void n64_hud_game_weapon_prompt(char const *name, int swap);
static boolean n64_hud_weapon_action_response(struct player_datum *player)
{
	struct weapon_datum *weapon;
	struct object_definition *definition;
	char name[48];
	const char *fallback;
	wchar_t const *localized = NULL;
	long strings;
	short i = 0;
	if (player->unit_index == NONE || player->action_object_index == NONE ||
		(player->action_result != _player_action_result_swap_for_weapon &&
		player->action_result != _player_action_result_add_weapon_to_inventory)) return FALSE;
	weapon = weapon_try_and_get(player->action_object_index);
	if (!weapon || weapon->object.parent_object_index != NONE) return FALSE;
	definition = object_definition_get(weapon->definition_index);
	strings = hud_globals->messaging.alternate_icon_text.index;
	if (strings != NONE && definition->object.icon_text_index >= 0)
	{
		struct string_list *list = unicode_string_list_definition_get(strings);
		if (definition->object.icon_text_index < list->strings.count)
			localized = unicode_string_list_get_string(strings, definition->object.icon_text_index);
	}
	if (localized && localized[0] && localized[0] != '<')
		while (i < (short)sizeof(name) - 1 && localized[i])
		{
			name[i] = localized[i] >= 32 && localized[i] < 127 ? (char)localized[i] : '?';
			i++;
		}
	if (!i)
	{
		fallback = tag_name_strip_path(tag_get_name(weapon->definition_index));
		if (!fallback || !fallback[0]) fallback = "WEAPON";
		while (i < (short)sizeof(name) - 1 && fallback[i])
		{
			name[i] = fallback[i] == '_' ? ' ' : fallback[i];
			i++;
		}
	}
	name[i] = 0;
	for (i = 0; name[i]; i++) if (name[i] >= 'a' && name[i] <= 'z') name[i] -= 'a' - 'A';
	/* Only the replaced state line is cleared; kill/pickup messages remain. */
	hud_enable_custom_state_message(render.local_player_index, FALSE);
	n64_hud_game_weapon_prompt(name, player->action_result == _player_action_result_swap_for_weapon);
	return TRUE;
}
#endif

static void hud_show_action_response(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	short respawn_failure = players_get_respawn_failure();
	short item_name_index;

#ifdef HALO_N64
	if (n64_hud_weapon_action_response(player)) return;
#endif

	if (respawn_failure != _player_respawn_failure_none &&
		player->unit_index == NONE)
	{
		switch (respawn_failure)
		{
		case _player_respawn_failure_combat:
			hud_set_state_message(
				render.local_player_index,
				_hud_message_respawn_failed_combat);
			return;

		case _player_respawn_failure_enemies:
			hud_set_state_message(
				render.local_player_index,
				_hud_message_respawn_failed_unsafe);
			return;

		case _player_respawn_failure_moving:
			hud_set_state_message(
				render.local_player_index,
				_hud_message_respawn_failed_moving);
			return;

		case _player_respawn_failure_vehicle:
			hud_set_state_message(
				render.local_player_index,
				_hud_message_respawn_failed_vehicle);
			return;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\interface\\hud.c",
				424,
				!"unreachable");
			break;
		}

		return;
	}

	if (player->action_object_index == NONE)
	{
		item_name_index = NONE;
	}
	else
	{
		item_name_index = object_definition_get(
			object_get(player->action_object_index)->definition_index)->object.icon_text_index;
	}

	switch (player->action_result)
	{
	case _player_action_result_pickup_powerup:
	case _player_action_result_pickup_weapon:
		hud_set_state_message(
			render.local_player_index,
			_hud_message_pickup);
		hud_set_state_message_text(
			render.local_player_index,
			0,
			item_name_index,
			FALSE);
		return;

	case _player_action_result_swap_for_powerup:
		hud_set_state_message(
			render.local_player_index,
			_hud_message_swap_powerup);
		hud_set_state_message_text(
			render.local_player_index,
			0,
			(short)get_object_icon_text_index(
				unit_get_current_equipment(player->unit_index)),
			FALSE);
		hud_set_state_message_text(
			render.local_player_index,
			1,
			item_name_index,
			FALSE);
		return;

	case _player_action_result_exit_vehicle:
	{
		struct unit_datum *unit = unit_get(player->unit_index);

		hud_set_state_message(
			render.local_player_index,
			_hud_message_exit_vehicle);
		hud_set_state_message_text(
			render.local_player_index,
			0,
			(short)get_object_icon_text_index(unit->object.parent_object_index),
			FALSE);
		return;
	}

	case _player_action_result_touch_device:
	{
		struct control_datum *control = control_get(player->action_object_index);

		if (control->control.custom_name_index != NONE)
		{
			hud_set_state_message(
				render.local_player_index,
				_hud_message_custom_device);
			hud_set_state_message_text(
				render.local_player_index,
				0,
				control->control.custom_name_index,
				TRUE);
		}
		else
		{
			hud_set_state_message(
				render.local_player_index,
				_hud_message_touch_device);
			hud_set_state_message_text(
				render.local_player_index,
				0,
				item_name_index,
				FALSE);
		}

		return;
	}

	case _player_action_result_swap_for_weapon:
	{
		struct weapon_datum *weapon = weapon_try_and_get(player->action_object_index);

		if (weapon != NULL)
		{
			struct icon_hud_element_definition const *icon = NULL;
			struct weapon_definition *weapon_definition = weapon_definition_get(
				weapon->definition_index);
			long hud_interface_index =
				weapon_definition->weapon.interface_definition.hud_interface.index;

			if (hud_interface_index != NONE)
			{
				icon = &weapon_hud_interface_definition_get(
					hud_interface_index)->messaging_icon;
				if (icon->sequence_index == NONE)
					icon = NULL;
			}

			hud_set_state_message(
				render.local_player_index,
				_hud_message_swap_weapon);
			if (icon != NULL)
			{
				hud_set_state_message_icon(
					render.local_player_index,
					0,
					icon);
			}
			else
			{
				hud_set_state_message_text(
					render.local_player_index,
					0,
					item_name_index,
					FALSE);
			}
		}

		return;
	}

	case _player_action_result_add_weapon_to_inventory:
	{
		struct weapon_datum *weapon = weapon_try_and_get(player->action_object_index);

		if (weapon != NULL)
		{
			struct icon_hud_element_definition const *icon = NULL;
			struct weapon_definition *weapon_definition = weapon_definition_get(
				weapon->definition_index);
			long hud_interface_index =
				weapon_definition->weapon.interface_definition.hud_interface.index;

			if (hud_interface_index != NONE)
			{
				icon = &weapon_hud_interface_definition_get(
					hud_interface_index)->messaging_icon;
				if (icon->sequence_index == NONE)
					icon = NULL;
			}

			hud_set_state_message(
				render.local_player_index,
				_hud_message_pickup);
			if (icon != NULL)
			{
				hud_set_state_message_icon(
					render.local_player_index,
					0,
					icon);
			}
			else
			{
				hud_set_state_message_text(
					render.local_player_index,
					0,
					item_name_index,
					FALSE);
			}
		}

		return;
	}

	case _player_action_result_enter_vehicle:
	case _player_action_result_evict_from_vehicle:
	{
		hud_set_state_message(
			render.local_player_index,
			_hud_message_enter_vehicle);
		hud_set_state_message_text(
			render.local_player_index,
			0,
			TAG_BLOCK_GET_ELEMENT(
				&unit_definition_get(
					unit_get(player->action_object_index)->definition_index)->unit.seats,
				player->action_seat_index,
				struct unit_seat)->icon_text_index,
			FALSE);
		hud_set_state_message_text(
			render.local_player_index,
			1,
			item_name_index,
			FALSE);
		return;
	}

	case _player_action_result_flip_vehicle:
		hud_set_state_message(
			render.local_player_index,
			_hud_message_flip_vehicle);
		hud_set_state_message_text(
			render.local_player_index,
			0,
			(short)get_object_icon_text_index(player->action_object_index),
			FALSE);
		return;

	default:
	{
		wchar_t state_message[1024];

		if (game_engine_get_state_message(
			player_index,
			state_message,
			1024))
		{
			hud_enable_custom_state_message(render.local_player_index, TRUE);
			hud_set_state_text(render.local_player_index, state_message);
			return;
		}

		if (player->unit_index == NONE)
		{
			hud_enable_custom_state_message(render.local_player_index, FALSE);
			return;
		}

		{
			long weapon_index = unit_inventory_get_weapon(
				player->unit_index,
				unit_get(player->unit_index)->unit.current_weapon_index);
			struct unit_datum *unit = unit_get(player->unit_index);
			boolean allow_swap_prompt = TRUE;

			if (unit->object.parent_object_index != NONE &&
				unit->unit.parent_seat_index != NONE)
			{
				struct unit_datum *parent_unit = unit_get(
					unit->object.parent_object_index);
				struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
					&unit_definition_get(
						parent_unit->definition_index)->unit.seats,
					unit->unit.parent_seat_index,
					struct unit_seat);

				allow_swap_prompt =
					!TEST_FLAG(seat->flags, _unit_seat_driver_bit) &&
					!TEST_FLAG(seat->flags, _unit_seat_gunner_bit);
			}

			if (weapon_index != NONE && allow_swap_prompt)
			{
				struct weapon_interface_state weapon_state;

				weapon_build_weapon_interface_state(weapon_index, &weapon_state);
				if (weapon_state_is_depleted(&weapon_state))
				{
					short weapon_slot = unit->unit.current_weapon_index;
					short weapon_count = unit_get_weapon_count(player->unit_index);
					long candidate_weapon_index;

					do
					{
						weapon_slot = unit_inventory_next_weapon(
							player->unit_index,
							weapon_slot,
							1);
						candidate_weapon_index = unit_inventory_get_weapon(
							player->unit_index,
							weapon_slot);
						weapon_build_weapon_interface_state(
							candidate_weapon_index,
							&weapon_state);
						if (!weapon_state_is_depleted(&weapon_state) ||
							candidate_weapon_index == weapon_index)
						{
							break;
						}
					}
					while (!--weapon_count);

					if (!weapon_state_is_depleted(&weapon_state) &&
						candidate_weapon_index != weapon_index)
					{
						struct weapon_datum *candidate_weapon;

						hud_set_state_message(
							render.local_player_index,
							_hud_message_remind_to_switch_weapons);
						candidate_weapon = weapon_try_and_get(
							candidate_weapon_index);
						if (candidate_weapon != NULL)
						{
							long hud_interface_index = weapon_definition_get(
								candidate_weapon->definition_index)->weapon.interface_definition.hud_interface.index;
							struct icon_hud_element_definition const *icon = NULL;

							if (hud_interface_index != NONE)
							{
								icon = &weapon_hud_interface_definition_get(
									hud_interface_index)->messaging_icon;
								if (icon->sequence_index == NONE)
									icon = NULL;
							}

							if (icon != NULL)
							{
								hud_set_state_message_icon(
									render.local_player_index,
									0,
									icon);
							}
							else
							{
								hud_set_state_message_text(
									render.local_player_index,
									0,
									item_name_index,
									FALSE);
							}

							return;
						}
					}
				}
			}
		}

		hud_enable_custom_state_message(render.local_player_index, FALSE);
		return;
	}
	}

	return;
}

wchar_t const *hud_get_item_string(
	long string_index)
{
	long string_list_index = hud_globals->messaging.hud_item_messages.index;
	wchar_t const *string = default_string;

	if (string_list_index != NONE)
	{
		struct string_list const *string_list =
			unicode_string_list_definition_get(string_list_index);
		if (string_list && VALID_INDEX(string_index, string_list->strings.count))
		{
			string = unicode_string_list_get_string(
				string_list_index,
				(short)string_index);
		}
	}

	return string;
}

void hud_picked_up_grenade(
	short local_player_index,
	long grenade_definition_index)
{
	if (local_player_index != NONE)
	{
		hud_add_item_message(
			local_player_index,
			grenade_definition_index,
			1,
			-1);
	}

	return;
}

void hud_picked_up_ammunition(
	short local_player_index,
	long weapon_definition_index,
	short ammunition_count)
{
	if (local_player_index != NONE)
	{
		hud_add_item_message(
			local_player_index,
			weapon_definition_index,
			ammunition_count,
			1);
	}

	return;
}

void hud_picked_up_weapon(
	short local_player_index,
	long weapon_definition_index)
{
	if (local_player_index != NONE)
	{
		hud_add_item_message(
			local_player_index,
			weapon_definition_index,
			0,
			0);
	}

	return;
}

void hud_picked_up_powerup(
	short local_player_index,
	long powerup_definition_index)
{
	if (local_player_index != NONE)
	{
		hud_add_item_message(
			local_player_index,
			powerup_definition_index,
			0,
			0);
	}

	return;
}

void temporary_hud_draw_reticle(
	real angle,
	real_argb_color const *color)
{
	real_point3d points[NUMBER_OF_TEMPORARY_HUD_RETICLE_POINTS];
	real radius = tangent(angle * 0.5f) * 0.0625f;
	long point_index;
	long next_point_index;
	long line_count;
	real_point3d *point;

	angle = 0.0f;
	for (point_index = 0; point_index < NUMBER_OF_TEMPORARY_HUD_RETICLE_POINTS; point_index++)
	{
		set_real_point3d(
			&points[point_index],
			cosine(angle) * radius,
			sine(angle) * radius,
			-0.0625f);
		matrix4x3_transform_point(
			&render.frustum.view_to_world,
			&points[point_index],
			&points[point_index]);
		angle += _pi / 8.0f;
	}

	next_point_index = 1;
	line_count = NUMBER_OF_TEMPORARY_HUD_RETICLE_POINTS;
	point = points;
	do
	{
		rasterizer_debug_line(
			point,
			&points[next_point_index % NUMBER_OF_TEMPORARY_HUD_RETICLE_POINTS],
			color);
		next_point_index++;
		point++;
	}
	while (--line_count);

	return;
}

void hud_load(
	boolean load)
{
	short string_index = load
		? hud_globals->loading_begin_index
		: hud_globals->loading_end_index;

	scripted_hud_messages_clear();
	if (string_index != NONE)
	{
		hud_print_message(
			local_player_get_next(NONE),
			hud_get_item_string(string_index));
	}

	return;
}

void hud_autosave(
	boolean active)
{
	short string_index = active
		? hud_globals->checkpoint_begin_index
		: hud_globals->checkpoint_end_index;
	short local_player_index;

	scripted_hud_messages_clear();
	if (active && hud_globals->checkpoint_sound.index != NONE)
	{
		unspatialized_impulse_sound_new(
			hud_globals->checkpoint_sound.index,
			1.0f);
	}

	if (string_index != NONE)
	{
		for (local_player_index = 0;
			local_player_index < MAXIMUM_LOCAL_PLAYERS;
			local_player_index++)
		{
			if (local_player_get_player_index(local_player_index) != NONE)
			{
				hud_print_message(
					local_player_index,
					hud_get_item_string(string_index));
			}
		}
	}

	return;
}

static void hud_draw_friendly_indicator(
	long player_index)
{
	struct player_datum const *player = player_get(player_index);
	real_point3d head_position;
	real_point3d view_position;
	real_point2d screen_position;
	point2d position;
	real_rectangle2d clip;
	struct bitmap_data *bitmap;
	real depth_factor;
	real fade;

	unit_get_head_position(
		player->unit_index,
		&head_position);
	head_position.z += 0.30000001f;
	matrix4x3_transform_point(
		&render.frustum.world_to_view,
		&head_position,
		&view_position);
	if (render_camera_view_to_screen(
		&render.camera,
		&render.frustum,
		&view_position,
		&screen_position))
	{
		bitmap = bitmap_group_get_bitmap_from_sequence(
			interface_get_tag_index(_interface_bitmap_multiplayer_hud),
			0,
			0);
		if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
		{
			position.x =
				(short)fast_ftol(screen_position.x) - render.camera.viewport_bounds.x0;
			position.y =
				(short)fast_ftol(screen_position.y) - render.camera.viewport_bounds.y0;
			depth_factor = 1.0f -
				((-view_position.z - render.frustum.z_near) * 100.0f) /
				(render.frustum.z_far - render.frustum.z_near);
			fade = PIN(depth_factor, 0.075000003f, 1.0f);
			clip.x0 = 0.0f;
			clip.x1 = 1.0f;
			clip.y0 = (1.0f - fade) * 0.5f;
			clip.y1 = 1.0f;
			interface_draw_bitmap(
				bitmap,
				&position,
				&clip,
				1.0f,
				0.0f,
				1.0f);
		}
	}

	return;
}

void hud_draw_players(
	void)
{
	long player_index = local_player_get_player_index(render.local_player_index);
	long team_index = player_get(player_index)->team_index;

	if (player_index != NONE)
	{
#ifdef HALO_LINUX
		/* the native builds' sessions hold more than 16 players */
		long teammate_indices[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
#else
		long teammate_indices[16];
#endif
		long teammate_count = 0;
		struct data_iterator iterator;
		struct player_datum *player;
		long teammate_index;

		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
		{
			if (player_index != iterator.datum_index &&
				team_index == player->team_index &&
				player->unit_index != NONE)
			{
#ifdef HALO_LINUX
				/* never write past the buffer, whatever the player data holds */
				if (teammate_count < (long)NUMBEROF(teammate_indices))
					teammate_indices[teammate_count++] = iterator.datum_index;
#else
				teammate_indices[teammate_count++] = iterator.datum_index;
#endif
			}
		}

		for (teammate_index = 0;
			teammate_index < teammate_count;
			teammate_index++)
		{
			hud_draw_friendly_indicator(teammate_indices[teammate_index]);
		}
	}

	return;
}

void temporary_hud_draw(
	void)
{
	long player_index = local_player_get_player_index(render.local_player_index);
	struct player_datum *player;
	struct unit_datum *unit;
	long weapon_index;
	long equipment_index;

	if (player_index == NONE)
		return;

	player = player_get(player_index);
	if (player->unit_index == NONE)
		return;

	unit = unit_get(player->unit_index);
	(void)unit_definition_get(unit->definition_index);
	weapon_index = unit_inventory_get_weapon(
		player->unit_index,
		unit_get(player->unit_index)->unit.current_weapon_index);
	equipment_index = unit_get_current_equipment(player->unit_index);
	if (equipment_index != NONE)
	{
		struct object_datum *equipment = object_get(equipment_index);
		sprintf(
			temporary,
			"%s (press WHITE to use)|n",
			strrchr(tag_get_name(equipment->definition_index), '\\') + 1);
	}
	else
	{
		sprintf(temporary, "|n");
	}

	if (unit->unit.current_grenade_index != NONE)
	{
		struct game_globals_grenade *grenade = TAG_BLOCK_GET_ELEMENT(
			&scenario_get_game_globals()->grenades,
			unit->unit.current_grenade_index,
			struct game_globals_grenade);
		sprintf(
			temporary + csstrlen(temporary),
			"%d %s|n",
			unit->unit.grenade_counts[unit->unit.current_grenade_index],
			tag_name_strip_path(tag_get_name(grenade->projectile.index)));
	}

	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);
		struct weapon_definition *weapon_definition = weapon_definition_get(
			weapon->definition_index);

		if (weapon_definition->weapon.magazines.count > 0)
		{
			struct weapon_magazine_definition *magazine_definition =
				TAG_BLOCK_GET_ELEMENT(
					&weapon_definition->weapon.magazines,
					0,
					struct weapon_magazine_definition);
			sprintf(
				temporary + csstrlen(temporary),
				"%s|ntotal %d/%d|nloaded %d/%d|nheat %3.2f|nage %3.2f|n",
				strrchr(tag_get_name(weapon->definition_index), '\\') + 1,
				weapon->weapon.magazines[0].rounds_total,
				magazine_definition->rounds_total_maximum,
				weapon->weapon.magazines[0].rounds_loaded,
				magazine_definition->rounds_loaded_maximum,
				weapon->weapon.heat,
				weapon->weapon.age);
		}
		else
		{
			sprintf(
				temporary + csstrlen(temporary),
				"%s|n",
				strrchr(tag_get_name(weapon->definition_index), '\\') + 1);
		}

		sprintf(
			temporary + csstrlen(temporary),
			"%s%s",
			(real)player->powerup_durations[_player_powerup_active_camouflage] > 0.0f
				? "ACTIVE-CAMOUFLAGE "
				: "",
			(real)player->powerup_durations[_player_powerup_full_spectrum_vision] > 0.0f
				? "FULL-SPECTRUM VISION "
				: "");

		temporary_hud_draw_reticle(
			weapon_definition->weapon.aim_assist_parameters.autoaim_angle /
				weapon_get_zoom_magnification(
					weapon_index,
					player_control_get_zoom_level(render.local_player_index)),
			player_control_get_autoaim_level(render.local_player_index) < 1.0f
				? global_real_argb_blue
				: global_real_argb_red);

		{
			struct weapon_trigger_definition *trigger_definition =
				TAG_BLOCK_GET_ELEMENT(
					&weapon_definition->weapon.triggers,
					0,
					struct weapon_trigger_definition);
			real error = TEST_FLAG(
				trigger_definition->flags,
				_weapon_trigger_analog_rate_of_fire_bit)
				? weapon->weapon.primary_trigger
				: weapon->weapon.triggers[0].error;

			temporary_hud_draw_reticle(
				(1.0f - error) *
					trigger_definition->projectile_error_angle_lower_bound +
					error * trigger_definition->projectile_error_angle_upper_bound,
				global_real_argb_yellow);
		}
	}

	{
		rectangle2d bounds = render.camera.window_bounds;
		bounds.x0 += 100;
		draw_string_set_format(NONE, 0, 0);
		draw_string_set_color(global_real_argb_white);
		rasterizer_draw_string(&bounds, NULL, NULL, 0, temporary);
	}

	return;
}

void hud_draw_screen(
	void)
{
	long player_index = local_player_get_player_index(render.local_player_index);
	director_perspective perspective = director_get_perspective(render.local_player_index);

	rasterizer_hud_begin();
	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);

		if ((!game_engine_running() || game_engine_display_team_indicators()) &&
			!cinematic_in_progress())
		{
			hud_draw_players();
		}

		if (!game_time_get_paused() &&
			render.local_player_index == local_player_get_next(NONE))
		{
			motion_sensor_tick();
		}

		if (hud_scripted_globals->show_hud)
		{
			if (perspective != _director_perspective_neutral &&
				perspective != _director_perspective_scripted &&
				player->unit_index != NONE)
			{
				hud_render_weapon_interface(player);
				hud_show_action_response(player_index);
				hud_play_unit_sounds(player, hud_scripted_globals->show_hud);
				hud_render_unit_interface(player);
				hud_render_nav_points(render.local_player_index);
				hud_render_damage_indicators(render.local_player_index);
			}
			else
			{
				hud_show_action_response(player_index);
				hud_play_unit_sounds(player, hud_scripted_globals->show_hud);
			}
		}
		else
		{
			hud_play_unit_sounds(player, FALSE);
		}

		hud_messaging_update(render.local_player_index);
	}

	rasterizer_hud_end();
	if (temporary_hud)
	{
		temporary_hud_draw();
	}

	return;
}

/* ---------- private code */
