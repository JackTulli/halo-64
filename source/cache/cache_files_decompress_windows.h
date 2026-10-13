/*
CACHE_FILES_DECOMPRESS_WINDOWS.H

header included in hcex build.
*/

#ifndef __CACHE_FILES_DECOMPRESS_WINDOWS_H
#define __CACHE_FILES_DECOMPRESS_WINDOWS_H
#pragma once

/* ---------- constants */

enum cache_copy_status
{
	_cache_copy_bad_file_failure,
	_cache_copy_read_failure,
	_cache_copy_write_failure,
	_cache_copy_in_progress,
	_cache_copy_finished,
	NUMBER_OF_CACHE_COPY_STATES,
};

/* ---------- macros */

/* ---------- structures */

struct cache_copy_read_request;
struct simple_decompressor_definition;

/* ---------- prototypes/CACHE_FILES_DECOMPRESS_WINDOWS.C */

long cache_copy_buffer_size(
	boolean copy_map);
void cache_copy_set_priority(
	boolean blocking);
boolean cache_copy_compressed_file_complete(
	void);
void cache_copy_begin(
	void *buffer,
	long buffer_size,
	void *file,
	long size,
	char const *source_path);
void cache_copy_queue_end(
	void);
short cache_copy_get_status(
	real *progress);
void cache_copy_end(
	void);
void cache_copy_initialize(
	void);
struct cache_copy_read_request *acquire_read_request(
	struct simple_decompressor_definition *self,
	short read_sequence_index);

/* ---------- globals */

/* ---------- public code */

#endif // __CACHE_FILES_DECOMPRESS_WINDOWS_H
