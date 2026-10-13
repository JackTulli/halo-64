/*
PRIME_NUMBERS.C

symbols in this file:
0006F3A0 0020:
	_compare_ulongs_descending (0000)
0006F3C0 0160:
	_primegen (0000)
0006F520 0050:
	_randomprime (0000)
0006F570 00c0:
	_probable_prime64 (0000)
002558F0 0008:
	__real@41f0000000000000 (0000)
002558F8 000b:
	??_C@_0L@BKHCFPJF@num_primes?$AA@ (0000)
00255904 0031:
	??_C@_0DB@FDHHNPGN@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/64bit_math.h"
#include "bungie_net/common/prime_numbers.h"
#include "bungie_net/common/random_numbers.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

unsigned long randomprime(
	unsigned long maximum)
{
	unsigned long result = 0;
	unsigned long prime_count;
	unsigned long *primes = primegen(maximum, &prime_count);

	if (primes)
	{
		result = primes[randomrange(0, prime_count - 1)];
		match_free("c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c", 137, primes);
	}

	return result;
}

/* ---------- private code */

static int compare_ulongs_descending(
	void const *left,
	void const *right)
{
	unsigned long left_value = *(unsigned long const *)left;
	unsigned long right_value = *(unsigned long const *)right;

	if (left_value < right_value)
		return 1;

	return left_value > right_value ? -1 : 0;
}

unsigned long *primegen(
	unsigned long maximum,
	unsigned long *num_primes)
{
	unsigned long *primes = NULL;
	unsigned long odd_count = (maximum & 1) ? (maximum >> 1) : ((maximum >> 1) - 1);
	unsigned long scan_count = 0;
	unsigned long i;
	unsigned long j;
	unsigned long sqrt_max;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c", 61, num_primes);

	if (maximum < 2)
	{
		*num_primes = 0;

		return NULL;
	}

	*num_primes = odd_count + 1;
	primes = match_malloc(
		"c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c",
		71,
		odd_count * sizeof(*primes) + sizeof(*primes));

	if (primes)
	{
		i = 0;
		j = 3;
		sqrt_max = (unsigned long)sqrt(maximum);

		while (i < odd_count)
		{
			primes[i] = j;
			i++;
			j += 2;
		}

		while (scan_count < odd_count)
		{
			if (primes[scan_count] > sqrt_max)
				break;

			scan_count++;
		}

		i = 0;
		while (i < scan_count)
		{
			if (primes[i])
			{
				j = i + 1;
				while (j < odd_count)
				{
					if (primes[j] && !(primes[j] % primes[i]))
					{
						primes[j] = 0;
						(*num_primes)--;
					}

					j++;
				}
			}

			i++;
		}

		primes[odd_count] = 2;
		qsort(primes, odd_count + 1, sizeof(*primes), compare_ulongs_descending);

		if (*num_primes < odd_count + 1)
		{
			primes = match_realloc(
				"c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c",
				117,
				primes,
				*num_primes * sizeof(*primes));
		}
	}

	return primes;
}

void probable_prime64(
	struct qword_value *result)
{
	unsigned long prime_count;
	unsigned long *primes;
	unsigned long prime;
	unsigned long iteration_count;
	struct qword_value two;
	struct qword_value prime_qword;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c", 150, result);

	result->qword = 1;
	two.qword = 2;
	iteration_count = 4;

	do
	{
		prime = 0;
		primes = primegen(0xFFFF, &prime_count);

		if (primes)
		{
			prime = primes[randomrange(0, prime_count - 1)];
			match_free("c:\\halo\\SOURCE\\bungie_net\\common\\prime_numbers.c", 137, primes);
		}

		prime_qword.qword = prime;
		multiply64(result, &prime_qword, result);
	}
	while (--iteration_count);

	add64(result, &two, result);

	return;
}
