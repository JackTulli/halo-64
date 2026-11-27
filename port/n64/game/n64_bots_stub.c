/*
N64_BOTS_STUB.C

The bots' resident part. Their code and state are a module, rom:/bots.dso
(port/n64/game/n64_bots.c, built by tools/n64_build.py): loaded for a game
with bots once its map is in (n64_bots_create_players, as the path search's
memory is: after the map, so that it splits none of the memory the map
wants), given back with the map (n64_game.c: n64_bots_module_end) or at the
next game's start. A game without bots -- BOTS 0, Firefight -- keeps none of
it. The campaign and online cartridges carry no module: their games have no
bots (a campaign level; online Slayer, n64_game.c).

Here: the lobby's names and teams (the menu's, before a game); what the
program keeps for the bots from game to game (the lobby's seeds, the gates
found on the map); the AI's two helpers that n64_campaign.c and
n64_firefight.c use too; and each entry point the game calls, passed on to
the module while it is loaded. Without it each gives what it gives in a
game without bots.
*/

#include "cseries.h"

#include "math/real_math.h"
#include "objects/objects.h"
#include "ai/path.h"
#include "ai/path_structure_bsp.h"
#include "ai/ai_debug.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"

#include "../src/n64.h"
#include "n64_bots_module.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "n64_bot_names.h"
#include "n64_bot_ghosts.h"

/* the module's file (n64_module.c finds it room) */
#define BOTS_MODULE "rom:/bots.dso"
#define MAXIMUM_BOTS 15

/* (the module's, kept here from game to game) */
struct n64_bot_gates n64_bot_gates;
uint32_t n64_bot_ghost_spawn_seed;

/* the module while it is loaded, and its entry points; the game's bots */
static void *module;
static const struct n64_bots_entries *entry;
static short bots_count, bots_skill;
static boolean bots_teams;

/* ---------- the lobby */

/* A lobby plan survives bots' per-map reset. Moving between teams never
rerolls names. The same seed and count produce the preview and real roster. */
static uint32_t bot_lobby_seed, bot_lobby_serial;
static boolean bot_lobby_prepared;

void n64_bots_prepare_lobby(void)
{
	char value[32];
	bot_lobby_seed = (uint32_t)n64_ticks() ^ (++bot_lobby_serial * UINT32_C(0x9e3779b9));
	/* Explicit private fixture; ordinary flags[] never pins the roster. */
	if (n64_debug_text("debug_bot_seed", value, sizeof(value)))
		bot_lobby_seed = (uint32_t)strtoul(value, NULL, 0);
	n64_bot_ghost_spawn_seed = bot_lobby_seed ^ UINT32_C(0xa341316c);
	bot_lobby_prepared = TRUE;
}

const char *n64_bot_lobby_name(int index, int bot_count)
{
	int profile;
	const char *name;
	if (index < 0 || index >= bot_count || bot_count > MAXIMUM_BOTS) return "";
	if (!bot_lobby_prepared) n64_bots_prepare_lobby();
	profile = n64_ghost_profile(bot_lobby_seed ^ UINT32_C(0xc8013ea4), index, bot_count);
	name = n64_ghost_name(profile);
	return name ? name : n64_bot_profile_name(bot_lobby_seed, index);
}

int n64_bot_lobby_team(int index, const int *human_teams, int human_count)
{
	return n64_ghost_lobby_team(index, human_teams, human_count);
}

int n64_bot_lobby_profile(int index, int count)
{
	if (!bot_lobby_prepared) n64_bots_prepare_lobby();
	return n64_ghost_profile(bot_lobby_seed ^ UINT32_C(0xc8013ea4), index, count);
}

/* ---------- the AI's helpers */

struct structure_bsp *global_structure_bsp_get(void);

/* whether the level's potentially visible set lets the clusters of two
objects (their ultimate parents') see each other: it only over-reports, so
a pair it rules out has no line of sight and needs no ray (Halo's own AI
asks it first too; also n64_firefight.c's) */
boolean n64_ai_clusters_may_see(
	long object_a,
	long object_b)
{
	struct structure_bsp *bsp = global_structure_bsp_get();
	short a = object_get(object_get_ultimate_parent(object_a))->object.location.cluster_index;
	short b = object_get(object_get_ultimate_parent(object_b))->object.location.cluster_index;

	if (!bsp || bsp->cluster_data.size <= 0 || a == NONE || b == NONE ||
		a < 0 || b < 0 || a >= bsp->clusters.count || b >= bsp->clusters.count)
	{
		return TRUE;
	}
	return scenario_test_pvs(a, b);
}

/* a straight walk from one point to another over the level's surfaces --
the pathfinder's own pill test, as its smoothing makes it -- as a path of
one step, with no search; FALSE where the way is blocked (also
n64_firefight.c's) */
boolean n64_ai_direct_path(
	real_point3d const *start,
	long start_surface,
	real_point3d const *goal,
	long goal_surface,
	struct path_result *path)
{
	struct path_collision_result collision;
	boolean blocked = structure_test_pill2d(global_structure_bsp_get(), FALSE,
		(real_point2d const *)start, start_surface, (real_point2d const *)goal, goal_surface,
		0.3f, 1 /* the end near a wall is all right */, &collision);

	if (blocked)
	{
		return FALSE;
	}
	csmemset(path, 0, sizeof(*path));
	path->valid = TRUE;
	path->endpoint.point = *goal;
	path->endpoint.surface_index = goal_surface;
	path->endpoint.target_radius = 0.f;
	path->steps_finish_path = TRUE;
	path->step_count = 1;
	path->step_index = 0;
	path->steps[0].surface_index = goal_surface;
	path->steps[0].point = *goal;
	return TRUE;
}

/* ---------- the game's side */

/* the module given back, its path search's memory first; its code no
longer called */
void n64_bots_module_end(
	void)
{
	if (entry)
		entry->stop();
	entry = NULL;
	n64_module_free(module);
	module = NULL;
}

/* n64_game.c's, as it sets a game up (before its map): the menu's bots */
void n64_bots_begin(
	int count,
	int skill,
	int teams)
{
	/* (the last game's module and its memory given back before the map
	arena grows) */
	n64_bots_module_end();
	bots_count = (short)PIN(count, 0, MAXIMUM_BOTS);
	bots_skill = (short)PIN(skill, 0, 3);
	bots_teams = teams != 0;
	if (!bot_lobby_prepared) n64_bots_prepare_lobby();
	if (bots_count > 0)
	{
		/* (the paths' last stage steers around nearby objects from the
		AI's own records of them, which the N64 has not got) */
		ai_debug.path_disable_obstacle_avoidance = TRUE;
	}
}

/* (a game with no bots: online, a campaign level, Firefight) */
void n64_bots_stop(
	void)
{
	n64_bots_module_end();
	bots_count = 0;
}

/* main.c's create_local_players, after the players on the controllers:
the module, once (its load timed in the log), then the bots */
void n64_bots_create_players(
	void)
{
	if (!bots_count)
		return;
	if (!entry)
	{
		unsigned long long start = n64_microseconds();
		unsigned long resident = 0;
		void *address;
		char line[96];

		/* (no room for it: no bots this game; n64_module.c's line says how
		much there was) */
		module = n64_module_load_into(BOTS_MODULE, N64_BOTS_ENTRIES, 0, &address, NULL, 0, &resident);
		if (!module)
		{
			n64_log("n64: the bots' module did not load: no bots");
			bots_count = 0;
			return;
		}
		entry = (const struct n64_bots_entries *)address;
		snprintf(line, sizeof(line), "n64: bots' module loaded, %lu bytes, in %lu us", resident,
			(unsigned long)(n64_microseconds() - start));
		n64_log(line);
		/* (no room for the bots' records: no bots this game either, the
		module given back) */
		if (!entry->begin(bots_count, bots_skill, bots_teams))
		{
			n64_log("n64: no room for the bots' records: no bots");
			n64_bots_module_end();
			bots_count = 0;
			return;
		}
	}
	entry->create_players();
}

/* update_client_dequeue's (player_queues_new.c, HALO_N64): a tick's
actions, the bots' written over */
void n64_bots_actions(
	struct player_action *actions)
{
	if (entry)
		entry->actions(actions);
}

/* objects.c's (n64_far_ai_biped): a bot's pose interval; with three or
four views every other tick */
int n64_bot_pose_interval(
	long unit_index)
{
	extern int n64_view_total;

	if (entry)
		return entry->pose_interval(unit_index);
	return n64_view_total >= 3 ? 2 : 1;
}

/* bipeds.c's (biped_update): a bot's movement interval; a campaign's
actors' (n64_campaign.c) */
int n64_bot_move_interval(
	long unit_index)
{
	if (entry)
		return entry->move_interval(unit_index);
#ifdef HALO_N64_CAMPAIGN
	{
		extern int n64_campaign_move_interval(long unit_index);

		return n64_campaign_move_interval(unit_index);
	}
#else
	return 1;
#endif
}

/* the bot a player is (its number, 0 on), or -1 */
int n64_bot_number(
	long player_index)
{
	return entry ? entry->number(player_index) : -1;
}

/* players_update_after_game's (players.c, HALO_N64): whether a player's
nearby objects are looked over this tick */
boolean n64_bots_examine_turn(
	long player_index)
{
	return entry ? entry->examine_turn(player_index) : TRUE;
}

/* the scoreboard's (n64_game.c): a bot's full name, or NULL */
const char *n64_bots_display_name(
	long player_index)
{
	return entry ? entry->display_name(player_index) : NULL;
}
