/*
RANDOM_NUMBERS.C

symbols in this file:
0006FA80 0070:
	_randomrange (0000)
0006FAF0 0140:
	_randomrange64 (0000)
00255A60 0008:
	__real@40dfffc000000000 (0000)
00255A68 001c:
	??_C@_0BM@GMOFAALD@result?9?$DOqword?5?$DM?$DN?5max?9?$DOqword?$AA@ (0000)
00255A84 001c:
	??_C@_0BM@EBIGLPBB@result?9?$DOqword?5?$DO?$DN?5min?9?$DOqword?$AA@ (0000)
00255AA0 0015:
	??_C@_0BF@IIHFCJPI@min?5?$CG?$CG?5max?5?$CG?$CG?5result?$AA@ (0000)
00255AB8 0032:
	??_C@_0DC@MOOKIGOF@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
0031C720 0001:
	_bss_0031c720 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/random_numbers.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- globals */

boolean random_numbers_initialized = FALSE;

/* ---------- public code */

long randomrange(long min, long max)
{
	if (!random_numbers_initialized)
	{
		srand(time(NULL));
		random_numbers_initialized= TRUE;
	}
	return (long)((double)rand()*(unsigned long)max/((unsigned long)min+32767.0))+min;
}

void
randomrange64(
	struct qword_value const *min,
	struct qword_value const *max,
	struct qword_value *result)
{
	struct qword_value offset;

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\random_numbers.c", 46, min && max && result);

	if (!random_numbers_initialized)
	{
		srand(time(NULL));
		random_numbers_initialized= TRUE;
	}

	offset.qword = (unsigned __int64)((double)rand() * max->qword / (min->qword + 32767.0));
	add64(min, &offset, result);

	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\random_numbers.c", 58, result->qword >= min->qword);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\random_numbers.c", 59, result->qword <= max->qword);
}

/* ---------- private code */
