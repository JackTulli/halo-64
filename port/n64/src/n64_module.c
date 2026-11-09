/*
N64_MODULE.C

A module: code and data in the ROM, loaded into the heap while they are
wanted and given back after (HUD STYLE 64's, rom:/hud64.dso:
port/n64/game/n64_hud64_stub.c). The file is libdragon's DSO format (its
n64dso tool: tools/n64_build.py), loaded with the part of libdragon's
dlopen (its src/dlfcn.c) that this program's modules need. dlopen itself,
linked in, keeps about 6 KB of the program in memory with either style; it
also keeps a list of modules, runs constructors, registers unwind tables
and names the module in backtraces, none of which a module here needs.

The file: its header, then the module's resident part (the module's header,
its symbols and their names, its image), then the part needed only while it
is linked (its imports' names, its relocations); both stored as they are
(n64dso -c 0). Each import is found by its name in the program's symbols
(n64dso-msym: the ROM's ".msym", sorted by name), read in for the link and
given back after it.
*/

#include <libdragon.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "n64.h"

/* (libdragon's rompak_internal.h) */
extern pi_addr_t rompak_search_ext(const char *ext, size_t *size);

#define DSO_MAGIC 0x44534F31 /* "DSO1" */
#define MSYM_MAGIC 0x4D53594D /* "MSYM" */

struct dso_header
{
	uint32_t magic;
	uint32_t resident_offset, resident_size;
	uint32_t link_offset, link_size;
};

/* a symbol: its name (an offset from the resident part's start; an
import's, from the link part's), its value, its flags and size */
struct dso_symbol
{
	uint32_t name, value, info;
};
#define DSO_WEAK 0x80000000 /* an import that may be missing */
#define DSO_ABSOLUTE 0x40000000 /* a value not in the image */

/* a relocation: the word's offset in the image; its type (the top 8 bits)
and its symbol's index (the rest) */
struct dso_relocation
{
	uint32_t offset, info;
};
enum { RELOC_NONE = 0, RELOC_32 = 2, RELOC_26 = 4, RELOC_HI16 = 5, RELOC_LO16 = 6 };

/* the resident part's start (libdragon's dso_module_t): offsets from it,
the relocations' from the link part's */
struct dso_module
{
	uint32_t prev, next, ref_count, source, filename;
	uint32_t symbols, symbol_count, import_count;
	uint32_t relocations, relocation_count;
	uint32_t image, image_size;
	uint32_t unwind[6], debug_symbols, mode;
};

/* a symbol by name in a table sorted by name (names: where the names'
offsets start) */
static __attribute__((noinline)) const struct dso_symbol *find(const struct dso_symbol *symbols, uint32_t count, const char *names, const char *name)
{
	uint32_t low = 0, high = count;

	while (low < high)
	{
		uint32_t middle = (low + high) / 2;
		int order = strcmp(name, names + symbols[middle].name);

		if (!order)
			return &symbols[middle];
		if (order < 0)
			high = middle;
		else
			low = middle + 1;
	}
	return NULL;
}

/* the program's symbols (the ROM's ".msym"), the names' offsets from the
table's start; NULL without them */
static struct dso_symbol *program_symbols(uint32_t *count)
{
	/* (a data cache line of its own: the DMA writes it) */
	static struct
	{
		uint32_t magic, size, count, unused;
	} header __attribute__((aligned(16)));
	pi_addr_t rom = rompak_search_ext(".msym", NULL);
	struct dso_symbol *table;

	if (!rom)
		return NULL;
	data_cache_hit_writeback_invalidate(&header, sizeof(header));
	dma_read_raw_async(&header, rom, 12);
	dma_wait();
	if (header.magic != MSYM_MAGIC || !header.size)
		return NULL;
	table = memalign(16, (header.size + 15) & ~15);
	if (!table)
		return NULL;
	data_cache_hit_writeback_invalidate(table, header.size);
	dma_read_raw_async(table, rom + 12, header.size);
	dma_wait();
	*count = header.count;
	return table;
}

/* the module's symbols given their values (an import's from the program,
the rest in the image), its relocations applied, the caches made to see
it; its export name's value */
static int link_module(uint8_t *base, const uint8_t *link, const struct dso_symbol *program, uint32_t program_count,
	const char *name, void **address)
{
	struct dso_module *module = (struct dso_module *)base;
	struct dso_symbol *symbols = (struct dso_symbol *)(base + module->symbols);
	const struct dso_relocation *relocations = (const struct dso_relocation *)(link + module->relocations);
	uint8_t *image = base + module->image;
	const struct dso_symbol *found;

	/* (symbol 0: the image's start, which the image's own relocations use) */
	for (uint32_t i = 0; i < module->symbol_count; i++)
	{
		struct dso_symbol *symbol = &symbols[i];

		if (i >= 1 && i <= module->import_count)
		{
			const char *import = (const char *)link + symbol->name;

			found = find(program, program_count, (const char *)program, import);
			if (found)
				symbol->value = found->value;
			else if (!(symbol->info & DSO_WEAK))
			{
				debugf("module: %s not in the program\n", import);
				return 0;
			}
		}
		else if (!(symbol->info & DSO_ABSOLUTE))
			symbol->value += (uint32_t)image;
	}
	for (uint32_t i = 0; i < module->relocation_count; i++)
	{
		/* (a C module's words are aligned) */
		uint32_t *target = (uint32_t *)(image + relocations[i].offset);
		uint32_t value = symbols[relocations[i].info & 0xFFFFFF].value;

		switch (relocations[i].info >> 24)
		{
		case RELOC_NONE:
			break;
		case RELOC_32:
			*target += value;
			break;
		case RELOC_26:
			*target = (*target & 0xFC000000) | ((((*target & 0x3FFFFFF) << 2) + value) & 0xFFFFFFC) >> 2;
			break;
		case RELOC_HI16:
		{
			/* (the address from its %lo pair, the next LO16) */
			uint32_t j = i + 1, sum;

			while (j < module->relocation_count && relocations[j].info >> 24 != RELOC_LO16)
				j++;
			if (j == module->relocation_count)
				return 0;
			sum = (*target << 16) + (int16_t)(*(uint32_t *)(image + relocations[j].offset) & 0xFFFF) + value;
			*target = (*target & 0xFFFF0000) | ((sum + 0x8000) >> 16);
			break;
		}
		case RELOC_LO16:
			*target = (*target & 0xFFFF0000) | ((*target + value) & 0xFFFF);
			break;
		default:
			/* (a module built with -G0 has no GP-relative ones) */
			return 0;
		}
	}
	data_cache_hit_writeback_invalidate(image, module->image_size);
	inst_cache_hit_invalidate(image, module->image_size);
	/* (the exports: after the imports, sorted by name) */
	found = find(symbols + module->import_count + 1, module->symbol_count - module->import_count - 1, (const char *)base, name);
	if (!found)
		return 0;
	*address = (void *)found->value;
	return 1;
}

void *n64_module_load(const char *path, const char *name, unsigned long margin, void **address)
{
	struct dso_header header;
	uint8_t *base = NULL, *link = NULL;
	struct dso_symbol *program = NULL;
	uint32_t program_count = 0;
	int file = open(path, O_RDONLY), linked = 0;

	*address = NULL;
	if (file < 0)
		return NULL;
	if (read(file, &header, sizeof(header)) != (ssize_t)sizeof(header) || header.magic != DSO_MAGIC)
	{
		close(file);
		return NULL;
	}
	/* (room for it that leaves the margin free once it is linked; while it
	links, the margin holds its relocations and the program's symbols) */
	if (!n64_render_room_leaving(header.resident_size, margin > header.link_size + 4096 ? margin :
		header.link_size + 4096))
	{
		debugf("module: %s: %lu + %lu KB wanted, %lu KB free\n", path,
			(unsigned long)header.resident_size / 1024, margin / 1024, n64_heap_free() / 1024);
		close(file);
		return NULL;
	}
	/* (the resident part first, below the rest, which is given back; at
	the alignment n64dso gives the image in it, the module's segment's) */
	if ((base = memalign(32, header.resident_size)) && (link = malloc(header.link_size)) &&
		lseek(file, header.resident_offset, SEEK_SET) == (off_t)header.resident_offset &&
		read(file, base, header.resident_size) == (ssize_t)header.resident_size &&
		lseek(file, header.link_offset, SEEK_SET) == (off_t)header.link_offset &&
		read(file, link, header.link_size) == (ssize_t)header.link_size &&
		(program = program_symbols(&program_count)))
		linked = link_module(base, link, program, program_count, name, address);
	close(file);
	free(program);
	free(link);
	if (!linked)
	{
		free(base);
		*address = NULL;
		return NULL;
	}
	return base;
}

void n64_module_free(void *module)
{
	free(module);
}
