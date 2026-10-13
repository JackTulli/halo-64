/*
NETWORK_GAME_MANAGER.C

symbols in this file:
0011A170 0040:
	_network_game_invalidate_player (0000)
0011A1B0 0090:
	_network_game_add_machine (0000)
0011A240 0090:
	_network_game_update_machine (0000)
0011A2D0 0070:
	_xbox_set_machine_name (0000)
0011A340 0090:
	_network_game_generate_local_machine_name (0000)
0011A3D0 0040:
	_network_game_end_and_load_ui (0000)
0011A410 00b0:
	_network_game_reset_for_next_round (0000)
0011A4C0 0010:
	_network_game_dump (0000)
0011A4D0 0030:
	_network_player_is_valid (0000)
0011A500 00b0:
	_network_game_invalidate_machine (0000)
0011A5B0 01a0:
	_network_game_add_player (0000)
0011A750 0140:
	_compare_network_players (0000)
0011A890 0080:
	_network_game_spawn_player (0000)
0011A910 0090:
	_network_game_player_is_valid (0000)
0011A9A0 00a0:
	_network_game_invalidate (0000)
0011AA40 00a0:
	_network_game_update_player (0000)
0011AAE0 00c0:
	_network_game_remove_player (0000)
0011ABA0 01b0:
	_network_game_create_game_objects (0000)
0011AD50 00e0:
	_network_game_remove_machine (0000)
00283DA4 0031:
	??_C@_0DB@DPKLAPFE@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
00283DD8 0035:
	??_C@_0DF@FMJAGNKK@game?5?$CG?$CG?5machine?5?$CG?$CG?5network_machi@ (0000)
00283E10 0043:
	??_C@_0ED@LDCLPLHI@?8?$CFs?8?5is?5not?5a?5valid?5machine?5name@ (0000)
00283E54 0016:
	??_C@_0BG@OIMNPPEI@XSetNickname?$CI?$CJ?5failed?$AA@ (0000)
00283E6C 002d:
	??_C@_0CN@JCIPKPMA@XSetNickname?$CI?$CJ?5failed?5to?5set?5sys@ (0000)
00283E9C 001c:
	??_C@_0BM@GOCBNICP@system?5nickname?5set?5to?5?8?$CFs?8?$AA@ (0000)
00283EB8 0036:
	??_C@_0DG@KPIAMNFF@game?5?$CG?$CG?5?$CImachine_index?$DMMAXIMUM_N@ (0000)
00283EF0 0039:
	??_C@_0DJ@PKEDFKIN@game?5is?5already?5at?5maximum?5playe@ (0000)
00283F2C 000f:
	??_C@_0P@KHIDKAOH@game?5?$CG?$CG?5player?$AA@ (0000)
00283F40 004b:
	??_C@_0EL@LDBNCIG@multiple?5players?5on?5the?5same?5mac@ (0000)
00283F8C 000f:
	??_C@_0P@NHMFPDJJ@player?5?$CG?$CG?5game?$AA@ (0000)
00283F9C 002c:
	??_C@_0CM@KIIMJJBI@tried?5to?5update?5a?5player?5with?5in@ (0000)
00283FC8 002c:
	??_C@_0CM@NFHLBLAN@tried?5to?5remove?5a?5player?5with?5in@ (0000)
00283FF4 0017:
	??_C@_0BH@GFBBGLI@?$CB?$CCbad?5game?5connection?$CC?$AA@ (0000)
0028400C 0024:
	??_C@_0CE@GKPEJODL@failed?5to?5remove?5a?5machine?8s?5pla@ (0000)
00284030 0010:
	??_C@_0BA@OPIINEDG@game?5?$CG?$CG?5machine?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "main/main.h"
#include "network_game_globals.h"
#include "network_game_manager.h"
#include "network_game_ui.h"
#include "text/unicode.h"

#include <xtl.h>

/* ---------- constants */

/* the machine and player slots of a network game: the Xbox's 4 and 16, or the
native builds' session limits (port/linux/include/halo_port_limits.h) */
#ifdef HALO_LINUX
#define NETWORK_GAME_MACHINE_SLOTS HALO_PORT_MAXIMUM_NETWORK_MACHINES
#define NETWORK_GAME_PLAYER_SLOTS HALO_PORT_MAXIMUM_NETWORK_PLAYERS
#else
#define NETWORK_GAME_MACHINE_SLOTS 4
#define NETWORK_GAME_PLAYER_SLOTS 16
#endif

enum
{
	MAXIMUM_NETWORK_MACHINE_COUNT = NETWORK_GAME_MACHINE_SLOTS,
};

/* ---------- macros */

#define network_machine_is_valid(machine) \
	((machine) && (machine)->machine_index >= 0 && (machine)->machine_index < NETWORK_GAME_MACHINE_SLOTS)

/* ---------- structures */

struct game_options
{
	unsigned long flags;
	short code_version;
	short difficulty;
	unsigned long random_seed;
	char map_name[256];
};

struct network_machine
{
	wchar_t name[32];
	char machine_index;
	byte __padding41[3];
};

struct network_game_map
{
	long __unknown0;
	char name[0x80];
};

struct network_game_local_data
{
	boolean game_objects_loaded;
	byte __padding431[3];
};

struct network_game
{
	wchar_t name[16];
	struct network_game_map map;
	struct game_variant variant;
	byte __padding10C;
	byte game_mode;
#ifdef HALO_LINUX
	/* 128 does not fit a signed char */
	byte maximum_player_count;
#else
	char maximum_player_count;
#endif
	byte __padding10F;
	short difficulty;
	short machine_count;
	struct network_machine machines[NETWORK_GAME_MACHINE_SLOTS];
	short player_count;
	struct network_player players[NETWORK_GAME_PLAYER_SLOTS];
	short __unknown426;
	unsigned long random_seed;
	byte __unknown42C[4];
	struct network_game_local_data local_data;
};

#ifdef HALO_LINUX
typedef char network_game_players_offset_assert[
	offsetof(struct network_game, players) == HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET ? 1 : -1];
typedef char network_game_size_assert[
	sizeof(struct network_game) == HALO_PORT_NETWORK_GAME_SIZE ? 1 : -1];
#endif

/* ---------- prototypes */

static long compare_network_players(
	struct network_player *p1,
	struct network_player *p2);

/* ---------- globals */

/* ---------- public code */

boolean network_game_add_machine(
	struct network_game *game,
	struct network_machine *machine)
{
	long machine_index;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x6A,
		game && machine && network_machine_is_valid(machine));

	for (machine_index = 0; machine_index < NETWORK_GAME_MACHINE_SLOTS; machine_index++)
	{
		if (!network_machine_is_valid(&game->machines[machine_index]))
		{
			csmemcpy(&game->machines[machine_index], machine, sizeof(*machine));
			game->machine_count++;
			result = TRUE;
			break;
		}
	}

	return result;
}

boolean network_game_update_machine(
	struct network_game *game,
	struct network_machine *machine)
{
	long machine_index;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x81,
		game && machine && network_machine_is_valid(machine));

	for (machine_index = 0; machine_index < NETWORK_GAME_MACHINE_SLOTS; machine_index++)
	{
		if (game->machines[machine_index].machine_index == machine->machine_index)
		{
			csmemcpy(&game->machines[machine_index], machine, sizeof(*machine));
			result = TRUE;
			break;
		}
	}

	return result;
}

void xbox_set_machine_name(
	char const *machine_name)
{
	wchar_t wide_machine_name[32];

	if (machine_name && machine_name[0])
	{
		if (ascii_to_wide(machine_name, wide_machine_name, sizeof(wide_machine_name)))
		{
			wide_machine_name[31] = 0;
			if (!XSetNicknameW(wide_machine_name, TRUE))
				error(2, "XSetNickname() failed");
		}
		else
		{
			error(2, "'%s' is not a valid machine name (max. name length= %d characters)", wide_machine_name, 31);
		}
	}

	return;
}

void network_game_generate_local_machine_name(
	wchar_t *machine_name)
{
	char ascii_machine_name[32];
	HANDLE find_handle = XFindFirstNicknameW(FALSE, machine_name, 32);

	if (find_handle == INVALID_HANDLE_VALUE)
	{
		ustrncpy(machine_name, network_game_get_random_player_name(), 32);
		machine_name[31] = 0;
		if (XSetNicknameW(machine_name, TRUE))
			error(2, "system nickname set to '%s'", wide_to_ascii(machine_name, ascii_machine_name, 32));
		else
			error(2, "XSetNickname() failed to set system nickname");
	}
	else
	{
		XFindClose(find_handle);
	}

	machine_name[31] = 0;
	return;
}

void network_game_invalidate_player(
	struct network_player *player)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x58,
		player);

	player->machine_index = NONE;
	player->controller_index = NONE;
	player->team_index = NONE;
	player->player_list_index = NONE;
	player->name[0] = 0;

	return;
}

void network_game_end_and_load_ui(
	struct network_game *game)
{
	if (game->local_data.game_objects_loaded)
		main_load_ui_scenario(TRUE);

	csmemset(&game->local_data, 0, sizeof(game->local_data));

	return;
}

void network_game_dump(
	struct network_game *game,
	char const *prefix)
{
	return;
}

boolean network_game_add_player(
	struct network_game *game,
	struct network_player *player)
{
	long player_index;
	long new_player_index;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0xBB,
		game && player);

	if (game->player_count < game->maximum_player_count)
	{
		if (VALID_INDEX(player->machine_index, MAXIMUM_NETWORK_MACHINE_COUNT) &&
			VALID_INDEX(player->controller_index, MAXIMUM_LOCAL_PLAYERS))
		{
			for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
			{
				if (game->players[player_index].machine_index == player->machine_index &&
					game->players[player_index].controller_index == player->controller_index)
				{
					break;
				}
			}

			if (player_index == NETWORK_GAME_PLAYER_SLOTS && network_player_is_valid(player))
			{
				new_player_index = NONE;
				for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
				{
					if (game->players[player_index].player_list_index == NONE)
					{
						new_player_index = player_index;
						break;
					}
				}

				if ((player->player_list_index == NONE || new_player_index == player->player_list_index) &&
					new_player_index != NONE)
				{
					player->player_list_index = (char)new_player_index;
					csmemcpy(&game->players[new_player_index], player, sizeof(struct network_player));
					game->player_count++;
					result = TRUE;
				}
			}
		}
	}
	else
	{
		error(2, "game is already at maximum players; can't add new player");
	}

	return result;
}

boolean network_player_is_valid(
	struct network_player *player)
{
	if (player &&
		player->controller_index >= 0 &&
		player->controller_index < MAXIMUM_LOCAL_PLAYERS &&
		player->machine_index >= 0 &&
#ifdef HALO_LINUX
		/* the Xbox game checks the machine against the split screen limit,
		which only works while both are 4 */
		(long)player->machine_index < MAXIMUM_NETWORK_MACHINE_COUNT)
#else
		player->machine_index < MAXIMUM_LOCAL_PLAYERS)
#endif
	{
		return TRUE;
	}

	return FALSE;
}

void network_game_invalidate_machine(
	struct network_game *game,
	word machine_index)
{
	long player_index;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x40,
		game && (machine_index<MAXIMUM_NETWORK_MACHINE_COUNT));

	game->machines[machine_index].machine_index = NONE;
	game->machines[machine_index].name[0] = 0;

	for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
	{
		if (game->players[player_index].machine_index == machine_index)
			network_game_invalidate_player(&game->players[player_index]);
	}

	return;
}

boolean network_game_spawn_player(
	struct network_player *player)
{
	long player_index;
	short controller_index;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x1BC,
		network_player_is_valid(player));

	controller_index = network_game_player_is_local(player) ? player->controller_index : NONE;
	player_index = player_new(player->machine_index, NONE, controller_index, player);
	if (player_index != NONE)
	{
		player->player_list_index = (char)player_index;
		return TRUE;
	}

	return FALSE;
}

boolean network_game_player_is_valid(
	struct network_player *player,
	struct network_game *game)
{
	long player_index;
	struct network_player *current_player;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x247,
		player && game);

	if (network_player_is_valid(player))
	{
		current_player = &game->players[0];
		for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++, current_player++)
		{
			if (current_player->machine_index == player->machine_index &&
				current_player->controller_index == player->controller_index)
			{
				result = TRUE;
				break;
			}
		}
	}

	return result;
}

void network_game_reset_for_next_round(
	struct network_game *game,
	boolean unload_game_objects)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x22D,
		game);

	if (unload_game_objects && game->local_data.game_objects_loaded)
	{
		main_load_ui_scenario(TRUE);
		csmemset(&game->local_data, 0, sizeof(game->local_data));
		if (global_network_game_server_get())
			game_connection_set(2);
		else if (global_network_game_client_get())
			game_connection_set(1);
	}
	else
	{
		csmemset(&game->local_data, 0, sizeof(game->local_data));
	}

	game_time_end();
	return;
}

void network_game_invalidate(
	struct network_game *game)
{
	long machine_index;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x23,
		game);

	csmemset(game, 0, sizeof(*game));
	csmemset(&game->map, 0, sizeof(game->map));
	game->machine_count = 0;
	game->player_count = 0;

	for (machine_index = 0; machine_index < NETWORK_GAME_MACHINE_SLOTS; machine_index++)
		network_game_invalidate_machine(game, (short)machine_index);

	csmemset(game->players, NONE, sizeof(game->players));
	game->game_mode = 2;
	game->maximum_player_count = NETWORK_GAME_PLAYER_SLOTS;
	game->local_data.game_objects_loaded = FALSE;

	return;
}

boolean network_game_update_player(
	struct network_game *game,
	struct network_player *player)
{
	struct network_player *current_player;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x101,
		game && player);

	if (network_game_player_is_valid(player, game))
	{
		current_player = &game->players[player->player_list_index];
		if (current_player->controller_index == player->controller_index &&
			current_player->machine_index == player->machine_index)
		{
			csmemcpy(current_player, player, sizeof(*current_player));
			result = TRUE;
		}
	}

	if (!result)
		error(2, "tried to update a player with indvalid data");

	return result;
}

boolean network_game_remove_player(
	struct network_game *game,
	struct network_player *player)
{
	long player_index;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x120,
		game && player);

	if (network_game_player_is_valid(player, game))
	{
		for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
		{
			if (network_player_is_valid(&game->players[player_index]) &&
				game->players[player_index].machine_index == player->machine_index &&
				game->players[player_index].controller_index == player->controller_index)
			{
				network_game_invalidate_player(&game->players[player_index]);
				game->player_count--;
				result = TRUE;
				break;
			}
		}
	}
	else
	{
		error(2, "tried to remove a player with indvalid data");
	}

	return result;
}

boolean network_game_remove_machine(
	struct network_game *game,
	struct network_machine *machine)
{
	long machine_index;
	long player_index;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x97,
		game && machine);

	if (network_machine_is_valid(machine))
	{
		for (machine_index = 0; machine_index < NETWORK_GAME_MACHINE_SLOTS; machine_index++)
		{
			if (game->machines[machine_index].machine_index == machine->machine_index)
			{
				for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
				{
					if (network_player_is_valid(&game->players[player_index]) &&
						game->players[player_index].machine_index == machine->machine_index)
					{
						if (!network_game_remove_player(game, &game->players[player_index]))
							error(2, "failed to remove a machine's player");
					}
				}

				network_game_invalidate_machine(game, machine->machine_index);
				game->machine_count--;
				result = TRUE;
				break;
			}
		}
	}

	return result;
}

boolean network_game_create_game_objects(
	struct network_game *game)
{
	long player_index;
	struct game_options options;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x170,
		game);

	game_options_new(&options);
	csstrncpy(options.map_name, game->map.name, sizeof(game->map.name) - 1);
	options.difficulty = game->difficulty;

	switch (game_connection())
	{
		case _game_connection_network_client:
		case _game_connection_network_server:
			options.random_seed = network_game_get_random_seed();
			break;

		case _game_connection_film_playback:
			options.random_seed = game->random_seed;
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
				0x17F,
				!"bad game connection");
			break;
	}

	game_precache_new_map(options.map_name, TRUE);
	main_menu_unload();

	if (game_in_progress())
	{
		game_dispose_from_old_map();
		game_unload();
	}

	if (game->variant.game_engine_index)
		game_set_game_variant(&game->variant);

	if (game_load(&options))
	{
		game->local_data.game_objects_loaded = TRUE;
		game_initialize_for_new_map();

		qsort(
			game->players,
			NETWORK_GAME_PLAYER_SLOTS,
			sizeof(struct network_player),
			(int(__cdecl *)(const void *, const void *))compare_network_players);

		for (player_index = 0; player_index < NETWORK_GAME_PLAYER_SLOTS; player_index++)
		{
			if (!network_player_is_valid(&game->players[player_index]))
				break;

			if (!network_game_spawn_player(&game->players[player_index]))
			{
				game->local_data.game_objects_loaded = FALSE;
				break;
			}
		}
	}
	else
	{
		error(0, "game_load() failed.");
	}

	return game->local_data.game_objects_loaded;
}

/* ---------- private code */

static long compare_network_players(
	struct network_player *p1,
	struct network_player *p2)
{
	long result = 0;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
		0x142,
		p1 && p2);

	if (!network_player_is_valid(p1) && !network_player_is_valid(p2))
		result = 0;
	else if (!network_player_is_valid(p1) && network_player_is_valid(p2))
		result = 1;
	else if (network_player_is_valid(p1) && !network_player_is_valid(p2))
		result = -1;
	else if (p1->machine_index > p2->machine_index)
		result = 1;
	else if (p1->machine_index < p2->machine_index)
		result = -1;
	else if (p1->controller_index > p2->controller_index)
		result = 1;
	else if (p1->controller_index < p2->controller_index)
		result = -1;
	else
		match_vassert(
			"c:\\halo\\SOURCE\\networking\\network_game_manager.c",
			0x165,
			FALSE,
			"multiple players on the same machine cannot have the same controller index");

	return result;
}
