/*
GAME_GLOBALS.H

header included in hcex build.
*/

#ifndef __GAME_GLOBALS_H
#define __GAME_GLOBALS_H
#pragma once

/* ---------- headers */

#include "math/integer_math.h"
#include "math/real_math.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* referenced in game_globals.c? */
enum
{
	GAME_GLOBALS_TAG = 'matg',

	_material_dirt = 0,
	_material_sand,
	_material_stone,
	_material_snow,
	_material_wood,
	_material_hollow_metal,
	_material_thin_metal,
	_material_thick_metal,
	_material_rubber,
	_material_glass,
	_material_force_field,
	_material_grunt,
	_material_hunter_armor,
	_material_hunter_skin,
	_material_elite,
	_material_jackal,
	_material_jackal_energy_shield,
	_material_engineer,
	_material_engineer_force_field,
	_material_flood_combat_form,
	_material_flood_carrier_form,
	_material_cyborg,
	_material_cyborg_energy_shield,
	_material_armored_human,
	_material_human,
	_material_sentinel,
	_material_monitor,
	_material_plastic,
	_material_water,
	_material_leaves,
	_material_elite_energy_shield,
	_material_ice,
	_material_hunter_shield,
	NUMBER_OF_MATERIAL_TYPES,
	MAXIMUM_NUMBER_OF_MATERIAL_TYPES = 40,
};

#define game_globals_definition_get(index) ((struct game_globals *)tag_get(GAME_GLOBALS_TAG, (index)))

enum
{
	_game_difficulty_value_enemy_damage = 0,
	_game_difficulty_value_enemy_vitality,
	_game_difficulty_value_enemy_shield,
	_game_difficulty_value_enemy_recharge,
	_game_difficulty_value_friend_damage,
	_game_difficulty_value_friend_vitality,
	_game_difficulty_value_friend_shield,
	_game_difficulty_value_friend_recharge,
	_game_difficulty_value_infection_forms,
	_game_difficulty_value_unused9,
	_game_difficulty_value_rate_of_fire,
	_game_difficulty_value_projectile_error,
	_game_difficulty_value_burst_error,
	_game_difficulty_value_new_target_delay,
	_game_difficulty_value_burst_separation,
	_game_difficulty_value_target_tracking,
	_game_difficulty_value_target_leading,
	_game_difficulty_value_overcharge_chance,
	_game_difficulty_value_special_fire_delay,
	_game_difficulty_value_guidance_vs_player,
	_game_difficulty_value_melee_delay_base,
	_game_difficulty_value_melee_delay_scale,
	_game_difficulty_value_unused22,
	_game_difficulty_value_grenade_chance_scale,
	_game_difficulty_value_grenade_timer_scale,
	_game_difficulty_value_unused25,
	_game_difficulty_value_unused26,
	_game_difficulty_value_unused27,
	_game_difficulty_value_major_upgrade,
	_game_difficulty_value_major_upgrade_1,
	_game_difficulty_value_major_upgrade_2,
	_game_difficulty_value_unused31,
	_game_difficulty_value_unused32,
	_game_difficulty_value_unused33,
	_game_difficulty_value_unused34,
	NUMBER_OF_GAME_DIFFICULTY_VALUES,
};

/* ---------- macros */

/* ---------- structures */

struct game_globals_grenade
{
	short maximum_count;
	short mp_spawn_default;
	struct tag_reference throwing_effect;
	struct tag_reference hud_interface;
	struct tag_reference item;
	struct tag_reference projectile;
};

struct game_globals_rasterizer_data
{
	struct tag_reference distance_attenuation;
	struct tag_reference vector_normalization;
	struct tag_reference atmospheric_fog_density;
	struct tag_reference planar_fog_density;
	struct tag_reference linear_corner_fade;
	struct tag_reference active_camouflage_distortion;
	struct tag_reference glow;
	long unused1[15];
	struct tag_reference default_textures[3];
	struct tag_reference test[4];
	struct tag_reference screen_effect_video_scanline_map;
	struct tag_reference screen_effect_video_noise_map;
	long unused2[13];
	word active_camouflage_flags;
	word pad;
	real active_camouflage_refraction_amount;
	real active_camouflage_distance_falloff;
	real_rgb_color active_camouflage_tint_color;
	real active_camouflage_hyper_stealth_refraction_amount;
	real active_camouflage_hyper_stealth_distance_falloff;
	real_rgb_color active_camouflage_hyper_stealth_tint_color;
	struct tag_reference distance_attenuation_2d_for_the_pc;
};

struct game_globals_player_information
{
	struct tag_reference player_unit;
	long unused1[7];
	real walking_speed;
	real double_speed_multiplier;
	real run_forward_speed;
	real run_backward_speed;
	real run_sideways_speed;
	real run_acceleration;
	real sneak_forward_speed;
	real sneak_backward_speed;
	real sneak_sideways_speed;
	real sneak_acceleration;
	real airborne_acceleration;
	real multiplayer_only_speed_muliplier;
	long movement_unused[3];
	real_vector3d grenade_origin;
	real grenade_unused[3];
	real stun_movement_penalty;
	real stun_turning_penalty;
	real stun_jumping_penalty;
	real minimum_stun_time;
	real maximum_stun_time;
	real stun_unused[2];
	real first_person_idle_time_lower_bound;
	real first_person_idle_time_upper_bound;
	real first_person_idle_skip_fraction;
	long unused_first_person_unused[4];
	struct tag_reference coop_respawn_effect;
	long unused2[11];
};

struct game_globals_first_person_interface
{
	struct tag_reference hands;
	struct tag_reference hud_base;
	struct tag_reference hud_shield_meter;
	point2d hud_shield_meter_origin;
	struct tag_reference hud_body_meter;
	point2d hud_body_meter_origin;
	struct tag_reference night_vision_off_on_effect;
	struct tag_reference night_vision_on_off_effect;
	unsigned long unused[22];
};

struct game_globals_difficulty_information
{
	real values[NUMBER_OF_GAME_DIFFICULTY_VALUES][4];
	real pad[21];
};

struct game_globals_vehicle
{
	struct tag_reference vehicle;
};

struct game_globals_multiplayer_information
{
	struct tag_reference flag;
	struct tag_reference unit;
	struct tag_block vehicles;			// game_globals_vehicle
	struct tag_reference hill_shader;
	struct tag_reference flag_shader;
	struct tag_reference ball;
	struct tag_block sounds;				// tag_reference
	byte tag_padding[56];
};

struct game_globals_player_control
{
	real magnetism_friction;
	real magnetism_adhesion;
	real magnetism_inconsequential_target_scale;
	real magnetism_unused[13];
	real look_acceleration_time;
	real look_acceleration_scale;
	real look_pegging_threshold;
	real look_default_pitch_rate;
	real look_default_yaw_rate;
	real look_autolevel_scale;
	real look_unused[5];
	short minimum_weapon_swap_ticks;
	short minimum_autolevel_enabled_ticks;
	real minimum_vehicle_flipping_angle;
	struct tag_block look_function;
};

typedef char game_globals_player_control_size_assert[
	sizeof(struct game_globals_player_control) == 0x80 ? 1 : -1];
typedef char game_globals_player_control_flipping_angle_offset_assert[
	offsetof(struct game_globals_player_control, minimum_vehicle_flipping_angle) == 0x70 ? 1 : -1];

typedef char verify_game_globals_multiplayer_information_size[sizeof(struct game_globals_multiplayer_information) == 0xA0 ? 1 : -1];
typedef char verify_game_globals_multiplayer_sounds_offset[offsetof(struct game_globals_multiplayer_information, sounds) == 0x5C ? 1 : -1];

struct game_globals
{
	unsigned long flags;
	long unused0[61];
	struct tag_block sounds;
	struct tag_block camera;
	struct tag_block player_control; // game_globals_player_control
	struct tag_block difficulty_information;
	struct tag_block grenades;					// game_globals_grenade
	struct tag_block rasterizer_data;			// game_globals_rasterizer_data
	struct tag_block interface_tag_references;
	struct tag_block weapon_list;
	struct tag_block cheat_powerups;
	struct tag_block multiplayer_information;
	struct tag_block player_information;		// game_globals_player_information
	struct tag_block first_person_interface;	// game_globals_first_person_interface
	struct tag_block falling_damage;
	struct tag_block materials;
	struct tag_block playlist;
};

/* ---------- prototypes/GAME_GLOBALS.C */

char const *material_get_name(short material_type);
real game_difficulty_get_value(short value_type);
real game_difficulty_get_team_value(short value_type, short team_index);

/* ---------- globals */

/* ---------- public code */

#endif // __GAME_GLOBALS_H
