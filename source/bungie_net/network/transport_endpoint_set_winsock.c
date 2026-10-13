/*
TRANSPORT_ENDPOINT_SET_WINSOCK.C

symbols in this file:
00070420 0010:
	_net_startup_debug (0000)
00070430 0040:
	_get_next_available_set_array_index (0000)
00070470 0080:
	_transport_push_key (0000)
000704F0 0040:
	_transport_pop_key (0000)
00070530 0070:
	_transport_get_nonce (0000)
000705A0 0070:
	_transport_nonce_is_equal (0000)
00070610 0070:
	_transport_is_nonce (0000)
00070680 0050:
	_transport_client_stop (0000)
000706D0 0030:
	_transport_get_xnaddr (0000)
00070700 0040:
	_transport_get_key_id (0000)
00070740 0060:
	_transport_get_key (0000)
000707A0 01a0:
	_transport_initialize (0000)
00070940 0030:
	_transport_dispose (0000)
00070970 0010:
	_transport_network_available (0000)
00070980 0100:
	_create_endpoint_set (0000)
00070A80 0090:
	_delete_endpoint_set (0000)
00070B10 0030:
	_poll_ep_array_compare_proc (0000)
00070B40 0230:
	_poll_endpoint_set (0000)
00070D70 0150:
	_add_endpoint_to_set (0000)
00070EC0 00f0:
	_remove_endpoint_from_set (0000)
00070FB0 0070:
	_rewind_endpoint_set (0000)
00071020 0080:
	_get_next_endpoint_from_set (0000)
000710A0 0060:
	_count_endpoints_in_set (0000)
00071100 00a0:
	_transport_server_initialize (0000)
000711A0 00a0:
	_transport_server_terminate (0000)
00071240 00c0:
	_transport_client_start (0000)
00256000 0004:
	??_C@_03KCHOJKKI@set?$AA@ (0000)
00256008 0043:
	??_C@_0ED@JLJCEPLC@c?3?2halo?2SOURCE?2bungie_net?2networ@ (0000)
0025604C 000b:
	??_C@_0L@LJNLIMFC@0?5?$DN?$DN?5error?$AA@ (0000)
00256058 0015:
	??_C@_0BF@BILAEEIK@global_key_depth?5?$DO?50?$AA@ (0000)
00256070 001e:
	??_C@_0BO@NAEOGANL@bytes?5?$DN?$DN?5sizeof?$CIglobal_nonce?$CJ?$AA@ (0000)
00256090 000c:
	??_C@_0M@HAHFDDCN@dst?5?$CB?$DN?5NULL?$AA@ (0000)
0025609C 000c:
	??_C@_0M@MAJHCAP@src?5?$CB?$DN?5NULL?$AA@ (0000)
002560A8 0022:
	??_C@_0CC@EEMKELAP@XNET_STARTUP_BYPASS_SECURITY?5?$FLON@ (0000)
002560CC 0017:
	??_C@_0BH@JKFKBOIJ@d?3?2bypass_security?4txt?$AA@ (0000)
002560E4 0021:
	??_C@_0CB@GPCGELJA@xbox?5ethernet?5link?5is?5?$CFs?$CFs?$CFs?$CFs?$CFs@ (0000)
00256108 000e:
	??_C@_0O@GLMIBBEG@not?5connected?$AA@ (0000)
00256118 000a:
	??_C@_09JGOMOAGJ@connected?$AA@ (0000)
00256124 000d:
	??_C@_0N@IDEDDGND@?5at?5100?5Mbps?$AA@ (0000)
00256134 000c:
	??_C@_0M@PHBFBOED@?5at?510?5Mbps?$AA@ (0000)
00256140 0015:
	??_C@_0BF@IFMPKCKC@?5in?5full?9duplex?5mode?$AA@ (0000)
00256158 0015:
	??_C@_0BF@CEMKPCLG@?5in?5half?9duplex?5mode?$AA@ (0000)
00256170 0012:
	??_C@_0BC@JBFMBDMK@max_endpoints?5?$DO?50?$AA@ (0000)
00256184 0015:
	??_C@_0BF@KAELMCII@set?5?$CG?$CG?5set?9?$DOep_array?$AA@ (0000)
0025619C 000a:
	??_C@_09OCPCIGPM@ep?5?$CG?$CG?5set?$AA@ (0000)
002561A8 0016:
	??_C@_0BG@FPPJIENA@0?5?$DN?$DN?5global_key_depth?$AA@ (0000)
0031CE30 0008:
	_transport_initialized (0000)
	_global_client_active (0001)
	_global_key_depth (0004)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "cseries/errors.h"
#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint_winsock.h"
#include "memory/byte_swapping.h"

/* ---------- constants */

enum
{
	TRANSPORT_NONCE_LENGTH = 8,
};

/* ---------- macros */

/* ---------- structures */

struct server_transport_globals
{
	boolean initialized;
};

struct transport_endpoint;

struct transport_endpoint_set
{
	fd_set sockets;
	struct transport_endpoint **ep_array;
	long max_endpoints;
	long last_endpoint_index;
	long current_endpoint_index;
	long needs_compaction;
};

#ifdef HALO_LINUX
/* the native builds raise FD_SETSIZE (port prefix headers) so that a server's
set holds its listening socket and a socket for every machine */
typedef char winsock_fd_set_size_assert[
	FD_SETSIZE >= HALO_PORT_MAXIMUM_NETWORK_MACHINES + 1 ? 1 : -1];
#else
typedef char winsock_fd_set_size_assert[
	sizeof(fd_set) == 0x104 ? 1 : -1];
typedef char transport_endpoint_set_ep_array_offset_assert[
	offsetof(struct transport_endpoint_set, ep_array) == 0x104 ? 1 : -1];
typedef char transport_endpoint_set_max_endpoints_offset_assert[
	offsetof(struct transport_endpoint_set, max_endpoints) == 0x108 ? 1 : -1];
typedef char transport_endpoint_set_last_endpoint_index_offset_assert[
	offsetof(struct transport_endpoint_set, last_endpoint_index) == 0x10C ? 1 : -1];
typedef char transport_endpoint_set_current_endpoint_index_offset_assert[
	offsetof(struct transport_endpoint_set, current_endpoint_index) == 0x110 ? 1 : -1];
typedef char transport_endpoint_set_needs_compaction_offset_assert[
	offsetof(struct transport_endpoint_set, needs_compaction) == 0x114 ? 1 : -1];
typedef char transport_endpoint_set_size_assert[
	sizeof(struct transport_endpoint_set) == 0x118 ? 1 : -1];
#endif

/* ---------- prototypes */

static long get_next_available_set_array_index(
	struct transport_endpoint_set *set);
static int __cdecl poll_ep_array_compare_proc(
	void const *a,
	void const *b);

/* ---------- globals */

boolean transport_initialized = FALSE;
boolean global_client_active = FALSE;
long global_key_depth = 0;
struct server_transport_globals server_transport_globals;
extern XNADDR global_address;
extern XNKID global_key_id;
extern XNKEY global_key;
extern byte global_nonce[TRANSPORT_NONCE_LENGTH];

/* ---------- private code */

void net_startup_debug(
	void)
{
	return;
}

static long get_next_available_set_array_index(
	struct transport_endpoint_set *set)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x39,
		set);
	/* January permits one-past-capacity here; preserve the original boundary. */
#ifdef HALO_LINUX
	/* ... except in the native builds, which keep to the array */
	if (set->last_endpoint_index >= set->max_endpoints - 1)
#else
	if (set->last_endpoint_index > set->max_endpoints - 1)
#endif
	{
		return NONE;
	}
	return set->last_endpoint_index + 1;
}

/* ---------- public code */

void transport_push_key(
	const XNKEY *key,
	const XNKID *key_id)
{
	int error;

	global_key = *key;
	global_key_id = *key_id;
	if (global_key_depth == 0)
	{
		error = XNetRegisterKey(&global_key_id, &global_key);
		match_assert(
			"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
			0x5C,
			0 == error);
	}
	global_key_depth++;
	return;
}

XNADDR *transport_get_xnaddr(
	XNADDR *address)
{
	*address = global_address;
	return address;
}

XNKID transport_get_key_id(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xE0,
		global_key_depth > 0);
	return global_key_id;
}

XNKEY transport_get_key(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xE7,
		global_key_depth > 0);
	return global_key;
}

void transport_pop_key(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x66,
		global_key_depth > 0);
	global_key_depth--;
	if (global_key_depth == 0)
	{
		XNetUnregisterKey(&global_key_id);
	}
	return;
}

short transport_initialize(
	void)
{
	if (!transport_initialized)
	{
		WSADATA wsa_data = { 0 };
		XNetStartupParams startup_params = { 0 };
		DWORD link_status;
		DWORD address_status;
		unsigned long deadline;
		FILE *bypass_file;
		short wsa_error;

		startup_params.cfgSizeOfStruct = sizeof(startup_params);
		startup_params.cfgPrivatePoolSizeInPages = 24;
		startup_params.cfgEnetReceiveQueueLength = 8;
		startup_params.cfgIpFragMaxSimultaneous = 4;
		startup_params.cfgIpFragMaxPacketDiv256 = 8;
		startup_params.cfgSockMaxSockets = 128;
		startup_params.cfgKeyRegMax = 1;
		startup_params.cfgSecRegMax = 32;

		link_status = XNetGetEthernetLinkStatus();
		error(
			_error_log,
			"xbox ethernet link is %s%s%s%s%s",
			(link_status & XNET_ETHERNET_LINK_ACTIVE) ? "connected" : "not connected",
			(link_status & XNET_ETHERNET_LINK_100MBPS) ? " at 100 Mbps" : "",
			(link_status & XNET_ETHERNET_LINK_10MBPS) ? " at 10 Mbps" : "",
			(link_status & XNET_ETHERNET_LINK_FULL_DUPLEX) ? " in full-duplex mode" : "",
			(link_status & XNET_ETHERNET_LINK_HALF_DUPLEX) ? " in half-duplex mode" : "");

		bypass_file = fopen("d:\\bypass_security.txt", "r");
		if (bypass_file)
		{
			error(_error_silent, "XNET_STARTUP_BYPASS_SECURITY [ON]");
			startup_params.cfgFlags |= XNET_STARTUP_BYPASS_SECURITY;
			fclose(bypass_file);
		}

		if (XNetStartup(&startup_params) != 0)
			return _transport_error_not_initialized;

		wsa_error = WSAStartup(MAKEWORD(2, 0), &wsa_data);
		if (wsa_error != 0)
		{
			XNetCleanup();
			winsock_error_to_string(wsa_error);
			return _transport_error_not_initialized;
		}

		deadline = system_milliseconds() + 10000;
		do
		{
			address_status = XNetGetTitleXnAddr(&global_address);
			if (system_milliseconds() > deadline ||
				address_status == XNET_GET_XNADDR_NONE)
			{
				WSACleanup();
				XNetCleanup();
				return _transport_error_not_initialized;
			}
		}
		while (address_status == XNET_GET_XNADDR_PENDING);

		XNetRandom(global_nonce, sizeof(global_nonce));
		transport_initialized = TRUE;
	}

	return _transport_error_none;
}

void transport_client_stop(
	void)
{
	if (global_client_active)
	{
		transport_pop_key();
		global_client_active = FALSE;
	}
	return;
}

short transport_server_initialize(
	void)
{
	XNKEY key;
	XNKID key_id;

	transport_client_stop();
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x79,
		0 == global_key_depth);
	server_transport_globals.initialized = TRUE;
	XNetCreateKey(&key_id, &key);
	transport_push_key(&key, &key_id);
	return _transport_error_none;
}

short transport_server_terminate(
	void)
{
	transport_client_stop();
	transport_pop_key();
	memset(&server_transport_globals, 0, sizeof(server_transport_globals));
	return _transport_error_none;
}

short transport_dispose(
	void)
{
	short result = _transport_error_none;

	if (transport_initialized)
	{
		WSACleanup();
		XNetCleanup();
		transport_initialized = FALSE;
	}
	else
	{
		result = _transport_error_not_initialized;
	}

	return result;
}

boolean transport_network_available(
	void)
{
	boolean available = XNetGetEthernetLinkStatus() & XNET_ETHERNET_LINK_ACTIVE;

	return available;
}

struct transport_endpoint_set *create_endpoint_set(
	short max_endpoints)
{
	struct transport_endpoint_set *set;

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x196,
		transport_initialized);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x197,
		max_endpoints > 0);
	set = match_malloc(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x199,
		sizeof(*set));
	if (set)
	{
		if (max_endpoints <= FD_SETSIZE)
		{
			set->needs_compaction = FALSE;
			FD_ZERO(&set->sockets);
			set->ep_array = debug_malloc(
				max_endpoints * sizeof(*set->ep_array),
				TRUE,
				"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
				0x1A2);
			if (set->ep_array)
			{
				set->max_endpoints = max_endpoints;
				set->last_endpoint_index = NONE;
				set->current_endpoint_index = 0;
			}
			else
			{
				match_free(
					"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
					0x1AA,
					set);
				set = NULL;
			}
		}
		else
		{
			match_free(
				"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
				0x1B0,
				set);
			set = NULL;
		}
	}
	return set;
}

short delete_endpoint_set(
	struct transport_endpoint_set *set)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1BB,
		set && set->ep_array);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1BC,
		transport_initialized);
	match_free(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1BE,
		set->ep_array);
	match_free(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1BF,
		set);
	return _transport_error_none;
}

/* ---------- private code */

static int __cdecl poll_ep_array_compare_proc(
	void const *a,
	void const *b)
{
	struct transport_endpoint *endpoint_a = *(struct transport_endpoint *const *)a;
	struct transport_endpoint *endpoint_b = *(struct transport_endpoint *const *)b;

	if (!endpoint_a && endpoint_b)
	{
		return 1;
	}
	else if (endpoint_a && !endpoint_b)
	{
		return -1;
	}
	return 0;
}

/* ---------- public code */

short poll_endpoint_set(
	struct transport_endpoint_set *set,
	word timeout)
{
	short result = _transport_error_none;
	struct timeval timeout_value;
	fd_set readable_sockets;
	long endpoint_index = 0;
	long select_result;

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1DD,
		set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x1DE,
		transport_initialized);

	timeout_value.tv_usec = timeout * MILLISECONDS_PER_SECOND;
	timeout_value.tv_sec = 0;

	if (set->needs_compaction)
	{
		qsort(
			set->ep_array,
			set->last_endpoint_index + 1,
			sizeof(*set->ep_array),
			poll_ep_array_compare_proc);
		/* January has no lower-bound guard when every entry has been removed. */
		while (!set->ep_array[set->last_endpoint_index])
		{
			set->last_endpoint_index--;
		}

		FD_ZERO(&set->sockets);
		while (endpoint_index <= set->last_endpoint_index)
		{
			FD_SET(set->ep_array[endpoint_index]->socket, &set->sockets);
			SET_FLAG(set->ep_array[endpoint_index]->flags, _transport_endpoint_readable_bit, FALSE);
			endpoint_index++;
		}
		set->needs_compaction = FALSE;
	}
	else
	{
		while (endpoint_index <= set->last_endpoint_index)
		{
			SET_FLAG(set->ep_array[endpoint_index]->flags, _transport_endpoint_readable_bit, FALSE);
			endpoint_index++;
		}
	}

	memcpy(&readable_sockets, &set->sockets, sizeof(readable_sockets));
	select_result = select(
		set->last_endpoint_index + 1,
		&readable_sockets,
		NULL,
		NULL,
		&timeout_value);
	if (select_result > 0)
	{
		for (endpoint_index = 0;
			endpoint_index <= set->last_endpoint_index;
			endpoint_index++)
		{
			if (set->ep_array[endpoint_index]->socket == INVALID_SOCKET)
			{
				result = _transport_error_bad_endpoint;
				break;
			}
			if (FD_ISSET(set->ep_array[endpoint_index]->socket, &readable_sockets))
			{
				SET_FLAG(set->ep_array[endpoint_index]->flags, _transport_endpoint_readable_bit, TRUE);
			}
		}
	}
	else if (select_result < 0)
	{
		winsock_error_to_string(WSAGetLastError());
		result = _transport_error_poll_error;
	}
	else
	{
		result = _transport_result_poll_timeout;
	}
	return result;
}

short add_endpoint_to_set(
	struct transport_endpoint *ep,
	struct transport_endpoint_set *set)
{
	short result = _transport_error_none;
	long endpoint_index;

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x22F,
		ep && set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x230,
		transport_initialized);

	endpoint_index = get_next_available_set_array_index(set);
	if (endpoint_index >= 0)
	{
		set->ep_array[endpoint_index] = ep;
		/* the listening test with two identical FD_SET arms is original: January and the first-party debug build both have it */
		if (TEST_FLAG(ep->flags, _transport_endpoint_listening_bit))
		{
			FD_SET(set->ep_array[endpoint_index]->socket, &set->sockets);
		}
		else
		{
			FD_SET(set->ep_array[endpoint_index]->socket, &set->sockets);
		}
		set->last_endpoint_index++;
		SET_FLAG(ep->flags, _transport_endpoint_in_set_bit, TRUE);
	}
	else
	{
		result = _transport_error_endpoint_set_full;
	}
	return result;
}

short remove_endpoint_from_set(
	struct transport_endpoint *ep,
	struct transport_endpoint_set *set)
{
	short result = _transport_error_endpoint_not_in_set;
	long endpoint_index = 0;

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x255,
		ep && set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x256,
		transport_initialized);

	while (endpoint_index <= set->last_endpoint_index)
	{
		if (set->ep_array[endpoint_index] == ep)
		{
			FD_CLR(ep->socket, &set->sockets);
			SET_FLAG(ep->flags, _transport_endpoint_in_set_bit, FALSE);
			set->ep_array[endpoint_index] = NULL;
			set->needs_compaction = TRUE;
			result = _transport_error_none;
			break;
		}
		endpoint_index++;
	}
	return result;
}

void rewind_endpoint_set(
	struct transport_endpoint_set *set)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x26D,
		set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x26E,
		transport_initialized);
	set->current_endpoint_index = 0;
	return;
}

struct transport_endpoint *get_next_endpoint_from_set(
	struct transport_endpoint_set *set)
{
	struct transport_endpoint *endpoint = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x27A,
		set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x27B,
		transport_initialized);
	if (set->current_endpoint_index <= set->last_endpoint_index)
	{
		endpoint = set->ep_array[set->current_endpoint_index++];
	}
	return endpoint;
}

long count_endpoints_in_set(
	struct transport_endpoint_set *set)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x289,
		set);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x28A,
		transport_initialized);
	return set->last_endpoint_index + 1;
}


void transport_get_nonce(
	void *dst,
	long bytes)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x97,
		dst != NULL);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0x98,
		bytes == sizeof(global_nonce));
	memcpy(dst, global_nonce, sizeof(global_nonce));
	return;
}

boolean transport_nonce_is_equal(
	void const *src,
	void const *dst)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xA3,
		src != NULL);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xA4,
		dst != NULL);

	return (boolean)(memcmp(src, dst, sizeof(global_nonce)) == 0);
}

boolean transport_is_nonce(
	void const *src,
	long bytes)
{
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xAF,
		src != NULL);
	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_set_winsock.c",
		0xB0,
		bytes == sizeof(global_nonce));
	return transport_nonce_is_equal(src, global_nonce);
}

void transport_client_start(
	XNADDR const *xnaddr,
	XNKEY const *key,
	XNKID const *key_id,
	word port,
	struct transport_address *address)
{
	IN_ADDR in_addr;

	transport_client_stop();
	transport_push_key(key, key_id);
	XNetXnAddrToInAddr(xnaddr, key_id, &in_addr);
	address->address.long_words[0] = SWAP4(in_addr.s_addr);
	address->address_length = IPV4_ADDRESS_LENGTH;
	address->port = port;
	address->address_type = 0;
	global_client_active = TRUE;

	return;
}

/* ---------- private code */
