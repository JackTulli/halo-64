/*
LRUV_CACHE.H

header included in hcex build.
*/

#ifndef __LRUV_CACHE_H
#define __LRUV_CACHE_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct data_array;

typedef void (*lruv_delete_block_proc)(
	long block_index);

typedef boolean (*lruv_locked_block_proc)(
	long block_index);

typedef void (*lruv_debug_header_proc)(
	FILE *stream);

typedef const char *(*lruv_debug_block_name_proc)(
	long block_index);

struct lruv_cache
{
	char name[32];
	lruv_delete_block_proc delete_block_proc;
	lruv_locked_block_proc locked_block_proc;
	long page_count;
	long page_size_bits;
	long tick;
	long first_block_index;
	long last_block_index;
	struct data_array *blocks;
	unsigned long signature;
};

struct lruv_cache_block
{
	short identifier;
	short flags;
	long page_count;
	long first_page_index;
	long next_block_index;
	long previous_block_index;
	long last_used_tick;
	long user_data;
};

typedef char lruv_cache_size_assert[
	sizeof(struct lruv_cache) == 0x44 ? 1 : -1];
typedef char lruv_cache_block_size_assert[
	sizeof(struct lruv_cache_block) == 0x1C ? 1 : -1];

/* ---------- prototypes/LRUV_CACHE.C */

long lruv_allocation_size(long maximum_block_count);
void lruv_initialize(
	struct lruv_cache *cache,
	const char *name,
	long page_count,
	long page_size_bits,
	long maximum_block_count,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc);

void lruv_update_function_pointers(
	struct lruv_cache *cache,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc);

boolean lruv_has_locked_proc(
	struct lruv_cache *cache);

struct lruv_cache *lruv_new(
	const char *name,
	long page_count,
	long page_size_bits,
	long maximum_block_count,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc);

void lruv_delete(
	struct lruv_cache *cache);

void lruv_idle(
	struct lruv_cache *cache);

void lruv_block_delete(
	struct lruv_cache *cache,
	long block_index);

void lruv_block_touch(
	struct lruv_cache *cache,
	long block_index);

void *lruv_block_get_address(
	struct lruv_cache *cache,
	long block_index);

boolean lruv_block_touched(
	struct lruv_cache *cache,
	long block_index);

long lruv_block_new(
	struct lruv_cache *cache,
	long size);

void lruv_cache_get_page_usage(
	struct lruv_cache *cache,
	byte *page_usage);

void lruv_resize(
	struct lruv_cache *cache,
	long new_page_count);

void lruv_debug_to_file(
	const char *path,
	const char *allocation_name,
	long allocation_size,
	struct lruv_cache *cache,
	lruv_debug_header_proc header_proc,
	lruv_debug_block_name_proc block_name_proc);

void lruv_flush(
	struct lruv_cache *cache);

/* ---------- globals */

/* ---------- public code */

#endif // __LRUV_CACHE_H
