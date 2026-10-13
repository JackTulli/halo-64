/*
MODEL_ANIMATION_DEFINITIONS.H

header included in hcex build.
*/

#ifndef __MODEL_ANIMATION_DEFINITIONS_H
#define __MODEL_ANIMATION_DEFINITIONS_H
#pragma once


/* ---------- headers */

#include "math/real_math.h"
#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	ANIMATION_GRAPH_TAG = 'antr',
	ANIMATION_GRAPH_VERSION = 4,
	MAXIMUM_FRAMES_PER_ANIMATION = 2048,
	MAXIMUM_NODES_PER_ANIMATION = 64,
	MAXIMUM_ANIMATIONS_PER_GRAPH = 256,
	MAXIMUM_SOUND_REFERENCES_PER_ANIMATION_GRAPH = 257,
	MINIMUM_COMPRESSED_ANIMATION_FRAME_COUNT = 6,
};

enum
{
	_animation_compressed_bit = 0,
	_animation_world_relative_bit,
	_animation_25Hz_bit,
	NUMBER_OF_ANIMATION_FLAGS,
};

enum
{
	_animation_base = 0,
	_animation_overlay,
	_animation_replacement,
	NUMBER_OF_ANIMATION_TYPES,
};


enum
{
	_object_overlay_mode_frame = 0,
	_object_overlay_mode_scale,
	NUMBER_OF_OBJECT_OVERLAY_MODES,
};


/* ---------- macros */

#define animation_graph_definition_get(index) ((struct animation_graph *)tag_get(ANIMATION_GRAPH_TAG, index))

#define animation_graph_animation_index_get(block)	((struct animation_graph_animation_index *)((block)->address))

#ifdef HALO_N64
/* an animation's default data streams from the cartridge like its frames
(port/n64/src/n64_anim.c): its tag data's flags 1, file offset */
#define animation_get_default_data(animation) \
	((animation)->default_data.pad & 1 ? \
		(byte *)n64_animation_frame((animation)->default_data.file_offset, (animation)->default_data.size, \
			0, (short)(animation)->default_data.size) : \
		(byte *)(animation)->default_data.address)
void *n64_animation_frame(long data_offset, long data_size, short frame_index, short frame_size);
#else
#define animation_get_default_data(animation) ((animation)->default_data.address)
#endif

/* ---------- structures */

struct animation
{
	char name[TAG_STRING_LENGTH+1];
	short type;
	short frame_count;
	short frame_size;
	short frame_info_type;
	unsigned long node_list_checksum;
	short node_count;
	short private_loop_frame_index;
	real weight;
	short private_key_frame_index;
	short private_second_key_frame_index;
	short next_animation_index;
	word flags;
	short sound_index;
	short private_sound_frame_index;
	byte private_left_foot_frame_index;
	byte private_right_foot_frame_index;
	short runtime_parent_animation_index;
	real runtime_normalized_weight;
	struct tag_data frame_info;
	unsigned long nodes_with_translation_flags[2];
	long unused_translation[2];
	unsigned long nodes_with_rotation_flags[2];
	long unused_rotation[2];
	unsigned int nodes_with_scale_flags[2];
	long unused_scale[1];
	long compressed_data_offset;
	struct tag_data default_data;
	struct tag_data data;
};

typedef char verify_animation_loop_frame_index_offset[
	offsetof(struct animation, private_loop_frame_index) == 0x2E ? 1 : -1];
typedef char verify_animation_second_key_frame_index_offset[
	offsetof(struct animation, private_second_key_frame_index) == 0x36 ? 1 : -1];
typedef char verify_animation_sound_frame_index_offset[
	offsetof(struct animation, private_sound_frame_index) == 0x3E ? 1 : -1];

struct animation_graph_object_overlay
{
	short animation_index;
	short function_index;
	short mode;
	word pad;
	long unused[3];
};

struct animation_aiming_screen_bounds
{
	real negative_yaw_delta;
	real positive_yaw_delta;
	short negative_yaw_frame_count;
	short positive_yaw_frame_count;
	real negative_pitch_delta;
	real positive_pitch_delta;
	short negative_pitch_frame_count;
	short positive_pitch_frame_count;
};

struct animation_graph_animation_index
{
	short animation_index;
};

struct animation_graph_ik_point
{
	char marker_name[TAG_STRING_LENGTH+1];
	char attached_to_marker_name[TAG_STRING_LENGTH+1];
};

struct animation_graph_weapon_type
{
	char label[TAG_STRING_LENGTH+1];
	long unused[4];
	struct tag_block animations;
};

struct animation_graph_weapon_class
{
	char label[TAG_STRING_LENGTH+1];
	char grip_marker_name[TAG_STRING_LENGTH+1];
	char hand_marker_name[TAG_STRING_LENGTH+1];
	struct animation_aiming_screen_bounds aiming_screen_bounds;
	long unused[8];
	struct tag_block animations;		// animation_graph_animation_index
	struct tag_block ik_points;			// animation_graph_ik_point
	struct tag_block weapon_types;		// animation_graph_weapon_type
};

struct animation_graph_unit_seat
{
	char label[TAG_STRING_LENGTH+1];
	struct animation_aiming_screen_bounds looking_screen_bounds;
	long unused[2];
	struct tag_block animations;		// animation_graph_animation_index
	struct tag_block ik_points;			// animation_graph_ik_point
	struct tag_block weapon_classes;	// animation_graph_weapon_class
};

struct animation_graph
{
	struct tag_block object_overlays;		// animation_graph_object_overlay
	struct tag_block unit_seats;			// animation_graph_unit_seat
	struct tag_block weapon_animations;
	struct tag_block vehicle_animations;
	struct tag_block device_animations;
	struct tag_block unit_damage_animations;
	struct tag_block first_person_weapon_animations;
	struct tag_block sound_references;
	real limp_body_node_collision_radius;
	word flags;
	word pad;
	struct tag_block nodes;
	struct tag_block animations;			// animation
};

struct animation_list_entry
{
	char *name;
	short type;
};

struct animation_list
{
	short count;
	struct animation_list_entry *animations;
};

/* ---------- prototypes/MODEL_ANIMATION_DEFINITIONS.C */

byte *animation_get_frame_data(struct animation const *animation, short frame_index);
byte *animation_get_frame_info(struct animation const *animation, short frame_index, short frame_info_size);
char const *animation_list_get_string(struct animation_list const *animation_list, short animation_index);

/* ---------- prototypes/MODEL_ANIMATIONS.C */

struct animation_state;

short animation_loop_frame_index(
	struct animation const *animation);
short animation_second_key_frame_index(
	struct animation const *animation);
short animation_sound_frame_index(
	struct animation const *animation);
short build_damage_animation_index(
	short damage_type,
	short damage_direction,
	short damage_part);
void animation_get_x_offsets(
	struct animation const *animation,
	real *key_frame_x_offset,
	real *total_x_offset);
void animation_get_root_matrix(
	struct model const *model,
	struct animation const *animation,
	short frame_index,
	real_matrix4x3 *root_matrix);
void animation_get_root_velocity(
	struct model const *model,
	struct animation const *animation,
	short frame_index,
	real_vector3d *root_velocity);
struct compressed_quaternion_8byte;
struct compressed_quaternion_6byte;
void quaternion_decompress_8byte(
	struct compressed_quaternion_8byte const *compressed,
	real_quaternion *quaternion);
void quaternion_decompress_6byte(
	struct compressed_quaternion_6byte const *compressed,
	real_quaternion *quaternion);
void quaternion_compress_8byte(
	real_quaternion const *quaternion,
	struct compressed_quaternion_8byte *compressed);
void quaternion_compress_6byte(
	real_quaternion const *quaternion,
	struct compressed_quaternion_6byte *compressed);

short animation_graph_get_animation_by_name(
	long animation_graph_index,
	char const *animation_name);
void animation_frame_get_xy_translation(
	struct animation const *animation,
	short frame_index,
	real_vector2d *translation);
short animation_choose_random_permutation_internal(
	long render_or_affects_game_state,
	long animation_graph_index,
	short animation_index);
short animation_update_internal(
	long render_or_affects_game_state,
	long animation_graph_index,
	struct animation_state *state,
	long *sound_index);

void animation_get_node_orientations(
	struct model const *model,
	struct animation const *animation,
	short frame_index, 
	struct real_orientation *node_orientations);
void replacement_animation_apply(
	struct animation const *animation,
	short frame_index,
	struct real_orientation *node_orientations);
void overlay_animation_apply(
	struct animation const *animation,
	short frame_index,
	struct real_orientation *node_orientations);
void overlay_animation_apply_scaled(
	struct animation const *animation,
	short frame_index,
	real animation_scale,
	struct real_orientation *node_orientations);
void overlay_animation_apply_continuous(
	struct animation const *animation,
	real real_frame_index,
	struct real_orientation *node_orientations);

void inverse_kinematics_adjust_matrices(
	struct real_matrix4x3 *desired_hand_matrix,
	struct real_matrix4x3 *shoulder_matrix,
	struct real_matrix4x3 *elbow_matrix,
	struct real_matrix4x3 *hand_matrix);

void interpolate_node_orientations(
	short node_count,
	struct real_orientation *original_node_orientations,
	struct real_orientation *target_node_orientations,
	short frame_index,
	short frame_count);
void quaternion_decompress_6byte_renormalized(
	void const *compressed,
	real_quaternion *quaternion);


/* ---------- globals */

extern struct animation_list weapon_type_animation_list;
extern struct animation_list weapon_class_animation_list;
extern struct animation_list unit_seat_animation_list;
extern struct animation_list first_person_weapon_animation_list;
extern struct animation_list weapon_animation_list;
extern struct animation_list vehicle_animation_list;
extern struct animation_list device_animation_list;

extern char *damage_type_strings[4];
extern char *damage_direction_strings[4];
extern char *damage_part_strings[11];

/* ---------- public code */

#endif // __MODEL_ANIMATION_DEFINITIONS_H
