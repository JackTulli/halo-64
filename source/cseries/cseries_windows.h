/*
CSERIES_WINDOWS.H
*/

#ifndef __CSERIES_WINDOWS_H
#define __CSERIES_WINDOWS_H
#pragma once

/* ---------- includes */

#define DEBUG_KEYBOARD
#include <xtl.h>
#include <xbdm.h>

/* ---------- prototypes/CSERIES_WINDOWS.C */

void display_debug_string(
	const char *string);
void system_unique_identifier_get(
	void *identifier);
long system_unique_identifiers_equal(
	const void *identifier1,
	const void *identifier2);
unsigned long system_milliseconds(
	void);
unsigned long system_seconds(
	void);
void system_get_user_name(
	char *user_name,
	short maximum_length);
void *system_calloc(
	long count,
	long size);
void *system_malloc(
	long size);
void system_free(
	void *pointer);
void *system_realloc(
	void *pointer,
	long size);
unsigned long system_get_used_memory_size(
	void *pointer);
struct system_memory_information;
void system_memory_information_get(
	struct system_memory_information *information);
void system_show_wait_cursor(
	const char *file,
	long line);
void system_alert(
	void);
void system_kill_screen_saver(
	void);

long generic_exception_filter(
	unsigned long exception_code,
	PEXCEPTION_POINTERS exception_information);

/* ---------- prototypes/STACK_WALK_WINDOWS.C */

long stack_walk_global_function_offset(
	void);
void stack_walk_disregard_symbol_names(
	boolean disregard);
void stack_walk_initialize(
	void);
void stack_walk_dispose(
	void);
void stack_walk(
	short levels_to_ignore);
void stack_walk_with_context(
	FILE *error_stream,
	short levels_to_ignore,
	CONTEXT *context_pointer);

#endif // __CSERIES_WINDOWS_H
