/*
64BIT_MATH.C

symbols in this file:
0006E5B0 00a0:
	_add64 (0000)
0006E650 0090:
	_code_0006e650 (0000)
0006E6E0 0060:
	_subtract64 (0000)
0006E740 0140:
	_multiply64 (0000)
0006E880 0120:
	_divide64 (0000)
002555F0 0011:
	??_C@_0BB@DKGDPHMB@a?5?$CG?$CG?5b?5?$CG?$CG?5result?$AA@ (0000)
00255604 002e:
	??_C@_0CO@LBKBEIL@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
00255634 000c:
	??_C@_0M@DAFMGHCE@a?5?$CG?$CG?5result?$AA@ (0000)
00255640 0019:
	??_C@_0BJ@DCPIPMGA@numerator?5?$CG?$CG?5denominator?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/64bit_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void negate64(
	struct qword_value const *a,
	struct qword_value *result);

/* ---------- globals */

/* ---------- public code */

void add64(
	struct qword_value const *a,
	struct qword_value const *b,
	struct qword_value *result)
{
	long carry;
	long value;
	long i;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\64bit_math.c", 33, a && b && result);

	carry = 0;
	for (i = 0; i < 4; i++)
	{
		value = a->words[i] + b->words[i] + carry;
		if (value > 0xFFFF)
			carry = 1;
		else
			carry = 0;
		result->words[i] = (word)value;
	}

	return;
}

void subtract64(
	struct qword_value const *a,
	struct qword_value const *b,
	struct qword_value *result)
{
	struct qword_value negative_b;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\64bit_math.c", 79, a && b && result);

	negate64(b, &negative_b);
	add64(a, &negative_b, result);

	return;
}

void multiply64(
	struct qword_value const *a,
	struct qword_value const *b,
	struct qword_value *result)
{
	unsigned long accumulator[7] = { 0, 1, 2, 3, 4, 5, 6 };
	unsigned long product;
	unsigned long i;
	unsigned long j;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\64bit_math.c", 95, a && b && result);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			product = a->words[i] * b->words[j];
			accumulator[i + j] += product & 0xFFFF;
			accumulator[i + j + 1] += product >> 16;
		}
	}

	result->words[0] = (word)accumulator[0];
	result->words[1] = (word)accumulator[1];
	result->words[2] = (word)accumulator[2];
	result->words[3] = (word)accumulator[3];

	return;
}

void divide64(
	struct qword_value const *numerator,
	struct qword_value const *denominator,
	struct qword_value *quotient,
	struct qword_value *remainder)
{
	word work[8];
	struct qword_value current;
	struct qword_value difference;
	unsigned long carry;
	unsigned long i;
	unsigned long bit;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\64bit_math.c", 124, numerator && denominator);

	for (i = 0; i < 4; i++)
	{
		work[i] = denominator->words[i];
		work[i + 4] = 0;
	}

	for (bit = 0; bit < 64; bit++)
	{
		carry = 0;
		for (i = 0; i < 8; i++)
		{
			carry += 2 * work[i];
			work[i] = (word)carry;
			carry >>= 16;
		}

		current = *(struct qword_value *)&work[4];
		subtract64(&current, numerator, &difference);
		if (!(difference.words[3] & 0x8000))
		{
			*(struct qword_value *)&work[4] = difference;
			work[0]++;
		}
	}

	if (quotient)
		*quotient = *(struct qword_value *)&work[0];
	if (remainder)
		*remainder = *(struct qword_value *)&work[4];

	return;
}

/* ---------- private code */

static void negate64(
	struct qword_value const *a,
	struct qword_value *result)
{
	unsigned long carry = 0;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\64bit_math.c", 58, a && result);

	result->words[0] = (word)-a->words[0];
	if (a->words[0])
		carry = 1;
	result->words[1] = (word)-(a->words[1] + carry);
	if (a->words[1])
		carry = 1;
	result->words[2] = (word)-(a->words[2] + carry);
	if (a->words[2])
		carry = 1;
	result->words[3] = (word)-(a->words[3] + carry);

	return;
}
