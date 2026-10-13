/*
HUD_SOUNDS.C

symbols in this file:
000C6430 0160:
	_hud_play_sound (0000)
00270240 0026:
	??_C@_0CG@LACJPAPB@c?3?2halo?2SOURCE?2interface?2hud_sou@ (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "interface/hud.h"
#include "objects/objects.h"
#include "sound/sound_definitions.h"
#include "sound/game_sound.h"
#include "sound/sound_manager.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct hud_sound_definition
{
	struct tag_reference sound;
	unsigned long state_flags;
	real scale;
	/* Reserved by the tag format; the original block accessor proves the 0x38-byte element size. */
	byte reserved[32];
};

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void hud_play_sound(
	short local_player_index,
	unsigned long state_flags,
	struct tag_block const *sounds,
	long *sound_indices,
	word *played_flags)
{
	long absolute_sound_index = 0;
	short sound_index = 0;

	if (sounds->count > 0)
		do
		{
			struct hud_sound_definition const *sound =
				TAG_BLOCK_GET_ELEMENT(sounds, absolute_sound_index, struct hud_sound_definition);

			if (state_flags & sound->state_flags)
			{
				switch (sound->sound.group_tag)
				{
				default:
					match_assert("c:\\halo\\SOURCE\\interface\\hud_sounds.c", 47, !"unreachable");
					break;

				case SOUND_DEFINITION_TAG:
					if (sound_indices[absolute_sound_index] == NONE ||
						!TEST_FLAG(*played_flags, absolute_sound_index))
					{
						if (sound_indices[absolute_sound_index] != NONE)
							sound_stop_impulse(sound_indices[absolute_sound_index]);
						sound_indices[absolute_sound_index] =
							unspatialized_impulse_sound_new(sound->sound.index, sound->scale);
					}
					break;

				case LOOPING_SOUND_DEFINITION_TAG:
					if (sound_indices[absolute_sound_index] == NONE)
						sound_indices[absolute_sound_index] =
							unattached_looping_sound_start(sound->sound.index, NONE, sound->scale);
					break;
				}

				SET_FLAG(*played_flags, absolute_sound_index, TRUE);
			}
			else
			{
				if (sound_indices[absolute_sound_index] != NONE)
				{
					switch (sound->sound.group_tag)
					{
					default:
						match_assert("c:\\halo\\SOURCE\\interface\\hud_sounds.c", 64, !"unreachable");
						break;
					case SOUND_DEFINITION_TAG:
						break;
					case LOOPING_SOUND_DEFINITION_TAG:
						unattached_looping_sound_stop(sound_indices[absolute_sound_index]);
						break;
					}

					sound_indices[absolute_sound_index] = NONE;
					SET_FLAG(*played_flags, absolute_sound_index, FALSE);
				}
			}

			sound_index++;
			absolute_sound_index = sound_index;
		}
		while (absolute_sound_index < sounds->count);

	return;
}

/* ---------- private code */
