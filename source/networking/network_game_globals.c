/*
NETWORK_GAME_GLOBALS.C

symbols in this file:
00119850 0020:
	_network_game_is_active (0000)
00119870 0040:
	_network_game_set_number_of_games_played (0000)
001198B0 0040:
	_network_game_set_random_seed (0000)
001198F0 0030:
	_network_game_get_game (0000)
00119920 0080:
	_network_game_player_is_local (0000)
001199A0 0010:
	_network_game_accept_remote_connections (0000)
001199B0 0010:
	_network_game_should_accept_remote_connections (0000)
001199C0 0020:
	_network_game_is_splitscreen_local (0000)
001199E0 0010:
	_network_game_set_quickstart_local (0000)
001199F0 0030:
	_network_game_is_quickstart_local (0000)
00119A20 0010:
	_global_network_game_server_get (0000)
00119A30 0030:
	_dispose_global_network_game_server (0000)
00119A60 0030:
	_network_game_server_start_frame (0000)
00119A90 0010:
	_global_network_game_client_get (0000)
00119AA0 0050:
	_create_global_network_game_client (0000)
00119AF0 0030:
	_dispose_global_network_game_client (0000)
00119B20 0230:
	_network_game_client_start_frame (0000)
00119D50 0190:
	_network_game_client_end_frame (0000)
00119EE0 0030:
	_network_game_client_get_local_machine_index (0000)
00119F10 00c0:
	_network_game_client_local_player_quit (0000)
00119FD0 0010:
	_network_game_abort (0000)
00119FE0 0010:
	_network_game_client_all_local_players_have_quit (0000)
00119FF0 0030:
	_network_game_client_request_immediate_start (0000)
0011A020 0060:
	_network_game_get_number_of_games_played (0000)
0011A080 0060:
	_network_game_get_random_seed (0000)
0011A0E0 0090:
	_create_global_network_game_server (0000)
00283ACC 0024:
	??_C@_0CE@HIIACJBM@player_action_collection_definit@ (0000)
00283AF0 0020:
	??_C@_0CA@IAPBLKNI@player_action_packet_definition?$AA@ (0000)
00283B10 0031:
	??_C@_0DB@EHHBIEMA@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
00283B44 0017:
	??_C@_0BH@PPJHLPOF@no?5network?5game?5server?$AA@ (0000)
00283B5C 0021:
	??_C@_0CB@ODDBPMEE@global_network_game_client?$DN?$DNNULL@ (0000)
00283B80 003e:
	??_C@_0DO@IGIJAIBE@internal?5networking?5error?5?$FLnetwo@ (0000)
00283BC0 003f:
	??_C@_0DP@BEEIMDCE@internal?5networking?5error?5?$FLnetwo@ (0000)
00283C00 0020:
	??_C@_0CA@NMHJFOCM@waiting?5for?5game?5to?5restart?5?4?4?4?$AA@ (0000)
00283C20 0033:
	??_C@_0DD@FNBJPPIH@client?5signalled?5to?5begin?5loadin@ (0000)
00283C54 001e:
	??_C@_0BO@DDPPJMGH@waiting?5for?5game?5to?5start?5?4?4?4?$AA@ (0000)
00283C74 001b:
	??_C@_0BL@HPHHNBBK@joining?5a?5network?5game?5?4?4?4?$AA@ (0000)
00283C90 0021:
	??_C@_0CB@LPCIEGFA@searching?5for?5a?5network?5game?5?4?4?4@ (0000)
00283CB4 003c:
	??_C@_0DM@CDPHHLCN@failed?5to?5create?5a?5_message_type@ (0000)
00283CF0 002b:
	??_C@_0CL@LCMJOEOC@failed?5to?5send?5a?5game?5update?5to?5@ (0000)
00283D1C 0038:
	??_C@_0DI@DKNGHDCK@failed?5to?5request?5player?5removal@ (0000)
00283D54 002b:
	??_C@_0CL@EPJNANF@network_game_client_request_star@ (0000)
00283D80 0021:
	??_C@_0CB@MPOLIODB@global_network_game_server?$DN?$DNNULL@ (0000)
0030A988 00da:
	_data_0030a988 (0000)
	_player_action_packet_definition (0028)
	_player_action_collection_definition (00c4)
004566DC 0010:
	_bss_004566dc (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "bungie_net/common/message_header.h"
#include "bungie_net/network/transport.h"
#include "game/player_queues_new.h"
#include "game/players.h"
#include "main/main.h"
#include "memory/data_packet_groups.h"
#include "network_client_manager.h"
#include "network_messages.h"
#include "network_game_manager.h"
#include "network_game_globals.h"
#include "network_server_manager_internal.h"

/* ---------- constants */

enum network_game_client_state
{
	_network_game_client_state_searching,
	_network_game_client_state_joining,
	_network_game_client_state_pregame,
	_network_game_client_state_ingame,
	_network_game_client_state_postgame,
	NUMBER_OF_NETWORK_GAME_CLIENT_STATES,
};

/* ---------- macros */

#define global_network_game_client bss_004566dc.client
#define global_network_game_server bss_004566dc.server

/* ---------- structures */

struct network_game_server;
struct network_game_client;

struct player_action_collection
{
	struct player_action actions[MAXIMUM_LOCAL_PLAYERS];
};

typedef char network_player_action_collection_size_assert[
	sizeof(struct player_action_collection) == 0x80 ? 1 : -1];

#pragma pack(push, 2)
struct player_action_collection_definition
{
	char const *name;
	long flags;
	short size;
	short version;
	struct data_packet_field *packet_fields;
	boolean initialized;
	byte __padding11[3];
	short previous_client_state;
};
#pragma pack(pop)

struct player_action_packet_definition_storage
{
	struct data_packet_definition definition;
	long __padding14;
	struct data_packet_field collection_fields[13];
	short __padding9a;
};

typedef char player_action_collection_definition_size_assert[
	sizeof(struct player_action_collection_definition) == 0x16 ? 1 : -1];
typedef char player_action_packet_definition_storage_size_assert[
	sizeof(struct player_action_packet_definition_storage) == 0x9C ? 1 : -1];

struct client_game_update_message
{
	long update_number;
	short __unknown4;
	short local_player_count;
	byte update[0x80];
};

struct network_machine
{
	byte __unknown0[0x40];
	char machine_index;
};

struct local_network_player
{
	byte __unknown0[0x1C];
	boolean machine_index;
};

struct network_game
{
#ifdef HALO_LINUX
	/* the players follow the machines, which follow the native builds'
	session limits (port/linux/include/halo_port_limits.h) */
	byte __unknown0[HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET];
	struct network_player players[HALO_PORT_MAXIMUM_NETWORK_PLAYERS];
#else
	byte __unknown0[0x226];
	struct network_player players[16];
#endif
	byte __unknown426[2];
	long random_seed;
	long number_of_games_played;
};

typedef char network_machine_index_offset_assert[
	offsetof(struct network_machine, machine_index) == 0x40 ? 1 : -1];
#ifdef HALO_LINUX
typedef char network_game_players_offset_assert[
	offsetof(struct network_game, players) == HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET ? 1 : -1];
typedef char network_game_random_seed_offset_assert[
	offsetof(struct network_game, random_seed) == HALO_PORT_NETWORK_GAME_RANDOM_SEED_OFFSET ? 1 : -1];
#else
typedef char network_game_players_offset_assert[
	offsetof(struct network_game, players) == 0x226 ? 1 : -1];
typedef char network_game_random_seed_offset_assert[
	offsetof(struct network_game, random_seed) == 0x428 ? 1 : -1];
#endif

struct network_game_globals
{
	struct network_game_server *server;
	struct network_game_client *client;
	boolean accept_remote_connections;
	boolean quickstart_local;
	boolean client_started;
	byte __padding0b;
	unsigned long last_client_update_time;
};

typedef char network_game_globals_size_assert[
	sizeof(struct network_game_globals) == 0x10 ? 1 : -1];

/* ---------- prototypes */

unsigned long *get_global_local_random_seed_address(
	void);
unsigned short seed_random(
	unsigned long *seed);

/* ---------- globals */

struct network_game_globals bss_004566dc = { 0 };
struct data_packet_field data_0030a988[4] =
{
	{ _data_packet_field_longs, 6, 0, 0, 0 },
	{ _data_packet_field_shorts, 3, 0, 0, 0 },
	{ _data_packet_field_pad, 2, 0, 0, 0 },
	{ _data_packet_field_end, 0, 0, 0, 0 },
};
struct player_action_packet_definition_storage player_action_packet_definition =
{
	{
		"player_action_packet_definition",
		0,
		0x20,
		1,
		data_0030a988,
		FALSE,
	},
	0,
	{
		{ _data_packet_field_longs, 6, 0, 0, 0 },
		{ _data_packet_field_shorts, 3, 0, 0, 0 },
		{ _data_packet_field_pad, 2, 0, 0, 0 },
		{ _data_packet_field_longs, 6, 0, 0, 0 },
		{ _data_packet_field_shorts, 3, 0, 0, 0 },
		{ _data_packet_field_pad, 2, 0, 0, 0 },
		{ _data_packet_field_longs, 6, 0, 0, 0 },
		{ _data_packet_field_shorts, 3, 0, 0, 0 },
		{ _data_packet_field_pad, 2, 0, 0, 0 },
		{ _data_packet_field_longs, 6, 0, 0, 0 },
		{ _data_packet_field_shorts, 3, 0, 0, 0 },
		{ _data_packet_field_pad, 2, 0, 0, 0 },
		{ _data_packet_field_end, 0, 0, 0, 0 },
	},
	0,
};
struct player_action_collection_definition player_action_collection_definition =
{
	"player_action_collection_definition",
	0,
	0x80,
	1,
	player_action_packet_definition.collection_fields,
	FALSE,
	{ 0, 0, 0 },
	-1,
};

/* ---------- public code */

#ifdef HALO_LINUX
/* the distributed netcode's per-tick state (port/linux/game/network_distributed.c),
unreliably to the host, as the game update is */
boolean network_distributed_client_send(
	void *message,
	word size)
{
	struct transport_address remote_server_address;
	byte buffer[0x1000];

	if (!global_network_game_client || size > sizeof(buffer))
		return FALSE;
	/* (the write swaps the header in place) */
	csmemcpy(buffer, message, size);
	network_game_client_get_remote_server_address(global_network_game_client, &remote_server_address);
	return network_game_client_write(network_game_client_get_connection(global_network_game_client),
		(message_header *)buffer, size, &remote_server_address, 0);
}

/* ... reliably (a client's players' hits, network_damage.c) */
boolean network_distributed_client_send_reliably(
	void *message,
	word size)
{
	byte buffer[0x1000];

	if (!global_network_game_client || size > sizeof(buffer))
		return FALSE;
	/* (the write swaps the header in place) */
	csmemcpy(buffer, message, size);
	return network_game_client_write(network_game_client_get_connection(global_network_game_client),
		(message_header *)buffer, size, NULL, 1);
}

/* the platform layer's (port/linux/src/port_config.c) */
char const *config_string(char const *name);

/* a client of the distributed netcode, which decides nothing the host does */
boolean network_game_distributed_client(
	void)
{
	return game_connection() == _game_connection_network_client && network_game_distributed();
}

boolean network_game_distributed(
	void)
{
	static short distributed = NONE;

	if (distributed == NONE)
		distributed = !csstrcmp(config_string("network.netcode"), "distributed");
	return distributed;
}

#endif
boolean network_game_is_active(
	void)
{
	return bss_004566dc.client != NULL || bss_004566dc.server != NULL;
}

void network_game_set_number_of_games_played(
	long number_of_games_played)
{
	if (bss_004566dc.server)
	{
		network_game_server_get_game(bss_004566dc.server)->number_of_games_played =
			number_of_games_played;
	}

	if (bss_004566dc.client)
	{
		network_game_client_get_game(bss_004566dc.client)->number_of_games_played =
			number_of_games_played;
	}

	return;
}

void network_game_set_random_seed(
	long random_seed)
{
	if (bss_004566dc.server)
		network_game_server_get_game(bss_004566dc.server)->random_seed = random_seed;

	if (bss_004566dc.client)
		network_game_client_get_game(bss_004566dc.client)->random_seed = random_seed;

	return;
}

struct network_game *network_game_get_game(
	void)
{
	if (global_network_game_server)
		return network_game_server_get_game(global_network_game_server);

	if (global_network_game_client)
		return network_game_client_get_game(global_network_game_client);

	return NULL;
}

boolean network_game_player_is_local(
	struct network_player *player)
{
	boolean machine_index;
	struct network_machine *machine;

	if (player && network_player_is_valid(player) && global_network_game_client)
	{
		machine = network_game_client_get_machine(global_network_game_client);
		if (!machine || machine->machine_index != player->machine_index)
			return FALSE;
	}
	else if (game_connection() == _game_connection_film_playback)
	{
		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
			0x9B,
			player);

		machine_index = ((struct local_network_player *)player)->machine_index;
		machine_index = !machine_index;

		return machine_index;
	}

	return TRUE;
}

void network_game_accept_remote_connections(
	boolean accept_remote_connections)
{
	bss_004566dc.accept_remote_connections = accept_remote_connections;
#ifdef HALO_LINUX
	p2p_accept_remote_connections(accept_remote_connections);
#endif

	return;
}

boolean network_game_should_accept_remote_connections(
	void)
{
	return bss_004566dc.accept_remote_connections;
}

boolean network_game_is_splitscreen_local(
	void)
{
	return bss_004566dc.server != NULL && !bss_004566dc.accept_remote_connections;
}

void network_game_set_quickstart_local(
	void)
{
	bss_004566dc.quickstart_local = TRUE;

	return;
}

boolean network_game_is_quickstart_local(
	void)
{
	return bss_004566dc.server != NULL &&
		!bss_004566dc.accept_remote_connections &&
		bss_004566dc.quickstart_local == TRUE;
}

struct network_game_server *global_network_game_server_get(
	void)
{
	return bss_004566dc.server;
}

void dispose_global_network_game_server(
	void)
{
	if (bss_004566dc.server)
	{
		network_game_server_dispose(bss_004566dc.server);
		bss_004566dc.server = NULL;
		bss_004566dc.quickstart_local = FALSE;
	}

	return;
}

boolean network_game_server_start_frame(
	void)
{
	boolean result;

	if (bss_004566dc.server)
		result = network_game_server_idle(bss_004566dc.server);
	else
	{
		error(_error_silent, "no network game server");
		result = TRUE;
	}

	return result;
}

struct network_game_client *global_network_game_client_get(
	void)
{
	return bss_004566dc.client;
}

boolean create_global_network_game_client(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
		0x10F,
		global_network_game_client==NULL);

	bss_004566dc.client = network_game_client_create();
	if (bss_004566dc.client)
		bss_004566dc.client_started = FALSE;

	return bss_004566dc.client != NULL;
}

void dispose_global_network_game_client(
	void)
{
	if (bss_004566dc.client)
	{
		network_game_client_dispose(bss_004566dc.client);
		bss_004566dc.client = NULL;
	}

	bss_004566dc.client_started = FALSE;

	return;
}

boolean network_game_client_start_frame(
	void)
{
	short state;
	short state_data;
	boolean result;
	struct network_game *game;

	if (bss_004566dc.client_started == TRUE)
	{
		game_connection_set(0);
		if (global_network_game_server)
			game = network_game_server_get_game(global_network_game_server);
		else if (global_network_game_client)
			game = network_game_client_get_game(global_network_game_client);
		else
			game = NULL;
		network_game_end_and_load_ui(game);

		if (global_network_game_client)
		{
			network_game_client_dispose(global_network_game_client);
			global_network_game_client = NULL;
		}

		bss_004566dc.client_started = FALSE;
		if (global_network_game_server)
		{
			network_game_server_dispose(global_network_game_server);
			global_network_game_server = NULL;
			bss_004566dc.quickstart_local = FALSE;
		}

		main_goto_main_menu();
		result = TRUE;
	}
	else
	{
		result = network_game_client_idle(global_network_game_client);
		if (result)
		{
			if (!network_game_client_get_error(global_network_game_client))
			{
				state = network_game_client_get_state(global_network_game_client, &state_data);
				switch ((unsigned short)state)
				{
				case _network_game_client_state_searching:
					if (player_action_collection_definition.previous_client_state != state)
						network_event("searching for a network game ...");
					break;
				case _network_game_client_state_joining:
					if (player_action_collection_definition.previous_client_state != state)
						network_event("joining a network game ...");
					break;
				case _network_game_client_state_pregame:
					if (player_action_collection_definition.previous_client_state != state)
						network_event("waiting for game to start ...");
					break;
				case _network_game_client_state_ingame:
					if (player_action_collection_definition.previous_client_state != state)
						network_event("client signalled to begin loading for network game");
					break;
				case _network_game_client_state_postgame:
					if (player_action_collection_definition.previous_client_state != state)
						network_event("waiting for game to restart ...");
					break;
				default:
					display_assert(
						"client is in an unknown state",
						"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
						0x160,
						TRUE);
					system_exit(-1);
					break;
				}

				player_action_collection_definition.previous_client_state = state;
			}
			else
			{
				network_event("internal networking error [network_game_client_get_error()!=0]");
				result = FALSE;
			}
		}
		else
		{
			network_event("internal networking error [network_game_client_idle() failed]");
		}
	}

	return result;
}

boolean network_game_client_end_frame(
	void)
{
	struct player_action_collection update;
	struct client_game_update_message message;
	struct transport_address remote_server_address;
	unsigned long now;
	message_header *encoded_message;
	boolean result;

	result = TRUE;
	if (!global_network_game_client)
	{
		game_connection_set(0);
		main_menu_ensure_player_queues_exist();
	}
	else if (network_game_client_get_state(global_network_game_client, NULL) == _network_game_client_state_ingame)
	{
		now = system_milliseconds();
		if (now-bss_004566dc.last_client_update_time >= 0x10 &&
			network_game_client_server_has_started_game(global_network_game_client))
		{
			network_game_client_get_next_update_number(global_network_game_client);
			network_game_client_get_game(global_network_game_client);
			update_client_build_client_update(&update);

			if (network_client_get_oos(global_network_game_client))
			{
				message.update_number = network_game_client_get_next_update_number(
					global_network_game_client) | 0x80000000;
			}
			else
			{
				message.update_number = network_game_client_get_next_update_number(
					global_network_game_client) & 0x7FFFFFFF;
			}

			csmemcpy(message.update, &update, sizeof(update));
			message.local_player_count = local_player_count();
			encoded_message = create_network_game_message(
				_message_client_game_update,
				&message,
				sizeof(message));
			if (encoded_message)
			{
				network_game_client_get_remote_server_address(
					global_network_game_client,
					&remote_server_address);
				result = network_game_client_write(
					network_game_client_get_connection(global_network_game_client),
					encoded_message,
					GET_MESSAGE_SIZE(*encoded_message),
					&remote_server_address,
					0);
				if (!result)
					network_event("failed to send a game update to the server");
			}
			else
			{
				network_event("failed to create a _message_type_client_game_update message");
				result = FALSE;
			}

			bss_004566dc.last_client_update_time = now;
		}
	}

	return result;
}

short network_game_client_get_local_machine_index(
	void)
{
	short machine_index = NONE;
	struct network_machine *machine;

	if (global_network_game_client)
	{
		machine = network_game_client_get_machine(global_network_game_client);
		if (machine)
			machine_index = machine->machine_index;
	}

	return machine_index;
}

void network_game_client_local_player_quit(
	short controller_index)
{
	long player_index;
	char *player_machine_index;
	struct network_game *game;
	struct network_machine *machine;
	struct network_player *player;
	struct network_player *test_player;

	if (global_network_game_client)
	{
		machine = network_game_client_get_machine(global_network_game_client);
		game = network_game_client_get_game(global_network_game_client);
		player = NULL;
		if (machine)
		{
			player_index = 0;
			player_machine_index = &game->players[0].machine_index;
#ifdef HALO_LINUX
			while (player_index < HALO_PORT_MAXIMUM_NETWORK_PLAYERS)
#else
			while (player_index < 16)
#endif
			{
				test_player = (struct network_player *)(
					player_machine_index - offsetof(struct network_player, machine_index));
				if (network_player_is_valid(test_player) &&
					player_machine_index[0] == machine->machine_index &&
					player_machine_index[1] == controller_index)
				{
					player = &game->players[player_index];
					break;
				}

				player_index++;
				player_machine_index += sizeof(struct network_player);
			}
		}

		if (player &&
			!network_game_client_request_remove_player(
				global_network_game_client,
				player))
		{
			error(
				_error_silent,
				"failed to request player removal in-game for player #%d",
				player->controller_index);
		}
	}

	return;
}

long network_game_get_number_of_games_played(
	void)
{
	struct network_game *game = network_game_get_game();

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
		0x55,
		game);

	return game->number_of_games_played;
}

long network_game_get_random_seed(
	void)
{
	struct network_game *game = network_game_get_game();

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
		0x73,
		game);

	return game->random_seed;
}

void network_game_abort(
	void)
{
	bss_004566dc.client_started = TRUE;

	return;
}

void network_game_client_all_local_players_have_quit(
	void)
{
	bss_004566dc.client_started = TRUE;

	return;
}

void network_game_client_request_immediate_start(
	void)
{
	if (global_network_game_client &&
		!network_game_client_request_start_time_change(global_network_game_client, 3))
	{
		error(_error_silent, "network_game_client_request_start() failed");
	}

	return;
}

boolean create_global_network_game_server(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_game_globals.c",
		0xD6,
		global_network_game_server==NULL);

	global_network_game_server = network_game_server_create();
	if (global_network_game_server)
	{
		network_game_set_random_seed(
			seed_random(get_global_local_random_seed_address()));
	}

	return global_network_game_server != NULL;
}

/* ---------- private code */
