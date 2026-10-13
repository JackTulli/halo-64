/*
NETWORK_CLIENT_MESSAGE_HANDLER.C

symbols in this file:
00116AB0 00b0:
	_network_game_client_handle_message_server_game_advertise (0000)
00116B60 0090:
	_network_game_client_handle_message_server_pong (0000)
00116BF0 00a0:
	_network_game_client_handle_message_server_machine_accepted (0000)
00116C90 00a0:
	_network_game_client_handle_message_server_machine_rejected (0000)
00116D30 0130:
	_network_game_client_handle_message_server_game_settings_update (0000)
00116E60 0100:
	_network_game_client_handle_message_server_pregame_countdown (0000)
00116F60 00e0:
	_network_game_client_handle_message_server_pregame_keep_alive (0000)
00117040 00e0:
	_network_game_client_handle_message_server_postgame_keep_alive (0000)
00117120 00d0:
	_network_game_client_handle_message_server_begin_game (0000)
001171F0 00b0:
	_network_game_client_handle_message_server_graceful_game_exit_pregame (0000)
001172A0 00c0:
	_network_game_client_handle_message_server_game_update (0000)
00117360 00c0:
	_network_game_client_handle_message_server_add_player_ingame (0000)
00117420 00c0:
	_network_game_client_handle_message_server_remove_player_ingame (0000)
001174E0 00a0:
	_network_game_client_handle_message_server_game_over (0000)
00117580 00c0:
	_network_game_client_handle_message_server_switch_to_pregame (0000)
00117640 00b0:
	_network_game_client_handle_message_server_graceful_game_exit_postgame (0000)
001176F0 0440:
	_network_game_client_handle_message (0000)
00281928 0045:
	??_C@_0EF@JEKJBGGG@ignoring?5an?5advertised?5game?5beca@ (0000)
00281970 0038:
	??_C@_0DI@DKMFJKNC@failed?5to?5decode?5a?5message_serve@ (0000)
002819A8 003e:
	??_C@_0DO@KGHCHMKG@ignoring?5a?5pong?5message?5because?5@ (0000)
002819E8 002e:
	??_C@_0CO@MNCKPDII@failed?5to?5decode?5a?5message_serve@ (0000)
00281A18 005e:
	??_C@_0FO@MDHPOCDK@ignoring?5a?5message_server_machin@ (0000)
00281A78 003a:
	??_C@_0DK@OHJLEEJA@failed?5to?5decode?5a?5message_serve@ (0000)
00281AB8 005e:
	??_C@_0FO@KFHDMLDG@ignoring?5a?5message_server_machin@ (0000)
00281B18 003a:
	??_C@_0DK@NPFONKEC@failed?5to?5decode?5a?5message_serve@ (0000)
00281B58 0048:
	??_C@_0EI@MBCELKHN@ignoring?5a?5message_server_game_s@ (0000)
00281BA0 0055:
	??_C@_0FF@MOCCMIII@failed?5to?5handle?5a?5message_serve@ (0000)
00281BF8 003e:
	??_C@_0DO@CFANMJHG@failed?5to?5decode?5a?5message_serve@ (0000)
00281C38 0033:
	??_C@_0DD@BCJHCHJP@network_game_client_game_setting@ (0000)
00281C6C 0017:
	??_C@_0BH@BJKENKMM@source_address?5?$CB?$DN?5NULL?$AA@ (0000)
00281C84 003b:
	??_C@_0DL@LEGDIILJ@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
00281CC0 0045:
	??_C@_0EF@KFKEPLOD@ignoring?5a?5message_server_pregam@ (0000)
00281D08 0052:
	??_C@_0FC@JHBFLLPP@failed?5to?5handle?5a?5message_serve@ (0000)
00281D5C 003b:
	??_C@_0DL@LHJKBMIA@failed?5to?5decode?5a?5message_serve@ (0000)
00281D98 0046:
	??_C@_0EG@IAKPIALP@ignoring?5a?5message_server_pregam@ (0000)
00281DE0 0053:
	??_C@_0FD@ECPEPJNC@failed?5to?5handle?5a?5message_serve@ (0000)
00281E34 003c:
	??_C@_0DM@GMAKBHHC@failed?5to?5decode?5a?5message_serve@ (0000)
00281E70 0047:
	??_C@_0EH@IABGEGJI@ignoring?5a?5message_server_postga@ (0000)
00281EB8 0055:
	??_C@_0FF@PKNMLFIC@failed?5to?5handle?5a?5message_serve@ (0000)
00281F10 003d:
	??_C@_0DN@BIGGNOLB@failed?5to?5decode?5a?5message_serve@ (0000)
00281F50 0046:
	??_C@_0EG@KGCDAEPA@ignoring?5a?5message_server_begin_@ (0000)
00281F98 004c:
	??_C@_0EM@LHDJIHFO@failed?5to?5handle?5a?5message_serve@ (0000)
00281FE4 0034:
	??_C@_0DE@OPOJGBIB@failed?5to?5decode?5a?5message_serve@ (0000)
00282018 002e:
	??_C@_0CO@IDLMDGAP@network_game_client_game_has_sta@ (0000)
00282048 0056:
	??_C@_0FG@NGBIOKLC@ignoring?5a?5message_server_gracef@ (0000)
002820A0 005c:
	??_C@_0FM@DIGPPEBG@failed?5to?5handle?5a?5message_serve@ (0000)
00282100 0044:
	??_C@_0EE@ECCKOGDL@failed?5to?5decode?5a?5message_serve@ (0000)
00282148 0047:
	??_C@_0EH@MEBMJMKF@ignoring?5a?5message_server_game_u@ (0000)
00282190 004a:
	??_C@_0EK@HLKFMGH@failed?5to?5handle?5a?5message_serve@ (0000)
002821DC 0035:
	??_C@_0DF@IIILBLL@failed?5to?5decode?5a?5message_serve@ (0000)
00282214 0030:
	??_C@_0DA@NKCCMGPH@network_game_client_handle_game_@ (0000)
00282248 004d:
	??_C@_0EN@FKBHABAC@ignoring?5a?5message_server_add_pl@ (0000)
00282298 0050:
	??_C@_0FA@NACEIJGI@failed?5to?5handle?5a?5message_serve@ (0000)
002822E8 003b:
	??_C@_0DL@FGDEKFMJ@failed?5to?5decode?5a?5message_serve@ (0000)
00282324 0030:
	??_C@_0DA@ICGCINCO@network_game_client_add_player_t@ (0000)
00282358 0050:
	??_C@_0FA@IBHOBKAI@ignoring?5a?5message_server_remove@ (0000)
002823A8 0053:
	??_C@_0FD@OLBMGIKB@failed?5to?5handle?5a?5message_serve@ (0000)
002823FC 003e:
	??_C@_0DO@GODOOHMN@failed?5to?5decode?5a?5message_serve@ (0000)
0028243C 002b:
	??_C@_0CL@NBDIBGCC@network_game_client_remove_playe@ (0000)
00282468 0045:
	??_C@_0EF@DDKCBEG@ignoring?5a?5message_server_game_o@ (0000)
002824B0 0048:
	??_C@_0EI@BGOBBEDF@failed?5to?5handle?5a?5message_serve@ (0000)
002824F8 0043:
	??_C@_0ED@LKOOJPFP@failed?5to?5decode?5a?5message_serve@ (0000)
00282540 004d:
	??_C@_0EN@IMILJCHD@ignoring?5a?5message_server_switch@ (0000)
00282590 0055:
	??_C@_0FF@LPOEGCEN@failed?5to?5handle?5a?5message_serve@ (0000)
002825E8 002f:
	??_C@_0CP@KPJNEAOB@network_game_client_switch_to_pr@ (0000)
00282618 003b:
	??_C@_0DL@DIFCCPGE@failed?5to?5decode?5a?5message_serve@ (0000)
00282658 0057:
	??_C@_0FH@MFCGNFNN@ignoring?5a?5message_server_gracef@ (0000)
002826B0 005f:
	??_C@_0FP@KCFNLJEN@failed?5to?5handle?5a?5message_serve@ (0000)
00282710 0054:
	??_C@_0FE@LHBDFEJN@failed?5to?5decode?5a?5message_serve@ (0000)
00282764 003a:
	??_C@_0DK@BPCDJE@client?5received?5a?5malformed?1dama@ (0000)
002827A0 0039:
	??_C@_0DJ@ILEOJBLB@client?5received?5low?9level?5error?5@ (0000)
002827DC 0038:
	??_C@_0DI@MKDILEED@client?5received?5a?5bad?5message?5ty@ (0000)
00282814 0037:
	??_C@_0DH@DOCAAFEI@unknown?5packet?5type?5received?5fro@ (0000)
00282850 004f:
	??_C@_0EP@PCPAJAJP@network_game_client_handle_messa@ (0000)
002828A0 0045:
	??_C@_0EF@GBNHBHMJ@network_game_client_handle_messa@ (0000)
002828E8 003d:
	??_C@_0DN@ODDAPCAH@network_game_client_handle_messa@ (0000)
00282928 0048:
	??_C@_0EI@EDFDAGJM@network_game_client_handle_messa@ (0000)
00282970 0045:
	??_C@_0EF@HHBDNBDF@network_game_client_handle_messa@ (0000)
002829B8 003f:
	??_C@_0DP@KPKJEOKA@network_game_client_handle_messa@ (0000)
002829F8 004e:
	??_C@_0EO@BMIPGLHI@network_game_client_handle_messa@ (0000)
00282A48 003e:
	??_C@_0DO@CAKHJPLK@network_game_client_handle_messa@ (0000)
00282A88 0047:
	??_C@_0EH@IEIFIIEF@network_game_client_handle_messa@ (0000)
00282AD0 0046:
	??_C@_0EG@NOAJAKOF@network_game_client_handle_messa@ (0000)
00282B18 0045:
	??_C@_0EF@OEBFMPPP@network_game_client_handle_messa@ (0000)
00282B60 0048:
	??_C@_0EI@JILEDHBD@network_game_client_handle_messa@ (0000)
00282BA8 0044:
	??_C@_0EE@JFCCKCCI@network_game_client_handle_messa@ (0000)
00282BF0 0044:
	??_C@_0EE@DBJLIDDD@network_game_client_handle_messa@ (0000)
00282C34 0038:
	??_C@_0DI@MEGCHKLI@network_game_client_handle_messa@ (0000)
00282C70 0042:
	??_C@_0EC@DCBNODDF@network_game_client_handle_messa@ (0000)
00282CB4 0037:
	??_C@_0DH@OBKILCJH@client?5received?5a?5message?5with?5a@ (0000)
00282CEC 0032:
	??_C@_0DC@OIBJKL@client?5received?5client?5message?5w@ (0000)
00282D20 0054:
	??_C@_0FE@FDBDJMOG@client?5?$CG?$CG?5message?5?$CG?$CG?5?$CImessage_si@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/message_header.h"
#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint_winsock.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "networking/network_client_manager.h"
#include "networking/network_client_message_handler.h"
#include "networking/network_messages.h"

#ifdef HALO_LINUX
/* port/linux/game/network_distributed.c's */
void network_distributed_handle_message(long machine_index, word const *message, word size);
#endif

/* ---------- constants */

#define NETWORK_CLIENT_MESSAGE_HANDLER_FILE "c:\\halo\\SOURCE\\networking\\network_client_message_handler.c"

enum
{
#ifdef HALO_LINUX
	/* the native builds' protocol and session limits
	(port/linux/include/halo_port_limits.h) */
	NETWORK_GAME_MESSAGE_VERSION = HALO_PORT_NETWORK_GAME_MESSAGE_VERSION,
	TRANSPORT_NONCE_LENGTH = 8,
	TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH = 0x80,
	MAXIMUM_NETWORK_MACHINE_COUNT = HALO_PORT_MAXIMUM_NETWORK_MACHINES,
	NETWORK_GAME_NAME_LENGTH = 16,
	MAXIMUM_NUMBER_OF_PLAYERS = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
#else
	NETWORK_GAME_MESSAGE_VERSION = 1,
	TRANSPORT_NONCE_LENGTH = 8,
	TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH = 0x80,
	MAXIMUM_NETWORK_MACHINE_COUNT = 4,
	NETWORK_GAME_NAME_LENGTH = 16,
	MAXIMUM_NUMBER_OF_PLAYERS = 16,
#endif
	JOIN_GAME_TOKEN_LENGTH = 16,
};

#define MINIMUM_TRANSPORT_ERROR_MESSAGE_SIZE (sizeof(word) + TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH + sizeof(byte))

enum
{
	_message_type_error = 1,
	_message_type_data = 2,
};

enum network_game_client_state
{
	_network_game_client_state_searching,
	_network_game_client_state_joining,
	_network_game_client_state_pregame,
	_network_game_client_state_ingame,
	_network_game_client_state_postgame,
	NUMBER_OF_NETWORK_GAME_CLIENT_STATES,
};

enum network_game_packet_class
{
	_network_game_packet_class_advertisement = 1,
	_network_game_packet_class_pregame = 2,
	_network_game_packet_class_ingame = 4,
	_network_game_packet_class_postgame = 6,
};

/* ---------- macros */

/* ---------- structures */

struct network_game_client;

struct network_machine
{
	wchar_t name[32];
	char machine_index;
	byte padding41[3];
};

struct network_game_map
{
	long unknown;
	char name[0x80];
};

struct network_game_local_data
{
	boolean game_objects_loaded;
	byte padding431[3];
};

struct network_game
{
	wchar_t name[NETWORK_GAME_NAME_LENGTH];
	struct network_game_map map;
	struct game_variant variant;
	byte unknown10C;
	char minimum_player_count;
#ifdef HALO_LINUX
	/* 128 does not fit a signed char */
	byte maximum_player_count;
#else
	char maximum_player_count;
#endif
	byte team_count;
	short difficulty;
	short machine_count;
	struct network_machine machines[MAXIMUM_NETWORK_MACHINE_COUNT];
	short player_count;
	struct network_player players[MAXIMUM_NUMBER_OF_PLAYERS];
	word reserved_after_players;
	unsigned long random_seed;
	long number_of_games_played;
	struct network_game_local_data local_data;
};

#ifdef HALO_LINUX
typedef char network_game_players_offset_assert[
	offsetof(struct network_game, players) == HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET ? 1 : -1];
typedef char network_game_size_assert[
	sizeof(struct network_game) == HALO_PORT_NETWORK_GAME_SIZE ? 1 : -1];

/* one piece of the game settings record, which no longer fits one message
(network_server_message_handler.c sends them in order) */
struct message_server_game_settings_update
{
	word total_size;
	word offset;
	word length;
	word pad;
	byte data[HALO_PORT_NETWORK_GAME_SETTINGS_FRAGMENT_SIZE];
};
#endif

struct message_server_game_advertise
{
	byte client_nonce[TRANSPORT_NONCE_LENGTH];
	byte nonce[TRANSPORT_NONCE_LENGTH];
	XNKID key_id;
	XNKEY key;
	XNADDR xnaddr;
	word port;
	word version;
	word platform;
	wchar_t game_name[NETWORK_GAME_NAME_LENGTH];
	byte reserved[0x1A];
	struct network_game_map map;
	short engine_type;
	short machine_count;
	short player_count;
	short maximum_player_count;
	short variant_setting;
	word flags;
	byte join_game_token[JOIN_GAME_TOKEN_LENGTH];
};

struct message_server_pong
{
	long timestamp;
};

struct message_server_machine_accepted
{
	long random_seed;
	short machine_index;
	byte padding6[2];
};

struct message_server_machine_rejected
{
	short reason;
};

struct message_server_pregame_countdown
{
	short seconds_remaining;
};

struct message_server_pregame_keep_alive
{
	short unused;
};

struct message_server_postgame_keep_alive
{
	short unused;
};

struct message_server_begin_game
{
	long unused;
};

struct message_server_graceful_game_exit_pregame
{
	long unused;
};

struct message_server_game_update
{
	unsigned long update_number;
	long random_seed;
	long game_time;
	byte padding0C[2];
	short local_player_count;
	struct player_action player_actions[MAXIMUM_NUMBER_OF_PLAYERS];
};

struct message_server_remove_player_ingame
{
	struct network_player player;
	long reason;
};

struct message_server_game_over
{
	long unused;
};

struct message_server_switch_to_pregame
{
	long unused;
};

struct message_server_graceful_game_exit_postgame
{
	long unused;
};

/* ---------- prototypes */

static boolean network_game_client_handle_message_server_game_advertise(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_pong(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_machine_accepted(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_machine_rejected(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_game_settings_update(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_pregame_countdown(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_pregame_keep_alive(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_postgame_keep_alive(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_begin_game(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_graceful_game_exit_pregame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_game_update(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_add_player_ingame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_remove_player_ingame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_game_over(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_switch_to_pregame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);
static boolean network_game_client_handle_message_server_graceful_game_exit_postgame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address);

/* ---------- globals */

/* ---------- public code */

boolean network_game_client_handle_message(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = TRUE;
	word message_type;
	byte packet_type;

	match_assert(
		NETWORK_CLIENT_MESSAGE_HANDLER_FILE,
		0x2F,
		client && message && (message_size == GET_MESSAGE_SIZE(*message)) && source_address);

	message_type = (byte)GET_MESSAGE_TYPE(*message);
	if (GET_MESSAGE_FLAGS(*message))
	{
		network_event("client received client message with invalid flags");
	}
	else
	{
		switch (message_type)
		{
			case _message_type_packet:
				packet_type = ((byte *)message)[message_size - 1];
				switch (packet_type)
				{
					case _message_server_game_advertise:
						result = network_game_client_handle_message_server_game_advertise(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_game_advertise() failed");
						}
						break;

					case _message_server_pong:
						result = network_game_client_handle_message_server_pong(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_pong() failed");
						}
						break;

					case _message_server_machine_accepted:
						result = network_game_client_handle_message_server_machine_accepted(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_machine_accepted() failed");
						}
						break;

					case _message_server_machine_rejected:
						result = network_game_client_handle_message_server_machine_rejected(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_machine_rejected() failed");
						}
						break;

					case _message_server_game_settings_update:
						result = network_game_client_handle_message_server_game_settings_update(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_game_settings_update() failed");
						}
						break;

					case _message_server_pregame_countdown:
						result = network_game_client_handle_message_server_pregame_countdown(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_pregame_countdown() failed");
						}
						break;

					case _message_server_pregame_keep_alive:
						result = network_game_client_handle_message_server_pregame_keep_alive(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_pregame_keep_alive() failed");
						}
						break;

					case _message_server_postgame_keep_alive:
						result = network_game_client_handle_message_server_postgame_keep_alive(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_postgame_keep_alive() failed");
						}
						break;

					case _message_server_begin_game:
						result = network_game_client_handle_message_server_begin_game(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_begin_game() failed");
						}
						break;

					case _message_server_graceful_game_exit_pregame:
						result = network_game_client_handle_message_server_graceful_game_exit_pregame(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_graceful_game_exit_pregame() failed");
						}
						break;

					case _message_server_game_update:
						result = network_game_client_handle_message_server_game_update(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_game_update() failed");
						}
						break;

					case _message_server_add_player_ingame:
						result = network_game_client_handle_message_server_add_player_ingame(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_add_player_ingame() failed");
						}
						break;

					case _message_server_remove_player_ingame:
						result = network_game_client_handle_message_server_remove_player_ingame(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_remove_player_ingame() failed");
						}
						break;

					case _message_server_game_over:
						result = network_game_client_handle_message_server_game_over(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_game_over() failed");
						}
						break;

					case _message_server_switch_to_pregame:
						result = network_game_client_handle_message_server_switch_to_pregame(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_switch_to_pregame() failed");
						}
						break;

					case _message_server_graceful_game_exit_postgame:
						result = network_game_client_handle_message_server_graceful_game_exit_postgame(
							client,
							message,
							message_size,
							source_address);
						if (!result)
						{
							network_event("network_game_client_handle_message_server_graceful_game_exit_postgame() failed");
						}
						break;

					default:
						network_event(
							"unknown packet type received from system @ address: %s",
							transport_address_to_string(source_address));
						break;
				}
				break;

			case _message_type_data:
#ifdef HALO_LINUX
				/* the distributed netcode's messages (port/linux/NETCODE.md).
				(Not checked against the host's address: over internet play an
				unreliable message's source is the tunnel's local stand-in, which
				the connected socket's peer name does not translate back to the
				host's, so the check dropped every per-tick state; see
				port/xbox/README.md, Bug audit) */
				network_distributed_handle_message(NONE, message, message_size);
#else
				network_event("client received a bad message type (_message_type_data)");
#endif
				break;

			case _message_type_error:
				if (message_size >= MINIMUM_TRANSPORT_ERROR_MESSAGE_SIZE)
				{
					byte *error_message = (byte *)(message + 1);

					network_event(
						"client received low-level error message: error= #%d (%s)",
						error_message[TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH],
						error_message);
				}
				else
				{
					network_event("client received a malformed/damaged message from a server");
				}
				break;

			default:
				network_event("client received a message with an unknown message type");
				break;
		}
	}

	return result;
}

/* ---------- private code */

static boolean network_game_client_handle_message_server_game_advertise(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	if (network_game_client_get_state(client, NULL) == _network_game_client_state_searching)
	{
		struct message_server_game_advertise advertisement;
		short packet_type = _message_server_game_advertise;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&advertisement,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_advertisement))
		{
			if (transport_is_nonce(&advertisement, TRANSPORT_NONCE_LENGTH))
			{
				network_game_client_new_advertised_game(client, &advertisement);
			}
		}
		else
		{
			network_event("failed to decode a message_server_game_advertise packet");
		}
	}
	else
	{
		network_event("ignoring an advertised game because we are not looking for new games");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_pong(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	if (network_game_client_get_state(client, NULL) == _network_game_client_state_searching)
	{
		struct message_server_pong pong;
		short packet_type = _message_server_pong;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&pong,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_advertisement))
		{
			network_game_client_ponged(client, source_address, pong.timestamp);
		}
		else
		{
			network_event("failed to decode a message_server_pong packet");
		}
	}
	else
	{
		network_event("ignoring a pong message because we are not listening for them");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_machine_accepted(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = TRUE;

	if (network_game_client_address_matches_server(client, source_address) &&
		network_game_client_get_state(client, NULL) == _network_game_client_state_joining)
	{
		struct message_server_machine_accepted acceptance;
		short packet_type = _message_server_machine_accepted;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&acceptance,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_pregame))
		{
			network_game_client_accepted_into_game(client, source_address, &acceptance);
		}
		else
		{
			network_event("failed to decode a message_server_machine_accepted packet");
			result = FALSE;
		}
	}
	else
	{
		network_event("ignoring a message_server_machine_accepted message; either a bad machine or we aren't joining");
		result = FALSE;
	}

	return result;
}

static boolean network_game_client_handle_message_server_machine_rejected(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = TRUE;

	if (network_game_client_address_matches_server(client, source_address) &&
		network_game_client_get_state(client, NULL) == _network_game_client_state_joining)
	{
		struct message_server_machine_rejected rejection;
		short packet_type = _message_server_machine_rejected;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&rejection,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_pregame))
		{
			network_game_client_rejected_by_game(client, source_address, rejection.reason);
		}
		else
		{
			network_event("failed to decode a message_server_machine_rejected packet");
			result = FALSE;
		}
	}
	else
	{
		network_event("ignoring a message_server_machine_rejected message; either a bad machine or we aren't joining");
		result = FALSE;
	}

	return result;
}

#ifdef HALO_LINUX
/* the game settings record as its pieces arrive; it is applied once the last
piece is in */
static struct network_game network_game_client_settings_staging;
static long network_game_client_settings_staging_size = 0;

static boolean network_game_client_receive_game_settings_piece(
	struct network_game_client *client,
	struct message_server_game_settings_update const *piece)
{
	boolean result = TRUE;

	if (piece->total_size != sizeof(network_game_client_settings_staging) ||
		piece->length > sizeof(piece->data) ||
		piece->offset + piece->length > sizeof(network_game_client_settings_staging))
	{
		network_event("got a message_server_game_settings_update piece for a different game layout");
		network_game_client_settings_staging_size = 0;
		result = FALSE;
	}
	else
	{
		/* each record is sent in order from its start, over a stream */
		if (piece->offset == 0)
		{
			network_game_client_settings_staging_size = 0;
		}
		if (piece->offset != network_game_client_settings_staging_size)
		{
			network_event("got a message_server_game_settings_update piece out of order; waiting for the next record");
			network_game_client_settings_staging_size = 0;
		}
		else
		{
			csmemcpy(
				(byte *)&network_game_client_settings_staging + piece->offset,
				piece->data,
				piece->length);
			network_game_client_settings_staging_size += piece->length;
			if (network_game_client_settings_staging_size == piece->total_size)
			{
				network_game_client_settings_staging_size = 0;
				result = network_game_client_game_settings_updated(client, &network_game_client_settings_staging);
				if (!result)
				{
					network_event("network_game_client_game_settings_updated() failed");
				}
			}
		}
	}

	return result;
}
#endif

static boolean network_game_client_handle_message_server_game_settings_update(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x169, client != NULL);
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x16A, source_address != NULL);

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_pregame)
		{
#ifdef HALO_LINUX
			struct message_server_game_settings_update piece;
			short packet_type = _message_server_game_settings_update;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&piece,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				result = network_game_client_receive_game_settings_piece(client, &piece);
			}
			else
			{
				network_event("failed to decode a message_server_game_settings_update packet");
			}
#else
			struct network_game game_settings;
			short packet_type = _message_server_game_settings_update;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&game_settings,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				result = network_game_client_game_settings_updated(client, &game_settings);
				if (!result)
				{
					network_event("network_game_client_game_settings_updated() failed");
				}
			}
			else
			{
				network_event("failed to decode a message_server_game_settings_update packet");
			}
#endif
		}
		else
		{
			network_event("failed to handle a message_server_game_settings_update message; not in pregame state");
			result = TRUE;
		}
	}
	else
	{
		network_event("ignoring a message_server_game_settings_update; came from a bad machine");
		result = TRUE;
	}

	return result;
}

static boolean network_game_client_handle_message_server_pregame_countdown(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x19B, client != NULL);
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x19C, source_address != NULL);

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_pregame)
		{
			struct message_server_pregame_countdown countdown;
			short packet_type = _message_server_pregame_countdown;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&countdown,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				network_game_client_countdown_timer_update(client, countdown.seconds_remaining);
			}
			else
			{
				network_event("failed to decode a message_server_pregame_countdown packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_pregame_countdown message; not in pregame state");
		}
	}
	else
	{
		network_event("ignoring a message_server_pregame_countdown; came from a bad machine");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_pregame_keep_alive(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x1C4, client != NULL);
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x1C5, source_address != NULL);

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_pregame)
		{
			struct message_server_pregame_keep_alive keep_alive;
			short packet_type = _message_server_pregame_keep_alive;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (!decode_network_game_message(
				&keep_alive,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				network_event("failed to decode a message_server_pregame_keep_alive packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_pregame_keep_alive message; not in pregame state");
		}
	}
	else
	{
		network_event("ignoring a message_server_pregame_keep_alive; came from a bad machine");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_postgame_keep_alive(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x1EE, client != NULL);
	match_assert(NETWORK_CLIENT_MESSAGE_HANDLER_FILE, 0x1EF, source_address != NULL);

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_postgame)
		{
			struct message_server_postgame_keep_alive keep_alive;
			short packet_type = _message_server_postgame_keep_alive;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (!decode_network_game_message(
				&keep_alive,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_postgame))
			{
				network_event("failed to decode a message_server_postgame_keep_alive packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_postgame_keep_alive message; not in postgame state");
		}
	}
	else
	{
		network_event("ignoring a message_server_postgame_keep_alive; came from a bad machine");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_begin_game(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_pregame)
		{
			struct message_server_begin_game begin_game;
			short packet_type = _message_server_begin_game;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&begin_game,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				result = network_game_client_game_has_started(client);
				if (!result)
				{
					network_event("network_game_client_game_has_started() failed");
				}
			}
			else
			{
				network_event("failed to decode a message_server_begin_game packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_begin_game message; we are not in pregame");
		}
	}
	else
	{
		network_event("ignoring a message_server_begin_game message; came from a bad machine");
		result = TRUE;
	}

	return result;
}

static boolean network_game_client_handle_message_server_graceful_game_exit_pregame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_pregame)
		{
			struct message_server_graceful_game_exit_pregame game_exit;
			short packet_type = _message_server_graceful_game_exit_pregame;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&game_exit,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_pregame))
			{
				network_game_client_game_shutdown(client);
			}
			else
			{
				network_event("failed to decode a message_server_graceful_game_exit_pregame packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_graceful_game_exit_pregame message; we are not in pregame");
		}
	}
	else
	{
		network_event("ignoring a message_server_graceful_game_exit_pregame message; came from a bad machine");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_game_update(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_ingame)
		{
			struct message_server_game_update game_update;
			short packet_type = _message_server_game_update;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&game_update,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_ingame))
			{
				result = network_game_client_handle_game_update(client, &game_update);
				if (!result)
				{
					network_event("network_game_client_handle_game_update() failed");
				}
			}
			else
			{
				network_event("failed to decode a message_server_game_update packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_game_update message; we are not in game");
		}
	}
	else
	{
		network_event("ignoring a message_server_game_update message; came from a bad machine");
		result = TRUE;
	}

	if (!result)
	{
		network_game_client_game_out_of_sync(client);
	}

	return result;
}

static boolean network_game_client_handle_message_server_add_player_ingame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_ingame)
		{
			struct network_player player;
			short packet_type = _message_server_add_player_ingame;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&player,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_ingame))
			{
				result = network_game_client_add_player_to_game(client, &player);
				if (!result)
				{
					network_event("network_game_client_add_player_to_game() failed");
				}
			}
			else
			{
				network_event("failed to decode a message_server_add_player_ingame packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_add_player_ingame message; we are not in game");
		}
	}
	else
	{
		network_event("ignoring a message_server_add_player_ingame message; came from a bad machine");
		result = TRUE;
	}

	if (!result)
	{
		network_game_client_game_out_of_sync(client);
	}

	return result;
}

static boolean network_game_client_handle_message_server_remove_player_ingame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_ingame)
		{
			struct message_server_remove_player_ingame removal;
			short packet_type = _message_server_remove_player_ingame;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (decode_network_game_message(
				&removal,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_ingame))
			{
				result = network_game_client_remove_player(client, &removal.player, removal.reason);
				if (!result)
				{
					network_event("network_game_client_remove_player() failed");
				}
			}
			else
			{
				network_event("failed to decode a message_server_remove_player_ingame packet");
			}
		}
		else
		{
			network_event("failed to handle a message_server_remove_player_ingame message; we are not in game");
		}
	}
	else
	{
		network_event("ignoring a message_server_remove_player_ingame message; came from a bad machine");
		result = TRUE;
	}

	if (!result)
	{
		network_game_client_game_out_of_sync(client);
	}

	return result;
}

static boolean network_game_client_handle_message_server_game_over(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_ingame)
		{
			struct message_server_game_over game_over;
			short packet_type = _message_server_game_over;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (!decode_network_game_message(
				&game_over,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_ingame))
			{
				network_event("failed to decode a message_server_game_over message (not critical)");
			}

			network_game_client_switch_to_postgame(client);
		}
		else
		{
			network_event("failed to handle a message_server_game_over message; we are not in game");
		}
	}
	else
	{
		network_event("ignoring a message_server_game_over message; came from a bad machine");
	}

	return TRUE;
}

static boolean network_game_client_handle_message_server_switch_to_pregame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_postgame)
		{
			struct message_server_switch_to_pregame switch_to_pregame;
			short packet_type = _message_server_switch_to_pregame;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (!decode_network_game_message(
				&switch_to_pregame,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_postgame))
			{
				network_event("failed to decode a message_server_switch_to_pregame packet");
			}

			result = network_game_client_switch_to_pregame(client);
			if (!result)
			{
				network_event("network_game_client_switch_to_pregame() failed");
			}
		}
		else
		{
			network_event("failed to handle a message_server_switch_to_pregame message; we are not in post-game");
		}
	}
	else
	{
		network_event("ignoring a message_server_switch_to_pregame message; came from a bad machine");
		result = TRUE;
	}

	return result;
}

static boolean network_game_client_handle_message_server_graceful_game_exit_postgame(
	struct network_game_client *client,
	word *message,
	short message_size,
	struct transport_address *source_address)
{
	boolean result = FALSE;

	if (network_game_client_address_matches_server(client, source_address))
	{
		if (network_game_client_get_state(client, NULL) == _network_game_client_state_postgame)
		{
			struct message_server_graceful_game_exit_postgame game_exit;
			short packet_type = _message_server_graceful_game_exit_postgame;
			short packet_version = NETWORK_GAME_MESSAGE_VERSION;

			message_size -= sizeof(word);
			if (!decode_network_game_message(
				&game_exit,
				message + 1,
				&message_size,
				&packet_type,
				&packet_version,
				_network_game_packet_class_postgame))
			{
				network_event("failed to decode a message_server_graceful_game_exit_postgame packet (not critical)");
			}

			network_game_client_game_shutdown(client);
		}
		else
		{
			network_event("failed to handle a message_server_graceful_game_exit_postgame message; we are not in post-game");
		}
	}
	else
	{
		network_event("ignoring a message_server_graceful_game_exit_postgame message; came from a bad machine");
		result = TRUE;
	}

	return result;
}
