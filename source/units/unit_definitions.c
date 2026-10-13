/*
UNIT_DEFINITIONS.C

symbols in this file:
00196100 0050:
	_unit_definition_get_active_hud_index (0000)
00196150 0060:
	_unit_definition_get_seat_active_hud_index (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "unit_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

long unit_definition_get_active_hud_index(
	struct unit_definition const *definition,
	boolean multiplayer)
{
	short hud_index = multiplayer != FALSE;
	hud_index = MIN(hud_index, definition->unit.huds.count - 1);

	if (hud_index < 0)
		return NONE;

	return verify_tag_reference(&TAG_BLOCK_GET_ELEMENT(
		&definition->unit.huds,
		hud_index,
		struct unit_hud_reference)->hud);
}

long unit_definition_get_seat_active_hud_index(
	struct unit_definition const *definition,
	short seat_index,
	boolean multiplayer)
{
	struct unit_seat *seat = TAG_BLOCK_GET_ELEMENT(
		&definition->unit.seats,
		seat_index,
		struct unit_seat);
	short hud_index = multiplayer != FALSE;
	hud_index = MIN(hud_index, seat->seat_huds.count - 1);

	if (hud_index < 0)
		return NONE;

	return verify_tag_reference(&TAG_BLOCK_GET_ELEMENT(
		&seat->seat_huds,
		hud_index,
		struct unit_hud_reference)->hud);
}

/* ---------- private code */
