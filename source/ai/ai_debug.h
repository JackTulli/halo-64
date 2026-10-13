/*
AI_DEBUG.H

header included in hcex build.
*/

#ifndef __AI_DEBUG_H
#define __AI_DEBUG_H
#pragma once

/* ---------- headers */

#include "actors.h"

#include "math/real_math.h"

/* ---------- constants */

enum
{
	MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS = 16,
	MAXIMUM_AI_DEBUG_IDLE_LOOK_PROPS = 32,
	NUMBER_OF_AI_DEBUG_COMMUNICATION_TYPES = 57,
	NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES = 209,
#ifdef HALO_N64
	/* no AI on the N64 build: its debug record keeps its fields, not its
	525 KB of history */
	MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS = 4,
	MAXIMUM_AI_DEBUG_LINEOFSIGHT_PAIRS = 4,
	NUMBER_OF_AI_DEBUG_ACTOR_RECORDS = 4,
#else
	MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS = 16384,
	MAXIMUM_AI_DEBUG_LINEOFSIGHT_PAIRS = 8192,
	NUMBER_OF_AI_DEBUG_ACTOR_RECORDS = 512,
#endif
	MAXIMUM_AI_DEBUG_PATH_STORAGE = 32,
};

enum
{
	_firing_disabled = 0,
	_firing_busy,
	_firing_wrong_target,
	_firing_no_target,
	_firing_outside_active_area,
	_firing_not_visible,
	_firing_outside_range,
	_firing_blocked,
	_firing_holding_for_line,
	_firing_holding,
	_firing_pausing_for_line,
	_firing_pausing,
	_firing_wild,
	_firing_burst,
	_firing_not_in_midair,
	_firing_not_crouching,
	_firing_not_standing,
	_firing_not_stationary,
	_firing_underwater,
	_firing_min_range,
	NUMBER_OF_ACTOR_DEBUG_FIRING_DECISIONS,
};

enum
{
	_grenade_vehicle = 0,
	_grenade_unit_busy,
	_grenade_being_hurt,
	_grenade_no_grenades,
	_grenade_random_failed,
	_grenade_encounter_timeout,
	_grenade_target_failed,
	_grenade_not_enough_enemies,
	_grenade_collateral_damage,
	_grenade_trajectory_failed,
	_grenade_success,
	NUMBER_OF_ACTOR_DEBUG_GRENADE_DECISIONS,
};

enum
{
	_danger_avoidance_none = 0,
	_danger_avoidance_unnoticed,
	_danger_avoidance_animation_busy,
	_danger_avoidance_vehicle,
	_danger_avoidance_far_away,
	_danger_avoidance_outside_zone,
	_danger_avoidance_evasion_disallowed,
	_danger_avoidance_no_safe_direction,
	_danger_avoidance_no_desire,
	_danger_avoidance_can_avoid,
	_danger_avoidance_imminent_explosion,
	_danger_avoidance_imminent_impact,
	_danger_avoidance_no_animation,
	_danger_avoidance_attached_to_us,
	NUMBER_OF_ACTOR_DEBUG_DANGER_AVOIDANCE_DECISIONS,
};

enum
{
	_dive_not_attempted = 0,
	_dive_cannot_move,
	_dive_no_animation,
	_dive_animation_failure,
	_dive_success,
	NUMBER_OF_ACTOR_DEBUG_DIVE_DECISIONS
};

enum
{
	_charge_vehicle_success = 0,
	_charge_vehicle_not_driver,
	_charge_melee_swarm_cant,
	_charge_melee_inhibited,
	_charge_melee_notarget,
	_charge_melee_no_animation,
	_charge_melee_cannot_move,
	_charge_melee_success,
	_charge_stalking_success,
	_charge_close_success,
	NUMBER_OF_ACTOR_DEBUG_CHARGE_DECISIONS
};

/* ---------- macros */

/* ---------- structures */

/* one record per encounter firing position, filled by actor_select_firing_position */
struct ai_debug_actor_record
{
	boolean pursuit;
	boolean valid;
	struct firing_position firing_position;
};

struct ai_debug_lineofsight_pair
{
	short start_index;
	short end_index;
	short reference_count;
};

struct ai_debug_state
{
	boolean enter_debugger;
	boolean select_this_actor;
	boolean fix_defending_guard_firing_positions;
	boolean fix_actor_variants;
	boolean fast_los;
	boolean evaluate_all_positions;
	boolean ignore_player;
	boolean invisible_player;
	boolean flee_always;
	boolean force_all_active;
	boolean disable_wounded_sounds;
	boolean blind;
	boolean deaf;
	boolean force_vocalizations;
	boolean force_crouch;
	boolean path_disable_obstacle_avoidance;
	boolean path_disable_smoothing;
	boolean oversteer_disable;
	char selected_squad_name[32];
	int selected_squad_index;
	int selected_actor_index;
	boolean path;
	boolean path_start_freeze;
	boolean path_end_freeze;
	boolean path_flood;
	real path_maximum_radius;
	boolean path_attractor;
	char __unknown45[3];
	real path_attractor_radius;
	real path_attractor_weight;
	real path_accept_radius;
	unsigned long communication_suppress_vector[BIT_VECTOR_SIZE_IN_LONGS(NUMBER_OF_AI_DEBUG_COMMUNICATION_TYPES)];
	unsigned long communication_ignore_vector[BIT_VECTOR_SIZE_IN_LONGS(NUMBER_OF_AI_DEBUG_COMMUNICATION_TYPES)];
	unsigned long communication_focus_vector[BIT_VECTOR_SIZE_IN_LONGS(NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES)];
	char __unknown80[4];
	boolean communication_focus_enable;
	boolean communication_random_disabled;
	boolean communication_timeout_disabled;
	boolean communication_unit_repeat_disabled;
	boolean ballistic_lineoffire_freeze;
	boolean print_major_upgrade;
	boolean print_pursuit_checks;
	boolean print_rules;
	boolean print_rule_values;
	boolean print_respawn;
	boolean print_evaluation_statistics;
	boolean print_communication;
	boolean print_communication_player;
	boolean print_vocalizations;
	boolean print_placement;
	boolean print_speech;
	boolean print_speech_timers;
	boolean print_allegiance;
	boolean print_lost_speech;
	boolean print_migration;
	boolean print_automatic_migration;
	boolean print_scripting;
	boolean print_surprise;
	boolean print_command_lists;
	boolean print_damage_modifiers;
	boolean print_secondary_looking;
	boolean print_oversteer;
	boolean print_conversations;
	boolean print_killing_sprees;
	boolean print_acknowledgement;
	boolean print_unfinished_paths;
	boolean print_bsp_transition;
	boolean print_uncovering;
	boolean render;
	boolean render_all_actors;
	boolean render_inactive_actors;
	boolean render_lineoffire_crouching;
	boolean render_lineoffire;
	boolean render_lineofsight;
	boolean render_ballistic_lineoffire;
	boolean render_encounter_activeregion;
	boolean render_vision_cones;
	boolean render_current_state;
	boolean render_detailed_state;
	boolean render_props;
	boolean render_props_web;
	boolean render_props_no_friends;
	boolean render_props_unreachable;
	boolean render_props_unopposable;
	boolean render_props_target_weight;
	boolean render_idle_look;
	boolean render_targets;
	boolean render_targets_last_visible;
	boolean render_states;
	boolean render_support_surfaces;
	boolean render_recent_damage;
	boolean render_threats;
	boolean render_emotions;
	boolean render_audibility;
	boolean render_aiming_vectors;
	boolean render_secondary_looking;
	boolean render_vitality;
	boolean render_active_cover_seeking;
	boolean render_evaluations;
	boolean render_pursuit;
	boolean render_shooting;
	boolean render_trigger;
	boolean render_projectile_aiming;
	boolean render_aiming_validity;
	boolean render_speech;
	boolean render_teams;
	boolean render_player_ratings;
	boolean render_spatial_effects;
	boolean render_firing_positions;
	boolean render_gun_positions;
	boolean render_burst_geometry;
	boolean render_vehicle_avoidance;
	boolean render_vehicles_enterable;
	boolean render_melee_check;
	boolean render_dialogue_variants;
	boolean render_grenade_decisions;
	boolean render_danger_zones;
	boolean render_charge_decisions;
	boolean render_control;
	boolean render_activation;
	boolean render_paths;
	boolean render_paths_selected_only;
	boolean render_paths_failed;
	boolean render_paths_current;
	boolean render_paths_raw;
	boolean render_paths_smoothed;
	boolean render_paths_avoided;
	short render_paths_avoidance_segment;
	boolean render_paths_avoidance_obstacles;
	boolean render_paths_avoidance_search;
	boolean render_paths_destination;
	boolean render_paths_nodes;
	boolean render_paths_nodes_all;
	boolean render_paths_nodes_polygons;
	boolean render_paths_nodes_costs;
	boolean render_paths_nodes_closest;
	boolean render_player_aiming_blocked;
	boolean render_vector_avoidance;
	boolean render_vector_avoidance_rays;
	boolean render_vector_avoidance_sense_t;
	boolean render_vector_avoidance_avoid_t;
	boolean render_vector_avoidance_clear_time;
	boolean render_vector_avoidance_weights;
	boolean render_vector_avoidance_objects;
	boolean render_vector_avoidance_intermediate;
	boolean render_postcombat;
	long last_render_id;
	boolean lineoffire_valid;
	boolean lineoffire_success;
	char __unknownFA[2];
	real_point3d lineoffire_start;
	real_vector3d lineoffire_vector;
	long lineoffire_pill_count;
	boolean lineoffire_pill_hit[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	real_point3d lineoffire_pill_start[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	real_vector3d lineoffire_pill_vector[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	real lineoffire_pill_radius[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	boolean lineofsight_overflowed;
	char __unknown2E9[3];
	long lineofsight_point_count;
	real_point3d lineofsight_point[MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS];
	short lineofsight_point_reference_count[MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS];
	short lineofsight_point_key[MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS];
	long lineofsight_pair_count;
	struct ai_debug_lineofsight_pair lineofsight_pair[MAXIMUM_AI_DEBUG_LINEOFSIGHT_PAIRS];
	boolean ballistic_lineoffire_valid;
	boolean ballistic_lineoffire_success;
	char __unknown4C2F6[2];
	real_point3d ballistic_lineoffire_start;
	real_vector3d ballistic_lineoffire_vector;
	long ballistic_lineoffire_pill_count;
	real_point3d ballistic_lineoffire_pill_start[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	real_vector3d ballistic_lineoffire_pill_end[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	real ballistic_lineoffire_pill_radius[MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS];
	long ballistic_lineoffire_point_count;
	// the bound is not provable from the object -- nothing references the end of
	// this array. 64 leaves the 16 bytes below, which are equally unaccounted for.
	real_point3d ballistic_lineoffire_point[64];
	char __unknown4C7D8[16];
	boolean path_start_valid;
	char __unknown4C7E9[3];
	real_point3d path_start_point;
	long path_start_surface_index;
	long path_start_unit_index;
	boolean path_end_valid;
	char __unknown4C801[3];
	real_point3d path_end_point;
	long path_end_surface_index;
	real field_4C814;
	boolean field_4C818;
	char __unknown4C819[3];
#ifndef HALO_N64 /* the AI's path and firing position records (200 KB): the
	N64 build has none of the AI that fills them */
	struct path_state path_state;
#endif
	boolean field_608A8;
	char __unknown608A9[91];
#ifndef HALO_N64
	struct path_debug_storage path_storage;
#endif
	boolean evaluation_context_valid;
	char __unknown7D381[3];
#ifndef HALO_N64
	struct firing_position_evaluation_context evaluation_context;
#endif
	struct ai_debug_actor_record actor_record[NUMBER_OF_AI_DEBUG_ACTOR_RECORDS];
	long field_859F4;
	boolean field_859F8;
	boolean field_859F9;
	char __unknown859FA[2];
	real_vector3d field_859FC;
	real_vector3d field_85A08;
	boolean idle_look_valid;
	char __unknown85A15[3];
	long idle_look_unit_index;
	short idle_look_prop_count;
	char __unknown85A1E[2];
	long idle_look_prop_index[MAXIMUM_AI_DEBUG_IDLE_LOOK_PROPS];
	real idle_look_prop_weight[MAXIMUM_AI_DEBUG_IDLE_LOOK_PROPS];
	boolean speak_valid;
	boolean field_85B21;
	boolean field_85B22;
	char __unknown85B23[1];
	long speak_unit_index;
	short field_85B28;
	short speak_vocalization_type;
};

typedef char ai_debug_state_path_offset_assert[
	offsetof(struct ai_debug_state, path) == 0x3C ? 1 : -1];
typedef char ai_debug_state_print_speech_offset_assert[
	offsetof(struct ai_debug_state, print_speech) == 0x93 ? 1 : -1];
typedef char ai_debug_state_render_offset_assert[
	offsetof(struct ai_debug_state, render) == 0xA5 ? 1 : -1];
typedef char ai_debug_state_last_render_id_offset_assert[
	offsetof(struct ai_debug_state, last_render_id) == 0xF4 ? 1 : -1];
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_ballistic_lineoffire_valid_offset_assert[
	offsetof(struct ai_debug_state, ballistic_lineoffire_valid) == 0x4C2F4 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_ballistic_lineoffire_point_count_offset_assert[
	offsetof(struct ai_debug_state, ballistic_lineoffire_point_count) == 0x4C4D4 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_field_859F4_offset_assert[
	offsetof(struct ai_debug_state, field_859F4) == 0x859F4 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_path_state_offset_assert[
	offsetof(struct ai_debug_state, path_state) == 0x4C81C ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_path_storage_offset_assert[
	offsetof(struct ai_debug_state, path_storage) == 0x60904 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_field_7D3C7_offset_assert[
	offsetof(struct ai_debug_state, evaluation_context.find_path_direction_from_target) == 0x7D3C7 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_field_7D980_offset_assert[
	offsetof(struct ai_debug_state, evaluation_context.has_target) == 0x7D980 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_field_7D988_offset_assert[
	offsetof(struct ai_debug_state, evaluation_context.target_point) == 0x7D988 ? 1 : -1];
#endif
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_actor_record_offset_assert[
	offsetof(struct ai_debug_state, actor_record) == 0x7D9F4 ? 1 : -1];
#endif
typedef char ai_debug_actor_record_size_assert[
	sizeof(struct ai_debug_actor_record) == 0x40 ? 1 : -1];
typedef char ai_debug_actor_record_field_24_offset_assert[
	offsetof(struct ai_debug_actor_record, firing_position.path_direction_from_target) == 0x24 ? 1 : -1];
typedef char ai_debug_actor_record_field_3C_offset_assert[
	offsetof(struct ai_debug_actor_record, firing_position.evaluation) == 0x3C ? 1 : -1];
#ifndef HALO_N64 /* the N64's record is shorter (above) */
typedef char ai_debug_state_field_7D380_offset_assert[
	offsetof(struct ai_debug_state, evaluation_context_valid) == 0x7D380 ? 1 : -1];
#endif

struct actor_debug_info
{
	long last_render_id;
	long last_path_refresh;
	short firing_decision;
	real shooting_rof;
	real_point3d burst_last_known_position;
	real_point3d burst_tracked_position;
	real_vector3d burst_lead_vector;
	struct
	{
		long time;
		boolean aligned;
		boolean aligned_immediately;
		real_vector3d weapon_vector;
		real_vector3d aim_vector;
		real threshold;
		real alignment;
	} burst_alignment;
	long last_projectile_aiming_time;
	boolean field_60;
	real_point3d field_64;
	real_vector3d field_70;
	real_point3d field_7C;
	boolean field_88;
	real_vector3d field_8C;
	real_vector3d field_98;
	boolean field_A4;
	short field_A6;
	real field_A8;
	real field_AC;
	real field_B0;
	real field_B4;
	boolean field_B8;
	short field_BA;
	short field_BC;
	real field_C0;
	long last_vehicle_avoidance_time;
	real_point3d field_C8;
	real_point3d field_D4;
	real field_E0;
	real_point3d field_E4;
	real field_F0;
	boolean field_F4;
	real_point3d field_F8;
	long last_melee_time;
	real_point3d field_108;
	real_vector3d field_114;
	real_point3d field_120;
	real_vector3d field_12C;
	boolean field_138;
	boolean field_139;
	real_point3d field_13C;
	real field_148;
	real field_14C;
	long grenade_eval_time;
	short grenade_decision;
	short grenade_encounter_timeout_ticks;
	short grenade_enemy_count;
	short grenade_required_enemy_count;
	real grenade_current_damage;
	real grenade_random_value;
	real grenade_random_chance;
	long danger_avoidance_time;
	short danger_decision;
	boolean danger_abandoned_path;
	real danger_far_dist;
	real danger_far_radius;
	real danger_zone_dist;
	real danger_zone_radius;
	real danger_intersect_time;
	long dive_decision_time;
	short dive_decision;
	long charge_last_time;
	short charge_decision;
	real field_194;
	boolean field_198;
	short flying_error_ticks;
	long field_19C;
	struct vector_avoidance_data avoidance_data;
	short avoidance_type[ACTOR_MAXIMUM_AVOIDANCE_RAYS];
	real collision_t[ACTOR_MAXIMUM_AVOIDANCE_RAYS];
	real_point3d ray_origin[ACTOR_MAXIMUM_AVOIDANCE_RAYS];
	real_vector3d ray_direction[ACTOR_MAXIMUM_AVOIDANCE_RAYS];
	short field_62F8[8][2];
	real avoid_t[8][2];
	real_point3d field_6358[8][2];
	real_vector3d field_6418[8][2];
	real field_64D8[8];
	long field_64F8;
	real field_64FC;
	short field_6500;
	real field_6504;
	real field_6508;
	real sign_no_danger;
	real field_6510;
	real sign_too_far_cosangle;
	real sign_rotated;
	real field_651C;
	real field_6520;
	real_vector3d field_6524;
	real_vector3d field_6530;
	short field_653C;
	real_vector3d field_6540;
	real field_654C;
	boolean field_6550;
	boolean field_6551;
	real field_6554;
	real field_6558;
	real_vector3d avoidance_vector;
	real field_6568;
	long vision_last_time;
	real vision_last_maximum_distance;
	real vision_last_perception_factor;
	short num_debug_evaluations;
	short firing_position_type_mismatch_ticks;
};

/* ---------- prototypes/AI_DEBUG.C */

void ai_debug_initialize(
	void);

void ai_debug_dispose(
	void);

void ai_debug_dispose_from_old_map(
	void);

void ai_debug_clear_storage(
	void);

void ai_debug_actor_deleted(
	long actor_index);

void ai_debug_initialize_for_new_map(
	void);

void ai_debug_update(
	void);

void ai_debug_render(
	void);

// hs.c declares this as (long, char const *); the object shows the actor comes
// from ai_debug.selected_actor_index and both parameters are names
void ai_debug_vocalize(
	char const *priority_name,
	char const *vocalization_name);

void ai_debug_change_selected_encounter(
	boolean a1);

void ai_debug_change_selected_actor(
	boolean a1);

void ai_debug_select_encounter(
	long encounter_index);
void ai_debug_select_actor(
	long encounter_index,
	long actor_index);

char *ai_debug_describe_actor(
	long actor_index,
	long unit_index,
	boolean include_squad,
	char *buffer,
	long bufsize);

struct path_debug_storage *ai_debug_get_last_path(
	long actor_index);

struct path_debug_storage *ai_debug_get_path_storage(
	long actor_index);

void ai_debug_sound_point_set(
	void);

void ai_debug_lineoffire_new(
	real_point3d const *origin,
	real_vector3d const *vector);

void ai_debug_lineoffire_addpill(
	real_point3d const *base,
	real_vector3d const *directedheight,
	real width,
	boolean hit);

void ai_debug_lineoffire_success(
	boolean success);

boolean ai_debug_highlight_cluster(
	short index,
	real_argb_color const **highlight_color);

void ai_debug_lineofsight_reset(
	void);
void ai_debug_lineofsight(
	real_point3d const *start,
	short start_key,
	real_point3d const *end,
	short end_key);

void ai_debug_idle_look_clear(
	long unit_index);

void ai_debug_idle_look_addprop(
	long prop_index,
	real weight);

void ai_debug_speak(
	char const *vocalization_type_name);

/* ---------- globals */

extern struct ai_debug_state ai_debug;
extern struct actor_debug_info *actor_debug_array;
extern struct path_debug_storage *actor_path_debug_array;

/* The debug records: one per actor (256 x 25 KB) and one per path
(32 x 115 KB) on a development kit. A retail console has no room for
them, so the Xbox build keeps one shared record of each, which every
actor and path uses in turn; only the AI debug displays read them. */
#ifdef HALO_XBOX
#define AI_DEBUG_ACTOR_COUNT 1
#define AI_DEBUG_PATH_COUNT 1
#define AI_DEBUG_ACTOR_SLOT(index) ((void)(index), 0)
#define AI_DEBUG_PATH_SLOT(index) ((void)(index), 0)
#else
#define AI_DEBUG_ACTOR_COUNT MAXIMUM_NUMBER_OF_ACTORS
#define AI_DEBUG_PATH_COUNT MAXIMUM_NUMBER_OF_ACTOR_PATHS
#define AI_DEBUG_ACTOR_SLOT(index) (index)
#define AI_DEBUG_PATH_SLOT(index) (index)
#endif


extern real_point3d global_ai_debug_drawstack_next_position;
extern real_point3d global_ai_debug_drawstack_last_position;
extern real global_ai_debug_drawstack_height;
extern short global_ai_debug_string_position;
extern real_argb_color global_temporary_render_color;

/* ---------- public code */

#endif // __AI_DEBUG_H
