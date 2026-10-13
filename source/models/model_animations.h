/*
MODEL_ANIMATIONS.H
*/

#ifndef __MODEL_ANIMATIONS_H
#define __MODEL_ANIMATIONS_H
#pragma once

/* ---------- headers */

#include "models/model_animation_definitions.h"

/* ---------- prototypes/MODEL_ANIMATIONS.C */

void overlay_animation_apply_continuous_scaled(
	struct animation const *animation,
	real real_frame_index,
	real animation_scale,
	struct real_orientation *node_orientations);
void animation_graph_node_matrices_from_orientations(
	long animation_graph_index,
	struct real_matrix4x3 *node_matrices,
	struct real_orientation const *node_orientations,
	real_point3d const *origin,
	real_vector3d const *forward,
	real_vector3d const *up);

#endif // __MODEL_ANIMATIONS_H
