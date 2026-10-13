/*
BITMAP_GROUP_INTERNAL.H

Narrow cross-translation-unit interface owned by BITMAP_GROUP.C.
*/

#ifndef __BITMAP_GROUP_INTERNAL_H
#define __BITMAP_GROUP_INTERNAL_H
#pragma once

struct bitmap_data;
struct bitmap_group;

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index);
short bitmap_group_add_bitmap(
	struct bitmap_group *group,
	short width,
	short height,
	short depth,
	short type,
	short format,
	short mipmap_count);

#endif /* __BITMAP_GROUP_INTERNAL_H */
