/*
PARTICLES.C

symbols in this file:
0008FAF0 0030:
	_particles_initialize (0000)
0008FB20 0010:
	_particles_initialize_for_new_map (0000)
0008FB30 0010:
	_particles_dispose_from_old_map (0000)
0008FB40 0020:
	_particles_dispose (0000)
0008FB60 0020:
	_particle_delete (0000)
0008FB80 0070:
	_particles_stop_on_first_person_weapon (0000)
0008FBF0 0010:
	_particles_disconnect_from_structure_bsp (0000)
0008FC00 00d0:
	_particles_reconnect_to_structure_bsp (0000)
0008FCD0 0010:
	_new_particle_is_visible (0000)
0008FCE0 0040:
	_particle_get_radius (0000)
0008FD20 0060:
	_valid_real_point3d (0000)
0008FD80 0060:
	_valid_real_argb_color (0000)
0008FDE0 0150:
	_particle_effect_new (0000)
0008FF30 0050:
	_particle_die (0000)
0008FF80 0180:
	_particle_next_sequence (0000)
00090100 00d0:
	_particle_next_frame (0000)
000901D0 00d0:
	_particle_update_frame_time (0000)
000902A0 03a0:
	_particle_update_physics (0000)
00090640 05a0:
	_particle_new (0000)
00090BE0 0120:
	_particles_update (0000)
0025A8F0 0008:
	??_C@_07GFBFDLBM@gravity?$AA@ (0000)
0025A8F8 0008:
	_particle_collision_effect_scale_bounds (0000)
0025A900 0011:
	??_C@_0BB@HHDONMHD@particles_update?$AA@ (0000)
0025A914 0023:
	??_C@_0CD@JPDMOBBK@couldn?8t?5allocate?5particle?5globa@ (0000)
0025A938 0023:
	??_C@_0CD@IKBENHKF@c?3?2halo?2SOURCE?2effects?2particles@ (0000)
0025A95C 0009:
	??_C@_08HIJBMAOA@?$CGdiffuse?$AA@ (0000)
0025A968 0007:
	??_C@_06IOIMBPOK@?$CGlight?$AA@ (0000)
0025A970 0031:
	??_C@_0DB@EEHLCDHM@?$CFs?3?5assert_valid_real_argb_color@ (0000)
0025A9A4 000d:
	??_C@_0N@MNPLGION@?$CGdata?9?$DOcolor?$AA@ (0000)
0025A9B4 002a:
	??_C@_0CK@EAOMHCPC@?$CFs?3?5assert_valid_real_point3d?$CI?$CFf@ (0000)
0025A9E0 0010:
	??_C@_0BA@IFGIHPJG@?$CGdata?9?$DOposition?$AA@ (0000)
0025A9F0 002b:
	??_C@_0CL@HMPNAHGK@?$CFs?3?5assert_valid_real_vector2d?$CI?$CF@ (0000)
0025AA1C 0010:
	??_C@_0BA@OHENJGHL@?$CGdata?9?$DOvelocity?$AA@ (0000)
002DD7A0 0600:
	_particles_update_section (0000)
*/

/* ---------- headers */

#include "effects/particles.h"

#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "cseries/errors.h"
#include "cseries/profile.h"
#include "effects/effect_definitions.h"
#include "effects/effects.h"
#include "effects/material_effects.h"
#include "game/players.h"
#include "interface/first_person_weapons.h"
#include "memory/data.h"
#include "objects/object_lights.h"
#include "objects/objects.h"
#include "physics/point_physics.h"
#include "render/render.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"

/* ---------- constants */

enum
{
	MAXIMUM_PARTICLE_UNRENDERED_FRAME_COUNT = 15,
	NUMBER_OF_PARTICLE_EFFECT_MARKERS = 2
};

#define PARTICLE_SHARED_LIFESPAN 0.7f

enum particle_effect_markers
{
	_particle_effect_velocity_marker = 0,
	_particle_effect_gravity_marker
};

enum material_effect_event
{
	_material_effect_impact = 8
};

enum particle_collision_scale_bounds
{
	_particle_collision_scale_upper_bound = 0,
	_particle_collision_scale_lower_bound
};

enum particle_definition_flags
{
	_particle_definition_can_animate_backwards_bit = 0,
	_particle_definition_animation_stops_at_rest_bit,
	_particle_definition_animation_starts_on_random_frame_bit,
	_particle_definition_animate_once_per_frame_bit,
	_particle_definition_dies_at_rest_bit,
	_particle_definition_dies_on_contact_with_structure_bit,
	_particle_definition_tint_from_diffuse_texture_bit,
	_particle_definition_dies_on_contact_with_water_bit,
	_particle_definition_dies_on_contact_with_air_bit,
	_particle_definition_self_illuminated_bit,
	_particle_definition_random_horizontal_mirroring_bit,
	_particle_definition_random_vertical_mirroring_bit,
	NUMBER_OF_PARTICLE_DEFINITION_FLAGS
};

enum particle_states
{
	_particle_state_next_sequence_initial = 0,
	_particle_state_next_sequence_looping,
	_particle_state_still_looping,
	_particle_state_next_sequence_final,
	NUMBER_OF_PARTICLE_STATES
};

/* ---------- macros */

#define match_assert_valid_real_argb_color(file, line, color)		\
match_vassert(																\
	file,																\
	line,																\
	valid_real_argb_color(color),									\
	csprintf(															\
		temporary,														\
		"%s: assert_valid_real_argb_color(%f, %f, %f, %f)",			\
		#color, (*color).alpha,											\
		(*color).rgb.red, (*color).rgb.green, (*color).rgb.blue		\
	)																	\
)

#define match_assert_valid_real_rgb_color(file, line, rgb)			\
match_vassert(																\
	file,																\
	line,																\
	valid_real_rgb_color(rgb),										\
	csprintf(															\
		temporary,														\
		"%s: assert_valid_real_rgb_color(%f, %f, %f)",					\
		#rgb, (*rgb).red, (*rgb).green, (*rgb).blue					\
	)																	\
)

/* ---------- structures */

/* ---------- prototypes */

static boolean new_particle_is_visible(
	struct particle_definition const *definition,
	struct location const *location,
	real_point3d const *position,
	real radius);
static void particle_effect_new(
	struct particle_datum *particle,
	unsigned long group_tag,
	long effect_index,
	real scale);
static void particle_die(
	long particle_index);
static boolean particle_next_sequence(
	long particle_index);
static boolean particle_next_frame(
	long particle_index);
static boolean particle_update_frame_time(
	long particle_index,
	real dt);
static boolean particle_update_physics(
	long particle_index,
	real dt);

/* ---------- globals */

// {upper, lower} bounds of the collision speed that scales a particle collision effect
real const particle_collision_effect_scale_bounds[2] = {1.5f, 0.5f};

static struct profile_section particles_update_section = {"particles_update", NONE, TRUE};
#ifdef HALO_LINUX
/* A frame was a tick on the Xbox. The native builds draw several frames per
tick (port/linux/game/render_interpolation.c), so what counted frames counts
30 Hz ticks of game time instead: particles that animate once a frame step
once a tick, and a particle is deleted once it has gone unrendered for
MAXIMUM_PARTICLE_UNRENDERED_FRAME_COUNT ticks, the frame index drawn at each
of the last ones kept here. */
static real particles_leftover_ticks = 0.f;
static long particles_update_ticks = 0;
static long particles_tick_frame_indices[MAXIMUM_PARTICLE_UNRENDERED_FRAME_COUNT + 1];
static long particles_tick_frame_head = 0;
#endif

static char const *particle_effect_marker_names[NUMBER_OF_PARTICLE_EFFECT_MARKERS] =
{
	"velocity",
	"gravity"
};

/* ---------- public code */

void particles_initialize(
	void)
{
#ifdef HALO_LINUX
	/* the native builds' larger particle pool (halo_port_capacity.h);
	render_particles sizes its unchecked list of visible particles by the same
	value */
	particle_data = game_state_data_new("particle", HALO_PORT_MAXIMUM_PARTICLES, 0x70);
#else
	particle_data = game_state_data_new("particle", 1024, 0x70);
#endif
	if (!particle_data)
		error(_error_immediate, "couldn't allocate particle globals");

	return;
}

void particles_initialize_for_new_map(
	void)
{
	data_make_valid(particle_data);

	return;
}

void particles_dispose_from_old_map(
	void)
{
	data_make_invalid(particle_data);

	return;
}

void particles_dispose(
	void)
{
	if (particle_data)
		particle_data = NULL;

	return;
}

void particle_delete(
	long particle_index)
{
	datum_delete(particle_data, particle_index);

	return;
}

void particles_stop_on_first_person_weapon(
	short local_player_index)
{
	long particle_index;

	for (particle_index = data_next_index(particle_data, NONE);
		particle_index != NONE;
		particle_index = data_next_index(particle_data, particle_index))
	{
		struct particle_datum *particle = particle_get(particle_index);

		if (particle->local_player_index == local_player_index &&
			TEST_FLAG(particle->flags, _particle_datum_attached_to_local_player_bit) &&
			particle->object_index != NONE)
		{
			datum_delete(particle_data, particle_index);
		}
	}

	return;
}

void particles_disconnect_from_structure_bsp(
	void)
{
	return;
}

void particles_reconnect_to_structure_bsp(
	void)
{
	long particle_index;

	for (particle_index = data_next_index(particle_data, NONE);
		particle_index != NONE;
		particle_index = data_next_index(particle_data, particle_index))
	{
		struct particle_datum *particle = particle_get(particle_index);
		real_point3d const *position;

		if (particle->object_index == NONE)
		{
			position = &particle->position;
		}
		else if (TEST_FLAG(particle->flags, _particle_datum_attached_to_local_player_bit))
		{
			position = &first_person_weapon_get_node_matrix(
				particle->local_player_index,
				particle->node_index)->position;
		}
		else if (object_try_and_get(particle->object_index))
		{
			position = &object_get_node_matrix(
				particle->object_index,
				particle->node_index)->position;
		}
		else
		{
			position = NULL;
		}

		if (!position)
		{
			particle_delete(particle_index);
		}
		else
		{
			scenario_location_from_point(&particle->location, position);
			if (particle->location.cluster_index == NONE)
			{
				particle_delete(particle_index);
			}
		}
	}

	return;
}

static boolean new_particle_is_visible(
	struct particle_definition const *definition,
	struct location const *location,
	real_point3d const *position,
	real radius)
{
	if (scenario_location_potentially_visible_local(location))
		return TRUE;

	return FALSE;
}

real particle_get_radius(
	long particle_index)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);

	return ((definition->radius_upper_bound - definition->radius_lower_bound) *
			(particle->age / particle->lifespan) + definition->radius_lower_bound) *
		particle->radius;
}

boolean valid_real_argb_color(
	real_argb_color const *color)
{
	return valid_real(color->alpha) &&
		color->alpha >= 0.0f &&
		color->alpha <= 1.0f &&
		valid_real_rgb_color(&color->rgb);
}

static void particle_effect_new(
	struct particle_datum *particle,
	unsigned long group_tag,
	long effect_index,
	real scale)
{
	real_vector3d velocity;

	velocity.i = particle->translational_velocity.i * (1.0f / TICKS_PER_SECOND);
	velocity.j = particle->translational_velocity.j * (1.0f / TICKS_PER_SECOND);
	velocity.k = particle->translational_velocity.k * (1.0f / TICKS_PER_SECOND);

	if (group_tag == EFFECT_DEFINITION_TAG)
	{
		real_point3d marker_points[NUMBER_OF_PARTICLE_EFFECT_MARKERS];
		real_vector3d marker_forwards[NUMBER_OF_PARTICLE_EFFECT_MARKERS];

		marker_points[_particle_effect_gravity_marker] = particle->position;
		marker_points[_particle_effect_velocity_marker] = particle->position;
		marker_forwards[_particle_effect_velocity_marker] = particle->direction;
		marker_forwards[_particle_effect_gravity_marker] = *global_down3d;

		normalize3d(&marker_forwards[_particle_effect_velocity_marker]);

		effect_new_unattached_from_markers(
			effect_index,
			NONE,
			&velocity,
			NUMBER_OF_PARTICLE_EFFECT_MARKERS,
			particle_effect_marker_names,
			marker_points,
			marker_forwards,
			scale,
			0.0f,
			NULL,
			NULL,
			FALSE);
	}
	else if (group_tag == SOUND_DEFINITION_TAG)
	{
		struct sound_location sound_location;

		sound_location.position = particle->position;
		sound_location.forward = *global_forward3d;
		sound_location.translational_velocity = velocity;
		sound_location.game_location = particle->location;

		unattached_impulse_sound_new(effect_index, &sound_location, scale);
	}
	else
	{
		match_vassert(
			"c:\\halo\\SOURCE\\effects\\particles.c",
			799,
			FALSE,
			NULL);
	}

	return;
}

static void particle_die(
	long particle_index)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);

	if (definition->effect.index != NONE)
	{
		particle_effect_new(
			particle,
			definition->effect.group_tag,
			definition->effect.index,
			0.0f);
	}

	datum_delete(particle_data, particle_index);

	return;
}

static boolean particle_next_sequence(
	long particle_index)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);
	struct bitmap_group *bitmap = bitmap_group_get(definition->bitmap.index);

	particle->sequence_index = NONE;

	if (particle->state == _particle_state_next_sequence_initial)
	{
		if (definition->initial_sequence_count > 0)
		{
			particle->sequence_index = (short)(definition->first_sequence_index +
				local_random_range(0, definition->initial_sequence_count));
		}

		particle->state++;
	}

	if ((particle->sequence_index == NONE &&
			particle->state == _particle_state_next_sequence_looping) ||
		particle->state == _particle_state_still_looping)
	{
		if (particle->state == _particle_state_next_sequence_looping)
			particle->state = _particle_state_still_looping;

		if (particle->age < particle->lifespan &&
			definition->looping_sequence_count > 0)
		{
			particle->sequence_index = (short)(definition->initial_sequence_count +
				definition->first_sequence_index +
				local_random_range(0, definition->looping_sequence_count));
		}
		else
		{
			particle->state++;
		}
	}

	if (particle->sequence_index == NONE &&
		particle->state == _particle_state_next_sequence_final)
	{
		if (definition->final_sequence_count > 0)
		{
			particle->sequence_index = (short)(definition->looping_sequence_count +
				definition->initial_sequence_count +
				definition->first_sequence_index +
				local_random_range(0, definition->final_sequence_count));
		}

		particle->state++;
	}

	if (particle->sequence_index == NONE || !bitmap->sequences.count)
	{
		particle_die(particle_index);

		return FALSE;
	}

	particle->sequence_index = (short)PIN(particle->sequence_index, 0, bitmap->sequences.count - 1);

	return TRUE;
}

static boolean particle_next_frame(
	long particle_index)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);
	struct bitmap_group *bitmap = bitmap_group_get(definition->bitmap.index);
	boolean result = TRUE;

	particle->frame_time = 0.0f;

	if (TEST_FLAG(particle->flags, _particle_datum_animates_backwards_bit))
	{
		if (particle->frame_index > 0)
		{
			particle->frame_index--;
		}
		else
		{
			result = particle_next_sequence(particle_index);

			if (result)
			{
				struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
					&bitmap->sequences,
					particle->sequence_index,
					struct bitmap_group_sequence);

				particle->frame_index = (short)(sequence->sprites.count - 1);
			}
		}
	}
	else
	{
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap->sequences,
			particle->sequence_index,
			struct bitmap_group_sequence);

		if (particle->frame_index + 1 < sequence->sprites.count)
		{
			particle->frame_index++;
		}
		else
		{
			result = particle_next_sequence(particle_index);
			particle->frame_index = 0;
		}
	}

	return result;
}

static boolean particle_update_frame_time(
	long particle_index,
	real dt)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);
	boolean result = TRUE;

	if (!TEST_FLAG(definition->flags, _particle_definition_animation_stops_at_rest_bit) ||
		!TEST_FLAG(particle->flags, _particle_datum_at_rest_bit))
	{
		if (TEST_FLAG(definition->flags, _particle_definition_animate_once_per_frame_bit))
		{
#ifdef HALO_LINUX
			if (dt != 0.0f && particles_update_ticks > 0)
#else
			if (dt != 0.0f)
#endif
				return particle_next_frame(particle_index);
		}
		else
		{
			if (particle->frame_time == -1.0f)
			{
				result = particle_next_frame(particle_index);
				particle->frame_time = 0.0f;
			}

			while (dt > 0.0f && result)
			{
				real frame_time_remaining = particle->frame_span - particle->frame_time;

				if (frame_time_remaining <= dt)
				{
					result = particle_next_frame(particle_index);
					dt = dt - frame_time_remaining;
				}
				else
				{
					particle->frame_time = dt + particle->frame_time;

					break;
				}
			}
		}
	}

	return result;
}

static boolean particle_update_physics(
	long particle_index,
	real dt)
{
	struct particle_datum *particle = particle_get(particle_index);
	struct particle_definition *definition = particle_definition_get(particle->definition_index);

	if (!TEST_FLAG(particle->flags, _particle_datum_at_rest_bit))
	{
		boolean settled = FALSE;
		struct point_physics_definition *physics = point_physics_definition_get(definition->physics.index);

		if (particle->object_index == NONE)
		{
			real_vector3d collision_normal;
			short collision_material_type;
			unsigned long collision_flags;

			collision_flags = point_physics_update(
				0,
				physics,
				&particle->location,
				NONE,
				&particle->position,
				&particle->translational_velocity,
				NULL,
				&collision_normal,
				&collision_material_type,
				particle_get_radius(particle_index),
				dt);

			if (TEST_FLAG(collision_flags, _point_physics_collided_with_structure_bit))
			{
				long collision_effect_index = definition->collision_effect.index;

				if (collision_effect_index != NONE ||
					definition->collision_material_effects.index)
				{
					real scale = (magnitude3d(&particle->translational_velocity) -
						particle_collision_effect_scale_bounds[_particle_collision_scale_lower_bound]) /
						(particle_collision_effect_scale_bounds[_particle_collision_scale_upper_bound] -
							particle_collision_effect_scale_bounds[_particle_collision_scale_lower_bound]);

					if (scale < 0.0f)
						scale = 0.0f;
					else if (scale > 1.0f)
						scale = 1.0f;

					if (collision_effect_index != NONE)
					{
						particle_effect_new(
							particle,
							definition->collision_effect.group_tag,
							collision_effect_index,
							scale);
					}

					if (definition->collision_material_effects.index != NONE &&
						material_effect_visible(&particle->position))
					{
						material_effect_new(
							definition->collision_material_effects.index,
							_material_effect_impact,
							collision_material_type,
							&particle->position,
							&collision_normal,
							&particle->location,
							scale);
					}
				}

				if (TEST_FLAG(definition->flags,
					_particle_definition_dies_on_contact_with_structure_bit))
				{
					if (definition->collision_effect.index == NONE)
						particle_die(particle_index);
					else
						particle_delete(particle_index);

					return FALSE;
				}
			}

			if ((TEST_FLAG(collision_flags, _point_physics_in_air_bit) &&
					TEST_FLAG(definition->flags,
						_particle_definition_dies_on_contact_with_air_bit)) ||
				(TEST_FLAG(collision_flags, _point_physics_in_water_bit) &&
					TEST_FLAG(definition->flags,
						_particle_definition_dies_on_contact_with_water_bit)))
			{
				particle_die(particle_index);

				return FALSE;
			}

			if (TEST_FLAG(collision_flags, _point_physics_collided_with_structure_bit) ||
				TEST_FLAG(collision_flags, _point_physics_collided_with_water_bit))
			{
				if (collision_normal.k > 0.8f)
					settled = TRUE;

				particle->frame_span = definition->frames_per_second_contact_deterioration +
					particle->frame_span;
			}
		}
		else
		{
			real radius;
			real friction;
			real mass;
			real scale;

			if (!TEST_FLAG(particle->flags, _particle_datum_attached_to_local_player_bit) &&
				!object_try_and_get(particle->object_index))
			{
				datum_delete(particle_data, particle_index);

				return FALSE;
			}

			radius = particle_get_radius(particle_index);
			friction = physics->air_friction * radius * radius;
			mass = point_physics_definition_get_mass(physics, radius);

			if (mass == 0.0f)
			{
				scale = (friction == 0.0f) ? 1.0f : 0.0f;
			}
			else
			{
				scale = 1.0f - friction / mass * dt;
				if (scale < 0.0f)
					scale = 0.0f;
				else if (scale > 1.0f)
					scale = 1.0f;
			}

			particle->translational_velocity.i = scale * particle->translational_velocity.i;
			particle->translational_velocity.j = scale * particle->translational_velocity.j;
			particle->translational_velocity.k = scale * particle->translational_velocity.k;

			particle->position.x = particle->translational_velocity.i * dt + particle->position.x;
			particle->position.y = particle->translational_velocity.j * dt + particle->position.y;
			particle->position.z = particle->translational_velocity.k * dt + particle->position.z;

			settled = TRUE;
		}

		if (magnitude_squared3d(&particle->translational_velocity) < 0.0625f)
		{
			if (settled)
			{
				if (TEST_FLAG(definition->flags, _particle_definition_dies_at_rest_bit))
				{
					particle_die(particle_index);

					return FALSE;
				}

				SET_FLAG(particle->flags, _particle_datum_at_rest_bit, TRUE);
			}
		}
		else
		{
			particle->direction = particle->translational_velocity;
		}

		particle->rotation = dt * particle->angular_velocity + particle->rotation;
	}
	else if (particle->object_index != NONE && !object_try_and_get(particle->object_index))
	{
		datum_delete(particle_data, particle_index);

		return FALSE;
	}

	return TRUE;
}

void particle_new(
	struct new_particle_data const *data)
{
	struct particle_definition *definition;
	struct particle_datum *particle;
	struct location location;
	real_point3d position;
	long particle_index;

#ifdef HALO_N64
	/* (the N64 draws no particles: none are made, and none are simulated --
	their point physics alone cast ~1,500 rays a second in Firefight) */
	return;
#endif

	match_assert_valid_real_vector3d(
		"c:\\halo\\SOURCE\\effects\\particles.c",
		109,
		&data->velocity);
	match_assert_valid_real_point3d(
		"c:\\halo\\SOURCE\\effects\\particles.c",
		110,
		&data->position);
	match_assert_valid_real_argb_color(
		"c:\\halo\\SOURCE\\effects\\particles.c",
		111,
		&data->color);

	if (data->definition_index == NONE)
		return;

	definition = particle_definition_get(data->definition_index);

	if (data->object_index != NONE)
	{
		if (data->attached_to_local_player)
		{
			matrix4x3_transform_point(
				first_person_weapon_get_node_matrix(data->local_player_index, data->node_index),
				&data->position,
				&position);
		}
		else
		{
			matrix4x3_transform_point(
				object_get_node_matrix(data->object_index, data->node_index),
				&data->position,
				&position);
		}
	}
	else
	{
		position = data->position;
	}

	scenario_location_from_point(&location, &position);

	if (location.leaf_index == NONE ||
		!new_particle_is_visible(definition, &location, &position, data->radius))
	{
		return;
	}

	particle_index = datum_new(particle_data);

	if (particle_index == NONE)
		return;

	particle = particle_get(particle_index);

	particle->flags = 0;
	if (TEST_FLAG(definition->flags, _particle_definition_can_animate_backwards_bit))
	{
		particle->flags |= local_random() &
			(unsigned)FLAG(_particle_datum_animates_backwards_bit);
	}
	if (TEST_FLAG(definition->flags, _particle_definition_random_horizontal_mirroring_bit))
	{
		particle->flags |= local_random() &
			(unsigned)FLAG(_particle_datum_u_mirror_bit);
	}
	if (TEST_FLAG(definition->flags, _particle_definition_random_vertical_mirroring_bit))
	{
		particle->flags |= local_random() &
			(unsigned)FLAG(_particle_datum_v_mirror_bit);
	}
	SET_FLAG(particle->flags, _particle_datum_dont_draw_first_person_bit,
		data->dont_draw_first_person);
	SET_FLAG(particle->flags, _particle_datum_dont_draw_third_person_bit,
		data->dont_draw_third_person);
	SET_FLAG(particle->flags, _particle_datum_attached_to_local_player_bit,
		data->attached_to_local_player);

	particle->definition_index = data->definition_index;
	particle->local_player_index = (byte)data->local_player_index;
	particle->object_index = data->object_index;
	particle->node_index = data->node_index;
	particle->state = _particle_state_next_sequence_initial;
	particle->last_rendered_frame_index = render.frame_index;

	particle->lifespan = real_local_random_range(
		definition->life_span_lower_bound,
		definition->life_span_upper_bound);
	if (particle->lifespan > PARTICLE_SHARED_LIFESPAN)
	{
		particle->lifespan = (particle->lifespan - PARTICLE_SHARED_LIFESPAN) /
			local_player_count() + PARTICLE_SHARED_LIFESPAN;
	}

	particle->frame_span = definition->frames_per_second_upper_bound != 0.0f
		? 1.0f / real_local_random_range(
			definition->frames_per_second_lower_bound,
			definition->frames_per_second_upper_bound)
		: FLT_MAX;
	particle->frame_time = -1.0f;

	particle->location = location;
	particle->position = data->position;
	particle->direction = data->direction;
	particle->rotation = data->rotation;
	particle->radius = data->radius;
	particle->color = data->color;
	particle->translational_velocity = data->velocity;

	if (particle->object_index == NONE)
	{
		real mass = point_physics_definition_get_mass(
			point_physics_definition_get(definition->physics.index),
			particle_get_radius(particle_index));

		particle->translational_velocity.i = data->initial_impulse.i * mass +
			particle->translational_velocity.i;
		particle->translational_velocity.j = data->initial_impulse.j * mass +
			particle->translational_velocity.j;
		particle->translational_velocity.k = data->initial_impulse.k * mass +
			particle->translational_velocity.k;
	}

	particle->angular_velocity = data->angular_velocity;

	if (!TEST_FLAG(definition->flags, _particle_definition_self_illuminated_bit) ||
		TEST_FLAG(definition->flags, _particle_definition_tint_from_diffuse_texture_bit))
	{
		real_rgb_color light;
		real_rgb_color diffuse;

		light_particle(&position, &light, &diffuse, FALSE);

		match_assert_valid_real_rgb_color(
			"c:\\halo\\SOURCE\\effects\\particles.c",
			207,
			&light);
		match_assert_valid_real_rgb_color(
			"c:\\halo\\SOURCE\\effects\\particles.c",
			208,
			&diffuse);

		if (!TEST_FLAG(definition->flags, _particle_definition_self_illuminated_bit))
		{
			particle->color.rgb.red *= light.red;
			particle->color.rgb.green *= light.green;
			particle->color.rgb.blue *= light.blue;
		}

		if (TEST_FLAG(definition->flags, _particle_definition_tint_from_diffuse_texture_bit))
		{
			particle->color.rgb.red *= diffuse.red;
			particle->color.rgb.green *= diffuse.green;
			particle->color.rgb.blue *= diffuse.blue;
		}
	}

	if (particle_next_sequence(particle_index))
	{
		struct bitmap_group *bitmap = bitmap_group_get(definition->bitmap.index);
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap->sequences,
			particle->sequence_index,
			struct bitmap_group_sequence);

		if (TEST_FLAG(definition->flags,
			_particle_definition_animation_starts_on_random_frame_bit))
		{
			particle->frame_index = (short)((TEST_FLAG(particle->flags,
					_particle_datum_animates_backwards_bit) ? 1 : NONE) +
				local_random_range(0, (short)sequence->sprites.count));
		}
		else if (TEST_FLAG(particle->flags, _particle_datum_animates_backwards_bit))
		{
			particle->frame_index = (short)sequence->sprites.count;
		}
		else
		{
			particle->frame_index = NONE;
		}
	}

	return;
}

void particles_update(
	real dt)
{
	long particle_index;

#ifdef HALO_N64
	/* (the N64's own: port/n64/src/n64_particles.c) */
	{
		void n64_particles_update(float dt);

		n64_particles_update(dt);
		return;
	}
#endif
	profile_enter(particles_update_section);

#ifdef HALO_LINUX
	{
		long tick;

		particles_leftover_ticks += dt * TICKS_PER_SECOND;
		particles_update_ticks = (long)particles_leftover_ticks;
		particles_leftover_ticks -= (real)particles_update_ticks;
		for (tick = 0;
			tick < MIN(particles_update_ticks, (long)NUMBEROF(particles_tick_frame_indices));
			tick++)
		{
			particles_tick_frame_head =
				(particles_tick_frame_head + 1) % NUMBEROF(particles_tick_frame_indices);
			particles_tick_frame_indices[particles_tick_frame_head] = render.frame_index;
		}
	}

#endif
	for (particle_index = data_next_index(particle_data, NONE);
		particle_index != NONE;
		particle_index = data_next_index(particle_data, particle_index))
	{
		struct particle_datum *particle = particle_get(particle_index);
		struct particle_definition *definition = particle_definition_get(particle->definition_index);
		boolean was_new = (particle->age == 0.0f);

#ifdef HALO_LINUX
		if (render.frame_index - particle->last_rendered_frame_index >
			MAXIMUM_PARTICLE_UNRENDERED_FRAME_COUNT &&
			particle->last_rendered_frame_index < particles_tick_frame_indices[
				(particles_tick_frame_head + 1) % NUMBEROF(particles_tick_frame_indices)])
#else
		if (render.frame_index - particle->last_rendered_frame_index >
			MAXIMUM_PARTICLE_UNRENDERED_FRAME_COUNT)
#endif
		{
			datum_delete(particle_data, particle_index);
		}
		else
		{
			particle->age = dt + particle->age;

			if (particle->age >= particle->lifespan &&
				!was_new &&
				!definition->final_sequence_count)
			{
				particle_die(particle_index);
			}
			else
			{
				if (particle_update_frame_time(particle_index, dt))
					particle_update_physics(particle_index, dt);
			}
		}
	}

	profile_exit(particles_update_section);

	return;
}

/* ---------- private code */
