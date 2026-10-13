/*
PREDICTED_RESOURCES.C

symbols in this file:
001AD870 0090:
	_code_001ad870 (0000)
001AD900 0080:
	_predicted_resources_precache (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cache/predicted_resources.h"
#include "cache/sound_cache.h"
#include "bitmaps/bitmap_group.h"
#include "sound/sound_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load);

static void predicted_resources_sound_precache(long sound_definition_index);

/* ---------- globals */

/* ---------- public code */

void predicted_resources_precache(
	struct tag_block *predicted_resources)
{
	short predicted_resource_index;

#ifdef HALO_N64
	/* nothing to warm up: the N64 has no texture or sound cache to fill
	ahead of time (and its sound tags are records of its own:
	port/n64/game/n64_sound_game.c) */
	return;
#endif

	for (predicted_resource_index = 0;
		predicted_resource_index < predicted_resources->count;
		predicted_resource_index++)
	{
		struct predicted_resource *predicted_resource;

		predicted_resource = TAG_BLOCK_GET_ELEMENT(
			predicted_resources,
			predicted_resource_index,
			struct predicted_resource);
		switch (predicted_resource->type)
		{
		case _predicted_resource_bitmap:
			_texture_cache_bitmap_get_hardware_format(
				TAG_BLOCK_GET_ELEMENT(
					&bitmap_group_get(predicted_resource->tag_index)->bitmaps,
					predicted_resource->resource_index,
					struct bitmap_data),
				FALSE,
				TRUE);
			break;

		case _predicted_resource_sound:
			predicted_resources_sound_precache(predicted_resource->tag_index);
			break;
		}
	}

	return;
}

/* ---------- private code */

static void predicted_resources_sound_precache(
	long sound_definition_index)
{
	struct sound_definition *sound_definition;
	struct tag_block *pitch_ranges;
	short pitch_range_index;

	sound_definition = sound_definition_get(sound_definition_index);
	pitch_range_index = 0;
	if (sound_definition->pitch_ranges.count <= 0)
		return;
	pitch_ranges = &sound_definition->pitch_ranges;

pitch_range_loop:
	{
		struct sound_pitch_range *pitch_range;
		short permutation_index;

		pitch_range = TAG_BLOCK_GET_ELEMENT(pitch_ranges, pitch_range_index, struct sound_pitch_range);
		for (permutation_index = 0;
			permutation_index < pitch_range->actual_permutation_count;
			permutation_index++)
		{
			_sound_cache_sound_request(
				TAG_BLOCK_GET_ELEMENT(&pitch_range->permutations, permutation_index, struct sound_permutation),
				FALSE,
				TRUE,
				FALSE);
		}
		pitch_range_index++;
	}
	if (pitch_range_index < pitch_ranges->count)
		goto pitch_range_loop;

	return;
}
