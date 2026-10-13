/*
UI_WIDGET_DEFINITIONS.H

January-derived UI widget tag-definition prefix.
*/

#ifndef __UI_WIDGET_DEFINITIONS_H
#define __UI_WIDGET_DEFINITIONS_H
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	UI_WIDGET_DEFINITION_TAG = 'DeLa'
};

enum
{
	_list_items_generated_in_code,
	_list_items_generated_from_string_list_tag,
	_list_items_only_one_tooltip_entry,
	_list_single_preview_box_no_scroll,
	NUMBER_OF_UI_WIDGET_LIST_FLAGS
};

/* ---------- macros */

#define ui_widget_definition_get(index) \
	((struct ui_widget_definition *)tag_get(UI_WIDGET_DEFINITION_TAG, (index)))

/* ---------- structures */

struct ui_widget_definition
{
	short type;                         /* 0x000 */
	byte unknown002[0x52];              /* 0x002 */
	struct tag_block event_handlers;    /* 0x054 */
	byte unknown060[0xF0];              /* 0x060 */
	long list_flags;                    /* 0x150 */
	byte unknown154[0x28C];             /* 0x154 */
	long child_count;                   /* 0x3E0, tag-block count */
	byte unknown3E4[8];                 /* 0x3E4, remainder of tag block */
};

#endif // __UI_WIDGET_DEFINITIONS_H
