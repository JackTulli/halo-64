/*
N64_MAIN.C

The N64 build's entry point and system services (port/n64/src/n64.h):
libdragon's start-up, the debug log, time, and the jump into the game's own
main (source/shell/shell_xbox.c, compiled as halo_shell_main).
*/

#include <libdragon.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "n64.h"

int halo_shell_main(void);
void n64_profile_start(void);
void n64_exceptions_install(void);

void n64_log(const char *text)
{
	size_t length = strlen(text);

	while (length && (text[length - 1] == '\n' || text[length - 1] == '\r'))
		length--;
	debugf("%.*s\n", (int)length, text);
}

void n64_fatal(const char *text)
{
	debugf("FATAL: %s\n", text);
	assertf(0, "%s", text);
	abort();
}

void n64_backtrace(void)
{
	debug_backtrace();
}

unsigned long n64_ticks(void)
{
	return TICKS_READ();
}

unsigned long long n64_ticks64(void)
{
	return get_ticks();
}

unsigned long long n64_microseconds(void)
{
	return get_ticks_us();
}

void n64_wait_microseconds(unsigned long microseconds)
{
	wait_ticks(TICKS_FROM_US(microseconds));
}

unsigned long n64_heap_free(void)
{
	heap_stats_t heap;

	sys_get_heap_stats(&heap);
	return (unsigned long)heap.free;
}

/* whether size bytes can be had in one piece now. A malloc freed unused is
dropped by the compiler, which then takes it for a success (the HUD's
sprite check never looked): the block goes through a volatile first */
static void *volatile heap_probe;

int n64_heap_has(unsigned long size)
{
	heap_probe = malloc(size);
	if (!heap_probe)
		return 0;
	free(heap_probe);
	heap_probe = NULL;
	return 1;
}

/* ---------- the heap's map

newlib's malloc (dlmalloc 2.6) lays its chunks end to end from its first
sbrk: each with its size four bytes in (the low bit: the chunk before is in
use), the top chunk last, and libdragon's sbrk room above that. A map's
image wants one piece: the free pieces of HEAP_PIECE and more are listed
("<KB>K@<KB from the heap's start>") with what is in use between them (how
many blocks, their KB, the first one's bytes), so that a "no memory" screen
says what stood in the way. Returns the largest piece. */
extern char *__malloc_sbrk_base;
extern void *__malloc_av_[];
extern char *__heap_end, *__heap_top;

#define HEAP_PIECE (64 * 1024)

unsigned long n64_heap_describe(char *text, unsigned long size)
{
	char *base = __malloc_sbrk_base, *top = __malloc_av_[2], *chunk;
	unsigned long largest = 0, length = 0, between = 0, between_bytes = 0, first_used = 0;
	int pieces = 0;

	if (size)
		text[0] = 0;
	if (base == (char *)-1 || !top)
		return (unsigned long)(__heap_top - __heap_end);
	/* (the first chunk: its memory 8-byte aligned) */
	chunk = base + ((8 - ((unsigned long)base & 7)) & 7);
	while (chunk <= top)
	{
		unsigned long chunk_size = *(unsigned long *)(chunk + 4) & ~3UL, free_size;
		int used;

		if (chunk == top)
		{
			free_size = chunk_size + (unsigned long)(__heap_top - __heap_end);
			used = 0;
		}
		else
		{
			if (chunk_size < 16 || chunk + chunk_size > top)
				break;
			used = *(unsigned long *)(chunk + chunk_size + 4) & 1;
			free_size = used ? 0 : chunk_size;
		}
		if (!used && free_size >= HEAP_PIECE)
		{
			if (free_size > largest)
				largest = free_size;
			if (length < size)
			{
				if (pieces)
					length += snprintf(text + length, size - length, " | %lu used %lu KB (first %lu B) | ", between,
						between_bytes / 1024, first_used);
				if (length < size)
					length += snprintf(text + length, size - length, "%luK@%lu", free_size / 1024,
						(unsigned long)(chunk - base) / 1024);
			}
			pieces++;
			between = between_bytes = first_used = 0;
		}
		else if (used)
		{
			if (!between)
				first_used = chunk_size;
			between++;
			between_bytes += chunk_size;
		}
		if (chunk == top)
			break;
		chunk += chunk_size;
	}
	return largest;
}

/* testing (port.json flag "debug_heap_dump"): every block in use from
start on, its offset from the heap's start, size and first two words, in
the log -- what a game or the menu left behind */
void n64_heap_dump(const char *tag, const void *start)
{
	char *base = __malloc_sbrk_base, *top = __malloc_av_[2], *chunk;
	int count = 0;

	if (!n64_debug_flag("debug_heap_dump") || base == (char *)-1 || !top)
		return;
	chunk = base + ((8 - ((unsigned long)base & 7)) & 7);
	while (chunk < top)
	{
		unsigned long chunk_size = *(unsigned long *)(chunk + 4) & ~3UL;

		if (chunk_size < 16 || chunk + chunk_size > top)
			break;
		if ((*(unsigned long *)(chunk + chunk_size + 4) & 1) && chunk >= (char *)start)
		{
			unsigned long *w = (unsigned long *)(chunk + 8);

			if (chunk_size <= 128)
				debugf("heap %s: +%lu KB %lu B %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx at %08lx\n", tag,
					(unsigned long)(chunk - base) / 1024, chunk_size, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7],
					(unsigned long)(chunk + 8));
			else
				debugf("heap %s: +%lu KB %lu B %08lx %08lx\n", tag, (unsigned long)(chunk - base) / 1024, chunk_size,
					w[0], w[1]);
			count++;
		}
		chunk += chunk_size;
	}
	debugf("heap %s: %d blocks in use\n", tag, count);
}

/* ---------- small uncached blocks

libdragon's mixer gives each channel a VADPCM decoder state of its own
(malloc_uncached, a few dozen bytes) and makes it again for each sound it
starts: hundreds over a game, each where malloc found the room. The ones a
game or the menu left behind sat wherever the last ones went -- in the
middle of the memory the next map's image wanted in one piece. Blocks of
SMALL_UNCACHED bytes or fewer come from a pool of their own instead (the
link wraps libdragon's malloc_uncached and free_uncached: port.json's
ldflags), bigger ones from the heap as before. */
#define SMALL_UNCACHED 64
#define SMALL_UNCACHED_COUNT 64

static uint8_t small_uncached[SMALL_UNCACHED_COUNT][SMALL_UNCACHED] __attribute__((aligned(16)));
static uint8_t small_uncached_used[SMALL_UNCACHED_COUNT];

void *__wrap_malloc_uncached(size_t size) __attribute__((used));
void *__wrap_malloc_uncached_aligned(int align, size_t size) __attribute__((used));
void __wrap_free_uncached(void *buffer) __attribute__((used));

/* libdragon's malloc_uncached_aligned (wrapped below): whole cache lines,
none of them in the data cache */
static void *heap_uncached(int align, size_t size)
{
	void *block;

	if (align < 16)
		align = 16;
	size = (size + 15) & ~(size_t)15;
	block = memalign(align, size);
	if (!block)
		return NULL;
	data_cache_hit_invalidate(block, size);
	return UncachedAddr(block);
}

/* ---------- the uncached blocks' reserve

libdragon asserts when one of its uncached blocks cannot be had -- the
chunks rspq and rdpq record into, up to 32 KB -- and the HUD's recording ran
out on Blood Gulch in high detail (its sprites, loaded while it recorded,
took the room it had begun with). A reserve is kept: when the heap has no
room the reserve is given back and the block tried again, and the renderer
takes room for it again at its next frame (n64_render_frame_begin). */
static void *uncached_reserve;
static int uncached_reserve_spent;

static void *heap_uncached_or_reserve(int align, size_t size)
{
	void *block = heap_uncached(align, size);

	if (!block && uncached_reserve)
	{
		free(uncached_reserve);
		uncached_reserve = NULL;
		uncached_reserve_spent++;
		block = heap_uncached(align, size);
	}
	return block;
}

/* the reserve taken (again); the number of times it was spent, so far */
int n64_uncached_reserve_take(void)
{
	if (!uncached_reserve)
		uncached_reserve = malloc(N64_UNCACHED_RESERVE);
	return uncached_reserve != NULL;
}

/* the reserve made again where the heap has room now (the end of a game:
taken again during it, it stayed wherever the heap had room then) */
void n64_uncached_reserve_settle(void)
{
	n64_uncached_reserve_release();
	n64_uncached_reserve_take();
}

void n64_uncached_reserve_release(void)
{
	free(uncached_reserve);
	uncached_reserve = NULL;
}

int n64_uncached_reserve_missing(void)
{
	return !uncached_reserve;
}

int n64_uncached_reserve_spent(void)
{
	return uncached_reserve_spent;
}

/* ---------- display buffers in the map's home

The map's image has a home of its own (n64_game.c's n64_map_home_reserve,
at boot, the size of the largest map) in which nothing else is made, so
that no block left behind by the menu or a game can split the memory an
image wants in one piece. What the image does not use is lent to the
display (n64_render.c): the menu's 640x480 buffers and z-buffer (no map
then), the game's 480i buffers past the image. While a loan is open the
surfaces libdragon makes (surface_alloc: malloc_uncached_aligned, wrapped
by the link) of LENT_SURFACE bytes or more are taken from it in turn;
freeing anything in the home is nothing (the home's memory stays the
home's). */
#define LENT_SURFACE (16 * 1024)

static uint8_t *home_start, *home_end;
static uint8_t *lent, *lent_end;

void n64_display_set_home(void *start, unsigned long size)
{
	home_start = start;
	home_end = home_start + size;
}

void n64_display_lend(void *start, unsigned long size)
{
	lent = size ? start : NULL;
	lent_end = lent ? lent + size : NULL;
}

void *__wrap_malloc_uncached_aligned(int align, size_t size)
{
	if (lent && size >= LENT_SURFACE)
	{
		uintptr_t alignment = align < 64 ? 64 : (uintptr_t)align;
		uint8_t *block = (uint8_t *)(((uintptr_t)lent + alignment - 1) & ~(alignment - 1));
		size_t rounded = (size + 15) & ~(size_t)15;

		if (block + rounded <= lent_end)
		{
			lent = block + rounded;
			data_cache_hit_invalidate(block, rounded);
			return UncachedAddr(block);
		}
	}
	return heap_uncached_or_reserve(align, size);
}

void *__wrap_malloc_uncached(size_t size)
{
	if (size <= SMALL_UNCACHED)
	{
		disable_interrupts();
		for (int i = 0; i < SMALL_UNCACHED_COUNT; i++)
		{
			if (!small_uncached_used[i])
			{
				small_uncached_used[i] = 1;
				enable_interrupts();
				/* (none of it in the data cache, as libdragon's: only ever
				used through the uncached address) */
				data_cache_hit_invalidate(small_uncached[i], SMALL_UNCACHED);
				return UncachedAddr(small_uncached[i]);
			}
		}
		enable_interrupts();
	}
	return heap_uncached_or_reserve(16, size);
}

void __wrap_free_uncached(void *buffer)
{
	uint8_t *cached = CachedAddr(buffer);

	if (cached >= small_uncached[0] && cached < small_uncached[0] + sizeof(small_uncached))
	{
		small_uncached_used[(cached - small_uncached[0]) / SMALL_UNCACHED] = 0;
		return;
	}
	/* (a display buffer lent from the map's home) */
	if (home_start && cached >= home_start && cached < home_end)
		return;
	free(cached);
}

/* ---------- room for libdragon's RSP crash screen

libdragon's RSP crash screen (__rsp_crash: the CPU waited too long for the
RSP or the RDP -- display_get's frame buffer, rspq_wait -- or an RSP assert)
makes a display of its own, console_init's two 640x240 buffers (600 KB),
when the game has all the memory: the model cache fills the heap and the
display's buffers are lent from the map's home. On the user's console
(2026-10-15) it stopped at "Out of memory" in display_init instead of
showing the crash. The link wraps console_init (port.json's ldflags; the
game never calls it, only the crash screen does): the map's whole home is
lent to the display -- the game is over by then -- and the uncached reserve
given back, before libdragon's console_init. */
void __real_console_init(void);
void __wrap_console_init(void) __attribute__((used));

void __wrap_console_init(void)
{
	if (home_start)
		n64_display_lend(home_start, (unsigned long)(home_end - home_start));
	n64_uncached_reserve_release();
	__real_console_init();
}

/* Halo's deepest calls (collision, physics under the game tick) need more
than the 64 KB libdragon keeps for main's stack at the top of RAM (~97 KB
in Wizard), and the heap grows up to it. The game runs on main's stack
made deeper: the heap's top is lowered by GAME_STACK_EXTRA (libdragon's
sbrk_top, the call its display makes for the Z buffer), right under those
64 KB, so the stack goes on down into it -- 192 KB of stack for 128 KB of
heap (a stack of its own from the heap, as before, cost all 192 KB and left
libdragon's 64 KB idle). Filled with a pattern so that its deepest use can
be reported (n64_stack_used). */
#define GAME_STACK_SIZE (192 * 1024)
#define GAME_STACK_EXTRA (GAME_STACK_SIZE - 64 * 1024)
#define GAME_STACK_FILL 0xA5A5A5A5u

void *sbrk_top(int incr);

static uint32_t *game_stack;
static unsigned long game_stack_size = GAME_STACK_SIZE;

__asm__(
	".text\n"
	".align 3\n"
	".globl n64_call_on_stack\n"
	".type n64_call_on_stack, @function\n"
	".set push\n"
	".set noreorder\n"
	"n64_call_on_stack:\n"
	"	addiu $sp, $sp, -16\n"
	"	sd $ra, 8($sp)\n"
	"	sd $s0, 0($sp)\n"
	"	move $s0, $sp\n"
	"	jalr $a0\n"
	"	move $sp, $a1\n"
	"	move $sp, $s0\n"
	"	ld $s0, 0($sp)\n"
	"	ld $ra, 8($sp)\n"
	"	jr $ra\n"
	"	addiu $sp, $sp, 16\n"
	".set pop\n"
	".size n64_call_on_stack, .-n64_call_on_stack\n");

/* calls function with the stack pointer at top */
void n64_call_on_stack(void (*function)(void), void *top);

unsigned long n64_stack_used(void)
{
	unsigned long words = game_stack_size / 4, i;

	if (!game_stack)
		return 0;
	for (i = 0; i < words && game_stack[i] == GAME_STACK_FILL; i++)
		;
	return (words - i) * 4;
}

static void game_main(void)
{
	halo_shell_main();
	debugf("halo-n64: the game returned\n");
	for (;;)
		;
}

int main(void)
{
	debug_init_isviewer();
	/* USB debug writes to cartridge SDRAM. This content build has no reserved
	USB scratch region; keep it offline until a bounded transport owns one. */
	n64_exceptions_install();
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();
	debugf("halo-n64: %d KB of RAM\n", (int)(get_memory_size() / 1024));
	/* what the game keeps from its first map on, made now, at the heap's
	bottom: made with the first map they sat above its image, and a later,
	bigger map (Sidewinder's 2.8 MB) found no room in one piece */
	n64_animation_reserve();
	n64_hud_reserve();
	n64_particles_reserve();
	/* the sound's buffers and mixer, made once: made with the title's
	music, after the menu's 640x480 buffers, they were left in the middle of
	the memory the first map's image wanted in one piece (Sidewinder's,
	Blood Gulch's) */
	n64_sound_initialize();
	n64_sound_reserve();
	/* newlib's float printing and parsing (dtoa) keeps its big numbers on
	lists of its own for good, made the first time a float is printed (a
	log line, the HUD): made now, a few sizes of them, at the heap's bottom
	(a "debug_heap_dump" cycle found them between games) */
	{
		char text[96];
		volatile double parsed = strtod("12345.678901", NULL) + strtod("0.000012345", NULL);

		snprintf(text, sizeof(text), "%f %e %g %f %.3f", 1.5, 1234567.875, 0.000125, 3.0e15, (double)parsed);
		debugf("halo-n64: floats %s\n", text);
	}
	/* libdragon's EEPROM cache (512 bytes, made the first time the EEPROM is
	used: the menu's settings) made now too */
	if (eeprom_present() != EEPROM_NONE)
	{
		uint8_t byte;

		eeprom_read_bytes(&byte, 0, 1);
	}
	/* the maps' images' home, from the heap's bottom (n64_game.c) */
	n64_map_home_reserve();
	n64_uncached_reserve_take();
	{
		/* an "autotest" file on the cartridge: scripted controllers
		(xdk_input.c) for unattended test runs */
		extern int n64_test_input;
		FILE *flag = fopen("rom:/autotest", "rb");

		if (flag)
		{
			n64_test_input = 1;
			fclose(flag);
			debugf("halo-n64: test mode, scripted controllers\n");
		}
	}
	n64_profile_start();
	{
		/* main's stack (the top 64 KB of RAM) with the heap's top lowered
		under it: one stack, the game on it from here */
		uint8_t *extra = sbrk_top(GAME_STACK_EXTRA);
		uint8_t *top = (uint8_t *)KSEG0_START_ADDR + get_memory_size();
		uint8_t *here = (uint8_t *)__builtin_frame_address(0);

		if (extra != (uint8_t *)-1 && extra + GAME_STACK_EXTRA == top - 64 * 1024 && here > top - 64 * 1024 &&
			here <= top)
		{
			/* the pattern up to a little under this frame */
			game_stack = (uint32_t *)extra;
			game_stack_size = (unsigned long)((here - 1024 - extra) & ~15);
			for (unsigned long i = 0; i < game_stack_size / 4; i++)
				game_stack[i] = GAME_STACK_FILL;
			debugf("halo-n64: the game's stack is main's, %lu KB deep\n", (unsigned long)(here - extra) / 1024);
			game_main();
			return 0;
		}
		if (extra != (uint8_t *)-1)
			sbrk_top(-GAME_STACK_EXTRA);
	}
	/* (as it was: a stack of the game's own from the heap) */
	game_stack = memalign(16, GAME_STACK_SIZE);
	if (!game_stack)
	{
		game_main();
		return 0;
	}
	game_stack_size = GAME_STACK_SIZE;
	for (unsigned long i = 0; i < GAME_STACK_SIZE / 4; i++)
		game_stack[i] = GAME_STACK_FILL;
	/* the top, less the argument area the ABI lets a callee use */
	n64_call_on_stack(game_main, (uint8_t *)game_stack + GAME_STACK_SIZE - 64);
	return 0;
}
