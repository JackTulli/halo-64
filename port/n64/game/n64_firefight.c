/*
N64_FIREFIGHT.C

Firefight: one to four players, one team, against waves of Covenant on a
multiplayer map. The Covenant are the campaign's own grunts, jackals and
elites (imported into every multiplayer map by tools/n64/merge.py) carrying
the map's plasma pistols, plasma rifles and needlers; Halo's own AI is not
on the N64 (too big for its memory and its processor), so each one is
driven here, a tick at a time, through unit_control() as Halo's AI and the
players' controllers drive units: it picks the nearest player, keeps to
its kind's range (grunts back off, elites close in), strafes, fires in
bursts with an aim that misses by more on easier difficulties, throws
plasma grenades now and then, meleees up close and works its way around
whatever stops it.

A game is waves of five; each wave sends more and tougher Covenant (a
few at a time: each biped costs the N64 ~3 ms a tick), from the map's
player spawn points away from the players. The team shares its lives: a
death costs one, and with none left a dead player stays out; when the
whole team is out the game ends (game_engine_end_game) and the menu shows
the wave reached and the score. Clearing five waves gives a life back.

The game itself is team slayer with every player on the red team and no
score to reach (port/n64/game/n64_game.c).
*/

#include "cseries.h"

#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "game/local_players.h"
#include "interface/hud_messaging.h"
#include "items/weapons.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"
#include "units/units.h"
#include "units/bipeds.h"
#include "units/unit_control_data.h"
#include "ai/path.h"
#include "ai/ai_profile.h"
#include "ai/ai_debug.h"

#include "../src/n64.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- constants */

enum
{
	_covenant_grunt = 0,
	_covenant_jackal,
	_covenant_elite,
	NUMBER_OF_COVENANT_KINDS
};

#define MAXIMUM_ENEMIES 12
/* a fallen Covenant's body stays this long (and at most this many stay) */
#define BODY_TICKS (5 * TICKS_PER_SECOND)
#define MAXIMUM_BODIES 16
#define WAVES_PER_SET 5
#define TICKS_BETWEEN_WAVES (6 * TICKS_PER_SECOND)
#define TICKS_BETWEEN_SPAWNS 20
#define FIREFIGHT_TEAM _game_team_covenant
/* the Covenant think at 15 Hz, half of them each tick (their units keep
the last control between) */
/* (ticks between a Covenant's thoughts: 2, or 3 with five or more up or
three or four players -- n64_firefight_tick; its timers count by it) */
static short think_interval = 2;
#define THINK_INTERVAL think_interval
/* a path is found again after this long, or when its target has moved */
#define PATH_LIFETIME (5 * TICKS_PER_SECOND)
/* paths found at most every this many ticks, among all the Covenant (a
search is the dearest thing an AI does: one every tick was too many) */
#define TICKS_BETWEEN_PATHS 2

static char const *const covenant_bipeds[NUMBER_OF_COVENANT_KINDS] =
{
	"characters\\grunt\\grunt",
	"characters\\jackal\\jackal",
	"characters\\elite\\elite",
};

/* each kind's way of fighting */
static struct covenant_kind
{
	real near_range, far_range; /* the distance it keeps */
	real fire_range;
	short burst_ticks, pause_ticks; /* firing */
	real aim_error; /* radians at normal difficulty */
	short grenade_chance; /* in 1000 a tick while it may throw */
	short points;
	char const *weapons[2];
} const kinds[NUMBER_OF_COVENANT_KINDS] =
{
	{ 8.f, 14.f, 22.f, 10, 30, 0.10f, 4, 10, { "weapons\\plasma pistol\\plasma pistol", "weapons\\needler\\needler" } },
	{ 9.f, 16.f, 24.f, 8, 26, 0.07f, 0, 20, { "weapons\\plasma pistol\\plasma pistol", "weapons\\plasma pistol\\plasma pistol" } },
	{ 3.f, 10.f, 25.f, 18, 20, 0.05f, 6, 50, { "weapons\\plasma rifle\\plasma rifle", "weapons\\needler\\needler" } },
};

/* by difficulty (easy, normal, heroic, legendary) */
static real const aim_error_scale[4] = { 1.8f, 1.f, 0.7f, 0.45f };
static real const pause_scale[4] = { 1.6f, 1.f, 0.75f, 0.55f };
static short const starting_lives[4] = { 10, 7, 5, 3 };
static real const enemy_count_scale[4] = { 0.75f, 1.f, 1.2f, 1.4f };

/* the Covenant's voices: their dialogue's vocalizations (dialogue_
definitions.c) that tools/n64/merge.py imports, said through unit_dialogue.c
(unit_vocalize, HALO_N64); their pain and death they voice themselves */
enum
{
	_vocalization_killed_enemy_player = 55,
	_vocalization_new_combat_alone = 108,
	_vocalization_unexpected_enemy = 111,
	_vocalization_grenade_throwing = 115,
	_vocalization_shooting = 149,
	_vocalization_taunt = 154,
	_vocalization_flee_leader_died = 158,
	_vocalization_melee_attack = 179,
	_vocalization_celebration = 188,
};

/* the least time between two Covenant's chatter (spotting, taunts) */
#define TICKS_BETWEEN_CHATTER (2 * TICKS_PER_SECOND)

boolean unit_vocalize(long unit_index, short vocalization_type, short priority);

/* ---------- state */

struct covenant
{
	long unit_index;
	short kind;
	short strafe_direction, strafe_timer;
	short fire_timer; /* > 0 firing, < 0 pausing */
	short sight_timer, stuck_timer, unstick_timer, jump_timer, melee_timer, grenade_timer;
	short unseen_ticks;
	short chatter_timer;
	boolean target_visible;
	boolean trigger_down;
	boolean has_spoken;
	/* the way to the target (Halo's pathfinding, source/ai/path.c) */
	boolean path_valid;
	short path_age;
	struct path_result path;
	real_point3d path_goal;
	/* the game time a straight walk to the target may be tested again (it
	was blocked: plan_path) */
	long direct_after;
	long target_player;
	real_point3d last_seen;
	real_point3d last_position;
	real_vector3d aim;
	real_vector2d unstick_throttle;
};

static struct
{
	boolean active;
	boolean over;
	boolean resolved, has_covenant;
	boolean players_seen;
	short difficulty;
	short player_count;
	long wave; /* 1-based; 0 before the first */
	long wave_total, wave_spawned, wave_killed;
	long break_timer, spawn_timer;
	long lives;
	long score;
	long kills[4];
	unsigned long random;
	/* who was alive last tick, who is out for good (by player datum's
	absolute index) */
	boolean alive[16];
	boolean out[16];
	long definitions[NUMBER_OF_COVENANT_KINDS];
	long weapon_definitions[NUMBER_OF_COVENANT_KINDS][2];
	struct covenant enemies[MAXIMUM_ENEMIES];
	/* the Covenant's bodies, each taken away BODY_TICKS after it fell */
	struct
	{
		long unit_index;
		short timer;
	} bodies[MAXIMUM_BODIES];
	short body_count;
	short enemy_count;
	/* paths found this tick (one at most), straight walks tested */
	short paths_this_tick;
	short direct_tests_this_tick;
	/* the search going on in parts (n64_bots.c's way): its Covenant's
	unit, where it goes */
	boolean searching;
	long search_unit;
	real_point3d search_goal;
	/* ticks until the next path may be found */
	short path_cooldown;
	/* (the log's: searches, no surface under one end, searches that
	reached, paths built) */
	long path_searches, path_no_surface, path_found, path_built;
	/* when a Covenant last chattered (game time) */
	long last_chatter;
} firefight;

/* the AI's meters, which the pathfinder counts in (ai_profile.c is the
AI's, left out) */
struct ai_profile_globals ai_profile;

/* the pathfinder's working state (47 KB, only while Firefight plays) */
static struct path_state *firefight_path_state;

/* ---------- small things */

static unsigned long ff_random(
	void)
{
	firefight.random = firefight.random * 1664525UL + 1013904223UL;
	return firefight.random >> 8;
}

/* -1 to 1 */
static real ff_random_real(
	void)
{
	return (real)(ff_random() & 0xFFFF) / 32768.f - 1.f;
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

static void message_all(
	wchar_t const *text)
{
	short local_player_index;

	for (local_player_index = 0; local_player_index < 4; local_player_index++)
	{
		if (local_player_get_player_index(local_player_index) != NONE)
			hud_print_message(local_player_index, text);
	}
}

static void message_all_ascii(
	char const *text)
{
	wchar_t wide[80];
	short i;

	for (i = 0; i < 79 && text[i]; i++)
		wide[i] = (wchar_t)text[i];
	wide[i] = 0;
	message_all(wide);
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

/* ---------- the waves */

/* how many Covenant a wave has, and how many may be up at once */
/* a line from a Covenant: chatter (spotting, taunts) only now and then
among them all */
static void vocalize(
	struct covenant *enemy,
	short vocalization_type,
	short priority,
	boolean chatter)
{
	if (chatter && game_time_get() - firefight.last_chatter < TICKS_BETWEEN_CHATTER)
		return;
	if (unit_vocalize(enemy->unit_index, vocalization_type, priority) && chatter)
		firefight.last_chatter = game_time_get();
}

static long wave_size(
	long wave)
{
	long set = (wave - 1) / WAVES_PER_SET, in_set = (wave - 1) % WAVES_PER_SET;
	real count = (4.f + 2.f * in_set + 3.f * set) * enemy_count_scale[firefight.difficulty] *
		(1.f + 0.2f * (firefight.player_count - 1));

	return (long)count;
}

static short alive_limit(
	void)
{
	/* (each Covenant biped costs ~3 ms of a tick: fewer with more views) */
	return firefight.player_count <= 2 ? 6 : 5;
}

/* the kind of the n-th Covenant of a wave: grunts at first, jackals from
the second wave, elites from the third, more of them as the sets go on */
static short wave_kind(
	long wave,
	long n)
{
	long set = (wave - 1) / WAVES_PER_SET, in_set = (wave - 1) % WAVES_PER_SET;
	long roll = (long)(ff_random() % 100);
	long elites = in_set >= 2 || set > 0 ? 10 + 8 * in_set + 10 * set : 0;
	long jackals = in_set >= 1 || set > 0 ? 25 : 0;

	(void)n;
	/* (measuring, port.json flag "debug_firefight_kind=<0-2>": every
	Covenant a grunt, a jackal or an elite) */
	{
		static int forced = -2;
		char text[8];

		if (forced == -2)
			forced = n64_debug_text("debug_firefight_kind", text, sizeof(text)) ? atoi(text) : -1;
		if (forced >= 0 && forced < NUMBER_OF_COVENANT_KINDS)
			return (short)forced;
	}
	if (elites > 60)
		elites = 60;
	if (roll < elites)
		return _covenant_elite;
	if (roll < elites + jackals)
		return _covenant_jackal;
	return _covenant_grunt;
}

/* a player spawn point away from the players (but not the farthest corner
of a big map), with a little scatter */
static boolean spawn_point(
	real_point3d *point,
	real *facing)
{
	struct scenario *scenario = global_scenario_get();
	long count, i, best = NONE;
	real best_score = -1e9f;

	if (!scenario)
		return FALSE;
	count = scenario->players.count;
	for (i = 0; i < count; i++)
	{
		struct player_starting_location *location =
			TAG_BLOCK_GET_ELEMENT(&scenario->players, i, struct player_starting_location);
		struct data_iterator iterator;
		struct player_datum *player;
		real nearest = 1e9f, score;

		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
		{
			if (player->unit_index != NONE)
			{
				real d = horizontal_distance(&location->position, &object_get(player->unit_index)->object.position);

				if (d < nearest)
					nearest = d;
			}
		}
		/* out of sight-ish (past 18 units), the nearer the better past
		that, a little randomness so the same corner is not always used */
		score = nearest < 18.f ? nearest - 100.f : -nearest + 8.f * ff_random_real();
		if (score > best_score)
		{
			best_score = score;
			best = i;
		}
	}
	if (best == NONE)
		return FALSE;
	{
		struct player_starting_location *location =
			TAG_BLOCK_GET_ELEMENT(&scenario->players, best, struct player_starting_location);

		*point = location->position;
		point->x += 0.6f * ff_random_real();
		point->y += 0.6f * ff_random_real();
		point->z += 0.25f;
		*facing = location->facing;
	}
	return TRUE;
}

static void give_weapon(
	long unit_index,
	long definition_index,
	boolean first)
{
	struct object_placement_data placement;
	long weapon_index;

	if (definition_index == NONE)
		return;
	object_placement_data_new(&placement, definition_index, NONE);
	weapon_index = object_new(&placement);
	if (weapon_index != NONE)
		unit_add_weapon_to_inventory(unit_index, weapon_index, first ? 2 : 0);
}

static boolean spawn_covenant(
	short kind)
{
	struct object_placement_data placement;
	struct covenant *enemy;
	real_point3d point;
	real facing;
	long unit_index;

	if (firefight.enemy_count >= MAXIMUM_ENEMIES || firefight.definitions[kind] == NONE ||
		!spawn_point(&point, &facing))
	{
		return FALSE;
	}
	object_placement_data_new(&placement, firefight.definitions[kind], NONE);
	placement.position = point;
	placement.forward.i = (real)cos(facing);
	placement.forward.j = (real)sin(facing);
	placement.forward.k = 0.f;
	placement.up.i = 0.f;
	placement.up.j = 0.f;
	placement.up.k = 1.f;
	placement.owner_team_index = FIREFIGHT_TEAM;
	unit_index = object_new(&placement);
	if (unit_index == NONE)
		return FALSE;
	/* awake wherever they are: Halo updates only the objects in the
	clusters the players can see (as the CTF flag and the oddball, out of
	sight they would stand frozen) */
	object_set_automatic_deactivation(unit_index, FALSE);
	/* (as a player's or an actor's: a unit nobody controls has its
	throttle and aim reset every tick, units.c) */
	unit_set_actively_controlled(unit_index, TRUE);
	give_weapon(unit_index, firefight.weapon_definitions[kind][ff_random() % 3 == 0 ? 1 : 0], TRUE);
	{
		struct unit_datum *unit = unit_get(unit_index);

		/* plasma grenades for the throwers */
		if (kinds[kind].grenade_chance && NUMBER_OF_UNIT_GRENADE_TYPES > 1)
			unit->unit.grenade_counts[1] = (char)(kind == _covenant_elite ? 2 : 1);
	}
	enemy = &firefight.enemies[firefight.enemy_count++];
	csmemset(enemy, 0, sizeof(*enemy));
	enemy->unit_index = unit_index;
	enemy->kind = kind;
	enemy->strafe_direction = ff_random() & 1 ? 1 : -1;
	enemy->fire_timer = -(short)(10 + ff_random() % 30);
	enemy->sight_timer = (short)(ff_random() % 10);
	enemy->target_player = NONE;
	enemy->last_position = point;
	enemy->aim = placement.forward;
	return TRUE;
}

static void start_wave(
	void)
{
	char text[48];

	/* (measuring, port.json flag "debug_firefight_wave=<n>": the game
	starts at wave n) */
	if (firefight.wave == 0 && n64_debug_text("debug_firefight_wave", text, sizeof(text)) && atol(text) > 1)
		firefight.wave = atol(text) - 1;
	firefight.wave++;
	firefight.wave_total = wave_size(firefight.wave);
	firefight.wave_spawned = 0;
	firefight.wave_killed = 0;
	firefight.spawn_timer = 0;
	if (firefight.wave > 1 && (firefight.wave - 1) % WAVES_PER_SET == 0)
	{
		firefight.lives++;
		snprintf(text, sizeof(text), "Set %ld complete: a life back", (firefight.wave - 1) / WAVES_PER_SET);
		message_all_ascii(text);
	}
	snprintf(text, sizeof(text), "Wave %ld", firefight.wave);
	message_all_ascii(text);
}

/* ---------- the Covenant's minds */

static long nearest_player_unit(
	real_point3d const *from,
	long *player_index_out)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long best = NONE;
	real best_distance = 1e9f;

	*player_index_out = NONE;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		if (unit_alive(player->unit_index))
		{
			real d = horizontal_distance(from, &object_get(player->unit_index)->object.position);

			if (d < best_distance)
			{
				best_distance = d;
				best = player->unit_index;
				*player_index_out = iterator.datum_index;
			}
		}
	}
	return best;
}

/* n64_bots.c's: a straight walk as a one-step path; the clusters' PVS */
boolean n64_ai_direct_path(real_point3d const *start, long start_surface, real_point3d const *goal,
	long goal_surface, struct path_result *path);
boolean n64_ai_clusters_may_see(long object_a, long object_b);

static boolean line_of_sight(
	real_point3d const *from,
	real_point3d const *to,
	long ignore_object_index)
{
	extern int n64_collision_any_hit;
	struct collision_result collision;
	real_vector3d vector;
	boolean blocked;

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
	return !blocked;
}

/* the search going on: this tick's part (at most SEARCH_SURFACES_PER_TICK
surfaces opened, ~1 ms), and once it is over the path to its Covenant (one
gone since gives it up); the next search may begin TICKS_BETWEEN_PATHS
after */
#define SEARCH_SURFACES_PER_TICK 40

static void firefight_search_step(
	void)
{
	struct covenant *enemy = NULL;
	boolean found = FALSE;
	short i;

	if (!firefight.searching || !firefight_path_state)
		return;
	for (i = 0; i < firefight.enemy_count; i++)
	{
		if (firefight.enemies[i].unit_index == firefight.search_unit)
		{
			enemy = &firefight.enemies[i];
			break;
		}
	}
	if (!enemy)
	{
		firefight.searching = FALSE;
		firefight.path_cooldown = TICKS_BETWEEN_PATHS;
		return;
	}
	if (!path_state_find_continue(firefight_path_state, SEARCH_SURFACES_PER_TICK, &found))
		return;
	firefight.searching = FALSE;
	firefight.path_cooldown = TICKS_BETWEEN_PATHS;
	if (found)
		firefight.path_found++;
	if (path_state_build_path(firefight_path_state, &enemy->path) && enemy->path.step_count > 0)
	{
		firefight.path_built++;
		enemy->path_valid = TRUE;
		enemy->path.step_index = 0;
		enemy->path_age = 0;
		enemy->path_goal = firefight.search_goal;
	}
}

/* a path to the target, if this tick still has one to find: Halo's own
(source/ai/path.c) over the collision BSP's surfaces; partial paths (to
the nearest point the search reached) are kept */
static void plan_path(
	struct covenant *enemy,
	long target_unit)
{
	struct path_input input;
	real_point3d start, goal;
	long start_surface, goal_surface;
	boolean search = !firefight.paths_this_tick && firefight.path_cooldown <= 0 && !firefight.searching;
	boolean direct = game_time_get() >= enemy->direct_after && firefight.direct_tests_this_tick < 2;

	/* (its own search still going on: it waits for that) */
	if (!firefight_path_state || (!search && !direct) ||
		(firefight.searching && firefight.search_unit == enemy->unit_index))
	{
		return;
	}
	start_surface = biped_find_pathfinding_surface_index(enemy->unit_index, &start);
	goal_surface = biped_find_pathfinding_surface_index(target_unit, &goal);
	if (start_surface == NONE || goal_surface == NONE)
	{
		if (search)
		{
			firefight.paths_this_tick++;
			firefight.path_cooldown = TICKS_BETWEEN_PATHS;
			enemy->path_age = 0;
			enemy->path_valid = FALSE;
			firefight.path_searches++;
			firefight.path_no_surface++;
		}
		return;
	}
	/* a straight walk there first (n64_bots.c): one test and no search
	where it's clear, two a tick at most; blocked, not tested again for a
	second */
	if (direct)
	{
		firefight.direct_tests_this_tick++;
		if (n64_ai_direct_path(&start, start_surface, &goal, goal_surface, &enemy->path))
		{
			enemy->path_valid = TRUE;
			enemy->path_age = 0;
			enemy->path_goal = goal;
			return;
		}
		enemy->direct_after = game_time_get() + TICKS_PER_SECOND;
	}
	if (!search)
		return;
	firefight.paths_this_tick++;
	firefight.path_cooldown = TICKS_BETWEEN_PATHS;
	enemy->path_age = 0;
	enemy->path_valid = FALSE;
	firefight.path_searches++;
	path_input_new(&input, 0.3f, FALSE, enemy->unit_index);
	path_input_set_start(&input, &start, start_surface);
	/* (Halo's own bound for its AI's searches) */
	path_input_set_search_bounds(&input, 2.5f * distance3d(&start, &goal) + 15.f);
	path_state_new(&input, firefight_path_state, NULL);
	path_state_destination(firefight_path_state, &goal, goal_surface, 40.f);
	/* (in parts, firefight_search_step; the next search waits for its end) */
	if (!path_state_find_begin(firefight_path_state))
		return;
	firefight.searching = TRUE;
	firefight.search_unit = enemy->unit_index;
	firefight.search_goal = goal;
	firefight_search_step();
}

/* the way along the path from here (horizontal, unit length); FALSE when
the path is used up */
static boolean follow_path(
	struct covenant *enemy,
	real_point3d const *position,
	real_vector2d *direction)
{
	struct path_result *path = &enemy->path;

	while (enemy->path_valid && path->step_index < path->step_count)
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
	enemy->path_valid = FALSE;
	return FALSE;
}

static void think(
	struct covenant *enemy)
{
	struct covenant_kind const *kind = &kinds[enemy->kind];
	struct unit_datum *unit = unit_get(enemy->unit_index);
	struct unit_control_data control;
	real_point3d eye, target_eye, target_position;
	long target_unit, target_player;
	real distance;
	real_vector3d to_target, desired;
	real_vector2d move = { 0.f, 0.f }; /* the way to go, in the world */
	real forward = 0.f, strafe = 0.f;
	boolean approaching, moving_along_path = FALSE;
	word flags = 0;
	real trigger = 0.f;

	unit_get_camera_position(enemy->unit_index, &eye);
	target_unit = nearest_player_unit(&unit->object.position, &target_player);
	if (target_unit == NONE)
	{
		/* nobody to fight: stand still */
		csmemset(&control, 0, sizeof(control));
		control.animation_state = _unit_animation_state_in_combat;
		control.weapon_index = unit->unit.current_weapon_index;
		control.grenade_index = NONE;
		control.zoom_level = NONE;
		control.facing_vector = control.aiming_vector = control.looking_vector = enemy->aim;
		unit_control(enemy->unit_index, &control);
		return;
	}
	enemy->target_player = target_player;
	unit_get_camera_position(target_unit, &target_eye);
	target_position = object_get(target_unit)->object.position;

	/* can it see the target? (a few times a second) */
	enemy->sight_timer -= THINK_INTERVAL;
	if (enemy->sight_timer <= 0)
	{
		enemy->sight_timer = 12;
		enemy->target_visible = n64_ai_clusters_may_see(enemy->unit_index, target_unit) &&
			line_of_sight(&eye, &target_eye, enemy->unit_index);
	}
	if (enemy->target_visible)
	{
		/* (a target found, or found again after a while: said aloud) */
		if (!enemy->has_spoken || enemy->unseen_ticks > 8 * TICKS_PER_SECOND)
		{
			enemy->has_spoken = TRUE;
			vocalize(enemy, horizontal_distance(&unit->object.position, &target_position) < 5.f ?
				_vocalization_unexpected_enemy : _vocalization_new_combat_alone, _unit_speech_shout, TRUE);
		}
		enemy->last_seen = target_eye;
		enemy->unseen_ticks = 0;
	}
	else
	{
		if (enemy->unseen_ticks < 32000)
			enemy->unseen_ticks += THINK_INTERVAL;
		/* (not seen yet: head for where the target is) */
		if (enemy->last_seen.x == 0.f && enemy->last_seen.y == 0.f)
			enemy->last_seen = target_eye;
	}
	distance = horizontal_distance(&unit->object.position, &target_position);

	/* where to go: along a path while the target is out of sight or out of
	range; in range, keep its kind's distance and move sideways */
	approaching = !enemy->target_visible || distance > kind->far_range;
	if (approaching)
	{
		enemy->path_age += THINK_INTERVAL;
		if (!enemy->path_valid || enemy->path_age > PATH_LIFETIME ||
			horizontal_distance(&enemy->path_goal, &target_position) > 7.f)
		{
			plan_path(enemy, target_unit);
		}
		if (follow_path(enemy, &unit->object.position, &move))
			moving_along_path = TRUE;
		else
		{
			/* no path (yet): straight for it */
			real dx = target_position.x - unit->object.position.x;
			real dy = target_position.y - unit->object.position.y;
			real length = square_root(dx * dx + dy * dy);

			if (length > 0.01f)
			{
				move.i = dx / length;
				move.j = dy / length;
			}
		}
	}

	/* where to look: at the target when it is in sight (missing by the
	difficulty's error), else the way it is going */
	if (enemy->target_visible || !moving_along_path)
	{
		to_target.i = enemy->last_seen.x - eye.x;
		to_target.j = enemy->last_seen.y - eye.y;
		to_target.k = enemy->last_seen.z - eye.z - 0.1f;
		desired = to_target;
		normalize(&desired);
		if (enemy->target_visible)
		{
			real error = kind->aim_error * aim_error_scale[firefight.difficulty];

			desired.i += error * ff_random_real();
			desired.j += error * ff_random_real();
			desired.k += error * 0.6f * ff_random_real();
			normalize(&desired);
		}
	}
	else
	{
		desired.i = move.i;
		desired.j = move.j;
		desired.k = 0.f;
	}
	/* turning: a quick but not instant swing */
	{
		/* (as quick a turn a tick, thinking every two ticks or three) */
		/* (as much of the turn as the ticks between thoughts would have
		made: 0.6 over two, 0.75 over three, 0.84 over four) */
		real turn = THINK_INTERVAL >= 4 ? 0.84f : THINK_INTERVAL >= 3 ? 0.75f : 0.6f;

		enemy->aim.i += (desired.i - enemy->aim.i) * turn;
		enemy->aim.j += (desired.j - enemy->aim.j) * turn;
		enemy->aim.k += (desired.k - enemy->aim.k) * turn;
	}
	normalize(&enemy->aim);

	if (approaching)
	{
		/* the way to go, relative to where it faces (Halo's throttle: i
		forward, j left) */
		real fi = enemy->aim.i, fj = enemy->aim.j;
		real length = square_root(fi * fi + fj * fj);

		if (length > 0.01f)
		{
			fi /= length;
			fj /= length;
		}
		forward = move.i * fi + move.j * fj;
		strafe = move.i * -fj + move.j * fi;
	}
	else
	{
		if (distance < kind->near_range)
			forward = enemy->kind == _covenant_grunt ? -0.8f : -0.4f;
		enemy->strafe_timer -= THINK_INTERVAL;
		if (enemy->strafe_timer <= 0)
		{
			enemy->strafe_timer = (short)(25 + ff_random() % 50);
			enemy->strafe_direction = (short)(ff_random() & 1 ? 1 : -1);
		}
		strafe = 0.75f * enemy->strafe_direction;
	}

	/* stuck (moving and getting nowhere): around for a while, a jump, and
	a new path */
	if (enemy->unstick_timer > 0)
	{
		enemy->unstick_timer -= THINK_INTERVAL;
		forward = enemy->unstick_throttle.i;
		strafe = enemy->unstick_throttle.j;
	}
	else if (((game_time_get() / THINK_INTERVAL) & 7) == (enemy->unit_index & 7))
	{
		real moved = horizontal_distance(&unit->object.position, &enemy->last_position);

		enemy->last_position = unit->object.position;
		if ((forward != 0.f || strafe != 0.f) && moved < 0.25f)
		{
			if (++enemy->stuck_timer >= 2)
			{
				enemy->stuck_timer = 0;
				enemy->unstick_timer = (short)(16 + ff_random() % 20);
				enemy->unstick_throttle.i = ff_random_real() < 0.f ? -0.5f : 0.4f;
				enemy->unstick_throttle.j = ff_random() & 1 ? 1.f : -1.f;
				enemy->jump_timer = THINK_INTERVAL;
				enemy->path_valid = FALSE;
			}
		}
		else
			enemy->stuck_timer = 0;
	}
	if (enemy->jump_timer > 0)
	{
		enemy->jump_timer -= THINK_INTERVAL;
		flags |= FLAG(_unit_control_jump_bit);
	}

	/* firing: bursts and pauses while the target is in sight and range */
	if (enemy->target_visible && distance < kind->fire_range)
	{
		if (enemy->fire_timer > 0)
		{
			enemy->fire_timer -= THINK_INTERVAL;
			/* (a plasma pistol fires a shot a press, and held it would
			overcharge: pressed every other update) */
			enemy->trigger_down = enemy->kind == _covenant_elite ? TRUE : !enemy->trigger_down;
			if (enemy->trigger_down)
			{
				trigger = 1.f;
				flags |= FLAG(_unit_control_weapon_primary_trigger_bit);
			}
			if (enemy->fire_timer <= 0)
				enemy->fire_timer = -(short)(kind->pause_ticks * pause_scale[firefight.difficulty] + ff_random() % 12);
			/* a taunt or a battle cry now and then */
			enemy->chatter_timer -= THINK_INTERVAL;
			if (enemy->chatter_timer <= 0)
			{
				enemy->chatter_timer = (short)((8 + ff_random() % 10) * TICKS_PER_SECOND);
				vocalize(enemy, ff_random() & 1 ? _vocalization_taunt : _vocalization_shooting,
					_unit_speech_shout, TRUE);
			}
		}
		else
		{
			enemy->fire_timer += THINK_INTERVAL;
			if (enemy->fire_timer >= 0)
				enemy->fire_timer = (short)(kind->burst_ticks + ff_random() % 6);
		}
	}
	/* melee up close */
	if (enemy->melee_timer > 0)
		enemy->melee_timer -= THINK_INTERVAL;
	else if (distance < 1.6f && enemy->target_visible)
	{
		flags |= FLAG(_unit_control_use_equipment_bit);
		enemy->melee_timer = 30;
		vocalize(enemy, _vocalization_melee_attack, _unit_speech_exclamation, FALSE);
	}
	/* a plasma grenade now and then */
	if (enemy->grenade_timer > 0)
		enemy->grenade_timer -= THINK_INTERVAL;
	else if (kind->grenade_chance && enemy->target_visible && distance > 7.f && distance < 20.f &&
		(long)(ff_random() % 1000) < kind->grenade_chance * THINK_INTERVAL)
	{
		flags |= FLAG(_unit_control_throw_grenade_bit);
		enemy->grenade_timer = 8 * TICKS_PER_SECOND;
		vocalize(enemy, _vocalization_grenade_throwing, _unit_speech_shout, FALSE);
	}

	if (forward > 1.f) forward = 1.f;
	if (forward < -1.f) forward = -1.f;
	if (strafe > 1.f) strafe = 1.f;
	if (strafe < -1.f) strafe = -1.f;
	csmemset(&control, 0, sizeof(control));
	control.animation_state = _unit_animation_state_in_combat;
	control.aiming_speed = 0;
	control.control_flags = flags;
	control.weapon_index = unit->unit.current_weapon_index;
	control.grenade_index = 1;
	control.zoom_level = NONE;
	control.throttle.i = forward;
	control.throttle.j = strafe;
	control.throttle.k = 0.f;
	control.primary_trigger = trigger;
	control.facing_vector = control.aiming_vector = control.looking_vector = enemy->aim;
	unit_control(enemy->unit_index, &control);
}

/* ---------- the team */

static void credit_kill(
	struct covenant *enemy)
{
	struct unit_datum *unit = (struct unit_datum *)object_try_and_get(enemy->unit_index);
	long player_index = NONE;
	unsigned long latest = 0;
	short i;

	firefight.score += kinds[enemy->kind].points * (firefight.difficulty + 1);
	if (!unit)
		return;
	for (i = 0; i < MAXIMUM_ATTACKERS_PER_UNIT; i++)
	{
		struct unit_attacker *attacker = &unit->unit.attackers[i];

		if (attacker->player_index != NONE && attacker->game_time_stamp >= latest)
		{
			latest = attacker->game_time_stamp;
			player_index = attacker->player_index;
		}
	}
	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->local_player_index >= 0 && player->local_player_index < 4)
			firefight.kills[player->local_player_index]++;
	}
}

/* a player killed by a Covenant: the killer says so */
static void covenant_celebrate(
	long player_unit_index)
{
	struct unit_datum *dead = (struct unit_datum *)object_try_and_get(player_unit_index);
	unsigned long latest = 0;
	long killer = NONE;
	short i;

	if (!dead)
		return;
	for (i = 0; i < MAXIMUM_ATTACKERS_PER_UNIT; i++)
	{
		struct unit_attacker *attacker = &dead->unit.attackers[i];

		if (attacker->object_index != NONE && attacker->game_time_stamp >= latest)
		{
			latest = attacker->game_time_stamp;
			killer = attacker->object_index;
		}
	}
	for (i = 0; i < firefight.enemy_count; i++)
	{
		if (firefight.enemies[i].unit_index == killer && unit_alive(killer))
		{
			vocalize(&firefight.enemies[i], ff_random() & 1 ? _vocalization_killed_enemy_player :
				_vocalization_celebration, _unit_speech_shout, FALSE);
			break;
		}
	}
}

/* an elite down: a grunt near it panics */
static void covenant_leader_died(
	struct covenant const *fallen)
{
	struct object_datum *body = object_try_and_get(fallen->unit_index);
	short i;

	if (fallen->kind != _covenant_elite || !body)
		return;
	for (i = 0; i < firefight.enemy_count; i++)
	{
		struct covenant *enemy = &firefight.enemies[i];

		if (enemy->kind == _covenant_grunt && unit_alive(enemy->unit_index) &&
			horizontal_distance(&object_get(enemy->unit_index)->object.position, &body->object.position) < 12.f)
		{
			vocalize(enemy, _vocalization_flee_leader_died, _unit_speech_shout, FALSE);
			break;
		}
	}
}

/* a Covenant fallen: its body goes in a few seconds (the oldest at once
when too many lie about) */
static void body_fell(
	long unit_index)
{
	if (!object_try_and_get(unit_index))
		return;
	if (firefight.body_count == MAXIMUM_BODIES)
	{
		short oldest = 0, i;

		for (i = 1; i < firefight.body_count; i++)
		{
			if (firefight.bodies[i].timer < firefight.bodies[oldest].timer)
				oldest = i;
		}
		if (object_try_and_get(firefight.bodies[oldest].unit_index))
			object_delete(firefight.bodies[oldest].unit_index);
		firefight.bodies[oldest] = firefight.bodies[--firefight.body_count];
	}
	firefight.bodies[firefight.body_count].unit_index = unit_index;
	firefight.bodies[firefight.body_count].timer = BODY_TICKS;
	firefight.body_count++;
}

/* deaths cost the team's lives; with none left, the dead stay dead */
static void count_lives(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;
	boolean anyone_in = FALSE;

	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		short slot = (short)(iterator.absolute_index & 15);
		boolean alive = unit_alive(player->unit_index);

		if (firefight.alive[slot] && !alive)
			covenant_celebrate(player->unit_index);
		if (firefight.alive[slot] && !alive && !firefight.out[slot])
		{
			if (firefight.lives > 0)
			{
				char text[32];

				firefight.lives--;
				snprintf(text, sizeof(text), "%ld lives left", firefight.lives);
				message_all_ascii(text);
			}
			else
				firefight.out[slot] = TRUE;
		}
		firefight.alive[slot] = alive;
		if (firefight.out[slot] && !alive)
		{
			/* (the game engine respawns at 0) */
			if (player->respawn_timer < 2 * TICKS_PER_SECOND)
				player->respawn_timer = 2 * TICKS_PER_SECOND;
		}
		if (!firefight.out[slot])
			anyone_in = TRUE;
		firefight.players_seen = TRUE;
	}
	if (firefight.players_seen && !anyone_in && !firefight.over)
	{
		firefight.over = TRUE;
		message_all_ascii("Game over");
		game_engine_end_game();
	}
}

/* ---------- public code */

void n64_firefight_begin(
	int difficulty,
	int player_count)
{
	csmemset(&firefight, 0, sizeof(firefight));
	firefight.active = TRUE;
	firefight.difficulty = (short)PIN(difficulty, 0, 3);
	firefight.player_count = (short)PIN(player_count, 1, 4);
	firefight.lives = starting_lives[firefight.difficulty];
	firefight.random = n64_ticks();
	firefight.break_timer = 3 * TICKS_PER_SECOND;
	if (!firefight_path_state)
		firefight_path_state = (struct path_state *)malloc(sizeof(struct path_state));
	/* (the paths' last stage steers around nearby objects from the AI's
	own records of them, which the N64 has not got: smoothed paths only;
	the Covenant push past each other) */
	ai_debug.path_disable_obstacle_avoidance = TRUE;
}

void n64_firefight_stop(
	void)
{
	firefight.active = FALSE;
	firefight.searching = FALSE;
	if (firefight_path_state)
	{
		free(firefight_path_state);
		firefight_path_state = NULL;
	}
}

int n64_firefight_active(
	void)
{
	return firefight.active;
}

/* the map's tags, once its game has started */
static boolean firefight_ready(
	void)
{
	short kind;

	if (firefight.resolved)
		return firefight.has_covenant;
	firefight.resolved = TRUE;
	for (kind = 0; kind < NUMBER_OF_COVENANT_KINDS; kind++)
	{
		firefight.definitions[kind] = tag_loaded('bipd', covenant_bipeds[kind]);
		firefight.weapon_definitions[kind][0] = tag_loaded('weap', kinds[kind].weapons[0]);
		firefight.weapon_definitions[kind][1] = tag_loaded('weap', kinds[kind].weapons[1]);
		if (firefight.weapon_definitions[kind][1] == NONE)
			firefight.weapon_definitions[kind][1] = firefight.weapon_definitions[kind][0];
	}
	firefight.has_covenant = firefight.definitions[0] != NONE;
	if (!firefight.has_covenant)
		n64_log("n64: firefight: this map has no Covenant (tools/n64/merge.py)");
	return firefight.has_covenant;
}

/* a tick, before the game's own */
void n64_firefight_tick(
	void)
{
	short i;

	if (!firefight.active || !game_in_progress() || !game_engine_running() || firefight.over)
		return;
	if (!firefight_ready())
		return;
	count_lives();

	/* the bodies, a few seconds after they fell: gone */
	for (i = 0; i < firefight.body_count; )
	{
		if (--firefight.bodies[i].timer <= 0)
		{
			if (object_try_and_get(firefight.bodies[i].unit_index))
				object_delete(firefight.bodies[i].unit_index);
			firefight.bodies[i] = firefight.bodies[--firefight.body_count];
		}
		else
			i++;
	}

	/* the fallen: score, and out of the list (their bodies taken away a
	few seconds later); one that has not seen a player for half a minute
	(stuck out of reach) is taken away and sent again from another spawn
	point */
	for (i = 0; i < firefight.enemy_count; )
	{
		if (unit_alive(firefight.enemies[i].unit_index) &&
			firefight.enemies[i].unseen_ticks > 30 * TICKS_PER_SECOND)
		{
			object_delete(firefight.enemies[i].unit_index);
			firefight.wave_spawned--;
			firefight.enemies[i] = firefight.enemies[--firefight.enemy_count];
		}
		else if (!unit_alive(firefight.enemies[i].unit_index))
		{
			credit_kill(&firefight.enemies[i]);
			covenant_leader_died(&firefight.enemies[i]);
			body_fell(firefight.enemies[i].unit_index);
			firefight.wave_killed++;
			firefight.enemies[i] = firefight.enemies[--firefight.enemy_count];
		}
		else
			i++;
	}

	/* between waves, then the next; Covenant come out a few at a time */
	if (firefight.break_timer > 0)
	{
		if (--firefight.break_timer == 0)
			start_wave();
	}
	else if (firefight.wave_killed >= firefight.wave_total && firefight.enemy_count == 0)
	{
		char text[48];

		firefight.score += 100 * firefight.wave * (firefight.difficulty + 1);
		snprintf(text, sizeof(text), "Wave %ld cleared", firefight.wave);
		message_all_ascii(text);
		firefight.break_timer = TICKS_BETWEEN_WAVES;
	}
	else if (firefight.wave_spawned < firefight.wave_total && firefight.enemy_count < alive_limit() &&
		--firefight.spawn_timer <= 0)
	{
		firefight.spawn_timer = TICKS_BETWEEN_SPAWNS;
		if (spawn_covenant(wave_kind(firefight.wave, firefight.wave_spawned)))
			firefight.wave_spawned++;
	}

	firefight.paths_this_tick = 0;
	firefight.direct_tests_this_tick = 0;
	if (firefight.path_cooldown > 0)
		firefight.path_cooldown--;
	/* (the search going on: this tick's part) */
	firefight_search_step();
	{
		/* (every fourth tick with ANIMATIONS HALF RATE: n64_menu.c) */
		extern int n64_half_animation_rate;

		think_interval = n64_half_animation_rate ? 4 :
			firefight.enemy_count >= 5 || firefight.player_count >= 3 ? 3 : 2;
	}
	for (i = 0; i < firefight.enemy_count; i++)
	{
		if ((game_time_get() + i) % THINK_INTERVAL == 0)
			think(&firefight.enemies[i]);
	}

	/* (the log, every two seconds with "debug_firefight_log": each Covenant's
	whereabouts -- a few dozen float formats, a hitch in the tick it ran) */
	if (!(game_time_get() % (2 * TICKS_PER_SECOND)) && n64_debug_flag("debug_firefight_log"))
	{
		char line[160];

		snprintf(line, sizeof(line), "firefight: wave %ld (%ld of %ld out, %ld down), lives %ld, score %ld; paths: %ld searches, %ld without a surface, %ld reached, %ld built",
			firefight.wave, firefight.wave_spawned, firefight.wave_total, firefight.wave_killed, firefight.lives,
			firefight.score, firefight.path_searches, firefight.path_no_surface, firefight.path_found, firefight.path_built);
		n64_log(line);
		for (i = 0; i < firefight.enemy_count; i++)
		{
			struct covenant *enemy = &firefight.enemies[i];
			struct unit_datum *unit = unit_get(enemy->unit_index);

			snprintf(line, sizeof(line), "  %s at %.1f %.1f %.1f, body %.2f shield %.2f, %s, fire %d, stuck %d, path %d %d/%d",
				covenant_bipeds[enemy->kind] + 11, unit->object.position.x, unit->object.position.y,
				unit->object.position.z, unit->object.body_vitality, unit->object.shield_vitality,
				enemy->target_visible ? "sees" : "hunts", enemy->fire_timer, enemy->unstick_timer,
				enemy->path_valid, enemy->path.step_index, enemy->path.step_count);
			n64_log(line);
			snprintf(line, sizeof(line), "    throttle %.2f %.2f, velocity %.3f %.3f %.3f, animation %d frame %d, state %d",
				unit->unit.throttle.i, unit->unit.throttle.j, unit->object.translational_velocity.i,
				unit->object.translational_velocity.j, unit->object.translational_velocity.k,
				unit->object.animation.state.index, unit->object.animation.state.frame_index,
				unit->unit.animation.state);
			n64_log(line);
			snprintf(line, sizeof(line), "    seat %d weapon %d type %d base seat %d desired %d, weapon %d of %d",
				unit->unit.animation.seat_index, unit->unit.animation.weapon_index,
				unit->unit.animation.weapon_type_index, unit->unit.animation.base_seat_index,
				unit->unit.animation.desired_state, unit->unit.current_weapon_index, unit_get_weapon_count(enemy->unit_index));
			n64_log(line);
		}
	}
}

/* the HUD's line (port/n64/game/n64_render_game.c) */
int n64_firefight_status(
	char *text,
	int size)
{
	if (!firefight.active)
		return 0;
	snprintf(text, size, "WAVE %ld   LIVES %ld   SCORE %ld", firefight.wave, firefight.lives, firefight.score);
	return 1;
}

/* the menu's results */
int n64_firefight_results(
	char lines[][64],
	int maximum)
{
	int count = 0;
	short i;

	if (maximum < 2)
		return 0;
	snprintf(lines[count++], 64, "REACHED WAVE %ld", firefight.wave);
	snprintf(lines[count++], 64, "SCORE %ld", firefight.score);
	for (i = 0; i < firefight.player_count && count < maximum; i++)
		snprintf(lines[count++], 64, "PLAYER %d   %ld COVENANT", i + 1, firefight.kills[i]);
	return count;
}
