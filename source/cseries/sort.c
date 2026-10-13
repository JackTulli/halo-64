/*
SORT.C

symbols in this file:
00080360 0060:
	_code_00080360 (0000)
000803C0 0050:
	_code_000803c0 (0000)
00080410 0150:
	_qsort_2byte (0000)
00080560 0140:
	_qsort_4byte (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "sort.h"

/* ---------- constants */

enum
{
	QSORT_STACK_SIZE = 30,
	QSORT_CUTOFF = 8
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void shortsort_2byte(
	word *hi,
	word *lo,
	boolean (*compare)(word, word));
static void shortsort_4byte(
	long *hi,
	long *lo,
	boolean (*compare)(long, long));

/* ---------- globals */

/* ---------- public code */

void qsort_2byte(
	word *elements,
	unsigned long element_count,
	boolean (*compare)(word, word))
{
	word *lo_stack[QSORT_STACK_SIZE];
	word *hi_stack[QSORT_STACK_SIZE];
	word *lo, *hi, *i, *j, *mid;
	long depth;
	unsigned long size;
	word temp;

	if (element_count<2)
		return;

	lo = elements;
	depth = 0;
	hi = elements+element_count-1;

	for (;;)
	{
		size = hi-lo+1;
		if (size<=QSORT_CUTOFF)
		{
			shortsort_2byte(hi, lo, compare);
		}
		else
		{
			mid = lo+(size>>1);
			temp = *mid;
			*mid = *lo;
			*lo = temp;

			i = lo;
			j = hi+1;

			for (;;)
			{
				do
				{
					i++;
				}
				while (i<=hi && !compare(*i, *lo));

				do
				{
					j--;
				}
				while (j>lo && compare(*j, *lo));

				if (j<i)
					break;

				temp = *i;
				*i = *j;
				*j = temp;
			}

			temp = *lo;
			*lo = *j;
			*j = temp;

			if ((char *)j-(char *)lo-1>=(char *)hi-(char *)i)
			{
				if (lo+1<j)
				{
					lo_stack[depth] = lo;
					hi_stack[depth] = j-1;
					depth++;
				}
				if (i<hi)
				{
					lo = i;
					continue;
				}
			}
			else
			{
				if (i<hi)
				{
					lo_stack[depth] = i;
					hi_stack[depth] = hi;
					depth++;
				}
				if (lo+1<j)
				{
					hi = j-1;
					continue;
				}
			}
		}

		if (--depth<0)
			break;

		lo = lo_stack[depth];
		hi = hi_stack[depth];
	}

	return;
}

void qsort_4byte(
	long *elements,
	unsigned long element_count,
	boolean (*compare)(long, long))
{
	long *lo_stack[QSORT_STACK_SIZE];
	long *hi_stack[QSORT_STACK_SIZE];
	long *lo, *hi, *i, *j, *mid;
	long depth;
	unsigned long size;
	long temp;

	if (element_count<2)
		return;

	lo = elements;
	depth = 0;
	hi = elements+element_count-1;

	for (;;)
	{
		size = hi-lo+1;
		if (size<=QSORT_CUTOFF)
		{
			shortsort_4byte(hi, lo, compare);
		}
		else
		{
			mid = lo+(size>>1);
			temp = *mid;
			*mid = *lo;
			*lo = temp;

			i = lo;
			j = hi+1;

			for (;;)
			{
				do
				{
					i++;
				}
				while (i<=hi && !compare(*i, *lo));

				do
				{
					j--;
				}
				while (j>lo && compare(*j, *lo));

				if (j<i)
					break;

				temp = *i;
				*i = *j;
				*j = temp;
			}

			temp = *lo;
			*lo = *j;
			*j = temp;

			if ((char *)j-(char *)lo-1>=(char *)hi-(char *)i)
			{
				if (lo+1<j)
				{
					lo_stack[depth] = lo;
					hi_stack[depth] = j-1;
					depth++;
				}
				if (i<hi)
				{
					lo = i;
					continue;
				}
			}
			else
			{
				if (i<hi)
				{
					lo_stack[depth] = i;
					hi_stack[depth] = hi;
					depth++;
				}
				if (lo+1<j)
				{
					hi = j-1;
					continue;
				}
			}
		}

		if (--depth<0)
			break;

		lo = lo_stack[depth];
		hi = hi_stack[depth];
	}

	return;
}

/* ---------- private code */

static void shortsort_2byte(
	word *hi,
	word *lo,
	boolean (*compare)(word, word))
{
	word *best, *scan;
	word temp;

	while (hi>lo)
	{
		best = lo;
		for (scan = lo+1; scan<=hi; scan++)
		{
			if (compare(*scan, *best))
				best = scan;
		}

		temp = *best;
		*best = *hi;
		*hi = temp;

		hi--;
	}

	return;
}

static void shortsort_4byte(
	long *hi,
	long *lo,
	boolean (*compare)(long, long))
{
	long *best, *scan;
	long temp;

	while (hi>lo)
	{
		best = lo;
		for (scan = lo+1; scan<=hi; scan++)
		{
			if (compare(*scan, *best))
				best = scan;
		}

		temp = *best;
		*best = *hi;
		*hi = temp;

		hi--;
	}

	return;
}
