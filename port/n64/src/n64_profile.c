/*
N64_PROFILE.C

A sampling profiler for development (cartridge flag "profile"): the
vertical blank interrupt records where the CPU was, 60 times a second, from
20 to 140 seconds after boot, then the samples go to the debug output as
hex ("prof: ..." lines) for tools/n64_profile.py to attribute to functions.
(A fast timer interrupt corrupted callee-saved registers in ares, so the
display's own interrupt does the sampling.) With the flag "profile_zone"
as well, only the CPU's time inside a zone (n64_profile_zone, e.g. the HUD)
is sampled, each sample with its return address ("prof: pc ra" pairs).
*/

#include <libdragon.h>
#include <stdint.h>

#include "n64.h"

#define SAMPLE_COUNT 8000
#define START_SECONDS 20
#define STOP_SECONDS 80

extern void *interrupt_exception_frame;

static uint32_t *samples;
static volatile int sample_count;
static volatile int dumped;
static int zone_only;
/* "profile_stack": each sample is STACK_WORDS words (pc, ra, callers) */
#define STACK_WORDS 12
static int stack_chain;
static uint32_t text_end;
extern char __text_end[];
volatile int n64_profile_zone;

static void sample(void)
{
	uint8_t *frame = interrupt_exception_frame;
	unsigned long long now = get_ticks_us();

	if (!frame || now < START_SECONDS * 1000000ULL || sample_count >= SAMPLE_COUNT)
		return;
	if (now > STOP_SECONDS * 1000000ULL)
		return;
	if (zone_only && (!n64_profile_zone || sample_count + 2 > SAMPLE_COUNT))
		return;
	if (stack_chain && sample_count + STACK_WORDS > SAMPLE_COUNT)
		return;
	/* the interrupted context's EPC (reg_block_t, 32 bytes above sp) */
	samples[sample_count++] = *(uint32_t *)(frame + 32 + 280);
	/* and its return address (the low word of gpr[31]) */
	if (zone_only || stack_chain)
		samples[sample_count++] = *(uint32_t *)(frame + 32 + 31 * 8 + 4);
	/* ("profile_stack": the first words on its stack that look like
	return addresses into the code, the callers further up, roughly) */
	if (stack_chain)
	{
		uint32_t sp = *(uint32_t *)(frame + 32 + 29 * 8 + 4);
		int found = 0;

		for (int i = 0; i < 512 && found < STACK_WORDS - 2 && (sp & 3) == 0 &&
			sp >= 0x80000400 && sp < 0x80800000 - 4 * 512; i++)
		{
			uint32_t word = ((uint32_t *)sp)[i];

			/* (a recursion's frames return to one place: once) */
			if (word >= 0x80000400 && word < text_end && (word & 3) == 0 &&
				(!found || samples[sample_count + found - 1] != word))
				samples[sample_count + found++] = word;
		}
		while (found < STACK_WORDS - 2)
			samples[sample_count + found++] = 0;
		sample_count += STACK_WORDS - 2;
	}
}

void n64_profile_start(void)
{
	if (!n64_debug_flag("profile"))
		return;
	zone_only = n64_debug_flag("profile_zone");
	stack_chain = n64_debug_flag("profile_stack");
	text_end = (uint32_t)__text_end;
	samples = malloc(SAMPLE_COUNT * sizeof(uint32_t));
	if (!samples)
		return;
	register_VI_handler(sample);
	debugf("prof: sampling from %d s to %d s\n", START_SECONDS, STOP_SECONDS);
}

/* called once per frame: dumps the samples when the window has closed */
void n64_profile_poll(void)
{
	char line[8 * 9 + 8];

	if (!samples || dumped || get_ticks_us() < (STOP_SECONDS + 1) * 1000000ULL)
		return;
	dumped = 1;
	debugf("prof: %d samples\n", sample_count);
	for (int i = 0; i < sample_count; i += 8)
	{
		int n = 0;

		for (int j = i; j < i + 8 && j < sample_count; j++)
			n += snprintf(line + n, sizeof(line) - n, "%08lx ", (unsigned long)samples[j]);
		debugf("prof: %s\n", line);
	}
	debugf("prof: end\n");
}
