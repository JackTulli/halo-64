/*
GAME_ENGINE_RACE.C

symbols in this file:
000A27A0 0010:
	_race_engine_dispose (0000)
000A27B0 0080:
	_delete_race_vehicles (0000)
000A2830 00f0:
	_race_get_vehicle_to_spawn (0000)
000A2920 0090:
	_race_flags_make_unique (0000)
000A29B0 0010:
	_race_engine_dispose_from_old_map (0000)
000A29C0 0030:
	_race_engine_player_added (0000)
000A29F0 0010:
	_race_engine_game_ending (0000)
000A2A00 0010:
	_race_engine_game_starting (0000)
000A2A10 0010:
	_race_engine_statistics_append (0000)
000A2A20 0010:
	_race_engine_handle_client_message (0000)
000A2A30 0010:
	_race_engine_handle_server_message (0000)
000A2A40 0010:
	_race_engine_pregame_post_rasterize (0000)
000A2A50 0010:
	_race_engine_post_rasterize (0000)
000A2A60 0190:
	_race_complete_lap (0000)
000A2BF0 0120:
	_can_touch_team (0000)
000A2D10 0010:
	_race_engine_weapon_update (0000)
000A2D20 0090:
	_race_team_can_win_game (0000)
000A2DB0 00e0:
	_build_player_speeds (0000)
000A2E90 0010:
	_race_engine_player_damaged_player (0000)
000A2EA0 0010:
	_race_engine_player_killed_player (0000)
000A2EB0 0480:
	_race_engine_display_score (0000)
000A3330 0010:
	_race_engine_prespawn_player_update (0000)
000A3340 0060:
	_race_goal_matches_player (0000)
000A33A0 0020:
	_count_bits_32 (0000)
000A33C0 00e0:
	_race_engine_get_score (0000)
000A34A0 0040:
	_race_get_score_string (0000)
000A34E0 0070:
	_race_get_score_header_string (0000)
000A3550 0030:
	_race_get_team_score_string (0000)
000A3580 00b0:
	_race_engine_did_player_win (0000)
000A3630 00e0:
	_find_closest_vehicle (0000)
000A3710 0130:
	_spawn_race_vehicles (0000)
000A3840 00f0:
	_new_rally_flag (0000)
000A3930 0150:
	_race_touch_flag (0000)
000A3A80 00a0:
	_race_engine_player_update (0000)
000A3B20 00c0:
	_race_engine_update (0000)
000A3BE0 0110:
	_race_engine_initialize_for_new_map (0000)
0025BFD4 0027:
	??_C@_0CH@MLHAJCND@c?3?2halo?2SOURCE?2game?2game_engine_@ (0000)
0025BFFC 0019:
	??_C@_0BJ@BIAKIHCM@itr?5?$DM?5MAXIMUM_RACE_FLAGS?$AA@ (0000)
0025C018 0036:
	??_C@_0DG@CFFMMMPN@?$CB?$CCrace?5goal?5matches?5player?5calle@ (0000)
0025C050 0011:
	??_C@_0BB@BCKDFNEI@?$CBcan_team_win?$FL1?$FN?$AA@ (0000)
0025C064 0011:
	??_C@_0BB@CAKPLNGL@new_flag?5?$CB?$DN?5NONE?$AA@ (0000)
0025C078 000a:
	??_C@_09DMMLFBJB@count?5?$DO?50?$AA@ (0000)
0025C084 0037:
	??_C@_0DH@EODBECII@?$CB?$CI?$CKlap_bit_vector?5?$CG?5?$HOrace_global@ (0000)
0025C0BC 0028:
	??_C@_0CI@ONDKDHNM@?$CBTEST_FLAG?$CI?$CKlap_bit_vector?0?5team@ (0000)
0025C0E8 0053:
	??_C@_0FD@CPKHPGHJ@_race_type_normal?5?$CB?$DN?5game_engine@ (0000)
0025C140 0050:
	??_C@_0FA@IHEEDPIP@one?5of?5the?5netgameflags?5that?5def@ (0000)
002DE5E8 0088:
	_race_engine (0000)
0043ECB0 00d0:
	_race_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game_engine_place.h"
#include "game_engine_race.h"
#include "game_globals.h"
#include "players.h"
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
	MAXIMUM_RACE_FLAGS = 32,
	MAXIMUM_RACE_VEHICLES = 8,
	MAXIMUM_RACE_VEHICLES_TO_DELETE = 32,
#ifdef HALO_LINUX
	/* port: the native builds' session limit (halo_port_limits.h) */
	MULTIPLAYER_MAXIMUM_PLAYERS = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
#else
	MULTIPLAYER_MAXIMUM_PLAYERS = 16,
#endif
};

enum race_type
{
	_race_type_normal = 0,
	_race_type_any_order,
	_race_type_flag_rally,
	NUMBER_OF_RACE_TYPES
};

enum race_team_scoring
{
	_race_team_scoring_minimum = 0,
	_race_team_scoring_maximum,
	_race_team_scoring_sum,
	NUMBER_OF_RACE_TEAM_SCORING_TYPES
};

enum
{
	_game_engine_vehicles_default = 0,
	_game_engine_vehicles_none,
	_game_engine_vehicles_warthog,
	_game_engine_vehicles_ghost,
	_game_engine_vehicles_tank,
};

enum
{
	_multiplayer_vehicle_warthog = 0,
	_multiplayer_vehicle_ghost,
	_multiplayer_vehicle_scorpion,
};

enum
{
	_multiplayer_sound_race = 0x14,
	_multiplayer_sound_warthog = 0x17,
	_multiplayer_sound_ghost = 0x18,
	_multiplayer_sound_scorpion = 0x19,
	_multiplayer_sound_countdown_timer = 0x1A,
	_multiplayer_sound_team_race = 0x22,
	_multiplayer_sound_countdown_timer_end = 0x2A,
};

enum
{
	_race_message_show_score = 0x16,
	_race_message_you_completed_a_lap = 0x1E,
	_race_message_ally_completed_a_lap,
	_race_message_enemy_completed_a_lap,
	_race_message_you_touched_a_flag_rally,
	_race_message_ally_touched_a_flag_rally,
	_race_message_enemy_touched_a_flag_rally,
	_race_message_new_best_lap_time,
};

/* ui\multiplayer_game_text string indices */
enum multiplayer_game_text
{
	_multiplayer_game_text_you_scored_a_flag = 0xA7,
	_multiplayer_game_text_ally_name_scored_a_flag,
	_multiplayer_game_text_enemy_name_scored_a_flag,
	_multiplayer_game_text_you_completed_lap_n_in_x_seconds,
	_multiplayer_game_text_ally_name_completed_a_lap_n,
	_multiplayer_game_text_enemy_name_completed_a_lap,
	_multiplayer_game_text_new_best_lap_time_x,
	_multiplayer_game_text_name_1_flag,
	_multiplayer_game_text_name_n_flags,
	_multiplayer_game_text_name_all_laps_complete,
	_multiplayer_game_text_name_lap_n_of_total,
};

/* ---------- macros */


/* ---------- structures */

typedef char verify_scenario_netgame_flag_size[
	sizeof(struct scenario_netgame_flag) == 0x94 ? 1 : -1];

struct race_globals
{
	unsigned long lap_completed_value;
	long first_flag[MULTIPLAYER_MAXIMUM_PLAYERS];
	long lap_bit_vector[MULTIPLAYER_MAXIMUM_PLAYERS];
	long rally_flag;
	long team_laps[MULTIPLAYER_MAXIMUM_PLAYERS];
	long persistent_team_score;
	boolean vehicles_have_been_added;
	byte padCD[3];
};

/* January's layout; the port's per-player arrays are larger */
#ifndef HALO_LINUX
typedef char verify_race_globals_size[
	sizeof(struct race_globals) == 0xD0 ? 1 : -1];
typedef char verify_race_globals_vehicles_have_been_added_offset[
	offsetof(struct race_globals, vehicles_have_been_added) == 0xCC ? 1 : -1];
#endif

/* ---------- prototypes */

static void delete_race_vehicles(
	void);
static long race_get_vehicle_to_spawn(
	long vehicle_number);
static void race_complete_lap(
	long player_index);
static boolean can_touch_team(
	long player_index,
	long team_index);
static long new_rally_flag(
	long old_flag);
static void race_touch_flag(
	long player_index,
	long netgame_flag_index);
static boolean race_team_can_win_game(
	long team_index);
static void build_player_speeds(
	void);
static long count_bits_32(
	unsigned long flags);
static long find_closest_vehicle(
	real_point3d const *point,
	long *ignore_these,
	long ignore_these_count);
static void spawn_race_vehicles(
	void);

/* ---------- globals */

extern long timeout_for_endgame_sound;

struct race_globals race_globals = { 0 };

/* ---------- public code */

void race_engine_dispose(
	void)
{
	return;
}

static void delete_race_vehicles(
	void)
{
	struct object_iterator iterator;
	long vehicle_indices[MAXIMUM_RACE_VEHICLES_TO_DELETE];
	long vehicle_count = 0;
	long itr;

	object_iterator_new(&iterator, _object_mask_vehicle, 0);
	while (object_iterator_next(&iterator))
	{
		if (vehicle_count < MAXIMUM_RACE_VEHICLES_TO_DELETE)
			vehicle_indices[vehicle_count++] = iterator.index;
	}

	for (itr = 0; itr < vehicle_count; itr++)
		object_delete(vehicle_indices[itr]);

	return;
}

static long race_get_vehicle_to_spawn(
	long vehicle_number)
{
	struct scenario *scenario = global_scenario_get();
	struct game_globals *game_globals = scenario_get_game_globals();
	struct game_globals_multiplayer_information *multiplayer_information =
		TAG_BLOCK_GET_ELEMENT(
			&game_globals->multiplayer_information,
			0,
			struct game_globals_multiplayer_information);
	struct game_globals_vehicle *warthog = TAG_BLOCK_GET_ELEMENT(
		&multiplayer_information->vehicles,
		_multiplayer_vehicle_warthog,
		struct game_globals_vehicle);
	struct game_globals_vehicle *ghost = TAG_BLOCK_GET_ELEMENT(
		&multiplayer_information->vehicles,
		_multiplayer_vehicle_ghost,
		struct game_globals_vehicle);
	struct game_globals_vehicle *scorpion = TAG_BLOCK_GET_ELEMENT(
		&multiplayer_information->vehicles,
		_multiplayer_vehicle_scorpion,
		struct game_globals_vehicle);
	long vehicle_definition_index = NONE;

	switch (game_engine_get_variant()->universal_variant.vehicle_set)
	{
	case _game_engine_vehicles_default:
		if (vehicle_number == 0)
			vehicle_definition_index = warthog->vehicle.index;
		else if (vehicle_number == 1)
			vehicle_definition_index = scorpion->vehicle.index;
		else if (vehicle_number < 6)
			vehicle_definition_index = ghost->vehicle.index;
		break;
	case _game_engine_vehicles_none:
		break;
	case _game_engine_vehicles_warthog:
		vehicle_definition_index = vehicle_number < 4 ? warthog->vehicle.index : NONE;
		break;
	case _game_engine_vehicles_ghost:
		vehicle_definition_index = vehicle_number < 8 ? ghost->vehicle.index : NONE;
		break;
	case _game_engine_vehicles_tank:
		vehicle_definition_index = vehicle_number < 4 ? scorpion->vehicle.index : NONE;
		break;
	}

	return vehicle_definition_index;
}

void race_flags_make_unique(
	void)
{
	struct scenario *scenario = global_scenario_get();
	unsigned long used_flags = 0;
	short itr;

	for (itr = 0; itr < scenario->netgame_flags.count; itr++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			itr,
			struct scenario_netgame_flag);

		if (flag->type == _netgame_flag_race_track &&
			flag->team_index >= 0 &&
			flag->team_index < MAXIMUM_RACE_FLAGS)
		{
			if (TEST_FLAG(used_flags, flag->team_index))
			{
				long free_index;

				for (free_index = 0; free_index < MAXIMUM_RACE_FLAGS; free_index++)
				{
					if (!TEST_FLAG(used_flags, free_index))
					{
						/* BUG (original): the flag that was already in use is
						   marked again rather than the newly assigned index. */
						SET_FLAG(used_flags, flag->team_index, TRUE);
						break;
					}
				}
				flag->team_index = (short)free_index;
			}
			else
			{
				SET_FLAG(used_flags, flag->team_index, TRUE);
			}
		}
	}

	return;
}

void race_engine_dispose_from_old_map(
	void)
{
	return;
}

void race_engine_player_added(
	long player_index)
{
	player_get(player_index)->multiplayer_special = 0;

	return;
}

void race_engine_game_ending(
	void)
{
	return;
}

void race_engine_game_starting(
	void)
{
	return;
}

void race_engine_statistics_append(
	long statistic)
{
	return;
}

void race_engine_handle_client_message(
	void *message)
{
	return;
}

void race_engine_handle_server_message(
	void *message)
{
	return;
}

void race_engine_pregame_post_rasterize(
	void)
{
	return;
}

void race_engine_post_rasterize(
	void)
{
	return;
}

static void race_complete_lap(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	long lap_time = game_time_get() - player->multiplayer_special;
	struct data_iterator iterator;
	struct player_datum *team_player;
	long team_score;

	race_globals.lap_bit_vector[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)] = 0;
	game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_timer_end);
	player->statistics.multiplayer_statistics.race_statistics.last_lap_time = (short)lap_time;

	if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
	{
		game_show_score_you_ally_enemy(
			player_index,
			_race_message_you_touched_a_flag_rally,
			_race_message_ally_touched_a_flag_rally,
			_race_message_enemy_completed_a_lap,
			player_index);
	}
	else
	{
		game_show_score_you_ally_enemy(
			player_index,
			_race_message_you_completed_a_lap,
			_race_message_ally_completed_a_lap,
			_race_message_enemy_completed_a_lap,
			player_index);
	}

	if (player->statistics.multiplayer_statistics.race_statistics.laps == 0)
	{
		player->statistics.multiplayer_statistics.race_statistics.best_lap_time = (short)lap_time;
	}
	else if (lap_time < player->statistics.multiplayer_statistics.race_statistics.best_lap_time)
	{
		player->statistics.multiplayer_statistics.race_statistics.best_lap_time = (short)lap_time;
		if (game_engine_get_variant()->game_engine_variant.race.race_type != _race_type_flag_rally)
		{
			game_show_score_extended(
				player_index,
				_race_message_new_best_lap_time,
				player_index);
		}
	}

	player->statistics.multiplayer_statistics.race_statistics.laps++;
	player->multiplayer_special = game_time_get();

	team_score = player->statistics.multiplayer_statistics.race_statistics.laps;
	data_iterator_new(&iterator, player_data);
	if (game_engine_get_variant()->game_engine_variant.race.team_scoring == _race_team_scoring_sum)
		team_score = 0;
	while ((team_player = data_iterator_next(&iterator)))
	{
		if (player->team_index == team_player->team_index)
		{
			long laps = team_player->statistics.multiplayer_statistics.race_statistics.laps;

			switch (game_engine_get_variant()->game_engine_variant.race.team_scoring)
			{
			case _race_team_scoring_minimum:
				team_score = MIN(team_score, laps);
				break;
			case _race_team_scoring_maximum:
				team_score = MAX(team_score, laps);
				break;
			case _race_team_scoring_sum:
				team_score += laps;
				break;
			default:
				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine_race.c",
					0x248,
					!"unreachable");
				break;
			}
		}
	}

	if (team_score > race_globals.team_laps[player->team_index])
		race_globals.team_laps[player->team_index] = team_score;
	if (race_globals.team_laps[player->team_index] >= game_engine_get_variant()->universal_variant.score_to_win)
		game_engine_end_game();

	return;
}

static boolean can_touch_team(
	long player_index,
	long team_index)
{
	struct player_datum *player = player_get(player_index);
	long absolute_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	unsigned long untouched_flags = race_globals.lap_completed_value &
		~race_globals.lap_bit_vector[absolute_player_index];
	boolean can_touch = TRUE;

	if (team_index >= MAXIMUM_RACE_FLAGS)
	{
		can_touch = FALSE;
	}
	else if (player->statistics.multiplayer_statistics.race_statistics.laps >=
		game_engine_get_variant()->universal_variant.score_to_win)
	{
		can_touch = FALSE;
	}
	else if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
	{
		can_touch = race_globals.rally_flag == team_index;
	}
	else if (race_globals.lap_bit_vector[absolute_player_index] == race_globals.lap_completed_value)
	{
		can_touch = team_index == race_globals.first_flag[absolute_player_index];
	}
	else if (!TEST_FLAG(untouched_flags, team_index))
	{
		can_touch = FALSE;
	}
	else if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_normal)
	{
		long itr;

		for (itr = 0; itr < MAXIMUM_RACE_FLAGS; itr++)
		{
			if (itr == team_index)
			{
				can_touch = TRUE;
				break;
			}
			if (TEST_FLAG(untouched_flags, itr))
			{
				can_touch = FALSE;
				break;
			}
		}
		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine_race.c",
			0x28A,
			itr < MAXIMUM_RACE_FLAGS);
	}
	else
	{
		can_touch = TRUE;
	}

	return can_touch;
}

static long new_rally_flag(
	long old_flag)
{
	long new_flag = NONE;
	struct scenario *scenario = global_scenario_get();
	long count = count_bits_32(race_globals.lap_completed_value);
	long random_index;
	long itr;

	if (old_flag != NONE)
		count--;
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_race.c",
		0x2A8,
		count > 0);
	random_index = random_range(0, count);

	for (itr = 0; itr < scenario->netgame_flags.count; itr++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			itr,
			struct scenario_netgame_flag);

		if (flag->type == _netgame_flag_race_track && flag->team_index != old_flag)
		{
			if (random_index == 0)
			{
				new_flag = flag->team_index;
				break;
			}
			random_index--;
		}
	}
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_race.c",
		0x2BD,
		new_flag != NONE);

	return new_flag;
}

static void race_touch_flag(
	long player_index,
	long netgame_flag_index)
{
	struct scenario *scenario = global_scenario_get();
	struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
		&scenario->netgame_flags,
		netgame_flag_index,
		struct scenario_netgame_flag);
	long team_index = flag->team_index;

	if (can_touch_team(player_index, team_index))
	{
		long absolute_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
		long *lap_bit_vector = &race_globals.lap_bit_vector[absolute_player_index];

		game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_timer);

		if (race_globals.first_flag[absolute_player_index] == NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_race.c",
				0x2D4,
				_race_type_normal != game_engine_get_variant()->game_engine_variant.race.race_type);
			race_globals.first_flag[absolute_player_index] = flag->team_index;
		}

		if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
		{
			race_complete_lap(player_index);
			race_globals.rally_flag = new_rally_flag(race_globals.rally_flag);
		}
		else if (*lap_bit_vector == race_globals.lap_completed_value)
		{
			race_complete_lap(player_index);
		}
		else
		{
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_race.c",
				0x2EA,
				!TEST_FLAG(*lap_bit_vector, team_index));
			SET_FLAG(*lap_bit_vector, team_index, TRUE);
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_race.c",
				0x2EE,
				!(*lap_bit_vector & ~race_globals.lap_completed_value));
		}
	}

	return;
}

void race_engine_weapon_update(
	long item_index,
	struct weapon_datum *weapon)
{
	return;
}

static boolean race_team_can_win_game(
	long team_index)
{
	boolean can_win = TRUE;

	if (game_engine_get_variant()->game_engine_variant.race.team_scoring == _race_team_scoring_minimum)
	{
		struct data_iterator iterator;
		struct player_datum *player;

		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)))
		{
			if (player->team_index == team_index &&
				player->statistics.multiplayer_statistics.race_statistics.laps <
					game_engine_get_variant()->universal_variant.score_to_win &&
				(game_engine_player_is_out_of_lives(iterator.datum_index) ||
					player->quit_out_of_game))
			{
				can_win = FALSE;
			}
		}
	}

	return can_win;
}

static void build_player_speeds(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long maximum_laps = 0;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)))
	{
		maximum_laps = MAX(
			maximum_laps,
			player->statistics.multiplayer_statistics.race_statistics.laps);
	}

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)))
	{
		real speed_multiplier = 1.0f;
		long laps_behind = maximum_laps -
			player->statistics.multiplayer_statistics.race_statistics.laps;

		if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
			laps_behind /= 3;

		if (laps_behind >= 2)
			speed_multiplier = 1.2f;
		else if (laps_behind >= 1)
			speed_multiplier = 1.1f;

		player->speed_multiplier = speed_multiplier;
	}

	return;
}

void race_engine_player_damaged_player(
	long killing_player_index,
	long dead_player_index,
	boolean friendly_fire)
{
	return;
}

void race_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire)
{
	return;
}

boolean race_engine_display_score(
	long player_index,
	long message,
	long message_player_index,
	wchar_t *buffer,
	long buffer_size)
{
	boolean result = TRUE;
	struct player_datum *other_player = NULL;
	long string_list_index;
	wchar_t *string;

	switch (message)
	{
	case _race_message_show_score:
	case _race_message_you_completed_a_lap:
	case _race_message_ally_completed_a_lap:
	case _race_message_enemy_completed_a_lap:
	case _race_message_ally_touched_a_flag_rally:
	case _race_message_enemy_touched_a_flag_rally:
	case _race_message_new_best_lap_time:
		other_player = player_get(message_player_index);
		break;

	default:
		break;
	}

	switch (message)
	{
	case _race_message_you_touched_a_flag_rally:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_you_scored_a_flag);
		}
		else
			string = L"";
		ustrncpy(buffer, string, buffer_size);
		break;

	case _race_message_ally_touched_a_flag_rally:
		{
			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_ally_name_scored_a_flag);
			}
			else
				string = L"";
			usnprintf(buffer, buffer_size, string, other_player->name);
		}
		break;

	case _race_message_enemy_touched_a_flag_rally:
		{
			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_enemy_name_scored_a_flag);
			}
			else
				string = L"";
			usnprintf(buffer, buffer_size, string, other_player->name);
		}
		break;

	case _race_message_you_completed_a_lap:
		{
			real lap_time =
				(real)player_get(message_player_index)->statistics.multiplayer_statistics.race_statistics.last_lap_time /
				TICKS_PER_SECOND;

			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_you_completed_lap_n_in_x_seconds);
			}
			else
				string = L"";
			usnprintf(
				buffer,
				buffer_size,
				string,
				other_player->statistics.multiplayer_statistics.race_statistics.laps + 1,
				lap_time);
		}
		break;

	case _race_message_ally_completed_a_lap:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_ally_name_completed_a_lap_n);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			other_player->name,
			other_player->statistics.multiplayer_statistics.race_statistics.laps + 1);
		break;

	case _race_message_enemy_completed_a_lap:
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
		{
			string = unicode_string_list_get_string(
				string_list_index,
				_multiplayer_game_text_enemy_name_completed_a_lap);
		}
		else
			string = L"";
		usnprintf(
			buffer,
			buffer_size,
			string,
			other_player->name,
			other_player->statistics.multiplayer_statistics.race_statistics.laps);
		break;

	case _race_message_new_best_lap_time:
		{
			real best_lap_time =
				(real)player_get(message_player_index)->statistics.multiplayer_statistics.race_statistics.best_lap_time /
				TICKS_PER_SECOND;

			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_new_best_lap_time_x);
			}
			else
				string = L"";
			usnprintf(buffer, buffer_size, string, best_lap_time);
		}
		break;

	case _race_message_show_score:
		if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
		{
			if (other_player->statistics.multiplayer_statistics.race_statistics.laps == 1)
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_multiplayer_game_text_name_1_flag);
				}
				else
					string = L"";
				usnprintf(
					buffer,
					buffer_size,
					string,
					get_place_name(game_engine_get_place(player_index, _get_score_team)));
			}
			else
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_multiplayer_game_text_name_n_flags);
				}
				else
					string = L"";
				usnprintf(
					buffer,
					buffer_size,
					string,
					get_place_name(game_engine_get_place(player_index, _get_score_team)),
					other_player->statistics.multiplayer_statistics.race_statistics.laps);
			}
		}
		else if (other_player->statistics.multiplayer_statistics.race_statistics.laps + 1 >
			game_engine_get_variant()->universal_variant.score_to_win)
		{
			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_name_all_laps_complete);
			}
			else
				string = L"";
			usnprintf(
				buffer,
				buffer_size,
				string,
				get_place_name(game_engine_get_place(player_index, _get_score_team)));
		}
		else
		{
			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				string = unicode_string_list_get_string(
					string_list_index,
					_multiplayer_game_text_name_lap_n_of_total);
			}
			else
				string = L"";
			usnprintf(
				buffer,
				buffer_size,
				string,
				get_place_name(game_engine_get_place(player_index, _get_score_team)),
				other_player->statistics.multiplayer_statistics.race_statistics.laps + 1,
				game_engine_get_variant()->universal_variant.score_to_win);
		}
		break;

	default:
		result = FALSE;
		break;
	}

	return result;
}

void race_engine_prespawn_player_update(
	long player_index)
{
	return;
}

boolean race_goal_matches_player(
	long player_index,
	long goal_index)
{
	struct player_datum *player = player_get(player_index);
	boolean result = FALSE;

	if (!TEST_FLAG(race_globals.lap_completed_value, goal_index))
	{
		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine_race.c",
			0x43E,
			!"race goal matches player called on an invalid goal");
	}
	else
	{
		result = can_touch_team(player_index, goal_index);
	}

	return result;
}

static long count_bits_32(
	unsigned long flags)
{
	long count = 0;
	long bit;

	for (bit = 0; bit < LONG_BITS; bit++)
	{
		if (TEST_FLAG(flags, bit))
			count++;
	}

	return count;
}

long race_engine_get_score(
	long player_index,
	enum get_score_type score_type)
{
	struct player_datum *player = player_get(player_index);
	long score;

	if (score_type == _get_score_team)
	{
		score = race_globals.team_laps[player->team_index];
	}
	else
	{
#ifdef HALO_LINUX
		/* port: lap_bit_vector is kept per player; January read it by team
		index, which picks another player's flags */
		long laps = player->statistics.multiplayer_statistics.race_statistics.laps;
		long flags_touched = count_bits_32(
			race_globals.lap_bit_vector[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)]);
#else
		long team_index = player->team_index;
		long laps = player->statistics.multiplayer_statistics.race_statistics.laps;
		long flags_touched = count_bits_32(race_globals.lap_bit_vector[team_index]);
#endif

		score = laps * (MAXIMUM_RACE_FLAGS + 1) + flags_touched;
	}

	return score;
}

wchar_t *race_get_score_string(
	long player_index,
	wchar_t *string)
{
	struct player_datum *player;

	player = player_get(player_index);
	usprintf(
		string,
		L"%d",
		player->statistics.multiplayer_statistics.race_statistics.laps);

	return string;
}

wchar_t *race_get_score_header_string(
	wchar_t *string)
{
	short string_index =
		game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally ?
			178 : 25;
	long string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	wchar_t *header_string;

	if (string_list_index != NONE)
		header_string = unicode_string_list_get_string(string_list_index, string_index);
	else
		header_string = L"";

	ustrcpy(string, header_string);

	return string;
}

wchar_t *race_get_team_score_string(
	long team_index,
	wchar_t *string)
{
	usprintf(string, L"%d", race_globals.team_laps[team_index]);

	return string;
}

long race_engine_did_player_win(
	long player_index)
{
	if (game_engine_has_teams())
	{
		struct player_datum *player = player_get(player_index);
		boolean can_team_win[2];

		can_team_win[0] = race_team_can_win_game(0);
		can_team_win[1] = race_team_can_win_game(1);

		if (can_team_win[0] != can_team_win[1])
			return can_team_win[player->team_index] != FALSE;

		if (!can_team_win[0])
		{
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_race.c",
				0x4AC,
				!can_team_win[1]);

			return NONE;
		}

		return game_engine_did_player_win_default(player_index);
	}

	return game_engine_did_player_win_default(player_index);
}

static long find_closest_vehicle(
	real_point3d const *point,
	long *ignore_these,
	long ignore_these_count)
{
	real closest_distance = 1000000.0f;
	long closest_flag_index = NONE;
	struct scenario *scenario = global_scenario_get();
	struct game_globals *game_globals = scenario_get_game_globals();
	struct game_globals_multiplayer_information *multiplayer_information =
		TAG_BLOCK_GET_ELEMENT(
			&game_globals->multiplayer_information,
			0,
			struct game_globals_multiplayer_information);
	long flag_index;

	for (flag_index = 0; flag_index < scenario->netgame_flags.count; flag_index++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag);

		if (flag->type == _netgame_flag_race_vehicle)
		{
			boolean ignored = FALSE;
			long itr;

			for (itr = 0; itr < ignore_these_count; itr++)
			{
				if (flag_index == ignore_these[itr])
				{
					ignored = TRUE;
					break;
				}
			}

			if (!ignored)
			{
				real distance;

				if (!point)
					return flag_index;

				distance = distance_squared3d(point, &flag->position);
				if (distance < closest_distance)
				{
					closest_distance = distance;
					closest_flag_index = flag_index;
				}
			}
		}
	}

	return closest_flag_index;
}

static void spawn_race_vehicles(
	void)
{
	struct scenario *scenario = global_scenario_get();
	long flag_indices[MAXIMUM_RACE_VEHICLES] = { NONE };
	long vehicle_count = 0;
	struct data_iterator iterator;
	struct player_datum *player;
	struct object_placement_data placement_data;
	long itr;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)))
	{
		struct unit_datum *unit =
			player->unit_index != NONE ? unit_get(player->unit_index) : NULL;
		long flag_index;

		if (vehicle_count == MAXIMUM_RACE_VEHICLES)
			break;

		if (unit)
		{
			flag_index = find_closest_vehicle(
				&unit->object.position,
				flag_indices,
				vehicle_count);
		}
		else
		{
			flag_index = find_closest_vehicle(
				NULL,
				flag_indices,
				vehicle_count);
		}
		if (flag_index == NONE)
			break;

		flag_indices[vehicle_count++] = flag_index;
	}

	for (itr = 0; itr < vehicle_count; itr++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_indices[itr],
			struct scenario_netgame_flag);
		long vehicle_definition_index = race_get_vehicle_to_spawn(itr);

		if (vehicle_definition_index != NONE)
		{
			object_placement_data_new(&placement_data, vehicle_definition_index, NONE);
			placement_data.position = flag->position;
			vector3d_from_angle(&placement_data.forward, flag->facing);
			object_new(&placement_data);
			race_globals.vehicles_have_been_added = TRUE;
		}
	}

	return;
}

void race_engine_player_update(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	game_engine_state_message(
		player_index,
		_race_message_show_score,
		player_index);

	if (player->unit_index != NONE && game_engine_can_score())
	{
		struct unit_datum *unit = unit_get(player->unit_index);
		long netgame_flag_index;

		if (unit->object.parent_object_index != NONE)
		{
			struct unit_datum *vehicle = unit_get(unit->object.parent_object_index);

			netgame_flag_index = find_netgame_flag(
				&vehicle->object.bounding_sphere_center,
				2.5f,
				0.0f,
				_netgame_flag_race_track,
				NONE);
		}
		else
		{
			netgame_flag_index = find_netgame_flag(
				&unit->object.bounding_sphere_center,
				1.5f,
				0.6f,
				_netgame_flag_race_track,
				NONE);
		}

		if (netgame_flag_index != NONE)
			race_touch_flag(player_index, netgame_flag_index);
	}

	return;
}

void race_engine_update(
	void)
{
	if (game_time_get() == 2)
	{
		delete_race_vehicles();
		spawn_race_vehicles();
		game_engine_play_multiplayer_sound(
			game_engine_has_teams() ?
				_multiplayer_sound_team_race :
				_multiplayer_sound_race);

		if (race_globals.vehicles_have_been_added)
		{
			switch (game_engine_get_variant()->universal_variant.vehicle_set)
			{
			case _game_engine_vehicles_default:
				game_engine_play_multiplayer_sound(_multiplayer_sound_warthog);
				game_engine_play_multiplayer_sound(_multiplayer_sound_ghost);
				game_engine_play_multiplayer_sound(_multiplayer_sound_scorpion);
				break;
			case _game_engine_vehicles_warthog:
				game_engine_play_multiplayer_sound(_multiplayer_sound_warthog);
				break;
			case _game_engine_vehicles_ghost:
				game_engine_play_multiplayer_sound(_multiplayer_sound_ghost);
				break;
			case _game_engine_vehicles_tank:
				game_engine_play_multiplayer_sound(_multiplayer_sound_scorpion);
				break;
			}
		}
	}

	if (game_engine_has_teams())
	{
		if (!race_team_can_win_game(0))
			game_engine_end_game();
		if (!race_team_can_win_game(1))
			game_engine_end_game();
	}

	build_player_speeds();

	return;
}

boolean race_engine_initialize_for_new_map(
	void)
{
	long lowest_flag_index = MAXIMUM_RACE_FLAGS;
	struct scenario *scenario = global_scenario_get();
	long itr;

	race_flags_make_unique();

	race_globals.vehicles_have_been_added = FALSE;
	csmemset(&race_globals, 0, sizeof(race_globals));
	timeout_for_endgame_sound = 30;

	for (itr = 0; itr < scenario->netgame_flags.count; itr++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			itr,
			struct scenario_netgame_flag);

		if (flag->type == _netgame_flag_race_track)
		{
			if (flag->team_index >= MAXIMUM_RACE_FLAGS)
			{
				error(
					_error_silent,
					"one of the netgameflags that defines the track was out of the legal range 0..%d",
					MAXIMUM_RACE_FLAGS);
			}
			else
			{
				lowest_flag_index = MIN(lowest_flag_index, flag->team_index);
				SET_FLAG(race_globals.lap_completed_value, flag->team_index, TRUE);
				game_engine_set_goal_position(
					flag->team_index,
					&flag->position,
					0.0f,
					"flag_blue",
					NONE,
					NONE,
					NONE);
			}
		}
	}

	if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_flag_rally)
	{
		race_globals.rally_flag = new_rally_flag(NONE);
	}
	else if (game_engine_get_variant()->game_engine_variant.race.race_type == _race_type_normal)
	{
		for (itr = 0; itr < MULTIPLAYER_MAXIMUM_PLAYERS; itr++)
			race_globals.first_flag[itr] = lowest_flag_index;
	}
	else
	{
		for (itr = 0; itr < MULTIPLAYER_MAXIMUM_PLAYERS; itr++)
			race_globals.first_flag[itr] = NONE;
	}

	return TRUE;
}

/* ---------- engine table */

struct game_engine race_engine =
{
	"race",
	game_engine_race,
	race_engine_dispose,
	race_engine_initialize_for_new_map,
	race_engine_dispose_from_old_map,
	race_engine_player_added,
	race_engine_game_ending,
	race_engine_game_starting,
	race_engine_statistics_append,
	race_engine_handle_client_message,
	race_engine_handle_server_message,
	race_engine_pregame_post_rasterize,
	race_engine_post_rasterize,
	race_engine_player_update,
	race_engine_weapon_update,
	NULL,
	NULL,
	race_engine_update,
	race_engine_get_score,
	race_get_score_string,
	race_get_score_header_string,
	race_get_team_score_string,
	NULL,
	race_engine_player_damaged_player,
	race_engine_player_killed_player,
	race_engine_display_score,
	NULL,
	race_engine_prespawn_player_update,
	NULL,
	NULL,
	race_goal_matches_player,
	NULL,
	NULL,
	race_engine_did_player_win,
};

/* ---------- private code */

#ifdef HALO_LINUX
/* the distributed netcode (port/linux/game/network_distributed.c): the game
type's state the host sends its clients, which take it as it is (not
whether this machine has added its race vehicles) */
long game_engine_race_write_network_state(
	byte *buffer,
	long size)
{
	if (size < (long)sizeof(race_globals))
		return 0;
	csmemcpy(buffer, &race_globals, sizeof(race_globals));
	return sizeof(race_globals);
}

void game_engine_race_read_network_state(
	byte const *buffer,
	long size)
{
	boolean vehicles_have_been_added = race_globals.vehicles_have_been_added;

	if (size != (long)sizeof(race_globals))
		return;
	csmemcpy(&race_globals, buffer, sizeof(race_globals));
	race_globals.vehicles_have_been_added = vehicles_have_been_added;
}
#endif
