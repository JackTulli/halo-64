/*
BITMAP_GROUP_LOOKUP.H

Narrow cross-translation-unit lookup interface owned by BITMAP_GROUP.C.
*/

#ifndef __BITMAP_GROUP_LOOKUP_H
#define __BITMAP_GROUP_LOOKUP_H
#pragma once

/* ---------- structures */

struct bitmap_data;

/* ---------- prototypes/BITMAP_GROUP.C */

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index);

#endif /* __BITMAP_GROUP_LOOKUP_H */
