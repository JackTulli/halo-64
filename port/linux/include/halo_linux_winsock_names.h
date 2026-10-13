/*
HALO_LINUX_WINSOCK_NAMES.H

The XDK declares the BSD socket calls as __stdcall Winsock functions with
Winsock structures. glibc exports functions of the same names with the cdecl
convention and different structures, so a game call to socket() would link
against glibc and corrupt the stack. These macros move every Xbox Winsock
name into the halo_ws_ namespace, which port/linux/src/xnet.c implements.

Included with HALO_LINUX_WINSOCK_NAMES_UNDEFINE defined, it removes them
again (the platform layer does that after including the XDK headers).
*/

#ifndef HALO_LINUX_WINSOCK_NAMES_UNDEFINE

#define accept halo_ws_accept
#define bind halo_ws_bind
#define closesocket halo_ws_closesocket
#define connect halo_ws_connect
#define getpeername halo_ws_getpeername
#define getsockname halo_ws_getsockname
#define getsockopt halo_ws_getsockopt
#define htonl halo_ws_htonl
#define htons halo_ws_htons
#define inet_addr halo_ws_inet_addr
#define ioctlsocket halo_ws_ioctlsocket
#define listen halo_ws_listen
#define ntohl halo_ws_ntohl
#define ntohs halo_ws_ntohs
#define recv halo_ws_recv
#define recvfrom halo_ws_recvfrom
#define select halo_ws_select
#define send halo_ws_send
#define sendto halo_ws_sendto
#define setsockopt halo_ws_setsockopt
#define shutdown halo_ws_shutdown
#define socket halo_ws_socket
#define fd_set halo_ws_fd_set
#define timeval halo_ws_timeval

#else

#undef accept
#undef bind
#undef closesocket
#undef connect
#undef getpeername
#undef getsockname
#undef getsockopt
#undef htonl
#undef htons
#undef inet_addr
#undef ioctlsocket
#undef listen
#undef ntohl
#undef ntohs
#undef recv
#undef recvfrom
#undef select
#undef send
#undef sendto
#undef setsockopt
#undef shutdown
#undef socket
#undef fd_set
#undef timeval

#endif
