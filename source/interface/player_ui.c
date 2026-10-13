/*
PLAYER_UI.C

symbols in this file:
000CFC30 0020:
	_player_ui_dispose (0000)
000CFC50 0020:
	_player_ui_reset_single_player_local_player_controllers (0000)
000CFC70 0080:
	_player_ui_set_single_player_local_player_controller (0000)
000CFCF0 0050:
	_player_ui_get_single_player_local_player_controller (0000)
000CFD40 0030:
	_player_ui_get_single_player_local_player_from_controller (0000)
000CFD70 0050:
	_player_ui_local_player_joined_multiplayer_game (0000)
000CFDC0 0050:
	_player_ui_local_player_wants_to_play_multiplayer (0000)
000CFE10 0050:
	_player_ui_clear_multiplayer_autojoin_for_local_player (0000)
000CFE60 0030:
	_player_ui_autojoin_players_to_next_multiplayer_game (0000)
000CFE90 0020:
	_player_ui_clear_multiplayer_variant (0000)
000CFEB0 0060:
	_player_ui_get_active_player_profile (0000)
000CFF10 0030:
	_player_ui_get_active_player_profile_index (0000)
000CFF40 0050:
	_player_ui_get_last_single_player_level_played (0000)
000CFF90 0050:
	_player_ui_set_game_variant (0000)
000CFFE0 0050:
	_player_ui_game_variant_specified (0000)
000D0030 0050:
	_player_ui_rumble_disabled (0000)
000D0080 00a0:
	_player_ui_autolevel_enabled (0000)
000D0120 0040:
	_player_ui_get_path_to_local_player_profile_directory (0000)
000D0160 0060:
	_player_ui_remember_player1_profile (0000)
000D01C0 0040:
	_player_ui_get_player1_last_used_profile_index (0000)
000D0200 00b0:
	_player_ui_fast_setup_network_server (0000)
000D02B0 0050:
	_player_ui_edit_profile_is_default_profile (0000)
000D0300 0070:
	_player_ui_edit_profile_name_is_dirty (0000)
000D0370 0060:
	_player_ui_prompt_user_to_rename_edit_profile (0000)
000D03D0 0020:
	_player_ui_get_edit_player_profile (0000)
000D03F0 0020:
	_player_ui_get_edit_playlist_profile (0000)
000D0410 00f0:
	_player_ui_edit_profile_is_dirty (0000)
000D0500 0030:
	_player_ui_activate_all_solo_levels (0000)
000D0530 0050:
	_hud_message_to_all (0000)
000D0580 0010:
	_player0_look_pitch_is_inverted (0000)
000D0590 0020:
	_player0_joystick_set_is_normal (0000)
000D05B0 0040:
	_generate_default_player_profile (0000)
000D05F0 0210:
	_set_local_player_controls_from_player_profile (0000)
000D0800 0010:
	_clear_profile_edit_data (0000)
000D0810 0070:
	_reset_local_player_profile (0000)
000D0880 00a0:
	_player_ui_initialize (0000)
000D0920 00a0:
	_player_ui_clear_multiplayer_joins (0000)
000D09C0 0070:
	_player_ui_set_active_player_profile (0000)
000D0A30 00b0:
	_player_ui_begin_editing_profile (0000)
000D0AE0 01b0:
	_player_ui_save_profile (0000)
000D0C90 0010:
	_player_ui_end_editing_profile (0000)
000D0CA0 0070:
	_player0_look_invert_pitch (0000)
00270BDC 0019:
	??_C@_0BJ@DFIFBFHJ@invalid?5controller?5index?$AA@ (0000)
00270BF8 001b:
	??_C@_0BL@FPFMMDCA@invalid?5local?5player?5index?$AA@ (0000)
00270C14 0025:
	??_C@_0CF@DLLNKDKO@c?3?2halo?2SOURCE?2interface?2player_@ (0000)
00270C40 0050:
	??_C@_0FA@COKNKHJP@?$CIlocal_player_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIloca@ (0000)
00270C90 0065:
	??_C@_0GF@IIJJCBPM@?$CIlocal_player_index?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIloca@ (0000)
00270CF8 0008:
	??_C@_07IFGLIMJI@variant?$AA@ (0000)
00270D00 002f:
	??_C@_0CP@JOJHLPFO@player?51?5has?5no?5active?5player?5pr@ (0000)
00270D30 006a:
	??_C@_0GK@DKOJLEOE@failed?5to?5load?5network?5pregame?5s@ (0000)
00270D9C 002d:
	??_C@_0CN@JHOFCMHM@failed?5to?5initiate?5a?5multiplayer@ (0000)
00270DD0 0056:
	??_C@_0FG@FFFNJFPJ@ui?2shell?2main_menu?2multiplayer_t@ (0000)
00270E28 002a:
	??_C@_0CK@FMHFAECE@unknown?5saved?5game?5file?5type?5bei@ (0000)
00270E54 0028:
	??_C@_0CI@CHDBALJG@not?5currently?5editing?5a?5saved?5ga@ (0000)
00270E7C 0022:
	??_C@_0CC@KDPBKPKM@unknown?5profile?5type?5being?5edite@ (0000)
00270EA0 0008:
	??_C@_07LNMDFKEJ@profile?$AA@ (0000)
00270EA8 0035:
	??_C@_0DF@EJKBCKFH@failed?5to?5retrieve?5player?5profil@ (0000)
00270EE0 0037:
	??_C@_0DH@HABNMGLM@failed?5to?5retrieve?5playlist?5prof@ (0000)
00270F18 001f:
	??_C@_0BP@IGBDNJOH@invalid?5profile?5index?5?$CI?$CD?$CF08lX?$CJ?$AA@ (0000)
00270F38 0032:
	??_C@_0DC@OODOEIMJ@?$CD?$CD?$CD?5WARNING?3?5saving?5over?5a?5defau@ (0000)
00270F70 0049:
	??_C@_0EJ@JACFDNDO@cannot?5save?5over?5default?5profile@ (0000)
00270FBC 0027:
	??_C@_0CH@HGHAKCCE@failed?5to?5save?5renamed?5profile?5t@ (0000)
00270FE8 0046:
	??_C@_0EG@JJOABCL@?$CD?$CD?$CD?5WARNING?3?5saving?5player?5profi@ (0000)
00271030 0036:
	??_C@_0DG@JHHBBKEF@failed?5to?5save?5profile?5because?5w@ (0000)
00271068 001e:
	??_C@_0BO@KMODNFHM@ui?2shell?2strings?2temp_strings?$AA@ (0000)
002FD5A4 0004:
	_player1_last_used_profile_index (0000)
00453D00 0230:
	_player_ui_globals (0000)
00453F30 0100:
	_player1_profile_path (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "text/unicode.h"
#include "text/text_group.h"
#include "input/input.h"
#include "input/input_abstraction.h"
#include "interface/hud_messaging.h"
#include "memory/data.h"
#include "networking/network_game_globals.h"
#include "saved games/player_profile.h"
#include "saved games/playlist_profile.h"
#include "saved games/saved_game_files.h"
#include "interface/ui_widget.h"
#include "interface/virtual_keyboard.h"
#include "main/main.h"
#include "tag_files/tag_groups.h"
#include "player_ui.h"

/* ---------- constants */

enum
{
	MAXIMUM_NUMBER_OF_LOCAL_PLAYERS = 4,
	_variant_is_system_default_bit = 0
};

/* ---------- macros */

/* ---------- structures */

struct player_ui_local_player
{
	struct player_profile profile;
	long active_profile_index;
	boolean prejoined_multiplayer;
};

union player_ui_edit_profile_data
{
	struct player_profile player;
	struct game_variant variant;
};

struct player_ui_edit_profile
{
	union player_ui_edit_profile_data current;
	union player_ui_edit_profile_data original;
};

struct player_ui_globals
{
	struct player_ui_local_player local_players[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	boolean multiplayer_autojoin[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	short single_player_controller[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	struct game_variant multiplayer_variant;
	boolean multiplayer_variant_specified;
	long edit_profile_index;
	struct player_ui_edit_profile edit_profile;
	boolean initialized;
};

typedef char player_profile_size_assert[
	sizeof(struct player_profile) == 0x30 ? 1 : -1];
typedef char player_ui_edit_profile_size_assert[
	sizeof(struct player_ui_edit_profile) == 0xD0 ? 1 : -1];
typedef char player_ui_globals_size_assert[
	sizeof(struct player_ui_globals) == 0x230 ? 1 : -1];

/* ---------- prototypes */

static void generate_default_player_profile(
	struct player_profile *profile);
static void reset_local_player_profile(
	short local_player_index);
static void clear_profile_edit_data(
	void);

/* ---------- globals */

static long player1_last_used_profile_index = NONE;
struct player_ui_globals player_ui_globals = { 0 };
static char player1_profile_path[0x100] = { 0 };

/* ---------- public code */

static void generate_default_player_profile(
	struct player_profile *profile)
{
	match_vassert("c:\\halo\\SOURCE\\interface\\player_ui.c", 0x369, profile, "profile");
	csmemset(profile, 0, sizeof(*profile));
	profile->primary_color_index = NONE;
	profile->controller_settings.button_preset = _button_preset_standard;
	profile->controller_settings.joystick_preset = _joystick_preset_standard;

	return;
}

static void reset_local_player_profile(
	short local_player_index)
{
	generate_default_player_profile(&player_ui_globals.local_players[local_player_index].profile);
	player_ui_globals.local_players[local_player_index].active_profile_index = NONE;
	player_ui_globals.single_player_controller[local_player_index] = NONE;

	return;
}

void player_ui_dispose(
	void)
{
	csmemset(&player_ui_globals, 0, sizeof(player_ui_globals));
	return;
}

void player_ui_initialize(
	void)
{
	long local_player_index;

	csmemset(&player_ui_globals, 0, sizeof(player_ui_globals));
	for (local_player_index = 0; local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; local_player_index++)
	{
		reset_local_player_profile((short)local_player_index);
	}
	player_ui_globals.edit_profile_index = NONE;
	player_ui_globals.initialized = TRUE;
	return;
}

void player_ui_clear_multiplayer_joins(
	void)
{
	long local_player_index;

	for (local_player_index = 0; local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; local_player_index++)
	{
		reset_local_player_profile((short)local_player_index);
		player_ui_globals.local_players[local_player_index].prejoined_multiplayer = FALSE;
		player_ui_globals.multiplayer_autojoin[local_player_index] = FALSE;
	}
	return;
}

void player_ui_reset_single_player_local_player_controllers(
	void)
{
	csmemset(
		player_ui_globals.single_player_controller,
		NONE,
		sizeof(player_ui_globals.single_player_controller));
	return;
}

short player_ui_get_single_player_local_player_from_controller(
	short controller_index)
{
	short local_player_index;
	short result;

	result = NONE;
	for (local_player_index = 0; local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; local_player_index++)
	{
		if (player_ui_globals.single_player_controller[local_player_index] == controller_index)
		{
			result = local_player_index;
			break;
		}
	}
	return result;
}

void player_ui_autojoin_players_to_next_multiplayer_game(
	void)
{
	player_ui_globals.local_players[0].prejoined_multiplayer = player_ui_globals.multiplayer_autojoin[0];
	player_ui_globals.local_players[1].prejoined_multiplayer = player_ui_globals.multiplayer_autojoin[1];
	player_ui_globals.local_players[2].prejoined_multiplayer = player_ui_globals.multiplayer_autojoin[2];
	player_ui_globals.local_players[3].prejoined_multiplayer = player_ui_globals.multiplayer_autojoin[3];
	return;
}

void player_ui_clear_multiplayer_variant(
	void)
{
	player_ui_globals.multiplayer_variant_specified = FALSE;
	game_connection_set(_game_connection_local);
	game_engine_dispose();
	game_set_game_variant(NULL);
	return;
}

long player_ui_get_active_player_profile_index(
	short local_player_index)
{
	long result;

	if (local_player_index >= 0 && local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)
		result = player_ui_globals.local_players[local_player_index].active_profile_index;
	else
		result = NONE;
	return result;
}

struct player_profile *player_ui_get_edit_player_profile(
	void)
{
	struct player_profile *result;

	if (saved_game_file_get_type(player_ui_globals.edit_profile_index) == _saved_game_file_type_player_profile)
		result = &player_ui_globals.edit_profile.current.player;
	else
		result = NULL;
	return result;
}

struct game_variant *player_ui_get_edit_playlist_profile(
	void)
{
	struct game_variant *result;

	if (saved_game_file_get_type(player_ui_globals.edit_profile_index) == _saved_game_file_type_game_variant)
		result = &player_ui_globals.edit_profile.current.variant;
	else
		result = NULL;
	return result;
}

boolean player_ui_edit_profile_is_dirty(
	void)
{
	boolean result;
	word original_flags, current_flags;

	result = FALSE;

	if (player_ui_globals.edit_profile_index != NONE)
	{
		switch (saved_game_file_get_type(player_ui_globals.edit_profile_index))
		{
			case _saved_game_file_type_player_profile:
			{
				original_flags = player_ui_globals.edit_profile.original.player.flags;
				current_flags = player_ui_globals.edit_profile.current.player.flags;

				player_ui_globals.edit_profile.original.player.flags = 0;
				player_ui_globals.edit_profile.current.player.flags = 0;
				if (csmemcmp(
					&player_ui_globals.edit_profile.original.player,
					&player_ui_globals.edit_profile.current.player,
					sizeof(struct player_profile)))
				{
					result = TRUE;
				}
				player_ui_globals.edit_profile.current.player.flags = current_flags;
				player_ui_globals.edit_profile.original.player.flags = original_flags;
				break;
			}

			case _saved_game_file_type_game_variant:
			{
				original_flags = player_ui_globals.edit_profile.original.variant.flags;
				current_flags = player_ui_globals.edit_profile.current.variant.flags;

				player_ui_globals.edit_profile.original.variant.flags = 0;
				player_ui_globals.edit_profile.current.variant.flags = 0;
				if (csmemcmp(
					&player_ui_globals.edit_profile.original.variant,
					&player_ui_globals.edit_profile.current.variant,
					sizeof(struct game_variant)))
				{
					result = TRUE;
				}
				player_ui_globals.edit_profile.original.variant.flags = original_flags;
				player_ui_globals.edit_profile.current.variant.flags = current_flags;
				break;
			}

			default:
				error(_error_silent, "unknown profile type being edited");
				break;
		}
	}

	return result;
}

boolean player0_look_pitch_is_inverted(
	void)
{
	return player_ui_globals.local_players[0].profile.controller_settings.invert_look;
}

boolean player0_joystick_set_is_normal(
	void)
{
	return player_ui_globals.local_players[0].profile.controller_settings.joystick_preset == _joystick_preset_standard ||
		player_ui_globals.local_players[0].profile.controller_settings.joystick_preset == _joystick_preset_south_paw;
}

void player_ui_end_editing_profile(
	void)
{
	clear_profile_edit_data();

	return;
}

boolean player_ui_local_player_wants_to_play_multiplayer(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 167, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	return player_ui_globals.local_players[local_player_index].prejoined_multiplayer;
}

void player_ui_clear_multiplayer_autojoin_for_local_player(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 175, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	player_ui_globals.local_players[local_player_index].prejoined_multiplayer = FALSE;
	return;
}

short player_ui_get_last_single_player_level_played(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 265, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	return player_ui_globals.local_players[local_player_index].profile.last_single_player_map_played;
}

short player_ui_get_single_player_local_player_controller(
	short local_player_index)
{
	match_vassert("c:\\halo\\SOURCE\\interface\\player_ui.c", 132,
		(local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS),
		"invalid local player index");

	return player_ui_globals.single_player_controller[local_player_index];
}

void player_ui_local_player_joined_multiplayer_game(
	short local_player_index)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 157, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	player_ui_globals.local_players[local_player_index].prejoined_multiplayer = TRUE;
	player_ui_globals.multiplayer_autojoin[local_player_index] = TRUE;
	return;
}

boolean player_ui_rumble_disabled(
	short local_player_index)
{
	if (local_player_index == NONE)
		return FALSE;

	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 305, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	return player_ui_globals.local_players[local_player_index].profile.controller_settings.vibration_disabled;
}

boolean player_ui_get_path_to_local_player_profile_directory(
	short local_player_index,
	char *path)
{
	if (local_player_index >= 0 && local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)
		return player_profile_get_enclosing_directory_path(
			player_ui_globals.local_players[local_player_index].active_profile_index,
			path);
	return FALSE;
}

void player_ui_get_active_player_profile(
	short local_player_index,
	void *profile)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 238, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS) && (profile != NULL));

	csmemcpy(profile, &player_ui_globals.local_players[local_player_index].profile, 0x30);
	return;
}

void player_ui_activate_all_solo_levels(
	void)
{
	long level_index;

	level_index = 0;
	do
	{
		player_ui_globals.local_players[0].profile.single_player_map_flags[level_index] |= 0xf;
	}
	while (++level_index < 10);

	if (player_ui_globals.local_players[0].active_profile_index != NONE)
		player_profile_save(
			player_ui_globals.local_players[0].active_profile_index,
			&player_ui_globals.local_players[0].profile);
	return;
}

void player_ui_set_game_variant(
	struct game_variant *variant)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 273, variant);

	csmemcpy(&player_ui_globals.multiplayer_variant, variant, sizeof(*variant));
	player_ui_globals.multiplayer_variant_specified = TRUE;
	return;
}

boolean player_ui_game_variant_specified(
	struct game_variant *variant)
{
	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 284, variant);

	if (player_ui_globals.multiplayer_variant_specified)
		csmemcpy(variant, &player_ui_globals.multiplayer_variant, sizeof(*variant));
	return player_ui_globals.multiplayer_variant_specified;
}

void player_ui_set_single_player_local_player_controller(
	short local_player_index,
	short controller_index)
{
	match_vassert("c:\\halo\\SOURCE\\interface\\player_ui.c", 119,
		(local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS),
		"invalid local player index");
	match_vassert("c:\\halo\\SOURCE\\interface\\player_ui.c", 121,
		(controller_index>=0) && (controller_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS),
		"invalid controller index");

	player_ui_globals.single_player_controller[local_player_index] = controller_index;
	return;
}

long player_ui_get_player1_last_used_profile_index(
	void)
{
	if (!player1_profile_path[0] &&
		saved_game_file_retrieve_player1_last_used_profile_directory(
			player1_profile_path))
	{
		player1_last_used_profile_index = saved_game_file_find_profile_index_for_directory_path(
			player1_profile_path, _saved_game_file_type_player_profile);
	}
	return player1_last_used_profile_index;
}

void player_ui_fast_setup_network_server(
	void)
{
	ui_widgets_close_all();
	dispose_global_network_game_server();
	dispose_global_network_game_client();
	game_connection_set(_game_connection_local);
	main_set_multiplayer_map_name("");
	player_ui_globals.multiplayer_variant_specified = FALSE;
	if (ui_widget_load_by_name_or_tag(
		"ui\\shell\\main_menu\\multiplayer_type_select\\connected\\pregame\\connected_pregame_screen",
		NONE,
		NULL,
		NONE,
		NONE,
		NONE,
		NONE))
	{
		game_engine_playlist_initialize();
		network_game_accept_remote_connections(TRUE);
		if (create_global_network_game_server() && create_global_network_game_client())
		{
			game_engine_playlist_begin();
			game_connection_set(_game_connection_network_server);
			return;
		}

		dispose_global_network_game_server();
		dispose_global_network_game_client();
		network_game_accept_remote_connections(FALSE);
		error(_error_silent, "failed to initiate a multiplayer game server");
		main_goto_main_menu();
		return;
	}

	error(
		_error_silent,
		"failed to load network pregame screen... maybe you ran this from some place other than the game shell UI?");
	main_goto_main_menu();
	return;
}

boolean player_ui_edit_profile_is_default_profile(
	void)
{
	boolean result;

	result = FALSE;
	if (player_ui_globals.edit_profile_index != NONE)
	{
		long type = saved_game_file_get_type(player_ui_globals.edit_profile_index);

		if (type>=0 && type<=_saved_game_file_type_game_variant)
			result = TEST_FLAG(player_ui_globals.edit_profile_index, _saved_game_file_index_read_only_bit);
		else
			error(_error_silent, "unknown saved game file type being edited");
	}

	return result;
}

void player_ui_remember_player1_profile(
	boolean save)
{
	if (player1_last_used_profile_index != player_ui_globals.local_players[0].active_profile_index)
	{
		if (player_ui_globals.local_players[0].active_profile_index==NONE ||
			!player_profile_get_enclosing_directory_path(
				player_ui_globals.local_players[0].active_profile_index,
				player1_profile_path))
		{
			error(_error_silent, "player 1 has no active player profile assigned");
		}

		player1_last_used_profile_index = player_ui_globals.local_players[0].active_profile_index;
	}

	if (save && player1_profile_path[0])
		saved_game_file_remember_player1_last_used_profile_directory(
			player1_profile_path);

	return;
}

void player_ui_begin_editing_profile(
	long profile_index)
{
	long type;

	player_ui_globals.edit_profile_index = NONE;
	type = saved_game_file_get_type(profile_index);

	switch (type)
	{
		case _saved_game_file_type_player_profile:
			if (player_profile_get(
				profile_index,
				&player_ui_globals.edit_profile.original.player))
			{
				csmemcpy(
					&player_ui_globals.edit_profile.current.player,
					&player_ui_globals.edit_profile.original.player,
					sizeof(struct player_profile));
			}
			else
			{
				error(_error_silent, "failed to retrieve player profile #%08lX for editing", profile_index);
				return;
			}
			break;

		case _saved_game_file_type_game_variant:
			if (playlist_profile_get(
				profile_index,
				&player_ui_globals.edit_profile.original.variant))
			{
				csmemcpy(
					&player_ui_globals.edit_profile.current.variant,
					&player_ui_globals.edit_profile.original.variant,
					sizeof(struct game_variant));
			}
			else
			{
				error(_error_silent, "failed to retrieve playlist profile #%08lX for editing", profile_index);
				return;
			}
			break;

		default:
			error(_error_silent, "invalid profile index (#%08lX)", profile_index);
			return;
	}

	player_ui_globals.edit_profile_index = profile_index;
	return;
}

boolean player_ui_save_profile(
	void)
{
	boolean result = FALSE;
	char directory_path[256];

	switch (saved_game_file_get_type(player_ui_globals.edit_profile_index))
	{
		case _saved_game_file_type_player_profile:
			if (TEST_FLAG(
				player_ui_globals.edit_profile_index,
				_saved_game_file_index_read_only_bit))
			{
				error(_error_silent, "### WARNING: saving over a default player profile");
			}

			if (!player_ui_edit_profile_is_dirty())
			{
				error(_error_silent, "### WARNING: saving player profile even though it hasn't been changed");
			}

			player_profile_save(
				player_ui_globals.edit_profile_index,
				&player_ui_globals.edit_profile.current.player);
			result = TRUE;
			break;

		case _saved_game_file_type_game_variant:
			if (!player_ui_edit_profile_is_dirty())
			{
				error(_error_silent, "### WARNING: saving player profile even though it hasn't been changed");
			}

			if (TEST_FLAG(
				player_ui_globals.edit_profile_index,
				_saved_game_file_index_read_only_bit))
			{
				if (ustrncmp(
					player_ui_globals.edit_profile.current.variant.human_readable_game_description,
					player_ui_globals.edit_profile.original.variant.human_readable_game_description,
					NUMBEROF(player_ui_globals.edit_profile.current.variant.human_readable_game_description)))
				{
					long new_profile_index;

					SET_FLAG(
						player_ui_globals.edit_profile.current.variant.flags,
						_variant_is_system_default_bit,
						FALSE);
					new_profile_index = playlist_profile_new(
						0,
						player_ui_globals.edit_profile.current.variant.human_readable_game_description);
					if (new_profile_index != NONE)
					{
						playlist_profile_save(
							new_profile_index,
							&player_ui_globals.edit_profile.current.variant);
						player_ui_globals.edit_profile_index = new_profile_index;
						if (saved_game_file_get_path_to_enclosing_directory(
							new_profile_index,
							directory_path))
						{
							saved_game_file_remember_last_used_multiplayer_variant_directory(
								directory_path);
						}
						result = TRUE;
					}
					else
					{
						error(_error_silent, "failed to save renamed profile to disk");
					}
				}
				else
				{
					error(
						_error_silent,
						"cannot save over default profiles; must rename and save-as a new profile");
				}
			}
			else
			{
				playlist_profile_save(
					player_ui_globals.edit_profile_index,
					&player_ui_globals.edit_profile.current.variant);
				if (saved_game_file_get_path_to_enclosing_directory(
					player_ui_globals.edit_profile_index,
					directory_path))
				{
					saved_game_file_remember_last_used_multiplayer_variant_directory(
						directory_path);
				}
				result = TRUE;
			}
			break;

		default:
			error(_error_silent, "failed to save profile because we are not editing one");
			break;
	}

	clear_profile_edit_data();

	return result;
}

boolean player_ui_autolevel_enabled(
	short controller_index)
{
	short local_player_index;

	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 320, (controller_index>=0) && (controller_index<MAXIMUM_GAMEPADS));

	if (network_game_is_active())
		local_player_index = controller_index;
	else
		local_player_index = player_ui_get_single_player_local_player_from_controller(controller_index);

	if (local_player_index == NONE)
		return FALSE;

	match_assert("c:\\halo\\SOURCE\\interface\\player_ui.c", 340, (local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	return player_ui_globals.local_players[local_player_index].profile.controller_settings.autocenter;
}

boolean player_ui_edit_profile_name_is_dirty(
	void)
{
	boolean result;

	result = FALSE;
	if (player_ui_globals.edit_profile_index != NONE)
	{
		long type = saved_game_file_get_type(player_ui_globals.edit_profile_index);

		switch (type)
		{
			case _saved_game_file_type_player_profile:
				if (ustrncmp(
					player_ui_globals.edit_profile.current.player.player_name,
					player_ui_globals.edit_profile.original.player.player_name,
					12)!=0)
				{
					result = TRUE;
				}
				break;

			case _saved_game_file_type_game_variant:
				if (ustrncmp(
					player_ui_globals.edit_profile.current.variant.human_readable_game_description,
					player_ui_globals.edit_profile.original.variant.human_readable_game_description,
					12)!=0)
				{
					result = TRUE;
				}
				break;

			default:
				error(_error_silent, "unknown saved game file type being edited");
				break;
		}
	}
	else
	{
		error(_error_silent, "not currently editing a saved game file");
	}

	return result;
}

boolean player_ui_prompt_user_to_rename_edit_profile(
	void)
{
	boolean result;

	result = FALSE;
	if (player_ui_globals.edit_profile_index != NONE)
	{
		long type = saved_game_file_get_type(player_ui_globals.edit_profile_index);

		switch (type)
		{
			case _saved_game_file_type_player_profile:
				result = virtual_keyboard_launch(
					player_ui_globals.edit_profile.current.player.player_name,
					sizeof(player_ui_globals.edit_profile.current.player.player_name),
					10);
				break;

			case _saved_game_file_type_game_variant:
				result = virtual_keyboard_launch(
					player_ui_globals.edit_profile.current.variant.human_readable_game_description,
					sizeof(player_ui_globals.edit_profile.current.variant.human_readable_game_description),
					10);
				break;

			default:
				error(_error_silent, "unknown saved game file type being edited");
				break;
		}
	}
	else
	{
		error(_error_silent, "not currently editing a saved game file");
	}

	return result;
}

/* ---------- private code */

static void hud_message_to_all(
	wchar_t const *message)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		short local_player_index = player->local_player_index;

		if (local_player_index != NONE)
			hud_print_message(local_player_index, message);
	}

	return;
}

static void set_local_player_controls_from_player_profile(
	short local_player_index)
{
	struct game_input_preferences preferences = { 0 };
	real pitch_rate_table[NUMBER_OF_LOOK_SENSITIVITY_SETTINGS] =
	{
		40.0f, 50.0f, 60.0f, 70.0f, 80.0f,
		90.0f, 100.0f, 110.0f, 120.0f, 130.0f
	};
	real yaw_rate_table[NUMBER_OF_LOOK_SENSITIVITY_SETTINGS] =
	{
		80.0f, 100.0f, 120.0f, 140.0f, 160.0f,
		180.0f, 200.0f, 220.0f, 240.0f, 260.0f
	};
	struct player_profile_controller_settings *controls;
	long look_sensitivity;
	long yaw_index;
	short pitch_index;
	short controller_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\player_ui.c",
		0x396,
		(local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));

	controls = &player_ui_globals.local_players[local_player_index].profile.controller_settings;
	look_sensitivity = controls->look_sensitivity;
	yaw_index = look_sensitivity - 1;
	if (look_sensitivity - 1 < 0)
	{
		pitch_index = 0;
	}
	else
	{
		if (yaw_index > NUMBER_OF_LOOK_SENSITIVITY_SETTINGS - 1)
			pitch_index = NUMBER_OF_LOOK_SENSITIVITY_SETTINGS - 1;
		else
			pitch_index = (short)yaw_index;
	}

	if (yaw_index < 0)
	{
		yaw_index = 0;
	}
	else
	{
		if (yaw_index > NUMBER_OF_LOOK_SENSITIVITY_SETTINGS - 1)
			yaw_index = NUMBER_OF_LOOK_SENSITIVITY_SETTINGS - 1;
	}

	preferences.pitch_rate = pitch_rate_table[pitch_index];
	preferences.yaw_rate = yaw_rate_table[(short)yaw_index];
	preferences.joystick_controls = controls->joystick_preset > _joystick_preset_legacy_south_paw ?
		_joystick_preset_legacy_south_paw : controls->joystick_preset;

	switch (controls->button_preset)
	{
		case _button_preset_standard:
			preferences.game_control_to_xbox_buttons[0] = _gamepad_analog_button_a;
			preferences.game_control_to_xbox_buttons[1] = _gamepad_analog_button_black;
			preferences.game_control_to_xbox_buttons[2] = _gamepad_analog_button_x;
			preferences.game_control_to_xbox_buttons[3] = _gamepad_analog_button_y;
			preferences.game_control_to_xbox_buttons[4] = _gamepad_analog_button_b;
			preferences.game_control_to_xbox_buttons[5] = _gamepad_analog_button_white;
			preferences.game_control_to_xbox_buttons[6] = _gamepad_analog_button_left_trigger;
			preferences.game_control_to_xbox_buttons[7] = _gamepad_analog_button_right_trigger;
			preferences.game_control_to_xbox_buttons[8] = _gamepad_binary_button_start;
			preferences.game_control_to_xbox_buttons[9] = _gamepad_binary_button_back;
			preferences.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
			preferences.game_control_to_xbox_buttons[11] = _gamepad_binary_button_right_thumb;
			break;

		case _button_preset_swap_triggers:
			preferences.game_control_to_xbox_buttons[0] = _gamepad_analog_button_a;
			preferences.game_control_to_xbox_buttons[1] = _gamepad_analog_button_black;
			preferences.game_control_to_xbox_buttons[2] = _gamepad_analog_button_x;
			preferences.game_control_to_xbox_buttons[3] = _gamepad_analog_button_y;
			preferences.game_control_to_xbox_buttons[4] = _gamepad_analog_button_b;
			preferences.game_control_to_xbox_buttons[5] = _gamepad_analog_button_white;
			preferences.game_control_to_xbox_buttons[6] = _gamepad_analog_button_right_trigger;
			preferences.game_control_to_xbox_buttons[7] = _gamepad_analog_button_left_trigger;
			preferences.game_control_to_xbox_buttons[8] = _gamepad_binary_button_start;
			preferences.game_control_to_xbox_buttons[9] = _gamepad_binary_button_back;
			preferences.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
			preferences.game_control_to_xbox_buttons[11] = _gamepad_binary_button_right_thumb;
			break;

		case _button_preset_swap_a_and_left_trigger:
			preferences.game_control_to_xbox_buttons[0] = _gamepad_analog_button_left_trigger;
			preferences.game_control_to_xbox_buttons[1] = _gamepad_analog_button_black;
			preferences.game_control_to_xbox_buttons[2] = _gamepad_analog_button_x;
			preferences.game_control_to_xbox_buttons[3] = _gamepad_analog_button_y;
			preferences.game_control_to_xbox_buttons[4] = _gamepad_analog_button_b;
			preferences.game_control_to_xbox_buttons[5] = _gamepad_analog_button_white;
			preferences.game_control_to_xbox_buttons[6] = _gamepad_analog_button_a;
			preferences.game_control_to_xbox_buttons[7] = _gamepad_analog_button_right_trigger;
			preferences.game_control_to_xbox_buttons[8] = _gamepad_binary_button_start;
			preferences.game_control_to_xbox_buttons[9] = _gamepad_binary_button_back;
			preferences.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
			preferences.game_control_to_xbox_buttons[11] = _gamepad_binary_button_right_thumb;
			break;

		case _button_preset_swap_b_and_left_trigger:
			preferences.game_control_to_xbox_buttons[0] = _gamepad_analog_button_a;
			preferences.game_control_to_xbox_buttons[1] = _gamepad_analog_button_black;
			preferences.game_control_to_xbox_buttons[2] = _gamepad_analog_button_x;
			preferences.game_control_to_xbox_buttons[3] = _gamepad_analog_button_y;
			preferences.game_control_to_xbox_buttons[4] = _gamepad_analog_button_left_trigger;
			preferences.game_control_to_xbox_buttons[5] = _gamepad_analog_button_white;
			preferences.game_control_to_xbox_buttons[6] = _gamepad_analog_button_b;
			preferences.game_control_to_xbox_buttons[7] = _gamepad_analog_button_right_trigger;
			preferences.game_control_to_xbox_buttons[8] = _gamepad_binary_button_start;
			preferences.game_control_to_xbox_buttons[9] = _gamepad_binary_button_back;
			preferences.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
			preferences.game_control_to_xbox_buttons[11] = _gamepad_binary_button_right_thumb;
			break;

		case _button_preset_swap_b_and_right_thumb:
			preferences.game_control_to_xbox_buttons[0] = _gamepad_analog_button_a;
			preferences.game_control_to_xbox_buttons[1] = _gamepad_analog_button_black;
			preferences.game_control_to_xbox_buttons[2] = _gamepad_analog_button_x;
			preferences.game_control_to_xbox_buttons[3] = _gamepad_analog_button_y;
			preferences.game_control_to_xbox_buttons[4] = _gamepad_binary_button_right_thumb;
			preferences.game_control_to_xbox_buttons[5] = _gamepad_analog_button_white;
			preferences.game_control_to_xbox_buttons[6] = _gamepad_analog_button_left_trigger;
			preferences.game_control_to_xbox_buttons[7] = _gamepad_analog_button_right_trigger;
			preferences.game_control_to_xbox_buttons[8] = _gamepad_binary_button_start;
			preferences.game_control_to_xbox_buttons[9] = _gamepad_binary_button_back;
			preferences.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
			preferences.game_control_to_xbox_buttons[11] = _gamepad_analog_button_b;
			break;
	}

	preferences.invert_look = controls->invert_look;
	preferences.invert_look_aircraft_control = controls->flight_stick_aircraft_controls;
	controller_index = player_ui_globals.single_player_controller[local_player_index];
	if (controller_index != NONE)
	{
		input_abstraction_update_local_player_preferences(
			controller_index,
			&preferences);
	}
	else
	{
		input_abstraction_update_local_player_preferences(
			local_player_index,
			&preferences);
	}

	return;
}

void player_ui_set_active_player_profile(
	short local_player_index,
	long profile_index,
	struct player_profile *profile)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\player_ui.c",
		0xE2,
		(local_player_index>=0) &&
		(local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS) &&
		(profile != NULL));

	player_ui_globals.local_players[local_player_index].active_profile_index = profile_index;
	csmemcpy(
		&player_ui_globals.local_players[local_player_index].profile,
		profile,
		sizeof(*profile));
	set_local_player_controls_from_player_profile(local_player_index);
	return;
}

void player0_look_invert_pitch(
	boolean invert)
{
	long string_list_index;
	wchar_t const *message;

	player_ui_globals.local_players[0].profile.controller_settings.invert_look = invert;
	if (player_ui_globals.local_players[0].active_profile_index != NONE)
	{
		string_list_index = tag_loaded(
			UNICODE_STRING_LIST_TAG,
			"ui\\shell\\strings\\temp_strings");
		if (string_list_index != NONE)
			message = unicode_string_list_get_string(string_list_index, 1);
		else
			message = L"";
		hud_message_to_all(message);
		player_profile_save(
			player_ui_globals.local_players[0].active_profile_index,
			&player_ui_globals.local_players[0].profile);
	}

	set_local_player_controls_from_player_profile(0);
	return;
}

static void clear_profile_edit_data(
	void)
{
	player_ui_globals.edit_profile_index = NONE;
	return;
}
