/*
N64_SOUND.C

Sound on the N64: libdragon's audio and mixer at 22 kHz.

The main menu's music (Halo's title1 looping sound, tools/n64/music.py):
its start ("in", the monks) once, then one of its three loop versions after
another, chosen at random as Halo's looping sounds do, each streamed from
the cartridge (rom:/music/title_in.wav64, title_loop0..2.wav64), on the
stereo pair of channels 0-1.

The game's sounds (port/n64/game/n64_sound_game.c) play on voices, the
mixer channels after the music's: each a mono VADPCM file of the
cartridge's one sound bank (rom:/snd/sounds.bank, tools/n64/soundbank.py)
streamed from its offset there, the map's table of offsets
(rom:/snd/<map>.ofs) giving each file of the map's sound list. A new sound
takes a free voice or steals the one the mixer finds cheapest of no higher
priority (mixer_ch_alloc: lower priority, quieter, older).

The mixer is fed at the end of every frame (n64_sound_poll, from
n64_render_frame_end) and between the game's ticks and views, in the menu
and in games alike, so the audio never runs dry; with nothing playing it
plays silence.
*/

#include <libdragon.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "n64.h"

#define SOUND_RATE 22050
/* the music: a stereo pair */
#define MUSIC_CHANNEL 0
#define LOOP_VERSIONS 3
/* the game's voices */
#define FIRST_VOICE 2
#define VOICE_COUNT 12
/* a voice's highest pitch (its buffer is sized for it) */
#define MAXIMUM_PITCH 2.0f

/* libdragon's (audio/wav64_internal.h): a wav64 file read from where an
open file is (the bank's offset) */
wav64_t *wav64_loadfd(int fd, const char *debug_file_name, wav64_loadparms_t *parms);

static struct
{
	int ready;
	/* the music: playing at all, past its start, the track streaming */
	int music_on;
	int looping;
	wav64_t *track;
	unsigned long random;
	int last_version;
	/* the map's sounds: the bank (a file descriptor, or -1), each list
	file's offset in it */
	int bank;
	unsigned long *offsets;
	int file_count;
	/* the game paused (n64_sound_pause): its voices silent */
	int paused;
	struct
	{
		wav64_t *wav;
		unsigned short generation;
		/* as last set */
		float volume, pan, pitch;
		/* the bank's file its wav is, and looped or not (n64_voice_play) */
		int file, loop;
	} voices[VOICE_COUNT];
	/* how many voices were started, stolen (statistics); the CPU's time
feeding the mixer, starting voices (counter ticks) */
	unsigned long started, stolen, refused;
	unsigned long poll_ticks, play_ticks, close_ticks;
	/* the output's loudest sample and its power since the last report
	("debug_sound_level") */
	int peak;
	unsigned long long level_sum, level_samples;
} sound;

void n64_sound_initialize(void)
{
	if (sound.ready)
		return;
	/* (the latency covers the longest gap between feeds: a slow 4-player
	frame is fed between its ticks and views) */
	audio_init(SOUND_RATE, AUDIO_INIT_LATENCY_MS(150));
	mixer_init(FIRST_VOICE + VOICE_COUNT);
	for (int i = 0; i < VOICE_COUNT; i++)
		mixer_ch_set_limits(FIRST_VOICE + i, 16, SOUND_RATE * MAXIMUM_PITCH, 0);
	sound.random = TICKS_READ();
	sound.bank = -1;
	sound.ready = 1;
}

/* What the sound keeps for good, made at boot (n64_main.c): the mixer
gives a channel its ring (a 4.6 KB VADPCM buffer it streams through) the
first time it plays and keeps it, and the bank's file stays open. Made with
a game's first sounds and the title's first music they stayed wherever the
heap had room then -- in the middle of the memory the menu and the next
level wanted (a "debug_heap_dump" cycle found them). A sound of the bank is
started and stopped on each voice's channel, and the title's music on its
own, so that the rings are made now, at the heap's bottom */
void n64_sound_reserve(void)
{
	wav64_t *wav;

	n64_sound_initialize();
	if (sound.bank < 0)
		sound.bank = open("rom:/snd/sounds.bank", O_RDONLY);
	if (sound.bank >= 0)
	{
		lseek(sound.bank, 0, SEEK_SET);
		wav = wav64_loadfd(sound.bank, "sfx", &(wav64_loadparms_t){ .streaming_mode = WAV64_STREAMING_FULL });
		if (wav)
		{
			for (int i = 0; i < VOICE_COUNT; i++)
			{
				wav64_play(wav, FIRST_VOICE + i);
				mixer_ch_stop(FIRST_VOICE + i);
			}
			wav64_close(wav);
		}
	}
	wav = wav64_load("rom:/music/title_in.wav64", &(wav64_loadparms_t){ .streaming_mode = WAV64_STREAMING_FULL });
	if (wav)
	{
		wav64_play(wav, MUSIC_CHANNEL);
		mixer_ch_stop(MUSIC_CHANNEL);
		wav64_close(wav);
	}
}

static void music_play(const char *path)
{
	if (sound.track)
	{
		mixer_ch_stop(MUSIC_CHANNEL);
		wav64_close(sound.track);
		sound.track = NULL;
	}
	sound.track = wav64_load(path, &(wav64_loadparms_t){ .streaming_mode = WAV64_STREAMING_FULL });
	if (!sound.track)
	{
		debugf("n64: no music %s\n", path);
		return;
	}
	mixer_ch_set_vol(MUSIC_CHANNEL, 0.8f, 0.8f);
	wav64_play(sound.track, MUSIC_CHANNEL);
	debugf("n64: music %s, %d kbit/s\n", path, wav64_get_bitrate(sound.track) / 1000);
}

/* the title music, from its start (nothing if it is already playing) */
void n64_music_start(void)
{
	n64_sound_initialize();
	if (sound.music_on)
		return;
	/* (the loops' order: the sound is made at boot, at much the same tick
	each time) */
	sound.random ^= TICKS_READ();
	sound.music_on = 1;
	sound.looping = 0;
	sound.last_version = -1;
	music_play("rom:/music/title_in.wav64");
}

void n64_music_stop(void)
{
	if (!sound.music_on)
		return;
	sound.music_on = 0;
	if (sound.track)
	{
		mixer_ch_stop(MUSIC_CHANNEL);
		wav64_close(sound.track);
		sound.track = NULL;
	}
}

void n64_sound_poll(void)
{
	if (!sound.ready)
		return;
	/* a track done: the next loop version (a random one, not the one just
	played) */
	if (sound.music_on && !mixer_ch_playing(MUSIC_CHANNEL))
	{
		char path[48];
		int version;

		do
		{
			sound.random = sound.random * 1664525UL + 1013904223UL;
			version = (int)((sound.random >> 16) % LOOP_VERSIONS);
		} while (version == sound.last_version);
		sound.last_version = version;
		snprintf(path, sizeof(path), "rom:/music/title_loop%d.wav64", version);
		sound.looping = 1;
		music_play(path);
	}
	unsigned long start = TICKS_READ();
	int zone = n64_profile_zone;
	/* (port.json flags "profile profile_zone profile_sound": the mixer's
	time sampled) */
	if (n64_debug_flag("profile_sound"))
		n64_profile_zone = 1;
	if (n64_debug_flag("debug_sound_level"))
	{
		/* (measuring: mixed here, synchronously, and the output's loudest
		sample kept) */
		while (audio_can_write())
		{
			short *buffer = audio_write_begin();
			int length = audio_get_buffer_length() * 2;

			mixer_poll(buffer, audio_get_buffer_length());
			for (int i = 0; i < length; i++)
			{
				int level = buffer[i] < 0 ? -buffer[i] : buffer[i];

				if (level > sound.peak)
					sound.peak = level;
				sound.level_sum += (unsigned long long)(level * level);
			}
			sound.level_samples += length;
			audio_write_end();
		}
	}
	else
		mixer_try_play();
	n64_profile_zone = zone;
	sound.poll_ticks += TICKS_READ() - start;
}


/* ---------- the game's sounds */

int n64_sound_load_map(const char *map_name)
{
	char path[64];
	int size = 0;

	n64_sound_initialize();
	n64_sound_unload_map();
	/* (port.json flag "debug_no_sound": none, to measure what sound
	costs) */
	if (n64_debug_flag("debug_no_sound"))
		return 0;
	snprintf(path, sizeof(path), "rom:/snd/%s.ofs", map_name);
	FILE *test = fopen(path, "rb");
	if (!test)
	{
		debugf("n64: no sounds for %s\n", map_name);
		return 0;
	}
	fclose(test);
	sound.offsets = asset_load(path, &size);
	sound.file_count = size / 4;
	if (sound.bank < 0)
		sound.bank = open("rom:/snd/sounds.bank", O_RDONLY);
	if (sound.bank < 0)
	{
		debugf("n64: no sound bank\n");
		free(sound.offsets);
		sound.offsets = NULL;
		sound.file_count = 0;
		return 0;
	}
	debugf("n64: %d sounds for %s\n", sound.file_count, map_name);
	return 1;
}

void n64_sound_unload_map(void)
{
	n64_voice_stop_all();
	for (int i = 0; i < VOICE_COUNT; i++)
	{
		if (sound.voices[i].wav)
		{
			wav64_close(sound.voices[i].wav);
			sound.voices[i].wav = NULL;
		}
	}
	free(sound.offsets);
	sound.offsets = NULL;
	sound.file_count = 0;
}

static int voice_slot(long voice)
{
	int slot = (int)(voice & 0xFF);

	if (voice < 0 || slot >= VOICE_COUNT || sound.voices[slot].generation != (unsigned short)(voice >> 8) ||
		!sound.voices[slot].wav)
		return -1;
	return slot;
}

static void voice_volume(int slot, float volume, float pan)
{
	/* pan -1 (left) to 1 (right): the far side keeps some of it */
	float left = volume, right = volume;

	if (pan > 0.0f)
		left *= 1.0f - 0.7f * (pan > 1.0f ? 1.0f : pan);
	else if (pan < 0.0f)
		right *= 1.0f + 0.7f * (pan < -1.0f ? -1.0f : pan);
	if (sound.paused)
		left = right = 0.0f;
	mixer_ch_set_vol(FIRST_VOICE + slot, left, right);
}

static void voice_pitch(int slot, float pitch)
{
	wav64_t *wav = sound.voices[slot].wav;

	if (pitch > MAXIMUM_PITCH)
		pitch = MAXIMUM_PITCH;
	if (pitch < 0.25f)
		pitch = 0.25f;
	mixer_ch_set_freq(FIRST_VOICE + slot, wav->wave.frequency * pitch);
}

long n64_voice_play(int file, float volume, float pan, float pitch, int priority, int loop)
{
	int channel, slot;

	if (!sound.offsets || file < 0 || file >= sound.file_count)
		return -1;
	if (priority < 0)
		priority = 0;
	if (priority > MIXER_PRIORITY_MAX)
		priority = MIXER_PRIORITY_MAX;
	if (mixer_ch_alloc(FIRST_VOICE, VOICE_COUNT, 1, false, priority, NULL, &channel) < 1)
	{
		sound.refused++;
		return -1;
	}
	slot = channel - FIRST_VOICE;
	unsigned long start = TICKS_READ();
	loop = loop != 0;
	if (sound.voices[slot].wav && mixer_ch_playing(channel))
		sound.stolen++;
	mixer_ch_stop(channel);
	/* The same sound again (a gun's every shot, footsteps, impacts): its
	file still loaded in this voice or another that has stopped is taken
	as it is -- the other voice gets this one's in exchange, its old
	handles made stale -- instead of closed and loaded again from the
	cartridge (two DMAs, an allocation and the codec's setup a sound) */
	{
		int reuse = -1;

		for (int i = 0; i < VOICE_COUNT; i++)
		{
			if (sound.voices[i].wav && sound.voices[i].file == file && sound.voices[i].loop == loop &&
				(i == slot || !mixer_ch_playing(FIRST_VOICE + i)))
			{
				reuse = i;
				if (i == slot)
					break;
			}
		}
		if (reuse >= 0 && reuse != slot)
		{
			wav64_t *wav = sound.voices[slot].wav;
			int old_file = sound.voices[slot].file, old_loop = sound.voices[slot].loop;

			mixer_ch_stop(FIRST_VOICE + reuse);
			sound.voices[slot].wav = sound.voices[reuse].wav;
			sound.voices[reuse].wav = wav;
			sound.voices[reuse].file = old_file;
			sound.voices[reuse].loop = old_loop;
			sound.voices[reuse].generation++;
		}
		if (reuse < 0 && sound.voices[slot].wav)
		{
			wav64_close(sound.voices[slot].wav);
			sound.voices[slot].wav = NULL;
		}
	}
	unsigned long opened = TICKS_READ();
	sound.close_ticks += opened - start;
	if (!sound.voices[slot].wav)
	{
		lseek(sound.bank, (off_t)sound.offsets[file], SEEK_SET);
		sound.voices[slot].wav = wav64_loadfd(sound.bank, "sfx",
			&(wav64_loadparms_t){ .streaming_mode = WAV64_STREAMING_FULL });
		if (!sound.voices[slot].wav)
			return -1;
		sound.voices[slot].file = file;
		sound.voices[slot].loop = loop;
		wav64_set_loop(sound.voices[slot].wav, loop);
	}
	wav64_play(sound.voices[slot].wav, channel);
	mixer_ch_set_priority(channel, priority);
	voice_pitch(slot, pitch);
	voice_volume(slot, volume, pan);
	sound.voices[slot].volume = volume;
	sound.voices[slot].pan = pan;
	sound.voices[slot].pitch = pitch;
	sound.voices[slot].generation++;
	sound.started++;
	sound.play_ticks += TICKS_READ() - opened;
	return slot | ((long)sound.voices[slot].generation << 8);
}

void n64_voice_set(long voice, float volume, float pan, float pitch)
{
	int slot = voice_slot(voice);

	if (slot < 0)
		return;
	/* (only a change worth hearing: each one starts the mixer's ramps) */
	if (fabsf(volume - sound.voices[slot].volume) > 0.01f || fabsf(pan - sound.voices[slot].pan) > 0.02f)
	{
		voice_volume(slot, volume, pan);
		sound.voices[slot].volume = volume;
		sound.voices[slot].pan = pan;
	}
	if (fabsf(pitch - sound.voices[slot].pitch) > 0.003f)
	{
		voice_pitch(slot, pitch);
		sound.voices[slot].pitch = pitch;
	}
}

void n64_voice_stop(long voice)
{
	int slot = voice_slot(voice);

	if (slot >= 0)
		mixer_ch_stop(FIRST_VOICE + slot);
}

int n64_voice_playing(long voice)
{
	int slot = voice_slot(voice);

	return slot >= 0 && mixer_ch_playing(FIRST_VOICE + slot) &&
		mixer_ch_playing_waveform(FIRST_VOICE + slot) == &sound.voices[slot].wav->wave;
}

void n64_voice_stop_all(void)
{
	if (!sound.ready)
		return;
	for (int i = 0; i < VOICE_COUNT; i++)
		mixer_ch_stop(FIRST_VOICE + i);
}

/* The pause menu's: the voices go on playing (a looping sound where it
was when the game goes on; a shot's sound runs out), unheard; a volume set
meanwhile is kept for after */
void n64_sound_pause(int paused)
{
	paused = paused != 0;
	if (!sound.ready || paused == sound.paused)
		return;
	sound.paused = paused;
	for (int i = 0; i < VOICE_COUNT; i++)
	{
		if (sound.voices[i].wav)
			voice_volume(i, sound.voices[i].volume, sound.voices[i].pan);
	}
}

void n64_sound_statistics(unsigned long *started, unsigned long *stolen, unsigned long *refused)
{
	*started = sound.started;
	*stolen = sound.stolen;
	*refused = sound.refused;
	debugf("sound cpu: mixer %lu ms, closing %lu ms, opening %lu ms\n", sound.poll_ticks / (TICKS_PER_SECOND / 1000),
		sound.close_ticks / (TICKS_PER_SECOND / 1000), sound.play_ticks / (TICKS_PER_SECOND / 1000));
	if (sound.level_samples)
	{
		debugf("sound level: peak %d, rms %d (of 32767)\n", sound.peak,
			(int)sqrtf((float)(sound.level_sum / sound.level_samples)));
		sound.peak = 0;
		sound.level_sum = sound.level_samples = 0;
	}
}
