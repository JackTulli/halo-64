/*
AIM_ASSIST.C

symbols in this file:
00093C00 0050:
	_compute_attenuation (0000)
00093C50 0030:
	_compute_composite_attenuation (0000)
00093C80 00f0:
	_unit_get_aim_assist_parameters (0000)
00093D70 00a0:
	_compare_targets (0000)
00093E10 0010:
	_reciprocal_square_root (0000)
00093E20 0060:
	_limit3d (0000)
00093E80 0020:
	_set_real_euler_angles2d (0000)
00093EA0 00f0:
	_aim_assist_clear_line_of_sight (0000)
00093F90 01a0:
	_object_compute_autoaim_target (0000)
00094130 01a0:
	_aim_assist_compute_target (0000)
000942D0 0110:
	_autoaim_compute_target (0000)
000943E0 0190:
	_find_aim_assist_targets_recursive (0000)
00094570 0130:
	_find_aim_assist_targets (0000)
000946A0 0100:
	_aim_assist (0000)
000947A0 0340:
	_player_aim_projectile (0000)
00094AE0 0170:
	_local_player_aim_assist (0000)
0025AC3C 0021:
	??_C@_0CB@CDDNCJJI@c?3?2halo?2SOURCE?2game?2aim_assist?4c@ (0000)
0025AC60 0004:
	__real@43000000 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#ifdef HALO_N64
#include "../../port/n64/src/n64.h"
#endif
#include "game/aim_assist.h"

#include "camera/director.h"
#include "camera/observer.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/game_globals.h"
#include "game/player_control.h"
#include "game/players.h"
#include "math/real_math_cones.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "objects/objects.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "structures/structures.h"
#include "tag_files/tag_groups.h"
#include "units/biped_definitions.h"
#include "units/bipeds.h"
#include "units/unit_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	MAXIMUM_AIM_ASSIST_TARGETS= 64,
	MAXIMUM_AIM_ASSIST_CLUSTERS= 512,
	MAXIMUM_AIM_ASSIST_OBJECTS= 2048
};

#ifdef HALO_N64
/* the N64's stronger aim assist (unit_get_aim_assist_parameters): the
weapons' cones wider, their reach longer (player_control.c makes the aim
stick harder too) */
#define N64_AIM_ASSIST_ANGLE_SCALE 1.75f
#define N64_AIM_ASSIST_DISTANCE_SCALE 1.25f
#endif

/* ---------- macros */

/* ---------- structures */

struct aim_assist_target
{
	long object_index;
	real_point3d position;
	real_vector3d vector;
	real_vector3d direction;
	real distance;
	real angle;
	real autoaim_level;
	real magnetism_level;
};

/* ---------- prototypes */

__inline real compute_attenuation(
	real variable,
	real maximum);
real compute_composite_attenuation(
	real distance,
	real maximum_distance,
	real angle,
	real maximum_angle);
static boolean unit_get_aim_assist_parameters(
	long unit_index,
	short zoom_level,
	struct aim_assist_parameters *parameters);
static int compare_targets(
	void const *target0,
	void const *target1);
static void object_compute_autoaim_target(
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real_point3d *target);
boolean aim_assist_compute_target(
	struct aim_assist_parameters const *parameters,
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	struct aim_assist_target *target);
static short find_aim_assist_targets_recursive(
	struct aim_assist_parameters const *parameters,
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real distance,
	real angle_sine,
	real angle_cosine,
	long ignore_object_index,
	short ignore_team_index,
	short maximum_target_count,
	struct aim_assist_target *targets);
static short find_aim_assist_targets(
	struct aim_assist_parameters const *parameters,
	short cluster_index,
	real_point3d const *position,
	real_vector3d const *direction,
	long ignore_object_index,
	short ignore_team_index,
	short maximum_target_count,
	struct aim_assist_target *targets);
boolean aim_assist(
	struct aim_assist_parameters const *parameters,
	real_point3d const *position,
	real_vector3d const *direction,
	long ignore_object_index,
	short ignore_team_index,
	struct aim_assist_target *target);

/* ---------- globals */

/* ---------- code (definitions in January object order) */

__inline real compute_attenuation(
	real variable,
	real maximum)
{
	real half_maximum= maximum*0.5f;

	if (variable>=maximum)
	{
		return 0.f;
	}

	if (variable<=half_maximum)
	{
		return 1.f;
	}

	return (maximum - variable) / (maximum - half_maximum);
}

real compute_composite_attenuation(
	real distance,
	real maximum_distance,
	real angle,
	real maximum_angle)
{
	return compute_attenuation(distance, maximum_distance) * compute_attenuation(angle, maximum_angle);
}

static boolean unit_get_aim_assist_parameters(
	long unit_index,
	short zoom_level,
	struct aim_assist_parameters *parameters)
{
	if (unit_index!=NONE)
	{
		struct unit_datum *unit= unit_get(unit_index);
		long weapon_index= unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);

		if (weapon_index!=NONE)
		{
			struct weapon_datum *weapon= weapon_get(weapon_index);
			struct weapon_definition *weapon_definition= weapon_definition_get(weapon->definition_index);

			if (zoom_level!=NONE || !TEST_FLAG(weapon_definition->weapon.flags, _weapon_aim_assists_only_when_zoomed_bit))
			{
				real magnification= weapon_get_zoom_magnification(weapon_index, zoom_level);
				real inverse_magnification= 1.f/magnification;

				parameters->autoaim_angle= weapon_definition->weapon.aim_assist_parameters.autoaim_angle*inverse_magnification;
				parameters->autoaim_distance= weapon_definition->weapon.aim_assist_parameters.autoaim_distance*magnification;
				parameters->magnetism_angle= weapon_definition->weapon.aim_assist_parameters.magnetism_angle*inverse_magnification;
				parameters->magnetism_distance= weapon_definition->weapon.aim_assist_parameters.magnetism_distance*magnification;
				parameters->deviation_angle= MAX(weapon_definition->weapon.aim_assist_parameters.deviation_angle,
					weapon_definition->weapon.aim_assist_parameters.autoaim_angle)*inverse_magnification;
#ifdef HALO_N64
				/* the N64's controls are coarser than the Xbox's two sticks (one
				stick; C buttons on or off): bullets bend toward targets, and
				the aim sticks to them, in wider cones and further away; the
				bullets may bend as far as the wider cone */
				parameters->autoaim_angle*= N64_AIM_ASSIST_ANGLE_SCALE;
				parameters->magnetism_angle*= N64_AIM_ASSIST_ANGLE_SCALE;
				parameters->autoaim_distance*= N64_AIM_ASSIST_DISTANCE_SCALE;
				parameters->magnetism_distance*= N64_AIM_ASSIST_DISTANCE_SCALE;
				parameters->deviation_angle= MAX(parameters->deviation_angle, parameters->autoaim_angle);
#endif

				return TRUE;
			}
		}
	}

	return FALSE;
}

static int compare_targets(
	void const *target0,
	void const *target1)
{
	struct aim_assist_target const *first= target0;
	struct aim_assist_target const *second= target1;

	if (first->autoaim_level>second->autoaim_level) return -1;
	if (first->autoaim_level<second->autoaim_level) return 1;

	if (first->magnetism_level>second->magnetism_level) return -1;
	if (first->magnetism_level<second->magnetism_level) return 1;

	if (first->distance<second->distance) return -1;
	if (first->distance>second->distance) return 1;

	if (first->angle<second->angle) return -1;
	if (first->angle>second->angle) return 1;

	return DATUM_INDEX_TO_ABSOLUTE_INDEX(first->object_index) - DATUM_INDEX_TO_ABSOLUTE_INDEX(second->object_index);
}

boolean aim_assist_clear_line_of_sight(
	real_point3d const *point0,
	real_point3d const *point1,
	long ignore_object_index,
	long target_object_index)
{
	boolean clear = FALSE;
	real_vector3d vector;
	struct collision_result collision;
	long ignore_parent_index;

	match_assert(
		"c:\\halo\\SOURCE\\game\\aim_assist.c",
		350,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = _collision_user_aim_assist;

	ignore_parent_index = object_get_ultimate_parent(ignore_object_index);
	vector.i = point1->x - point0->x;
	vector.j = point1->y - point0->y;
	vector.k = point1->z - point0->z;

	if (!collision_test_vector(
		_collision_test_for_line_of_sight_flags,
		point0,
		&vector,
		ignore_parent_index,
		&collision))
	{
		clear = TRUE;
	}
	else if (collision.type == _collision_result_object &&
		object_get_ultimate_parent(collision.object_index) ==
		object_get_ultimate_parent(target_object_index))
	{
		clear = TRUE;
	}

	match_assert(
		"c:\\halo\\SOURCE\\game\\aim_assist.c",
		367,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	return clear;
}

static void object_compute_autoaim_target(
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real_point3d *target)
{
	real width;

	// closest point on the pill axis to the aiming line
	{
		real_point3d base;
		real_vector3d height, normal;
		real normal_magnitude_squared;

		biped_get_autoaim_pill(object_index, &base, &height, &width);
		normal.i= height.j*direction->k - height.k*direction->j;
		normal.j= height.k*direction->i - height.i*direction->k;
		normal.k= height.i*direction->j - height.j*direction->i;
		normal_magnitude_squared= magnitude_squared3d(&normal);
		if (normal_magnitude_squared>0.f)
		{
			real_vector3d offset, cross;
			real t;

			vector_from_points3d(&base, position, &offset);
			cross_product3d(&offset, direction, &cross);
			t= PIN(dot_product3d(&cross, &normal)/normal_magnitude_squared, 0.f, 1.f);
			target->x= height.i*t + base.x;
			target->y= height.j*t + base.y;
			target->z= height.k*t + base.z;
		}
		else
		{
			*target= base;
		}
	}

	// pull the target toward the aiming line by at most the pill width
	{
		real_vector3d offset, perpendicular;
		real projection;

		vector_from_points3d(position, target, &offset);
		projection= -dot_product3d(&offset, direction);
		perpendicular.i= direction->i*projection + offset.i;
		perpendicular.j= direction->j*projection + offset.j;
		perpendicular.k= direction->k*projection + offset.k;
		limit3d(&perpendicular, width);
		target->x-= perpendicular.i;
		target->y-= perpendicular.j;
		target->z-= perpendicular.k;
	}

	return;
}

boolean aim_assist_compute_target(
	struct aim_assist_parameters const *parameters,
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	struct aim_assist_target *target)
{
	real cosine, angle;

	target->object_index= object_index;
	object_compute_autoaim_target(object_index, position, direction, &target->position);
	vector_from_points3d(position, &target->position, &target->vector);
	target->direction= target->vector;
	target->distance= normalize3d(&target->direction);
	cosine= dot_product3d(direction, &target->direction);
	angle= arccosine(PIN(cosine, -1.f, 1.f));
	target->angle= angle;

	if (parameters)
	{
		target->autoaim_level= compute_composite_attenuation(target->distance, parameters->autoaim_distance,
			angle, parameters->autoaim_angle);
		target->magnetism_level= compute_composite_attenuation(target->distance, parameters->magnetism_distance,
			angle, parameters->magnetism_angle);
		if (target->magnetism_level>0.f)
		{
			struct unit_datum *unit= unit_get(target->object_index);
			struct unit_definition *definition= unit_definition_get(unit->definition_index);

			if (TEST_FLAG(definition->unit.flags, _unit_is_inconsequential_bit))
			{
				target->magnetism_level*= TAG_BLOCK_GET_ELEMENT(&scenario_get_game_globals()->player_control, 0,
					struct game_globals_player_control)->magnetism_inconsequential_target_scale;
			}
		}
	}
	else
	{
		target->autoaim_level= 0.f;
		target->magnetism_level= 0.f;
	}

	return target->autoaim_level>0.f || target->magnetism_level>0.f;
}

boolean autoaim_compute_target(
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	long unit_index,
	real_point3d *target_position,
	real_vector3d *target_direction,
	real *target_distance,
	real *target_angle)
{
	boolean result= FALSE;

	object_compute_autoaim_target(object_index, position, direction, target_position);
	if (aim_assist_clear_line_of_sight(position, target_position, unit_index, object_index))
	{
		vector_from_points3d(position, target_position, target_direction);
		*target_distance= normalize3d(target_direction);
		if (*target_distance!=0.f)
		{
			*target_angle= arccosine(PIN(dot_product3d(direction, target_direction), -1.f, 1.f));
			result= TRUE;
		}
	}

	return result;
}

#ifdef HALO_N64
/* autoaim_compute_target with the angle tested before the ray: game_engine.c's
player under the crosshair cast one to every unit along the view, for each
view's HUD, though only those within a small angle counted. The same
answer: TRUE when the angle is under maximum_angle and the line is clear */
boolean n64_autoaim_compute_target_within(
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	long unit_index,
	real maximum_angle,
	real_point3d *target_position,
	real_vector3d *target_direction,
	real *target_distance,
	real *target_angle)
{
	object_compute_autoaim_target(object_index, position, direction, target_position);
	vector_from_points3d(position, target_position, target_direction);
	*target_distance= normalize3d(target_direction);
	if (*target_distance==0.f)
		return FALSE;
	*target_angle= arccosine(PIN(dot_product3d(direction, target_direction), -1.f, 1.f));
	if (!(fabs(*target_angle)<maximum_angle))
		return FALSE;
	return aim_assist_clear_line_of_sight(position, target_position, unit_index, object_index);
}
#endif

static short find_aim_assist_targets_recursive(
	struct aim_assist_parameters const *parameters,
	long object_index,
	real_point3d const *position,
	real_vector3d const *direction,
	real distance,
	real angle_sine,
	real angle_cosine,
	long ignore_object_index,
	short ignore_team_index,
	short maximum_target_count,
	struct aim_assist_target *targets)
{
	short target_count= 0;

	do
	{
		struct object_datum *object= object_get(object_index);

		if (TEST_FLAG(_object_mask_unit, object->object.type) &&
			!TEST_FLAG(object->object.flags, _object_invisible_bit) &&
			unit_get(object_index)->unit.active_camouflage<1.f &&
			sphere_intersects_cone3d(&object->object.bounding_sphere_center, object->object.bounding_sphere_radius,
				position, direction, distance, angle_sine, angle_cosine))
		{
			if (TEST_FLAG(_object_mask_biped, object->object.type) &&
				!TEST_FLAG(object->object.damage_flags, _object_dead_bit) &&
				object_index!=ignore_object_index &&
				game_team_is_enemy(ignore_team_index, object->object.owner_team_index) &&
				!TEST_FLAG(biped_definition_get(object->definition_index)->unit.flags, _unit_ignored_by_autoaiming_bit))
			{
				struct aim_assist_target target;

				if (aim_assist_compute_target(parameters, object_index, position, direction, &target) &&
					target_count<maximum_target_count)
				{
					targets[target_count++]= target;
				}
			}

			if (object->object.first_child_object_index!=NONE && target_count<maximum_target_count)
			{
				target_count+= find_aim_assist_targets_recursive(parameters, object->object.first_child_object_index,
					position, direction, distance, angle_sine, angle_cosine, ignore_object_index, ignore_team_index,
					maximum_target_count - target_count, &targets[target_count]);
			}
		}

		object_index= object->object.next_object_index;
	}
	while (object_index!=NONE && target_count<maximum_target_count);

	return target_count;
}

static short find_aim_assist_targets(
	struct aim_assist_parameters const *parameters,
	short cluster_index,
	real_point3d const *position,
	real_vector3d const *direction,
	long ignore_object_index,
	short ignore_team_index,
	short maximum_target_count,
	struct aim_assist_target *targets)
{
	short target_count= 0;
	real distance= MAX(parameters->autoaim_distance, parameters->magnetism_distance);
	real angle= MAX(parameters->autoaim_angle, parameters->magnetism_angle);

	if (distance>0.f && angle>0.f)
	{
		real angle_sine= sine(angle);
		real angle_cosine= cosine(angle);
		short cluster_indices[MAXIMUM_AIM_ASSIST_CLUSTERS];
		long object_indices[MAXIMUM_AIM_ASSIST_OBJECTS];
		short cluster_count= structure_clusters_in_cone(cluster_index, position, direction, distance, angle_sine, angle_cosine,
			NUMBEROF(cluster_indices), cluster_indices);
		short object_count= objects_in_clusters_by_indices(FLAG(0), cluster_count, cluster_indices,
			NUMBEROF(object_indices), object_indices); // collideable objects only
		short object_number;

		for (object_number= 0; object_number<object_count; ++object_number)
		{
			long object_index= object_indices[object_number];

			target_count+= find_aim_assist_targets_recursive(parameters, object_index, position, direction,
				distance, angle_sine, angle_cosine, ignore_object_index, ignore_team_index,
				maximum_target_count - target_count, &targets[target_count]);
			if (target_count>=maximum_target_count)
			{
				break;
			}
		}
	}

	return target_count;
}

boolean aim_assist(
	struct aim_assist_parameters const *parameters,
	real_point3d const *position,
	real_vector3d const *direction,
	long ignore_object_index,
	short ignore_team_index,
	struct aim_assist_target *target)
{
#ifdef HALO_N64
	/* (the leaf looked up once: it walks the BSP, each frame for each
	player) */
	long leaf_index= scenario_leaf_index_from_point(position);

	if (leaf_index!=NONE)
	{
		short cluster_index= TAG_BLOCK_GET_ELEMENT(
			&global_structure_bsp_get()->leaves,
			leaf_index & LONG_MAX,
			struct structure_leaf)->cluster_index;
#else
	if (scenario_leaf_index_from_point(position)!=NONE)
	{
		short cluster_index= TAG_BLOCK_GET_ELEMENT(
			&global_structure_bsp_get()->leaves,
			scenario_leaf_index_from_point(position) & LONG_MAX,
			struct structure_leaf)->cluster_index;
#endif

		if (cluster_index!=NONE)
		{
			struct aim_assist_target targets[MAXIMUM_AIM_ASSIST_TARGETS];
			short target_count= find_aim_assist_targets(parameters, cluster_index, position, direction,
				ignore_object_index, ignore_team_index, NUMBEROF(targets), targets);

			if (target_count>0)
			{
				short target_index;
#ifdef HALO_N64
				struct structure_bsp *bsp= global_structure_bsp_get();
				boolean pvs_valid= bsp->cluster_data.size>0 && cluster_index>=0 && cluster_index<bsp->clusters.count;
#endif

				qsort(targets, target_count, sizeof(struct aim_assist_target), compare_targets);
				for (target_index= 0; target_index<target_count; ++target_index)
				{
#ifdef HALO_N64
					/* (a target in a cluster the eye's can't see -- the PVS
					only over-reports -- needs no ray) */
					short target_cluster= object_get(object_get_ultimate_parent(
						targets[target_index].object_index))->object.location.cluster_index;

					if (pvs_valid && target_cluster>=0 && target_cluster<bsp->clusters.count &&
						!scenario_test_pvs(cluster_index, target_cluster))
					{
						continue;
					}
#endif
					if (aim_assist_clear_line_of_sight(position, &targets[target_index].position,
						ignore_object_index, targets[target_index].object_index))
					{
						*target= targets[target_index];
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

long player_aim_projectile(
	long player_index,
	real_point3d const *position,
	real_vector3d *direction)
{
	long target_object_index= NONE;
	struct player_datum *player= player_get(player_index);
#ifdef HALO_N64
	/* The visible Golden Style cursor already follows the authoritative aim
	   vector. Do not add Halo's second, invisible projectile correction. */
	if (n64_golden_projectile(player_index, (const float *)position, (float *)direction, &target_object_index))
	{
		player->aim_assist_unit_index = target_object_index;
		player->aim_assist_timestamp = game_time_get();
		return target_object_index;
	}
#endif
	long aiming_unit_index= unit_get_aiming_unit_index(player->unit_index);
	struct aim_assist_parameters parameters;

	match_assert_valid_real_normal3d(
		"c:\\halo\\SOURCE\\game\\aim_assist.c",
		77,
		direction);

	match_assert(
		"c:\\halo\\SOURCE\\game\\aim_assist.c",
		79,
		global_current_collision_user_depth < MAXIMUM_COLLISION_USER_STACK_DEPTH);
	global_current_collision_users[global_current_collision_user_depth++] = _collision_user_aim_assist;

	if (unit_get_aim_assist_parameters(aiming_unit_index, unit_get_zoom_level(aiming_unit_index), &parameters))
	{
		real_point3d camera_position;
		real_vector3d camera_direction;
		real_vector3d target_direction;
		real_vector3d collision_direction;
		real autoaim_level;

		director_camera_deterministic(player->unit_index, &camera_position, &camera_direction);
		target_direction= *direction;
		autoaim_level= 0.f;

		{
			struct aim_assist_target target;

			if (aim_assist(&parameters, &camera_position, &camera_direction, player->unit_index, player->team_index, &target))
			{
				vector_from_points3d(position, &target.position, &target_direction);
				if (normalize3d(&target_direction)==0.f)
				{
					target_direction= *direction;
				}

				autoaim_level= target.autoaim_level;
				target_object_index= target.object_index;
			}
		}

		/* Trace from the camera at the aiming unit's distance along the camera direction. */
		{
			real_vector3d camera_vector;
			struct collision_result collision;
			real_vector3d camera_to_unit;
			real_vector3d camera_displacement;
			struct unit_datum *unit= unit_get(aiming_unit_index);
			real camera_to_unit_distance;

			vector_from_points3d(&unit->object.position, &camera_position, &camera_to_unit);
			camera_to_unit_distance= magnitude3d(&camera_to_unit);
			camera_displacement= camera_direction;
			normalize3d(&camera_displacement);
			scale_vector3d(&camera_displacement, camera_to_unit_distance, &camera_displacement);
			set_real_point3d(&camera_position, camera_position.x + camera_displacement.i,
				camera_position.y + camera_displacement.j, camera_position.z + camera_displacement.k);
			scale_vector3d(&camera_direction, 128.f, &camera_vector);
			collision_test_vector(_collision_test_for_projectiles_flags, &camera_position, &camera_vector,
				player->unit_index, &collision);

			vector_from_points3d(position, &collision.point, &collision_direction);
			if (normalize3d(&collision_direction)==0.f)
			{
				collision_direction= *direction;
			}
		}

		/* Blend toward the autoaim target, then pin inside the deviation cone. */
		{
			real_vector3d desired_direction;

			fast_normals_interpolate(&collision_direction, &target_direction, autoaim_level, &desired_direction);
			pin_normal_to_cone3d(&desired_direction, direction, sine(parameters.deviation_angle),
				cosine(parameters.deviation_angle), direction);
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\game\\aim_assist.c",
		140,
		global_current_collision_user_depth > 1);
	--global_current_collision_user_depth;

	player->aim_assist_unit_index= target_object_index;
	player->aim_assist_timestamp= game_time_get();

	return target_object_index;
}

long local_player_aim_assist(
	short local_player_index,
	real *autoaim_level,
	real *magnetism_level,
	real_euler_angles2d *target_angular_position,
	real_euler_angles2d *target_angular_velocity)
{
	long target_index= NONE;
	director_perspective perspective= director_get_perspective(local_player_index);

	*autoaim_level= 0.f;
	*magnetism_level= 0.f;
	set_real_euler_angles2d(target_angular_position, 0.f, 0.f);
	set_real_euler_angles2d(target_angular_velocity, 0.f, 0.f);

	if (perspective==_director_perspective_first_person || perspective==_director_perspective_third_person)
	{
		struct player_datum *player= player_get(local_player_get_player_index(local_player_index));
		long aiming_unit_index= unit_get_aiming_unit_index(player->unit_index);
		struct aim_assist_parameters parameters;

		if (unit_get_aim_assist_parameters(aiming_unit_index, player_control_get_zoom_level(local_player_index), &parameters))
		{
			struct observer_result const *camera= observer_get_camera(local_player_index);
			struct aim_assist_target target;

			if (aim_assist(&parameters, &camera->position, &camera->forward, aiming_unit_index, player->team_index, &target))
			{
				real_vector3d velocity, target_velocity, magnetism_velocity;

				*autoaim_level= target.autoaim_level;
				*magnetism_level= target.magnetism_level;
				euler_angles2d_from_vector3d(target_angular_position, &target.vector);

				object_get_velocities(player->unit_index, &velocity, NULL);
				object_get_velocities(target.object_index, &target_velocity, NULL);
				subtract_vectors3d(&target_velocity, &velocity, &magnetism_velocity);

				{
					real_vector3d const *relative_position= &target.vector;
					real_vector3d const *relative_velocity= &magnetism_velocity;
					real horizontal_distance_squared= relative_position->i*relative_position->i +
						relative_position->j*relative_position->j;
					real horizontal_distance= square_root(horizontal_distance_squared);

					target_angular_velocity->yaw= (relative_position->i*relative_velocity->j -
						relative_position->j*relative_velocity->i)/horizontal_distance_squared;
					target_angular_velocity->pitch= (horizontal_distance*relative_velocity->k -
						relative_position->k/horizontal_distance*(relative_position->i*relative_velocity->i +
						relative_position->j*relative_velocity->j))/(relative_position->k*relative_position->k +
						horizontal_distance_squared);
				}

				target_index= target.object_index;
			}
		}
	}

	return target_index;
}
