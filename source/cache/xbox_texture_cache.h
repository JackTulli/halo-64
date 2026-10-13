/*
XBOX_TEXTURE_CACHE.H

focused owner header for the xbox_texture_cache.c bitmap-format-to-D3D mapping
routines that the platform-neutral texture_cache.h does not declare; adding them
to texture_cache.h regresses exact consumers through declaration-count
sensitivity.
*/

#ifndef __XBOX_TEXTURE_CACHE_H
#define __XBOX_TEXTURE_CACHE_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- prototypes/XBOX_TEXTURE_CACHE.C */

long bitmap_format_to_d3d_format(
	short format,
	word flags);
long bitmap_format_to_d3d_linear_format(
	short format,
	word flags);

#endif // __XBOX_TEXTURE_CACHE_H
