/*
TEXT_GROUP.H

header included in hcex build.
*/

#ifndef __TEXT_GROUP_H
#define __TEXT_GROUP_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	STRING_LIST_TAG = 'str#',
	UNICODE_STRING_LIST_TAG = 'ustr',
};

enum
{
	_text_style_plain = NONE,
	_text_style_bold = 0,
	_text_style_italic,
	_text_style_condense,
	_text_style_underline,
	NUMBER_OF_TEXT_STYLES
};

/* ---------- macros */

#define string_list_definition_get(index) ((struct string_list *)tag_get(STRING_LIST_TAG, (index)))
#define unicode_string_list_definition_get(index) ((struct string_list *)tag_get(UNICODE_STRING_LIST_TAG, (index)))

/* ---------- structures */

struct string_list
{
	struct tag_block strings;
};

struct string_list_entry
{
	struct tag_data string;
};

/* ---------- prototypes/TEXT_GROUP.C */

char *string_list_get_string(long tag_index, short string_index);
wchar_t *unicode_string_list_get_string(long tag_index, short string_index);

/* ---------- globals */

/* ---------- public code */

#endif // __TEXT_GROUP_H
