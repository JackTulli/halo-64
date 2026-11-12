/*
N64_PROJECTILES_GAME.C

The projectiles' module's game side (rom:/projectiles.dso, with
port/n64/src/n64_projectiles.c; tools/n64_build.py): each tick's move of a
projectile handed to the segments with its look. The program's hook
(source/items/projectiles.c) calls it only while the module is in.

Halo draws a bullet or a bolt with its contrail (the tracer) and its light
volume widget (a plasma bolt's, a needle's, a rocket's glow), and the N64
makes neither. So a move gets a trail where the definition has a contrail and
the round is a tracer (weapons.c clears the bit on the rounds between
tracers, as Halo deletes their contrails), a glow where it has a light
volume, in the volume's first frame's far colour, radius, alpha and length. A
projectile with neither (the pistol's bullet, the shotgun's pellets) shows
nothing, as in Halo. The contrail tags are not on the cartridge
(tools/n64/convert.py drops them): a trail is the assault rifle tracer's
yellow. The looks are a table of N64_PROJECTILE_LOOKS by the definition's
index, found again where two definitions share a place and after each load
of the module. Visual only.
*/

#include "n64_hud64_imports.h"

#include "cseries.h"

#include "effects/contrail_definitions.h"
#include "game/players.h"
#include "items/projectile_definitions.h"
#include "objects/objects.h"
#include "objects/widgets/light_volumes.h"

#include "../src/n64.h"
#include "../src/n64_projectiles_module.h"

#define N64_LOOK_STREAK 1
#define N64_LOOK_GLOW 2

/* the looks, by definition (0: none found yet) */
static struct n64_projectile_look looks[N64_PROJECTILE_LOOKS];
typedef char projectile_look_size_check[sizeof(struct n64_projectile_look) == 8 ? 1 : -1];

/* x (0 to 1) as 0 to scale, at most limit */
static unsigned long look_part(
	real x,
	real scale,
	real limit)
{
	return (unsigned long)(int)PIN(x * scale, 0.f, limit);
}

static void look_find(
	struct n64_projectile_look *look,
	long definition_index)
{
	struct projectile_definition const *definition = projectile_definition_get(definition_index);
	struct light_volume_frame const *frame = n64_projectile_volume(definition_index);
	short i;

	look->parts = 0;
	look->radius = 0;
	look->color = 0xFFE348;
	for (i = 0; i < definition->object.attachments.count; i++)
	{
		if (TAG_BLOCK_GET_ELEMENT(&definition->object.attachments, i,
			struct object_attachment_definition)->type.group_tag == CONTRAIL_DEFINITION_TAG)
			look->parts |= N64_LOOK_STREAK;
	}
	if (frame)
	{
		look->color = n64_projectile_rgb(frame->color_yon.red, frame->color_yon.green, frame->color_yon.blue) |
			look_part(frame->length, 32.f, 63.f) << 26;
		look->radius = (unsigned char)(look_part(frame->radius_yon, 256.f, 254.f) + 1);
		/* (the far alpha in 1/20, at most 14; 15: solid, 1) */
		look->parts |= N64_LOOK_GLOW | (frame->color_yon.alpha >= 0.999f ? 15UL :
			look_part(frame->color_yon.alpha * 20.f + 0.5f, 1.f, 14.f)) << 4;
	}
}

/* the looks new (the module's begin) */
void n64_projectiles_looks_clear(
	void)
{
	csmemset(looks, 0, sizeof(looks));
}

/* a projectile's move this tick, from from to to: its look's segment, if
any; tracer: the round is a tracer (projectiles.c). The module's entry
(n64_projectiles_entries.moved) */
void n64_projectiles_moved(
	long projectile_index,
	long definition_index,
	long owner_player_index,
	int tracer,
	const float *from,
	const float *to)
{
	unsigned short definition = (unsigned short)(DATUM_INDEX_TO_ABSOLUTE_INDEX(definition_index) + 1);
	struct n64_projectile_look *look = &looks[definition % N64_PROJECTILE_LOOKS];
	unsigned long parts;

	/* (two places a definition: its own, else the next -- two definitions
	that share a place are both kept, not found again each move) */
	if (look->definition != definition)
	{
		struct n64_projectile_look *next = &looks[(definition + 1) % N64_PROJECTILE_LOOKS];

		if (next->definition == definition)
			look = next;
		else
		{
			if (look->definition)
				look = next;
			look_find(look, definition_index);
			look->definition = definition;
		}
	}
	/* (LIGHT: a bolt's glow alone; FULL: its trail too, on its tracer
	rounds) */
	parts = look->parts & N64_LOOK_GLOW;
	if ((look->parts & N64_LOOK_STREAK) && tracer && (n64_projectiles_map_mode == 2 || !parts))
		parts |= N64_LOOK_STREAK;
	if (parts)
	{
		/* (a solid glow's owner, its local player + 1: in that player's own
		view it is drawn as a see-through one near the view) */
		int owner = 0;

		if ((look->parts >> 4) == 15 && (parts & N64_LOOK_GLOW) && owner_player_index != NONE)
		{
			struct player_datum *player = player_try_and_get(owner_player_index);

			if (player && player->local_player_index >= 0 && player->local_player_index < 4)
				owner = player->local_player_index + 1;
		}
		n64_projectiles_segment_record(from, to,
			look->color | parts << 24, look->radius, look->parts >> 4,
			(int)DATUM_INDEX_TO_ABSOLUTE_INDEX(projectile_index) + 1 + (owner << 13));
	}
}
