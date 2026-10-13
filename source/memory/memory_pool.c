/*
MEMORY_POOL.C

symbols in this file:
0010DB50 0010:
	_memory_pool_allocation_size (0000)
0010DB60 0050:
	_memory_pool_initialize (0000)
0010DBB0 0010:
	_memory_pool_get_free_size (0000)
0010DBC0 0020:
	_memory_pool_get_used_size (0000)
0010DBE0 0030:
	_memory_pool_get_contiguous_free_size (0000)
0010DC10 0010:
	_code_0010dc10 (0000)
0010DC20 0030:
	_code_0010dc20 (0000)
0010DC50 0170:
	_code_0010dc50 (0000)
0010DDC0 00b0:
	_code_0010ddc0 (0000)
0010DE70 0040:
	_memory_pool_new (0000)
0010DEB0 0030:
	_memory_pool_delete (0000)
0010DEE0 00e0:
	_memory_pool_block_allocate (0000)
0010DFC0 00a0:
	_memory_pool_block_free (0000)
0010E060 0060:
	_memory_pool_compact (0000)
0010E0C0 0120:
	_memory_pool_block_reallocate (0000)
0027E458 0041:
	??_C@_0EB@NADHALE@?$CIbyte?5?$CK?$CJblock?$CLblock?9?$DOsize?$DM?$DN?$CIbyte@ (0000)
0027E49C 002a:
	??_C@_0CK@LLMKELFI@?$CIbyte?5?$CK?$CJblock?$DO?$DN?$CIbyte?5?$CK?$CJpool?9?$DObas@ (0000)
0027E4C8 0032:
	??_C@_0DC@HIOFDAGC@block?9?$DOtrailer_signature?$DN?$DNBLOCK_@ (0000)
0027E4FC 0030:
	??_C@_0DA@IHOELNEF@block?9?$DOheader_signature?$DN?$DNBLOCK_H@ (0000)
0027E52C 002d:
	??_C@_0CN@BLDKBJCH@block?9?$DOnext_block?5?$HM?$HM?5pool?9?$DOlast_@ (0000)
0027E55C 0026:
	??_C@_0CG@CJPANJAP@block?9?$DOprevious_block?$DN?$DNprevious_@ (0000)
0027E584 000d:
	??_C@_0N@EHOFABCJ@pool?9?$DOsize?$DO0?$AA@ (0000)
0027E594 0020:
	??_C@_0CA@HGIACAOK@pool?9?$DOsignature?$DN?$DNPOOL_SIGNATURE?$AA@ (0000)
0027E5B4 0024:
	??_C@_0CE@POLAKNJH@c?3?2halo?2SOURCE?2memory?2memory_poo@ (0000)
0027E5D8 000c:
	??_C@_0M@MJBIHABJ@other_block?$AA@ (0000)
0027E5E4 0025:
	??_C@_0CF@IJNEMAOG@expected?5reference?5?$CF08x?5but?5got?5@ (0000)
0027E60C 001a:
	??_C@_0BK@MFLHGBFC@reference?5?$CG?$CG?5?$CI?$CKreference?$CJ?$AA@ (0000)
0027E628 0013:
	??_C@_0BD@IKFFHLNA@pool?9?$DOfree_size?$DO?$DN0?$AA@ (0000)
0027E63C 001c:
	??_C@_0BM@JADHFLPK@pool?9?$DOfree_size?$DM?$DNpool?9?$DOsize?$AA@ (0000)
0027E658 001c:
	??_C@_0BM@HDOIAFHN@actual_new_size?$DOblock?9?$DOsize?$AA@ (0000)
0027E674 0032:
	??_C@_0DC@FAPCJHLD@pool?9?$DOfree_size?$DO?$DN0?5?$CG?$CG?5pool?9?$DOfree@ (0000)
0027E6A8 000c:
	??_C@_0M@PJCPCKMH@new_size?$DO?$DN0?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "memory_pool.h"

/* ---------- constants */

#define POOL_SIGNATURE 'pool'
#define BLOCK_HEADER_SIGNATURE 'head'
#define BLOCK_TRAILER_SIGNATURE 'tail'

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static long code_0010dc10(
	long size);
static void *code_0010dc20(
	struct memory_pool *pool,
	long size);
static void code_0010dc50(
	struct memory_pool *pool);
static struct memory_pool_block *code_0010ddc0(
	struct memory_pool *pool,
	void **reference);

/* ---------- globals */

/* ---------- public code */

long memory_pool_allocation_size(
	long size)
{
	return size+sizeof(struct memory_pool);
}

void memory_pool_initialize(
	struct memory_pool *pool,
	char const *name,
	long size)
{
	csmemset(pool, 0, sizeof(struct memory_pool));
	pool->signature = 'pool';
	csstrncpy(pool->name, name, sizeof(pool->name)-1);
	pool->base_address = pool+1;
	pool->size = size;
	pool->free_size = size;
	pool->first_block = NULL;
	pool->last_block = NULL;
}

struct memory_pool *memory_pool_new(
	char const *name,
	long size)
{
	struct memory_pool *pool = match_malloc(
		"c:\\halo\\SOURCE\\memory\\memory_pool.c",
		70,
		memory_pool_allocation_size(size));

	if (pool)
	{
		memory_pool_initialize(pool, name, size);
	}

	return pool;
}

void memory_pool_delete(
	struct memory_pool *pool)
{
	code_0010dc50(pool);
	csmemset(pool, 0, sizeof(*pool));
	match_free("c:\\halo\\SOURCE\\memory\\memory_pool.c", 85, pool);
	return;
}

boolean memory_pool_block_allocate(
	struct memory_pool *pool,
	void **reference,
	long size)
{
	long actual_size;
	struct memory_pool_block *block;

	actual_size = size+sizeof(*block);
	if (actual_size&3)
		actual_size = (actual_size|3)+1;
	code_0010dc50(pool);
	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 124, size>=0);

	block = pool->last_block
		? (struct memory_pool_block *)((byte *)pool->last_block+pool->last_block->size)
		: pool->base_address;
	if ((byte *)block+actual_size <= (byte *)pool->base_address+pool->size && block)
	{
		block->size = actual_size;
		block->header_signature = BLOCK_HEADER_SIGNATURE;
		block->reference = reference;
		block->next_block = NULL;
		block->previous_block = pool->last_block;
		block->trailer_signature = BLOCK_TRAILER_SIGNATURE;
		if (!pool->first_block)
			pool->first_block = block;
		if (pool->last_block)
			pool->last_block->next_block = block;
		pool->last_block = block;
		pool->free_size -= block->size;
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 156, pool->free_size>=0);
		*reference = block+1;
		return TRUE;
	}
	/*
	The January object retains the private fit helper even though this caller
	contains its exact expanded source shape.
	*/
	if (FALSE)
		code_0010dc20(pool, actual_size);
	return FALSE;
}

void memory_pool_block_free(
	struct memory_pool *pool,
	void **reference)
{
	struct memory_pool_block *block;

	block = code_0010ddc0(pool, reference);
	pool->free_size += block->size;
	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 230, pool->free_size<=pool->size);
	if (block->previous_block)
		block->previous_block->next_block = block->next_block;
	else
		pool->first_block = block->next_block;
	if (block->next_block)
		block->next_block->previous_block = block->previous_block;
	else
		pool->last_block = block->previous_block;
	csmemset(block, 0, block->size);
	return;
}

void memory_pool_compact(
	struct memory_pool *pool)
{
	byte *destination;
	struct memory_pool_block *block;
	struct memory_pool_block *previous_block;

	block = pool->first_block;
	if (block)
	{
		destination = pool->base_address;
		previous_block = NULL;
		do
		{
			if ((byte *)block > destination)
			{
				csmemmove(destination, block, block->size);
				block = (struct memory_pool_block *)destination;
				*block->reference = block+1;
			}
			block->previous_block = previous_block;
			if (previous_block)
				previous_block->next_block = block;
			else
				pool->first_block = block;
			destination = (byte *)block+block->size;
			previous_block = block;
			block = block->next_block;
		} while (block);
		previous_block->next_block = NULL;
		pool->last_block = previous_block;
	}
	code_0010dc50(pool);
	return;
}

boolean memory_pool_block_reallocate(
	struct memory_pool *pool,
	void **reference,
	long new_size)
{
	long actual_new_size;
	struct memory_pool_block *block;
	byte *next_block_address;

	block = code_0010ddc0(pool, reference);
	actual_new_size = code_0010dc10(new_size);
	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 174, new_size>=0);

	next_block_address = block->next_block ? (byte *)block->next_block : (byte *)pool->base_address+pool->size;
	if ((byte *)block+actual_new_size <= next_block_address)
	{
		pool->free_size += block->size-actual_new_size;
		match_assert(
			"c:\\halo\\SOURCE\\memory\\memory_pool.c",
			184,
			pool->free_size>=0 && pool->free_size<=pool->size);
		block->size = actual_new_size;
		return TRUE;
	}

	{
		void *new_reference;
		struct memory_pool_block *new_block;

		if (memory_pool_block_allocate(pool, &new_reference, new_size))
		{
			match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 200, actual_new_size>block->size);
			csmemcpy(new_reference, *reference, block->size-sizeof(*block));
			memory_pool_block_free(pool, reference);
			new_block = code_0010ddc0(pool, &new_reference);
			new_block->reference = reference;
			*reference = new_reference;
			return TRUE;
		}
	}
	return FALSE;
}

long memory_pool_get_free_size(
	struct memory_pool *pool)
{
	return pool->free_size;
}

long memory_pool_get_used_size(
	struct memory_pool *pool)
{
	return !pool->last_block ? 0 : (unsigned long)pool->last_block + pool->last_block->size - (unsigned long)pool->base_address;
}

long memory_pool_get_contiguous_free_size(
	struct memory_pool *pool)
{
	return pool->size - memory_pool_get_used_size(pool);
}

/* ---------- private code */

static long code_0010dc10(
	long size)
{
	size += sizeof(struct memory_pool_block);
	if (size&3)
	{
		size = (size|3)+1;
	}

	return size;
}

static void *code_0010dc20(
	struct memory_pool *pool,
	long size)
{
	byte *address;

	address = pool->last_block
		? (byte *)pool->last_block+pool->last_block->size
		: pool->base_address;

	return address+size <= (byte *)pool->base_address+pool->size
		? address
		: NULL;
}

static void code_0010dc50(
	struct memory_pool *pool)
{
	struct memory_pool_block *block;
	struct memory_pool_block *previous_block;

	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 340, pool->signature==POOL_SIGNATURE);
	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 341, pool->size>0);

	previous_block = NULL;
	for (block = pool->first_block; block; block = block->next_block)
	{
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 352, block->previous_block==previous_block);
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 353, block->next_block || pool->last_block==block);
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 355, block->header_signature==BLOCK_HEADER_SIGNATURE);
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 356, block->trailer_signature==BLOCK_TRAILER_SIGNATURE);
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 358, (byte *)block>=(byte *)pool->base_address);
		match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 359, (byte *)block+block->size<=(byte *)pool->base_address+pool->size);
		previous_block = block;
	}
	return;
}

static struct memory_pool_block *code_0010ddc0(
	struct memory_pool *pool,
	void **reference)
{
	struct memory_pool_block *block;
	struct memory_pool_block *other_block;

	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 372, reference && (*reference));
	code_0010dc50(pool);
	block = (struct memory_pool_block *)((byte *)*reference-sizeof(*block));
	match_vassert(
		"c:\\halo\\SOURCE\\memory\\memory_pool.c",
		379,
		block->reference==reference,
		csprintf(temporary, "expected reference %08x but got %08x", block->reference, reference));

	for (other_block = pool->first_block; other_block && block != other_block; other_block = other_block->next_block)
	{
	}
	match_assert("c:\\halo\\SOURCE\\memory\\memory_pool.c", 388, other_block);
	return block;
}
