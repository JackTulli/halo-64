/*
NETWORK_MESSAGES.C

symbols in this file:
0011AE90 0010:
	_initialize_network_game_packets (0000)
0011AEA0 0060:
	_network_event (0000)
0011AF00 0050:
	_code_0011af00 (0000)
0011AF50 05e0:
	_create_network_game_message (0000)
0011B530 00a0:
	_decode_network_game_message (0000)
00284040 001c:
	??_C@_0BM@EBOFJEK@network_game_messages_group?$AA@ (0000)
0028405C 0032:
	??_C@_0DC@BOGDHCMH@message_client_graceful_game_exi@ (0000)
00284090 0028:
	??_C@_0CI@EEPBNHAG@message_client_switch_to_pregame@ (0000)
002840B8 0035:
	??_C@_0DF@EMNEGNHK@message_client_remove_player_req@ (0000)
002840F0 0032:
	??_C@_0DC@MCHEBCFB@message_server_graceful_game_exi@ (0000)
00284124 0028:
	??_C@_0CI@LMKDEEMM@message_server_switch_to_pregame@ (0000)
0028414C 0024:
	??_C@_0CE@ILDHAAGE@message_client_join_new_host_pac@ (0000)
00284170 0030:
	??_C@_0DA@NAFCJNBH@message_client_host_crashed_cry_@ (0000)
002841A0 0033:
	??_C@_0DD@DPFJDDJL@message_client_remove_player_req@ (0000)
002841D4 0030:
	??_C@_0DA@HJJAAOFP@message_client_add_player_reques@ (0000)
00284204 0022:
	??_C@_0CC@JNOAENJF@message_client_game_update_packe@ (0000)
00284228 001d:
	??_C@_0BN@NJKENIOP@message_client_loaded_packet?$AA@ (0000)
00284248 0020:
	??_C@_0CA@NHACOKNP@message_server_game_over_packet?$AA@ (0000)
00284268 002b:
	??_C@_0CL@ONEMLOHO@message_server_remove_player_ing@ (0000)
00284294 0028:
	??_C@_0CI@NCMFMOGB@message_server_add_player_ingame@ (0000)
002842BC 0022:
	??_C@_0CC@EKNLMOAI@message_server_game_update_packe@ (0000)
002842E0 002f:
	??_C@_0CP@ICNCBHBJ@message_client_map_is_precached_@ (0000)
00284310 0031:
	??_C@_0DB@MBNEDKDF@message_client_graceful_game_exi@ (0000)
00284344 0029:
	??_C@_0CJ@DBOKMPL@message_client_game_start_reques@ (0000)
00284370 002e:
	??_C@_0CO@ONAIPAAF@message_client_player_settings_r@ (0000)
002843A0 0027:
	??_C@_0CH@DFKBPHDI@message_client_settings_request_@ (0000)
002843C8 0034:
	??_C@_0DE@BOCJLBIG@message_client_remove_player_req@ (0000)
002843FC 0031:
	??_C@_0DB@ICGGHOBC@message_client_add_player_reques@ (0000)
00284430 0028:
	??_C@_0CI@NJAKJDJG@message_client_join_game_request@ (0000)
00284458 0031:
	??_C@_0DB@LJGDBL@message_server_graceful_game_exi@ (0000)
0028448C 0021:
	??_C@_0CB@GBFOJNFG@message_server_begin_game_packet@ (0000)
002844B0 002a:
	??_C@_0CK@NGOKIJFI@message_server_postgame_keep_ali@ (0000)
002844DC 0029:
	??_C@_0CJ@OOOJECPL@message_server_pregame_keep_aliv@ (0000)
00284508 0028:
	??_C@_0CI@DDGLHHCI@message_server_pregame_countdown@ (0000)
00284530 002b:
	??_C@_0CL@KGHPJAMF@message_server_game_settings_upd@ (0000)
0028455C 0027:
	??_C@_0CH@PPCEIPKP@message_server_machine_rejected_@ (0000)
00284584 0027:
	??_C@_0CH@MHOBBBHN@message_server_machine_accepted_@ (0000)
002845AC 001b:
	??_C@_0BL@OBJNBJFL@message_server_pong_packet?$AA@ (0000)
002845C8 0025:
	??_C@_0CF@LPLCJKKH@message_server_game_advertise_pa@ (0000)
002845F0 001b:
	??_C@_0BL@KJGLBKLM@message_client_ping_packet?$AA@ (0000)
0028460C 002c:
	??_C@_0CM@DBKCBLPI@message_client_broadcast_game_se@ (0000)
00284638 002d:
	??_C@_0CN@IGMHDBNJ@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
00284668 0057:
	??_C@_0FH@LDAHLJDB@message_struct?5?$CG?$CG?5encoded_messag@ (0000)
002846C0 0025:
	??_C@_0CF@FAGKPKLA@encode_network_game_message?$CI?$CJ?5fa@ (0000)
002846E8 0018:
	??_C@_0BI@HDENCOBE@create_message?$CI?$CJ?5failed?$AA@ (0000)
00284700 002c:
	??_C@_0CM@HANECPBB@unknown?5network?5game?5message?5str@ (0000)
00284730 0048:
	??_C@_0EI@LDIICBJE@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284778 003e:
	??_C@_0DO@FGHCKKNA@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
002847B8 004b:
	??_C@_0EL@FPPCOKOP@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284808 0048:
	??_C@_0EI@GOMGGMKH@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284850 003e:
	??_C@_0DO@IBEJCJEN@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284890 003a:
	??_C@_0DK@FANDEEMI@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
002848D0 0046:
	??_C@_0EG@BNKKIHMJ@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284918 0049:
	??_C@_0EJ@DEANEKBI@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284968 0046:
	??_C@_0EG@ENKLANMK@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
002849B0 0038:
	??_C@_0DI@IKHDCGFA@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
002849E8 0033:
	??_C@_0DD@IMEOHJNL@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284A1C 0036:
	??_C@_0DG@HDEEJKMJ@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284A58 0041:
	??_C@_0EB@OBJOGCPO@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284A9C 003e:
	??_C@_0DO@HENBBIFD@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284ADC 0038:
	??_C@_0DI@FLGIACGI@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284B18 0045:
	??_C@_0EF@LBGBNCAB@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284B60 0047:
	??_C@_0EH@FNFCMEAG@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284BA8 003f:
	??_C@_0DP@KKICJIGK@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284BE8 0044:
	??_C@_0EE@CKGGABKC@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284C2C 003d:
	??_C@_0DN@DENDNKBM@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284C70 004a:
	??_C@_0EK@EEGLJJMH@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284CC0 0047:
	??_C@_0EH@CMMPIMHI@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284D08 003e:
	??_C@_0DO@DMPHOMMG@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284D48 0040:
	??_C@_0EA@HDLMPJOG@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284D88 0047:
	??_C@_0EH@BOGDDOGJ@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284DD0 0037:
	??_C@_0DH@NCBFHEAI@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284E08 003f:
	??_C@_0DP@DPJHGIHE@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284E48 003e:
	??_C@_0DO@CMCEHFEG@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284E88 0041:
	??_C@_0EB@IBENFKED@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284ECC 003d:
	??_C@_0DN@OFBIEKEC@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284F0C 003d:
	??_C@_0DN@MAJGGCDN@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284F4C 0031:
	??_C@_0DB@MDKHEKHO@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284F80 003b:
	??_C@_0DL@IBPEMGOC@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284FBC 0031:
	??_C@_0DB@GADNMNKL@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00284FF0 0042:
	??_C@_0EC@OBBNPBFB@message_struct_size?$DN?$DNsizeof?$CImess@ (0000)
00285034 0025:
	??_C@_0CF@MENKCHHP@decode_network_game_message?$CI?$CJ?5fa@ (0000)
00285060 00a4:
	??_C@_0KE@BCMKEKI@message_struct?5?$CG?$CG?5encoded_messag@ (0000)
0030AA68 0844:
	_data_0030aa68 (0000)
004566F0 0604:
	_bss_004566f0 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "bungie_net/common/message_header.h"
#include "memory/data_packet_groups.h"
#include "networking/network_messages.h"

/* ---------- constants */

/* ---------- macros */

#define DATA_PACKET_FIELD(type, count) { type, count, 0, 0, 0 }
#define DATA_PACKET_FIELD_END DATA_PACKET_FIELD(_data_packet_field_end, 0)
#define NETWORK_GAME_MESSAGE_DEFINITION(member, name, structure) \
	{ name, 0, sizeof(structure), 1, data_0030aa68.member##_fields, FALSE }

/* ---------- structures */

struct network_game_message_packet_definitions
{
	struct data_packet_field client_broadcast_game_search_fields[3];
	struct data_packet_definition client_broadcast_game_search;
	struct data_packet_field client_ping_fields[4];
	struct data_packet_definition client_ping;
	struct data_packet_field server_game_advertise_fields[2];
	struct data_packet_definition server_game_advertise;
	struct data_packet_field server_pong_fields[2];
	struct data_packet_definition server_pong;
	struct data_packet_field server_machine_accepted_fields[4];
	struct data_packet_definition server_machine_accepted;
	struct data_packet_field server_machine_rejected_fields[2];
	struct data_packet_definition server_machine_rejected;
#ifdef HALO_LINUX
	struct data_packet_field server_game_settings_update_fields[4];
#else
	struct data_packet_field server_game_settings_update_fields[2];
#endif
	struct data_packet_definition server_game_settings_update;
	struct data_packet_field server_pregame_countdown_fields[2];
	struct data_packet_definition server_pregame_countdown;
	struct data_packet_field server_pregame_keep_alive_fields[2];
	struct data_packet_definition server_pregame_keep_alive;
	struct data_packet_field server_postgame_keep_alive_fields[2];
	struct data_packet_definition server_postgame_keep_alive;
	struct data_packet_field server_begin_game_fields[2];
	struct data_packet_definition server_begin_game;
	struct data_packet_field server_graceful_game_exit_pregame_fields[2];
	struct data_packet_definition server_graceful_game_exit_pregame;
	struct data_packet_field client_join_game_request_fields[3];
	struct data_packet_definition client_join_game_request;
	struct data_packet_field client_add_player_request_pregame_fields[4];
	struct data_packet_definition client_add_player_request_pregame;
	struct data_packet_field client_remove_player_request_pregame_fields[4];
	struct data_packet_definition client_remove_player_request_pregame;
	struct data_packet_field client_settings_request_fields[4];
	struct data_packet_definition client_settings_request;
	struct data_packet_field client_player_settings_request_fields[4];
	struct data_packet_definition client_player_settings_request;
	struct data_packet_field client_game_start_request_fields[2];
	struct data_packet_definition client_game_start_request;
	struct data_packet_field client_graceful_game_exit_pregame_fields[2];
	struct data_packet_definition client_graceful_game_exit_pregame;
	struct data_packet_field client_map_is_precached_pregame_fields[2];
	struct data_packet_definition client_map_is_precached_pregame;
	struct data_packet_field server_game_update_fields[8];
	struct data_packet_definition server_game_update;
	struct data_packet_field server_add_player_ingame_fields[4];
	struct data_packet_definition server_add_player_ingame;
	struct data_packet_field server_remove_player_ingame_fields[5];
	struct data_packet_definition server_remove_player_ingame;
	struct data_packet_field server_game_over_fields[2];
	struct data_packet_definition server_game_over;
	struct data_packet_field client_loaded_fields[2];
	struct data_packet_definition client_loaded;
	struct data_packet_field client_game_update_fields[8];
	struct data_packet_definition client_game_update;
	struct data_packet_field client_add_player_request_ingame_fields[4];
	struct data_packet_definition client_add_player_request_ingame;
	struct data_packet_field client_remove_player_request_ingame_fields[4];
	struct data_packet_definition client_remove_player_request_ingame;
	struct data_packet_field client_host_crashed_cry_for_help_fields[4];
	struct data_packet_definition client_host_crashed_cry_for_help;
	struct data_packet_field client_join_new_host_fields[4];
	struct data_packet_definition client_join_new_host;
	struct data_packet_field server_switch_to_pregame_fields[2];
	struct data_packet_definition server_switch_to_pregame;
	struct data_packet_field server_graceful_game_exit_postgame_fields[2];
	struct data_packet_definition server_graceful_game_exit_postgame;
	struct data_packet_field client_remove_player_request_postgame_fields[4];
	struct data_packet_definition client_remove_player_request_postgame;
	struct data_packet_field client_switch_to_pregame_fields[2];
	struct data_packet_definition client_switch_to_pregame;
	struct data_packet_field client_graceful_game_exit_postgame_fields[2];
	struct data_packet_definition client_graceful_game_exit_postgame;
	struct data_packet_entry packets[35];
	struct data_packet_group_definition group;
};

union network_game_message_size
{
	long value;
	short encoded;
};

#define DEFINE_NETWORK_GAME_MESSAGE(name, size) typedef struct name { byte opaque[size]; } name

DEFINE_NETWORK_GAME_MESSAGE(message_client_broadcast_game_search, 0x0C);
DEFINE_NETWORK_GAME_MESSAGE(message_client_ping, 0x08);
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_advertise, 0x114);
DEFINE_NETWORK_GAME_MESSAGE(message_server_pong, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_server_machine_accepted, 0x08);
DEFINE_NETWORK_GAME_MESSAGE(message_server_machine_rejected, 0x02);
#ifdef HALO_LINUX
/* one piece of the game settings record: total size, offset, length, then the
bytes (network_server_message_handler.c) */
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_settings_update, 8 + HALO_PORT_NETWORK_GAME_SETTINGS_FRAGMENT_SIZE);
#else
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_settings_update, 0x434);
#endif
DEFINE_NETWORK_GAME_MESSAGE(message_server_pregame_countdown, 0x02);
DEFINE_NETWORK_GAME_MESSAGE(message_server_begin_game, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_server_graceful_game_exit_pregame, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_server_pregame_keep_alive, 0x02);
DEFINE_NETWORK_GAME_MESSAGE(message_server_postgame_keep_alive, 0x02);
DEFINE_NETWORK_GAME_MESSAGE(message_client_join_game_request, 0x50);
DEFINE_NETWORK_GAME_MESSAGE(message_client_add_player_request_pregame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_remove_player_request_pregame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_settings_request, 0x44);
DEFINE_NETWORK_GAME_MESSAGE(message_client_player_settings_request, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_game_start_request, 0x02);
DEFINE_NETWORK_GAME_MESSAGE(message_client_graceful_game_exit_pregame, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_client_map_is_precached_pregame, 0x100);
#ifdef HALO_LINUX
/* one 0x20-byte action per player */
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_update, 0x10 + HALO_PORT_MAXIMUM_NETWORK_PLAYERS * 0x20);
#else
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_update, 0x210);
#endif
DEFINE_NETWORK_GAME_MESSAGE(message_server_add_player_ingame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_server_remove_player_ingame, 0x24);
DEFINE_NETWORK_GAME_MESSAGE(message_server_game_over, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_client_loaded, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_client_game_update, 0x88);
DEFINE_NETWORK_GAME_MESSAGE(message_client_add_player_request_ingame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_remove_player_request_ingame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_host_crashed_cry_for_help, 0x10);
DEFINE_NETWORK_GAME_MESSAGE(message_client_join_new_host, 0x10);
DEFINE_NETWORK_GAME_MESSAGE(message_server_switch_to_pregame, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_server_graceful_game_exit_postgame, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_client_remove_player_request_postgame, 0x20);
DEFINE_NETWORK_GAME_MESSAGE(message_client_switch_to_pregame, 0x04);
DEFINE_NETWORK_GAME_MESSAGE(message_client_graceful_game_exit_postgame, 0x04);

#undef DEFINE_NETWORK_GAME_MESSAGE

/* ---------- prototypes */

/* ---------- globals */

struct network_game_message_packet_definitions data_0030aa68 =
{
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 8),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_broadcast_game_search, "message_client_broadcast_game_search_packet", message_client_broadcast_game_search),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_ping, "message_client_ping_packet", message_client_ping),
	{
		DATA_PACKET_FIELD(_data_packet_field_raw, 276),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_game_advertise, "message_server_game_advertise_packet", message_server_game_advertise),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_pong, "message_server_pong_packet", message_server_pong),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_machine_accepted, "message_server_machine_accepted_packet", message_server_machine_accepted),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_machine_rejected, "message_server_machine_rejected_packet", message_server_machine_rejected),
	{
#ifdef HALO_LINUX
		DATA_PACKET_FIELD(_data_packet_field_shorts, 3),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD(_data_packet_field_raw, HALO_PORT_NETWORK_GAME_SETTINGS_FRAGMENT_SIZE),
		DATA_PACKET_FIELD_END,
#else
		DATA_PACKET_FIELD(_data_packet_field_raw, 1076),
		DATA_PACKET_FIELD_END,
#endif
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_game_settings_update, "message_server_game_settings_update_packet", message_server_game_settings_update),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_pregame_countdown, "message_server_pregame_countdown_packet", message_server_pregame_countdown),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_pregame_keep_alive, "message_server_pregame_keep_alive_packet", message_server_pregame_keep_alive),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_postgame_keep_alive, "message_server_postgame_keep_alive_packet", message_server_postgame_keep_alive),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_begin_game, "message_server_begin_game_packet", message_server_begin_game),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_graceful_game_exit_pregame, "message_server_graceful_game_exit_pregame_packet", message_server_graceful_game_exit_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 32),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 16),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_join_game_request, "message_client_join_game_request_packet", message_client_join_game_request),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_add_player_request_pregame, "message_client_add_player_request_pregame_packet", message_client_add_player_request_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_remove_player_request_pregame, "message_client_remove_player_request_pregame_packet", message_client_remove_player_request_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 32),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 3),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_settings_request, "message_client_settings_request_packet", message_client_settings_request),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_player_settings_request, "message_client_player_settings_request_packet", message_client_player_settings_request),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_game_start_request, "message_client_game_start_request_packet", message_client_game_start_request),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_graceful_game_exit_pregame, "message_client_graceful_game_exit_pregame_packet", message_client_graceful_game_exit_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_bytes, 256),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_map_is_precached_pregame, "message_client_map_is_precached_pregame_packet", message_client_map_is_precached_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 3),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
#ifdef HALO_LINUX
		DATA_PACKET_FIELD(_data_packet_field_array, HALO_PORT_MAXIMUM_NETWORK_PLAYERS),
#else
		DATA_PACKET_FIELD(_data_packet_field_array, 16),
#endif
		DATA_PACKET_FIELD(_data_packet_field_longs, 6),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 3),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_game_update, "message_server_game_update_packet", message_server_game_update),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_add_player_ingame, "message_server_add_player_ingame_packet", message_server_add_player_ingame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_remove_player_ingame, "message_server_remove_player_ingame_packet", message_server_remove_player_ingame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_game_over, "message_server_game_over_packet", message_server_game_over),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_loaded, "message_client_loaded_packet", message_client_loaded),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD(_data_packet_field_array, 4),
		DATA_PACKET_FIELD(_data_packet_field_longs, 6),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 3),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_game_update, "message_client_game_update_packet", message_client_game_update),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_add_player_request_ingame, "message_client_add_player_request_ingame_packet", message_client_add_player_request_ingame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_remove_player_request_ingame, "message_client_remove_player_request_ingame_packet", message_client_remove_player_request_ingame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 3),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_host_crashed_cry_for_help, "message_client_host_crashed_cry_for_help_packet", message_client_host_crashed_cry_for_help),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 3),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 1),
		DATA_PACKET_FIELD(_data_packet_field_pad, 2),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_join_new_host, "message_client_join_new_host_packet", message_client_join_new_host),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_switch_to_pregame, "message_server_switch_to_pregame_packet", message_server_switch_to_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(server_graceful_game_exit_postgame, "message_server_graceful_game_exit_postgame_packet", message_server_graceful_game_exit_postgame),
	{
		DATA_PACKET_FIELD(_data_packet_field_shorts, 12),
		DATA_PACKET_FIELD(_data_packet_field_shorts, 2),
		DATA_PACKET_FIELD(_data_packet_field_bytes, 4),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_remove_player_request_postgame, "message_client_remove_player_request_postgame_packet", message_client_remove_player_request_postgame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_switch_to_pregame, "message_client_switch_to_pregame_packet", message_client_switch_to_pregame),
	{
		DATA_PACKET_FIELD(_data_packet_field_longs, 1),
		DATA_PACKET_FIELD_END,
	},
	NETWORK_GAME_MESSAGE_DEFINITION(client_graceful_game_exit_postgame, "message_client_graceful_game_exit_postgame_packet", message_client_graceful_game_exit_postgame),
	{
		{ 0, 0, &data_0030aa68.client_broadcast_game_search },
		{ 0, 0, &data_0030aa68.client_ping },
		{ 1, 0, &data_0030aa68.server_game_advertise },
		{ 1, 0, &data_0030aa68.server_pong },
		{ 2, 0, &data_0030aa68.server_machine_accepted },
		{ 2, 0, &data_0030aa68.server_machine_rejected },
		{ 2, 0, &data_0030aa68.server_game_settings_update },
		{ 2, 0, &data_0030aa68.server_pregame_countdown },
		{ 2, 0, &data_0030aa68.server_pregame_keep_alive },
		{ 2, 0, &data_0030aa68.server_begin_game },
		{ 2, 0, &data_0030aa68.server_graceful_game_exit_pregame },
		{ 6, 0, &data_0030aa68.server_postgame_keep_alive },
		{ 3, 0, &data_0030aa68.client_join_game_request },
		{ 3, 0, &data_0030aa68.client_add_player_request_pregame },
		{ 3, 0, &data_0030aa68.client_remove_player_request_pregame },
		{ 3, 0, &data_0030aa68.client_settings_request },
		{ 3, 0, &data_0030aa68.client_player_settings_request },
		{ 3, 0, &data_0030aa68.client_game_start_request },
		{ 3, 0, &data_0030aa68.client_graceful_game_exit_pregame },
		{ 3, 0, &data_0030aa68.client_map_is_precached_pregame },
		{ 4, 0, &data_0030aa68.server_game_update },
		{ 4, 0, &data_0030aa68.server_add_player_ingame },
		{ 4, 0, &data_0030aa68.server_remove_player_ingame },
		{ 4, 0, &data_0030aa68.server_game_over },
		{ 5, 0, &data_0030aa68.client_loaded },
		{ 5, 0, &data_0030aa68.client_game_update },
		{ 5, 0, &data_0030aa68.client_add_player_request_ingame },
		{ 5, 0, &data_0030aa68.client_remove_player_request_ingame },
		{ 5, 0, &data_0030aa68.client_host_crashed_cry_for_help },
		{ 5, 0, &data_0030aa68.client_join_new_host },
		{ 6, 0, &data_0030aa68.server_switch_to_pregame },
		{ 6, 0, &data_0030aa68.server_graceful_game_exit_postgame },
		{ 7, 0, &data_0030aa68.client_remove_player_request_postgame },
		{ 7, 0, &data_0030aa68.client_switch_to_pregame },
		{ 7, 0, &data_0030aa68.client_graceful_game_exit_postgame },
	},
	{
		"network_game_messages_group",
		35,
		8,
#ifdef HALO_LINUX
		/* the per-tick update of 128 players decodes to 0x1010 bytes */
		HALO_PORT_NETWORK_PACKET_SIZE,
		HALO_PORT_NETWORK_PACKET_SIZE,
#else
		0x600,
		0x800,
#endif
		data_0030aa68.packets,
	},
};

#ifdef HALO_LINUX
static byte network_game_message_buffer[HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE + 4];
#else
static byte network_game_message_buffer[0x604];
#endif

/* ---------- public code */

void initialize_network_game_packets(
	void)
{
	data_packet_group_initialize(&data_0030aa68.group);

	return;
}

void network_event(
	char *format,
	...)
{
	va_list arguments;

#line 331 "c:\\halo\\SOURCE\\networking\\network_messages.c"
	match_assert(__FILE__, __LINE__, format);

	va_start(arguments, format);
	_vsnprintf(temporary, NUMBEROF(temporary) - 1, format, arguments);
	va_end(arguments);

	error(3, temporary);

	return;
}

static boolean encode_network_game_message(
	const void *message_struct,
	void *encoded_message,
	short *encoded_message_size,
	enum network_game_message_type message_type,
	long message_version)
{
#line 353 "c:\\halo\\SOURCE\\networking\\network_messages.c"
	match_assert(__FILE__, __LINE__, message_struct && encoded_message && encoded_message_size && (*encoded_message_size>0));

	return data_packet_group_encode_packet(&data_0030aa68.group, message_struct, encoded_message, encoded_message_size, message_type, message_version);
}

void *create_network_game_message(
	enum network_game_message_type message_type,
	const void *message_struct,
	short message_struct_size)
{
#ifdef HALO_LINUX
	/* as large as the packet group lets the encoder write */
	byte encoded_message[HALO_PORT_NETWORK_PACKET_SIZE];
#else
	byte encoded_message[0x600];
#endif
	union network_game_message_size encoded_message_size;
	void *message;

	encoded_message_size.value = sizeof(encoded_message);

	switch ((short)message_type)
	{
	case _message_client_broadcast_game_search:
#line 160 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_broadcast_game_search));
		break;
	case _message_client_ping:
#line 161 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_ping));
		break;
	case _message_server_game_advertise:
#line 164 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_game_advertise));
		break;
	case _message_server_pong:
#line 165 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_pong));
		break;
	case _message_server_machine_accepted:
#line 168 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_machine_accepted));
		break;
	case _message_server_machine_rejected:
#line 169 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_machine_rejected));
		break;
	case _message_server_game_settings_update:
#line 170 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_game_settings_update));
		break;
	case _message_server_pregame_countdown:
#line 171 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_pregame_countdown));
		break;
	case _message_server_pregame_keep_alive:
#line 172 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_pregame_keep_alive));
		break;
	case _message_server_begin_game:
#line 173 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_begin_game));
		break;
	case _message_server_graceful_game_exit_pregame:
#line 174 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_graceful_game_exit_pregame));
		break;
	case _message_server_postgame_keep_alive:
#line 177 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_postgame_keep_alive));
		break;
	case _message_client_join_game_request:
#line 180 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_join_game_request));
		break;
	case _message_client_add_player_request_pregame:
#line 181 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_add_player_request_pregame));
		break;
	case _message_client_remove_player_request_pregame:
#line 182 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_remove_player_request_pregame));
		break;
	case _message_client_settings_request:
#line 183 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_settings_request));
		break;
	case _message_client_player_settings_request:
#line 184 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_player_settings_request));
		break;
	case _message_client_game_start_request:
#line 185 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_game_start_request));
		break;
	case _message_client_graceful_game_exit_pregame:
#line 186 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_graceful_game_exit_pregame));
		break;
	case _message_client_map_is_precached_pregame:
#line 187 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_map_is_precached_pregame));
		break;
	case _message_server_game_update:
#line 190 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_game_update));
		break;
	case _message_server_add_player_ingame:
#line 191 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_add_player_ingame));
		break;
	case _message_server_remove_player_ingame:
#line 192 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_remove_player_ingame));
		break;
	case _message_server_game_over:
#line 193 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_game_over));
		break;
	case _message_client_loaded:
#line 196 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_loaded));
		break;
	case _message_client_game_update:
#line 197 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_game_update));
		break;
	case _message_client_add_player_request_ingame:
#line 198 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_add_player_request_ingame));
		break;
	case _message_client_remove_player_request_ingame:
#line 199 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_remove_player_request_ingame));
		break;
	case _message_client_host_crashed_cry_for_help:
#line 201 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_host_crashed_cry_for_help));
		break;
	case _message_client_join_new_host:
#line 202 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_join_new_host));
		break;
	case _message_server_switch_to_pregame:
#line 205 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_switch_to_pregame));
		break;
	case _message_server_graceful_game_exit_postgame:
#line 206 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_server_graceful_game_exit_postgame));
		break;
	case _message_client_remove_player_request_postgame:
#line 209 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_remove_player_request_postgame));
		break;
	case _message_client_switch_to_pregame:
#line 210 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_switch_to_pregame));
		break;
	case _message_client_graceful_game_exit_postgame:
#line 211 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_assert(__FILE__, __LINE__, message_struct_size==sizeof(message_client_graceful_game_exit_postgame));
		break;
	default:
#line 213 "c:\\halo\\SOURCE\\networking\\network_messages.c"
		match_vassert(__FILE__, __LINE__, FALSE, "unknown network game message structure type");
		break;
	}

	if (encode_network_game_message(message_struct, encoded_message, &encoded_message_size.encoded, message_type, 1))
	{
#ifdef HALO_LINUX
		/* a message header holds lengths up to HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE-1,
		header included: a longer message would go out with a wrong length and
		derail the stream (create_message only asserts on it) */
		if (encoded_message_size.value + sizeof(word) >= HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE)
		{
			network_event("create_network_game_message(): the encoded message is too long");
			message = NULL;
		}
		else
#endif
		message = create_message(3, encoded_message, encoded_message_size.value, network_game_message_buffer, sizeof(network_game_message_buffer));
		if (!message)
		{
			network_event("create_message() failed");
		}
	}
	else
	{
		network_event("encode_network_game_message() failed");
		message = NULL;
	}

	return message;
}

boolean decode_network_game_message(
	void *message_struct,
	const void *encoded_message,
	short *encoded_message_size,
	short *packet_type,
	short *packet_version,
	long expected_packet_class)
{
	boolean result;

#line 313 "c:\\halo\\SOURCE\\networking\\network_messages.c"
	match_assert(__FILE__, __LINE__, message_struct && encoded_message && encoded_message_size && (*encoded_message_size>0) && packet_type && (*packet_type>=0) && packet_version && (*packet_version>0));

	result = data_packet_group_decode_packet(&data_0030aa68.group, message_struct, encoded_message, encoded_message_size, packet_type, packet_version, expected_packet_class);

	if (!result)
	{
		network_event("decode_network_game_message() failed");
	}

	return result;
}

/* ---------- private code */
