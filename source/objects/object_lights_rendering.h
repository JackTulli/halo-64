/*
OBJECT_LIGHTS_RENDERING.H

Narrow cross-translation-unit rendering interface owned by OBJECT_LIGHTS.C.
*/

#ifndef __OBJECT_LIGHTS_RENDERING_H
#define __OBJECT_LIGHTS_RENDERING_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- structures */

union real_point3d;
struct render_lighting;

/* ---------- prototypes/OBJECT_LIGHTS.C */

void lights_preprocess_scene(
	void);
void lights_render_diffuse(
	void);
void lights_render_specular(
	void);
boolean lights_distant_lighting_at_point(
	long flags,
	union real_point3d const *position,
	struct render_lighting *lighting);
void lights_prepare_for_object_static(
	long object_index,
	struct render_lighting *lighting);

/* ---------- globals */

extern boolean object_light_interpolate;
extern real object_light_ambient_base;
extern real object_light_ambient_scale;
extern real object_light_secondary_scale;

#endif /* __OBJECT_LIGHTS_RENDERING_H */
