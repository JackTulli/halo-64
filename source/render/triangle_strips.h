/*
TRIANGLE_STRIPS.H

header included in hcex build.
*/

#ifndef __TRIANGLE_STRIPS_H
#define __TRIANGLE_STRIPS_H
#pragma once

/* ---------- constants */

enum
{
	_valid_strip_iterator_signature = 's',
};

/* ---------- macros */

/* ---------- structures */

struct triangle_strip_iterator
{
	short strip_count;
	short vertex_count;
	word const *triangle_strip_vertex_indices;
	boolean reverse_winding;
	char signature;
};

typedef char triangle_strip_iterator_size_assert[(sizeof(struct triangle_strip_iterator) == 12) ? 1 : -1];
typedef char triangle_strip_iterator_indices_offset_assert[(offsetof(struct triangle_strip_iterator, triangle_strip_vertex_indices) == 4) ? 1 : -1];
typedef char triangle_strip_iterator_winding_offset_assert[(offsetof(struct triangle_strip_iterator, reverse_winding) == 8) ? 1 : -1];
typedef char triangle_strip_iterator_signature_offset_assert[(offsetof(struct triangle_strip_iterator, signature) == 9) ? 1 : -1];

/* ---------- prototypes/TRIANGLE_STRIPS.C */

void triangle_strip_iterator_new(
	struct triangle_strip_iterator *iterator,
	word const *triangle_strip_vertex_indices,
	short strip_count);
boolean triangle_strip_iterator_get_triangle(
	struct triangle_strip_iterator *iterator,
	word *vertices);

/* ---------- globals */

/* ---------- public code */

#endif // __TRIANGLE_STRIPS_H
