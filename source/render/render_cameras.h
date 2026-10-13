/*
RENDER_CAMERAS.H

header included in hcex build.
*/

#ifndef __RENDER_CAMERAS_H
#define __RENDER_CAMERAS_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"
#include "math/integer_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct render_screen_flash
{
	short type;
	real intensity;
	real_argb_color color;
};

struct render_screen_effect
{
	short type;
	real intensity;
};

struct render_camera
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	boolean mirrored;
	real vertical_field_of_view;
	rectangle2d viewport_bounds;
	rectangle2d window_bounds;
	real z_near;
	real z_far;
	real_plane3d mirror_plane;
};

struct render_window
{
	short local_player_index;
	boolean console_window;
	struct render_camera render_camera;
	struct render_camera rasterizer_camera;
};

struct render_frustum
{
	real_rectangle2d frustum_bounds;
	real_matrix4x3 world_to_view;
	real_matrix4x3 view_to_world;
	real_plane3d world_planes[6];
	real z_near;
	real z_far;
	real_point3d world_vertices[5];
	real_point3d world_midpoint;
	real_rectangle3d world_bounds;
	boolean projection_valid;
	real projection_matrix[4][4];
	real_vector2d projection_world_to_screen;
};

struct render_fog
{
	word fog_definition_flags;
	word runtime_flags;
	real_rgb_color atmospheric_color;
	real atmospheric_maximum_density;
	real atmospheric_minimum_distance;
	real atmospheric_maximum_distance;
	short planar_mode;
	real_plane3d plane;
	real_rgb_color planar_color;
	real planar_maximum_density;
	real planar_maximum_distance;
	real planar_maximum_depth;
	const struct fog_screen *screen;
	real screen_external_intensity;
};

/* ---------- prototypes/RENDER_CAMERAS.C */

void render_camera_build_frustum(
	const struct render_camera *camera,
	const real_rectangle2d *frustum_bounds,
	struct render_frustum *frustum,
	boolean build_projection);
real render_camera_get_adjusted_field_of_view_tangent(
	real field_of_view);
void render_camera_hack_frustum_z(
	struct render_frustum *frustum,
	real z_near,
	real z_far);
void render_camera_debug_frustum(
	struct render_camera const *camera,
	struct render_frustum const *frustum);

/* ---------- globals */

/* ---------- public code */

#endif // __RENDER_CAMERAS_H
