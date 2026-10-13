/*
STACK_MEMORY_POOL.H

header included in hcex build.
*/

#ifndef __STACK_MEMORY_POOL_H
#define __STACK_MEMORY_POOL_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct stack_memory_pool;

/* ---------- prototypes/STACK_MEMORY_POOL.C */

void stack_memory_pool_reset(
	struct stack_memory_pool *pool);
void *pool_new_pointer(
	struct stack_memory_pool *pool,
	long allocation_size,
	char const *file,
	unsigned long line);
void dispose_pointer(
	struct stack_memory_pool *pool,
	void *pointer);
void *pool_resize_pointer(
	struct stack_memory_pool *pool,
	void *pointer,
	long allocation_size,
	char const *file,
	unsigned long line);

/* ---------- globals */

/* ---------- public code */

#endif // __STACK_MEMORY_POOL_H
