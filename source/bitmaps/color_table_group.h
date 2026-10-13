/*
COLOR_TABLE_GROUP.H

header included in hcex build.
*/

#ifndef __COLOR_TABLE_GROUP_H
#define __COLOR_TABLE_GROUP_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	COLOR_TABLE_TAG = 'colo',
};

/* ---------- macros */

#define color_table_definition_get(index) \
	((struct color_table_definition *)tag_get(COLOR_TABLE_TAG, (index)))

/* ---------- structures */

struct color_table_color
{
	char name[32];
	real_argb_color real_color;
};

struct color_table_definition
{
	struct tag_block colors;
};

typedef char verify_color_table_color_size[
	sizeof(struct color_table_color) == 0x30 ? 1 : -1];

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

#endif // __COLOR_TABLE_GROUP_H
