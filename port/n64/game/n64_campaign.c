/*
N64_CAMPAIGN.C

A campaign level on a cartridge of its own (HALO_N64_CAMPAIGN; the first is
a30, "Halo"). Halo's scripts, its actor AI and its cutscenes are not on the
N64, so the level is played without them:

- the encounters' squads (the map's own, read by tools/n64/campaign.py into
  rom:/levels/<map>.enc: which biped and weapon each actor is, how many by
  difficulty, where they start) are placed as the players come near; the
  squads Halo's scripts sent in waves (named wave_1_..., wave_2_..., brought
  by dropships) come one wave after another, the next once the last is
  down. A squad placed in the air (in a dropship's hold) is put on the
  ground below;
- each actor is driven here, a tick at a time, through unit_control() as
  Halo's AI drives units: the Covenant stand where they were placed until
  they see an enemy (or are hurt, or another of their squad has), then
  fight with Firefight's simplified AI (n64_firefight.c: keep their kind's
  range, strafe, fire in bursts, throw grenades, melee, find their way with
  Halo's pathfinding); the Marines follow the nearest player and fight the
  Covenant they see. A few of each at a time (each biped costs ~3 ms a
  tick): the rest of a squad comes as its first ones fall;
- there are no saved games: every few seconds the players are somewhere
  safe (on the ground, no Covenant close) is the checkpoint, and when they
  are all dead they come back there (n64_player_respawn_at, players.c)
  with the scenario's starting weapons;
- the scenery and the ambient sounds are made near the players only
  (stream_scenery): all of a30's at once filled Halo's object memory.
*/

#include "cseries.h"

#ifdef HALO_N64_CAMPAIGN

#include "game/game.h"
#include "game/game_engine.h"
#include "game/players.h"
#include "game/local_players.h"
#include "interface/hud_messaging.h"
#include "items/weapons.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "objects/object_types.h"
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

/* ---------- the encounter table (tools/n64/campaign.py encounter_table) */

struct campaign_kind_record
{
	unsigned long unit, weapon;
	byte team; /* 0 the players', 1 the Covenant's */
	byte pad[3];
};

struct campaign_squad_record
{
	char name[32], encounter[32];
	unsigned short kind;
	byte normal_count, insane_count; /* easy and normal; heroic and legendary */
	unsigned short first_location, location_count;
	unsigned short encounter_index, flags;
	short wave; /* 0, or the encounter's wave it is in */
};

struct campaign_location_record
{
	real x, y, z, facing;
};

typedef char campaign_squad_record_size_assert[sizeof(struct campaign_squad_record) == 78 ? 1 : -1];

/* ---------- constants */

/* how each kind of actor fights (by its biped: grunt, jackal, elite, the
Marines and the crew) */
enum
{
	_role_grunt = 0,
	_role_jackal,
	_role_elite,
	_role_marine,
	NUMBER_OF_ROLES
};

static struct campaign_role
{
	real near_range, far_range; /* the distance it keeps */
	real fire_range;
	short burst_ticks, pause_ticks; /* firing */
	real aim_error; /* radians at normal difficulty */
	short grenade_chance; /* in 1000 a tick while it may throw */
	short grenade_type; /* 0 fragmentation, 1 plasma */
	boolean holds_trigger; /* automatic weapons (a plasma pistol is pressed, not held) */
} const roles[NUMBER_OF_ROLES] =
{
	{ 8.f, 14.f, 22.f, 10, 30, 0.10f, 4, 1, FALSE },
	{ 9.f, 16.f, 24.f, 8, 26, 0.07f, 0, 1, FALSE },
	{ 3.f, 10.f, 25.f, 18, 20, 0.05f, 6, 1, TRUE },
	{ 6.f, 14.f, 25.f, 15, 25, 0.08f, 3, 0, TRUE },
};

/* by difficulty (easy, normal, heroic, legendary): the Covenant's aim and
pauses; the Marines' never change */
static real const aim_error_scale[4] = { 1.8f, 1.f, 0.7f, 0.45f };
static real const pause_scale[4] = { 1.6f, 1.f, 0.75f, 0.55f };

#define MAXIMUM_ACTORS 12
/* at once, of each side (each biped ~3 ms of a tick) */
#define MAXIMUM_COVENANT 6
#define MAXIMUM_ALLIES 3
/* the fallen kept a while (a body is ~7 KB of Halo's object memory) */
#define MAXIMUM_BODIES 4
#define BODY_TICKS (15 * TICKS_PER_SECOND)
/* an actor that found no room (Halo's object memory full: "OUT OF
OBJECTS") is tried again this much later; it used to count as placed and
down, and whole squads never came */
#define SPAWN_RETRY_TICKS (2 * TICKS_PER_SECOND)
#define MAXIMUM_SQUADS 256
/* a squad is placed when a player comes this near one of its starting
locations */
#define ACTIVATION_DISTANCE 38.f
#define ACTIVATION_HEIGHT 30.f
/* an actor this far (horizontally) from every living player, and not
fighting one, is put away -- deleted, its squad to come again from its
starting locations once a player is back within ACTIVATION_DISTANCE of
them (the user: "ensure the ai is shut down until the player is near":
everything placed kept running after the players moved on, each biped ~3 ms
of a tick); looked at every DORMANT_CHECK_TICKS */
#define DORMANT_DISTANCE 55.f
#define DORMANT_CHECK_TICKS 15
/* how far an actor sees an enemy to start fighting, and how far it fights */
#define ALERT_DISTANCE 22.f
#define TARGET_DISTANCE 45.f
/* a Marine stays this near the player it follows */
#define FOLLOW_DISTANCE 5.f
#define FOLLOW_RANGE 40.f
/* a starting location this far above the ground (a dropship's hold) is
moved down onto it */
#define GROUND_SEARCH 30.f
#define PATH_LIFETIME (5 * TICKS_PER_SECOND)
#define TICKS_BETWEEN_PATHS 2
#define SEARCH_SURFACES_PER_TICK 40
#define TICKS_BETWEEN_CHATTER (2 * TICKS_PER_SECOND)
#define CHECKPOINT_TICKS (2 * TICKS_PER_SECOND)
#define RESPAWN_TICKS (3 * TICKS_PER_SECOND)

/* the voices (dialogue_definitions.c), said through unit_dialogue.c
(tools/n64/merge.py DIALOG_VOCALIZATIONS: the ones kept) */
enum
{
	_vocalization_killed_enemy_player = 55,
	_vocalization_new_combat_alone = 108,
	_vocalization_unexpected_enemy = 111,
	_vocalization_grenade_throwing = 115,
	_vocalization_shooting = 149,
	_vocalization_taunt = 154,
	_vocalization_melee_attack = 179,
	_vocalization_celebration = 188,
};

boolean unit_vocalize(long unit_index, short vocalization_type, short priority);
boolean n64_ai_direct_path(real_point3d const *start, long start_surface, real_point3d const *goal,
	long goal_surface, struct path_result *path);
boolean n64_ai_clusters_may_see(long object_a, long object_b);
boolean n64_player_respawn_at(long player_index, real_point3d const *position, real facing);
char const *n64_loaded_map_name(void);
extern int n64_collision_any_hit;

/* ---------- state */

struct campaign_actor
{
	long unit_index;
	short role;
	short squad;
	short team; /* _game_team_player or _game_team_covenant */
	boolean alert;
	boolean target_visible;
	boolean trigger_down;
	boolean has_spoken;
	short strafe_direction, strafe_timer;
	short fire_timer; /* > 0 firing, < 0 pausing */
	short sight_timer, stuck_timer, unstick_timer, jump_timer, melee_timer, grenade_timer;
	short unseen_ticks;
	short chatter_timer;
	real last_body, last_shield;
	/* the way to where it goes (Halo's pathfinding, source/ai/path.c) */
	boolean path_valid;
	short path_age;
	struct path_result path;
	real_point3d path_goal;
	long direct_after;
	long target_unit;
	real_point3d home;
	real home_facing;
	real_point3d last_seen;
	real_point3d last_position;
	real_vector3d aim;
	real_vector2d unstick_throttle;
};

struct campaign_squad
{
	short spawned, alive, total;
	boolean active, complete;
};

/* the outcomes of placing an actor */
enum
{
	_spawn_placed,
	_spawn_impossible, /* nowhere to stand: counted as placed and down */
	_spawn_no_room /* tried again later */
};

static struct
{
	boolean active;
	boolean resolved;
	short difficulty;
	unsigned long random;
	/* the table */
	long kind_count, squad_count, location_count;
	struct campaign_kind_record *kinds;
	struct campaign_squad_record *squad_records;
	struct campaign_location_record *locations;
	short *kind_roles;
	struct campaign_squad squads[MAXIMUM_SQUADS];
	struct campaign_actor actors[MAXIMUM_ACTORS];
	short actor_count;
	struct
	{
		long unit_index;
		short timer;
	} bodies[MAXIMUM_BODIES];
	short body_count;
	/* paths: one search at a time, in parts (n64_firefight.c's) */
	short paths_this_tick, direct_tests_this_tick, path_cooldown;
	boolean searching;
	long search_unit;
	real_point3d search_goal;
	long last_chatter;
	/* the checkpoint, and how long the players have all been dead */
	boolean has_checkpoint;
	real_point3d checkpoint;
	real checkpoint_facing;
	long checkpoint_timer;
	long dead_ticks;
	long covenant_down;
	/* ticks before the squads try again after no room */
	short spawn_wait;
} campaign;

static struct path_state *campaign_path_state;

/* D-pad right's taps (xdk_input.c): each doubles the players' speed, up to
8 times, then back to 1 (for looking round a level quickly) */
int n64_campaign_speed_requests;
static int campaign_speed = 1;

/* ---------- small things */

static unsigned long cg_random(
	void)
{
	campaign.random = campaign.random * 1664525UL + 1013904223UL;
	return campaign.random >> 8;
}

/* -1 to 1 */
static real cg_random_real(
	void)
{
	return (real)(cg_random() & 0xFFFF) / 32768.f - 1.f;
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

static void message_all_ascii(
	char const *text)
{
	wchar_t wide[80];
	short i, local_player_index;

	for (i = 0; i < 79 && text[i]; i++)
		wide[i] = (wchar_t)text[i];
	wide[i] = 0;
	for (local_player_index = 0; local_player_index < 4; local_player_index++)
	{
		if (local_player_get_player_index(local_player_index) != NONE)
			hud_print_message(local_player_index, wide);
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

static void vocalize(
	struct campaign_actor *actor,
	short vocalization_type,
	short priority,
	boolean chatter)
{
	if (chatter && game_time_get() - campaign.last_chatter < TICKS_BETWEEN_CHATTER)
		return;
	if (unit_vocalize(actor->unit_index, vocalization_type, priority) && chatter)
		campaign.last_chatter = game_time_get();
}

/* the ground under a point (within GROUND_SEARCH), or FALSE */
static boolean ground_below(
	real_point3d const *point,
	real_point3d *ground)
{
	struct collision_result collision;
	real_point3d start = *point;
	real_vector3d down = { 0.f, 0.f, -GROUND_SEARCH };

	start.z += 0.5f;
	if (!collision_test_vector(FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit),
			&start, &down, NONE, &collision))
	{
		return FALSE;
	}
	*ground = collision.point;
	return TRUE;
}

static boolean line_of_sight(
	real_point3d const *from,
	real_point3d const *to,
	long ignore_object_index)
{
	struct collision_result collision;
	real_vector3d vector;
	boolean blocked;

	vector.i = to->x - from->x;
	vector.j = to->y - from->y;
	vector.k = to->z - from->z;
	n64_collision_any_hit = 1;
	blocked = collision_test_vector(
		FLAG(_collision_test_front_facing_surfaces_bit) | FLAG(_collision_test_structure_bit) |
			FLAG(_collision_test_objects_bit) | _collision_test_objects_sight_blocking_flags,
		from, &vector, ignore_object_index, &collision);
	n64_collision_any_hit = 0;
	return !blocked;
}

/* ---------- the table */

static boolean campaign_load_table(
	void)
{
	char path[64];
	struct
	{
		char magic[4];
		unsigned long version, kind_count, squad_count, location_count;
	} header;
	long file;
	short k;

	snprintf(path, sizeof(path), "rom:/levels/%s.enc", n64_loaded_map_name());
	file = n64_file_open(path);
	if (file < 0)
	{
		n64_log("n64: campaign: no encounter table");
		return FALSE;
	}
	if (n64_file_read(file, &header, sizeof(header)) != (long)sizeof(header) || memcmp(header.magic, "H64E", 4) ||
		header.version != 1)
	{
		n64_file_close(file);
		n64_log("n64: campaign: the encounter table was converted for another build");
		return FALSE;
	}
	campaign.kind_count = header.kind_count;
	campaign.squad_count = header.squad_count < MAXIMUM_SQUADS ? header.squad_count : MAXIMUM_SQUADS;
	campaign.location_count = header.location_count;
	campaign.kinds = malloc(header.kind_count * sizeof(struct campaign_kind_record) + 1);
	campaign.kind_roles = malloc(header.kind_count * sizeof(short) + 2);
	campaign.squad_records = malloc(header.squad_count * sizeof(struct campaign_squad_record) + 1);
	campaign.locations = malloc(header.location_count * sizeof(struct campaign_location_record) + 1);
	if (!campaign.kinds || !campaign.kind_roles || !campaign.squad_records || !campaign.locations)
	{
		n64_file_close(file);
		n64_log("n64: campaign: no memory for the encounter table");
		return FALSE;
	}
	n64_file_read(file, campaign.kinds, header.kind_count * sizeof(struct campaign_kind_record));
	n64_file_read(file, campaign.squad_records, header.squad_count * sizeof(struct campaign_squad_record));
	n64_file_read(file, campaign.locations, header.location_count * sizeof(struct campaign_location_record));
	n64_file_close(file);
	/* each kind's way of fighting, by its biped's name */
	for (k = 0; k < campaign.kind_count; k++)
	{
		long definition_index = (long)campaign.kinds[k].unit;
		char const *name = definition_index != NONE ? tag_get_name(definition_index) : "";

		if (campaign.kinds[k].team == 0)
			campaign.kind_roles[k] = _role_marine;
		else if (strstr(name, "elite"))
			campaign.kind_roles[k] = _role_elite;
		else if (strstr(name, "jackal"))
			campaign.kind_roles[k] = _role_jackal;
		else
			campaign.kind_roles[k] = _role_grunt;
	}
	{
		char line[96];

		snprintf(line, sizeof(line), "n64: campaign: %ld kinds of actor, %ld squads, %ld starting locations",
			campaign.kind_count, campaign.squad_count, campaign.location_count);
		n64_log(line);
	}
	return TRUE;
}

/* ---------- placing the squads */

static short count_side(
	short team)
{
	short i, count = 0;

	for (i = 0; i < campaign.actor_count; i++)
	{
		if (campaign.actors[i].team == team)
			count++;
	}
	return count;
}

/* FALSE: there was a weapon to give and no room for it */
static boolean give_weapon(
	long unit_index,
	long definition_index)
{
	struct object_placement_data placement;
	long weapon_index;

	if (definition_index == NONE)
		return TRUE;
	object_placement_data_new(&placement, definition_index, NONE);
	weapon_index = object_new(&placement);
	if (weapon_index == NONE)
		return FALSE;
	unit_add_weapon_to_inventory(unit_index, weapon_index, 2);
	return TRUE;
}

/* one actor of a squad at one of its starting locations: placed, or not
(nowhere to stand, or no room in Halo's object memory) */
static short spawn_actor(
	short squad_index)
{
	struct campaign_squad_record const *record = &campaign.squad_records[squad_index];
	struct campaign_squad *squad = &campaign.squads[squad_index];
	struct campaign_kind_record const *kind = &campaign.kinds[record->kind];
	struct campaign_location_record const *location =
		&campaign.locations[record->first_location + squad->spawned % record->location_count];
	struct object_placement_data placement;
	struct campaign_actor *actor;
	real_point3d point, ground;
	long unit_index;

	if ((long)kind->unit == NONE)
		return _spawn_impossible;
	if (campaign.actor_count >= MAXIMUM_ACTORS)
		return _spawn_no_room;
	point.x = location->x;
	point.y = location->y;
	point.z = location->z;
	/* (a dropship's hold: on the ground below; nothing below, not placed) */
	if (!ground_below(&point, &ground))
		return _spawn_impossible;
	if (point.z - ground.z > 1.5f)
		point = ground;
	point.z += 0.25f;
	object_placement_data_new(&placement, (long)kind->unit, NONE);
	placement.position = point;
	placement.forward.i = (real)cos(location->facing);
	placement.forward.j = (real)sin(location->facing);
	placement.forward.k = 0.f;
	placement.up.i = 0.f;
	placement.up.j = 0.f;
	placement.up.k = 1.f;
	placement.owner_team_index = kind->team ? _game_team_covenant : _game_team_player;
	unit_index = object_new(&placement);
	if (unit_index == NONE)
		return _spawn_no_room;
	/* (as a player's or an actor's: a unit nobody controls has its throttle
	and aim reset every tick, units.c) */
	unit_set_actively_controlled(unit_index, TRUE);
	/* (an actor without its weapon cannot fight: none, then) */
	if (!give_weapon(unit_index, (long)kind->weapon))
	{
		object_delete(unit_index);
		return _spawn_no_room;
	}
	{
		struct unit_datum *unit = unit_get(unit_index);
		short role = campaign.kind_roles[record->kind];

		if (roles[role].grenade_chance && roles[role].grenade_type < NUMBER_OF_UNIT_GRENADE_TYPES)
			unit->unit.grenade_counts[roles[role].grenade_type] = (char)(role == _role_elite ? 2 : 1);
	}
	actor = &campaign.actors[campaign.actor_count++];
	csmemset(actor, 0, sizeof(*actor));
	actor->unit_index = unit_index;
	actor->role = campaign.kind_roles[record->kind];
	actor->squad = squad_index;
	actor->team = kind->team ? _game_team_covenant : _game_team_player;
	actor->strafe_direction = cg_random() & 1 ? 1 : -1;
	actor->fire_timer = -(short)(10 + cg_random() % 30);
	actor->sight_timer = (short)(cg_random() % 10);
	actor->target_unit = NONE;
	actor->home = point;
	actor->home_facing = location->facing;
	actor->last_position = point;
	actor->aim = placement.forward;
	actor->last_body = 1.f;
	actor->last_shield = 1.f;
	squad->spawned++;
	squad->alive++;
	return _spawn_placed;
}

/* the nearest living player to a point (its horizontal distance and height
difference), or NONE */
static long nearest_player(
	real_point3d const *point,
	real *distance,
	real *height)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long best = NONE;

	*distance = 1e9f;
	*height = 1e9f;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		if (unit_alive(player->unit_index))
		{
			real_point3d const *p = &object_get(player->unit_index)->object.position;
			real d = horizontal_distance(point, p);

			if (d < *distance)
			{
				*distance = d;
				*height = fabsf(point->z - p->z);
				best = player->unit_index;
			}
		}
	}
	return best;
}

/* the squads near the players placed (the waves one after another), a
little at a time: up to the actors each side may have */
static void place_squads(
	void)
{
	short s;

	if (campaign.spawn_wait > 0)
		return;
	for (s = 0; s < campaign.squad_count; s++)
	{
		struct campaign_squad_record const *record = &campaign.squad_records[s];
		struct campaign_squad *squad = &campaign.squads[s];
		short team = campaign.kinds[record->kind].team ? _game_team_covenant : _game_team_player;
		short limit = team == _game_team_covenant ? MAXIMUM_COVENANT : MAXIMUM_ALLIES;

		if (squad->complete)
			continue;
		if (!squad->active)
		{
			real distance, height;
			short l;
			boolean near = FALSE;

			/* (a wave after the last: once the encounter's wave before is
			all placed and down) */
			if (record->wave > 1)
			{
				short o;
				boolean waiting = FALSE;

				for (o = 0; o < campaign.squad_count && !waiting; o++)
				{
					struct campaign_squad_record const *other = &campaign.squad_records[o];

					if (other->encounter_index == record->encounter_index && other->wave == record->wave - 1 &&
						(!campaign.squads[o].complete || campaign.squads[o].alive > 0))
					{
						waiting = TRUE;
					}
				}
				if (waiting)
					continue;
			}
			for (l = 0; l < record->location_count && !near; l++)
			{
				struct campaign_location_record const *location = &campaign.locations[record->first_location + l];
				real_point3d point;

				point.x = location->x;
				point.y = location->y;
				point.z = location->z;
				nearest_player(&point, &distance, &height);
				near = distance < ACTIVATION_DISTANCE && height < ACTIVATION_HEIGHT;
			}
			if (!near)
				continue;
			squad->active = TRUE;
			squad->total = campaign.difficulty >= 2 ? record->insane_count : record->normal_count;
			if (n64_debug_flag("debug_campaign_log"))
			{
				char line[96];

				snprintf(line, sizeof(line), "campaign: squad %.31s/%.31s comes, %d", record->encounter,
					record->name, squad->total);
				n64_log(line);
			}
		}
		/* (one actor a tick at most, as there is room) */
		if (squad->spawned < squad->total && count_side(team) < limit)
		{
			short outcome = spawn_actor(s);

			/* (one with nowhere to stand counts as placed and down; one
			with no room comes later) */
			if (outcome == _spawn_impossible)
				squad->spawned++;
			else if (outcome == _spawn_no_room)
				campaign.spawn_wait = SPAWN_RETRY_TICKS;
			return;
		}
		if (squad->spawned >= squad->total)
			squad->complete = TRUE;
	}
}

/* ---------- the actors' minds (n64_firefight.c's, with an enemy to fight
that may be a player or another actor) */

/* the nearest enemy unit of an actor within TARGET_DISTANCE, or NONE: the
players and the Marines for the Covenant, the Covenant for the Marines */
static long nearest_enemy(
	struct campaign_actor const *actor,
	real *distance_out)
{
	real_point3d const *position = &object_get(actor->unit_index)->object.position;
	long best = NONE;
	real best_distance = TARGET_DISTANCE;
	short i;

	if (actor->team == _game_team_covenant)
	{
		struct data_iterator iterator;
		struct player_datum *player;

		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
		{
			if (unit_alive(player->unit_index))
			{
				real d = horizontal_distance(position, &object_get(player->unit_index)->object.position);

				if (d < best_distance)
				{
					best_distance = d;
					best = player->unit_index;
				}
			}
		}
	}
	for (i = 0; i < campaign.actor_count; i++)
	{
		struct campaign_actor const *other = &campaign.actors[i];

		if (other->team != actor->team && unit_alive(other->unit_index))
		{
			real d = horizontal_distance(position, &object_get(other->unit_index)->object.position);

			if (d < best_distance)
			{
				best_distance = d;
				best = other->unit_index;
			}
		}
	}
	*distance_out = best_distance;
	return best;
}

static void campaign_search_step(
	void)
{
	struct campaign_actor *actor = NULL;
	boolean found = FALSE;
	short i;

	if (!campaign.searching || !campaign_path_state)
		return;
	for (i = 0; i < campaign.actor_count; i++)
	{
		if (campaign.actors[i].unit_index == campaign.search_unit)
		{
			actor = &campaign.actors[i];
			break;
		}
	}
	if (!actor)
	{
		campaign.searching = FALSE;
		campaign.path_cooldown = TICKS_BETWEEN_PATHS;
		return;
	}
	if (!path_state_find_continue(campaign_path_state, SEARCH_SURFACES_PER_TICK, &found))
		return;
	campaign.searching = FALSE;
	campaign.path_cooldown = TICKS_BETWEEN_PATHS;
	if (path_state_build_path(campaign_path_state, &actor->path) && actor->path.step_count > 0)
	{
		actor->path_valid = TRUE;
		actor->path.step_index = 0;
		actor->path_age = 0;
		actor->path_goal = campaign.search_goal;
	}
}

/* a path to a unit (n64_firefight.c plan_path) */
static void plan_path(
	struct campaign_actor *actor,
	long target_unit)
{
	struct path_input input;
	real_point3d start, goal;
	long start_surface, goal_surface;
	boolean search = !campaign.paths_this_tick && campaign.path_cooldown <= 0 && !campaign.searching;
	boolean direct = game_time_get() >= actor->direct_after && campaign.direct_tests_this_tick < 2;

	if (!campaign_path_state || (!search && !direct) ||
		(campaign.searching && campaign.search_unit == actor->unit_index))
	{
		return;
	}
	start_surface = biped_find_pathfinding_surface_index(actor->unit_index, &start);
	goal_surface = biped_find_pathfinding_surface_index(target_unit, &goal);
	if (start_surface == NONE || goal_surface == NONE)
	{
		if (search)
		{
			campaign.paths_this_tick++;
			campaign.path_cooldown = TICKS_BETWEEN_PATHS;
			actor->path_age = 0;
			actor->path_valid = FALSE;
		}
		return;
	}
	if (direct)
	{
		campaign.direct_tests_this_tick++;
		if (n64_ai_direct_path(&start, start_surface, &goal, goal_surface, &actor->path))
		{
			actor->path_valid = TRUE;
			actor->path_age = 0;
			actor->path_goal = goal;
			return;
		}
		actor->direct_after = game_time_get() + TICKS_PER_SECOND;
	}
	if (!search)
		return;
	campaign.paths_this_tick++;
	campaign.path_cooldown = TICKS_BETWEEN_PATHS;
	actor->path_age = 0;
	actor->path_valid = FALSE;
	path_input_new(&input, 0.3f, FALSE, actor->unit_index);
	path_input_set_start(&input, &start, start_surface);
	path_input_set_search_bounds(&input, 2.5f * distance3d(&start, &goal) + 15.f);
	path_state_new(&input, campaign_path_state, NULL);
	path_state_destination(campaign_path_state, &goal, goal_surface, 40.f);
	if (!path_state_find_begin(campaign_path_state))
		return;
	campaign.searching = TRUE;
	campaign.search_unit = actor->unit_index;
	campaign.search_goal = goal;
	campaign_search_step();
}

static boolean follow_path(
	struct campaign_actor *actor,
	real_point3d const *position,
	real_vector2d *direction)
{
	struct path_result *path = &actor->path;

	while (actor->path_valid && path->step_index < path->step_count)
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
	actor->path_valid = FALSE;
	return FALSE;
}

/* the whole squad of an actor that has seen an enemy (or been hurt) fights */
static void alert_squad(
	short squad)
{
	short i;

	for (i = 0; i < campaign.actor_count; i++)
	{
		if (campaign.actors[i].squad == squad)
			campaign.actors[i].alert = TRUE;
	}
}

static void stand(
	struct campaign_actor *actor,
	struct unit_datum *unit,
	real_vector3d const *look)
{
	struct unit_control_data control;

	csmemset(&control, 0, sizeof(control));
	control.animation_state = actor->alert ? _unit_animation_state_in_combat : _unit_animation_state_alert;
	control.weapon_index = unit->unit.current_weapon_index;
	control.grenade_index = NONE;
	control.zoom_level = NONE;
	control.facing_vector = control.aiming_vector = control.looking_vector = *look;
	unit_control(actor->unit_index, &control);
}

static void think(
	struct campaign_actor *actor)
{
	struct campaign_role const *role = &roles[actor->role];
	struct unit_datum *unit = unit_get(actor->unit_index);
	struct unit_control_data control;
	real_point3d eye, target_eye, target_position;
	long target_unit = NONE;
	real distance = 0.f, enemy_distance;
	real_vector3d to_target, desired;
	real_vector2d move = { 0.f, 0.f };
	real forward = 0.f, strafe = 0.f;
	boolean approaching = FALSE, moving_along_path = FALSE, following = FALSE;
	word flags = 0;
	real trigger = 0.f;
	short difficulty = actor->team == _game_team_covenant ? campaign.difficulty : 1;
	short interval = 2;

	unit_get_camera_position(actor->unit_index, &eye);
	/* hurt: the squad fights */
	if (unit->object.body_vitality < actor->last_body - 0.01f || unit->object.shield_vitality < actor->last_shield - 0.01f)
	{
		if (!actor->alert)
			alert_squad(actor->squad);
	}
	actor->last_body = unit->object.body_vitality;
	actor->last_shield = unit->object.shield_vitality;

	target_unit = nearest_enemy(actor, &enemy_distance);
	if (target_unit != NONE)
	{
		unit_get_camera_position(target_unit, &target_eye);
		target_position = object_get(target_unit)->object.position;
		distance = horizontal_distance(&unit->object.position, &target_position);
		/* can it see the enemy? (a few times a second) */
		actor->sight_timer -= interval;
		if (actor->sight_timer <= 0 || actor->target_unit != target_unit)
		{
			actor->sight_timer = 12;
			actor->target_visible = (actor->alert ? distance < TARGET_DISTANCE : distance < ALERT_DISTANCE) &&
				n64_ai_clusters_may_see(actor->unit_index, target_unit) &&
				line_of_sight(&eye, &target_eye, actor->unit_index);
		}
		actor->target_unit = target_unit;
		if (actor->target_visible && !actor->alert)
			alert_squad(actor->squad);
	}
	else
	{
		actor->target_unit = NONE;
		actor->target_visible = FALSE;
	}

	/* not fighting: the Covenant stand where they were placed, the Marines
	follow the nearest player */
	if (!actor->alert || target_unit == NONE)
	{
		real player_distance, height;
		long player_unit = actor->team == _game_team_player ?
			nearest_player(&unit->object.position, &player_distance, &height) : NONE;

		if (player_unit != NONE && player_distance > FOLLOW_DISTANCE && player_distance < FOLLOW_RANGE)
		{
			target_unit = player_unit;
			target_position = object_get(player_unit)->object.position;
			unit_get_camera_position(player_unit, &target_eye);
			distance = player_distance;
			following = TRUE;
			approaching = TRUE;
		}
		else
		{
			real_vector3d look;

			if (player_unit != NONE)
			{
				real_point3d player_eye;

				unit_get_camera_position(player_unit, &player_eye);
				look.i = player_eye.x - eye.x;
				look.j = player_eye.y - eye.y;
				look.k = 0.f;
			}
			else
			{
				look.i = (real)cos(actor->home_facing);
				look.j = (real)sin(actor->home_facing);
				look.k = 0.f;
			}
			normalize(&look);
			actor->aim = look;
			stand(actor, unit, &look);
			return;
		}
	}

	if (actor->target_visible && !following)
	{
		if (!actor->has_spoken || actor->unseen_ticks > 8 * TICKS_PER_SECOND)
		{
			actor->has_spoken = TRUE;
			vocalize(actor, distance < 5.f ? _vocalization_unexpected_enemy : _vocalization_new_combat_alone,
				_unit_speech_shout, TRUE);
		}
		actor->last_seen = target_eye;
		actor->unseen_ticks = 0;
	}
	else
	{
		if (actor->unseen_ticks < 32000)
			actor->unseen_ticks += interval;
		if (actor->last_seen.x == 0.f && actor->last_seen.y == 0.f)
			actor->last_seen = target_eye;
	}

	if (!following)
		approaching = !actor->target_visible || distance > role->far_range;
	if (approaching)
	{
		actor->path_age += interval;
		if (!actor->path_valid || actor->path_age > PATH_LIFETIME ||
			horizontal_distance(&actor->path_goal, &target_position) > 7.f)
		{
			plan_path(actor, target_unit);
		}
		if (follow_path(actor, &unit->object.position, &move))
			moving_along_path = TRUE;
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
	}

	/* where to look: at the enemy in sight (missing by the difficulty's
	error), else the way it goes */
	if (!following && (actor->target_visible || !moving_along_path))
	{
		to_target.i = actor->last_seen.x - eye.x;
		to_target.j = actor->last_seen.y - eye.y;
		to_target.k = actor->last_seen.z - eye.z - 0.1f;
		desired = to_target;
		normalize(&desired);
		if (actor->target_visible)
		{
			real error = role->aim_error * aim_error_scale[difficulty];

			desired.i += error * cg_random_real();
			desired.j += error * cg_random_real();
			desired.k += error * 0.6f * cg_random_real();
			normalize(&desired);
		}
	}
	else
	{
		desired.i = move.i;
		desired.j = move.j;
		desired.k = 0.f;
		if (move.i == 0.f && move.j == 0.f)
			desired = actor->aim;
	}
	actor->aim.i += (desired.i - actor->aim.i) * 0.6f;
	actor->aim.j += (desired.j - actor->aim.j) * 0.6f;
	actor->aim.k += (desired.k - actor->aim.k) * 0.6f;
	normalize(&actor->aim);

	if (approaching)
	{
		real fi = actor->aim.i, fj = actor->aim.j;
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
		if (distance < role->near_range)
			forward = actor->role == _role_grunt ? -0.8f : -0.4f;
		actor->strafe_timer -= interval;
		if (actor->strafe_timer <= 0)
		{
			actor->strafe_timer = (short)(25 + cg_random() % 50);
			actor->strafe_direction = (short)(cg_random() & 1 ? 1 : -1);
		}
		strafe = 0.75f * actor->strafe_direction;
	}

	/* stuck: around for a while, a jump, a new path */
	if (actor->unstick_timer > 0)
	{
		actor->unstick_timer -= interval;
		forward = actor->unstick_throttle.i;
		strafe = actor->unstick_throttle.j;
	}
	else if (((game_time_get() / interval) & 7) == (actor->unit_index & 7))
	{
		real moved = horizontal_distance(&unit->object.position, &actor->last_position);

		actor->last_position = unit->object.position;
		if ((forward != 0.f || strafe != 0.f) && moved < 0.25f)
		{
			if (++actor->stuck_timer >= 2)
			{
				actor->stuck_timer = 0;
				actor->unstick_timer = (short)(16 + cg_random() % 20);
				actor->unstick_throttle.i = cg_random_real() < 0.f ? -0.5f : 0.4f;
				actor->unstick_throttle.j = cg_random() & 1 ? 1.f : -1.f;
				actor->jump_timer = interval;
				actor->path_valid = FALSE;
			}
		}
		else
			actor->stuck_timer = 0;
	}
	if (actor->jump_timer > 0)
	{
		actor->jump_timer -= interval;
		flags |= FLAG(_unit_control_jump_bit);
	}

	if (!following)
	{
		/* firing: bursts and pauses while the enemy is in sight and range */
		if (actor->target_visible && distance < role->fire_range)
		{
			if (actor->fire_timer > 0)
			{
				actor->fire_timer -= interval;
				actor->trigger_down = role->holds_trigger ? TRUE : !actor->trigger_down;
				if (actor->trigger_down)
				{
					trigger = 1.f;
					flags |= FLAG(_unit_control_weapon_primary_trigger_bit);
				}
				if (actor->fire_timer <= 0)
					actor->fire_timer = -(short)(role->pause_ticks * pause_scale[difficulty] + cg_random() % 12);
				actor->chatter_timer -= interval;
				if (actor->chatter_timer <= 0)
				{
					actor->chatter_timer = (short)((8 + cg_random() % 10) * TICKS_PER_SECOND);
					vocalize(actor, cg_random() & 1 ? _vocalization_taunt : _vocalization_shooting,
						_unit_speech_shout, TRUE);
				}
			}
			else
			{
				actor->fire_timer += interval;
				if (actor->fire_timer >= 0)
					actor->fire_timer = (short)(role->burst_ticks + cg_random() % 6);
			}
		}
		/* melee up close */
		if (actor->melee_timer > 0)
			actor->melee_timer -= interval;
		else if (distance < 1.6f && actor->target_visible)
		{
			flags |= FLAG(_unit_control_use_equipment_bit);
			actor->melee_timer = 30;
			vocalize(actor, _vocalization_melee_attack, _unit_speech_exclamation, FALSE);
		}
		/* a grenade now and then */
		if (actor->grenade_timer > 0)
			actor->grenade_timer -= interval;
		else if (role->grenade_chance && actor->target_visible && distance > 7.f && distance < 20.f &&
			(long)(cg_random() % 1000) < role->grenade_chance * interval)
		{
			flags |= FLAG(_unit_control_throw_grenade_bit);
			actor->grenade_timer = 8 * TICKS_PER_SECOND;
			vocalize(actor, _vocalization_grenade_throwing, _unit_speech_shout, FALSE);
		}
	}

	if (forward > 1.f) forward = 1.f;
	if (forward < -1.f) forward = -1.f;
	if (strafe > 1.f) strafe = 1.f;
	if (strafe < -1.f) strafe = -1.f;
	csmemset(&control, 0, sizeof(control));
	control.animation_state = _unit_animation_state_in_combat;
	control.control_flags = flags;
	control.weapon_index = unit->unit.current_weapon_index;
	control.grenade_index = roles[actor->role].grenade_type;
	control.zoom_level = NONE;
	control.throttle.i = forward;
	control.throttle.j = strafe;
	control.throttle.k = 0.f;
	control.primary_trigger = trigger;
	control.facing_vector = control.aiming_vector = control.looking_vector = actor->aim;
	unit_control(actor->unit_index, &control);
}

/* ---------- the fallen */

static void body_fell(
	long unit_index)
{
	if (!object_try_and_get(unit_index))
		return;
	if (campaign.body_count == MAXIMUM_BODIES)
	{
		short oldest = 0, i;

		for (i = 1; i < campaign.body_count; i++)
		{
			if (campaign.bodies[i].timer < campaign.bodies[oldest].timer)
				oldest = i;
		}
		if (object_try_and_get(campaign.bodies[oldest].unit_index))
			object_delete(campaign.bodies[oldest].unit_index);
		campaign.bodies[oldest] = campaign.bodies[--campaign.body_count];
	}
	campaign.bodies[campaign.body_count].unit_index = unit_index;
	campaign.bodies[campaign.body_count].timer = BODY_TICKS;
	campaign.body_count++;
}

/* ---------- checkpoints */

static void campaign_checkpoints(
	void)
{
	struct data_iterator iterator;
	struct player_datum *player;

	if (players_are_all_dead())
	{
		/* (all dead: back at the checkpoint a few seconds later) */
		if (++campaign.dead_ticks < RESPAWN_TICKS || !campaign.has_checkpoint)
			return;
		campaign.dead_ticks = 0;
		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
		{
			if (player->unit_index == NONE)
				n64_player_respawn_at(iterator.datum_index, &campaign.checkpoint, campaign.checkpoint_facing);
		}
		message_all_ascii("Back at the checkpoint");
		return;
	}
	campaign.dead_ticks = 0;
	if (--campaign.checkpoint_timer > 0)
		return;
	campaign.checkpoint_timer = CHECKPOINT_TICKS;
	/* a living player on the ground with no Covenant near: the checkpoint */
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL)
	{
		struct biped_datum *biped = biped_try_and_get(player->unit_index);
		boolean safe = biped && unit_alive(player->unit_index) && biped->object.parent_object_index == NONE &&
			!TEST_FLAG(biped->biped.flags, _biped_airborne_bit);
		short i;

		for (i = 0; i < campaign.actor_count && safe; i++)
		{
			struct campaign_actor const *actor = &campaign.actors[i];

			if (actor->team == _game_team_covenant && unit_alive(actor->unit_index) &&
				distance3d(&object_get(actor->unit_index)->object.position, &biped->object.position) < 15.f)
			{
				safe = FALSE;
			}
		}
		if (!safe)
			continue;
		if (campaign.has_checkpoint && distance3d(&campaign.checkpoint, &biped->object.position) > 25.f)
			message_all_ascii("Checkpoint");
		campaign.has_checkpoint = TRUE;
		campaign.checkpoint = biped->object.position;
		campaign.checkpoint.z += 0.1f;
		campaign.checkpoint_facing = (real)atan2(biped->unit.desired_facing_vector.j, biped->unit.desired_facing_vector.i);
		break;
	}
}

/* ---------- the scenery near the players

The trees, rocks and ambient sounds (tools/n64/campaign.py
STREAMED_PLACEMENTS, left out of the start's placing) are made as a player
comes within SCENERY_NEAR of them and deleted past SCENERY_FAR. All of
a30's at once took ~230 KB of Halo's object memory and left no room for the
encounters' actors (~7 KB a biped): "OUT OF OBJECTS", few Covenant. Past
the fog's end (55 units, one view) nothing shows, so what is made there is
still hidden; a share of the placements is looked at a tick, a few made or
deleted, all of them the first time. A placement with no room is tried
again a while later (each try Halo prints "OUT OF OBJECTS") */
#define SCENERY_NEAR 58.f
#define SCENERY_FAR 66.f
#define SCENERY_CHECKS_PER_TICK 64
#define SCENERY_CHANGES_PER_TICK 6
#define SCENERY_RETRY_TICKS (2 * TICKS_PER_SECOND)

static short const streamed_types[] = { _object_type_scenery, _object_type_sound_scenery };
#define NUMBER_OF_STREAMED_TYPES ((short)NUMBEROF(streamed_types))

static struct
{
	/* each streamed type's placements: the object made of each (NONE) */
	long *objects[NUMBER_OF_STREAMED_TYPES];
	long counts[NUMBER_OF_STREAMED_TYPES];
	short type, wait;
	long cursor;
	boolean filled;
	short made; /* in the world now */
} scenery;

static void scenery_free(
	void)
{
	short t;

	for (t = 0; t < NUMBER_OF_STREAMED_TYPES; t++)
	{
		if (scenery.objects[t])
			free(scenery.objects[t]);
	}
	csmemset(&scenery, 0, sizeof(scenery));
}

static void scenery_begin(
	void)
{
	struct scenario *scenario = global_scenario_get();
	short t;

	scenery_free();
	for (t = 0; t < NUMBER_OF_STREAMED_TYPES && scenario; t++)
	{
		struct tag_block *placements = scenario_get_object_type_scenario_datums(scenario, streamed_types[t], NULL);
		long i;

		scenery.counts[t] = placements->count;
		scenery.objects[t] = placements->count ? malloc(placements->count * sizeof(long)) : NULL;
		if (!scenery.objects[t])
		{
			scenery.counts[t] = 0;
			continue;
		}
		for (i = 0; i < placements->count; i++)
			scenery.objects[t][i] = NONE;
	}
}

/* the horizontal distance (squared) from the nearest living player */
static real nearest_player_squared(
	real_point3d const *point,
	real_point3d const *players,
	short player_count)
{
	real best = 1e18f;
	short p;

	for (p = 0; p < player_count; p++)
	{
		real dx = point->x - players[p].x, dy = point->y - players[p].y;

		if (dx * dx + dy * dy < best)
			best = dx * dx + dy * dy;
	}
	return best;
}

static void stream_scenery(
	void)
{
	struct scenario *scenario = global_scenario_get();
	struct data_iterator iterator;
	struct player_datum *player;
	real_point3d players[4];
	short player_count = 0, changes = 0;
	long checks;

	if (!scenario)
		return;
	data_iterator_new(&iterator, player_data);
	while ((player = data_iterator_next(&iterator)) != NULL && player_count < 4)
	{
		if (unit_alive(player->unit_index))
			players[player_count++] = object_get(player->unit_index)->object.position;
	}
	if (!player_count)
		return;
	if (scenery.wait > 0)
		scenery.wait--;
	/* (the first time: every placement, every change) */
	checks = scenery.filled ? SCENERY_CHECKS_PER_TICK : 0x7FFFFFFF;
	while (checks-- > 0 && (!scenery.filled || changes < SCENERY_CHANGES_PER_TICK))
	{
		short t = scenery.type;
		long element_size;
		struct tag_block *placements = scenario_get_object_type_scenario_datums(scenario, streamed_types[t],
			&element_size);
		struct tag_block *palette = scenario_get_object_type_scenario_palette(scenario, streamed_types[t]);
		long i = scenery.cursor;

		if (i >= scenery.counts[t])
		{
			/* (the next type; all of them looked at: filled) */
			scenery.cursor = 0;
			scenery.type = (short)((t + 1) % NUMBER_OF_STREAMED_TYPES);
			if (scenery.type == 0)
			{
				scenery.filled = TRUE;
				if (checks > SCENERY_CHECKS_PER_TICK)
					checks = 0;
			}
			continue;
		}
		scenery.cursor++;
		{
			struct scenario_object_datum *placement =
				(struct scenario_object_datum *)tag_block_get_element_with_size(placements, i, element_size);
			long *object_index = &scenery.objects[t][i];
			real distance;

			if (placement->palette_entry_index == NONE)
				continue;
			/* (deleted by the game meanwhile: none) */
			if (*object_index != NONE && !object_try_and_get(*object_index))
			{
				*object_index = NONE;
				scenery.made--;
			}
			distance = nearest_player_squared(&placement->position, players, player_count);
			if (*object_index == NONE && distance < SCENERY_NEAR * SCENERY_NEAR && !scenery.wait)
			{
				*object_index = object_new_from_scenario(placement, palette);
				if (*object_index != NONE)
					scenery.made++;
				else
					scenery.wait = SCENERY_RETRY_TICKS;
				changes++;
			}
			else if (*object_index != NONE && distance > SCENERY_FAR * SCENERY_FAR)
			{
				object_delete(*object_index);
				*object_index = NONE;
				scenery.made--;
				changes++;
			}
		}
	}
}

/* ---------- public code */

/* the encounter table given back (in the game's units free is cseries.h's
debug_free, which halts on NULL: none freed that was never made) */
static void campaign_free_tables(
	void)
{
	if (campaign.kinds)
		free(campaign.kinds);
	if (campaign.kind_roles)
		free(campaign.kind_roles);
	if (campaign.squad_records)
		free(campaign.squad_records);
	if (campaign.locations)
		free(campaign.locations);
	campaign.kinds = NULL;
	campaign.kind_roles = NULL;
	campaign.squad_records = NULL;
	campaign.locations = NULL;
	scenery_free();
}

void n64_campaign_begin(
	int difficulty)
{
	short i;

	campaign_free_tables();
	csmemset(&campaign, 0, sizeof(campaign));
	campaign_speed = 1;
	n64_campaign_speed_requests = 0;
	campaign.active = TRUE;
	campaign.difficulty = (short)PIN(difficulty, 0, 3);
	campaign.random = n64_ticks();
	for (i = 0; i < MAXIMUM_SQUADS; i++)
		campaign.squads[i].active = FALSE;
	if (!campaign_path_state)
		campaign_path_state = (struct path_state *)malloc(sizeof(struct path_state));
	/* (the paths' last stage steers around nearby objects from the AI's
	own records, which the N64 has not got: n64_firefight.c) */
	ai_debug.path_disable_obstacle_avoidance = TRUE;
}

/* the game over: what the campaign made for it given back (else it stays
through the menu, in the middle of what the next map wants) */
void n64_campaign_stop(
	void)
{
	campaign.active = FALSE;
	campaign.searching = FALSE;
	if (campaign_path_state)
	{
		free(campaign_path_state);
		campaign_path_state = NULL;
	}
	campaign_free_tables();
	campaign.squad_count = campaign.actor_count = campaign.body_count = 0;
	campaign.resolved = FALSE;
}

int n64_campaign_active(
	void)
{
	return campaign.active;
}

/* a tick, before the game's own */
void n64_campaign_tick(
	void)
{
	short i;

	if (!campaign.active || !game_in_progress())
		return;
	if (!campaign.resolved)
	{
		campaign.resolved = TRUE;
		if (!campaign_load_table())
			campaign.squad_count = 0;
		scenery_begin();
	}
	stream_scenery();
	campaign_checkpoints();
	if (campaign.spawn_wait > 0)
		campaign.spawn_wait--;

	/* the players' speed (D-pad right): players.c's speed multiplier, which
	bipeds.c's movement takes (and its acceleration, HALO_N64_CAMPAIGN) */
	if (n64_campaign_speed_requests)
	{
		struct data_iterator iterator;
		struct player_datum *player;
		char text[32];

		campaign_speed = campaign_speed >= 8 ? 1 : campaign_speed * 2;
		n64_campaign_speed_requests = 0;
		data_iterator_new(&iterator, player_data);
		while ((player = data_iterator_next(&iterator)) != NULL)
			player->speed_multiplier = (real)campaign_speed;
		snprintf(text, sizeof(text), "Speed x%d", campaign_speed);
		message_all_ascii(text);
	}

	/* the bodies, a while after they fell: gone */
	for (i = 0; i < campaign.body_count; )
	{
		if (--campaign.bodies[i].timer <= 0)
		{
			if (object_try_and_get(campaign.bodies[i].unit_index))
				object_delete(campaign.bodies[i].unit_index);
			campaign.bodies[i] = campaign.bodies[--campaign.body_count];
		}
		else
			i++;
	}
	/* the fallen out of the list (their squads' next come) */
	for (i = 0; i < campaign.actor_count; )
	{
		struct campaign_actor *actor = &campaign.actors[i];

		if (!unit_alive(actor->unit_index))
		{
			if (actor->squad >= 0 && actor->squad < MAXIMUM_SQUADS)
				campaign.squads[actor->squad].alive--;
			if (actor->team == _game_team_covenant)
				campaign.covenant_down++;
			body_fell(actor->unit_index);
			campaign.actors[i] = campaign.actors[--campaign.actor_count];
		}
		else
			i++;
	}
	/* the actors the players left far behind: put away (DORMANT_DISTANCE) */
	if (!(game_time_get() % DORMANT_CHECK_TICKS))
	{
		for (i = 0; i < campaign.actor_count; )
		{
			struct campaign_actor *actor = &campaign.actors[i];
			real distance, height;

			if (nearest_player(&object_get(actor->unit_index)->object.position, &distance, &height) != NONE &&
				distance > DORMANT_DISTANCE && !actor->target_visible)
			{
				if (actor->squad >= 0 && actor->squad < MAXIMUM_SQUADS)
				{
					struct campaign_squad *squad = &campaign.squads[actor->squad];

					squad->alive--;
					squad->spawned--;
					squad->complete = FALSE;
					squad->active = FALSE;
				}
				object_delete(actor->unit_index);
				campaign.actors[i] = campaign.actors[--campaign.actor_count];
			}
			else
				i++;
		}
	}
	/* new squads as the players come near (every few ticks) */
	if (!(game_time_get() % 5))
		place_squads();

	campaign.paths_this_tick = 0;
	campaign.direct_tests_this_tick = 0;
	if (campaign.path_cooldown > 0)
		campaign.path_cooldown--;
	campaign_search_step();
	/* each actor thinks every other tick (half of them each tick) */
	for (i = 0; i < campaign.actor_count; i++)
	{
		if ((game_time_get() + i) % 2 == 0)
			think(&campaign.actors[i]);
	}
}

/* the HUD's and the pause menu's line (n64_render_game.c, n64_game.c) */
int n64_campaign_status(
	char *text,
	int size)
{
	static char const *const difficulties[4] = { "EASY", "NORMAL", "HEROIC", "LEGENDARY" };

	if (!campaign.active)
		return 0;
	snprintf(text, size, "HALO   %s   COVENANT DOWN %ld", difficulties[campaign.difficulty & 3], campaign.covenant_down);
	return 1;
}

#endif /* HALO_N64_CAMPAIGN */

/* the FPS counter's line of the game's own (n64_render.c draw_counter): a
campaign's objects, the share of Halo's object memory in use, its actors
and the scenery near the players. 0: none (the multiplayer cartridge) */
int n64_game_debug_line(
	char *text,
	int size)
{
#ifdef HALO_N64_CAMPAIGN
	struct objects_information information;

	if (!campaign.active || !game_in_progress())
		return 0;
	objects_information_get(&information);
	snprintf(text, size, "OBJECTS %d MEMORY %d%% ACTORS %d SCENERY %d", information.object_count,
		(int)(information.used_memory * 100.f), campaign.actor_count, scenery.made);
	return 1;
#else
	(void)text;
	(void)size;
	return 0;
#endif
}
