/*
N64_MAIN.C

The N64 build's entry point and system services (port/n64/src/n64.h):
libdragon's start-up, the debug log, time, and the jump into the game's own
main (source/shell/shell_xbox.c, compiled as halo_shell_main).
*/

#include <libdragon.h>
#include <stdarg.h>
#include <stdio.h>
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
	debug_init_usblog();
	n64_exceptions_install();
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();
	debugf("halo-n64: %d KB of RAM\n", (int)(get_memory_size() / 1024));
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
