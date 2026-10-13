/*
GAME_ENGINE_CTF.C

symbols in this file:
0009EB60 0060:
	_ctf_create_flag_object (0000)
0009EBC0 0010:
	_ctf_engine_dispose (0000)
0009EBD0 0060:
	_create_the_flag (0000)
0009EC30 0040:
	_ctf_single_flag_what_is_up_message (0000)
0009EC70 0010:
	_ctf_engine_dispose_from_old_map (0000)
0009EC80 0020:
	_ctf_engine_player_added (0000)
0009ECA0 0010:
	_ctf_engine_game_ending (0000)
0009ECB0 0010:
	_ctf_engine_game_starting (0000)
0009ECC0 0010:
	_ctf_engine_statistics_append (0000)
0009ECD0 0010:
	_ctf_engine_handle_client_message (0000)
0009ECE0 0010:
	_ctf_engine_handle_server_message (0000)
0009ECF0 0010:
	_ctf_engine_pregame_post_rasterize (0000)
0009ED00 0010:
	_ctf_engine_post_rasterize (0000)
0009ED10 00c0:
	_ctf_award_capture (0000)
0009EDD0 0040:
	_ctf_flag_failure_sound (0000)
0009EE10 0070:
	_ctf_find_flag_carrier (0000)
0009EE80 0080:
	_ctf_engine_allow_pick_up (0000)
0009EF00 0010:
	_ctf_engine_player_damaged_player (0000)
0009EF10 0010:
	_ctf_engine_player_killed_player (0000)
0009EF20 03a0:
	_ctf_engine_display_score (0000)
0009F2C0 0010:
	_ctf_engine_prespawn_player_update (0000)
0009F2D0 0010:
	_ctf_state_message_update_warning (0000)
0009F2E0 0040:
	_ctf_sound_update_warning (0000)
0009F320 0020:
	_ctf_set_flag_warning (0000)
0009F340 0040:
	_ctf_weapon_drop (0000)
0009F380 0040:
	_ctf_get_score (0000)
0009F3C0 0010:
	_ctf_test_flag (0000)
0009F3D0 0040:
	_ctf_get_score_string (0000)
0009F410 0060:
	_ctf_get_score_header_string (0000)
0009F470 0030:
	_ctf_get_team_score_string (0000)
0009F4A0 03d0:
	_ctf_engine_initialize_for_new_map (0000)
0009F870 0050:
	_ctf_reset_flag (0000)
0009F8C0 0090:
	_ctf_player_drop_flag (0000)
0009F950 0050:
	_ctf_position_near_flag (0000)
0009F9A0 0150:
	_ctf_engine_player_update (0000)
0009FAF0 0240:
	_ctf_engine_weapon_update (0000)
0009FD30 0080:
	_ctf_engine_update (0000)
0009FDB0 0160:
	_ctf_weapon_pickup (0000)
0009FF10 0120:
	_ctf_engine_starting_location_rating (0000)
0025BBC4 000f:
	??_C@_0P@HPMPGFLN@created?5a?5flag?$AA@ (0000)
0025BBD4 001a:
	??_C@_0BK@JCPFMGLB@failed?5to?5create?5the?5flag?$AA@ (0000)
0025BBF0 0015:
	??_C@_0BF@KKINPGPB@NONE?5?$CB?$DN?5weapon_index?$AA@ (0000)
0025BC08 0026:
	??_C@_0CG@KLNGAFMF@c?3?2halo?2SOURCE?2game?2game_engine_@ (0000)
0025BC30 0013:
	??_C@_0BD@MAOGAFHI@NONE?5?$CB?$DN?5team_index?$AA@ (0000)
0025BC44 0018:
	??_C@_0BI@HLCGDKNJ@game_engine_can_score?$CI?$CJ?$AA@ (0000)
0025BC60 004b:
	??_C@_0EL@BLLOIBND@NETGAME_FLAG_WARNING?5starting?5lo@ (0000)
0025BCAC 003c:
	??_C@_0DM@LOHOHMDO@NETGAME_FLAG_WARNING?5starting?5lo@ (0000)
0025BCE8 002f:
	??_C@_0CP@KMKIMAED@?$CIflag_to_create?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIflag_t@ (0000)
0025BD18 0024:
	??_C@_0CE@FKOBANOL@failed?5to?5find?5one?5of?5the?5ctf?5fl@ (0000)
0025BD3C 001d:
	??_C@_0BN@PNGPDONM@ctf?5started?5up?5without?5teams?$AA@ (0000)
0025BD5C 0013:
	??_C@_0BD@CBKEKJFH@NONE?5?$CB?$DN?5unit_index?$AA@ (0000)
0025BD70 0036:
	??_C@_0DG@PMEHEDDI@weapon?9?$DOobject?4owner_team_index?5@ (0000)
0025BDA8 000a:
	??_C@_09NOBKAKN@flag_blue?$AA@ (0000)
0025BDB8 0008:
	__real@3fd51eb860000000 (0000)
002DE400 0088:
	_ctf_engine (0000)
0043E914 0030:
	_ctf_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "cseries/errors.h"
#include "game_engine.h"
#include "game_engine_runtime.h"
#include "players.h"
#include "players_runtime.h"
#include "items/weapons.h"
#include "objects/objects.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	NUMBER_OF_CTF_TEAMS = 2,
	CTF_WARNING_SOUND_INTERVAL = 20 * TICKS_PER_SECOND,
	CTF_FLAG_FAILURE_SOUND_INTERVAL = 4 * TICKS_PER_SECOND,
	CTF_FLAG_RETURN_TIME = 17 * TICKS_PER_SECOND,
	CTF_ENDGAME_SOUND_TIMEOUT = 2 * TICKS_PER_SECOND,
	CTF_STARTING_LOCATION_RATING_WARMUP = TICKS_PER_SECOND,
	CTF_STARTING_LOCATION_BAD_TEAM_INDEX = 3,
};

enum
{
	_ctf_weapon_handled_bit = 6,
};

enum
{
	_multiplayer_sound_blue_team_has_the_flag = 0x8,
	_multiplayer_sound_blue_team_flag_returned = 0x9,
	_multiplayer_sound_blue_team_score = 0xA,
	_multiplayer_sound_red_team_has_the_flag = 0xB,
	_multiplayer_sound_red_team_flag_returned = 0xC,
	_multiplayer_sound_red_team_score = 0xD,
	_multiplayer_sound_capture_the_flag = 0x16,
	_multiplayer_sound_flag_failure = 0x1C,
	_multiplayer_sound_blue_team_ctf = 0x25,
	_multiplayer_sound_red_team_ctf = 0x26,
};

enum
{
	_ctf_message_score = 0x1E,
	_ctf_message_you_scored,
	_ctf_message_enemy_scored,
	_ctf_message_good_guys_scored,
	_ctf_message_you_have_the_flag,
	_ctf_message_you_returned_the_flag,
	_ctf_message_enemy_has_the_flag,
	_ctf_message_enemy_has_the_flag_tick,
	_ctf_message_enemy_returned_the_flag,
	_ctf_message_good_guys_have_the_flag,
	_ctf_message_good_guys_returned_the_flag,
	_ctf_message_your_flag_was_returned,
	_ctf_message_enemy_flag_was_returned,
	_ctf_message_time_expired,
	_ctf_message_you_are_on_attack,
	_ctf_message_you_are_on_defense,
};

enum
{
	_string_red_team_score_blue_team_score = 0x8C,
	_string_you_scored_score_to_score,
	_string_enemy_scored_score_to_score,
	_string_your_ally_scored_score_to_score,
	_string_you_returned_the_flag,
	_string_the_enemy_has_your_flag,
	_string_the_enemy_returned_the_flag,
	_string_your_ally_has_the_flag,
	_string_your_ally_returned_the_flag,
	_string_your_flag_was_returned,
	_string_the_enemys_flag_was_returned,
	_string_time_expired,
	_string_you_are_on_offense,
	_string_you_are_on_defense,
	_string_score,
};

/* ---------- macros */

/* ---------- structures */

typedef char verify_scenario_netgame_flag_size[
	sizeof(struct scenario_netgame_flag) == 0x94 ? 1 : -1];

struct ctf_globals
{
	struct scenario_netgame_flag *flags[NUMBER_OF_CTF_TEAMS];
	long weapon_indices[NUMBER_OF_CTF_TEAMS];
	long scores[NUMBER_OF_CTF_TEAMS];
	long score_to_win;
	boolean flag_warnings[NUMBER_OF_CTF_TEAMS];
	byte pad1E[2];
	long flag_warning_ticks[NUMBER_OF_CTF_TEAMS];
	long flag_swap_timer;
	long next_flag_failure_time;
};

typedef char verify_ctf_globals_scores_offset[
	offsetof(struct ctf_globals, scores) == 0x10 ? 1 : -1];
typedef char verify_ctf_globals_size[
	sizeof(struct ctf_globals) == 0x30 ? 1 : -1];

/* ---------- prototypes */

static long ctf_create_flag_object(
	struct scenario_netgame_flag *flag);

static void create_the_flag(
	long team_index);

static void ctf_single_flag_what_is_up_message(
	long flag_team_index);

static void ctf_award_capture(
	long player_index,
	long team_index);

static void ctf_flag_failure_sound(
	long player_index);

static long ctf_find_flag_carrier(
	long weapon_index);

static void ctf_sound_update_warning(
	long team_index);

static void ctf_set_flag_warning(
	long team_index,
	boolean warning);

static void ctf_reset_flag(
	long weapon_index);

static void ctf_player_drop_flag(
	long player_index,
	long weapon_index);

static boolean ctf_position_near_flag(
	long team_index,
	real_point3d const *position,
	real radius);

/* ---------- globals */

static struct ctf_globals ctf_globals = { 0 };
extern long timeout_for_endgame_sound;

/* ---------- code */

static void ctf_engine_dispose(
	void)
{
	return;
}

static void ctf_engine_dispose_from_old_map(
	void)
{
	return;
}

static void ctf_engine_game_ending(
	void)
{
	return;
}

static void ctf_engine_statistics_append(
	long statistic)
{
	return;
}

static void ctf_engine_handle_client_message(
	void *message)
{
	return;
}

static void ctf_engine_handle_server_message(
	void *message)
{
	return;
}

static void ctf_engine_pregame_post_rasterize(
	void)
{
	return;
}

static void ctf_engine_post_rasterize(
	void)
{
	return;
}

static void ctf_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type)
{
	return;
}

static void ctf_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire)
{
	return;
}

static void ctf_engine_prespawn_player_update(
	long player_index)
{
	return;
}

void ctf_state_message_update_warning(
	long team_index)
{
	return;
}

static void ctf_engine_player_added(
	long player_index)
{
	player_get(player_index);

	return;
}

static void ctf_engine_game_starting(
	void)
{
	game_engine_play_multiplayer_sound(
		_multiplayer_sound_capture_the_flag);

	return;
}

static boolean ctf_engine_allow_pick_up(
	long unit_index,
	long weapon_index)
{
	boolean allow_pick_up = TRUE;
	long player_index = player_index_from_unit_index(unit_index);

	if (player_index != NONE && weapon_index != NONE)
	{
		struct player_datum *player = player_get(player_index);
		struct weapon_datum *weapon = weapon_try_and_get(weapon_index);

		if (weapon &&
			weapon_is_flag(weapon_index) &&
			!TEST_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit) &&
			weapon->object.owner_team_index == player->team_index)
		{
			allow_pick_up = FALSE;
		}
	}

	return allow_pick_up;
}

static wchar_t *ctf_get_score_string(
	long player_index,
	wchar_t *buffer)
{
	struct player_datum *player = player_get(player_index);

	usprintf(
		buffer,
		L"%d",
		player->statistics.multiplayer_statistics.ctf_statistics.flag_scores);

	return buffer;
}

static wchar_t *ctf_get_team_score_string(
	long team_index,
	wchar_t *buffer)
{
	usprintf(
		buffer,
		L"%d",
		ctf_globals.scores[team_index]);

	return buffer;
}

static boolean ctf_engine_initialize_for_new_map(
	void)
{
	struct scenario *scenario;
	long team_index;
	short starting_location_count;
	short starting_location_index;

	if (!game_engine_get_variant()->universal_variant.teams)
	{
		error(_error_silent, "ctf started up without teams");
	}

	csmemset(&ctf_globals, 0, sizeof(ctf_globals));
	ctf_globals.weapon_indices[0] = NONE;
	ctf_globals.weapon_indices[1] = NONE;
	timeout_for_endgame_sound = CTF_ENDGAME_SOUND_TIMEOUT;

	scenario = global_scenario_get();
	for (team_index = 0; team_index < NUMBER_OF_CTF_TEAMS; team_index++)
	{
		long flag_index = find_netgame_flag(
			NULL,
			0.0f,
			0.0f,
			_netgame_flag_ctf_flag,
			(short)team_index);
		long flag_slot;

		ctf_globals.scores[team_index] = 0;
		flag_slot = team_index;
		if (game_engine_get_variant()->game_engine_variant.ctf.assault)
			flag_slot = (team_index + 1) % 2;

		ctf_globals.flags[flag_slot] = NULL;
		if (flag_index != NONE)
		{
			ctf_globals.flags[flag_slot] = TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_flags,
				flag_index,
				struct scenario_netgame_flag);
		}

		if (!ctf_globals.flags[flag_slot])
		{
			error(_error_silent, "failed to find one of the ctf flags");
		}
	}

	if (game_engine_get_variant()->game_engine_variant.ctf.single_flag_time > 0)
	{
		long flag_to_create = random_range(0, NUMBER_OF_CTF_TEAMS);

		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
			0x107,
			(flag_to_create >= 0) && (flag_to_create <= 1));

		create_the_flag(flag_to_create);
		ctf_single_flag_what_is_up_message(flag_to_create);
		ctf_globals.flag_swap_timer = game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;
	}
	else
	{
		for (team_index = 0; team_index < NUMBER_OF_CTF_TEAMS; team_index++)
		{
			create_the_flag(team_index);
		}
	}

	ctf_globals.score_to_win = game_engine_get_variant()->universal_variant.score_to_win;

	starting_location_count = player_get_starting_location_count();
	for (starting_location_index = 0;
		starting_location_index < starting_location_count;
		starting_location_index++)
	{
		struct player_starting_location *starting_location =
			player_get_starting_location(starting_location_index);

		if (starting_location->team_index != _team_red &&
			starting_location->team_index != _team_blue)
		{
			error(
				_error_silent,
				"NETGAME_FLAG_WARNING starting location %d bad team index %d",
				starting_location_index,
				starting_location->team_index);
		}
		else if (match_game_type(game_engine_ctf, 4, starting_location->game_types))
		{
			boolean assault = game_engine_get_variant()->game_engine_variant.ctf.assault > 0;
			long location_team_index = starting_location->team_index;
			long own_team_index = location_team_index % 2;
			real distance_to_own_flag = distance_squared3d(
				&ctf_globals.flags[own_team_index]->position,
				&starting_location->position);
			real distance_to_enemy_flag = distance_squared3d(
				&ctf_globals.flags[(own_team_index + 1) % 2]->position,
				&starting_location->position);

			if (assault ?
				distance_to_own_flag < distance_to_enemy_flag :
				distance_to_own_flag > distance_to_enemy_flag)
			{
				error(
					_error_silent,
					"NETGAME_FLAG_WARNING starting location %d team %d, too close to enemy flag",
					starting_location_index,
					location_team_index);
				starting_location->team_index = CTF_STARTING_LOCATION_BAD_TEAM_INDEX;
			}
		}
	}

	return TRUE;
}

static void ctf_engine_player_update(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	long unit_index;

	if (game_engine_player_has_flag(player_index))
		game_engine_player_depower_active_camo(player_index);

	unit_index = player->unit_index;
	if (unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(unit_index);

		if (unit->unit.current_weapon_index != NONE)
		{
			long weapon_index = unit->unit.weapon_object_indices[unit->unit.current_weapon_index];

			if (weapon_index != NONE)
			{
				struct weapon_datum *weapon = weapon_get(weapon_index);

				if (game_engine_can_score() && weapon_is_flag(weapon_index))
				{
					match_assert(
						"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
						0x214,
						weapon->object.owner_team_index != player->team_index);

					if (ctf_position_near_flag(player->team_index, &unit->object.position, 1.0f))
					{
						boolean flag_at_home = TRUE;

						if (game_engine_get_variant()->game_engine_variant.ctf.flag_at_home_to_score &&
							game_engine_get_variant()->game_engine_variant.ctf.single_flag_time == 0)
						{
							struct weapon_datum *own_flag = weapon_get(
								ctf_globals.weapon_indices[player->team_index]);

							flag_at_home = !TEST_FLAG(own_flag->weapon.flags, _ctf_weapon_handled_bit);
						}

						if (flag_at_home)
						{
							ctf_award_capture(player_index, player->team_index);
							ctf_player_drop_flag(player_index, weapon_index);
							/* BUG (preserved for exact matching): January (and the
							   later HCEA build) call game_engine_get_variant() here
							   and discard the result. A corrected build should drop
							   this statement. */
							game_engine_get_variant();
						}
						else
						{
							ctf_flag_failure_sound(player_index);
						}
					}
				}
			}
		}
	}

	return;
}

static void ctf_engine_weapon_update(
	long weapon_index,
	struct weapon_datum *weapon)
{
	long carrier_player_index;
	long team_index;
	long ticks;
	long other_team_index;
	real_point3d position;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x261,
		weapon_is_flag(weapon_index));

	if (game_engine_get_variant()->game_engine_variant.ctf.single_flag_time > 0)
	{
		if (ctf_globals.flag_swap_timer > 0)
			ctf_globals.flag_swap_timer--;

		if (ctf_globals.flag_swap_timer == 0 &&
			!TEST_FLAG(weapon->item.flags, _item_attached_to_unit_bit))
		{
			game_show_score(NONE, _ctf_message_time_expired);
			ctf_set_flag_warning(_team_red, FALSE);
			ctf_set_flag_warning(_team_blue, FALSE);
			ctf_globals.weapon_indices[weapon->object.owner_team_index] = NONE;
			game_engine_clear_goal_position(0);
			game_engine_clear_goal_position(1);
			weapon->object.owner_team_index = (weapon->object.owner_team_index + 1) % 2;
			game_engine_play_multiplayer_sound(
				weapon->object.owner_team_index == _team_red ?
					_multiplayer_sound_blue_team_ctf :
					_multiplayer_sound_red_team_ctf);
			ctf_reset_flag(weapon_index);
			game_engine_clear_goal_position(2);
			game_engine_clear_goal_position(3);
			ctf_globals.flag_swap_timer = game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;
			ctf_single_flag_what_is_up_message(weapon->object.owner_team_index);
		}
	}

	ticks = game_time_get();
	if ((unsigned long)(ticks - weapon->item.last_owned_time) > CTF_FLAG_RETURN_TIME &&
		weapon_is_flag(weapon_index))
	{
		boolean connected_to_map = TEST_FLAG(
			weapon->object.flags,
			_object_connected_to_map_bit);

		if (connected_to_map && weapon->object.parent_object_index == NONE)
		{
			long return_team_index = weapon->object.owner_team_index;
			long return_other_team_index = (weapon->object.owner_team_index + 1) % 2;

			if (TEST_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit))
			{
				game_engine_play_multiplayer_sound(
					return_team_index == _team_red ?
						_multiplayer_sound_red_team_flag_returned :
						_multiplayer_sound_blue_team_flag_returned);
				ctf_set_flag_warning(weapon->object.owner_team_index, FALSE);
				game_show_score_team(return_team_index, _ctf_message_your_flag_was_returned);
				game_show_score_team(return_other_team_index, _ctf_message_enemy_flag_was_returned);
			}
			ctf_reset_flag(weapon_index);
		}
	}

	carrier_player_index = ctf_find_flag_carrier(weapon_index);
	team_index = weapon->object.owner_team_index;
	other_team_index = (team_index + 1) % 2;
	item_get_position_even_if_in_inventory(weapon_index, &position);
	game_engine_set_goal_position(
		team_index,
		&position,
		0.0f,
		"flag_blue",
		NONE,
		other_team_index,
		carrier_player_index);
	if (carrier_player_index != NONE)
	{
		position = ctf_globals.flags[other_team_index]->position;
		position.y += 0.5f;
		game_engine_set_goal_position(
			team_index + 2,
			&position,
			0.0f,
			"default",
			carrier_player_index,
			NONE,
			NONE);
	}
	else
	{
		game_engine_clear_goal_position(team_index + 2);
	}

	return;
}

static boolean ctf_weapon_pickup(
	long weapon_index,
	long player_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	boolean result = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x3CE,
		weapon_is_flag(weapon_index));

	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (weapon->object.owner_team_index == player->team_index)
		{
			if (!game_engine_get_variant()->game_engine_variant.ctf.flag_must_reset)
			{
				if (TEST_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit) &&
					game_engine_can_score())
				{
					ctf_set_flag_warning(weapon->object.owner_team_index, FALSE);
					player->statistics.multiplayer_statistics.ctf_statistics.flag_returns++;
					game_show_score_you_ally_enemy(
						player_index,
						_ctf_message_you_returned_the_flag,
						_ctf_message_good_guys_returned_the_flag,
						_ctf_message_enemy_returned_the_flag,
						player_index);
					game_engine_play_multiplayer_sound(
						player->team_index == _team_red ?
							_multiplayer_sound_red_team_flag_returned :
							_multiplayer_sound_blue_team_flag_returned);
				}
				ctf_reset_flag(weapon_index);
			}
			else if (TEST_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit))
			{
				ctf_flag_failure_sound(player_index);
			}
			result = FALSE;
		}
		else
		{
			if (!TEST_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit) &&
				game_engine_can_score())
			{
				player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs++;
				if (!game_engine_get_variant()->game_engine_variant.ctf.assault)
				{
					game_engine_play_multiplayer_sound(
						player->team_index == _team_red ?
							_multiplayer_sound_red_team_has_the_flag :
							_multiplayer_sound_blue_team_has_the_flag);
					ctf_set_flag_warning(weapon->object.owner_team_index, TRUE);
					game_show_score_you_ally_enemy(
						player_index,
						NONE,
						_ctf_message_good_guys_have_the_flag,
						_ctf_message_enemy_has_the_flag,
						player_index);
				}
			}
			SET_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit, TRUE);
		}
	}

	return result;
}

static void ctf_weapon_drop(
	long weapon_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x41A,
		weapon_is_flag(weapon_index));

	return;
}

static void ctf_engine_update(
	void)
{
	boolean game_over = FALSE;
	long team_index;

	for (team_index = 0; team_index < NUMBER_OF_CTF_TEAMS; team_index++)
	{
		if (ctf_globals.scores[team_index] >= ctf_globals.score_to_win)
			game_over = TRUE;
	}

	if (game_over)
		game_engine_end_game();

	ctf_sound_update_warning(_team_red);
	ctf_sound_update_warning(_team_blue);

	return;
}

static long ctf_get_score(
	long player_index,
	enum get_score_type score_type)
{
	struct player_datum *player = player_get(player_index);

	if (score_type == _get_score_individual)
		return player->statistics.multiplayer_statistics.ctf_statistics.flag_scores;

	return ctf_globals.scores[player->team_index];
}

static wchar_t *ctf_get_score_header_string(
	wchar_t *buffer)
{
	long string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");

	wchar_t *string;

	if (string_list_index != NONE)
		string = unicode_string_list_get_string(string_list_index, _string_score);
	else
		string = L"";

	ustrcpy(buffer, string);

	return buffer;
}

static boolean ctf_engine_display_score(
	long player_index,
	long message,
	long parameter2,
	wchar_t *buffer,
	long buffer_size)
{
	struct player_datum *player = player_get(player_index);
	long team_index = player->team_index;
	long other_team_index = (team_index + 1) % 2;
	long string_list_index;
	wchar_t *string;
	boolean result = TRUE;

	switch (message)
	{
	case _ctf_message_score:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_red_team_score_blue_team_score);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			ctf_globals.scores[_team_red],
			ctf_globals.scores[_team_blue]);
		break;

	case _ctf_message_you_scored:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_you_scored_score_to_score);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			ctf_globals.scores[team_index],
			ctf_globals.scores[other_team_index]);
		break;

	case _ctf_message_enemy_scored:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_enemy_scored_score_to_score);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			ctf_globals.scores[team_index],
			ctf_globals.scores[other_team_index]);
		break;

	case _ctf_message_good_guys_scored:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_your_ally_scored_score_to_score);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			ctf_globals.scores[team_index],
			ctf_globals.scores[other_team_index]);
		break;

	case _ctf_message_you_have_the_flag:
		usnprintf(buffer, buffer_size, L"");
		break;

	case _ctf_message_you_returned_the_flag:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_you_returned_the_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_enemy_has_the_flag:
	case _ctf_message_enemy_has_the_flag_tick:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_the_enemy_has_your_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_enemy_returned_the_flag:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_the_enemy_returned_the_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_good_guys_have_the_flag:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_your_ally_has_the_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_good_guys_returned_the_flag:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_your_ally_returned_the_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_your_flag_was_returned:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_your_flag_was_returned);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_enemy_flag_was_returned:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_the_enemys_flag_was_returned);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_time_expired:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_time_expired);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_you_are_on_attack:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_you_are_on_offense);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _ctf_message_you_are_on_defense:
		string_list_index = tag_loaded(UNICODE_STRING_LIST_TAG, "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_string_you_are_on_defense);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	default:
		result = FALSE;
		break;
	}

	return result;
}

static real ctf_engine_starting_location_rating(
	long player_index,
	struct player_starting_location const *starting_location)
{
	real rating = 1.0f;

	if (game_engine_get_variant()->game_engine_variant.ctf.assault)
	{
		struct player_datum *player = player_get(player_index);
		struct scenario_netgame_flag *flag = ctf_globals.flags[(player->team_index + 1) % 2];
		real dx = flag->position.x - starting_location->position.x;
		real dy = flag->position.y - starting_location->position.y;
		real dz = flag->position.z - starting_location->position.z;
		real distance_squared = dx * dx;

		distance_squared += dy * dy;
		distance_squared += dz * dz;

		distance_squared = PIN(distance_squared, 0.5f, 10.0f);
		rating = 1.0f / distance_squared;

		if (game_time_get() > CTF_STARTING_LOCATION_RATING_WARMUP)
		{
			if (distance_squared > 1.0f)
				rating = (real)pow(rating, 0.33f);

			rating = PIN(rating, 0.5f, 2.0f);
		}
	}

	return rating;
}

static boolean ctf_test_flag(
	long flag)
{
	boolean result = FALSE;

	if (flag == 0)
		result = TRUE;

	return result;
}

/* ---------- private code */

static long ctf_create_flag_object(
	struct scenario_netgame_flag *flag)
{
	struct object_placement_data placement_data;
	long flag_definition_index = get_flag_definition_index();
	long weapon_index;

	object_placement_data_new(&placement_data, flag_definition_index, NONE);
	placement_data.position = flag->position;
	weapon_index = object_new(&placement_data);
	object_set_automatic_deactivation(weapon_index, FALSE);
	OutputDebugString("created a flag");

	return weapon_index;
}

static void create_the_flag(
	long team_index)
{
	struct scenario_netgame_flag *flag = ctf_globals.flags[team_index];

	if (flag)
	{
		long weapon_index = ctf_create_flag_object(flag);

		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
			0xBD,
			NONE != weapon_index);

		if (weapon_index == NONE)
		{
			error(_error_silent, "failed to create the flag");
		}
		else
		{
			weapon_get(weapon_index)->object.owner_team_index = (short)team_index;
			ctf_globals.weapon_indices[team_index] = weapon_index;
		}
	}

	return;
}

static void ctf_single_flag_what_is_up_message(
	long flag_team_index)
{
	game_show_score_team(flag_team_index % 2, _ctf_message_you_are_on_defense);
	game_show_score_team((flag_team_index + 1) % 2, _ctf_message_you_are_on_attack);

	return;
}

static void ctf_award_capture(
	long player_index,
	long team_index)
{
	struct player_datum *player = player_get(player_index);

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x1B4,
		game_engine_can_score());
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x1B5,
		NONE != player_index);
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x1B6,
		NONE != team_index);

	ctf_globals.scores[team_index]++;
	player->statistics.multiplayer_statistics.ctf_statistics.flag_scores++;
	game_engine_play_multiplayer_sound(
		player->team_index == _team_red ?
			_multiplayer_sound_red_team_score :
			_multiplayer_sound_blue_team_score);
	game_show_score_you_ally_enemy(
		player_index,
		_ctf_message_you_scored,
		_ctf_message_good_guys_scored,
		_ctf_message_enemy_scored,
		player_index);

	return;
}

static void ctf_flag_failure_sound(
	long player_index)
{
	if (game_time_get() > ctf_globals.next_flag_failure_time)
	{
		if (player_index != NONE &&
			player_get(player_index)->local_player_index != NONE)
		{
			game_engine_play_multiplayer_sound(_multiplayer_sound_flag_failure);
			ctf_globals.next_flag_failure_time =
				game_time_get() + CTF_FLAG_FAILURE_SOUND_INTERVAL;
		}
	}

	return;
}

static long ctf_find_flag_carrier(
	long weapon_index)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long carrier_player_index = NONE;

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);
	while (player)
	{
		if (player->unit_index != NONE &&
			unit_has_weapon(player->unit_index, weapon_index))
		{
			carrier_player_index = iterator.datum_index;
			break;
		}

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return carrier_player_index;
}

static void ctf_sound_update_warning(
	long team_index)
{
	if (ctf_globals.flag_warnings[team_index])
	{
		if (ctf_globals.flag_warning_ticks[team_index] > CTF_WARNING_SOUND_INTERVAL)
		{
			game_engine_play_multiplayer_sound(
				team_index == _team_red ?
					_multiplayer_sound_blue_team_has_the_flag :
					_multiplayer_sound_red_team_has_the_flag);
			ctf_globals.flag_warning_ticks[team_index] = 0;
		}

		ctf_globals.flag_warning_ticks[team_index]++;
	}

	return;
}

static void ctf_set_flag_warning(
	long team_index,
	boolean warning)
{
	ctf_globals.flag_warnings[team_index] = warning;
	ctf_globals.flag_warning_ticks[team_index] = 0;

	return;
}

static void ctf_reset_flag(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	struct scenario_netgame_flag *flag;

	ctf_set_flag_warning(weapon->object.owner_team_index, FALSE);
	flag = ctf_globals.flags[weapon->object.owner_team_index];
	if (flag)
	{
		game_engine_flag_reset(weapon_index, &flag->position);
		SET_FLAG(weapon->weapon.flags, _ctf_weapon_handled_bit, FALSE);
	}

	return;
}

static void ctf_player_drop_flag(
	long player_index,
	long weapon_index)
{
	struct player_datum *player = player_get(player_index);
	long unit_index = player->unit_index;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_ctf.c",
		0x1A7,
		NONE != unit_index);

	unit_drop_current_weapon(unit_index, TRUE);
	ctf_reset_flag(weapon_index);

	return;
}

static boolean ctf_position_near_flag(
	long team_index,
	real_point3d const *position,
	real radius)
{
	boolean result = FALSE;

	if (position)
	{
		struct scenario_netgame_flag *flag = ctf_globals.flags[team_index];

		if (flag)
		{
			real dx = flag->position.x - position->x;
			real dy = flag->position.y - position->y;
			real dz = flag->position.z - position->z;
			real distance_squared = dx * dx;

			distance_squared += dy * dy;
			distance_squared += dz * dz;
			if (distance_squared < radius * radius)
				result = TRUE;
		}
	}

	return result;
}

/* ---------- engine table */

struct game_engine ctf_engine =
{
	"ctf",
	game_engine_ctf,
	ctf_engine_dispose,
	ctf_engine_initialize_for_new_map,
	ctf_engine_dispose_from_old_map,
	ctf_engine_player_added,
	ctf_engine_game_ending,
	ctf_engine_game_starting,
	ctf_engine_statistics_append,
	ctf_engine_handle_client_message,
	ctf_engine_handle_server_message,
	ctf_engine_pregame_post_rasterize,
	ctf_engine_post_rasterize,
	ctf_engine_player_update,
	ctf_engine_weapon_update,
	ctf_weapon_pickup,
	ctf_weapon_drop,
	ctf_engine_update,
	ctf_get_score,
	ctf_get_score_string,
	ctf_get_score_header_string,
	ctf_get_team_score_string,
	ctf_engine_allow_pick_up,
	ctf_engine_player_damaged_player,
	ctf_engine_player_killed_player,
	ctf_engine_display_score,
	ctf_engine_starting_location_rating,
	ctf_engine_prespawn_player_update,
	NULL,
	NULL,
	NULL,
	ctf_test_flag,
	NULL,
	NULL,
};

#ifdef HALO_LINUX
/* the parts of ctf_globals that are the same on every machine (the flag
objects are the host's, at the same indices everywhere: the flags' scenario
places are each machine's own pointers) */
struct ctf_network_state
{
	long weapon_indices[NUMBER_OF_CTF_TEAMS];
	long scores[NUMBER_OF_CTF_TEAMS];
	boolean flag_warnings[NUMBER_OF_CTF_TEAMS];
	long flag_warning_ticks[NUMBER_OF_CTF_TEAMS];
	long flag_swap_timer;
};

/* the distributed netcode (port/linux/game/network_distributed.c): the game
type's state the host sends its clients, which take it as it is */
long game_engine_ctf_write_network_state(
	byte *buffer,
	long size)
{
	struct ctf_network_state state;

	if (size < (long)sizeof(state))
		return 0;
	csmemset(&state, 0, sizeof(state));
	csmemcpy(state.weapon_indices, ctf_globals.weapon_indices, sizeof(state.weapon_indices));
	csmemcpy(state.scores, ctf_globals.scores, sizeof(state.scores));
	csmemcpy(state.flag_warnings, ctf_globals.flag_warnings, sizeof(state.flag_warnings));
	csmemcpy(state.flag_warning_ticks, ctf_globals.flag_warning_ticks, sizeof(state.flag_warning_ticks));
	state.flag_swap_timer = ctf_globals.flag_swap_timer;
	csmemcpy(buffer, &state, sizeof(state));
	return sizeof(state);
}

void game_engine_ctf_read_network_state(
	byte const *buffer,
	long size)
{
	struct ctf_network_state state;

	if (size != (long)sizeof(state))
		return;
	csmemcpy(&state, buffer, sizeof(state));
	csmemcpy(ctf_globals.weapon_indices, state.weapon_indices, sizeof(state.weapon_indices));
	csmemcpy(ctf_globals.scores, state.scores, sizeof(state.scores));
	csmemcpy(ctf_globals.flag_warnings, state.flag_warnings, sizeof(state.flag_warnings));
	csmemcpy(ctf_globals.flag_warning_ticks, state.flag_warning_ticks, sizeof(state.flag_warning_ticks));
	ctf_globals.flag_swap_timer = state.flag_swap_timer;
}
#endif
