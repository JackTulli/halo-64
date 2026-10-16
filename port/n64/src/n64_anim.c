/*
N64_ANIM.C

Animation frames streamed from the cartridge. The converter moves every
animation's frame data out of the map (tools/n64/convert.py) into a file
for each animation graph, shared between maps by content
(rom:/anim/<hash>.anim; rom:/maps/<map>.anims lists the map's); the
animation's tag data keeps its size, a flag and its offset: its graph's
place in the list in the top 8 bits, the offset in that graph's file below.
A frame is read with the 4 KB chunk around it into a small
least-recently-used cache, so the frames that follow (the next ticks') are
already there.
*/

#include <libdragon.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"

#define CHUNK_SIZE 4096
#define CHUNK_COUNT 16 /* 64 KiB; frames are unchanged, misses stream from ROM */

#define MAXIMUM_GRAPHS 256

struct chunk
{
	long start; /* cartridge address of the chunk's first byte, or -1 */
	long length; /* bytes read: a chunk stops where its animation ends */
	unsigned long last_used;
	uint8_t *data;
};

static struct
{
	/* each graph's file: its cartridge address and size */
	uint32_t rom_addresses[MAXIMUM_GRAPHS];
	long sizes[MAXIMUM_GRAPHS];
	int graph_count;
	long file_size;
	struct chunk chunks[CHUNK_COUNT];
	unsigned long clock;
	unsigned long misses, requests;
	int last_hit;
} anim;

/* the cache's chunks, in one block made at boot (n64_main.c): made at the
first map's load, cache blocks sat above its image, kept for good, and
split the heap a later, bigger map needs in one piece (Sidewinder's
2.8 MB: "no memory for the map") */
void n64_animation_reserve(void)
{
	uint8_t *block;

	if (anim.chunks[0].data)
		return;
	block = memalign(16, CHUNK_COUNT * CHUNK_SIZE);
	if (!block)
		return;
	for (int i = 0; i < CHUNK_COUNT; i++)
		anim.chunks[i].data = block + i * CHUNK_SIZE;
}

void n64_animation_open(const char *map_name)
{
	char path[64], line[32];
	FILE *list;

	anim.graph_count = 0;
	anim.file_size = 0;
	snprintf(path, sizeof(path), "rom:/maps/%s.anims", map_name);
	list = fopen(path, "r");
	while (list && anim.graph_count < MAXIMUM_GRAPHS && fgets(line, sizeof(line), list))
	{
		char graph_path[48];
		FILE *file;

		if (strlen(line) < 16)
			continue;
		snprintf(graph_path, sizeof(graph_path), "anim/%.16s.anim", line);
		anim.rom_addresses[anim.graph_count] = dfs_rom_addr(graph_path);
		anim.sizes[anim.graph_count] = 0;
		snprintf(path, sizeof(path), "rom:/%s", graph_path);
		file = fopen(path, "rb");
		if (file)
		{
			fseek(file, 0, SEEK_END);
			anim.sizes[anim.graph_count] = ftell(file);
			fclose(file);
		}
		anim.file_size += anim.sizes[anim.graph_count];
		anim.graph_count++;
	}
	if (list)
		fclose(list);
	snprintf(path, sizeof(path), "rom:/maps/%s.anims", map_name);
	for (int i = 0; i < CHUNK_COUNT; i++)
	{
		if (!anim.chunks[i].data)
			anim.chunks[i].data = memalign(16, CHUNK_SIZE);
		anim.chunks[i].start = -1;
	}
	debugf("n64: animation frames %s: %ld KB streamed, %d KB cache\n", path, anim.file_size / 1024,
		CHUNK_COUNT * CHUNK_SIZE / 1024);
}

/* a frame of a streamed animation: its tag data's file offset and size, the
frame and the frame size */
void *n64_animation_frame(long data_offset, long data_size, short frame_index, short frame_size)
{
	int graph = (unsigned long)data_offset >> 24;
	long graph_offset = data_offset & 0xFFFFFF;
	long base = graph < anim.graph_count ? (long)anim.rom_addresses[graph] : 0;
	long graph_size = graph < anim.graph_count ? anim.sizes[graph] : 0;
	/* addresses on the cartridge, so that a chunk is a chunk of any graph */
	long offset = base + graph_offset + (long)frame_index * frame_size;
	long start;
	struct chunk *chunk = NULL;
	struct chunk *oldest = &anim.chunks[0];

	anim.requests++;
	anim.clock++;
	/* (the chunk the last frame came from first: a tick's frames come mostly
	from the same few) */
	{
		struct chunk *c = &anim.chunks[anim.last_hit];

		if (c->start >= 0 && offset >= c->start && offset + frame_size <= c->start + c->length)
		{
			c->last_used = anim.clock;
			return c->data + (offset - c->start);
		}
	}
	/* a chunk that holds the whole frame */
	for (int i = 0; i < CHUNK_COUNT; i++)
	{
		struct chunk *c = &anim.chunks[i];

		/* only what was read counts: the next animation's frames follow in
		the file, and a chunk read for this one stopped at its end */
		if (c->start >= 0 && offset >= c->start && offset + frame_size <= c->start + c->length)
		{
			chunk = c;
			anim.last_hit = i;
			break;
		}
		if (c->last_used < oldest->last_used)
			oldest = c;
	}
	if (!chunk)
	{
		long length;

		anim.misses++;
		chunk = oldest;
		/* from the frame on, as much of the animation as fits */
		start = offset & ~1L;
		length = base + graph_offset + data_size - start;
		if (length > CHUNK_SIZE)
			length = CHUNK_SIZE;
		if (start + length > base + graph_size)
			length = base + graph_size - start;
		data_cache_hit_writeback_invalidate(chunk->data, CHUNK_SIZE);
		dma_read(chunk->data, start, (length + 1) & ~1L);
		chunk->start = start;
		chunk->length = length;
		anim.last_hit = (int)(chunk - anim.chunks);
	}
	chunk->last_used = anim.clock;
	if ((anim.requests & 4095) == 0)
	{
		if (n64_debug_flag("debug_stats"))
			debugf("anim: %lu requests, %lu%% read from the cartridge\n", anim.requests, anim.misses * 100 / anim.requests);
		anim.misses = anim.requests = 0;
	}
	return chunk->data + (offset - chunk->start);
}
