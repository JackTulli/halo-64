/*
NETWORK_CONNECTION.C

symbols in this file:
00117B30 0010:
	_network_connection_initialize (0000)
00117B40 0070:
	_network_server_allow_client_connections (0000)
00117BB0 0060:
	_network_connection_connected (0000)
00117C10 00a0:
	_network_connection_get_address (0000)
00117CB0 0120:
	_network_connection_connect (0000)
00117DD0 0040:
	_network_connection_set_connection_rejection_procedure (0000)
00117E10 00a0:
	_network_connection_server_accept_client_connection (0000)
00117EB0 0040:
	_network_connection_active (0000)
00117EF0 0040:
	_network_connection_going_stale (0000)
00117F30 0200:
	_network_client_unreliable_connection_read (0000)
00118130 0440:
	_network_connection_log_traffic_event (0000)
00118570 0010:
	_network_connection_keep_alive (0000)
00118580 00d0:
	_network_connection_delete (0000)
00118650 0330:
	_network_connection_write (0000)
00118980 0140:
	_network_server_close_client_connection (0000)
00118AC0 0080:
	_network_connection_create_client_from_endpoint (0000)
00118B40 01e0:
	_network_client_reliable_connection_read (0000)
00118D20 01e0:
	_network_connection_idle_client_reliable_endpoint (0000)
00118F00 0240:
	_network_connection_new (0000)
00119140 0090:
	_network_connection_read (0000)
001191D0 00b0:
	_network_connection_disconnect (0000)
00119280 02c0:
	_network_connection_idle_server_reliable_endpoint (0000)
00119540 0310:
	_network_connection_idle (0000)
00282D74 003d:
	??_C@_0DN@LALHHLDL@server_connection?9?$DOflags?$CGFLAG?$CI_c@ (0000)
00282DB4 0012:
	??_C@_0BC@MBGFNKLH@server_connection?$AA@ (0000)
00282DC8 002f:
	??_C@_0CP@PHHPIMDF@c?3?2halo?2SOURCE?2networking?2networ@ (0000)
00282DF8 000b:
	??_C@_0L@IJAFCDGM@connection?$AA@ (0000)
00282E04 003c:
	??_C@_0DM@JFKHFKGL@connect_endpoint?$CI?$CJ?5on?5reliable?5e@ (0000)
00282E40 003e:
	??_C@_0DO@PHFNEMHH@connect_endpoint?$CI?$CJ?5on?5unreliable@ (0000)
00282E80 002d:
	??_C@_0CN@MMKPPGGM@connect_endpoint_async?$CI?$CJ?5returne@ (0000)
00282EB0 000f:
	??_C@_0P@GENNJCOH@remote_address?$AA@ (0000)
00282EC0 0012:
	??_C@_0BC@IIEOAGFH@client_connection?$AA@ (0000)
00282ED4 003e:
	??_C@_0DO@KJEOOHFJ@partial?5datagram?5in?5queue?5?$CI?$CD?$CFd?5o@ (0000)
00282F14 0020:
	??_C@_0CA@COBEMAD@encryption?5should?5not?5be?5active?$AA@ (0000)
00282F38 0065:
	??_C@_0GF@CMFKGJHL@packet?5in?5queue?5is?5?$CD?$CFd?5bytes?0?5bu@ (0000)
00282FA0 0050:
	??_C@_0FA@BBOJNMPA@got?5an?5unusually?5large?5datagram?5@ (0000)
00282FF0 0024:
	??_C@_0CE@GAKKKNNF@?$CKbuffer_size?$DOsizeof?$CImessage_head@ (0000)
00283014 000c:
	??_C@_0M@JLCCDMKC@buffer_size?$AA@ (0000)
00283020 0008:
	??_C@_07ONPBMBOP@message?$AA@ (0000)
00283028 007b:
	??_C@_0HL@CDIBBPO@connection?5?$CG?$CG?5connection?9?$DOunreli@ (0000)
002830A4 0019:
	??_C@_0BJ@POEPMLNA@?$CB?$CCunknown?5traffic?5event?$CC?$AA@ (0000)
002830C0 0014:
	??_C@_0BE@HPLCINIF@?$CFg?7?$CFld?7?$CFld?7?$CFld?7?$CFld?6?$AA@ (0000)
002830D4 0025:
	??_C@_0CF@EIHEBDPA@connection?8s?5remote?5address?5was?3@ (0000)
002830FC 0020:
	??_C@_0CA@JMADEGFJ@connection?5lifetime?7?$CFg?7seconds?6?$AA@ (0000)
00283120 0008:
	__real@3f50624dd2f1a9fc (0000)
00283128 0042:
	??_C@_0EC@HNHLBFPB@NOTE?3?5header?5overhead?5is?5not?5inc@ (0000)
0028316C 002f:
	??_C@_0CP@CCJIMFN@stream?5overhead?5?$CIheaders?$CJ?7?$CFld?7by@ (0000)
0028319C 0032:
	??_C@_0DC@JMBOAMCF@datagram?5overhead?5?$CIheaders?$CJ?7?$CFld?7@ (0000)
002831D0 001e:
	??_C@_0BO@BKOHNOBI@stream?5messages?5received?7?$CFld?6?$AA@ (0000)
002831F0 001a:
	??_C@_0BK@PLGDKDKM@stream?5messages?5sent?7?$CFld?6?$AA@ (0000)
0028320C 0018:
	??_C@_0BI@CKENGKMK@datagrams?5received?7?$CFld?6?$AA@ (0000)
00283224 0014:
	??_C@_0BE@MACKCCIC@datagrams?5sent?7?$CFld?6?$AA@ (0000)
00283238 0003:
	??_C@_02PHMGELLB@?6?6?$AA@ (0000)
00283240 0045:
	??_C@_0EF@PFDCDHNI@time?0?5seconds?7udp?5bytes?5out?7udp?5@ (0000)
00283288 0011:
	??_C@_0BB@KBOPNJJN@_traffic_log?4xls?$AA@ (0000)
0028329C 0034:
	??_C@_0DE@OCDLPLKI@client?5call?5to?5write_endpoint?$CI?$CJ?5@ (0000)
002832D0 0089:
	??_C@_0IJ@ILIPJAEL@?$CIconnection?9?$DOflags?$CGFLAG?$CI_connect@ (0000)
0028335C 002a:
	??_C@_0CK@OKGABJCB@message?5size?5exceeds?5maximum?5all@ (0000)
00283388 0025:
	??_C@_0CF@KHJDIFFG@buffer_size?5?$DM?$DN?5DATAGRAM_MAXIMUM_@ (0000)
002833B0 001d:
	??_C@_0BN@GKPOMGCO@buffer?5size?5was?5?$CFd?5max?5is?5?$CFd?$AA@ (0000)
002833D0 000d:
	??_C@_0N@JGCLEACH@dest_address?$AA@ (0000)
002833E0 000a:
	??_C@_09PFGDAODL@?$CBreliable?$AA@ (0000)
002833EC 0025:
	??_C@_0CF@HBNJJBN@bad?5message?5or?5buffer_size?5param@ (0000)
00283418 0061:
	??_C@_0GB@CPNIJODL@failed?5to?5remove?5a?5client?5endpoi@ (0000)
0028347C 0014:
	??_C@_0BE@LJCKNEPN@server?9?$DOclient_list?$AA@ (0000)
00283490 0015:
	??_C@_0BF@OMPKMEKG@server?9?$DOendpoint_set?$AA@ (0000)
002834A8 003f:
	??_C@_0DP@KMHECGJA@server_connection?9?$DOflags?5?$CG?5FLAG?$CI@ (0000)
002834E8 0012:
	??_C@_0BC@EKDFJKMJ@incoming?9reliable?$AA@ (0000)
002834FC 0012:
	??_C@_0BC@LBJNKONM@reliable_endpoint?$AA@ (0000)
00283510 0063:
	??_C@_0GD@KDKCLKAJ@packet?5in?5queue?5is?5?$CD?$CFd?5bytes?0?5bu@ (0000)
00283578 004d:
	??_C@_0EN@FFFBCEEK@got?5an?5unusually?5large?5message?5?$CI@ (0000)
002835C8 0032:
	??_C@_0DC@NIPJCDFJ@connection?5?$CG?$CG?5connection?9?$DOreliab@ (0000)
002835FC 003c:
	??_C@_0DM@BHKHEGGG@blocked?5in?5network_connection_id@ (0000)
00283638 0020:
	??_C@_0CA@CDMGOOMC@client?5reliable?5connection?5lost?$AA@ (0000)
00283658 0031:
	??_C@_0DB@HLCKNJA@error?5?8?$CFs?8?5reading?5from?5client?5r@ (0000)
0028368C 0023:
	??_C@_0CD@EEPBCOBK@circular_queue_queue_data?$CI?$CJ?5fail@ (0000)
002836B0 0024:
	??_C@_0CE@ELOAAFGD@connection?9?$DOreliable_incoming_qu@ (0000)
002836D4 001e:
	??_C@_0BO@GNMCHDOP@connection?9?$DOreliable_endpoint?$AA@ (0000)
002836F4 0014:
	??_C@_0BE@NPIEPOBN@incoming?9unreliable?$AA@ (0000)
00283708 0030:
	??_C@_0DA@ONKNJAHC@well_known_port?5?$DO?5MAXIMUM_RESERV@ (0000)
00283738 0065:
	??_C@_0GF@LBGPIAAM@?$CIflags?$CGFLAG?$CI_connection_create_s@ (0000)
002837A0 0085:
	??_C@_0IF@LOPDJAMN@connection?9?$DOflags?$CGFLAG?$CI_connecti@ (0000)
00283828 0028:
	??_C@_0CI@GBLKHCFP@poll_endpoint_set?$CI?$CJ?5returned?5err@ (0000)
00283850 0027:
	??_C@_0CH@EFPCNJNH@rogue?5endpoint?5connected?5to?5the?5@ (0000)
00283878 0042:
	??_C@_0EC@FCONNFAI@failed?5to?5remove?5a?5client?5endpoi@ (0000)
002838BC 0020:
	??_C@_0CA@KMGAFGOG@accept_endpoint?$CI?$CJ?5returned?5NULL?$AA@ (0000)
002838DC 0018:
	??_C@_0BI@LPNBCLGA@error?5adding?5new?5client?$AA@ (0000)
002838F4 0016:
	??_C@_0BG@BDEGOGDF@new_client_connection?$AA@ (0000)
0028390C 0019:
	??_C@_0BJ@CNCMMBDB@connection?9?$DOendpoint_set?$AA@ (0000)
00283928 0029:
	??_C@_0CJ@LIBJNIBK@connection?9?$DOconnection?4reliable_@ (0000)
00283954 0013:
	??_C@_0BD@NCEDJJHP@connection?5?$CB?$DN?5NULL?$AA@ (0000)
00283968 0027:
	??_C@_0CH@PJEDOOFH@datagram?5received?5from?5unknown?5a@ (0000)
00283990 0049:
	??_C@_0EJ@EBOPBIBA@circular_queue_queue_data?$CI?$CJ?5fail@ (0000)
002839DC 0020:
	??_C@_0CA@NHHJOLOM@endpoint?5read?5buffer?5overflowed?$AA@ (0000)
002839FC 0038:
	??_C@_0DI@GAIEALCC@network_connection_idle_client_r@ (0000)
00283A34 0023:
	??_C@_0CD@EAHAMPLA@timeout?5in?5network_connection_id@ (0000)
00283A58 0038:
	??_C@_0DI@JINGJIOI@network_connection_idle_server_r@ (0000)
00283A90 0039:
	??_C@_0DJ@ODICILDO@dont?5timeout?5is?5active?5so?5not?5ti@ (0000)
004566DA 0001:
	_global_connection_dont_timeout (0000)
*/

/* ---------- headers */

#include <stdio.h>

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "cseries/errors.h"
#include "bungie_net/common/message_header.h"
#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint_winsock.h"
#include "memory/circular_queue.h"
#include "network_connection.h"

/* ---------- constants */

/* the client connections a server accepts: one per machine, the host's own
included. The Xbox game uses the split screen player count (4), which is
also its machine count; the native builds use their machine limit
(port/linux/include/halo_port_limits.h). */
#ifdef HALO_LINUX
#define NETWORK_CONNECTION_MAXIMUM_CLIENTS HALO_PORT_MAXIMUM_NETWORK_MACHINES
#else
#define NETWORK_CONNECTION_MAXIMUM_CLIENTS MAXIMUM_NUMBER_OF_LOCAL_PLAYERS
#endif

enum
{
#ifdef HALO_LINUX
	/* the per-tick update of 128 players is 3,857 bytes */
	RELIABLE_MESSAGE_MAXIMUM_SIZE = HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE,
	/* how long a stream write waits for a peer that is not reading */
	NETWORK_CONNECTION_WRITE_TIMEOUT = 2000,
#else
	RELIABLE_MESSAGE_MAXIMUM_SIZE = 2048,
#endif
	MAXIMUM_RESERVED_NETWORK_PORT = 1023,
	_transport_type_udp = 0x11,
	_transport_type_tcp,
	_connection_closed_bit = 4,
	_connection_going_stale_bit,
};

enum network_connection_traffic_event
{
	_network_connection_traffic_event_open,
	_network_connection_traffic_event_close,
	_network_connection_traffic_event_datagram_sent,
	_network_connection_traffic_event_datagram_received,
	_network_connection_traffic_event_stream_bytes_sent,
	_network_connection_traffic_event_stream_bytes_received,
	_network_connection_traffic_event_stream_message_sent,
	_network_connection_traffic_event_stream_message_received,
};

/* ---------- macros */

/* ---------- structures */

struct network_connection
{
	struct transport_endpoint *reliable_endpoint;
	struct transport_endpoint *unreliable_endpoint;
	unsigned long last_keep_alive_time;
	network_connection_rejection_procedure connection_rejection_procedure;
	struct circular_queue *reliable_incoming_queue;
	struct circular_queue *unreliable_incoming_queue;
	FILE *traffic_log;
	unsigned long traffic_log_start_time;
	long datagrams_sent;
	long datagrams_received;
	long stream_messages_sent;
	long stream_messages_received;
	unsigned long flags;
	word well_known_port;
	word padding36;
};

struct network_server_connection
{
	struct network_connection connection;
	struct transport_endpoint_set *endpoint_set;
	struct network_connection *client_list[NETWORK_CONNECTION_MAXIMUM_CLIENTS];
	boolean allow_client_connections;
};

/* ---------- prototypes */

static boolean network_client_reliable_connection_read(
	struct network_connection *connection,
	void *message,
	word *buffer_size,
	struct transport_address *source_address);
static struct network_connection *network_connection_create_client_from_endpoint(
	struct transport_endpoint *reliable_endpoint);
static boolean network_connection_idle_client_reliable_endpoint(
	struct network_connection *connection);
static boolean network_connection_idle_server_reliable_endpoint(
	struct network_server_connection *connection,
	struct network_connection **new_client_connection);
static void network_connection_log_traffic_event(
	enum network_connection_traffic_event event,
	long amount,
	struct network_connection *connection);
static boolean network_client_unreliable_connection_read(
	struct network_connection *connection,
	void *message,
	word *buffer_size,
	struct transport_address *source_address);

/* ---------- globals */

boolean global_connection_dont_timeout = FALSE;

/* ---------- public code */

void network_connection_initialize(
	void)
{
	return;
}

boolean network_connection_connected(
	struct network_connection *connection)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x15C,
		connection);

	return ((connection->flags & FLAG(_connection_create_clientside_client_bit)) ||
		(connection->flags & FLAG(_connection_create_serverside_client_bit))) &&
		connection->reliable_endpoint &&
		(boolean)endpoint_connected(connection->reliable_endpoint);
}

static void network_connection_log_traffic_event(
	enum network_connection_traffic_event event,
	long amount,
	struct network_connection *connection)
{
	struct transport_address address;
	double elapsed_seconds;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x4CC,
		connection);

	if (amount <= 0)
	{
		return;
	}

	switch (event)
	{
	case _network_connection_traffic_event_open:
	{
		if (get_endpoint_address(connection->reliable_endpoint, &address) &&
			get_endpoint_address(connection->unreliable_endpoint, &address))
		{
			memset(&address, 0, sizeof(address));
			address.address_length = IPV4_ADDRESS_LENGTH;
		}
		{
			char traffic_log_name[256] = { 0 };
			long character_index;

			strcpy(traffic_log_name, transport_address_to_string(&address));
			for (character_index = 0; traffic_log_name[character_index]; character_index++)
			{
				if (traffic_log_name[character_index] == ':')
				{
					traffic_log_name[character_index] = 0;
					break;
				}
			}
			strcat(traffic_log_name, "_traffic_log.xls");
			connection->traffic_log = fopen(traffic_log_name, "w");
			if (connection->traffic_log)
			{
				fprintf(connection->traffic_log, "time, seconds\tudp bytes out\tudp bytes in\ttcp bytes out\ttcp bytes in\n");
				fflush(connection->traffic_log);
			}
		}
		connection->traffic_log_start_time = system_milliseconds();
		return;
	}

	case _network_connection_traffic_event_close:
		if (!connection->traffic_log)
		{
			return;
		}
		if (get_endpoint_address(connection->reliable_endpoint, &address))
		{
			memset(&address, 0, sizeof(address));
			address.address_length = IPV4_ADDRESS_LENGTH;
		}
		fprintf(connection->traffic_log, "\n\n");
		fprintf(connection->traffic_log, "datagrams sent\t%ld\n", connection->datagrams_sent);
		fprintf(connection->traffic_log, "datagrams received\t%ld\n", connection->datagrams_received);
		fprintf(connection->traffic_log, "stream messages sent\t%ld\n", connection->stream_messages_sent);
		fprintf(connection->traffic_log, "stream messages received\t%ld\n", connection->stream_messages_received);
		fprintf(connection->traffic_log, "datagram overhead (headers)\t%ld\tbytes per packet\n", 0x1C);
		fprintf(connection->traffic_log, "stream overhead (headers)\t%ld\tbytes per chunk\n", 0x28);
		fprintf(connection->traffic_log, "NOTE: header overhead is not included in the above traffic graph\n");
		fprintf(
			connection->traffic_log,
			"connection lifetime\t%g\tseconds\n",
			(double)(system_milliseconds() - connection->traffic_log_start_time) / 1000.0);
		fprintf(connection->traffic_log, "connection's remote address was: %s\n", transport_address_to_string(&address));
		fclose(connection->traffic_log);
		connection->traffic_log = NULL;
		return;

	case _network_connection_traffic_event_datagram_sent:
		if (connection->traffic_log)
		{
			elapsed_seconds = (double)(system_milliseconds() - connection->traffic_log_start_time) / 1000.0;
			fprintf(connection->traffic_log, "%g\t%ld\t%ld\t%ld\t%ld\n", elapsed_seconds, amount, 0, 0, 0);
			fflush(connection->traffic_log);
		}
		connection->datagrams_sent++;
		return;

	case _network_connection_traffic_event_datagram_received:
		if (connection->traffic_log)
		{
			elapsed_seconds = (double)(system_milliseconds() - connection->traffic_log_start_time) / 1000.0;
			fprintf(connection->traffic_log, "%g\t%ld\t%ld\t%ld\t%ld\n", elapsed_seconds, 0, amount, 0, 0);
			fflush(connection->traffic_log);
		}
		connection->datagrams_received++;
		return;

	case _network_connection_traffic_event_stream_bytes_sent:
		if (connection->traffic_log)
		{
			elapsed_seconds = (double)(system_milliseconds() - connection->traffic_log_start_time) / 1000.0;
			fprintf(connection->traffic_log, "%g\t%ld\t%ld\t%ld\t%ld\n", elapsed_seconds, 0, 0, amount, 0);
			fflush(connection->traffic_log);
		}
		return;

	case _network_connection_traffic_event_stream_bytes_received:
		if (connection->traffic_log)
		{
			elapsed_seconds = (double)(system_milliseconds() - connection->traffic_log_start_time) / 1000.0;
			fprintf(connection->traffic_log, "%g\t%ld\t%ld\t%ld\t%ld\n", elapsed_seconds, 0, 0, 0, amount);
			fflush(connection->traffic_log);
		}
		return;

	case _network_connection_traffic_event_stream_message_sent:
		connection->stream_messages_sent++;
		return;

	case _network_connection_traffic_event_stream_message_received:
		connection->stream_messages_received++;
		return;

	default:
		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x557,
			!"unknown traffic event");
		return;
	}
}

void network_connection_get_address(
	struct network_connection *connection,
	struct transport_address *reliable_address,
	struct transport_address *unreliable_address)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x292,
		connection);

	if (reliable_address)
	{
		if (connection->reliable_endpoint)
		{
			if (get_endpoint_address(connection->reliable_endpoint, reliable_address))
			{
				memset(reliable_address, 0, sizeof(*reliable_address));
				reliable_address->address_length = IPV4_ADDRESS_LENGTH;
			}
		}
		else
		{
			memset(reliable_address, 0, sizeof(*reliable_address));
			reliable_address->address_length = IPV4_ADDRESS_LENGTH;
		}
	}

	if (unreliable_address)
	{
		if (connection->unreliable_endpoint)
		{
			if (get_endpoint_address(connection->unreliable_endpoint, unreliable_address))
			{
				memset(unreliable_address, 0, sizeof(*unreliable_address));
				unreliable_address->address_length = IPV4_ADDRESS_LENGTH;
			}
		}
		else
		{
			memset(unreliable_address, 0, sizeof(*unreliable_address));
			unreliable_address->address_length = IPV4_ADDRESS_LENGTH;
		}
	}

	return;
}

boolean network_connection_connect(
	struct network_connection *connection,
	struct transport_address const *remote_address,
	void *process_reference)
{
	short result;
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x2C0,
		connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x2C1,
		remote_address);

	if (!connection->reliable_endpoint && !connection->unreliable_endpoint)
	{
		return FALSE;
	}
	success = TRUE;

	if (connection->unreliable_endpoint)
	{
		result = connect_endpoint(connection->unreliable_endpoint, remote_address);
		if (result)
		{
			error(
				2,
				"connect_endpoint() on unreliable endpoint returned error '%s'",
				transport_error_to_string(result));
			return FALSE;
		}
	}

	if (connection->reliable_endpoint)
	{
		if (process_reference)
		{
			result = connect_endpoint_async(connection->reliable_endpoint, remote_address, process_reference);
			if (result && result != _transport_result_connect_in_progress)
			{
				error(
					2,
					"connect_endpoint_async() returned error '%s'",
					transport_error_to_string(result));
				return FALSE;
			}
		}
		else
		{
			result = connect_endpoint(connection->reliable_endpoint, remote_address);
			if (result)
			{
				error(
					2,
					"connect_endpoint() on reliable endpoint returned error '%s'",
					transport_error_to_string(result));
				return FALSE;
			}
		}
	}

	return success;
}

void network_connection_set_connection_rejection_procedure(
	struct network_connection *connection,
	network_connection_rejection_procedure connection_rejection_procedure)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x318,
		connection);

	connection->connection_rejection_procedure = connection_rejection_procedure;

	return;
}

boolean network_connection_server_accept_client_connection(
	struct network_connection *server_connection,
	struct network_connection *client_connection)
{
	struct network_server_connection *server = (struct network_server_connection *)server_connection;
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x324,
		server_connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x325,
		server_connection->flags&FLAG(_connection_create_server_bit));
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x326,
		client_connection);

	success = !add_endpoint_to_set(client_connection->reliable_endpoint, server->endpoint_set);

	return success;
}

boolean network_connection_active(
	struct network_connection *connection)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x330,
		connection);

	return !TEST_FLAG(connection->flags, _connection_closed_bit);
}

boolean network_connection_going_stale(
	struct network_connection *connection)
{
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x338,
		connection);

	return TEST_FLAG(connection->flags, _connection_going_stale_bit);
}

static boolean network_client_unreliable_connection_read(
	struct network_connection *connection,
	void *message,
	word *buffer_size,
	struct transport_address *source_address)
{
	message_header header;
	unsigned long source_ipv4_address;
	word message_size;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x3BA,
		connection &&
		connection->unreliable_incoming_queue &&
		!(connection->flags&FLAG(_connection_create_serverside_client_bit)));
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x3BB,
		message);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x3BC,
		buffer_size);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x3BD,
		*buffer_size>sizeof(message_header));

	if (circular_queue_dequeue_data(connection->unreliable_incoming_queue, &header, sizeof(header), FALSE))
	{
		byte_swap_message_header(&header, _byte_order_host);
		message_size = GET_MESSAGE_SIZE(header);
		if (message_size > DATAGRAM_MAXIMUM_SIZE)
		{
			error(
				_error_silent,
				"got an unusually large datagram (#d bytes); resetting unreliable incoming queue",
				message_size);
		}
		else if (message_size > *buffer_size)
		{
			error(
				_error_silent,
				"packet in queue is #%d bytes, but we can only handle #%d bytes!; resetting unreliable incoming queue",
				message_size,
				*buffer_size);
		}
		else if (message_size + sizeof(source_ipv4_address) <= (unsigned long)circular_queue_size(connection->unreliable_incoming_queue) &&
			circular_queue_dequeue_data(connection->unreliable_incoming_queue, message, message_size, TRUE) &&
			circular_queue_dequeue_data(connection->unreliable_incoming_queue, &source_ipv4_address, sizeof(source_ipv4_address), TRUE))
		{
			*(message_header *)message = header;
			match_vassert(
				"c:\\halo\\SOURCE\\networking\\network_connection.c",
				0x3DF,
				!TEST_FLAG(header, 0),
				"encryption should not be active");
			if (source_address)
			{
				source_address->address.long_words[0] = source_ipv4_address;
				source_address->port = 0;
				source_address->address_length = IPV4_ADDRESS_LENGTH;
			}
			*buffer_size = message_size;
			return TRUE;
		}
		else
		{
			error(
				_error_silent,
				"partial datagram in queue (#%d of #%d bytes); resetting queue",
				circular_queue_size(connection->unreliable_incoming_queue),
				message_size);
		}
		circular_queue_reset(connection->unreliable_incoming_queue);
	}

	return FALSE;
}

void network_connection_keep_alive(
	struct network_connection *connection)
{
	connection->last_keep_alive_time = system_milliseconds();

	return;
}

void network_connection_delete(
	struct network_connection *connection)
{
	struct network_server_connection *server = (struct network_server_connection *)connection;
	struct network_connection **client;
	long client_index;

	if (connection)
	{
		network_connection_log_traffic_event(
			_network_connection_traffic_event_close,
			TRUE,
			connection);
		if (connection->reliable_endpoint)
		{
			delete_transport_endpoint(connection->reliable_endpoint);
		}
		if (connection->unreliable_endpoint)
		{
			delete_transport_endpoint(connection->unreliable_endpoint);
		}
		if (connection->reliable_incoming_queue)
		{
			circular_queue_delete(connection->reliable_incoming_queue);
		}
		if (connection->unreliable_incoming_queue)
		{
			circular_queue_delete(connection->unreliable_incoming_queue);
		}
		if (TEST_FLAG(connection->flags, _connection_create_server_bit))
		{
			client = server->client_list;
			if (client)
			{
				for (client_index = 0; client_index < NETWORK_CONNECTION_MAXIMUM_CLIENTS; client_index++, client++)
				{
					if (*client)
					{
						if (server->endpoint_set)
						{
							remove_endpoint_from_set((*client)->reliable_endpoint, server->endpoint_set);
						}
						network_connection_delete(*client);
					}
				}
			}
			if (server->endpoint_set)
			{
				delete_endpoint_set(server->endpoint_set);
			}
		}
		match_free(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x145,
			connection);
	}

	return;
}

void network_server_allow_client_connections(
	struct network_connection *server_connection,
	boolean allow_client_connections)
{
	struct network_server_connection *server = (struct network_server_connection *)server_connection;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x151,
		server_connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x152,
		server_connection->flags&FLAG(_connection_create_server_bit));

	server->allow_client_connections = allow_client_connections;

	return;
}

boolean network_connection_write(
	struct network_connection *connection,
	void *message,
	word buffer_size,
	struct transport_address *dest_address,
	boolean reliable)
{
	message_header *header = message;
	long result = 0;
	boolean success;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x170,
		message);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x171,
		buffer_size);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x172,
		connection);
	match_vassert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x174,
		GET_MESSAGE_SIZE(*header) == buffer_size,
		"bad message or buffer_size parameter");

	byte_swap_message_header(header, _byte_order_network);

	if (TEST_FLAG(connection->flags, _connection_create_server_bit))
	{
		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x17B,
			!reliable);
		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x17C,
			dest_address);
		if (buffer_size > DATAGRAM_MAXIMUM_SIZE)
		{
			error(
				_error_silent,
				"buffer size was %d max is %d",
				buffer_size,
				DATAGRAM_MAXIMUM_SIZE);
			match_assert(
				"c:\\halo\\SOURCE\\networking\\network_connection.c",
				0x182,
				buffer_size <= DATAGRAM_MAXIMUM_SIZE);
		}
		result = write_to_endpoint(
			connection->unreliable_endpoint,
			message,
			buffer_size,
			dest_address);
		network_connection_log_traffic_event(
			_network_connection_traffic_event_datagram_sent,
			buffer_size,
			connection);
	}
	else if (reliable)
	{
		long bytes_written;

		match_vassert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x18E,
			buffer_size <= RELIABLE_MESSAGE_MAXIMUM_SIZE,
			"message size exceeds maximum allowed size");
		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x190,
			(connection->flags&FLAG(_connection_create_clientside_client_bit)) ||
			(connection->flags&FLAG(_connection_create_serverside_client_bit)));

#ifdef HALO_LINUX
		/* A stream socket may take only part of a message (the per-tick
		update of 128 players is 3.9 KB): send the rest too, or the peer
		loses its place in the stream. A peer that stops reading for
		NETWORK_CONNECTION_WRITE_TIMEOUT is dropped rather than stalling
		everyone else. */
		{
			long bytes_sent = 0;
			unsigned long start_time = system_milliseconds();
			boolean timed_out = FALSE;

			bytes_written = 0;
			while (bytes_sent < buffer_size)
			{
				long sent = write_endpoint(
					connection->reliable_endpoint,
					(byte *)message + bytes_sent,
					buffer_size - bytes_sent);

				if (sent > 0)
				{
					bytes_sent += sent;
				}
				else if (sent != _transport_result_operation_would_block)
				{
					bytes_written = sent < 0 ? sent : _transport_error_endpoint_io;
					break;
				}
				else if (system_milliseconds() - start_time >= NETWORK_CONNECTION_WRITE_TIMEOUT)
				{
					bytes_written = _transport_error_endpoint_io;
					timed_out = TRUE;
					break;
				}
			}
			if (bytes_sent == buffer_size)
			{
				bytes_written = bytes_sent;
			}
			else if (bytes_sent > 0 || timed_out)
			{
				/* part of a message is on its way, or the peer has stopped
				reading: the stream cannot recover, and going on without this
				message would leave the peer out of step (a client that misses
				a game update puts the whole game out of sync) */
				SET_FLAG(connection->flags, _connection_closed_bit, TRUE);
			}
		}
#else
		do
		{
			bytes_written = write_endpoint(
				connection->reliable_endpoint,
				message,
				buffer_size);
		}
		while (bytes_written <= 0 && bytes_written == _transport_result_operation_would_block);
#endif

		if (bytes_written > 0)
		{
			result = TRUE;
			network_connection_log_traffic_event(
				_network_connection_traffic_event_stream_bytes_sent,
				bytes_written,
				connection);
			network_connection_log_traffic_event(
				_network_connection_traffic_event_stream_message_sent,
				TRUE,
				connection);
		}
		else
		{
			error(
				_error_silent,
				"client call to write_endpoint() returned error '%s'",
				transport_error_to_string((short)bytes_written));
		}
	}
	else if (connection->unreliable_endpoint)
	{
		match_vassert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0x1B0,
			buffer_size <= DATAGRAM_MAXIMUM_SIZE,
			"message size exceeds maximum allowed size");
		if (!dest_address)
		{
			if ((boolean)endpoint_connected(connection->unreliable_endpoint))
			{
				write_endpoint(
					connection->unreliable_endpoint,
					message,
					buffer_size);
				network_connection_log_traffic_event(
					_network_connection_traffic_event_datagram_sent,
					buffer_size,
					connection);
			}
		}
		else
		{
			write_to_endpoint(
				connection->unreliable_endpoint,
				message,
				buffer_size,
				dest_address);
			network_connection_log_traffic_event(
				_network_connection_traffic_event_datagram_sent,
				buffer_size,
				connection);
		}
	}

	if (!reliable)
	{
		success = TRUE;
	}
	else
	{
		success = result > 0;
	}

	return success;
}

static struct network_connection *network_connection_create_client_from_endpoint(
	struct transport_endpoint *reliable_endpoint)
{
	struct network_connection *connection;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x345,
		reliable_endpoint);

	connection = debug_malloc(
		sizeof(*connection),
		TRUE,
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x347);
	if (connection)
	{
		connection->flags = FLAG(_connection_create_serverside_client_bit);
		connection->reliable_endpoint = reliable_endpoint;
		connection->reliable_incoming_queue = circular_queue_new(
			"incoming-reliable",
			0x8000);
		if (!connection->reliable_incoming_queue)
		{
			network_connection_delete(connection);
			return NULL;
		}

		network_connection_log_traffic_event(
			_network_connection_traffic_event_open,
			TRUE,
			connection);
	}

	return connection;
}

static boolean network_connection_idle_client_reliable_endpoint(
	struct network_connection *connection)
{
	byte buffer[RELIABLE_MESSAGE_MAXIMUM_SIZE];
	unsigned long start_time = system_milliseconds();
	boolean success = TRUE;
	long free_space;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x481,
		connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x482,
		connection->reliable_endpoint);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x483,
		connection->reliable_incoming_queue);

	free_space = circular_queue_free_space(connection->reliable_incoming_queue);
	while (success && endpoint_readable(connection->reliable_endpoint, 0) && free_space > 0)
	{
		long bytes_read;

		if (free_space >= RELIABLE_MESSAGE_MAXIMUM_SIZE)
		{
			free_space = RELIABLE_MESSAGE_MAXIMUM_SIZE;
		}
		bytes_read = read_endpoint(connection->reliable_endpoint, buffer, free_space);
		if (bytes_read <= 0)
		{
			if (bytes_read != _transport_result_operation_would_block)
			{
				if (bytes_read == _transport_error_connection_lost)
				{
					SET_FLAG(connection->flags, _connection_closed_bit, TRUE);
				}
				else if (bytes_read)
				{
					error(
						_error_silent,
						"error '%s' reading from client reliable endpoint",
						transport_error_to_string((short)bytes_read));
				}
				else
				{
					error(_error_silent, "client reliable connection lost");
				}
				success = FALSE;
			}
			break;
		}

		connection->last_keep_alive_time = system_milliseconds();
		network_connection_log_traffic_event(
			_network_connection_traffic_event_stream_bytes_received,
			bytes_read,
			connection);
		if (!circular_queue_queue_data(connection->reliable_incoming_queue, buffer, bytes_read))
		{
			error(_error_silent, "circular_queue_queue_data() failed");
			success = FALSE;
		}
		free_space = circular_queue_free_space(connection->reliable_incoming_queue);
	}

	if (system_milliseconds() - start_time > MILLISECONDS_PER_SECOND)
	{
		error(_error_silent, "blocked in network_connection_idle_client_reliable_endpoint");
	}

	return success;
}

struct network_connection *network_connection_new(
	unsigned long flags,
	word well_known_port)
{
	struct network_connection *connection = NULL;
	long reliable_queue_size;
	long unreliable_queue_size;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x9D,
		(flags&FLAG(_connection_create_server_bit))|| (flags&FLAG(_connection_create_clientside_client_bit)));

	if (TEST_FLAG(flags, _connection_create_server_bit))
	{
		struct network_server_connection *server;

		match_assert(
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0xA3,
			well_known_port > MAXIMUM_RESERVED_NETWORK_PORT);
		server = debug_malloc(
			sizeof(struct network_server_connection),
			TRUE,
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0xA5);
		if (server)
		{
			server->allow_client_connections = TRUE;
			server->endpoint_set = create_endpoint_set(NETWORK_CONNECTION_MAXIMUM_CLIENTS + 1);
			if (server->endpoint_set)
			{
				connection = &server->connection;
				reliable_queue_size = 0;
#ifdef HALO_LINUX
				/* room for every machine's input datagrams between two idles */
				unreliable_queue_size = 0x20000;
#else
				unreliable_queue_size = 0x1900;
#endif
			}
			else
			{
				network_connection_delete(&server->connection);
			}
		}
	}
	else if (TEST_FLAG(flags, _connection_create_clientside_client_bit))
	{
		connection = debug_malloc(
			sizeof(struct network_connection),
			TRUE,
			"c:\\halo\\SOURCE\\networking\\network_connection.c",
			0xB6);
		if (connection)
		{
#ifdef HALO_LINUX
			/* a few seconds of per-tick updates of 128 players (3.9 KB each) */
			reliable_queue_size = 0x40000;
#else
			reliable_queue_size = 0x8000;
#endif
			unreliable_queue_size = 0x640;
		}
	}

	if (connection)
	{
		boolean success = TRUE;

		connection->last_keep_alive_time = system_milliseconds();
		connection->flags = flags;
		connection->reliable_endpoint = create_transport_endpoint(_transport_type_tcp);
		if (!connection->reliable_endpoint)
		{
			success = FALSE;
		}

		if (success && TEST_FLAG(flags, _connection_create_server_bit))
		{
			struct transport_address server_address = {0};

			server_address.address_length = IPV4_ADDRESS_LENGTH;
			server_address.port = well_known_port;
			if (bind_endpoint(connection->reliable_endpoint, &server_address) ||
				set_endpoint_blocking(connection->reliable_endpoint, FALSE) ||
				listen_endpoint(connection->reliable_endpoint) ||
				add_endpoint_to_set(
					connection->reliable_endpoint,
					((struct network_server_connection *)connection)->endpoint_set))
			{
				success = FALSE;
			}
		}

		if (success)
		{
			connection->unreliable_endpoint = create_transport_endpoint(_transport_type_udp);
			if (!connection->unreliable_endpoint)
			{
				success = FALSE;
			}
		}

		if (success)
		{
			struct transport_address address;

			address.address_length = IPV4_ADDRESS_LENGTH;
			address.address.long_words[0] = 0;
			address.port = well_known_port;
			connection->well_known_port = well_known_port;
			if (bind_endpoint(connection->unreliable_endpoint, &address) ||
				set_endpoint_blocking(connection->unreliable_endpoint, FALSE))
			{
				success = FALSE;
			}
		}

		if (success && reliable_queue_size)
		{
			connection->reliable_incoming_queue = circular_queue_new(
				"incoming-reliable",
				reliable_queue_size);
			if (!connection->reliable_incoming_queue)
			{
				success = FALSE;
			}
		}

		if (success && unreliable_queue_size)
		{
			connection->unreliable_incoming_queue = circular_queue_new(
				"incoming-unreliable",
				unreliable_queue_size);
			if (!connection->unreliable_incoming_queue)
			{
				success = FALSE;
			}
		}

		if (!success)
		{
			network_connection_delete(connection);
			connection = NULL;
		}
		else
		{
			network_connection_log_traffic_event(
				_network_connection_traffic_event_open,
				TRUE,
				connection);
		}
	}

	return connection;
}

static boolean network_client_reliable_connection_read(
	struct network_connection *connection,
	void *message,
	word *buffer_size,
	struct transport_address *source_address)
{
	message_header header;
	word message_size;
	boolean success = FALSE;
	boolean reset_queue = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x371,
		connection && connection->reliable_incoming_queue);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x372,
		message);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x373,
		buffer_size);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x374,
		*buffer_size>sizeof(message_header));

	if (circular_queue_dequeue_data(connection->reliable_incoming_queue, &header, sizeof(header), FALSE))
	{
		byte_swap_message_header(&header, _byte_order_host);
		message_size = GET_MESSAGE_SIZE(header);
		if (message_size > RELIABLE_MESSAGE_MAXIMUM_SIZE)
		{
			error(
				_error_silent,
				"got an unusually large message (#d bytes); resetting reliable incoming queue",
				message_size);
			reset_queue = TRUE;
		}
		else if (message_size > *buffer_size)
		{
			error(
				_error_silent,
				"packet in queue is #%d bytes, but we can only handle #%d bytes!; resetting reliable incoming queue",
				message_size,
				*buffer_size);
			reset_queue = TRUE;
		}
		else if (message_size <= circular_queue_size(connection->reliable_incoming_queue) &&
			circular_queue_dequeue_data(connection->reliable_incoming_queue, message, message_size, TRUE))
		{
			*(message_header *)message = header;
			match_vassert(
				"c:\\halo\\SOURCE\\networking\\network_connection.c",
				0x394,
				!TEST_FLAG(header, 0),
				"encryption should not be active");
			if (source_address && get_endpoint_address(connection->reliable_endpoint, source_address))
			{
				memset(source_address, 0, sizeof(*source_address));
				source_address->address_length = IPV4_ADDRESS_LENGTH;
			}
			*buffer_size = message_size;
			success = TRUE;
			connection->stream_messages_received++;
		}

		if (reset_queue)
		{
			circular_queue_reset(connection->reliable_incoming_queue);
		}
	}

	return success;
}

boolean network_connection_read(
	struct network_connection *connection,
	void *buffer,
	word *buffer_size,
	struct transport_address *source_address)
{
	boolean result;

	if (TEST_FLAG(connection->flags, _connection_create_server_bit))
	{
		return network_client_unreliable_connection_read(connection, buffer, buffer_size, source_address);
	}

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1E0,
		connection->flags&FLAG(_connection_create_clientside_client_bit) ||
		connection->flags&FLAG(_connection_create_serverside_client_bit));

	result = network_client_reliable_connection_read(connection, buffer, buffer_size, source_address);
	if (!result && TEST_FLAG(connection->flags, _connection_create_clientside_client_bit))
	{
		result = network_client_unreliable_connection_read(connection, buffer, buffer_size, source_address);
	}

	return result;
}

boolean network_server_close_client_connection(
	struct network_connection *server_connection,
	struct network_connection *client_connection)
{
	struct network_server_connection *server = (struct network_server_connection *)server_connection;
	long client_index;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1F7,
		server_connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1F8,
		client_connection);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1F9,
		server_connection->flags & FLAG(_connection_create_server_bit));
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1FA,
		server->endpoint_set);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x1FB,
		server->client_list);

	for (client_index = 0; client_index < NETWORK_CONNECTION_MAXIMUM_CLIENTS; client_index++)
	{
		if (server->client_list[client_index] &&
			server->client_list[client_index] == client_connection)
		{
			if (client_connection->reliable_endpoint &&
				remove_endpoint_from_set(
					server->client_list[client_index]->reliable_endpoint,
					server->endpoint_set))
			{
				error(
					_error_silent,
					"failed to remove a client endpoint from the server's endpoint set (maybe it was already removed)");
			}

			network_connection_delete(server->client_list[client_index]);
			server->client_list[client_index] = NULL;
			return TRUE;
		}
	}

	return FALSE;
}

boolean network_connection_disconnect(
	struct network_connection *connection)
{
	boolean success = TRUE;

	if (network_connection_connected(connection))
	{
		if ((connection->flags & FLAG(_connection_create_clientside_client_bit)) ||
			(connection->flags & FLAG(_connection_create_serverside_client_bit)))
		{
			network_connection_idle_client_reliable_endpoint(connection);
		}
		disconnect_endpoint(connection->reliable_endpoint);
	}

	if (connection->unreliable_endpoint && connection->well_known_port)
	{
		struct transport_address address;

		address.address_length = IPV4_ADDRESS_LENGTH;
		address.address.ipv4_address = 0;
		address.port = connection->well_known_port;

		delete_transport_endpoint(connection->unreliable_endpoint);
		connection->unreliable_endpoint = create_transport_endpoint(_transport_type_udp);
		success = connection->unreliable_endpoint &&
			(bind_endpoint(connection->unreliable_endpoint, &address) == _transport_error_none) &&
			(set_endpoint_blocking(connection->unreliable_endpoint, FALSE) == _transport_error_none);
	}

	return success;
}

static boolean network_connection_idle_server_reliable_endpoint(
	struct network_server_connection *connection,
	struct network_connection **new_client_connection)
{
	struct transport_endpoint *endpoint;
	boolean success = TRUE;
	short result;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x405,
		connection != NULL);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x406,
		connection->connection.reliable_endpoint);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x407,
		connection->endpoint_set);
	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x408,
		new_client_connection);

	*new_client_connection = NULL;
	result = poll_endpoint_set(connection->endpoint_set, 0);
	if (result == _transport_error_none)
	{
		rewind_endpoint_set(connection->endpoint_set);
		while (success &&
			(endpoint = get_next_endpoint_from_set(connection->endpoint_set)) != NULL &&
			result == _transport_error_none)
		{
			if (endpoint_readable(endpoint, 0))
			{
				if (endpoint == connection->connection.reliable_endpoint)
				{
					if (connection->allow_client_connections &&
						count_endpoints_in_set(connection->endpoint_set) < NETWORK_CONNECTION_MAXIMUM_CLIENTS + 1)
					{
						struct transport_endpoint *accepted_endpoint = accept_endpoint(endpoint);
						struct network_connection *client_connection = NULL;

						if (accepted_endpoint &&
							set_endpoint_blocking(accepted_endpoint, FALSE) == _transport_error_none)
						{
							client_connection = network_connection_create_client_from_endpoint(accepted_endpoint);
						}
						if (client_connection)
						{
							long client_index;

							for (client_index = 0; client_index < NETWORK_CONNECTION_MAXIMUM_CLIENTS; client_index++)
							{
								if (!connection->client_list[client_index])
								{
									*new_client_connection = client_connection;
									connection->client_list[client_index] = client_connection;
									break;
								}
							}
							if (client_index >= NETWORK_CONNECTION_MAXIMUM_CLIENTS)
							{
								error(_error_silent, "error adding new client");
							}
						}
						else
						{
							error(_error_silent, "accept_endpoint() returned NULL");
						}
					}
					else if (connection->connection.connection_rejection_procedure)
					{
						struct transport_endpoint *accepted_endpoint = accept_endpoint(endpoint);

						if (accepted_endpoint)
						{
							connection->connection.connection_rejection_procedure(accepted_endpoint);
							delete_transport_endpoint(accepted_endpoint);
						}
					}
					else
					{
						result = reject_endpoint(endpoint);
					}
				}
				else
				{
					long client_index;

					for (client_index = 0; client_index < NETWORK_CONNECTION_MAXIMUM_CLIENTS; client_index++)
					{
						if (connection->client_list[client_index] &&
							connection->client_list[client_index]->reliable_endpoint == endpoint)
						{
							success = network_connection_idle_client_reliable_endpoint(connection->client_list[client_index]);
							if (!success)
							{
								if (remove_endpoint_from_set(
									connection->client_list[client_index]->reliable_endpoint,
									connection->endpoint_set) != _transport_error_none)
								{
									error(
										_error_silent,
										"failed to remove a client endpoint from the server's endpoint set");
								}
								SET_FLAG(connection->client_list[client_index]->flags, _connection_closed_bit, TRUE);
								success = TRUE;
							}
							break;
						}
					}
					match_vassert(
						"c:\\halo\\SOURCE\\networking\\network_connection.c",
						0x469,
						client_index < NETWORK_CONNECTION_MAXIMUM_CLIENTS,
						"rogue endpoint connected to the server");
				}
			}
		}
	}
	else if (result != _transport_result_poll_timeout)
	{
		error(
			_error_silent,
			"poll_endpoint_set() returned error '%s'",
			transport_error_to_string(result));
		success = FALSE;
	}

	return success;
}

boolean network_connection_idle(
	struct network_connection *connection,
	long timeout,
	struct network_connection **new_client_connection)
{
	byte buffer[DATAGRAM_MAXIMUM_SIZE + sizeof(unsigned long)];
	unsigned long current_time = system_milliseconds();
	boolean success = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\networking\\network_connection.c",
		0x21D,
		connection);

	SET_FLAG(connection->flags, _connection_going_stale_bit, FALSE);
	if (timeout)
	{
		if (current_time > connection->last_keep_alive_time + MILLISECONDS_PER_SECOND * 5)
		{
			SET_FLAG(connection->flags, _connection_going_stale_bit, TRUE);
		}
		if (current_time > connection->last_keep_alive_time + timeout)
		{
			if (global_connection_dont_timeout)
			{
				error(
					_error_silent,
					"dont timeout is active so not timing out of a connection");
				connection->last_keep_alive_time = current_time;
			}
			else
			{
				error(_error_silent, "timeout in network_connection_idle");
				return FALSE;
			}
		}
	}
	else
	{
		connection->last_keep_alive_time = current_time;
	}

	if (TEST_FLAG(connection->flags, _connection_create_server_bit))
	{
		success = network_connection_idle_server_reliable_endpoint(
			(struct network_server_connection *)connection,
			new_client_connection);
		if (!success)
		{
			error(_error_silent, "network_connection_idle_server_reliable_endpoint failed");
		}
	}
	else if (connection->flags &
		(FLAG(_connection_create_clientside_client_bit) | FLAG(_connection_create_serverside_client_bit)))
	{
		success = network_connection_idle_client_reliable_endpoint(connection);
		if (!success)
		{
			error(_error_silent, "network_connection_idle_client_reliable_endpoint failed");
		}
	}

	if (success && connection->unreliable_endpoint)
	{
		long free_space = circular_queue_free_space(connection->unreliable_incoming_queue);

		while (success &&
			free_space >= DATAGRAM_MAXIMUM_SIZE + sizeof(unsigned long))
		{
			struct transport_address source_address;
			long buffer_size;
			unsigned long source_ipv4_address;

			if ((boolean)endpoint_connected(connection->unreliable_endpoint))
			{
				buffer_size = read_endpoint(
					connection->unreliable_endpoint,
					buffer,
					DATAGRAM_MAXIMUM_SIZE);
				if (buffer_size > 0)
				{
					if (get_endpoint_address(connection->unreliable_endpoint, &source_address) != _transport_error_none)
					{
						memset(&source_address, 0, sizeof(source_address));
						source_address.address_length = IPV4_ADDRESS_LENGTH;
					}
					network_connection_log_traffic_event(
						_network_connection_traffic_event_datagram_received,
						buffer_size,
						connection);
				}
			}
			else
			{
				buffer_size = read_from_endpoint(
					connection->unreliable_endpoint,
					buffer,
					DATAGRAM_MAXIMUM_SIZE,
					&source_address);
				if (buffer_size > 0)
				{
					network_connection_log_traffic_event(
						_network_connection_traffic_event_datagram_received,
						buffer_size,
						connection);
				}
			}

			match_vassert(
				"c:\\halo\\SOURCE\\networking\\network_connection.c",
				0x26D,
				buffer_size <= DATAGRAM_MAXIMUM_SIZE,
				"endpoint read buffer overflowed");
			if (buffer_size <= 0)
			{
				return success;
			}

			source_ipv4_address = source_address.address.long_words[0];
			if (source_ipv4_address)
			{
				csmemcpy(
					buffer + buffer_size,
					&source_ipv4_address,
					sizeof(source_ipv4_address));
				buffer_size += sizeof(source_ipv4_address);
				success = circular_queue_queue_data(
					connection->unreliable_incoming_queue,
					buffer,
					buffer_size);
				match_vassert(
					"c:\\halo\\SOURCE\\networking\\network_connection.c",
					0x279,
					success,
					"circular_queue_queue_data() failed though it should have had enough room");
			}
			else
			{
				error(_error_silent, "datagram received from unknown address");
			}

			free_space = circular_queue_free_space(connection->unreliable_incoming_queue);
		}
	}

	return success;
}

/* ---------- private code */
