/*
HS_LIBRARY_INTERNAL_COMPILE.H

file has inline function assertions.
*/

#ifndef __HS_LIBRARY_INTERNAL_COMPILE_H
#define __HS_LIBRARY_INTERNAL_COMPILE_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/HS_COMPILE.C */

void hs_compile_initialize(
	boolean compiling_scenario);
void hs_compile_dispose(
	void);
long hs_compile_expression(
	long source_size,
	char const *source,
	char const **error_message,
	char const **error_source);

boolean hs_compile_postprocess(
	char const **error_message,
	char const **error_source);
boolean hs_parse_sleep(
	short function_index,
	long expression_index);

boolean hs_macro_function_parse(
	short function_index,
	long expression_index);
boolean hs_parse_if(
	short function_index,
	long expression_index);
boolean hs_parse_begin(
	short function_index,
	long expression_index);
boolean hs_parse_logical(
	short function_index,
	long expression_index);
boolean hs_parse_arithmetic(
	short function_index,
	long expression_index);
boolean hs_parse_equality(
	short function_index,
	long expression_index);
boolean hs_parse_inequality(
	short function_index,
	long expression_index);
boolean hs_parse_cond(
	short function_index,
	long expression_index);
boolean hs_parse_set(
	short function_index,
	long expression_index);
boolean hs_parse_sleep_until(
	short function_index,
	long expression_index);
boolean hs_parse_wake(
	short function_index,
	long expression_index);
boolean hs_parse_object_cast_up(
	short function_index,
	long expression_index);
boolean hs_parse_debug_string(
	short function_index,
	long expression_index);
boolean hs_parse_inspect(
	short function_index,
	long expression_index);

/* ---------- globals */

/* ---------- public code */

#endif // __HS_LIBRARY_INTERNAL_COMPILE_H
