/*
LRAR_CACHE.H

header included in hcex build.
*/

#ifndef __LRAR_CACHE_H
#define __LRAR_CACHE_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct lrar_cache;

typedef void (*lrar_lock_proc)(
	short *reference,
	short value);
typedef void (*lrar_unlock_proc)(
	void *reference);

/* ---------- prototypes/LRAR_CACHE.C */

struct lrar_cache *lrar_new(
	char const *name,
	unsigned long minimum_address,
	unsigned long maximum_address,
	short block_count,
	short alignment_bit,
	short boundary_bit,
	lrar_lock_proc lock_proc,
	lrar_unlock_proc unlock_proc);
void lrar_dispose(
	struct lrar_cache *cache);
void lrar_flush(
	struct lrar_cache *cache);
short lrar_allocate(
	struct lrar_cache *cache,
	long size,
	void *user_data);
unsigned long lrar_block_address(
	struct lrar_cache *cache,
	short block_index);
void lrar_deallocate(
	struct lrar_cache *cache,
	short block_index);

/* ---------- globals */

/* ---------- public code */

#endif // __LRAR_CACHE_H
