/*
SOUND_DEFINITIONS.C

symbols in this file:
001B8800 0040:
	_sound_definition_get_maximum_distance (0000)
001B8840 0040:
	_sound_definition_get_minimum_distance (0000)
001B8880 0050:
	_sound_permutation_get_mouth_aperture (0000)
001B88D0 0100:
	_sound_definition_find_pitch_range_by_pitch (0000)
001B89D0 0040:
	_try_to_reset_permutations (0000)
001B8A10 0060:
	_sound_permutation_get_real_mouth_aperture (0000)
001B8A70 0160:
	_sound_definition_next_permutation (0000)
002AB1D0 000c:
	_sound_sample_rate_samples_per_second (0000)
	_oo_unsigned_char_max (0008)
002AB1DC 0039:
	??_C@_0DJ@MJNMCFCL@tick_index?$DO?$DN0?5?$CG?$CG?5tick_index?$DMperm@ (0000)
002AB218 0029:
	??_C@_0CJ@KBJMLIGI@c?3?2halo?2SOURCE?2sound?2sound_defin@ (0000)
002AB248 0042:
	??_C@_0EC@IMDGFJGB@but?5how?5can?5you?5speak?5if?5you?5hav@ (0000)
002AB28C 001a:
	??_C@_0BK@DPJOOPFK@range?9?$DOpermutations?4count?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "errors.h"
#include "sound_classes.h"
#include "sound_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

unsigned long const sound_sample_rate_samples_per_second[2] =
{
	22050,
	44100
};

real const oo_unsigned_char_max = 1.f / 255.f;

/* ---------- public code */

real sound_definition_get_maximum_distance(
	long sound_index)
{
	struct sound_definition *definition = sound_definition_get(sound_index);
	real maximum_distance = definition->maximum_distance;

	if (maximum_distance == 0.f)
		maximum_distance = sound_class_get(definition->sound_class)->maximum_distance;

	return maximum_distance;
}

real sound_definition_get_minimum_distance(
	long sound_index)
{
	struct sound_definition *definition = sound_definition_get(sound_index);
	real minimum_distance = definition->minimum_distance;

	if (minimum_distance == 0.f)
		minimum_distance = sound_class_get(definition->sound_class)->minimum_distance;

	return minimum_distance;
}

byte *sound_permutation_get_mouth_aperture(
	struct sound_permutation *permutation,
	short tick_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\sound\\sound_definitions.c",
		800,
		tick_index>=0 && tick_index<permutation->mouth_data.size);

	return (byte *)permutation->mouth_data.address + tick_index;
}

short sound_definition_find_pitch_range_by_pitch(
	struct sound_definition *definition,
	real pitch,
	short old_range_index)
{
	short result = NONE;

	if (old_range_index != NONE && old_range_index < definition->pitch_ranges.count)
	{
		struct sound_pitch_range *range = TAG_BLOCK_GET_ELEMENT(
			&definition->pitch_ranges,
			old_range_index,
			struct sound_pitch_range);

		if (range->bend_bounds.lower <= pitch &&
			pitch <= range->bend_bounds.upper &&
			range->permutations.count)
		{
			result = old_range_index;
		}
	}

	if (result == NONE)
	{
		real closest_pitch_ratio = FLT_MAX;
		short range_index;

		for (range_index = 0; range_index < definition->pitch_ranges.count; range_index++)
		{
			struct sound_pitch_range *range = TAG_BLOCK_GET_ELEMENT(
				&definition->pitch_ranges,
				range_index,
				struct sound_pitch_range);

			if (range->permutations.count)
			{
				if (range->bend_bounds.lower <= pitch && pitch <= range->bend_bounds.upper)
				{
					result = range_index;
					break;
				}
				else
				{
					real pitch_ratio = range->bend_bounds.upper < pitch ?
						pitch / range->bend_bounds.upper :
						range->bend_bounds.lower / pitch;

					if (pitch_ratio < closest_pitch_ratio)
					{
						result = range_index;
						closest_pitch_ratio = pitch_ratio;
					}
				}
			}
		}
	}

	return result;
}

void try_to_reset_permutations(
	struct sound_pitch_range *range)
{
	short permutation_count = range->actual_permutation_count;
	unsigned long all_permutations_mask = (FLAG(permutation_count) - 1);

	if ((~range->played_permutation_mask & all_permutations_mask) == 0)
	{
		range->played_permutation_mask = 0;
		if (permutation_count > 1)
			range->played_permutation_mask = FLAG((byte)range->previous_permutation_index);
	}

	return;
}

real sound_permutation_get_real_mouth_aperture(
	struct sound_permutation *permutation,
	long tick_index)
{
	if (permutation->mouth_data.size)
	{
		long aperture = *sound_permutation_get_mouth_aperture(
			permutation,
			PIN((short)tick_index, 0, permutation->mouth_data.size - 1));

		return aperture * oo_unsigned_char_max;
	}

	error(
		_error_silent,
		"but how can you speak if you have no mouth data? (permutation %s)",
		permutation->name);

	return 0.f;
}

short sound_definition_next_permutation(
	struct sound_definition *definition,
	short pitch_range_index,
	short permutation_index)
{
	struct sound_pitch_range *range = TAG_BLOCK_GET_ELEMENT(
		&definition->pitch_ranges,
		pitch_range_index,
		struct sound_pitch_range);
	short selected_permutation_index;
	short attempt_count = 0;

	match_assert(
		"c:\\halo\\SOURCE\\sound\\sound_definitions.c",
		892,
		range->permutations.count);

	if (range->forced_permutation_index != NONE)
	{
		selected_permutation_index = range->forced_permutation_index;
		range->forced_permutation_index = NONE;
		range->previous_permutation_index = selected_permutation_index;
		return selected_permutation_index;
	}

	if (TEST_FLAG(definition->flags, 1) && permutation_index != NONE)
	{
		struct sound_permutation *permutation = TAG_BLOCK_GET_ELEMENT(
			&range->permutations,
			permutation_index,
			struct sound_permutation);

		return permutation->next_permutation_index;
	}

	selected_permutation_index = seed_random_range(
		get_global_local_random_seed_address(),
		0,
		range->actual_permutation_count);

	for (;;)
	{
		try_to_reset_permutations(range);

		if (!TEST_FLAG(range->played_permutation_mask, selected_permutation_index))
		{
			struct sound_permutation *permutation;

			SET_FLAG(range->played_permutation_mask, selected_permutation_index, TRUE);
			if (attempt_count++ == 16)
				break;

			{
				real random = real_seed_random(get_global_local_random_seed_address());

				permutation = TAG_BLOCK_GET_ELEMENT(
				&range->permutations,
				selected_permutation_index,
				struct sound_permutation);
				if (random >= permutation->skip_fraction)
					break;
			}
		}

		selected_permutation_index++;
		if (selected_permutation_index == range->actual_permutation_count)
			selected_permutation_index = 0;
	}

	range->previous_permutation_index = selected_permutation_index;
	return selected_permutation_index;
}

/* ---------- private code */
