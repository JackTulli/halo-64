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
#include "camera/director.h"
#include "game/players.h"
#include "items/projectile_definitions.h"
#include "objects/objects.h"
#include "objects/widgets/light_volumes.h"
#include "objects/light_definitions.h"
#include "models/model_definitions.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"

#include "../src/n64.h"
#include "../src/n64_projectiles_module.h"

#define N64_LOOK_STREAK 1
#define N64_LOOK_GLOW 2
/* (the look's colour: dimmer seen end-on; its model only a glow, drawn by
the glow, not by the model) */
#define N64_LOOK_DIM_END_ON (1UL << 29)
#define N64_LOOK_GLOW_MODEL (1UL << 30)

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
		unsigned long group = TAG_BLOCK_GET_ELEMENT(&definition->object.attachments, i,
			struct object_attachment_definition)->type.group_tag;

		if (group == CONTRAIL_DEFINITION_TAG)
			look->parts |= N64_LOOK_STREAK;
		/* (a light: Halo's plasma bolts, charged shot and fuel rod light the
		world, and their glows are bright, white in the middle; needles and
		rockets have none, and their glows are faint) */
		if (group == LIGHT_DEFINITION_TAG)
			look->color |= 1UL << 31;
	}
	if (frame)
	{
		look->color = (look->color & 1UL << 31) |
			n64_projectile_rgb(frame->color_yon.red, frame->color_yon.green, frame->color_yon.blue) |
			look_part(frame->length, 8.f, 7.f) << 26;
		/* (the light volume's parallel brightness: under 1 it is dimmer seen
		end-on -- a needle's 0.2; a rocket's and the bolts' are 1) */
		for (i = 0; i < definition->object.widgets.count; i++)
		{
			struct n64_widget { struct tag_reference reference; long pad[4]; };
			struct tag_reference const *widget = &TAG_BLOCK_GET_ELEMENT(&definition->object.widgets, i,
				struct n64_widget)->reference;

			if (widget->group_tag == 'mgs2' && widget->index != NONE)
			{
				if (light_volume_definition_get(widget->index)->parallel_brightness_scale < 0.999f)
					look->color |= N64_LOOK_DIM_END_ON;
				break;
			}
		}
		look->radius = (unsigned char)(look_part(frame->radius_yon, 256.f, 254.f) + 1);
		/* (the far alpha in 1/20, at most 14; 15: solid, 1) */
		look->parts |= N64_LOOK_GLOW | (frame->color_yon.alpha >= 0.999f ? 15UL :
			look_part(frame->color_yon.alpha * 20.f + 0.5f, 1.f, 14.f)) << 4;
		/* (a model whose every shader is transparent -- a needle's, one
		dim colour added to the picture in Halo -- is only a glow: the N64
		draws it solid and dark, so the light volume's glow stands for it) */
		if (definition->object.model.index != NONE)
		{
			struct model const *model = model_definition_get(definition->object.model.index);

			look->color |= model->shaders.count > 0 ? N64_LOOK_GLOW_MODEL : 0;
			for (i = 0; i < model->shaders.count; i++)
			{
				/* (a model's shader: its reference, then its permutation and
				padding, as models.c's model_shader_reference) */
				struct n64_model_shader { struct tag_reference shader; short permutation; word pad; long unused[3]; };
				long shader = TAG_BLOCK_GET_ELEMENT(&model->shaders, i, struct n64_model_shader)->shader.index;

				if (shader == NONE || !shader_type_is_transparent(shader_definition_get(shader)->base.type))
					look->color &= ~N64_LOOK_GLOW_MODEL;
			}
		}
	}
}

/* the looks new (the module's begin) */
void n64_projectiles_looks_clear(
	void)
{
	csmemset(looks, 0, sizeof(looks));
}

/* a definition's look: two places a definition, its own and the next --
two definitions that share a place are both kept, not found again each move */
static struct n64_projectile_look *look_get(
	long definition_index)
{
	unsigned short definition = (unsigned short)(DATUM_INDEX_TO_ABSOLUTE_INDEX(definition_index) + 1);
	struct n64_projectile_look *look = &looks[definition % N64_PROJECTILE_LOOKS];

	if (look->definition != definition)
	{
		struct n64_projectile_look *next = &looks[(definition + 1) % N64_PROJECTILE_LOOKS];

		if (next->definition == definition)
			return next;
		if (look->definition)
			look = next;
		look_find(look, definition_index);
		look->definition = definition;
	}
	return look;
}

/* whether the projectile is drawn by its glow, not by its model
(n64_render_game.c, while the module is in): a model that is only a glow (a
needle's), where the frame has room for its glow. A needle that did not move
in the last tick -- stuck in a player or the ground, the warning of a
supercombine, or a frame without a tick -- gets a still glow where it is
(Halo draws its light volume, moving or not); past the room, its model */
int n64_projectiles_hides_model(
	long projectile_index,
	long definition_index)
{
	struct n64_projectile_look const *look = look_get(definition_index);
	int key = (int)DATUM_INDEX_TO_ABSOLUTE_INDEX(projectile_index) + 1;

	if (!(look->color & N64_LOOK_GLOW_MODEL) || !n64_projectiles_model_glow())
		return 0;
	if (!n64_projectiles_segment_fresh(key))
	{
		/* (its place: a stuck one's is its parent's node's) */
		struct object_datum *object = object_get(projectile_index);
		real_point3d origin;
		real_point3d const *at = object->object.parent_object_index == NONE ? &object->object.position :
			object_get_origin(projectile_index, &origin);
		/* (stuck in a local player's body: its local player + 1 in the key's
		top bits, so that player's own view leaves it out where it leaves out
		the model -- the unit on foot, or seated and seen in first person, as
		the renderer's rule, Halo's object_is_first_person_camera) */
		int owner = 0, depth;
		long parent = object->object.parent_object_index;

		for (depth = 0; parent != NONE && depth < 4; depth++)
		{
			long player_index = player_index_from_unit_index(parent);
			struct player_datum *player = player_index != NONE ? player_try_and_get(player_index) : NULL;

			if (player)
			{
				if (player->unit_index == parent && player->local_player_index >= 0 && player->local_player_index < 4 &&
					(object_get(parent)->object.parent_object_index == NONE ||
					director_get_perspective(player->local_player_index) == _director_perspective_first_person))
					owner = player->local_player_index + 1;
				break;
			}
			parent = object_get(parent)->object.parent_object_index;
		}
		n64_projectiles_segment_still((const float *)at, look->color | (unsigned long)N64_LOOK_GLOW << 24, look->radius,
			look->parts >> 4, key + (owner << 13));
	}
	return 1;
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
	struct n64_projectile_look *look = look_get(definition_index);
	unsigned long parts;

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
		/* (without the model's bit: a moving needle's glow stands for its
		model only where hides_model marks it, at most 16 a frame) */
		n64_projectiles_segment_record(from, to,
			(look->color & ~N64_LOOK_GLOW_MODEL) | parts << 24, look->radius, look->parts >> 4,
			(int)DATUM_INDEX_TO_ABSOLUTE_INDEX(projectile_index) + 1 + (owner << 13));
	}
}
