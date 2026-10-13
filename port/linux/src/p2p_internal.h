/*
P2P_INTERNAL.H

Shared internals of internet play (p2p.c, p2p_master.c, p2p_crypto.c,
p2p_discord.c; see p2p.c for the design).
*/

#ifndef __HALO_LINUX_P2P_INTERNAL_H
#define __HALO_LINUX_P2P_INTERNAL_H

#include "p2p.h"

enum
{
	/* a machine's identifier: random, and also its XNADDR's abEnet */
	P2P_IDENTIFIER_SIZE = 6,
	/* a game's secret: whoever holds it can join */
	P2P_TOKEN_SIZE = 16,
	/* the addresses a machine offers to be reached at */
	P2P_MAXIMUM_CANDIDATES = 4,
	/* an invite link's text: "halo://join/", the host's identifier and the
	token in hexadecimal, and a terminator */
	P2P_LINK_SIZE = 64,
	/* the most machines one tunnels to: a host and the rest of a system
	link game's 128 machines (include/halo_port_limits.h) */
	P2P_MAXIMUM_PEERS = 127,
};

struct p2p_candidate
{
	/* network byte order */
	unsigned long address;
	unsigned short port;
};

/* ---------- p2p.c: what the master server side calls back (all under
p2p_lock, on the p2p thread, unless said otherwise) */

/* the milliseconds of a monotonic clock */
unsigned long p2p_now(void);
/* looks up a host name (posix_resolve_ipv4), letting go of the p2p lock
while it waits; the p2p thread's */
unsigned long p2p_resolve(const char *host);
/* registers this executable for links of scheme (posix_register_url_scheme),
unless it is an automated run (debug.exit_after, a hidden window, no
renderer), which must not take the links over */
void p2p_register_url_scheme(const char *scheme, const char *description);
/* formats bytes as lower-case hexadecimal (text holds 2 * size + 1) */
void p2p_hex(const unsigned char *bytes, int size, char *text);
/* the addresses this machine can be reached at; returns their count */
int p2p_local_candidates(struct p2p_candidate *candidates, int maximum_count);
/* a joiner (on the host) or the host (on a joiner) offered its addresses
through signalling, and the key their tunnel traffic is sealed with
(P2P_SHA256_SIZE bytes); the tunnel starts reaching it */
void p2p_peer_offered(const unsigned char *identifier, const unsigned char *key,
	const struct p2p_candidate *candidates, int count, int is_host);
/* the master server listed a host: its identifier, the token that joins
it and where it can be reached. The game can see it from now on, and its
connecting to it starts the join. Returns 0 if there is no room */
int p2p_peer_listed(const unsigned char *identifier, const unsigned char *token,
	const struct p2p_candidate *candidates, int count);
/* gives the game, at its socket on to_port, a datagram as if the host
(listed or connected) had sent it from its from_port */
void p2p_deliver_datagram(const unsigned char *host_identifier, unsigned short from_port,
	unsigned short to_port, const void *data, int size);
/* the address the game's sockets are reached at (p2p_initialize's) */
unsigned long p2p_local_address(void);
/* the game's ports (network_game_protocol.h), in network byte order: the
host's server (5150: its searches' destination, its connections' and its
advertisements' source) and a client's (5151). A peer and the master may
name no other port on this machine: anything else is some other local
service (the telnet console on port 23, which evaluates scripts) */
#define P2P_GAME_SERVER_PORT 5150
#define P2P_GAME_CLIENT_PORT 5151
int p2p_game_port(unsigned short port, unsigned short wanted);
/* a UDP socket bound to that address, not blocking, and its port; -1 on
failure */
int p2p_open_local_udp_socket(unsigned short *port);
/* sends a packet from the tunnel's socket */
void p2p_tunnel_send(const void *packet, int size, unsigned long address, unsigned short port);
/* the players (not hosts) the tunnel carries */
int p2p_connected_player_count(void);
/* p2p_lock, for the game's threads */
void p2p_lock_acquire(void);
void p2p_lock_release(void);
/* an invite that arrived on the p2p thread (from Discord, or another copy
of the game) */
void p2p_invite_received(const char *text);

/* ---------- p2p_master.c: the master server, which lists games and passes
signalling between machines */

/* reads the server's address from the settings, once; looked up later on
the p2p thread */
void p2p_master_start(void);
/* adds its sockets to the p2p thread's select lists */
void p2p_master_select_sets(int *read, int *read_count, int *write, int *write_count, int maximum_count);
/* services the sockets and timers; called from the p2p thread each pass */
void p2p_master_update(const int *read, int read_count, const int *write, int write_count);
/* a packet on the tunnel socket, from address:port, that may be the
server's; returns nonzero if it was one of its messages */
int p2p_master_received(const unsigned char *packet, int size, unsigned long address, unsigned short port);
/* the game broadcast a datagram from socket to port: a search, or a
hosting game's advertisement; the game's thread */
void p2p_master_broadcast_sent(int socket, unsigned short port, const void *data, int size);
/* the hosted lobby's report for the server (p2p.c holds the lock) */
void p2p_master_lobby(const void *data, int size);
/* this machine's address as the server sees it, once it has answered */
int p2p_master_mapped_address(struct p2p_candidate *candidate);
/* whether the server answered lately */
int p2p_master_connected(void);
/* whether a join can go ahead: the server told this machine's address, or
is not going to (none configured, unreachable, or slow) */
int p2p_master_ready(void);
/* hosting: listed, and answering joiners who hold this token */
void p2p_master_host(const unsigned char *token);
void p2p_master_stop_hosting(void);
/* joining: asks the host with this identifier, holding this token, until
it answers (or p2p_master_stop_joining) */
void p2p_master_join(const unsigned char *host_identifier, const unsigned char *token);
void p2p_master_stop_joining(void);

/* ---------- p2p_crypto.c */

enum
{
	P2P_SHA256_SIZE = 32,
	/* what p2p_seal adds: a 12-byte nonce and a 16-byte tag */
	P2P_SEAL_OVERHEAD = 28,
};

void p2p_sha256(const void *data, int size, unsigned char *digest);
void p2p_hmac_sha256(const unsigned char *key, int key_size, const void *data, int size, unsigned char *digest);
/* encrypts and authenticates plaintext with key into sealed (size +
P2P_SEAL_OVERHEAD bytes); returns the sealed size */
int p2p_seal(const unsigned char *key, const void *plaintext, int size, unsigned char *sealed);
/* the reverse, if sealed was made with key: returns the plaintext size, or
-1 if it was not (or was altered) */
int p2p_open(const unsigned char *key, const unsigned char *sealed, int size, unsigned char *plaintext);

/* ---------- p2p_discord.c: rich presence and invites through the Discord
desktop client (not on the console) */

/* called from the p2p thread each pass */
void p2p_discord_update(void);
/* what to show: hosting with an invite link's secret and player counts, or
not (secret NULL) */
void p2p_discord_set_hosting(const char *secret, int player_count, int maximum_player_count);

#endif
