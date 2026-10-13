/*
HASHTABLE.H

header included in hcex build.
*/

#ifndef __HASHTABLE_H
#define __HASHTABLE_H
#pragma once

#include "math/real_math.h"
#include "memory/array.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct hashtable;

typedef short (*hashtable_hash_function)(
	void *user_data,
	const void *key);

typedef boolean (*hashtable_compare_function)(
	void *user_data,
	const void *element,
	const void *key);

struct hashtable
{
	short key_size;
	short element_size;
	short count;
	short capacity_bits;
	real load_factor;
	void *user_data;
	hashtable_hash_function hash_function;
	hashtable_compare_function compare_function;
	unsigned long *used_slots;
	struct dynamic_array elements;
};

/* ---------- prototypes/EXAMPLE.C */

void hashtable_new(
	struct hashtable *table,
	short key_size,
	short element_size,
	real load_factor,
	hashtable_hash_function hash_function,
	hashtable_compare_function compare_function);

void hashtable_set_user_data(
	struct hashtable *table,
	void *user_data);

void hashtable_delete(
	struct hashtable *table);

void *hashtable_get(
	struct hashtable *table,
	const void *key);

void hashtable_remove(
	struct hashtable *table,
	const void *key);

boolean hashtable_grow(
	struct hashtable *table,
	short growth_bits);

void *hashtable_put(
	struct hashtable *table,
	const void *key);

/* ---------- globals */

/* ---------- public code */

#endif // __HASHTABLE_H
