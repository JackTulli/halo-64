/*
VEHICLES.C

symbols in this file:
001A4E30 0010:
	_vehicles_initialize (0000)
001A4E40 0010:
	_vehicles_initialize_for_new_map (0000)
001A4E50 0010:
	_vehicles_dispose_from_old_map (0000)
001A4E60 0010:
	_vehicles_dispose (0000)
001A4E70 0030:
	_vehicle_place (0000)
001A4EA0 0010:
	_vehicle_delete (0000)
001A4EB0 0030:
	_vehicle_causes_collision_damage (0000)
001A4EE0 0010:
	_vehicle_build_update (0000)
001A4EF0 0010:
	_vehicle_parse_update (0000)
001A4F00 0050:
	_vehicle_hover (0000)
001A4F50 0020:
	_code_001a4f50 (0000)
001A4F70 0030:
	_vehicle_is_flipped (0000)
001A4FA0 00a0:
	_code_001a4fa0 (0000)
001A5040 0020:
	_set_real_quaternion (0000)
001A5060 00b0:
	_vehicle_reset (0000)
001A5110 0070:
	_vehicle_new (0000)
001A5180 0400:
	_vehicle_preprocess_node_orientations (0000)
001A5580 0100:
	_vehicle_accelerate (0000)
001A5680 0060:
	_vehicle_render_debug (0000)
001A56E0 0130:
	_vehicle_find_pathfinding_surface_index (0000)
001A5810 00d0:
	_code_001a5810 (0000)
001A58E0 0150:
	_code_001a58e0 (0000)
001A5A30 0110:
	_code_001a5a30 (0000)
001A5B40 0310:
	_code_001a5b40 (0000)
001A5E50 0440:
	_code_001a5e50 (0000)
001A6290 0300:
	_code_001a6290 (0000)
001A6590 0180:
	_code_001a6590 (0000)
001A6710 0200:
	_code_001a6710 (0000)
001A6910 0290:
	_code_001a6910 (0000)
001A6BA0 0220:
	_code_001a6ba0 (0000)
001A6DC0 0320:
	_code_001a6dc0 (0000)
001A70E0 01d0:
	_code_001a70e0 (0000)
001A72B0 0520:
	_vehicle_export_function_values (0000)
001A77D0 0180:
	_vehicle_moving_near_any_player (0000)
001A7950 0170:
	_vehicle_stuck (0000)
001A7AC0 03a0:
	_code_001a7ac0 (0000)
001A7E60 09a0:
	_code_001a7e60 (0000)
001A8800 0070:
	_code_001a8800 (0000)
001A8870 0910:
	_vehicle_update (0000)
002A6200 000f:
	??_C@_0P@GMMIJBCG@vehicle_update?$AA@ (0000)
002A6210 0004:
	__real@3c64c389 (0000)
002A6214 0004:
	__real@bc64c389 (0000)
002A6218 0008:
	__real@3f9c987120000000 (0000)
002A6220 0004:
	__real@3e088889 (0000)
002A6224 0004:
	__real@3f866666 (0000)
002A6228 0004:
	__real@3f55c28f (0000)
002A622C 000a:
	??_C@_09IPNEIKLN@reflected?$AA@ (0000)
002A6238 000e:
	??_C@_0O@FEOFEIPO@jet?5thrusters?$AA@ (0000)
002A6248 0010:
	??_C@_0BA@JGHLOGHL@hover?5thrusters?$AA@ (0000)
002A6258 0009:
	??_C@_08FLPDFBMI@midpoint?$AA@ (0000)
002A6264 0004:
	__real@4235d174 (0000)
002A6268 0004:
	__real@3fd55556 (0000)
002A626C 0004:
	__real@4091745d (0000)
002A6270 0004:
	__real@3f969697 (0000)
002A6274 0004:
	__real@bd4ccccd (0000)
002A6278 0004:
	__real@3b03126f (0000)
002A627C 0004:
	__real@3b83126f (0000)
002A6280 0004:
	__real@bbbea2f1 (0000)
002A6284 0004:
	__real@3b7e2e97 (0000)
002A6288 0004:
	__real@3acb5879 (0000)
002A628C 0004:
	__real@3b64c389 (0000)
002A6290 0004:
	__real@bb64c389 (0000)
002A6298 0008:
	__real@3f7c987120000000 (0000)
002A62A0 0006:
	??_C@_05KCCEOPLE@?$HOblur?$AA@ (0000)
002A62A8 0020:
	??_C@_0CA@NBAFPFDO@c?3?2halo?2SOURCE?2units?2vehicles?4c?$AA@ (0000)
002A62C8 0008:
	__real@3fd3333333333333 (0000)
002A62D0 0008:
	__real@bfd3333333333333 (0000)
00316228 05f8:
	_data_00316228 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/profile.h"
#include "vehicles.h"
#include "math/real_math.h"
#include "game/game_globals.h"
#include "game/players.h"
#include "effects/material_effects.h"
#include "effects/effects.h"
#include "models/model_animation_definitions.h"
#include "objects/damage.h"
#include "objects/objects.h"
#include "physics/collision_bsp.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "physics/physics.h"
#include "physics/physics_definitions.h"
#include "physics/physics_variables.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "structures/structure_bsp_definitions.h"
#include "units/unit_definitions.h"
#include "units/vehicle_definitions.h"

/* ---------- constants */

enum
{
	_vehicle_type_human_tank = 0,
	_vehicle_type_human_jeep,
	_vehicle_type_human_boat,
	_vehicle_type_human_plane,
	_vehicle_type_alien_scout,
	_vehicle_type_alien_fighter,
	_vehicle_type_turret,
	NUMBER_OF_VEHICLE_TYPES
};

/* ---------- macros */

/* 0.8 degrees in radians. Spelled as a float literal because MSVC folds
constant expressions in double and only rounds at the final assignment, so
((real)(0.8*_pi/180)) yields 0x3c64c388 where January has 0x3c64c389 -- and
2.0*it then differs in the double constant too. */
#define VEHICLE_ANGULAR_ACCELERATION 0.0139626344f

/* ---------- structures */

struct vehicle_definition
{
	struct unit_definition unit;
	unsigned long flags;
	short vehicle_type;
	short pad2f6;
	/* 0x2f8 is a four-real 'speed' block and 0x308 a two-real 'turn' block;
	their sub-field names are not recovered, so they keep offset names. */
	real unknown2f8;
	real unknown2fc;
	real unknown300;
	real unknown304;
	real unknown308;
	real unknown30c;
	real wheel_circumference;
	real unknown314;
	real unknown318;
	short function_modes[4];
	byte unknown324[0xc];
	real unknown330;
	real unknown334;
	byte unused338[8];
	real unknown340;
	real unknown344;
	byte unused348[0x1c];
	real unknown364;
	byte unknown368[0x48];
	struct tag_reference suspension_sound;
	struct tag_reference crash_sound;
	struct tag_reference material_effects;
	struct tag_reference effect;
};

struct game_globals_falling_damage
{
	byte unused0[0x2c];
	struct tag_reference maximum_distance_damage;
	struct tag_reference vehicle_hit_environment_damage_effect;
	struct tag_reference vehicle_killed_unit_damage_effect;
	struct tag_reference vehicle_collision_damage;
	struct tag_reference flaming_death_damage;
	long unused7c[4];
	real runtime_maximum_falling_velocity;
	real runtime_minimum_damage_velocity;
	real runtime_maximum_damage_velocity;
};

#include "vehicle_datum.h"

struct physics_mass_point_definition
{
	byte unused0[0x20];
	short powered_mass_point_index;
	short pad22;
	unsigned long flags;
	byte unused28[0x10];
	real_point3d position;
	byte unused44[0xc];
	real_vector3d normal;
	byte unused5c[0xc];
	real unknown68;
	byte unused6c[0x14];
};

struct vehicle_animation
{
	byte unused0[0x5c];
	struct tag_block animations;
	struct tag_block suspensions;
};

struct vehicle_suspension
{
	short mass_point_index;
	short animation_index;
	real unknown4;
	real unknown8;
	byte unknownc[8];
};

#include "physics/mass_point_datum.h"

#include "physics/powered_mass_point_datum.h"

struct scenario_object_permutation;
struct scenario_unit;

struct scenario_vehicle
{
	byte unused0[0x28];
	byte permutation[0x20];
	byte unit[0x4];
};

/* ---------- prototypes */

void aiming_screen_apply(
	struct animation const *animation,
	struct vehicle_animation const *vehicle_animation,
	real yaw,
	real pitch,
	struct real_orientation *node_orientations);
short unit_update_animation(
	long unit_index,
	struct unit_animation_update_data *update_data);

/* NOTE: code_001a5e50 and code_001a6290 are file statics in January, but they
are not reconstructed yet. They are declared here rather than defined so that
code_001a8800 can call them: January passes their arguments on the stack, and a
declared-but-undefined static would give MSVC a body to inspect and a register
convention to invent. The relocation carries the name either way, which is what
the comparator checks. */


extern boolean debug_objects_vehicle_powered_mass_points;
extern real global_gravity;

/* ---------- globals */

static struct profile_section vehicle_update_section = {"vehicle_update", NONE, TRUE};

/* ---------- code */

void vehicles_initialize(
	void)
{
	return;
}

void vehicles_initialize_for_new_map(
	void)
{
	return;
}

void vehicles_dispose_from_old_map(
	void)
{
	return;
}

void vehicles_dispose(
	void)
{
	return;
}

void vehicle_place(
	long vehicle_index,
	struct scenario_vehicle *vehicle)
{
	unit_place(vehicle_index, (struct scenario_unit_datum const *)vehicle->unit);
	object_add_scenario_permutation(vehicle_index, (struct scenario_object_permutation *)vehicle->permutation);

	return;
}

void vehicle_delete(
	long vehicle_index)
{
	return;
}

boolean vehicle_causes_collision_damage(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);

	return TEST_FLAG(definition->flags, 7);
}

boolean vehicle_build_update(
	long vehicle_index,
	void *update)
{
	return TRUE;
}

boolean vehicle_parse_update(
	long vehicle_index,
	void *update)
{
	return TRUE;
}

void vehicle_hover(
	long vehicle_index,
	boolean hover)
{
	if (vehicle_index!=NONE)
	{
		struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);

		if (hover)
		{
			object_get_origin(vehicle_index, &vehicle->vehicle.hover_position);
			vehicle->vehicle.flags |= FLAG(1);
		}
		else
		{
			vehicle->vehicle.flags &= ~FLAG(1);
		}
	}

	return;
}

boolean vehicle_is_flipped(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);

	return vehicle->object.up.k<0.2f;
}

void vehicle_reset(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);

	vehicle->vehicle.flags = 0;
	vehicle->vehicle.stop_time = 0;
	vehicle->vehicle.airborne_ticks = 0;
	vehicle->vehicle.upending_type = 0;
	vehicle->vehicle.upending_ticks = 0;
	vehicle->vehicle.on_ground_ticks = 0;
	vehicle->vehicle.speed = 0;
	vehicle->vehicle.slide = 0;
	vehicle->vehicle.turn = 0;
	vehicle->vehicle.wheel = 0;
	vehicle->vehicle.left_tread = 0;
	vehicle->vehicle.right_tread = 0;
	vehicle->vehicle.thrust = 0;
	vehicle->vehicle.hover = 0;
	csmemset(vehicle->vehicle.suspension, 0, sizeof(vehicle->vehicle.suspension));
	set_real_quaternion(&vehicle->vehicle.collision_accumulator_head, 0.0f, 0.0f, 0.0f, 0.0f);
	set_real_point2d(&vehicle->vehicle.collision_accumulator_tail, 0.0f, 0.0f);
	vehicle->vehicle.stuck_mass_point_flags = 0;

	return;
}

boolean vehicle_new(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct unit_definition *definition = vehicle_definition_get(vehicle->definition_index);

	vehicle_reset(vehicle_index);

	if (definition->object.physics.index==NONE)
		vehicle->object.flags |= FLAG(5);
	else
		vehicle->object.flags &= ~FLAG(5);

	if (definition->object.physics.index!=NONE)
		vehicle->object.position.z += definition->object.bounding_radius*0.5f;

	return TRUE;
}

void vehicle_accelerate(
	long vehicle_index,
	real_vector3d const *acceleration)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct unit_definition *definition = vehicle_definition_get(vehicle->definition_index);

	if (definition->object.physics.index!=NONE)
	{
		struct physics_definition *physics = physics_definition_get(definition->object.physics.index);
		real_vector3d torque;
		real magnitude;

		add_vectors3d(&vehicle->object.translational_velocity, acceleration,
			&vehicle->object.translational_velocity);

		cross_product3d(global_up3d, acceleration, &torque);

		magnitude = normalize3d(&torque);
		if (magnitude>0.0f)
		{
			scale_vector3d(&torque, magnitude*_pi, &torque);

			add_vectors3d(&vehicle->object.angular_velocity, &torque,
				&vehicle->object.angular_velocity);
		}

		vehicle->object.flags &= ~FLAG(5);
	}

	return;
}

void vehicle_render_debug(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct unit_definition *definition = vehicle_definition_get(vehicle->definition_index);

	if (definition->object.physics.index!=NONE)
	{
		struct physics_definition *physics = physics_definition_get(definition->object.physics.index);

		if (debug_objects_vehicle_powered_mass_points)
		{
			short mass_point_index;

			for (mass_point_index = 0; mass_point_index<physics->mass_points.count; mass_point_index++)
			{
			}
		}
	}

	return;
}

boolean vehicle_stuck(
	long vehicle_index,
	real_vector3d *direction)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	boolean stuck = FALSE;

	if (vehicle->vehicle.stuck_mass_point_flags)
	{
		struct physics_instance instance;

		if (physics_instance_new(&instance, vehicle_index))
		{
			real_point3d center = *global_origin3d;
			short mass_point_count = 0;
			short mass_point_index;

			for (mass_point_index = 0;
				mass_point_index<instance.physics->mass_points.count;
				mass_point_index++)
			{
				if (TEST_FLAG(vehicle->vehicle.stuck_mass_point_flags, mass_point_index))
				{
					struct physics_mass_point_definition *mass_point = TAG_BLOCK_GET_ELEMENT(
						&instance.physics->mass_points, mass_point_index,
						struct physics_mass_point_definition);

					add_vectors3d(&center, &mass_point->position, &center);
					mass_point_count++;
				}
			}

			if (mass_point_count>0)
			{
				real scale = 1.0f/mass_point_count;
				real_point3d center_in_world;
				real_point3d origin;

				center.x *= scale;
				center.y *= scale;
				center.z *= scale;

				matrix4x3_transform_point(&instance.world_matrix, &center, &center_in_world);
				object_get_origin(vehicle_index, &origin);
				subtract_vectors3d(&center_in_world, &origin, direction);

				if (normalize3d(direction)!=0.0f)
					stuck = TRUE;
			}
		}
	}

	return stuck;
}

/* NOTE: the vehicle function enum is not recovered from January. These names
describe what each case computes, read off the disassembly. They are descriptive,
not authentic. */

enum
{
	_vehicle_function_none = 0,
	_vehicle_function_speed_absolute,
	_vehicle_function_speed_forward,
	_vehicle_function_speed_reverse,
	_vehicle_function_slide_absolute,
	_vehicle_function_slide_left,
	_vehicle_function_slide_right,
	_vehicle_function_speed_or_slide,
	_vehicle_function_turn_absolute,
	_vehicle_function_turn_left,
	_vehicle_function_turn_right,
	_vehicle_function_flag2,
	_vehicle_function_flag3,
	_vehicle_function_unused13,
	_vehicle_function_velocity_absolute,
	_vehicle_function_velocity_moving,
	_vehicle_function_velocity_sliding,
	_vehicle_function_velocity_forward,
	_vehicle_function_velocity_up,
	_vehicle_function_velocity_up_alternate,
	_vehicle_function_left_tread_position,
	_vehicle_function_right_tread_position,
	_vehicle_function_speed_minus_turn,
	_vehicle_function_speed_plus_turn,
	_vehicle_function_wheel_position_a,
	_vehicle_function_wheel_position_b,
	_vehicle_function_wheel_position_c,
	_vehicle_function_wheel_position_d,
	_vehicle_function_speed_absolute_a,
	_vehicle_function_speed_absolute_b,
	_vehicle_function_speed_absolute_c,
	_vehicle_function_speed_absolute_d,
	_vehicle_function_sideslip,
	_vehicle_function_unknown444,
	_vehicle_function_unknown448,
	_vehicle_function_speed_blend,
	_vehicle_function_boost,
	NUMBER_OF_VEHICLE_FUNCTIONS
};

void vehicle_export_function_values(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	real forward_speed = (real)fabs(definition->unknown2f8);
	real reverse_speed = (real)fabs(definition->unknown2fc);
	real maximum_speed = MAX(forward_speed, reverse_speed);
	real left_slide = (real)fabs(definition->unknown330);
	real right_slide = (real)fabs(definition->unknown334);
	real maximum_slide = MAX(left_slide, right_slide);
	real left_turn = (real)fabs(definition->unknown308);
	real right_turn = (real)fabs(definition->unknown30c);
	real maximum_turn = MAX(left_turn, right_turn);
	real *value = vehicle->object.incoming_function_values;
	short *mode = definition->function_modes;
	long remaining;

	for (remaining = NUMBER_OF_INCOMING_OBJECT_FUNCTIONS; remaining; remaining--)
	{
		if (*mode)
		{
			real result = 0.0f;

			switch (*mode)
			{
				case _vehicle_function_speed_absolute:
				case _vehicle_function_speed_absolute_a:
				case _vehicle_function_speed_absolute_b:
				case _vehicle_function_speed_absolute_c:
				case _vehicle_function_speed_absolute_d:
					result = (real)fabs(vehicle->vehicle.speed)/maximum_speed;
					break;

				case _vehicle_function_speed_forward:
					result = (vehicle->vehicle.speed<0.0f ? 0.0f :
						vehicle->vehicle.speed)/forward_speed;
					break;

				case _vehicle_function_speed_reverse:
					result = (real)fabs(vehicle->vehicle.speed>0.0f ? 0.0f :
						vehicle->vehicle.speed)/reverse_speed;
					break;

				case _vehicle_function_slide_absolute:
					result = (real)fabs(vehicle->vehicle.slide)/maximum_slide;
					break;

				case _vehicle_function_slide_left:
					result = (real)fabs(vehicle->vehicle.slide)/left_slide;
					break;

				case _vehicle_function_slide_right:
					result = (real)fabs(vehicle->vehicle.slide)/right_slide;
					break;

				case _vehicle_function_speed_or_slide:
					result = MAX(fabs(vehicle->vehicle.speed)/maximum_speed,
						fabs(vehicle->vehicle.slide)/maximum_slide);
					break;

				case _vehicle_function_turn_absolute:
					result = (real)fabs(vehicle->vehicle.turn)/maximum_turn;
					break;

				case _vehicle_function_turn_left:
					result = (real)fabs(vehicle->vehicle.turn)/left_turn;
					break;

				case _vehicle_function_turn_right:
					result = (real)fabs(vehicle->vehicle.turn)/right_turn;
					break;

				case _vehicle_function_flag2:
					result = TEST_FLAG(vehicle->vehicle.flags, 2) ? 1.0f : 0.0f;
					break;

				case _vehicle_function_flag3:
					result = TEST_FLAG(vehicle->vehicle.flags, 3) ? 1.0f : 0.0f;
					break;

				case _vehicle_function_velocity_absolute:
					result = magnitude3d(&vehicle->object.translational_velocity)/
						maximum_speed;
					break;

				case _vehicle_function_velocity_moving:
					result = vehicle->object.flags&(FLAG(2)|FLAG(3)|FLAG(4)) ?
						magnitude3d(&vehicle->object.translational_velocity)/maximum_speed :
						0.0f;
					break;

				case _vehicle_function_velocity_sliding:
					result = TEST_FLAG(vehicle->object.flags, 1) ?
						magnitude3d(&vehicle->object.translational_velocity)/maximum_speed :
						0.0f;
					break;

				case _vehicle_function_velocity_forward:
					result = (real)fabs(dot_product3d(
						&vehicle->object.translational_velocity,
						&vehicle->object.forward))/maximum_speed;
					break;

				case _vehicle_function_velocity_up:
				case _vehicle_function_velocity_up_alternate:
					result = (real)fabs(dot_product3d(
						&vehicle->object.translational_velocity,
						&vehicle->object.up))/maximum_speed;
					break;

				case _vehicle_function_left_tread_position:
					result = vehicle->vehicle.left_tread/definition->wheel_circumference;
					break;

				case _vehicle_function_right_tread_position:
					result = vehicle->vehicle.right_tread/definition->wheel_circumference;
					break;

				case _vehicle_function_speed_minus_turn:
					result = (real)fabs(vehicle->vehicle.speed-
						vehicle->vehicle.turn)/maximum_speed;
					break;

				case _vehicle_function_speed_plus_turn:
					result = (real)fabs(vehicle->vehicle.turn+
						vehicle->vehicle.speed)/maximum_speed;
					break;

				case _vehicle_function_wheel_position_a:
				case _vehicle_function_wheel_position_b:
				case _vehicle_function_wheel_position_c:
				case _vehicle_function_wheel_position_d:
					result = vehicle->vehicle.wheel/definition->wheel_circumference;
					break;

				case _vehicle_function_sideslip:
				{
					real_vector3d along, across;

					component_vectors_from_normal3d(
						&vehicle->object.translational_velocity,
						&vehicle->object.forward, &along, &across);

					result = magnitude3d(&across)*(1.0f/0.3f);
					result = result*result;
					break;
				}

				case _vehicle_function_unknown448:
					result = vehicle->vehicle.thrust;
					break;

				case _vehicle_function_unknown444:
					result = vehicle->vehicle.hover;
					break;

				case _vehicle_function_speed_blend:
				{
					real dot_speed;
					real forward_value;
					real blend;

					dot_speed = (real)fabs(dot_product3d(&vehicle->object.translational_velocity, &vehicle->object.forward))/maximum_speed;
					forward_value = (real)fabs(vehicle->vehicle.speed)/forward_speed;
					blend = PIN(((real)vehicle->vehicle.airborne_ticks*0.2f+1.0f)*0.5f, 0.0f, 1.0f);
					result = dot_speed*(1.0f-blend)+forward_value*blend;
					break;
				}

				case _vehicle_function_boost:
					result = magnitude3d(&vehicle->object.translational_velocity)/
						definition->unknown2f8;
					result = (result*vehicle->vehicle.thrust-0.05f)*
						(1.0f/(0.9f-0.05f));
					break;
			}

			*value = PIN(result, 0.0f, 1.0f);
		}

		mode++;
		value++;
	}

	return;
}

boolean vehicle_moving_near_any_player(
	void)
{
	long unit_indices[MAXIMUM_LOCAL_PLAYERS];
	real_point3d unit_centers[MAXIMUM_LOCAL_PLAYERS];
	short unit_count = 0;
	boolean no_vehicles_moving = TRUE;
	short local_player_index;

	for (local_player_index = local_player_get_next(NONE);
		local_player_index!=NONE;
		local_player_index = local_player_get_next(local_player_index))
	{
		if (local_player_get_player_index(local_player_index)!=NONE)
		{
			long unit_index = player_get(local_player_get_player_index(local_player_index))->unit_index;

			if (unit_index!=NONE
				&& ((struct unit_datum *)unit_get(unit_index))->object.parent_object_index==NONE)
			{
				real radius;

				unit_indices[unit_count] = unit_index;
				object_get_bounding_sphere(unit_index, &unit_centers[unit_count], &radius);
				unit_count++;
			}
		}
	}

	if (unit_count)
	{
		struct object_iterator iterator;
		struct object_datum *object;

		object_iterator_new(&iterator, _object_mask_vehicle, 0);
		do
		{
			object = (struct object_datum *)object_iterator_next(&iterator);
			if (object)
			{
				short index;

				for (index = 0; index<unit_count; index++)
				{
					if (((struct unit_datum *)unit_get(unit_indices[index]))->object.parent_object_index!=iterator.index
						&& distance_squared3d(&unit_centers[index], &object->object.bounding_sphere_center)<100.0f
						&& magnitude_squared3d(&object->object.translational_velocity)>=(real)((1.0/30.0)*(1.0/30.0)))
					{
						no_vehicles_moving = FALSE;
						goto done;
					}
				}
			}
		} while (object);
	}

done:
	return !no_vehicles_moving;
}

long vehicle_find_pathfinding_surface_index(
	long vehicle_index,
	real_point3d *position)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	long surface_index = NONE;

	object_get_origin(vehicle_index, position);

	switch (definition->vehicle_type)
	{
		case _vehicle_type_human_tank:
		case _vehicle_type_human_jeep:
		case _vehicle_type_alien_scout:
		case _vehicle_type_turret:
		{
			struct collision_bsp *bsp = global_collision_bsp_get();
			struct collision_bsp_test_vector_result result;
			real_point3d origin;
			real_vector3d vector;

			object_get_origin(vehicle_index, &origin);

			point_from_line3d(&origin, global_up3d, 0.4f, &origin);

			add_vectors3d(global_down3d, global_down3d, &vector);

			if (collision_bsp_test_vector(1, bsp, 0, NULL, &origin, &vector, FLT_MAX, &result))
			{
				surface_index = result.surface_index;

				point_from_line3d(&origin, &vector, result.t, position);
			}
			break;
		}
	}

	return surface_index;
}

static void compute_airborne_ticks(
	long vehicle_index,
	struct mass_point_datum *mass_points,
	void *state)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct physics_definition *physics = physics_definition_get(
		vehicle_definition_get(vehicle->definition_index)->object.physics.index);
	short mass_point_index;

#ifdef HALO_N64
	/* (counted in ticks: a physics step covers one to four) */
	extern real n64_vehicle_physics_step(void);
	short n64_ticks = (short)n64_vehicle_physics_step();

	vehicle->vehicle.airborne_ticks = (byte)MIN(0xff, vehicle->vehicle.airborne_ticks+n64_ticks);
#else
	if (vehicle->vehicle.airborne_ticks<0xff)
		vehicle->vehicle.airborne_ticks++;
#endif

	for (mass_point_index = 0; mass_point_index<physics->mass_points.count; mass_point_index++)
	{
		struct mass_point_datum *mass_point = &mass_points[mass_point_index];

		if (TEST_FLAG(mass_point->flags, 1))
		{
			vehicle->vehicle.airborne_ticks = 0;
#ifdef HALO_N64
			vehicle->vehicle.on_ground_ticks = (byte)MIN(0xff, vehicle->vehicle.on_ground_ticks+n64_ticks);
#else
			if (vehicle->vehicle.on_ground_ticks<0xff)
				vehicle->vehicle.on_ground_ticks++;
#endif
			return;
		}

		if (TEST_FLAG(mass_point->flags, 4))
			vehicle->vehicle.airborne_ticks = 0;
	}

	vehicle->vehicle.on_ground_ticks = 0;

	return;
}

static real_vector3d *compute_acceleration(
	real_vector3d *a,
	real_vector3d *b,
	real_vector3d *result,
	real maximum,
	real minimum);

static void update_alien_fighter_physics_new(
	long vehicle_index,
	struct powered_mass_point_datum *state,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);

	if (physics->powered_mass_points.count==2)
	{
		real_vector3d magic_force;
		real_vector3d magic_torque;
		real_vector3d axis;
		real_vector3d desired_angular_velocity;
		real_vector3d angular_acceleration;
		real spin;
		real thrust_delta;

		{
			real_vector3d desired_velocity;
			real_vector3d acceleration;
			real throttle;

			scale_vector3d(&vehicle->object.forward, vehicle->vehicle.speed, &desired_velocity);

			if (vehicle->vehicle.speed>0.0f)
				throttle = vehicle->vehicle.speed/definition->unknown2f8;
			else
				throttle = -(vehicle->vehicle.speed/definition->unknown2fc);

			compute_acceleration(&desired_velocity, &vehicle->object.translational_velocity, &acceleration,
				throttle*definition->unknown300, throttle*definition->unknown304);

			scale_vector3d(&acceleration, physics->mass, &magic_force);
			scale_vector3d(&magic_force, vehicle->unit.seat_power[0], &magic_force);
		}

		{
			real_matrix3x3 current_rotation, desired_rotation, rotation;
			real_quaternion rotation_quaternion;
			real yaw;
			real angle;

			matrix3x3_from_forward_and_up(&current_rotation, &vehicle->object.forward,
				&vehicle->object.up);

			desired_rotation.forward = vehicle->unit.desired_facing_vector;

			point_from_line3d(global_up3d, &desired_rotation.forward, -desired_rotation.forward.k, &desired_rotation.up);

			if (normalize3d(&desired_rotation.up)==0.0f)
				desired_rotation.up = *global_forward3d;

			if (!unit_driven_by_ai(vehicle_index))
				pitch_vectors(&desired_rotation.forward, &desired_rotation.up, sine(definition->unknown364),
					cosine(definition->unknown364));

			yaw = cross_product2d((real_vector2d const *)&desired_rotation.forward,
				(real_vector2d const *)&vehicle->object.translational_velocity)/
				definition->unknown2f8*definition->unknown308;

			yaw_vectors(&desired_rotation.up, &desired_rotation.forward, sine(yaw), cosine(yaw));

			cross_product3d(&desired_rotation.up, &desired_rotation.forward, &desired_rotation.left);

			matrix3x3_transpose(&current_rotation, &current_rotation);
			matrix3x3_multiply(&desired_rotation, &current_rotation, &rotation);
			matrix3x3_rotation_to_quaternion(&rotation, &rotation_quaternion);
			quaternion_to_angle_and_vector(&rotation_quaternion, &angle, &axis);
			scale_vector3d(&axis, (-angle)*definition->unknown314*(1.0f/_pi), &desired_angular_velocity);
		}

		subtract_vectors3d(&desired_angular_velocity, &vehicle->object.angular_velocity, &angular_acceleration);

		scale_vector3d(&angular_acceleration,
			(physics->zz_moment+physics->yy_moment+physics->xx_moment)*(1.0f/3), &magic_torque);
		scale_vector3d(&magic_torque, vehicle->unit.seat_power[0], &magic_torque);

		spin = magnitude3d(&vehicle->object.angular_velocity)/definition->unknown314;

		if (spin>vehicle->vehicle.thrust)
		{
			thrust_delta = PIN((1.0f-vehicle->vehicle.thrust)*
				(1.0f-vehicle->vehicle.thrust)*0.2f,
				0.01f, 0.05f);
			thrust_delta = MIN(spin-vehicle->vehicle.thrust, thrust_delta);
		}
		else
		{
			thrust_delta = -MAX(vehicle->vehicle.thrust*vehicle->vehicle.thrust*0.05f,
				0.005f);
			thrust_delta = MAX(spin-vehicle->vehicle.thrust, thrust_delta);
		}

		vehicle->vehicle.thrust += thrust_delta;

		state[0].antigrav_fraction = vehicle->unit.seat_power[0];
		state[0].rotation = *global_identity_quaternion;

		state[1].antigrav_fraction = vehicle->unit.seat_power[0];
		state[1].rotation = *global_identity_quaternion;

		physics_update(vehicle_index, state, mass_points, &magic_force, &magic_torque);
	}
	else
	{
		physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
	}

	return;
}

static void update_alien_fighter_physics_old(
	long vehicle_index,
	struct mass_point_datum *mass_points,
	struct powered_mass_point_datum *state)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);

	if (physics->powered_mass_points.count==2)
	{
		real_vector3d facing = vehicle->unit.desired_facing_vector;
		real_vector3d perpendicular = *global_up3d;
		real_matrix4x3 actual, desired, difference;
		real_vector3d axis, force, torque;
		real_vector3d scaled;
		real_vector2d velocity;
		real yaw, speed, thrust, lift, scale;

		perpendicular.i = -facing.k*facing.i;
		perpendicular.j = -(facing.k*facing.j);
		perpendicular.k = 1.0f-facing.k*facing.k;

		if (normalize3d(&perpendicular)==0.0f)
			set_real_vector3d(&perpendicular, 1.0f, 0.0f, 0.0f);

		speed = dot_product3d(&vehicle->object.forward,
			&vehicle->object.translational_velocity);
		thrust = (vehicle->vehicle.speed-speed)*physics->mass*0.05f;
		lift = ((real)fabs(speed/definition->unknown2f8)*physics->mass)*global_gravity*1.05f;

		force.i = lift*vehicle->object.up.i+thrust*vehicle->object.forward.i;
		force.j = lift*vehicle->object.up.j+thrust*vehicle->object.forward.j;
		force.k = lift*vehicle->object.up.k+thrust*vehicle->object.forward.k;

		{
			real *destination = velocity.n;
			real const *source = vehicle->object.translational_velocity.n;
			short component_index;

			for (component_index = 0; component_index<2; component_index++)
				destination[component_index] = source[component_index];
		}

		yaw = (facing.i*velocity.j-facing.j*velocity.i)*(_pi*0.5f)/
			(real)fabs(definition->unknown2f8);

		yaw_vectors(&perpendicular, &facing, sine(yaw), cosine(yaw));

		{
			real_quaternion rotation;
			real angle;

			matrix4x3_rotation_from_vectors(&actual, &vehicle->object.forward,
				&vehicle->object.up);
			matrix4x3_rotation_from_vectors(&desired, &facing, &perpendicular);
			matrix4x3_inverse(&desired, &desired);
			matrix4x3_multiply(&actual, &desired, &difference);
			matrix4x3_rotation_to_quaternion(&difference, &rotation);
			quaternion_to_angle_and_vector(&rotation, &angle, &axis);
			scale_vector3d(&axis, angle*(4.0f/30.0f), &scaled);
		}

		scale = physics->radius*physics->radius*physics->mass*0.05f;

		torque.i = (scaled.i-vehicle->object.angular_velocity.i)*scale;
		torque.j = (scaled.j-vehicle->object.angular_velocity.j)*scale;
		torque.k = (scaled.k-vehicle->object.angular_velocity.k)*scale;

		state[0].antigrav_fraction = vehicle->unit.seat_power[0];
		state[0].rotation.w = 1.0f;
		state[0].rotation.v.i = 0.0f;
		state[0].rotation.v.j = 0.0f;
		state[0].rotation.v.k = 0.0f;

		state[1].antigrav_fraction = vehicle->unit.seat_power[0];
		state[1].rotation.w = 1.0f;
		state[1].rotation.v.i = 0.0f;
		state[1].rotation.v.j = 0.0f;
		state[1].rotation.v.k = 0.0f;

		scale_vector3d(&force, vehicle->unit.seat_power[0], &force);
		scale_vector3d(&torque, vehicle->unit.seat_power[0], &torque);

		physics_update(vehicle_index, state, mass_points, &force, &torque);
	}
	else
	{
		physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
	}

	return;
}

static void slowly_stop_vehicle(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	real_point3d *object_position = &vehicle->object.position;
	real_vector3d *velocity = &vehicle->object.translational_velocity;
	real_vector3d *angular_velocity = &vehicle->object.angular_velocity;
	real_matrix4x3 rotation;
	real_point3d position;
	real_vector3d forward, up;
	real magnitude;

	vehicle->vehicle.stop_time--;

	velocity->i *= 0.835f;
	velocity->j *= 0.835f;
	velocity->k *= 0.835f;
	angular_velocity->i *= 0.835f;
	angular_velocity->j *= 0.835f;
	angular_velocity->k *= 0.835f;

	position.x = velocity->i+object_position->x;
	position.y = object_position->y+velocity->j;
	position.z = object_position->z+velocity->k;

	{
		real_vector3d axis;
		axis = *angular_velocity;

		magnitude = normalize3d(&axis);
		if (magnitude!=0.0f)
		{
			matrix4x3_rotation_from_axis_and_angle(&rotation, &axis, sine(magnitude), cosine(magnitude));
			matrix4x3_transform_vector(&rotation, &vehicle->object.forward, &forward);
			matrix4x3_transform_vector(&rotation, &vehicle->object.up, &up);
		}
		else
		{
			forward = vehicle->object.forward;
			up = vehicle->object.up;
		}
	}

	if (!vehicle->vehicle.stop_time)
	{
		*velocity = *global_zero_vector3d;
		*angular_velocity = *global_zero_vector3d;
	}

	object_set_position(vehicle_index, &position, &forward, &up);

	return;
}

static real_vector3d *compute_acceleration(
	real_vector3d *a,
	real_vector3d *b,
	real_vector3d *result,
	real maximum,
	real minimum)
{
	real dot;

	subtract_vectors3d(a, b, result);
	result->k += global_gravity;

	dot = dot_product3d(a, result);
	if (dot>_real_epsilon)
		maximum = (maximum-minimum)*((dot*dot/magnitude_squared3d(result))/magnitude_squared3d(a))+minimum;
	else
		maximum = minimum;

	limit3d(result, maximum);

	return result;
}

static void create_ghost_effect(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);

	if (definition->effect.index!=NONE && vehicle->unit.seat_power[0]>0.0f)
	{
		struct object_marker markers[16];
		short hover_marker_count = object_get_marker_by_name(vehicle_index, "hover thrusters", markers, 15);
		short jet_marker_count = 0;
		short marker_index;

		for (marker_index = 0; marker_index<hover_marker_count+jet_marker_count; marker_index++)
		{
			struct object_marker *marker = &markers[marker_index];
			struct collision_result collision;
			real_vector3d direction;
			real_vector3d vector;
			real_point3d midpoint;
			real_point3d marker_points[4];
			real_vector3d marker_forwards[4];
			char const *marker_names[4];
			real scale;

			seed_random_vector_in_cone3d(get_global_local_random_seed_address(),
				&marker->matrix.forward, 0.0f, 15.0f, &direction);

			vector = direction;

			if (collision_test_vector(0x61, &marker->matrix.position, &vector, vehicle_index,
				&collision))
			{
				scale = PIN((-marker->matrix.forward.k)*(1.0f-collision.t)*vehicle->unit.seat_power[0],
					0.0f, 1.0f);

				if (scale>0.0f)
				{
					midpoint.x = (marker->matrix.position.x+collision.point.x)*0.5f;
					midpoint.y = (marker->matrix.position.y+collision.point.y)*0.5f;
					midpoint.z = (marker->matrix.position.z+collision.point.z)*0.5f;
					marker_points[0] = collision.point;
					marker_points[1] = collision.point;
					marker_forwards[0].i = -direction.i;
					marker_names[0] = "incident";
					marker_forwards[0].j = -direction.j;
					marker_names[1] = "normal";
					marker_forwards[1] = collision.plane.n;
					marker_points[2] = collision.point;
					marker_forwards[0].k = -direction.k;
					marker_names[2] = "reflected";

					reflect_vector3d(&direction, &collision.plane.n, &marker_forwards[2]);

					marker_points[3] = midpoint;
					marker_names[3] = "midpoint";

					reflect_vector3d(&direction, &collision.plane.n, &marker_forwards[3]);

					effect_new_unattached_from_markers(definition->effect.index, NONE, NULL, 4,
						marker_names, marker_points, marker_forwards, scale, scale,
						NULL, NULL, TRUE);
				}
			}
		}
	}

	return;
}


static void create_pelican_effect(
	long vehicle_index)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);

	if (definition->effect.index!=NONE)
	{
		struct object_marker markers[16];
		short marker_count = object_get_marker_by_name(vehicle_index, "hover thrusters", markers, 15);
		long total_marker_count = marker_count;
		short marker_index;

		total_marker_count += object_get_marker_by_name(vehicle_index,
			"jet thrusters", &markers[total_marker_count], (short)(16-marker_count));

		for (marker_index = 0; marker_index<total_marker_count; marker_index++)
		{
			struct object_marker *marker = &markers[marker_index];
			struct collision_result collision;
			real_vector3d direction;
			real_vector3d vector;
			real_point3d marker_points[3];
			real_vector3d marker_forwards[3];
			char const *marker_names[3];
			real length;
			real scale;

			seed_random_vector_in_cone3d(get_global_local_random_seed_address(),
				&marker->matrix.forward, 0.0f, _pi/12, &direction);

			length = (marker_index<marker_count
				? vehicle->vehicle.hover
				: vehicle->vehicle.thrust)*6.0f+2.0f;

			vector.i = direction.i*length;
			vector.j = direction.j*length;
			vector.k = direction.k*length;

			if (collision_test_vector(0x61, &marker->matrix.position, &vector, vehicle_index,
				&collision))
			{
				marker_points[0] = collision.point;
				marker_points[1] = collision.point;
				marker_forwards[0].i = -direction.i;
				marker_names[0] = "incident";
				marker_forwards[0].j = -direction.j;
				marker_names[1] = "normal";
				marker_forwards[1] = collision.plane.n;
				marker_points[2] = collision.point;
				marker_forwards[0].k = -direction.k;
				marker_names[2] = "reflected";

				reflect_vector3d(&direction, &collision.plane.n, &marker_forwards[2]);

				scale = 1.0f-collision.t;

				effect_new_unattached_from_markers(definition->effect.index, NONE, NULL, 3,
					marker_names, marker_points, marker_forwards, scale, scale,
					NULL, NULL, TRUE);
			}
		}
	}

	return;
}

static real vehicle_dot_product3d_test(
	real_vector3d const *a,
	real_vector3d const *b)
{
	return a->i*b->i+a->j*b->j+a->k*b->k;
}

void vehicle_preprocess_node_orientations(
	long vehicle_index,
	struct real_orientation *node_orientations)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct animation_graph *graph;
	struct vehicle_animation *animation;
	struct animation *overlay;
	real value;
	short suspension_index;

	if (definition->unit.object.animation_graph.index==NONE)
		return;

	graph = animation_graph_definition_get(
		definition->unit.object.animation_graph.index);

	if (!graph->vehicle_animations.count)
		return;

	animation = TAG_BLOCK_GET_ELEMENT(&graph->vehicle_animations, 0, struct vehicle_animation);
	if (!animation)
		return;

	if (animation->animations.count>0
		&& ((short *)animation->animations.address)[0]!=NONE)
	{
		aiming_screen_apply(TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[0], struct animation),
			animation, vehicle->vehicle.turn, 0.0f, node_orientations);
	}

	if (animation->animations.count>1
		&& ((short *)animation->animations.address)[1]!=NONE)
	{
		overlay = TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[1], struct animation);

		value = (triple_product3d(&vehicle->object.up, &vehicle->object.forward,
			&vehicle->object.translational_velocity)/definition->unknown2f8+1.0f)*0.5f;

		overlay_animation_apply_continuous(overlay,
			PIN(value, 0.0f, 1.0f)*(overlay->frame_count-1), node_orientations);
	}

	if (animation->animations.count>2
		&& ((short *)animation->animations.address)[2]!=NONE)
	{
		overlay = TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[2], struct animation);

		if (vehicle->vehicle.speed<0.0f)
			value = 0.5f-vehicle->vehicle.speed/definition->unknown2fc*0.5f;
		else
			value = (vehicle->vehicle.speed/definition->unknown2f8+1.0f)*0.5f;

		overlay_animation_apply_continuous(overlay, value*(overlay->frame_count-1),
			node_orientations);
	}


	if (animation->animations.count>3
		&& ((short *)animation->animations.address)[3]!=NONE)
	{
		overlay = TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[3], struct animation);

		value = vehicle_dot_product3d_test(&vehicle->object.translational_velocity, &vehicle->object.forward);
		value = PIN(value, 0.0f, 1.0f)/(real)fabs(definition->unknown2f8);

		overlay_animation_apply_continuous(overlay,
			PIN(value, 0.0f, 1.0f)*(overlay->frame_count-1), node_orientations);
	}

	if (animation->animations.count>4
		&& ((short *)animation->animations.address)[4]!=NONE)
	{
		TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[4], struct animation);
	}

	if (animation->animations.count>5
		&& ((short *)animation->animations.address)[5]!=NONE)
	{
		overlay = TAG_BLOCK_GET_ELEMENT(&graph->animations,
			((short *)animation->animations.address)[5], struct animation);

		if (definition->wheel_circumference>0.0f)
			value = vehicle->vehicle.wheel/definition->wheel_circumference;
		else
			value = 0.0f;

		overlay_animation_apply_continuous(overlay, value*overlay->frame_count,
			node_orientations);
	}

	for (suspension_index = 0;
		suspension_index<animation->suspensions.count;
		suspension_index++)
	{
		struct vehicle_suspension *suspension = TAG_BLOCK_GET_ELEMENT(
			&animation->suspensions, suspension_index, struct vehicle_suspension);

		if (suspension->animation_index!=NONE)
		{
			overlay = TAG_BLOCK_GET_ELEMENT(&graph->animations,
				suspension->animation_index, struct animation);

			if (vehicle->vehicle.suspension[suspension_index]==0xff)
				value = 1.0f;
			else
				value = vehicle->vehicle.suspension[suspension_index]*(1.0f/255);

			overlay_animation_apply_continuous(overlay, value*(overlay->frame_count-1),
				node_orientations);
		}
	}

	return;
}

static void update_human_plane_physics(
	long vehicle_index,
	struct powered_mass_point_datum *state,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);
	if (TEST_FLAG(vehicle->vehicle.flags, 1))
	{
		csmemset(mass_points, 0,
			physics->mass_points.count*sizeof(struct mass_point_datum));
	}
	else
	{
		real_vector3d desired_forward;
		real_vector3d desired_up;
		real_vector3d force;
		real_vector3d torque;
		real throttle;
		real factor;

		throttle = PIN(vehicle->vehicle.speed, 0.0f, definition->unknown2f8)/
			definition->unknown2f8;
		throttle = throttle*throttle;

		factor = !TEST_FLAG(vehicle->vehicle.flags, 2) ? (TEST_FLAG(vehicle->vehicle.flags, 3) ? 1.0f : 0.75f) : 0.25f;
		factor *= 1.0f-throttle;
		interpolate_scalar(&vehicle->vehicle.hover, factor*vehicle->unit.seat_power[0], 0.05f);

		vehicle->vehicle.thrust = throttle*vehicle->unit.seat_power[0];

		desired_forward = vehicle->unit.desired_facing_vector;

		desired_up.i = -(desired_forward.k*desired_forward.i);
		desired_up.j = -(desired_forward.k*desired_forward.j);
		desired_up.k = 1.0f-desired_forward.k*desired_forward.k;

		if (normalize3d(&desired_up)==0.0f)
		{
			desired_up.i = 1.0f;
			desired_up.j = 0.0f;
			desired_up.k = 0.0f;
		}

		{
			real const drive_scale = 0.05f;
			real const hover_lift_scale = 1.3f;
			real const speed_lift_scale = 1.05f;
			real dot;
			real drive;
			real lift;

			dot = dot_product3d(&vehicle->object.translational_velocity, &vehicle->object.forward);

			drive = (vehicle->vehicle.speed-dot)*vehicle->vehicle.thrust*physics->mass*drive_scale;
			lift = ((real)fabs(dot/definition->unknown2f8)*speed_lift_scale+
				vehicle->vehicle.hover*hover_lift_scale)*global_gravity;
			lift *= physics->mass;

			force.i = lift*vehicle->object.up.i+drive*vehicle->object.forward.i;
			force.j = lift*vehicle->object.up.j+drive*vehicle->object.forward.j;
			force.k = lift*vehicle->object.up.k+drive*vehicle->object.forward.k;
		}

		{
			real const yaw_scale = _pi/2;
			real_vector2d velocity;
			real yaw;

			{
				real *destination = velocity.n;
				real const *source = vehicle->object.translational_velocity.n;
				short component_index;

				for (component_index = 0; component_index<2; component_index++)
					destination[component_index] = source[component_index];
			}

			yaw = (velocity.j*desired_forward.i-velocity.i*desired_forward.j)*yaw_scale/
				(real)fabs(definition->unknown2f8);

			yaw_vectors(&desired_up, &desired_forward, sine(yaw), cosine(yaw));
		}

		{
			real const angle_scale = 1.0f/30;
			real const torque_scale = 0.05f;
			real_matrix4x3 vehicle_rotation;
			real_matrix4x3 desired_rotation;
			real_matrix4x3 rotation;
			real_quaternion quaternion;
			real_vector3d axis;
			real_vector3d scaled;
			real angle;
			real scale;

			matrix4x3_rotation_from_vectors(&vehicle_rotation, &vehicle->object.forward,
				&vehicle->object.up);
			matrix4x3_rotation_from_vectors(&desired_rotation, &desired_forward, &desired_up);
			matrix4x3_inverse(&desired_rotation, &desired_rotation);
			matrix4x3_multiply(&vehicle_rotation, &desired_rotation, &rotation);
			matrix4x3_rotation_to_quaternion(&rotation, &quaternion);
			quaternion_to_angle_and_vector(&quaternion, &angle, &axis);

			scale_vector3d(&axis, angle*angle_scale, &scaled);

			scale = physics->radius*physics->radius*physics->mass*torque_scale;

			torque.i = (scaled.i-vehicle->object.angular_velocity.i)*scale;
			torque.j = (scaled.j-vehicle->object.angular_velocity.j)*scale;
			torque.k = (scaled.k-vehicle->object.angular_velocity.k)*scale;
		}

		scale_vector3d(&force, vehicle->unit.seat_power[0], &force);
		scale_vector3d(&torque, vehicle->unit.seat_power[0], &torque);

		physics_update(vehicle_index, NULL, mass_points, &force, &torque);
	}

	create_pelican_effect(vehicle_index);

	return;
}

static boolean update_suspension(
	long vehicle_index)
{
	boolean result = FALSE;
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);

	if (definition->unit.object.animation_graph.index!=NONE)
	{
		struct animation_graph *graph = animation_graph_definition_get(
			definition->unit.object.animation_graph.index);
		struct vehicle_animation *animation;
		struct physics_definition *physics;
		real_matrix4x3 matrix;
		real maximum_shift;
		short suspension_index;

		if (graph->vehicle_animations.count)
		{
			animation = TAG_BLOCK_GET_ELEMENT(&graph->vehicle_animations, 0,
				struct vehicle_animation);

			if (animation)
			{

				physics = physics_definition_get(definition->unit.object.physics.index);

				maximum_shift = 0.0f;

				matrix4x3_from_point_and_vectors(&matrix, &vehicle->object.position,
					&vehicle->object.forward, &vehicle->object.up);

				for (suspension_index = 0;
					suspension_index<animation->suspensions.count;
					suspension_index++)
				{
					struct vehicle_suspension *suspension = TAG_BLOCK_GET_ELEMENT(
						&animation->suspensions, suspension_index, struct vehicle_suspension);

					if (suspension->mass_point_index>=0
						&& suspension->mass_point_index<physics->mass_points.count
						&& suspension->animation_index!=NONE)
					{
						struct physics_mass_point_definition *mass_point;
						struct collision_result collision;
						real_point3d point;
						real_vector3d normal;
						real_point3d start;
						real_vector3d vector;
						real current;
						real extent;
						real offset;
						real shift;

						TAG_BLOCK_GET_ELEMENT(&graph->animations, suspension->animation_index,
							struct animation);
						mass_point = TAG_BLOCK_GET_ELEMENT(&physics->mass_points,
							suspension->mass_point_index, struct physics_mass_point_definition);

						if (vehicle->vehicle.suspension[suspension_index]==0xff)
							current = 1.0f;
						else
							current = vehicle->vehicle.suspension[suspension_index]*(1.0f/255);

						matrix4x3_transform_point(&matrix, &mass_point->position, &point);
						matrix4x3_transform_normal(&matrix, &mass_point->normal, &normal);

						extent = suspension->unknown4-suspension->unknown8;
						offset = (suspension->unknown8-physics->center_of_mass.z)-extent;

						{
							real_point3d const *p = &point;
							real_vector3d const *v = &normal;
							real t = offset;
							real_point3d *r = &start;
							r->x = (v->i*t) + p->x;
							r->y = (v->j*t) + p->y;
							r->z = (v->k*t) + p->z;
						}

						scale_vector3d(&normal, extent+extent, &vector);

						collision_test_vector(0xc0a0, &start, &vector, vehicle_index, &collision);

						shift = PIN((1.0f-collision.t)+(1.0f-collision.t), 0.0f, 1.0f);

						if (shift-current>maximum_shift)
							maximum_shift = shift-current;

						vehicle->vehicle.suspension[suspension_index] =
							quantize_real_to_byte_lower_bound(0.0f, 1.0f, (shift+current)*0.5f);
					}
				}

				if (definition->suspension_sound.index!=NONE && maximum_shift>0.3f)
				{
					object_impulse_sound_new(vehicle_index, definition->suspension_sound.index, NONE,
						global_origin3d, global_forward3d,
						PIN((maximum_shift-0.3f)*(1.0f/(0.9f-0.3f)), 0.0f, 1.0f));

					result = TRUE;
				}
			}
		}
	}

	return result;
}

static void create_crashing_effects(
	long vehicle_index,
	real_vector3d const *previous_velocity,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);
	struct game_globals_falling_damage *falling_damage = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->falling_damage, 0, struct game_globals_falling_damage);
	real_vector3d delta;
	real speed;

	if (falling_damage->vehicle_hit_environment_damage_effect.index!=NONE ||
		definition->crash_sound.index!=NONE)
	{
		short mass_point_index;

		delta.i = vehicle->object.translational_velocity.i-previous_velocity->i;
		delta.j = vehicle->object.translational_velocity.j-previous_velocity->j;
		delta.k = vehicle->object.translational_velocity.k-previous_velocity->k;

		speed = magnitude3d(&delta);
#ifdef HALO_N64
		{
			/* (the change a tick: a physics step covers one to four) */
			extern real n64_vehicle_physics_step(void);

			speed /= n64_vehicle_physics_step();
		}
#endif
		if (speed>0.02f)
		{
			for (mass_point_index = 0; mass_point_index<physics->mass_points.count; mass_point_index++)
			{
				TAG_BLOCK_GET_ELEMENT(&physics->mass_points, mass_point_index,
					struct physics_mass_point_definition);

				if (TEST_FLAG(mass_points[mass_point_index].flags, 1))
				{
					real scale = (speed-0.02f)*45.454544f;
					real sound_scale;

					if (falling_damage->vehicle_hit_environment_damage_effect.index!=NONE)
					{
						struct damage_data damage_data;

						damage_data_new(&damage_data,
							falling_damage->vehicle_hit_environment_damage_effect.index);

						damage_data.scale = PIN(scale, 0.0f, 1.0f);
						damage_data.origin = vehicle->object.bounding_sphere_center;
						damage_data.direction = delta;

						object_cause_damage(&damage_data, vehicle_index, NONE, NONE, NONE, NULL);
					}

					if (definition->crash_sound.index!=NONE)
					{
						sound_scale = PIN(scale, 0.0f, 1.0f);
						object_impulse_sound_new(vehicle_index, definition->crash_sound.index, NONE,
							global_origin3d, global_forward3d, sound_scale);
					}

					break;
				}
			}
		}
	}

	return;
}

static void update_turret_physics(
	long vehicle_index,
	struct mass_point_datum *mass_points)
{
	physics_update(vehicle_index, NULL, mass_points, NULL, NULL);

	return;
}

static void create_slipping_effects(
	long vehicle_index,
	struct powered_mass_point_datum *state,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);

	if (definition->material_effects.index!=NONE)
	{
		short mass_point_index;

		for (mass_point_index = 0; mass_point_index<physics->mass_points.count; mass_point_index++)
		{
			struct mass_point_datum *mass_point = &mass_points[mass_point_index];
			struct physics_mass_point_definition *mass_point_definition = TAG_BLOCK_GET_ELEMENT(
				&physics->mass_points, mass_point_index, struct physics_mass_point_definition);

			if (TEST_FLAG(mass_point->flags, 1))
			{
				real speed = magnitude3d(&mass_point->velocity_relative_to_ground);

				if (speed>0.03f)
				{
					real scale = (speed-0.03f)*4.5454545f;
					real depth = mass_point->ground_depth-mass_point_definition->unknown68+0.003f;
					real_point3d position;
					real_vector3d normal;
					real inverse;

					position.x = depth*mass_point->ground_plane.n.i+mass_point->position.x;
					position.y = depth*mass_point->ground_plane.n.j+mass_point->position.y;
					position.z = depth*mass_point->ground_plane.n.k+mass_point->position.z;

					inverse = 0.8660254f/speed;

					scale_vector3d(&mass_point->velocity_relative_to_ground, inverse, &normal);

					normal.i += mass_point->ground_plane.n.i*0.5f;
					normal.j += mass_point->ground_plane.n.j*0.5f;
					normal.k += mass_point->ground_plane.n.k*0.5f;

					material_effect_new(definition->material_effects.index,
						TEST_FLAG(mass_point_definition->flags, 0) ? 10 : 9,
						mass_point->ground_material_type, &position, &normal,
						&vehicle->object.location, PIN(scale, 0.0f, 1.0f));
				}
			}
		}
	}

	return;
}

static void update_human_tank_physics(
	long vehicle_index,
	struct mass_point_datum *mass_points,
	struct powered_mass_point_datum *state)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);
	real left = vehicle->vehicle.speed-vehicle->vehicle.turn;
	real right = vehicle->vehicle.turn+vehicle->vehicle.speed;

	vehicle->vehicle.left_tread = left+vehicle->vehicle.left_tread;
	vehicle->vehicle.left_tread = (real)fmod(vehicle->vehicle.left_tread, definition->wheel_circumference);
	if (vehicle->vehicle.left_tread<0.0f)
		vehicle->vehicle.left_tread += definition->wheel_circumference;

	vehicle->vehicle.right_tread = right+vehicle->vehicle.right_tread;
	vehicle->vehicle.right_tread = (real)fmod(vehicle->vehicle.right_tread, definition->wheel_circumference);
	if (vehicle->vehicle.right_tread<0.0f)
		vehicle->vehicle.right_tread += definition->wheel_circumference;

	if (physics->powered_mass_points.count==2)
	{
		state[0].ground_friction_velocity = left;
		set_real_vector2d(&state[0].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[0].rotation_pairs[1], 0.0f, 1.0f);

		state[1].ground_friction_velocity = right;
		set_real_vector2d(&state[1].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[1].rotation_pairs[1], 0.0f, 1.0f);

		physics_update(vehicle_index, state, mass_points, NULL, NULL);
	}
	else
	{
		physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
	}

	return;
}

static real_vector3d *vehicle_cross_product3d_target(
	real_vector3d const *a,
	real_vector3d const *b,
	real_vector3d *result)
{
	real const *bn = b->n;
	real i = bn[2]*a->n[1]-a->n[2]*bn[1];
	real j = a->n[2]*bn[0]-a->n[0]*bn[2];
	real k = bn[1]*a->n[0]-a->n[1]*bn[0];
	result->i = i;
	result->j = j;
	result->k = k;
	return result;
}

static real vehicle_dot_product3d_target(
	real_vector3d const *a,
	real_vector3d const *b)
{
	real const *an = a->n;
	real const *fn = b->n;
	return an[0]*fn[0]+an[1]*fn[1]+an[2]*fn[2];
}

static real vehicle_triple_product3d_target(
	real_vector3d const *velocity,
	real_vector3d const *forward,
	real_vector3d const *normal)
{
	real const *forward_k = &forward->k;
	real const *velocity_j = &velocity->j;
	real const *forward_components = forward->n;
	real const *velocity_components = velocity->n;
	real_vector3d cross;

	cross.i = velocity_components[2]*forward->j-(*velocity_j)*(*forward_k);
	cross.j = forward_components[2]*velocity_components[0]-velocity_components[2]*forward->i;
	cross.k = velocity_components[1]*forward->i-forward->j*velocity->i;
	{
		real j_term;

		j_term = cross.j*normal->j;
		return cross.i*normal->i+j_term+cross.k*normal->k;
	}
}
static void update_human_boat_physics(
	long vehicle_index,
	struct mass_point_datum *mass_points,
	struct powered_mass_point_datum *state)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);

	if (physics->powered_mass_points.count==3)
	{
		real_vector3d zero;
		real_vector3d cross;
		real_vector3d thrust;
		real_vector3d up_relative;
		real dot;
		real angle;
		long sign;
		real speed;
		real negative_k;
		real spin;
		real maximum_angle;
		real *rotation;
		real const *forward;

		speed = (real)fabs(magnitude3d(&vehicle->object.translational_velocity)*2.5f);
		maximum_angle = vehicle->vehicle.turn*0.5f;
		angle = 1.0f-CEILING(speed, 1.0f);

		state[0].water_friction_velocity = vehicle->vehicle.speed;
		state[0].water_lift_ratio = 0.003f;
		angle *= maximum_angle;
		rotation = state[0].rotation_values;
		rotation[0] = 0.0f;
		rotation[2] = sine(angle);
		rotation[1] = 0.0f;
		rotation[3] = cosine(angle);
		zero.i = 0.0f;
		zero.j = 0.0f;
		zero.k = 0.0f;

		forward = vehicle->object.forward.n;

		state[1].water_lift_ratio = 0.003f;
		set_real_vector2d(&state[1].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[1].rotation_pairs[1], 0.0f, 1.0f);

		state[2].water_lift_ratio = 0.005f;
		set_real_vector2d(&state[2].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[2].rotation_pairs[1], 0.0f, 1.0f);

		negative_k = -vehicle->object.forward.k;

		up_relative.i = forward[0]*negative_k+global_up3d->i;
		up_relative.j = forward[1]*negative_k+global_up3d->j;
		up_relative.k = forward[2]*negative_k+global_up3d->k;

		if (normalize3d(&up_relative)!=0.0f)
		{
			vehicle_cross_product3d_target(&vehicle->object.up, &vehicle->object.forward, &cross);

			spin = vehicle_triple_product3d_target(&vehicle->object.translational_velocity,
				&vehicle->object.forward, global_up3d)*2*_pi;

			rotate_vector_about_axis(&up_relative, &vehicle->object.forward,
				sine(spin), cosine(spin));

			angle = angle_between_vectors3d(&up_relative, &vehicle->object.up);

			if (dot_product3d(&cross, &up_relative)>0.0f)
				angle = -angle;

			dot = vehicle_dot_product3d_target(&vehicle->object.angular_velocity, &vehicle->object.forward);

			sign = angle!=0.0f ? (angle<0.0f ? -1 : 1) : 0;

			scale_vector3d(&vehicle->object.forward,
				PIN(square_root((real)fabs(angle)*2.0*VEHICLE_ANGULAR_ACCELERATION)*sign-dot,
					-VEHICLE_ANGULAR_ACCELERATION, VEHICLE_ANGULAR_ACCELERATION)*physics->xx_moment,
				&thrust);
		}
		else
		{
			thrust.i = 0.0f;
			thrust.j = 0.0f;
			thrust.k = 0.0f;
		}

		physics_update(vehicle_index, state, mass_points, &zero, &thrust);
	}
	else
	{
		physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
	}

	return;
}

static void update_human_jeep_physics(
	long vehicle_index,
	struct mass_point_datum *mass_points,
	struct powered_mass_point_datum *state)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);

#ifdef HALO_N64
	/* Physics steps cover two ticks (four at half rate); wheel phase
	must cover the same signed motor travel as that step. */
	extern real n64_vehicle_physics_step(void);
	vehicle->vehicle.wheel = vehicle->vehicle.speed*n64_vehicle_physics_step()+vehicle->vehicle.wheel;
#else
	vehicle->vehicle.wheel = vehicle->vehicle.speed+vehicle->vehicle.wheel;
#endif
	vehicle->vehicle.wheel = (real)fmod(vehicle->vehicle.wheel, definition->wheel_circumference);
	if (vehicle->vehicle.wheel<0.0f)
		vehicle->vehicle.wheel += definition->wheel_circumference;

	if (physics->powered_mass_points.count==2)
	{
		real angle = vehicle->vehicle.turn*0.5f;
		real cs = cosine(angle);
		real sn = sine(angle);

		state[0].ground_friction_velocity = vehicle->vehicle.speed;
		set_real_vector2d(&state[0].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[0].rotation_pairs[1], sn, cs);

		state[1].ground_friction_velocity = vehicle->vehicle.speed;
		set_real_vector2d(&state[1].rotation_pairs[0], 0.0f, 0.0f);
		set_real_vector2d(&state[1].rotation_pairs[1], -sn, cs);

		physics_update(vehicle_index, state, mass_points, NULL, NULL);
	}
	else
	{
		physics_update(vehicle_index, NULL, mass_points, NULL, NULL);
	}

	return;
}

static void update_alien_scout_physics(
	long vehicle_index,
	real steering,
	struct powered_mass_point_datum *state,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct vehicle_definition *definition = vehicle_specific_definition_get(
		vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(
		definition->unit.object.physics.index);
	real_matrix4x3 matrix;
	real_vector3d local_velocity;
	real_vector3d magic_force;
	real_vector3d magic_torque;
	real antigrav;
	real water_depth;
	short mass_point_index;

	water_depth = scenario_location_water_depth(&vehicle->object.location,
		&vehicle->object.position);

	magic_force = *global_zero_vector3d;
	magic_torque = *global_zero_vector3d;
	antigrav = vehicle->unit.seat_power[0];

	for (mass_point_index = 0;
		mass_point_index<physics->powered_mass_points.count;
		mass_point_index++)
	{
		state[mass_point_index].antigrav_fraction = antigrav;
		state[mass_point_index].rotation.v.i = 0.0f;
		state[mass_point_index].rotation.v.j = 0.0f;
		state[mass_point_index].rotation.v.k = 0.0f;
		state[mass_point_index].rotation.w = 1.0f;
	}

	if (water_depth<0.5f && vehicle->object.up.k>-0.2f)
	{
		real_vector3d const *object_forward = &vehicle->object.forward;
		real_vector3d const *object_up = &vehicle->object.up;
		real_vector3d const *object_angular_velocity =
			&vehicle->object.angular_velocity;
		matrix4x3_from_point_and_vectors(&matrix, &vehicle->object.position,
			object_forward, object_up);
		matrix4x3_inverse_transform_vector(&matrix,
			&vehicle->object.translational_velocity, &local_velocity);

		if (vehicle->vehicle.hover>0.0f)
		{
			real maximum_speed = definition->unknown2f8;
			real maximum_acceleration;
			real_vector2d target_velocity;
			real_vector3d acceleration;
			real scale;

			if (TEST_FLAG(vehicle->vehicle.flags, 3))
				maximum_speed *= 0.8f;

			maximum_acceleration = definition->unknown300;

			target_velocity.i = maximum_speed*vehicle->unit.throttle.i;
			target_velocity.j = maximum_speed*vehicle->unit.throttle.j;
			acceleration.i = target_velocity.i-local_velocity.i;
			acceleration.j = target_velocity.j-local_velocity.j;
			acceleration.k = 0.0f;

			if (vehicle->vehicle.on_ground_ticks>0 && fabs(steering)>0.785398185f)
			{
				real reduction = MIN(vehicle->vehicle.on_ground_ticks*0.05f, 0.98f);
				maximum_acceleration *= 1.0f-reduction;
			}

			limit3d(&acceleration, maximum_acceleration);
			matrix4x3_transform_vector(&matrix, &acceleration, &acceleration);

			scale = physics->mass*vehicle->vehicle.hover;
			magic_force.i += acceleration.i*scale;
			magic_force.j += acceleration.j*scale;
			magic_force.k += acceleration.k*scale;
		}

		if (vehicle->vehicle.hover>0.0f)
		{
			real current = dot_product3d(object_up,
				object_angular_velocity);
			long sign = steering!=0.0f ? (steering<0.0f ? -1 : 1) : 0;
			real desired = square_root((real)(fabs(steering)*0.0069813174f))*sign;
			real error;
			real torque;

			if (fabs(desired)>_real_epsilon && steering/desired<2.0f)
				desired = steering*0.5f;

			error = PIN(desired-current, -0.0034906587f, 0.0034906587f);
			torque = error*physics->zz_moment;
			torque *= vehicle->vehicle.hover;

			magic_torque.i += object_up->i*torque;
			magic_torque.j += object_up->j*torque;
			magic_torque.k += object_up->k*torque;
		}

		if (vehicle->vehicle.hover<1.0f)
		{
			real_vector3d left;
			real_vector2d forward2d;
			real_vector2d left2d;
			real_vector2d control_torque = *global_zero_vector2d;
			real torque_a;
			real torque_b;

			vehicle_cross_product3d_target(object_up, object_forward, &left);
			forward2d.i = object_forward->i;
			forward2d.j = object_forward->j;
			left2d.i = left.i;
			left2d.j = left.j;
			normalize2d(&forward2d);
			normalize2d(&left2d);

			if (object_up->k>0.0f)
			{
				real_vector2d level_torque;
				real_vector2d alignment;
				real_vector2d rate;
				real_vector2d up;
				real_vector2d angular_velocity;
				long sign_a;
				long sign_b;
				real weight_a;
				real weight_b;

				up.i = object_up->i;
				up.j = object_up->j;
				angular_velocity.i = object_angular_velocity->i;
				angular_velocity.j = object_angular_velocity->j;
				alignment.i = dot_product2d(&up, &forward2d);
				alignment.j = dot_product2d(&up, &left2d);
				rate.i = dot_product2d(&angular_velocity, &left2d);
				rate.j = -dot_product2d(&angular_velocity, &forward2d);
				level_torque = *global_zero_vector2d;
				level_torque.i -= alignment.i;
				level_torque.j -= alignment.j;
				level_torque.i -= 15.0f*rate.i;
				level_torque.j -= 15.0f*rate.j;
				sign_a = vehicle->unit.throttle.i*level_torque.i!=0.0f ?
					(vehicle->unit.throttle.i*level_torque.i<0.0f ? -1 : 1) : 0;
				weight_a = (real)fabs(level_torque.i)*sign_a;
				sign_b = vehicle->unit.throttle.j*level_torque.j!=0.0f ?
					(vehicle->unit.throttle.j*level_torque.j<0.0f ? -1 : 1) : 0;
				weight_b = (real)fabs(level_torque.j)*sign_b;
				control_torque.i += vehicle->unit.throttle.i*
					PIN(weight_a+1.0f, 0.3f, 2.5f)*0.0015514038f;
				control_torque.j += vehicle->unit.throttle.j*
					PIN(weight_b+1.0f, 0.3f, 2.5f)*0.0015514038f;
				{
					real level_scale = (1.0f-object_up->k)*0.0038785094f;

					torque_a = level_scale*level_torque.i;
					torque_b = level_scale*level_torque.j;
					torque_a += control_torque.i;
				}
			}
			else
			{
				torque_a = vehicle->unit.throttle.i*0.0015514038f;
				torque_b = vehicle->unit.throttle.j*0.0015514038f;
				torque_a += control_torque.i;
			}

			torque_b += control_torque.j;

			{
				real_vector3d torque = *global_zero_vector3d;
				real left_scale = physics->yy_moment*torque_a;
				real forward_scale = -(physics->xx_moment*torque_b);
				real scale = 1.0f-vehicle->vehicle.hover;

				torque.i += left.i*left_scale;
				torque.j += left.j*left_scale;
				torque.k += left.k*left_scale;
				torque.i += object_forward->i*forward_scale;
				torque.j += object_forward->j*forward_scale;
				torque.k += object_forward->k*forward_scale;

				magic_torque.i += torque.i*scale;
				magic_torque.j += torque.j*scale;
				magic_torque.k += torque.k*scale;
			}
		}

		if (TEST_FLAG(vehicle->vehicle.flags, 3))
		{
			real speed = dot_product3d(object_forward,
				&vehicle->object.translational_velocity)/definition->unknown2f8;
			real_vector3d left;

			speed = PIN(speed, 0.0f, 1.0f);

			cross_product3d(object_up, object_forward, &left);

			if (speed>0.0f)
			{
				real torque = physics->yy_moment*speed*vehicle->vehicle.hover*-0.005817764f;
				real lift;

				magic_torque.i += left.i*torque;
				magic_torque.j += left.j*torque;
				magic_torque.k += left.k*torque;

				lift = physics->mass*speed*vehicle->vehicle.hover*0.004f;
				magic_force.i += global_up3d->i*lift;
				magic_force.j += global_up3d->j*lift;
				magic_force.k += global_up3d->k*lift;
			}

			if (vehicle->vehicle.airborne_ticks>0)
			{
				real_vector3d axis;

				cross_product3d(&left, global_up3d, &axis);
				if (normalize3d(&axis)>0.0f)
				{
					real fade = PIN(1.0f-vehicle->vehicle.airborne_ticks*(1.0f/30), 0.0f, 1.0f);
					real axis_scale = (1.0f-vehicle->vehicle.hover)*
						physics->mass*fade*0.002f;
					real up_scale;

					magic_force.i += axis.i*axis_scale;
					magic_force.j += axis.j*axis_scale;
					magic_force.k += axis.k*axis_scale;

					up_scale = (1.0f-vehicle->vehicle.hover)*
						physics->mass*fade*0.001f;
					magic_force.i += global_up3d->i*up_scale;
					magic_force.j += global_up3d->j*up_scale;
					magic_force.k += global_up3d->k*up_scale;
				}
			}
		}

		magic_force.i *= antigrav;
		magic_force.j *= antigrav;
		magic_force.k *= antigrav;
		magic_torque.i *= antigrav;
		magic_torque.j *= antigrav;
		magic_torque.k *= antigrav;
	}

	physics_update(vehicle_index, state, mass_points, &magic_force, &magic_torque);

	{
		real maximum = MAX(vehicle->object.up.k, 0.4f);
		long powered_count = 0;
		long grounded_count = 0;
		real ratio = 0.0f;
		real target;

		for (mass_point_index = 0;
			mass_point_index<physics->mass_points.count;
			mass_point_index++)
		{
			struct physics_mass_point_definition *mass_point_definition = TAG_BLOCK_GET_ELEMENT(
				&physics->mass_points, mass_point_index, struct physics_mass_point_definition);

			if (mass_point_definition->powered_mass_point_index!=NONE)
			{
				powered_count++;
				if (TEST_FLAG(mass_points[mass_point_index].flags, 4))
					grounded_count++;
			}
		}

		if ((short)powered_count>0)
			ratio = (real)(short)grounded_count/(real)(short)powered_count;

		target = PIN(ratio*maximum, 0.0f, 1.0f);
		if (target-vehicle->vehicle.hover>0.1f)
			target = vehicle->vehicle.hover+0.1f;
		else if (target-vehicle->vehicle.hover<-0.1f)
			target = vehicle->vehicle.hover-0.1f;

		vehicle->vehicle.hover = target;
	}

	create_ghost_effect(vehicle_index);

	return;
}

static void update_alien_fighter_physics(
	long vehicle_index,
	struct powered_mass_point_datum *state,
	struct mass_point_datum *mass_points)
{
	struct vehicle_datum *vehicle = vehicle_datum_get(vehicle_index);
	struct unit_definition *definition = vehicle_definition_get(vehicle->definition_index);
	struct physics_definition *physics = physics_definition_get(definition->object.physics.index);

	if (physics->radius>0.0f)
		update_alien_fighter_physics_old(vehicle_index, mass_points, state);
	else
		update_alien_fighter_physics_new(vehicle_index, state, mass_points);

	create_ghost_effect(vehicle_index);

	return;
}

/* Full semantic reconstruction. January's vehicle_update is 2320 bytes; this
body has the same padded size and 98 relocations. The remaining residual is
instruction scheduling, branch layout, and relocation placement. Keep the
file-static call topology intact while closing it. */

boolean vehicle_update(
	long vehicle_index)
{
	struct mass_point_datum mass_points[32];
	struct powered_mass_point_datum powered_mass_points[32];
	struct vehicle_datum *vehicle;
	real steering_angle;
	real torque;
	struct vehicle_definition *definition;

	vehicle = vehicle_datum_get(vehicle_index);
	definition = vehicle_specific_definition_get(vehicle->definition_index);

	profile_enter(vehicle_update_section);

	if (vehicle->object.parent_object_index!=NONE)
	{
		unsigned long *object_flags = &vehicle->object.flags;

		vehicle->object.angular_velocity.i = 0.0f;
		vehicle->object.angular_velocity.j = 0.0f;
		vehicle->object.angular_velocity.k = 0.0f;
		vehicle->object.translational_velocity.i = 0.0f;
		vehicle->object.translational_velocity.j = 0.0f;
		vehicle->object.translational_velocity.k = 0.0f;
		*object_flags &= ~FLAG(_object_at_rest_bit);
		goto animate;
	}

	SET_FLAG(vehicle->vehicle.flags, 2,
		TEST_FLAG(vehicle->unit.control_flags, _unit_control_crouch_modifier_bit));

	SET_FLAG(vehicle->vehicle.flags, 3,
		TEST_FLAG(vehicle->unit.control_flags, _unit_control_jump_bit) ||
		(TEST_FLAG(definition->flags, 4) &&
			((vehicle->unit.throttle.i>0.0f && vehicle->vehicle.speed<0.0f) ||
			(vehicle->unit.throttle.i<0.0f && vehicle->vehicle.speed>0.0f))));

	{
		real_vector3d left;
		real steer_cross;
		real steer_dot;

		cross_product3d(&vehicle->object.up, &vehicle->object.forward, &left);
		steer_cross = dot_product3d(&left, &vehicle->unit.desired_facing_vector);
		steer_dot = dot_product3d(&vehicle->unit.desired_facing_vector, &vehicle->object.forward);
		steering_angle = arctangent(steer_cross, steer_dot);
	}

	{
		if (TEST_FLAG(vehicle->vehicle.flags, 4) &&
			vehicle->vehicle.upending_type &&
			vehicle->vehicle.upending_ticks<30 &&
			vehicle->object.up.k<=0.9f)
		{
			real_vector3d torque_axis;
			real roll;

			torque = (vehicle->vehicle.upending_type==2 ||
				vehicle->vehicle.upending_type==4) ? 0.3 : -0.3;

			if (vehicle->vehicle.upending_type==4 || vehicle->vehicle.upending_type==3)
				cross_product3d(&vehicle->object.forward, &vehicle->object.up, &torque_axis);
			else
				torque_axis = vehicle->object.forward;

			roll = vehicle->object.up.k*-2.0f;
			if (roll<definition->unknown340)
				roll = definition->unknown340;
			else if (roll>definition->unknown344)
				roll = definition->unknown344;

			torque *= roll;
			vehicle->object.flags &= ~FLAG(_object_at_rest_bit);

			if (vehicle->vehicle.upending_type==2 || vehicle->vehicle.upending_type==1)
			{
				real_vector3d cross;

				cross_product3d(&vehicle->object.forward, &vehicle->object.up, &cross);
				point_from_line3d(
					(real_point3d *)&torque_axis,
					&cross,
					-vehicle->object.forward.k,
					(real_point3d *)&torque_axis);
			}

			scale_vector3d(&torque_axis, torque, &vehicle->object.angular_velocity);

			switch (definition->vehicle_type)
			{
			case _vehicle_type_alien_fighter:
				vehicle->object.translational_velocity.k =
					MIN(-0.01f, vehicle->object.translational_velocity.k);
				break;
			case _vehicle_type_human_tank:
				scale_vector3d(
					&vehicle->object.forward,
					dot_product3d(
						&vehicle->object.forward,
						&vehicle->object.translational_velocity),
					&vehicle->object.translational_velocity);
				break;
			}

			vehicle->vehicle.upending_ticks++;
		}
		else
		{
			vehicle->vehicle.upending_ticks = 0;
			vehicle->vehicle.upending_type = 0;
			vehicle->vehicle.flags &= ~FLAG(4);
		}
	}

	{
		struct physics_variable_speed_parameters *speed_parameters =
			(struct physics_variable_speed_parameters *)&definition->unknown2f8;

		if (TEST_FLAG(vehicle->vehicle.flags, 3))
		{
			physics_variable_speed_update_seek(
				&vehicle->vehicle.speed, speed_parameters, 0.0f, 1.0f);
		}
		else
		{
			physics_variable_speed_update_seek(
				&vehicle->vehicle.speed, speed_parameters, vehicle->unit.throttle.i, 1.0f);
			physics_variable_speed_update_seek(
				&vehicle->vehicle.slide,
				(struct physics_variable_speed_parameters *)&definition->unknown330,
				vehicle->unit.throttle.j, 1.0f);
		}

		if (definition->vehicle_type)
		{
			real desired_position = vehicle->vehicle.speed<0.0f
				? -steering_angle
				: steering_angle;

			if (desired_position<DEGREES_TO_RADIANS(definition->unknown30c))
				desired_position = DEGREES_TO_RADIANS(definition->unknown30c);
			else if (desired_position>DEGREES_TO_RADIANS(definition->unknown308))
				desired_position = DEGREES_TO_RADIANS(definition->unknown308);

			physics_variable_position_update_seek(
				&vehicle->vehicle.turn,
				&definition->unknown308,
				FALSE,
				desired_position,
				DEGREES_TO_RADIANS(definition->unknown314)*(1.0f/TICKS_PER_SECOND));
		}
		else
		{
			if (vehicle->vehicle.speed==0.0f)
			{
				physics_variable_speed_update_seek(
					&vehicle->vehicle.turn, speed_parameters, 0.0f, 1.0f);
			}
			else
			{
				physics_variable_speed_update_seek(
					&vehicle->vehicle.turn,
					speed_parameters,
					PIN(steering_angle*0.63661975f, -1.0f, 1.0f)*definition->unknown2f8,
					2.0f);
			}
		}
	}

	if (definition->unit.object.physics.index!=NONE)
	{
		if ((TEST_FLAG(definition->flags, 0) && vehicle->vehicle.speed!=0.0f) ||
			(TEST_FLAG(definition->flags, 1) && vehicle->vehicle.turn!=0.0f) ||
			(TEST_FLAG(definition->flags, 2) && vehicle->unit.seat_power[0]!=0.0f) ||
			(TEST_FLAG(definition->flags, 3) && vehicle->unit.seat_power[1]!=0.0f) ||
			(TEST_FLAG(definition->flags, 5) && vehicle->vehicle.slide!=0.0f))
		{
			vehicle->object.flags &= ~FLAG(_object_at_rest_bit);
		}
	}

	match_assert("c:\\halo\\SOURCE\\units\\vehicles.c", 308,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] =
		_collision_user_vehicles;

#ifdef HALO_N64
	/* (its physics on its turn only: every other tick, every fourth with
	ANIMATIONS HALF RATE -- physics/physics.c; the mass points below are
	filled by the step, so the slipping, suspension, crashing and airborne
	updates that read them wait for it too) */
	extern boolean n64_vehicle_physics_turn(long object_index);

	if (definition->unit.object.physics.index!=NONE &&
		!TEST_FLAG(vehicle->object.flags, _object_at_rest_bit) &&
		n64_vehicle_physics_turn(vehicle_index))
#else
	if (definition->unit.object.physics.index!=NONE &&
		!TEST_FLAG(vehicle->object.flags, _object_at_rest_bit))
#endif
	{
		real_vector3d previous_velocity = vehicle->object.translational_velocity;

		switch (definition->vehicle_type)
		{
		case _vehicle_type_human_tank:
			update_human_tank_physics(vehicle_index, mass_points, powered_mass_points);
			break;
		case _vehicle_type_human_jeep:
			update_human_jeep_physics(vehicle_index, mass_points, powered_mass_points);
			break;
		case _vehicle_type_human_boat:
			update_human_boat_physics(vehicle_index, mass_points, powered_mass_points);
			break;
		case _vehicle_type_human_plane:
			update_human_plane_physics(vehicle_index, powered_mass_points, mass_points);
			break;
		case _vehicle_type_alien_scout:
			update_alien_scout_physics(vehicle_index, steering_angle, powered_mass_points, mass_points);
			break;
		case _vehicle_type_alien_fighter:
			update_alien_fighter_physics(vehicle_index, powered_mass_points, mass_points);
			break;
		case _vehicle_type_turret:
			update_turret_physics(vehicle_index, mass_points);
			break;
		}

		create_slipping_effects(vehicle_index, powered_mass_points, mass_points);
		if (!update_suspension(vehicle_index))
			create_crashing_effects(vehicle_index, &previous_velocity, mass_points);
		compute_airborne_ticks(vehicle_index, mass_points, powered_mass_points);

		if (TEST_FLAG(vehicle->object.flags, _object_at_rest_bit))
			vehicle->vehicle.stop_time = 15;

		if (!TEST_FLAG(vehicle->object.flags, _object_no_collisions_bit) &&
			TEST_FLAG(FLAG(_vehicle_type_human_plane) | FLAG(_vehicle_type_alien_fighter),
				definition->vehicle_type))
		{
			real floor = global_structure_bsp_get()->vehicle_floor;
			real ceiling = global_structure_bsp_get()->vehicle_ceiling;

			if (floor!=0.0f && vehicle->object.position.z<floor)
			{
				vehicle->object.translational_velocity.k +=
					((floor-vehicle->object.position.z)*0.015625f-
					vehicle->object.translational_velocity.k*0.0625f)*
					vehicle->unit.seat_power[0];
			}

			if (ceiling!=0.0f && vehicle->object.position.z>ceiling)
			{
				vehicle->object.translational_velocity.k -=
					((vehicle->object.position.z-ceiling)*0.015625f+
					vehicle->object.translational_velocity.k*0.0625f)*
					vehicle->unit.seat_power[0];
			}
		}
	}
	else if (vehicle->vehicle.stop_time>0)
	{
		slowly_stop_vehicle(vehicle_index);
		update_suspension(vehicle_index);
	}

	match_assert("c:\\halo\\SOURCE\\units\\vehicles.c", 372,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	if (TEST_FLAG(definition->flags, 6))
	{
		struct game_globals_falling_damage *falling_damage = TAG_BLOCK_GET_ELEMENT(
			&scenario_get_game_globals()->falling_damage, 0, struct game_globals_falling_damage);

		if (vehicle->object.translational_velocity.k<
			-falling_damage->runtime_maximum_falling_velocity)
		{
			long child_index = vehicle->object.first_child_object_index;

			while (child_index!=NONE)
			{
				struct object_datum *child = object_get(child_index);
				struct damage_data damage_data;

				damage_data_new(&damage_data, falling_damage->maximum_distance_damage.index);
				object_cause_damage(&damage_data, child_index, NONE, NONE, NONE, NULL);
				child_index = child->object.next_object_index;
			}
		}
	}

animate:
	if (definition->unit.object.animation_graph.index!=NONE)
	{
		byte animation_update[2];

		animation_update[0] = 0;
		animation_update[1] = 0;
		unit_update_animation(vehicle_index, animation_update);
	}

	{
		boolean blur = fabs(vehicle->vehicle.speed)>=definition->unknown318;

		if (blur!=TEST_FLAG(vehicle->vehicle.flags, 0))
		{
			object_permute_region(vehicle_index, "~blur", NONE, blur);
			SET_FLAG(vehicle->vehicle.flags, 0, blur);
		}
	}

	profile_exit(vehicle_update_section);

	return TRUE;
}
