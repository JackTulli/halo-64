/*
RENDER_CAMERAS_INTERNAL.H

Narrow cross-translation-unit interface owned by RENDER_CAMERAS.C.
*/

#ifndef __RENDER_CAMERAS_INTERNAL_H
#define __RENDER_CAMERAS_INTERNAL_H
#pragma once

#include "render_cameras.h"

struct render_mirror;

word render_frustum_build_point_flags(
	struct render_frustum const *frustum,
	real_point3d const *point);
real render_frustum_sphere_diameter_in_pixels(
	struct render_frustum const *frustum,
	real_point3d const *point,
	real radius);
real render_frustum_cube_view_fraction(
	struct render_frustum const *frustum,
	real_rectangle3d const *bounds);
void render_frustum_get_projection_bounds(
	struct render_frustum const *frustum,
	real_rectangle2d *bounds);
short render_frustum_sphere_visible(
	struct render_frustum const *frustum,
	real_point3d const *point,
	real radius);
short render_frustum_cube_visible(
	struct render_frustum const *frustum,
	real_rectangle3d const *bounds,
	boolean test_frustum_against_cube);
boolean render_frustum_triangle_visible(
	struct render_frustum const *frustum,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2);
boolean render_camera_triangle_frontfacing(
	struct render_camera const *camera,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2);
boolean render_camera_build_clipped_frustum_bounds(
	struct render_camera const *camera,
	real_rectangle2d const *clip,
	real_rectangle2d *frustum_bounds);
void render_camera_build_frustum_bounds(
	struct render_camera const *camera,
	real_rectangle2d *frustum_bounds);
void render_camera_mirror(
	struct render_camera const *camera,
	struct render_mirror const *mirror,
	struct render_camera *result);
boolean render_camera_view_to_screen(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	real_point3d const *view_point,
	real_point2d *screen_point);
void render_camera_screen_to_view(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	real_point2d const *screen_point,
	real_vector3d *view_vector);
boolean render_camera_world_to_screen(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	real_point3d const *world_point,
	real_point2d *screen_point);
void render_camera_screen_to_world(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	real_point2d const *screen_point,
	real_point3d *world_point,
	real_vector3d *world_vector);

#endif /* __RENDER_CAMERAS_INTERNAL_H */
