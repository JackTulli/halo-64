/*
FONT_GROUP.H

header included in hcex build.
*/

#ifndef __FONT_GROUP_H
#define __FONT_GROUP_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"
#include "text/text_group.h"

/* ---------- constants */

enum
{
	FONT_GROUP_TAG = 'font',
	FONT_CHARACTER_SIZE = 0x14,
};

/* ---------- macros */

#define font_definition_get(index) ((struct font_header *)tag_get(FONT_GROUP_TAG, index))

/* ---------- structures */

struct font_character;

struct font_character_table
{
	struct tag_block character_indices;
};

struct font_header
{
	unsigned long flags;
	short ascending_height;
	short descending_height;
	short leading_height;
	short leading_width;
	long pad[9];
	struct tag_block character_tables;
	struct tag_reference style_fonts[NUMBER_OF_TEXT_STYLES];
	struct tag_block characters;
	struct tag_data pixels;
};

/* ---------- prototypes/FONT_GROUP.C */

/* ---------- globals */

/* ---------- public code */

struct font_character *font_get_character_by_ascii_code(struct font_header *font, word character_code);

#endif // __FONT_GROUP_H
