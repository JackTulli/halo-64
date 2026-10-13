/*
GAME_ENGINE_MULTIPLAYER_SOUNDS.C

symbols in this file:
000A1460 0060:
	_code_000a1460 (0000)
000A14C0 0020:
	_code_000a14c0 (0000)
000A14E0 0050:
	_game_engine_update_multiplayer_sound (0000)
000A1530 0080:
	_code_000a1530 (0000)
000A15B0 0050:
	_game_engine_play_multiplayer_sound (0000)
000A1600 0030:
	_game_engine_intialize_queued_sounds (0000)
002DE530 002b:
	_data_002de530 (0000)
0043EB78 002c:
	_bss_0043eb78 (0000)
*/

/* ---------- headers */

#include "cseries.h"

#include "game/game_globals.h"
#include "objects/objects.h"
#include "scenario/scenario.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"

/* ---------- constants */

enum multiplayer_information_sound
{
	_multiplayer_sound_play_ball,
	_multiplayer_sound_game_over,
	_multiplayer_sound_one_minute_to_win,
	_multiplayer_sound_30_seconds_to_win,
	_multiplayer_sound_red_team_minute_to_win,
	_multiplayer_sound_red_team_30_seconds_to_win,
	_multiplayer_sound_blue_team_minute_to_win,
	_multiplayer_sound_blue_team_30_seconds_to_win,
	_multiplayer_sound_blue_team_has_the_flag,
	_multiplayer_sound_blue_team_flag_returned,
	_multiplayer_sound_blue_team_score,
	_multiplayer_sound_red_team_has_the_flag,
	_multiplayer_sound_red_team_flag_returned,
	_multiplayer_sound_red_team_score,
	_multiplayer_sound_double_kill,
	_multiplayer_sound_triple_kill,
	_multiplayer_sound_killtacular,
	_multiplayer_sound_running_riot,
	_multiplayer_sound_killing_spree,
	_multiplayer_sound_oddball,
	_multiplayer_sound_race,
	_multiplayer_sound_slayer,
	_multiplayer_sound_capture_the_flag,
	_multiplayer_sound_warthog,
	_multiplayer_sound_ghost,
	_multiplayer_sound_scorpion,
	_multiplayer_sound_countdown_timer,
	_multiplayer_sound_teleporter_activate,
	_multiplayer_sound_flag_failure,
	_multiplayer_sound_countdown_for_respawn,
	_multiplayer_sound_hill_move,
	_multiplayer_sound_player_respawn,
	_multiplayer_sound_team_king_of_the_hill,
	_multiplayer_sound_team_oddball,
	_multiplayer_sound_team_race,
	_multiplayer_sound_team_slayer,
	_multiplayer_sound_king_of_the_hill,
	_multiplayer_sound_blue_team_ctf,
	_multiplayer_sound_red_team_ctf,
	_multiplayer_sound_hill_contested,
	_multiplayer_sound_hill_controlled,
	_multiplayer_sound_hill_occupied,
	_multiplayer_sound_countdown_timer_end,
	_multiplayer_sound_ting,
	NUMBER_OF_MULTIPLAYER_INFORMATION_SOUNDS
};

enum
{
	MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS = 5,
	MULTIPLAYER_SOUND_QUEUE_INITIAL_DELAY_TICKS = 2 * TICKS_PER_SECOND,
};

/* ---------- macros */

/* ---------- structures */

struct queued_multiplayer_sound
{
	long sound_index;
	long delay_ticks;
};

struct multiplayer_sound_queue
{
	long count;
	struct queued_multiplayer_sound sounds[MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS];
};

struct multiplayer_sound_queue_count
{
	long count;
};

struct game_globals_multiplayer_sound_view
{
	byte unused[0x164];
	struct tag_block multiplayer_information;
};

typedef char verify_multiplayer_sound_queue_size[sizeof(struct multiplayer_sound_queue) == 0x2C ? 1 : -1];
typedef char verify_multiplayer_sound_queue_count_size[sizeof(struct multiplayer_sound_queue_count) == sizeof(long) ? 1 : -1];
typedef char verify_game_globals_multiplayer_information_offset[
	offsetof(struct game_globals_multiplayer_sound_view, multiplayer_information) == 0x164 ? 1 : -1];

/* ---------- prototypes */

/* game_engine_play_multiplayer_sound_immediate */
static void _game_engine_play_multiplayer_sound(
	long sound_index);
/* game_engine_queue_multiplayer_sound */
static void push_queued_sound(
	long sound_index,
	long delay_ticks);
/* game_engine_get_multiplayer_sound_duration */
static long get_sound_length_in_ticks(
	long sound_index);

/* ---------- globals */

boolean sound_is_queueable[_multiplayer_sound_ting] =
{
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE,
	FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
	TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
	TRUE, TRUE, FALSE,
};

struct multiplayer_sound_queue_count mp_sound_queue_count = { 0 };
struct queued_multiplayer_sound mp_sound_queue[
	MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS] = { 0 };

#define multiplayer_sound_queue_count mp_sound_queue_count.count
#define multiplayer_sound_queue_sounds mp_sound_queue

/* ---------- public code */

static void _game_engine_play_multiplayer_sound(
	long sound_index)
{
	struct game_globals_multiplayer_sound_view *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct tag_reference *sound;

	global_scenario_get();
	game_globals = (struct game_globals_multiplayer_sound_view *)scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);
	if (multiplayer_information && sound_index < multiplayer_information->sounds.count)
	{
		sound = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->sounds,
			sound_index,
			struct tag_reference);
		if (sound && sound->index != NONE)
			unspatialized_impulse_sound_new(sound->index, 1.0f);
	}

	return;
}

static void push_queued_sound(
	long sound_index,
	long delay_ticks)
{
	long queue_index = multiplayer_sound_queue_count;

	if (queue_index < MAXIMUM_QUEUED_MULTIPLAYER_SOUNDS)
	{
		multiplayer_sound_queue_sounds[queue_index].sound_index = sound_index;
		multiplayer_sound_queue_sounds[queue_index].delay_ticks = delay_ticks;
		multiplayer_sound_queue_count++;
	}

	return;
}

void game_engine_update_multiplayer_sound(
	void)
{
	long i;
	long queue_count = multiplayer_sound_queue_count;

	if (queue_count && --multiplayer_sound_queue_sounds[0].delay_ticks == 0)
	{
		for (i = 1; i < queue_count; i++)
			multiplayer_sound_queue_sounds[i - 1] = multiplayer_sound_queue_sounds[i];

		queue_count--;
		multiplayer_sound_queue_count = queue_count;
		if (queue_count)
			_game_engine_play_multiplayer_sound(multiplayer_sound_queue_sounds[0].sound_index);
	}

	return;
}

static long get_sound_length_in_ticks(
	long sound_index)
{
	struct game_globals_multiplayer_sound_view *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct tag_reference *sound;

	global_scenario_get();
	game_globals = (struct game_globals_multiplayer_sound_view *)scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);
	if (!multiplayer_information)
		return 0;
	if (sound_index >= multiplayer_information->sounds.count)
		return 0;

	sound = TAG_BLOCK_GET_ELEMENT(
		&multiplayer_information->sounds,
		sound_index,
		struct tag_reference);
	if (!sound)
		return 0;
	if (sound->index == NONE)
		return 0;

#ifdef HALO_N64
	/* (the N64's sound tags are records of its own: n64_sound_game.c) */
	{
		long n64_sound_definition_length(long sound_index);

		return (n64_sound_definition_length(sound->index) * TICKS_PER_SECOND) / 1000;
	}
#else
	return (long)(sound_definition_get(sound->index)->longest_permutation_length * TICKS_PER_SECOND) / 1000;
#endif
}

void game_engine_play_multiplayer_sound(
	long sound_index)
{
	if (sound_is_queueable[sound_index])
	{
		push_queued_sound(
			sound_index,
			get_sound_length_in_ticks(sound_index) + 5);
		if (multiplayer_sound_queue_count == 1)
			_game_engine_play_multiplayer_sound(sound_index);
	}
	else
	{
		_game_engine_play_multiplayer_sound(sound_index);
	}

	return;
}

void game_engine_intialize_queued_sounds(
	void)
{
	csmemset(
		multiplayer_sound_queue_sounds,
		0,
		sizeof(multiplayer_sound_queue_sounds));
	multiplayer_sound_queue_count = 1;
	multiplayer_sound_queue_sounds[0].sound_index = NONE;
	multiplayer_sound_queue_sounds[0].delay_ticks = MULTIPLAYER_SOUND_QUEUE_INITIAL_DELAY_TICKS;

	return;
}

/* ---------- private code */
