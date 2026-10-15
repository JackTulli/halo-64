/*
HALO_N64_LIMITS.H

The Nintendo 64 build's limits. The console has 8 MB with the Expansion
Pak, so the pools are a fraction of the Xbox's (port/include/
halo_session_limits.h): four local players and nothing else, and as few
objects, effects, particles and sounds as a multiplayer map needs. The game
state is an ordinary allocation (physical_memory_map.c); its base address
here is only the Xbox's, kept for code that names it.
*/

#ifndef __HALO_N64_LIMITS_H
#define __HALO_N64_LIMITS_H

#define HALO_PORT_FD_SETSIZE 8

/* ---------- game state */

#define HALO_PORT_GAME_STATE_BASE_ADDRESS 0x80061000
#define HALO_PORT_GAME_STATE_CPU_SIZE 0x100000
#define HALO_PORT_GAME_STATE_GPU_SIZE 0
#define HALO_PORT_GAME_STATE_SIZE (HALO_PORT_GAME_STATE_CPU_SIZE+HALO_PORT_GAME_STATE_GPU_SIZE)

/* ---------- session limits: split screen only */

#define HALO_PORT_MAXIMUM_NETWORK_PLAYERS 16
#define HALO_PORT_MAXIMUM_NETWORK_MACHINES 16

/* ---------- pools */

#define HALO_PORT_MAXIMUM_OBJECTS_PER_MAP 512
#ifdef HALO_N64_CAMPAIGN
/* (a campaign level: its encounters' actors, ~7 KB a biped, beside the
scenery near the players -- port/n64/game/n64_campaign.c) */
#define HALO_PORT_OBJECT_MEMORY_POOL_SIZE 0x48000
#else
#define HALO_PORT_OBJECT_MEMORY_POOL_SIZE 0x40000
#endif
#define HALO_PORT_MAXIMUM_CLUSTER_REFERENCES 512
#define HALO_PORT_MAXIMUM_RENDERED_OBJECTS 64
#define HALO_PORT_MAXIMUM_CACHED_OBJECT_RENDER_STATES 64
#define HALO_PORT_MAXIMUM_AREA_OF_EFFECT_OBJECTS 32
#define HALO_PORT_MAXIMUM_LISTED_OBJECTS_PER_MAP 128
#define HALO_PORT_MAXIMUM_EFFECTS 64
#define HALO_PORT_MAXIMUM_EFFECT_LOCATIONS 128
/* Halo's particles, particle systems and contrails are never made on the
N64 (their creators return at once; port/n64/src/n64_particles.c has its
own): a token pool each, the game state's memory given back to the heap
(n64_game_state_trim) */
#define HALO_PORT_MAXIMUM_PARTICLES 1
#define HALO_PORT_MAXIMUM_PARTICLE_SYSTEMS 1
#define HALO_PORT_MAXIMUM_SYSTEM_PARTICLES 1
#define HALO_PORT_MAXIMUM_CONTRAILS 1
#define HALO_PORT_MAXIMUM_CONTRAIL_POINTS 1
#define HALO_PORT_MAXIMUM_LIGHTS_PER_MAP 64
#define HALO_PORT_MAXIMUM_GAME_LOOPING_SOUNDS 128

/* ---------- struct network_game's layout (the game's records) */

#include "../../include/halo_session_limits.h"

#endif /* __HALO_N64_LIMITS_H */
