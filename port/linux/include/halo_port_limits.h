/*
HALO_PORT_LIMITS.H

Multiplayer session limits of the native builds (Windows, Linux, Android),
force-included by halo_linux_prefix.h and halo_windows_prefix.h.

By default the native builds use the limits every build shares
(port/include/halo_session_limits.h: 16 players on up to 16 machines, the
retail game's pools), so that they play with the Xbox build. Built with
configure.py --big-sessions (HALO_BIG_SESSIONS) they allow 128 players on
up to 128 machines with larger pools (halo_port_capacity.h); split screen
stays at 4 players per machine. Such builds play only with each other.

128 is the largest session that fits the game's existing records: player,
machine and team indices are stored in signed chars (0..127 with NONE), and
a finishing place in 7 bits.
*/

#ifndef __HALO_PORT_LIMITS_H
#define __HALO_PORT_LIMITS_H

#ifdef HALO_BIG_SESSIONS
#define HALO_PORT_MAXIMUM_NETWORK_PLAYERS 128
#define HALO_PORT_MAXIMUM_NETWORK_MACHINES 128
#endif

/* a host polls its listening socket and one socket per machine; the Xbox's
Winsock headers default to 64 (the prefix headers define FD_SETSIZE from
this before any of them is read) */
#define HALO_PORT_FD_SETSIZE 256

/* ---------- the game state's place and size, and the pools of big
sessions */
#include "halo_port_capacity.h"

/* ---------- the shared limits (unless HALO_BIG_SESSIONS), the layout of
struct network_game and the protocol */
#include "../../include/halo_session_limits.h"

#endif /* __HALO_PORT_LIMITS_H */
