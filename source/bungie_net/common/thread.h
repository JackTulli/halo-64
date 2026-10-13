/*
THREAD.H

header included in hcex build.
*/

#ifndef __THREAD_H
#define __THREAD_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct thread_reference;
struct mutex_reference;

/* ---------- prototypes/THREAD_WIN32.C */

boolean create_thread(
	word flags,
	unsigned long (__stdcall *function)(void *),
	void *function_input,
	struct thread_reference **thread_reference);
boolean thread_has_exited(
	struct thread_reference *thread_reference);
void dispose_thread(
	struct thread_reference *thread_reference);
boolean create_mutex(
	struct mutex_reference **mutex_reference);
boolean take_mutex(
	struct mutex_reference *mutex_reference,
	unsigned long timeout_ms);
void release_mutex(
	struct mutex_reference *mutex_reference);
void dispose_mutex(
	struct mutex_reference *mutex_reference);

/* ---------- globals */

/* ---------- public code */

#endif // __THREAD_H
