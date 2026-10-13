/*
RASTERIZER_GLOBALS_INTERNAL.H

Narrow cross-translation-unit interface owned by RASTERIZER.C.
*/

#ifndef __RASTERIZER_GLOBALS_INTERNAL_H
#define __RASTERIZER_GLOBALS_INTERNAL_H
#pragma once

#include "cseries.h"

struct rasterizer_global_defaults
{
	real near_clip_distance;
	real far_clip_distance;
	real first_person_weapon_near_clip_distance;
	real first_person_weapon_far_clip_distance;
};

extern const struct rasterizer_global_defaults rasterizer_global_defaults;

#endif /* __RASTERIZER_GLOBALS_INTERNAL_H */
