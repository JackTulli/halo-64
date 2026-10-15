/*
N64_BOTS.C

Bots in multiplayer games: players of the game like the ones on the
controllers -- made with player_new (each on a machine of its own, as a
network game's remote players are: a machine holds four), spawned by the
game engine in the player biped, scored, respawned, on a team and coloured
by the game's own rules -- whose actions (the stick, the aim, the buttons)
come from here instead of a controller. update_client_dequeue (player_
queues_new.c, HALO_N64) hands each tick's actions to n64_bots_actions, which
writes the bots' own; players_update_before_game then applies them exactly
as it does a person's: the action button picks up a weapon (player_handle_
action), swaps them, gets out of a vehicle; triggers, grenades, melee, jump
and reload go to unit_control.

Their way of fighting is the Firefight Covenant's (n64_firefight.c), kept
simple: each picks the nearest enemy (another team's player in a team game,
anyone else otherwise), goes for it along Halo's own paths while it is out
of sight, keeps its weapon's range and strafes when in sight, fires in
bursts (held for the automatic weapons, pressed shot by shot for the rest)
with an aim that misses by more at an easier skill, throws grenades now and
then, meleees up close and works its way around whatever stops it. With no
enemy in sight it goes for a better weapon lying near, and it reloads,
swaps to its other weapon when this one is empty and gets out of any
vehicle it found itself in. It never fires through a teammate: a shot needs
a clear line (objects included) to the enemy.
*/

#include "cseries.h"

#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "game/local_players.h"
#include "items/weapons.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "physics/collisions.h"
#include "tag_files/tag_groups.h"
#include "units/units.h"
#include "units/bipeds.h"
#include "ai/path.h"
#include "ai/path_structure_bsp.h"
#include "ai/ai_debug.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"

#include "../src/n64.h"

#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "n64_bot_names.h"
#include "n64_bot_ghosts.h"

/* ---------- constants */

#define MAXIMUM_BOTS 12
/* a path is found again after this long, or when its target has moved */
#define PATH_LIFETIME (5 * TICKS_PER_SECOND)
/* a straight walk to the target is tested before a search, at most this
many a tick among the bots; a bot whose straight walk was blocked searches
for this long before testing one again */
#define DIRECT_TESTS_PER_TICK 2
#define DIRECT_PATH_RETRY_TICKS 30
/* a search goes on over several ticks, opening at most this many surfaces
a tick (~1 ms) */
#define SEARCH_SURFACES_PER_TICK 40
/* paths found at most every this many ticks, among all the bots (a search
is the dearest thing a bot does) */
#define TICKS_BETWEEN_PATHS 3
/* no line of sight looked for past this (world units): out of sight */
#define SIGHT_RANGE 50.f
/* how far a bot looks for a better weapon (world units), how often */
#define WEAPON_SEARCH_RANGE 14.f
#define WEAPON_SEARCH_TICKS 45
/* a weapon it could not reach is left alone this long */
#define WEAPON_GIVE_UP_TICKS (4 * TICKS_PER_SECOND)
/* a bot's level of detail (bot_detail): near a player within this, far
past that (world units) */
#define BOT_NEAR 15.f
#define BOT_FAR 30.f
/* the longest a bot's thought counts for (its timers' step) */
#define MAXIMUM_THINK_STEP 12

enum
{
	_style_semi, /* pressed shot by shot */
	_style_auto, /* held */
	_style_explosive, /* pressed, never up close */
};

/* the weapons it knows: how much it wants each, how it fires it and the
range it keeps with it */
static struct bot_weapon
{
	char const *name;
	short score;
	short style;
	real near_range, far_range;
} const bot_weapons[] =
{
	{ "weapons\\rocket launcher\\rocket launcher", 9, _style_explosive, 7.f, 18.f },
	{ "weapons\\sniper rifle\\sniper rifle", 8, _style_semi, 10.f, 28.f },
	{ "weapons\\shotgun\\shotgun", 8, _style_semi, 1.5f, 5.f },
	{ "weapons\\pistol\\pistol", 8, _style_semi, 4.f, 16.f },
	{ "weapons\\plasma_cannon\\plasma_cannon", 7, _style_explosive, 7.f, 18.f },
	{ "weapons\\assault rifle\\assault rifle", 6, _style_auto, 2.5f, 10.f },
	{ "weapons\\plasma rifle\\plasma rifle", 6, _style_auto, 3.f, 12.f },
	{ "weapons\\needler\\needler", 5, _style_auto, 4.f, 14.f },
	{ "weapons\\flamethrower\\flamethrower", 4, _style_auto, 1.5f, 5.f },
	{ "weapons\\plasma pistol\\plasma pistol", 3, _style_semi, 3.f, 12.f },
};
#define NUMBER_OF_BOT_WEAPONS ((short)(sizeof(bot_weapons) / sizeof(bot_weapons[0])))

/* by skill (easy, normal, heroic, legendary) */
static real const aim_error[4] = { 0.16f, 0.09f, 0.055f, 0.03f };
static short const burst_ticks[4] = { 8, 12, 16, 22 };
static short const pause_ticks[4] = { 34, 22, 14, 8 };
static short const grenade_chance[4] = { 2, 4, 6, 8 }; /* in 1000 a tick */

/* their armour in free-for-all games (Halo's colour indices: pink, orange,
purple, cyan, cobalt, brown, teal, sage, salmon, tan, maroon, white) */
static short const bot_colors[MAXIMUM_BOTS] = { 7, 11, 8, 9, 10, 14, 12, 13, 17, 15, 16, 0 };

/* ---------- state */

struct bot
{
	long player_index;
	long unit_index; /* the unit its state is about (a new one resets it) */
	long target_unit;
	boolean target_visible;
	boolean trigger_down;
	short sight_timer, choose_timer;
	short strafe_direction, strafe_timer;
	short fire_timer; /* > 0 firing, < 0 pausing */
	short stuck_timer, stuck_check, unstick_timer, jump_timer;
	short melee_timer, grenade_timer, action_timer, reload_timer, swap_timer;
	short weapon_timer;
	real_point3d last_seen, last_position;
	real_vector3d aim;
	real_vector2d unstick_throttle;
	/* a better weapon on the ground it is going for, and one it gave up on */
	long goal_weapon;
	short goal_ticks;
	long given_up_weapon;
	short given_up_timer;
	/* its action when it last thought (held between thoughts, its presses
	not repeated) */
	struct player_action last_action;
	/* the way to its target (Halo's pathfinding, source/ai/path.c) */
	boolean path_valid;
	short path_age;
	struct path_result path;
	real_point3d path_goal;
	short direct_wait; /* > 0: no straight walk tested (plan_path) */
	short ghost_profile; /* preserved across native respawns */
	/* when it last thought and thinks next (game ticks: its own rate,
	bot_think_interval) */
	long last_think, next_think;
	/* its level of detail (bot_detail) and the tick it was worked out */
	short detail;
	long detail_time;
	/* its action fires (a trigger, a grenade, a melee): its pose is kept at
	the count's rate (n64_bot_pose_interval) */
	boolean firing;
};

/* New LOD fields extend the old prefix; read diagnostic stride from DWARF. */
typedef char n64_bot_observer_stride[sizeof(struct bot) == 268 ? 1 : -1];
typedef char n64_bot_observer_ghost_offset[offsetof(struct bot, ghost_profile) == 246 ? 1 : -1];

static struct
{
	boolean active;
	boolean resolved;
	boolean teams;
	short count;
	short skill;
	short created;
	unsigned long random;
	short paths_this_tick;
	short path_cooldown;
	short direct_tests_this_tick;
	/* the search going on in parts: its bot + 1 (0: none), that bot's
	unit when it began, where it goes */
	short search_owner;
	long search_unit;
	real_point3d search_goal;
	long weapon_definitions[NUMBER_OF_BOT_WEAPONS];
	struct bot bots[MAXIMUM_BOTS];
} bots;

/* the pathfinder's working state (47 KB, only while there are bots) */
static struct path_state *bots_path_state;

/* A lobby plan survives bots' per-map reset. Moving between teams never
rerolls names. The same seed and count produce the preview and real roster. */
static uint32_t bot_lobby_seed, bot_ghost_spawn_seed, bot_lobby_serial;
static boolean bot_lobby_prepared;
/* A life identifier is independent of cadence/paused thought-state resets. */
static long bot_ghost_spawn_units[MAXIMUM_BOTS];

void n64_bots_prepare_lobby(void)
{
	char value[32];
	bot_lobby_seed = (uint32_t)n64_ticks() ^ (++bot_lobby_serial * UINT32_C(0x9e3779b9));
	/* Explicit private fixture; ordinary flags[] never pins the roster. */
	if (n64_debug_text("debug_bot_seed", value, sizeof(value)))
		bot_lobby_seed = (uint32_t)strtoul(value, NULL, 0);
	bot_ghost_spawn_seed = bot_lobby_seed ^ UINT32_C(0xa341316c);
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

const char *n64_bots_display_name(long player_index)
{
	int i;
	for (i = 0; bots.active && i < bots.created; i++)
		if (bots.bots[i].player_index == player_index)
			return n64_ghost_name(bots.bots[i].ghost_profile);
	return NULL;
}

/* Private diagnostic only. Disabled in production; the A/B distance gate
skips an expensive shortcut attempt, then keeps the native path search. */
static int bot_profile_enabled = -1, bot_direct_limit = 0;
static unsigned long bot_profile_calls[4], bot_profile_worst[4];
static unsigned long long bot_profile_ticks[4];
static unsigned long bot_profile_actions;

static unsigned long bot_profile_start(void)
{
    if (bot_profile_enabled < 0)
    {
        char limit[12];
        bot_profile_enabled = n64_debug_flag("debug_bot_profile");
        if (n64_debug_text("debug_bot_direct_limit", limit, sizeof(limit)))
            bot_direct_limit = atoi(limit);
    }
    return bot_profile_enabled ? n64_ticks() : 0;
}

static void bot_profile_end(int part, unsigned long start)
{
    unsigned long elapsed;
    if (!bot_profile_enabled) return;
    elapsed = n64_ticks() - start;
    bot_profile_calls[part]++;
    bot_profile_ticks[part] += elapsed;
    if (elapsed > bot_profile_worst[part]) bot_profile_worst[part] = elapsed;
}

static void bot_profile_poll(void)
{
    int part;
    char line[160];
    static char const *const names[4] = { "LOS", "direct", "search", "build/smooth" };
    if (!bot_profile_enabled || ++bot_profile_actions < 150) return;
    for (part = 0; part < 4; part++)
    {
        snprintf(line, sizeof(line), "bot profile %s: %lu calls, %llu us total/150 actions, %llu us worst",
            names[part], bot_profile_calls[part],
            bot_profile_ticks[part] * 1000000 / N64_TICKS_PER_SECOND,
            (unsigned long long)bot_profile_worst[part] * 1000000 / N64_TICKS_PER_SECOND);
        n64_log(line);
        bot_profile_calls[part] = bot_profile_worst[part] = 0;
        bot_profile_ticks[part] = 0;
    }
    bot_profile_actions = 0;
}

/* ---------- small things */

static unsigned long bot_random(
	void)
{
	bots.random = bots.random * 1664525UL + 1013904223UL;
	return bots.random >> 8;
}

/* -1 to 1 */
static real bot_random_real(
	void)
{
	return (real)(bot_random() & 0xFFFF) / 32768.f - 1.f;
}

static real horizontal_distance(
	real_point3d const *a,
	real_point3d const *b)
{
	real dx = a->x - b->x, dy = a->y - b->y;

	return square_root(dx * dx + dy * dy);
}

static void normalize(
	real_vector3d *v)
{
	real length = square_root(v->i * v->i + v->j * v->j + v->k * v->k);

	if (length > 1e-4f)
	{
		v->i /= length;
		v->j /= length;
		v->k /= length;
	}
	else
	{
		v->i = 1.f;
		v->j = 0.f;
		v->k = 0.f;
	}
}

static boolean unit_alive(
	long unit_index)
{
	struct object_datum *object;

	if (unit_index == NONE)
		return FALSE;
	object = object_try_and_get(unit_index);
	return object && TEST_FLAG(_object_mask_unit, object->object.type) &&
		!TEST_FLAG(object->object.damage_flags, _object_dead_bit);
}

/* ---------- a bot's level of detail

Each biped costs ~2.7 ms of a tick, its pose (node matrices) the dearest
part, and with 8 to 12 bots most of them are at any moment behind a wall
or across the map: a bot drawn in a view lately and near a player is posed
and thinks at the count's rate (n64_bots_pose_interval); one far from every
player or in no view is posed less often; one far and in no view is posed
least and thinks half as often (objects.c's AI rule, n64_far_ai_biped, as
Firefight's Covenant have it). A bot firing is posed at the count's rate
whatever it is: its shots come from its weapon's markers, its pose */
enum
{
	_bot_detail_full,
	_bot_detail_reduced,
	_bot_detail_least
};

boolean n64_object_recently_seen(long object_index);

static short bot_detail(
	struct bot *bot)
{
	long now = game_time_get();
	real nearest = 1e30f;
	short local_player_index;
	boolean seen;

	if (bot->detail_time == now)
		return bot->detail;
	bot->detail_time = now;
	if (!unit_alive(bot->unit_index))
		return bot->detail = _bot_detail_full;
	seen = n64_object_recently_seen(bot->unit_index);
	for (local_player_index = 0; local_player_index < MAXIMUM_LOCAL_PLAYERS; local_player_index++)
	{
		long player_index = local_player_get_player_index(local_player_index);
		long unit_index = player_index != NONE ? player_get(player_index)->unit_index : NONE;

		if (unit_index != NONE && object_try_and_get(unit_index))
		{
			real d = distance_squared3d(&object_get(unit_index)->object.position,
				&object_get(bot->unit_index)->object.position);

			if (d < nearest)
				nearest = d;
		}
	}
	if (seen && nearest < BOT_NEAR * BOT_NEAR)
		bot->detail = _bot_detail_full;
	else if (!seen && nearest >= BOT_FAR * BOT_FAR)
		bot->detail = _bot_detail_least;
	else
		bot->detail = _bot_detail_reduced;
	return bot->detail;
}

static struct bot *bot_of_unit(
	long unit_index)
{
	short i;

	for (i = 0; bots.active && i < bots.created; i++)
	{
		if (bots.bots[i].unit_index == unit_index)
			return &bots.bots[i];
	}
	return NULL;
}

/* a clear line from one point to another: nothing of the level and no
object in between (a teammate in the way: no shot) */
static boolean line_of_sight(
	real_point3d const *from,
	real_point3d const *to,
	long ignore_object_index)
{
	extern int n64_collision_any_hit;
	struct collision_result collision;
	real_vector3d vector;
	boolean blocked;
	unsigned long profile_start = bot_profile_start();

	vector.i = to->x - from->x;
	vector.j = to->y - from->y;
	vector.k = to->z - from->z;
	/* (only whether anything is in the way: with the level in the way its
	objects aren't tested -- collisions.c) */
	n64_collision_any_hit = 1;
	blocked = collision_test_vector(
		FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit) |
			FLAG(_collision_test_objects_bit) | _collision_test_objects_sight_blocking_flags,
		from, &vector, ignore_object_index, &collision);
	n64_collision_any_hit = 0;
	bot_profile_end(0, profile_start);
	return !blocked;
}

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
	unsigned long profile_start = bot_profile_start();
	boolean blocked = structure_test_pill2d(global_structure_bsp_get(), FALSE,
		(real_point2d const *)start, start_surface, (real_point2d const *)goal, goal_surface,
		0.3f, 1 /* the end near a wall is all right */, &collision);
	bot_profile_end(1, profile_start);
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

/* the bot weapons table's entry for a weapon object's definition, or NONE */
static short weapon_entry(
	long weapon_index)
{
	struct object_datum *object;
	short i;

	if (weapon_index == NONE || !(object = object_try_and_get(weapon_index)))
		return NONE;
	for (i = 0; i < NUMBER_OF_BOT_WEAPONS; i++)
	{
		if (bots.weapon_definitions[i] != NONE && bots.weapon_definitions[i] == object->definition_index)
			return i;
	}
	return NONE;
}

static short weapon_score(
	long weapon_index)
{
	short entry = weapon_entry(weapon_index);

	return entry == NONE ? 0 : bot_weapons[entry].score;
}

/* ---------- the game's side */

void n64_bots_begin(
	int count,
	int skill,
	int teams)
{
	/* Release the previous map's path scratch before the map arena grows. */
	if (bots_path_state) free(bots_path_state);
	bots_path_state = NULL;
	csmemset(&bots, 0, sizeof(bots));
	bots.teams = teams != 0;
	bots.count = (short)PIN(count, 0, MAXIMUM_BOTS);
	bots.skill = (short)PIN(skill, 0, 3);
	bots.active = bots.count > 0;
	bots.random = n64_ticks() | 1;
	if (!bot_lobby_prepared) n64_bots_prepare_lobby();
	if (bots.active)
	{
		/* (the paths' last stage steers around nearby objects from the
		AI's own records of them, which the N64 has not got) */
		ai_debug.path_disable_obstacle_avoidance = TRUE;
	}
}

void n64_bots_stop(
	void)
{
	bots.active = FALSE;
	bots.count = 0;
	bots.search_owner = 0;
	if (bots_path_state)
	{
		free(bots_path_state);
		bots_path_state = NULL;
	}
}

int n64_bots_count(
	void)
{
	return bots.active ? bots.count : 0;
}

/* how often a bot's pose is made (objects.c, n64_far_ai_biped), in ticks:
every other with 4 to 6 bots, every third with 7 or 8 (the user's choice:
each biped costs ~2.7 ms of a tick; with ANIMATIONS HALF RATE counted on
its half-speed clock there: every fourth or sixth) */
int n64_bots_pose_interval(
	void)
{
	short count = bots.active ? bots.created : 0;

	return count >= 7 ? 3 : count >= 4 ? 2 : 1;
}

/* a bot's own (objects.c, n64_far_ai_biped): the count's rate near and in
view, or firing; less often far or out of view, least both (bot_detail).
With three or four views, every other tick at most */
int n64_bot_pose_interval(
	long unit_index)
{
	extern int n64_view_total;
	struct bot *bot = bot_of_unit(unit_index);
	int interval = n64_bots_pose_interval();

	if (bot && !bot->firing)
	{
		short detail = bot_detail(bot);

		if (detail == _bot_detail_reduced)
			interval = MAX(interval + 1, 2);
		else if (detail == _bot_detail_least)
			interval = MAX(interval * 2, 4);
	}
	if (n64_view_total >= 3 && interval < 2)
		interval = 2;
	return interval;
}

/* how often bots think: as their pose, or every fourth tick with
ANIMATIONS HALF RATE (n64_menu.c; the user's ask) -- its timers, turning
and chances count by the ticks since its last thought, so it plays the
same, slower to react */
int n64_bots_update_interval(
	void)
{
	extern int n64_half_animation_rate;

	return n64_half_animation_rate ? 4 : n64_bots_pose_interval();
}

/* bipeds.c's (biped_update): a bot's movement and its physics every other
tick, twice as far, with four bots or more (the user's ask: its physics was
the dearest part of a biped's tick after its pose); else every tick */
int n64_bot_move_interval(
	long unit_index)
{
	if (!bots.active || bots.created < 4)
		return 1;
	return bot_of_unit(unit_index) ? 2 : 1;
}

/* one bot's: twice that far from every player and in no view (it fights
the other bots out there; near or seen it stays as quick) */
static short bot_think_interval(
	struct bot *bot)
{
	short interval = (short)n64_bots_update_interval();

	if (bot_detail(bot) == _bot_detail_least)
		interval = (short)MAX(interval * 2, 4);
	return interval;
}

/* the bot a player is (its number, 0 on), or -1 */
int n64_bot_number(
	long player_index)
{
	short i;

	for (i = 0; bots.active && i < bots.created; i++)
	{
		if (bots.bots[i].player_index == player_index)
			return i;
	}
	return -1;
}

/* players_update_after_game's (players.c, HALO_N64): whether a player's
nearby objects are looked over this tick (what its action button would
do, the items walked over): a bot's only before a tick it thinks -- it
presses its buttons then -- the rest every tick. A tick's actions come at
its start, so the tick before n64_bots_actions' next thought */
boolean n64_bots_examine_turn(
	long player_index)
{
	int number;

	if (!bots.active || (number = n64_bot_number(player_index)) < 0)
		return TRUE;
	return game_time_get() + 1 >= bots.bots[number].next_think;
}

/* main.c's create_local_players, after the players on the controllers:
the bots, on the team with fewer players in a team game */
void n64_bots_create_players(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	int human_teams[4], human_count = 0;
	short i;

	bots.created = 0;
	for (i = 0; i < MAXIMUM_BOTS; i++) bot_ghost_spawn_units[i] = NONE;
	/* The native map has loaded now; keep this 32 KiB scratch outside the
	arena, rather than pinning its released tail before a larger map loads. */
	if (bots.active && !bots_path_state)
		bots_path_state = (struct path_state *)malloc(sizeof(struct path_state));
	if (!bots.active)
		return;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL && human_count < 4)
		human_teams[human_count++] = player->network_player_data.team_index & 1;
	if (!bot_lobby_prepared) n64_bots_prepare_lobby();
	for (i = 0; i < bots.count; i++)
	{
		struct network_player network_player;
		struct bot *bot = &bots.bots[bots.created];
		char name[12];
		short c, team = (short)n64_bot_lobby_team(i, human_teams, human_count);
		short ghost_profile = (short)n64_ghost_profile(bot_lobby_seed ^ UINT32_C(0xc8013ea4), i, bots.count);
		long player_index;

		csmemset(&network_player, 0, sizeof(network_player));
		/* The native profile is bounded to eleven characters; the scoreboard
		uses the complete Ghost name through n64_bots_display_name. */
		snprintf(name, sizeof(name), "%s", ghost_profile == N64_GHOST_CHAOTIC ? "ChaoticSP1" :
			ghost_profile == N64_GHOST_KODE ? "KodeBrkr16" : n64_bot_lobby_name(i, bots.count));
		for (c = 0; name[c] && c < 11; c++)
			network_player.name[c] = (wchar_t)name[c];
		/* (in a team game the team's colour, as the menu gives the players) */
		network_player.primary_color_index = bots.teams ? (team ? 3 : 2) :
			ghost_profile == N64_GHOST_CHAOTIC ? 6 : ghost_profile == N64_GHOST_KODE ? 12 : bot_colors[i];
		/* (a machine holds four players: the bots on machines 1 to 3) */
		network_player.machine_index = (char)(1 + i / 4);
		network_player.controller_index = (char)(i % 4);
		network_player.team_index = (char)team;
		network_player.player_list_index = (char)(4 + i);
		player_index = player_new(1 + i / 4, NONE, NONE, &network_player);
		if (player_index == NONE)
			break;
		csmemset(bot, 0, sizeof(*bot));
		bot->ghost_profile = ghost_profile;
		bot->player_index = player_index;
		bot->unit_index = NONE;
		bot->target_unit = NONE;
		bot->goal_weapon = NONE;
		bot->given_up_weapon = NONE;
		bots.created++;
	}
	{
		char line[64];

		snprintf(line, sizeof(line), "n64: %d bots (skill %d)", bots.created, bots.skill);
		n64_log(line);
	}
}

/* the map's weapons, once its game has started */
static void resolve_weapons(
	void)
{
	short i;

	if (bots.resolved)
		return;
	bots.resolved = TRUE;
	for (i = 0; i < NUMBER_OF_BOT_WEAPONS; i++)
		bots.weapon_definitions[i] = tag_loaded('weap', bot_weapons[i].name);
}

/* ---------- thinking */

/* the nearest enemy's living unit: another team's player in a team game,
any other player otherwise */
static long nearest_enemy(
	struct bot *bot,
	real_point3d const *from)
{
	struct player_datum *self = player_get(bot->player_index);
	boolean teams = game_engine_has_teams();
	struct data_iterator iterator;
	struct player_datum *player;
	long best = NONE;
	real best_distance = 1e9f;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		real d;

		if (iterator.datum_index == bot->player_index || !unit_alive(player->unit_index))
			continue;
		if (teams && player->team_index == self->team_index)
			continue;
		d = horizontal_distance(from, &object_get(player->unit_index)->object.position);
		/* (the one it is fighting stays a little nearer than it is) */
		if (player->unit_index == bot->target_unit)
			d -= 3.f;
		if (d < best_distance)
		{
			best_distance = d;
			best = player->unit_index;
		}
	}
	return best;
}

/* the search going on: this tick's part, and once it is over the path to
its bot (a bot that died or changed bodies since gives it up); the next
search may begin TICKS_BETWEEN_PATHS after */
static void bots_search_step(
	void)
{
	struct bot *bot;
	boolean found, complete, built;
	unsigned long profile_start;

	if (!bots.search_owner || !bots_path_state)
		return;
	bot = &bots.bots[bots.search_owner - 1];
	if (bot->unit_index != bots.search_unit || !unit_alive(bot->unit_index))
	{
		bots.search_owner = 0;
		bots.path_cooldown = TICKS_BETWEEN_PATHS;
		return;
	}
	profile_start = bot_profile_start();
	complete = path_state_find_continue(bots_path_state, SEARCH_SURFACES_PER_TICK, &found);
	bot_profile_end(2, profile_start);
	if (!complete) return;
	bots.search_owner = 0;
	bots.path_cooldown = TICKS_BETWEEN_PATHS;
	profile_start = bot_profile_start();
	built = path_state_build_path(bots_path_state, &bot->path);
	bot_profile_end(3, profile_start);
	if (built && bot->path.step_count > 0)
	{
		bot->path_valid = TRUE;
		bot->path.step_index = 0;
		bot->path_age = 0;
		bot->path_goal = bots.search_goal;
	}
}

/* a path to the target, if this tick still has one to find: Halo's own
over the collision BSP's surfaces; partial paths are kept */
static void plan_path(
	struct bot *bot,
	long target_unit)
{
	struct path_input input;
	real_point3d start, goal;
	long start_surface, goal_surface;
	boolean search = !bots.paths_this_tick && bots.path_cooldown <= 0 && !bots.search_owner;
	boolean direct = bot->direct_wait <= 0 && bots.direct_tests_this_tick < DIRECT_TESTS_PER_TICK;

	/* (its own search still going on: it waits for that) */
	if (!bots_path_state || (!search && !direct) || bots.search_owner == (short)(bot - bots.bots) + 1)
		return;
	start_surface = biped_find_pathfinding_surface_index(bot->unit_index, &start);
	goal_surface = biped_find_pathfinding_surface_index(target_unit, &goal);
	if (start_surface == NONE || goal_surface == NONE)
	{
		if (search)
		{
			bots.paths_this_tick++;
			bots.path_cooldown = TICKS_BETWEEN_PATHS;
			bot->path_age = 0;
			bot->path_valid = FALSE;
		}
		return;
	}
	/* Diagnostic A/B gate: keep search fallback, never return a cached
	geometry result. No limit is active unless the private flag is set. */
	(void)bot_profile_start();
	if (direct && bot_direct_limit > 0)
	{
		real dx = goal.x - start.x, dy = goal.y - start.y;
		if (dx * dx + dy * dy > (real)bot_direct_limit * bot_direct_limit)
			direct = FALSE;
	}
	/* a straight walk there first: one test and no search where it's clear
	(most of the open maps; the search's smoothing would make the same one
	step of it), not counted against the searches */
	if (direct)
	{
		bots.direct_tests_this_tick++;
		if (n64_ai_direct_path(&start, start_surface, &goal, goal_surface, &bot->path))
		{
			bot->path_valid = TRUE;
			bot->path_age = 0;
			bot->path_goal = goal;
			return;
		}
		bot->direct_wait = DIRECT_PATH_RETRY_TICKS;
	}
	if (!search)
		return;
	bots.paths_this_tick++;
	bots.path_cooldown = TICKS_BETWEEN_PATHS;
	bot->path_age = 0;
	bot->path_valid = FALSE;
	path_input_new(&input, 0.3f, FALSE, bot->unit_index);
	path_input_set_start(&input, &start, start_surface);
	/* (Halo's own bound for its AI's searches: a way much longer than the
	straight line is not worth the search -- the closest point instead) */
	path_input_set_search_bounds(&input, 2.5f * distance3d(&start, &goal) + 15.f);
	path_state_new(&input, bots_path_state, NULL);
	path_state_destination(bots_path_state, &goal, goal_surface, 40.f);
	/* (in parts, bots_search_step; the next search waits for its end) */
	if (!path_state_find_begin(bots_path_state))
		return;
	bots.search_owner = (short)(bot - bots.bots) + 1;
	bots.search_unit = bot->unit_index;
	bots.search_goal = goal;
	bots_search_step();
}

/* the way along the path from here (horizontal, unit length); FALSE when
the path is used up */
static boolean follow_path(
	struct bot *bot,
	real_point3d const *position,
	real_vector2d *direction)
{
	struct path_result *path = &bot->path;

	while (bot->path_valid && path->step_index < path->step_count)
	{
		real_point3d const *point = &path->steps[path->step_index].point;
		real dx = point->x - position->x, dy = point->y - position->y;
		real length = square_root(dx * dx + dy * dy);

		if (length > 0.6f)
		{
			direction->i = dx / length;
			direction->j = dy / length;
			return TRUE;
		}
		path->step_index++;
	}
	bot->path_valid = FALSE;
	return FALSE;
}

/* a better weapon than the worse of its two lying near enough and in
sight, or NONE */
static long find_better_weapon(
	struct bot *bot,
	struct unit_datum *unit,
	real_point3d const *eye)
{
	struct object_iterator iterator;
	struct object_datum *object;
	short carried = 0, worst = 99, i;
	long best = NONE, second = NONE;
	real best_value = 0.f, second_value = 0.f;

	for (i = 0; i < MAXIMUM_WEAPONS_PER_UNIT; i++)
	{
		long weapon_index = unit_inventory_get_weapon(bot->unit_index, i);

		if (weapon_index != NONE)
		{
			short score = weapon_can_be_fired(weapon_index) ? weapon_score(weapon_index) : 0;

			carried++;
			if (score < worst)
				worst = score;
		}
	}
	if (carried < 2)
		worst = 0;
	object_iterator_new(&iterator, _object_mask_weapon, 0);
	while ((object = object_iterator_next(&iterator)) != NULL)
	{
		long weapon_index = iterator.index;
		short score;
		real distance, value;

		if (object->object.parent_object_index != NONE || weapon_index == bot->given_up_weapon)
			continue;
		score = weapon_score(weapon_index);
		if (score <= worst || !weapon_can_be_fired(weapon_index))
			continue;
		distance = horizontal_distance(&unit->object.position, &object->object.position);
		if (distance > WEAPON_SEARCH_RANGE || fabsf(object->object.position.z - unit->object.position.z) > 2.5f)
			continue;
		value = (real)(score - worst) - distance * 0.25f;
		if (value > best_value)
		{
			second = best;
			second_value = best_value;
			best = weapon_index;
			best_value = value;
		}
		else if (value > second_value)
		{
			second = weapon_index;
			second_value = value;
		}
	}
	/* (a line of sight for the two it wants most, no more) */
	for (i = 0; i < 2; i++)
	{
		long weapon_index = i ? second : best;
		real_point3d point;

		if (weapon_index == NONE)
			break;
		point = object_get(weapon_index)->object.position;
		point.z += 0.15f;
		if (n64_ai_clusters_may_see(bot->unit_index, weapon_index) && line_of_sight(eye, &point, bot->unit_index))
			return weapon_index;
	}
	return NONE;
}

/* the bot's action for a tick: it thinks every step ticks (its timers
count by it) */
/* Reset exactly once for a new native unit, keeping its match profile. */
static void bot_begin_life(
	struct bot *bot,
	struct player_datum *player,
	struct unit_datum *unit)
{
	/* a new life: a fresh state */
	if (bot->unit_index != player->unit_index)
	{
		long player_index = bot->player_index;
		short ghost_profile = bot->ghost_profile;

		csmemset(bot, 0, sizeof(*bot));
		bot->ghost_profile = ghost_profile;
		bot->player_index = player_index;
		bot->unit_index = player->unit_index;
		bot->target_unit = NONE;
		bot->goal_weapon = NONE;
		bot->given_up_weapon = NONE;
		bot->aim = unit->object.forward;
		bot->last_position = unit->object.position;
		/* One independent roll per native unit life. Native powerup expiry,
		firing and damage still control how much camouflage remains. */
		/* The action gate may clear a thought state while a game is paused.
		A remembered native datum prevents that from granting a second roll. */
		int number = (int)(bot - bots.bots);
		if (number >= 0 && number < MAXIMUM_BOTS && bot_ghost_spawn_units[number] != player->unit_index)
		{
			bot_ghost_spawn_units[number] = player->unit_index;
			boolean cloaked = n64_ghost_camo_roll(ghost_profile, &bot_ghost_spawn_seed);
			if (cloaked)
			{
				player_handle_powerup_minor(player_index, _player_powerup_active_camouflage,
					60 * TICKS_PER_SECOND);
				unit->unit.active_camouflage = 1.f;
			}
			if (ghost_profile && n64_debug_flag("debug_bot_ghosts"))
			{
				char line[160];
				snprintf(line, sizeof(line), "n64: ghost %s slot %d unit %08lx camo roll %lu granted %d ticks %d",
					n64_ghost_name(ghost_profile), number, (unsigned long)player->unit_index,
					(unsigned long)(bot_ghost_spawn_seed % 100), cloaked,
					player->powerup_durations[_player_powerup_active_camouflage]);
				n64_log(line);
			}
		}
	}
}

static void think(
	struct bot *bot,
	struct player_datum *player,
	struct player_action *action,
	short step)
{
	struct unit_datum *unit = unit_get(player->unit_index);
	real_point3d eye, target_eye, target_position;
	real_vector3d desired;
	real_vector2d move = { 0.f, 0.f }; /* the way to go, in the world */
	real forward = 0.f, strafe = 0.f, trigger = 0.f;
	real distance = 1e9f;
	long current_weapon;
	short entry;
	/* Chaotic's 3DS accuracy advantage uses the N64's native Legendary
	combat parameters, without fake damage, speed or health changes. */
	short combat_skill = bot->ghost_profile == N64_GHOST_CHAOTIC ? 3 : bots.skill;
	boolean moving = FALSE, along_path = FALSE, in_range = FALSE;
	unsigned long flags = 0;
	struct bot_weapon const *weapon;
	static struct bot_weapon const unarmed = { "", 0, _style_semi, 0.f, 1.5f };

	bot_begin_life(bot, player, unit);
	unit_get_camera_position(player->unit_index, &eye);

	/* in a vehicle: out (the action button) */
	if (unit->object.parent_object_index != NONE)
	{
		if (bot->action_timer > 0)
			bot->action_timer -= step;
		else
		{
			flags |= FLAG(_unit_control_action_bit);
			bot->action_timer = TICKS_PER_SECOND;
		}
		action->control_flags = flags;
		euler_angles2d_from_vector3d(&action->desired_facing, &bot->aim);
		return;
	}

	current_weapon = unit_inventory_get_weapon(player->unit_index, unit->unit.current_weapon_index);
	entry = weapon_entry(current_weapon);
	weapon = entry == NONE ? &unarmed : &bot_weapons[entry];

	/* whom to fight (now and then), and can it see them (a few times a
	second) */
	if ((bot->choose_timer -= step) <= 0 || !unit_alive(bot->target_unit))
	{
		long target = nearest_enemy(bot, &unit->object.position);

		bot->choose_timer = 20;
		if (target != bot->target_unit)
		{
			bot->target_unit = target;
			bot->sight_timer = 0;
			bot->path_valid = FALSE;
			bot->last_seen.x = bot->last_seen.y = bot->last_seen.z = 0.f;
		}
	}
	if (bot->target_unit != NONE)
	{
		unit_get_camera_position(bot->target_unit, &target_eye);
		target_position = object_get(bot->target_unit)->object.position;
		if ((bot->sight_timer -= step) <= 0)
		{
			bot->sight_timer = 10;
			bot->target_visible =
				horizontal_distance(&unit->object.position, &target_position) < SIGHT_RANGE &&
				n64_ai_clusters_may_see(player->unit_index, bot->target_unit) &&
				line_of_sight(&eye, &target_eye, player->unit_index);
		}
		if (bot->target_visible || (bot->last_seen.x == 0.f && bot->last_seen.y == 0.f))
			bot->last_seen = target_eye;
		distance = horizontal_distance(&unit->object.position, &target_position);
		in_range = bot->target_visible && distance <= weapon->far_range + 2.f;
	}
	else
		bot->target_visible = FALSE;

	/* a better weapon lying near, while there is no fight in sight */
	if (bot->direct_wait > 0)
		bot->direct_wait -= step;
	if (bot->given_up_timer > 0 && (bot->given_up_timer -= step) <= 0)
	{
		bot->given_up_timer = 0;
		bot->given_up_weapon = NONE;
	}
	if (bot->target_visible && distance < 12.f)
		bot->goal_weapon = NONE;
	else if ((bot->weapon_timer -= step) <= 0)
	{
		bot->weapon_timer = WEAPON_SEARCH_TICKS;
		if (bot->goal_weapon == NONE)
		{
			bot->goal_weapon = find_better_weapon(bot, unit, &eye);
			bot->goal_ticks = 0;
		}
	}
	if (bot->goal_weapon != NONE)
	{
		struct object_datum *object = object_try_and_get(bot->goal_weapon);

		if (!object || object->object.parent_object_index != NONE ||
			!TEST_FLAG(_object_mask_weapon, object->object.type))
		{
			bot->goal_weapon = NONE;
		}
		else if ((bot->goal_ticks += step) > 6 * TICKS_PER_SECOND)
		{
			/* (out of its reach after all) */
			bot->given_up_weapon = bot->goal_weapon;
			bot->given_up_timer = WEAPON_GIVE_UP_TICKS;
			bot->goal_weapon = NONE;
		}
		else
		{
			real dx = object->object.position.x - unit->object.position.x;
			real dy = object->object.position.y - unit->object.position.y;
			real length = square_root(dx * dx + dy * dy);

			if (length > 0.01f)
			{
				move.i = dx / length;
				move.j = dy / length;
			}
			moving = length > 0.35f;
			/* there: the action button picks it up (it faces it too) */
			if (length < 1.1f)
			{
				if (bot->action_timer > 0)
					bot->action_timer -= step;
				else
				{
					flags |= FLAG(_unit_control_action_bit);
					bot->action_timer = 20;
				}
			}
		}
	}

	/* where to go: along a path while the enemy is out of sight or out of
	range; in range, keep the weapon's range and move sideways */
	if (bot->goal_weapon == NONE && bot->target_unit != NONE)
	{
		if (!in_range)
		{
			bot->path_age += step;
			if (!bot->path_valid || bot->path_age > PATH_LIFETIME ||
				horizontal_distance(&bot->path_goal, &target_position) > 7.f)
			{
				plan_path(bot, bot->target_unit);
			}
			if (follow_path(bot, &unit->object.position, &move))
				along_path = TRUE;
			else
			{
				real dx = target_position.x - unit->object.position.x;
				real dy = target_position.y - unit->object.position.y;
				real length = square_root(dx * dx + dy * dy);

				if (length > 0.01f)
				{
					move.i = dx / length;
					move.j = dy / length;
				}
			}
			moving = TRUE;
		}
	}

	/* where to look: at the enemy in sight (missing by the skill's error),
	else the way it is going */
	if (bot->target_unit != NONE && (bot->target_visible || (!along_path && bot->goal_weapon == NONE)))
	{
		desired.i = bot->last_seen.x - eye.x;
		desired.j = bot->last_seen.y - eye.y;
		desired.k = bot->last_seen.z - eye.z - 0.12f;
		normalize(&desired);
		if (bot->target_visible)
		{
			real error = aim_error[combat_skill];

			desired.i += error * bot_random_real();
			desired.j += error * bot_random_real();
			desired.k += error * 0.6f * bot_random_real();
			normalize(&desired);
		}
	}
	else if (move.i != 0.f || move.j != 0.f)
	{
		desired.i = move.i;
		desired.j = move.j;
		desired.k = bot->goal_weapon != NONE ? -0.35f : 0.f;
		normalize(&desired);
	}
	else
		desired = bot->aim;
	/* turning: quick but not instant (half the way a tick) */
	{
		real turn = step >= 3 ? 0.875f : step == 2 ? 0.75f : 0.5f;

		bot->aim.i += (desired.i - bot->aim.i) * turn;
		bot->aim.j += (desired.j - bot->aim.j) * turn;
		bot->aim.k += (desired.k - bot->aim.k) * turn;
	}
	normalize(&bot->aim);

	if (moving)
	{
		/* the way to go, relative to where it faces (the throttle: i
		forward, j left) */
		real fi = bot->aim.i, fj = bot->aim.j;
		real length = square_root(fi * fi + fj * fj);

		if (length > 0.01f)
		{
			fi /= length;
			fj /= length;
		}
		forward = move.i * fi + move.j * fj;
		strafe = move.i * -fj + move.j * fi;
	}
	else if (in_range)
	{
		if (distance < weapon->near_range)
			forward = -0.6f;
		else if (distance > weapon->far_range)
			forward = 0.6f;
		if ((bot->strafe_timer -= step) <= 0)
		{
			bot->strafe_timer = (short)(20 + bot_random() % 45);
			bot->strafe_direction = (short)(bot_random() & 1 ? 1 : -1);
		}
		strafe = 0.8f * bot->strafe_direction;
	}

	/* stuck (moving and getting nowhere): around for a while, a jump, and
	a new path */
	if (bot->unstick_timer > 0)
	{
		bot->unstick_timer -= step;
		forward = bot->unstick_throttle.i;
		strafe = bot->unstick_throttle.j;
	}
	else if ((bot->stuck_check -= step) <= 0)
	{
		bot->stuck_check = 16;
		real moved = horizontal_distance(&unit->object.position, &bot->last_position);

		bot->last_position = unit->object.position;
		if ((forward != 0.f || strafe != 0.f) && moved < 0.25f)
		{
			if (++bot->stuck_timer >= 2)
			{
				bot->stuck_timer = 0;
				bot->unstick_timer = (short)(16 + bot_random() % 20);
				bot->unstick_throttle.i = bot_random_real() < 0.f ? -0.5f : 0.4f;
				bot->unstick_throttle.j = bot_random() & 1 ? 1.f : -1.f;
				bot->jump_timer = 1;
				bot->path_valid = FALSE;
			}
		}
		else
			bot->stuck_timer = 0;
	}
	if (bot->jump_timer > 0)
	{
		bot->jump_timer -= step;
		flags |= FLAG(_unit_control_jump_bit);
	}

	/* its weapon: reloaded when empty, the other one when this one is
	spent */
	if (bot->reload_timer > 0)
		bot->reload_timer -= step;
	if (bot->swap_timer > 0)
		bot->swap_timer -= step;
	if (current_weapon != NONE)
	{
		struct weapon_datum *weapon_datum = weapon_get(current_weapon);
		short loaded = weapon_datum->weapon.magazines[0].rounds_loaded;
		short total = weapon_datum->weapon.magazines[0].rounds_total;

		if (!weapon_can_be_fired(current_weapon) && bot->swap_timer <= 0)
		{
			short i;

			for (i = 0; i < MAXIMUM_WEAPONS_PER_UNIT; i++)
			{
				long other = unit_inventory_get_weapon(player->unit_index, i);

				if (other != NONE && other != current_weapon && weapon_can_be_fired(other))
				{
					flags |= FLAG(_unit_control_swap_weapons_bit);
					bot->swap_timer = TICKS_PER_SECOND;
					break;
				}
			}
		}
		else if (!loaded && total > 0 && bot->reload_timer <= 0)
		{
			flags |= FLAG(_unit_control_weapon_reload_bit);
			bot->reload_timer = TICKS_PER_SECOND;
		}
	}

	/* firing: bursts and pauses while the enemy is in sight and range (an
	explosive one never up close) */
	if (bot->target_visible && distance < weapon->far_range + 6.f && current_weapon != NONE &&
		!(weapon->style == _style_explosive && distance < 4.f) && bot->swap_timer <= 0)
	{
		if (bot->fire_timer > 0)
		{
			bot->fire_timer -= step;
			bot->trigger_down = weapon->style == _style_auto ? TRUE : !bot->trigger_down;
			if (bot->trigger_down)
			{
				trigger = 1.f;
				flags |= FLAG(_unit_control_weapon_primary_trigger_bit);
			}
			/* (longer pauses with many bots: fewer shots in the air, each one
			an object of its own a tick, its ray every tick -- the count's
			share of a pause: 1 up to 3 bots, 1.5 to 6, 2 to 8, 2.5 to 10, 3
			past; half again for a bot far and in no view, which fights the
			other bots out there) */
			if (bot->fire_timer <= 0)
			{
				short halves = bots.created <= 3 ? 2 : bots.created <= 6 ? 3 : bots.created <= 8 ? 4 :
					bots.created <= 10 ? 5 : 6;

				if (bot_detail(bot) == _bot_detail_least)
					halves += halves / 2;
				bot->fire_timer = -(short)((pause_ticks[combat_skill] + bot_random() % 10) * halves / 2);
			}
		}
		else
		{
			bot->fire_timer += step;
			bot->trigger_down = FALSE;
			/* (a quarter shorter with more than eight bots) */
			if (bot->fire_timer >= 0)
				bot->fire_timer = (short)((burst_ticks[combat_skill] + bot_random() % 8) *
					(bots.created > 8 ? 3 : 4) / 4);
		}
	}
	else
		bot->trigger_down = FALSE;

	/* melee up close */
	if (bot->melee_timer > 0)
		bot->melee_timer -= step;
	else if (bot->target_visible && distance < 1.6f)
	{
		flags |= FLAG(_unit_control_use_equipment_bit);
		bot->melee_timer = 30;
	}
	/* a grenade now and then */
	if (bot->grenade_timer > 0)
		bot->grenade_timer -= step;
	else if (bot->target_visible && distance > 6.f && distance < 18.f &&
		(long)(bot_random() % 1000) < grenade_chance[bots.skill] * step)
	{
		/* (the grenade button sets both, as player_control.c does) */
		flags |= FLAG(_unit_control_throw_grenade_bit) | FLAG(_unit_control_weapon_secondary_trigger_bit);
		bot->grenade_timer = 6 * TICKS_PER_SECOND;
	}

	if (forward > 1.f) forward = 1.f;
	if (forward < -1.f) forward = -1.f;
	if (strafe > 1.f) strafe = 1.f;
	if (strafe < -1.f) strafe = -1.f;
	action->control_flags = flags;
	euler_angles2d_from_vector3d(&action->desired_facing, &bot->aim);
	action->throttle.i = forward;
	action->throttle.j = strafe;
	action->primary_trigger = trigger;
}

/* update_client_dequeue's (player_queues_new.c, HALO_N64): a tick's
actions, by player datum's absolute index -- the bots' written over */
void n64_bots_actions(
	struct player_action *actions)
{
	/* (the presses a held action does not repeat between thoughts) */
	unsigned long const pressed =
		FLAG(_unit_control_action_bit) | FLAG(_unit_control_jump_bit) |
		FLAG(_unit_control_use_equipment_bit) | FLAG(_unit_control_throw_grenade_bit) |
		FLAG(_unit_control_weapon_secondary_trigger_bit) | FLAG(_unit_control_weapon_reload_bit) |
		FLAG(_unit_control_swap_weapons_bit);
	long now = game_time_get();
	short i;

	if (!bots.active || !bots.created)
		return;
	(void)bot_profile_start();
	resolve_weapons();
	bots.paths_this_tick = 0;
	bots.direct_tests_this_tick = 0;
	if (bots.path_cooldown > 0)
		bots.path_cooldown--;
	/* (the search going on: this tick's part) */
	bots_search_step();
	for (i = 0; i < bots.created; i++)
	{
		struct bot *bot = &bots.bots[i];
		struct player_datum *player = (struct player_datum *)datum_try_and_get(player_data, bot->player_index);
		struct player_action *action;

		if (!player)
			continue;
		action = &actions[DATUM_INDEX_TO_ABSOLUTE_INDEX(bot->player_index)];
		csmemset(action, 0, sizeof(*action));
		action->desired_weapon_index = NONE;
		action->desired_grenade_index = NONE;
		action->desired_zoom_level = NONE;
		action->desired_facing.yaw = 0.f;
		action->desired_facing.pitch = 0.f;
		if (!game_in_progress() || !game_engine_running() || !unit_alive(player->unit_index))
		{
			bot->unit_index = NONE;
			continue;
		}
		/* its own tick to think (bot_think_interval, staggered between
		the bots), or a new life: a thought, its timers counting the ticks
		since the last; else its last action held, presses not again */
		if (bot->unit_index != player->unit_index)
		{
			short interval = (short)n64_bots_update_interval();

			think(bot, player, action, interval);
			bot->last_action = *action;
			bot->last_think = now;
			bot->next_think = now + interval + i % interval;
		}
		else if (now >= bot->next_think)
		{
			think(bot, player, action, (short)PIN(now - bot->last_think, 1, MAXIMUM_THINK_STEP));
			bot->last_action = *action;
			bot->last_think = now;
			bot->next_think = now + bot_think_interval(bot);
		}
		else
		{
			*action = bot->last_action;
			action->control_flags &= ~pressed;
		}
		bot->firing = (action->control_flags & (FLAG(_unit_control_weapon_primary_trigger_bit) |
			FLAG(_unit_control_throw_grenade_bit) | FLAG(_unit_control_use_equipment_bit))) != 0;
	}
	bot_profile_poll();
}
