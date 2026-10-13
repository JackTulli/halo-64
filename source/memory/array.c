/*
ARRAY.C

symbols in this file:
00107340 0070:
	_dynamic_array_new (0000)
001073B0 0160:
	_dynamic_array_resize (0000)
00107510 00b0:
	_dynamic_array_delete (0000)
001075C0 0140:
	_dynamic_array_add_element (0000)
00107700 0110:
	_dynamic_array_get_element (0000)
00107810 01a0:
	_dynamic_array_delete_element (0000)
001079B0 00d0:
	_static_array_new (0000)
00107A80 0110:
	_static_array_resize (0000)
00107B90 00f0:
	_static_array_add_element (0000)
00107C80 00c0:
	_static_array_get_element (0000)
00107D40 0100:
	_static_array_delete_element (0000)
0027CE78 0024:
	??_C@_0CE@KCKHAMIF@memory_dynamic_array_delete_elem@ (0000)
0027CE9C 0021:
	??_C@_0CB@NKFOJKLF@memory_dynamic_array_add_element@ (0000)
0027CEC0 001c:
	??_C@_0BM@IGGAIAHM@memory_dynamic_array_resize?$AA@ (0000)
0027CEDC 000f:
	??_C@_0P@CJCKPFMH@element_size?$DO0?$AA@ (0000)
0027CEEC 0006:
	??_C@_05HIFJDKLD@array?$AA@ (0000)
0027CEF4 001e:
	??_C@_0BO@EGDDFGGJ@c?3?2halo?2SOURCE?2memory?2array?4c?$AA@ (0000)
0027CF14 002b:
	??_C@_0CL@PCJNGNEA@?$CIarray?9?$DOcount?$CB?$DN0?$CJ?$DN?$DN?$CIarray?9?$DOeleme@ (0000)
0027CF40 0010:
	??_C@_0BA@HPNKGLPB@array?9?$DOcount?$DO?$DN0?$AA@ (0000)
0027CF50 0016:
	??_C@_0BG@KKKOHAGJ@array?9?$DOelement_size?$DO0?$AA@ (0000)
0027CF68 001f:
	??_C@_0BP@JEHOMNNO@index?$DO?$DN0?5?$CG?$CG?5index?$DMarray?9?$DOcount?$AA@ (0000)
0027CF88 0022:
	??_C@_0CC@JPIBLJKM@array?9?$DOelement_size?$DN?$DNelement_siz@ (0000)
0027CFAC 0021:
	??_C@_0CB@JIEOIIBG@maximum_count?$DM?$DNUNSIGNED_CHAR_MAX@ (0000)
0027CFD0 0009:
	??_C@_08DIGDHHDM@elements?$AA@ (0000)
0027CFDC 0006:
	??_C@_05IOMEMJEC@count?$AA@ (0000)
0027CFE4 0013:
	??_C@_0BD@BKIDJHK@count?5?$CG?$CG?5?$CKcount?$DO?$DN0?$AA@ (0000)
0027CFF8 0018:
	??_C@_0BI@BFNCGHOC@index?$DO?$DN0?5?$CG?$CG?5index?$DMcount?$AA@ (0000)
0027D010 0019:
	??_C@_0BJ@BPOKGEAL@index?$DO?$DN0?5?$CG?$CG?5index?$DM?$CKcount?$AA@ (0000)
0027D02C 0012:
	??_C@_0BC@OADPDCKA@count?5?$CG?$CG?5?$CKcount?$DO0?$AA@ (0000)
00308BC0 11e8:
	_data_00308bc0 (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/profile.h"
#include "array.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

struct profile_section data_00308bc0[3] =
{
	{ "memory_dynamic_array_resize", NONE, TRUE },
	{ "memory_dynamic_array_add_element", NONE, TRUE },
	{ "memory_dynamic_array_delete_element", NONE, TRUE },
};

/* ---------- public code */

void dynamic_array_new(
	struct dynamic_array *array,
	long element_size)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x10,
		array);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x11,
		element_size>0);

	array->element_size = element_size;
	array->count = 0;
	array->elements = NULL;

	return;
}

boolean dynamic_array_resize(
	struct dynamic_array *array,
	long count)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x21,
		array);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x22,
		array->element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x23,
		array->count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x24,
		(array->count!=0)==(array->elements!=NULL));

	profile_enter(data_00308bc0[0]);
	if (count>=0 && count<=LONG_MAX)
	{
		if (count == array->count)
		{
			result = TRUE;
		}
		else
		{
			void *elements = debug_realloc(
				array->elements,
				count * array->element_size,
				"c:\\halo\\SOURCE\\memory\\array.c",
				0x2C);
			if ((count!=0)==(elements!=NULL))
			{
				if (count > array->count)
				{
					csmemset(
						(char *)elements + array->count * array->element_size,
						0,
						(count - array->count) * array->element_size);
				}
				array->count = count;
				array->elements = elements;
				result = TRUE;
			}
		}
	}
	profile_exit(data_00308bc0[0]);

	return result;
}

void dynamic_array_delete(
	struct dynamic_array *array)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x49,
		array);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x4A,
		array->count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x4B,
		(array->count!=0)==(array->elements!=NULL));

	array->element_size = NONE;
	array->count = NONE;
	if (array->elements)
	{
		array->elements = debug_realloc(
			array->elements,
			0,
			"c:\\halo\\SOURCE\\memory\\array.c",
			0x50);
	}

	return;
}

long dynamic_array_add_element(
	struct dynamic_array *array)
{
	long index = NONE;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x5D,
		array);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x5E,
		array->element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x5F,
		array->count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x60,
		(array->count!=0)==(array->elements!=NULL));

	profile_enter(data_00308bc0[1]);
	if (array->count < LONG_MAX)
	{
		long new_count = array->count + 1;
		void *elements = debug_realloc(
			array->elements,
			new_count * array->element_size,
			"c:\\halo\\SOURCE\\memory\\array.c",
			0x67);
		if (elements)
		{
			index = array->count;
			csmemset(
				(char *)elements + index * array->element_size,
				0,
				array->element_size);
			array->count = new_count;
			array->elements = elements;
		}
	}
	profile_exit(data_00308bc0[1]);

	return index;
}

void *dynamic_array_get_element(
	struct dynamic_array *array,
	long index,
	long element_size)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x7D,
		array);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x7E,
		array->element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x7F,
		array->element_size==element_size);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x80,
		array->count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x81,
		(array->count!=0)==(array->elements!=NULL));
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0x82,
		index>=0 && index<array->count);

	return (char *)array->elements + index * array->element_size;
}

void dynamic_array_delete_element(
	struct dynamic_array *array,
	long index)
{
	char *element;
	void *elements;

	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 139, array);
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 140, array->element_size>0);
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 141, array->count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		142,
		(array->count!=0)==(array->elements!=NULL));
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 143, index>=0 && index<array->count);

	profile_enter(data_00308bc0[2]);
	array->count--;
	if (index<array->count)
	{
		element = (char *)array->elements+index*array->element_size;
		csmemmove(
			element,
			element+array->element_size,
			(array->count-index)*array->element_size);
	}
	elements = debug_realloc(
		array->elements,
		array->count*array->element_size,
		"c:\\halo\\SOURCE\\memory\\array.c",
		156);
	array->elements = elements;
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		158,
		(array->count!=0)==(array->elements!=NULL));
	profile_exit(data_00308bc0[2]);

	return;
}

void static_array_new(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xAB,
		count);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xAC,
		elements);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xAD,
		element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xAE,
		maximum_count<=UNSIGNED_CHAR_MAX);

	*count = 0;
	csmemset(elements, NONE, element_size * maximum_count);

	return;
}

boolean static_array_resize(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count,
	short new_count)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xBF,
		count && *count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xC0,
		elements);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xC1,
		element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xC2,
		maximum_count<=UNSIGNED_CHAR_MAX);

	if (new_count>=0 && new_count<maximum_count)
	{
		if (*count != new_count)
		{
			char *old_end = (char *)elements + *count * element_size;
			char *new_end = (char *)elements + new_count * element_size;
			if (new_end > old_end)
				csmemset(old_end, 0, new_end - old_end);
			else
				csmemset(new_end, NONE, old_end - new_end);
			*count = (unsigned char)new_count;
		}

		result = TRUE;
	}

	return result;
}

short static_array_add_element(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count)
{
	short index = NONE;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xE6,
		count && *count>=0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xE7,
		elements);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xE8,
		element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xE9,
		maximum_count<=UNSIGNED_CHAR_MAX);

	if (*count < maximum_count)
	{
		index = *count;
		(*count)++;
		csmemset((char *)elements + index * element_size, 0, element_size);
	}

	return index;
}

void *static_array_get_element(
	unsigned char count,
	void *elements,
	short element_size,
	short index)
{
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xFB,
		count>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xFC,
		elements);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xFD,
		element_size>0);
	match_assert(
		"c:\\halo\\SOURCE\\memory\\array.c",
		0xFE,
		index>=0 && index<count);

	return (char *)elements + index * element_size;
}

void static_array_delete_element(
	unsigned char *count,
	void *elements,
	short element_size,
	short index)
{
	char *element;

	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 265, count && *count>0);
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 266, elements);
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 267, element_size>0);
	match_assert("c:\\halo\\SOURCE\\memory\\array.c", 268, index>=0 && index<*count);

	(*count)--;
	if (index<*count)
	{
		element = (char *)elements+index*element_size;
		csmemmove(
			element,
			element+element_size,
			(*count-index)*element_size);
	}
	csmemset((char *)elements+*count*element_size, NONE, element_size);

	return;
}


/* ---------- private code */
