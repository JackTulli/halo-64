/*
N64_BOTS_MODULE.H

The bots' module (rom:/bots.dso: port/n64/game/n64_bots.c) and the program
(port/n64/game/n64_bots_stub.c): the one symbol the module exports,
n64_bots_entries, its entry points; what the program keeps for the bots
from game to game; and the program's functions the module calls by name.
The program loads the module for a game with bots, after the map, and gives
it back with the map: a game without bots keeps none of it. After
cseries.h and math/real_math.h.
*/

#ifndef N64_BOTS_MODULE_H
#define N64_BOTS_MODULE_H

#include <stdint.h>

#define N64_BOTS_ENTRIES "n64_bots_entries"

struct player_action;
struct path_result;
struct structure_bsp;

struct n64_bots_entries
{
	/* a game's bots, once the module is in (n64_bots_create_players): their
	count, skill and whether the game has teams; FALSE: no room for their
	records, no bots */
	int (*begin)(int count, int skill, int teams);
	/* the game over: the path search's memory given back, nothing more
	called */
	void (*stop)(void);
	/* the game's entry points (n64_bots_stub.c), passed on */
	void (*create_players)(void);
	void (*actions)(struct player_action *actions);
	int (*pose_interval)(long unit_index);
	int (*move_interval)(long unit_index);
	int (*number)(long player_index);
	boolean (*examine_turn)(long player_index);
	const char *(*display_name)(long player_index);
};

/* the gates the bots found on the map (n64_bots.c, its gates section),
kept by the program with the map's image from game to game: the level they
are in and the scenario's name, whether the look was over, a pathfinding
surface they took out (NONE: none), how many, and each one's line across
the passage (its near face) and its foot's height */
#define N64_BOT_GATES 4
struct n64_bot_gates
{
	struct structure_bsp const *structure;
	char scenario[64];
	boolean complete;
	long probe;
	short count;
	struct
	{
		real_point2d a, b;
		real z;
	} gates[N64_BOT_GATES];
};
extern struct n64_bot_gates n64_bot_gates;

/* the lobby's (n64_bots_stub.c): a bot's Ghost profile in it
(n64_bot_ghosts.h), and the seed of the bots' camouflage rolls, which runs
on from game to game */
int n64_bot_lobby_profile(int index, int count);
extern uint32_t n64_bot_ghost_spawn_seed;

/* the AI's helpers (n64_bots_stub.c; n64_campaign.c's and n64_firefight.c's
too) */
boolean n64_ai_clusters_may_see(long object_a, long object_b);
boolean n64_ai_direct_path(real_point3d const *start, long start_surface, real_point3d const *goal,
	long goal_surface, struct path_result *path);

#endif
