/*
PLAYLIST_PROFILE.C

symbols in this file:
001B1890 0020:
	_playlist_profiles_initialize (0000)
001B18B0 0060:
	_playlist_profiles_dispose (0000)
001B1910 0150:
	_playlist_profile_new (0000)
001B1A60 0030:
	_playlist_profile_delete (0000)
001B1A90 0130:
	_playlist_profile_get_from_path (0000)
001B1BC0 0040:
	_playlist_profile_get_display_name (0000)
001B1C00 0010:
	_playlist_profile_number_of_default_profiles_on_disk (0000)
001B1C10 01c0:
	_playlist_profile_create_default_profiles_on_disk (0000)
001B1DD0 0270:
	_playlist_profile_read (0000)
001B2040 0160:
	_playlist_profile_write_thread_proc@4 (0000)
001B21A0 0040:
	_playlist_profiles_enumerate_available_to_local_player_index (0000)
001B21E0 0060:
	_playlist_profile_get (0000)
001B2240 00a0:
	_playlist_profile_write (0000)
001B22E0 0050:
	_playlist_profile_save (0000)
002A89E8 003e:
	??_C@_0DO@CHOMLMPM@waiting?5for?5asynchronous?5playlis@ (0000)
002A8A28 002e:
	??_C@_0CO@NLNDKCIB@failed?5to?5open?5newly?5created?5pla@ (0000)
002A8A58 0034:
	??_C@_0DE@JJMPHFIN@failed?5to?5initialize?5newly?5creat@ (0000)
002A8A8C 0039:
	??_C@_0DJ@GIOFGNJD@playlist_profile_delete?$CI?$CJ?5failed@ (0000)
002A8AC8 0025:
	??_C@_0CF@JENNJBDB@failed?5to?5open?5playlist?5profile?5@ (0000)
002A8AF0 0020:
	??_C@_0CA@JIHEDGDA@failed?5to?5read?5playlist?5profile?$AA@ (0000)
002A8B10 0029:
	??_C@_0CJ@LCMCPKBB@checksum?5failed?5on?5playlist?5prof@ (0000)
002A8B3C 0015:
	??_C@_0BF@BJMJPDMI@full_path?5?$CG?$CG?5variant?$AA@ (0000)
002A8B54 002e:
	??_C@_0CO@ELJJEBEK@c?3?2halo?2SOURCE?2saved?5games?2playl@ (0000)
002A8B88 0064:
	??_C@_0GE@NLKIPHIJ@failed?5to?5load?5localized?5default@ (0000)
002A8BEC 003c:
	??_C@_0DM@KFHPCEIC@failed?5to?5create?5default?5playlis@ (0000)
002A8C28 000a:
	??_C@_09MBMCAFGJ@?2blam?4lst?$AA@ (0000)
002A8C34 0029:
	??_C@_0CJ@HEDEFBDK@z?3?2saved?2playlists?2default_playl@ (0000)
002A8C60 002a:
	??_C@_0CK@KIMIIBCE@failed?5to?5read?5playlist?5profile?5@ (0000)
002A8C90 0050:
	??_C@_0FA@GBEOJFIM@checksum?5failed?5on?5playlist?5prof@ (0000)
002A8CE0 003a:
	??_C@_0DK@BHBCOKFG@waiting?5for?5asynchronous?5playlis@ (0000)
002A8D1C 001b:
	??_C@_0BL@BLFKNELG@end?5playlist?5profile?5write?$AA@ (0000)
002A8D38 0029:
	??_C@_0CJ@EKFPEBOB@failed?5to?5write?5playlist?5profile@ (0000)
002A8D64 001d:
	??_C@_0BN@IJDOMGDA@begin?5playlist?5profile?5write?$AA@ (0000)
003168C8 0069:
	_playlist_profile_default_data (0000)
004D2858 0074:
	_playlist_profile_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "saved games/playlist_profile.h"
#include "bungie_net/common/thread.h"
#include "cseries/errors.h"
#include "text/unicode.h"
#include "saved games/saved_game_files.h"
#include "tag_files/files.h"
#include "game/game_engine_playlist.h"
#include "text/text_group.h"
#include "tag_files/tag_groups.h"
/* the saved game file checksum is an XDK content signature. */
#include <xtl.h>

/* ---------- constants */

enum
{
	PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE = 104,
	MAXIMUM_GAME_VARIANT_NAME_LENGTH = 12,
	NUMBER_OF_DEFAULT_PLAYLIST_PROFILES = 26,
};

enum
{
	_game_variant_is_system_default_bit = 0
};

/* ---------- macros */

/* ---------- structures */

#pragma pack(push, 1)
struct playlist_profile_data
{
	struct game_variant *(*default_variant_building_functions[
		NUMBER_OF_DEFAULT_PLAYLIST_PROFILES])(
		struct game_variant *variant);
	boolean first_time;
};
#pragma pack(pop)

typedef char verify_playlist_profile_building_functions_size[
	sizeof(((struct playlist_profile_data *)0)->
		default_variant_building_functions) == 0x68 ? 1 : -1];
typedef char verify_playlist_profile_first_time_offset[
	offsetof(struct playlist_profile_data, first_time) == 0x68 ? 1 : -1];
typedef char verify_playlist_profile_data_size[
	sizeof(struct playlist_profile_data) == 0x69 ? 1 : -1];

struct playlist_profile_write_request
{
	long playlist_profile_index;
	struct game_variant variant;
};

typedef char verify_playlist_profile_request_variant_offset[
	offsetof(struct playlist_profile_write_request, variant) == 0x4 ? 1 : -1];
typedef char verify_playlist_profile_request_size[
	sizeof(struct playlist_profile_write_request) == 0x6C ? 1 : -1];

struct playlist_profile_runtime_globals_prefix
{
	struct playlist_profile_write_request thread_input;
	struct thread_reference *thread;
	word number_of_default_profiles;
	boolean initialized;
	byte pad;
};

typedef char verify_playlist_profile_thread_offset[
	offsetof(struct playlist_profile_runtime_globals_prefix, thread) == 0x6C ? 1 : -1];
typedef char verify_playlist_profile_default_count_offset[
	offsetof(
		struct playlist_profile_runtime_globals_prefix,
		number_of_default_profiles) == 0x70 ? 1 : -1];
typedef char verify_playlist_profile_initialized_offset[
	offsetof(
		struct playlist_profile_runtime_globals_prefix,
		initialized) == 0x72 ? 1 : -1];
typedef char verify_playlist_profile_globals_size[
	sizeof(struct playlist_profile_runtime_globals_prefix) == 0x74 ? 1 : -1];

/* ---------- prototypes */

static void playlist_profile_create_default_profiles_on_disk(
	void);
static boolean playlist_profile_read(
	long playlist_profile_index,
	struct game_variant *variant);
static unsigned long __stdcall playlist_profile_write_thread_proc(
	void *input);
static void playlist_profile_write(
	long playlist_profile_index,
	struct game_variant *variant);

/* ---------- globals */

struct playlist_profile_runtime_globals_prefix playlist_profile_globals = { 0 };
struct playlist_profile_data playlist_profile_default_data =
{
	{
		build_game_variant_slayer,
		build_game_variant_slayer_pro,
		build_game_variant_elimination,
		build_game_variant_phantoms,
		build_game_variant_endurance,
		build_game_variant_rockets,
		build_game_variant_snipers,
		build_game_variant_oddball,
		build_game_variant_reverse_tag,
		build_game_variant_accumulation,
		build_game_variant_juggernaut,
		build_game_variant_stalker,
		build_game_variant_king,
		build_game_variant_king_pro,
		build_game_variant_crazy_king,
		build_game_variant_race,
		build_game_variant_rally,
		build_game_variant_ctf,
		build_game_variant_invasion,
		build_game_variant_iron_ctf,
		build_game_variant_ctf_pro,
		build_game_variant_team_race,
		build_game_variant_team_rally,
		build_game_variant_team_oddball,
		build_game_variant_team_king,
		build_game_variant_team_slayer,
	},
	TRUE
};

/* ---------- public code */

void playlist_profiles_initialize(
	void)
{
	csmemset(
		&playlist_profile_globals,
		0,
		sizeof(playlist_profile_globals));
	playlist_profile_globals.initialized = TRUE;

	return;
}

void playlist_profiles_dispose(
	void)
{
	if (playlist_profile_globals.thread)
	{
		error(
			_error_silent,
			"waiting for asynchronous playlist profile writes to finish...");
		while (!thread_has_exited(playlist_profile_globals.thread))
		{
		}
		dispose_thread(playlist_profile_globals.thread);
		playlist_profile_globals.thread = NULL;
	}

	csmemset(
		&playlist_profile_globals,
		0,
		sizeof(playlist_profile_globals));

	return;
}

void playlist_profile_delete(
	long playlist_profile_index)
{
	if (playlist_profile_index != NONE &&
		!delete_enumerated_saved_game_file(playlist_profile_index))
	{
		error(
			_error_silent,
			"playlist_profile_delete() failed (profile index= #0x%lX)",
			playlist_profile_index);
	}

	return;
}

word playlist_profile_number_of_default_profiles_on_disk(
	void)
{
	return playlist_profile_globals.number_of_default_profiles;
}

void playlist_profiles_enumerate_available_to_local_player_index(
	short local_player_index,
	word *number_of_profiles,
	long *playlist_profile_indices)
{
	if (playlist_profile_default_data.first_time == TRUE)
	{
		playlist_profile_create_default_profiles_on_disk();
		playlist_profile_default_data.first_time = FALSE;
	}

	saved_game_files_enumerate_available_to_local_player_index(
		local_player_index,
		_saved_game_file_type_game_variant,
		number_of_profiles,
		playlist_profile_indices,
		TRUE);

	return;
}

long playlist_profile_new(
	short local_player_index,
	wchar_t *name)
{
	struct game_variant temporary;
	struct file_reference file;
	long playlist_profile_index = create_enumerated_saved_game_file(
		_saved_game_file_type_game_variant,
		local_player_index,
		name);

	if (playlist_profile_index != NONE)
	{
		if (saved_game_file_open(&file, playlist_profile_index))
		{
			byte block[SAVED_GAME_FILE_BLOCK_SIZE] = {0};
			struct game_variant variant = *build_game_variant_slayer(&temporary);

			csmemcpy(
				block,
				&variant,
				sizeof(struct game_variant));
			SET_FLAG(
				((struct game_variant *)block)->flags,
				_game_variant_is_system_default_bit,
				FALSE);
			game_engine_variant_cleanup((struct game_variant *)block);
			ustrncpy(
				(wchar_t *)block,
				name,
				MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1);
			((struct game_variant *)block)->human_readable_game_description[MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1] = 0;
			saved_game_file_generate_checksum(
				block,
				PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
				(XCALCSIG_SIGNATURE *)(block + PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE));

			if (!file_set_position(&file, 0) ||
				!file_write(&file, sizeof(block), block))
			{
				error(
					_error_silent,
					"failed to initialize newly created playlist profile");
				delete_enumerated_saved_game_file(playlist_profile_index);
				playlist_profile_index = NONE;
			}

			saved_game_file_close(&file, playlist_profile_index);
		}
		else
		{
			error(
				_error_silent,
				"failed to open newly created playlist profile");
			delete_enumerated_saved_game_file(playlist_profile_index);
			playlist_profile_index = NONE;
		}
	}

	return playlist_profile_index;
}

boolean playlist_profile_get_from_path(
	char *full_path,
	struct game_variant *variant)
{
	byte block[SAVED_GAME_FILE_BLOCK_SIZE];
	struct file_reference file;
	XCALCSIG_SIGNATURE checksum;
	boolean success = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		243,
		full_path && variant);

	if (file_reference_create_from_path(&file, full_path, FALSE) &&
		file_open(&file, FLAG(_permission_read_bit)))
	{
		if (file_read(&file, sizeof(block), block))
		{
			saved_game_file_generate_checksum(
				block,
				PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
				&checksum);

			if (!csmemcmp(
				&checksum,
				block + PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
				sizeof(checksum)))
			{
				csmemcpy(
					variant,
					block,
					sizeof(struct game_variant));
				success = TRUE;
			}
			else
			{
				error(
					_error_silent,
					"checksum failed on playlist profile file");
			}
		}
		else
		{
			error(
				_error_silent,
				"failed to read playlist profile");
		}

		file_close(&file);
	}
	else
	{
		error(
			_error_silent,
			"failed to open playlist profile file");
	}

	return success;
}

boolean playlist_profile_get_display_name(
	long playlist_profile_index,
	wchar_t *display_name)
{
	wchar_t *name = saved_game_file_get_display_name(playlist_profile_index);

	if (name)
	{
		ustrncpy(display_name, name, MAX_GAMENAME - 1);
		display_name[MAX_GAMENAME - 1] = 0;

		return TRUE;
	}

	return FALSE;
}

boolean playlist_profile_get(
	long playlist_profile_index,
	struct game_variant *variant)
{
	boolean success = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		217,
		variant);

	if (playlist_profile_index == NONE)
	{
		game_engine_playlist_next(0, 0, 4);
	}
	else
	{
		success = playlist_profile_read(playlist_profile_index, variant);
	}

	return success;
}

void playlist_profile_save(
	long playlist_profile_index,
	struct game_variant *variant)
{
	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		305,
		variant);

	if (playlist_profile_index != NONE)
	{
		game_engine_variant_cleanup(variant);
		playlist_profile_write(playlist_profile_index, variant);
	}

	return;
}

/* ---------- private code */

static void playlist_profile_create_default_profiles_on_disk(
	void)
{
	struct game_variant temporary;
	byte block[SAVED_GAME_FILE_BLOCK_SIZE];
	struct game_variant variant;
	struct file_reference file;
	char path[MAXIMUM_FILENAME_LENGTH+1];
	long string_list_index = tag_loaded(
		UNICODE_STRING_LIST_TAG,
		"ui\\default_multiplayer_game_setting_names");

	if (string_list_index != NONE)
	{
		long profile_index;

		for (profile_index = 0;
			profile_index < NUMBER_OF_DEFAULT_PLAYLIST_PROFILES;
			profile_index++)
		{
			wchar_t *display_name;
			boolean file_written = FALSE;

			variant = *playlist_profile_default_data.default_variant_building_functions[
				profile_index](&temporary);

			_snprintf(path, MAXIMUM_FILENAME_LENGTH, "z:\\saved\\playlists\\default_playlist\\%02d", profile_index);
			path[MAXIMUM_FILENAME_LENGTH] = 0;
			directory_create_or_delete_contents(path);
			csstrncat(path, "\\blam.lst", MAXIMUM_FILENAME_LENGTH);
			path[MAXIMUM_FILENAME_LENGTH] = 0;

			display_name = unicode_string_list_get_string(
				string_list_index,
				(short)profile_index);

			csmemcpy(
				block,
				&variant,
				sizeof(struct game_variant));
			ustrncpy(
				(wchar_t *)block,
				display_name,
				MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1);
			((struct game_variant *)block)->human_readable_game_description[MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1] = 0;
			((struct game_variant *)block)->flags |= (short)(profile_index << 8);
			saved_game_file_generate_checksum(
				block,
				PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
				(XCALCSIG_SIGNATURE *)(block + PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE));

			if (file_reference_create_from_path(&file, path, FALSE) &&
				file_create(&file) &&
				file_open(&file, FLAG(_permission_write_bit)) &&
				file_set_position(&file, 0))
			{
				file_written = file_write(&file, sizeof(block), block);
				file_close(&file);
			}

			if (file_written == TRUE)
			{
				playlist_profile_globals.number_of_default_profiles++;
			}
			else
			{
				error(
					_error_silent,
					"failed to create default playlist profile file '%s' on disk",
					path);
			}
		}

		saved_game_files_notify_memory_units_changed();
	}
	else
	{
		error(
			_error_silent,
			"failed to load localized default variant names string list tag; no default game variants enumerated");
	}

	return;
}

static boolean playlist_profile_read(
	long playlist_profile_index,
	struct game_variant *variant)
{
	byte block[SAVED_GAME_FILE_BLOCK_SIZE];
	struct file_reference file;
	struct game_variant temporary;
	struct game_variant default_variant;
	XCALCSIG_SIGNATURE checksum;
	boolean success = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		396,
		variant);

	if (playlist_profile_globals.thread)
	{
		error(
			_error_silent,
			"waiting for asynchronous playlist profile io to finish...");
		while (!thread_has_exited(playlist_profile_globals.thread))
		{
		}
		dispose_thread(playlist_profile_globals.thread);
		playlist_profile_globals.thread = NULL;
	}

	if (TEST_FLAG(playlist_profile_index, _saved_game_file_index_valid_bit))
	{
		if (saved_game_files_take_mutex())
		{
			if (saved_game_file_open(&file, playlist_profile_index))
			{
				if (file_read(&file, sizeof(block), block))
				{
					saved_game_file_generate_checksum(
						block,
						PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
						&checksum);

					if (!csmemcmp(
						&checksum,
						block + PLAYLIST_PROFILE_CHECKSUM_DATA_SIZE,
						sizeof(checksum)))
					{
						csmemcpy(
							variant,
							block,
							sizeof(struct game_variant));
					}
					else
					{
						default_variant = *build_game_variant_slayer(&temporary);
						error(
							_error_silent,
							"checksum failed on playlist profile file, sanitizing memory resident version...");
						default_variant.flags = 0;
						ustrncpy(
							(wchar_t *)&default_variant,
							saved_game_file_get_display_name(playlist_profile_index),
							MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1);
						default_variant.human_readable_game_description[MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1] = 0;
						csmemcpy(
							variant,
							&default_variant,
							sizeof(struct game_variant));
					}

					success = TRUE;
				}
				else
				{
					error(
						_error_silent,
						"failed to read playlist profile from file");
				}

				saved_game_file_close(&file, playlist_profile_index);
			}
			else
			{
				error(
					_error_silent,
					"failed to open playlist profile file");
			}

			saved_game_files_release_mutex();
		}
		else
		{
			error(
				_error_silent,
				"failed to get saved game files mutex; perhaps another operation is in progress?");
		}
	}
	else
	{
		default_variant = *build_game_variant_slayer(&temporary);
		error(
			_error_silent,
			"checksum failed on playlist profile file, sanitizing memory resident version...");
		default_variant.flags = 0;
		ustrncpy(
			(wchar_t *)&default_variant,
			saved_game_file_get_display_name(playlist_profile_index),
			MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1);
		default_variant.human_readable_game_description[MAXIMUM_GAME_VARIANT_NAME_LENGTH - 1] = 0;
		csmemcpy(
			variant,
			&default_variant,
			sizeof(struct game_variant));
		success = TRUE;
	}

	return success;
}

static unsigned long __stdcall playlist_profile_write_thread_proc(
	void *input)
{
	struct playlist_profile_write_request *request = input;
	struct file_reference file;
	byte block[SAVED_GAME_FILE_BLOCK_SIZE];

	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		514,
		input);

	error(
		_error_silent,
		"begin playlist profile write");

	if (saved_game_files_take_mutex())
	{
		long playlist_profile_index = request->playlist_profile_index;
		boolean failed = FALSE;
		struct game_variant *variant = &request->variant;

		if (saved_game_file_open(&file, playlist_profile_index))
		{
			csmemcpy(
				block,
				variant,
				sizeof(struct game_variant));
			saved_game_file_generate_checksum(
				block,
				sizeof(struct game_variant),
				(struct _XCALCSIG_SIGNATURE *)(block + sizeof(struct game_variant)));

			if (!file_set_position(&file, 0) ||
				!file_write(&file, sizeof(block), block))
			{
				error(
					_error_silent,
					"failed to write playlist profile to file");
				failed = TRUE;
			}

			if (saved_game_file_close(&file, playlist_profile_index) &&
				!synchronize_metadata_display_name_with_profile_name(
					playlist_profile_index,
					(wchar_t *)variant))
			{
				error(
					_error_silent,
					"metadata name may not match game display name");
			}

		}
		else
		{
			error(
				_error_silent,
				"failed to open playlist profile file");
		}

		if (failed)
		{
			delete_enumerated_saved_game_file(playlist_profile_index);
		}

		saved_game_files_release_mutex();
	}
	else
	{
		error(
			_error_silent,
			"failed to get saved game files mutex; perhaps another operation is in progress?");
	}

	error(
		_error_silent,
		"end playlist profile write");

	return 0;
}

static void playlist_profile_write(
	long playlist_profile_index,
	struct game_variant *variant)
{
	match_assert(
		"c:\\halo\\SOURCE\\saved games\\playlist_profile.c",
		490,
		variant);

	if (playlist_profile_globals.thread)
	{
		error(
			_error_silent,
			"waiting for asynchronous playlist profile io to finish...");
		while (!thread_has_exited(playlist_profile_globals.thread))
		{
		}
		dispose_thread(playlist_profile_globals.thread);
		playlist_profile_globals.thread = NULL;
	}

	playlist_profile_globals.thread_input.playlist_profile_index = playlist_profile_index;
	csmemcpy(
		&playlist_profile_globals.thread_input.variant,
		variant,
		sizeof(struct game_variant));

	create_thread(
		0,
		playlist_profile_write_thread_proc,
		&playlist_profile_globals.thread_input,
		&playlist_profile_globals.thread);

	return;
}
