/*
RASTERIZER_XBOX_WATER.H
*/

#ifndef __RASTERIZER_XBOX_WATER_H
#define __RASTERIZER_XBOX_WATER_H
#pragma once

struct transparent_geometry_group;

void rasterizer_water_draw(
	struct transparent_geometry_group const *group);
boolean rasterizer_water_get_visibility_for_window(
	void);

#endif /* __RASTERIZER_XBOX_WATER_H */
