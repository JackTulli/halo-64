/*
RENDER_MEDIA.C

symbols in this file:
0017A6F0 0020:
	_render_media_initialize_for_new_map (0000)
0017A710 0010:
	_render_media_dispose_from_old_map (0000)
0017A720 0010:
	_render_media_build_render_target (0000)
0017A730 0010:
	_render_media (0000)
0029FEAC 0004:
	_media_wave_amplitude (0000)
0030D4D8 00ac:
	_standard_frame_level_of_detail (0000)
	_render_target_frame_level_of_detail (0058)
004C004C 002c:
	_bss_004c004c (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct render_media_globals
{
	boolean initialized;
	byte unused[3];
	long render_target_index;
	byte unknown[36];
};

typedef char render_media_globals_size_assert[
	sizeof(struct render_media_globals) == 44 ? 1 : -1];

struct frame_level_of_detail
{
	long level_count;
	real base_fraction;
	real frame_fraction_thresholds[5];
	real reserved_thresholds[3];
	real detail_fraction_thresholds[3];
	real distance_bias;
	real distance_thresholds[5];
	real maximum_level;
	real minimum_level;
};

typedef char frame_level_of_detail_size_assert[
	sizeof(struct frame_level_of_detail) == 84 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

struct render_media_globals bss_004c004c = {0};

const real media_wave_amplitude = 0.05f;

struct frame_level_of_detail standard_frame_level_of_detail =
{
	4,
	0.0f,
	{ 0.33f, 0.66f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	0.0f,
	{ 200.0f, 400.0f, 700.0f, 0.0f, 0.0f },
	3.0f,
	2.0f
};

struct frame_level_of_detail render_target_frame_level_of_detail =
{
	6,
	0.0f,
	{ 0.2f, 0.4f, 0.6f, 0.8f, 1.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.4f, 0.7f, 1.0f },
	0.0f,
	{ 50.0f, 70.0f, 125.0f, 250.0f, 500.0f },
	6.0f,
	4.0f
};

/* ---------- public code */

void render_media_initialize_for_new_map(
	void)
{
	bss_004c004c.initialized = TRUE;
	bss_004c004c.render_target_index = 0;

	return;
}

void render_media_dispose_from_old_map(
	void)
{
	bss_004c004c.initialized = FALSE;

	return;
}

boolean render_media_build_render_target(
	void)
{
	return FALSE;
}

void render_media(
	void)
{
	return;
}

/* ---------- private code */
