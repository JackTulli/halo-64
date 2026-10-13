/*
SOUND_IMPORT.C

symbols in this file:
001B66F0 00a0:
	_sound_file_info_get (0000)
001B6790 0080:
	_sound_raw_sample_data_get (0000)
002AA8E0 0031:
	??_C@_0DB@FLHLMIPJ@c?3?2halo?2SOURCE?2sound?2sound_impor@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "sound_import.h"

#include "tag_files/files.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

struct sound_file_info;

boolean sound_file_is_aiff(struct file_reference const *file);
boolean sound_file_aiff_info_get(struct file_reference const *file, struct sound_file_info *info);
boolean sound_file_aiff_raw_data_get(struct file_reference const *file, long *size, void *data);
void sound_file_aiff_format(struct sound_file_info const *info, long *size, void *data);

boolean sound_file_is_wave(struct file_reference const *file);
boolean sound_file_wave_info_get(struct file_reference const *file, struct sound_file_info *info);
boolean sound_file_wave_raw_data_get(struct file_reference const *file, long *size, void *data);
void sound_file_wave_format(struct sound_file_info const *info, long *size, void *data);

/* ---------- globals */

/* ---------- public code */

boolean sound_file_info_get(
	struct sound_file_info *info,
	struct file_reference const *file)
{
	boolean result = TRUE;

	match_assert("c:\\halo\\SOURCE\\sound\\sound_import\\sound_import.c", 18, info);
	match_assert("c:\\halo\\SOURCE\\sound\\sound_import\\sound_import.c", 19, file);

	if (!(sound_file_is_aiff(file) && sound_file_aiff_info_get(file, info)) &&
		!(sound_file_is_wave(file) && sound_file_wave_info_get(file, info)))
	{
		result = FALSE;
	}

	return result;
}

boolean sound_raw_sample_data_get(
	struct file_reference const *file,
	struct sound_file_info const *info,
	long *size,
	void *data)
{
	boolean result = TRUE;

	if (sound_file_is_aiff(file) && sound_file_aiff_raw_data_get(file, size, data))
	{
		sound_file_aiff_format(info, size, data);
	}
	else if (sound_file_is_wave(file) && sound_file_wave_raw_data_get(file, size, data))
	{
		sound_file_wave_format(info, size, data);
	}
	else
	{
		result = FALSE;
	}

	return result;
}

/* ---------- private code */
