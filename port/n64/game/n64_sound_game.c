/*
N64_SOUND_GAME.C

The game's sounds on the N64: Halo's own sound code (source/sound/) is left
out of the build, and the calls the game makes into it -- impulse sounds
on objects, at places and unspatialized, looping sounds on objects and the
HUD's, the frame's update -- are these, played on port/n64/src/n64_sound.c's
voices.

A sound tag ('snd!') or looping sound ('lsnd') the N64 keeps is a record of
its own (tools/n64/sounds.py), what the tag instance's address points at:
the sound's class, distances, random pitch, gain and its scale modifiers,
its pitch ranges and their permutations (each a file of the map's sound
list); a looping sound's tracks (start, loop and end sounds). The
environment's sounds (ambience, devices, music) are left out by the
converter, so their references are NONE.

As Halo's sound manager does (sound_manager.c): a sound starts unless its
skip fraction says not; its pitch is random within its bounds, scaled by
the scale (0-1) the game gives; the pitch picks the pitch range, a random
permutation of it not played lately is chosen (try_to_reset_permutations);
its gain is the permutation's times the sound's modifier and the scale's.
Spatialized sounds are heard by the nearest local player's camera: full
within the minimum distance, then falling as minimum / distance (Halo's
DirectSound 3D rolloff), none past the maximum distance; panned by where
they are to the camera's left or right. Sounds on objects follow them
(their node), as do looping sounds, whose loops' scale comes from the
object's function each frame (a vehicle's engine pitch). A class allows a
few of the same sound at once and fewer on the same object (a weapon's new
shot cuts the last one's).
*/

#include "cseries.h"

#include "camera/observer.h"
#include "game/players.h"
#include "math/real_math.h"
#include "objects/objects.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"
#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"

#include "../src/n64.h"

#include <math.h>
#include <string.h>

char const *n64_loaded_map_name(void);

/* ---------- the converter's records (tools/n64/sounds.py) */

struct n64_sound_definition
{
	unsigned char sound_class;
	unsigned char flags;
	unsigned char pitch_range_count;
	unsigned char skip_fraction; /* /255 */
	real minimum_distance;
	real maximum_distance;
	real random_pitch_lower, random_pitch_upper;
	real gain_modifier;
	/* /4096 */
	unsigned short zero_skip_fraction_modifier, one_skip_fraction_modifier;
	unsigned short zero_gain_modifier, one_gain_modifier;
	unsigned short zero_pitch_modifier, one_pitch_modifier;
	unsigned long longest_permutation_length; /* milliseconds */
};

struct n64_sound_pitch_range
{
	real bend_lower, bend_upper;
	real playback_rate;
	unsigned long played_permutation_mask;
	unsigned short first_permutation;
	unsigned char permutation_count;
	signed char previous_permutation;
};

struct n64_sound_permutation
{
	unsigned short file;
	unsigned char gain; /* /255 */
	unsigned char skip_fraction;
};

struct n64_looping_sound_track
{
	long start, loop, end;
	real gain, fade_in, fade_out;
	unsigned long flags;
};

struct n64_looping_sound_definition
{
	unsigned char track_count;
	unsigned char flags;
	unsigned short pad;
	real maximum_distance;
	struct n64_looping_sound_track tracks[1];
};

typedef char n64_sound_definition_size_assert[sizeof(struct n64_sound_definition) == 40 ? 1 : -1];
typedef char n64_sound_pitch_range_size_assert[sizeof(struct n64_sound_pitch_range) == 20 ? 1 : -1];
typedef char n64_looping_sound_track_size_assert[sizeof(struct n64_looping_sound_track) == 28 ? 1 : -1];

#define MODIFIER(value) ((real)(value) * (1.f / 4096.f))
#define FRACTION(value) ((real)(value) * (1.f / 255.f))

/* ---------- constants */

enum
{
	MAXIMUM_PLAYING_SOUNDS = 24,
	MAXIMUM_LOOPING_SOUNDS = 32,
	MAXIMUM_LOOPING_TRACKS = 2,
	NUMBER_OF_N64_SOUND_CLASSES = 51,
};

/* the sound's own sounds: the louder of all (the mixer's headroom) */
#define MASTER_GAIN 0.7f
/* a voice's priority: the class's (Halo's 1-6) and these */
#define PRIORITY_PER_CLASS 30
#define PRIORITY_UNSPATIALIZED 60
#define PRIORITY_LOOPING 20

/* Halo's sound classes (sound_classes.c): how many of one sound at once,
how many on one object, the priority */
static unsigned char const class_limits[NUMBER_OF_N64_SOUND_CLASSES][3] =
{
	{ 6, 4, 4 }, { 4, 1, 5 }, { 0, 0, 0 }, { 0, 0, 0 },
	{ 4, 1, 4 }, { 4, 1, 4 }, { 4, 1, 4 }, { 4, 1, 4 },
	{ 4, 1, 4 }, { 4, 1, 4 }, { 4, 1, 4 }, { 0, 0, 0 }, { 0, 0, 0 },
	{ 4, 1, 3 }, { 4, 1, 3 }, { 4, 1, 3 }, { 0, 0, 0 }, { 0, 0, 0 },
	{ 4, 1, 3 }, { 4, 1, 3 }, { 0, 0, 0 }, { 0, 0, 0 }, { 4, 2, 3 },
	{ 4, 2, 3 }, { 0, 0, 0 }, { 0, 0, 0 }, { 4, 1, 2 }, { 4, 1, 2 },
	{ 4, 1, 1 }, { 4, 1, 1 }, { 4, 1, 1 }, { 0, 0, 0 }, { 4, 4, 2 },
	{ 4, 1, 1 }, { 4, 1, 1 }, { 4, 1, 1 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
	{ 4, 1, 4 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 4, 4, 6 },
	{ 4, 4, 3 }, { 4, 4, 5 }, { 4, 4, 6 }, { 0, 0, 0 }, { 0, 0, 0 }, { 4, 1, 5 },
};

/* ---------- structures */

struct playing_sound
{
	long voice; /* -1: free */
	long definition_index;
	/* followed (NONE: a place, or unspatialized) */
	long object_index;
	short node_index;
	boolean spatialized;
	/* in the node's space when followed, else the world's */
	real_point3d position;
	real base_gain; /* the permutation's and the sound's */
	real gain; /* that and the scale's, before distance */
	real pitch; /* the voice's rate */
	short pitch_range_index;
	short looping_index; /* the looping sound it plays a track of, or NONE */
	short salt;
	short pad;
};

struct looping_sound
{
	long definition_index; /* NONE: free */
	long object_index;
	short node_index;
	short function_index;
	real_point3d position;
	real scale;
	boolean unattached;
	boolean stopping;
	boolean active;
	byte pad;
	short salt;
	/* each track's sound (an index of the playing sounds) and whether it is
	its start sound still */
	long track_sounds[MAXIMUM_LOOPING_TRACKS];
	boolean track_starting[MAXIMUM_LOOPING_TRACKS];
};

struct listener
{
	boolean valid;
	real_point3d position;
	real_vector3d left;
};

/* ---------- globals */

static struct
{
	boolean loaded;
	struct playing_sound playing[MAXIMUM_PLAYING_SOUNDS];
	struct looping_sound looping[MAXIMUM_LOOPING_SOUNDS];
	struct listener listeners[MAXIMUM_LOCAL_PLAYERS];
	unsigned long random;
	short next_salt;
	long frame;
	long logged;
} n64_sounds;

/* ---------- private code */

static real sound_random(void)
{
	n64_sounds.random = n64_sounds.random * 1664525UL + 1013904223UL;
	return (real)(n64_sounds.random >> 8) * (1.f / 16777216.f);
}

static short next_salt(void)
{
	n64_sounds.next_salt = (short)((n64_sounds.next_salt + 1) & 0x7FFF);
	if (!n64_sounds.next_salt)
		n64_sounds.next_salt = 1;
	return n64_sounds.next_salt;
}

/* the N64's record of a kept sound, or NULL */
static struct n64_sound_definition *sound_get(long definition_index)
{
	if (!n64_sounds.loaded || definition_index == NONE ||
		tag_get_group_tag(definition_index) != SOUND_DEFINITION_TAG)
		return NULL;
	return (struct n64_sound_definition *)tag_get(SOUND_DEFINITION_TAG, definition_index);
}

static struct n64_looping_sound_definition *looping_sound_get(long definition_index)
{
	if (!n64_sounds.loaded || definition_index == NONE ||
		tag_get_group_tag(definition_index) != LOOPING_SOUND_DEFINITION_TAG)
		return NULL;
	return (struct n64_looping_sound_definition *)tag_get(LOOPING_SOUND_DEFINITION_TAG, definition_index);
}

static struct n64_sound_pitch_range *sound_pitch_ranges(struct n64_sound_definition *definition)
{
	return (struct n64_sound_pitch_range *)(definition + 1);
}

static struct n64_sound_permutation *sound_permutations(struct n64_sound_definition *definition)
{
	return (struct n64_sound_permutation *)(sound_pitch_ranges(definition) + definition->pitch_range_count);
}

/* sound_definition_find_pitch_range_by_pitch */
static short sound_find_pitch_range(struct n64_sound_definition *definition, real pitch)
{
	struct n64_sound_pitch_range *ranges = sound_pitch_ranges(definition);
	real closest = 1e30f;
	short result = NONE;
	short i;

	for (i = 0; i < definition->pitch_range_count; i++)
	{
		struct n64_sound_pitch_range *range = &ranges[i];

		if (!range->permutation_count)
			continue;
		if (range->bend_lower <= pitch && pitch <= range->bend_upper)
			return i;
		{
			real ratio = range->bend_upper < pitch ?
				pitch / (range->bend_upper > 0.f ? range->bend_upper : 1.f) :
				range->bend_lower / (pitch > 0.f ? pitch : 1.f);

			if (ratio < closest)
			{
				closest = ratio;
				result = i;
			}
		}
	}

	return result;
}

/* sound_definition_next_permutation: a random one not played since all
were */
static short sound_next_permutation(struct n64_sound_definition *definition, short range_index)
{
	struct n64_sound_pitch_range *range = &sound_pitch_ranges(definition)[range_index];
	struct n64_sound_permutation *permutations = sound_permutations(definition) + range->first_permutation;
	short count = range->permutation_count;
	short selected = (short)(sound_random() * count);
	short attempts = 0;
	unsigned long all = count >= 32 ? 0xFFFFFFFFUL : (1UL << count) - 1;

	if (selected >= count)
		selected = (short)(count - 1);
	for (;;)
	{
		if ((~range->played_permutation_mask & all) == 0)
		{
			range->played_permutation_mask = 0;
			if (count > 1 && range->previous_permutation >= 0)
				range->played_permutation_mask = 1UL << range->previous_permutation;
		}
		if (!(range->played_permutation_mask & (1UL << selected)))
		{
			range->played_permutation_mask |= 1UL << selected;
			if (attempts++ == 16 || sound_random() >= FRACTION(permutations[selected].skip_fraction))
				break;
		}
		if (++selected == count)
			selected = 0;
	}
	range->previous_permutation = (signed char)selected;

	return selected;
}

static real scale_value(real base, real zero_modifier, real one_modifier, real scale)
{
	return ((one_modifier - zero_modifier) * scale + zero_modifier) * base;
}

static void update_listeners(void)
{
	short i;

	for (i = 0; i < MAXIMUM_LOCAL_PLAYERS; i++)
	{
		struct listener *listener = &n64_sounds.listeners[i];

		listener->valid = FALSE;
		if (i < local_player_count() && local_player_get_player_index(i) != NONE)
		{
			struct observer_result const *camera = observer_get_camera(i);

			if (camera)
			{
				listener->valid = TRUE;
				listener->position = camera->position;
				/* left = up x forward */
				listener->left.i = camera->up.j * camera->forward.k - camera->up.k * camera->forward.j;
				listener->left.j = camera->up.k * camera->forward.i - camera->up.i * camera->forward.k;
				listener->left.k = camera->up.i * camera->forward.j - camera->up.j * camera->forward.i;
			}
		}
	}
}

/* how loud a place is to the nearest local player (0: out of hearing),
and where to pan it */
static real hear(real_point3d const *position, real minimum_distance, real maximum_distance, real *pan)
{
	struct listener const *nearest = NULL;
	real nearest_distance_squared = 1e30f;
	real_vector3d offset;
	real distance;
	short i;

	for (i = 0; i < MAXIMUM_LOCAL_PLAYERS; i++)
	{
		struct listener const *listener = &n64_sounds.listeners[i];

		if (listener->valid)
		{
			real dx = position->x - listener->position.x;
			real dy = position->y - listener->position.y;
			real dz = position->z - listener->position.z;
			real d2 = dx * dx + dy * dy + dz * dz;

			if (d2 < nearest_distance_squared)
			{
				nearest_distance_squared = d2;
				nearest = listener;
			}
		}
	}
	*pan = 0.f;
	if (!nearest)
		return 1.f;
	if (nearest_distance_squared > maximum_distance * maximum_distance)
		return 0.f;
	distance = sqrtf(nearest_distance_squared);
	if (distance > 0.01f)
	{
		offset.i = (position->x - nearest->position.x) / distance;
		offset.j = (position->y - nearest->position.y) / distance;
		offset.k = (position->z - nearest->position.z) / distance;
		/* (left is +: the voice's pan is - to the left) */
		*pan = -(offset.i * nearest->left.i + offset.j * nearest->left.j + offset.k * nearest->left.k);
		/* (close by, both ears) */
		if (distance < minimum_distance)
			*pan *= distance / minimum_distance;
	}

	return distance <= minimum_distance ? 1.f : minimum_distance / distance;
}

/* where a playing sound is now (FALSE: its object is gone) */
static boolean playing_position(struct playing_sound const *sound, real_point3d *position)
{
	if (sound->object_index == NONE)
	{
		*position = sound->position;
		return TRUE;
	}
	if (!object_try_and_get(sound->object_index))
		return FALSE;
	matrix4x3_transform_point(
		object_get_node_matrix(sound->object_index, sound->node_index == NONE ? 0 : sound->node_index),
		&sound->position,
		position);

	return TRUE;
}

static void playing_free(short index)
{
	struct playing_sound *sound = &n64_sounds.playing[index];

	if (sound->voice >= 0)
		n64_voice_stop(sound->voice);
	sound->voice = -1;
	sound->looping_index = NONE;
}

/* a free playing sound (else the oldest that is not a loop's), its
index */
static short playing_new(void)
{
	short i;

	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
	{
		if (n64_sounds.playing[i].voice < 0)
			return i;
		if (!n64_voice_playing(n64_sounds.playing[i].voice) && n64_sounds.playing[i].looping_index == NONE)
		{
			playing_free(i);
			return i;
		}
	}

	return NONE;
}

/* the class's limits: stops what the new sound replaces; FALSE when the
same sound plays on too many voices already and none of them is older */
static void make_room(long definition_index, struct n64_sound_definition *definition, long object_index)
{
	unsigned char const *limits = definition->sound_class < NUMBER_OF_N64_SOUND_CLASSES ?
		class_limits[definition->sound_class] : class_limits[0];
	short same_definition = 0;
	short same_object = 0;
	short oldest_definition = NONE;
	short oldest_object = NONE;
	short i;

	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
	{
		struct playing_sound *sound = &n64_sounds.playing[i];

		if (sound->voice < 0 || sound->definition_index != definition_index || sound->looping_index != NONE)
			continue;
		if (!n64_voice_playing(sound->voice))
		{
			playing_free(i);
			continue;
		}
		/* (the oldest has the lowest salt mostly; good enough) */
		same_definition++;
		if (oldest_definition == NONE)
			oldest_definition = i;
		if (object_index != NONE && sound->object_index == object_index)
		{
			same_object++;
			if (oldest_object == NONE)
				oldest_object = i;
		}
	}
	if (limits[1] && same_object >= limits[1] && oldest_object != NONE)
		playing_free(oldest_object);
	else if (limits[0] && same_definition >= limits[0] && oldest_definition != NONE)
		playing_free(oldest_definition);
}

/* starts sound definition_index (sound_new_impulse): at position (in the
object's node's space when object_index is one, else the world's; NULL:
unspatialized); returns the playing sound's index (with its salt) or
NONE */
static long sound_start(
	long definition_index,
	long object_index,
	short node_index,
	real_point3d const *position,
	real scale,
	short looping_index,
	real looping_gain,
	boolean loop)
{
	struct n64_sound_definition *definition = sound_get(definition_index);
	struct playing_sound *sound;
	struct n64_sound_permutation *permutation;
	struct n64_sound_pitch_range *range;
	real pitch, gain, audible, pan;
	real_point3d world;
	short range_index, permutation_index, index;
	int priority;

	if (!definition || !definition->pitch_range_count)
		return NONE;
	if (scale == 0.f && definition->zero_gain_modifier == 0)
		return NONE;
	if (looping_index == NONE && sound_random() <= scale_value(
		FRACTION(definition->skip_fraction),
		MODIFIER(definition->zero_skip_fraction_modifier),
		MODIFIER(definition->one_skip_fraction_modifier),
		scale))
		return NONE;

	/* audible at all? */
	pan = 0.f;
	audible = 1.f;
	if (position)
	{
		if (object_index != NONE)
		{
			if (!object_try_and_get(object_index))
				return NONE;
			matrix4x3_transform_point(object_get_node_matrix(object_index, node_index == NONE ? 0 : node_index),
				position, &world);
		}
		else
		{
			world = *position;
		}
		audible = hear(&world, definition->minimum_distance, definition->maximum_distance, &pan);
		if (audible <= 0.f && looping_index == NONE)
			return NONE;
	}

	pitch = scale_value(
		definition->random_pitch_lower + (definition->random_pitch_upper - definition->random_pitch_lower) * sound_random(),
		MODIFIER(definition->zero_pitch_modifier),
		MODIFIER(definition->one_pitch_modifier),
		scale);
	range_index = sound_find_pitch_range(definition, pitch);
	if (range_index == NONE)
		return NONE;
	range = &sound_pitch_ranges(definition)[range_index];
	permutation_index = sound_next_permutation(definition, range_index);
	permutation = &sound_permutations(definition)[range->first_permutation + permutation_index];
	if (permutation->file == 0xFFFF)
		return NONE;

	make_room(definition_index, definition, object_index);
	index = playing_new();
	if (index == NONE)
		return NONE;
	sound = &n64_sounds.playing[index];
	gain = FRACTION(permutation->gain) * definition->gain_modifier *
		scale_value(looping_gain, MODIFIER(definition->zero_gain_modifier), MODIFIER(definition->one_gain_modifier), scale);
	sound->base_gain = FRACTION(permutation->gain) * definition->gain_modifier;
	priority = (definition->sound_class < NUMBER_OF_N64_SOUND_CLASSES ?
		class_limits[definition->sound_class][2] : 3) * PRIORITY_PER_CLASS;
	if (!position)
		priority += PRIORITY_UNSPATIALIZED;
	if (looping_index != NONE)
		priority += PRIORITY_LOOPING;

	sound->voice = n64_voice_play(permutation->file, gain * audible * MASTER_GAIN, pan,
		pitch * range->playback_rate, priority, loop);
	if (sound->voice < 0)
		return NONE;
	sound->definition_index = definition_index;
	sound->object_index = position ? object_index : NONE;
	sound->node_index = node_index;
	sound->spatialized = position != NULL;
	if (position)
		sound->position = *position;
	sound->gain = gain;
	sound->pitch = pitch * range->playback_rate;
	sound->pitch_range_index = range_index;
	sound->looping_index = looping_index;
	sound->salt = next_salt();
	static int sound_log = -1;

	if (sound_log < 0)
		sound_log = n64_debug_flag("debug_sound") != 0;
	if (sound_log && n64_sounds.logged < 300)
	{
		char line[160];

		n64_sounds.logged++;
		sprintf(line, "sound %d: %s (class %d, voice %lx, looping %d, gain %.2f, pitch %.2f)", index,
			tag_get_name(definition_index), definition->sound_class, sound->voice, looping_index,
			gain * audible, sound->pitch);
		n64_log(line);
	}

	return index | ((long)sound->salt << 16);
}

static struct playing_sound *playing_get(long sound_index)
{
	short index = (short)(sound_index & 0xFFFF);

	if (sound_index == NONE || index < 0 || index >= MAXIMUM_PLAYING_SOUNDS ||
		n64_sounds.playing[index].voice < 0 || n64_sounds.playing[index].salt != (short)(sound_index >> 16))
		return NULL;
	return &n64_sounds.playing[index];
}

static void update_playing(void)
{
	short i;

	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
	{
		struct playing_sound *sound = &n64_sounds.playing[i];
		struct n64_sound_definition *definition;
		real_point3d position;
		real audible, pan;

		if (sound->voice < 0 || sound->looping_index != NONE)
			continue;
		if (!n64_voice_playing(sound->voice))
		{
			playing_free(i);
			continue;
		}
		if (!sound->spatialized)
			continue;
		definition = sound_get(sound->definition_index);
		if (!definition || !playing_position(sound, &position))
		{
			/* (its object is gone: it stays where it was) */
			if (definition && sound->object_index != NONE)
			{
				sound->object_index = NONE;
				continue;
			}
			playing_free(i);
			continue;
		}
		audible = hear(&position, definition->minimum_distance, definition->maximum_distance, &pan);
		n64_voice_set(sound->voice, sound->gain * audible * MASTER_GAIN, pan, sound->pitch);
	}
}

/* ---------- looping sounds */

static struct looping_sound *looping_get(long looping_sound_index)
{
	short index = (short)(looping_sound_index & 0xFFFF);

	if (looping_sound_index == NONE || index < 0 || index >= MAXIMUM_LOOPING_SOUNDS ||
		n64_sounds.looping[index].definition_index == NONE ||
		n64_sounds.looping[index].salt != (short)(looping_sound_index >> 16))
		return NULL;
	return &n64_sounds.looping[index];
}

static void looping_tracks_stop(struct looping_sound *looping, boolean play_ends)
{
	struct n64_looping_sound_definition *definition = looping_sound_get(looping->definition_index);
	short track;

	for (track = 0; track < MAXIMUM_LOOPING_TRACKS; track++)
	{
		struct playing_sound *sound = playing_get(looping->track_sounds[track]);

		if (sound)
			playing_free((short)(looping->track_sounds[track] & 0xFFFF));
		looping->track_sounds[track] = NONE;
		if (play_ends && definition && track < definition->track_count && looping->active &&
			definition->tracks[track].end != NONE)
		{
			real_point3d *position = looping->object_index != NONE ? &looping->position : NULL;

			sound_start(definition->tracks[track].end, looping->object_index, looping->node_index, position,
				1.f, NONE, definition->tracks[track].gain, FALSE);
		}
	}
	looping->active = FALSE;
}

static void looping_free(short index)
{
	struct looping_sound *looping = &n64_sounds.looping[index];

	looping_tracks_stop(looping, FALSE);
	looping->definition_index = NONE;
}

/* a looping sound's tracks this frame: started, its loops kept going and
their scale applied, or stopped */
static void update_looping(short index)
{
	struct looping_sound *looping = &n64_sounds.looping[index];
	struct n64_looping_sound_definition *definition = looping_sound_get(looping->definition_index);
	real_point3d position, *at = NULL;
	boolean audible;
	real scale = looping->scale;
	short track;

	if (!definition)
	{
		looping_free(index);
		return;
	}
	if (looping->unattached)
	{
		audible = !looping->stopping;
		if (looping->object_index != NONE && !object_try_and_get(looping->object_index))
		{
			looping_free(index);
			return;
		}
	}
	else
	{
		if (!object_try_and_get(looping->object_index))
		{
			looping_free(index);
			return;
		}
		audible = object_get_function_value(looping->object_index, looping->function_index, &scale);
	}
	if (looping->object_index != NONE && !looping->unattached)
	{
		real pan;

		matrix4x3_transform_point(object_get_node_matrix(looping->object_index, looping->node_index),
			&looping->position, &position);
		at = &looping->position;
		/* (out of everyone's hearing: its voices are given up) */
		if (hear(&position, 1.f, definition->maximum_distance > 0.f ? definition->maximum_distance : 10.f, &pan) <= 0.f)
			audible = FALSE;
	}

	if (!audible)
	{
		if (looping->active)
			looping_tracks_stop(looping, TRUE);
		if (looping->unattached && looping->stopping)
			looping_free(index);
		return;
	}

	for (track = 0; track < definition->track_count && track < MAXIMUM_LOOPING_TRACKS; track++)
	{
		struct n64_looping_sound_track const *data = &definition->tracks[track];
		struct playing_sound *sound = playing_get(looping->track_sounds[track]);

		if (sound && !n64_voice_playing(sound->voice))
		{
			/* the start sound done (or the loop's voice taken by another
			sound): the loop */
			playing_free((short)(looping->track_sounds[track] & 0xFFFF));
			sound = NULL;
			looping->track_sounds[track] = NONE;
		}
		else if (sound && looping->track_starting[track])
		{
			continue;
		}
		if (!sound)
		{
			boolean starting = !looping->active && data->start != NONE;
			long definition_index = starting ? data->start : data->loop;

			if (definition_index == NONE)
				continue;
			looping->track_sounds[track] = sound_start(definition_index, looping->object_index, looping->node_index,
				at, scale, index, data->gain, !starting);
			looping->track_starting[track] = starting;
			continue;
		}
		/* the loop: its gain, pitch and place now */
		{
			struct n64_sound_definition *sound_definition = sound_get(sound->definition_index);
			real pitch, gain, audible_gain = 1.f, pan = 0.f;

			if (!sound_definition)
				continue;
			pitch = scale_value(1.f, MODIFIER(sound_definition->zero_pitch_modifier),
				MODIFIER(sound_definition->one_pitch_modifier), scale);
			/* another pitch range: its own sound */
			if (sound_find_pitch_range(sound_definition, pitch) != sound->pitch_range_index)
			{
				playing_free((short)(looping->track_sounds[track] & 0xFFFF));
				looping->track_sounds[track] = sound_start(sound->definition_index, looping->object_index,
					looping->node_index, at, scale, index, data->gain, TRUE);
				looping->track_starting[track] = FALSE;
				continue;
			}
			pitch *= sound_pitch_ranges(sound_definition)[sound->pitch_range_index].playback_rate;
			gain = sound->base_gain * scale_value(data->gain, MODIFIER(sound_definition->zero_gain_modifier),
				MODIFIER(sound_definition->one_gain_modifier), scale);
			if (at)
				audible_gain = hear(&position, sound_definition->minimum_distance, sound_definition->maximum_distance, &pan);
			n64_voice_set(sound->voice, gain * audible_gain * MASTER_GAIN, pan, pitch);
		}
	}
	looping->active = TRUE;
}

static long looping_new(long object_index, long definition_index, short node_index, real_point3d const *position,
	short function_index, boolean unattached, real scale)
{
	short i;

	if (!looping_sound_get(definition_index))
		return NONE;
	for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
	{
		struct looping_sound *looping = &n64_sounds.looping[i];

		if (looping->definition_index == NONE)
		{
			memset(looping, 0, sizeof(*looping));
			looping->definition_index = definition_index;
			looping->object_index = object_index;
			looping->node_index = node_index;
			looping->function_index = function_index;
			if (position)
				looping->position = *position;
			looping->scale = scale;
			looping->unattached = unattached;
			looping->track_sounds[0] = looping->track_sounds[1] = NONE;
			looping->salt = next_salt();
			return i | ((long)looping->salt << 16);
		}
	}

	return NONE;
}

/* ---------- public code (game_sound.h, sound_manager.h, sound_definitions.h) */

void game_sound_initialize(void)
{
	short i;

	n64_sounds.random = n64_ticks();
	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
	{
		n64_sounds.playing[i].voice = -1;
		n64_sounds.playing[i].looping_index = NONE;
	}
	for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
		n64_sounds.looping[i].definition_index = NONE;
}

void game_sound_initialize_for_new_map(void)
{
	game_sound_initialize();
	n64_sounds.loaded = n64_sound_load_map(n64_loaded_map_name());
}

void game_sound_dispose_from_old_map(void)
{
	short i;

	for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
		n64_sounds.looping[i].definition_index = NONE;
	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
		playing_free(i);
	n64_sound_unload_map();
	n64_sounds.loaded = FALSE;
}

void sound_stop_all(void)
{
	short i;

	for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
	{
		if (n64_sounds.looping[i].definition_index != NONE)
			looping_free(i);
	}
	for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
		playing_free(i);
}

void sound_stop_impulse(long sound_index)
{
	if (playing_get(sound_index))
		playing_free((short)(sound_index & 0xFFFF));
}

real sound_definition_get_maximum_distance(long sound_index)
{
	struct n64_sound_definition *definition = sound_get(sound_index);

	return definition ? definition->maximum_distance : 0.f;
}

/* a sound's longest permutation in milliseconds (for the code that reads
it from Halo's sound definition: the announcer's queue, speech) */
long n64_sound_definition_length(long sound_index)
{
	struct n64_sound_definition *definition = sound_get(sound_index);

	return definition ? (long)definition->longest_permutation_length : 0;
}

long object_impulse_sound_new(
	long object_index,
	long definition_index,
	short node_index,
	real_point3d const *position,
	real_vector3d const *forward,
	real scale)
{
	(void)forward;
	if (object_index == NONE)
		return NONE;
	return sound_start(definition_index, object_index, node_index, position, scale, NONE, 1.f, FALSE);
}

long unattached_impulse_sound_new(
	long definition_index,
	struct sound_location const *location,
	real scale)
{
	return sound_start(definition_index, NONE, NONE, &location->position, scale, NONE, 1.f, FALSE);
}

long unspatialized_impulse_sound_new(
	long definition_index,
	real scale)
{
	return sound_start(definition_index, NONE, NONE, NULL, scale, NONE, 1.f, FALSE);
}

long game_looping_sound_new(
	long object_index,
	long definition_index,
	char const *marker_name,
	short function_index)
{
	struct object_marker marker;

	if (definition_index == NONE || !looping_sound_get(definition_index))
		return NONE;
	if (object_index == NONE)
		return looping_new(NONE, definition_index, 0, NULL, function_index, FALSE, 1.f);
	if (!object_get_marker_by_name(object_index, marker_name, &marker, 1))
		return NONE;

	return looping_new(object_index, definition_index, marker.node_index, &marker.node_matrix.position,
		function_index, FALSE, 1.f);
}

void game_looping_sound_delete(long looping_sound_index)
{
	struct looping_sound *looping = looping_get(looping_sound_index);

	if (looping)
		looping_free((short)(looping_sound_index & 0xFFFF));
}

long unattached_looping_sound_start(
	long definition_index,
	long source_object_index,
	real scale)
{
	(void)source_object_index;
	return looping_new(NONE, definition_index, 0, NULL, NONE, TRUE, scale);
}

void unattached_looping_sound_stop(long looping_sound_index)
{
	struct looping_sound *looping = looping_get(looping_sound_index);

	if (looping)
		looping->stopping = TRUE;
}

void game_sound_update(real dt)
{
	short i;

	(void)dt;
	if (!n64_sounds.loaded)
		return;
	update_listeners();
	for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
	{
		if (n64_sounds.looping[i].definition_index != NONE)
			update_looping(i);
	}
	update_playing();
	n64_sounds.frame++;
	if (n64_debug_flag("debug_sound") && (n64_sounds.frame % 150) == 0)
	{
		unsigned long started, stolen, refused;
		short playing = 0, looping = 0;
		char line[96];

		for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
			playing += n64_sounds.playing[i].voice >= 0;
		for (i = 0; i < MAXIMUM_LOOPING_SOUNDS; i++)
			looping += n64_sounds.looping[i].definition_index != NONE;
		n64_sound_statistics(&started, &stolen, &refused);
		sprintf(line, "sound: %lu started, %lu stolen, %lu refused; %d playing, %d looping, heap %lu",
			started, stolen, refused, playing, looping, n64_heap_free());
		n64_log(line);
		for (i = 0; i < MAXIMUM_PLAYING_SOUNDS; i++)
		{
			struct playing_sound *sound = &n64_sounds.playing[i];

			if (sound->voice < 0)
				continue;
			sprintf(line, "  %d: voice %lx %s, looping %d, %s", i, sound->voice,
				n64_voice_playing(sound->voice) ? "playing" : "done", sound->looping_index,
				tag_get_name(sound->definition_index));
			n64_log(line);
		}
	}
}
