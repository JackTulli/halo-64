/*
UNIT_DIALOGUE.C

symbols in this file:
001961B0 02f0:
	_unit_test_speech (0000)
001964A0 0030:
	_unit_is_speaking (0000)
001964D0 00b0:
	_unit_dialogue_determine_variant (0000)
00196580 0030:
	_unit_get_speech_priority_name (0000)
001965B0 0040:
	_unit_get_speech_priority_by_name (0000)
001965F0 0110:
	_unit_describe_speech (0000)
00196700 00d0:
	_code_00196700 (0000)
001967D0 01e0:
	_unit_speak (0000)
001969B0 00f0:
	_unit_notify_impulse_sound (0000)
00196AA0 0310:
	_unit_make_damage_sound (0000)
00196DB0 0180:
	_unit_scream (0000)
00196F30 00e0:
	_code_00196f30 (0000)
00197010 0060:
	_code_00197010 (0000)
00197070 0300:
	_unit_dialogue_update (0000)
002A4AF4 0044:
	_global_speech_override_priorities (0000)
	_global_speech_queue_times (0018)
002A4B38 0007:
	??_C@_06CHNPJHKA@scream?$AA@ (0000)
002A4B40 0005:
	??_C@_04PPONMOMN@pain?$AA@ (0000)
002A4B48 004f:
	??_C@_0EP@HLLDDEB@?$CIvocalization_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIvoc@ (0000)
002A4B98 0041:
	??_C@_0EB@BEHAGLIC@?$CIpriority?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIpriority?5?$DM?5N@ (0000)
002A4BDC 0021:
	??_C@_0CB@KLBJPFCB@sound_definition_index_reference@ (0000)
002A4C00 001c:
	??_C@_0BM@JBKHEHDK@vocalization_type_reference?$AA@ (0000)
002A4C1C 0025:
	??_C@_0CF@GDJDFIKG@c?3?2halo?2SOURCE?2units?2unit_dialog@ (0000)
002A4C44 0039:
	??_C@_0DJ@FLLOGMMH@unit_dialogue_determine_variant?5@ (0000)
002A4C80 0016:
	??_C@_0BG@NPMCLPNN@?$CFs?3?5lost?5?$CFs?5speech?5?$CFs?$AA@ (0000)
002A4C98 0007:
	??_C@_06DBNNGIJH@queued?$AA@ (0000)
002A4CA0 0008:
	??_C@_07BMPLEEBA@waiting?$AA@ (0000)
002A4CA8 000c:
	??_C@_0M@PNDDBCJG@speech_item?$AA@ (0000)
002A4CB4 0037:
	??_C@_0DH@LMOPNAFJ@unit?9?$DOunit?4speech?4current?4priori@ (0000)
002A4CEC 0021:
	??_C@_0CB@FIDKMHLI@AI_BEHAVIOR?$CIforce_vocalizations?$CJ@ (0000)
002A4D10 004b:
	??_C@_0EL@PILBDJLD@unit?9?$DOunit?4speech?4current?4sound_@ (0000)
002A4D60 0042:
	??_C@_0EC@EADCCPEN@?$CIscream_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIscream_ty@ (0000)
002A4DA4 0028:
	??_C@_0CI@HNAKBIJH@dialogue_definition_get?$CIdialogue@ (0000)
00315BD8 002c:
	_global_speech_priority_names (0000)
004CCB14 0004:
	_bss_004ccb14 (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "units.h"

#include "ai/ai_communication.h"
#include "ai/ai_debug.h"
#include "ai/ai_runtime.h"
#include "ai/actors.h"
#include "dialogue_definitions.h"
#include "game/game.h"
#include "game/game_globals.h"
#include "main/console.h"
#include "objects/damage.h"
#include "objects/damage_effect_definitions.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"
#include "tag_files/tag_groups.h"
#include "unit_definitions.h"

/* ---------- constants */

enum unit_play_speech_type
{
	_unit_play_speech_none = 0,
	_unit_play_speech_queue,
	_unit_play_speech_immediate,
	_unit_play_speech_immediate_dequeue,
};

enum unit_dialogue_vocalization_type
{
	_vocalization_pain_body = 6,
	_vocalization_pain_body_major = 7,
	_vocalization_pain_shield = 8,
	_vocalization_pain_falling = 9,
	_vocalization_scream_fear = 10,
	_vocalization_scream_pain = 11,
	_vocalization_maimed_limb = 12,
	_vocalization_maimed_head = 13,
	_vocalization_death_quiet = 14,
	_vocalization_death_violent = 15,
	_vocalization_death_falling = 16,
	_vocalization_death_agonizing = 17,
	_vocalization_death_instant = 18,
	_vocalization_death_flying = 19,
	_vocalization_hurt_enemy_grenade = 39,
	_vocalization_resurrect = 183,
};

enum unit_dialogue_damage_category
{
	_unit_dialogue_damage_category_none = 0,
	_unit_dialogue_damage_category_falling = 1,
	_unit_dialogue_damage_category_flame = 7,
};

enum unit_dialogue_actor_combat_status
{
	_unit_dialogue_actor_combat_status_definite = 3,
};

enum unit_dialogue_ai_unit_effect
{
	_unit_dialogue_ai_unit_effect_death_scream = 2,
};

/* ---------- macros */

#define AI_BEHAVIOR(field) \
	(game_connection() == _game_connection_local && ai_debug.field)
#define NUMBER_OF_VOCALIZATION_TYPES NUMBER_OF_DIALOGUE_VOCALIZATION_TYPES

/* ---------- structures */

struct dialogue_definition
{
	short vocalization_enum_version;
	word pad;
	long unused[3];
	struct tag_reference vocalizations[NUMBER_OF_DIALOGUE_VOCALIZATION_TYPES];
	struct tag_reference unused_vocalizations[47];
};

/* ---------- prototypes */

static long unit_find_dialogue_variant(
	struct unit_definition *definition,
	short variant_number);
static void unit_dialogue_setup(
	long unit_index);
static void unit_lose_speech(
	long unit_index,
	short play_type,
	struct unit_speech_item const *speech_item);

/* ---------- globals */

extern short const dialogue_vocalization_lookup[NUMBER_OF_DIALOGUE_VOCALIZATION_TYPES];

char const *global_speech_priority_names[NUMBER_OF_UNIT_SPEECH_PRIORITIES] =
{
	"none",
	"idle",
	"pain",
	"talk",
	"communicate",
	"shout",
	"script",
	"involuntary",
	"exclaim",
	"scream",
	"death",
};

short const global_speech_override_priorities[NUMBER_OF_UNIT_SPEECH_PRIORITIES] =
{
	_unit_speech_none,
	_unit_speech_none,
	_unit_speech_idle,
	_unit_speech_idle,
	_unit_speech_pain,
	_unit_speech_pain,
	_unit_speech_shout,
	_unit_speech_shout,
	_unit_speech_involuntary,
	_unit_speech_involuntary,
	_unit_speech_death,
};

real const global_speech_queue_times[NUMBER_OF_UNIT_SPEECH_PRIORITIES] =
{
	0.0f,
	0.0f,
	0.0f,
	1.5f,
	3.0f,
	4.0f,
	REAL_MAX,
	3.0f,
	3.0f,
	3.0f,
	3.0f,
};

static long sequential_counter;

/* ---------- public code */

short unit_test_speech(
	long unit_index,
	short priority,
	boolean allow_recursive_lookup,
	boolean allow_queue,
	long *unit_last_speech_time,
	short *vocalization_type_reference,
	long *sound_definition_index_reference)
{
	struct unit_datum *unit = unit_get(unit_index);
	short vocalization_type;
	short result;
	long sound_definition_index;
	boolean can_queue;
	real queue_time;

	game_time_get();
	result = _unit_play_speech_none;

	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		128,
		vocalization_type_reference);
	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		129,
		sound_definition_index_reference);
	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		130,
		(priority >= 0) && (priority < NUMBER_OF_UNIT_SPEECH_PRIORITIES));

	vocalization_type = *vocalization_type_reference;
	sound_definition_index = *sound_definition_index_reference;

	if (sound_definition_index == NONE &&
		unit->unit.dialogue_index != NONE &&
		vocalization_type != NONE)
	{
		struct dialogue_definition *dialogue_definition =
			dialogue_definition_get(unit->unit.dialogue_index);

		do
		{
			match_assert(
				"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
				144,
				(vocalization_type >= 0) &&
					(vocalization_type < NUMBER_OF_VOCALIZATION_TYPES));

			sound_definition_index =
				dialogue_definition->vocalizations[vocalization_type].index;

			if (!allow_recursive_lookup ||
				sound_definition_index != NONE ||
				AI_BEHAVIOR(force_vocalizations))
			{
				break;
			}

			vocalization_type = dialogue_vocalization_lookup[vocalization_type];
		}
		while (vocalization_type != NONE);
	}

	if ((!TEST_FLAG(unit->object.damage_flags, _object_dead_bit) ||
		priority == _unit_speech_death) &&
		(AI_BEHAVIOR(force_vocalizations) || sound_definition_index != NONE))
	{
		short current_priority = unit->unit.speech.current.priority;

		if (current_priority == _unit_speech_none)
		{
			result = _unit_play_speech_immediate;
		}
		else
		{
			short queued_priority = unit->unit.speech.queued.priority;
			short highest_priority = MAX(current_priority, queued_priority);
			long priority_index = priority;

			switch (priority_index)
			{
			case _unit_speech_pain:
			case _unit_speech_involuntary:
			case _unit_speech_death:
				if (unit->unit.speech.played &&
					unit->unit.speech.sound_timer == 0 &&
					priority > highest_priority)
				{
					current_priority = _unit_speech_none;
					highest_priority = queued_priority;
				}
				break;
			}

			if (global_speech_override_priorities[priority_index] >= highest_priority)
			{
				result = _unit_play_speech_immediate_dequeue;
			}
			else if (priority >= _unit_speech_involuntary &&
				global_speech_override_priorities[priority_index] >= current_priority)
			{
				result = _unit_play_speech_immediate;
			}
			else if (allow_queue)
			{
				queue_time = global_speech_queue_times[priority_index];

				if (queue_time != 0.0f)
				{
					if (queue_time == REAL_MAX)
					{
						can_queue = TRUE;
					}
					else
					{
						can_queue =
							unit->unit.speech.post_delay_timer +
								unit->unit.speech.sound_timer <
							(short)(queue_time * TICKS_PER_SECOND);
					}

					if (can_queue)
					{
						if (priority <= highest_priority)
						{
							if (priority <= queued_priority)
								can_queue = FALSE;
							else
							{
								switch (current_priority)
								{
								case _unit_speech_pain:
								case _unit_speech_involuntary:
									can_queue = TRUE;
									break;
								}
								if (priority == _unit_speech_scripted)
									can_queue = TRUE;
							}
						}
						else
						{
							can_queue = TRUE;
						}

						if (can_queue)
							result = _unit_play_speech_queue;
					}
				}
			}
		}
	}

	*vocalization_type_reference = vocalization_type;
	*sound_definition_index_reference = sound_definition_index;
	if (unit_last_speech_time)
		*unit_last_speech_time = unit->unit.speech.last_speech_finished_time;

	return result;
}

boolean unit_is_speaking(
	long unit_index)
{
	return unit_get(unit_index)->unit.speech.current.priority > 0;
}

void unit_dialogue_determine_variant(
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	struct unit_definition *definition = unit_definition_get(unit->definition_index);

	if (unit->object.variant_number == 0)
	{
		short variants[16];
		short variant_count = 0;
		short variant_index;

		for (variant_index = 0; variant_index < definition->unit.dialogue_variants.count; variant_index++)
		{
			struct unit_dialogue_variant *variant = TAG_BLOCK_GET_ELEMENT(
				&definition->unit.dialogue_variants,
				variant_index,
				struct unit_dialogue_variant);

			if (variant->variant_number < 100)
			{
				if (variant_count >= NUMBEROF(variants))
				{
					error(_error_silent, "unit_dialogue_determine_variant overflowed variant array");
					break;
				}

				variants[variant_count++] = variant->variant_number;
			}
		}

		if (variant_count > 0)
		{
			unit->object.variant_number = variants[sequential_counter % variant_count];
			sequential_counter++;
		}
	}

	return;
}

char const *unit_get_speech_priority_name(
	short priority)
{
	char const *name = "<error>";

	if (priority >= _unit_speech_none && priority < NUMBER_OF_UNIT_SPEECH_PRIORITIES)
		name = global_speech_priority_names[priority];

	return name;
}

short unit_get_speech_priority_by_name(
	char const *name)
{
	short result = _unit_speech_none;
	short priority;

	for (priority = _unit_speech_none; priority < NUMBER_OF_UNIT_SPEECH_PRIORITIES; priority++)
	{
		if (csstrcmp(global_speech_priority_names[priority], name) == 0)
		{
			result = priority;
			break;
		}
	}

	return result;
}

char const *unit_describe_speech(
	long unit_index,
	boolean abbreviated,
	long buffer_size,
	char *buffer)
{
	struct unit_datum *unit = unit_get(unit_index);
	char *sound_name;
	char *separator;
	char *scan;

	if (unit->unit.speech.current.priority == _unit_speech_none)
	{
		_snprintf(buffer, (short)buffer_size, "<none>");
		return buffer;
	}

	sound_name = "<none>";
	if (unit->unit.speech.current.sound_definition_index != NONE)
	{
		sound_name = tag_get_name(
			unit->unit.speech.current.sound_definition_index);
	}

	if (abbreviated)
	{
		boolean done = FALSE;

		scan = sound_name;
		while (!done)
		{
			if (scan)
			{
				scan = strchr(scan, '\\');
				if (scan)
				{
					scan++;
					sound_name = scan;
				}
				else
				{
					done = TRUE;
				}
			}
			else
			{
				done = TRUE;
			}
		}
	}
	else
	{
		separator = strrchr(sound_name, '\\');
		if (separator)
			sound_name = separator + 1;
	}

	if (unit->unit.speech.current.vocalization_type == NONE)
	{
		_snprintf(buffer, (short)buffer_size, "%s", sound_name);
	}
	else if (abbreviated)
	{
		_snprintf(
			buffer,
			(short)buffer_size,
			"%s %s",
			dialogue_get_vocalization_name(
				unit->unit.speech.current.vocalization_type,
				FALSE),
			sound_name);
	}
	else
	{
		_snprintf(
			buffer,
			(short)buffer_size,
			"%s",
			dialogue_get_vocalization_name(
				unit->unit.speech.current.vocalization_type,
				FALSE));
	}

	return buffer;
}

static void unit_lose_speech(
	long unit_index,
	short play_type,
	struct unit_speech_item const *speech_item)
{
	struct unit_datum *unit = unit_get(unit_index);

	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		1045,
		speech_item);

	if (ai_debug.print_lost_speech)
	{
		struct unit_definition *definition = unit_definition_get(unit->definition_index);
		char const *speech_name;

		if (speech_item->vocalization_type != NONE)
		{
			speech_name = dialogue_get_vocalization_name(
				speech_item->vocalization_type,
				FALSE);
		}
		else if (speech_item->sound_definition_index != NONE)
		{
			char const *separator;

			speech_name = tag_get_name(speech_item->sound_definition_index);
			separator = strrchr(speech_name, '\\');
			if (separator)
				speech_name = separator + 1;
		}
		else
		{
			speech_name = "<unknown>";
		}

		console_printf(
			FALSE,
			"%s: lost %s speech %s",
			definition->object.model.name,
			play_type == _unit_play_speech_immediate ? "waiting" : "queued",
			speech_name);
	}

	return;
}

void unit_speak(
	long unit_index,
	short play_type,
	struct unit_speech_item const *speech_item)
{
	struct unit_datum *unit = unit_get(unit_index);

	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		302,
		speech_item);

	if (TEST_FLAG(unit->object.damage_flags, _object_dead_bit) &&
		speech_item->priority != _unit_speech_death)
	{
		return;
	}

	if (play_type >= _unit_play_speech_immediate)
	{
		if (unit->unit.speech.current.priority > _unit_speech_none &&
			!unit->unit.speech.played)
		{
			unit_lose_speech(
				unit_index,
				_unit_play_speech_immediate,
				&unit->unit.speech.current);
		}

		unit->unit.speech.current = *speech_item;

		if (play_type == _unit_play_speech_immediate_dequeue &&
			unit->unit.speech.queued.priority > _unit_speech_none)
		{
			if (speech_item != &unit->unit.speech.queued)
			{
				unit_lose_speech(
					unit_index,
					_unit_play_speech_queue,
					&unit->unit.speech.queued);
			}
			unit->unit.speech.queued.priority = _unit_speech_none;
		}

		unit->unit.speech.pre_delay_timer = unit->unit.speech.current.delay_time;
		unit->unit.speech.played = FALSE;
		unit->unit.speech.notified_ai = FALSE;
		unit->unit.speech.finished = FALSE;
		unit->unit.speech.impulse_sound_index = NONE;
		unit->unit.speech.post_delay_timer = unit->unit.speech.current.pause_time;
		unit->unit.speech.ai_delay_timer =
			unit->unit.speech.current.ai_notification_delay;

		if (unit->unit.speech.current.sound_definition_index != NONE)
		{
#ifdef HALO_N64
			/* (the N64's sound tags are records of its own:
			n64_sound_game.c) */
			long n64_sound_definition_length(long sound_index);

			unit->unit.speech.sound_timer = (short)(
				n64_sound_definition_length(unit->unit.speech.current.sound_definition_index) *
				TICKS_PER_SECOND / 1000);
#else
			struct sound_definition *sound_definition =
				sound_definition_get(unit->unit.speech.current.sound_definition_index);
			unit->unit.speech.sound_timer = (short)(
				(long)(sound_definition->longest_permutation_length * TICKS_PER_SECOND) / 1000);
#endif
		}
		else
		{
			match_assert(
				"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
				360,
				AI_BEHAVIOR(force_vocalizations));
			unit->unit.speech.sound_timer = 45;
		}
	}
	else if (play_type == _unit_play_speech_queue)
	{
		match_assert(
			"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
			366,
			unit->unit.speech.current.priority > _unit_speech_none);
		unit->unit.speech.queued = *speech_item;
	}

	return;
}

void unit_notify_impulse_sound(
	long unit_index,
	long sound_definition_index,
	long impulse_sound_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	struct unit_speech_item speech_item;
	short vocalization_type = NONE;
	long test_sound_definition_index = sound_definition_index;
	short play_type = unit_test_speech(
		unit_index,
		_unit_speech_scripted,
		FALSE,
		FALSE,
		NULL,
		&vocalization_type,
		&test_sound_definition_index);

	play_type = MAX(play_type, _unit_play_speech_immediate);

	csmemset(&speech_item, 0, sizeof(speech_item));
	speech_item.priority = _unit_speech_scripted;
	speech_item.vocalization_type = NONE;
	speech_item.sound_definition_index = sound_definition_index;
	speech_item.pause_time = 24;
	ai_communication_packet_new(&speech_item.ai);
	unit_speak(unit_index, play_type, &speech_item);

	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		406,
		unit->unit.speech.current.sound_definition_index == sound_definition_index);

	unit->unit.speech.impulse_sound_index = impulse_sound_index;
	unit->unit.speech.played = TRUE;
	unit->unit.speech.pre_delay_timer = 0;
	ai_communication_started(
		unit_index,
		_unit_speech_scripted,
		NONE,
		&unit->unit.speech.current.ai);

	return;
}

boolean unit_make_damage_sound(
	long unit_index,
	struct damage_data *damage_data,
	boolean died,
	boolean died_instantly,
	real body_damage,
	real shield_damage)
{
	struct unit_datum *unit = unit_get(unit_index);
	boolean spoke = FALSE;

	(void)shield_damage;

	if (unit->unit.dialogue_index != NONE)
	{
		short vocalization_type = NONE;
		short damage_category = _unit_dialogue_damage_category_none;
		boolean took_body_damage = unit->object.recent_body_damage > 0.f;
		boolean took_major_body_damage = unit->object.recent_body_damage >= 0.6f;
		short unit_effect_type = NONE;
		short unit_effect_volume = 0;
		boolean involuntary_vocalization = FALSE;

		if (damage_data && damage_data->definition_index != NONE)
		{
			damage_category = damage_effect_definition_get(
				damage_data->definition_index)->damage.category;
		}

		if (died)
		{
			long actor_index = unit->unit.swarm_actor_index != NONE ?
				unit->unit.swarm_actor_index :
				unit->unit.actor_index;
			boolean severe = FALSE;
			boolean instantaneous = FALSE;

			if (damage_data->definition_index != NONE)
			{
				instantaneous = damage_effect_definition_get(
					damage_data->definition_index)->damage.instantaneous_acceleration >= 2.f;
			}

			if (actor_index != NONE)
			{
				struct actor_datum *actor = actor_get(actor_index);

				severe = actor->state.combat_status >=
					_unit_dialogue_actor_combat_status_definite;
			}
			else if (unit->object.recent_body_damage > body_damage + 0.2f)
			{
				severe = TRUE;
			}

			if (damage_category == _unit_dialogue_damage_category_falling)
			{
				vocalization_type = _vocalization_death_falling;
			}
			else if (damage_category == _unit_dialogue_damage_category_flame)
			{
				vocalization_type = _vocalization_death_agonizing;
			}
			else if (instantaneous)
			{
				vocalization_type = _vocalization_death_flying;
			}
			else if (!severe)
			{
				vocalization_type = _vocalization_death_quiet;
			}
			else
			{
				vocalization_type = died_instantly ?
					_vocalization_death_instant :
					_vocalization_death_violent;
			}

			involuntary_vocalization = TRUE;
			if (vocalization_type != _vocalization_death_quiet)
			{
				unit_effect_type = _unit_dialogue_ai_unit_effect_death_scream;
				unit_effect_volume = vocalization_type == _vocalization_death_instant ? 4 : 1;
			}
		}
		else if (unit->unit.speech.damage_major_timer == 0)
		{
			if (damage_category == _unit_dialogue_damage_category_falling)
			{
				involuntary_vocalization = TRUE;
				vocalization_type = _vocalization_pain_falling;
			}
			else if (took_major_body_damage)
			{
				involuntary_vocalization = TRUE;
				vocalization_type = _vocalization_pain_body_major;
			}
			else if (unit->unit.speech.damage_minor_timer == 0 &&
				unit->unit.speech.damage_minor_sounds < 3 &&
				(unit->unit.speech.current.priority == _unit_speech_none ||
					real_seed_random(get_global_random_seed_address()) < 0.4f))
			{
				vocalization_type = took_body_damage ?
					_vocalization_pain_body :
					_vocalization_pain_shield;
			}
		}

		if (vocalization_type != NONE)
		{
			long sound_definition_index = NONE;
			short priority = died ?
				_unit_speech_death :
				(involuntary_vocalization ? _unit_speech_involuntary : _unit_speech_pain);
			short play_type = unit_test_speech(
				unit_index,
				priority,
				TRUE,
				FALSE,
				NULL,
				&vocalization_type,
				&sound_definition_index);

			if (!ai_debug.disable_wounded_sounds &&
				play_type > _unit_play_speech_none)
			{
				struct unit_speech_item speech_item;

				csmemset(&speech_item, 0, sizeof(speech_item));
				speech_item.priority = priority;
				speech_item.vocalization_type = vocalization_type;
				speech_item.sound_definition_index = sound_definition_index;
				speech_item.pause_time = 7;
				ai_communication_packet_new(&speech_item.ai);
				unit_speak(unit_index, play_type, &speech_item);
				spoke = TRUE;

				if (took_major_body_damage)
				{
					unit->unit.speech.damage_major_timer = 60;
				}
				else
				{
					unit->unit.speech.damage_minor_sounds++;
					unit->unit.speech.damage_minor_timer = 30;
					unit->unit.speech.damage_minor_decay_timer = 22;
				}
			}
		}

		if (unit_effect_type != NONE)
		{
			ai_handle_unit_effect(
				unit_index,
				unit_effect_type,
				unit_effect_volume);
		}
	}

	return spoke;
}

boolean unit_scream(
	long unit_index,
	short scream_type)
{
	struct unit_datum *unit = unit_get(unit_index);
	short vocalization_type;

	match_assert(
		"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
		599,
		(scream_type >= 0) && (scream_type < NUMBER_OF_UNIT_SCREAM_TYPES));

	switch (scream_type)
	{
	case _unit_scream_falling:
		vocalization_type = _vocalization_scream_fear;
		break;
	case _unit_scream_grenade_attached_to_us:
		if (real_seed_random(get_global_random_seed_address()) < 0.5f)
			vocalization_type = _vocalization_hurt_enemy_grenade;
		else
			vocalization_type = _vocalization_scream_pain;
		break;
	case _unit_scream_burning_to_death:
		vocalization_type = _vocalization_scream_pain;
		break;
	case _unit_scream_destroyed_limb:
		vocalization_type = _vocalization_maimed_limb;
		break;
	case _unit_scream_destroyed_head:
		vocalization_type = _vocalization_maimed_head;
		break;
	case _unit_scream_resurrection:
		vocalization_type = _vocalization_resurrect;
		break;
	/* vocalization_type is left unassigned only by this default arm. Not reached unassigned: the
	 * arm's assertion failure calls system_exit, which does not return in January
	 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
	 * Source-policy approval pending (2026-09-27 audit). */
	default:
		match_assert(
			"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
			635,
			!"unreachable");
		break;
	}

	if (unit->unit.dialogue_index != NONE)
	{
		struct dialogue_definition *dialogue_definition =
			dialogue_definition_get(unit->unit.dialogue_index);
		long sound_definition_index =
			dialogue_definition->vocalizations[vocalization_type].index;

		if (sound_definition_index != NONE)
		{
			short play_type = unit_test_speech(
				unit_index,
				_unit_speech_scream,
				TRUE,
				FALSE,
				NULL,
				&vocalization_type,
				&sound_definition_index);

			if (play_type > _unit_play_speech_none)
			{
				struct unit_speech_item speech_item;

				csmemset(&speech_item, 0, sizeof(speech_item));
				speech_item.priority = _unit_speech_scream;
				speech_item.vocalization_type = vocalization_type;
				speech_item.sound_definition_index = sound_definition_index;
				speech_item.pause_time = 7;
				ai_communication_packet_new(&speech_item.ai);
				unit_speak(unit_index, play_type, &speech_item);
				return TRUE;
			}
		}
	}

	return FALSE;
}

#ifdef HALO_N64
/* a vocalization the N64's Firefight AI asks a unit to say (Halo's AI,
left out, does it through ai_dialogue.c): TRUE when it speaks */
boolean unit_vocalize(
	long unit_index,
	short vocalization_type,
	short priority)
{
	struct unit_datum *unit = unit_get(unit_index);
	long sound_definition_index = NONE;
	short play_type;

	if (unit->unit.dialogue_index == NONE ||
		vocalization_type < 0 ||
		vocalization_type >= NUMBER_OF_VOCALIZATION_TYPES)
	{
		return FALSE;
	}
	play_type = unit_test_speech(
		unit_index,
		priority,
		FALSE,
		FALSE,
		NULL,
		&vocalization_type,
		&sound_definition_index);
	if (play_type > _unit_play_speech_none && sound_definition_index != NONE)
	{
		struct unit_speech_item speech_item;

		csmemset(&speech_item, 0, sizeof(speech_item));
		speech_item.priority = priority;
		speech_item.vocalization_type = vocalization_type;
		speech_item.sound_definition_index = sound_definition_index;
		speech_item.pause_time = 15;
		ai_communication_packet_new(&speech_item.ai);
		unit_speak(unit_index, play_type, &speech_item);
		return TRUE;
	}

	return FALSE;
}
#endif

void unit_dialogue_update(
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);

	if (TEST_FLAG(unit->unit.flags, _unit_must_set_up_dialogue_bit))
	{
		unit_dialogue_setup(unit_index);
		SET_FLAG(unit->unit.flags, _unit_must_set_up_dialogue_bit, FALSE);
	}

	if (unit->unit.speech.damage_minor_decay_timer > 0 && --unit->unit.speech.damage_minor_decay_timer == 0)
	{
		if (unit->unit.speech.damage_minor_sounds > 0)
		{
			unit->unit.speech.damage_minor_sounds--;
			unit->unit.speech.damage_minor_decay_timer = 22;
		}
	}

	if (unit->unit.speech.damage_minor_timer > 0)
		unit->unit.speech.damage_minor_timer--;
	/* BUG (preserved for exact matching): January decrements damage_minor_timer
	 * (+0x39C) a second time here and never touches damage_major_timer (+0x39E,
	 * the field unit_make_damage_sound tests against zero and sets to 60), so a
	 * unit's major pain vocalization is suppressed for good once one has played.
	 * A corrected build should decrement damage_major_timer in this statement.
	 */
	if (unit->unit.speech.damage_minor_timer > 0)
		unit->unit.speech.damage_minor_timer--;

	if (unit->unit.speech.current.priority > _unit_speech_none)
	{
		if (unit->unit.speech.pre_delay_timer > 0)
		{
			unit->unit.speech.pre_delay_timer--;
		}
		else
		{
			if (!unit->unit.speech.played)
			{
				struct object_marker marker;
				real_point3d position;
				real_vector3d forward;
				short node_index;

				if (object_get_marker_by_name(unit_index, "head", &marker, 1))
				{
					node_index = marker.node_index;
					position = marker.node_matrix.position;
					forward = marker.node_matrix.forward;
				}
				else
				{
					node_index = 0;
					position = *global_origin3d;
					forward = *global_forward3d;
				}

				if (unit->unit.speech.current.sound_definition_index != NONE)
				{
					unit->unit.speech.impulse_sound_index = object_impulse_sound_new(
						unit_index,
						unit->unit.speech.current.sound_definition_index,
						node_index,
						&position,
						&forward,
						1.0f);
				}

				ai_communication_started(
					unit_index,
					unit->unit.speech.current.priority,
					unit->unit.speech.current.vocalization_type,
					&unit->unit.speech.current.ai);
				unit->unit.speech.played = TRUE;
			}

			if (unit->unit.speech.ai_delay_timer > 0)
				unit->unit.speech.ai_delay_timer--;

			if (unit->unit.speech.sound_timer > 0)
			{
				match_assert(
					"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
					757,
					unit->unit.speech.current.priority > _unit_speech_none);
				if (--unit->unit.speech.sound_timer == 0)
					unit->unit.speech.impulse_sound_index = NONE;
			}
			else
			{
				if (!unit->unit.speech.finished)
				{
					ai_communication_finished(
						unit_index,
						unit->unit.speech.current.priority,
						unit->unit.speech.current.vocalization_type,
						FALSE,
						NONE,
						&unit->unit.speech.current.ai);
					unit->unit.speech.finished = TRUE;
				}

				if (unit->unit.speech.post_delay_timer > 0)
					unit->unit.speech.post_delay_timer--;
				if (unit->unit.speech.post_delay_timer == 0)
					unit->unit.speech.ai_delay_timer = 0;
			}
		}
	}

	if (unit->unit.speech.ai_delay_timer == 0 && !unit->unit.speech.notified_ai)
	{
		ai_communication_notify(
			unit_index,
			unit->unit.speech.current.priority,
			unit->unit.speech.current.vocalization_type,
			&unit->unit.speech.current.ai);
		unit->unit.speech.notified_ai = TRUE;
	}

	if (unit->unit.speech.current.priority > _unit_speech_none &&
		unit->unit.speech.sound_timer == 0 &&
		unit->unit.speech.post_delay_timer == 0)
	{
		unit->unit.speech.current.priority = _unit_speech_none;
	}

	if (unit->unit.speech.current.priority == _unit_speech_none &&
		unit->unit.speech.queued.priority > _unit_speech_none)
	{
		unit_speak(unit_index, _unit_play_speech_immediate_dequeue, &unit->unit.speech.queued);
	}

	return;
}

/* ---------- private code */

static long unit_find_dialogue_variant(
	struct unit_definition *definition,
	short variant_number)
{
	short variant_indices[16];
	long dialogue_index = NONE;
	short variant_count = 0;
	short variant_index;

	for (variant_index = 0; variant_index < definition->unit.dialogue_variants.count; variant_index++)
	{
		struct unit_dialogue_variant *variant = TAG_BLOCK_GET_ELEMENT(
			&definition->unit.dialogue_variants,
			variant_index,
			struct unit_dialogue_variant);

		if (variant_number == NONE || variant->variant_number == variant_number)
			variant_indices[variant_count++] = variant_index;
	}

	if (variant_count > 0)
	{
		short selected_variant_index;
		struct unit_dialogue_variant *selected_variant;

		if (variant_count == 1)
			selected_variant_index = variant_indices[0];
		else
		{
			selected_variant_index = variant_indices[seed_random_range(
				get_global_random_seed_address(),
				0,
				variant_count)];
		}

		selected_variant = TAG_BLOCK_GET_ELEMENT(
			&definition->unit.dialogue_variants,
			selected_variant_index,
			struct unit_dialogue_variant);
		dialogue_index = selected_variant->dialogue_index;
		match_assert(
			"c:\\halo\\SOURCE\\units\\unit_dialogue.c",
			1032,
			dialogue_definition_get(dialogue_index));

	}

	return dialogue_index;
}

static void unit_dialogue_setup(
	long unit_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	struct unit_definition *definition = unit_definition_get(unit->definition_index);
	short variant_number = unit->object.variant_number;
	long dialogue_index = NONE;

	if (variant_number > 0)
		dialogue_index = unit_find_dialogue_variant(definition, variant_number);

	if (dialogue_index == NONE)
	{
		dialogue_index = unit_find_dialogue_variant(definition, 0);
		if (dialogue_index == NONE)
			dialogue_index = unit_find_dialogue_variant(definition, NONE);
	}

	unit->unit.dialogue_index = dialogue_index;

	return;
}
