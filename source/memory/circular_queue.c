/*
CIRCULAR_QUEUE.C

symbols in this file:
00108580 0010:
	_circular_queue_reset (0000)
00108590 0070:
	_circular_queue_verify (0000)
00108600 0060:
	_circular_queue_new (0000)
00108660 0030:
	_circular_queue_delete (0000)
00108690 0020:
	_circular_queue_size (0000)
001086B0 0030:
	_circular_queue_free_space (0000)
001086E0 00f0:
	_circular_queue_queue_data (0000)
001087D0 0100:
	_circular_queue_dequeue_data (0000)
0027D1DC 002e:
	??_C@_0CO@NAJBHEFM@the?5circular?5queue?5?$EA?$CFp?5appears?5t@ (0000)
0027D20C 0027:
	??_C@_0CH@CMCOODHF@c?3?2halo?2SOURCE?2memory?2circular_q@ (0000)
0027D238 0041:
	??_C@_0EB@KGLMCPKK@queue?9?$DOwrite_offset?$DO?$DN0?5?$CG?$CG?5queue?9@ (0000)
0027D27C 0034:
	??_C@_0DE@GEMEIDKC@data?5?$CG?$CG?5data_size?$DO0?5?$CG?$CG?5data_size@ (0000)
0027D2B0 0031:
	??_C@_0DB@LGMLFMAL@read_offset?$DO?$DN0?5?$CG?$CG?5read_offset?$DMqu@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"

#include "memory/circular_queue.h"

/* ---------- constants */

enum
{
	CIRCULAR_QUEUE_SIGNATURE = 'circ',
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void circular_queue_verify(
	struct circular_queue *queue);

/* ---------- globals */

/* ---------- public code */

void circular_queue_reset(
	struct circular_queue *queue)
{
	queue->write_offset = 0;
	queue->read_offset = 0;
	return;
}

struct circular_queue *circular_queue_new(
	char const *name,
	long buffer_size)
{
	struct circular_queue *queue = match_malloc("c:\\halo\\SOURCE\\memory\\circular_queue.c", 52, sizeof(*queue) + buffer_size + 1);

	if (queue)
	{
		csmemset(queue, 0, sizeof(*queue));
		queue->name = name;
		queue->signature = CIRCULAR_QUEUE_SIGNATURE;
		queue->buffer_size = buffer_size + 1;
		queue->buffer = (byte *)(queue + 1);
		circular_queue_verify(queue);
	}

	return queue;
}

void circular_queue_delete(
	struct circular_queue *queue)
{
	circular_queue_verify(queue);
	match_free("c:\\halo\\SOURCE\\memory\\circular_queue.c", 72, queue);
	return;
}

long circular_queue_size(
	struct circular_queue *queue)
{
	long size;

	circular_queue_verify(queue);
	size = queue->write_offset - queue->read_offset;
	if (size < 0)
		size += queue->buffer_size;

	return size;
}

long circular_queue_free_space(
	struct circular_queue *queue)
{
	long size;

	circular_queue_verify(queue);
	size = queue->write_offset - queue->read_offset;
	if (size < 0)
		size += queue->buffer_size;

	return queue->buffer_size - size - 1;
}

boolean circular_queue_queue_data(
	struct circular_queue *queue,
	void const *data,
	long data_size)
{
	long write_offset;
	long size;
	long contiguous_size;

	circular_queue_verify(queue);
	match_assert("c:\\halo\\SOURCE\\memory\\circular_queue.c", 116, data && data_size>0 && data_size<queue->buffer_size);
	circular_queue_verify(queue);

	write_offset = queue->write_offset;
	size = write_offset - queue->read_offset;
	if (size < 0)
		size += queue->buffer_size;

	if (size + data_size < queue->buffer_size)
	{
		contiguous_size = queue->buffer_size - write_offset;
		if (data_size >= contiguous_size)
		{
			csmemcpy(queue->buffer + write_offset, data, contiguous_size);
			queue->write_offset = 0;
			data = (byte const *)data + contiguous_size;
			data_size -= contiguous_size;
		}

		if (data_size > 0)
		{
			csmemcpy(queue->buffer + queue->write_offset, data, data_size);
			queue->write_offset += data_size;
		}

		match_assert("c:\\halo\\SOURCE\\memory\\circular_queue.c", 136, queue->write_offset>=0 && queue->write_offset<queue->buffer_size);
		return TRUE;
	}

	return FALSE;
}

boolean circular_queue_dequeue_data(
	struct circular_queue *queue,
	void *data,
	long data_size,
	boolean advance)
{
	boolean result = FALSE;

	circular_queue_verify(queue);
	match_assert("c:\\halo\\SOURCE\\memory\\circular_queue.c", 153, data && data_size>0 && data_size<queue->buffer_size);

	if (data_size <= circular_queue_size(queue))
	{
		long contiguous_size = queue->buffer_size - queue->read_offset;
		long read_offset = queue->read_offset;

		if (data_size >= contiguous_size)
		{
			csmemcpy(data, queue->buffer + read_offset, contiguous_size);
			read_offset = 0;
			data = (byte *)data + contiguous_size;
			data_size -= contiguous_size;
		}

		if (data_size > 0)
		{
			csmemcpy(data, queue->buffer + read_offset, data_size);
			read_offset += data_size;
		}

		match_assert("c:\\halo\\SOURCE\\memory\\circular_queue.c", 174, read_offset>=0 && read_offset<queue->buffer_size);
		if (advance)
			queue->read_offset = read_offset;
		result = TRUE;
	}

	return result;
}

/* ---------- private code */

static void circular_queue_verify(
	struct circular_queue *queue)
{
	if (!queue ||
		queue->signature != CIRCULAR_QUEUE_SIGNATURE ||
		!queue->buffer ||
		queue->buffer_size <= 0 ||
		queue->read_offset < 0 ||
		queue->read_offset >= queue->buffer_size ||
		queue->write_offset < 0 ||
		queue->write_offset >= queue->buffer_size)
	{
		display_assert(
			csprintf(temporary, "the circular queue @%p appears to be corrupt.", queue),
			"c:\\halo\\SOURCE\\memory\\circular_queue.c",
			204,
			TRUE);
		system_exit(-1);
	}

	return;
}
