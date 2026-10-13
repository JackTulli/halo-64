/*
FONT_GROUP.C

symbols in this file:
0018C840 0070:
	_font_get_character_by_ascii_code (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "font_group.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

struct font_character *font_get_character_by_ascii_code(
	struct font_header *font,
	word character_code)
{
	struct font_character_table *character_table = TAG_BLOCK_GET_ELEMENT(
		&font->character_tables,
		character_code >> 8,
		struct font_character_table);
	struct font_character *character = NULL;

	if (character_table->character_indices.count > 0)
	{
		short *character_index = character_table->character_indices.count == 256
			? TAG_BLOCK_GET_ELEMENT(&character_table->character_indices, character_code & 0xFF, short)
			: NULL;

		if (*character_index != NONE)
		{
			character = tag_block_get_element_with_size(
				&font->characters,
				*character_index,
				FONT_CHARACTER_SIZE);
		}
	}

	return character;
}

/* ---------- private code */
