/*
SAVED_GAME_FILES.H

header included in hcex build.
*/

#ifndef __SAVED_GAME_FILES_H
#define __SAVED_GAME_FILES_H
#pragma once

#include "cseries/cseries.h"

/* ---------- constants */

enum
{
	SAVED_GAME_FILE_BLOCK_SIZE = 512
};

enum
{
	_saved_game_file_index_read_only_bit = 30,
	_saved_game_file_index_valid_bit
};

enum
{
	_saved_game_file_type_player_profile = 0,
	_saved_game_file_type_game_variant,
	NUMBER_OF_SAVED_GAME_FILE_TYPES
};

/* ---------- macros */

/* ---------- structures */

struct file_reference;
struct _XCALCSIG_SIGNATURE;

/* ---------- prototypes/SAVED_GAME_FILES.C */

void saved_game_files_initialize(
	void);
void saved_game_files_dispose(
	void);
boolean saved_game_file_close(
	struct file_reference *saved_game_file,
	long profile_index);
void saved_game_files_notify_memory_units_changed(
	void);
boolean saved_game_files_take_mutex(
	void);
void saved_game_files_release_mutex(
	void);
short saved_game_perform_file_system_checks(
	void);
boolean saved_game_file_name_unique(
	wchar_t const *name);
void saved_game_file_remember_player1_last_used_profile_directory(
	char const *directory_path);
boolean saved_game_file_retrieve_player1_last_used_profile_directory(
	char *directory_path);
void saved_game_file_remember_last_used_multiplayer_variant_directory(
	char const *directory_path);
boolean saved_game_file_retrieve_last_used_multiplayer_variant_directory(
	char *directory_path);
void saved_game_file_remember_last_used_multiplayer_map(
	char const *map_name);
boolean saved_game_file_retrieve_last_used_multiplayer_map(
	char *map_name);
void saved_game_file_generate_checksum(
	void const *buffer,
	word buffer_size,
	struct _XCALCSIG_SIGNATURE *checksum);
wchar_t *saved_game_file_get_display_name(
	long profile_index);
boolean saved_game_file_open(
	struct file_reference *saved_game_file,
	long profile_index);
boolean synchronize_metadata_display_name_with_profile_name(
	long profile_index,
	wchar_t *game_display_name);
boolean saved_game_file_get_path_to_enclosing_directory(
	long profile_index,
	char *directory_path);
long create_enumerated_saved_game_file(
	word saved_game_file_type,
	short local_player_index,
	wchar_t *display_name);
boolean delete_enumerated_saved_game_file(
	long profile_index);
void saved_game_file_get_useable_untitled_profile_name(
	wchar_t *display_name);
void saved_game_files_enumerate_available_to_local_player_index(
	short player_index,
	word saved_game_file_type,
	word *number_of_profiles,
	long *player_profile_indices,
	boolean include_default_profiles);
long saved_game_file_find_profile_index_for_directory_path(
	char const *directory_path,
	short type);
void saved_game_files_delete_all_custom_profiles(
	void);
word saved_game_file_get_type(
	long profile_index);
void enumerate_memory_units_test(
	void);

/* ---------- globals */

/* ---------- public code */

#endif // __SAVED_GAME_FILES_H
