/*
SOUND_IMPORT.H

header included in hcex build.
*/

#ifndef __SOUND_IMPORT_H
#define __SOUND_IMPORT_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct file_reference;
struct sound_file_info
{
	long samples_per_second;
	short channel_count;
	short block_alignment;
	short significant_bits_per_sample;
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

boolean sound_file_info_get(struct sound_file_info *info, struct file_reference const *file);
boolean sound_raw_sample_data_get(
	struct file_reference const *file,
	struct sound_file_info const *info,
	long *size,
	void *data);

#endif // __SOUND_IMPORT_H
