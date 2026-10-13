/*
PRIME_NUMBERS.H

header included in hcex build.
*/

#ifndef __PRIME_NUMBERS_H
#define __PRIME_NUMBERS_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct qword_value;

/* ---------- prototypes/PRIME_NUMBERS.C */

unsigned long randomprime(
	unsigned long maximum);

unsigned long *primegen(
	unsigned long maximum,
	unsigned long *num_primes);

void probable_prime64(
	struct qword_value *result);

/* ---------- globals */

/* ---------- public code */

#endif // __PRIME_NUMBERS_H
