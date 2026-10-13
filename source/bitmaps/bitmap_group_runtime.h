/*
BITMAP_GROUP_RUNTIME.H
*/

#ifndef __BITMAP_GROUP_RUNTIME_H
#define __BITMAP_GROUP_RUNTIME_H
#pragma once

#include "bitmaps/bitmap_group.h"

/* ---------- prototypes/BITMAP_GROUP.C */

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index);

#endif // __BITMAP_GROUP_RUNTIME_H
