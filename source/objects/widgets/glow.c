/*
GLOW.C

symbols in this file:
00122790 0010:
	_glow_dispose (0000)
001227A0 0030:
	_glow_initialize_for_new_map (0000)
001227D0 0030:
	_glow_dispose_from_old_map (0000)
00122800 00f0:
	_glow_new (0000)
001228F0 0050:
	_glow_delete (0000)
00122940 0080:
	_glow_trailing_particle_update_color (0000)
001229C0 0060:
	_glow_trailing_particle_update_size (0000)
00122A20 0090:
	_glow_trailing_particle_update_velocity (0000)
00122AB0 0040:
	_glow_trailing_particle_update_position (0000)
00122AF0 0060:
	_glow_trailing_particle_age (0000)
00122B50 01a0:
	_glow_normal_particle_update_color (0000)
00122CF0 0010:
	_glow_normal_particle_update_size (0000)
00122D00 0040:
	_glow_particle_new (0000)
00122D40 0030:
	_point_from_parametric_line (0000)
00122D70 00c0:
	_glow_render (0000)
00122E30 00c0:
	_nonuniform_cubic_spline (0000)
00122EF0 00b0:
	_nonuniform_cubic_spline_vector3d (0000)
00122FA0 0070:
	_glow_initialize (0000)
00123010 01e0:
	_glow_normal_particle_new (0000)
001231F0 06d0:
	_get_particle_world_position (0000)
001238C0 0230:
	_glow_normal_particle_update_position (0000)
00123AF0 00b0:
	_glow_particles_initialize (0000)
00123BA0 0260:
	_glow_trailing_particle_new (0000)
00123E00 0530:
	_glow_update (0000)
00124330 0070:
	_glow_submit (0000)
00289014 0012:
	??_C@_0BC@NKHKLAHF@t?$DO?$DN?5t0?5?$CG?$CG?5t?5?$DM?$DN?5t3?$AA@ (0000)
00289028 0023:
	??_C@_0CD@NAJLAIFJ@could?5not?5allocate?5glow?5data?5arr@ (0000)
0028904C 002c:
	??_C@_0CM@DGMFKJDA@could?5not?5allocate?5glow?5particle@ (0000)
00289078 000f:
	??_C@_0P@CJGCAKMC@glow?5particles?$AA@ (0000)
00289088 0005:
	??_C@_04EAMKIICI@glow?$AA@ (0000)
00289090 0026:
	??_C@_0CG@PCCALJDB@c?3?2halo?2SOURCE?2objects?2widgets?2g@ (0000)
002890B8 001c:
	??_C@_0BM@LFIJOPFP@glow?9?$DOnumber_of_markers?5?$DO?51?$AA@ (0000)
002890D4 0027:
	??_C@_0CH@HNGLDBKF@marker_index?$DMglow?9?$DOnumber_of_mar@ (0000)
002890FC 002d:
	??_C@_0CN@JJJNNAME@glow?5effect?5received?5illegal?5bou@ (0000)
0028912C 0028:
	??_C@_0CI@PJIOCDIO@unknown?5trailing?5particle?5distri@ (0000)
00289158 0047:
	??_C@_0EH@KHNBMDOO@the?5map?5limit?5for?5the?5number?5of?5@ (0000)
*/

/* ---------- headers */

#define nonuniform_cubic_spline glow_nonuniform_cubic_spline_inline
#define nonuniform_cubic_spline_vector3d glow_nonuniform_cubic_spline_vector3d_inline
#include "objects/widgets/glow.h"
#undef nonuniform_cubic_spline_vector3d
#undef nonuniform_cubic_spline

#include "bitmaps/bitmap_group.h"
#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "render/render.h"
#include "render/render_sprite.h"
#include "saved games/game_state.h"

/* ---------- constants */

enum
{
	GLOW_TAG = 'glw!',
	_bitmap_group_type_sprites = 3,
};

enum
{
	MAXIMUM_GLOW_MARKERS = 5,
#ifdef HALO_N64
	/* (no widgets are made on the N64, widgets.c: token pools, 57 KB of
	the game state given back) */
	MAXIMUM_GLOWS = 1,
	MAXIMUM_GLOW_PARTICLES = 1
#else
	MAXIMUM_GLOWS = 8,
	MAXIMUM_GLOW_PARTICLES = 512
#endif
};

enum glow_definition_flags
{
	_glow_definition_modify_particle_color_bit,
	_glow_definition_particles_move_backwards_bit,
	_glow_definition_particles_move_in_both_directions_bit,
	_glow_definition_trailing_particles_fade_over_time_bit,
	_glow_definition_trailing_particles_shrink_over_time_bit,
	_glow_definition_trailing_particles_slow_over_time_bit,
	NUMBER_OF_GLOW_DEFINITION_FLAGS
};

enum glow_particle_flags
{
	_glow_particle_moving_backwards_bit,
	_glow_particle_trailing_bit,
	NUMBER_OF_GLOW_PARTICLE_FLAGS
};

enum glow_boundary_effect
{
	_glow_boundary_effect_bounce,
	_glow_boundary_effect_wrap,
	NUMBER_OF_GLOW_BOUNDARY_EFFECTS
};

enum glow_particle_distribution
{
	_glow_particle_distribution_random,
	_glow_particle_distribution_uniform,
	NUMBER_OF_GLOW_PARTICLE_DISTRIBUTIONS
};

enum glow_trailing_particle_distribution
{
	_glow_trailing_particle_distribution_vertical,
	_glow_trailing_particle_distribution_normal,
	_glow_trailing_particle_distribution_random,
	NUMBER_OF_GLOW_TRAILING_PARTICLE_DISTRIBUTIONS
};

/* ---------- macros */

#define glow_get(index) ((struct glow_datum *)datum_get(glow_globals.glow_data, (index)))
#define glow_particle_get(index) ((struct glow_particle *)datum_get(glow_globals.glow_particle_data, (index)))
#define glow_definition_get(index) ((struct glow_definition *)tag_get(GLOW_TAG, (index)))

/* ---------- structures */

struct glow_particle
{
	struct datum_header header;
	short parent_marker_index;
	long index;
	real initial_angle;
	real_argb_color color;
	real distance_to_object;
	real initial_size;
	real present_size;
	real t;
	real_point3d position;
	real_vector3d initial_velocity;
	real_vector3d present_velocity;
	short ticks_in_existence;
	short lifetime;
	unsigned long flags;
	real fade;
	struct glow_particle *next;
	struct glow_particle *previous;
};

struct glow_datum
{
	struct datum_header header;
	boolean initialized;
	byte pad3;
	short number_of_markers;
	short pad6;
	struct object_marker markers[MAXIMUM_GLOW_MARKERS];
	long definition_index;
	short bitmap_dimension;
	short marker_order[MAXIMUM_GLOW_MARKERS];
	real total_time;
	real marker_time_index[MAXIMUM_GLOW_MARKERS];
	short number_of_particles;
	short pad24E;
	struct glow_particle *head_particle;
	struct glow_particle *tail_particle;
	short accumulated_trailing_particle_generation_ticks;
};

struct glow_definition
{
	char attachment_marker[32];
	short number_of_particles;
	short boundary_effect;
	short particle_distribution;
	short trailing_particle_distribution;
	unsigned long flags;
	long unused020[7];
	short render_mode;
	short render_orientation;
	long render_flags;
	short particle_rotational_velocity_attachment_index;
	short pad052;
	real particle_rotational_velocity;
	real particle_rotational_velocity_scale_lower_bound;
	real particle_rotational_velocity_scale_upper_bound;
	short effect_rotational_velocity_attachment_index;
	short pad062;
	real effect_rotational_velocity;
	real effect_rotational_velocity_scale_lower_bound;
	real effect_rotational_velocity_scale_upper_bound;
	short effect_translational_velocity_attachment_index;
	short pad072;
	real effect_translational_velocity;
	real effect_translational_velocity_scale_lower_bound;
	real effect_translational_velocity_scale_upper_bound;
	short distance_to_object_attachment_index;
	short pad082;
	real minimum_distance_to_object;
	real maximum_distance_to_object;
	real distance_to_object_scale_lower_bound;
	real distance_to_object_scale_upper_bound;
	long unused094[2];
	short particle_size_attachment_index;
	short pad09E;
	real particle_size_lower_bound;
	real particle_size_upper_bound;
	real particle_size_scale_lower_bound;
	real particle_size_scale_upper_bound;
	short color_attachment_index;
	short pad0B2;
	real_argb_color color_lower_bound;
	real_argb_color color_upper_bound;
	real_argb_color scale_color_lower_bound;
	real_argb_color scale_color_upper_bound;
	real color_rate_of_change;
	real percentage_edge_fade;
	real trailing_particle_generation_frequency;
	real trailing_particle_lifetime;
	real trailing_particle_velocity;
	real trailing_particle_minimum_t;
	real trailing_particle_maximum_t;
	long unused110[13];
	struct tag_reference texture;
};

typedef char glow_datum_definition_index_offset_assert[
	offsetof(struct glow_datum, definition_index) == 0x224 ? 1 : -1];
typedef char glow_datum_bitmap_dimension_offset_assert[
	offsetof(struct glow_datum, bitmap_dimension) == 0x228 ? 1 : -1];
typedef char glow_datum_number_of_particles_offset_assert[
	offsetof(struct glow_datum, number_of_particles) == 0x24C ? 1 : -1];
typedef char glow_datum_size_assert[
	sizeof(struct glow_datum) == 0x25C ? 1 : -1];
typedef char glow_particle_size_assert[
	sizeof(struct glow_particle) == 0x64 ? 1 : -1];
typedef char glow_definition_flags_offset_assert[
	offsetof(struct glow_definition, flags) == 0x28 ? 1 : -1];
typedef char glow_definition_color_lower_bound_offset_assert[
	offsetof(struct glow_definition, color_lower_bound) == 0xB4 ? 1 : -1];
typedef char glow_definition_texture_offset_assert[
	offsetof(struct glow_definition, texture) == 0x144 ? 1 : -1];
typedef char glow_definition_size_assert[
	sizeof(struct glow_definition) == 0x154 ? 1 : -1];
typedef char bitmap_group_sequences_offset_assert[
	offsetof(struct bitmap_group, sequences) == 0x54 ? 1 : -1];
/* ---------- prototypes */

static void glow_trailing_particle_update_color(
	struct glow_datum *glow,
	struct glow_particle *particle);
static void glow_trailing_particle_update_size(
	struct glow_datum *glow,
	struct glow_particle *particle);
static void glow_trailing_particle_update_velocity(
	struct glow_datum *glow,
	struct glow_particle *particle);
static void glow_trailing_particle_update_position(
	struct glow_datum *glow,
	struct glow_particle *particle,
	real elapsed_time);
static void glow_trailing_particle_age(
	struct glow_datum *glow,
	struct glow_particle *particle);
static void glow_normal_particle_update_color(
	long object_index,
	struct glow_datum *glow,
	struct glow_particle *particle);
static void glow_normal_particle_update_size(
	struct glow_particle *particle);
static struct glow_particle *glow_particle_new(
	struct glow_datum *glow);
static struct glow_particle *glow_normal_particle_new(
	struct glow_datum *glow,
	short particle_index,
	short particle_count);
static void get_particle_world_position(
	struct glow_datum *glow,
	struct glow_particle *particle,
	real rotation_per_unit_t);
static void glow_normal_particle_update_position(
	long object_index,
	struct glow_datum *glow,
	struct glow_particle *particle,
	real elapsed_time,
	real rotation_per_unit_t);
static void glow_particles_initialize(
	struct glow_datum *glow);
static struct glow_particle *glow_trailing_particle_new(
	struct glow_datum *glow);
static void glow_update(
	struct glow_datum *glow,
	long object_index);

/* ---------- globals */

/* ---------- public code */

void glow_initialize(
	void)
{
	if (!glow_globals.glow_data)
	{
		glow_globals.glow_data = game_state_data_new(
			"glow",
			MAXIMUM_GLOWS,
			sizeof(struct glow_datum));
		if (glow_globals.glow_data)
		{
			if (!glow_globals.glow_particle_data)
			{
				glow_globals.glow_particle_data = game_state_data_new(
					"glow particles",
					MAXIMUM_GLOW_PARTICLES,
					sizeof(struct glow_particle));
				if (!glow_globals.glow_particle_data)
				{
					error(_error_silent, "could not allocate glow particle data array");
					return;
				}
			}
		}
		else
		{
			error(_error_silent, "could not allocate glow data array");
		}
	}

	return;
}

void glow_initialize_for_new_map(
	void)
{
	if (glow_globals.glow_data)
		data_make_valid(glow_globals.glow_data);
	if (glow_globals.glow_particle_data)
		data_make_valid(glow_globals.glow_particle_data);

	return;
}

void glow_dispose_from_old_map(
	void)
{
	if (glow_globals.glow_data)
		data_make_invalid(glow_globals.glow_data);
	if (glow_globals.glow_particle_data)
		data_make_invalid(glow_globals.glow_particle_data);

	return;
}

void glow_dispose(
	void)
{
	return;
}

long glow_new(
	long definition_index)
{
	long glow_index = NONE;

	if (definition_index != NONE)
	{
		glow_index = datum_new(glow_globals.glow_data);
		if (glow_index != NONE)
		{
			struct glow_datum *glow = glow_get(glow_index);
			struct glow_definition *definition = glow_definition_get(definition_index);
			struct bitmap_group *sprite_group =
				bitmap_group_get(definition->texture.index);

			if (sprite_group->type == _bitmap_group_type_sprites)
			{
				struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
					&TAG_BLOCK_GET_ELEMENT(
						&sprite_group->sequences,
						0,
						struct bitmap_group_sequence)->sprites,
					0,
					struct bitmap_group_sprite);
				struct bitmap_data *bitmap =
					bitmap_group_get_bitmap_from_sequence(
						definition->texture.index,
						0,
						sprite->bitmap_index);

				glow->definition_index = definition_index;
				glow->number_of_particles = definition->number_of_particles;
				glow->bitmap_dimension = (short)(long)(
					(sprite->bounds.x1 - sprite->bounds.x0) * bitmap->width);
			}
		}
	}

	return glow_index;
}

void glow_delete(
	long glow_index)
{
	struct glow_datum *glow = glow_get(glow_index);
	struct glow_particle *particle = glow->head_particle;

	while (particle)
	{
		struct glow_particle *next = particle->next;

		datum_delete(glow_globals.glow_particle_data, particle->index);
		particle = next;
	}

	datum_delete(glow_globals.glow_data, glow_index);
	return;
}

/* ---------- private code */

void point_from_parametric_line(
	real_point3d const *point,
	real_vector3d const *forward,
	real t,
	real_point3d *result)
{
	result->x = forward->i * t + point->x;
	result->y = forward->j * t + point->y;
	result->z = forward->k * t + point->y;

	return;
}

real nonuniform_cubic_spline(
	real f0,
	real f1,
	real f2,
	real f3,
	real t0,
	real t1,
	real t2,
	real t3,
	real t)
{
	match_assert("..\\math\\real_math.h", 1530, t>= t0 && t <= t3);

	f3 = (f3 - f2) / (t3 - t2);
	f2 = (f2 - f1) / (t2 - t1);
	f1 = (f1 - f0) / (t1 - t0);
	f3 = (f3 - f2) / (t3 - t1);
	f2 = (f2 - f1) / (t2 - t0);
	f3 = (f3 - f2) / (t3 - t0);

	return f0 + (t - t0) * (f1 + (t - t1) * (f2 + (t - t2) * f3));
}

void nonuniform_cubic_spline_vector3d(
	real_vector3d *result,
	real_vector3d const *f0,
	real_vector3d const *f1,
	real_vector3d const *f2,
	real_vector3d const *f3,
	real t0,
	real t1,
	real t2,
	real t3,
	real t)
{
	result->i = nonuniform_cubic_spline(
		f0->i,
		f1->i,
		f2->i,
		f3->i,
		t0,
		t1,
		t2,
		t3,
		t);
	result->j = nonuniform_cubic_spline(
		f0->j,
		f1->j,
		f2->j,
		f3->j,
		t0,
		t1,
		t2,
		t3,
		t);
	result->k = nonuniform_cubic_spline(
		f0->k,
		f1->k,
		f2->k,
		f3->k,
		t0,
		t1,
		t2,
		t3,
		t);

	return;
}

void glow_render(
	long object_index,
	long glow_index)
{
	struct glow_datum *glow;
	struct glow_definition *definition;
	struct build_sprite_data sprite_data;
	struct glow_particle *particle;

	glow = glow_get(glow_index);
	definition = glow_definition_get(glow->definition_index);
	build_sprites_begin(
		&sprite_data,
		glow->number_of_particles,
		definition->texture.index,
		&global_shader_effect_additive,
		0);

	for (particle = glow->head_particle; particle; particle = particle->next)
	{
		build_sprite(
			&sprite_data,
			0,
			0,
			0,
			&particle->position,
			&glow->markers[particle->parent_marker_index].matrix.forward,
			0.0f,
			particle->present_size,
			&particle->color,
			particle->fade,
			0);
	}

	build_sprites_end(&sprite_data);
	return;
}

static void glow_trailing_particle_update_color(
	struct glow_datum *glow,
	struct glow_particle *particle)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);

	if (TEST_FLAG(definition->flags, _glow_definition_trailing_particles_fade_over_time_bit))
	{
		particle->fade = 1.0f - (real)particle->ticks_in_existence / particle->lifetime;
		particle->fade = PIN(particle->fade, 0.0f, 1.0f);
	}
	else
	{
		particle->fade = 1.0f;
	}

	return;
}

static void glow_trailing_particle_update_size(
	struct glow_datum *glow,
	struct glow_particle *particle)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);

	if (TEST_FLAG(definition->flags, _glow_definition_trailing_particles_shrink_over_time_bit))
	{
		real scale = MAX(0.0f, 1.0f - (real)particle->ticks_in_existence / particle->lifetime);

		particle->present_size = scale * particle->initial_size;
	}

	return;
}

static void glow_trailing_particle_update_velocity(
	struct glow_datum *glow,
	struct glow_particle *particle)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);

	if (TEST_FLAG(definition->flags, _glow_definition_trailing_particles_slow_over_time_bit))
	{
		real scale = MAX(0.0f, 1.0f - (real)particle->ticks_in_existence / particle->lifetime);

		particle->present_velocity.i = scale * particle->initial_velocity.i;
		particle->present_velocity.j = scale * particle->initial_velocity.j;
		particle->present_velocity.k = scale * particle->initial_velocity.k;
	}
	else
	{
		particle->present_velocity = particle->initial_velocity;
	}

	return;
}

static void glow_trailing_particle_update_position(
	struct glow_datum *glow,
	struct glow_particle *particle,
	real elapsed_time)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);

	particle->position.x = elapsed_time * particle->present_velocity.i + particle->position.x;
	particle->position.y = elapsed_time * particle->present_velocity.j + particle->position.y;
	particle->position.z = elapsed_time * particle->present_velocity.k + particle->position.z;

	return;
}

static void glow_trailing_particle_age(
	struct glow_datum *glow,
	struct glow_particle *particle)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);

	if (particle->ticks_in_existence > particle->lifetime)
	{
		struct glow_particle *previous_particle = particle->previous;
		struct glow_particle *next_particle = particle->next;

		if (previous_particle)
			previous_particle->next = next_particle;
		else
			glow->head_particle = next_particle;

		if (next_particle)
			next_particle->previous = previous_particle;
		else
			glow->tail_particle = previous_particle;

		datum_delete(glow_globals.glow_particle_data, particle->index);
		glow->number_of_particles--;
	}

	return;
}

static void glow_normal_particle_update_color(
	long object_index,
	struct glow_datum *glow,
	struct glow_particle *particle)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	real function_value;
	real t;
	real edge_fade;

	if (definition->color_attachment_index != NONE)
	{
		real scale;

		if (!object_get_function_value(
			object_index,
			definition->color_attachment_index,
			&function_value))
		{
			scale = 0.0f;
		}
		else
		{
			scale = function_value;
		}

		particle->color.red = (definition->color_upper_bound.red - definition->color_lower_bound.red) * scale + definition->color_lower_bound.red;
		particle->color.green = (definition->color_upper_bound.green - definition->color_lower_bound.green) * scale + definition->color_lower_bound.green;
		particle->color.blue = (definition->color_upper_bound.blue - definition->color_lower_bound.blue) * scale + definition->color_lower_bound.blue;
		particle->color.alpha = 1.0f;
	}

	if (TEST_FLAG(definition->flags, _glow_definition_modify_particle_color_bit))
	{
		particle->color.red = (definition->color_upper_bound.red - definition->color_lower_bound.red) * definition->color_rate_of_change * particle->t + definition->color_lower_bound.red;
		particle->color.green = (definition->color_upper_bound.green - definition->color_lower_bound.green) * definition->color_rate_of_change * particle->t + definition->color_lower_bound.green;
		particle->color.blue = (definition->color_upper_bound.blue - definition->color_lower_bound.blue) * definition->color_rate_of_change * particle->t + definition->color_lower_bound.blue;
		particle->color.alpha = 1.0f;
	}

	t = particle->t / glow->total_time;
	edge_fade = definition->percentage_edge_fade * 0.5f;
	if (t < edge_fade)
		particle->fade = t / edge_fade;
	else if (t > 1.0f - edge_fade)
		particle->fade = (1.0f - t) / edge_fade;
	else
		particle->fade = 1.0f;

	particle->fade = PIN(particle->fade, 0.0f, 1.0f);
	return;
}

static void glow_normal_particle_update_size(
	struct glow_particle *particle)
{
	particle->present_size = particle->initial_size;
	return;
}

static struct glow_particle *glow_particle_new(
	struct glow_datum *glow)
{
	struct glow_particle *particle = NULL;
	long particle_index = datum_new(glow_globals.glow_particle_data);

	if (particle_index != NONE)
	{
		particle = glow_particle_get(particle_index);
		particle->index = particle_index;
	}

	return particle;
}

static struct glow_particle *glow_normal_particle_new(
	struct glow_datum *glow,
	short particle_index,
	short particle_count)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	struct glow_particle *particle = glow_particle_new(glow);

	if (particle)
	{
		if (definition->distance_to_object_attachment_index == NONE)
		{
			particle->distance_to_object = real_local_random_range(
				definition->minimum_distance_to_object,
				definition->maximum_distance_to_object);
		}

		if (definition->particle_size_attachment_index == NONE)
		{
			particle->initial_size = real_local_random_range(
				definition->particle_size_lower_bound,
				definition->particle_size_upper_bound);
			particle->initial_size /= glow->bitmap_dimension;
		}

		if (definition->color_attachment_index == NONE &&
			!TEST_FLAG(definition->flags, _glow_definition_modify_particle_color_bit))
		{
			real scale = real_local_random_range(0.0f, 1.0f);

			particle->color.alpha = 1.0f;
			particle->color.red = (definition->color_upper_bound.red - definition->color_lower_bound.red) * scale + definition->color_lower_bound.red;
			particle->color.green = (definition->color_upper_bound.green - definition->color_lower_bound.green) * scale + definition->color_lower_bound.green;
			particle->color.blue = (definition->color_upper_bound.blue - definition->color_lower_bound.blue) * scale + definition->color_lower_bound.blue;
		}

		switch (definition->particle_distribution)
		{
			case _glow_particle_distribution_random:
				particle->t = real_local_random_range(0.0f, glow->total_time);
				break;

			case _glow_particle_distribution_uniform:
				particle->t = (real)particle_index / particle_count * glow->total_time;
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 945, FALSE, NULL);
				return particle;
		}

		particle->initial_angle = real_local_random_range(0.0f, 2.0f * _pi);
	}

	return particle;
}

static void get_particle_world_position(
	struct glow_datum *glow,
	struct glow_particle *particle,
	real rotation_per_unit_t)
{
	real_vector3d sides[4];
	real_vector3d side;
	real_vector3d ups[4];
	real_vector3d up;
	real_point3d positions[4];
	real_vector3d position_vectors[4];
	real_vector3d position;
	real knots[4];
	real angle;
	short marker_index;
	short first_marker_index;
	short last_marker_index;
	short index;

	for (marker_index = 0; marker_index < glow->number_of_markers - 1; marker_index++)
	{
		if (glow->marker_time_index[marker_index] <= particle->t &&
			glow->marker_time_index[marker_index + 1] > particle->t)
		{
			break;
		}
	}
	match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 1079, marker_index < glow->number_of_markers - 1, "marker_index<glow->number_of_markers-1");
	particle->parent_marker_index = PIN(marker_index, 0, glow->number_of_markers - 1);

	match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 1083, glow->number_of_markers > 1, "glow->number_of_markers > 1");

	switch (glow->number_of_markers)
	{
		case 2:
			positions[0] = glow->markers[0].matrix.position;
			positions[3] = glow->markers[1].matrix.position;
			ups[0] = glow->markers[0].matrix.up;
			ups[3] = glow->markers[1].matrix.up;
			cross_product3d(
				&glow->markers[0].matrix.up,
				&glow->markers[0].matrix.forward,
				&sides[0]);
			cross_product3d(
				&glow->markers[1].matrix.up,
				&glow->markers[1].matrix.forward,
				&sides[3]);
			knots[0] = glow->marker_time_index[0];
			knots[3] = glow->marker_time_index[1];

			positions[1].x = (positions[3].x - positions[0].x) * 0.25f + positions[0].x;
			positions[1].y = (positions[3].y - positions[0].y) * 0.25f + positions[0].y;
			positions[1].z = (positions[3].z - positions[0].z) * 0.25f + positions[0].y;
			positions[2].x = (positions[3].x - positions[0].x) * 0.75f + positions[0].x;
			positions[2].y = (positions[3].y - positions[0].y) * 0.75f + positions[0].y;
			positions[2].z = (positions[3].z - positions[0].z) * 0.75f + positions[0].y;

			ups[1].i = (ups[3].i - ups[0].i) * 0.25f + ups[0].i;
			ups[1].j = (ups[3].j - ups[0].j) * 0.25f + ups[0].j;
			ups[1].k = (ups[3].k - ups[0].k) * 0.25f + ups[0].j;
			ups[2].i = (ups[3].i - ups[0].i) * 0.75f + ups[0].i;
			ups[2].j = (ups[3].j - ups[0].j) * 0.75f + ups[0].j;
			ups[2].k = (ups[3].k - ups[0].k) * 0.75f + ups[0].j;
			sides[1].i = (sides[3].i - sides[0].i) * 0.25f + sides[0].i;
			sides[1].j = (sides[3].j - sides[0].j) * 0.25f + sides[0].j;
			sides[1].k = (sides[3].k - sides[0].k) * 0.25f + sides[0].k;
			sides[2].i = (sides[3].i - sides[0].i) * 0.75f + sides[0].i;
			sides[2].j = (sides[3].j - sides[0].j) * 0.75f + sides[0].j;
			sides[2].k = (sides[3].k - sides[0].k) * 0.75f + sides[0].k;

			knots[1] = (knots[3] - knots[0]) * 0.25f + knots[0];
			knots[2] = (knots[3] - knots[0]) * 0.75f + knots[0];
			break;

		case 3:
			positions[0] = glow->markers[0].matrix.position;
			positions[3] = glow->markers[2].matrix.position;
			ups[0] = glow->markers[0].matrix.up;
			ups[3] = glow->markers[2].matrix.up;
			cross_product3d(
				&glow->markers[0].matrix.up,
				&glow->markers[0].matrix.forward,
				&sides[0]);
			cross_product3d(
				&glow->markers[2].matrix.up,
				&glow->markers[2].matrix.forward,
				&sides[3]);
			knots[0] = glow->marker_time_index[0];
			knots[3] = glow->marker_time_index[2];

			switch (particle->parent_marker_index)
			{
				case 0:
					positions[1] = glow->markers[1].matrix.position;
					ups[1] = glow->markers[1].matrix.up;
					cross_product3d(
						&glow->markers[1].matrix.up,
						&glow->markers[1].matrix.forward,
						&sides[1]);
					knots[1] = glow->marker_time_index[1];

					positions[2].x = (positions[3].x - positions[1].x) * 0.5f + positions[1].x;
					positions[2].y = (positions[3].y - positions[1].y) * 0.5f + positions[1].y;
					positions[2].z = (positions[3].z - positions[1].z) * 0.5f + positions[1].y;

					ups[2].i = (ups[3].i - ups[1].i) * 0.5f + ups[1].i;
					ups[2].j = (ups[3].j - ups[1].j) * 0.5f + ups[1].j;
					ups[2].k = (ups[3].k - ups[1].k) * 0.5f + ups[1].j;
					sides[2].i = (sides[3].i - sides[1].i) * 0.5f + sides[1].i;
					sides[2].j = (sides[3].j - sides[1].j) * 0.5f + sides[1].j;
					sides[2].k = (sides[3].k - sides[1].k) * 0.5f + sides[1].k;

					knots[2] = (knots[3] - knots[1]) * 0.5f + knots[1];
					break;

				case 1:
					positions[2] = glow->markers[1].matrix.position;
					ups[2] = glow->markers[1].matrix.up;
					cross_product3d(
						&glow->markers[1].matrix.up,
						&glow->markers[1].matrix.forward,
						&sides[2]);

					positions[1].x = (positions[2].x - positions[0].x) * 0.5f + positions[0].x;
					positions[1].y = (positions[2].y - positions[0].y) * 0.5f + positions[0].y;
					positions[1].z = (positions[2].z - positions[0].z) * 0.5f + positions[0].y;

					ups[1].i = (ups[2].i - ups[0].i) * 0.5f + ups[0].i;
					ups[1].j = (ups[2].j - ups[0].j) * 0.5f + ups[0].j;
					ups[1].k = (ups[2].k - ups[0].k) * 0.5f + ups[0].j;
					sides[1].i = (sides[2].i - sides[0].i) * 0.5f + sides[0].i;
					sides[1].j = (sides[2].j - sides[0].j) * 0.5f + sides[0].j;
					sides[1].k = (sides[2].k - sides[0].k) * 0.5f + sides[0].k;

					knots[1] = (glow->marker_time_index[1] - knots[0]) * 0.5f + knots[0];
					knots[2] = glow->marker_time_index[1];
					break;
			}
			break;

		default:
			for (marker_index = 0; marker_index < glow->number_of_markers - 1; marker_index++)
			{
				if (glow->marker_time_index[marker_index] <= particle->t &&
					particle->t <= glow->marker_time_index[marker_index + 1])
				{
					break;
				}
			}
			match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 1180, marker_index < glow->number_of_markers - 1, "marker_index<glow->number_of_markers-1");
			marker_index = PIN(marker_index, 0, glow->number_of_markers - 1);

			first_marker_index = marker_index;
			last_marker_index = (short)(marker_index + 1);
			while (last_marker_index - first_marker_index + 1 < 4)
			{
				if (first_marker_index > 0)
					first_marker_index--;
				if (last_marker_index < glow->number_of_markers - 1)
					last_marker_index++;
			}

			knots[0] = glow->marker_time_index[first_marker_index];
			knots[1] = glow->marker_time_index[first_marker_index + 1];
			knots[2] = glow->marker_time_index[first_marker_index + 2];
			knots[3] = glow->marker_time_index[first_marker_index + 3];

			for (index = 0; index < 4; index++)
			{
				struct object_marker *marker = &glow->markers[glow->marker_order[first_marker_index + index]];

				positions[index] = marker->matrix.position;
				ups[index] = marker->matrix.up;
				cross_product3d(&marker->matrix.up, &marker->matrix.forward, &sides[index]);
			}
			break;
	}

	for (index = 0; index < NUMBEROF(position_vectors); index++)
	{
		position_vectors[index].i = positions[index].x;
		position_vectors[index].j = positions[index].y;
		position_vectors[index].k = positions[index].z;
	}

	nonuniform_cubic_spline_vector3d(
		&position,
		&position_vectors[0],
		&position_vectors[1],
		&position_vectors[2],
		&position_vectors[3],
		knots[0],
		knots[1],
		knots[2],
		knots[3],
		particle->t);
	nonuniform_cubic_spline_vector3d(
		&up,
		&ups[0],
		&ups[1],
		&ups[2],
		&ups[3],
		knots[0],
		knots[1],
		knots[2],
		knots[3],
		particle->t);
	nonuniform_cubic_spline_vector3d(
		&side,
		&sides[0],
		&sides[1],
		&sides[2],
		&sides[3],
		knots[0],
		knots[1],
		knots[2],
		knots[3],
		particle->t);

	particle->position.x = position.i;
	particle->position.y = position.j;
	particle->position.z = position.k;

	angle = rotation_per_unit_t * particle->t + particle->initial_angle;
	particle->position.x = (side.i * (real)cos(angle) + up.i * (real)sin(angle)) * particle->distance_to_object + particle->position.x;
	particle->position.y = (side.j * (real)cos(angle) + up.j * (real)sin(angle)) * particle->distance_to_object + particle->position.y;
	particle->position.z = (side.k * (real)cos(angle) + up.k * (real)sin(angle)) * particle->distance_to_object + particle->position.z;

	return;
}

static void glow_normal_particle_update_position(
	long object_index,
	struct glow_datum *glow,
	struct glow_particle *particle,
	real elapsed_time,
	real rotation_per_unit_t)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	real function_value;

	if (definition->distance_to_object_attachment_index != NONE)
	{
		if (!object_get_function_value(
			object_index,
			definition->distance_to_object_attachment_index,
			&function_value))
		{
			function_value = 0.0f;
		}

		particle->distance_to_object =
			((definition->distance_to_object_scale_upper_bound - definition->distance_to_object_scale_lower_bound) * function_value + definition->distance_to_object_scale_lower_bound) *
			(definition->maximum_distance_to_object - definition->minimum_distance_to_object) +
			definition->minimum_distance_to_object;
	}

	if (TEST_FLAG(particle->flags, _glow_particle_moving_backwards_bit))
	{
		particle->t -= elapsed_time;

		switch (definition->boundary_effect)
		{
			case _glow_boundary_effect_bounce:
				if (particle->t < 0.0f)
				{
					while (particle->t < 0.0f)
						particle->t += glow->total_time;
					particle->t = glow->total_time - particle->t;
					SET_FLAG(particle->flags, _glow_particle_moving_backwards_bit, FALSE);
				}
				break;

			case _glow_boundary_effect_wrap:
				while (particle->t < 0.0f)
					particle->t += glow->total_time;
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 800, FALSE, "glow effect received illegal boundary effect");
				break;
		}
	}
	else
	{
		particle->t += elapsed_time;

		switch (definition->boundary_effect)
		{
			case _glow_boundary_effect_bounce:
				if (particle->t > glow->total_time)
				{
					while (particle->t > glow->total_time)
						particle->t -= glow->total_time;
					particle->t = glow->total_time - particle->t;
					SET_FLAG(particle->flags, _glow_particle_moving_backwards_bit, TRUE);
				}
				break;

			case _glow_boundary_effect_wrap:
				while (particle->t > glow->total_time)
					particle->t -= glow->total_time;
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 829, FALSE, "glow effect received illegal boundary effect");
				break;
		}
	}

	get_particle_world_position(glow, particle, rotation_per_unit_t);
	return;
}

static void glow_particles_initialize(
	struct glow_datum *glow)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	struct glow_particle *previous_particle = NULL;
	boolean moving_backwards = TRUE;
	short particle_index;

	for (particle_index = 0; particle_index < glow->number_of_particles; particle_index++)
	{
		struct glow_particle *particle = glow_normal_particle_new(
			glow,
			particle_index,
			glow->number_of_particles);

		if (!particle)
			break;

		if (TEST_FLAG(definition->flags, _glow_definition_particles_move_backwards_bit))
			SET_FLAG(particle->flags, _glow_particle_moving_backwards_bit, TRUE);

		if (TEST_FLAG(definition->flags, _glow_definition_particles_move_in_both_directions_bit))
		{
			SET_FLAG(particle->flags, _glow_particle_moving_backwards_bit, !moving_backwards);
			moving_backwards = !moving_backwards;
		}

		if (!glow->head_particle)
			glow->head_particle = particle;
		if (previous_particle)
			previous_particle->next = particle;
		particle->previous = previous_particle;
		glow->tail_particle = particle;
		previous_particle = particle;
	}

	return;
}

static struct glow_particle *glow_trailing_particle_new(
	struct glow_datum *glow)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	struct glow_particle *particle = glow_particle_new(glow);

	if (particle)
	{
		real scale;

		if (glow->number_of_markers > 1)
		{
			particle->t = real_local_random_range(
				definition->trailing_particle_minimum_t * glow->total_time,
				definition->trailing_particle_maximum_t * glow->total_time);
			get_particle_world_position(glow, particle, 0.0f);
		}
		else
		{
			particle->position = glow->markers[0].matrix.position;
		}

		switch (definition->trailing_particle_distribution)
		{
			case _glow_trailing_particle_distribution_vertical:
				particle->initial_velocity.i = 0.0f;
				particle->initial_velocity.j = 0.0f;
				particle->initial_velocity.k = 1.0f;
				break;

			case _glow_trailing_particle_distribution_normal:
				particle->initial_velocity = glow->markers[particle->parent_marker_index].matrix.up;
				break;

			case _glow_trailing_particle_distribution_random:
				particle->initial_velocity.i = real_local_random_range(-1.0f, 1.0f);
				particle->initial_velocity.j = real_local_random_range(-1.0f, 1.0f);
				particle->initial_velocity.k = real_local_random_range(-1.0f, 1.0f);
				normalize3d(&particle->initial_velocity);
				break;

			default:
				match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 996, FALSE, "unknown trailing particle distribution?");
				break;
		}

		scale = definition->trailing_particle_velocity / 30.0f;
		particle->initial_velocity.i *= scale;
		particle->initial_velocity.j *= scale;
		particle->initial_velocity.k *= scale;

		particle->initial_size = real_local_random_range(
			definition->particle_size_lower_bound,
			definition->particle_size_upper_bound);
		particle->initial_size /= glow->bitmap_dimension;

		particle->lifetime = (short)(long)(definition->trailing_particle_lifetime * 30.0f);

		scale = real_local_random_range(0.0f, 1.0f);
		particle->color.alpha = 1.0f;
		particle->color.red = (definition->color_upper_bound.red - definition->color_lower_bound.red) * scale + definition->color_lower_bound.red;
		particle->color.green = (definition->color_upper_bound.green - definition->color_lower_bound.green) * scale + definition->color_lower_bound.green;
		particle->color.blue = (definition->color_upper_bound.blue - definition->color_lower_bound.blue) * scale + definition->color_lower_bound.blue;

		SET_FLAG(particle->flags, _glow_particle_trailing_bit, TRUE);
	}

	return particle;
}

static void glow_update(
	struct glow_datum *glow,
	long object_index)
{
	struct glow_definition *definition = glow_definition_get(glow->definition_index);
	struct glow_particle *particle;
	real rotation_per_unit_t;
	real effect_rotational_velocity;
	real effect_translational_velocity;

	if (!definition)
		return;

	glow->number_of_markers = object_get_marker_by_name(
		object_index,
		definition->attachment_marker,
		glow->markers,
		MAXIMUM_GLOW_MARKERS);

	if (!glow->initialized && glow->number_of_markers > 1)
	{
		short marker_order[MAXIMUM_GLOW_MARKERS];
		short marker_index;
		short order_index;
		short previous_marker_index;

		for (marker_index = 0; marker_index < glow->number_of_markers; marker_index++)
		{
			short best_marker_index = NONE;
			real best_dot = 0.0f;
			short candidate_index;

			for (candidate_index = 0; candidate_index < glow->number_of_markers; candidate_index++)
			{
				real_vector3d direction;
				real dot;

				if (marker_index == candidate_index)
					continue;

				vector_from_points3d(
					&glow->markers[marker_index].matrix.position,
					&glow->markers[candidate_index].matrix.position,
					&direction);
				normalize3d(&direction);

				dot = direction.i * glow->markers[marker_index].matrix.forward.i +
					direction.j * glow->markers[marker_index].matrix.forward.j +
					direction.k * glow->markers[marker_index].matrix.forward.k;
				if (dot > best_dot)
				{
					best_dot = dot;
					best_marker_index = candidate_index;
				}
			}

			marker_order[marker_index] = best_marker_index;
		}

		previous_marker_index = NONE;
		for (order_index = glow->number_of_markers - 1; order_index >= 0; order_index--)
		{
			short search_index;

			for (search_index = glow->number_of_markers - 1; search_index >= 0; search_index--)
			{
				if (marker_order[search_index] == previous_marker_index)
				{
					glow->marker_order[order_index] = search_index;
					break;
				}
			}

			previous_marker_index = search_index;
		}

		glow->total_time = 0.0f;
		glow->marker_time_index[0] = 0.0f;
		for (marker_index = 0; marker_index < glow->number_of_markers - 1; marker_index++)
		{
			struct object_marker *first_marker = &glow->markers[glow->marker_order[marker_index]];
			struct object_marker *second_marker = &glow->markers[glow->marker_order[marker_index + 1]];
			real_point3d first_position = first_marker->matrix.position;
			real_point3d second_position = second_marker->matrix.position;

			glow->total_time += distance3d(&first_position, &second_position);
			glow->marker_time_index[marker_index + 1] = glow->total_time;
		}

		glow_particles_initialize(glow);
		glow->accumulated_trailing_particle_generation_ticks = 0;
		glow->initialized = TRUE;
		return;
	}

	if (glow->number_of_markers > 1)
	{
		effect_rotational_velocity = definition->effect_rotational_velocity;
		if (definition->effect_rotational_velocity_attachment_index != NONE)
		{
			real function_value;
			real scale;

			if (!object_get_function_value(
				object_index,
				definition->effect_rotational_velocity_attachment_index,
				&function_value))
			{
				scale = 0.0f;
			}
			else
			{
				scale = function_value;
			}

			effect_rotational_velocity =
				((definition->effect_rotational_velocity_scale_upper_bound - definition->effect_rotational_velocity_scale_lower_bound) * scale +
					definition->effect_rotational_velocity_scale_lower_bound) * effect_rotational_velocity;
		}

		effect_translational_velocity = definition->effect_translational_velocity;
		if (definition->effect_translational_velocity_attachment_index != NONE)
		{
			real function_value;
			real scale;

			if (!object_get_function_value(
				object_index,
				definition->effect_translational_velocity_attachment_index,
				&function_value))
			{
				scale = 0.0f;
			}
			else
			{
				scale = function_value;
			}

			effect_translational_velocity =
				((definition->effect_translational_velocity_scale_upper_bound - definition->effect_translational_velocity_scale_lower_bound) * scale +
					definition->effect_translational_velocity_scale_lower_bound) * effect_translational_velocity;
		}

		rotation_per_unit_t = effect_rotational_velocity / effect_translational_velocity;
	}

	glow->accumulated_trailing_particle_generation_ticks += (short)game_time_get();

	if (glow->number_of_markers > 1)
	{
		for (particle = glow->head_particle; particle; particle = particle->next)
		{
			if (!TEST_FLAG(particle->flags, _glow_particle_trailing_bit))
			{
				glow_normal_particle_update_position(
					object_index,
					glow,
					particle,
					render.time_delta_since_tick_sec * effect_translational_velocity,
					rotation_per_unit_t);
				glow_normal_particle_update_color(object_index, glow, particle);
				glow_normal_particle_update_size(particle);
			}
		}
	}

	for (particle = glow->head_particle; particle; particle = particle->next)
	{
		if (TEST_FLAG(particle->flags, _glow_particle_trailing_bit))
		{
			particle->ticks_in_existence += (short)game_time_get();
			glow_trailing_particle_update_color(glow, particle);
			glow_trailing_particle_update_size(glow, particle);
			glow_trailing_particle_update_velocity(glow, particle);
			glow_trailing_particle_update_position(glow, particle, render.time_delta_since_tick_sec);
			glow_trailing_particle_age(glow, particle);
		}
	}

	if (definition->trailing_particle_generation_frequency > 0.01f)
	{
		real ticks_per_particle = 30.0f / definition->trailing_particle_generation_frequency;

		if (ticks_per_particle < 1.0f)
			ticks_per_particle = 1.0f;

		while (glow->accumulated_trailing_particle_generation_ticks > ticks_per_particle)
		{
			particle = glow_trailing_particle_new(glow);
			match_vassert("c:\\halo\\SOURCE\\objects\\widgets\\glow.c", 521, particle, "the map limit for the number of active glow particles has been reached");
			if (!particle)
				break;

			glow->number_of_particles++;
			if (glow->tail_particle)
			{
				glow->tail_particle->next = particle;
				particle->previous = glow->tail_particle;
			}
			else
			{
				glow->head_particle = particle;
			}
			glow->tail_particle = particle;

			glow->accumulated_trailing_particle_generation_ticks -= (short)ticks_per_particle;
		}
	}

	return;
}

void glow_submit(
	long object_index,
	long glow_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation)
{
	if (object_index != NONE && glow_index != NONE)
	{
			struct glow_datum *glow = glow_get(glow_index);
			struct glow_definition *definition = glow_definition_get(glow->definition_index);
			struct object_marker marker;

			glow_update(glow_get(glow_index), object_index);
		object_get_marker_by_name(object_index, definition->attachment_marker, &marker, 1);
		glow_render(object_index, glow_index);
	}

	return;
}
