/*
GAME_ENGINE_ODDBALL.C

symbols in this file:
000A1630 0010:
	_oddball_engine_dispose (0000)
000A1640 0010:
	_oddball_engine_dispose_from_old_map (0000)
000A1650 0020:
	_oddball_engine_player_added (0000)
000A1670 0010:
	_oddball_engine_game_ending (0000)
000A1680 0010:
	_oddball_engine_game_starting (0000)
000A1690 0010:
	_oddball_engine_statistics_append (0000)
000A16A0 0010:
	_oddball_engine_handle_client_message (0000)
000A16B0 0010:
	_oddball_engine_handle_server_message (0000)
000A16C0 0010:
	_oddball_engine_pregame_post_rasterize (0000)
000A16D0 0010:
	_oddball_engine_post_rasterize (0000)
000A16E0 00d0:
	_oddball_add_score (0000)
000A17B0 0040:
	_oddball_add_time_with_ball (0000)
000A17F0 0030:
	_player_ball_count (0000)
000A1820 0010:
	_oddball_engine_player_damaged_player (0000)
000A1830 0030:
	_player_has_ball (0000)
000A1860 0040:
	_ball_available (0000)
000A18A0 02e0:
	_oddball_engine_display_score (0000)
000A1B80 0010:
	_oddball_engine_prespawn_player_update (0000)
000A1B90 0040:
	_oddball_weapon_drop (0000)
000A1BD0 0040:
	_oddball_get_score (0000)
000A1C10 0020:
	_oddball_ball_transfer_by_killing (0000)
000A1C30 0020:
	_accumulate_score_by_time (0000)
000A1C50 0020:
	_terminator_scoring_rules (0000)
000A1C70 0020:
	_oddball_test_flag (0000)
000A1C90 0050:
	_oddball_test_trait (0000)
000A1CE0 0050:
	_oddball_get_score_string (0000)
000A1D30 0070:
	_oddball_get_score_header_string (0000)
000A1DA0 0050:
	_oddball_get_team_score_string (0000)
000A1DF0 0140:
	_find_position_for_ball (0000)
000A1F30 0090:
	_create_the_ball (0000)
000A1FC0 0120:
	_oddball_engine_initialize_for_new_map (0000)
000A20E0 0070:
	_reset_ball (0000)
000A2150 0090:
	_update_ball_ownership (0000)
000A21E0 01c0:
	_oddball_engine_player_update (0000)
000A23A0 00c0:
	_oddball_engine_weapon_update (0000)
000A2460 00d0:
	_oddball_engine_update (0000)
000A2530 01c0:
	_oddball_engine_player_killed_player (0000)
000A26F0 00b0:
	_oddball_weapon_pickup (0000)
0025BE74 002a:
	??_C@_0CK@LKLFFONL@c?3?2halo?2SOURCE?2game?2game_engine_@ (0000)
0025BEA0 0034:
	??_C@_0DE@EJKHILOB@0?5?$CG?$CG?5?$CCthis?5map?5was?5not?5correctly@ (0000)
0025BED8 0040:
	??_C@_0EA@MPBOOMHB@?$CD?$CD?$CD?5failed?5to?5find?5any?5suitable?5@ (0000)
0025BF18 0023:
	??_C@_0CD@CKBFGHJI@?$CD?$CD?$CD?5failed?5to?5find?5the?5flag?5obje@ (0000)
0025BF3C 0028:
	??_C@_0CI@KJBBGHKH@?$CD?$CD?$CD?5failed?5to?5find?5ball?5spawn?5te@ (0000)
0025BF64 0028:
	??_C@_0CI@LAAKFGOG@?$CD?$CD?$CD?5failed?5to?5find?5ball?5spawn?5te@ (0000)
0025BF8C 000a:
	??_C@_09GMEHKGKL@ball_blue?$AA@ (0000)
0025BF98 000c:
	??_C@_0M@FNMEMKCB@target_blue?$AA@ (0000)
0025BFA4 002d:
	??_C@_0CN@DJFDFML@?$CBball_available?$CI?$CJ?5?$HM?$HM?5?$CIcapture_in@ (0000)
002DE560 0088:
	_oddball_engine (0000)
0043EBA8 0104:
	_oddball_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game_engine_place.h"
#include "game/game_engine_runtime.h"
#include "game.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "players.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	MAXIMUM_ODDBALLS = 16,

	ODDBALL_START_ANNOUNCEMENT_TICKS = 2 * TICKS_PER_SECOND,
	ODDBALL_SPAWN_DELAY_TICKS = 15 * TICKS_PER_SECOND,
	ODDBALL_UNTOUCHED_RESET_TICKS = 40 * TICKS_PER_SECOND,
	ODDBALL_SCORE_TICKS_PER_UNIT = 60 * TICKS_PER_SECOND,
	ODDBALL_SCORE_WARNING_TICKS = 5 * TICKS_PER_SECOND,
	ODDBALL_MAXIMUM_BALLS_FOR_RESET_SOUND = 2,
};

enum oddball_weapon_flags
{
	_oddball_weapon_touched_bit = 6,
};

/* game_engine_test_flag() queries */
enum oddball_game_engine_test_flags
{
	_game_engine_test_flag_rasterize_score = 1,
};

enum oddball_ball_type
{
	_oddball_normal = 0,
	_oddball_magic,
	_oddball_terminator,
};

enum oddball_speed
{
	_oddball_speed_slow = 0,
	_oddball_speed_normal,
	_oddball_speed_faster,
};

enum game_engine_trait
{
	_game_trait_none = 0,
	_game_trait_invisible,
	_game_trait_extra_damage,
	_game_trait_damage_resistant,
};

enum oddball_message
{
	_oddball_message_you_have_the_ball = 30,
	_oddball_message_ally_has_the_ball,
	_oddball_message_enemy_has_the_ball,
	_oddball_message_you_are_it,
	_oddball_message_ally_is_it,
	_oddball_message_enemy_is_it,
	_oddball_message_ball_was_reset,
	_oddball_message_enemy_has_the_ball_tick,
	_oddball_message_ally_has_the_ball_tick,
	_oddball_message_you_have_the_ball_tick,
};

enum multiplayer_information_sound
{
	_multiplayer_sound_play_ball = 0,
	_multiplayer_sound_one_minute_to_win = 2,
	_multiplayer_sound_30_seconds_to_win = 3,
	_multiplayer_sound_red_team_minute_to_win = 4,
	_multiplayer_sound_red_team_30_seconds_to_win = 5,
	_multiplayer_sound_blue_team_minute_to_win = 6,
	_multiplayer_sound_blue_team_30_seconds_to_win = 7,
	_multiplayer_sound_oddball = 0x13,
	_multiplayer_sound_hill_move = 0x1E,
	_multiplayer_sound_team_oddball = 0x21,
	_multiplayer_sound_countdown_timer_end = 0x2A,
};

/* ui\multiplayer_game_text string indices */
enum multiplayer_game_text
{
	_multiplayer_game_text_score = 0x9A,
	_multiplayer_game_text_place_score = 0x9B,
	_multiplayer_game_text_time = 0x9E,
	_multiplayer_game_text_you_are_it = 0x9F,
	_multiplayer_game_text_ally_is_it = 0xA0,
	_multiplayer_game_text_enemy_is_it = 0xA1,
	_multiplayer_game_text_you_have_the_ball = 0xA2,
	_multiplayer_game_text_ally_has_the_ball = 0xA3,
	_multiplayer_game_text_enemy_has_the_ball = 0xA4,
	_multiplayer_game_text_ally_has_the_ball_score = 0xA5,
	_multiplayer_game_text_enemy_has_the_ball_score = 0xA6,
};

/* ---------- macros */


/* ---------- structures */

struct oddball_globals
{
	long score_to_win;
#ifdef HALO_LINUX
	/* port: the scores are indexed by team (free for all gives every player
	a team) and by absolute player index, not by ball */
	long team_score[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
	long individual_score[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
#else
	long team_score[MAXIMUM_ODDBALLS];
	long individual_score[MAXIMUM_ODDBALLS];
#endif
	long ball_spawn_timer[MAXIMUM_ODDBALLS];
	long current_ball_owner[MAXIMUM_ODDBALLS];
};

/* January's layout; the port's score arrays are larger */
#ifndef HALO_LINUX
typedef char verify_oddball_globals_size[
	sizeof(struct oddball_globals) == 0x104 ? 1 : -1];
typedef char verify_oddball_globals_ball_spawn_timer_offset[
	offsetof(struct oddball_globals, ball_spawn_timer) == 0x84 ? 1 : -1];
typedef char verify_oddball_globals_current_ball_owner_offset[
	offsetof(struct oddball_globals, current_ball_owner) == 0xC4 ? 1 : -1];
#endif

typedef char verify_scenario_netgame_flag_size[
	sizeof(struct scenario_netgame_flag) == 0x94 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

static struct oddball_globals oddball_globals = { 0 };

/* ---------- public code */

static void oddball_engine_dispose(
	void)
{
	return;
}

static void oddball_engine_dispose_from_old_map(
	void)
{
	return;
}

static void oddball_engine_game_ending(
	void)
{
	return;
}

static void oddball_engine_game_starting(
	void)
{
	return;
}

static void oddball_engine_statistics_append(
	long statistic)
{
	return;
}

static void oddball_engine_handle_client_message(
	void *message)
{
	return;
}

static void oddball_engine_handle_server_message(
	void *message)
{
	return;
}

static void oddball_engine_pregame_post_rasterize(
	void)
{
	return;
}

static void oddball_engine_post_rasterize(
	void)
{
	return;
}

static void oddball_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type)
{
	return;
}

static void oddball_engine_prespawn_player_update(
	long player_index)
{
	return;
}

static void oddball_engine_player_added(
	long player_index)
{
	player_get(player_index);

	return;
}

static void oddball_weapon_drop(
	long weapon_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
		0x3C8,
		weapon_is_flag(weapon_index));

	return;
}

static boolean oddball_ball_transfer_by_killing(
	void)
{
	enum oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;

	if (ball_type > _oddball_normal && ball_type <= _oddball_terminator)
		return TRUE;

	return FALSE;
}

static boolean accumulate_score_by_time(
	void)
{
	switch (game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type)
	{
	case _oddball_terminator:
		return FALSE;
	}

	return TRUE;
}

static boolean terminator_scoring_rules(
	void)
{
	switch (game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type)
	{
	case _oddball_terminator:
		return TRUE;
	}

	return FALSE;
}

boolean player_has_ball(
	long player_index)
{
	long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
	boolean has_ball = FALSE;
	long ball_index;

	for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
	{
		if (oddball_globals.current_ball_owner[ball_index] == player_index)
		{
			has_ball = TRUE;
			break;
		}
	}

	return has_ball;
}

boolean ball_available(
	void)
{
	long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
	boolean available = FALSE;
	long ball_index;

	for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
	{
		if (oddball_globals.ball_spawn_timer[ball_index] == 0 &&
			oddball_globals.current_ball_owner[ball_index] == NONE)
		{
			available = TRUE;
			break;
		}
	}

	return available;
}

/* ---------- private code */

static long player_ball_count(
	long player_index)
{
	long ball_count = 0;
	long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
	long ball_index;

	for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
	{
		if (oddball_globals.current_ball_owner[ball_index] == player_index)
			ball_count++;
	}

	return ball_count;
}

static void oddball_add_score(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	oddball_globals.individual_score[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)]++;
	oddball_globals.team_score[player->team_index]++;

	if (oddball_globals.score_to_win - oddball_globals.team_score[player->team_index] ==
		30 * TICKS_PER_SECOND)
	{
		if (game_engine_has_teams())
		{
			game_engine_play_multiplayer_sound(
				player->team_index == _team_red ?
					_multiplayer_sound_red_team_30_seconds_to_win :
					_multiplayer_sound_blue_team_30_seconds_to_win);
		}
		else
		{
			game_engine_play_multiplayer_sound(_multiplayer_sound_30_seconds_to_win);
		}
	}

	if (oddball_globals.score_to_win - oddball_globals.team_score[player->team_index] ==
		60 * TICKS_PER_SECOND)
	{
		if (game_engine_has_teams())
		{
			game_engine_play_multiplayer_sound(
				player->team_index == _team_red ?
					_multiplayer_sound_red_team_minute_to_win :
					_multiplayer_sound_blue_team_minute_to_win);
		}
		else
		{
			game_engine_play_multiplayer_sound(_multiplayer_sound_one_minute_to_win);
		}
	}

	if (oddball_globals.team_score[player->team_index] >= oddball_globals.score_to_win)
		game_engine_end_game();

	return;
}

static void oddball_add_time_with_ball(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	if (game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_normal)
	{
		game_engine_state_message(
			player_index,
			_oddball_message_you_have_the_ball_tick,
			player_index);
	}

	player->statistics.multiplayer_statistics.oddball_statistics.time_with_the_ball++;
	oddball_add_score(player_index);

	return;
}

static real_point3d find_position_for_ball(
	long ball_index)
{
	struct scenario *scenario = global_scenario_get();
	long flag_index = NONE;
	/* BUG (original): after the fatal missing-spawn assertion, January returns
	 * the untouched value. A corrected build should report failure explicitly.
	 */
	real_point3d position;

	if (!game_engine_get_variant()->game_engine_variant.oddball.random_start)
	{
		flag_index = find_netgame_flag(
			NULL,
			0.0f,
			0.0f,
			_netgame_flag_oddball_ball_spawn,
			(short)ball_index);
	}

	if (flag_index == NONE)
	{
		long ball_flag_count = 0;
		short netgame_flag_index;

		for (netgame_flag_index = 0;
			netgame_flag_index < scenario->netgame_flags.count;
			netgame_flag_index++)
		{
			struct scenario_netgame_flag *netgame_flag = TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_flags,
				netgame_flag_index,
				struct scenario_netgame_flag);

			if (netgame_flag->type == _netgame_flag_oddball_ball_spawn)
				ball_flag_count++;
		}

		if (ball_flag_count == 0)
		{
			error(_error_silent, "### failed to find any suitable netgame flag to create the ball");
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
				0xAD,
				0 && "this map was not correctly setup for oddball");
		}
		else
		{
			long skip_count = seed_random_range(
				get_global_random_seed_address(),
				0,
				(short)ball_flag_count);

			for (netgame_flag_index = 0;
				netgame_flag_index < scenario->netgame_flags.count;
				netgame_flag_index++)
			{
				struct scenario_netgame_flag *netgame_flag = TAG_BLOCK_GET_ELEMENT(
					&scenario->netgame_flags,
					netgame_flag_index,
					struct scenario_netgame_flag);

				if (netgame_flag->type == _netgame_flag_oddball_ball_spawn)
				{
					if (skip_count == 0)
					{
						flag_index = netgame_flag_index;
						break;
					}
					skip_count--;
				}
			}
		}
	}

	if (flag_index != NONE)
	{
		position = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag)->position;
	}

	return position;
}

static void create_the_ball(
	long ball_index)
{
	if (!oddball_ball_transfer_by_killing())
	{
		long ball_definition_index = get_ball_definition_index();

		if (ball_definition_index == NONE)
		{
			error(_error_silent, "### failed to find the flag object");
		}
		else
		{
			struct object_placement_data placement_data;
			long ball_object_index;

			object_placement_data_new(&placement_data, ball_definition_index, NONE);
			placement_data.position = find_position_for_ball(ball_index);
			ball_object_index = object_new(&placement_data);
			weapon_get(ball_object_index)->object.owner_team_index = (short)ball_index;
			object_set_automatic_deactivation(ball_object_index, FALSE);
		}
	}

	return;
}

static void reset_ball(
	long weapon_index)
{
	struct weapon_datum *weapon = weapon_get(weapon_index);
	real_point3d position = find_position_for_ball(weapon->object.owner_team_index);

	if (game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count <= ODDBALL_MAXIMUM_BALLS_FOR_RESET_SOUND)
		game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move);

	game_engine_flag_reset(weapon_index, &position);
	SET_FLAG(weapon->weapon.flags, _oddball_weapon_touched_bit, FALSE);

	return;
}

static void update_ball_ownership(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	if (!oddball_ball_transfer_by_killing())
	{
		long ball_index;

		for (ball_index = 0; ball_index < MAXIMUM_ODDBALLS; ball_index++)
		{
			if (oddball_globals.current_ball_owner[ball_index] == player_index)
				oddball_globals.current_ball_owner[ball_index] = NONE;
		}

		if (player->unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);

			if (unit->unit.current_weapon_index != NONE)
			{
				long weapon_index = unit->unit.weapon_object_indices[unit->unit.current_weapon_index];

				if (weapon_index != NONE && weapon_is_flag(weapon_index))
				{
					struct weapon_datum *weapon = weapon_get(weapon_index);

					oddball_globals.current_ball_owner[weapon->object.owner_team_index] = player_index;
				}
			}
		}
	}

	return;
}

/* ---------- public code (game engine callbacks) */

static boolean oddball_engine_initialize_for_new_map(
	void)
{
	long ball_index;

	/* January performs this scenario touch even though the returned pointer is
	 * not consumed here; its call and relocation are present in the target.
	 */
	global_scenario_get();
	csmemset(&oddball_globals, 0, sizeof(oddball_globals));

	oddball_globals.score_to_win = game_engine_get_variant()->universal_variant.score_to_win;
	if (accumulate_score_by_time())
		oddball_globals.score_to_win *= ODDBALL_SCORE_TICKS_PER_UNIT;

	if (find_netgame_flag(NULL, 0.0f, 0.0f, _netgame_flag_oddball_ball_spawn, 0) == NONE)
		error(_error_silent, "### failed to find ball spawn team id 0");
	if (find_netgame_flag(NULL, 0.0f, 0.0f, _netgame_flag_oddball_ball_spawn, 1) == NONE)
		error(_error_silent, "### failed to find ball spawn team id 1");

	for (ball_index = 0; ball_index < MAXIMUM_ODDBALLS; ball_index++)
		oddball_globals.current_ball_owner[ball_index] = NONE;

	if (!oddball_ball_transfer_by_killing())
	{
		long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
		long spawn_delay = 0;

		for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
		{
			spawn_delay += ODDBALL_SPAWN_DELAY_TICKS;
			oddball_globals.ball_spawn_timer[ball_index] = spawn_delay;
		}
	}
	else
	{
		long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;

		for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
		{
			oddball_globals.ball_spawn_timer[ball_index] = 0;
			create_the_ball(ball_index);
		}
	}

	return TRUE;
}

static void oddball_engine_player_update(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	long ball_count;

	game_engine_state_message(player_index, NONE, NONE);
	update_ball_ownership(player_index);
	ball_count = player_ball_count(player_index);

	player->speed_multiplier = 1.0f;
	if (ball_count > 0)
	{
		if (game_engine_get_variant()->game_engine_variant.oddball.trait_with_ball != _game_trait_invisible)
			game_engine_player_depower_active_camo(player_index);

		switch (game_engine_get_variant()->game_engine_variant.oddball.speed_with_ball)
		{
		case _oddball_speed_normal:
			player->speed_multiplier = 1.0f;
			break;
		case _oddball_speed_faster:
			player->speed_multiplier = 1.25f;
			break;
		default:
			player->speed_multiplier = 0.75f;
			break;
		}
	}

	if (game_engine_can_score() && accumulate_score_by_time() && ball_count > 0)
	{
		long ball_index;

		for (ball_index = 0; ball_index < ball_count; ball_index++)
			oddball_add_time_with_ball(player_index);
	}

	if (oddball_ball_transfer_by_killing() && ball_count > 0)
	{
		game_engine_state_message(
			player_index,
			_oddball_message_you_are_it,
			player_index);
	}

	if (player->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);

		if (unit->unit.current_weapon_index != NONE)
		{
			long weapon_index = unit->unit.weapon_object_indices[unit->unit.current_weapon_index];

			if (weapon_index != NONE && weapon_is_flag(weapon_index))
			{
				long *score = &oddball_globals.individual_score[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];
				short rounds_loaded = (short)(*score / TICKS_PER_SECOND);
				struct weapon_datum *weapon = weapon_get(weapon_index);

				if (*score > 0 &&
					*score % ODDBALL_SCORE_WARNING_TICKS == 0 &&
					*score < oddball_globals.score_to_win)
				{
					game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_timer_end);
				}

				weapon->weapon.magazines[0].rounds_loaded = rounds_loaded;
			}
		}
	}

	return;
}

static void oddball_engine_weapon_update(
	long weapon_index,
	struct weapon_datum *weapon)
{
	real_point3d position;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
		0x253,
		weapon_is_flag(weapon_index));

	item_get_position_even_if_in_inventory(weapon_index, &position);
	game_engine_set_goal_position(
		weapon->object.owner_team_index,
		&position,
		0.0f,
		"ball_blue",
		NONE,
		NONE,
		oddball_globals.current_ball_owner[weapon->object.owner_team_index]);

	if ((unsigned long)(game_time_get() - weapon->item.last_owned_time) > ODDBALL_UNTOUCHED_RESET_TICKS &&
		weapon_is_flag(weapon_index))
	{
		boolean connected_to_map = TEST_FLAG(weapon->object.flags, _object_connected_to_map_bit);

		if (connected_to_map && weapon->object.parent_object_index == NONE)
		{
			if (TEST_FLAG(weapon->weapon.flags, _oddball_weapon_touched_bit))
				game_show_score(NONE, _oddball_message_ball_was_reset);

			reset_ball(weapon_index);
		}
	}

	return;
}

static boolean oddball_weapon_pickup(
	long weapon_index,
	long player_index)
{
	boolean allow_pick_up = TRUE;
	struct weapon_datum *weapon = weapon_get(weapon_index);

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
		0x3AE,
		weapon_is_flag(weapon_index));

	if (oddball_ball_transfer_by_killing())
	{
		game_show_score_you_ally_enemy(
			player_index,
			_oddball_message_you_have_the_ball,
			_oddball_message_ally_has_the_ball,
			_oddball_message_enemy_has_the_ball,
			player_index);
	}
	else
	{
		long unit_index = player_get(player_index)->unit_index;

		if (unit_index != NONE)
		{
			allow_pick_up = !unit_has_weapon_with_flag(unit_index, _weapon_must_be_readied_bit);
			if (allow_pick_up)
				SET_FLAG(weapon->weapon.flags, _oddball_weapon_touched_bit, TRUE);
		}
	}

	return allow_pick_up;
}

static void oddball_engine_update(
	void)
{
	long ball_spawn_count;
	long ball_index;

	if (game_time_get() == ODDBALL_START_ANNOUNCEMENT_TICKS)
	{
		game_engine_play_multiplayer_sound(
			game_engine_has_teams() ?
				_multiplayer_sound_team_oddball :
				_multiplayer_sound_oddball);
	}

	ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
	for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
	{
		if (oddball_globals.ball_spawn_timer[ball_index] > 0)
		{
			if (--oddball_globals.ball_spawn_timer[ball_index] == 0)
			{
				game_engine_play_multiplayer_sound(_multiplayer_sound_play_ball);
				create_the_ball(ball_index);
			}
		}
	}

	if (oddball_ball_transfer_by_killing())
	{
		for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
		{
			long owner_player_index = oddball_globals.current_ball_owner[ball_index];

			if (owner_player_index == NONE)
			{
				game_engine_clear_goal_position((short)ball_index);
			}
			else
			{
				long unit_index = player_get(owner_player_index)->unit_index;

				if (unit_index != NONE)
				{
					struct unit_datum *unit = unit_get(unit_index);

					game_engine_set_goal_position(
						(short)ball_index,
						&unit->object.bounding_sphere_center,
						0.0f,
						"target_blue",
						NONE,
						NONE,
						owner_player_index);
				}
			}
		}
	}

	return;
}

static long oddball_get_score(
	long player_index,
	enum get_score_type score_type)
{
	struct player_datum *player = player_get(player_index);
	long score;

	if (score_type == _get_score_team)
		score = oddball_globals.team_score[player->team_index];
	else
		score = oddball_globals.individual_score[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];

	return score;
}

static wchar_t *oddball_get_score_string(
	long player_index,
	wchar_t *buffer)
{
	long score = oddball_globals.individual_score[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];

	if (terminator_scoring_rules())
		usprintf(buffer, L"%d", score);
	else
		ticks_to_unicode_time_string(score, 256, buffer);

	return buffer;
}

static wchar_t *oddball_get_score_header_string(
	wchar_t *buffer)
{
	short string_index;
	long string_list_index;

	if (terminator_scoring_rules())
		string_index = _multiplayer_game_text_score;
	else
		string_index = _multiplayer_game_text_time;

	string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	ustrcpy(
		buffer,
		string_list_index != NONE ?
			unicode_string_list_get_string(string_list_index, string_index) :
			L"");

	return buffer;
}

static wchar_t *oddball_get_team_score_string(
	long team_index,
	wchar_t *buffer)
{
	long score = oddball_globals.team_score[team_index];

	if (terminator_scoring_rules())
		usprintf(buffer, L"%d", score);
	else
		ticks_to_unicode_time_string(score, 256, buffer);

	return buffer;
}

static void oddball_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire)
{
	if (oddball_ball_transfer_by_killing())
	{
		long ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;
		long capture_index = NONE;
		long ball_index;

		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
			0x2F3,
			dead_player_index != NONE);

		if (killing_player_index != NONE && !friendly_fire)
		{
			struct player_datum *killer = player_get(killing_player_index);

			if (player_has_ball(dead_player_index))
			{
				killer->statistics.multiplayer_statistics.oddball_statistics.ball_carrier_kills++;
				if (terminator_scoring_rules() && game_engine_can_score())
					oddball_add_score(killing_player_index);
			}
			else if (player_has_ball(killing_player_index))
			{
				killer->statistics.multiplayer_statistics.oddball_statistics.kills_as_ball_carrier++;
				if (terminator_scoring_rules() && game_engine_can_score())
					oddball_add_score(killing_player_index);
			}
			else if (ball_available())
			{
				oddball_add_score(killing_player_index);
			}

			if (killer->unit_index != NONE)
			{
				for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
				{
					if (oddball_globals.ball_spawn_timer[ball_index] == 0 &&
						capture_index == NONE &&
						oddball_globals.current_ball_owner[ball_index] == NONE)
					{
						capture_index = ball_index;
					}

					if (oddball_globals.current_ball_owner[ball_index] == dead_player_index)
					{
						capture_index = ball_index;
						break;
					}
				}

				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine_oddball.c",
					0x326,
					!ball_available() || (capture_index != NONE));

				if (capture_index != NONE)
				{
					/* January and HCEA both retain this mode re-query inside
					 * the already transfer-only arm.
					 */
					game_show_score_you_ally_enemy(
						killing_player_index,
						oddball_ball_transfer_by_killing() ? NONE : _oddball_message_you_are_it,
						_oddball_message_ally_is_it,
						_oddball_message_enemy_is_it,
						killing_player_index);
					oddball_globals.current_ball_owner[capture_index] = killing_player_index;
				}
			}
		}

		for (ball_index = 0; ball_index < ball_spawn_count; ball_index++)
		{
			if (oddball_globals.current_ball_owner[ball_index] == dead_player_index)
				oddball_globals.current_ball_owner[ball_index] = NONE;
		}
	}

	return;
}

static boolean oddball_test_flag(
	long flag)
{
	boolean result = FALSE;

	switch (flag)
	{
	case _game_engine_test_flag_rasterize_score:
		result = terminator_scoring_rules();
		break;
	}

	return result;
}

static boolean oddball_test_trait(
	long player_index,
	long trait)
{
	boolean result = FALSE;

	if (trait != _game_trait_none)
	{
		if (player_has_ball(player_index))
			result = trait == game_engine_get_variant()->game_engine_variant.oddball.trait_with_ball;
		else
			result = trait == game_engine_get_variant()->game_engine_variant.oddball.trait_without_ball;
	}

	return result;
}

static boolean oddball_engine_display_score(
	long player_index,
	long message,
	long message_player_index,
	wchar_t *buffer,
	long buffer_size)
{
	struct player_datum *player = player_get(player_index);
	boolean result = TRUE;
	long string_list_index;
	wchar_t *string;

	switch (message)
	{
	case _oddball_message_you_are_it:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_you_are_it);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _oddball_message_ally_is_it:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_ally_is_it);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _oddball_message_enemy_is_it:
		{
			struct player_datum *other_player = player_get(message_player_index);

			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_enemy_is_it);
			}
			else
				string = L"";
			usnprintf(buffer, buffer_size, string, other_player->name);
		}
		break;

	case _oddball_message_you_have_the_ball:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_you_have_the_ball);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _oddball_message_ally_has_the_ball:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_ally_has_the_ball);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _oddball_message_enemy_has_the_ball:
		{
			struct player_datum *other_player = player_get(message_player_index);

			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_enemy_has_the_ball);
			}
			else
				string = L"";
			usnprintf(buffer, buffer_size, string, other_player->name);
		}
		break;

	case _oddball_message_enemy_has_the_ball_tick:
	case _oddball_message_ally_has_the_ball_tick:
	case _oddball_message_you_have_the_ball_tick:
		{
			struct player_datum *other_player = player_get(message_player_index);
			long score = oddball_globals.team_score[other_player->team_index] / TICKS_PER_SECOND;

			if (message == _oddball_message_you_have_the_ball_tick)
			{
				wchar_t *place_name = get_place_name(
					game_engine_get_place(player_index, _get_score_team));

				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_multiplayer_game_text_place_score);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, place_name, score);
			}
			else if (message == _oddball_message_ally_has_the_ball_tick)
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_multiplayer_game_text_ally_has_the_ball_score);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, other_player->name, score);
			}
			else if (message == _oddball_message_enemy_has_the_ball_tick)
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_multiplayer_game_text_enemy_has_the_ball_score);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, other_player->name, score);
			}
		}
		break;

	default:
		result = FALSE;
		break;
	}

	return result;
}

/* ---------- engine table */

struct game_engine oddball_engine =
{
	"oddball",
	game_engine_oddball,
	oddball_engine_dispose,
	oddball_engine_initialize_for_new_map,
	oddball_engine_dispose_from_old_map,
	oddball_engine_player_added,
	oddball_engine_game_ending,
	oddball_engine_game_starting,
	oddball_engine_statistics_append,
	oddball_engine_handle_client_message,
	oddball_engine_handle_server_message,
	oddball_engine_pregame_post_rasterize,
	oddball_engine_post_rasterize,
	oddball_engine_player_update,
	oddball_engine_weapon_update,
	oddball_weapon_pickup,
	oddball_weapon_drop,
	oddball_engine_update,
	oddball_get_score,
	oddball_get_score_string,
	oddball_get_score_header_string,
	oddball_get_team_score_string,
	NULL,
	oddball_engine_player_damaged_player,
	oddball_engine_player_killed_player,
	oddball_engine_display_score,
	NULL,
	oddball_engine_prespawn_player_update,
	NULL,
	NULL,
	NULL,
	oddball_test_flag,
	oddball_test_trait,
	NULL,
};

#ifdef HALO_LINUX
/* the parts of oddball_globals that are the same on every machine */
struct oddball_network_state
{
	long team_score[NUMBEROF(oddball_globals.team_score)];
	long individual_score[NUMBEROF(oddball_globals.individual_score)];
	long ball_spawn_timer[MAXIMUM_ODDBALLS];
	/* the balls' carriers, by absolute index (their datum identifiers are
	each machine's own) */
	byte current_ball_owner[MAXIMUM_ODDBALLS];
};

/* port/linux/game/network_distributed.c's */
byte distributed_player_to_byte(long player_index);
long distributed_player_from_byte(byte player_index);

/* the distributed netcode (port/linux/game/network_distributed.c): the game
type's state the host sends its clients, which take it as it is */
long game_engine_oddball_write_network_state(
	byte *buffer,
	long size)
{
	struct oddball_network_state state;

	short ball_index;

	if (size < (long)sizeof(state))
		return 0;
	csmemset(&state, 0, sizeof(state));
	for (ball_index = 0; ball_index < MAXIMUM_ODDBALLS; ball_index++)
		state.current_ball_owner[ball_index] = distributed_player_to_byte(oddball_globals.current_ball_owner[ball_index]);
	csmemcpy(state.team_score, oddball_globals.team_score, sizeof(state.team_score));
	csmemcpy(state.individual_score, oddball_globals.individual_score, sizeof(state.individual_score));
	csmemcpy(state.ball_spawn_timer, oddball_globals.ball_spawn_timer, sizeof(state.ball_spawn_timer));
	csmemcpy(buffer, &state, sizeof(state));
	return sizeof(state);
}

void game_engine_oddball_read_network_state(
	byte const *buffer,
	long size)
{
	struct oddball_network_state state;
	short ball_index;

	if (size != (long)sizeof(state))
		return;
	csmemcpy(&state, buffer, sizeof(state));
	for (ball_index = 0; ball_index < MAXIMUM_ODDBALLS; ball_index++)
		oddball_globals.current_ball_owner[ball_index] = distributed_player_from_byte(state.current_ball_owner[ball_index]);
	csmemcpy(oddball_globals.team_score, state.team_score, sizeof(state.team_score));
	csmemcpy(oddball_globals.individual_score, state.individual_score, sizeof(state.individual_score));
	csmemcpy(oddball_globals.ball_spawn_timer, state.ball_spawn_timer, sizeof(state.ball_spawn_timer));
}
#endif
