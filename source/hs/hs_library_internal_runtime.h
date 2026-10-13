/*
HS_LIBRARY_INTERNAL_RUNTIME.H

file has inline function assertions.
*/

#ifndef __HS_LIBRARY_INTERNAL_RUNTIME_H
#define __HS_LIBRARY_INTERNAL_RUNTIME_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/HS_RUNTIME.C */

void hs_evaluate_inequality(
	short function_index,
	long thread_index,
	boolean initialize);

void hs_evaluate_set(
	short function_index,
	long thread_index,
	boolean initialize);

/* ---------- globals */

/* ---------- public code */

#endif // __HS_LIBRARY_INTERNAL_RUNTIME_H
