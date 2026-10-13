/*
RENDER_CAMERA_PROJECTION.H

Narrow projection interface kept separate from the shared camera-structure
header so unrelated translation units retain their January declaration order.
*/

#ifndef __RENDER_CAMERA_PROJECTION_H
#define __RENDER_CAMERA_PROJECTION_H
#pragma once

struct render_camera;
struct render_frustum;
union real_point2d;
union real_point3d;

boolean render_camera_view_to_screen(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	union real_point3d const *view_point,
	union real_point2d *screen_point);

#endif // __RENDER_CAMERA_PROJECTION_H
