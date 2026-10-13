/*
RANDOM_NUMBERS.H

header included in hcex build.
*/

#ifndef __RANDOM_NUMBERS_H
#define __RANDOM_NUMBERS_H
#pragma once

#include "bungie_net/common/64bit_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

long randomrange(long min, long max);
void randomrange64(
	struct qword_value const *min,
	struct qword_value const *max,
	struct qword_value *result);

#endif // __RANDOM_NUMBERS_H
