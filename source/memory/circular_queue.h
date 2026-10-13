/*
CIRCULAR_QUEUE.H

header included in hcex build.
*/

#ifndef __CIRCULAR_QUEUE_H
#define __CIRCULAR_QUEUE_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct circular_queue
{
	char const *name;
	unsigned long signature;
	long read_offset;
	long write_offset;
	long buffer_size;
	byte *buffer;
};

/* ---------- prototypes/CIRCULAR_QUEUE.C */

void circular_queue_reset(
	struct circular_queue *queue);
struct circular_queue *circular_queue_new(
	char const *name,
	long buffer_size);
void circular_queue_delete(
	struct circular_queue *queue);
long circular_queue_size(
	struct circular_queue *queue);
long circular_queue_free_space(
	struct circular_queue *queue);
boolean circular_queue_queue_data(
	struct circular_queue *queue,
	void const *data,
	long data_size);
boolean circular_queue_dequeue_data(
	struct circular_queue *queue,
	void *data,
	long data_size,
	boolean advance);

/* ---------- globals */

/* ---------- public code */

#endif // __CIRCULAR_QUEUE_H
