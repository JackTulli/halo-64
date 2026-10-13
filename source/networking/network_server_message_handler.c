/*
NETWORK_SERVER_MESSAGE_HANDLER.C

symbols in this file:
0011E890 0090:
	_code_0011e890 (0000)
0011E920 00a0:
	_code_0011e920 (0000)
0011E9C0 0090:
	_code_0011e9c0 (0000)
0011EA50 0090:
	_code_0011ea50 (0000)
0011EAE0 00a0:
	_code_0011eae0 (0000)
0011EB80 00a0:
	_code_0011eb80 (0000)
0011EC20 0020:
	_code_0011ec20 (0000)
0011EC40 0040:
	_network_game_server_send_message_to_machine (0000)
0011EC80 0110:
	_network_game_server_send_message_to_all_machines (0000)
0011ED90 0090:
	_network_game_server_send_player_joined_info_ingame (0000)
0011EE20 00c0:
	_network_game_server_send_game_data_pregame (0000)
0011EEE0 0240:
	_code_0011eee0 (0000)
0011F120 00c0:
	_code_0011f120 (0000)
0011F1E0 03f0:
	_code_0011f1e0 (0000)
0011F5D0 00c0:
	_code_0011f5d0 (0000)
0011F690 00c0:
	_code_0011f690 (0000)
0011F750 00e0:
	_code_0011f750 (0000)
0011F830 00d0:
	_code_0011f830 (0000)
0011F900 00d0:
	_code_0011f900 (0000)
0011F9D0 00f0:
	_code_0011f9d0 (0000)
0011FAC0 0310:
	_network_game_server_handle_datagram (0000)
0011FDD0 0400:
	_network_game_server_handle_client_message (0000)
00286F00 005a:
	??_C@_0FK@KMJJAKFL@failed?5to?5handle?5a?5message_clien@ (0000)
00286F60 0043:
	??_C@_0ED@BONJCMOD@server?5failed?5to?5decode?5a?5messag@ (0000)
00286FA8 0065:
	??_C@_0GF@BGPKJGEH@failed?5to?5handle?5a?5message_type_@ (0000)
00287010 004e:
	??_C@_0EO@HMJBCLNP@server?5failed?5to?5decode?5a?5messag@ (0000)
00287060 0056:
	??_C@_0FG@LPGCCINN@failed?5to?5handle?5a?5message_clien@ (0000)
002870B8 0037:
	??_C@_0DH@MMOKOPCN@server?5failed?5to?5decode?5a?5messag@ (0000)
002870F0 005e:
	??_C@_0FO@NHCIMMDF@failed?5to?5handle?5a?5message_clien@ (0000)
00287150 004a:
	??_C@_0EK@ONPDOJKK@server?5failed?5to?5decode?5a?5messag@ (0000)
002871A0 0068:
	??_C@_0GI@DLKCJODI@failed?5to?5handle?5a?5message_clien@ (0000)
00287208 004f:
	??_C@_0EP@DAOANLDM@server?5failed?5to?5decode?5a?5messag@ (0000)
00287258 0033:
	??_C@_0DD@FJKDMFNA@server?5failed?5to?5remove?5a?5networ@ (0000)
00287290 005b:
	??_C@_0FL@NDHDFCNK@failed?5to?5handle?5a?5message_clien@ (0000)
002872F0 0045:
	??_C@_0EF@MJLJMBIP@network_game_server_switch_machi@ (0000)
00287338 0059:
	??_C@_0FJ@PEAAKLFN@network_game_server_write?$CI?$CJ?5fail@ (0000)
00287394 0027:
	??_C@_0CH@EEEHHOGO@message_length?$DM?$DNsizeof?$CImessage_b@ (0000)
002873BC 0012:
	??_C@_0BC@FDHBPMJE@server?5?$CG?$CG?5message?$AA@ (0000)
002873D0 003b:
	??_C@_0DL@ICDCPLFI@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
0028740C 003c:
	??_C@_0DM@MOMFHJIG@failed?5to?5create?5a?5message_serve@ (0000)
00287448 0072:
	??_C@_0HC@FGJDKKLC@network_game_server_send_message@ (0000)
002874C0 0058:
	??_C@_0FI@LBJCENJB@failed?5to?5handle?5a?5message_serve@ (0000)
00287518 003f:
	??_C@_0DP@CCCLFFGC@failed?5to?5create?5a?5message_serve@ (0000)
00287558 004b:
	??_C@_0EL@NFHCHAEG@failed?5to?5send?5message_server_ga@ (0000)
002875A4 0039:
	??_C@_0DJ@PCNJDMDI@failed?5to?5create?5a?5message_serve@ (0000)
002875E0 0054:
	??_C@_0FE@HMKEDIDF@network_game_server_write?$CI?$CJ?5fail@ (0000)
00287634 002b:
	??_C@_0CL@FOJJMNII@server?5?$CG?$CG?5source_address?5?$CG?$CG?5clie@ (0000)
00287660 002f:
	??_C@_0CP@JCCANCCC@failed?5to?5create?5a?5message_serve@ (0000)
00287690 0043:
	??_C@_0ED@LGNNJLKJ@network_game_server_write?$CI?$CJ?5fail@ (0000)
002876D8 0042:
	??_C@_0EC@MGMKMMHI@server?5failed?5to?5decode?5a?5messag@ (0000)
0028771C 003b:
	??_C@_0DL@JAHACCAG@failed?5to?5create?5a?5message_serve@ (0000)
00287758 0043:
	??_C@_0ED@PLLDKIEI@client?5machine?5?8?$CFs?8?5?$EA?$CFs?5tried?5to@ (0000)
002877A0 0041:
	??_C@_0EB@ONCPMINI@client?5machine?5?8?$CFs?8?5?$EA?$CFs?5tried?5to@ (0000)
002877E8 0043:
	??_C@_0ED@DKNIPKFL@network_game_server_write?$CI?$CJ?5fail@ (0000)
00287830 0044:
	??_C@_0EE@MGEHHOCB@server?5failed?5to?5accept?5valid?5cl@ (0000)
00287878 0075:
	??_C@_0HF@HFOMEJBD@network_game_server_send_game_da@ (0000)
002878F0 0039:
	??_C@_0DJ@CKOCAMEF@sent?5_message_type_server_machin@ (0000)
00287930 0064:
	??_C@_0GE@MLODGBHN@network_game_server_write?$CI?$CJ?5fail@ (0000)
00287994 0029:
	??_C@_0CJ@EPMEBAJK@network_machine_is_valid?$CIclient_@ (0000)
002879C0 0040:
	??_C@_0EA@HPKAIHHK@server?5refused?5client?5?8?$CFs?8?5becau@ (0000)
00287A00 000d:
	??_C@_0N@MPMCKMFC@d?3?2hosts?4txt?$AA@ (0000)
00287A10 002d:
	??_C@_0CN@LBHCOBFK@ignoring?5redundant?5join?5request?5@ (0000)
00287A40 0062:
	??_C@_0GC@IBIEHGFG@failed?5to?5handle?5a?5message_clien@ (0000)
00287AA8 004b:
	??_C@_0EL@BFEPJEFO@server?5failed?5to?5decode?5a?5messag@ (0000)
00287AF8 0070:
	??_C@_0HA@DHHGOECG@server?5failed?5to?5add?5a?5network?5p@ (0000)
00287B68 0072:
	??_C@_0HC@LODCIJJJ@server?5failed?5to?5send?5pregame?5ga@ (0000)
00287BE0 0065:
	??_C@_0GF@EKICMCEL@failed?5to?5handle?5a?5message_clien@ (0000)
00287C48 004e:
	??_C@_0EO@JJCAGDLG@server?5failed?5to?5decode?5a?5messag@ (0000)
00287C98 0076:
	??_C@_0HG@LNIKCCLC@server?5failed?5to?5remove?5a?5networ@ (0000)
00287D10 0075:
	??_C@_0HF@KPAHGLDA@server?5failed?5to?5send?5pregame?5ga@ (0000)
00287D88 0058:
	??_C@_0FI@HEMIANGD@failed?5to?5handle?5a?5message_clien@ (0000)
00287DE0 0041:
	??_C@_0EB@EACNFEBF@server?5failed?5to?5decode?5a?5messag@ (0000)
00287E28 0075:
	??_C@_0HF@CJBDOMOO@network_game_server_adjust_machi@ (0000)
00287EA0 0068:
	??_C@_0GI@IHEHKNKB@server?5failed?5to?5send?5pregame?5ga@ (0000)
00287F08 0036:
	??_C@_0DG@ECGMMKBK@server?5received?5machine?5settings@ (0000)
00287F40 005f:
	??_C@_0FP@BKMGBKPC@failed?5to?5handle?5a?5message_clien@ (0000)
00287FA0 0048:
	??_C@_0EI@NDLJFMCJ@server?5failed?5to?5decode?5a?5messag@ (0000)
00287FE8 006b:
	??_C@_0GL@MGDAOAGD@network_game_update_player?$CI?$CJ?5fai@ (0000)
00288058 006f:
	??_C@_0GP@IGEJNGEG@server?5failed?5to?5send?5pregame?5ga@ (0000)
002880C8 0028:
	??_C@_0CI@NAMNDBJF@server?5received?5updated?5player?5s@ (0000)
002880F0 006a:
	??_C@_0GK@PIEMGCFK@failed?5to?5handle?5a?5message_clien@ (0000)
00288160 004b:
	??_C@_0EL@FGPNNAHJ@server?5failed?5to?5decode?5a?5messag@ (0000)
002881B0 0080:
	??_C@_0IA@GMAELFDC@network_game_server_remove_machi@ (0000)
00288230 0072:
	??_C@_0HC@JPOEIOK@server?5failed?5to?5send?5pregame?5ga@ (0000)
002882A8 0061:
	??_C@_0GB@BICEKEIA@failed?5to?5handle?5a?5message_clien@ (0000)
00288310 004d:
	??_C@_0EN@FBHIPCPE@server?5failed?5to?5decode?5a?5messag@ (0000)
00288360 003a:
	??_C@_0DK@HGNCDCGJ@server?5received?5a?5malformed?1dama@ (0000)
002883A0 0047:
	??_C@_0EH@HDCJJGCF@server?5received?5low?9level?5error?5@ (0000)
002883E8 0046:
	??_C@_0EG@NGIBKKFK@server?5received?5a?5bad?5message?5ty@ (0000)
00288430 003f:
	??_C@_0DP@DJCDAEKP@failed?5to?5decode?5a?5message_clien@ (0000)
00288470 003e:
	??_C@_0DO@EMEMPFEN@server?5failed?5to?5advertise?5game?5@ (0000)
002884B0 002e:
	??_C@_0CO@IFNMPAGP@failed?5to?5decode?5a?5message_clien@ (0000)
002884E0 0026:
	??_C@_0CG@PBGOHBCL@server?5failed?5to?5handle?5a?5client@ (0000)
00288508 0042:
	??_C@_0EC@OIEIALKJ@ignoring?5a?5message_client_game_u@ (0000)
00288550 0062:
	??_C@_0GC@EMAKPFP@failed?5to?5handle?5a?5message_clien@ (0000)
002885B4 0035:
	??_C@_0DF@NPLDDCCG@failed?5to?5decode?5a?5message_clien@ (0000)
002885F0 0046:
	??_C@_0EG@DEJFBBLL@server?5received?5datagram?5with?5an@ (0000)
00288638 004c:
	??_C@_0EM@NPFBANHI@server?5received?5a?5datagram?5with?5@ (0000)
00288684 003c:
	??_C@_0DM@HFFMPMPG@server?5received?5a?5datagram?5with?5@ (0000)
002886C0 0081:
	??_C@_0IB@OPFACLKA@server?5?$CG?$CG?5message?5?$CG?$CG?5source_addr@ (0000)
00288744 003a:
	??_C@_0DK@NEANDKEL@server?5received?5a?5malformed?1dama@ (0000)
00288780 0047:
	??_C@_0EH@PCKBOEPO@server?5received?5low?9level?5error?5@ (0000)
002887C8 0046:
	??_C@_0EG@GPPJDFA@server?5received?5a?5bad?5message?5ty@ (0000)
00288810 003e:
	??_C@_0DO@LBJNGIAJ@bad?5or?5inappropriate?5packet?5type@ (0000)
00288850 004e:
	??_C@_0EO@OPDINPHL@network_game_server_handle_messa@ (0000)
002888A0 0045:
	??_C@_0EF@FHPBCOMI@network_game_server_handle_messa@ (0000)
002888E8 0052:
	??_C@_0FC@HBMALEKE@network_game_server_handle_messa@ (0000)
00288940 0050:
	??_C@_0FA@OHGBEJHJ@network_game_server_handle_messa@ (0000)
00288990 004d:
	??_C@_0EN@IOBKAAMM@network_game_server_handle_messa@ (0000)
002889E0 003a:
	??_C@_0DK@DJBGFGMF@network_game_server_handle_messa@ (0000)
00288A20 0046:
	??_C@_0EG@KNLLMEKN@network_game_server_handle_messa@ (0000)
00288A68 004b:
	??_C@_0EL@IBNDPBLB@network_game_server_handle_messa@ (0000)
00288AB8 0044:
	??_C@_0EE@KLMGADOJ@network_game_server_handle_messa@ (0000)
00288B00 0051:
	??_C@_0FB@PPCOJHGP@network_game_server_handle_messa@ (0000)
00288B58 004e:
	??_C@_0EO@EEEMKLCF@network_game_server_handle_messa@ (0000)
00288BA8 0045:
	??_C@_0EF@FHABLMKA@network_game_server_handle_messa@ (0000)
00288BF0 0048:
	??_C@_0EI@INBBOEGP@an?5un?9validated?5client?5sent?5some@ (0000)
00288C38 0044:
	??_C@_0EE@EPDDKPMF@server?5received?5a?5client?5message@ (0000)
00288C7C 0032:
	??_C@_0DC@DAGHEIED@server?5received?5client?5message?5w@ (0000)
00288CB0 0054:
	??_C@_0FE@BPJHKGCF@server?5?$CG?$CG?5machine?5?$CG?$CG?5message?5?$CG?$CG?5@ (0000)
*/


/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/message_header.h"
#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint_winsock.h"
#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "networking/network_connection.h"
#include "networking/network_game_globals.h"
#include "networking/network_game_manager.h"
#include "networking/network_game_protocol.h"
#include "networking/network_messages.h"
#include "networking/network_server_manager_internal.h"
#include "networking/network_server_message_handler.h"
#include "text/unicode.h"
#ifdef HALO_LINUX
/* system_milliseconds(), for the settings update interval */
#include "cseries/cseries_windows.h"
#endif

#ifdef HALO_LINUX
/* port/linux/game/network_distributed.c's */
void network_distributed_handle_message(long machine_index, word const *message, word size);
#endif

/* ---------- constants */

enum
{
#ifdef HALO_LINUX
	/* the native builds' session limits and protocol
	(port/linux/include/halo_port_limits.h) */
	MAXIMUM_NETWORK_MACHINE_COUNT = HALO_PORT_MAXIMUM_NETWORK_MACHINES,
	NETWORK_MESSAGE_BUFFER_SIZE = HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE,
	NETWORK_GAME_MESSAGE_VERSION = HALO_PORT_NETWORK_GAME_MESSAGE_VERSION,
#else
	MAXIMUM_NETWORK_MACHINE_COUNT = 4,
	NETWORK_MESSAGE_BUFFER_SIZE = 0x600,
	NETWORK_GAME_MESSAGE_VERSION = 1,
#endif
	MAXIMUM_MACHINE_NAME_LENGTH = 32,
	NETWORK_GAME_MAP_NAME_LENGTH = 0x100,
	JOIN_GAME_TOKEN_LENGTH = 16,
	MAXIMUM_HOSTS_FILE_LINE_LENGTH = 32,
	TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH = 0x80,
	REMOVE_PLAYER_INGAME_GAME_TIME_DELAY = 33,
	TRANSPORT_NONCE_LENGTH = 8,
	NETWORK_GAME_NAME_LENGTH = 16,
#ifdef HALO_LINUX
	MAXIMUM_NUMBER_OF_PLAYERS = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
#else
	MAXIMUM_NUMBER_OF_PLAYERS = 16,
#endif
};

enum
{
	_game_advertisement_open_bit = 1,
	_game_advertisement_has_teams_bit,
	_game_advertisement_oddball_variant_bit,
};

#define MINIMUM_TRANSPORT_ERROR_MESSAGE_SIZE (sizeof(word) + TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH + sizeof(byte))

enum
{
	_message_type_error = 1,
	_message_type_data = 2,
};

enum network_game_server_state
{
	_network_game_server_state_pregame = 0,
	_network_game_server_state_ingame,
	_network_game_server_state_postgame,
};

enum network_game_packet_class
{
	_network_game_packet_class_client_search = 0,
	_network_game_packet_class_client_pregame = 3,
	_network_game_packet_class_client_ingame = 5,
	_network_game_packet_class_client_postgame = 7,
};

enum network_game_server_rejection_reason
{
	_network_game_server_rejection_reason_bad_join_token = 2,
	_network_game_server_rejection_reason_game_not_open = 5,
	_network_game_server_rejection_reason_not_in_hosts_file = 6,
};

/* ---------- macros */

#define network_machine_is_valid(machine) \
	((machine) && (machine)->machine_index >= 0 && \
	(machine)->machine_index < MAXIMUM_NETWORK_MACHINE_COUNT)

/* ---------- structures */

struct network_connection;
struct network_game_server;
struct network_game_server_client_machine;

struct network_machine
{
	wchar_t name[MAXIMUM_MACHINE_NAME_LENGTH];
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
	byte padding[3];
};

struct network_game
{
	wchar_t name[NETWORK_GAME_NAME_LENGTH];
	struct network_game_map map;
	struct game_variant variant;
	byte unknown;
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
#endif

struct network_message
{
	word header;
};

struct message_client_broadcast_game_search
{
	word port;
	short version;
	byte nonce[TRANSPORT_NONCE_LENGTH];
};

struct message_client_ping
{
	long timestamp;
	short port;
	byte padding[2];
};

struct message_client_game_update
{
	unsigned long update_number;
	short unknown;
	short player_count;
#ifdef HALO_LINUX
	/* a machine's players, which 16 players / 4 machines only happened to give */
	struct player_action actions[MAXIMUM_LOCAL_PLAYERS];
#else
	struct player_action actions[MAXIMUM_NUMBER_OF_PLAYERS / MAXIMUM_NETWORK_MACHINE_COUNT];
#endif
};

struct message_server_pong
{
	long timestamp;
};

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

struct message_client_join_game_request
{
	wchar_t machine_name[MAXIMUM_MACHINE_NAME_LENGTH];
	byte join_game_token[JOIN_GAME_TOKEN_LENGTH];
};

struct message_client_settings_request
{
	wchar_t machine_name[MAXIMUM_MACHINE_NAME_LENGTH];
	char machine_index;
	byte padding[3];
};

struct message_client_game_start_request
{
	long countdown_time;
};

struct message_client_graceful_game_exit_pregame
{
	long unused;
};

struct message_client_map_is_precached_pregame
{
	char map_name[NETWORK_GAME_MAP_NAME_LENGTH];
};

struct message_client_loaded
{
	long unused;
};

struct message_client_switch_to_pregame
{
	long unused;
};

struct message_server_machine_accepted
{
	long random_seed;
	short machine_index;
	byte padding[2];
};

struct message_server_machine_rejected
{
	short reason;
};

#ifdef HALO_LINUX
/* the game settings record no longer fits one message: it goes out as
consecutive pieces, which the clients put back together */
struct message_server_game_settings_update
{
	word total_size;
	word offset;
	word length;
	word pad;
	byte data[HALO_PORT_NETWORK_GAME_SETTINGS_FRAGMENT_SIZE];
};
#else
struct message_server_game_settings_update
{
	struct network_game game;
};
#endif

struct message_server_remove_player_ingame
{
	struct network_player player;
	long game_time;
};

/* ---------- prototypes */

static boolean network_game_server_handle_message_client_broadcast_game_search(
	struct network_game_server *server,
	struct transport_address *source_address,
	struct message_client_broadcast_game_search *client_message);
static boolean network_game_server_handle_message_client_ping(
	struct network_game_server *server,
	struct transport_address *source_address,
	struct message_client_ping *client_message);
static boolean network_game_server_handle_message_client_join_game_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_add_player_request_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_remove_player_request_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_settings_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_player_settings_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_game_start_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_graceful_game_exit_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_map_is_precached_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_loaded(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_add_player_request_ingame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_remove_player_request_ingame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_remove_player_request_postgame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);
static boolean network_game_server_handle_message_client_switch_to_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size);

/* ---------- globals */

/* ---------- private code */

static boolean network_game_server_write(
	struct network_connection *connection,
	void *message,
	word buffer_size,
	struct transport_address *destination_address,
	boolean reliable)
{
	return network_connection_write(
		connection,
		message,
		buffer_size,
		destination_address,
		reliable);
}

/* ---------- public code */

boolean network_game_server_send_message_to_machine(
	struct network_game_server *server,
	struct network_machine *machine,
	struct network_message *message)
{
	boolean result = FALSE;
	struct network_connection *connection =
		network_game_server_get_machine_connection(server, machine);

	if (connection)
	{
		word message_size = GET_MESSAGE_SIZE(message->header);

		result = network_game_server_write(
			connection,
			message,
			message_size,
			NULL,
			1);
	}

	return result;
}

#ifdef HALO_LINUX
/* the distributed netcode's per-tick state (port/linux/game/network_distributed.c),
unreliably (a lost one is overtaken by the next) to every machine in the game */
boolean network_distributed_server_send_to_all(
	void *message,
	word size)
{
	struct network_game_server *server = global_network_game_server_get();
	boolean result = TRUE;
	long machine_index;

	if (!server)
		return FALSE;
	for (machine_index = 0; machine_index < MAXIMUM_NETWORK_MACHINE_COUNT; machine_index++)
	{
		struct network_game_server_client_machine *machine =
			network_game_server_get_client_machine_at_index(server, machine_index);

		if (network_game_server_client_machine_is_joined_to_game(server, machine))
		{
			struct network_connection *connection = network_game_server_get_client_connection(machine);
			byte buffer[NETWORK_MESSAGE_BUFFER_SIZE];

			if (connection && network_connection_active(connection) && size <= sizeof(buffer))
			{
				struct transport_address address;

				/* from the game's public datagram endpoint (the one clients send
				their game updates to) to the client's, at the address its
				connection comes from */
				network_connection_get_address(connection, &address, NULL);
				address.port = NETWORK_GAME_CLIENT_PORT;
				/* (the write swaps the header in place) */
				csmemcpy(buffer, message, size);
				result &= network_game_server_write(network_game_server_get_connection(server), buffer, size, &address, 0);
			}
		}
	}
	return result;
}

/* reliably to one machine in the game (the host's objects, to a client
that has loaded) */
boolean network_distributed_server_send_to_machine_reliably(
	long machine_index,
	void *message,
	word size)
{
	struct network_game_server *server = global_network_game_server_get();
	byte buffer[NETWORK_MESSAGE_BUFFER_SIZE];
	long client_index;

	if (!server || size > sizeof(buffer))
		return FALSE;
	/* (machine_index is the game's machine, as the message handlers have
	it: the client machine of that machine) */
	for (client_index = 0; client_index < MAXIMUM_NETWORK_MACHINE_COUNT; client_index++)
	{
		struct network_game_server_client_machine *machine =
			network_game_server_get_client_machine_at_index(server, client_index);
		struct network_connection *connection;
		long game_machine_index;

		if (!network_game_server_client_machine_is_joined_to_game(server, machine))
			continue;
		network_game_server_get_client_machine(server, machine, &game_machine_index);
		if (game_machine_index != machine_index)
			continue;
		connection = network_game_server_get_client_connection(machine);
		if (!connection || !network_connection_active(connection))
			return FALSE;
		csmemcpy(buffer, message, size);
		return network_game_server_write(connection, buffer, size, NULL, 1);
	}
	return FALSE;
}

/* to every machine, reliably (the netcode's objects appearing and going
must not be lost) */
boolean network_distributed_server_send_to_all_reliably(
	void *message,
	word size)
{
	struct network_game_server *server = global_network_game_server_get();
	boolean result = TRUE;
	long machine_index;

	if (!server)
		return FALSE;
	for (machine_index = 0; machine_index < MAXIMUM_NETWORK_MACHINE_COUNT; machine_index++)
	{
		struct network_game_server_client_machine *machine =
			network_game_server_get_client_machine_at_index(server, machine_index);

		if (network_game_server_client_machine_is_joined_to_game(server, machine))
		{
			struct network_connection *connection = network_game_server_get_client_connection(machine);
			byte buffer[NETWORK_MESSAGE_BUFFER_SIZE];

			if (connection && network_connection_active(connection) && size <= sizeof(buffer))
			{
				csmemcpy(buffer, message, size);
				result &= network_game_server_write(connection, buffer, size, NULL, 1);
			}
		}
	}
	return result;
}

#endif
boolean network_game_server_send_message_to_all_machines(
	struct network_game_server *server,
	struct network_message *message)
{
	byte message_buffer[NETWORK_MESSAGE_BUFFER_SIZE];
	boolean result = TRUE;
	word message_length;
	long machine_index;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x187,
		server && message);

	message_length = GET_MESSAGE_SIZE(message->header);
	for (machine_index = 0; machine_index < MAXIMUM_NETWORK_MACHINE_COUNT; machine_index++)
	{
		struct network_game_server_client_machine *machine =
			network_game_server_get_client_machine_at_index(server, machine_index);

		if (network_game_server_client_machine_is_joined_to_game(server, machine))
		{
			struct network_connection *connection =
				network_game_server_get_client_connection(machine);

			if (connection && network_connection_active(connection))
			{
				match_assert(
					"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
					0x19A,
					message_length<=sizeof(message_buffer));

				csmemcpy(message_buffer, message, message_length);
				if (!network_game_server_write(
					connection,
					message_buffer,
					message_length,
					NULL,
					1))
				{
					network_event(
						"network_game_server_write() failed in network_game_server_send_message_to_all_machines()");
					result = FALSE;
				}
			}
		}
	}

	return result;
}

boolean network_game_server_send_player_joined_info_ingame(
	struct network_game_server *server,
	struct network_player *player)
{
	struct network_player message;
	void *encoded_message;
	boolean result;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x1B0,
		server && player);

	message = *player;
	encoded_message = create_network_game_message(
		_message_server_add_player_ingame,
		&message,
		sizeof(message));
	if (encoded_message)
	{
		result = network_game_server_send_message_to_all_machines(server, encoded_message);
		if (!result)
		{
			network_event(
				"network_game_server_send_message_to_all_machines() failed in network_game_server_send_player_joined_info_ingame()");
		}

		return result;
	}

	network_event("failed to create a message_server_add_player_ingame message");
	return FALSE;
}

#ifdef HALO_LINUX
/* With up to 128 machines, sending the whole settings record (13 KB) to every
machine on each lobby change would flood the network while a lobby fills, so
changes are collected and sent at most this often; the server's pregame idle
sends what is pending. */
enum
{
	NETWORK_GAME_SETTINGS_UPDATE_INTERVAL = MILLISECONDS_PER_SECOND / 4,
};

static boolean network_game_settings_update_pending = FALSE;
static unsigned long network_game_settings_update_time = 0;

boolean network_game_server_send_game_settings_to_all_machines(
	struct network_game_server *server,
	void const *game,
	long game_size)
{
	struct message_server_game_settings_update message;
	long offset;
	boolean result = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x1C8,
		server);

	/* every piece goes out even if one fails for a machine: the others
	would otherwise keep the old settings (a machine whose connection failed
	is closed, and is skipped when the update is sent again) */
	for (offset = 0; offset < game_size; offset += sizeof(message.data))
	{
		void *encoded_message;

		message.total_size = (word)game_size;
		message.offset = (word)offset;
		message.length = (word)MIN((long)sizeof(message.data), game_size - offset);
		message.pad = 0;
		csmemset(message.data, 0, sizeof(message.data));
		csmemcpy(message.data, (byte const *)game + offset, message.length);
		encoded_message = create_network_game_message(
			_message_server_game_settings_update,
			&message,
			sizeof(message));
		if (encoded_message)
		{
			result = network_game_server_send_message_to_all_machines(server, encoded_message);
			if (!result)
			{
				network_event(
					"failed to send message_server_game_settings_update message to all machines");
			}
		}
		else
		{
			network_event("failed to create a message_server_game_settings_update message");
			result = FALSE;
		}
	}

	network_game_settings_update_pending = !result;
	network_game_settings_update_time = system_milliseconds();

	return result;
}

boolean network_game_server_flush_game_data_pregame(
	struct network_game_server *server)
{
	boolean result = TRUE;

	if (network_game_settings_update_pending &&
		system_milliseconds() - network_game_settings_update_time >= NETWORK_GAME_SETTINGS_UPDATE_INTERVAL)
	{
		struct network_game *game = network_game_server_get_game(server);

		if (game)
		{
			result = network_game_server_send_game_settings_to_all_machines(server, game, sizeof(*game));
		}
		else
		{
			network_event(
				"failed to handle a message_server_game_settings_update because their was no server game");
			network_game_settings_update_pending = FALSE;
			result = FALSE;
		}
	}

	return result;
}

boolean network_game_server_send_game_data_pregame(
	struct network_game_server *server)
{
	network_game_settings_update_pending = TRUE;

	return network_game_server_flush_game_data_pregame(server);
}
#else
boolean network_game_server_send_game_data_pregame(
	struct network_game_server *server)
{
	struct message_server_game_settings_update message;
	struct network_game *game;
	void *encoded_message;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x1C8,
		server);

	game = network_game_server_get_game(server);
	if (game)
	{
		csmemcpy(&message, game, sizeof(message));
		encoded_message = create_network_game_message(
			_message_server_game_settings_update,
			&message,
			sizeof(message));
		if (encoded_message)
		{
			result = network_game_server_send_message_to_all_machines(server, encoded_message);
			if (!result)
			{
				network_event(
					"failed to send message_server_game_settings_update message to all machines");
			}
		}
		else
		{
			network_event("failed to create a message_server_game_settings_update message");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_server_game_settings_update because their was no server game");
	}

	return result;
}
#endif

boolean network_game_server_handle_client_message(
	struct network_game_server *server,
	struct network_game_server_client_machine *machine,
	word *message,
	short message_buffer_size)
{
	boolean result = TRUE;
	word message_type;
	byte packet_type;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0xCF,
		server && machine && message && (message_buffer_size == GET_MESSAGE_SIZE(*message)));

	message_type = (byte)GET_MESSAGE_TYPE(*message);
	if (GET_MESSAGE_FLAGS(*message))
	{
		network_event("server received client message with invalid flags");
	}
	else
	{
		switch (message_type)
		{
			case _message_type_packet:
				packet_type = ((byte *)message)[message_buffer_size - 1];
				if (network_game_server_client_machine_is_joined_to_game(server, machine) ||
					packet_type == _message_client_join_game_request)
				{
					switch (packet_type)
					{
						case _message_client_join_game_request:
							result = network_game_server_handle_message_client_join_game_request(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_join_game_request() failed");
							}
							break;

						case _message_client_add_player_request_pregame:
							result = network_game_server_handle_message_client_add_player_request_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_add_player_request_pregame() failed");
							}
							break;

						case _message_client_remove_player_request_pregame:
							result = network_game_server_handle_message_client_remove_player_request_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_remove_player_request_pregame() failed");
							}
							break;

						case _message_client_settings_request:
							result = network_game_server_handle_message_client_settings_request(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_settings_request() failed");
							}
							break;

						case _message_client_player_settings_request:
							result = network_game_server_handle_message_client_player_settings_request(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_player_settings_request() failed");
							}
							break;

						case _message_client_game_start_request:
							result = network_game_server_handle_message_client_game_start_request(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_game_start_request() failed");
							}
							break;

						case _message_client_graceful_game_exit_pregame:
							result = network_game_server_handle_message_client_graceful_game_exit_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_graceful_game_exit_pregame() failed");
							}
							break;

						case _message_client_map_is_precached_pregame:
							result = network_game_server_handle_message_client_map_is_precached_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_graceful_game_exit_pregame() failed");
							}
							break;

						case _message_client_loaded:
							result = network_game_server_handle_message_client_loaded(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_loaded() failed");
							}
							break;

						case _message_client_add_player_request_ingame:
							result = network_game_server_handle_message_client_add_player_request_ingame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_add_player_request_ingame() failed");
							}
							break;

						case _message_client_remove_player_request_ingame:
							result = network_game_server_handle_message_client_remove_player_request_ingame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_remove_player_request_ingame() failed");
							}
							break;

						case _message_client_remove_player_request_postgame:
							result = network_game_server_handle_message_client_remove_player_request_postgame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_remove_player_request_postgame() failed");
							}
							break;

						case _message_client_switch_to_pregame:
							result = network_game_server_handle_message_client_switch_to_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_switch_to_pregame() failed");
							}
							break;

						case _message_client_graceful_game_exit_postgame:
							result = network_game_server_handle_message_client_graceful_game_exit_pregame(
								server,
								machine,
								message,
								message_buffer_size);
							if (!result)
							{
								network_event("network_game_server_handle_message_client_graceful_game_exit_pregame() failed");
							}
							break;

						default:
							network_event(
								"bad or inappropriate packet type received from a client (#%d)",
								packet_type);
							break;
					}
				}
				else
				{
					network_event("an un-validated client sent something other than a join request message");
				}
				break;

			case _message_type_data:
#ifdef HALO_LINUX
				/* the distributed netcode's messages (port/linux/NETCODE.md) */
				if (network_game_server_client_machine_is_joined_to_game(server, machine))
				{
					long machine_index;

					network_game_server_get_client_machine(server, machine, &machine_index);
					network_distributed_handle_message(machine_index, message, message_buffer_size);
				}
#else
				network_event("server received a bad message type from a client (_message_type_data)");
#endif
				break;

			case _message_type_error:
				if (message_buffer_size >= MINIMUM_TRANSPORT_ERROR_MESSAGE_SIZE)
				{
					byte *error_message = (byte *)(message + 1);

					network_event(
						"server received low-level error message from a client: error= #%d (%s)",
						error_message[TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH],
						error_message);
				}
				else
				{
					network_event("server received a malformed/damaged message from a client");
				}
				break;

			default:
				network_event(
					"server received a client message with an unknown message type (#%d)",
					message_type);
				break;
		}
	}

	return result;
}

boolean network_game_server_handle_datagram(
	struct network_game_server *server,
	word *message,
	short datagram_size,
	struct transport_address *source_address)
{
	word message_type;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x40,
		server && message && source_address &&
			(datagram_size > sizeof(message_header)) &&
			(datagram_size == GET_MESSAGE_SIZE(*message)));

	message_type = (byte)GET_MESSAGE_TYPE(*message);
	if (GET_MESSAGE_FLAGS(*message))
	{
		network_event(
			"server received a datagram with invalid flags; sender= '%s'",
			transport_address_to_string(source_address));
	}
	else
	{
		switch (message_type)
		{
			case _message_type_packet:
			{
				short packet_version = NETWORK_GAME_MESSAGE_VERSION;
				short packet_type = ((byte *)message)[datagram_size - 1];

				datagram_size -= sizeof(word);
				switch (packet_type)
				{
					case _message_client_broadcast_game_search:
						if (network_game_should_accept_remote_connections())
						{
							struct message_client_broadcast_game_search game_search;

							if (decode_network_game_message(
								&game_search,
								message + 1,
								&datagram_size,
								&packet_type,
								&packet_version,
								_network_game_packet_class_client_search))
							{
								if (!network_game_server_handle_message_client_broadcast_game_search(
									server,
									source_address,
									&game_search))
								{
									network_event(
										"server failed to advertise game to prospective client at '%s'",
										transport_address_to_string(source_address));
								}
							}
							else
							{
								network_event("failed to decode a message_client_broadcast_game_search packet");
							}
						}
						break;

					case _message_client_ping:
						if (network_game_should_accept_remote_connections())
						{
							struct message_client_ping ping;

							if (decode_network_game_message(
								&ping,
								message + 1,
								&datagram_size,
								&packet_type,
								&packet_version,
								_network_game_packet_class_client_search))
							{
								if (!network_game_server_handle_message_client_ping(
									server,
									source_address,
									&ping))
								{
									network_event("server failed to handle a client ping");
								}
							}
							else
							{
								network_event("failed to decode a message_client_ping packet");
							}
						}
						break;

					case _message_client_game_update:
						if (network_game_server_get_state(server, NULL) == _network_game_server_state_ingame)
						{
							struct network_game_server_client_machine *client_machine =
								network_game_server_get_client_machine_at_address(
									server,
									source_address->address.long_words[0]);

							if (client_machine)
							{
								struct message_client_game_update game_update;

								if (decode_network_game_message(
									&game_update,
									message + 1,
									&datagram_size,
									&packet_type,
									&packet_version,
									_network_game_packet_class_client_ingame))
								{
									network_game_server_handle_client_update_packet(
										server,
										client_machine,
										&game_update);
								}
								else
								{
									network_event("failed to decode a message_client_game_update packet");
								}
							}
							else
							{
								network_event(
									"failed to handle a message_client_game_update message; this client doesn't seem to be in the game");
							}
						}
						else
						{
							network_event("ignoring a message_client_game_update message; we are not in game");
						}
						break;

					default:
						network_event(
							"server received datagram with an unexpected packet type; sender= '%s'",
							transport_address_to_string(source_address));
						break;
				}
			}
			break;

			case _message_type_data:
#ifdef HALO_LINUX
				/* the distributed netcode's messages (port/linux/NETCODE.md) */
				{
					struct network_game_server_client_machine *client_machine =
						network_game_server_get_client_machine_at_address(server, source_address->address.long_words[0]);

					if (client_machine && network_game_server_get_state(server, NULL) == _network_game_server_state_ingame)
					{
						long machine_index;

						network_game_server_get_client_machine(server, client_machine, &machine_index);
						network_distributed_handle_message(machine_index, message, datagram_size);
					}
				}
#else
				network_event(
					"server received a bad message type (_message_type_data); sender= '%s'",
					transport_address_to_string(source_address));
#endif
				break;

			case _message_type_error:
				if (datagram_size >= MINIMUM_TRANSPORT_ERROR_MESSAGE_SIZE)
				{
					byte *error_message = (byte *)(message + 1);

					network_event(
						"server received low-level error message: error= #%d (%s); sender= '%s'",
						error_message[TRANSPORT_ERROR_MESSAGE_TEXT_LENGTH],
						error_message,
						transport_address_to_string(source_address));
				}
				else
				{
					network_event(
						"server received a malformed/damaged message; sender= '%s'",
						transport_address_to_string(source_address));
				}
				break;

			default:
				network_event(
					"server received a datagram with an unknown message type (#%d); sender= '%s'",
					message_type,
					transport_address_to_string(source_address));
				break;
		}
	}

	return TRUE;
}

/* ---------- private code */

static boolean network_game_server_handle_message_client_broadcast_game_search(
	struct network_game_server *server,
	struct transport_address *source_address,
	struct message_client_broadcast_game_search *client_message)
{
	boolean result = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x21F,
		server && source_address && client_message);

	if (client_message->version == NETWORK_GAME_MESSAGE_VERSION)
	{
		struct network_game *game = network_game_server_get_game(server);

		if (game)
		{
			struct message_server_game_advertise advertisement = {0};
			struct transport_address address;
			struct network_message *reply;

			address.address_length = IPV4_ADDRESS_LENGTH;
			address.address.long_words[0] = NONE;
			address.port = NETWORK_GAME_CLIENT_PORT;

			csmemcpy(
				advertisement.client_nonce,
				client_message->nonce,
				sizeof(client_message->nonce));
			transport_get_nonce(advertisement.nonce, TRANSPORT_NONCE_LENGTH);
			advertisement.key_id = transport_get_key_id();
			advertisement.key = transport_get_key();

			{
				XNADDR xnaddr;

				advertisement.xnaddr = *transport_get_xnaddr(&xnaddr);
			}

			advertisement.port = NETWORK_GAME_SERVER_PORT;
			advertisement.version = NETWORK_GAME_MESSAGE_VERSION;
			advertisement.platform = 0;
			ustrncpy(advertisement.game_name, game->name, NETWORK_GAME_NAME_LENGTH - 1);
			advertisement.engine_type = (short)game->variant.game_engine_index;
			csmemcpy(&advertisement.map, &game->map, sizeof(game->map));
			advertisement.machine_count = game->machine_count;
			advertisement.player_count = game->player_count;
			advertisement.maximum_player_count = game->maximum_player_count;
			advertisement.variant_setting = (short)game->variant.universal_variant.score_to_win;
			advertisement.flags = 0;
			if (game->variant.universal_variant.teams == TRUE)
			{
				advertisement.flags = FLAG(_game_advertisement_has_teams_bit);
			}

			if (game->variant.game_engine_index == game_engine_oddball &&
				game->variant.game_engine_variant.oddball.oddball_ball_type == 2)
			{
				advertisement.flags |= FLAG(_game_advertisement_oddball_variant_bit);
			}

			if (network_game_server_game_is_open(server))
			{
				advertisement.flags |= FLAG(_game_advertisement_open_bit);
			}

			network_game_generate_join_game_token(advertisement.join_game_token);
			reply = create_network_game_message(
				_message_server_game_advertise,
				&advertisement,
				sizeof(advertisement));
			if (reply)
			{
				word message_size = GET_MESSAGE_SIZE(reply->header);
				struct network_connection *connection =
					network_game_server_get_connection(server);

				result = network_game_server_write(
					connection,
					reply,
					message_size,
					&address,
					0);
				if (!result)
				{
					network_event(
						"network_game_server_write() failed in handle_message_client_broadcast_game_search()");
				}
			}
			else
			{
				network_event("failed to create a message_server_game_advertise message");
			}
		}
	}

	return result;
}

static boolean network_game_server_handle_message_client_ping(
	struct network_game_server *server,
	struct transport_address *source_address,
	struct message_client_ping *client_message)
{
	struct message_server_pong pong;
	struct network_message *reply;
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
		0x26A,
		server && source_address && client_message);

	pong.timestamp = client_message->timestamp;
	reply = create_network_game_message(
		_message_server_pong,
		&pong,
		sizeof(pong));
	if (reply)
	{
		struct transport_address address;
		address.address_length = IPV4_ADDRESS_LENGTH;
		address.address.long_words[0] = source_address->address.long_words[0];
		address.port = client_message->port;
		result = network_game_server_write(
			network_game_server_get_connection(server),
			reply,
			GET_MESSAGE_SIZE(reply->header),
			&address,
			0);
		if (!result)
		{
			network_event(
				"network_game_server_write() failed in handle_message_client_ping()");
		}
	}
	else
	{
		network_event("failed to create a message_server_pong message");
	}

	return result;
}

static boolean network_game_server_handle_message_client_join_game_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *server_client_machine,
	word *message,
	short message_size)
{
	boolean result = TRUE;

	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct message_client_join_game_request join_game_request;
		short packet_type = _message_client_join_game_request;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (network_game_server_client_machine_is_joined_to_game(server, server_client_machine))
		{
			network_event("ignoring redundant join request from machine");
		}
		else if (decode_network_game_message(
			&join_game_request,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			struct transport_address source_address;

			network_connection_get_address(
				network_game_server_get_client_connection(server_client_machine),
				&source_address,
				FALSE);
			if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame &&
				network_game_server_game_is_open(server))
			{
				byte join_game_token[JOIN_GAME_TOKEN_LENGTH];

				network_game_generate_join_game_token(join_game_token);
				if (!csmemcmp(
					join_game_request.join_game_token,
					join_game_token,
					sizeof(join_game_token)))
				{
					boolean machine_is_in_hosts_file = TRUE;
					FILE *hosts_file;

					wide_to_ascii(
						join_game_request.machine_name,
						(char *)join_game_request.machine_name,
						sizeof(join_game_request.machine_name));
					hosts_file = fopen("d:\\hosts.txt", "r");
					if (hosts_file)
					{
						char host_line[MAXIMUM_HOSTS_FILE_LINE_LENGTH] = "";

						machine_is_in_hosts_file = FALSE;
						while (fgets(host_line, sizeof(host_line), hosts_file))
						{
							if (!csstrncmp(
								(char *)join_game_request.machine_name,
								host_line,
								csstrlen((char *)join_game_request.machine_name)))
							{
								machine_is_in_hosts_file = TRUE;
								break;
							}
						}

						fclose(hosts_file);
					}

					if (!machine_is_in_hosts_file)
					{
						struct message_server_machine_rejected rejection;
						struct network_message *reply;

						rejection.reason = _network_game_server_rejection_reason_not_in_hosts_file;
						network_event(
							"server refused client '%s' because it is not in your hosts file",
							join_game_request.machine_name);
						reply = create_network_game_message(
							_message_server_machine_rejected,
							&rejection,
							sizeof(rejection));
						if (reply)
						{
							word message_size = GET_MESSAGE_SIZE(reply->header);
							struct network_connection *connection =
								network_game_server_get_client_connection(server_client_machine);

							network_game_server_write(
								connection,
								reply,
								message_size,
								NULL,
								1);
						}

						result = FALSE;
					}
					else
					{
						if (network_game_server_accept_client_machine_into_game(server, server_client_machine))
						{
							struct message_server_machine_accepted acceptance;
							struct network_message *reply;
							struct network_machine *client_machine;
							long machine_index = NONE;

							client_machine = network_game_server_get_client_machine(
								server,
								server_client_machine,
								&machine_index);
							network_game_server_get_game(server);

							match_assert(
								"c:\\halo\\SOURCE\\networking\\network_server_message_handler.c",
								0x2CE,
								network_machine_is_valid(client_machine));

							acceptance.machine_index = (short)machine_index;
							acceptance.random_seed = network_game_get_random_seed();
							reply = create_network_game_message(
								_message_server_machine_accepted,
								&acceptance,
								sizeof(acceptance));
							if (reply)
							{
								word message_size = GET_MESSAGE_SIZE(reply->header);
								struct network_connection *connection =
									network_game_server_get_client_connection(server_client_machine);

								result = network_game_server_write(
									connection,
									reply,
									message_size,
									NULL,
									1);
								if (!result)
								{
									network_event(
										"network_game_server_write() failed in network_game_server_handle_message_client_join_game_request()");
								}
								else
								{
									network_event(
										"sent _message_type_server_machine_accepted message to %d",
										machine_index);
								}

								if (result == TRUE)
								{
									result = network_game_server_send_game_data_pregame(server);
									if (!result)
									{
										network_event(
											"network_game_server_send_game_data_pregame() failed in network_game_server_handle_message_client_join_game_request()");
									}
								}
							}
							else
							{
								result = FALSE;
							}
						}
						else
						{
							struct message_server_machine_rejected rejection;
							struct network_message *reply;

							rejection.reason = _network_game_server_rejection_reason_game_not_open;
							network_event(
								"server failed to accept valid client machine '%s' @%s into the game",
								join_game_request.machine_name,
								transport_address_to_string(&source_address));
							reply = create_network_game_message(
								_message_server_machine_rejected,
								&rejection,
								sizeof(rejection));
							if (reply)
							{
								word message_size = GET_MESSAGE_SIZE(reply->header);
								struct network_connection *connection =
									network_game_server_get_client_connection(server_client_machine);

								if (!network_game_server_write(
									connection,
									reply,
									message_size,
									NULL,
									1))
								{
									network_event(
										"network_game_server_write() failed while sending a rejection reply");
								}
							}

							result = FALSE;
						}
					}
				}
				else
				{
					struct message_server_machine_rejected rejection;
					struct network_message *reply;

					rejection.reason = _network_game_server_rejection_reason_bad_join_token;
					network_event(
						"client machine '%s' @%s tried to join game with a bad join token",
						join_game_request.machine_name,
						transport_address_to_string(&source_address));
					reply = create_network_game_message(
						_message_server_machine_rejected,
						&rejection,
						sizeof(rejection));
					if (reply)
					{
						word message_size = GET_MESSAGE_SIZE(reply->header);
						struct network_connection *connection =
							network_game_server_get_client_connection(server_client_machine);

						if (!network_game_server_write(
							connection,
							reply,
							message_size,
							NULL,
							1))
						{
							network_event(
								"network_game_server_write() failed while sending a rejection reply");
						}
					}
					else
					{
						network_event("failed to create a message_server_machine_rejected message");
					}

					result = FALSE;
				}
			}
			else
			{
				struct message_server_machine_rejected rejection;
				struct network_message *reply;

				rejection.reason = _network_game_server_rejection_reason_game_not_open;
				network_event(
					"client machine '%s' @%s tried to join game when they should not be",
					join_game_request.machine_name,
					transport_address_to_string(&source_address));
				reply = create_network_game_message(
					_message_server_machine_rejected,
					&rejection,
					sizeof(rejection));
				if (reply)
				{
					word message_size = GET_MESSAGE_SIZE(reply->header);
					struct network_connection *connection =
						network_game_server_get_client_connection(server_client_machine);

					if (!network_game_server_write(
						connection,
						reply,
						message_size,
						NULL,
						1))
					{
						network_event(
							"network_game_server_write() failed while sending a rejection reply");
					}
				}
				else
				{
					network_event("failed to create a message_server_machine_rejected message");
				}

				result = FALSE;
			}
		}
		else
		{
			network_event("server failed to decode a message_client_join_game_request packet");
			result = FALSE;
		}
	}

	return result;
}

static boolean network_game_server_handle_message_client_add_player_request_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct network_player player;
		short packet_type = _message_client_add_player_request_pregame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			if (network_game_server_add_player_to_game(server, client_machine, &player))
			{
				if (!network_game_server_send_game_data_pregame(server))
				{
					network_event(
						"server failed to send pregame game data in network_game_server_handle_message_client_add_player_request_pregame()");
				}
			}
			else
			{
				network_event(
					"server failed to add a network player in network_game_server_handle_message_client_add_player_request_pregame()");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_add_player_request_pregame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_add_player_request_pregame because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_remove_player_request_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct network_player player;
		short packet_type = _message_client_remove_player_request_pregame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			if (network_game_server_remove_player_from_game(server, client_machine, &player))
			{
				if (!network_game_server_send_game_data_pregame(server))
				{
					network_event(
						"server failed to send pregame game data in network_game_server_handle_message_client_remove_player_request_pregame()");
				}
			}
			else
			{
				network_event(
					"server failed to remove a network player in network_game_server_handle_message_client_remove_player_request_pregame()");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_remove_player_request_pregame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_remove_player_request_pregame because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_settings_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct network_machine machine_settings;
		short packet_type = _message_client_settings_request;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&machine_settings,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			if (network_game_server_adjust_machine_settings(server, client_machine, &machine_settings))
			{
				network_event(
					"server received machine settings for machine #%d/'%s'",
					machine_settings.machine_index,
					wide_to_ascii(
						machine_settings.name,
						(char *)machine_settings.name,
						sizeof(machine_settings.name)));
				if (!network_game_server_send_game_data_pregame(server))
				{
					network_event(
						"server failed to send pregame game data in network_game_server_handle_message_client_settings_request()");
				}
			}
			else
			{
				network_event(
					"network_game_server_adjust_machine_settings() failed in network_game_server_handle_message_client_settings_request()");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_settings_request packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_settings_request because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_player_settings_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct network_player player;
		short packet_type = _message_client_player_settings_request;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			if (network_game_update_player(network_game_server_get_game(server), &player))
			{
				network_event("server received updated player settings");
				if (!network_game_server_send_game_data_pregame(server))
				{
					network_event(
						"server failed to send pregame game data in network_game_server_handle_message_client_player_settings_request()");
				}
			}
			else
			{
				network_event(
					"network_game_update_player() failed in network_game_server_handle_message_client_player_settings_request()");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_player_settings_request packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_player_settings_request because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_game_start_request(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct message_client_game_start_request game_start_request;
		short packet_type = _message_client_player_settings_request;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&game_start_request,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			network_game_server_update_countdown(server, game_start_request.countdown_time);
		}
		else
		{
			network_event("server failed to decode a message_client_game_start_request packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_game_start_request because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_graceful_game_exit_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct message_client_graceful_game_exit_pregame graceful_game_exit;
		short packet_type = _message_client_graceful_game_exit_pregame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&graceful_game_exit,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			if (network_game_server_remove_machine_from_game(
				server,
				network_game_server_get_client_machine(server, client_machine, NULL)))
			{
				if (!network_game_server_send_game_data_pregame(server))
				{
					network_event(
						"server failed to send pregame game data in network_game_server_handle_message_client_graceful_game_exit_pregame()");
				}
			}
			else
			{
				network_event(
					"network_game_server_remove_machine_from_game() failed in network_game_server_handle_message_client_graceful_game_exit_pregame()");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_graceful_game_exit_pregame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_graceful_game_exit_pregame message because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_map_is_precached_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct message_client_map_is_precached_pregame map_is_precached;
		short packet_type = _message_client_map_is_precached_pregame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&map_is_precached,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_pregame))
		{
			network_game_server_client_machine_is_precached(
				server,
				client_machine,
				map_is_precached.map_name);
		}
		else
		{
			network_event("server failed to decode a message_type_client_map_is_precached_pregame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_type_client_map_is_precached_pregame because the server is not in pregame");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_loaded(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	boolean result = TRUE;

	if (network_game_server_get_state(server, NULL) == _network_game_server_state_pregame)
	{
		struct message_client_loaded loaded;
		short packet_type = _message_client_loaded;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&loaded,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_ingame))
		{
			network_game_server_client_machine_game_loading_complete(server, client_machine);
		}
		else
		{
			network_event("server failed to decode a message_client_loaded packet");
			result = FALSE;
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_loaded message because the server is not in pregame");
		result = FALSE;
	}

	return result;
}

static boolean network_game_server_handle_message_client_add_player_request_ingame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_ingame)
	{
		struct network_player player;
		short packet_type = _message_client_add_player_request_ingame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_ingame))
		{
			network_game_server_queue_player_for_addition(server, &player);
		}
		else
		{
			network_event("server failed to decode a message_client_add_player_request_ingame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_add_player_request_ingame because the server is not in game");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_remove_player_request_ingame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	boolean result = TRUE;

	if (network_game_server_get_state(server, NULL) == _network_game_server_state_ingame)
	{
		struct network_player player;
		short packet_type = _message_client_add_player_request_ingame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_ingame))
		{
			struct network_game *game = network_game_server_get_game(server);

			if (network_game_remove_player(game, &player))
			{
				struct message_server_remove_player_ingame remove_player;
				void *encoded_message;

				remove_player.player = player;
				remove_player.game_time = game_time_get() + REMOVE_PLAYER_INGAME_GAME_TIME_DELAY;
				encoded_message = create_network_game_message(
					_message_server_remove_player_ingame,
					&remove_player,
					sizeof(remove_player));
				if (encoded_message)
				{
					result = network_game_server_send_message_to_all_machines(server, encoded_message);
					if (!result)
					{
						network_event(
							"network_game_server_send_message_to_all_machines() failed in network_game_server_handle_message_client_remove_player_request_ingame()");
					}

					return result;
				}
			}
		}
		else
		{
			network_event("server failed to decode a message_client_remove_player_request_ingame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_remove_player_request_ingame because the server is not in game");
	}

	return result;
}

static boolean network_game_server_handle_message_client_remove_player_request_postgame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	if (network_game_server_get_state(server, NULL) == _network_game_server_state_postgame)
	{
		struct network_player player;
		short packet_type = _message_client_remove_player_request_postgame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&player,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_postgame))
		{
			if (!network_game_server_remove_player_from_game(server, client_machine, &player))
			{
				network_event("server failed to remove a network player post-game");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_remove_player_request_postgame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_remove_player_request_postgame because the server is not in post-game");
	}

	return TRUE;
}

static boolean network_game_server_handle_message_client_switch_to_pregame(
	struct network_game_server *server,
	struct network_game_server_client_machine *client_machine,
	word *message,
	short message_size)
{
	boolean result = TRUE;

	if (network_game_server_get_state(server, NULL) == _network_game_server_state_postgame)
	{
		struct message_client_switch_to_pregame switch_to_pregame;
		short packet_type = _message_client_switch_to_pregame;
		short packet_version = NETWORK_GAME_MESSAGE_VERSION;

		message_size -= sizeof(word);
		if (decode_network_game_message(
			&switch_to_pregame,
			message + 1,
			&message_size,
			&packet_type,
			&packet_version,
			_network_game_packet_class_client_postgame))
		{
			result = network_game_server_switch_machine_from_postgame_to_pregame(server, client_machine);
			if (!result)
			{
				network_event("network_game_server_switch_machine_from_postgame_to_pregame() failed");
			}
		}
		else
		{
			network_event("server failed to decode a message_client_remove_player_request_postgame packet");
		}
	}
	else
	{
		network_event(
			"failed to handle a message_client_switch_to_pregame because the server is not in post-game");
	}

	return result;
}
