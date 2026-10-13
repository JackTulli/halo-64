/*
IMA_ADPCM.H

header included in hcex build.
*/

#ifndef __IMA_ADPCM_H
#define __IMA_ADPCM_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bungie_ima_adpcm_header
{
	long sample_count;
	short initial_sample;
	short unused;
};

struct bungie_ima_adpcm_state
{
	long sample_count;
	long sample_index;
	short sample;
	short step_size_index;
};

typedef char verify_bungie_ima_adpcm_header_size[
	sizeof(struct bungie_ima_adpcm_header) == 0x8 ? 1 : -1];
typedef char verify_bungie_ima_adpcm_state_size[
	sizeof(struct bungie_ima_adpcm_state) == 0xC ? 1 : -1];

/* ---------- prototypes/IMA_ADPCM.C */

long compress_ima_adpcm_audio_data(
	short *input_samples,
	long input_sample_count,
	byte *output_data,
	long output_data_size);
long decompress_ima_adpcm_audio_data(
	struct bungie_ima_adpcm_header const *input_header,
	long input_data_size,
	short *output_samples,
	long output_sample_count,
	struct bungie_ima_adpcm_state *state);
void byte_swap_bungie_ima_adpcm_header(
	struct bungie_ima_adpcm_header *header);

/* ---------- globals */

/* ---------- public code */

#endif // __IMA_ADPCM_H
