/*
LRUV_CACHE.C

symbols in this file:
0010CCA0 0020:
	_lruv_allocation_size (0000)
0010CCC0 0050:
	_lruv_update_function_pointers (0000)
0010CD10 0040:
	_lruv_has_locked_proc (0000)
0010CD50 0020:
	_lruv_cache_bytes_to_pages (0000)
0010CD70 0230:
	_lruv_cache_verify (0000)
0010CFA0 0110:
	_lruv_initialize (0000)
0010D0B0 0040:
	_lruv_delete (0000)
0010D0F0 0020:
	_lruv_idle (0000)
0010D110 00e0:
	_lruv_block_delete (0000)
0010D1F0 0030:
	_lruv_block_touch (0000)
0010D220 0030:
	_lruv_block_get_address (0000)
0010D250 0030:
	_lruv_block_touched (0000)
0010D280 00a0:
	_lruv_cache_get_page_usage (0000)
0010D320 0090:
	_lruv_resize (0000)
0010D3B0 01d0:
	_lruv_debug_to_file (0000)
0010D580 0060:
	_lruv_new (0000)
0010D5E0 0050:
	_lruv_flush (0000)
0010D630 0520:
	_lruv_block_new (0000)
0027DFC0 0023:
	??_C@_0CD@NCBKDBJP@c?3?2halo?2SOURCE?2memory?2lruv_cache@ (0000)
0027DFE8 0048:
	??_C@_0EI@HEIFIJJL@block?9?$DOfirst_page_index?$CLblock?9?$DOp@ (0000)
0027E030 0035:
	??_C@_0DF@MDNEJLAH@next_block?9?$DOfirst_page_index?$DOblo@ (0000)
0027E068 002e:
	??_C@_0CO@LEEGCGLF@next_block?9?$DOprevious_block_index@ (0000)
0027E098 0025:
	??_C@_0CF@NBCPOKGO@cache?9?$DOlast_block_index?$DN?$DNblock_i@ (0000)
0027E0C0 0055:
	??_C@_0FF@HOIJOPMD@previous_block?9?$DOfirst_page_index@ (0000)
0027E118 0039:
	??_C@_0DJ@EDMBFAJL@previous_block?9?$DOfirst_page_index@ (0000)
0027E154 002e:
	??_C@_0CO@DBDNOGDK@previous_block?9?$DOnext_block_index@ (0000)
0027E184 0026:
	??_C@_0CG@BJMLDEPL@cache?9?$DOfirst_block_index?$DN?$DNblock_@ (0000)
0027E1AC 0027:
	??_C@_0CH@NAJJPKBF@cache?9?$DOsignature?$DN?$DNLRUV_CACHE_SIG@ (0000)
0027E1D4 0016:
	??_C@_0BG@DHPHNPNL@maximum_block_count?$DO0?$AA@ (0000)
0027E1EC 002e:
	??_C@_0CO@MICADOHD@page_size_bits?$DO0?5?$CG?$CG?5page_size_bi@ (0000)
0027E21C 000d:
	??_C@_0N@FBNLCABO@page_count?$DO0?$AA@ (0000)
0027E22C 0011:
	??_C@_0BB@KFLMCPF@new_page_count?$DO0?$AA@ (0000)
0027E240 0010:
	??_C@_0BA@DBGNJGAO@?$CFs?5?$CF?55d?$CF?55d?5?$CFs?6?$AA@ (0000)
0027E250 0002:
	??_C@_01OCOKONAJ@L?$AA@ (0000)
0027E258 006f:
	??_C@_0GP@KEGBJJID@?6?$CD?$CFd?5pages?0?5each?5?$CD?$CFd?5bytes?6?$CD?$CFd?5b@ (0000)
0027E2C8 0031:
	??_C@_0DB@MNKBOABM@?$CFs?5?$CIv1?3?5only?5blocks?5used?5this?5fr@ (0000)
0027E2FC 0003:
	??_C@_02CHHAKFFB@w?$CL?$AA@ (0000)
0027E300 0027:
	??_C@_0CH@CKDODDDO@next_block?9?$DOprevious_block_index@ (0000)
0027E328 001e:
	??_C@_0BO@JHCOBGKL@cache?9?$DOlast_block_index?$DN?$DNNONE?$AA@ (0000)
0027E348 0054:
	??_C@_0FE@PMNMONNB@?$CBcache?9?$DOlocked_block_proc?5?$HM?$HM?5?$CBca@ (0000)
0027E39C 0039:
	??_C@_0DJ@OEGLJEMH@lruv_cache_block_get?$CIcache?0?5olde@ (0000)
0027E3D8 0047:
	??_C@_0EH@JOOPPGKG@?$CBcache?9?$DOlocked_block_proc?5?$HM?$HM?5?$CBca@ (0000)
0027E420 001c:
	??_C@_0BM@BFNMIBPB@hole_read_index?$DN?$DNhole_index?$AA@ (0000)
0027E43C 0015:
	??_C@_0BF@OEOIEJGL@desired_page_count?$DO0?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "memory/data.h"
#include "memory/lruv_cache.h"

/* ---------- constants */

enum
{
	LRUV_CACHE_SIGNATURE = 'weee',
	MAXIMUM_LRUV_CACHE_HOLES = 256
};

/* ---------- macros */

#define NEXT_HOLE_INDEX(index) \
	((index)==MAXIMUM_LRUV_CACHE_HOLES-1 ? 0 : (index)+1)

#define lruv_cache_block_get(cache, block_index) \
	((struct lruv_cache_block *)datum_get((cache)->blocks, (block_index)))

/* ---------- structures */

struct lruv_cache_hole
{
	long block_index;
	unsigned long last_used_tick;
	long first_page_index;
	long page_count;
};

/* ---------- prototypes */

void lruv_cache_verify(
	struct lruv_cache *cache,
	boolean verify_blocks);

static long lruv_cache_bytes_to_pages(
	struct lruv_cache *cache,
	long size);

/* ---------- globals */

/* ---------- public code */

long lruv_allocation_size(
	long maximum_block_count)
{
	return sizeof(struct lruv_cache) + data_allocation_size(
		(short)maximum_block_count,
		sizeof(struct lruv_cache_block));
}

void lruv_update_function_pointers(
	struct lruv_cache *cache,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc)
{
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 148, cache);

	cache->delete_block_proc = delete_block_proc;
	cache->locked_block_proc = locked_block_proc;

	return;
}

boolean lruv_has_locked_proc(
	struct lruv_cache *cache)
{
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 631, cache);

	return cache->locked_block_proc != NULL;
}

void lruv_initialize(
	struct lruv_cache *cache,
	const char *name,
	long page_count,
	long page_size_bits,
	long maximum_block_count,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc)
{
	struct data_array *blocks = (struct data_array *)(cache + 1);

	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 109, name);
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 110, page_count>0);
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 111, page_size_bits>0 && page_size_bits<SHORT_BITS);
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 112, maximum_block_count>0);

	data_initialize(
		blocks,
		name,
		(short)maximum_block_count,
		sizeof(struct lruv_cache_block));
	data_make_valid(blocks);
	csmemset(cache, 0, sizeof(*cache));
	csstrncpy(cache->name, name, NUMBEROF(cache->name) - 1);

	cache->delete_block_proc = delete_block_proc;
	cache->locked_block_proc = locked_block_proc;
	cache->page_count = page_count;
	cache->page_size_bits = page_size_bits;
	cache->blocks = blocks;
	cache->signature = LRUV_CACHE_SIGNATURE;
	cache->first_block_index = NONE;
	cache->last_block_index = NONE;
	cache->tick = 1;

	lruv_cache_verify(cache, TRUE);

	return;
}

void lruv_delete(
	struct lruv_cache *cache)
{
	lruv_cache_verify(cache, TRUE);
	data_dispose(cache->blocks);
	csmemset(cache, 0, sizeof(*cache));
	match_free("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 163, cache);

	return;
}

void lruv_idle(
	struct lruv_cache *cache)
{
	lruv_cache_verify(cache, FALSE);
	cache->tick++;

	return;
}

void lruv_block_delete(
	struct lruv_cache *cache,
	long block_index)
{
	struct lruv_cache_block *block;
	struct lruv_cache_block *next_block;
	struct lruv_cache_block *previous_block;

	block = datum_get(cache->blocks, block_index);
	lruv_cache_verify(cache, TRUE);

	if (cache->delete_block_proc)
		cache->delete_block_proc(block_index);

	if (block->previous_block_index != NONE)
	{
		previous_block = datum_get(cache->blocks, block->previous_block_index);
		previous_block->next_block_index = block->next_block_index;
	}
	else
	{
		match_assert(
			"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
			488,
			cache->first_block_index==block_index);
		cache->first_block_index = block->next_block_index;
	}

	if (block->next_block_index != NONE)
	{
		next_block = datum_get(cache->blocks, block->next_block_index);
		next_block->previous_block_index = block->previous_block_index;
	}
	else
	{
		match_assert(
			"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
			501,
			cache->last_block_index==block_index);
		cache->last_block_index = block->previous_block_index;
	}

	datum_delete(cache->blocks, block_index);
	lruv_cache_verify(cache, TRUE);

	return;
}

void lruv_block_touch(
	struct lruv_cache *cache,
	long block_index)
{
	struct lruv_cache_block *block;

	lruv_cache_verify(cache, FALSE);
	block = datum_get(cache->blocks, block_index);
	block->last_used_tick = cache->tick;

	return;
}

void *lruv_block_get_address(
	struct lruv_cache *cache,
	long block_index)
{
	struct lruv_cache_block *block;

	lruv_cache_verify(cache, FALSE);
	block = datum_get(cache->blocks, block_index);

	return (void *)(block->first_page_index << cache->page_size_bits);
}

boolean lruv_block_touched(
	struct lruv_cache *cache,
	long block_index)
{
	struct lruv_cache_block *block;

	lruv_cache_verify(cache, FALSE);
	block = datum_get(cache->blocks, block_index);

	return block->last_used_tick == cache->tick;
}

void lruv_cache_get_page_usage(
	struct lruv_cache *cache,
	byte *page_usage)
{
	struct data_iterator iterator;
	struct lruv_cache_block *block;
	byte usage;

	lruv_cache_verify(cache, TRUE);
	csmemset(page_usage, 0, cache->page_count);

	data_iterator_new(&iterator, cache->blocks);
	while ((block = data_iterator_next(&iterator)) != NULL)
	{
		usage = 1;
		if (cache->locked_block_proc && cache->locked_block_proc(iterator.datum_index))
			usage |= 8;
		if (block->last_used_tick == cache->tick)
			usage |= 2;
		if ((unsigned long)(block->last_used_tick + 30) < (unsigned long)cache->tick)
			usage |= 4;

		csmemset(
			page_usage + block->first_page_index,
			usage,
			block->page_count);
	}

	return;
}

void lruv_resize(
	struct lruv_cache *cache,
	long new_page_count)
{
	struct data_iterator iterator;
	struct lruv_cache_block *block;

	match_assert(
		"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
		603,
		new_page_count>0);
	lruv_cache_verify(cache, TRUE);

	data_iterator_new(&iterator, cache->blocks);
	while ((block = data_iterator_next(&iterator)) != NULL)
	{
		if (block->first_page_index + block->page_count > new_page_count)
			lruv_block_delete(cache, iterator.datum_index);
	}

	cache->page_count = new_page_count;

	return;
}

void lruv_debug_to_file(
	const char *path,
	const char *allocation_name,
	long allocation_size,
	struct lruv_cache *cache,
	lruv_debug_header_proc header_proc,
	lruv_debug_block_name_proc block_name_proc)
{
	FILE *stream;
	struct lruv_cache_block *block;

#ifdef HALO_XBOX
	/* the console: a cache that failed to make room is logged, but not
	dumped block by block to a file on the game's thread mid-game */
	return;
#endif
	long page_size;
	long allocation_page_count;
	long page_index;
	long block_index;
	long page_count;
	unsigned long age;
	boolean locked;
	const char *block_name;

	lruv_cache_verify(cache, TRUE);
	stream = fopen(path, "w+");
	if (stream)
	{
		fprintf(
			stream,
			"%s (v1: only blocks used this frame are locked)\n",
			cache->name);
		header_proc(stream);

		page_size = 1 << cache->page_size_bits;
		allocation_page_count = lruv_cache_bytes_to_pages(cache, allocation_size);

		fprintf(
			stream,
			"\n#%d pages, each #%d bytes\n"
			"#%d blocks at frame index #%d\n"
			"failed allocation of \"%s\" was #%d bytes (#%d pages)\n\n",
			cache->page_count,
			page_size,
			cache->blocks->actual_count,
			cache->tick,
			allocation_name,
			allocation_size,
			allocation_page_count);

		block_index = cache->first_block_index;
		page_index = 0;
		while (page_index < cache->page_count)
		{
			age = 0;
			locked = FALSE;
			block_name = NULL;
			if (block_index == NONE)
			{
				page_count = cache->page_count - page_index;
				page_index = cache->page_count;
			}
			else
			{
				block = datum_get(cache->blocks, block_index);
				if (page_index == block->first_page_index)
				{
					age = cache->tick - block->last_used_tick;
					page_count = block->page_count;
					locked = cache->locked_block_proc &&
						cache->locked_block_proc(block_index);
					if ((unsigned long)(block->last_used_tick + 1) >=
						(unsigned long)cache->tick)
					{
						locked = TRUE;
					}

					page_index = block->first_page_index + block->page_count;
					block_name = block_name_proc(block_index);
					block_index = block->next_block_index;
				}
				else
				{
					page_count = block->first_page_index - page_index;
					match_assert(
						"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
						716,
						page_count>0);
					page_index = block->first_page_index;
				}
			}

			fprintf(
				stream,
				"%s % 5d% 5d %s\n",
				locked ? "L" : " ",
				page_count,
				MIN(9999, age),
				block_name ? block_name : "");
		}

		fprintf(stream, "\n");
		fclose(stream);
	}

	return;
}

struct lruv_cache *lruv_new(
	const char *name,
	long page_count,
	long page_size_bits,
	long maximum_block_count,
	lruv_delete_block_proc delete_block_proc,
	lruv_locked_block_proc locked_block_proc)
{
	struct lruv_cache *cache = match_malloc(
		"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
		82,
		lruv_allocation_size(maximum_block_count));

	if (cache)
	{
		lruv_initialize(
			cache,
			name,
			page_count,
			page_size_bits,
			maximum_block_count,
			delete_block_proc,
			locked_block_proc);
	}

	return cache;
}

void lruv_flush(
	struct lruv_cache *cache)
{
	struct data_iterator iterator;

	lruv_cache_verify(cache, TRUE);
	data_iterator_new(&iterator, cache->blocks);
	while (data_iterator_next(&iterator))
		lruv_block_delete(cache, iterator.datum_index);

	return;
}

long lruv_block_new(
	struct lruv_cache *cache,
	long size)
{
	struct lruv_cache_hole holes[MAXIMUM_LRUV_CACHE_HOLES];
	struct lruv_cache_hole best_hole;
	struct data_iterator iterator;
	struct lruv_cache_block *block;
	struct lruv_cache_block *next_block;
	struct lruv_cache_block *new_block;
	struct lruv_cache_hole *hole;
	short hole_index;
	short hole_read_index;
	short hole_write_index;
	long desired_page_count;
	long page_index;
	long block_index;
	long pending_block_index;
	long page_count;
	unsigned long last_used_tick;
	boolean locked;
	long oldest_unlocked_block_index;
	unsigned long oldest_unlocked_tick;
	boolean found_hole;
	long new_block_index;
#define index datum_index

	desired_page_count = lruv_cache_bytes_to_pages(cache, size);
	found_hole = FALSE;
	new_block_index = NONE;
	oldest_unlocked_block_index = NONE;
	match_assert(
		"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
		225,
		desired_page_count>0);

	hole_read_index = 0;
	hole_write_index = 0;
	pending_block_index = NONE;
	page_index = 0;
	block_index = cache->first_block_index;

	while (page_index < cache->page_count)
	{
		locked = FALSE;

		if (NEXT_HOLE_INDEX(hole_write_index) != hole_read_index)
		{
			hole = &holes[hole_write_index];
			hole->block_index = pending_block_index;
			hole->first_page_index = page_index;
			hole->last_used_tick = 0;
			hole->page_count = 0;
			hole_write_index = NEXT_HOLE_INDEX(hole_write_index);
		}

		if (block_index == NONE)
		{
			last_used_tick = 0;
			page_count = cache->page_count - page_index;
			page_index = cache->page_count;
		}
		else
		{
			block = datum_get(cache->blocks, block_index);
			if (page_index == block->first_page_index)
			{
				last_used_tick = block->last_used_tick;
				page_count = block->page_count;
				locked = cache->locked_block_proc &&
					cache->locked_block_proc(block_index);
				if ((unsigned long)block->last_used_tick == (unsigned long)cache->tick)
					locked = TRUE;
				else if (!locked &&
					(oldest_unlocked_block_index == NONE ||
					(unsigned long)block->last_used_tick < oldest_unlocked_tick))
				{
					oldest_unlocked_block_index = block_index;
					oldest_unlocked_tick = block->last_used_tick;
				}

				page_index = block->first_page_index + block->page_count;
				pending_block_index = block_index;
				block_index = block->next_block_index;
			}
			else
			{
				last_used_tick = 0;
				page_count = block->first_page_index - page_index;
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					311,
					page_count>0);
				page_index = block->first_page_index;
			}
		}

		if (locked)
			hole_read_index = hole_write_index;
		else
		{
			hole_index = hole_read_index;
			while (hole_index != hole_write_index)
			{
				hole = &holes[hole_index];
				if (last_used_tick > hole->last_used_tick)
					hole->last_used_tick = last_used_tick;
				hole->page_count += page_count;
				if (hole->page_count >= desired_page_count)
				{
					if (!found_hole ||
						hole->last_used_tick < best_hole.last_used_tick ||
						(hole->last_used_tick == best_hole.last_used_tick &&
						 hole->page_count < best_hole.page_count))
					{
						best_hole = *hole;
						found_hole = TRUE;
					}

					match_assert(
						"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
						351,
						hole_read_index==hole_index);
					hole_read_index = NEXT_HOLE_INDEX(hole_read_index);
				}

				hole_index = NEXT_HOLE_INDEX(hole_index);
			}
		}
	}

	if (found_hole)
	{
		data_iterator_new(&iterator, cache->blocks);
		while ((block = data_iterator_next(&iterator)) != NULL)
		{
			if (block->first_page_index < best_hole.first_page_index + desired_page_count &&
				block->first_page_index + block->page_count > best_hole.first_page_index)
			{
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					375,
					!cache->locked_block_proc || !cache->locked_block_proc(iterator.index));
				lruv_block_delete(cache, iterator.datum_index);
			}
		}

		if (cache->blocks->actual_count == cache->blocks->maximum_count &&
			oldest_unlocked_block_index != NONE)
		{
			if (best_hole.block_index == oldest_unlocked_block_index)
			{
				block = datum_get(cache->blocks, oldest_unlocked_block_index);
				best_hole.block_index = block->previous_block_index;
			}
			match_assert(
				"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
				392,
				lruv_cache_block_get(cache, oldest_unlocked_block_index));
			match_assert(
				"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
				393,
				!cache->locked_block_proc || !cache->locked_block_proc(oldest_unlocked_block_index));
			lruv_block_delete(cache, oldest_unlocked_block_index);
		}

		new_block_index = datum_new(cache->blocks);
		if (new_block_index != NONE)
		{
			new_block = datum_get(cache->blocks, new_block_index);
			if (best_hole.block_index == NONE)
			{
				if (cache->first_block_index == NONE)
				{
					match_assert(
						"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
						408,
						cache->last_block_index==NONE);
					new_block->previous_block_index = NONE;
					cache->last_block_index = new_block_index;
				}
				else
				{
					next_block = datum_get(cache->blocks, cache->first_block_index);
					match_assert(
						"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
						416,
						next_block->previous_block_index==NONE);
					new_block->previous_block_index = NONE;
					next_block->previous_block_index = new_block_index;
				}
			}
			else
			{
				block = datum_get(cache->blocks, best_hole.block_index);
				if (block->next_block_index == NONE)
				{
					new_block->previous_block_index = cache->last_block_index;
					cache->last_block_index = new_block_index;
				}
				else
				{
					next_block = datum_get(cache->blocks, block->next_block_index);
					new_block->previous_block_index = next_block->previous_block_index;
					next_block->previous_block_index = new_block_index;
				}
			}

			if (best_hole.block_index == NONE)
			{
				new_block->next_block_index = cache->first_block_index;
				cache->first_block_index = new_block_index;
			}
			else
			{
				block = datum_get(cache->blocks, best_hole.block_index);
				new_block->next_block_index = block->next_block_index;
				block->next_block_index = new_block_index;
			}

			new_block->first_page_index = best_hole.first_page_index;
			new_block->page_count = desired_page_count;
			new_block->last_used_tick = cache->tick;
			lruv_cache_verify(cache, TRUE);
		}
	}

	return new_block_index;
#undef index
}

/* ---------- private code */

static long lruv_cache_bytes_to_pages(
	struct lruv_cache *cache,
	long size)
{
	long page_count;
	long page_size;

	page_size = 1 << cache->page_size_bits;
	page_count = size >> cache->page_size_bits;
	if (size & (page_size - 1))
		page_count++;

	return page_count;
}

void lruv_cache_verify(
	struct lruv_cache *cache,
	boolean verify_blocks)
{
	long block_index;
	struct lruv_cache_block *block;
	struct lruv_cache_block *next_block;
	struct lruv_cache_block *previous_block;

	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 754, cache);
	match_assert("c:\\halo\\SOURCE\\memory\\lruv_cache.c", 755, cache->signature==LRUV_CACHE_SIGNATURE);
	data_verify(cache->blocks);
#ifdef HALO_XBOX
	/* the console: not the walk of every block on every allocation and
	free (the texture and sound caches'), which only checks */
	verify_blocks = FALSE;
#endif

	if (verify_blocks)
	{
		block_index = cache->first_block_index;
		while (block_index != NONE)
		{
			block = datum_get(cache->blocks, block_index);
			if (block->previous_block_index == NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					770,
					cache->first_block_index==block_index);
			}
			else
			{
				previous_block = datum_get(cache->blocks, block->previous_block_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					776,
					previous_block->next_block_index==block_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					777,
					previous_block->first_page_index<block->first_page_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					778,
					previous_block->first_page_index+previous_block->page_count<=block->first_page_index);
			}

			if (block->next_block_index == NONE)
			{
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					784,
					cache->last_block_index==block_index);
			}
			else
			{
				next_block = datum_get(cache->blocks, block->next_block_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					790,
					next_block->previous_block_index==block_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					791,
					next_block->first_page_index>block->first_page_index);
				match_assert(
					"c:\\halo\\SOURCE\\memory\\lruv_cache.c",
					792,
					block->first_page_index+block->page_count<=next_block->first_page_index);
			}

			block_index = block->next_block_index;
		}
	}

	return;
}
