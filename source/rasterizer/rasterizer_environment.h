/*
RASTERIZER_ENVIRONMENT.H

Narrow cross-translation-unit environment interface owned by RASTERIZER.C.
*/

#ifndef __RASTERIZER_ENVIRONMENT_H
#define __RASTERIZER_ENVIRONMENT_H
#pragma once

#include "cseries.h"

/* ---------- constants */

enum rasterizer_lightmap_mode
{
	_rasterizer_lightmap_mode_normal = 0,
	_rasterizer_lightmap_mode_no_lightmap,
	_rasterizer_lightmap_mode_fullbright,
	_rasterizer_lightmap_mode_lightmap_bitmap,
	_rasterizer_lightmap_mode_fullbright_without_bump,
};

/* ---------- prototypes/RASTERIZER.C */

void rasterizer_environment_lightmaps_begin(
	void);
void rasterizer_environment_diffuse_textures_begin(
	void);
void rasterizer_environment_specular_lightmaps_begin(
	void);
void rasterizer_environment_reflection_lightmap_masks_begin(
	void);
void rasterizer_environment_diffuse_lights_begin(
	void);
void rasterizer_environment_specular_lights_begin(
	void);
void rasterizer_environment_fog_begin(
	void);
void rasterizer_environment_fog_end(
	void);
void rasterizer_environment_fog_screen_end(
	void);

#endif // __RASTERIZER_ENVIRONMENT_H
