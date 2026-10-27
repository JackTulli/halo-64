/*
REFERENCE_LISTS.H

file has inline function assertions.
*/

#ifndef __REFERENCE_LISTS_H
#define __REFERENCE_LISTS_H
#pragma once

/* ---------- headers */

#include "game_state.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct data_reference
{
	short identifier;
	word pad;
	long datum_index;
	long next_reference_index;
};

#ifdef HALO_N64
typedef char n64_data_reference_size_assert[sizeof(struct data_reference) == 12 ? 1 : -1];
#endif

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

__inline struct data_array *reference_list_new(
	const char *name,
	short maximum_count)
{
	char buffer[256];

	sprintf(buffer, "%s reference", name);
	return game_state_data_new(buffer, maximum_count, sizeof(struct data_reference));
}

__inline long reference_list_get_next_datum_index(
	struct data_array *array,
	long *reference_index)
{
	long result;
	struct data_reference *reference;

	if (*reference_index!=NONE)
	{
#ifdef HALO_N64
		/* datum_get's tests, inline and without its multiply: every
		reference list is reference_list_new's, an array of
		sizeof(struct data_reference) elements. A cluster's objects are
		walked through here for each collision query (a reference and a call
		to datum_get for each object in the clusters it reaches). As in
		objects.h's object lookups, one unsigned compare of the index's low
		half stands for "not negative, below the count"; an index that is no
		reference's reads through NULL, as datum_get's did. */
		long index = *reference_index;
		unsigned long absolute_index = (unsigned short)index;
		long identifier = index >> 16;

		reference = NULL;
		if (absolute_index < (unsigned long)(unsigned short)array->count)
		{
			reference = (struct data_reference *)array->data + absolute_index;
			if (identifier ? reference->identifier != identifier : !reference->identifier)
				reference = NULL;
		}
#else
		reference = (struct data_reference *)datum_get(array, *reference_index);
#endif
		*reference_index = reference->next_reference_index;
		result = reference->datum_index;
	}
	else
	{
		result = NONE;
	}

	return result;
}

__inline void reference_list_add(
	struct data_array *array,
	long *first_reference_index,
	long datum_index)
{
	long reference_index = datum_new(array);

	if (reference_index!=NONE)
	{
		struct data_reference *reference = (struct data_reference*)datum_get(array, reference_index);
		reference->datum_index = datum_index;
		reference->next_reference_index = *first_reference_index;
		*first_reference_index = reference_index;
	}
	else
	{
		error(_error_silent, "WARNING: maximum %ss per map (%d) exceeded.", array->name, array->maximum_count);
	}

	return;
}

__inline void reference_list_delete(
	struct data_array *array,
	long first_reference_index)
{
	long i;
	struct data_reference *reference;

	for (i = first_reference_index; i!=NONE; i = reference->next_reference_index)
	{
		reference = (struct data_reference*)datum_get(array, i);
		datum_delete(array, i);
	}

	return;
}

#endif // __REFERENCE_LISTS_H
