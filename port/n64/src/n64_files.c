/*
N64_FILES.C

Read-only files on the cartridge (libdragon's DragonFS, "rom:/..."), for
the XDK-facing file layer (xdk_kernel.c) and the map loader.
*/

#include <libdragon.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"

#define MAXIMUM_FILES 8

static FILE *files[MAXIMUM_FILES];
static int decoded_sizes[MAXIMUM_FILES];

long n64_file_open(const char *path)
{
	for (int i = 0; i < MAXIMUM_FILES; i++)
	{
		if (!files[i])
		{
			/* Map reads are sequential, with only forward/alignment seeks.
			Streaming LZ4 uses a bounded 64 KiB history, never a second map image. */
			size_t length = strlen(path);
			decoded_sizes[i] = -1;
			files[i] = length > 4 && !strcmp(path + length - 4, ".map") ?
				asset_fopen(path, &decoded_sizes[i]) : fopen(path, "rb");
			return files[i] ? i : -1;
		}
	}
	return -1;
}

long n64_file_size(long handle)
{
	if (decoded_sizes[handle] >= 0)
		return decoded_sizes[handle];
	FILE *file = files[handle];
	long position = ftell(file);
	long size;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, position, SEEK_SET);
	return size;
}

long n64_file_read(long handle, void *buffer, long size)
{
	return (long)fread(buffer, 1, size, files[handle]);
}

long n64_file_seek(long handle, long offset)
{
	return fseek(files[handle], offset, SEEK_SET) == 0 ? offset : -1;
}

void n64_file_close(long handle)
{
	if (handle >= 0 && handle < MAXIMUM_FILES && files[handle])
	{
		fclose(files[handle]);
		files[handle] = NULL;
	}
}

int n64_debug_flag(const char *name)
{
	/* (asked for in hot loops -- per object, per sound -- by literal: the
	same pointer each time; a name the cache did not hold opened a file
	each time, and there are more than its first 16) */
	static struct { const char *name; int value; } cache[64];
	static int count;
	/* (and a slot by the pointer in front: some are asked for per object,
	per ray, per sound) */
	static struct { const char *name; int value; } slots[64];
	unsigned slot = ((unsigned long)name >> 2) & 63;
	char path[64];
	FILE *file;

	if (slots[slot].name == name)
		return slots[slot].value;
	for (int i = 0; i < count; i++)
	{
		if (cache[i].name == name)
		{
			slots[slot].name = name;
			slots[slot].value = cache[i].value;
			return cache[i].value;
		}
	}
	for (int i = 0; i < count; i++)
	{
		/* (the same name at another address: another file's literal; it
		takes the slot too, or each call came back here to compare names) */
		if (!strcmp(cache[i].name, name))
		{
			slots[slot].name = name;
			slots[slot].value = cache[i].value;
			return cache[i].value;
		}
	}
	snprintf(path, sizeof(path), "rom:/%s", name);
	file = fopen(path, "rb");
	if (file)
		fclose(file);
	if (count < 64)
	{
		cache[count].name = name;
		cache[count].value = file != NULL;
		count++;
	}
	return file != NULL;
}

/* a debug setting's text: a file on the cartridge (port.json "flags" entry
"name=value"), its first line; 0 when there is none */
int n64_debug_text(const char *name, char *buffer, int size)
{
	char path[64];
	FILE *file;
	int length;

	snprintf(path, sizeof(path), "rom:/%s", name);
	file = fopen(path, "rb");
	if (!file)
		return 0;
	length = (int)fread(buffer, 1, size - 1, file);
	fclose(file);
	if (length < 0)
		length = 0;
	buffer[length] = 0;
	for (int i = 0; i < length; i++)
	{
		if (buffer[i] == '\n' || buffer[i] == '\r')
		{
			buffer[i] = 0;
			break;
		}
	}
	return buffer[0] != 0;
}
