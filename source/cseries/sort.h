/*
SORT.H
*/

#ifndef __SORT_H
#define __SORT_H
#pragma once

/* ---------- prototypes/SORT.C */

void qsort_2byte(
	word *elements,
	unsigned long element_count,
	boolean (*compare)(word, word));
void qsort_4byte(
	long *elements,
	unsigned long element_count,
	boolean (*compare)(long, long));

#endif // __SORT_H
