/*
HALO_SESSION_LIMITS.H

The session limits and memory capacity every build shares, so that consoles
and computers play together: 16 players on up to 16 machines (split screen
stays at 4 players per machine), and the retail game's pools of objects,
effects, particles, lights and sounds, which fit the console's 64 MB. Every
machine in a session must be built with the same values: the game is
simulated in lockstep on every machine, and a pool that runs full changes
the simulation (an object or a deterministic effect is not created).

The native builds can instead be built for sessions of up to 128 players
and machines with larger pools (configure.py --big-sessions,
port/linux/include/halo_port_limits.h and halo_port_capacity.h); such
builds play only with each other. The game sources use these names only
under #ifdef HALO_LINUX, so the byte-matching MSVC build keeps the original
values (16 players on 4 machines).

The layout of struct network_game and the protocol constants at the end
follow from the limits, whichever they are.
*/

#ifndef __HALO_SESSION_LIMITS_H
#define __HALO_SESSION_LIMITS_H

#if !defined(HALO_BIG_SESSIONS) && !defined(HALO_N64)

/* ---------- session limits */

#define HALO_PORT_MAXIMUM_NETWORK_PLAYERS 16
#define HALO_PORT_MAXIMUM_NETWORK_MACHINES 16

/* ---------- pools: the retail game's (physical_memory_map.c) */

#define HALO_PORT_MAXIMUM_OBJECTS_PER_MAP 2048
#define HALO_PORT_OBJECT_MEMORY_POOL_SIZE 0x100000
#define HALO_PORT_MAXIMUM_CLUSTER_REFERENCES 2048
#define HALO_PORT_MAXIMUM_RENDERED_OBJECTS 256
#define HALO_PORT_MAXIMUM_CACHED_OBJECT_RENDER_STATES 256
#define HALO_PORT_MAXIMUM_AREA_OF_EFFECT_OBJECTS 64
#define HALO_PORT_MAXIMUM_LISTED_OBJECTS_PER_MAP 128
#define HALO_PORT_MAXIMUM_EFFECTS 256
#define HALO_PORT_MAXIMUM_EFFECT_LOCATIONS 512
#define HALO_PORT_MAXIMUM_PARTICLES 1024
#define HALO_PORT_MAXIMUM_PARTICLE_SYSTEMS 64
#define HALO_PORT_MAXIMUM_SYSTEM_PARTICLES 512
#define HALO_PORT_MAXIMUM_CONTRAILS 256
#define HALO_PORT_MAXIMUM_CONTRAIL_POINTS 1024
#define HALO_PORT_MAXIMUM_LIGHTS_PER_MAP 896
#define HALO_PORT_MAXIMUM_GAME_LOOPING_SOUNDS 1024

#endif /* HALO_BIG_SESSIONS */

/* ---------- struct network_game layout

The game settings record (struct network_game) is declared separately in
several networking and interface units; its layout follows from the limits.
Every copy checks its offsets against these values. The Xbox game's values
(4 machines, 16 players) are 0x226 and 0x434. */

#define HALO_PORT_NETWORK_MACHINE_SIZE 0x44
#define HALO_PORT_NETWORK_PLAYER_SIZE 0x20
#define HALO_PORT_NETWORK_GAME_MACHINES_OFFSET 0x114
#define HALO_PORT_NETWORK_GAME_PLAYER_COUNT_OFFSET \
	(HALO_PORT_NETWORK_GAME_MACHINES_OFFSET + HALO_PORT_MAXIMUM_NETWORK_MACHINES * HALO_PORT_NETWORK_MACHINE_SIZE)
#define HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET (HALO_PORT_NETWORK_GAME_PLAYER_COUNT_OFFSET + 2)
#define HALO_PORT_NETWORK_GAME_PLAYERS_END \
	(HALO_PORT_NETWORK_GAME_PLAYERS_OFFSET + HALO_PORT_MAXIMUM_NETWORK_PLAYERS * HALO_PORT_NETWORK_PLAYER_SIZE)
#define HALO_PORT_NETWORK_GAME_RANDOM_SEED_OFFSET (HALO_PORT_NETWORK_GAME_PLAYERS_END + 2)
#define HALO_PORT_NETWORK_GAME_LOCAL_DATA_OFFSET (HALO_PORT_NETWORK_GAME_PLAYERS_END + 0xA)
#define HALO_PORT_NETWORK_GAME_SIZE (HALO_PORT_NETWORK_GAME_PLAYERS_END + 0xE)

/* ---------- system link protocol

The ports' messages differ from the Xbox game's (longer arrays, the game
settings record in fragments), so they search for games with their own
protocol version and never see the Xbox game's, or it theirs. The version
also names the layout: 2 is the released builds' (128 machines and
players, which --big-sessions keeps), 3 this one of 16 and the retail
pools, so the two never list each other's games only to fail joining. */

#ifdef HALO_BIG_SESSIONS
#define HALO_PORT_NETWORK_GAME_MESSAGE_VERSION 2
#else
#define HALO_PORT_NETWORK_GAME_MESSAGE_VERSION 3
#endif
/* a message header's 12-bit length allows messages of up to 0xFFF bytes,
header included; the per-tick update of 128 players is 3,857 */
#define HALO_PORT_MAXIMUM_NETWORK_MESSAGE_SIZE 0x1000
/* the packet codec's limit on a decoded or encoded packet; the per-tick
update of 128 players decodes to 0x1010 bytes */
#define HALO_PORT_NETWORK_PACKET_SIZE 0x1100
/* the game settings record (HALO_PORT_NETWORK_GAME_SIZE, 13,092 bytes at 128
machines and players) does not fit one message; it is sent in pieces of
this many bytes (4 pieces), each 3,594 bytes on the wire */
#define HALO_PORT_NETWORK_GAME_SETTINGS_FRAGMENT_SIZE 0xE00

#endif /* __HALO_SESSION_LIMITS_H */
