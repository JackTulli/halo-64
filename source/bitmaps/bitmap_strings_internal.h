/*
BITMAP_STRINGS_INTERNAL.H

Narrow string-query interface owned by BITMAPS.C.  It remains separate from
bitmaps_internal.h because January's VC7 output is declaration-position
sensitive in bitmap_group.c.
*/

#ifndef __BITMAP_STRINGS_INTERNAL_H
#define __BITMAP_STRINGS_INTERNAL_H
#pragma once

char const *bitmap_type_get_string(
	short type);

#endif /* __BITMAP_STRINGS_INTERNAL_H */
