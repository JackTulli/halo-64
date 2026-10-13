/*
CACHE_FILES.H

header included in hcex build.
*/

#ifndef __CACHE_FILES_H
#define __CACHE_FILES_H
#pragma once

/* ---------- headers */

#include "real_math.h"

/* ---------- constants */

enum cached_map_file_precache_state
{
	_cached_map_file_in_progress,
	_cached_map_file_success,
	_cached_map_file_failed,
	NUMBER_OF_CACHED_MAP_FILE_PRECACHE_STATES,
};

/* ---------- macros */

/* ---------- structures */

struct tag_block;
struct tag_data;
struct tag_reference;
struct bitmap_data;
struct cache_file_header;
struct cache_file_tag_header;
struct cache_file_structure_bsp_header;

struct tag_iterator
{
	byte reserved0[4];
	short absolute_index;
	byte reserved6[10];
	long group_tag;
};

/* ---------- prototypes/CACHE_FILES.C */

const char *cache_files_map_directory(
	void);

boolean cache_file_header_verify(
	struct cache_file_header *header,
	char const *scenario_name,
	boolean fatal);

unsigned long cache_files_get_checksum(
	void);
unsigned long tag_groups_checksum(
	void);
void cache_files_enable_writes(
	void);
void cache_files_disable_writes(
	void);
void scenario_structure_bsp_unload(
	struct scenario_structure_bsp_reference *reference);
void scenario_tags_unload(
	void);
long scenario_tags_load(
	char const *scenario_name);
boolean scenario_structure_bsp_load(
	struct scenario_structure_bsp_reference *reference);
void tag_files_close(
	void);
void tag_files_open(
	void);
boolean tag_block_resize(
	struct tag_block *block,
	long count);
boolean tag_data_resize(
	struct tag_data *data,
	long size);
long tag_block_add_element(
	struct tag_block *block);
void tag_block_delete_element(
	struct tag_block *block,
	long element_index);
long tag_load(
	long group_tag,
	char const *name,
	unsigned long flags);
void tag_unload(
	long tag_index);
void tag_file_get_path(
	long group_tag,
	char const *name,
	char *path);
void tag_reference_set(
	struct tag_reference *reference,
	unsigned long group_tag,
	char const *name);
void tag_iterator_new(
	struct tag_iterator *iterator,
	long group_tag);
long tag_iterator_next(
	struct tag_iterator *iterator);

/* ---------- prototypes/CACHE_FILES_WINDOWS.C */

void tags_header_register_vertex_and_index_buffers(
	struct cache_file_tag_header *header);
void tags_header_deregister_vertex_and_index_buffers(
	struct cache_file_tag_header *header);
void structure_bsp_header_register_vertex_buffers(
	struct cache_file_structure_bsp_header *header);
void structure_bsp_header_deregister_vertex_buffers(
	struct cache_file_structure_bsp_header *header);
void cache_files_initialize(
	void);
void cache_files_dispose(
	void);
void cache_files_precache_set_priority(
	boolean blocking);

boolean cache_files_precache_in_progress(
	void);

boolean cache_files_precache_is_copying_map(
	char const *map_name);
boolean cache_files_precache_map_loaded(
	char const *map_name);

short cache_files_precache_map_status(
	real *progress);

void cache_files_precache_map_end(
	void);
void cache_files_precache_map_queue_end(
	void);

boolean cache_files_precache_map_begin(
	char const *map_name,
	boolean copy_map);

void cache_file_promote_read(
	short request_index);
void cache_file_block_until_not_busy(
	void);
void cache_file_close(
	void);
boolean cache_file_open(
	char const *scenario_name,
	struct cache_file_header *header);
short cache_file_read(
	long tag_index,
	long offset,
	long size,
	void *buffer,
	boolean *completion_flag_reference,
	boolean blocking);

unsigned long tag_get_group_tag(long tag_index);

/* ---------- prototypes/XBOX_TEXTURE_CACHE.C */

void texture_cache_bitmap_new(
	long bitmap_group_index,
	struct bitmap_data *bitmap);

/* ---------- globals */

/* ---------- public code */

#endif // __CACHE_FILES_H
