/*
RASTERIZER_ACTIVE_CAMOUFLAGE.H
*/

#ifndef __RASTERIZER_ACTIVE_CAMOUFLAGE_H
#define __RASTERIZER_ACTIVE_CAMOUFLAGE_H
#pragma once

struct transparent_geometry_group;

/* ---------- prototypes/RASTERIZER_XBOX_ACTIVE_CAMOUFLAGE.C */

void rasterizer_active_camouflage_cache_primary_render_target(
	void);
void rasterizer_active_camouflage_draw(
	struct transparent_geometry_group *group);

#endif /* __RASTERIZER_ACTIVE_CAMOUFLAGE_H */
