/*
STRUCTURE_RENDER_LIGHTS.H

Narrow cross-translation-unit light-rendering interface owned by
STRUCTURE_RENDER.C.
*/

#ifndef __STRUCTURE_RENDER_LIGHTS_H
#define __STRUCTURE_RENDER_LIGHTS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- structures */

union real_point3d;

/* ---------- prototypes/STRUCTURE_RENDER.C */

void structure_render_diffuse_light(
	long rasterizer_light_index,
	union real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	short cluster_count,
	short *cluster_indices);
void structure_render_specular_light(
	long rasterizer_light_index,
	union real_point3d const *bounding_sphere_center,
	real bounding_sphere_radius,
	short cluster_count,
	short *cluster_indices);

#endif /* __STRUCTURE_RENDER_LIGHTS_H */
