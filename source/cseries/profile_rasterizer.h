/*
PROFILE_RASTERIZER.H

Narrow rasterizer statistics interface owned by PROFILE.C.
*/

#ifndef __PROFILE_RASTERIZER_H
#define __PROFILE_RASTERIZER_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- prototypes/PROFILE.C */

void profile_rasterizer_stats(
	real gpu_time,
	__int64 pushbuffer_size);

#endif // __PROFILE_RASTERIZER_H
