/*
RASTERIZER_XBOX_DEBUG.H
*/

#ifndef __RASTERIZER_XBOX_DEBUG_H
#define __RASTERIZER_XBOX_DEBUG_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- prototypes/RASTERIZER_XBOX_DEBUG.C */

/* rasterizer_xbox_debug.c defines rasterizer_debug_drawing_begin with the single
 * parameter opaque, as rasterizer.h declares it. This declaration, used only by
 * rasterizer_debug.c, adds zbias because January's rasterizer_debug_draw (0x56d9e0
 * +0x20e, +0x2ee, +0x3a0) pushes two arguments while the callee (0x549db0) reads only
 * [ebp+8]. The two declarations conflict (C4031); under cdecl the callee ignores the
 * extra argument. Source-policy approval pending (2026-09-27 audit). */
void rasterizer_debug_drawing_begin(
	boolean opaque,
	long zbias);
void rasterizer_debug_drawing_end(
	void);

#endif // __RASTERIZER_XBOX_DEBUG_H
