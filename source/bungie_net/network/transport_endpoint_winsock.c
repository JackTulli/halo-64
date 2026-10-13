/*
TRANSPORT_ENDPOINT_WINSOCK.C

symbols in this file:
00071300 0060:
	_add_connect_thread (0000)
00071360 0040:
	_mark_connection_thread_as_terminated (0000)
000713A0 0040:
	_connection_thread_list_maintenance (0000)
000713E0 0080:
	_create_transport_endpoint (0000)
00071460 0060:
	_get_endpoint_type (0000)
000714C0 0100:
	_read_endpoint (0000)
000715C0 00f0:
	_write_endpoint (0000)
000716B0 00c0:
	_endpoint_readable (0000)
00071770 00a0:
	_endpoint_writeable (0000)
00071810 0040:
	_endpoint_connected (0000)
00071850 0040:
	_endpoint_listening (0000)
00071890 0040:
	_endpoint_blocking (0000)
000718D0 0040:
	_get_endpoint_error (0000)
00071910 0070:
	_endpoint_equivalent (0000)
00071980 0620:
	_winsock_error_to_string (0000)
00071FA0 0130:
	_create_socket (0000)
000720D0 0170:
	_get_endpoint_address (0000)
00072240 0110:
	_set_endpoint_blocking (0000)
00072350 0140:
	_bind_endpoint (0000)
00072490 01e0:
	_connect_endpoint (0000)
00072670 0080:
	_disconnect_endpoint (0000)
000726F0 0130:
	_connect_async_thread_proc@4 (0000)
00072820 0150:
	_connect_endpoint_async (0000)
00072970 00a0:
	_cancel_connect_process (0000)
00072A10 00b0:
	_listen_endpoint (0000)
00072AC0 00d0:
	_accept_endpoint (0000)
00072B90 0220:
	_read_from_endpoint (0000)
00072DB0 0180:
	_write_to_endpoint (0000)
00072F30 0080:
	_delete_transport_endpoint (0000)
00072FB0 0030:
	_reject_endpoint (0000)
002561C0 0007:
	??_C@_06FJHNOCKE@thread?$AA@ (0000)
002561C8 003f:
	??_C@_0DP@BJNHIBAC@c?3?2halo?2SOURCE?2bungie_net?2networ@ (0000)
00256208 0003:
	??_C@_02GBJNFGNA@ep?$AA@ (0000)
0025620C 001d:
	??_C@_0BN@LCGPPBOK@ep?5?$CG?$CG?5buffer?5?$CG?$CG?5?$CIlength?5?$DO?50?$CJ?$AA@ (0000)
0025622C 0025:
	??_C@_0CF@LILAICMM@ep?5?$CG?$CG?5?$CIep?9?$DOsocket?5?$CB?$DN?5INVALID_SOC@ (0000)
00256254 0016:
	??_C@_0BG@JGDABANF@winsock?5error?5?$CD?$CFd?3?5?$CFs?$AA@ (0000)
0025626C 0010:
	??_C@_0BA@MMFHLOKK@?$DMunknown?5error?$DO?$AA@ (0000)
0025627C 0016:
	??_C@_0BG@MLBCKNNP@WSA_QOS_GENERIC_ERROR?$AA@ (0000)
00256294 001b:
	??_C@_0BL@FIDPCPEO@WSA_QOS_TRAFFIC_CTRL_ERROR?$AA@ (0000)
002562B0 0013:
	??_C@_0BD@BLONOKBN@WSA_QOS_BAD_OBJECT?$AA@ (0000)
002562C4 0012:
	??_C@_0BC@NKDHAC@WSA_QOS_BAD_STYLE?$AA@ (0000)
002562D8 0017:
	??_C@_0BH@OPFHJNHJ@WSA_QOS_POLICY_FAILURE?$AA@ (0000)
002562F0 001a:
	??_C@_0BK@CCDCMKNF@WSA_QOS_ADMISSION_FAILURE?$AA@ (0000)
0025630C 001a:
	??_C@_0BK@IGGBDKMP@WSA_QOS_REQUEST_CONFIRMED?$AA@ (0000)
00256328 0015:
	??_C@_0BF@NCBEDIJO@WSA_QOS_NO_RECEIVERS?$AA@ (0000)
00256340 0013:
	??_C@_0BD@EICDPPFO@WSA_QOS_NO_SENDERS?$AA@ (0000)
00256354 0010:
	??_C@_0BA@FBDFNBLN@WSA_QOS_SENDERS?$AA@ (0000)
00256364 0012:
	??_C@_0BC@HLJBIMGI@WSA_QOS_RECEIVERS?$AA@ (0000)
00256378 000b:
	??_C@_0L@CBLHPKCP@WSANO_DATA?$AA@ (0000)
00256384 000f:
	??_C@_0P@HLNEBJNJ@WSANO_RECOVERY?$AA@ (0000)
00256394 000d:
	??_C@_0N@LHDAEOPK@WSATRY_AGAIN?$AA@ (0000)
002563A4 0012:
	??_C@_0BC@JCJCFNAO@WSAHOST_NOT_FOUND?$AA@ (0000)
002563B8 000c:
	??_C@_0M@NLNGHJCJ@WSAEREFUSED?$AA@ (0000)
002563C4 0010:
	??_C@_0BA@MEKAPNF@WSA_E_CANCELLED?$AA@ (0000)
002563D4 000e:
	??_C@_0O@LPJENGIE@WSA_E_NO_MORE?$AA@ (0000)
002563E4 0012:
	??_C@_0BC@CHACNBCC@WSATYPE_NOT_FOUND?$AA@ (0000)
002563F8 0015:
	??_C@_0BF@IEFJBMNO@WSASERVICE_NOT_FOUND?$AA@ (0000)
00256410 0012:
	??_C@_0BC@JMKCGGLG@WSASYSCALLFAILURE?$AA@ (0000)
00256424 0017:
	??_C@_0BH@MFNMMHCD@WSAEPROVIDERFAILEDINIT?$AA@ (0000)
0025643C 0014:
	??_C@_0BE@BDJFCAFA@WSAEINVALIDPROVIDER?$AA@ (0000)
00256450 0015:
	??_C@_0BF@CPFHOEAL@WSAEINVALIDPROCTABLE?$AA@ (0000)
00256468 000e:
	??_C@_0O@NAJLOKAM@WSAECANCELLED?$AA@ (0000)
00256478 000b:
	??_C@_0L@FEFCEDKC@WSAENOMORE?$AA@ (0000)
00256484 000b:
	??_C@_0L@EEHHIAEC@WSAEDISCON?$AA@ (0000)
00256490 0012:
	??_C@_0BC@CLCHBKPK@WSANOTINITIALISED?$AA@ (0000)
002564A4 0013:
	??_C@_0BD@PKACBPOA@WSAVERNOTSUPPORTED?$AA@ (0000)
002564B8 000f:
	??_C@_0P@MDJAKAKC@WSASYSNOTREADY?$AA@ (0000)
002564C8 000b:
	??_C@_0L@FBDKOKMM@WSAEREMOTE?$AA@ (0000)
002564D4 000a:
	??_C@_09GPMJAMEI@WSAESTALE?$AA@ (0000)
002564E0 000a:
	??_C@_09DHBOOMJK@WSAEDQUOT?$AA@ (0000)
002564EC 000a:
	??_C@_09IOPCJPLP@WSAEUSERS?$AA@ (0000)
002564F8 000c:
	??_C@_0M@CDIHOKBK@WSAEPROCLIM?$AA@ (0000)
00256504 000d:
	??_C@_0N@KFICKHOH@WSAENOTEMPTY?$AA@ (0000)
00256514 0010:
	??_C@_0BA@HKCFHLFG@WSAEHOSTUNREACH?$AA@ (0000)
00256524 000d:
	??_C@_0N@NJBKCOKK@WSAEHOSTDOWN?$AA@ (0000)
00256534 0010:
	??_C@_0BA@PCKIJCGI@WSAENAMETOOLONG?$AA@ (0000)
00256544 0009:
	??_C@_08PGIMLMFD@WSAELOOP?$AA@ (0000)
00256550 0010:
	??_C@_0BA@ECEEDLKJ@WSAECONNREFUSED?$AA@ (0000)
00256560 000d:
	??_C@_0N@BJEDIOFA@WSAETIMEDOUT?$AA@ (0000)
00256570 0010:
	??_C@_0BA@COKPFEMB@WSAETOOMANYREFS?$AA@ (0000)
00256580 000d:
	??_C@_0N@JOJKMOCF@WSAESHUTDOWN?$AA@ (0000)
00256590 000c:
	??_C@_0M@BJHJCMPN@WSAENOTCONN?$AA@ (0000)
0025659C 000b:
	??_C@_0L@KMBJICHC@WSAEISCONN?$AA@ (0000)
002565A8 000b:
	??_C@_0L@OOBBNNPD@WSAENOBUFS?$AA@ (0000)
002565B4 000e:
	??_C@_0O@CIPFFFJE@WSAECONNRESET?$AA@ (0000)
002565C4 0010:
	??_C@_0BA@HNCPIGIO@WSAECONNABORTED?$AA@ (0000)
002565D4 000d:
	??_C@_0N@KABONKCJ@WSAENETRESET?$AA@ (0000)
002565E4 000f:
	??_C@_0P@MMGFNLEA@WSAENETUNREACH?$AA@ (0000)
002565F4 000c:
	??_C@_0M@BMNODHFM@WSAENETDOWN?$AA@ (0000)
00256600 0011:
	??_C@_0BB@MBBPJMGK@WSAEADDRNOTAVAIL?$AA@ (0000)
00256614 000e:
	??_C@_0O@OPDDJBEB@WSAEADDRINUSE?$AA@ (0000)
00256624 0010:
	??_C@_0BA@DNDGDCNF@WSAEAFNOSUPPORT?$AA@ (0000)
00256634 0010:
	??_C@_0BA@KKILJDEG@WSAEPFNOSUPPORT?$AA@ (0000)
00256644 000e:
	??_C@_0O@IFNEEJHC@WSAEOPNOTSUPP?$AA@ (0000)
00256654 0013:
	??_C@_0BD@KNONLHOA@WSAESOCKTNOSUPPORT?$AA@ (0000)
00256668 0013:
	??_C@_0BD@EDJBJFH@WSAEPROTONOSUPPORT?$AA@ (0000)
0025667C 000f:
	??_C@_0P@KODINEND@WSAENOPROTOOPT?$AA@ (0000)
0025668C 000e:
	??_C@_0O@HCKGAENP@WSAEPROTOTYPE?$AA@ (0000)
0025669C 000c:
	??_C@_0M@PJKLHKI@WSAEMSGSIZE?$AA@ (0000)
002566A8 0010:
	??_C@_0BA@BLGPIJAD@WSAEDESTADDRREQ?$AA@ (0000)
002566B8 000c:
	??_C@_0M@MDGNMGJ@WSAENOTSOCK?$AA@ (0000)
002566C4 000c:
	??_C@_0M@EAGDEEAC@WSAEALREADY?$AA@ (0000)
002566D0 000f:
	??_C@_0P@FHEJEBJC@WSAEINPROGRESS?$AA@ (0000)
002566E0 000f:
	??_C@_0P@BLDHJENA@WSAEWOULDBLOCK?$AA@ (0000)
002566F0 000a:
	??_C@_09IJIBIDKO@WSAEMFILE?$AA@ (0000)
002566FC 000a:
	??_C@_09PDAAOFKK@WSAEINVAL?$AA@ (0000)
00256708 000a:
	??_C@_09BIHAGEEK@WSAEFAULT?$AA@ (0000)
00256714 000a:
	??_C@_09EKFHKCJJ@WSAEACCES?$AA@ (0000)
00256720 0009:
	??_C@_08LJKOCPDH@WSAEBADF?$AA@ (0000)
0025672C 0009:
	??_C@_08KEJFHKFF@WSAEINTR?$AA@ (0000)
00256738 000f:
	??_C@_0P@NPGBNDPH@WSA_IO_PENDING?$AA@ (0000)
00256748 0012:
	??_C@_0BC@FBGHLLOK@WSA_IO_INCOMPLETE?$AA@ (0000)
0025675C 0016:
	??_C@_0BG@FAAIAFCH@WSA_OPERATION_ABORTED?$AA@ (0000)
00256774 0017:
	??_C@_0BH@FPLHJKEA@WSA_WAIT_IO_COMPLETION?$AA@ (0000)
0025678C 0011:
	??_C@_0BB@IEEIFHNM@WSA_WAIT_TIMEOUT?$AA@ (0000)
002567A0 0016:
	??_C@_0BG@PLFAIPGK@WSA_INVALID_PARAMETER?$AA@ (0000)
002567B8 0010:
	??_C@_0BA@FAMENHPI@WSA_WAIT_FAILED?$AA@ (0000)
002567C8 0018:
	??_C@_0BI@MPMIPNBJ@WSA_MAXIMUM_WAIT_EVENTS?$AA@ (0000)
002567E0 0012:
	??_C@_0BC@KMEIGLBE@WSA_INVALID_EVENT?$AA@ (0000)
002567F4 0016:
	??_C@_0BG@GHKDMPOF@WSA_NOT_ENOUGH_MEMORY?$AA@ (0000)
0025680C 0013:
	??_C@_0BD@OCCEFHAI@WSA_INVALID_HANDLE?$AA@ (0000)
00256820 000e:
	??_C@_0O@HPGLJJN@ep?5?$CG?$CG?5address?$AA@ (0000)
00256830 000e:
	??_C@_0O@HEFAJMIG@input?9?$DOthread?$AA@ (0000)
00256840 000a:
	??_C@_09GPBDNHNE@input?9?$DOep?$AA@ (0000)
0025684C 0006:
	??_C@_05DFJCHPDH@input?$AA@ (0000)
00256854 0021:
	??_C@_0CB@MKIOMIKP@ep?5?$CG?$CG?5address?5?$CG?$CG?5process_ref_ptr@ (0000)
00256878 0034:
	??_C@_0DE@NJDDCKIE@?$CB?$CCunable?5to?5get?5mutex?5in?5cancel_@ (0000)
002568AC 0024:
	??_C@_0CE@KNDEECJL@input?5?$CG?$CG?5input?9?$DOep?5?$CG?$CG?5input?9?$DOthr@ (0000)
002568D0 0038:
	??_C@_0DI@BLIDBCNB@listening_endpoint?5?$CG?$CG?5?$CIlistening@ (0000)
00256908 0018:
	??_C@_0BI@JLDBFBGK@?$CBendpoint_connected?$CIep?$CJ?$AA@ (0000)
00256920 001d:
	??_C@_0BN@IMCCCKCG@err?5?$DN?$DN?5_transport_error_none?$AA@ (0000)
00256940 0020:
	??_C@_0CA@KHMMBKBD@ep?9?$DOtype?5?$DN?$DN?5_transport_type_udp?$AA@ (0000)
00256960 0029:
	??_C@_0CJ@GBBFPHGP@ep?5?$CG?$CG?5buffer?5?$CG?$CG?5src_addr?5?$CG?$CG?5?$CIlen@ (0000)
0025698C 002a:
	??_C@_0CK@OGLHOBHI@ep?5?$CG?$CG?5buffer?5?$CG?$CG?5?$CIlength?5?$DO?50?$CJ?5?$CG?$CG?5@ (0000)
0031CE38 020c:
	_transport_endpoint_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/cseries_windows.h"
#include "cseries/errors.h"

#include "bungie_net/common/thread.h"
#include "memory/byte_swapping.h"

#include "bungie_net/network/transport.h"
#include "bungie_net/network/transport_endpoint_winsock.h"

/* ---------- constants */

enum
{
	_transport_type_udp = 0x11,
	_transport_type_tcp,
#ifdef HALO_LINUX
	/* every machine of a session may connect at once */
	MAXIMUM_PENDING_CONNECTIONS = HALO_PORT_MAXIMUM_NETWORK_MACHINES,
#else
	MAXIMUM_PENDING_CONNECTIONS = 32,
#endif
	MAXIMUM_ENDPOINT_THREADS = 64,
#ifdef HALO_LINUX
	/* a host's connection to a machine holds the updates that machine has
	not acknowledged, up to 128 ticks of 3.9 KB with 128 players, and the
	host's connection to its own client all it sends in one frame; a full
	buffer blocks the host (network_connection_write) */
	MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE = 1024 * 1024,
#else
	MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE = 16 * 1024,
#endif
};

/* ---------- macros */

#define TRANSPORT_ENDPOINT_WINSOCK_FILE "c:\\halo\\SOURCE\\bungie_net\\network\\transport_endpoint_winsock.c"

/* ---------- structures */

struct endpoint_thread_reference
{
	struct thread_reference *thread;
	boolean dispose;
	byte pad[3];
};

struct connect_process_input
{
	struct transport_endpoint *ep;
	struct transport_address address;
	struct thread_reference *thread;
	struct mutex_reference *mutex;
	boolean cancelled;
	byte pad25[3];
};

struct transport_endpoint_winsock_globals
{
	char const *error_string;
	long unknown4;
	struct endpoint_thread_reference endpoint_threads[MAXIMUM_ENDPOINT_THREADS];
	long last_error;
};

/* ---------- prototypes */

static SOCKET create_socket(
	int address_family,
	int socket_type,
	int protocol);
static boolean add_connect_thread(
	struct thread_reference *thread);
static void mark_connection_thread_as_terminated(
	struct thread_reference *thread);
static unsigned long __stdcall connect_async_thread_proc(
	void *input_pointer);
static void connection_thread_list_maintenance(
	void);

/* ---------- globals */

static struct transport_endpoint_winsock_globals transport_endpoint_globals = {0};

/* ---------- public code */

static boolean add_connect_thread(
	struct thread_reference *thread)
{
	long endpoint_thread_index = 0;

	while (transport_endpoint_globals.endpoint_threads[endpoint_thread_index].thread &&
		endpoint_thread_index < MAXIMUM_ENDPOINT_THREADS)
	{
		endpoint_thread_index++;
	}

	if (endpoint_thread_index < MAXIMUM_ENDPOINT_THREADS)
	{
		transport_endpoint_globals.endpoint_threads[endpoint_thread_index].thread = thread;
		transport_endpoint_globals.endpoint_threads[endpoint_thread_index].dispose = FALSE;
	}
	else
	{
		endpoint_thread_index = NONE;
	}

	return endpoint_thread_index != NONE;
}

static void mark_connection_thread_as_terminated(
	struct thread_reference *thread)
{
	long endpoint_thread_index;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x4F, thread);

	for (endpoint_thread_index = 0;
		endpoint_thread_index < MAXIMUM_ENDPOINT_THREADS;
		endpoint_thread_index++)
	{
		if (transport_endpoint_globals.endpoint_threads[endpoint_thread_index].thread == thread)
		{
			transport_endpoint_globals.endpoint_threads[endpoint_thread_index].dispose = TRUE;
			break;
		}
	}

	return;
}

static void connection_thread_list_maintenance(
	void)
{
	long endpoint_thread_index = 0;

	do
	{
		struct endpoint_thread_reference *endpoint_thread =
			&transport_endpoint_globals.endpoint_threads[endpoint_thread_index];

		if (endpoint_thread->thread && endpoint_thread->dispose)
		{
			dispose_thread(endpoint_thread->thread);
			endpoint_thread->thread = NULL;
			endpoint_thread->dispose = FALSE;
		}

		endpoint_thread_index++;
	}
	while (endpoint_thread_index < MAXIMUM_ENDPOINT_THREADS);

	return;
}

struct transport_endpoint *create_transport_endpoint(
	long type)
{
	struct transport_endpoint *ep = NULL;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xCE, transport_initialized);
	connection_thread_list_maintenance();

	if (type == _transport_type_udp || type == _transport_type_tcp)
	{
		ep = match_malloc(
			TRANSPORT_ENDPOINT_WINSOCK_FILE,
			0xD4,
			sizeof(*ep));
		if (ep)
		{
			ep->error = _transport_error_none;
			ep->type = (char)type;
			ep->socket = INVALID_SOCKET;
			ep->flags = 0;
		}
	}

	return ep;
}

long get_endpoint_type(
	struct transport_endpoint const *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x12C, ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x12D, transport_initialized);

	return ep->type;
}

long read_endpoint(
	struct transport_endpoint *ep,
	void *buffer,
	long length)
{
	long result;

	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x322,
		ep && buffer && (length > 0));
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x323, transport_initialized);

	result = recv(ep->socket, buffer, length, 0);
	if (result == SOCKET_ERROR)
	{
		switch (WSAGetLastError())
		{
		case WSAEWOULDBLOCK:
			result = _transport_result_operation_would_block;
			break;

		case WSAENETRESET:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAENOTCONN:
		case WSAESHUTDOWN:
		case WSAETIMEDOUT:
			SET_FLAG(ep->flags, _transport_endpoint_connected_bit, FALSE);
			SET_FLAG(ep->flags, _transport_endpoint_readable_bit, FALSE);
			result = _transport_error_connection_lost;
			break;

		default:
			result = _transport_error_endpoint_io;
			SET_FLAG(ep->flags, _transport_endpoint_readable_bit, FALSE);
			break;
		}

		ep->error = (word)result;
	}
	else if (result == 0)
	{
		result = _transport_error_connection_lost;
	}

	return result;
}

long write_endpoint(
	struct transport_endpoint *ep,
	void const *buffer,
	long length)
{
	long result;

	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x350,
		ep && buffer && (length > 0));
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x351, transport_initialized);

	result = send(ep->socket, buffer, length, 0);
	if (result == INVALID_SOCKET)
	{
		switch (WSAGetLastError())
		{
		case 0x2733:
			ep->error = _transport_result_operation_would_block;
			return _transport_result_operation_would_block;

		case 0x2744:
		case 0x2745:
		case 0x2746:
		case 0x2749:
		case 0x274A:
		case 0x274C:
			ep->flags &= ~1;
			ep->error = _transport_error_connection_lost;
			return _transport_error_connection_lost;

		default:
			ep->error = _transport_error_endpoint_io;
			return _transport_error_endpoint_io;
		}
	}

	return result;
}

long endpoint_connected(
	struct transport_endpoint const *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x426, ep);

	return ep->flags & 1;
}

long endpoint_listening(
	struct transport_endpoint const *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x42E, ep);

	return TEST_FLAG(ep->flags, _transport_endpoint_listening_bit);
}

boolean endpoint_readable(
	struct transport_endpoint *ep,
	word timeout)
{
	boolean readable = FALSE;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x3F3, ep);

	if (ep->socket != INVALID_SOCKET)
	{
		if (TEST_FLAG(ep->flags, _transport_endpoint_in_set_bit))
		{
			readable = TEST_FLAG(ep->flags, _transport_endpoint_readable_bit);
		}
		else
		{
			fd_set readable_sockets;
			struct timeval timeval;

			readable_sockets.fd_array[0] = ep->socket;
			timeval.tv_sec = 0;
			timeval.tv_usec = timeout * MILLISECONDS_PER_SECOND;
			readable_sockets.fd_count = 1;

			readable = select(1, &readable_sockets, NULL, NULL, &timeval) > 0 &&
				__WSAFDIsSet(ep->socket, &readable_sockets);
		}
	}

	return readable;
}

boolean endpoint_writeable(
	struct transport_endpoint *ep,
	word timeout)
{
	fd_set writeable;
	struct timeval timeval;

	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x417,
		ep && (ep->socket != INVALID_SOCKET));

	timeval.tv_sec = 0;
	timeval.tv_usec = timeout * MILLISECONDS_PER_SECOND;
	writeable.fd_array[0] = ep->socket;
	writeable.fd_count = 1;

	if (select(1, NULL, &writeable, NULL, &timeval) > 0 &&
		__WSAFDIsSet(ep->socket, &writeable))
		return TRUE;

	return FALSE;
}

boolean endpoint_blocking(
	struct transport_endpoint const *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x436, ep);

	return !TEST_FLAG(ep->flags, _transport_endpoint_nonblocking_bit);
}

short get_endpoint_error(
	struct transport_endpoint const *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x43E, ep);

	return ep->error;
}

char const *winsock_error_to_string(
	long error_code)
{
	char const *error_string;

	/* The January table names the Win32 WSA aliases. Use their underlying
	 * system constants where the XDK's WIN16 fallback has different values.
	 */
	switch (error_code)
	{
	case ERROR_INVALID_HANDLE:
		error_string = "WSA_INVALID_HANDLE";
		break;

	case ERROR_NOT_ENOUGH_MEMORY:
		error_string = "WSA_NOT_ENOUGH_MEMORY";
		break;

	case (long)WSA_INVALID_EVENT:
		error_string = "WSA_INVALID_EVENT";
		break;

	case WSA_MAXIMUM_WAIT_EVENTS:
		error_string = "WSA_MAXIMUM_WAIT_EVENTS";
		break;

	case (long)WSA_WAIT_FAILED:
		error_string = "WSA_WAIT_FAILED";
		break;

	case ERROR_INVALID_PARAMETER:
		error_string = "WSA_INVALID_PARAMETER";
		break;

	case WSA_WAIT_TIMEOUT:
		error_string = "WSA_WAIT_TIMEOUT";
		break;

	case WAIT_IO_COMPLETION:
		error_string = "WSA_WAIT_IO_COMPLETION";
		break;

	case ERROR_OPERATION_ABORTED:
		error_string = "WSA_OPERATION_ABORTED";
		break;

	case ERROR_IO_INCOMPLETE:
		error_string = "WSA_IO_INCOMPLETE";
		break;

	case ERROR_IO_PENDING:
		error_string = "WSA_IO_PENDING";
		break;

	case WSAEINTR:
		error_string = "WSAEINTR";
		break;

	case WSAEBADF:
		error_string = "WSAEBADF";
		break;

	case WSAEACCES:
		error_string = "WSAEACCES";
		break;

	case WSAEFAULT:
		error_string = "WSAEFAULT";
		break;

	case WSAEINVAL:
		error_string = "WSAEINVAL";
		break;

	case WSAEMFILE:
		error_string = "WSAEMFILE";
		break;

	case WSAEWOULDBLOCK:
		error_string = "WSAEWOULDBLOCK";
		break;

	case WSAEINPROGRESS:
		error_string = "WSAEINPROGRESS";
		break;

	case WSAEALREADY:
		error_string = "WSAEALREADY";
		break;

	case WSAENOTSOCK:
		error_string = "WSAENOTSOCK";
		break;

	case WSAEDESTADDRREQ:
		error_string = "WSAEDESTADDRREQ";
		break;

	case WSAEMSGSIZE:
		error_string = "WSAEMSGSIZE";
		break;

	case WSAEPROTOTYPE:
		error_string = "WSAEPROTOTYPE";
		break;

	case WSAENOPROTOOPT:
		error_string = "WSAENOPROTOOPT";
		break;

	case WSAEPROTONOSUPPORT:
		error_string = "WSAEPROTONOSUPPORT";
		break;

	case WSAESOCKTNOSUPPORT:
		error_string = "WSAESOCKTNOSUPPORT";
		break;

	case WSAEOPNOTSUPP:
		error_string = "WSAEOPNOTSUPP";
		break;

	case WSAEPFNOSUPPORT:
		error_string = "WSAEPFNOSUPPORT";
		break;

	case WSAEAFNOSUPPORT:
		error_string = "WSAEAFNOSUPPORT";
		break;

	case WSAEADDRINUSE:
		error_string = "WSAEADDRINUSE";
		break;

	case WSAEADDRNOTAVAIL:
		error_string = "WSAEADDRNOTAVAIL";
		break;

	case WSAENETDOWN:
		error_string = "WSAENETDOWN";
		break;

	case WSAENETUNREACH:
		error_string = "WSAENETUNREACH";
		break;

	case WSAENETRESET:
		error_string = "WSAENETRESET";
		break;

	case WSAECONNABORTED:
		error_string = "WSAECONNABORTED";
		break;

	case WSAECONNRESET:
		error_string = "WSAECONNRESET";
		break;

	case WSAENOBUFS:
		error_string = "WSAENOBUFS";
		break;

	case WSAEISCONN:
		error_string = "WSAEISCONN";
		break;

	case WSAENOTCONN:
		error_string = "WSAENOTCONN";
		break;

	case WSAESHUTDOWN:
		error_string = "WSAESHUTDOWN";
		break;

	case WSAETOOMANYREFS:
		error_string = "WSAETOOMANYREFS";
		break;

	case WSAETIMEDOUT:
		error_string = "WSAETIMEDOUT";
		break;

	case WSAECONNREFUSED:
		error_string = "WSAECONNREFUSED";
		break;

	case WSAELOOP:
		error_string = "WSAELOOP";
		break;

	case WSAENAMETOOLONG:
		error_string = "WSAENAMETOOLONG";
		break;

	case WSAEHOSTDOWN:
		error_string = "WSAEHOSTDOWN";
		break;

	case WSAEHOSTUNREACH:
		error_string = "WSAEHOSTUNREACH";
		break;

	case WSAENOTEMPTY:
		error_string = "WSAENOTEMPTY";
		break;

	case WSAEPROCLIM:
		error_string = "WSAEPROCLIM";
		break;

	case WSAEUSERS:
		error_string = "WSAEUSERS";
		break;

	case WSAEDQUOT:
		error_string = "WSAEDQUOT";
		break;

	case WSAESTALE:
		error_string = "WSAESTALE";
		break;

	case WSAEREMOTE:
		error_string = "WSAEREMOTE";
		break;

	case WSASYSNOTREADY:
		error_string = "WSASYSNOTREADY";
		break;

	case WSAVERNOTSUPPORTED:
		error_string = "WSAVERNOTSUPPORTED";
		break;

	case WSANOTINITIALISED:
		error_string = "WSANOTINITIALISED";
		break;

	case WSAEDISCON:
		error_string = "WSAEDISCON";
		break;

	case WSAENOMORE:
		error_string = "WSAENOMORE";
		break;

	case WSAECANCELLED:
		error_string = "WSAECANCELLED";
		break;

	case WSAEINVALIDPROCTABLE:
		error_string = "WSAEINVALIDPROCTABLE";
		break;

	case WSAEINVALIDPROVIDER:
		error_string = "WSAEINVALIDPROVIDER";
		break;

	case WSAEPROVIDERFAILEDINIT:
		error_string = "WSAEPROVIDERFAILEDINIT";
		break;

	case WSASYSCALLFAILURE:
		error_string = "WSASYSCALLFAILURE";
		break;

	case WSASERVICE_NOT_FOUND:
		error_string = "WSASERVICE_NOT_FOUND";
		break;

	case WSATYPE_NOT_FOUND:
		error_string = "WSATYPE_NOT_FOUND";
		break;

	case WSA_E_NO_MORE:
		error_string = "WSA_E_NO_MORE";
		break;

	case WSA_E_CANCELLED:
		error_string = "WSA_E_CANCELLED";
		break;

	case WSAEREFUSED:
		error_string = "WSAEREFUSED";
		break;

	case WSAHOST_NOT_FOUND:
		error_string = "WSAHOST_NOT_FOUND";
		break;

	case WSATRY_AGAIN:
		error_string = "WSATRY_AGAIN";
		break;

	case WSANO_RECOVERY:
		error_string = "WSANO_RECOVERY";
		break;

	case WSANO_DATA:
		error_string = "WSANO_DATA";
		break;

	case WSA_QOS_RECEIVERS:
		error_string = "WSA_QOS_RECEIVERS";
		break;

	case WSA_QOS_SENDERS:
		error_string = "WSA_QOS_SENDERS";
		break;

	case WSA_QOS_NO_SENDERS:
		error_string = "WSA_QOS_NO_SENDERS";
		break;

	case WSA_QOS_NO_RECEIVERS:
		error_string = "WSA_QOS_NO_RECEIVERS";
		break;

	case WSA_QOS_REQUEST_CONFIRMED:
		error_string = "WSA_QOS_REQUEST_CONFIRMED";
		break;

	case WSA_QOS_ADMISSION_FAILURE:
		error_string = "WSA_QOS_ADMISSION_FAILURE";
		break;

	case WSA_QOS_POLICY_FAILURE:
		error_string = "WSA_QOS_POLICY_FAILURE";
		break;

	case WSA_QOS_BAD_STYLE:
		error_string = "WSA_QOS_BAD_STYLE";
		break;

	case WSA_QOS_BAD_OBJECT:
		error_string = "WSA_QOS_BAD_OBJECT";
		break;

	case WSA_QOS_TRAFFIC_CTRL_ERROR:
		error_string = "WSA_QOS_TRAFFIC_CTRL_ERROR";
		break;

	case WSA_QOS_GENERIC_ERROR:
		error_string = "WSA_QOS_GENERIC_ERROR";
		break;

	default:
		error_string = "<unknown error>";
		break;
	}

	transport_endpoint_globals.error_string = error_string;
	if (error_code != transport_endpoint_globals.last_error)
	{
		error(_error_log, "winsock error #%d: %s", error_code, error_string);
		transport_endpoint_globals.last_error = error_code;
	}

	return transport_endpoint_globals.error_string;
}

long endpoint_equivalent(
	struct transport_endpoint const *a,
	struct transport_endpoint const *b)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x447, a);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x448, b);

	if (a->socket != -1 && a->socket == b->socket)
		return TRUE;

	return FALSE;
}

short get_endpoint_address(
	struct transport_endpoint *ep,
	struct transport_address *address)
{
	long error = _transport_error_none;
	int address_length = sizeof(struct sockaddr_in);
	struct sockaddr_in socket_address;
	unsigned long network_address;
	word network_port;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xF7, ep && address);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xF8, transport_initialized);

	if (ep->socket != INVALID_SOCKET)
	{
		if (getpeername(ep->socket, (struct sockaddr *)&socket_address, &address_length) == 0)
		{
			if (socket_address.sin_family == 2)
			{
				network_address = socket_address.sin_addr.s_addr;
				network_port = socket_address.sin_port;
				address->address.long_words[0] =
					(((network_address & 0xFF0000) | (network_address >> 16)) >> 8) |
					(((network_address & 0xFF00) | (network_address << 16)) << 8);
				address->address_length = IPV4_ADDRESS_LENGTH;
				address->port = (word)((network_port << 8) | (network_port >> 8));
			}
			else
			{
				winsock_error_to_string(WSAGetLastError());
				error = _transport_error_address_unknown;
			}
		}
		else if (getsockname(ep->socket, (struct sockaddr *)&socket_address, &address_length) == 0 &&
			socket_address.sin_family == 2)
		{
			network_address = socket_address.sin_addr.s_addr;
			network_port = socket_address.sin_port;
			address->address.long_words[0] =
				(((network_address & 0xFF0000) | (network_address >> 16)) >> 8) |
				(((network_address & 0xFF00) | (network_address << 16)) << 8);
			address->address_length = IPV4_ADDRESS_LENGTH;
			address->port = (word)((network_port << 8) | (network_port >> 8));
		}
		else
		{
			winsock_error_to_string(WSAGetLastError());
			error = _transport_error_address_unknown;
		}
	}
	else
		error = _transport_error_address_unknown;

	ep->error = (short)error;
	return (short)error;
}

short set_endpoint_blocking(
	struct transport_endpoint *ep,
	long blocking)
{
	short error = _transport_error_none;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x139, ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x13A, transport_initialized);

	if (!endpoint_blocking(ep))
	{
		if (blocking)
		{
			u_long nonblocking = FALSE;

			error = ioctlsocket(ep->socket, FIONBIO, &nonblocking);
			if (error == 0)
			{
				SET_FLAG(ep->flags, _transport_endpoint_nonblocking_bit, FALSE);
			}
			else
			{
				winsock_error_to_string(WSAGetLastError());
				error = _transport_error_options_failed;
			}
		}
	}
	else if (!blocking)
	{
		u_long nonblocking = TRUE;

		error = ioctlsocket(ep->socket, FIONBIO, &nonblocking);
		if (error == 0)
		{
			SET_FLAG(ep->flags, _transport_endpoint_nonblocking_bit, TRUE);
		}
		else
		{
			winsock_error_to_string(WSAGetLastError());
			error = _transport_error_options_failed;
		}
	}

	ep->error = error;
	return error;
}

short bind_endpoint(
	struct transport_endpoint *ep,
	struct transport_address *address)
{
	short error = _transport_error_none;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x16C, ep && address);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x16D, transport_initialized);

	if (ep->socket == INVALID_SOCKET)
	{
		int socket_type;

		if (ep->type == _transport_type_tcp)
		{
			socket_type = SOCK_STREAM;
		}
		else if (ep->type == _transport_type_udp)
		{
			socket_type = SOCK_DGRAM;
		}
		else
		{
			error = _transport_error_bad_endpoint;
		}

		if (error == _transport_error_none)
		{
			ep->socket = create_socket(AF_INET, socket_type, IPPROTO_IP);
			if (ep->socket == INVALID_SOCKET)
			{
				error = _transport_error_unknown;
			}
		}
	}

	if (ep->socket != INVALID_SOCKET && error == _transport_error_none)
	{
		struct sockaddr_in socket_address;

		socket_address.sin_addr.s_addr = SWAP4(address->address.long_words[0]);
		socket_address.sin_port = (word)SWAP2(address->port);
		socket_address.sin_family = AF_INET;

		if (bind(ep->socket, (struct sockaddr *)&socket_address, sizeof(socket_address)) != 0)
		{
			winsock_error_to_string(WSAGetLastError());
			error = _transport_error_bind_endpoint;
		}
	}
	else
	{
		error = _transport_error_unknown;
	}

	ep->error = error;
	return error;
}

short connect_endpoint(
	struct transport_endpoint *ep,
	struct transport_address const *address)
{
	short result = _transport_error_none;
	int socket_type;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x1B5, ep && address);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x1B6, transport_initialized);

	if (ep->type == _transport_type_udp)
	{
		socket_type = SOCK_DGRAM;
	}
	else if (ep->type == _transport_type_tcp)
	{
		socket_type = SOCK_STREAM;
	}
	else
	{
		result = _transport_error_bad_endpoint;
	}

	if (result == _transport_error_none)
	{
		struct sockaddr_in socket_address;
		boolean blocking;
		long error;

		if (ep->socket == INVALID_SOCKET)
		{
			ep->socket = create_socket(AF_INET, socket_type, IPPROTO_IP);
		}

		socket_address.sin_addr.s_addr = SWAP4(address->address.long_words[0]);
		socket_address.sin_port = (word)SWAP2(address->port);
		socket_address.sin_family = AF_INET;
		blocking = endpoint_blocking(ep);
		set_endpoint_blocking(ep, FALSE);
		error = connect(ep->socket, (struct sockaddr *)&socket_address, sizeof(socket_address));

		if (error != 0)
		{
			error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				unsigned long timeout = system_milliseconds() + 10 * MILLISECONDS_PER_SECOND;
				struct timeval timeval;

				timeval.tv_sec = 1;
				timeval.tv_usec = 0;

				do
				{
					fd_set writeable;

					writeable.fd_array[0] = ep->socket;
					writeable.fd_count = 1;

					if (select(1, NULL, &writeable, NULL, &timeval) == 1)
					{
						error = 0;
					}
					else
					{
						error = WSAGetLastError();
					}

					if (system_milliseconds() > timeout)
					{
						error = WSAEINPROGRESS;
						closesocket(ep->socket);
						break;
					}
				}
				while (error == WSAEINPROGRESS);
			}
		}

		if (error != 0)
		{
			winsock_error_to_string(error);
			result = _transport_error_connect_failed;
		}
		else
		{
			set_endpoint_blocking(ep, blocking);
			SET_FLAG(ep->flags, _transport_endpoint_nonblocking_bit, FALSE);
			SET_FLAG(ep->flags, _transport_endpoint_connected_bit, TRUE);
			SET_FLAG(ep->flags, _transport_endpoint_client_bit, TRUE);
		}
	}

	ep->error = result;
	return result;
}

static unsigned long __stdcall connect_async_thread_proc(
	void *input_pointer)
{
	struct connect_process_input *input = input_pointer;
	struct thread_reference *thread = NULL;
	struct mutex_reference *mutex = NULL;
	short error;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x239, input);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x23A, input->ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x23B, input->thread);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x23C, transport_initialized);

	error = connect_endpoint(input->ep, &input->address);
	if (take_mutex(input->mutex, 1000))
	{
		if (input->cancelled)
		{
			disconnect_endpoint(input->ep);
		}

		thread = input->thread;
		mutex = input->mutex;
	}
	else
	{
		error = _transport_error_unknown;
	}

	input->ep->error = error;
	if (mutex)
	{
		match_free(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x252, input);
		release_mutex(mutex);
		dispose_mutex(mutex);
	}

	if (thread)
	{
		mark_connection_thread_as_terminated(thread);
	}

	return error;
}

short connect_endpoint_async(
	struct transport_endpoint *ep,
	struct transport_address const *address,
	transport_connect_process_ref *process_ref_ptr)
{
	struct connect_process_input *input;
	short result;

	connection_thread_list_maintenance();
	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x268,
		ep && address && process_ref_ptr);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x269, transport_initialized);

	input = debug_malloc(
		sizeof(*input),
		TRUE,
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x26B);
	if (input)
	{
		input->address = *address;
		input->ep = ep;
		input->cancelled = FALSE;

		if (create_mutex(&input->mutex) &&
			create_thread(2, connect_async_thread_proc, input, &input->thread))
		{
			if (add_connect_thread(input->thread))
			{
				result = _transport_result_connect_in_progress;
				*process_ref_ptr = input;
			}
			else
			{
				dispose_thread(input->thread);
				dispose_mutex(input->mutex);
				input->thread = NULL;
				result = _transport_error_unknown;
			}
		}
		else
		{
			match_free(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x282, input);
			result = _transport_error_connect_failed;
		}
	}
	else
	{
		result = _transport_error_out_of_memory;
	}

	ep->error = result;
	return result;
}

void disconnect_endpoint(
	struct transport_endpoint *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x221, ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x222, transport_initialized);

	if (ep->socket != INVALID_SOCKET)
	{
		if (closesocket(ep->socket) != 0)
			winsock_error_to_string(WSAGetLastError());

		ep->socket = INVALID_SOCKET;
	}

	ep->flags &= ~1;
	return;
}

void cancel_connect_process(
	transport_connect_process_ref input)
{
	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x298,
		input && input->ep && input->thread);

	connection_thread_list_maintenance();
	if (take_mutex(input->mutex, 1000))
	{
		disconnect_endpoint(input->ep);
		input->ep->error = _transport_error_none;
		input->cancelled = TRUE;
		release_mutex(input->mutex);
	}
	else
	{
		match_vassert(
			TRANSPORT_ENDPOINT_WINSOCK_FILE,
			0x2A5,
			FALSE,
			"!\"unable to get mutex in cancel_connect_process()!\"");
	}

	return;
}

short listen_endpoint(
	struct transport_endpoint *ep)
{
	long error = _transport_error_none;

	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x2B0, ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x2B1, transport_initialized);

	if (ep->socket != INVALID_SOCKET)
	{
		if (listen(ep->socket, MAXIMUM_PENDING_CONNECTIONS) == 0)
		{
			ep->flags |= 2;
		}
		else
		{
			winsock_error_to_string(WSAGetLastError());
			error = _transport_error_listen_failed;
		}
	}
	else
		error = _transport_error_bad_endpoint;

	ep->error = (short)error;
	return (short)error;
}

struct transport_endpoint *accept_endpoint(
	struct transport_endpoint *listening_endpoint)
{
	struct transport_endpoint *ep = NULL;
	int address_length = sizeof(struct sockaddr);
	struct sockaddr address;
	SOCKET socket;

	match_vassert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x2D1,
		listening_endpoint,
		"listening_endpoint && (listening_endpoint->socket >= 0)");
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x2D2, transport_initialized);

	socket = accept(
		listening_endpoint->socket,
		&address,
		&address_length);
	if (socket != INVALID_SOCKET)
	{
		ep = create_transport_endpoint(listening_endpoint->type);
		if (ep)
		{
			ep->socket = socket;
			ep->flags |= 1;
		}
		else
			listening_endpoint->error = _transport_error_out_of_memory;
	}
	else
	{
		winsock_error_to_string(WSAGetLastError());
		listening_endpoint->error = _transport_error_unknown;
	}

	return ep;
}

long read_from_endpoint(
	struct transport_endpoint *ep,
	void *buffer,
	long length,
	struct transport_address *src_addr)
{
	struct sockaddr_in socket_address;
	int address_length = sizeof(socket_address);
	long result;

	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x377,
		ep && buffer && src_addr && (length > 0));
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x378, transport_initialized);

	if (ep->socket == INVALID_SOCKET)
	{
		match_assert(
			TRANSPORT_ENDPOINT_WINSOCK_FILE,
			0x37C,
			ep->type == _transport_type_udp);

		ep->socket = create_socket(AF_INET, SOCK_DGRAM, 0);
		if (ep->socket != INVALID_SOCKET)
		{
			struct transport_address bind_address = {0};
			short err;

			bind_address.address_length = IPV4_ADDRESS_LENGTH;
			err = bind_endpoint(ep, &bind_address);
			match_assert(
				TRANSPORT_ENDPOINT_WINSOCK_FILE,
				0x384,
				err == _transport_error_none);
		}
	}

	if (ep->socket != INVALID_SOCKET)
	{
		match_assert(
			TRANSPORT_ENDPOINT_WINSOCK_FILE,
			0x38B,
			!endpoint_connected(ep));

		result = recvfrom(
			ep->socket,
			buffer,
			length,
			0,
			(struct sockaddr *)&socket_address,
			&address_length);
	}
	else
	{
		ep->error = _transport_error_unknown;
		result = SOCKET_ERROR;
	}

	if (result == SOCKET_ERROR)
	{
		switch (WSAGetLastError())
		{
		case WSAEWOULDBLOCK:
			result = _transport_result_operation_would_block;
			break;

		case WSAENETRESET:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAENOTCONN:
		case WSAESHUTDOWN:
		case WSAETIMEDOUT:
			SET_FLAG(ep->flags, _transport_endpoint_connected_bit, FALSE);
			SET_FLAG(ep->flags, _transport_endpoint_readable_bit, FALSE);
			result = _transport_error_connection_lost;
			break;

		default:
			result = _transport_error_endpoint_io;
			SET_FLAG(ep->flags, _transport_endpoint_readable_bit, FALSE);
			break;
		}
	}
	else if (result >= 0)
	{
		src_addr->address.long_words[0] = SWAP4(socket_address.sin_addr.s_addr);
		src_addr->address_length = IPV4_ADDRESS_LENGTH;
		src_addr->port = (word)SWAP2(socket_address.sin_port);
	}

	return result;
}

long write_to_endpoint(
	struct transport_endpoint *ep,
	void const *buffer,
	long length,
	struct transport_address const *dest_addr)
{
	struct sockaddr_in socket_address;
	long result;

	match_assert(
		TRANSPORT_ENDPOINT_WINSOCK_FILE,
		0x3BD,
		ep && buffer && (length > 0) && dest_addr);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0x3BE, transport_initialized);

	socket_address.sin_addr.s_addr = SWAP4(dest_addr->address.long_words[0]);
	socket_address.sin_port = (word)SWAP2(dest_addr->port);
	socket_address.sin_family = AF_INET;

	if (ep->socket == INVALID_SOCKET)
	{
		match_assert(
			TRANSPORT_ENDPOINT_WINSOCK_FILE,
			0x3C6,
			ep->type == _transport_type_udp);

		ep->socket = create_socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	}

	if (ep->socket != INVALID_SOCKET)
	{
		result = sendto(
			ep->socket,
			buffer,
			length,
			0,
			(struct sockaddr const *)&socket_address,
			sizeof(socket_address));
	}
	else
	{
		ep->error = _transport_error_unknown;
		result = SOCKET_ERROR;
	}

	if (result == SOCKET_ERROR)
	{
		switch (WSAGetLastError())
		{
		case WSAEWOULDBLOCK:
			result = _transport_result_operation_would_block;
			break;

		case WSAENETRESET:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAENOTCONN:
		case WSAESHUTDOWN:
		case WSAETIMEDOUT:
			SET_FLAG(ep->flags, _transport_endpoint_connected_bit, FALSE);
			result = _transport_error_connection_lost;
			break;

		default:
			result = _transport_error_endpoint_io;
			break;
		}
	}

	return result;
}

void delete_transport_endpoint(
	struct transport_endpoint *ep)
{
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xE4, ep);
	match_assert(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xE5, transport_initialized);

	disconnect_endpoint(ep);
	match_free(TRANSPORT_ENDPOINT_WINSOCK_FILE, 0xE8, ep);
	connection_thread_list_maintenance();
	return;
}

short reject_endpoint(
	struct transport_endpoint *listening_endpoint)
{
	struct transport_endpoint *endpoint = accept_endpoint(listening_endpoint);

	if (endpoint)
		delete_transport_endpoint(endpoint);

	return 0;
}

/* ---------- private code */

static SOCKET create_socket(
	int address_family,
	int socket_type,
	int protocol)
{
	SOCKET endpoint_socket = socket(address_family, socket_type, protocol);

	if (endpoint_socket != INVALID_SOCKET)
	{
		int option;
		int option_length;

		if (socket_type == SOCK_DGRAM)
		{
			option = -1;
			if (setsockopt(endpoint_socket, SOL_SOCKET, SO_BROADCAST,
				(char const *)&option, sizeof(option)) != 0)
			{
				winsock_error_to_string(WSAGetLastError());
			}
		}

		option = TRUE;
		if (setsockopt(endpoint_socket, SOL_SOCKET, SO_REUSEADDR,
			(char const *)&option, sizeof(option)) != 0)
		{
			winsock_error_to_string(WSAGetLastError());
		}

#if defined(HALO_LINUX) && !defined(HALO_WINDOWS)
		/* Linux (and Android) grow a stream socket's buffers as far as the
		connection needs, to several megabytes; setting a size would fix them,
		at no more than the system's limit (about 416 KB by default) */
		if (socket_type != SOCK_STREAM)
		{
#endif
		option_length = sizeof(option);
		if (getsockopt(endpoint_socket, SOL_SOCKET, SO_SNDBUF,
			(char *)&option, &option_length) == 0)
		{
			if (option < MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE)
			{
				option = MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE;
				if (setsockopt(endpoint_socket, SOL_SOCKET, SO_SNDBUF,
					(char const *)&option, sizeof(option)) != 0)
				{
					winsock_error_to_string(WSAGetLastError());
				}
			}
		}
		else
		{
			winsock_error_to_string(WSAGetLastError());
		}

		option_length = sizeof(option);
		if (getsockopt(endpoint_socket, SOL_SOCKET, SO_RCVBUF,
			(char *)&option, &option_length) == 0)
		{
			if (option < MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE)
			{
				option = MINIMUM_ENDPOINT_SOCKET_BUFFER_SIZE;
				if (setsockopt(endpoint_socket, SOL_SOCKET, SO_RCVBUF,
					(char const *)&option, sizeof(option)) != 0)
				{
					winsock_error_to_string(WSAGetLastError());
				}
			}
		}
		else
		{
			winsock_error_to_string(WSAGetLastError());
		}
#if defined(HALO_LINUX) && !defined(HALO_WINDOWS)
		}
#endif
	}
	else
	{
		winsock_error_to_string(WSAGetLastError());
	}

	return endpoint_socket;
}
