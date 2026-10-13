/*
P2P.H

Internet play (p2p.c): machines reach each other's system link games over
the internet as if they were on one LAN, through the master server that
lists them (p2p_master.c) or an invite. xnet.c routes the game's traffic
for them through here; sdl_platform.c passes invite links in and out
through the clipboard.

Addresses and ports are in network byte order.
*/

#ifndef __HALO_LINUX_P2P_H
#define __HALO_LINUX_P2P_H

/* starts internet play, if network.online is set, when the game starts
its networking; local_address is the address the game's sockets are
reached at (network.address, else 127.0.0.1) */
void p2p_initialize(unsigned long local_address);

/* on the desktop, before anything else: if this process was started with
an invite link (halo://join/...) and another copy of the game is running,
passes the link to it and returns nonzero (this one should quit) */
int p2p_hand_off_invite(void);

/* joins the game an invite link or code leads to; text may hold other
words around it. Returns nonzero if it held an invite */
int p2p_join_invite(const char *text);

/* this machine's identifier, which its XNADDR carries (6 bytes) */
const unsigned char *p2p_identifier(void);
/* the address the game reaches the machine with this identifier at, if it
is an internet play peer */
int p2p_peer_address(const unsigned char *identifier, unsigned long *address);

/* a destination the game sends to or connects to (stream: a TCP socket):
if it is a peer's address, the local address standing in for it */
int p2p_outgoing(int stream, unsigned long *address, unsigned short *port);
/* a source the game received from, accepted from or is connected to: if
it is one standing in for a peer, the peer's address */
int p2p_incoming(int stream, unsigned long *address, unsigned short *port);
/* the local addresses standing in for this port (a broadcast's) on every
peer; returns their count */
int p2p_broadcast_targets(unsigned short port, unsigned long *addresses, unsigned short *ports, int maximum_count);

/* the game listens (it is hosting) on socket, or closes a socket */
void p2p_socket_listening(int socket);
void p2p_socket_closed(int socket);
/* the game broadcast a datagram from socket to port (after it was sent on
the LAN): a search for games, or a hosting game's answer to one, which the
master server passes on (p2p_master.c) */
void p2p_broadcast_sent(int socket, unsigned short port, const void *data, int size);

/* text for the clipboard (a new invite link), once; NULL if none. Called
from the main thread */
const char *p2p_take_clipboard_text(void);

#endif
