/*
LRU_CACHE.H

header included in hcex build.
*/

#ifndef __LRU_CACHE_H
#define __LRU_CACHE_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct lru_cache;

typedef void (*lru_block_new_proc)(
	void *reference,
	void *block);
typedef void (*lru_block_delete_proc)(
	void *reference);

/* ---------- prototypes/EXAMPLE.C */

struct lru_cache *lru_new(
	char const *name,
	long total_size,
	long block_size,
	lru_block_new_proc block_new_proc,
	lru_block_delete_proc block_delete_proc,
	void *blocks);

void lru_dispose(
	struct lru_cache *cache);
void lru_flush(
	struct lru_cache *cache);
long lru_free_blocks(
	struct lru_cache *cache);
void *lru_allocate(
	struct lru_cache *cache,
	void *user_data);
void lru_lock(
	struct lru_cache *cache,
	void *block);
void lru_unlock(
	struct lru_cache *cache,
	void *block);
void lru_touch(
	struct lru_cache *cache,
	void *block);

/* ---------- globals */

/* ---------- public code */

#endif // __LRU_CACHE_H
