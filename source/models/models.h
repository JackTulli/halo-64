/*
MODELS.H

header included in hcex build.
*/

#ifndef __MODELS_H
#define __MODELS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct model;
struct model_geometry_part;
struct object_marker;
struct real_matrix4x3;
struct real_orientation;
struct render_lighting;
struct render_model_effect;
union real_point3d;
union real_rgb_color;
union real_vector3d;

/* ---------- prototypes/MODELS.C */

void model_interpolate_node_orientations(
	struct model const *model,
	struct real_orientation *original_node_orientations,
	struct real_orientation *target_node_orientations,
	short frame_index,
	short frame_count);
void model_get_node_orientations(
	struct model const *model,
	struct real_orientation *node_orientations);
void model_get_node_matrices(
	struct model const *model,
	struct real_matrix4x3 *node_matrices,
	union real_point3d const *origin,
	union real_vector3d const *forward,
	union real_vector3d const *up);
void model_node_matrices_from_orientations(
	struct model const *model,
	struct real_matrix4x3 *node_matrices,
	struct real_orientation const *node_orientations,
	union real_point3d const *origin,
	union real_vector3d const *forward,
	union real_vector3d const *up);
short model_find_marker(
	long model_index,
	char const *name);
struct real_matrix4x3 *model_get_default_inverse_matrix(
	struct model *model,
	short node_index);
short model_find_node(
	long model_index,
	char const *name);
short model_get_marker_by_name(
	long model_index,
	char const *name,
	byte const *region_permutations,
	short const *node_remapping_table,
	short node_count,
	struct real_matrix4x3 const *node_matrices,
	boolean mirrored_flag,
	struct object_marker *markers,
	short maximum_marker_count);
void model_build_tangent_matrices(
	struct model *model);
void model_geometry_part_build_tangent_matrices(
	struct model_geometry_part *part);
void render_model(
	long model_index,
	real level_of_detail_pixels,
	struct real_matrix4x3 const *node_matrices,
	char const *region_permutation_indices,
	union real_rgb_color const *change_colors,
	real const *function_values,
	struct render_lighting const *lighting,
	union real_point3d const *centroid,
	real radius,
	struct render_model_effect const *model_effect,
	long unique_identifier,
	short forced_shader_permutation_index,
	unsigned long flags);

/* ---------- globals */

/* ---------- public code */

#endif // __MODELS_H
