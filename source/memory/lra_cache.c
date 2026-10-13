/*
LRA_CACHE.C

symbols in this file:
0010B9D0 0020:
	_lra_full (0000)
0010B9F0 0010:
	_lra_default_update_proc (0000)
0010BA00 0010:
	_lra_default_delete_proc (0000)
0010BA10 0020:
	_lra_block_delete (0000)
0010BA30 0080:
	_verify_lra_cache_block (0000)
0010BAB0 0080:
	_verify_lra_cache (0000)
0010BB30 0120:
	_lra_new (0000)
0010BC50 0050:
	_lra_dispose (0000)
0010BCA0 0050:
	_lra_flush (0000)
0010BCF0 0060:
	_lra_free (0000)
0010BD50 0050:
	_lra_lock (0000)
0010BDA0 0050:
	_lra_unlock (0000)
0010BDF0 0020:
	_lra_block_offset (0000)
0010BE10 0190:
	_lra_allocate (0000)
0027DC64 0031:
	??_C@_0DB@HDJOAIKJ@lra?5cache?5?$CFs?5?$EA?$CFp?5block?5?$EA?$CFp?5appea@ (0000)
0027DC98 0022:
	??_C@_0CC@HHIPALBL@c?3?2halo?2SOURCE?2memory?2lra_cache?4@ (0000)
0027DCBC 0027:
	??_C@_0CH@KNCLMCKO@lra?5cache?5?$CFs?5?$EA?$CFp?5appears?5to?5be?5c@ (0000)
0027DCE4 0006:
	??_C@_05BDNCCAMC@cache?$AA@ (0000)
0027DCEC 0018:
	??_C@_0BI@IEDIFNJK@?$CB?$CI?$CIlong?$CJbase_address?$CG3?$CJ?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "memory/lra_cache.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void lra_default_update_proc(
	long *address,
	long new_address);
static void lra_default_delete_proc(
	long *address);
static void lra_block_delete(
	struct lra_block *block,
	struct lra_cache *cache);
static void verify_lra_cache_block(
	struct lra_block *block,
	struct lra_cache *cache);
static void verify_lra_cache(
	struct lra_cache *cache);
static long lra_block_offset(
	struct lra_cache *cache,
	struct lra_block *block);

/* ---------- globals */

/* ---------- public code */

long lra_full(
	struct lra_cache *cache)
{
	if (cache->last_block && cache->last_block->next)
	{
		return TRUE;
	}

	return FALSE;
}

struct lra_cache *lra_new(
	char const *name,
	long size,
	lra_update_proc update_proc,
	lra_delete_proc delete_proc,
	void *base_address)
{
	struct lra_cache *cache = match_malloc("c:\\halo\\SOURCE\\memory\\lra_cache.c", 86, sizeof(struct lra_cache));

	match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 88, size>=0);

	if (!update_proc || !delete_proc)
	{
		update_proc = lra_default_update_proc;
		delete_proc = lra_default_delete_proc;
	}

	if (cache)
	{
		boolean malloced = FALSE;

		if (!base_address)
		{
			base_address = match_malloc("c:\\halo\\SOURCE\\memory\\lra_cache.c", 102, size);
			malloced = TRUE;
		}

		if (base_address)
		{
			match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 107, !((long)base_address&3));

			csmemset(cache, 0, sizeof(struct lra_cache));
			csstrncpy(cache->name, name, MAXIMUM_LRA_CACHE_NAME_LENGTH);
			cache->name[MAXIMUM_LRA_CACHE_NAME_LENGTH] = 0;
			cache->size = size;
			cache->base_address = base_address;
			cache->last_block = NULL;
			cache->signature = LRA_CACHE_SIGNATURE;
			cache->malloced = malloced;
			cache->delete_proc = delete_proc;
			cache->update_proc = update_proc;

			verify_lra_cache(cache);
		}
		else
		{
			match_free("c:\\halo\\SOURCE\\memory\\lra_cache.c", 126, cache);

			return NULL;
		}
	}

	return cache;
}

void lra_dispose(
	struct lra_cache *cache)
{
	verify_lra_cache(cache);

	if (cache->malloced)
	{
		match_free("c:\\halo\\SOURCE\\memory\\lra_cache.c", 140, cache->base_address);
	}

	match_free("c:\\halo\\SOURCE\\memory\\lra_cache.c", 141, cache);

	return;
}

void lra_flush(
	struct lra_cache *cache)
{
	verify_lra_cache(cache);

	if (cache->last_block && cache->base_address)
	{
		struct lra_block *block;

		for (block = (struct lra_block *)cache->base_address; block; block = block->next)
		{
			lra_block_delete(block, cache);
		}
	}

	cache->last_block = NULL;

	return;
}

void lra_free(
	struct lra_cache *cache,
	void *pointer)
{
	struct lra_block *block = (struct lra_block *)((char *)pointer - sizeof(struct lra_block));

	match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 282, pointer);

	verify_lra_cache(cache);
	verify_lra_cache_block(block, cache);
	lra_block_delete(block, cache);

	return;
}

void lra_lock(
	struct lra_cache *cache,
	void *pointer)
{
	struct lra_block *block = (struct lra_block *)((char *)pointer - sizeof(struct lra_block));

	match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 298, pointer);

	verify_lra_cache(cache);
	verify_lra_cache_block(block, cache);

	block->signature |= FLAG(_lra_block_locked_bit);

	return;
}

void lra_unlock(
	struct lra_cache *cache,
	void *pointer)
{
	struct lra_block *block = (struct lra_block *)((char *)pointer - sizeof(struct lra_block));

	match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 314, pointer);

	verify_lra_cache(cache);
	verify_lra_cache_block(block, cache);

	block->signature &= ~FLAG(_lra_block_locked_bit);

	return;
}

void *lra_allocate(
	struct lra_cache *cache,
	long size,
	long *address)
{
	void *result = NULL;

	verify_lra_cache(cache);

	size += sizeof(struct lra_block);
	size = (size&3) ? (size|3)+1 : size;

	if (size>=0 && size<=cache->size)
	{
		struct lra_block *last_block = cache->last_block;
		struct lra_block *next_block = last_block ? last_block->next : NULL;
		struct lra_block *first_deleted_block = NULL;
		short number_of_passes = 0;

		do
		{
			long write_offset = last_block ? lra_block_offset(cache, last_block)+last_block->size : 0;

			if (next_block)
			{
				verify_lra_cache_block(next_block, cache);

				if (write_offset+size>lra_block_offset(cache, next_block))
				{
					if (TEST_FLAG(next_block->signature, _lra_block_locked_bit))
					{
						last_block = next_block;
						next_block = next_block->next;
						first_deleted_block = NULL;
					}
					else
					{
						if (!first_deleted_block)
						{
							first_deleted_block = next_block;
						}
						next_block = next_block->next;
					}

					continue;
				}
			}

			if (write_offset+size>cache->size)
			{
				next_block = (struct lra_block *)cache->base_address;
				last_block = NULL;
				first_deleted_block = NULL;

				if (number_of_passes++)
				{
					break;
				}
			}
			else
			{
				struct lra_block *block;

				for (block = first_deleted_block; block && block!=next_block; block = block->next)
				{
					lra_block_delete(block, cache);
				}

				block = (struct lra_block *)((char *)cache->base_address + write_offset);
				block->size = size;
				block->signature = LRA_BLOCK_SIGNATURE;
				block->address = address;
				block->next = next_block;

				result = (char *)block + sizeof(struct lra_block);
				cache->update_proc(address, (long)result);

				if (last_block)
				{
					last_block->next = block;
				}
				cache->last_block = block;
			}
		}
		while (!result);
	}

	return result;
}

/* ---------- private code */

static void lra_default_update_proc(
	long *address,
	long new_address)
{
	*address = new_address;

	return;
}

static void lra_default_delete_proc(
	long *address)
{
	*address = 0;

	return;
}

static void lra_block_delete(
	struct lra_block *block,
	struct lra_cache *cache)
{
	if (!TEST_FLAG(block->signature, _lra_block_deleted_bit))
	{
		cache->delete_proc(block->address);
		block->signature = (block->signature&~FLAG(_lra_block_locked_bit))|FLAG(_lra_block_deleted_bit);
	}

	return;
}

static void verify_lra_cache_block(
	struct lra_block *block,
	struct lra_cache *cache)
{
	long block_offset;

	match_vassert(
		"c:\\halo\\SOURCE\\memory\\lra_cache.c",
		398,
		(block->signature&~(FLAG(_lra_block_locked_bit)|FLAG(_lra_block_deleted_bit)))==LRA_BLOCK_SIGNATURE &&
			block->size>=0 && block->size<cache->size &&
			(block_offset=(char *)block-(char *)cache->base_address)>=0 &&
			block->size+block_offset<=cache->size &&
			(block_offset=(block->next ? (char *)block->next-(char *)cache->base_address : 0))>=0 &&
			(unsigned long)(block_offset+sizeof(struct lra_block))<=(unsigned long)cache->size,
		csprintf(temporary, "lra cache %s @%p block @%p appears to be corrupt", cache->name, cache, block));

	return;
}

static void verify_lra_cache(
	struct lra_cache *cache)
{
	struct lra_block *last_block;

	match_assert("c:\\halo\\SOURCE\\memory\\lra_cache.c", 408, cache);

	match_vassert(
		"c:\\halo\\SOURCE\\memory\\lra_cache.c",
		418,
		cache->signature==LRA_CACHE_SIGNATURE && cache->base_address && cache->size>=0,
		csprintf(temporary, "lra cache %s @%p appears to be corrupt", cache->name, cache));

	last_block = cache->last_block;
	if (last_block)
	{
		verify_lra_cache_block(last_block, cache);
	}

	return;
}

static long lra_block_offset(
	struct lra_cache *cache,
	struct lra_block *block)
{
	verify_lra_cache_block(block, cache);

	return (long)((char *)block - (char *)cache->base_address);
}
