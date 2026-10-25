/*
N64_ONLINE_GAME.C

Halo 64 Online's games, the game's half (port/n64/docs/online; the
platform's half, the Link's frames, is port/n64/src/n64_online.c). The
online cartridge plays a game of its own on the Link's map -- slayer, or
team slayer, with the cartridge's player alone in it, no bots, no vehicles
and none of the map's weapons -- so that the player's own unit moves, aims
and fires at once, with its first-person weapon, sounds, effects and HUD as
in any game of the cartridge. Halo 64 Link plays the same control in the
real game, a tick at a time (n64_online_game_tick_begin sends each tick's),
and its state of the unit corrects the local one (n64_online_game_tick_end):

- position: the Link's position for the tick it last played is compared
  with where this unit was after that same tick (the ticks' history); a
  small difference is taken off a part at a time, a large one (a teleporter,
  a spawn, a push) at once, the ticks played since kept;
- health and shields are the Link's (the local unit takes no damage);
- weapons, rounds and grenades become the Link's when they have differed
  for a few of its frames (a weapon picked up there, rounds the local
  firing has not yet been told of);
- death: the unit dies when the Link's does, and is spawned again where
  the Link's is when the Link's is (game_engine_should_spawn_player asks);
- in a vehicle the view is the Link's (n64_render_game.c) and the unit
  stays where the Link's is, its control kept from it (players.c).

Everything else in the game -- the other players, vehicles, weapons on the
ground, projectiles -- is drawn as the Link sends it (n64_online.c).
*/

#ifdef HALO_N64_ONLINE

#include "cseries.h"

#include "game/game.h"
#include "game/players.h"
#include "game/player_control.h"
#include "game/game_engine.h"
#include "main/main.h"
#include "objects/objects.h"
#include "units/units.h"
#include "items/weapons.h"
#include "cache/cache_files.h"
#include "tag_files/tag_groups.h"
#include "interface/hud_messaging.h"
#include "effects/effects.h"
#include "effects/effect_definitions.h"
#include "effects/player_effects.h"
#include "objects/damage.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"

#include "../src/n64.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ticks of predicted positions kept (a few seconds: more than the Link is
ever behind) */
#define HISTORY_TICKS 128
/* a difference past this is taken at once (world units) */
#define SNAP_DISTANCE 3.f
/* under this, none (the Link's numbers are floats sent whole, but the two
games' collisions differ a little) */
#define SMALL_DISTANCE 0.02f
/* the part of a difference taken off each frame of the Link's */
#define BLEND 0.35f
/* up and down: only differences past this (a biped on the ground is held
there by its own physics) */
#define VERTICAL_DISTANCE 0.25f
/* frames of the Link's that must differ before its weapons, rounds or
grenades are taken */
#define INVENTORY_FRAMES 2
#define ROUNDS_FRAMES 6

static struct
{
	/* the frame's action (update_client_queue), sent with each tick's */
	struct player_action action;
	boolean action_valid;
	/* the tick being played (its control sent), and the history */
	long tick;
	struct
	{
		long tick;
		real_point3d position;
	} history[HISTORY_TICKS];
	unsigned long serial;
	boolean snap;
	int inventory_frames, rounds_frames[4], grenade_frames;
	/* (the log's) */
	unsigned long corrections, snaps, reported;
	real largest;
} online_game;

/* ---------- helpers */

static long local_unit(
	void)
{
	long player_index = local_player_get_player_index(0);

	return player_index != NONE ? player_get(player_index)->unit_index : NONE;
}

static boolean is_local_player(
	long player_index)
{
	return player_index != NONE && player_get(player_index)->local_player_index == 0;
}

/* a weapon definition's tag index from its absolute index (the Link names
tags by it: the cartridge's maps keep the Xbox maps' tag indices) */
/* cache_files.c's (HALO_N64_ONLINE): a tag by its absolute index, of a
group, or NONE */
long n64_tag_from_absolute(long absolute_index, long group_tag);
/* player_effects.c's and game_engine.c's (HALO_N64_ONLINE) */
extern real_point3d const *n64_online_damage_attacker;
void n64_online_set_goals(long count, real_point3d const *positions, short const *nav_points, long goal_radar);

static long weapon_definition(
	int absolute_index)
{
	return n64_tag_from_absolute(absolute_index, 'weap');
}

static int slot_definition(
	struct unit_datum *unit,
	short slot)
{
	long weapon_index = unit->unit.weapon_object_indices[slot];

	if (weapon_index == NONE || !object_try_and_get(weapon_index))
		return -1;
	return (int)DATUM_INDEX_TO_ABSOLUTE_INDEX(object_get(weapon_index)->definition_index);
}

static void history_clear(
	void)
{
	short i;

	for (i = 0; i < HISTORY_TICKS; i++)
		online_game.history[i].tick = NONE;
}

static void history_shift(
	real_vector3d const *delta)
{
	short i;

	for (i = 0; i < HISTORY_TICKS; i++)
	{
		online_game.history[i].position.x += delta->i;
		online_game.history[i].position.y += delta->j;
		online_game.history[i].position.z += delta->k;
	}
}

static void move_unit(
	long unit_index,
	real_point3d const *position)
{
	object_set_position(unit_index, position, NULL, NULL);
}

/* ---------- the Link's inventory */

static void take_rounds(
	long unit_index,
	struct n64_online_self const *self,
	short slot)
{
	struct unit_datum *unit = unit_get(unit_index);
	long weapon_index = unit->unit.weapon_object_indices[slot];
	struct weapon_datum *weapon = weapon_index != NONE ? weapon_try_and_get(weapon_index) : NULL;

	if (!weapon)
		return;
	weapon->weapon.magazines[0].rounds_loaded = (short)self->weapons[slot].loaded;
	weapon->weapon.magazines[0].rounds_total = (short)self->weapons[slot].total;
	weapon->weapon.age = self->weapons[slot].age;
}

/* the unit's weapons made the Link's, the one it holds too */
static void take_weapons(
	long unit_index,
	struct n64_online_self const *self)
{
	struct unit_datum *unit = unit_get(unit_index);
	short slot;
	char line[96];

	unit_delete_all_weapons(unit_index);
	for (slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT && slot < 4; slot++)
	{
		long definition_index = weapon_definition(self->weapons[slot].definition);
		struct object_placement_data placement_data;
		long weapon_index;

		if (definition_index == NONE)
			continue;
		object_placement_data_new(&placement_data, definition_index, NONE);
		placement_data.position = unit->object.position;
		weapon_index = object_new(&placement_data);
		if (weapon_index != NONE && !unit_add_weapon_to_inventory(unit_index, weapon_index, _unit_add_weapon_normal))
			object_delete(weapon_index);
	}
	for (slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT && slot < 4; slot++)
		take_rounds(unit_index, self, slot);
	if (self->current_weapon >= 0 && self->current_weapon < MAXIMUM_WEAPONS_PER_UNIT &&
		unit->unit.weapon_object_indices[self->current_weapon] != NONE)
	{
		unit->unit.desired_weapon_index = (short)self->current_weapon;
		player_control_get(0)->desired_weapon_index = (short)self->current_weapon;
	}
	snprintf(line, sizeof(line), "n64: online: weapons from the Link: %d %d %d %d, holding %d",
		self->weapons[0].definition, self->weapons[1].definition, self->weapons[2].definition,
		self->weapons[3].definition, self->current_weapon);
	n64_log(line);
}

static void check_inventory(
	long unit_index,
	struct n64_online_self const *self,
	boolean at_once)
{
	struct unit_datum *unit = unit_get(unit_index);
	boolean different = FALSE;
	short slot;

	for (slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT && slot < 4; slot++)
	{
		if (slot_definition(unit, slot) != self->weapons[slot].definition)
			different = TRUE;
	}
	online_game.inventory_frames = different ? online_game.inventory_frames + 1 : 0;
	if (different && (at_once || online_game.inventory_frames >= INVENTORY_FRAMES))
	{
		take_weapons(unit_index, self);
		online_game.inventory_frames = 0;
		return;
	}
	for (slot = 0; slot < MAXIMUM_WEAPONS_PER_UNIT && slot < 4; slot++)
	{
		long weapon_index = unit->unit.weapon_object_indices[slot];
		struct weapon_datum *weapon = weapon_index != NONE ? weapon_try_and_get(weapon_index) : NULL;
		boolean rounds_differ = weapon &&
			(weapon->weapon.magazines[0].rounds_loaded != self->weapons[slot].loaded ||
			weapon->weapon.magazines[0].rounds_total != self->weapons[slot].total ||
			fabsf(weapon->weapon.age - self->weapons[slot].age) > 0.03f);

		online_game.rounds_frames[slot] = rounds_differ ? online_game.rounds_frames[slot] + 1 : 0;
		if (rounds_differ && (at_once || online_game.rounds_frames[slot] >= ROUNDS_FRAMES))
		{
			take_rounds(unit_index, self, slot);
			online_game.rounds_frames[slot] = 0;
		}
	}
	if (unit->unit.grenade_counts[0] != self->frags || unit->unit.grenade_counts[1] != self->plasmas)
	{
		if (at_once || ++online_game.grenade_frames >= ROUNDS_FRAMES)
		{
			unit->unit.grenade_counts[0] = (char)self->frags;
			unit->unit.grenade_counts[1] = (char)self->plasmas;
			online_game.grenade_frames = 0;
		}
	}
	else
		online_game.grenade_frames = 0;
}

/* ---------- the unit where the Link's is */

static void correct_position(
	long unit_index,
	struct n64_online_self const *self)
{
	struct unit_datum *unit = unit_get(unit_index);
	real_point3d now = unit->object.position, predicted = now, target;
	real_vector3d error, delta;
	real horizontal;
	short i;

	for (i = 0; i < HISTORY_TICKS; i++)
	{
		if (online_game.history[i].tick != NONE &&
			(unsigned long)online_game.history[i].tick == self->applied_tick)
		{
			predicted = online_game.history[i].position;
			break;
		}
	}
	error.i = self->position[0] - predicted.x;
	error.j = self->position[1] - predicted.y;
	error.k = self->position[2] - predicted.z;
	horizontal = sqrtf(error.i * error.i + error.j * error.j);
	if (online_game.snap || horizontal > SNAP_DISTANCE || fabsf(error.k) > SNAP_DISTANCE)
	{
		/* (at once: the Link's position and what was played since its tick) */
		target.x = self->position[0] + (now.x - predicted.x);
		target.y = self->position[1] + (now.y - predicted.y);
		target.z = self->position[2] + (now.z - predicted.z);
		if (i == HISTORY_TICKS)
		{
			target.x = self->position[0];
			target.y = self->position[1];
			target.z = self->position[2];
		}
		delta.i = target.x - now.x;
		delta.j = target.y - now.y;
		delta.k = target.z - now.z;
		move_unit(unit_index, &target);
		unit->object.translational_velocity.i = self->velocity[0];
		unit->object.translational_velocity.j = self->velocity[1];
		unit->object.translational_velocity.k = self->velocity[2];
		history_shift(&delta);
		online_game.snap = FALSE;
		online_game.snaps++;
		return;
	}
	if (i == HISTORY_TICKS)
		return;
	delta.i = horizontal > SMALL_DISTANCE ? error.i * BLEND : 0.f;
	delta.j = horizontal > SMALL_DISTANCE ? error.j * BLEND : 0.f;
	delta.k = fabsf(error.k) > VERTICAL_DISTANCE ? error.k * BLEND : 0.f;
	if (delta.i == 0.f && delta.j == 0.f && delta.k == 0.f)
		return;
	target.x = now.x + delta.i;
	target.y = now.y + delta.j;
	target.z = now.z + delta.k;
	move_unit(unit_index, &target);
	history_shift(&delta);
	online_game.corrections++;
	if (horizontal > online_game.largest)
		online_game.largest = horizontal;
}

/* ---------- the game's hooks */

/* hud_print_message's (hud_messaging.c): in an online game only the Link's
game's messages are shown (the cartridge's own game has nothing to say:
it is the player's alone), while this is set */
boolean n64_online_game_printing;

/* ---------- what happens in the Link's game, made again here: its
effects (with their sounds), its other sounds and the announcer, the hits
the player takes; and the HUD's view of it (the goals, the motion
sensor's blips, the crosshair's target, the state message) */

/* effects made a frame at most (the rest wait; the queue drops the oldest) */
#define EFFECTS_PER_FRAME 12

static void make_events(
	void)
{
	struct n64_online_effect effect;
	struct n64_online_sound sound;
	struct n64_online_damage damage;
	int made = 0;

	if (!game_in_progress())
		return;
	while (made < EFFECTS_PER_FRAME && n64_online_take_effect(&effect))
	{
		long definition_index = n64_tag_from_absolute(effect.definition, EFFECT_DEFINITION_TAG);
		real_point3d point;
		real_vector3d forward;

		if (definition_index == NONE)
			continue;
		point.x = effect.point[0];
		point.y = effect.point[1];
		point.z = effect.point[2];
		forward.i = effect.forward[0];
		forward.j = effect.forward[1];
		forward.k = effect.forward[2];
		if (normalize3d(&forward) == 0.f)
			forward = *global_forward3d;
		effect_new_unattached_from_markers(definition_index, NONE, NULL, 1, NULL, &point, &forward,
			PIN(effect.scale_a, 0.f, 1.f), PIN(effect.scale_b, 0.f, 1.f), NULL, NULL, FALSE);
		made++;
	}
	while (n64_online_take_sound(&sound))
	{
		if (sound.announcer)
		{
			n64_online_game_printing = TRUE;
			game_engine_play_multiplayer_sound(sound.definition);
			n64_online_game_printing = FALSE;
		}
		else
		{
			long definition_index = n64_tag_from_absolute(sound.definition, SOUND_DEFINITION_TAG);
			struct sound_location location;

			if (definition_index == NONE)
				continue;
			csmemset(&location, 0, sizeof(location));
			location.position.x = sound.point[0];
			location.position.y = sound.point[1];
			location.position.z = sound.point[2];
			location.forward = *global_forward3d;
			location.game_location.cluster_index = NONE;
			unattached_impulse_sound_new(definition_index, &location, PIN(sound.scale, 0.f, 1.f));
		}
	}
	while (n64_online_take_damage(&damage))
	{
		long definition_index = n64_tag_from_absolute(damage.definition, 'jpt!');
		long player_index = damage.local_player_index >= 0 && damage.local_player_index < MAXIMUM_LOCAL_PLAYERS ?
			local_player_get_player_index((short)damage.local_player_index) : NONE;
		struct damage_data data;
		real_vector3d direction;
		real_point3d attacker;

		if (definition_index == NONE || player_index == NONE)
			continue;
		csmemset(&data, 0, sizeof(data));
		data.definition_index = definition_index;
		data.owner_player_index = NONE;
		data.owner_object_index = NONE;
		data.owner_team_index = NONE;
		direction.i = damage.direction[0];
		direction.j = damage.direction[1];
		direction.k = damage.direction[2];
		attacker.x = damage.attacker[0];
		attacker.y = damage.attacker[1];
		attacker.z = damage.attacker[2];
		n64_online_damage_attacker = damage.has_attacker ? &attacker : NULL;
		player_effect_start(player_index, &data, &direction, PIN(damage.scale, 0.f, 1.f), damage.total);
		n64_online_damage_attacker = NULL;
	}
}

/* the Link's game's goals, as this game's, each frame of the Link's */
static void take_goals(
	void)
{
	static unsigned long serial;
	struct n64_online_hud const *hud = n64_online_hud(0);
	real_point3d positions[N64_ONLINE_GOALS];
	short nav_points[N64_ONLINE_GOALS];
	int i;

	if (!hud || hud->serial == serial || !game_in_progress())
		return;
	serial = hud->serial;
	for (i = 0; i < hud->goal_count; i++)
	{
		positions[i].x = hud->goals[i].position[0];
		positions[i].y = hud->goals[i].position[1];
		positions[i].z = hud->goals[i].position[2];
		nav_points[i] = hud->goals[i].nav_point;
	}
	n64_online_set_goals(hud->goal_count, positions, nav_points, hud->goal_radar);
}

/* motion_sensor.c's (HALO_N64_ONLINE): the Link's game's units its motion
sensor shows; -1 not an online game */
int n64_online_game_blips(
	short local_player_index,
	real_point3d *positions,
	char *types,
	char *sizes,
	int maximum)
{
	struct n64_online_hud const *hud;
	int i;

	if (!n64_online_active())
		return -1;
	hud = n64_online_hud(local_player_index);
	if (!hud)
		return 0;
	for (i = 0; i < hud->blip_count && i < maximum; i++)
	{
		positions[i].x = hud->blips[i].position[0];
		positions[i].y = hud->blips[i].position[1];
		positions[i].z = hud->blips[i].position[2];
		types[i] = (char)hud->blips[i].type;
		sizes[i] = (char)hud->blips[i].size;
	}
	return i;
}

/* player_control.c's (HALO_N64_ONLINE): the crosshair's autoaim level, the
Link's player's (red at 1); FALSE not an online game */
boolean n64_online_game_autoaim(
	short local_player_index,
	real *autoaim)
{
	struct n64_online_hud const *hud;

	if (!n64_online_active())
		return FALSE;
	hud = n64_online_hud(local_player_index);
	*autoaim = hud && hud->autoaim > 0.99f ? 1.f : hud ? hud->autoaim : 0.f;
	return TRUE;
}

/* player_control.c's (HALO_N64_ONLINE): what draws the aim -- the other
players are the Link's, so are the targets: its player's magnetism level
and the target's motion across the view (radians a tick), played into this
unit's look with the cartridge's own aim assist (GitHub issue 12); FALSE
not an online game */
boolean n64_online_game_magnetism(
	short local_player_index,
	real *magnetism,
	real_euler_angles2d *target_angular_velocity)
{
	struct n64_online_hud const *hud;

	if (!n64_online_active())
		return FALSE;
	hud = n64_online_hud(local_player_index);
	*magnetism = hud ? hud->magnetism : 0.f;
	target_angular_velocity->yaw = hud ? hud->target_velocity[0] : 0.f;
	target_angular_velocity->pitch = hud ? hud->target_velocity[1] : 0.f;
	return TRUE;
}

static boolean online_local_player(
	long player_index,
	short *local_player_index)
{
	if (!n64_online_active() || player_index == NONE)
		return FALSE;
	*local_player_index = player_get(player_index)->local_player_index;
	return *local_player_index == 0;
}

/* game_engine.c's (HALO_N64_ONLINE): the name the crosshair shows, the
Link's player's target's ("" none); FALSE not an online game's player */
boolean n64_online_game_target_name(
	long player_index,
	wchar_t *name,
	long size)
{
	struct n64_online_hud const *hud;
	short local_player_index;
	long i;

	if (!online_local_player(player_index, &local_player_index))
		return FALSE;
	hud = n64_online_hud(local_player_index);
	for (i = 0; i < size - 1 && hud && hud->target[i]; i++)
		name[i] = (wchar_t)(unsigned char)hud->target[i];
	name[i] = 0;
	return TRUE;
}

/* game_engine.c's (HALO_N64_ONLINE): the state message ("respawn in 3",
"you have the flag"), the Link's game's; FALSE not an online game's player */
boolean n64_online_game_state_message(
	long player_index,
	wchar_t *message,
	long size,
	boolean *has_message)
{
	struct n64_online_hud const *hud;
	short local_player_index;
	long i;

	if (!online_local_player(player_index, &local_player_index))
		return FALSE;
	hud = n64_online_hud(local_player_index);
	for (i = 0; i < size - 1 && hud && hud->state[i]; i++)
		message[i] = (wchar_t)hud->state[i];
	if (size > 0)
		message[i] = 0;
	*has_message = i > 0;
	return TRUE;
}

/* render_frame's (n64_render_game.c), each frame: the cable, the leaving,
the Link's game's messages */
void n64_online_game_frame(
	void)
{
	int local_player_index;
	unsigned short text[64];

	if (!n64_online_active())
		return;
	if (!n64_online_update())
	{
		n64_online_end();
		main_goto_main_menu();
		return;
	}
	while (n64_online_take_message(&local_player_index, text, NUMBEROF(text)))
	{
		if (local_player_index < 0 || local_player_index >= MAXIMUM_LOCAL_PLAYERS ||
			local_player_get_player_index((short)local_player_index) == NONE)
		{
			continue;
		}
		n64_online_game_printing = TRUE;
		hud_print_message((short)local_player_index, (wchar_t const *)text);
		n64_online_game_printing = FALSE;
	}
	make_events();
	take_goals();
	if (n64_microseconds() / 1000 - online_game.reported >= 5000)
	{
		char line[128];

		online_game.reported = (unsigned long)(n64_microseconds() / 1000);
		snprintf(line, sizeof(line), "n64: online: %lu corrections (largest %.2f), %lu snaps in 5 s",
			online_game.corrections, (double)online_game.largest, online_game.snaps);
		n64_log(line);
		online_game.corrections = online_game.snaps = 0;
		online_game.largest = 0.f;
	}
}

/* update_client_queue's (player_queues_new.c), each frame: a local player's
action as the input made it */
void n64_online_game_queue_action(
	short local_player_index,
	struct player_action const *action)
{
	if (local_player_index != 0 || !n64_online_active())
		return;
	online_game.action = *action;
	online_game.action_valid = TRUE;
}

/* players_update_before_game's (players.c), each tick before the players
play it: the tick's control to the Link */
void n64_online_game_tick_begin(
	void)
{
	struct n64_online_control control;

	if (!n64_online_active())
		return;
	online_game.tick = game_time_get();
	if (!online_game.action_valid)
		return;
	control.tick = (unsigned long)online_game.tick;
	control.local_player = 0;
	control.flags = online_game.action.control_flags;
	control.yaw = online_game.action.desired_facing.yaw;
	control.pitch = online_game.action.desired_facing.pitch;
	control.throttle[0] = online_game.action.throttle.i;
	control.throttle[1] = online_game.action.throttle.j;
	control.trigger = online_game.action.primary_trigger;
	control.weapon = online_game.action.desired_weapon_index;
	control.grenade = online_game.action.desired_grenade_index;
	control.zoom = online_game.action.desired_zoom_level;
	n64_online_send_control(&control);
}

/* n64_game_tick's, after each tick: where the unit is kept, then the Link's
state taken */
void n64_online_game_tick_end(
	void)
{
	struct n64_online_self const *self;
	long player_index, unit_index;
	struct player_datum *player;
	struct unit_datum *unit;
	boolean new_frame;

	if (!n64_online_active())
		return;
	player_index = local_player_get_player_index(0);
	if (player_index == NONE)
		return;
	player = player_get(player_index);
	unit_index = player->unit_index;
	unit = unit_index != NONE ? unit_try_and_get(unit_index) : NULL;
	if (unit && !TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
	{
		short slot = (short)(online_game.tick & (HISTORY_TICKS - 1));

		online_game.history[slot].tick = online_game.tick;
		online_game.history[slot].position = unit->object.position;
	}
	self = n64_online_self(0);
	if (!self)
		return;
	new_frame = self->serial != online_game.serial;
	online_game.serial = self->serial;
	if (!self->alive)
	{
		/* (the Link's died: this one too, its respawn counted down as the
		Link's is) */
		if (unit && !TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
		{
			n64_log("n64: online: the Link's unit died");
			/* (the damage that kills it is damage: let it be taken) */
			SET_FLAG(unit->object.damage_flags, _object_cannot_take_damage_bit, FALSE);
			unit->object.shield_vitality = 0.f;
			unit_kill(unit_index);
		}
		player->respawn_timer = self->respawn_ticks > 0 ? self->respawn_ticks : 1;
		return;
	}
	player->respawn_timer = 0;
	if (!unit || TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
		return;
	SET_FLAG(unit->object.damage_flags, _object_cannot_take_damage_bit, TRUE);
	unit->object.body_vitality = self->health;
	unit->object.shield_vitality = self->shields;
	if (self->in_vehicle)
	{
		/* (the Link's view; the unit kept where the Link's sits) */
		real_point3d position;

		position.x = self->position[0];
		position.y = self->position[1];
		position.z = self->position[2];
		move_unit(unit_index, &position);
		unit->object.translational_velocity = *global_zero_vector3d;
		history_clear();
		online_game.snap = TRUE;
		return;
	}
	if (!new_frame)
		return;
	correct_position(unit_index, self);
	check_inventory(unit_index, self, FALSE);
}

/* game_engine_should_spawn_player's (game_engine.c): TRUE if an online game
decides, whether to in should_spawn -- when the Link's unit lives */
boolean n64_online_game_should_spawn(
	long player_index,
	boolean *should_spawn)
{
	struct n64_online_self const *self;

	if (!n64_online_active())
		return FALSE;
	self = is_local_player(player_index) ? n64_online_self(0) : NULL;
	*should_spawn = self && self->alive;
	return TRUE;
}

/* players_update_before_game's, after a local player's unit is spawned: it
is put where the Link's is, facing as it does, with its weapons */
void n64_online_game_spawned(
	long player_index)
{
	struct n64_online_self const *self;
	long unit_index;
	struct unit_datum *unit;

	if (!n64_online_active() || !is_local_player(player_index))
		return;
	unit_index = player_get(player_index)->unit_index;
	unit = unit_index != NONE ? unit_try_and_get(unit_index) : NULL;
	if (!unit)
		return;
	SET_FLAG(unit->object.damage_flags, _object_cannot_take_damage_bit, TRUE);
	history_clear();
	online_game.inventory_frames = online_game.grenade_frames = 0;
	csmemset(online_game.rounds_frames, 0, sizeof(online_game.rounds_frames));
	self = n64_online_self(0);
	if (!self)
		return;
	{
		real_point3d position;
		real_vector3d forward;
		struct player_control *control = player_control_get(0);

		position.x = self->position[0];
		position.y = self->position[1];
		position.z = self->position[2];
		forward.i = cosf(self->yaw);
		forward.j = sinf(self->yaw);
		forward.k = 0.f;
		object_set_position(unit_index, &position, &forward, NULL);
		unit->object.translational_velocity = *global_zero_vector3d;
		control->desired_angles.yaw = self->yaw;
		control->desired_angles.pitch = self->pitch;
	}
	check_inventory(unit_index, self, TRUE);
	online_game.snap = FALSE;
	n64_log("n64: online: spawned where the Link's unit is");
}

/* whether a player's unit plays no control of its own now (in a vehicle:
the Link's drives it) */
boolean n64_online_game_inhibited(
	long player_index)
{
	struct n64_online_self const *self;

	if (!n64_online_active() || !is_local_player(player_index))
		return FALSE;
	self = n64_online_self(0);
	return self && self->in_vehicle;
}

/* whether the Link's unit has something to pick up or get into: the action
button is not a reload then */
boolean n64_online_game_action_target(
	long player_index)
{
	struct n64_online_self const *self;

	if (!n64_online_active() || !is_local_player(player_index))
		return FALSE;
	self = n64_online_self(0);
	return self && self->action_target;
}

/* a new online game: nothing kept from the last */
void n64_online_game_reset(
	void)
{
	csmemset(&online_game, 0, sizeof(online_game));
	history_clear();
}

#endif /* HALO_N64_ONLINE */
