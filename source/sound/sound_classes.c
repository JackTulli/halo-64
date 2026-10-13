/*
SOUND_CLASSES.C

symbols in this file:
001B83B0 00d0:
	_sound_class_get (0000)
001B8480 0020:
	_sound_classes_initialize (0000)
001B84A0 0010:
	_sound_classes_dispose_from_old_map (0000)
001B84B0 0010:
	_sound_classes_dispose (0000)
001B84C0 0070:
	_sound_class_datum_get (0000)
001B8530 0050:
	_debug_sound_classes_enable (0000)
001B8580 0050:
	_debug_sound_classes_set_distances (0000)
001B85D0 0090:
	_debug_sound_classes_set_wet (0000)
001B8660 0090:
	_sound_classes_initialize_for_new_map (0000)
001B86F0 0060:
	_sound_classes_update (0000)
001B8750 0020:
	_sound_class_get_gain (0000)
001B8770 0090:
	_sound_class_set_gain (0000)
002AAE2C 000b:
	??_C@_0L@BIEBCFPL@game_event?$AA@ (0000)
002AAE38 0024:
	??_C@_0CE@FFIIALPE@scripted_dialog_force_unspatiali@ (0000)
002AAE5C 0016:
	??_C@_0BG@PMEDKJAG@scripted_dialog_other?$AA@ (0000)
002AAE74 0010:
	??_C@_0BA@PIFDKGKE@scripted_effect?$AA@ (0000)
002AAE84 0017:
	??_C@_0BH@IEDDKOJK@scripted_dialog_player?$AA@ (0000)
002AAE9C 0014:
	??_C@_0BE@KBCEHMLC@first_person_damage?$AA@ (0000)
002AAEB0 0006:
	??_C@_05LLDJOKNO@music?$AA@ (0000)
002AAEB8 0011:
	??_C@_0BB@INOIALEP@device_computers?$AA@ (0000)
002AAECC 000e:
	??_C@_0O@OPEICHCG@device_nature?$AA@ (0000)
002AAEDC 0011:
	??_C@_0BB@LOPDIFLM@device_machinery?$AA@ (0000)
002AAEF0 0013:
	??_C@_0BD@COPDMILP@device_force_field?$AA@ (0000)
002AAF04 000c:
	??_C@_0M@KOCNEJDE@device_door?$AA@ (0000)
002AAF10 000f:
	??_C@_0P@KBGPBKJK@vehicle_engine?$AA@ (0000)
002AAF20 0012:
	??_C@_0BC@IEONMMEH@vehicle_collision?$AA@ (0000)
002AAF34 000c:
	??_C@_0M@NKENHPBM@unit_dialog?$AA@ (0000)
002AAF40 000f:
	??_C@_0P@IMONABBA@unit_footsteps?$AA@ (0000)
002AAF50 0016:
	??_C@_0BG@JOJJIGFG@slow_particle_impacts?$AA@ (0000)
002AAF68 0011:
	??_C@_0BB@HKBJNHLL@particle_impacts?$AA@ (0000)
002AAF7C 000f:
	??_C@_0P@LIMHLKCD@object_impacts?$AA@ (0000)
002AAF8C 000c:
	??_C@_0M@GNEGCAEJ@weapon_idle?$AA@ (0000)
002AAF98 0010:
	??_C@_0BA@CKECLKEF@weapon_overheat?$AA@ (0000)
002AAFA8 000e:
	??_C@_0O@KJCLMPCH@weapon_charge?$AA@ (0000)
002AAFB8 000d:
	??_C@_0N@IMGJPOMB@weapon_empty?$AA@ (0000)
002AAFC8 000e:
	??_C@_0O@JDGJENID@weapon_reload?$AA@ (0000)
002AAFD8 000d:
	??_C@_0N@FGEJBEKJ@weapon_ready?$AA@ (0000)
002AAFE8 000c:
	??_C@_0M@LMEOBDP@weapon_fire?$AA@ (0000)
002AAFF4 0016:
	??_C@_0BG@ELFKKADI@projectile_detonation?$AA@ (0000)
002AB00C 0012:
	??_C@_0BC@EEOLAAAL@projectile_impact?$AA@ (0000)
002AB020 0059:
	??_C@_0FJ@KPKABKBN@definition?9?$DOmaximum_number_per_o@ (0000)
002AB080 0052:
	??_C@_0FC@BCDFOEO@definition?9?$DOmaximum_number_per_d@ (0000)
002AB0D4 0022:
	??_C@_0CC@KKMODDB@sound_class_names?$FLclass_index?$FN?$FL0@ (0000)
002AB0F8 0036:
	??_C@_0DG@LJHIOPFH@class_index?$DO?$DN0?5?$CG?$CG?5class_index?$DMNU@ (0000)
002AB130 0025:
	??_C@_0CF@HELLJLLD@c?3?2halo?2source?2sound?2sound_class@ (0000)
002AB158 000e:
	??_C@_0O@CJFDJJBL@sound?5classes?$AA@ (0000)
002AB168 0011:
	??_C@_0BB@MPOJAGHF@sound_class_data?$AA@ (0000)
002AB17C 002a:
	??_C@_0CK@PIDIAGFK@index?$DO?$DN0?5?$CG?$CG?5index?$DMNUMBER_OF_SOUN@ (0000)
002AB1A8 0025:
	??_C@_0CF@KNEKLCO@c?3?2halo?2SOURCE?2sound?2sound_class@ (0000)
00316AA8 0994:
	_sound_classes (0000)
	_sound_class_names (08c8)
*/

/* ---------- headers */

#include "cseries.h"
#include "sound_classes.h"
#include "game_state.h"

/* ---------- constants */

enum
{
	NUMBER_OF_SOUND_CLASSES = 51
};

/* ---------- macros */

#define SOUND_CLASS_DEFINITION(maximum_per_definition, maximum_per_object, preemption_time, speech, priority, cache_miss_mode, wet_gain, minimum_distance, maximum_distance, unknown_gain, unknown_scale, disabled) \
	{ maximum_per_definition, maximum_per_object, preemption_time, speech, 0, priority, cache_miss_mode, 0, wet_gain, 0.f, minimum_distance, maximum_distance, unknown_gain, unknown_scale, disabled, { 0, 0, 0 } }

/* ---------- structures */

struct sound_class_runtime
{
	real target_gain;
	real current_gain;
	short interpolation_ticks;
	short pad;
};

typedef char verify_sound_class_definition_size[
	sizeof(struct sound_class_definition) == 0x2C ? 1 : -1];

/* ---------- prototypes */

struct sound_class_definition *sound_class_get(
	short sound_class);

/* ---------- globals */

extern struct sound_class_runtime *sound_class_data;
struct sound_class_definition sound_classes[NUMBER_OF_SOUND_CLASSES] =
{
	SOUND_CLASS_DEFINITION(6, 4, 100, 0, 4, 0, 0.5f, 1.4f, 8.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 200, 0, 5, 1, 0.5f, 8.f, 120.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 0, 0, 4, 1, 0.5f, 4.f, 70.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 500, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 500, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 60, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 500, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 500, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 500, 0, 4, 1, 0.5f, 1.f, 9.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 3, 1, 0.5f, 0.5f, 3.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 3, 0, 0.5f, 0.5f, 3.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 1000, 0, 3, 0, 0.5f, 0.5f, 3.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 200, 0, 3, 0, 0.5f, 0.9f, 10.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 1, 3, 1, 0.8f, 3.f, 20.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 2, 400, 0, 3, 0, 0.5f, 1.4f, 8.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 2, 100, 0, 3, 1, 0.9f, 1.4f, 8.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 2, 1, 0.5f, 0.9f, 5.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 2, 1, 0.5f, 0.9f, 5.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 0.5f, 0.9f, 5.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 0.5f, 0.9f, 5.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 0.5f, 0.5f, 3.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 4, 100, 0, 2, 1, 1.f, 0.9f, 5.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 1.f, 0.9f, 5.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 1.f, 0.9f, 5.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 1, 1, 1.f, 0.5f, 3.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 4, 1, 1.f, 0.5f, 3.f, 1.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 4, 100, 1, 6, 1, 0.8f, 3.f, 20.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 4, 100, 0, 3, 1, 0.8f, 2.f, 5.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 4, 100, 1, 5, 1, 0.8f, 3.f, 20.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(4, 4, 100, 1, 6, 1, 0.8f, 3.f, 20.f, 0.f, 1.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0),
	SOUND_CLASS_DEFINITION(4, 1, 100, 0, 5, 1, 1.f, 3.f, 20.f, 1.f, 1.f, 0),
};

char const *sound_class_names[NUMBER_OF_SOUND_CLASSES] =
{
	"projectile_impact", "projectile_detonation", "", "",
	"weapon_fire", "weapon_ready", "weapon_reload", "weapon_empty",
	"weapon_charge", "weapon_overheat", "weapon_idle", "", "",
	"object_impacts", "particle_impacts", "slow_particle_impacts", "", "",
	"unit_footsteps", "unit_dialog", "", "", "vehicle_collision",
	"vehicle_engine", "", "", "device_door", "device_force_field",
	"device_machinery", "device_nature", "device_computers", "", "music",
	"ambient_nature", "ambient_machinery", "ambient_computers", "", "", "",
	"first_person_damage", "", "", "", "", "scripted_dialog_player",
	"scripted_effect", "scripted_dialog_other",
	"scripted_dialog_force_unspatialized", "", "", "game_event",
};

/* ---------- public code */

struct sound_class_definition *sound_class_get(
	short class_index)
{
	struct sound_class_definition *definition;

	definition = &sound_classes[class_index];
	match_assert(
		"c:\\halo\\source\\sound\\sound_classes.h",
		131,
		class_index>=0 && class_index<NUMBER_OF_SOUND_CLASSES);
	match_assert(
		"c:\\halo\\source\\sound\\sound_classes.h",
		132,
		sound_class_names[class_index][0]);
	match_assert(
		"c:\\halo\\source\\sound\\sound_classes.h",
		133,
		definition->maximum_number_per_definition<=MAXIMUM_SOUND_INSTANCES_PER_DEFINITION);
	match_assert(
		"c:\\halo\\source\\sound\\sound_classes.h",
		134,
		definition->maximum_number_per_object<=MAXIMUM_SOUND_INSTANCES_PER_OBJECT_PER_DEFINITION);

	return &sound_classes[class_index];
}

void sound_classes_initialize(
	void)
{
	sound_class_data = game_state_malloc(
		"sound classes",
		NULL,
		0x264);

	return;
}

void sound_classes_dispose_from_old_map(
	void)
{
	return;
}

void sound_classes_dispose(
	void)
{
	sound_class_data = NULL;

	return;
}

static struct sound_class_runtime *sound_class_datum_get(
	short index)
{
	match_assert(
		"c:\\halo\\SOURCE\\sound\\sound_classes.c",
		288,
		index>=0 && index<NUMBER_OF_SOUND_CLASSES);
	match_assert(
		"c:\\halo\\SOURCE\\sound\\sound_classes.c",
		289,
		sound_class_data);

	return &sound_class_data[index];
}

real sound_class_get_gain(
	short index)
{
	return sound_class_datum_get(index)->current_gain;
}

void debug_sound_classes_enable(
	char const *name,
	boolean enable)
{
	short class_index;
	char const **sound_class_name;

	class_index = 0;
	sound_class_name = sound_class_names;
	do
	{
		if ((*sound_class_name)[0] && strstr(*sound_class_name, name))
		{
			sound_class_get(class_index)->disabled = !enable;
		}
		class_index++;
		sound_class_name++;
	}
	while (class_index < NUMBER_OF_SOUND_CLASSES);

	return;
}

void debug_sound_classes_set_distances(
	char const *name,
	real minimum_distance,
	real maximum_distance)
{
	short class_index;
	char const **sound_class_name;

	class_index = 0;
	sound_class_name = sound_class_names;
	do
	{
		if ((*sound_class_name)[0] && strstr(*sound_class_name, name))
		{
			sound_class_get(class_index)->minimum_distance = minimum_distance;
			sound_class_get(class_index)->maximum_distance = maximum_distance;
		}
		class_index++;
		sound_class_name++;
	}
	while (class_index < NUMBER_OF_SOUND_CLASSES);

	return;
}

void sound_classes_initialize_for_new_map(
	void)
{
	short class_index;

	for (class_index = 0; class_index < NUMBER_OF_SOUND_CLASSES; class_index++)
	{
		struct sound_class_runtime *sound_class = sound_class_datum_get(class_index);

		sound_class->current_gain = 1.f;
		sound_class->target_gain = 1.f;
		sound_class->interpolation_ticks = 0;
	}

	return;
}

void sound_classes_update(
	long ticks)
{
	if (ticks > 0)
	{
		short class_index;

		for (class_index = 0; class_index < NUMBER_OF_SOUND_CLASSES; class_index++)
		{
			struct sound_class_runtime *sound_class = sound_class_datum_get(class_index);
			if (sound_class->interpolation_ticks > ticks)
			{
				sound_class->current_gain =
					(real)ticks / sound_class->interpolation_ticks *
					(sound_class->target_gain - sound_class->current_gain) +
					sound_class->current_gain;
				sound_class->interpolation_ticks -= ticks;
			}
			else
			{
				sound_class->current_gain = sound_class->target_gain;
				sound_class->interpolation_ticks = 0;
			}
		}
	}

	return;
}

void debug_sound_classes_set_wet(
	char const *name,
	real wet)
{
	short class_index;
	char const **sound_class_name;

	class_index = 0;
	sound_class_name = sound_class_names;
	do
	{
		if ((*sound_class_name)[0] && strstr(*sound_class_name, name))
		{
			real wet_gain = PIN(1.f - wet, 0.f, 1.f);

			sound_class_get(class_index)->wet_gain = wet_gain;
		}
		class_index++;
		sound_class_name++;
	}
	while (class_index < NUMBER_OF_SOUND_CLASSES);

	return;
}

void sound_class_set_gain(
	char const *name,
	real gain,
	short interpolation_ticks)
{
	short class_index;
	char const **sound_class_name;

	class_index = 0;
	sound_class_name = sound_class_names;
	do
	{
		if ((*sound_class_name)[0] && strstr(*sound_class_name, name))
		{
			struct sound_class_runtime *sound_class = sound_class_datum_get(class_index);

			sound_class->target_gain = PIN(gain, 0.f, 1.f);
			sound_class->interpolation_ticks =
				interpolation_ticks < 0 ? 0 : interpolation_ticks;
		}
		class_index++;
		sound_class_name++;
	}
	while (class_index < NUMBER_OF_SOUND_CLASSES);

	return;
}

/* ---------- private code */
