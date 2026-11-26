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
as it does a person's: the swap button (a person's held X) picks up a
weapon (player_handle_weapon_swap), the action button gets out of a
vehicle; the weapon wanted, triggers, grenades, melee, jump and reload go
to unit_control.
A bot charges the plasma pistol now and then, as Halo's jackals do.

Their way of fighting is the Firefight Covenant's (n64_firefight.c), kept
simple: each picks the nearest enemy (another team's player in a team game,
anyone else otherwise), goes for it along Halo's own paths while it is out
of sight, keeps its weapon's range and strafes when in sight, fires in
bursts (held for the automatic weapons, pressed shot by shot for the rest,
the plasma pistol at the pace of Halo's own) with an aim that misses by
more at an easier skill, throws grenades now and then (never at an enemy
beside a teammate), meleees up close and works its way around whatever
stops it. With no enemy in sight it goes for a better weapon lying near,
and it reloads and swaps to its other weapon when this one is empty. It
drives a vehicle near with no driver, and in a team game rides with a
teammate driver while the vehicle is almost still (vehicles section below). It sees
and fires along a clear line
to the enemy: the level, vehicles, scenery and machines block it; bipeds,
teammates included, do not (Halo's sight-blocking objects).
A cloaked enemy it sees only within 2 units, or for 1 s after it fires.
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
#include "units/unit_definitions.h"
#include "units/bipeds.h"
#include "units/vehicles.h"
#include "units/vehicle_definitions.h"
#include "ai/path.h"
#include "ai/path_structure_bsp.h"
#include "ai/ai_debug.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "items/items.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "scenario/scenario_definitions.h"
#include "game/game_engine_king.h"
#include "math/geometry.h"

#include "../src/n64.h"

#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "n64_bot_names.h"
#include "n64_bot_ghosts.h"

/* ---------- constants */

#define MAXIMUM_BOTS 15
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
/* vehicles (world units, ticks): how far a bot looks for one to drive, how
far a teammate's it rides may be (Halo's AI: a friendly player driving
within 10 units, actions.c), how often it looks (Halo's 45 ticks), how long
it tries to get in, how long one it gave up on is left alone */
#define VEHICLE_SEARCH_RANGE 20.f
#define VEHICLE_RIDE_RANGE 10.f
#define VEHICLE_SEARCH_TICKS 45
#define VEHICLE_ENTRY_TICKS (12 * TICKS_PER_SECOND)
#define VEHICLE_GIVE_UP_TICKS (6 * TICKS_PER_SECOND)
/* an approach that gets no nearer its door (by a unit) this long is given
up; after it, no seat of that goal's kind is chosen this long on foot */
#define VEHICLE_PROGRESS_TICKS (3 * TICKS_PER_SECOND)
#define VEHICLE_RETRY_TICKS (25 * TICKS_PER_SECOND)
/* a sideways stick counts towards a stuck event only under this way driven
in a check (30 ticks of thoughts or more; a Ghost strafing freely drives
some 0.8-1.6 units a second, its net move short when its strafe turns back) */
#define DRIVE_STRAFE_BLOCK 0.75f
/* no new driver goal while this many bots drive or have one: a moving vehicle's
physics and pose are the dearest part of a bot's driving */
#define MAXIMUM_BOT_DRIVERS 2
/* objectives (the objectives section): a goal that gets no unit nearer
this long (its thoughts) is left for a while */
#define OBJECTIVE_SHARE_TICKS 30
#define OBJECTIVE_PROGRESS_TICKS (3 * TICKS_PER_SECOND)
#define OBJECTIVE_PAUSE_TICKS (10 * TICKS_PER_SECOND)

enum
{
	_style_semi, /* pressed shot by shot */
	_style_auto, /* held */
	_style_explosive, /* pressed, never up close */
	_style_pistol, /* the plasma pistol: pressed at a pace, held to charge */
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
	{ "weapons\\plasma pistol\\plasma pistol", 3, _style_pistol, 3.f, 12.f },
};
#define NUMBER_OF_BOT_WEAPONS ((short)(sizeof(bot_weapons) / sizeof(bot_weapons[0])))

/* by skill (easy, normal, heroic, legendary) */
static real const aim_error[4] = { 0.16f, 0.09f, 0.055f, 0.03f };
static short const burst_ticks[4] = { 8, 12, 16, 22 };
static short const pause_ticks[4] = { 34, 22, 14, 8 };
/* the plasma pistol as a30's plasma-pistol actors fire it (their actor
variants: grunt minor at easy, jackal minor at normal, grunt major at
heroic, jackal major at legendary): a press each so many ticks (its
trigger has no rate of fire of its own, its bolt going as it is let go:
rate_of_fire 5 a second at easy and normal, 6 at heroic and legendary),
its bursts and the pauses between them (burst_duration and burst_
separation, in ticks) */
static struct bot_pistol
{
	short shot_ticks;
	short burst_least, burst_most;
	short pause_least, pause_most;
} const pistol_fire[4] =
{
	{ 6, 8, 21, 36, 51 }, /* 0.25-0.7 s, 1.2-1.7 s */
	{ 6, 12, 26, 45, 60 }, /* 0.4-0.85 s, 1.5-2 s */
	{ 5, 5, 17, 23, 33 }, /* 0.175-0.55 s, 0.75-1.1 s */
	{ 5, 9, 18, 15, 23 } /* 0.3-0.6 s, 0.5-0.75 s */
};
/* (in 100: the jackals' special_fire_chance, 0.2 minor and 0.3 major, a
roll each special_fire_delay of 5 s, overcharge their special_fire_mode) */
static short const charge_chance[4] = { 10, 20, 30, 30 };
/* (in 100, a roll a second: Halo's marines roll throw_grenade_chance 0.25
each throw_grenade_delay of 1 s, a30's actor variants) */
static short const grenade_chance[4] = { 10, 15, 20, 25 };

/* their armour in free-for-all games (Halo's colour indices: pink, orange,
purple, cyan, cobalt, brown, teal, sage, salmon, tan, maroon, white) */
static short const bot_colors[MAXIMUM_BOTS] = { 7, 11, 8, 9, 10, 14, 12, 13, 17, 15, 16, 0,
	/* (past twelve bots there are three players or fewer: black, grey,
	and the fourth player's green) */
	1, 4, 6 };

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
	/* the plasma pistol (bot_trigger_tick): its next press (a game tick)
	and the ticks between presses while it fires (0: not firing it); the
	time to its next roll to charge it, and whether it holds the trigger
	to charge it */
	long next_shot;
	short shot_ticks;
	short charge_timer;
	boolean charging;
	/* vehicles (the vehicles section): the one it goes for, the seat and
	its role there, how long it has tried; one it gave up on and for how long
	more; its next look for one; one it left flipped, to turn back over */
	long goal_vehicle;
	long given_up_vehicle;
	long flip_vehicle;
	short goal_seat, goal_role;
	short vehicle_ticks, given_up_vehicle_timer, vehicle_timer, flip_timer;
	/* in a seat: its role there; how long the vehicle has been flipped or
	without its driver, how long its target has been out of the vehicle's
	reach; how long it waits for a teammate to ride; its stuck checks (where
	it was, the next check, the ones in a row, the stuck spells lately and
	how long they count), its time backing up, the side it circles on */
	short seat_role;
	short upset_ticks, reach_ticks, wait_ticks;
	short drive_check, drive_stuck, stuck_events, stuck_window, reverse_timer, detour_timer, circle;
	real_point3d drive_position;
	/* the door of the seat it goes for, and when it is worked out again */
	real_point3d goal_door;
	short door_timer;
	/* its approach: the ticks since it last got a unit nearer the door or
	pressed at it, and its nearest distance to the door so far */
	short goal_still_ticks;
	real goal_best_distance;
	/* the vehicle and the seat it was in at its last thought (NONE on
	foot), whether or not it kept that seat: a change of vehicle is a seat
	change (bot_seat_changed); between thoughts, a change of either holds
	its stick and trigger (n64_bots_actions) */
	long seen_parent;
	short seen_seat;
	/* after a give-up: the on-foot ticks left before it chooses that kind
	of seat again (a driver's, a rider's) */
	short driver_pause, rider_pause;
	/* the way its vehicle drove since its last stuck check (its
	horizontal speed over its thoughts) */
	real drive_path;
	/* in a seat: the vehicle's kind and the seat's role, worked out when
	it sat in that seat of that vehicle (seat_vehicle, seat_index); its
	gun's way of firing (vehicle_guns), worked out when its gun
	(seat_weapon) changes */
	short seat_kind, seat_gun, seat_index;
	long seat_vehicle, seat_weapon;
	/* its objective (the objectives section): its role and its rank there
	(an escort's second, a second ball-goer), the unit it goes with, the
	goal point asked for last and the ground under it and the pathfinding surface there (NONE: not
	looked for, -2: none), the nearest it came, where it was and the ticks
	since it came a unit nearer or moved 2 units, a pause after a give-up; CTF: where it last knew its own
	flag (or its carrier) to be, when, and the carrier it saw (NONE: the flag itself), its next look; Oddball: where its
	holder runs to (flag_known) and when it chooses again */
	short role, role_rank;
	long role_unit;
	real_point3d role_point, role_ground;
	long role_surface;
	real role_best;
	real_point3d role_from;
	short role_still, role_pause;
	real_point3d flag_known;
	long flag_known_time, flag_known_unit;
	short flag_look, keep_timer;
	/* walking straight to its goal along a wall: how long more, to which
	side, and how long it has done so */
	short side_timer, side_turn, side_ticks;
	/* a returner: how long it has searched the place it knew */
	short search_ticks;
	/* King: whether it was near its hill's centre (where it never gives
	up) at its last thought */
	short hill_zone;
	/* a way up to a point above it that its search could not reach: 0
	none, 1 a search backward to make, 2 walking to the bend it found, 3
	tried; that bend and its surface */
	short approach_state;
	real_point3d approach;
	long approach_surface;
};

/* New LOD fields extend the old prefix; read diagnostic stride from DWARF.
(The vehicles' fields come after the rest: the stride grows, the old offsets
stay.) */
typedef char n64_bot_observer_stride[sizeof(struct bot) == 500 ? 1 : -1];
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
	/* (whether the search is to a point, and whether it goes backward:
	from a point above its bot to the bot) */
	boolean search_point, search_backward;
	long weapon_definitions[NUMBER_OF_BOT_WEAPONS];
	struct bot bots[MAXIMUM_BOTS];
} bots;

/* the pathfinder's working state (32,908 bytes, only while there are bots) */
static struct path_state *bots_path_state;

/* the game's objectives, as the bots read them (the objectives section):
the game type's engine if it has objectives the bots play (else none, as
in Slayer); CTF: each team's flag, its carrier, whether it is away from
home, what its team's bots can know of it (home, taken, lying where one
of them knows), its base point, the tick it last changed hands and the tick of the last
announcement about it (taken from home, home again); Oddball: the ball
(flag[0]) and its holder; the next share of roles */
static struct
{
	boolean ready;
	short engine;
	long flag[2], carrier[2];
	boolean away[2];
	short believed[2];
	real_point3d base[2];
	long changed[2], announced[2];
	long next_share;
} objectives;

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

/* least to most, both included */
static short bot_random_range(
	short least,
	short most)
{
	return (short)(least + (short)(bot_random() % (unsigned long)(most - least + 1)));
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
	real_point3d bot_position;
	short local_player_index;
	boolean seen;

	if (bot->detail_time == now)
		return bot->detail;
	bot->detail_time = now;
	if (!unit_alive(bot->unit_index))
		return bot->detail = _bot_detail_full;
	seen = n64_object_recently_seen(bot->unit_index);
	/* (where it truly is: in a vehicle its own position is its seat's) */
	object_get_origin(bot->unit_index, &bot_position);
	for (local_player_index = 0; local_player_index < MAXIMUM_LOCAL_PLAYERS; local_player_index++)
	{
		long player_index = local_player_get_player_index(local_player_index);
		long unit_index = player_index != NONE ? player_get(player_index)->unit_index : NONE;

		if (unit_index != NONE && object_try_and_get(unit_index))
		{
			real_point3d position;
			real d = distance_squared3d(object_get_origin(unit_index, &position), &bot_position);

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
sight-blocking object in between but the one it leaves out and the one it
takes as its end (a rider's vehicle, seen from another vehicle: the ray
leaves out the bot's own) */
static boolean line_of_sight_past(
	real_point3d const *from,
	real_point3d const *to,
	long ignore_object_index,
	long end_object_index)
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
	objects aren't tested -- collisions.c; with an end object, the nearest
	hit, which may be that object) */
	n64_collision_any_hit = end_object_index == NONE;
	blocked = collision_test_vector(
		FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit) |
			FLAG(_collision_test_objects_bit) | _collision_test_objects_sight_blocking_flags,
		from, &vector, ignore_object_index, &collision);
	n64_collision_any_hit = 0;
	bot_profile_end(0, profile_start);
	return !blocked || (end_object_index != NONE && collision.type == _collision_result_object &&
		collision.object_index == end_object_index);
}

static boolean line_of_sight(
	real_point3d const *from,
	real_point3d const *to,
	long ignore_object_index)
{
	return line_of_sight_past(from, to, ignore_object_index, NONE);
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
#ifdef HALO_N64_CAMPAIGN
	extern int n64_campaign_move_interval(long unit_index);

	/* (a campaign's actors: n64_campaign.c) */
	if (!bots.active)
		return n64_campaign_move_interval(unit_index);
#endif
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
	csmemset(&objectives, 0, sizeof(objectives));
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
		bot->goal_vehicle = NONE;
		bot->given_up_vehicle = NONE;
		bot->flip_vehicle = NONE;
		bot->role_unit = NONE;
		bot->role_surface = NONE;
		bot->flag_known_unit = NONE;
		bots.created++;
	}
	{
		char line[64];

		/* (and when the path search's memory did not fit: no paths then) */
		snprintf(line, sizeof(line), "n64: %d bots (skill %d)%s", bots.created, bots.skill,
			bots_path_state ? "" : ", no path memory");
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

/* whether a unit is hidden by its active camouflage from a bot so far
away (horizontally): Halo's AI never sees one cloaked over half
(actor_perception.c) but notices it when it fires (weapons.c makes the
shot known, actors.c); a bot sees one within 2 units, or for 1 s after it
fired */
static boolean bot_camouflaged(
	long unit_index,
	real distance)
{
	struct unit_datum *unit = unit_get(unit_index);
	long weapon_index;

	if (unit->unit.active_camouflage <= 0.5f || distance < 2.f)
		return FALSE;
	weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
	if (weapon_index != NONE)
	{
		long fired = weapon_get(weapon_index)->weapon.game_time_last_fired;

		if (fired > 0 && game_time_get() - fired <= TICKS_PER_SECOND)
			return FALSE;
	}
	return TRUE;
}

static long bot_hidden_enemy(struct bot const *bot); /* (the objectives section) */

/* the nearest enemy's living unit: another team's player in a team game,
any other player otherwise -- none it cannot see for its camouflage
(bot_camouflaged: it would otherwise go straight for it, its way found
from where it truly is) */
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
	/* (CTF: its own flag's carrier only where it knows it) */
	long hidden = bot_hidden_enemy(bot);

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		real_point3d position;
		real d;

		if (iterator.datum_index == bot->player_index || !unit_alive(player->unit_index))
			continue;
		if ((teams && player->team_index == self->team_index) || player->unit_index == hidden)
			continue;
		/* (a rider's own position is its seat's, in its vehicle: objects.c,
		object_get_origin) */
		d = horizontal_distance(from, object_get_origin(player->unit_index, &position));
		if (bot_camouflaged(player->unit_index, d))
			continue;
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

/* whether a teammate (a player of its team in a team game, not itself)
stands within 3 units of a point: no grenade there */
static boolean teammate_near(
	struct bot *bot,
	real_point3d const *point)
{
	struct player_datum *self = player_get(bot->player_index);
	struct data_iterator iterator;
	struct player_datum *player;

	if (!game_engine_has_teams())
		return FALSE;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		real_point3d position;

		if (iterator.datum_index == bot->player_index || player->team_index != self->team_index ||
			!unit_alive(player->unit_index))
		{
			continue;
		}
		if (distance_squared3d(object_get_origin(player->unit_index, &position), point) < 3.f * 3.f)
			return TRUE;
	}
	return FALSE;
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
	if (bots.search_backward)
	{
		/* (backward, from the point above: the bot walks first to the last
		bend of that way that is not its own place) */
		short k = (short)(bot->path.step_count - (bot->path.steps_finish_path ? 2 : 1));
		real_point3d position;

		bot->path_valid = FALSE;
		bot->approach_state = 3;
		object_get_origin(bot->unit_index, &position);
		if (built && k >= 0 && horizontal_distance(&bot->path.steps[k].point, &position) > 2.f)
		{
			bot->approach = bot->path.steps[k].point;
			bot->approach_surface = bot->path.steps[k].surface_index;
			bot->approach_state = 2;
			bot->role_best = 1e9f;
			bot->role_still = 0;
		}
	}
	else if (built && bots.search_point && bot->approach_state == 0 && bot->path.steps_finish_path &&
		bot->path.step_count > 0)
	{
		/* (a whole way to a point that ends short of it, under it -- its
		closest place, over a unit below it and within 6 units across: a
		search backward next; not for a point at its end's height, such as a
		flag room the path data leaves apart, whose way backward leads out) */
		real_point3d const *end = &bot->path.steps[bot->path.step_count - 1].point;

		if (horizontal_distance(end, &bots.search_goal) < 6.f && bots.search_goal.z - end->z > 1.f &&
			distance3d(&bots.search_goal, &bot->role_ground) < 0.5f)
			bot->approach_state = 1;
	}
}

/* a path to the target, if this tick still has one to find: Halo's own
over the collision BSP's surfaces; partial paths are kept */
static void plan_path(
	struct bot *bot,
	long target_unit,
	real_point3d const *goal_point,
	long goal_point_surface)
{
	struct path_input input;
	real_point3d start, goal;
	long start_surface, goal_surface;
	boolean search = !bots.paths_this_tick && bots.path_cooldown <= 0 && !bots.search_owner;
	boolean direct = bot->direct_wait <= 0 && bots.direct_tests_this_tick < DIRECT_TESTS_PER_TICK;
	boolean backward = FALSE;

	/* (its own search still going on: it waits for that) */
	if (!bots_path_state || (!search && !direct) || bots.search_owner == (short)(bot - bots.bots) + 1)
		return;
	start_surface = biped_find_pathfinding_surface_index(bot->unit_index, &start);
	/* (a unit's own point on its surface, or a point and its surface: an
	objective's) */
	if (target_unit != NONE)
		goal_surface = biped_find_pathfinding_surface_index(target_unit, &goal);
	else
	{
		goal = *goal_point;
		goal_surface = goal_point_surface;
	}
	/* (a point above it its search did not reach: a search backward, from
	the point to the bot -- its last bend is on the way up) */
	if (target_unit == NONE && bot->approach_state == 1 && search)
	{
		real_point3d point = start;
		long surface = start_surface;

		start = goal;
		start_surface = goal_surface;
		goal = point;
		goal_surface = surface;
		backward = TRUE;
		direct = FALSE;
	}
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
	straight line is not worth the search -- the closest point instead; to
	a point, an objective, a longer way round is: up a ramp, round a ledge;
	between a point and one above it, the way may start far off: round a
	hill) */
	path_input_set_search_bounds(&input, target_unit != NONE ? 2.5f * distance3d(&start, &goal) + 15.f :
		4.f * distance3d(&start, &goal) + (fabsf(goal.z - start.z) > 1.f ? 80.f : 30.f));
	path_state_new(&input, bots_path_state, NULL);
	path_state_destination(bots_path_state, &goal, goal_surface, 40.f);
	/* (in parts, bots_search_step; the next search waits for its end) */
	if (!path_state_find_begin(bots_path_state))
		return;
	bots.search_owner = (short)(bot - bots.bots) + 1;
	bots.search_unit = bot->unit_index;
	bots.search_goal = goal;
	bots.search_point = target_unit == NONE;
	bots.search_backward = backward;
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

/* ---------- objectives

In Capture the Flag, King of the Hill, Oddball and Race (and their team
forms) each bot has a role, shared out by its team (n64_bots_actions, once a
second and at once when a flag or the ball changes hands): a goal (a point,
or a unit to go with) and how far a fight may hold it from that goal. Its
fight is the same as in Slayer; the role chooses where it walks when no
enemy holds it, and which enemy comes first. A bot knows what a player
knows: what the HUD marks (the enemy flag, the hill, the ball, its next
checkpoint), its own base, what it sees itself, and what its motion sensor
would show (motion_sensor.c's rule). With a flag or the ball in hand it
never pulls a trigger or throws -- Halo drops what it carries then
(players.c) -- and it melees only; no bot holds a trigger near an
objective it is to pick up (players.c picks up no flag or ball then). In
the other game types, and on a map without the game type's objectives,
the bots have no role and play as before */

enum
{
	_bot_role_none = 0,
	_bot_role_attack, /* CTF: to the enemy flag */
	_bot_role_carry, /* CTF: the enemy flag home; Oddball: the ball, kept away */
	_bot_role_return, /* CTF: to its own flag, dropped where it was seen */
	_bot_role_chase, /* CTF: after its own flag's carrier */
	_bot_role_escort, /* CTF, Team Oddball: with a teammate who carries */
	_bot_role_defend, /* CTF: at its own base */
	_bot_role_hill, /* King: to the hill and on it */
	_bot_role_ball, /* Oddball: to the ball lying free */
	_bot_role_hunt, /* Oddball: after the ball's holder */
	_bot_role_race /* Race: to its next checkpoint */
};

/* what a team's bots can know of their own flag (CTF): at home (seen, or
announced: a return, a capture), taken (announced as it leaves home), or
lying where one of them knows -- a drop is not announced; a dropped flag
taken again is not either */
enum
{
	_bot_flag_home = 0,
	_bot_flag_taken,
	_bot_flag_lying
};

/* what its role makes of a thought: walk to the goal, stay where it is (a
fight in range strafes there), or fight as in Slayer */
enum
{
	_bot_goal_fight = 0,
	_bot_goal_move,
	_bot_goal_hold
};

/* (the campaign and online cartridges start no game with objectives -- a
campaign level, online Slayer (n64_game.c) -- so the code is left out of
them: the stand-ins at the end of this section give no role) */
#if !defined(HALO_N64_CAMPAIGN) && !defined(HALO_N64_ONLINE)
#define BOT_OBJECTIVES 1
#else
#define BOT_OBJECTIVES 0
#endif

#if BOT_OBJECTIVES
/* the bot's unit as its player has it now (a dead bot's: NONE) */
static long bot_player_unit(
	struct bot const *bot)
{
	struct player_datum *player = (struct player_datum *)datum_try_and_get(player_data, bot->player_index);

	return player && unit_alive(player->unit_index) ? player->unit_index : NONE;
}

static short bot_team(
	struct bot const *bot)
{
	return player_get(bot->player_index)->team_index;
}

/* the point under a goal on the level and its pathfinding surface: a ray
down from just above it (once for each new goal point) */
static void bot_goal_ground(
	struct bot *bot,
	real_point3d const *point)
{
	struct collision_result collision;
	real_point3d from = *point;
	real_vector3d down = { 0.f, 0.f, -3.f };

	/* (the same point as asked for last, within half a unit) */
	if (bot->role_surface != NONE && fabsf(bot->role_point.x - point->x) < 0.5f &&
		fabsf(bot->role_point.y - point->y) < 0.5f && fabsf(bot->role_point.z - point->z) < 0.5f)
	{
		return;
	}
	bot->role_point = *point;
	from.z += 0.5f;
	bot->role_ground = *point;
	bot->role_surface = NONE;
	bot->role_best = 1e9f;
	bot->role_still = 0;
	bot->role_from = *point;
	bot->side_timer = bot->side_ticks = 0;
	bot->approach_state = 0;
	/* (its own path search going on, a search backward or one for a point
	over 2 units from this one: dropped) */
	if (bots.search_owner == (short)(bot - bots.bots) + 1 &&
		(bots.search_backward || horizontal_distance(point, &bots.search_goal) > 2.f))
	{
		bots.search_owner = 0;
		bots.path_cooldown = TICKS_BETWEEN_PATHS;
	}
	if (collision_test_vector(FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit),
		&from, &down, NONE, &collision) && collision.type == _collision_result_structure)
	{
		bot->role_ground = collision.point;
		bot->role_surface = collision.surface_index;
	}
	else
		bot->role_surface = -2; /* (none under it: walked to straight) */
}

/* the unit carrying an objective (a flag, the ball), or NONE */
static long objective_carrier(
	long weapon_index)
{
	struct object_datum *object = weapon_index != NONE ? object_try_and_get(weapon_index) : NULL;
	long parent = object ? object->object.parent_object_index : NONE;

	if (parent == NONE || !TEST_FLAG(_object_mask_unit, object_get(parent)->object.type))
		return NONE;
	return parent;
}

/* whether a bot holds a flag or the ball in its hands */
static boolean bot_holds_objective(
	long weapon_index)
{
	return weapon_index != NONE && weapon_is_flag(weapon_index);
}

/* whether the bot's motion sensor would show a unit: within its range and
moving, or firing or throwing (motion_sensor.c, should_draw_object; the
variant can turn the sensor off) */
static boolean bot_senses(
	struct bot const *bot,
	long unit_index,
	real_point3d const *position)
{
	struct unit_datum *unit;
	real_vector3d velocity;
	real range = hud_globals ? hud_globals->defaults.motion_sensor_range : 0.f;
	real_point3d at;

	if (range <= 0.f || !unit_alive(unit_index) || !game_engine_hud_draw_motion_sensor(bot->player_index) ||
		!game_engine_draw_object_in_motion_sensor(unit_index))
	{
		return FALSE;
	}
	object_get_origin(unit_index, &at);
	if (fabsf(at.x - position->x) >= range || fabsf(at.y - position->y) >= range ||
		horizontal_distance(&at, position) >= range)
	{
		return FALSE;
	}
	unit = unit_get(unit_index);
	if (TEST_FLAG(unit->unit.control_flags, _unit_control_weapon_primary_trigger_bit) ||
		(unit->unit.grenade_throw_state != _unit_grenade_throw_idle &&
		unit->unit.grenade_throw_state != _unit_grenade_throw_ending))
	{
		return TRUE;
	}
	object_get_velocities(unit_index, &velocity, NULL);
	return magnitude_squared3d(&velocity) >= hud_globals->defaults.motion_sensor_velocity_sensitivity;
}

/* how long a sight of its own flag or its carrier counts (as in sight now):
more than the longest gap between two looks, 16 ticks (a look each 10
ticks or more, a thought each 8 ticks at most) */
#define OWN_FLAG_SIGHT_TICKS 24

/* CTF: what the bot knows of its own flag, looked at a few times a second
while it is away, and at once when its carrier changed: its carrier where
the bot sees it or its sensor shows it; the flag lying where the bot sees
it */
static void bot_watch_own_flag(
	struct bot *bot,
	real_point3d const *position,
	real_point3d const *eye,
	short step)
{
	short team = bot_team(bot);
	long flag = objectives.flag[team], carrier = objectives.carrier[team];
	/* (as its target's sight test: a seated bot's ray leaves its vehicle
	out; a seated carrier's vehicle may end it) */
	long vehicle = object_get(bot->unit_index)->object.parent_object_index;
	long ignore = vehicle != NONE ? vehicle : bot->unit_index;
	real_point3d at;

	if (flag == NONE)
		return;
	if (!objectives.away[team])
	{
		bot->flag_known = objectives.base[team];
		bot->flag_known_time = game_time_get();
		bot->flag_known_unit = NONE;
		return;
	}
	if (objectives.changed[team] >= bot->last_think)
		bot->flag_look = 0;
	if ((bot->flag_look -= step) > 0)
		return;
	bot->flag_look = 10;
	if (carrier != NONE)
	{
		real_point3d carrier_eye;

		object_get_origin(carrier, &at);
		unit_get_camera_position(carrier, &carrier_eye);
		if (bot_senses(bot, carrier, position) ||
			(horizontal_distance(position, &at) < SIGHT_RANGE && !bot_camouflaged(carrier, horizontal_distance(position, &at)) &&
			n64_ai_clusters_may_see(bot->unit_index, carrier) &&
			line_of_sight_past(eye, &carrier_eye, ignore, object_get(carrier)->object.parent_object_index != NONE ?
				object_get_ultimate_parent(carrier) : NONE)))
		{
			bot->flag_known = at;
			bot->flag_known_time = game_time_get();
			bot->flag_known_unit = carrier;
		}
		return;
	}
	item_get_position_even_if_in_inventory(flag, &at);
	at.z += 0.3f;
	if (horizontal_distance(position, &at) < SIGHT_RANGE && n64_ai_clusters_may_see(bot->unit_index, flag) &&
		line_of_sight(eye, &at, ignore))
	{
		bot->flag_known = at;
		bot->flag_known_time = game_time_get();
		bot->flag_known_unit = NONE;
	}
}

/* whether it knows where its own flag (or its carrier) is since the last
announcement about it (the flag taken from home, returned, captured) */
static boolean bot_knows_own_flag(
	struct bot const *bot)
{
	short team = bot_team(bot);

	return bot->flag_known_time > 0 && bot->flag_known_time >= objectives.announced[team];
}

/* whether the bot saw (or its sensor showed) this very unit carrying its
own flag in the last OWN_FLAG_SIGHT_TICKS */
static boolean bot_sees_own_carrier(
	struct bot const *bot,
	long carrier)
{
	if (carrier != NONE && bot_knows_own_flag(bot) && bot->flag_known_unit == carrier &&
		game_time_get() - bot->flag_known_time <= OWN_FLAG_SIGHT_TICKS)
	{
		return TRUE;
	}
	return FALSE;
}

/* the enemy a bot may not choose as its target now (NONE: any): CTF, its
own flag's carrier, unless it knows where that carrier is (seen or on its
motion sensor within OWN_FLAG_SIGHT_TICKS) -- a player is not told where
it is */
static long bot_hidden_enemy(
	struct bot const *bot)
{
	if (objectives.engine == game_engine_ctf)
	{
		long carrier = objectives.carrier[bot_team(bot)];

		if (carrier != NONE && !bot_sees_own_carrier(bot, carrier))
			return carrier;
	}
	return NONE;
}


/* King: whether a point (a unit's centre, as king_engine_player_update
tests it) is in the hill, and how far it is from the hill's edge */
static boolean bot_in_hill(
	real_point3d const *point,
	real *edge)
{
	real_point2d at;
	short i, count = (short)king_globals.hill_point_count;

	*edge = 0.f;
	if (count < 3 || point->z < king_globals.hill_bottom || point->z > king_globals.hill_top)
		return FALSE;
	at.x = point->x;
	at.y = point->y;
	if (!convex_hull2d_test_point(count, king_globals.convex_hull, &at, 0.f))
		return FALSE;
	*edge = 1e9f;
	for (i = 0; i < count; i++)
	{
		real_point2d const *a = &king_globals.convex_hull[i], *b = &king_globals.convex_hull[(i + 1) % count];
		real ex = b->x - a->x, ey = b->y - a->y, length = square_root(ex * ex + ey * ey);

		if (length > 0.01f)
		{
			real d = fabsf((at.x - a->x) * ey - (at.y - a->y) * ex) / length;

			if (d < *edge)
				*edge = d;
		}
	}
	return TRUE;
}


/* the point its role sends it to, if a point (FALSE: none, or a unit) */
static boolean bot_role_point(
	struct bot *bot,
	real_point3d const *position,
	real_point3d *point)
{
	short team = bot_team(bot);
	real_point3d enemy;

	switch (bot->role)
	{
	case _bot_role_attack:
		/* (the enemy flag, as the HUD marks it: at its base, or lying) */
		if (objectives.flag[1 - team] == NONE)
			return FALSE;
		item_get_position_even_if_in_inventory(objectives.flag[1 - team], point);
		return TRUE;
	case _bot_role_carry:
		if (objectives.engine == game_engine_ctf)
		{
			*point = objectives.base[team];
			return TRUE;
		}
		return FALSE;
	case _bot_role_return:
		*point = bot->flag_known;
		return TRUE;
	case _bot_role_chase:
		/* (where it last knew the carrier, for 5 s, until it gets within 3
		units with no sight of it in the last OWN_FLAG_SIGHT_TICKS -- then
		that sight counts as old, searched; then the carrier's likely way: to
		the enemy base) */
		if (bot_knows_own_flag(bot) && game_time_get() - bot->flag_known_time < 5 * TICKS_PER_SECOND)
		{
			if (horizontal_distance(position, &bot->flag_known) > 3.f || fabsf(bot->flag_known.z - position->z) > 2.f ||
				game_time_get() - bot->flag_known_time <= OWN_FLAG_SIGHT_TICKS)
			{
				*point = bot->flag_known;
				return TRUE;
			}
			/* (there, and the carrier not seen lately) */
			bot->flag_known_time = game_time_get() - 5 * TICKS_PER_SECOND - 1;
		}
		*point = objectives.base[1 - team];
		return TRUE;
	case _bot_role_defend:
		*point = objectives.base[team];
		return TRUE;
	case _bot_role_hill:
		*point = king_globals.hill_center;
		return king_globals.hill_point_count > 0;
	}
	return FALSE;
}

/* the enemy its role puts first, or NONE (the nearest enemy then):
CTF: its own flag's carrier for a chaser or a defender that knows where it
is now; of the enemies it knows of (its target in sight, or on its motion
sensor), the one nearest a carrier it escorts, within 15 units of it;
Oddball: the holder (the HUD marks the ball) */
static long bot_objective_target(
	struct bot *bot,
	real_point3d const *position)
{
	switch (bot->role)
	{
	case _bot_role_chase:
	case _bot_role_defend:
	{
		long carrier = objectives.carrier[bot_team(bot)];

		if (bot_sees_own_carrier(bot, carrier) &&
			(bot->role == _bot_role_chase || horizontal_distance(&bot->flag_known, &objectives.base[bot_team(bot)]) < 20.f))
		{
			return carrier;
		}
		break;
	}
	case _bot_role_escort:
	{
		struct data_iterator iterator;
		struct player_datum *player;
		real_point3d with;
		long best = NONE;
		real best_distance = 15.f;

		if (!unit_alive(bot->role_unit))
			break;
		object_get_origin(bot->role_unit, &with);
		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
		{
			real_point3d at;
			real d;

			if (!unit_alive(player->unit_index) || player->team_index == bot_team(bot) ||
				!((player->unit_index == bot->target_unit && bot->target_visible) ||
				bot_senses(bot, player->unit_index, position)))
			{
				continue;
			}
			d = horizontal_distance(&with, object_get_origin(player->unit_index, &at));
			if (d < best_distance && !bot_camouflaged(player->unit_index, horizontal_distance(position, &at)))
			{
				best_distance = d;
				best = player->unit_index;
			}
		}
		return best;
	}
	}
	return NONE;
}

/* how far it may search for a better weapon: near its way while it goes
for an objective; none with a flag or the ball in hand (a swap drops what
is in hand: players.c) */
static real bot_weapon_range(
	struct bot const *bot)
{
	switch (bot->role)
	{
	case _bot_role_attack:
	case _bot_role_escort:
	case _bot_role_race:
	case _bot_role_ball:
		return 6.f;
	case _bot_role_carry:
		return 0.f;
	case _bot_role_hill:
	{
		real edge;

		/* (on the hill: none, unless no weapon it carries can fire; on the way
		to it, near its way) */
		if (unit_alive(bot->unit_index) && bot_in_hill(&object_get(bot->unit_index)->object.bounding_sphere_center, &edge))
		{
			short i;

			for (i = 0; i < 4; i++)
			{
				long weapon = unit_inventory_get_weapon(bot->unit_index, i);

				if (weapon != NONE && weapon_can_be_fired(weapon))
					return 0.f;
			}
		}
		return 6.f;
	}
	}
	return WEAPON_SEARCH_RANGE;
}

/* whether its role keeps it on foot (no driver's seat): a carrier (it
rides only as a passenger), a defender, a returner, and a bot within 25
units of its hill */
static boolean bot_role_walks(
	struct bot *bot,
	real_point3d const *position)
{
	switch (bot->role)
	{
	case _bot_role_carry:
	case _bot_role_defend:
	case _bot_role_return:
		return TRUE;
	case _bot_role_hill:
	{
		real_point3d goal;

		return !bot_role_point(bot, position, &goal) || horizontal_distance(position, &goal) < 25.f;
	}
	}
	return FALSE;
}

/* the walk to its role's goal (a point by a path over the level, or the
unit it goes with), unless a fight within its reach holds it: what it
does this thought (move, hold, or fight as in Slayer) */
/* the eight ways round a bot (1-8 in bot_free_way's answer) */
static real const bot_ways[8][2] = { { 1.f, 0.f }, { 0.7071f, 0.7071f }, { 0.f, 1.f }, { -0.7071f, 0.7071f },
	{ -1.f, 0.f }, { -0.7071f, -0.7071f }, { 0.f, -1.f }, { 0.7071f, -0.7071f } };

/* whether a ray from a point (raised by a height), along a horizontal way
for a length, is free of the level and of the objects that stop a body
(the bot's own vehicle left out when it sits in one; a walkable slope of
the level, rising ground, is no blockage) */
static boolean bot_way_free(
	struct bot *bot,
	real_point3d const *position,
	real height,
	real i,
	real j,
	real length)
{
	struct collision_result collision;
	real_point3d from = *position;
	real_vector3d ray;
	long vehicle = object_get(bot->unit_index)->object.parent_object_index;

	from.z += height;
	ray.i = length * i;
	ray.j = length * j;
	ray.k = 0.f;
	return !collision_test_vector(FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit) |
		FLAG(_collision_test_objects_bit) | FLAG(_collision_test_objects_scenery_bit) |
		FLAG(_collision_test_objects_machines_bit) | FLAG(_collision_test_objects_vehicles_bit),
		&from, &ray, vehicle != NONE ? vehicle : bot->unit_index, &collision) ||
		(collision.type == _collision_result_structure && collision.plane.n.k > 0.7f);
}

/* a bot stopped or blocked on its way: of the eight ways round it (rays of
a length, at a height above its point), the free one nearest its goal's,
not straight on (it is stopped there); 1-8, or 0 when none is free */
static short bot_free_way(
	struct bot *bot,
	real_point3d const *position,
	real ti,
	real tj,
	real height,
	real length)
{
	real best = -2.f;
	short k, way = 0;

	for (k = 0; k < 8; k++)
	{
		real along = bot_ways[k][0] * ti + bot_ways[k][1] * tj;

		if (along > 0.9f || along <= best || !bot_way_free(bot, position, height, bot_ways[k][0], bot_ways[k][1], length))
			continue;
		best = along;
		way = (short)(k + 1);
	}
	return way;
}

static short bot_walk_objective(
	struct bot *bot,
	struct unit_datum *unit,
	real_point3d const *position,
	boolean in_range,
	real distance,
	real_vector2d *move,
	boolean *moving,
	boolean *along_path,
	short step)
{
	real_point3d goal, enemy;
	real arrive = 0.3f, d;
	long surface;
	boolean point = bot_role_point(bot, position, &goal);
	boolean engage = FALSE;
	short team = bot_team(bot);
	boolean zone;

	if (bot->keep_timer > 0)
		bot->keep_timer -= step;
	/* (on the hill: it holds its place and fights from there, a pause or
	not; a dodge that takes it off, and it walks back in) */
	if (bot->role == _bot_role_hill)
	{
		real edge;

		if (bot_in_hill(&unit->object.bounding_sphere_center, &edge))
		{
			/* (and its count of no progress starts afresh: a dodge off
			the hill gets its full 3 s) */
			bot->role_pause = 0;
			bot->role_still = 0;
			bot->role_best = 1e9f;
			return _bot_goal_hold;
		}
	}
	/* (a goal that moved -- a hill that moved, a flag that fell -- is a
	new goal: no pause) */
	if (point && bot->role_pause > 0 && bot->role_surface != NONE && distance3d(&goal, &bot->role_point) > 2.f)
		bot->role_pause = 0;
	/* (near the hill's centre -- 3 units across, 2 of its height -- a pause
	ends: it goes back in) */
	if (bot->role == _bot_role_hill && bot->role_pause > 0 &&
		horizontal_distance(position, &king_globals.hill_center) < 3.f &&
		fabsf(king_globals.hill_center.z - position->z) < 2.f)
	{
		bot->role_pause = 0;
	}
	if (bot->role_pause > 0)
	{
		/* (a goal given up a while ago: it fights as in Slayer) */
		bot->role_pause -= step;
		return _bot_goal_fight;
	}
	switch (bot->role)
	{
	case _bot_role_attack:
	case _bot_role_return:
		engage = in_range && distance < (bot->role == _bot_role_attack ? 10.f : 6.f);
		break;
	case _bot_role_carry:
		arrive = objectives.engine == game_engine_ctf ? 0.3f : 1.f;
		break;
	case _bot_role_chase:
		engage = in_range;
		arrive = 3.f;
		break;
	case _bot_role_defend:
		/* (a fight near its base, within 20 units of it) */
		engage = bot->target_unit != NONE && bot->target_visible &&
			horizontal_distance(object_get_origin(bot->target_unit, &enemy), &objectives.base[team]) < 20.f;
		arrive = 3.f;
		break;
	case _bot_role_escort:
		if (!unit_alive(bot->role_unit))
			return _bot_goal_fight;
		object_get_origin(bot->role_unit, &goal);
		point = FALSE;
		arrive = bot->role_rank ? 9.f : 4.f;
		d = horizontal_distance(position, &goal);
		/* (near it only on its floor: from below or above it, on along the
		path) */
		if (fabsf(goal.z - position->z) < 2.f)
		{
			if (d <= arrive + 3.f && in_range)
				return _bot_goal_fight;
			if (d <= arrive)
				return _bot_goal_hold;
		}
		/* (along a path to the one it goes with, as to a target) */
		bot->path_age += step;
		if (!bot->path_valid || bot->path_age > PATH_LIFETIME || horizontal_distance(&bot->path_goal, &goal) > 4.f)
			plan_path(bot, bot->role_unit, NULL, NONE);
		if (follow_path(bot, &unit->object.position, move))
			*along_path = TRUE;
		else if (d > 0.01f)
		{
			move->i = (goal.x - position->x) / d;
			move->j = (goal.y - position->y) / d;
		}
		*moving = TRUE;
		return _bot_goal_move;
	case _bot_role_hill:
	{
		/* (just off it -- within 2 units of its widest point -- back in
		first) */
		{
			real reach = 0.f;
			short i;

			for (i = 0; i < (short)king_globals.hill_point_count; i++)
				reach = MAX(reach, horizontal_distance(&king_globals.hill_center, &king_globals.hill_points[i]));
			engage = in_range && distance < 10.f && horizontal_distance(position, &king_globals.hill_center) > reach + 2.f;
		}
		break;
	}
	}
	if (!point)
		return _bot_goal_fight;
	if (engage)
		return _bot_goal_fight;
	bot_goal_ground(bot, &goal);
	goal = bot->role_ground;
	d = horizontal_distance(position, &goal);
	/* (a returner at the place it searches: a second there without seeing
	the flag, and it knows no more; the roles are shared again) */
	if (bot->role == _bot_role_return)
	{
		if (d > 1.5f || fabsf(goal.z - position->z) > 1.f || game_time_get() - bot->flag_known_time <= OWN_FLAG_SIGHT_TICKS)
			bot->search_ticks = 0;
		else if ((bot->search_ticks += step) >= TICKS_PER_SECOND)
		{
			short i;

			/* (and its teammates who knew it lying there -- within 2 units
			across and 1 in height, seen no later than this bot saw it --
			forget it too: the team shares what it found) */
			for (i = 0; i < bots.created; i++)
			{
				struct bot *mate = &bots.bots[i];

				if (mate != bot && bot_team(mate) == team && mate->flag_known_unit == NONE &&
					mate->flag_known_time > 0 && mate->flag_known_time <= bot->flag_known_time &&
					horizontal_distance(&mate->flag_known, &bot->flag_known) < 2.f &&
					fabsf(mate->flag_known.z - bot->flag_known.z) < 1.f)
				{
					mate->flag_known_time = 0;
				}
			}
			bot->search_ticks = 0;
			bot->flag_known_time = 0;
			objectives.next_share = 0;
			return _bot_goal_fight;
		}
	}
	if (d <= arrive && goal.z - position->z < (arrive >= 3.f ? 2.f : 0.4f) &&
		position->z - goal.z < (arrive >= 3.f ? 2.f : 1.f))
	{
		return (bot->role == _bot_role_carry && objectives.engine == game_engine_ctf) || bot->role == _bot_role_return ||
			bot->role == _bot_role_attack || bot->role == _bot_role_race || bot->role == _bot_role_hill ?
			_bot_goal_move : _bot_goal_hold;
	}
	/* (a way up found backward from its point: to that bend first, then
	its search from there) */
	surface = bot->role_surface;
	if (bot->approach_state == 2)
	{
		if (horizontal_distance(position, &bot->approach) < 1.5f && fabsf(bot->approach.z - position->z) < 1.f)
		{
			bot->approach_state = 0;
			bot->role_best = 1e9f;
			bot->role_still = 0;
			bot->path_valid = FALSE;
		}
		else
		{
			goal = bot->approach;
			surface = bot->approach_surface;
			d = horizontal_distance(position, &goal);
		}
	}
	/* (near the hill's centre -- 3 units across, 2 of its height -- it never
	leaves the goal (but on its way to a bend found backward): there its
	count runs only to just under the give-up, for its 1-s stop below, and
	it starts afresh as the bot steps out: there it gets its full 3 s) */
	zone = bot->role == _bot_role_hill && bot->approach_state != 2 && d < 3.f && fabsf(goal.z - position->z) < 2.f;
	if (bot->hill_zone && !zone)
		bot->role_still = 0;
	bot->hill_zone = (short)zone;
	/* (in 3 s of its thoughts neither a unit nearer nor 2 units moved (not
	on a free way: that leads off the path) --
	a way round may lead away at first: given up for 10 s; a carrier finds
	a new way instead) */
	if (d < bot->role_best - 1.f ||
		((bot->side_timer <= 0 || bot->side_turn == 0) && horizontal_distance(position, &bot->role_from) > 2.f))
	{
		bot->role_best = MIN(d, bot->role_best);
		bot->role_from = *position;
		bot->role_still = 0;
	}
	else if (zone)
	{
		if ((bot->role_still += step) >= OBJECTIVE_PROGRESS_TICKS)
			bot->role_still = OBJECTIVE_PROGRESS_TICKS - 1;
	}
	else if ((bot->role_still += step) >= OBJECTIVE_PROGRESS_TICKS)
	{
		bot->role_still = 0;
		bot->path_valid = FALSE;
		/* (a bend found backward and not reached: tried; the point itself
		at once, its own search for the bend dropped) */
		if (bot->approach_state == 2)
		{
			bot->approach_state = 3;
			goal = bot->role_ground;
			surface = bot->role_surface;
			d = horizontal_distance(position, &goal);
			bot->role_from = *position;
			if (bots.search_owner == (short)(bot - bots.bots) + 1)
			{
				bots.search_owner = 0;
				bots.path_cooldown = TICKS_BETWEEN_PATHS;
			}
		}
		bot->role_best = d;
		/* (a carrier or a racer: a real search this time, not the straight
		test that led it here, and no pause) */
		bot->direct_wait = DIRECT_PATH_RETRY_TICKS;
		if (bot->role != _bot_role_carry && bot->role != _bot_role_race)
		{
			bot->role_pause = OBJECTIVE_PAUSE_TICKS;
			return _bot_goal_fight;
		}
	}
	bot->path_age += step;
	if (surface >= 0 &&
		(!bot->path_valid || bot->path_age > PATH_LIFETIME || horizontal_distance(&bot->path_goal, &goal) > 2.f))
	{
		plan_path(bot, NONE, &goal, surface);
	}
	/* (a point above it within 2.5 units, a ledge or a rock: a jump up,
	once a second at most -- a person's way onto it; right below it too) */
	if (d < 2.5f && goal.z - position->z > 0.4f && goal.z - position->z < 2.f && bot->role != _bot_role_carry &&
		bot->keep_timer <= 0)
	{
		bot->jump_timer = 1;
		bot->keep_timer = TICKS_PER_SECOND;
	}
	/* (stopped -- 1 s neither a unit nearer nor 2 units on; on its path, and
	less than a unit moved too: a jump, as over a sill, and 2 s -- on its
	path, the free way nearest its own of eight (round a rock); walking
	straight, or with no free way, along the wall to one side, leaning in
	(out of a room or past a crate the level's path data does not hold), the
	other side after 10 s of that) */
	{
		real ti = d > 0.01f ? (goal.x - position->x) / d : 0.f, tj = d > 0.01f ? (goal.y - position->y) / d : 0.f;
		boolean pathed = surface >= 0 && (d > 1.f || fabsf(goal.z - position->z) > 0.4f) && bot->path_valid &&
			bot->path.step_index < bot->path.step_count;

		if (d > 0.01f && (bot->unstick_timer > 0 || (bot->side_timer <= 0 && bot->role_still >= TICKS_PER_SECOND &&
			(!pathed || horizontal_distance(position, &bot->role_from) < 1.f))))
		{
			bot->unstick_timer = 0;
			bot->jump_timer = 1;
			bot->side_timer = 2 * TICKS_PER_SECOND;
			bot->side_turn = pathed ? bot_free_way(bot, position, ti, tj, 0.5f, 3.f) : 0;
			/* (wall walks only: five to one side, 10 s, then five to the
			other) */
			if (bot->side_turn == 0 && (bot->side_ticks += 2 * TICKS_PER_SECOND) > 20 * TICKS_PER_SECOND)
				bot->side_ticks = 2 * TICKS_PER_SECOND;
		}
		/* (right under its point: no way across to walk along a wall) */
		if (d <= 0.01f)
			bot->side_timer = 0;
		if (bot->side_timer > 0)
		{
			bot->side_timer -= step;
			/* (a free way that ends: what it moved is no progress) */
			if (bot->side_timer <= 0 && bot->side_turn > 0)
				bot->role_from = *position;
			if (bot->side_turn > 0)
			{
				move->i = bot_ways[bot->side_turn - 1][0];
				move->j = bot_ways[bot->side_turn - 1][1];
			}
			else
			{
				real turn = (bot->side_ticks <= 10 * TICKS_PER_SECOND) == (((bot - bots.bots) & 1) != 0) ? 1.f : -1.f;
				real mi = -tj * turn + 0.4f * ti, mj = ti * turn + 0.4f * tj;
				real m = square_root(mi * mi + mj * mj);

				move->i = mi / m;
				move->j = mj / m;
			}
		}
		else if (surface >= 0 && (d > 1.f || fabsf(goal.z - position->z) > 0.4f) &&
			follow_path(bot, &unit->object.position, move))
		{
			*along_path = TRUE;
		}
		else if (d > 0.01f)
		{
			/* (no path to the point itself -- the last step, a point with no
			surface under it, or one Halo's paths do not reach: straight) */
			move->i = ti;
			move->j = tj;
		}
	}
	*moving = d > 0.05f || *along_path || bot->side_timer > 0;
	return _bot_goal_move;
}

/* whether the bot is about to walk over an objective it is to pick up
(within 2 units of it): no trigger held then */
static boolean bot_near_pickup(
	struct bot *bot,
	real_point3d const *position)
{
	real_point3d goal;

	if (bot->role != _bot_role_attack && bot->role != _bot_role_return && bot->role != _bot_role_ball)
		return FALSE;
	return bot_role_point(bot, position, &goal) && horizontal_distance(position, &goal) < 2.f &&
		fabsf(goal.z - position->z) < 2.f;
}

/* CTF: a carrier riding: out within 6 units of its base (it scores on
foot, with the flag in hand) */
static boolean bot_ride_done(
	struct bot *bot,
	real_point3d const *position)
{
	return objectives.engine == game_engine_ctf && bot->role == _bot_role_carry &&
		horizontal_distance(position, &objectives.base[bot_team(bot)]) < 6.f;
}

/* the share: a role for each living bot of a team (or each bot in
free-for-all), its previous role kept where the case allows (ties: the
lower bot number) */
static void bot_set_role(
	struct bot *bot,
	short role,
	short rank,
	long role_unit)
{
	if (bot->role != role || bot->role_rank != rank || bot->role_unit != role_unit)
	{
		bot->role = role;
		bot->role_rank = rank;
		bot->role_unit = role_unit;
		bot->role_surface = NONE;
		bot->role_pause = 0;
		bot->path_valid = FALSE;
		bot->goal_weapon = NONE;
		bot->keep_timer = 0;
		/* (its own path search going on is for the old goal: dropped) */
		if (bots.search_owner == (short)(bot - bots.bots) + 1)
		{
			bots.search_owner = 0;
			bots.path_cooldown = TICKS_BETWEEN_PATHS;
		}
	}
}

/* the living bot of a team without a role yet nearest a point (one with
that role and rank already first), or NONE */
static short bot_nearest_free(
	short team,
	unsigned long taken,
	short role,
	short rank,
	real_point3d const *point)
{
	short i, best = NONE;
	real best_value = 1e30f;

	for (i = 0; i < bots.created; i++)
	{
		struct bot *bot = &bots.bots[i];
		long unit_index = bot_player_unit(bot);
		real_point3d at;
		real value;

		if ((taken & (1UL << i)) || unit_index == NONE || (team != NONE && bot_team(bot) != team))
			continue;
		value = point ? horizontal_distance(object_get_origin(unit_index, &at), point) : 0.f;
		if (bot->role == role && bot->role_rank == rank)
			value -= 1000.f;
		if (value < best_value)
		{
			best_value = value;
			best = i;
		}
	}
	return best;
}

static short bot_team_members(
	short team)
{
	struct data_iterator iterator;
	struct player_datum *player;
	short count = 0;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		if (player->team_index == team)
			count++;
	}
	return count;
}

/* CTF, for a team: the carrier; a returner (its flag lying where one of
its bots knows); chasers (its flag carried: 1, 2 with 4 bots or more); an
escort (a teammate carries the enemy flag); a defender from 3 members (2
from 6); the rest attack -- or, the enemy flag carried by a teammate, go
with it farther back */
static void bots_share_ctf(
	short team)
{
	unsigned long taken = 0;
	long enemy_carrier = objectives.carrier[1 - team];
	boolean ours = enemy_carrier != NONE && unit_get(enemy_carrier)->unit.player_index != NONE &&
		player_get(unit_get(enemy_carrier)->unit.player_index)->team_index == team;
	short members = bot_team_members(team), living = 0, defenders, chasers, i, b;
	real_point3d carrier_at;

	for (i = 0; i < bots.created; i++)
	{
		struct bot *bot = &bots.bots[i];
		long unit_index = bot_player_unit(bot);

		if (unit_index == NONE || bot_team(bot) != team)
			continue;
		living++;
		if (unit_index == enemy_carrier)
		{
			bot_set_role(bot, _bot_role_carry, 0, NONE);
			taken |= 1UL << i;
		}
	}
	if (objectives.believed[team] == _bot_flag_lying)
	{
		/* (the nearest of those that saw it lying, one returning already
		first) */
		real best_distance = 1e30f;

		b = NONE;
		for (i = 0; i < bots.created; i++)
		{
			struct bot *bot = &bots.bots[i];
			long unit_index = bot_player_unit(bot);
			real_point3d at;
			real d;

			if ((taken & (1UL << i)) || unit_index == NONE || bot_team(bot) != team || !bot_knows_own_flag(bot) ||
				bot->flag_known_unit != NONE)
			{
				continue;
			}
			d = horizontal_distance(object_get_origin(unit_index, &at), &bot->flag_known) -
				(bot->role == _bot_role_return ? 1000.f : 0.f);
			if (d < best_distance)
			{
				best_distance = d;
				b = i;
			}
		}
		if (b != NONE)
		{
			bot_set_role(&bots.bots[b], _bot_role_return, 0, NONE);
			taken |= 1UL << b;
		}
	}
	if (objectives.believed[team] == _bot_flag_taken)
	{
		chasers = living >= 4 ? 2 : 1;
		while (chasers-- > 0 && (b = bot_nearest_free(team, taken, _bot_role_chase, 0, &objectives.base[team])) != NONE)
		{
			bot_set_role(&bots.bots[b], _bot_role_chase, 0, NONE);
			taken |= 1UL << b;
		}
	}
	if (ours)
	{
		object_get_origin(enemy_carrier, &carrier_at);
		if ((b = bot_nearest_free(team, taken, _bot_role_escort, 0, &carrier_at)) != NONE)
		{
			bot_set_role(&bots.bots[b], _bot_role_escort, 0, enemy_carrier);
			taken |= 1UL << b;
		}
	}
	defenders = members >= 6 ? 2 : members >= 3 ? 1 : 0;
	while (defenders-- > 0 && (b = bot_nearest_free(team, taken, _bot_role_defend, 0, &objectives.base[team])) != NONE)
	{
		bot_set_role(&bots.bots[b], _bot_role_defend, 0, NONE);
		taken |= 1UL << b;
	}
	for (i = 0; i < bots.created; i++)
	{
		if ((taken & (1UL << i)) || bot_player_unit(&bots.bots[i]) == NONE || bot_team(&bots.bots[i]) != team)
			continue;
		if (ours)
			bot_set_role(&bots.bots[i], _bot_role_escort, 1, enemy_carrier);
		else
			bot_set_role(&bots.bots[i], _bot_role_attack, 0, NONE);
	}
}


/* the objectives' state, each tick before the bots think: the game type
and its objectives once a game is under way; the flags' or the ball's
hands; the roles shared out once a second, and at once when a flag or the
ball changes hands */
static void objectives_update(
	long now)
{
	boolean changed = FALSE;
	short k, count, i;

	if (!game_engine || !game_in_progress() || !game_engine_running())
		return;
	if (!objectives.ready)
	{
		struct game_variant *variant = game_engine_get_variant();

		objectives.ready = TRUE;
		objectives.engine = game_engine_none;
		objectives.flag[0] = objectives.flag[1] = NONE;
		objectives.carrier[0] = objectives.carrier[1] = NONE;
		switch (variant->game_engine_index)
		{
		case game_engine_ctf:
			/* (both bases on the map, two flags, no flag swapping sides:
			the built-in variant) */
			if (game_engine_has_teams() && variant->game_engine_variant.ctf.single_flag_time <= 0 &&
				!variant->game_engine_variant.ctf.assault)
			{
				short team;

				objectives.engine = game_engine_ctf;
				for (team = 0; team < 2; team++)
				{
					long flag = find_netgame_flag(NULL, 0.f, 0.f, _netgame_flag_ctf_flag, team);

					if (flag == NONE)
						objectives.engine = game_engine_none;
					else
						objectives.base[team] = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->netgame_flags, flag,
							struct scenario_netgame_flag)->position;
				}
			}
			break;
		case game_engine_king:
			if (king_globals.hill_point_count > 0)
				objectives.engine = game_engine_king;
			break;
		}
	}
	if (objectives.engine == game_engine_none)
		return;
	count = objectives.engine == game_engine_ctf ? 2 : objectives.engine == game_engine_oddball ? 1 : 0;
	for (k = 0; k < count; k++)
	{
		long flag = objectives.flag[k], carrier;
		boolean away;

		/* (a flag or the ball found again when it is gone, a few times a
		second) */
		if (flag == NONE || !object_try_and_get(flag) || !TEST_FLAG(_object_mask_weapon, object_get(flag)->object.type) ||
			!weapon_is_flag(flag))
		{
			struct object_iterator iterator;
			struct object_datum *object;

			objectives.flag[k] = flag = NONE;
			objectives.carrier[k] = NONE;
			objectives.away[k] = FALSE;
			if (now % 15)
				continue;
			object_iterator_new(&iterator, _object_mask_weapon, 0);
			while ((object = object_iterator_next(&iterator)) != NULL && flag == NONE)
			{
				if (weapon_is_flag(iterator.index) && (count == 1 || object->object.owner_team_index == k))
					flag = iterator.index;
			}
			if ((objectives.flag[k] = flag) == NONE)
				continue;
			changed = TRUE;
		}
		carrier = objective_carrier(flag);
		away = TEST_FLAG(weapon_get(flag)->weapon.flags, 6) && objectives.engine == game_engine_ctf;
		if (carrier != objectives.carrier[k] || away != objectives.away[k])
		{
			objectives.carrier[k] = carrier;
			objectives.away[k] = away;
			objectives.changed[k] = now;
			changed = TRUE;
		}
	}
	/* (what each team's bots can know of their own flag: home; taken, as
	the grab from home is announced; lying, or taken again, once one of
	them sees it or its carrier) */
	if (objectives.engine == game_engine_ctf)
	{
		short team;

		for (team = 0; team < 2; team++)
		{
			short was = objectives.believed[team];
			boolean seen_lying = FALSE, seen_carried = FALSE, known_lying = FALSE;

			/* (a bot of the team saw it lying, or saw its carrier, lately; one
		knows where it lies) */
			for (i = 0; i < bots.created; i++)
			{
				struct bot *bot = &bots.bots[i];

				if (bot_team(bot) == team && bot_knows_own_flag(bot) && bot->flag_known_unit == NONE)
					known_lying = TRUE;
				if (bot_team(bot) == team && bot_knows_own_flag(bot) && bot->flag_known_time >= now - OWN_FLAG_SIGHT_TICKS)
				{
					if (bot->flag_known_unit == NONE)
						seen_lying = TRUE;
					else if (bot->flag_known_unit == objectives.carrier[team])
						seen_carried = TRUE;
				}
			}
			if (!objectives.away[team])
				objectives.believed[team] = _bot_flag_home;
			else if (objectives.believed[team] == _bot_flag_home)
			{
				objectives.believed[team] = _bot_flag_taken;
				objectives.announced[team] = now;
			}
			else if (seen_lying)
				objectives.believed[team] = _bot_flag_lying;
			else if (seen_carried)
				objectives.believed[team] = _bot_flag_taken;
			/* (lying, but no bot of the team knows the place any more -- its
			search found nothing: someone picked it up, unseen; the chasers go
			to the enemy base, the carrier's way home) */
			else if (objectives.believed[team] == _bot_flag_lying && !known_lying)
				objectives.believed[team] = _bot_flag_taken;
			if (objectives.believed[team] != was)
			{
				changed = TRUE;
				if (objectives.believed[team] == _bot_flag_home)
					objectives.announced[team] = now;
			}
		}
	}
	if (!changed && now < objectives.next_share)
		return;
	objectives.next_share = now + OBJECTIVE_SHARE_TICKS;
	switch (objectives.engine)
	{
	case game_engine_ctf:
		bots_share_ctf(0);
		bots_share_ctf(1);
		break;
	case game_engine_king:
		for (i = 0; i < bots.created; i++)
			bot_set_role(&bots.bots[i], _bot_role_hill, 0, NONE);
		break;
	}
}

#else
/* (the campaign and online cartridges) */
static long bot_hidden_enemy(
	struct bot const *bot)
{
	(void)bot;
	return NONE;
}

#define bot_holds_objective(weapon_index) FALSE
#define bot_objective_target(bot, position) NONE
#define bot_weapon_range(bot) WEAPON_SEARCH_RANGE
#define bot_role_walks(bot, position) FALSE
#define bot_walk_objective(bot, unit, position, in_range, distance, move, moving, along_path, step) _bot_goal_fight
#define bot_near_pickup(bot, position) FALSE
#define bot_watch_own_flag(bot, position, eye, step) ((void)0)
#define bot_ride_done(bot, position) FALSE
#define objectives_update(now) ((void)0)
#endif
/* a better weapon lying near enough and in sight, or NONE: with two
carried a pickup is a swap that drops the weapon in hand (players.c,
player_handle_weapon_swap), so it must beat that one; with fewer, any
weapon the game would give it (an add, or a fresher copy of its own) */
static long find_better_weapon(
	struct bot *bot,
	struct unit_datum *unit,
	real_point3d const *eye,
	real range)
{
	struct object_iterator iterator;
	struct object_datum *object;
	short carried = 0, worst = 0, i;
	long in_hand = unit_inventory_get_weapon(bot->unit_index, unit->unit.current_weapon_index);
	boolean armed = in_hand != NONE;
	long best = NONE, second = NONE;
	real best_value = 0.f, second_value = 0.f;

	for (i = 0; i < MAXIMUM_WEAPONS_PER_UNIT; i++)
	{
		if (unit_inventory_get_weapon(bot->unit_index, i) != NONE)
			carried++;
	}
	if (carried >= 2 && armed && weapon_can_be_fired(in_hand))
		worst = weapon_score(in_hand);
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
		/* (none the game would not give it: a kind it carries, unless the
		one in its hands is older -- units.c, unit_approve_weapon_swap, as
		players.c offers a person a weapon on the ground) */
		if (armed && !unit_approve_weapon_swap(bot->unit_index, weapon_index))
			continue;
		distance = horizontal_distance(&unit->object.position, &object->object.position);
		if (distance > range || fabsf(object->object.position.z - unit->object.position.z) > 2.5f)
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

/* ---------- vehicles

A bot drives a vehicle that has no driver. In a team game it also rides
with a teammate (a person or a bot) at the wheel, boarding below 0.5 units a
second, a gunner's seat scored 3.5 more -- as Halo's AI, which joins a friendly
player driving within 10 units (actions.c, action_vehicle.c). In free-for-all
it never rides, it only drives. It never takes a vehicle a person is near or
walking to, nor one an opponent is in (units.c refuses that too). It gets in
as a person does: the action button at the seat the game offers it
(players.c), the vehicle upright and still, as Halo's AI asks (action_
vehicle.c). It drives with a person's stick and facing, which the vehicle
follows (units.c, vehicles.c), steering as Halo's AI does with the vehicle
tag's own fields (actor_moving.c). It gets out when the vehicle is flipped
(then turns it back over, as a person does) or stuck, when its target is out
of the vehicle's reach, or (a rider) when its driver has gone. The kinds it
knows are by the tag's vehicle_type: the Warthogs, the Ghost and the
Scorpion. The Banshee (it flies, and the bots' ways are on the ground) and
turrets it leaves alone */

enum
{
	_bot_vehicle_none = 0,
	_bot_vehicle_jeep, /* a Warthog: a driver, a gunner, a passenger */
	_bot_vehicle_scout, /* the Ghost: its driver fires */
	_bot_vehicle_tank /* the Scorpion: its driver fires, riders on top */
};

enum
{
	_bot_seat_none = 0,
	_bot_seat_driver,
	_bot_seat_gunner,
	_bot_seat_passenger
};

/* the vehicle tag's fields a bot reads: vehicles.c's vehicle_type, and the
AI's steering as actor_moving.c reads it */
struct bot_vehicle_definition
{
	byte __unknown0[0x2F4];
	short vehicle_type;
	byte __unknown2f6[0x3A0 - 0x2F6];
	real ai_steering_max_angle;
	real ai_steering_max_throttle;
};

typedef char n64_bot_vehicle_type_offset[offsetof(struct bot_vehicle_definition, vehicle_type) == 0x2F4 ? 1 : -1];
typedef char n64_bot_vehicle_steering_offset[
	offsetof(struct bot_vehicle_definition, ai_steering_max_angle) == 0x3A0 ? 1 : -1];

/* (units.c's; units.h has not got it, action_vehicle.c declares it too) */
boolean unit_get_seat_entrance_point(
	long unit_index,
	long parent_unit_index,
	short seat_index,
	real_point3d *entrance_point,
	real_point3d *seat_point,
	real_point3d *hint_point);

static struct bot_vehicle_definition *bot_vehicle_definition(
	long vehicle_index)
{
	return (struct bot_vehicle_definition *)tag_get(VEHICLE_DEFINITION_TAG, object_get(vehicle_index)->definition_index);
}

/* the kind of a vehicle, by its tag's vehicle_type (vehicles.c: 0 a tank,
1 a jeep, 4 an alien scout; not the boat, the plane, the alien fighter --
the Banshee -- or a turret) */
static short bot_vehicle_kind(
	long vehicle_index)
{
	struct object_datum *object = vehicle_index != NONE ? object_try_and_get(vehicle_index) : NULL;

	if (!object || object->object.type != _object_type_vehicle)
		return _bot_vehicle_none;
	switch (bot_vehicle_definition(vehicle_index)->vehicle_type)
	{
	case 0:
		return _bot_vehicle_tank;
	case 1:
		return _bot_vehicle_jeep;
	case 4:
		return _bot_vehicle_scout;
	}
	return _bot_vehicle_none;
}

/* a seat's role: the driver's (a Ghost's and a Scorpion's driver fires
too), a gunner's, or a passenger's */
static short bot_seat_role(
	long vehicle_index,
	short seat_index)
{
	struct unit_definition *definition = unit_definition_get(object_get(vehicle_index)->definition_index);
	struct unit_seat *seat;

	if (seat_index < 0 || seat_index >= definition->unit.seats.count)
		return _bot_seat_none;
	seat = TAG_BLOCK_GET_ELEMENT(&definition->unit.seats, seat_index, struct unit_seat);
	if (TEST_FLAG(seat->flags, _unit_seat_driver_bit))
		return _bot_seat_driver;
	return TEST_FLAG(seat->flags, _unit_seat_gunner_bit) ? _bot_seat_gunner : _bot_seat_passenger;
}

/* a unit of a player on the bot's team, in a team game (in free-for-all
everyone else is an opponent) */
static boolean bot_teammate(
	struct bot *bot,
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);

	return game_engine_has_teams() && unit->unit.player_index != NONE &&
		player_get(unit->unit.player_index)->team_index == player_get(bot->player_index)->team_index;
}

/* who is in a vehicle: its driver (NONE without), a bit for each seat
taken, whether an opponent of the bot is in it */
struct bot_riders
{
	long driver;
	unsigned long seats;
	boolean opponent;
};

static void bot_vehicle_riders(
	struct bot *bot,
	long vehicle_index,
	struct bot_riders *riders)
{
	long child = object_get(vehicle_index)->object.first_child_object_index;

	riders->driver = NONE;
	riders->seats = 0;
	riders->opponent = FALSE;
	while (child != NONE)
	{
		struct object_datum *object = object_get(child);

		if (TEST_FLAG(_object_mask_unit, object->object.type) && child != bot->unit_index)
		{
			short seat_index = unit_get(child)->unit.parent_seat_index;

			if (seat_index >= 0 && seat_index < 32)
				riders->seats |= 1UL << seat_index;
			if (bot_seat_role(vehicle_index, seat_index) == _bot_seat_driver)
				riders->driver = child;
			if (!bot_teammate(bot, child))
				riders->opponent = TRUE;
		}
		child = object->object.next_object_index;
	}
}

/* a vehicle a bot may get into: one of the kinds it knows, not destroyed,
open to players, standing free, upright (Halo's AI: up.k at least 0.5,
action_vehicle.c) */
static boolean bot_vehicle_usable(
	long vehicle_index)
{
	struct unit_datum *vehicle;

	if (bot_vehicle_kind(vehicle_index) == _bot_vehicle_none)
		return FALSE;
	vehicle = unit_get(vehicle_index);
	return !TEST_FLAG(vehicle->object.damage_flags, _object_dead_bit) &&
		!TEST_FLAG(vehicle->unit.flags, _unit_not_enterable_by_player_bit) &&
		vehicle->object.parent_object_index == NONE && vehicle->object.up.k >= 0.5f;
}

/* still: under half a unit a second (Halo's AI: (1/60)^2 a tick,
action_vehicle.c) */
static boolean bot_vehicle_still(
	long vehicle_index)
{
	return magnitude_squared3d(&object_get(vehicle_index)->object.translational_velocity) <
		1.f / (60.f * 60.f);
}

/* whether a person (a player on a controller) on foot is within 4 units of
a vehicle, or within 15 and walking to it: the vehicle is left to them */
static boolean bot_vehicle_persons(
	long vehicle_index)
{
	real_point3d const *at = &object_get(vehicle_index)->object.position;
	short local_player_index;

	for (local_player_index = 0; local_player_index < MAXIMUM_LOCAL_PLAYERS; local_player_index++)
	{
		long player_index = local_player_get_player_index(local_player_index);
		long unit_index = player_index != NONE ? player_get(player_index)->unit_index : NONE;
		struct unit_datum *unit;
		real dx, dy, d, vi, vj, speed;

		if (!unit_alive(unit_index))
			continue;
		unit = unit_get(unit_index);
		if (unit->object.parent_object_index != NONE)
			continue;
		dx = at->x - unit->object.position.x;
		dy = at->y - unit->object.position.y;
		d = square_root(dx * dx + dy * dy);
		if (d < 4.f)
			return TRUE;
		vi = unit->object.translational_velocity.i;
		vj = unit->object.translational_velocity.j;
		speed = square_root(vi * vi + vj * vj);
		/* (over a unit a second -- its velocity is a tick's -- within ~37
		degrees of the way to it) */
		if (d < 15.f && speed > 1.f / TICKS_PER_SECOND && vi * dx + vj * dy > 0.8f * speed * d)
			return TRUE;
	}
	return FALSE;
}

/* whether a bot sits in a vehicle's driver seat now: a seat with the
driver flag, as bot_vehicle_riders reads a vehicle's driver */
static boolean bot_in_driver_seat(
	struct bot const *bot)
{
	long parent = object_get(bot->unit_index)->object.parent_object_index;

	return parent != NONE && object_get(parent)->object.type == _object_type_vehicle &&
		bot_seat_role(parent, unit_get(bot->unit_index)->unit.parent_seat_index) == _bot_seat_driver;
}

/* the bots driving or going to drive, but this one */
static short bot_drivers(
	struct bot *self)
{
	short i, count = 0;

	for (i = 0; i < bots.created; i++)
	{
		struct bot *bot = &bots.bots[i];

		if (bot == self || !unit_alive(bot->unit_index))
			continue;
		if (bot_in_driver_seat(bot) || (bot->goal_vehicle != NONE && bot->goal_role == _bot_seat_driver))
		{
			count++;
		}
	}
	return count;
}

/* the seat a vehicle's driver sits in, or NONE */
static short bot_driver_seat(
	long vehicle_index)
{
	struct unit_definition *definition = unit_definition_get(object_get(vehicle_index)->definition_index);
	short seat_index;

	for (seat_index = 0; seat_index < definition->unit.seats.count && seat_index < 32; seat_index++)
	{
		if (bot_seat_role(vehicle_index, seat_index) == _bot_seat_driver)
			return seat_index;
	}
	return NONE;
}

/* whether a vehicle has a free seat that is not a driver's */
static boolean bot_rider_seat_free(
	long unit_index,
	long vehicle_index,
	unsigned long taken)
{
	struct unit_definition *definition = unit_definition_get(object_get(vehicle_index)->definition_index);
	short seat_index;

	for (seat_index = 0; seat_index < definition->unit.seats.count && seat_index < 32; seat_index++)
	{
		real_point3d entrance, seat;

		/* (one a biped of the driver's kind could get into: the game's own
		test, as bot_ride_seat makes it) */
		if (!(taken & (1UL << seat_index)) && bot_seat_role(vehicle_index, seat_index) != _bot_seat_driver &&
			unit_get_seat_entrance_point(unit_index, vehicle_index, seat_index, &entrance, &seat, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/* the seat to ride in a vehicle its teammate drives: a free one that is not
a driver's, by Halo's weight (action_vehicle.c: 10/(d+1), 3.5 more for a
gunner's), or NONE */
static short bot_ride_seat(
	struct bot *bot,
	long vehicle_index,
	unsigned long taken,
	real_point3d const *position)
{
	struct unit_definition *definition = unit_definition_get(object_get(vehicle_index)->definition_index);
	short seat_index, best = NONE;
	real best_weight = 0.f;

	for (seat_index = 0; seat_index < definition->unit.seats.count && seat_index < 32; seat_index++)
	{
		short role = bot_seat_role(vehicle_index, seat_index);
		real_point3d entrance, seat;
		real d, weight;

		if (role == _bot_seat_driver || (taken & (1UL << seat_index)) ||
			!unit_get_seat_entrance_point(bot->unit_index, vehicle_index, seat_index, &entrance, &seat, NULL))
		{
			continue;
		}
		d = MIN(horizontal_distance(position, &entrance), horizontal_distance(position, &seat));
		weight = 10.f / (d + 1.f) + (role == _bot_seat_gunner ? 3.5f : 0.f);
		if (weight > best_weight)
		{
			best_weight = weight;
			best = seat_index;
		}
	}
	return best;
}

/* after a give-up, no seat of that goal's kind (a driver's, or a rider's)
chosen for VEHICLE_RETRY_TICKS of on-foot time: an unreachable seat is not
tried again at once, and a teammate waiting at the wheel can still be ridden
with after a driver goal is given up */
static void bot_pause_vehicle_kind(
	struct bot *bot)
{
	if (bot->goal_role == _bot_seat_driver)
		bot->driver_pause = VEHICLE_RETRY_TICKS;
	else
		bot->rider_pause = VEHICLE_RETRY_TICKS;
}

/* the seats of a vehicle that are not a passenger's (the driver's, a
gunner's): a carrier, which rides only as a passenger, takes none */
static unsigned long bot_unlike_seats(
	long vehicle_index)
{
	struct unit_definition *definition = unit_definition_get(object_get(vehicle_index)->definition_index);
	unsigned long seats = 0;
	short seat_index;

	for (seat_index = 0; seat_index < definition->unit.seats.count && seat_index < 32; seat_index++)
	{
		if (bot_seat_role(vehicle_index, seat_index) != _bot_seat_passenger)
			seats |= 1UL << seat_index;
	}
	return seats;
}

/* a vehicle to get into, looked for now and then on foot: a teammate's
almost still one first (team games), else the nearest with no driver
(fewer than MAXIMUM_BOT_DRIVERS bots driving) -- in a fight, only a
teammate's (Halo's AI joins a player's vehicle in combat too: actions.c) */
static void find_vehicle(
	struct bot *bot,
	real_point3d const *position,
	boolean fighting)
{
	struct object_iterator iterator;
	struct object_datum *object;
	boolean teams = game_engine_has_teams();
	/* (a kind of seat it gave up a while ago, or a driver's with enough
	bots driving, is not looked for) */
	boolean may_ride = teams && bot->rider_pause <= 0;
	boolean may_drive = bot->driver_pause <= 0 && bot_drivers(bot) < MAXIMUM_BOT_DRIVERS;
	/* (its role's way: a carrier rides only as a passenger; a defender, a
	returner and a bot near its hill walk; a defender and a returner ride
	with nobody) */
	boolean passenger_only = bot->role == _bot_role_carry;

	if (bot_role_walks(bot, position))
		may_drive = FALSE;
	if (bot->role == _bot_role_defend || bot->role == _bot_role_return)
		may_ride = FALSE;
	long best = NONE;
	short best_seat = NONE, best_role = _bot_seat_none;
	real best_value = 1e9f;

	if (!may_drive && !may_ride)
		return;
	object_iterator_new(&iterator, _object_mask_vehicle, 0);
	while ((object = object_iterator_next(&iterator)) != NULL)
	{
		long vehicle_index = iterator.index;
		struct bot_riders riders;
		short seat_index, role;
		real d, value;

		if (vehicle_index == bot->given_up_vehicle || !bot_vehicle_usable(vehicle_index) ||
			!bot_vehicle_still(vehicle_index))
		{
			continue;
		}
		d = horizontal_distance(position, &object->object.position);
		if (d > VEHICLE_SEARCH_RANGE || fabsf(object->object.position.z - position->z) > 4.f)
			continue;
		bot_vehicle_riders(bot, vehicle_index, &riders);
		if (riders.opponent || bot_vehicle_persons(vehicle_index))
			continue;
		/* (a seat another bot goes for counts as taken) */
		{
			short i;

			for (i = 0; i < bots.created; i++)
			{
				if (&bots.bots[i] != bot && unit_alive(bots.bots[i].unit_index) &&
					bots.bots[i].goal_vehicle == vehicle_index && bots.bots[i].goal_seat >= 0 &&
					bots.bots[i].goal_seat < 32)
				{
					riders.seats |= 1UL << bots.bots[i].goal_seat;
				}
			}
		}
		if (riders.driver == NONE)
		{
			if (fighting || !may_drive || (seat_index = bot_driver_seat(vehicle_index)) == NONE ||
				(riders.seats & (1UL << seat_index)))
			{
				continue;
			}
			role = _bot_seat_driver;
			value = d;
		}
		else
		{
			/* (a rider only in a team game, with its teammate at the wheel
			and the vehicle almost still, as above) */
			if (!may_ride || d > VEHICLE_RIDE_RANGE ||
				(seat_index = bot_ride_seat(bot, vehicle_index,
					riders.seats | (passenger_only ? bot_unlike_seats(vehicle_index) : 0), position)) == NONE)
			{
				continue;
			}
			role = bot_seat_role(vehicle_index, seat_index);
			value = d - VEHICLE_SEARCH_RANGE;
		}
		if (value < best_value)
		{
			best_value = value;
			best = vehicle_index;
			best_seat = seat_index;
			best_role = role;
		}
	}
	bot->goal_vehicle = best;
	bot->goal_seat = best_seat;
	bot->goal_role = best_role;
	bot->vehicle_ticks = 0;
	bot->door_timer = 0;
	bot->goal_still_ticks = 0;
	bot->goal_best_distance = 1e9f;
}

/* a vehicle it no longer goes for (or got out of): left alone a while */
static void bot_give_up_vehicle(
	struct bot *bot,
	long vehicle_index)
{
	bot->given_up_vehicle = vehicle_index;
	bot->given_up_vehicle_timer = VEHICLE_GIVE_UP_TICKS;
	if (bot->goal_vehicle == vehicle_index)
		bot->goal_vehicle = NONE;
}

/* on its way to the vehicle it goes for: still to be had (else given up),
the way to its seat's door, and there the action button once the game
offers it that seat (players.c), as it offers it a person: TRUE while it
goes */
static boolean bot_go_to_vehicle(
	struct bot *bot,
	struct player_datum *player,
	real_point3d const *position,
	real_vector2d *move,
	boolean *moving,
	unsigned long *flags,
	short step)
{
	long vehicle_index = bot->goal_vehicle;
	struct bot_riders riders;
	real_point3d entrance, seat;
	real dx, dy, length;
	boolean wanted;

	if (!bot_vehicle_usable(vehicle_index))
	{
		bot->goal_vehicle = NONE;
		return FALSE;
	}
	bot_vehicle_riders(bot, vehicle_index, &riders);
	/* (still to be had: free, with no opponent, nobody's at the wheel to
	drive it -- or its teammate driver's still there and near -- and no
	person coming for it; not tried too long) */
	wanted = !riders.opponent && !(riders.seats & (1UL << bot->goal_seat)) && !bot_vehicle_persons(vehicle_index) &&
		(bot->vehicle_ticks += step) <= VEHICLE_ENTRY_TICKS;
	if (bot->goal_role == _bot_seat_driver)
		wanted = wanted && riders.driver == NONE;
	else
	{
		wanted = wanted && riders.driver != NONE &&
			horizontal_distance(position, &object_get(vehicle_index)->object.position) <= VEHICLE_RIDE_RANGE &&
			magnitude_squared3d(&object_get(vehicle_index)->object.translational_velocity) <
				(2.f / TICKS_PER_SECOND) * (2.f / TICKS_PER_SECOND);
	}
	/* (its door worked out again each half second: a still vehicle's door
	does not move, and the marker search is dear) */
	if (wanted && (bot->door_timer -= step) <= 0)
	{
		wanted = unit_get_seat_entrance_point(bot->unit_index, vehicle_index, bot->goal_seat, &bot->goal_door, &seat, NULL);
		bot->door_timer = 15;
	}
	if (!wanted)
	{
		bot_pause_vehicle_kind(bot);
		bot_give_up_vehicle(bot, vehicle_index);
		return FALSE;
	}
	entrance = bot->goal_door;
	dx = entrance.x - position->x;
	dy = entrance.y - position->y;
	length = square_root(dx * dx + dy * dy);
	/* (no nearer the door by a unit, and no press at it, in VEHICLE_
	PROGRESS_TICKS of its thoughts: given up, and no seat of its kind chosen
	for a while) */
	if (length < bot->goal_best_distance - 1.f)
	{
		bot->goal_best_distance = length;
		bot->goal_still_ticks = 0;
	}
	else if ((bot->goal_still_ticks += step) >= VEHICLE_PROGRESS_TICKS)
	{
		bot_pause_vehicle_kind(bot);
		bot_give_up_vehicle(bot, vehicle_index);
		return FALSE;
	}
	*moving = length > 0.3f;
	{
		/* (from the vehicle's far side, round it first: towards a point off
		its hull between the bot's side and the door's, so that it does not
		walk into the hull) */
		struct object_datum *object = object_get(vehicle_index);
		real_point3d const *center = &object->object.bounding_sphere_center;
		real si = position->x - center->x, sj = position->y - center->y;
		real oi = entrance.x - center->x, oj = entrance.y - center->y;
		real side = square_root(si * si + sj * sj), out = square_root(oi * oi + oj * oj);
		real radius = object->object.bounding_sphere_radius;

		if (side > 0.01f && out > 0.01f && side < radius + 3.f && (si * oi + sj * oj) < 0.5f * side * out)
		{
			real mi = si / side + oi / out, mj = sj / side + oj / out;
			real m = square_root(mi * mi + mj * mj);

			if (m < 0.1f)
			{
				/* (right behind it: round its left) */
				mi = -oj / out;
				mj = oi / out;
				m = 1.f;
			}
			dx = center->x + mi / m * (radius + 0.7f) - position->x;
			dy = center->y + mj / m * (radius + 0.7f) - position->y;
		}
	}
	{
		real way = square_root(dx * dx + dy * dy);

		if (way > 0.01f)
		{
			move->i = dx / way;
			move->j = dy / way;
		}
	}
	if (length < 1.2f)
	{
		if (bot->action_timer > 0)
			bot->action_timer -= step;
		else if (player->action_result == _player_action_result_enter_vehicle &&
			player->action_object_index == vehicle_index && player->action_seat_index == bot->goal_seat &&
			(bot->goal_role == _bot_seat_driver || bot_vehicle_still(vehicle_index)))
		{
			*flags |= FLAG(_unit_control_action_bit);
			bot->action_timer = 20;
			bot->goal_still_ticks = 0;
		}
	}
	return TRUE;
}

/* a vehicle it got out of flipped: turned back over (the action button by
it, as a person does: players.c), a few seconds at most; TRUE while at it */
static boolean bot_flip_vehicle(
	struct bot *bot,
	struct player_datum *player,
	real_point3d const *position,
	real_vector2d *move,
	boolean *moving,
	unsigned long *flags,
	short step)
{
	struct object_datum *object = object_try_and_get(bot->flip_vehicle);
	real dx, dy, length;

	if (!object || object->object.type != _object_type_vehicle ||
		TEST_FLAG(object->object.damage_flags, _object_dead_bit) || (bot->flip_timer -= step) <= 0)
	{
		bot->flip_vehicle = NONE;
		return FALSE;
	}
	if (object->object.up.k >= 0.5f)
	{
		/* (over again: its to take once more) */
		if (bot->given_up_vehicle == bot->flip_vehicle)
			bot->given_up_vehicle = NONE;
		bot->flip_vehicle = NONE;
		return FALSE;
	}
	dx = object->object.position.x - position->x;
	dy = object->object.position.y - position->y;
	length = square_root(dx * dx + dy * dy);
	if (length > 0.01f)
	{
		move->i = dx / length;
		move->j = dy / length;
	}
	*moving = length > 1.5f;
	if (bot->action_timer > 0)
		bot->action_timer -= step;
	else if (player->action_result == _player_action_result_flip_vehicle &&
		player->action_object_index == bot->flip_vehicle)
	{
		*flags |= FLAG(_unit_control_action_bit);
		bot->action_timer = 20;
	}
	return TRUE;
}

/* in a seat: whether it stays (else it gets out). It stays in a driver's
seat, or (in a team game) a rider's, of a kind it knows; it leaves any other
seat it finds itself in (an unknown kind, an invalid seat, a rider's in
free-for-all). The first thought in a seat sets its driving up: a
driver of a Warthog or a Scorpion in a team game waits up to 4 s for a
teammate to ride */
static boolean bot_seat_kept(
	struct bot *bot,
	struct unit_datum *unit,
	long vehicle_index)
{
	/* (the vehicle's kind and the seat's role once, as it sits in that seat
	of that vehicle: neither changes while it sits there) */
	boolean const same_seat = bot->seat_role != _bot_seat_none && vehicle_index == bot->seat_vehicle &&
		unit->unit.parent_seat_index == bot->seat_index;
	short kind = same_seat ? bot->seat_kind : bot_vehicle_kind(vehicle_index);
	short role = same_seat ? bot->seat_role : kind == _bot_vehicle_none ? _bot_seat_none :
		bot_seat_role(vehicle_index, unit->unit.parent_seat_index);
	long gun;

	if (kind == _bot_vehicle_none || role == _bot_seat_none ||
		(role != _bot_seat_driver && !game_engine_has_teams()))
	{
		return FALSE;
	}
	/* (its gun's way of firing, again whenever its gun changes: rockets and
	the Scorpion's cannon pressed, never up close; the Ghost's held, near;
	a chain gun held) */
	gun = unit_inventory_get_weapon(vehicle_index, unit_get(vehicle_index)->unit.current_weapon_index);
	if (!same_seat || gun != bot->seat_weapon)
	{
		bot->seat_weapon = gun;
		bot->seat_gun = (short)(gun == NONE ? NONE : kind == _bot_vehicle_tank ||
			strstr(tag_get_name(object_get(vehicle_index)->definition_index), "rwarthog") ||
			strstr(tag_get_name(object_get(gun)->definition_index), "rocket") ? 2 : kind == _bot_vehicle_scout ? 1 : 0);
	}
	if (!same_seat)
	{
		bot->seat_role = role;
		bot->sight_timer = 0;
		bot->action_timer = 0;
		bot->seat_kind = kind;
		bot->seat_vehicle = vehicle_index;
		bot->seat_index = unit->unit.parent_seat_index;
		bot->goal_vehicle = NONE;
		bot->goal_weapon = NONE;
		bot->upset_ticks = bot->reach_ticks = 0;
		bot->drive_check = TICKS_PER_SECOND;
		bot->drive_path = 0.f;
		bot->drive_stuck = bot->stuck_events = bot->stuck_window = bot->reverse_timer = bot->detour_timer = 0;
		object_get_origin(bot->unit_index, &bot->drive_position);
		bot->circle = (short)(bot_random() & 1 ? 1 : -1);
		bot->wait_ticks = (short)(role == _bot_seat_driver && kind != _bot_vehicle_scout && game_engine_has_teams() ?
			4 * TICKS_PER_SECOND : 0);
	}
	return TRUE;
}

/* a teammate on foot within 10 units, the one a driver waits for */
static boolean bot_teammate_on_foot_near(
	struct bot *bot,
	real_point3d const *position)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		if (iterator.datum_index == bot->player_index || !unit_alive(player->unit_index) ||
			!bot_teammate(bot, player->unit_index) ||
			object_get(player->unit_index)->object.parent_object_index != NONE)
		{
			continue;
		}
		if (horizontal_distance(position, &object_get(player->unit_index)->object.position) < VEHICLE_RIDE_RANGE)
			return TRUE;
	}
	return FALSE;
}

/* a seat change seen on its first thought after it (in, out, or into
another vehicle): nothing carries over from before it -- no walking path or
path search of its own, no walking recovery (a turn, a jump), its movement
measured from here, its sight tested afresh, its action button free */
static void bot_seat_changed(
	struct bot *bot,
	struct unit_datum *unit,
	long vehicle_index)
{
	bot->seen_parent = vehicle_index;
	bot->path_valid = FALSE;
	bot->path_age = 0;
	if (bots.search_owner == (short)(bot - bots.bots) + 1)
	{
		bots.search_owner = 0;
		bots.path_cooldown = TICKS_BETWEEN_PATHS;
	}
	bot->stuck_timer = bot->unstick_timer = bot->jump_timer = 0;
	/* (a side walk or a driver's turn does not go on across a seat change) */
	bot->side_timer = 0;
	bot->stuck_check = 16;
	bot->last_position = unit->object.position;
	bot->sight_timer = 0;
	bot->action_timer = 0;
}

/* why a driver gets out */
enum
{
	_bot_drive_on = 0,
	_bot_drive_flipped,
	_bot_drive_stuck,
	_bot_drive_out_of_reach,
};


/* a driver's stick and facing (the vehicle turns to its driver's facing:
units.c, vehicles.c). Its goal is its target: a Warthog with no gunner
drives at it (to run it over), with a gunner circles it some 10 units off; a
Ghost or a Scorpion turns to its aim (facing, in), closes to its gun's range
and holds or strafes there. Steering as Halo's AI (actor_moving.c): the way
to the goal at most three times as far round, at most the tag's
ai_steering_max_angle off the nose, at the tag's ai_steering_max_throttle.
Stuck (under a unit in a second, twice running): it backs up 1.5 s, then
goes 1.5 s some 60 degrees off its way; stuck three times in 10 s, flipped a
second, or its target out of reach (beside it with the vehicle stopped, or
well above it) a second: out (the reason) */
static short bot_drive(
	struct bot *bot,
	long vehicle_index,
	short kind,
	real_point3d const *position,
	real distance,
	struct bot_weapon const *weapon,
	real *forward,
	real *strafe,
	real_vector3d *facing,
	boolean *detoured,
	short step)
{
	struct unit_datum *vehicle = unit_get(vehicle_index);
	struct bot_vehicle_definition *definition = bot_vehicle_definition(vehicle_index);
	real throttle = definition->ai_steering_max_throttle > 0.f ? MIN(definition->ai_steering_max_throttle, 1.f) : 1.f;
	real speed_squared = magnitude_squared3d(&vehicle->object.translational_velocity);
	boolean target = bot->target_unit != NONE && unit_alive(bot->target_unit);
	boolean gunner = FALSE, reversing;
	real_point3d goal;

	*forward = *strafe = 0.f;
	*detoured = FALSE;
	/* (the stuck spells' window runs on, driving or not) */
	if (bot->stuck_window > 0 && (bot->stuck_window -= step) <= 0)
		bot->stuck_events = 0;
	if (vehicle->object.up.k < 0.5f)
	{
		bot->reach_ticks = bot->drive_stuck = 0;
		if ((bot->upset_ticks += step) >= TICKS_PER_SECOND)
			return _bot_drive_flipped;
		return _bot_drive_on;
	}
	bot->upset_ticks = 0;
	/* (stuck three times: out, until it is) */
	if (bot->stuck_events >= 3)
		return _bot_drive_stuck;
	if (kind == _bot_vehicle_jeep)
	{
		struct bot_riders riders;
		short seat_index;

		bot_vehicle_riders(bot, vehicle_index, &riders);
		for (seat_index = 0; seat_index < 32 && !gunner; seat_index++)
			gunner = (riders.seats & (1UL << seat_index)) && bot_seat_role(vehicle_index, seat_index) == _bot_seat_gunner;
	}
	/* waiting for a teammate to ride (team games, a while after getting in) */
	if (bot->wait_ticks > 0)
	{
		struct bot_riders riders;

		bot->wait_ticks -= step;
		bot_vehicle_riders(bot, vehicle_index, &riders);
		if (bot_rider_seat_free(bot->unit_index, vehicle_index, riders.seats) && bot_teammate_on_foot_near(bot, position) &&
			!(bot->target_visible && distance < 10.f))
		{
			if (kind == _bot_vehicle_jeep)
				*facing = vehicle->object.forward;
			bot->reach_ticks = bot->drive_stuck = 0;
			return _bot_drive_on;
		}
		bot->wait_ticks = 0;
	}
	if (!target)
	{
		bot->reach_ticks = bot->drive_stuck = 0;
		if (kind == _bot_vehicle_jeep)
			*facing = vehicle->object.forward;
		return _bot_drive_on;
	}
	object_get_origin(bot->target_unit, &goal);
	/* its target out of the vehicle's reach a second: beside it with the
	vehicle stopped (a Warthog that cannot run it over), or above it on a
	ledge (on a base, up a cliff) */
	if ((kind == _bot_vehicle_jeep && !gunner && distance < 4.f && speed_squared < 1.f / (30.f * 30.f)) ||
		(distance < 8.f && goal.z - position->z > 2.5f))
	{
		if ((bot->reach_ticks += step) >= TICKS_PER_SECOND)
			return _bot_drive_out_of_reach;
	}
	else
		bot->reach_ticks = 0;
	if (kind == _bot_vehicle_jeep)
	{
		real gi = goal.x - position->x, gj = goal.y - position->y;
		real ni = vehicle->object.forward.i, nj = vehicle->object.forward.j;
		real goal_length = square_root(gi * gi + gj * gj), nose_length = square_root(ni * ni + nj * nj);
		real angle, steer;

		/* (with a gunner, a point some 10 units to the target's side: it
		goes round it) */
		if (gunner && goal_length > 0.01f && goal_length < 18.f)
		{
			goal.x -= gj / goal_length * 10.f * bot->circle;
			goal.y += gi / goal_length * 10.f * bot->circle;
			gi = goal.x - position->x;
			gj = goal.y - position->y;
			goal_length = square_root(gi * gi + gj * gj);
		}
		if (goal_length < 0.01f || nose_length < 0.01f)
		{
			*facing = vehicle->object.forward;
			return _bot_drive_on;
		}
		gi /= goal_length;
		gj /= goal_length;
		ni /= nose_length;
		nj /= nose_length;
		angle = atan2f(ni * gj - nj * gi, ni * gi + nj * gj);
		steer = fabsf(angle);
		if (definition->ai_steering_max_angle > 0.f)
			steer = MIN(steer * 3.f, definition->ai_steering_max_angle);
		if (angle < 0.f)
			steer = -steer;
		facing->i = ni * cosf(steer) - nj * sinf(steer);
		facing->j = ni * sinf(steer) + nj * cosf(steer);
		facing->k = 0.f;
		*forward = throttle;
	}
	else
	{
		/* (its aim steers it: closer to its gun's range, back when too close,
		a Ghost strafing there) */
		if (distance > weapon->far_range)
			*forward = throttle;
		else if (distance < weapon->near_range)
			*forward = -0.6f * throttle;
		else if (kind == _bot_vehicle_scout)
		{
			if ((bot->strafe_timer -= step) <= 0)
			{
				bot->strafe_timer = (short)(20 + bot_random() % 45);
				bot->strafe_direction = (short)(bot_random() & 1 ? 1 : -1);
			}
			*strafe = 0.8f * bot->strafe_direction;
		}
	}
	/* stuck: backing up a while (still steering for the goal, which
	vehicles.c turns round when it goes backwards), then 1.5 s some 60
	degrees off the way to the goal, to a side picked at random, round what
	stopped it -- the phase taken once, before its timers count down: it
	decides the stick, the stuck check and (bot_drive's caller) the facing */
	reversing = bot->reverse_timer > 0;
	*detoured = !reversing && bot->detour_timer > 0;
	if (reversing)
	{
		if ((bot->reverse_timer -= step) <= 0)
		{
			bot->detour_timer = 45;
			bot->circle = (short)(bot_random() & 1 ? 1 : -1);
		}
		*forward = -throttle;
		*strafe = 0.f;
	}
	else if (*detoured)
	{
		real gi = goal.x - position->x, gj = goal.y - position->y;
		real goal_length = square_root(gi * gi + gj * gj);

		bot->detour_timer -= step;
		if (goal_length > 0.01f)
		{
			real c = cosf(1.f), s = sinf(1.f) * bot->circle;

			gi /= goal_length;
			gj /= goal_length;
			facing->i = gi * c - gj * s;
			facing->j = gi * s + gj * c;
			facing->k = 0.f;
		}
		*forward = throttle;
		*strafe = 0.f;
	}
	bot->drive_path += square_root(vehicle->object.translational_velocity.i * vehicle->object.translational_velocity.i +
		vehicle->object.translational_velocity.j * vehicle->object.translational_velocity.j) * step;
	if ((bot->drive_check -= step) <= 0)
	{
		real moved = horizontal_distance(position, &bot->drive_position);
		real path = bot->drive_path;

		bot->drive_check = TICKS_PER_SECOND;
		bot->drive_position = *position;
		bot->drive_path = 0.f;
		if (!reversing && (fabsf(*forward) > 0.3f || (fabsf(*strafe) > 0.3f && path < DRIVE_STRAFE_BLOCK)) &&
			moved < 1.f)
		{
			if (++bot->drive_stuck >= 2)
			{
				bot->drive_stuck = 0;
				bot->reverse_timer = 45;
				bot->stuck_window = 10 * TICKS_PER_SECOND;
				if (++bot->stuck_events >= 3)
					return _bot_drive_stuck;
			}
		}
		else
			bot->drive_stuck = 0;
	}
	return _bot_drive_on;
}

/* out of its vehicle (the action button, once a second until it is out:
its count runs on every thought in a seat, think), which it leaves alone a
while; one flipped it turns back over once out */
static void bot_leave_vehicle(
	struct bot *bot,
	long vehicle_index,
	boolean flipped,
	unsigned long *flags)
{
	if (bot->action_timer <= 0)
	{
		*flags |= FLAG(_unit_control_action_bit);
		bot->action_timer = TICKS_PER_SECOND;
	}
	bot_give_up_vehicle(bot, vehicle_index);
	if (flipped && bot->flip_vehicle != vehicle_index)
	{
		bot->flip_vehicle = vehicle_index;
		bot->flip_timer = 4 * TICKS_PER_SECOND;
	}
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
		/* (its role, but a carrier's, and what it knew of its own flag) */
		short role = bot->role == _bot_role_carry ? _bot_role_none : bot->role, role_rank = bot->role_rank;
		long role_unit = bot->role_unit, flag_known_time = bot->flag_known_time, flag_known_unit = bot->flag_known_unit;
		real_point3d flag_known = bot->flag_known;

		csmemset(bot, 0, sizeof(*bot));
		bot->ghost_profile = ghost_profile;
		bot->role = role;
		bot->role_rank = role_rank;
		bot->role_unit = role_unit;
		bot->role_surface = NONE;
		bot->flag_known = flag_known;
		bot->flag_known_time = flag_known_time;
		bot->flag_known_unit = flag_known_unit;
		objectives.next_share = 0;
		bot->player_index = player_index;
		bot->unit_index = player->unit_index;
		bot->target_unit = NONE;
		bot->goal_weapon = NONE;
		bot->given_up_weapon = NONE;
		bot->goal_vehicle = NONE;
		bot->given_up_vehicle = NONE;
		bot->flip_vehicle = NONE;
		bot->seen_parent = NONE;
		bot->seen_seat = NONE;
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
	real_point3d eye, target_eye, target_position, position;
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
	/* a vehicle's gun: held (a Warthog's chain gun; the Ghost's, whose
	driver keeps near), or pressed and never up close (rockets, the
	Scorpion's cannon) */
	static struct bot_weapon const vehicle_guns[3] =
	{
		{ "", 0, _style_auto, 2.f, 25.f },
		{ "", 0, _style_auto, 5.f, 14.f },
		{ "", 0, _style_explosive, 8.f, 30.f }
	};
	/* the vehicle it rides (NONE on foot), its kind, whether it fires the
	vehicle's gun, the way it drives */
	long vehicle_index = unit->object.parent_object_index;
	short kind = _bot_vehicle_none;
	boolean vehicle_gun = FALSE, to_vehicle = FALSE, boarding = FALSE, fighting;
	real_vector3d steering = { 0.f, 0.f, 0.f };
	/* what its role makes of this thought (the objectives section) */
	short objective = _bot_goal_fight;

	bot_begin_life(bot, player, unit);
	/* (where it truly is: a rider's own position is its seat's, in its
	vehicle -- objects.c, object_get_origin) */
	object_get_origin(player->unit_index, &position);
	/* (a seat change since its last thought; in a seat, its action count
	runs on every thought, so an exit press is never late) */
	if (vehicle_index != bot->seen_parent)
		bot_seat_changed(bot, unit, vehicle_index);
	else if (vehicle_index != NONE && bot->action_timer > 0)
		bot->action_timer -= step;
	bot->seen_seat = unit->unit.parent_seat_index;

	/* in a vehicle: a seat it uses (bot_seat_kept), else out (the action
	button); out of a destroyed one too (Halo does not take its riders out:
	units.c, unit_died) */
	if (vehicle_index != NONE &&
		(TEST_FLAG(object_get(vehicle_index)->object.damage_flags, _object_dead_bit) ||
		!bot_seat_kept(bot, unit, vehicle_index)))
	{
		bot->shot_ticks = 0;
		bot->charging = FALSE;
		/* (no vehicle goal in a seat it leaves: no claim on another seat,
		and that vehicle left alone a while, as after any exit; no seat
		kept, so a seat it keeps later is set up afresh) */
		bot->goal_vehicle = NONE;
		bot_give_up_vehicle(bot, vehicle_index);
		bot->seat_role = _bot_seat_none;
		if (bot->action_timer <= 0)
		{
			flags |= FLAG(_unit_control_action_bit);
			bot->action_timer = TICKS_PER_SECOND;
		}
		action->control_flags = flags;
		euler_angles2d_from_vector3d(&action->desired_facing, &bot->aim);
		return;
	}
	/* (its eye after the seat checks: a seat's camera marker is read only
	in a seat it keeps) */
	unit_get_camera_position(player->unit_index, &eye);
	if (vehicle_index == NONE)
	{
		bot->seat_role = _bot_seat_none;
		bot->seat_vehicle = NONE;
	}
	else
		kind = bot->seat_kind;

	/* its gun: a gunner's, a Ghost's or a Scorpion's driver's is the
	vehicle's (units.c gives the vehicle its gunner's aim and trigger); a
	Warthog's driver has none; a passenger fires its own */
	if (vehicle_index != NONE && bot->seat_role != _bot_seat_passenger)
	{
		vehicle_gun = bot->seat_role == _bot_seat_gunner || kind != _bot_vehicle_jeep;
		current_weapon = vehicle_gun ?
			unit_inventory_get_weapon(vehicle_index, unit_get(vehicle_index)->unit.current_weapon_index) : NONE;
		weapon = current_weapon != NONE && bot->seat_gun != NONE ? &vehicle_guns[bot->seat_gun] : &unarmed;
	}
	else
	{
		current_weapon = unit_inventory_get_weapon(player->unit_index, unit->unit.current_weapon_index);
		entry = weapon_entry(current_weapon);
		weapon = entry == NONE ? &unarmed : &bot_weapons[entry];
	}

	/* what it knows of its own flag (CTF), before it chooses whom to fight */
	if (objectives.engine == game_engine_ctf)
		bot_watch_own_flag(bot, &position, &eye, step);
	/* whom to fight (now and then), and can it see them (a few times a
	second) */
	if ((bot->choose_timer -= step) <= 0 || !unit_alive(bot->target_unit))
	{
		/* (its role's first, else the nearest) */
		long target = bot->role != _bot_role_none ? bot_objective_target(bot, &position) : NONE;

		if (target == NONE)
			target = nearest_enemy(bot, &position);

		bot->choose_timer = 20;
		if (target != bot->target_unit)
		{
			bot->target_unit = target;
			/* (a driver's out-of-reach count; a rider counts its driver
			gone in it, which a new target does not restart) */
			if (bot->seat_role != _bot_seat_gunner && bot->seat_role != _bot_seat_passenger)
				bot->reach_ticks = 0;
			bot->sight_timer = 0;
			bot->path_valid = FALSE;
			bot->last_seen.x = bot->last_seen.y = bot->last_seen.z = 0.f;
		}
	}
	/* (CTF: its own flag's carrier, no longer known: another target, its
	way found afresh) */
	if (bot->target_unit != NONE && bot->target_unit == bot_hidden_enemy(bot))
	{
		bot->target_unit = nearest_enemy(bot, &position);
		bot->sight_timer = 0;
		bot->path_valid = FALSE;
		bot->last_seen.x = bot->last_seen.y = bot->last_seen.z = 0.f;
	}
	if (bot->target_unit != NONE)
	{
		/* (the object the ray leaves out: the bot itself, a rider's
		vehicle, or the bot's own vehicle -- and then a rider's vehicle is
		where the ray may end) */
		long sight_ignore = vehicle_index != NONE ? vehicle_index : player->unit_index;
		long sight_end = NONE;

		/* a rider: where it truly is (its own position is its seat's, in
		its vehicle: objects.c, object_get_origin), its body to aim at --
		Halo's AI fires at a target's centre of mass (actor_combat.c) -- and
		no stop at its own vehicle, which Halo's AI leaves out of its sight
		test (actor_perception.c, ai_test_line_of_sight) */
		if (object_get(bot->target_unit)->object.parent_object_index != NONE)
		{
			object_get_origin(bot->target_unit, &target_position);
			unit_get_center_of_mass(bot->target_unit, &target_eye);
			if (vehicle_index == NONE)
				sight_ignore = object_get_ultimate_parent(bot->target_unit);
			else
				sight_end = object_get_ultimate_parent(bot->target_unit);
		}
		else
		{
			unit_get_camera_position(bot->target_unit, &target_eye);
			target_position = object_get(bot->target_unit)->object.position;
		}
		if ((bot->sight_timer -= step) <= 0)
		{
			real range = horizontal_distance(&position, &target_position);

			/* (one hidden by its camouflage: no ray) */
			bot->sight_timer = 10;
			bot->target_visible = range < SIGHT_RANGE && !bot_camouflaged(bot->target_unit, range) &&
				n64_ai_clusters_may_see(player->unit_index, bot->target_unit) &&
				line_of_sight_past(&eye, &target_eye, sight_ignore, sight_end);
		}
		if (bot->target_visible || (bot->last_seen.x == 0.f && bot->last_seen.y == 0.f))
			bot->last_seen = target_eye;
		distance = horizontal_distance(&position, &target_position);
		in_range = bot->target_visible && distance <= weapon->far_range + 2.f;
	}
	else
		bot->target_visible = FALSE;

	/* a better weapon lying near, while there is no fight in sight (near
	its way while its role sends it somewhere; none with a flag or the ball
	in hand) */
	if (bot->direct_wait > 0)
		bot->direct_wait -= step;
	if (bot->given_up_timer > 0 && (bot->given_up_timer -= step) <= 0)
	{
		bot->given_up_timer = 0;
		bot->given_up_weapon = NONE;
	}
	if ((bot->target_visible && distance < 12.f) || vehicle_index != NONE || bot_weapon_range(bot) <= 0.f ||
		bot_holds_objective(current_weapon))
		bot->goal_weapon = NONE;
	else if ((bot->weapon_timer -= step) <= 0)
	{
		bot->weapon_timer = WEAPON_SEARCH_TICKS;
		if (bot->goal_weapon == NONE)
		{
			bot->goal_weapon = find_better_weapon(bot, unit, &eye, bot_weapon_range(bot));
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
			/* there: the swap button picks it up (it faces it too) once
			the game offers it, as it offers it a player (players.c sets the
			player's action result as it looks over the objects near) -- a
			player's held X (player_control.c), whose one press is enough
			here; the action button beside a weapon only reloads (players.c,
			player_handle_action) */
			if (length < 1.1f)
			{
				if (bot->action_timer > 0)
					bot->action_timer -= step;
				else if (player->action_object_index == bot->goal_weapon &&
					(player->action_result == _player_action_result_swap_for_weapon ||
					player->action_result == _player_action_result_add_weapon_to_inventory))
				{
					flags |= FLAG(_unit_control_swap_weapons_bit);
					bot->action_timer = 20;
				}
			}
		}
	}

	/* (a role that walks drops a driver's seat it was going for; a carrier
	keeps only a passenger's; a defender or a returner none) */
	if (bot->goal_vehicle != NONE && ((bot->goal_role == _bot_seat_driver && bot_role_walks(bot, &position)) ||
		(bot->role == _bot_role_carry && bot->goal_role != _bot_seat_passenger) ||
		bot->role == _bot_role_defend || bot->role == _bot_role_return))
	{
		bot->goal_vehicle = NONE;
	}
	/* a vehicle near, on foot with no weapon to go for (the vehicles
	section) -- in a fight in sight only a teammate's to ride, which it
	fights from as it goes -- or one it got out of flipped, turned back over */
	if (bot->given_up_vehicle_timer > 0 && (bot->given_up_vehicle_timer -= step) <= 0)
		bot->given_up_vehicle = NONE;
	fighting = bot->target_visible && distance < 12.f;
	if (vehicle_index != NONE || bot->goal_weapon != NONE ||
		(fighting && bot->goal_vehicle != NONE && bot->goal_role == _bot_seat_driver))
	{
		bot->goal_vehicle = NONE;
		if (vehicle_index == NONE)
			bot->flip_vehicle = NONE;
	}
	else
	{
		if (fighting)
			bot->flip_vehicle = NONE;
		if (bot->driver_pause > 0)
			bot->driver_pause -= step;
		if (bot->rider_pause > 0)
			bot->rider_pause -= step;
		if ((bot->vehicle_timer -= step) <= 0)
		{
			bot->vehicle_timer = VEHICLE_SEARCH_TICKS;
			if (bot->goal_vehicle == NONE && bot->flip_vehicle == NONE)
				find_vehicle(bot, &position, fighting);
		}
		if (bot->flip_vehicle != NONE)
			to_vehicle = bot_flip_vehicle(bot, player, &position, &move, &moving, &flags, step);
		else if (bot->goal_vehicle != NONE)
		{
			to_vehicle = bot_go_to_vehicle(bot, player, &position, &move, &moving, &flags, step);
			boarding = (flags & FLAG(_unit_control_action_bit)) != 0;
		}
	}

	/* where to go: along a path while the enemy is out of sight or out of
	range; in range, keep the weapon's range and move sideways (on foot) */
	if (bot->role != _bot_role_none && bot->goal_weapon == NONE && !to_vehicle && vehicle_index == NONE)
		objective = bot_walk_objective(bot, unit, &position, in_range, distance, &move, &moving, &along_path, step);
	if (objective == _bot_goal_fight && bot->goal_weapon == NONE && bot->target_unit != NONE && !to_vehicle &&
		vehicle_index == NONE)
	{
		if (!in_range)
		{
			bot->path_age += step;
			if (!bot->path_valid || bot->path_age > PATH_LIFETIME ||
				horizontal_distance(&bot->path_goal, &target_position) > 7.f)
			{
				plan_path(bot, bot->target_unit, NULL, NONE);
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
	else the way it is going -- a Ghost's or a Scorpion's driver, its aim
	steering it, to where its enemy truly is (as a bot on foot finds its way
	there) */
	if (bot->seat_role == _bot_seat_driver && kind != _bot_vehicle_jeep && bot->target_unit != NONE &&
		!bot->target_visible)
	{
		desired.i = target_position.x - position.x;
		desired.j = target_position.y - position.y;
		desired.k = 0.f;
		normalize(&desired);
	}
	else if (bot->target_unit != NONE && (bot->target_visible || (!along_path && bot->goal_weapon == NONE)))
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

	if (bot->seat_role == _bot_seat_driver)
	{
		/* a driver: its stick and the way it steers (a Warthog's driver
		looks where it steers; a Ghost or a Scorpion goes where it aims) --
		out when the vehicle is flipped or stuck, or its target out of reach */
		short why;
		boolean detoured;

		steering = bot->aim;
		why = bot_drive(bot, vehicle_index, kind, &position, distance, weapon, &forward, &strafe, &steering,
			&detoured, step);
		if (kind == _bot_vehicle_jeep || detoured)
			bot->aim = steering;
		if (why != _bot_drive_on)
			bot_leave_vehicle(bot, vehicle_index, why == _bot_drive_flipped, &flags);
		/* (a role that walks -- a carrier, a defender, a returner -- leaves a
		driver's seat) */
		if (why == _bot_drive_on && (bot->role == _bot_role_carry || bot->role == _bot_role_defend ||
			bot->role == _bot_role_return))
		{
			bot_leave_vehicle(bot, vehicle_index, FALSE, &flags);
		}
	}
	else if (vehicle_index != NONE)
	{
		/* a rider: out when its driver has gone a second (Halo's actors
		leave with their driver: action_vehicle.c), or the vehicle is
		flipped */
		struct bot_riders riders;

		bot_vehicle_riders(bot, vehicle_index, &riders);
		/* (each on its own count: its driver gone in reach_ticks, which a
		rider does not use for reach and a new target does not restart; the
		vehicle flipped in upset_ticks) */
		bot->reach_ticks = (short)(riders.driver == NONE ? bot->reach_ticks + step : 0);
		bot->upset_ticks = (short)(unit_get(vehicle_index)->object.up.k < 0.5f ? bot->upset_ticks + step : 0);
		if (bot->reach_ticks >= TICKS_PER_SECOND || bot->upset_ticks >= TICKS_PER_SECOND)
			bot_leave_vehicle(bot, vehicle_index, bot->upset_ticks >= TICKS_PER_SECOND, &flags);
		/* (a carrier riding: out near its base; a defender or a returner: out) */
		if (bot->reach_ticks < TICKS_PER_SECOND && bot->upset_ticks < TICKS_PER_SECOND &&
			(bot_ride_done(bot, &position) || bot->role == _bot_role_defend || bot->role == _bot_role_return))
			bot_leave_vehicle(bot, vehicle_index, FALSE, &flags);
	}
	else if (moving)
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
	/* (a role that holds its place: a dodge sideways only, and no walking
	recovery that would carry it off) */
	if (objective == _bot_goal_hold)
	{
		forward = 0.f;
		bot->unstick_timer = 0;
	}

	/* stuck (moving and getting nowhere): around for a while, a jump, and
	a new path (on foot: a driver has its own, bot_drive) */
	if (vehicle_index == NONE && bot->unstick_timer > 0)
	{
		bot->unstick_timer -= step;
		forward = bot->unstick_throttle.i;
		strafe = bot->unstick_throttle.j;
	}
	else if (vehicle_index == NONE && (bot->stuck_check -= step) <= 0)
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
	if (bot->jump_timer > 0 && vehicle_index == NONE)
	{
		bot->jump_timer -= step;
		flags |= FLAG(_unit_control_jump_bit);
	}

	/* its weapon: reloaded when empty, the other one when this one is
	spent (the weapon wanted, as a person's switch asks for it: player_
	control.c's desired_weapon_index; the swap button takes one from the
	ground) */
	if (bot->reload_timer > 0)
		bot->reload_timer -= step;
	if (bot->swap_timer > 0)
		bot->swap_timer -= step;
	/* (a vehicle's gun has no magazine to load or weapon to swap to) */
	if (current_weapon != NONE && !vehicle_gun)
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
					action->desired_weapon_index = i;
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
	explosive one never up close; the plasma pistol pressed at its pace each
	tick, bot_trigger_tick) */
	bot->shot_ticks = 0;
	if (bot->target_visible && distance < weapon->far_range + 6.f && current_weapon != NONE &&
		!(weapon->style == _style_explosive && distance < 4.f) && bot->swap_timer <= 0)
	{
		struct bot_pistol const *pistol = &pistol_fire[combat_skill];

		if (bot->fire_timer > 0)
		{
			bot->fire_timer -= step;
			if (weapon->style == _style_pistol)
				bot->shot_ticks = pistol->shot_ticks;
			else
			{
				bot->trigger_down = weapon->style == _style_auto ? TRUE : !bot->trigger_down;
				if (bot->trigger_down)
				{
					trigger = 1.f;
					flags |= FLAG(_unit_control_weapon_primary_trigger_bit);
				}
			}
			/* (longer pauses with many bots: fewer shots in the air, each one
			an object of its own a tick, its ray every tick -- the count's
			share of a pause: 1 up to 3 bots, 1.5 to 6, 2 to 8, 2.5 to 10, 3
			past; half again for a bot far and in no view, which fights the
			other bots out there. The plasma pistol's are a30's at any count) */
			if (bot->fire_timer <= 0)
			{
				if (weapon->style == _style_pistol)
					bot->fire_timer = -bot_random_range(pistol->pause_least, pistol->pause_most);
				else
				{
					short halves = bots.created <= 3 ? 2 : bots.created <= 6 ? 3 : bots.created <= 8 ? 4 :
						bots.created <= 10 ? 5 : 6;

					if (bot_detail(bot) == _bot_detail_least)
						halves += halves / 2;
					bot->fire_timer = -(short)(bot_random_range(pause_ticks[combat_skill], pause_ticks[combat_skill] + 9) *
						halves / 2);
				}
			}
		}
		else
		{
			bot->fire_timer += step;
			bot->trigger_down = FALSE;
			/* (a quarter shorter with more than eight bots; the plasma
			pistol's a30's at any count) */
			if (bot->fire_timer >= 0)
				bot->fire_timer = weapon->style == _style_pistol ?
					bot_random_range(pistol->burst_least, pistol->burst_most) :
					(short)(bot_random_range(burst_ticks[combat_skill], burst_ticks[combat_skill] + 7) *
						(bots.created > 8 ? 3 : 4) / 4);
		}
	}
	else
		bot->trigger_down = FALSE;

	/* the plasma pistol charged now and then, as Halo's jackals do it (a30's
	jackal actor variants: special_fire_mode overcharge, special_fire_
	situation enemy visible; actor_combat.c holds the trigger between bursts
	and lets go when one begins): a roll at most every 5 s, in a pause, with
	the enemy in sight 4 to 15 units away (the jackals' maximum_firing_
	range); the trigger then held until the bolt is fully charged and let go
	in a burst with the enemy in sight -- or 5 s after the roll whatever */
	if (bot->charge_timer > 0)
		bot->charge_timer -= step;
	if (weapon->style != _style_pistol)
		bot->charging = FALSE;
	else if (bot->charging)
	{
		if (bot->charge_timer <= 0 || (bot->fire_timer > 0 && bot->target_visible &&
			weapon_get(current_weapon)->weapon.triggers[0].state == _trigger_charged))
		{
			/* (let go: the charged bolt; the next press a pace later) */
			bot->charging = FALSE;
			bot->charge_timer = 5 * TICKS_PER_SECOND;
			bot->next_shot = game_time_get() + pistol_fire[combat_skill].shot_ticks;
		}
	}
	else if (bot->fire_timer < 0 && bot->charge_timer <= 0 && bot->target_visible &&
		distance >= 4.f && distance <= 15.f)
	{
		bot->charge_timer = 5 * TICKS_PER_SECOND;
		bot->charging = (short)(bot_random() % 100) < charge_chance[combat_skill];
	}

	/* melee up close (on foot) */
	if (bot->melee_timer > 0)
		bot->melee_timer -= step;
	else if (bot->target_visible && distance < 1.6f && vehicle_index == NONE)
	{
		flags |= FLAG(_unit_control_use_equipment_bit);
		bot->melee_timer = 30;
	}
	/* a grenade now and then: of a kind it has (the one wanted, as
	player_control.c picks one: the one in hand, else the next it has some
	of -- a throw takes the kind in hand, units.c); a roll a second with the
	enemy in sight 4 to 14 units away (a30's marines: 3 to 14), 6 s at
	least between throws, and none with a teammate within 3 units of the
	enemy (Halo's AI keeps its friends out of the blast: actor_combat.c) */
	{
		short grenade = unit->unit.current_grenade_index;

		if (grenade == NONE || !unit_get_grenade_count(player->unit_index, grenade))
			grenade = unit_inventory_next_grenade(player->unit_index, grenade, 1);
		action->desired_grenade_index = grenade;
	}
	/* (the timer counted down before the test: a roll each second, not
	each second and a thought) */
	if (bot->grenade_timer > 0)
		bot->grenade_timer -= step;
	if (bot->grenade_timer <= 0 && bot->target_visible && distance >= 4.f && distance <= 14.f &&
		vehicle_index == NONE && unit_get_grenade_count(player->unit_index, unit->unit.current_grenade_index) > 0 &&
		!teammate_near(bot, &target_position))
	{
		if ((short)(bot_random() % 100) < grenade_chance[bots.skill])
		{
			/* (the grenade button sets both, as player_control.c does) */
			flags |= FLAG(_unit_control_throw_grenade_bit) | FLAG(_unit_control_weapon_secondary_trigger_bit);
			bot->grenade_timer = 6 * TICKS_PER_SECOND;
		}
		else
			bot->grenade_timer = TICKS_PER_SECOND;
	}

	/* the action button to get in: no other press, stick or trigger that
	thought (the weapon and grenade wanted still go) -- the game seats it
	before it reads the stick and the trigger (players.c) */
	/* a flag or the ball in hand: no trigger, grenade, swap or reload (Halo
	drops what is carried at a trigger: players.c), a melee only; near an
	objective to pick up, no trigger held (players.c picks up no flag or ball
	then) */
	if ((vehicle_index == NONE || bot->seat_role == _bot_seat_passenger) &&
		bot_holds_objective(unit_inventory_get_weapon(player->unit_index, unit->unit.current_weapon_index)))
	{
		flags &= ~(FLAG(_unit_control_weapon_primary_trigger_bit) | FLAG(_unit_control_weapon_secondary_trigger_bit) |
			FLAG(_unit_control_throw_grenade_bit) | FLAG(_unit_control_swap_weapons_bit) |
			FLAG(_unit_control_weapon_reload_bit));
		trigger = 0.f;
		bot->trigger_down = FALSE;
		bot->shot_ticks = 0;
		bot->charging = FALSE;
	}
	else if (bot->role != _bot_role_none && vehicle_index == NONE && bot_near_pickup(bot, &position))
	{
		flags &= ~(FLAG(_unit_control_weapon_primary_trigger_bit) | FLAG(_unit_control_weapon_secondary_trigger_bit) |
			FLAG(_unit_control_throw_grenade_bit));
		trigger = 0.f;
		bot->trigger_down = FALSE;
		bot->shot_ticks = 0;
		bot->charging = FALSE;
	}
	if (boarding)
	{
		forward = strafe = trigger = 0.f;
		flags = FLAG(_unit_control_action_bit);
		bot->shot_ticks = 0;
		bot->charging = FALSE;
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

/* the plasma pistol's trigger each tick, whatever the bot's thinking rate
(its bolt goes as the trigger is let go, its tag giving it no rate of fire
of its own): held while it charges; while it fires, pressed for a tick
each shot_ticks, so 5 or 6 bolts a second whether it thinks every tick or
every eighth */
static void bot_trigger_tick(
	struct bot *bot,
	struct player_action *action,
	long now)
{
	boolean down;

	if (bot->charging)
		down = TRUE;
	else if (bot->shot_ticks > 0)
	{
		down = now >= bot->next_shot;
		if (down)
			bot->next_shot = now + bot->shot_ticks;
	}
	else
		return;
	if (down)
	{
		action->control_flags |= FLAG(_unit_control_weapon_primary_trigger_bit);
		action->primary_trigger = 1.f;
	}
	else
	{
		action->control_flags &= ~FLAG(_unit_control_weapon_primary_trigger_bit);
		action->primary_trigger = 0.f;
	}
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
	objectives_update(now);
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
		boolean seat_changed = FALSE;

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
			/* (in or out of a seat, or into another seat, since that
			thought: none of its stick or trigger goes on into the new seat,
			or onto foot) */
			struct unit_datum *seated = unit_get(player->unit_index);

			if (seated->object.parent_object_index != bot->seen_parent ||
				seated->unit.parent_seat_index != bot->seen_seat)
			{
				action->throttle.i = action->throttle.j = 0.f;
				action->primary_trigger = 0.f;
				action->control_flags &= ~FLAG(_unit_control_weapon_primary_trigger_bit);
				seat_changed = TRUE;
			}
		}
		if (!seat_changed)
			bot_trigger_tick(bot, action, now);
		/* (a flag or the ball in hand, on foot or as a passenger: no trigger,
		grenade, swap or reload on any tick, a held action or a pistol's
		press between thoughts too) */
		{
			struct unit_datum *carrier = unit_get(player->unit_index);

			if ((carrier->object.parent_object_index == NONE || bot->seat_role == _bot_seat_passenger) &&
				bot_holds_objective(unit_inventory_get_weapon(player->unit_index, carrier->unit.current_weapon_index)))
			{
				action->control_flags &= ~(FLAG(_unit_control_weapon_primary_trigger_bit) |
					FLAG(_unit_control_weapon_secondary_trigger_bit) | FLAG(_unit_control_throw_grenade_bit) |
					FLAG(_unit_control_swap_weapons_bit) | FLAG(_unit_control_weapon_reload_bit));
				action->primary_trigger = 0.f;
				bot->shot_ticks = 0;
				bot->charging = FALSE;
			}
			else if ((bot->role == _bot_role_attack || bot->role == _bot_role_return || bot->role == _bot_role_ball) &&
				carrier->object.parent_object_index == NONE)
			{
				real_point3d at;

				/* (near an objective it is to pick up: no trigger on any tick) */
				if (bot_near_pickup(bot, object_get_origin(player->unit_index, &at)))
				{
					action->control_flags &= ~(FLAG(_unit_control_weapon_primary_trigger_bit) |
						FLAG(_unit_control_weapon_secondary_trigger_bit) | FLAG(_unit_control_throw_grenade_bit));
					action->primary_trigger = 0.f;
					bot->shot_ticks = 0;
					bot->charging = FALSE;
				}
			}
		}
		bot->firing = (action->control_flags & (FLAG(_unit_control_weapon_primary_trigger_bit) |
			FLAG(_unit_control_throw_grenade_bit) | FLAG(_unit_control_use_equipment_bit))) != 0 ||
			bot->shot_ticks > 0 || bot->charging;
	}
	bot_profile_poll();
}
