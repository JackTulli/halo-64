/*
PATH.H

file has inline function assertions.
*/

#ifndef __PATH_H
#define __PATH_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"

/* ---------- constants */

enum
{
#ifdef HALO_N64
	/* the N64's Firefight finds paths for a few Covenant on multiplayer maps
	(port/n64/game/n64_firefight.c): 35 KB of nodes instead of 70 */
	PATH_NODE_LIST_SIZE = 0x200,
#else
	PATH_NODE_LIST_SIZE = 0x400,
#endif
#ifdef HALO_N64
	/* (512 nodes at most: half of the table, cleared for every search) */
	PATH_HASH_TABLE_SIZE = 0x400,
#else
	PATH_HASH_TABLE_SIZE = 0x1000,
#endif
	MAXIMUM_DISC_COUNT = 128,
	MAXIMUM_OBSTACLE_AVOIDANCE_STEPS = 128,
	MAXIMUM_SMOOTHED_PATH_STEPS = 4,
};

enum
{
	_disc_optional_bit = 0,
};

enum
{
	PATH_HASH_TABLE_SIZE_PER_KEY = 8,
#ifdef HALO_N64
	PATH_HASH_KEY_MASK = 127,
	PATH_HASH_TABLE_MASK = 1023,
#else
	PATH_HASH_KEY_MASK = 511,
	PATH_HASH_TABLE_MASK = 4095,
#endif
	MAXIMUM_PATH_EDGES_PER_COLLISION_SURFACE = 64
};

enum
{
	_path_traverse_result_none = 0,
	_path_traverse_result_initial_not_pathfindable,
	_path_traverse_result_never_close_enough,
	_path_traverse_result_exhausted_search,
	_path_traverse_result_overflowed_nodes,
	_path_traverse_result_success,
	NUMBER_OF_PATH_TRAVERSE_RESULTS,
};

enum
{
	_path_build_result_none = 0,
	_path_build_result_no_destination,
	_path_build_result_cached_node_missing,
	_path_build_result_not_close_enough_to_destination,
	_path_build_result_obstacle_avoidance_failed,
	_path_build_result_success,
	NUMBER_OF_PATH_BUILD_RESULTS,
};


/* ---------- macros */

/* ---------- structures */

struct structure_bsp;

struct path_destination
{
	real_point3d point;
	long surface_index;
	real target_radius;
};

struct path_step
{
	long surface_index;
	real_point3d point;
};

struct path_result
{
	boolean valid;
	struct path_destination endpoint;
	boolean steps_finish_path;
	char step_count;
	char step_index;
	struct path_step steps[4];
};

struct path_input
{
	real pathfinding_radius;
	boolean ignore_broken_surfaces;
	long ignore_source_object_index;
	long ignore_target_object_index;
	boolean start_valid;
	real_point3d start_point;
	long start_surface_index;
	boolean attractor_valid;
	real_point3d attractor_point;
	long attractor_object_index;
	real attractor_radius;
	real attractor_weight;
	boolean search_bounded;
	real search_maximum_distance;
};

struct path_node
{
	short child_node_index;
	short parent_node_index;
	long parent_node_surface_index;
	long surface_index;
	real_point3d entry_point;
	real linear_distance_to_entry_point;
	real closest_approach_to_attractor;
	real path_distance_from_origin;
	real cumulative_cost;
	real total_cost_estimate;
	short quantized_cost_estimate;
	short depth;
	short heap_location;
#ifndef HALO_N64
	/* (only ai_debug.c reads these, not built for the N64: 52 bytes a node
	instead of 68, fewer cache lines touched by a search) */
	short last_render_id;
	real closest_distance_to_attractor;
	real_point3d closest_point_to_attractor;
#endif
};

#ifndef HALO_N64
typedef char path_node_size_assert[
	sizeof(struct path_node) == 0x44 ? 1 : -1];
#endif

struct path_heap_element
{
	short node_index;
	short quantized_cost_estimate;
};

struct path_state
{
	struct path_input input;
	struct path_debug_storage *debug;
	boolean destination_valid;
	struct path_destination destination;
	struct structure_bsp const *structure;
	short closest_node_index;
	real closest_distance;
	real closest_cost_estimate;
	real_point3d closest_point;
	short node_count;
	struct path_node node_list[PATH_NODE_LIST_SIZE];
	short heap_count;
	struct path_heap_element heap[1025];
	short hash_table[PATH_HASH_TABLE_SIZE];
};

struct obstacle_disc
{
	short flags;
	short obstacle_index;
	long object_index;
	real_point2d center;
	real radius;
	real height;
};

struct obstacles
{
	short obstacle_count;
	short disc_count;
	short disc_optional_count;
	struct obstacle_disc discs[MAXIMUM_DISC_COUNT];
};

struct obstacle_path_step
{
	real_point2d point;
	long surface_index;
	real_vector2d direction;
	real distance;
	short obstacle_index;
	byte obstacle_direction_index;
	byte reserved1B;
	short obstructed_goal_step_indices[2];
	real total_distance;
	short previous_step_index;
	byte reserved26[2];
};

struct obstacle_path
{
	real radius;
	boolean ignore_broken_surfaces;
	byte reserved5[3];
	struct obstacles const *obstacles;
	struct structure_bsp const *structure;
	real_point2d goal;
	long goal_surface_index;
	short goal_obstacle_index;
	short goal_step_index;
	short best_goal_blocked_step_index;
	byte reserved22[2];
	real best_goal_blocked_distance;
	boolean goal_found_exactly;
	boolean finishing;
	boolean ignore_optional;
	byte reserved2B;
	short step_count;
	byte reserved2E[2];
	struct obstacle_path_step steps[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	short heap_count;
	short heap[MAXIMUM_OBSTACLE_AVOIDANCE_STEPS];
	byte reserved1532[2];
};

typedef char obstacle_path_step_size_assert[
	sizeof(struct obstacle_path_step) == 0x28 ? 1 : -1];
typedef char obstacle_path_size_assert[
	sizeof(struct obstacle_path) == 0x1534 ? 1 : -1];
typedef char obstacle_path_step_count_offset_assert[
	offsetof(struct obstacle_path, step_count) == 0x2C ? 1 : -1];
typedef char obstacle_path_steps_offset_assert[
	offsetof(struct obstacle_path, steps) == 0x30 ? 1 : -1];
typedef char obstacle_path_heap_count_offset_assert[
	offsetof(struct obstacle_path, heap_count) == 0x1430 ? 1 : -1];
typedef char obstacle_path_heap_offset_assert[
	offsetof(struct obstacle_path, heap) == 0x1432 ? 1 : -1];

struct obstacles_test_pill_result
{
	real distance;
	short disc_index;
	short obstacle_index;
};

typedef char obstacles_test_pill_result_size_assert[
	sizeof(struct obstacles_test_pill_result) == 0x8 ? 1 : -1];
typedef char obstacles_test_pill_result_disc_index_offset_assert[
	offsetof(struct obstacles_test_pill_result, disc_index) == 0x4 ? 1 : -1];
typedef char obstacles_test_pill_result_obstacle_index_offset_assert[
	offsetof(struct obstacles_test_pill_result, obstacle_index) == 0x6 ? 1 : -1];

typedef char obstacle_disc_size_assert[
	sizeof(struct obstacle_disc) == 0x18 ? 1 : -1];
typedef char obstacle_disc_obstacle_index_offset_assert[
	offsetof(struct obstacle_disc, obstacle_index) == 0x2 ? 1 : -1];
typedef char obstacle_disc_height_offset_assert[
	offsetof(struct obstacle_disc, height) == 0x14 ? 1 : -1];
typedef char obstacles_size_assert[
	sizeof(struct obstacles) == 0xC08 ? 1 : -1];
typedef char obstacles_disc_count_offset_assert[
	offsetof(struct obstacles, disc_count) == 0x2 ? 1 : -1];
typedef char obstacles_discs_offset_assert[
	offsetof(struct obstacles, discs) == 0x8 ? 1 : -1];

struct path_debug_storage
{
	long actor_index;
	long path_time;
	long last_render_id;
	boolean valid;
	boolean failure;
	short structure_bsp_index;
	short path_traverse_result;
	short path_build_result;
	struct path_state path_state;
	struct path_result result;
	short raw_step_count;
	word pad_140FE;
	struct path_step raw_steps[64];
	short smoothed_step_count;
	word pad_14502;
	struct path_step smoothed_steps[4];
	short avoided_step_count;
	word pad_14546;
	struct path_step avoided_steps[4];
	boolean use_stored_obstacles;
	byte pad_14589;
	short avoidance_path_count;
	struct obstacles avoidance_obstacles[4];
	struct obstacle_path avoidance_paths[4];
};

typedef char path_state_node_count_offset_assert[
	offsetof(struct path_state, node_count) == 0x80 ? 1 : -1];
typedef char path_state_node_list_offset_assert[
	offsetof(struct path_state, node_list) == 0x84 ? 1 : -1];
#ifndef HALO_N64
typedef char path_state_hash_table_offset_assert[
	offsetof(struct path_state, hash_table) == 0x1208A ? 1 : -1];
#endif
#ifndef HALO_N64 /* (smaller on the N64: above) */
typedef char path_state_size_assert[
	sizeof(struct path_state) == 0x1408C ? 1 : -1];
#endif

#ifndef HALO_N64
typedef char path_debug_storage_size_assert[
	sizeof(struct path_debug_storage) == 0x1CA7C ? 1 : -1];
#endif
#ifndef HALO_N64
typedef char path_debug_storage_raw_steps_offset_assert[
	offsetof(struct path_debug_storage, raw_steps) == 0x14100 ? 1 : -1];
typedef char path_debug_storage_avoided_steps_offset_assert[
	offsetof(struct path_debug_storage, avoided_steps) == 0x14548 ? 1 : -1];
#endif

/* ---------- prototypes/PATH.C */

// argument order is read off the call sites in ai_debug_update; every parameter
// is dword-sized, so the middle names of path_input_new are not pinned by it
void path_input_new(
	struct path_input *input,
	real pathfinding_radius,
	boolean ignore_broken_surfaces,
	long ignore_source_object_index);
void path_input_set_start(
	struct path_input *input,
	real_point3d const *point,
	long surface_index);
void path_input_set_search_bounds(
	struct path_input *input,
	real maximum_distance);
void path_input_set_attractor(
	struct path_input *input,
	real_point3d const *point,
	real radius,
	long object_index,
	real weight);

#ifdef HALO_N64
/* a search in parts (path.c) */
boolean path_state_find_begin(struct path_state *state);
boolean path_state_find_continue(struct path_state *state, short budget, boolean *found);
#endif
void path_state_new(
	struct path_input const *input,
	struct path_state *state,
	struct path_debug_storage *debug);
void path_state_destination(
	struct path_state *state,
	real_point3d const *point,
	long surface_index,
	real target_radius);
boolean path_state_find(
	struct path_state *state);
boolean path_state_build_path(
	struct path_state *state,
	struct path_result *path);
void path_smooth(
	struct path_state *state,
	short raw_step_count,
	struct path_step const *raw_steps,
	short *smoothed_step_count,
	struct path_step *smoothed_steps,
	boolean *steps_finish_path);

struct path_node *path_get_node(
	struct path_state *state,
	short node_index);
short path_node_from_hash_table(
	struct path_state *state,
	long surface_index);
boolean path_3d_available(
	struct structure_bsp *structure,
	real_point3d const *start_point,
	real avoidance_distance,
	real_point3d const *end_point,
	boolean *finishing_path_reference,
	real_point3d *path_endpoint);
boolean path_3d_build_path(
	struct structure_bsp *structure,
	real_point3d const *start_point,
	real avoidance_distance,
	real_point3d const *end_point,
	struct path_result *path);
void closest_point_to_attractor(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *q,
	real_point3d *result);
real path_attractor_weight(
	struct path_state *state,
	real_point3d const *point,
	real_point3d const *previous_point,
	real *closest_distance);
boolean path_state_estimated_distance(
	struct path_state *state,
	real_point3d const *end_point,
	long end_surface_index,
	real *distance_reference,
	real *closest_approach_to_attractor_reference,
	real_vector3d *estimated_direction_reference);

void paths_initialize(
	void);
void paths_dispose(
	void);
void paths_initialize_for_new_map(
	void);
void paths_dispose_from_old_map(
	void);

/* ---------- prototypes/PATH_OBSTACLES.C */

void obstacles_new(
	struct obstacles *obstacles);
boolean obstacles_add_disc(
	struct obstacles *obstacles,
	long object_index,
	short flags,
	real_point3d const *center,
	real radius);
short obstacles_test_circle(
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *center,
	real radius);
boolean obstacles_test_pill(
	struct obstacles const *obstacles,
	short ignore_disc_index,
	real_point2d const *point,
	real_vector2d const *direction,
	real radius,
	real distance,
	boolean ignore_optional,
	struct obstacles_test_pill_result *result);
void obstacles_get_discs_in_sphere(
	struct obstacles *obstacles,
	real_point3d const *center,
	real radius,
	real_vector3d const *movement_direction,
	long ignore_source_object_index,
	long ignore_target_object_index);
void obstacles_disc_tangents(
	struct obstacles const *obstacles,
	short disc_index,
	real_point2d const *point,
	real radius,
	real_vector2d *right_direction,
	real_vector2d *left_direction,
	real *tangent_distance);
void obstacles_recompute(
	struct obstacles *obstacles,
	real radius);
void render_debug_obstacles(
	struct obstacles const *obstacles,
	real radius);

/* ---------- prototypes/PATH_OBSTACLE_AVOIDANCE.C */

__inline struct obstacle_disc const *obstacles_get_disc(
	struct obstacles const *obstacles,
	short disc_index);
short obstacle_from_disc(
	struct obstacles const *obstacles,
	short disc_index);
void render_debug_path(
	struct obstacle_path *path);
boolean path_avoid_obstacles(
	struct path_state *state,
	short input_step_count,
	struct path_step const *input_steps,
	short *avoided_step_count,
	struct path_step *avoided_steps,
	boolean *steps_finish_path);
void render_debug_obstacle_path(
	void);

/* ---------- globals */

extern boolean debug_obstacle_path;
extern boolean debug_obstacle_path_on_failure;
extern real_point3d debug_obstacle_path_start_point;
extern long debug_obstacle_path_start_surface_index;
extern real_point3d debug_obstacle_path_goal_point;
extern long debug_obstacle_path_goal_surface_index;
extern real debug_obstacle_path_radius;
extern boolean debug_obstacle_path_finishing;
extern boolean debug_ignore_broken_surfaces;

/* ---------- public code */

__inline struct obstacle_disc const *obstacles_get_disc(
	struct obstacles const *obstacles,
	short disc_index)
{
	match_assert(
		"c:\\halo\\source\\ai\\path.h",
		0x18C,
		disc_index>=0 && disc_index<obstacles->disc_count && obstacles->disc_count<=MAXIMUM_DISC_COUNT);

	return &obstacles->discs[disc_index];
}

#endif // __PATH_H

/* Opt-in PATH.C declarations used by ActionFlee.  Keeping this block after
 * the legacy include guard preserves the C2 declaration schedule of the
 * shared path header for unrelated translation units. */
#ifdef PATH_EXTERNAL_FLEE_ROUTINES
void path_input_set_target_object(
	struct path_input *input,
	long target_object_index);
boolean path_state_approach_point(
	struct path_state *state,
	real_point2d const *end_point,
	long end_surface_index,
	boolean *straight_line_reference,
	real_point3d *approach_point_reference);
#endif
