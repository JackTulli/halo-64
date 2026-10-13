/*
LRA_CACHE.H

header included in hcex build.
*/

#ifndef __LRA_CACHE_H
#define __LRA_CACHE_H
#pragma once

/* ---------- constants */

enum
{
	LRA_CACHE_SIGNATURE = 'lrac',
	LRA_BLOCK_SIGNATURE = 'Ablh',

	MAXIMUM_LRA_CACHE_NAME_LENGTH = 31,
};

enum
{
	_lra_block_locked_bit,
	_lra_block_deleted_bit,
};

/* ---------- macros */

/* ---------- types */

typedef void (*lra_update_proc)(
	long *address,
	long new_address);

typedef void (*lra_delete_proc)(
	long *address);

/* ---------- structures */

struct lra_block
{
	long *address;
	unsigned long signature;
	long size;
	struct lra_block *next;
};

struct lra_cache
{
	char name[MAXIMUM_LRA_CACHE_NAME_LENGTH+1];
	long size;
	void *base_address;
	boolean malloced;
	struct lra_block *last_block;
	lra_update_proc update_proc;
	lra_delete_proc delete_proc;
	unsigned long signature;
};

/* ---------- prototypes/LRA_CACHE.C */

long lra_full(
	struct lra_cache *cache);
struct lra_cache *lra_new(
	char const *name,
	long size,
	lra_update_proc update_proc,
	lra_delete_proc delete_proc,
	void *base_address);
void lra_dispose(
	struct lra_cache *cache);
void lra_flush(
	struct lra_cache *cache);
void lra_free(
	struct lra_cache *cache,
	void *pointer);
void lra_lock(
	struct lra_cache *cache,
	void *pointer);
void lra_unlock(
	struct lra_cache *cache,
	void *pointer);
void *lra_allocate(
	struct lra_cache *cache,
	long size,
	long *address);

/* ---------- globals */

/* ---------- public code */

#endif // __LRA_CACHE_H
