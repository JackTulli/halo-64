/*
RASTERIZER_XBOX_HARDWARE_BITMAPS.H

Bitmap lifecycle interface owned by RASTERIZER_XBOX_HARDWARE_BITMAPS.C.
*/

#ifndef __RASTERIZER_XBOX_HARDWARE_BITMAPS_H
#define __RASTERIZER_XBOX_HARDWARE_BITMAPS_H
#pragma once

#include "cseries/cseries.h"

struct bitmap_data;

boolean rasterizer_bitmap_new(
	struct bitmap_data *bitmap);
void rasterizer_bitmap_delete(
	struct bitmap_data *bitmap);
void rasterizer_bitmap_changed(
	struct bitmap_data *bitmap);

#endif /* __RASTERIZER_XBOX_HARDWARE_BITMAPS_H */
