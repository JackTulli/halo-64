/*
ITEMS.C

symbols in this file:
000E60A0 0020:
	_object_get_type (0000)
000E60C0 0010:
	_items_initialize (0000)
000E60D0 0010:
	_items_initialize_for_new_map (0000)
000E60E0 0010:
	_items_dispose_from_old_map (0000)
000E60F0 0010:
	_items_dispose (0000)
000E6100 0040:
	_item_new (0000)
000E6140 0010:
	_item_delete (0000)
000E6150 0010:
	_verify_item_location (0000)
000E6160 0050:
	_dangerous_items_near_player (0000)
000E61B0 00a0:
	_item_in_unit_inventory (0000)
000E6250 0090:
	_item_get_position_even_if_in_inventory (0000)
000E62E0 0090:
	_item_detonate (0000)
000E6370 00c0:
	_item_adjust_for_angular_velocity_change (0000)
000E6430 00c0:
	_valid_real_vector3d_axes3 (0000)
000E64F0 0060:
	_valid_real_matrix4x3 (0000)
000E6550 03b0:
	_item_accelerate (0000)
000E6900 0230:
	_item_align_to_normal_and_point (0000)
000E6B30 0930:
	_item_update (0000)
00278FB0 0004:
	_item_maximum_impact_velocity (0000)
00278FB4 000c:
	??_C@_0M@HIOGOKBN@item_update?$AA@ (0000)
00278FC0 000d:
	??_C@_0N@MIPPJENF@ground?5point?$AA@ (0000)
00278FD0 001d:
	??_C@_0BN@FFDGPJIN@c?3?2halo?2SOURCE?2items?2items?4c?$AA@ (0000)
00278FF0 002b:
	??_C@_0CL@EEILLIKO@valid_real_matrix4x3?$CI?$CGground_poi@ (0000)
0027901C 0008:
	??_C@_07GAHNFPHG@scale?$DO0?$AA@ (0000)
00279024 0004:
	__real@3f350481 (0000)
00306498 05f8:
	_item_update_section (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "cseries/profile.h"
#include "effects/material_effects.h"
#include "game/game.h"
#include "effects/effects.h"
#include "game/players.h"
#include "game/game_engine.h"
#include "items/item_definitions.h"
#include "items.h"
#include "physics/breakable_surfaces.h"
#include "physics/collision_bsp_definitions.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "physics/physics.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	_item_definition_always_maintains_z_up_bit = 0,
	_item_definition_destroyed_by_explosions_bit,
	_item_definition_antigravity_bit,
};

enum
{
	_collision_surface_breakable_bit = 3,
};

enum
{
	ITEM_UPDATE_COLLISION_TEST_FLAGS = 0x1FF3E9,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

struct profile_section item_update_section = { "item_update", NONE, TRUE };
real const item_maximum_impact_velocity = 0.1f;

/* ---------- public code */

void items_initialize(
	void)
{
	return;
}

void items_initialize_for_new_map(
	void)
{
	return;
}

void items_dispose_from_old_map(
	void)
{
	return;
}

void items_dispose(
	void)
{
	return;
}

void item_delete(
	long item_index)
{
	return;
}

void verify_item_location(
	void)
{
	return;
}

boolean item_new(
	long item_index)
{
	struct item_datum *item = item_get(item_index);

	item->object.flags |= FLAG(_object_dynamic_lighting_recompute_bit) |
		FLAG(_object_static_lighting_recompute_bit);
	item->item.last_owned_time = game_time_get();
	item->item.ignore_object_index = NONE;

	return TRUE;
}

boolean dangerous_items_near_player(
	void)
{
	struct object_iterator iterator;
	struct item_datum *item;

	object_iterator_new(&iterator, _object_mask_item, 1);
	while (item = (struct item_datum *)object_iterator_next(&iterator))
	{
		if (item->item.detonation_ticks > 0)
			return TRUE;
	}

	return FALSE;
}

void item_in_unit_inventory(
	long item_index,
	long owner_unit_index)
{
	struct item_datum *item = item_get(item_index);

	if (owner_unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(owner_unit_index);

		item->item.flags |= FLAG(_item_attached_to_unit_bit);
		if (unit->unit.player_index != NONE)
		{
			item->item.flags |= FLAG(_item_belongs_to_player_bit);
		}
		else
		{
			item->item.flags &= ~FLAG(_item_belongs_to_player_bit);
		}

		item->object.owner_player_index = unit->unit.player_index;
		object_set_garbage(item_index, FALSE);
		item->item.flags &=
			~(FLAG(_item_on_structure_bit) | FLAG(_item_does_not_accelerate_bit));
		item->object.location.leaf_index = NONE;
		item->object.location.cluster_index = NONE;
		scenario_location_award_bonus(&item->object.location);

		return;
	}

	item->item.flags &=
		~(FLAG(_item_attached_to_unit_bit) | FLAG(_item_belongs_to_player_bit));

	return;
}

void item_get_position_even_if_in_inventory(
	long item_index,
	real_point3d *position)
{
	struct item_datum *item = item_try_and_get(item_index);

	position->x = 0.f;
	position->y = 0.f;
	position->z = 0.f;

	if (!item)
	{
		return;
	}

	if (TEST_FLAG(item->item.flags, _item_attached_to_unit_bit))
	{
		long player_index = item->object.owner_player_index;
		struct player_datum *player;
		struct unit_datum *unit;

		if (player_index == NONE)
		{
			return;
		}

		player = player_get(player_index);
		if (player->unit_index == NONE)
		{
			return;
		}

		unit = unit_get(player->unit_index);
		*position = unit->object.bounding_sphere_center;

		return;
	}

	*position = item->object.bounding_sphere_center;

	return;
}

void item_detonate(
	long item_index)
{
	struct item_datum *item = item_get(item_index);
	struct item_definition *definition = item_definition_get(item->definition_index);

	if (!item->item.detonation_ticks)
	{
		effect_new_from_object(
			definition->item.detonating_effect.index,
			item_index,
			item_index,
			NONE,
			0.f,
			0.f,
			NULL,
			NULL);
		item->item.detonation_ticks =
			(short)(real_random_range(
				definition->item.detonation_delay_timer_lower_bound,
				definition->item.detonation_delay_timer_upper_bound) * TICKS_PER_SECOND);
	}

	return;
}

/* ---------- private code */

static void item_adjust_for_angular_velocity_change(
	long item_index)
{
	struct item_datum *item = item_get(item_index);
	real angular_velocity_magnitude = magnitude3d(&item->object.angular_velocity);

	if (angular_velocity_magnitude != 0.f)
	{
		item->item.flags |= FLAG(_item_has_nonzero_angular_velocity_bit);
		if (!TEST_FLAG(item->object.flags, _object_at_rest_bit))
		{
			scale_vector3d(
				&item->object.angular_velocity,
				1.f / angular_velocity_magnitude,
				&item->item.rotation_axis);
		}

		item->item.rotation_sine = sine(angular_velocity_magnitude);
		item->item.rotation_cosine = cosine(angular_velocity_magnitude);
	}
	else
	{
		item->item.flags &= ~FLAG(_item_has_nonzero_angular_velocity_bit);
		item->item.rotation_sine = 0.f;
		item->item.rotation_cosine = 1.f;
	}

	return;
}

void item_accelerate(
	long item_index,
	real_vector3d const *acceleration,
	boolean detonates_explosives)
{
	struct item_datum *item = item_get(item_index);
	struct item_definition *definition = item_definition_get(item->definition_index);

	if (TEST_FLAG(item->item.flags, _item_does_not_accelerate_bit))
	{
		return;
	}

	match_assert(
		"c:\\halo\\SOURCE\\items\\items.c",
		536,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] =
		_collision_user_items;

	if (item->object.parent_object_index == NONE)
	{
	if (detonates_explosives &&
		!game_engine_running() &&
		TEST_FLAG(definition->item.flags, 1))
	{
		item_detonate(item_index);
	}

	if (TEST_FLAG(item->item.flags, _item_on_structure_bit))
	{
		if (!(magnitude_squared3d(acceleration) < 0.0001f))
		{
			struct object_marker marker;

			if (object_get_marker_by_name(item_index, "ground point", &marker, 1))
			{
				struct collision_bsp *collision_bsp;
				struct collision_surface const *surface;
				real_point3d new_position;
				real_plane3d plane;
				real distance_above;

				collision_bsp = global_collision_bsp_get();
				surface = TAG_BLOCK_GET_ELEMENT(
					&collision_bsp->surfaces,
					item->item.rested_surface_index,
					struct collision_surface);
				bsp3d_get_plane_from_designator(
					&collision_bsp->bsp3d,
					surface->plane_designator,
					&plane);

				distance_above =
					0.05f - (plane3d_distance_to_point(&plane, &marker.matrix.position));
				point_from_line3d(
					&marker.matrix.position,
					&plane.n,
					distance_above,
					&new_position);
				object_translate(item_index, &new_position, NULL);
			}

			item->object.flags &= ~FLAG(_object_at_rest_bit);
			item->item.flags &= ~FLAG(_item_on_structure_bit);
		}
	}
	else
	{
		item->object.flags &= ~FLAG(_object_at_rest_bit);
	}

	add_vectors3d(
		&item->object.translational_velocity,
		acceleration,
		&item->object.translational_velocity);

	if (item->item.ignore_object_index == NONE &&
		TEST_FLAG(item->item.flags, _item_on_structure_bit) &&
		magnitude_squared3d(acceleration) < 0.0001f)
	{
		struct object_marker marker;
		real_vector3d rotation_axis;
		real_vector3d scaled_rotation_axis;
		real rotation_magnitude;

		if (object_get_marker_by_name(item_index, "ground point", &marker, 1))
		{
			rotation_axis = marker.matrix.up;
		}
		else
		{
			rotation_axis = *global_up3d;
		}

		rotation_magnitude = real_random_range(-1.5707964f, 1.5707964f);
		scale_vector3d(
			&rotation_axis,
			rotation_magnitude,
			&scaled_rotation_axis);
		add_vectors3d(
			&item->object.angular_velocity,
			&scaled_rotation_axis,
			&item->object.angular_velocity);
	}
	else
	{
		real acceleration_magnitude = magnitude3d(acceleration);
		real_vector3d rotation_axis;
		double rotation_magnitude;

		if (acceleration_magnitude < 0.0001f)
		{
			acceleration_magnitude =
				real_seed_random(get_global_random_seed_address());
		}

		cross_product3d(global_up3d, acceleration, &rotation_axis);
		if (!(normalize3d(&rotation_axis) > 0.f))
		{
			seed_random_direction3d(
				get_global_random_seed_address(),
				&rotation_axis);
		}

		rotation_magnitude =
			real_seed_random(get_global_random_seed_address()) *
			acceleration_magnitude * 1.5707964f;
		scale_vector3d(&rotation_axis, (real)rotation_magnitude, &rotation_axis);
		add_vectors3d(
			&item->object.angular_velocity,
			&rotation_axis,
			&item->object.angular_velocity);
	}

	item_adjust_for_angular_velocity_change(item_index);
	object_set_garbage(item_index, FALSE);
	}

	match_assert(
		"c:\\halo\\SOURCE\\items\\items.c",
		651,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return;
}

static void item_align_to_normal_and_point(
	long item_index,
	real_vector3d const *normal,
	real_point3d const *position,
	real_point3d *new_position)
{
	struct item_datum *item = item_get(item_index);
	struct object_marker marker;
	real_vector3d new_forward;
	real_matrix4x3 ground_point_matrix;
	real scale;

	match_assert("c:\\halo\\SOURCE\\items\\items.c", 713, normal);
	if (!object_get_marker_by_name(item_index, "ground point", &marker, 1))
	{
		return;
	}

	if (!position)
	{
		position = &marker.matrix.position;
	}
	if (!new_position)
	{
		new_position = &marker.matrix.position;
	}

	scale = square_root(
		2.f * (dot_product3d(&marker.matrix.up, normal) + 1.f));
	if (scale > 0.01)
	{
		real_quaternion rotation;
		real inverse_scale;

		match_assert("c:\\halo\\SOURCE\\items\\items.c", 732, scale>0);
		cross_product3d(&marker.matrix.up, normal, &rotation.v);
		inverse_scale = 1.f / scale;
		scale_vector3d(&rotation.v, inverse_scale, &rotation.v);
		rotation.w = scale * 0.5f;
		quaternion_transform_point(
			&rotation,
			(real_point3d const *)&marker.matrix.forward,
			(real_point3d *)&new_forward);
	}
	else
	{
		real_vector3d cross;

		cross_product3d(normal, &marker.matrix.forward, &cross);
		cross_product3d(&cross, normal, &new_forward);
	}

	normalize3d(&new_forward);
	matrix4x3_from_point_and_vectors(
		&ground_point_matrix,
		position,
		&new_forward,
		normal);
	match_assert(
		"c:\\halo\\SOURCE\\items\\items.c",
		755,
		valid_real_matrix4x3(&ground_point_matrix));
	object_align_marker_to_matrix(
		object_get(item_index),
		&marker,
		&ground_point_matrix);
	*new_position = item->object.position;

	return;
}

boolean item_update(
	long item_index)
{
	struct item_datum *item;
	struct item_definition *definition;

	item = item_get(item_index);
	definition = item_definition_get(item->definition_index);
	profile_enter(item_update_section);

	match_assert(
		"c:\\halo\\SOURCE\\items\\items.c",
		174,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] =
		_collision_user_items;

	if (TEST_FLAG(item->object.flags, _object_connected_to_map_bit) &&
		item->object.parent_object_index == NONE)
	{
		if (TEST_FLAG(
				definition->item.flags,
				_item_definition_always_maintains_z_up_bit) &&
			!(fabs(item->object.up.k - 1.f) < 0.0001f))
		{
			real_vector3d left;

			item->object.up = *global_up3d;
			cross_product3d(
				&item->object.up,
				&item->object.forward,
				&left);
			cross_product3d(
				&left,
				&item->object.up,
				&item->object.forward);
			if (normalize3d(&item->object.forward) == 0.f)
			{
				item->object.forward = *global_forward3d;
			}
		}

		if (!TEST_FLAG(item->object.flags, _object_at_rest_bit))
		{
			struct collision_result collision;
			real_vector3d new_velocity;
			real_point3d new_position;

			new_velocity = item->object.translational_velocity;
			if (!TEST_FLAG(
					definition->item.flags,
					_item_definition_antigravity_bit))
			{
				new_velocity.k -= global_gravity;
			}

			new_position.x = item->object.position.x + new_velocity.i;
			new_position.y = item->object.position.y + new_velocity.j;
			new_position.z = item->object.position.z + new_velocity.k;

			if (collision_test_line(
					ITEM_UPDATE_COLLISION_TEST_FLAGS,
					&item->object.position,
					&new_position,
					item->item.ignore_object_index,
					&collision))
			{
				long impulse_sound_index;
				real impact_scale;

				point_from_line3d(
					&new_position,
					&collision.plane.n,
					0.05f,
					&new_position);

				impact_scale = magnitude3d(&new_velocity);
				impact_scale /= item_maximum_impact_velocity;
				if (impact_scale < 0.f)
				{
					impact_scale = 0.f;
				}
				else if (impact_scale > 1.f)
				{
					impact_scale = 1.f;
				}

				if (definition->item.material_effects.index != NONE &&
					material_effect_visible(&collision.point))
				{
					material_effect_new(
						definition->item.material_effects.index,
						8,
						collision.material_type,
						&collision.point,
						&collision.plane.n,
						&collision.location,
						impact_scale);
				}

				impulse_sound_index = definition->item.collision_sound.index;
				if (impulse_sound_index != NONE)
				{
					struct sound_location location;

					location.position = new_position;
					location.forward = collision.plane.n;
					location.translational_velocity = *global_zero_vector3d;
					location.game_location = item->object.location;
					unattached_impulse_sound_new(
						impulse_sound_index,
						&location,
						impact_scale);
				}

				if ((collision.type == _collision_result_structure ||
					(collision.type == _collision_result_object &&
						TEST_FLAG(
							_object_mask_scenery | _object_mask_device,
							object_get_type(collision.object_index)))) &&
					collision.plane.n.k > 0.7071f &&
					-dot_product3d(&collision.plane.n, &new_velocity) < 0.05f)
				{
					new_position = collision.point;
					item_align_to_normal_and_point(
						item_index,
						&collision.plane.n,
						&collision.point,
						&new_position);
					new_velocity.i = new_velocity.j = new_velocity.k = 0.f;
					scale_vector3d(
						&collision.plane.n,
						dot_product3d(
							&item->object.angular_velocity,
							&collision.plane.n),
						&item->object.angular_velocity);

					if (!game_engine_running() &&
						item->object.owner_player_index == NONE)
					{
						object_set_garbage(item_index, TRUE);
					}

					SET_FLAG(item->object.flags, _object_at_rest_bit, TRUE);
					switch (collision.type)
					{
					case _collision_result_structure:
						SET_FLAG(item->item.flags, _item_on_structure_bit, TRUE);
						item->item.rested_surface_index =
							(short)collision.surface_index;
						item->item.bsp_index =
							global_structure_bsp_index_get();
						break;

					case _collision_result_object:
					{
						real_matrix4x3 const *node_matrix;

						SET_FLAG(item->item.flags, _item_on_object_bit, TRUE);
						item->item.item_on_rest_object_index =
							collision.object_index;
						node_matrix = object_get_node_matrix(
							collision.object_index,
							0);
						matrix4x3_inverse_transform_point(
							node_matrix,
							&collision.point,
							&item->item.item_rest_object_offset);
						break;
					}

					default:
						match_assert(
							"c:\\halo\\SOURCE\\items\\items.c",
							283,
							!"unreachable");
						break;
					}

					item->item.rotation_axis = collision.plane.n;
					item_adjust_for_angular_velocity_change(item_index);
					item->item.ignore_object_index = NONE;
				}
				else
				{
					real reflection;

					reflection = -1.4f * dot_product3d(
						&collision.plane.n,
						&new_velocity);
					if (collision.type != _collision_result_structure)
					{
						reflection = MIN(1.5f, reflection);
					}
					new_position = collision.point;
					new_velocity.i += collision.plane.n.i * reflection;
					new_velocity.j += collision.plane.n.j * reflection;
					new_velocity.k += collision.plane.n.k * reflection;
					if (collision_test_point(
							ITEM_UPDATE_COLLISION_TEST_FLAGS,
							&new_position,
							item_index))
					{
						point_from_line3d(
							&collision.point,
							&collision.plane.n,
							0.05f,
							&new_position);
					}
					collision_test_point(
						ITEM_UPDATE_COLLISION_TEST_FLAGS,
						&new_position,
						item_index);
				}
			}

			item->object.translational_velocity = new_velocity;
			object_translate(
				item_index,
				&new_position,
				&collision.location);
		}
		else if (!TEST_FLAG(
			definition->item.flags,
			_item_definition_antigravity_bit))
		{
#ifndef HALO_N64
			/* (its answer is never used; the N64 skips the search) */
			struct object_marker marker;

			object_get_marker_by_name(
				item_index,
				"ground point",
				&marker,
				1);
#endif

			if (TEST_FLAG(item->item.flags, _item_on_structure_bit) &&
				item->item.rested_surface_index != NONE &&
				item->item.bsp_index == global_structure_bsp_index_get())
			{
				struct collision_bsp *collision_bsp;
				struct collision_surface const *surface;

				collision_bsp = global_collision_bsp_get();
				surface = TAG_BLOCK_GET_ELEMENT(
					&collision_bsp->surfaces,
					item->item.rested_surface_index,
					struct collision_surface);
				if (TEST_FLAG(surface->flags, _collision_surface_breakable_bit) &&
					!breakable_surface_extant(
						surface->breakable_surface_index))
				{
					real_vector3d gravity;

					scale_vector3d(
						global_down3d,
						global_gravity,
						&gravity);
					SET_FLAG(item->item.flags, _item_on_structure_bit, FALSE);
					item->item.rested_surface_index = NONE;
					item_accelerate(
						item_index,
						&gravity,
						FALSE);
				}
			}
			else if (TEST_FLAG(item->item.flags, _item_on_object_bit))
			{
				if (object_try_and_get(item->item.item_on_rest_object_index))
				{
					real_matrix4x3 const *node_matrix;
					real_point3d origin;

					node_matrix = object_get_node_matrix(
						item->item.item_on_rest_object_index,
						0);
					matrix4x3_transform_point(
						node_matrix,
						&item->item.item_rest_object_offset,
						&origin);
					item_align_to_normal_and_point(
						item_index,
						&item->item.rotation_axis,
						&origin,
						NULL);
				}
				else
				{
					real_vector3d gravity;

					scale_vector3d(
						global_down3d,
						global_gravity,
						&gravity);
					SET_FLAG(item->item.flags, _item_on_object_bit, FALSE);
					item_accelerate(
						item_index,
						&gravity,
						FALSE);
				}
			}

			item->object.angular_velocity.i *= 0.9f;
			item->object.angular_velocity.j *= 0.9f;
			item->object.angular_velocity.k *= 0.9f;
			item_adjust_for_angular_velocity_change(item_index);
		}

		if (TEST_FLAG(
				item->item.flags,
				_item_has_nonzero_angular_velocity_bit))
		{
			struct object_marker marker;
			real_vector3d left;

			if (!TEST_FLAG(item->object.flags, _object_at_rest_bit) ||
				!object_get_marker_by_name(
					item_index,
					"ground point",
					&marker,
					1))
			{
				rotate_vector_about_axis(
					&item->object.forward,
					&item->item.rotation_axis,
					item->item.rotation_sine,
					item->item.rotation_cosine);
				rotate_vector_about_axis(
					&item->object.up,
					&item->item.rotation_axis,
					item->item.rotation_sine,
					item->item.rotation_cosine);
			}
			else
			{
				real_matrix4x3 ground_point_matrix;

				ground_point_matrix = marker.matrix;
				rotate_vector_about_axis(
					&ground_point_matrix.forward,
					&item->item.rotation_axis,
					item->item.rotation_sine,
					item->item.rotation_cosine);
				rotate_vector_about_axis(
					&ground_point_matrix.up,
					&item->item.rotation_axis,
					item->item.rotation_sine,
					item->item.rotation_cosine);
				cross_product3d(
					&ground_point_matrix.up,
					&ground_point_matrix.forward,
					&ground_point_matrix.left);
				cross_product3d(
					&ground_point_matrix.left,
					&ground_point_matrix.up,
					&ground_point_matrix.forward);
				normalize3d(&ground_point_matrix.forward);
				normalize3d(&ground_point_matrix.left);
				normalize3d(&ground_point_matrix.up);
				object_align_marker_to_matrix(
					object_get(item_index),
					&marker,
					&ground_point_matrix);
			}

			normalize3d(&item->object.up);
			cross_product3d(
				&item->object.up,
				&item->object.forward,
				&left);
			cross_product3d(
				&left,
				&item->object.up,
				&item->object.forward);
			normalize3d(&item->object.forward);
		}
	}

	if (item->item.detonation_ticks > 0)
	{
		--item->item.detonation_ticks;
		if (item->item.detonation_ticks == 0)
		{
			effect_new_from_object(
				definition->item.detonation_effect.index,
				item_index,
				item_index,
				NONE,
				0.f,
				0.f,
				NULL,
				NULL);
			object_delete(item_index);
		}
	}

	if (TEST_FLAG(item->item.flags, _item_attached_to_unit_bit))
	{
		item->item.last_owned_time = game_time_get();
	}

	match_assert(
		"c:\\halo\\SOURCE\\items\\items.c",
		468,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;
	profile_exit(item_update_section);

	return TRUE;
}
