/*
XDK_WINSOCK.H

Winsock and Xbox network declarations the game and the platform layer
use.
(README.md)
*/

#ifndef HALO_XDK_WINSOCK_H
#define HALO_XDK_WINSOCK_H

#include "xdk_xbox.h"

/* ---------- macros */

/* Winsock's calling convention, as every Winsock function's
(learn.microsoft.com, Winsock reference; mingw-w64 winsock.h, public
domain, spells it WINAPI) */
#define WSAAPI __stdcall

/* Addresses, sockets and options (learn.microsoft.com, socket,
setsockopt, "SOL_SOCKET Socket Options" and in_addr; the same values as
mingw-w64 winsock.h and psdk_inc/_socket_types.h, public domain).
cachebeta.exe's socket set-up passes SOL_SOCKET 0xFFFF with SO_BROADCAST
0x20, SO_REUSEADDR 4, SO_SNDBUF 0x1001 and SO_RCVBUF 0x1002, and
set_endpoint_blocking passes FIONBIO 0x8004667E. */
#define AF_INET 2

#define SOCK_STREAM 1
#define SOCK_DGRAM 2

#define IPPROTO_IP 0

#define SOL_SOCKET 0xFFFF
#define SO_REUSEADDR 0x0004
#define SO_BROADCAST 0x0020
#define SO_SNDBUF 0x1001
#define SO_RCVBUF 0x1002

#define INADDR_ANY ((u_long)0x00000000)
#define INADDR_BROADCAST ((u_long)0xFFFFFFFF)
#define INADDR_NONE 0xFFFFFFFF

#define INVALID_SOCKET ((SOCKET)(~0))
#define SOCKET_ERROR (-1)

/* ioctlsocket commands: an input or output flag, the argument's size
(a u_long) and the group 'f' with the command number (learn.microsoft.com,
ioctlsocket and "Winsock IOCTLs") */
#define FIONREAD 0x4004667FUL /* output, 4 bytes, 'f', 127 */
#define FIONBIO 0x8004667EUL /* input, 4 bytes, 'f', 126 */

/* in_addr's address as one u_long (learn.microsoft.com, in_addr) */
#define s_addr S_un.S_addr

/* fd_set helpers (learn.microsoft.com, fd_set and select). A set is a
count and an array of sockets. They are written here for the ports: FD_SET
stops at the array's end, whatever FD_SETSIZE a unit sets. The platform
layer reads glibc's <sys/select.h> before these headers, so its versions
of the names give way. (FD_SETSIZE's default is in xdk_win32.h.) */

#undef FD_ZERO
#undef FD_SET
#undef FD_CLR
#undef FD_ISSET

#define FD_ZERO(set) ((set)->fd_count = 0)

#define FD_SET(fd, set) do { \
	u_int fd_set_index_; \
	for (fd_set_index_ = 0; fd_set_index_ < (set)->fd_count; fd_set_index_++) \
	{ \
		if ((set)->fd_array[fd_set_index_] == (SOCKET)(fd)) \
			break; \
	} \
	if (fd_set_index_ == (set)->fd_count && \
		(set)->fd_count < sizeof((set)->fd_array) / sizeof((set)->fd_array[0])) \
	{ \
		(set)->fd_array[(set)->fd_count++] = (SOCKET)(fd); \
	} \
} while (0)

#define FD_CLR(fd, set) do { \
	u_int fd_set_index_; \
	for (fd_set_index_ = 0; fd_set_index_ < (set)->fd_count; fd_set_index_++) \
	{ \
		if ((set)->fd_array[fd_set_index_] == (SOCKET)(fd)) \
		{ \
			(set)->fd_count--; \
			for (; fd_set_index_ < (set)->fd_count; fd_set_index_++) \
				(set)->fd_array[fd_set_index_] = (set)->fd_array[fd_set_index_ + 1]; \
			break; \
		} \
	} \
} while (0)

#define FD_ISSET(fd, set) __WSAFDIsSet((SOCKET)(fd), (set))

/* Winsock error codes (learn.microsoft.com, "Windows Sockets Error
Codes"; the same values as mingw-w64 psdk_inc/_wsa_errnos.h, public
domain). cachebeta.exe's winsock_error_to_string switch covers the same
ranges (10004-10071, 10091-10112, 11001-11015). */
#define WSABASEERR 10000
#define WSAEINTR (WSABASEERR + 4)
#define WSAEBADF (WSABASEERR + 9)
#define WSAEACCES (WSABASEERR + 13)
#define WSAEFAULT (WSABASEERR + 14)
#define WSAEINVAL (WSABASEERR + 22)
#define WSAEMFILE (WSABASEERR + 24)
#define WSAEWOULDBLOCK (WSABASEERR + 35)
#define WSAEINPROGRESS (WSABASEERR + 36)
#define WSAEALREADY (WSABASEERR + 37)
#define WSAENOTSOCK (WSABASEERR + 38)
#define WSAEDESTADDRREQ (WSABASEERR + 39)
#define WSAEMSGSIZE (WSABASEERR + 40)
#define WSAEPROTOTYPE (WSABASEERR + 41)
#define WSAENOPROTOOPT (WSABASEERR + 42)
#define WSAEPROTONOSUPPORT (WSABASEERR + 43)
#define WSAESOCKTNOSUPPORT (WSABASEERR + 44)
#define WSAEOPNOTSUPP (WSABASEERR + 45)
#define WSAEPFNOSUPPORT (WSABASEERR + 46)
#define WSAEAFNOSUPPORT (WSABASEERR + 47)
#define WSAEADDRINUSE (WSABASEERR + 48)
#define WSAEADDRNOTAVAIL (WSABASEERR + 49)
#define WSAENETDOWN (WSABASEERR + 50)
#define WSAENETUNREACH (WSABASEERR + 51)
#define WSAENETRESET (WSABASEERR + 52)
#define WSAECONNABORTED (WSABASEERR + 53)
#define WSAECONNRESET (WSABASEERR + 54)
#define WSAENOBUFS (WSABASEERR + 55)
#define WSAEISCONN (WSABASEERR + 56)
#define WSAENOTCONN (WSABASEERR + 57)
#define WSAESHUTDOWN (WSABASEERR + 58)
#define WSAETOOMANYREFS (WSABASEERR + 59)
#define WSAETIMEDOUT (WSABASEERR + 60)
#define WSAECONNREFUSED (WSABASEERR + 61)
#define WSAELOOP (WSABASEERR + 62)
#define WSAENAMETOOLONG (WSABASEERR + 63)
#define WSAEHOSTDOWN (WSABASEERR + 64)
#define WSAEHOSTUNREACH (WSABASEERR + 65)
#define WSAENOTEMPTY (WSABASEERR + 66)
#define WSAEPROCLIM (WSABASEERR + 67)
#define WSAEUSERS (WSABASEERR + 68)
#define WSAEDQUOT (WSABASEERR + 69)
#define WSAESTALE (WSABASEERR + 70)
#define WSAEREMOTE (WSABASEERR + 71)
#define WSASYSNOTREADY (WSABASEERR + 91)
#define WSAVERNOTSUPPORTED (WSABASEERR + 92)
#define WSANOTINITIALISED (WSABASEERR + 93)
#define WSAEDISCON (WSABASEERR + 101)
#define WSAENOMORE (WSABASEERR + 102)
#define WSAECANCELLED (WSABASEERR + 103)
#define WSAEINVALIDPROCTABLE (WSABASEERR + 104)
#define WSAEINVALIDPROVIDER (WSABASEERR + 105)
#define WSAEPROVIDERFAILEDINIT (WSABASEERR + 106)
#define WSASYSCALLFAILURE (WSABASEERR + 107)
#define WSASERVICE_NOT_FOUND (WSABASEERR + 108)
#define WSATYPE_NOT_FOUND (WSABASEERR + 109)
#define WSA_E_NO_MORE (WSABASEERR + 110)
#define WSA_E_CANCELLED (WSABASEERR + 111)
#define WSAEREFUSED (WSABASEERR + 112)
#define WSAHOST_NOT_FOUND (WSABASEERR + 1001)
#define WSATRY_AGAIN (WSABASEERR + 1002)
#define WSANO_RECOVERY (WSABASEERR + 1003)
#define WSANO_DATA (WSABASEERR + 1004)
#define WSA_QOS_RECEIVERS (WSABASEERR + 1005)
#define WSA_QOS_SENDERS (WSABASEERR + 1006)
#define WSA_QOS_NO_SENDERS (WSABASEERR + 1007)
#define WSA_QOS_NO_RECEIVERS (WSABASEERR + 1008)
#define WSA_QOS_REQUEST_CONFIRMED (WSABASEERR + 1009)
#define WSA_QOS_ADMISSION_FAILURE (WSABASEERR + 1010)
#define WSA_QOS_POLICY_FAILURE (WSABASEERR + 1011)
#define WSA_QOS_BAD_STYLE (WSABASEERR + 1012)
#define WSA_QOS_BAD_OBJECT (WSABASEERR + 1013)
#define WSA_QOS_TRAFFIC_CTRL_ERROR (WSABASEERR + 1014)
#define WSA_QOS_GENERIC_ERROR (WSABASEERR + 1015)

/* Winsock events, which are Win32 events and waits (learn.microsoft.com,
WSACreateEvent and WSAWaitForMultipleEvents). The game's
winsock_error_to_string tests 0, 64, 0x102 and -1 for them. */
typedef HANDLE WSAEVENT;
#define WSA_INVALID_EVENT ((WSAEVENT)0)
#define WSA_MAXIMUM_WAIT_EVENTS 64
#define WSA_WAIT_FAILED WAIT_FAILED
#define WSA_WAIT_TIMEOUT WAIT_TIMEOUT

/* XNetStartupParams.cfgFlags: cachebeta.exe's transport_initialize sets
bit 0 when d:\bypass_security.txt exists. */
#define XNET_STARTUP_BYPASS_SECURITY 0x01

/* XNetGetTitleXnAddr's result. cachebeta.exe's transport_initialize keeps
polling while it is 0 and gives up on 1, and the XNet library in the same
binary returns 1 before start-up, otherwise 0 or one of the bits 0x02,
0x04, 0x08, 0x10 for how the address was obtained (plus 0x20). Which of
those bits means Ethernet and which DHCP is the platform layer's own choice
(port/linux/src/xnet.c is the only code that reports them, and the game
tests neither); any two distinct bits of that set work. */
#define XNET_GET_XNADDR_PENDING 0x00
#define XNET_GET_XNADDR_NONE 0x01
#define XNET_GET_XNADDR_ETHERNET 0x02
#define XNET_GET_XNADDR_DHCP 0x08

/* XNetGetEthernetLinkStatus's result: cachebeta.exe's transport_initialize
reports bit 0 as connected, 1 as 100 Mbps, 2 as 10 Mbps, 3 as full duplex
and 4 as half duplex, and transport_network_available tests bit 0. */
#define XNET_ETHERNET_LINK_ACTIVE 0x01
#define XNET_ETHERNET_LINK_100MBPS 0x02
#define XNET_ETHERNET_LINK_10MBPS 0x04
#define XNET_ETHERNET_LINK_FULL_DUPLEX 0x08
#define XNET_ETHERNET_LINK_HALF_DUPLEX 0x10

/* ---------- functions */

#endif
