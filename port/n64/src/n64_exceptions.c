/*
N64_EXCEPTIONS.C

Where the VR4300 and the Xbox's x86 disagree about what is an error, the
game gets the x86's answer:

- A load or store not aligned to its size raises an address error here and
  works on x86; the game has a few (byte buffers read as longs). The access
  is emulated with byte loads and stores.
- The FPU cannot compute with NaNs or denormals: any such operand raises an
  "unimplemented operation" exception that cannot be masked. The game makes
  NaNs as x86 lets it (0/0 where a tag value is zero, e.g. units.c's aiming
  change) and expects them to flow on quietly. The instruction is emulated:
  a NaN operand gives a NaN, a denormal counts as zero, comparisons with a
  NaN are unordered.
- Converting a float that does not fit (infinity, NaN, beyond 32 bits) to an
  integer gives x86's "integer indefinite", 0x80000000.
- Division by zero, overflow and invalid operations simply produce IEEE
  results: the game runs with the FPU's exceptions disabled, as on the Xbox.

An instruction in a branch delay slot is handled too: the branch is
evaluated first (as the CPU would), then the instruction, then execution
continues at the branch's target.

Each place is reported once so the hot ones can be fixed in the source.
Anything else goes to libdragon's handler (the crash screen).
*/

#include <libdragon.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "n64.h"

#define MAXIMUM_REPORTED 64

static uint32_t reported[MAXIMUM_REPORTED];
static int reported_count;
/* the exceptions emulated so far (the render statistics print them: each
costs the CPU a trap and the decoding) */
unsigned long n64_exceptions_emulated;
unsigned long n64_emulated_count;

static void report(uint32_t pc, const char *what, uint32_t address)
{
	for (int i = 0; i < reported_count; i++)
	{
		if (reported[i] == pc)
			return;
	}
	if (reported_count < MAXIMUM_REPORTED)
		reported[reported_count++] = pc;
	debugf("emulated %s at pc %08lx (%08lx)\n", what, (unsigned long)pc, (unsigned long)address);
}

/* ---------- branches */

/* the address execution continues at after the branch at pc, whose delay
slot is being emulated; 0 for an instruction this does not know */
static uint32_t branch_target(reg_block_t *regs, uint32_t pc)
{
	uint32_t instruction = *(volatile uint32_t *)pc;
	uint32_t opcode = instruction >> 26;
	uint32_t rs = (instruction >> 21) & 31;
	uint32_t rt = (instruction >> 16) & 31;
	int64_t s = (int64_t)regs->gpr[rs];
	int64_t t = (int64_t)regs->gpr[rt];
	uint32_t taken = pc + 4 + ((int32_t)(int16_t)(instruction & 0xFFFF) << 2);
	uint32_t not_taken = pc + 8;

	switch (opcode)
	{
	case 0x00: /* SPECIAL */
		switch (instruction & 63)
		{
		case 0x08: /* jr */
			return (uint32_t)regs->gpr[rs];
		case 0x09: /* jalr */
		{
			uint32_t target = (uint32_t)regs->gpr[rs];
			uint32_t rd = (instruction >> 11) & 31;

			if (rd)
				regs->gpr[rd] = (int64_t)(int32_t)(pc + 8);
			return target;
		}
		}
		return 0;
	case 0x01: /* REGIMM */
		switch (rt)
		{
		case 0x00: case 0x02: return s < 0 ? taken : not_taken; /* bltz(l) */
		case 0x01: case 0x03: return s >= 0 ? taken : not_taken; /* bgez(l) */
		case 0x10: case 0x12: regs->ra = (int64_t)(int32_t)(pc + 8); return s < 0 ? taken : not_taken;
		case 0x11: case 0x13: regs->ra = (int64_t)(int32_t)(pc + 8); return s >= 0 ? taken : not_taken;
		}
		return 0;
	case 0x02: /* j */
		return ((pc + 4) & 0xF0000000) | ((instruction & 0x03FFFFFF) << 2);
	case 0x03: /* jal */
		regs->ra = (int64_t)(int32_t)(pc + 8);
		return ((pc + 4) & 0xF0000000) | ((instruction & 0x03FFFFFF) << 2);
	case 0x04: case 0x14: return s == t ? taken : not_taken; /* beq(l) */
	case 0x05: case 0x15: return s != t ? taken : not_taken; /* bne(l) */
	case 0x06: case 0x16: return s <= 0 ? taken : not_taken; /* blez(l) */
	case 0x07: case 0x17: return s > 0 ? taken : not_taken; /* bgtz(l) */
	case 0x11: /* COP1 */
		if (rs == 8)
		{
			int condition = (regs->fc31 >> 23) & 1;
			int on_true = rt & 1;

			return condition == on_true ? taken : not_taken;
		}
		return 0;
	}
	return 0;
}

/* ---------- unaligned loads and stores */

static uint64_t load(uint32_t address, int size)
{
	const volatile uint8_t *p = (const volatile uint8_t *)address;
	uint64_t value = 0;

	for (int i = 0; i < size; i++)
		value = (value << 8) | p[i];
	return value;
}

static void store(uint32_t address, int size, uint64_t value)
{
	volatile uint8_t *p = (volatile uint8_t *)address;

	for (int i = size - 1; i >= 0; i--)
	{
		p[i] = (uint8_t)value;
		value >>= 8;
	}
}

static int emulate_memory(reg_block_t *regs, uint32_t pc, uint32_t instruction)
{
	uint32_t opcode = instruction >> 26;
	uint32_t base = (instruction >> 21) & 31;
	uint32_t rt = (instruction >> 16) & 31;
	uint32_t address = (uint32_t)regs->gpr[base] + (int16_t)(instruction & 0xFFFF);

	switch (opcode)
	{
	case 0x21: if (rt) regs->gpr[rt] = (uint64_t)(int64_t)(int16_t)load(address, 2); break; /* lh */
	case 0x25: if (rt) regs->gpr[rt] = load(address, 2); break; /* lhu */
	case 0x23: if (rt) regs->gpr[rt] = (uint64_t)(int64_t)(int32_t)load(address, 4); break; /* lw */
	case 0x27: if (rt) regs->gpr[rt] = load(address, 4); break; /* lwu */
	case 0x37: if (rt) regs->gpr[rt] = load(address, 8); break; /* ld */
	case 0x29: store(address, 2, regs->gpr[rt]); break; /* sh */
	case 0x2B: store(address, 4, regs->gpr[rt]); break; /* sw */
	case 0x3F: store(address, 8, regs->gpr[rt]); break; /* sd */
	case 0x31: regs->fpr[rt] = (regs->fpr[rt] & 0xFFFFFFFF00000000ULL) | load(address, 4); break; /* lwc1 */
	case 0x39: store(address, 4, regs->fpr[rt]); break; /* swc1 */
	case 0x35: regs->fpr[rt] = load(address, 8); break; /* ldc1 */
	case 0x3D: store(address, 8, regs->fpr[rt]); break; /* sdc1 */
	default:
		return 0;
	}
	report(pc, "unaligned access", address);
	return 1;
}

/* ---------- floating point */

#define SINGLE_NAN 0x7FBFFFFFU
#define DOUBLE_NAN 0x7FF7FFFFFFFFFFFFULL

enum { FORMAT_S = 16, FORMAT_D = 17, FORMAT_W = 20, FORMAT_L = 21 };

/* an operand as a double: NaN stays NaN, denormals become (signed) zero */
static double operand(reg_block_t *regs, uint32_t index, uint32_t format, int *nan)
{
	if (format == FORMAT_S)
	{
		uint32_t bits = (uint32_t)regs->fpr[index];
		uint32_t exponent = (bits >> 23) & 0xFF;
		float value;

		if (exponent == 0xFF && (bits & 0x7FFFFF))
		{
			*nan = 1;
			return 0;
		}
		if (exponent == 0)
			bits &= 0x80000000;
		memcpy(&value, &bits, 4);
		return value;
	}
	else if (format == FORMAT_D)
	{
		uint64_t bits = regs->fpr[index];
		uint32_t exponent = (bits >> 52) & 0x7FF;
		double value;

		if (exponent == 0x7FF && (bits & 0xFFFFFFFFFFFFFULL))
		{
			*nan = 1;
			return 0;
		}
		if (exponent == 0)
			bits &= 0x8000000000000000ULL;
		memcpy(&value, &bits, 8);
		return value;
	}
	else if (format == FORMAT_W)
	{
		return (double)(int32_t)regs->fpr[index];
	}
	return (double)(int64_t)regs->fpr[index];
}

static void result(reg_block_t *regs, uint32_t index, uint32_t format, double value, int nan)
{
	if (format == FORMAT_S)
	{
		float single = (float)value;
		uint32_t bits;

		memcpy(&bits, &single, 4);
		if (nan)
			bits = SINGLE_NAN;
		else if (((bits >> 23) & 0xFF) == 0)
			bits &= 0x80000000;
		regs->fpr[index] = (regs->fpr[index] & 0xFFFFFFFF00000000ULL) | bits;
	}
	else
	{
		uint64_t bits;

		memcpy(&bits, &value, 8);
		if (nan)
			bits = DOUBLE_NAN;
		else if (((bits >> 52) & 0x7FF) == 0)
			bits &= 0x8000000000000000ULL;
		regs->fpr[index] = bits;
	}
}

static void integer_result(reg_block_t *regs, uint32_t index, int wide, double value, int nan, uint32_t rounding)
{
	double limit = wide ? 9223372036854775808.0 : 2147483648.0;

	if (!nan)
	{
		switch (rounding)
		{
		case 0: value = nearbyint(value); break; /* round (to even) */
		case 1: value = trunc(value); break;
		case 2: value = ceil(value); break;
		case 3: value = floor(value); break;
		}
	}
	if (nan || value >= limit || value < -limit)
	{
		regs->fpr[index] = wide ? 0x8000000000000000ULL
			: ((regs->fpr[index] & 0xFFFFFFFF00000000ULL) | 0x80000000ULL);
	}
	else if (wide)
	{
		regs->fpr[index] = (uint64_t)(int64_t)value;
	}
	else
	{
		regs->fpr[index] = (regs->fpr[index] & 0xFFFFFFFF00000000ULL) | (uint32_t)(int32_t)value;
	}
}

static int emulate_float(reg_block_t *regs, uint32_t pc, uint32_t instruction)
{
	uint32_t format = (instruction >> 21) & 31;
	uint32_t ft = (instruction >> 16) & 31;
	uint32_t fs = (instruction >> 11) & 31;
	uint32_t fd = (instruction >> 6) & 31;
	uint32_t function = instruction & 63;
	uint32_t rounding = (regs->fc31 & 3) == 0 ? 0 : (regs->fc31 & 3) == 1 ? 1 : (regs->fc31 & 3) == 2 ? 2 : 3;
	int nan = 0;
	double a, b;

	if ((instruction >> 26) != 0x11)
		return 0;
	if (format != FORMAT_S && format != FORMAT_D && format != FORMAT_W && format != FORMAT_L)
		return 0;
	a = operand(regs, fs, format, &nan);
	switch (function)
	{
	case 0x00: b = operand(regs, ft, format, &nan); result(regs, fd, format, a + b, nan); break;
	case 0x01: b = operand(regs, ft, format, &nan); result(regs, fd, format, a - b, nan); break;
	case 0x02: b = operand(regs, ft, format, &nan); result(regs, fd, format, a * b, nan); break;
	case 0x03:
		b = operand(regs, ft, format, &nan);
		if (b == 0 && a == 0)
			nan = 1;
		result(regs, fd, format, b == 0 ? (a < 0 ? -INFINITY : INFINITY) * (signbit(b) ? -1 : 1) : a / b, nan);
		break;
	case 0x04: result(regs, fd, format, a < 0 ? 0 : sqrt(a), nan || a < 0); break;
	case 0x05: result(regs, fd, format, fabs(a), nan); break;
	case 0x06: result(regs, fd, format, a, nan); break;
	case 0x07: result(regs, fd, format, -a, nan); break;
	case 0x08: case 0x09: case 0x0A: case 0x0B: integer_result(regs, fd, 1, a, nan, function - 0x08); break;
	case 0x0C: case 0x0D: case 0x0E: case 0x0F: integer_result(regs, fd, 0, a, nan, function - 0x0C); break;
	case 0x20: result(regs, fd, FORMAT_S, a, nan); break;
	case 0x21: result(regs, fd, FORMAT_D, a, nan); break;
	case 0x24: integer_result(regs, fd, 0, a, nan, rounding); break;
	case 0x25: integer_result(regs, fd, 1, a, nan, rounding); break;
	default:
		if (function >= 0x30)
		{
			/* c.cond: bit 0 unordered, bit 1 equal, bit 2 less */
			int condition;

			b = operand(regs, ft, format, &nan);
			if (nan)
				condition = function & 1;
			else
				condition = ((function & 2) && a == b) || ((function & 4) && a < b);
			if (condition)
				regs->fc31 |= 1 << 23;
			else
				regs->fc31 &= ~(1U << 23);
			break;
		}
		return 0;
	}
	report(pc, nan ? "float NaN operation" : "float operation", instruction);
	return 1;
}

/* ---------- the handler */

static void exception_handler(exception_t *exception)
{
	reg_block_t *regs = exception->regs;
	int in_delay_slot = (regs->cr & 0x80000000) != 0;
	uint32_t pc = regs->epc + (in_delay_slot ? 4 : 0);
	uint32_t next = pc + 4;
	uint32_t instruction;
	int handled = 0;
	static int traced;

	n64_exceptions_emulated++;
	if (traced < 24)
	{
		traced++;
		debugf("exception %d at %08lx bd=%d fc31=%08lx\n", (int)exception->code, (unsigned long)regs->epc,
			in_delay_slot, (unsigned long)regs->fc31);
	}
	if (exception->code != EXCEPTION_CODE_LOAD_I_ADDRESS_ERROR &&
		exception->code != EXCEPTION_CODE_STORE_ADDRESS_ERROR &&
		!(exception->code == EXCEPTION_CODE_FLOATING_POINT && (regs->fc31 & C1_CAUSE_NOT_IMPLEMENTED)))
	{
		exception_default_handler(exception);
		return;
	}
	/* an instruction fetch from a bad address is not ours to fix */
	if (exception->code == EXCEPTION_CODE_LOAD_I_ADDRESS_ERROR && (regs->epc & 3))
	{
		exception_default_handler(exception);
		return;
	}
	if (in_delay_slot)
	{
		next = branch_target(regs, regs->epc);
		if (!next)
		{
			exception_default_handler(exception);
			return;
		}
	}
	instruction = *(volatile uint32_t *)pc;
	if (exception->code == EXCEPTION_CODE_FLOATING_POINT)
		handled = emulate_float(regs, pc, instruction);
	else
		handled = emulate_memory(regs, pc, instruction);
	if (!handled)
	{
		exception_default_handler(exception);
		return;
	}
	regs->fc31 &= ~C1_CAUSE_MASK;
	regs->epc = next;
	n64_emulated_count++;
}

void n64_exceptions_install(void)
{
	register_exception_handler(exception_handler);
	/* the Xbox's floating point: no traps (division by zero gives infinity),
	denormal results flushed to zero */
	C1_WRITE_FCR31((C1_FCR31() & ~(C1_ENABLE_MASK | C1_CAUSE_MASK)) | C1_FCR31_FS);
}
