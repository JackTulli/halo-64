/*
ARRAY.H

header included in hcex build.
*/

#ifndef __ARRAY_H
#define __ARRAY_H
#pragma once

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct dynamic_array
{
	long element_size;
	long count;
	void *elements;
};

/* ---------- prototypes/EXAMPLE.C */

void dynamic_array_new(
	struct dynamic_array *array,
	long element_size);
boolean dynamic_array_resize(
	struct dynamic_array *array,
	long count);
void dynamic_array_delete(
	struct dynamic_array *array);
long dynamic_array_add_element(
	struct dynamic_array *array);
void *dynamic_array_get_element(
	struct dynamic_array *array,
	long index,
	long element_size);
void dynamic_array_delete_element(
	struct dynamic_array *array,
	long index);
void static_array_new(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count);
boolean static_array_resize(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count,
	short new_count);
short static_array_add_element(
	unsigned char *count,
	void *elements,
	short element_size,
	short maximum_count);
void *static_array_get_element(
	unsigned char count,
	void *elements,
	short element_size,
	short index);
void static_array_delete_element(
	unsigned char *count,
	void *elements,
	short element_size,
	short index);

/* ---------- globals */

/* ---------- public code */

#endif // __ARRAY_H
