/*
MODEL_ANIMATIONS.C

symbols in this file:
0010FE40 0010:
	_animation_loop_frame_index (0000)
0010FE50 0010:
	_animation_second_key_frame_index (0000)
0010FE60 0010:
	_animation_sound_frame_index (0000)
0010FE70 0050:
	_code_0010fe70 (0000)
0010FEC0 00a0:
	_build_damage_animation_index (0000)
0010FF60 0080:
	_animation_get_x_offsets (0000)
0010FFE0 0080:
	_animation_set_frame_size (0000)
00110060 0060:
	_quaternion_decompress_8byte (0000)
001100C0 00c0:
	_quaternion_decompress_6byte (0000)
00110180 0020:
	_quaternion_decompress_6byte_renormalized (0000)
001101A0 0060:
	_quaternion_compress_8byte (0000)
00110200 0090:
	_quaternion_compress_6byte (0000)
00110290 0160:
	_animation_graph_node_matrices_from_orientations (0000)
001103F0 0110:
	_interpolate_node_orientations (0000)
00110500 0060:
	_animation_graph_get_animation_by_name (0000)
00110560 01d0:
	_code_00110560 (0000)
00110730 0040:
	_animation_frame_get_xy_translation (0000)
00110770 00b0:
	_animation_choose_random_permutation_internal (0000)
00110820 0360:
	_inverse_kinematics_adjust_matrices (0000)
00110B80 0310:
	_code_00110b80 (0000)
00110E90 0300:
	_code_00110e90 (0000)
00111190 02f0:
	_code_00111190 (0000)
00111480 0130:
	_animation_update_internal (0000)
001115B0 0300:
	_animation_get_node_orientations (0000)
001118B0 01e0:
	_replacement_animation_apply (0000)
00111A90 0210:
	_overlay_animation_apply (0000)
00111CA0 0240:
	_overlay_animation_apply_scaled (0000)
00111EE0 03c0:
	_overlay_animation_apply_continuous (0000)
001122A0 0400:
	_overlay_animation_apply_continuous_scaled (0000)
001126A0 0620:
	_aiming_screen_apply (0000)
00112CC0 0040:
	_animation_get_root_matrix (0000)
00112D00 00b0:
	_animation_get_root_velocity (0000)
0027F208 000a:
	??_C@_09KCBFNMAJ@animation?$AA@ (0000)
0027F214 0029:
	??_C@_0CJ@JGGOLPOM@c?3?2halo?2SOURCE?2models?2model_anim@ (0000)
0027F240 0035:
	??_C@_0DF@NDBCCBGA@damage_part?$DO?$DN0?5?$CG?$CG?5damage_part?$DMNU@ (0000)
0027F278 004e:
	??_C@_0EO@PDDEEPOK@damage_direction?$DO?$DN0?5?$CG?$CG?5damage_di@ (0000)
0027F2C8 003f:
	??_C@_0DP@CAICGJCA@damage_type?$DO?$DN0?5?$CG?$CG?5damage_type?$DMNU@ (0000)
0027F308 0004:
	__real@38000100 (0000)
0027F30C 0024:
	??_C@_0CE@FLNDNLBA@write_index?$DMMAXIMUM_NODES_PER_MO@ (0000)
0027F330 0018:
	??_C@_0BI@NCOKEGAP@frame_index?$DMframe_count?$AA@ (0000)
0027F348 000e:
	??_C@_0O@HJPIDGAL@frame_count?$DO0?$AA@ (0000)
0027F358 007a:
	??_C@_0HK@JCHJDAIG@target_frame_index?$DO?$DNkeyframe_fra@ (0000)
0027F3D4 0035:
	??_C@_0DF@BHOCMEDJ@keyframe_index?$DO?$DN0?5?$CG?$CG?5keyframe_in@ (0000)
0027F40C 001b:
	??_C@_0BL@NIECDBGN@?$CL?$CLinfinite_loop_killer?$DM200?$AA@ (0000)
0027F428 0033:
	??_C@_0DD@DKIDONPD@keyframe_index?$DO?$DN0?5?$CG?$CG?5keyframe_in@ (0000)
0027F460 0055:
	??_C@_0FF@IFOFHFLN@target_frame_index?$DO?$DN0?5?$CG?$CG?5target_@ (0000)
0027F4B8 001c:
	??_C@_0BM@EIBPKAOD@keyframe_frame_indices?$FL0?$FN?$DO0?$AA@ (0000)
0027F4D4 0017:
	??_C@_0BH@FMNFCNKN@keyframe_frame_indices?$AA@ (0000)
0027F4EC 0011:
	??_C@_0BB@INOOFBHG@keyframe_count?$DO1?$AA@ (0000)
0027F500 008e:
	??_C@_0IO@GIPANHA@?$CIanimation_update_kind_affects_g@ (0000)
0027F590 0004:
	__real@3f7ae148 (0000)
0027F594 0032:
	??_C@_0DC@FHNJEFLD@real_frame_index?$DM?5?$CIreal?$CJnext_key@ (0000)
0027F5C8 0032:
	??_C@_0DC@FGDAEANG@real_frame_index?$DO?$DN?$CIreal?$CJthis_key@ (0000)
0027F600 0043:
	??_C@_0ED@ICKGJKPC@keyframe_frame_indices?$FLkeyframe_@ (0000)
0027F648 0048:
	??_C@_0EI@OOKDDBPM@frame_index?$DO?$DN0?5?$CG?$CG?5frame_index?$DM?$DNk@ (0000)
0027F690 0012:
	??_C@_0BC@GPLHPFMP@keyframe_count?$DO?$DN0?$AA@ (0000)
0027F6A4 002e:
	??_C@_0CO@NEGCNKNH@real_frame_index?$DM?$CIreal?$CJanimation@ (0000)
0027F6D4 0017:
	??_C@_0BH@JENGDKGC@real_frame_index?$DO?$DN0?40f?$AA@ (0000)
0027F6F0 006f:
	??_C@_0GP@FNDBHMKG@compressed?5?$HM?$HM?5?$CIbyte?5?$CK?$CJdefault_da@ (0000)
0027F760 006b:
	??_C@_0GL@JOCEOLB@compressed?5?$HM?$HM?5?$CIbyte?5?$CK?$CJdata?9?$CIbyte@ (0000)
0027F7D0 006d:
	??_C@_0GN@PACKELMI@compressed?5?$HM?$HM?5?$CI?$CIbyte?5?$CK?$CJdata?9?$CIbyt@ (0000)
0027F840 0077:
	??_C@_0HH@GKOAHDOE@compressed?5?$HM?$HM?5?$CI?$CIbyte?5?$CK?$CJnext_data@ (0000)
0027F8B8 0048:
	??_C@_0EI@JELMFEDP@?$CD?$CD?$CD?5ERROR?5animation?5frame?5index?5@ (0000)
0027F900 0048:
	??_C@_0EI@GGOLLJMH@?$CD?$CD?$CD?5ERROR?5animation?5frame?5index?5@ (0000)
0027F948 0026:
	??_C@_0CG@FPLKLLFK@d0?$DN?$DN?$CFf?5direction?$CI?$CFf?$CJ?5yaw_delta?$CI?$CF@ (0000)
0027F970 0019:
	??_C@_0BJ@LEBMJMIB@animation?9?$DOframe_count?$DO1?$AA@ (0000)
0030A388 0001:
	_hs_model_animation_compression_enabled (0000)
0045662C 0024:
	_hs_model_animation_data_compressed_size (0000)
	_hs_model_animation_data_uncompressed_size (0004)
	_hs_model_animation_data_compression_savings_in_bytes (0008)
	_hs_model_animation_data_compression_savings_in_bytes_at_import (000c)
	_hs_model_animation_data_compression_savings_in_percent (0010)
	_hs_model_animation_bullshit (0014)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "models/model_animations.h"
#include "models/models.h"
#include "models/model_definitions.h"
#include "objects/objects.h"

#ifdef HALO_N64
#define N64_FRACTION(x) n64_fraction(x)
#define N64_FLOOR(x) n64_floorf((real)(x))
#else
#define N64_FRACTION(x) ((real)fmod((double)(x), 1.0))
#define N64_FLOOR(x) floor(x)
#endif

/* ---------- constants */

enum
{
	COMPRESSED_QUATERNION_COMPONENT_MAXIMUM = 32767,
	NUMBER_OF_ANIMATION_DAMAGE_TYPES = 4,
	NUMBER_OF_ANIMATION_DAMAGE_DIRECTIONS = 4,
	NUMBER_OF_DAMAGE_PARTS = 11,
};

enum
{
	animation_update_kind_render_only = 0,
	animation_update_kind_affects_game_state,
};

/* No shared header declares the update results yet; first_person_weapons.c keeps a partial copy. */
enum animation_update_result
{
	_animation_running = 0,
	_animation_key_frame,
	_animation_will_restart_on_next_frame,
	_animation_restarted,
	_animation_looped,
	NUMBER_OF_ANIMATION_UPDATE_RESULTS,
};

enum
{
	COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS = 12,
};

/* ---------- macros */

/* ---------- structures */

struct compressed_quaternion_8byte
{
	short i;
	short j;
	short k;
	short w;
};

struct compressed_quaternion_6byte
{
	word words[3];
};

typedef char verify_compressed_quaternion_8byte_size[
	sizeof(struct compressed_quaternion_8byte) == 0x08 ? 1 : -1];
typedef char verify_compressed_quaternion_6byte_size[
	sizeof(struct compressed_quaternion_6byte) == 0x06 ? 1 : -1];

struct animation_frame_info_dx_dy
{
	real dx;
	real dy;
};

struct animation_frame_info_dx_dy_dyaw
{
	real dx;
	real dy;
	real dyaw;
};

struct animation_frame_info_dx_dy_dz_dyaw
{
	real dx;
	real dy;
	real dz;
	real dyaw;
};

typedef char verify_animation_frame_info_dx_dy_size[
	sizeof(struct animation_frame_info_dx_dy) == 0x08 ? 1 : -1];
typedef char verify_animation_frame_info_dx_dy_dyaw_size[
	sizeof(struct animation_frame_info_dx_dy_dyaw) == 0x0C ? 1 : -1];
typedef char verify_animation_frame_info_dx_dy_dz_dyaw_size[
	sizeof(struct animation_frame_info_dx_dy_dz_dyaw) == 0x10 ? 1 : -1];
struct compressed_animation_header
{
	long rotation_keyframe_frame_indices_offset;
	long default_rotations_offset;
	long rotation_keyframes_offset;
	long translation_node_headers_offset;
	long translation_keyframe_frame_indices_offset;
	long default_translations_offset;
	long translation_keyframes_offset;
	long scale_node_headers_offset;
	long scale_keyframe_frame_indices_offset;
	long default_scales_offset;
	long scale_keyframes_offset;
	unsigned long rotation_node_headers[1];
};

/* Recovered animation-graph block layouts kept TU-private to preserve VC7 header scheduling. */
struct animation_graph_node
{
	char name[TAG_STRING_LENGTH+1];
	short next_sibling_node_index;
	short first_child_node_index;
	short parent_node_index;
	word pad;
	unsigned long flags;
	real_vector3d base_vector;
	real range;
	long pad1;
};

typedef char verify_animation_graph_node_size[
	sizeof(struct animation_graph_node) == 0x40 ? 1 : -1];

/* No shared header declares this block element yet; first_person_weapons.c keeps the same copy. */
struct animation_graph_sound_reference
{
	struct tag_reference sound;
	long unused;
};

typedef char verify_animation_graph_sound_reference_size[
	sizeof(struct animation_graph_sound_reference) == 0x14 ? 1 : -1];

typedef char verify_compressed_animation_header_rotation_node_headers_offset[
	offsetof(struct compressed_animation_header, rotation_node_headers) == 0x2C ? 1 : -1];

/* ---------- prototypes */

static boolean animation_is_compressed(
	struct animation const *animation);
static short animation_keyframe_search(
	short const *keyframe_frame_indices,
	short keyframe_count,
	short target_frame_index);
static void animation_get_keyframe_rotation(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real_quaternion *rotation);
static void animation_get_keyframe_translation(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real_point3d *translation);
static void animation_get_keyframe_scale(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real *scale);

/* ---------- globals */

boolean hs_model_animation_compression_enabled = TRUE;
long hs_model_animation_data_compressed_size = 0;
long hs_model_animation_data_uncompressed_size = 0;
long hs_model_animation_data_compression_savings_in_bytes = 0;
long hs_model_animation_data_compression_savings_in_bytes_at_import = 0;
real hs_model_animation_data_compression_savings_in_percent = 0.f;
long hs_model_animation_bullshit[4] = { 0 };

/* ---------- public code */

short animation_loop_frame_index(
	struct animation const *animation)
{
	return animation->private_loop_frame_index;
}

short animation_second_key_frame_index(
	struct animation const *animation)
{
	return animation->private_second_key_frame_index;
}

short animation_sound_frame_index(
	struct animation const *animation)
{
	return animation->private_sound_frame_index;
}

short build_damage_animation_index(
	short damage_type,
	short damage_direction,
	short damage_part)
{
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		55,
		damage_type>=0 && damage_type<NUMBER_OF_ANIMATION_DAMAGE_TYPES);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		56,
		damage_direction>=0 && damage_direction<NUMBER_OF_ANIMATION_DAMAGE_DIRECTIONS);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		57,
		damage_part>=0 && damage_part<NUMBER_OF_DAMAGE_PARTS);

	return (damage_type * NUMBER_OF_ANIMATION_DAMAGE_DIRECTIONS + damage_direction) *
		NUMBER_OF_DAMAGE_PARTS + damage_part;
}

void animation_get_x_offsets(
	struct animation const *animation,
	real *key_frame_x_offset,
	real *total_x_offset)
{
	short frame_index;
	real x_offset = 0.f;
	real key_x_offset = 0.f;
	byte const *frame_info = animation->frame_info.address;

	for (frame_index = 0; frame_index < animation->frame_count; frame_index++)
	{
#ifdef HALO_N64
		/* (the frame info streams from the cartridge, a frame at a time:
		animation_get_frame_info; frame_info is not advanced past it) */
		if (animation->frame_info.pad & 1)
		{
			short sizes[4] = {0, sizeof(struct animation_frame_info_dx_dy), sizeof(struct animation_frame_info_dx_dy_dyaw),
				sizeof(struct animation_frame_info_dx_dy_dz_dyaw)};

			if (animation->frame_info_type >= 1 && animation->frame_info_type <= 3)
			{
				x_offset += ((struct animation_frame_info_dx_dy const *)animation_get_frame_info(animation, frame_index,
					sizes[animation->frame_info_type]))->dx;
			}
			if (frame_index == animation->private_key_frame_index)
				key_x_offset = x_offset;
			continue;
		}
#endif
		switch (animation->frame_info_type)
		{
		case 1:
			x_offset += ((struct animation_frame_info_dx_dy const *)frame_info)->dx;
			frame_info += sizeof(struct animation_frame_info_dx_dy);
			break;

		case 2:
			x_offset += ((struct animation_frame_info_dx_dy_dyaw const *)frame_info)->dx;
			frame_info += sizeof(struct animation_frame_info_dx_dy_dyaw);
			break;

		case 3:
			x_offset += ((struct animation_frame_info_dx_dy_dz_dyaw const *)frame_info)->dx;
			frame_info += sizeof(struct animation_frame_info_dx_dy_dz_dyaw);
			break;
		}

		if (frame_index == animation->private_key_frame_index)
		{
			key_x_offset = x_offset;
		}
	}

	if (total_x_offset)
	{
		*total_x_offset = x_offset;
	}
	if (key_frame_x_offset)
	{
		*key_frame_x_offset = key_x_offset;
	}

	return;
}

void animation_set_frame_size(
	struct animation *animation)
{
	short frame_size = 0;
	short node_index;

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		123,
		animation);

	for (node_index = 0; node_index < animation->node_count; node_index++)
	{
		if (BIT_VECTOR_TEST_FLAG(animation->nodes_with_rotation_flags, node_index))
		{
			frame_size += sizeof(struct compressed_quaternion_8byte);
		}
		if (BIT_VECTOR_TEST_FLAG(animation->nodes_with_translation_flags, node_index))
		{
			frame_size += sizeof(real_point3d);
		}
		if (BIT_VECTOR_TEST_FLAG(animation->nodes_with_scale_flags, node_index))
		{
			frame_size += sizeof(real);
		}
	}

	animation->frame_size = frame_size;

	return;
}

short animation_update_internal(
	long render_or_affects_game_state,
	long animation_graph_index,
	struct animation_state *state,
	long *sound_index)
{
	struct animation_graph const *animation_graph = animation_graph_definition_get(animation_graph_index);
	struct animation const *animation;
	short result = _animation_running;

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		147,
		state);

	animation = TAG_BLOCK_GET_ELEMENT(
		&animation_graph->animations,
		state->index,
		struct animation);

	if (sound_index)
	{
		if (animation->sound_index!=NONE && animation->private_sound_frame_index==state->frame_index)
		{
			struct animation_graph_sound_reference const *sound_reference = TAG_BLOCK_GET_ELEMENT(
				&animation_graph->sound_references,
				animation->sound_index,
				struct animation_graph_sound_reference);

			*sound_index = sound_reference->sound.index;
		}
		else
		{
			*sound_index = NONE;
		}
	}

	state->frame_index++;
	if (state->frame_index>=animation->frame_count)
	{
		if (animation->private_loop_frame_index>0)
		{
			state->frame_index = MIN(animation->private_loop_frame_index, animation->frame_count-1);
			result = _animation_looped;
		}
		else
		{
			state->index = animation_choose_random_permutation_internal(
				render_or_affects_game_state,
				animation_graph_index,
				animation->runtime_parent_animation_index);
			state->frame_index = 0;
			result = _animation_restarted;
		}
	}
	else if (state->frame_index+1==animation->frame_count && animation->private_loop_frame_index==0)
	{
		result = _animation_will_restart_on_next_frame;
	}
	else if (state->frame_index==animation->private_key_frame_index ||
		state->frame_index==animation->private_second_key_frame_index)
	{
		result = _animation_key_frame;
	}

	return result;
}

void animation_graph_node_matrices_from_orientations(
	long animation_graph_index,
	real_matrix4x3 *node_matrices,
	struct real_orientation const *node_orientations,
	real_point3d const *origin,
	real_vector3d const *forward,
	real_vector3d const *up)
{
	struct animation_graph const *animation_graph = animation_graph_definition_get(
		animation_graph_index);
	real_matrix4x3 root_matrix;
	short node_indices[MAXIMUM_NODES_PER_MODEL];
	short read_index;
	short write_index;

	matrix4x3_from_point_and_vectors(&root_matrix, origin, forward, up);

	if (animation_graph->nodes.count > 0)
	{
		read_index = 0;
		write_index = 1;
		node_indices[0] = 0;

		do
		{
			short node_index = node_indices[read_index++];
			struct animation_graph_node *node = TAG_BLOCK_GET_ELEMENT(
				&animation_graph->nodes,
				node_index,
				struct animation_graph_node);
			real_matrix4x3 const *parent_matrix;
			real_matrix4x3 local_matrix;

			if (!node_index)
				parent_matrix = &root_matrix;
			else
				parent_matrix = &node_matrices[node->parent_node_index];

			matrix4x3_from_orientation(&local_matrix, &node_orientations[node_index]);
			matrix4x3_multiply(parent_matrix, &local_matrix, &node_matrices[node_index]);

			if (node->next_sibling_node_index != NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\models\\model_animations.c",
					1250,
					write_index<MAXIMUM_NODES_PER_MODEL);
				node_indices[write_index++] = node->next_sibling_node_index;
			}

			if (node->first_child_node_index != NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\models\\model_animations.c",
					1256,
					write_index<MAXIMUM_NODES_PER_MODEL);
				node_indices[write_index++] = node->first_child_node_index;
			}
		}
		while (read_index != write_index);
	}

	return;
}

short animation_graph_get_animation_by_name(
	long animation_graph_index,
	char const *animation_name)
{
	struct animation_graph const *animation_graph = animation_graph_definition_get(animation_graph_index);
	short animation_index;

	for (animation_index = 0; animation_index < animation_graph->animations.count; animation_index++)
	{
		struct animation const *animation = TAG_BLOCK_GET_ELEMENT(
			&animation_graph->animations,
			animation_index,
			struct animation);

		if (!_stricmp(animation_name, animation->name))
		{
			return animation_index;
		}
	}

	return NONE;
}

void animation_frame_get_xy_translation(
	struct animation const *animation,
	short frame_index,
	real_vector2d *translation)
{
	if (animation->frame_info_type == 1)
	{
		*translation = *(real_vector2d const *)animation_get_frame_info(
			animation,
			frame_index,
			sizeof(struct animation_frame_info_dx_dy));
	}
	else
	{
		translation->i = 0.f;
		translation->j = 0.f;
	}

	return;
}

short animation_choose_random_permutation_internal(
	long render_or_affects_game_state,
	long animation_graph_index,
	short animation_index)
{
	struct animation_graph const *animation_graph = animation_graph_definition_get(animation_graph_index);
	real random;

	if (render_or_affects_game_state == animation_update_kind_affects_game_state)
	{
		random = real_seed_random(get_global_random_seed_address());
	}
	else
	{
		random = real_seed_random(get_global_local_random_seed_address());
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1008,
			(animation_update_kind_affects_game_state==render_or_affects_game_state) ||
			(animation_update_kind_render_only==render_or_affects_game_state));
	}

	while (animation_index != NONE)
	{
		struct animation const *animation = TAG_BLOCK_GET_ELEMENT(
			&animation_graph->animations,
			animation_index,
			struct animation);

		if (random <= animation->runtime_normalized_weight)
		{
			break;
		}

		animation_index = animation->next_animation_index;
	}

	return animation_index;
}

void interpolate_node_orientations(
	short node_count,
	struct real_orientation *original_node_orientations,
	struct real_orientation *target_node_orientations,
	short frame_index,
	short frame_count)
{
	real fraction = (real)(frame_index + 1) / (real)frame_count;
	real inverse_fraction = 1.f - fraction;
	short node_index;

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1277,
		frame_count>0);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1278,
		frame_index<frame_count);

	for (node_index = 0; node_index < node_count; node_index++)
	{
		struct real_orientation *target = &target_node_orientations[node_index];
		struct real_orientation const *original = &original_node_orientations[node_index];

		target->scale = original->scale * inverse_fraction + target->scale * fraction;
		quaternions_interpolate_and_normalize(
			&original->rotation,
			&target->rotation,
			fraction,
			&target->rotation);
		target->translation.x = original->translation.x * inverse_fraction + target->translation.x * fraction;
		target->translation.y = original->translation.y * inverse_fraction + target->translation.y * fraction;
		target->translation.z = original->translation.z * inverse_fraction + target->translation.z * fraction;
	}

	return;
}

void animation_get_root_matrix(
	struct model const *model,
	struct animation const *animation,
	short frame_index,
	real_matrix4x3 *root_matrix)
{
	struct real_orientation node_orientations[MAXIMUM_NODES_PER_ANIMATION];

	animation_get_node_orientations(model, animation, frame_index, node_orientations);
	matrix4x3_from_point_and_quaternion(
		root_matrix,
		&node_orientations[0].translation,
		&node_orientations[0].rotation);

	return;
}

void animation_get_root_velocity(
	struct model const *model,
	struct animation const *animation,
	short frame_index,
	real_vector3d *root_velocity)
{
	struct real_orientation node_orientations[MAXIMUM_NODES_PER_ANIMATION];
	struct real_orientation previous_node_orientations[MAXIMUM_NODES_PER_ANIMATION];

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		221,
		animation->frame_count>1);

	if (frame_index == 0)
	{
		frame_index = 1;
	}

	animation_get_node_orientations(model, animation, frame_index, node_orientations);
	animation_get_node_orientations(model, animation, frame_index - 1, previous_node_orientations);

	vector_from_points3d(
		&previous_node_orientations[0].translation,
		&node_orientations[0].translation,
		root_velocity);

	return;
}

void animation_get_node_orientations(
	struct model const *model,
	struct animation const *animation,
	short frame_index,
	struct real_orientation *node_orientations)
{
	if (animation->type==_animation_base &&
		(!model ||
			((!animation->node_list_checksum || animation->node_list_checksum==model->node_list_checksum || !model->node_list_checksum) &&
			model->nodes.count==animation->node_count)))
	{
		boolean compressed = TEST_FLAG(animation->flags, _animation_compressed_bit) &&
			(hs_model_animation_compression_enabled || !animation->compressed_data_offset);
		byte *data = animation_get_frame_data(animation, frame_index);
		byte *default_data = animation_get_default_data(animation);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_rotation(animation, (real)frame_index, rotation_index++, node_index, &orientation->rotation);
				}
				else
				{
					quaternion_decompress_8byte((struct compressed_quaternion_8byte *)data, &orientation->rotation);
					data += sizeof(struct compressed_quaternion_8byte);
				}
			}
			else if (compressed)
			{
				struct compressed_animation_header const *header = (struct compressed_animation_header const *)data;

				quaternion_decompress_6byte(
					(struct compressed_quaternion_6byte *)(data+header->default_rotations_offset)+node_index,
					&orientation->rotation);
				quaternion_normalize(&orientation->rotation);
			}
			else
			{
				quaternion_decompress_8byte((struct compressed_quaternion_8byte *)default_data, &orientation->rotation);
				default_data += sizeof(struct compressed_quaternion_8byte);
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_translation(animation, (real)frame_index, translation_index++, node_index, &orientation->translation);
				}
				else
				{
					orientation->translation = *(real_point3d *)data;
					data += sizeof(real_point3d);
				}
			}
			else if (compressed)
			{
				struct compressed_animation_header const *header = (struct compressed_animation_header const *)data;

				orientation->translation = *((real_point3d *)(data+header->default_translations_offset)+node_index);
			}
			else
			{
				orientation->translation = *(real_point3d *)default_data;
				default_data += sizeof(real_point3d);
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_scale(animation, (real)frame_index, scale_index++, node_index, &orientation->scale);
				}
				else
				{
					orientation->scale = *(real *)data;
					data += sizeof(real);
				}
			}
			else if (compressed)
			{
				orientation->scale = 1.0f;
			}
			else
			{
				orientation->scale = *(real *)default_data;
				default_data += sizeof(real);
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			321,
			compressed || (byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size);
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			322,
			compressed || (byte *)default_data-(byte *)animation_get_default_data(animation)==animation->default_data.size);
	}
	else
	{
		model_get_node_orientations(model, node_orientations);
	}

	return;
}

void replacement_animation_apply(
	struct animation const *animation,
	short frame_index,
	struct real_orientation *node_orientations)
{
	if (animation->type==_animation_replacement && frame_index>=0 && frame_index<animation->frame_count)
	{
		boolean compressed = animation_is_compressed(animation);
		byte *data = animation_get_frame_data(animation, frame_index);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_rotation(animation, (real)frame_index, rotation_index++, node_index, &orientation->rotation);
				}
				else
				{
					quaternion_decompress_8byte((struct compressed_quaternion_8byte *)data, &orientation->rotation);
					data += sizeof(struct compressed_quaternion_8byte);
				}
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_translation(animation, (real)frame_index, translation_index++, node_index, &orientation->translation);
				}
				else
				{
					orientation->translation = *(real_point3d *)data;
					data += sizeof(real_point3d);
				}
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				if (compressed)
				{
					animation_get_keyframe_scale(animation, (real)frame_index, scale_index++, node_index, &orientation->scale);
				}
				else
				{
					orientation->scale = *(real *)data;
					data += sizeof(real);
				}
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			391,
			compressed || ((byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size));
	}

	return;
}

void overlay_animation_apply(
	struct animation const *animation,
	short frame_index,
	struct real_orientation *node_orientations)
{
	if (animation->type==_animation_overlay && frame_index>=0 && frame_index<animation->frame_count)
	{
		boolean compressed = animation_is_compressed(animation);
		byte *data = animation_get_frame_data(animation, frame_index);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				real_quaternion rotation;

				if (compressed)
				{
					animation_get_keyframe_rotation(animation, (real)frame_index, rotation_index++, node_index, &rotation);
				}
				else
				{
					quaternion_decompress_8byte((struct compressed_quaternion_8byte *)data, &rotation);
					data += sizeof(struct compressed_quaternion_8byte);
				}
				quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				real_point3d translation;

				if (compressed)
				{
					animation_get_keyframe_translation(animation, (real)frame_index, translation_index++, node_index, &translation);
				}
				else
				{
					translation = *(real_point3d *)data;
					data += sizeof(real_point3d);
				}
				orientation->translation.x += translation.x;
				orientation->translation.y += translation.y;
				orientation->translation.z += translation.z;
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				real scale;

				if (compressed)
				{
					animation_get_keyframe_scale(animation, (real)frame_index, scale_index++, node_index, &scale);
				}
				else
				{
					scale = *(real *)data;
					data += sizeof(real);
				}
				orientation->scale *= scale;
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			470,
			compressed || ((byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size));
	}

	return;
}

void overlay_animation_apply_scaled(
	struct animation const *animation,
	short frame_index,
	real animation_scale,
	struct real_orientation *node_orientations)
{
	real inverse_animation_scale = 1.0f-animation_scale;

	if (animation->type==_animation_overlay && frame_index>=0 && frame_index<animation->frame_count)
	{
		boolean compressed = animation_is_compressed(animation);
		byte *data = animation_get_frame_data(animation, frame_index);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				real_quaternion rotation;

				if (compressed)
				{
					animation_get_keyframe_rotation(animation, (real)frame_index, rotation_index++, node_index, &rotation);
				}
				else
				{
					quaternion_decompress_8byte((struct compressed_quaternion_8byte *)data, &rotation);
					data += sizeof(struct compressed_quaternion_8byte);
				}
				quaternions_interpolate(global_identity_quaternion, &rotation, animation_scale, &rotation);
				quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				real_point3d translation;

				if (compressed)
				{
					animation_get_keyframe_translation(animation, (real)frame_index, translation_index++, node_index, &translation);
				}
				else
				{
					translation = *(real_point3d *)data;
					data += sizeof(real_point3d);
				}
				orientation->translation.x += translation.x*animation_scale;
				orientation->translation.y += translation.y*animation_scale;
				orientation->translation.z += translation.z*animation_scale;
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				real scale;

				if (compressed)
				{
					animation_get_keyframe_scale(animation, (real)frame_index, scale_index++, node_index, &scale);
				}
				else
				{
					scale = *(real *)data;
					data += sizeof(real);
				}
				orientation->scale *= scale*animation_scale+inverse_animation_scale;
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			554,
			compressed || ((byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size));
	}

	return;
}

void overlay_animation_apply_continuous(
	struct animation const *animation,
	real real_frame_index,
	struct real_orientation *node_orientations)
{
	real fraction;
	short frame_index;

	fraction = N64_FRACTION(real_frame_index);
	frame_index = (short)fast_ftol((real)N64_FLOOR(fabs(real_frame_index)));

	if (real_frame_index < 0.0f || real_frame_index > (real)animation->frame_count)
	{
		error(
			_error_silent,
			"### ERROR animation frame index out of bounds A(%f,%x) -- tell Bernie!!",
			real_frame_index,
			*((long *)&real_frame_index));
	}

	if (frame_index >= animation->frame_count)
	{
		frame_index = animation->frame_count - 1;
		fraction = 1.0f;
		real_frame_index = (real)frame_index;
	}

	if (animation->type == _animation_overlay)
	{
		boolean compressed = animation_is_compressed(animation);
		short next_frame_index = frame_index == animation->frame_count - 1 ? 0 : frame_index + 1;
		byte *data = animation_get_frame_data(animation, frame_index);
		byte *next_data = animation_get_frame_data(animation, next_frame_index);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index < animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index & (LONG_BITS - 1)))
			{
				short long_index = node_index >> LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				real_quaternion rotation;

				if (compressed)
				{
					animation_get_keyframe_rotation(
						animation,
						(real)frame_index,
						(short)rotation_index++,
						node_index,
						&rotation);
				}
				else
				{
					real_quaternion this_rotation;
					real_quaternion next_rotation;

					quaternion_decompress_8byte(
						(struct compressed_quaternion_8byte const *)data,
						&this_rotation);
					data += sizeof(struct compressed_quaternion_8byte);
					quaternion_decompress_8byte(
						(struct compressed_quaternion_8byte const *)next_data,
						&next_rotation);
					next_data += sizeof(struct compressed_quaternion_8byte);
					quaternions_interpolate_and_normalize(
						&this_rotation,
						&next_rotation,
						fraction,
						&rotation);
				}

				quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				real_point3d translation;

				if (compressed)
				{
					animation_get_keyframe_translation(
						animation,
						real_frame_index,
						(short)translation_index++,
						node_index,
						&translation);
				}
				else
				{
					real_point3d const *this_translation;
					real_point3d const *next_translation;

					this_translation = (real_point3d const *)data;
					data += sizeof(real_point3d);
					next_translation = (real_point3d const *)next_data;
					next_data += sizeof(real_point3d);
					points_interpolate(
						this_translation,
						next_translation,
						fraction,
						&translation);
				}

				orientation->translation.x += translation.x;
				orientation->translation.y += translation.y;
				orientation->translation.z += translation.z;
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				real scale;

				if (compressed)
				{
					animation_get_keyframe_scale(
						animation,
						real_frame_index,
						(short)scale_index++,
						node_index,
						&scale);
				}
				else
				{
					real this_scale;
					real next_scale;

					this_scale = *(real const *)data;
					data += sizeof(real);
					next_scale = *(real const *)next_data;
					next_data += sizeof(real);
					scalars_interpolate(
						this_scale,
						next_scale,
						fraction,
						&scale);
				}

				orientation->scale *= scale;
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			693,
			compressed || ((byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size));
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			694,
			compressed || ((byte *)next_data-(byte *)animation_get_frame_data(animation, next_frame_index)==animation->frame_size));
	}

	return;
}

void inverse_kinematics_adjust_matrices(
	struct real_matrix4x3 *desired_hand_matrix,
	struct real_matrix4x3 *shoulder_matrix,
	struct real_matrix4x3 *elbow_matrix,
	struct real_matrix4x3 *hand_matrix)
{
	real upper_arm_length = distance3d(&shoulder_matrix->position, &elbow_matrix->position);
	real forearm_length = distance3d(&elbow_matrix->position, &hand_matrix->position);
	real hand_distance = distance3d(&desired_hand_matrix->position, &shoulder_matrix->position);
	real inverse_hand_distance;
	real_vector3d shoulder_to_elbow;
	real_vector3d hand_direction;
	real_vector3d bend_plane_normal;
	real_vector3d bend_direction;
	real maximum_reach;
	real upper_arm_length_squared;
	real elbow_projection;
	real elbow_remainder;
	real elbow_height;
	real_point3d new_elbow_position;

	shoulder_to_elbow.i = elbow_matrix->position.x-shoulder_matrix->position.x;
	shoulder_to_elbow.j = elbow_matrix->position.y-shoulder_matrix->position.y;
	shoulder_to_elbow.k = elbow_matrix->position.z-shoulder_matrix->position.z;
	inverse_hand_distance = 1.0f/hand_distance;
	hand_direction.i = (desired_hand_matrix->position.x-shoulder_matrix->position.x)*inverse_hand_distance;
	hand_direction.j = (desired_hand_matrix->position.y-shoulder_matrix->position.y)*inverse_hand_distance;
	hand_direction.k = (desired_hand_matrix->position.z-shoulder_matrix->position.z)*inverse_hand_distance;
	cross_product3d(&hand_direction, &shoulder_to_elbow, &bend_plane_normal);
	normalize3d(&bend_plane_normal);
	cross_product3d(&bend_plane_normal, &hand_direction, &bend_direction);

	maximum_reach = (upper_arm_length+forearm_length)*0.98f;
	if (maximum_reach<hand_distance)
	{
		desired_hand_matrix->position.x = shoulder_matrix->position.x+hand_direction.i*maximum_reach;
		desired_hand_matrix->position.y = shoulder_matrix->position.y+hand_direction.j*maximum_reach;
		desired_hand_matrix->position.z = shoulder_matrix->position.z+hand_direction.k*maximum_reach;
		hand_distance = maximum_reach;
	}

	upper_arm_length_squared = upper_arm_length*upper_arm_length;
	elbow_projection =
		(hand_distance*hand_distance+upper_arm_length_squared-forearm_length*forearm_length)/
		(hand_distance+hand_distance);
	elbow_remainder = hand_distance-elbow_projection;
	elbow_height = square_root(upper_arm_length_squared-elbow_projection*elbow_projection);

	{
		real_vector3d *shoulder_forward = &shoulder_matrix->forward;
		real_vector3d *shoulder_left = &shoulder_matrix->left;
		real_vector3d *shoulder_up = &shoulder_matrix->up;

		shoulder_forward->i = elbow_projection*hand_direction.i+elbow_height*bend_direction.i;
		shoulder_forward->j = elbow_projection*hand_direction.j+elbow_height*bend_direction.j;
		shoulder_forward->k = elbow_projection*hand_direction.k+elbow_height*bend_direction.k;
		normalize3d(shoulder_forward);
		cross_product3d(shoulder_forward, shoulder_left, shoulder_up);
		normalize3d(shoulder_up);
		cross_product3d(shoulder_up, shoulder_forward, shoulder_left);

		new_elbow_position.x = shoulder_matrix->position.x+shoulder_forward->i*upper_arm_length;
		new_elbow_position.y = shoulder_matrix->position.y+shoulder_forward->j*upper_arm_length;
		new_elbow_position.z = shoulder_matrix->position.z+shoulder_forward->k*upper_arm_length;
	}
	{
		real_vector3d *elbow_forward = &elbow_matrix->forward;
		real_vector3d *elbow_left = &elbow_matrix->left;
		real_vector3d *elbow_up = &elbow_matrix->up;

		elbow_forward->i = elbow_remainder*hand_direction.i-elbow_height*bend_direction.i;
		elbow_forward->j = elbow_remainder*hand_direction.j-elbow_height*bend_direction.j;
		elbow_forward->k = elbow_remainder*hand_direction.k-elbow_height*bend_direction.k;
		normalize3d(elbow_forward);
		cross_product3d(elbow_forward, elbow_left, elbow_up);
		normalize3d(elbow_up);
		cross_product3d(elbow_up, elbow_forward, elbow_left);
	}
	elbow_matrix->position = new_elbow_position;

	*hand_matrix = *desired_hand_matrix;

	return;
}

void overlay_animation_apply_continuous_scaled(
	struct animation const *animation,
	real real_frame_index,
	real animation_scale,
	struct real_orientation *node_orientations)
{
	real inverse_animation_scale = 1.0f-animation_scale;
	real fraction = N64_FRACTION(real_frame_index);
	short frame_index = (short)fast_ftol((real)N64_FLOOR(real_frame_index));

	if (real_frame_index<0.0f || real_frame_index>(real)animation->frame_count)
	{
		error(
			_error_silent,
			"### ERROR animation frame index out of bounds B(%f,%x) -- tell Bernie!!",
			real_frame_index,
			*((long *)&real_frame_index));
	}

	if (frame_index>=animation->frame_count)
	{
		frame_index = animation->frame_count-1;
		fraction = 1.0f;
		real_frame_index = (real)frame_index;
	}

	if (animation->type==_animation_overlay)
	{
		boolean compressed = animation_is_compressed(animation);
		short next_frame_index = frame_index==animation->frame_count-1 ? 0 : frame_index+1;
		byte *data = animation_get_frame_data(animation, frame_index);
		byte *next_data = animation_get_frame_data(animation, next_frame_index);
		long rotation_index = 0;
		unsigned long rotation_flags;
		long translation_index = 0;
		unsigned long translation_flags;
		long scale_index = 0;
		unsigned long scale_flags;
		short node_index;

		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
				scale_flags = animation->nodes_with_scale_flags[long_index];
			}

			if (TEST_FLAG(rotation_flags, 0))
			{
				real_quaternion rotation;

				if (compressed)
				{
					animation_get_keyframe_rotation(
						animation,
						(real)frame_index,
						(short)rotation_index++,
						node_index,
						&rotation);
				}
				else
				{
					real_quaternion this_rotation;
					real_quaternion next_rotation;

					quaternion_decompress_8byte(
						(struct compressed_quaternion_8byte const *)data,
						&this_rotation);
					data += sizeof(struct compressed_quaternion_8byte);
					quaternion_decompress_8byte(
						(struct compressed_quaternion_8byte const *)next_data,
						&next_rotation);
					next_data += sizeof(struct compressed_quaternion_8byte);
					quaternions_interpolate_and_normalize(
						&this_rotation,
						&next_rotation,
						fraction,
						&rotation);
				}

				quaternions_interpolate_and_normalize(
					global_identity_quaternion,
					&rotation,
					animation_scale,
					&rotation);
				quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
			}
			rotation_flags >>= 1;

			if (TEST_FLAG(translation_flags, 0))
			{
				real_point3d translation;

				if (compressed)
				{
					animation_get_keyframe_translation(
						animation,
						real_frame_index,
						(short)translation_index++,
						node_index,
						&translation);
				}
				else
				{
					real_point3d const *this_translation;
					real_point3d const *next_translation;

					this_translation = (real_point3d const *)data;
					data += sizeof(real_point3d);
					next_translation = (real_point3d const *)next_data;
					next_data += sizeof(real_point3d);
					points_interpolate(
						this_translation,
						next_translation,
						fraction,
						&translation);
				}

				orientation->translation.x += translation.x*animation_scale;
				orientation->translation.y += translation.y*animation_scale;
				orientation->translation.z += translation.z*animation_scale;
			}
			translation_flags >>= 1;

			if (TEST_FLAG(scale_flags, 0))
			{
				real scale;

				if (compressed)
				{
					animation_get_keyframe_scale(
						animation,
						real_frame_index,
						(short)scale_index++,
						node_index,
						&scale);
				}
				else
				{
					real this_scale;
					real next_scale;

					this_scale = *(real const *)data;
					data += sizeof(real);
					next_scale = *(real const *)next_data;
					next_data += sizeof(real);
					scalars_interpolate(
						this_scale,
						next_scale,
						fraction,
						&scale);
				}

				orientation->scale *= scale*animation_scale+inverse_animation_scale;
			}
			scale_flags >>= 1;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			820,
			compressed || ((byte *)data-(byte *)animation_get_frame_data(animation, frame_index)==animation->frame_size));
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			821,
			compressed || ((byte *)next_data-(byte *)animation_get_frame_data(animation, next_frame_index)==animation->frame_size));
	}

	return;
}

void aiming_screen_apply(
	struct animation const *animation,
	struct animation_aiming_screen_bounds const *aiming_screen_bounds,
	real yaw,
	real pitch,
	struct real_orientation *node_orientations)
{
	short grid_width = aiming_screen_bounds->negative_yaw_frame_count+
		aiming_screen_bounds->positive_yaw_frame_count+1;
	short grid_height = aiming_screen_bounds->negative_pitch_frame_count+
		aiming_screen_bounds->positive_pitch_frame_count+1;
	short yaw_frame_index;
	boolean compressed;
	real yaw_delta;
	real yaw_frame;
	real yaw_fraction;
	real pitch_delta;
	real pitch_frame;
	short pitch_frame_index;
	real pitch_fraction;
	short next_yaw_cell;
	short next_pitch_cell;
	short frame_index00;
	short frame_index10;
	short frame_index01;
	short frame_index11;
	byte *data00;
	byte *data10;
	byte *data01;
	byte *data11;
	long rotation_index;
	long translation_index;
	unsigned long rotation_flags;
	unsigned long translation_flags;
	short node_index;

	if (animation->type!=_animation_overlay ||
		animation->frame_count<grid_width*grid_height)
	{
		return;
	}

	compressed = animation_is_compressed(animation);
	yaw_delta = yaw<0.0f ? aiming_screen_bounds->negative_yaw_delta :
		aiming_screen_bounds->positive_yaw_delta;
	yaw_frame = yaw_delta==0.0f ? 0.0f : yaw/yaw_delta;
	yaw_frame_index = (short)yaw_frame;
	yaw_fraction = N64_FRACTION(yaw_frame);
	if (yaw_fraction<0.0f)
	{
		yaw_fraction += 1.0f;
		yaw_frame_index--;
	}
	if (yaw_frame_index>=aiming_screen_bounds->positive_yaw_frame_count)
	{
		yaw_frame_index = aiming_screen_bounds->positive_yaw_frame_count-1;
		yaw_fraction = 1.0f;
	}
	if (yaw_frame_index<-aiming_screen_bounds->negative_yaw_frame_count)
	{
		yaw_frame_index = -aiming_screen_bounds->negative_yaw_frame_count;
		yaw_fraction = 0.0f;
	}
	yaw_frame_index += aiming_screen_bounds->negative_yaw_frame_count;

	match_vassert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		869,
		yaw_fraction>=0.0f && yaw_fraction<=1.0f,
		csprintf(
			temporary,
			"d0==%f direction(%f) yaw_delta(%f,%f)",
			yaw_fraction,
			yaw,
			aiming_screen_bounds->negative_yaw_delta,
			aiming_screen_bounds->positive_yaw_delta));

	pitch_delta = pitch<0.0f ? aiming_screen_bounds->negative_pitch_delta :
		aiming_screen_bounds->positive_pitch_delta;
	pitch_frame = pitch_delta==0.0f ? 0.0f : pitch/pitch_delta;
	pitch_frame_index = (short)pitch_frame;
	pitch_fraction = N64_FRACTION(pitch_frame);
	if (pitch_fraction<0.0f)
	{
		pitch_fraction += 1.0f;
		pitch_frame_index--;
	}
	if (pitch_frame_index>=aiming_screen_bounds->positive_pitch_frame_count)
	{
		pitch_frame_index = aiming_screen_bounds->positive_pitch_frame_count-1;
		pitch_fraction = 1.0f;
	}
	if (pitch_frame_index<-aiming_screen_bounds->negative_pitch_frame_count)
	{
		pitch_frame_index = -aiming_screen_bounds->negative_pitch_frame_count;
		pitch_fraction = 0.0f;
	}
	pitch_frame_index += aiming_screen_bounds->negative_pitch_frame_count;

	if (pitch_frame_index<0 || pitch_frame_index>=grid_height ||
		yaw_frame_index<0 || yaw_frame_index>=grid_width)
	{
		return;
	}

	next_yaw_cell = yaw_frame_index+1==grid_width ? yaw_frame_index : yaw_frame_index+1;
	next_pitch_cell = pitch_frame_index+1==grid_height ? pitch_frame_index : pitch_frame_index+1;
	frame_index00 = yaw_frame_index+pitch_frame_index*grid_width;
	frame_index10 = next_yaw_cell+pitch_frame_index*grid_width;
	frame_index01 = yaw_frame_index+next_pitch_cell*grid_width;
	frame_index11 = next_yaw_cell+next_pitch_cell*grid_width;
	data00 = animation_get_frame_data(animation, frame_index00);
	data10 = animation_get_frame_data(animation, frame_index10);
	data01 = animation_get_frame_data(animation, frame_index01);
	data11 = animation_get_frame_data(animation, frame_index11);
	rotation_index = 0;
	translation_index = 0;

#ifdef HALO_N64
	/* on a frame of the grid (the looking screen while a unit looks where
	it aims): the blends below come to the corner frame alone, normalized
	twice, and its translation added -- the same numbers without the other
	three frames; an identity rotation changes nothing */
	if (yaw_fraction==0.0f && pitch_fraction==0.0f && !compressed)
	{
		for (node_index = 0; node_index<animation->node_count; node_index++)
		{
			struct real_orientation *orientation = &node_orientations[node_index];

			if (!(node_index&(LONG_BITS-1)))
			{
				short long_index = node_index>>LONG_BITS_BITS;

				translation_flags = animation->nodes_with_translation_flags[long_index];
				rotation_flags = animation->nodes_with_rotation_flags[long_index];
			}
			if (TEST_FLAG(rotation_flags, 0))
			{
				struct compressed_quaternion_8byte const *compressed00 = (struct compressed_quaternion_8byte const *)data00;

				if (compressed00->i || compressed00->j || compressed00->k ||
					compressed00->w!=COMPRESSED_QUATERNION_COMPONENT_MAXIMUM)
				{
					real_quaternion rotation;

					quaternion_decompress_8byte(compressed00, &rotation);
					quaternion_normalize(&rotation);
					quaternion_normalize(&rotation);
					quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
				}
				data00 += sizeof(struct compressed_quaternion_8byte);
			}
			rotation_flags >>= 1;
			if (TEST_FLAG(translation_flags, 0))
			{
				real_point3d const *translation00 = (real_point3d const *)data00;

				orientation->translation.x += translation00->x;
				orientation->translation.y += translation00->y;
				orientation->translation.z += translation00->z;
				data00 += sizeof(real_point3d);
			}
			translation_flags >>= 1;
		}
		return;
	}
#endif

	for (node_index = 0; node_index<animation->node_count; node_index++)
	{
		struct real_orientation *orientation = &node_orientations[node_index];

		if (!(node_index&(LONG_BITS-1)))
		{
			short long_index = node_index>>LONG_BITS_BITS;

			translation_flags = animation->nodes_with_translation_flags[long_index];
			rotation_flags = animation->nodes_with_rotation_flags[long_index];
		}

		if (TEST_FLAG(rotation_flags, 0))
		{
			real_quaternion rotation00;
			real_quaternion rotation10;
			real_quaternion rotation01;
			real_quaternion rotation11;
			real_quaternion yaw_rotation0;
			real_quaternion yaw_rotation1;
			real_quaternion rotation;

			if (compressed)
			{
				animation_get_keyframe_rotation(animation, (real)frame_index00, (short)rotation_index, node_index, &rotation00);
				animation_get_keyframe_rotation(animation, (real)frame_index10, (short)rotation_index, node_index, &rotation10);
				animation_get_keyframe_rotation(animation, (real)frame_index01, (short)rotation_index, node_index, &rotation01);
				animation_get_keyframe_rotation(animation, (real)frame_index11, (short)rotation_index++, node_index, &rotation11);
			}
			else
			{
				quaternion_decompress_8byte((struct compressed_quaternion_8byte const *)data00, &rotation00);
				data00 += sizeof(struct compressed_quaternion_8byte);
				quaternion_decompress_8byte((struct compressed_quaternion_8byte const *)data10, &rotation10);
				data10 += sizeof(struct compressed_quaternion_8byte);
				quaternion_decompress_8byte((struct compressed_quaternion_8byte const *)data01, &rotation01);
				data01 += sizeof(struct compressed_quaternion_8byte);
				quaternion_decompress_8byte((struct compressed_quaternion_8byte const *)data11, &rotation11);
				data11 += sizeof(struct compressed_quaternion_8byte);
			}

			quaternions_interpolate_and_normalize(&rotation00, &rotation10, yaw_fraction, &yaw_rotation0);
			quaternions_interpolate_and_normalize(&rotation01, &rotation11, yaw_fraction, &yaw_rotation1);
			quaternions_interpolate_and_normalize(&yaw_rotation0, &yaw_rotation1, pitch_fraction, &rotation);
			quaternions_multiply(&rotation, &orientation->rotation, &orientation->rotation);
		}
		rotation_flags >>= 1;

		if (TEST_FLAG(translation_flags, 0))
		{
			real_point3d translation00;
			real_point3d translation10;
			real_point3d translation01;
			real_point3d translation11;
			real inverse_yaw_fraction = 1.0f-yaw_fraction;
			real inverse_pitch_fraction = 1.0f-pitch_fraction;

			if (compressed)
			{
				animation_get_keyframe_translation(animation, (real)frame_index00, (short)translation_index, node_index, &translation00);
				animation_get_keyframe_translation(animation, (real)frame_index10, (short)translation_index, node_index, &translation10);
				animation_get_keyframe_translation(animation, (real)frame_index01, (short)translation_index, node_index, &translation01);
				animation_get_keyframe_translation(animation, (real)frame_index11, (short)translation_index++, node_index, &translation11);
			}
			else
			{
				translation00 = *(real_point3d const *)data00;
				data00 += sizeof(real_point3d);
				translation10 = *(real_point3d const *)data10;
				data10 += sizeof(real_point3d);
				translation01 = *(real_point3d const *)data01;
				data01 += sizeof(real_point3d);
				translation11 = *(real_point3d const *)data11;
				data11 += sizeof(real_point3d);
			}

			orientation->translation.x +=
				(translation01.x*inverse_yaw_fraction+translation11.x*yaw_fraction)*pitch_fraction+
				(translation00.x*inverse_yaw_fraction+translation10.x*yaw_fraction)*inverse_pitch_fraction;
			orientation->translation.y +=
				(translation01.y*inverse_yaw_fraction+translation11.y*yaw_fraction)*pitch_fraction+
				(translation00.y*inverse_yaw_fraction+translation10.y*yaw_fraction)*inverse_pitch_fraction;
			orientation->translation.z +=
				(translation00.z*inverse_yaw_fraction+translation10.z*yaw_fraction)*inverse_pitch_fraction+
				(translation01.z*inverse_yaw_fraction+translation11.z*yaw_fraction)*pitch_fraction;
		}
		translation_flags >>= 1;
	}

	return;
}

void quaternion_decompress_8byte(
	struct compressed_quaternion_8byte const *compressed,
	real_quaternion *quaternion)
{
	real const scale = 1.f / COMPRESSED_QUATERNION_COMPONENT_MAXIMUM;

	quaternion->v.i = compressed->i * scale;
	quaternion->v.j = compressed->j * scale;
	quaternion->v.k = compressed->k * scale;
	quaternion->w = compressed->w * scale;

	return;
}

void quaternion_decompress_6byte(
	struct compressed_quaternion_6byte const *compressed,
	real_quaternion *quaternion)
{
	word word0 = compressed->words[0];
	word word1 = compressed->words[1];
	word word2 = compressed->words[2];
	short i = (short)((word0 >> 12) | (word0 & 0xFFF0));
	short j = (short)(((word1 >> 4) & 0x0FF0) | (word0 & 0x000F) | (word0 << 12));
	short k = (short)(((((word2 >> 4) & 0x0F00) | (word1 & 0x00F0)) >> 4) | (word1 << 8));
	short w = (short)(((word2 >> 8) & 0x000F) | (word2 << 4));
	real const scale = 1.f / COMPRESSED_QUATERNION_COMPONENT_MAXIMUM;

	quaternion->v.i = i * scale;
	quaternion->v.j = j * scale;
	quaternion->v.k = k * scale;
	quaternion->w = w * scale;

	return;
}

void quaternion_decompress_6byte_renormalized(
	void const *compressed,
	real_quaternion *quaternion)
{
	quaternion_decompress_6byte(compressed, quaternion);
	quaternion_normalize(quaternion);

	return;
}

void quaternion_compress_8byte(
	real_quaternion const *quaternion,
	struct compressed_quaternion_8byte *compressed)
{
	compressed->i = (short)(quaternion->v.i * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	compressed->j = (short)(quaternion->v.j * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	compressed->k = (short)(quaternion->v.k * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	compressed->w = (short)(quaternion->w * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);

	return;
}

void quaternion_compress_6byte(
	real_quaternion const *quaternion,
	struct compressed_quaternion_6byte *compressed)
{
	long j = (long)(quaternion->v.j * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	long k = (long)(quaternion->v.k * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	long w = (long)(quaternion->w * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);
	long i = (long)(quaternion->v.i * COMPRESSED_QUATERNION_COMPONENT_MAXIMUM);

	compressed->words[0] = (word)((i & 0xFFF0) | ((word)j >> 12));
	compressed->words[1] = (word)(((j & 0xFFF0) << 4) | (((word)k >> 8) & 0x00FF));
	compressed->words[2] = (word)(((k & 0x00F0) << 8) | ((word)w >> 4));

	return;
}

/* ---------- private code */

static boolean animation_is_compressed(
	struct animation const *animation)
{
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		38,
		animation);

	return TEST_FLAG(animation->flags, _animation_compressed_bit) &&
		(hs_model_animation_compression_enabled || !animation->compressed_data_offset);
}

// binary search for the keyframe containing target_frame_index
static short animation_keyframe_search(
	short const *keyframe_frame_indices,
	short keyframe_count,
	short target_frame_index)
{
	short low = 0;
	short high = keyframe_count-1;
	short keyframe_index;
	short infinite_loop_killer = 0;

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1334,
		keyframe_count>1);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1335,
		keyframe_frame_indices);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1336,
		keyframe_frame_indices[0]>0);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1337,
		target_frame_index>=0 && target_frame_index<keyframe_frame_indices[keyframe_count-1]);

	while (TRUE)
	{
		keyframe_index = (low+high)>>1;

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1343,
			keyframe_index>=0 && keyframe_index<keyframe_count);

		if (keyframe_index+1<keyframe_count && keyframe_frame_indices[keyframe_index+1]<=target_frame_index)
		{
			low = keyframe_index;
		}
		else if (keyframe_frame_indices[keyframe_index]>target_frame_index)
		{
			high = keyframe_index;
		}
		else
		{
			break;
		}

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1356,
			++infinite_loop_killer<200);
	}

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1360,
		keyframe_index>=0 && keyframe_index<keyframe_count-1);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1361,
		target_frame_index>=keyframe_frame_indices[keyframe_index] && target_frame_index<keyframe_frame_indices[keyframe_index+1]);

	return keyframe_index;
}

static void animation_get_keyframe_rotation(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real_quaternion *rotation)
{
	byte *data = tag_data_get_pointer(&animation->data, animation->compressed_data_offset, 0);
	struct compressed_animation_header const *header = (struct compressed_animation_header const *)data;
	struct compressed_quaternion_6byte const *default_rotations = (struct compressed_quaternion_6byte const *)(data+header->default_rotations_offset);
	unsigned long node_header = header->rotation_node_headers[adjusted_node_index];
	short first_keyframe_index = (short)(node_header>>COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS);
	short keyframe_count = (short)(node_header&(FLAG(COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS)-1));
	word const *keyframe_frame_indices;
	struct compressed_quaternion_6byte const *keyframe_rotations;
	short frame_index;
	struct compressed_quaternion_6byte const *this_keyframe;
	struct compressed_quaternion_6byte const *next_keyframe;
	short this_keyframe_frame_index;
	short next_keyframe_frame_index;

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1428,
		real_frame_index>=0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1430,
		real_frame_index<(real)animation->frame_count);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1432,
		keyframe_count>=0);

	if (keyframe_count==0)
	{
		// this node never changes rotation
		quaternion_decompress_6byte(&default_rotations[node_index], rotation);
		quaternion_normalize(rotation);
		return;
	}

	keyframe_rotations = (struct compressed_quaternion_6byte const *)(data+header->rotation_keyframes_offset)+first_keyframe_index;
	keyframe_frame_indices = (word const *)(data+header->rotation_keyframe_frame_indices_offset)+first_keyframe_index;
	frame_index = (short)fast_ftol(N64_FLOOR(real_frame_index));

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1451,
		frame_index>=0 && frame_index<=keyframe_frame_indices[keyframe_count-1]);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1452,
		keyframe_frame_indices[keyframe_count-1]==animation->frame_count-1);

	if (frame_index<keyframe_frame_indices[0])
	{
		this_keyframe_frame_index = 0;
		this_keyframe = &default_rotations[node_index];
		next_keyframe_frame_index = keyframe_frame_indices[0];
		next_keyframe = keyframe_rotations;
	}
	else if (frame_index==keyframe_frame_indices[keyframe_count-1])
	{
		this_keyframe_frame_index = keyframe_frame_indices[keyframe_count-1];
		this_keyframe = &keyframe_rotations[keyframe_count-1];
		next_keyframe_frame_index = this_keyframe_frame_index+1;
		next_keyframe = &default_rotations[node_index];
	}
	else
	{
		short keyframe_index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, frame_index);

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1472,
			keyframe_index>=0 && keyframe_index<keyframe_count-1);

		this_keyframe_frame_index = keyframe_frame_indices[keyframe_index];
		this_keyframe = &keyframe_rotations[keyframe_index];
		next_keyframe_frame_index = keyframe_frame_indices[keyframe_index+1];
		next_keyframe = &keyframe_rotations[keyframe_index+1];
	}

	if (real_frame_index==(real)this_keyframe_frame_index)
	{
		quaternion_decompress_6byte(this_keyframe, rotation);
		quaternion_normalize(rotation);
	}
	else
	{
		real fraction = (real_frame_index-(real)this_keyframe_frame_index)/(next_keyframe_frame_index-this_keyframe_frame_index);
		real_quaternion this_rotation;
		real_quaternion next_rotation;

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1491,
			real_frame_index>=(real)this_keyframe_frame_index);
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1492,
			real_frame_index< (real)next_keyframe_frame_index);

		quaternion_decompress_6byte(this_keyframe, &this_rotation);
		quaternion_decompress_6byte(next_keyframe, &next_rotation);
		quaternions_interpolate_and_normalize(&this_rotation, &next_rotation, fraction, rotation);
	}

	return;
}

static void animation_get_keyframe_translation(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real_point3d *translation)
{
	byte *data = tag_data_get_pointer(&animation->data, animation->compressed_data_offset, 0);
	struct compressed_animation_header const *header = (struct compressed_animation_header const *)data;
	real_point3d const *default_translations = (real_point3d const *)(data+header->default_translations_offset);
	unsigned long node_header = ((unsigned long const *)(data+header->translation_node_headers_offset))[adjusted_node_index];
	short first_keyframe_index = (short)(node_header>>COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS);
	short keyframe_count = (short)(node_header&(FLAG(COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS)-1));

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1522,
		real_frame_index>=0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1524,
		real_frame_index<(real)animation->frame_count);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1526,
		keyframe_count>=0);

	if (keyframe_count==0)
	{
		*translation = default_translations[node_index];
	}
	else
	{
		real_point3d const *keyframe_translations = (real_point3d const *)(data+header->translation_keyframes_offset)+first_keyframe_index;
		word const *keyframe_frame_indices = (word const *)(data+header->translation_keyframe_frame_indices_offset)+first_keyframe_index;
		short frame_index = (short)fast_ftol(N64_FLOOR(real_frame_index));
		real_point3d const *this_keyframe;
		real_point3d const *next_keyframe;
		short this_keyframe_frame_index;
		short next_keyframe_frame_index;

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1545,
			frame_index>=0 && frame_index<=keyframe_frame_indices[keyframe_count-1]);
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1546,
			keyframe_frame_indices[keyframe_count-1]==animation->frame_count-1);

		if (frame_index<keyframe_frame_indices[0])
		{
			this_keyframe_frame_index = 0;
			this_keyframe = &default_translations[node_index];
			next_keyframe_frame_index = keyframe_frame_indices[0];
			next_keyframe = keyframe_translations;
		}
		else if (frame_index==keyframe_frame_indices[keyframe_count-1])
		{
			this_keyframe_frame_index = keyframe_frame_indices[keyframe_count-1];
			this_keyframe = &keyframe_translations[keyframe_count-1];
			next_keyframe_frame_index = this_keyframe_frame_index+1;
			next_keyframe = &default_translations[node_index];
		}
		else
		{
			short keyframe_index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, frame_index);

			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1566,
				keyframe_index>=0 && keyframe_index<keyframe_count-1);

			this_keyframe_frame_index = keyframe_frame_indices[keyframe_index];
			this_keyframe = &keyframe_translations[keyframe_index];
			next_keyframe_frame_index = keyframe_frame_indices[keyframe_index+1];
			next_keyframe = &keyframe_translations[keyframe_index+1];
		}

		if (real_frame_index==(real)this_keyframe_frame_index)
		{
			*translation = *this_keyframe;
		}
		else
		{
			real fraction = (real_frame_index-(real)this_keyframe_frame_index)/(next_keyframe_frame_index-this_keyframe_frame_index);

			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1583,
				real_frame_index>=(real)this_keyframe_frame_index);
			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1584,
				real_frame_index< (real)next_keyframe_frame_index);

			points_interpolate(this_keyframe, next_keyframe, fraction, translation);
		}
	}

	return;
}

static void animation_get_keyframe_scale(
	struct animation const *animation,
	real real_frame_index,
	short adjusted_node_index,
	short node_index,
	real *scale)
{
	byte *data = tag_data_get_pointer(&animation->data, animation->compressed_data_offset, 0);
	struct compressed_animation_header const *header = (struct compressed_animation_header const *)data;
	real const *default_scales = (real const *)(data+header->default_scales_offset);
	unsigned long node_header = ((unsigned long const *)(data+header->scale_node_headers_offset))[adjusted_node_index];
	short first_keyframe_index = (short)(node_header>>COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS);
	short keyframe_count = (short)(node_header&(FLAG(COMPRESSED_ANIMATION_NODE_HEADER_KEYFRAME_COUNT_BITS)-1));

	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1610,
		real_frame_index>=0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1612,
		real_frame_index<(real)animation->frame_count);
	match_assert(
		"c:\\halo\\SOURCE\\models\\model_animations.c",
		1614,
		keyframe_count>=0);

	if (keyframe_count==0)
	{
		*scale = default_scales[adjusted_node_index];
	}
	else
	{
		real const *keyframe_scales = (real const *)(data+header->scale_keyframes_offset)+first_keyframe_index;
		word const *keyframe_frame_indices = (word const *)(data+header->scale_keyframe_frame_indices_offset)+first_keyframe_index;
		short frame_index = (short)fast_ftol(N64_FLOOR(real_frame_index));
		real this_keyframe_scale;
		real next_keyframe_scale;
		short this_keyframe_frame_index;
		short next_keyframe_frame_index;

		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1634,
			frame_index>=0 && frame_index<=keyframe_frame_indices[keyframe_count-1]);
		match_assert(
			"c:\\halo\\SOURCE\\models\\model_animations.c",
			1635,
			keyframe_frame_indices[keyframe_count-1]==animation->frame_count-1);

		if (frame_index<keyframe_frame_indices[0])
		{
			this_keyframe_frame_index = 0;
			this_keyframe_scale = default_scales[adjusted_node_index];
			next_keyframe_frame_index = keyframe_frame_indices[0];
			next_keyframe_scale = keyframe_scales[0];
		}
		else if (frame_index==keyframe_frame_indices[keyframe_count-1])
		{
			this_keyframe_frame_index = keyframe_frame_indices[keyframe_count-1];
			this_keyframe_scale = keyframe_scales[keyframe_count-1];
			next_keyframe_frame_index = this_keyframe_frame_index+1;
			next_keyframe_scale = default_scales[adjusted_node_index];
		}
		else
		{
			short keyframe_index = animation_keyframe_search(keyframe_frame_indices, keyframe_count, frame_index);

			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1655,
				keyframe_index>=0 && keyframe_index<keyframe_count-1);

			this_keyframe_frame_index = keyframe_frame_indices[keyframe_index];
			this_keyframe_scale = keyframe_scales[keyframe_index];
			next_keyframe_frame_index = keyframe_frame_indices[keyframe_index+1];
			next_keyframe_scale = keyframe_scales[keyframe_index+1];
		}

		if (real_frame_index==(real)this_keyframe_frame_index)
		{
			*scale = this_keyframe_scale;
		}
		else
		{
			real fraction = (real_frame_index-(real)this_keyframe_frame_index)/(next_keyframe_frame_index-this_keyframe_frame_index);

			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1672,
				real_frame_index>=(real)this_keyframe_frame_index);
			match_assert(
				"c:\\halo\\SOURCE\\models\\model_animations.c",
				1673,
				real_frame_index< (real)next_keyframe_frame_index);

			scalars_interpolate(this_keyframe_scale, next_keyframe_scale, fraction, scale);
		}
	}

	return;
}
