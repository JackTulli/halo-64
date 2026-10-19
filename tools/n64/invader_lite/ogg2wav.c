/*
 * ogg2wav: decode one Ogg Vorbis stream read from standard input into a
 * canonical 16-bit PCM WAV file (44-byte RIFF header: "fmt " chunk with
 * format 1, then "data"), with the stream's own sample rate and channel
 * count. A small stand-in for
 *
 *     ffmpeg -hide_banner -loglevel error -y -i pipe:0 -acodec pcm_s16le OUT.wav
 *
 * for the ROM builder, which only needs that one conversion.
 *
 *     ogg2wav OUT.wav < in.ogg
 *
 * Exits 0 on success; on failure prints a message on standard error, removes
 * the partial output and exits non-zero (1 usage, 2 input/decoding, 3 output).
 * More than two channels are written in Vorbis channel order.
 *
 * Decoding is stb_vorbis (public domain / MIT, vendored next to this file as
 * stb_vorbis.c, see README.md). This file is part of halo-64's
 * tools/n64/invader_lite and is released under the same terms as
 * stb_vorbis: public domain (or MIT, at your option).
 */
#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_NO_STDIO
#include "stb_vorbis.c"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <wchar.h>
typedef wchar_t path_char;
#define open_output(path) _wfopen((path), L"wb")
#define remove_output(path) _wremove(path)
#else
typedef char path_char;
#define open_output(path) fopen((path), "wb")
#define remove_output(path) remove(path)
#endif

/* stb_vorbis_open_memory takes an int length */
#define INPUT_LIMIT ((size_t)0x7FFFFFFF)

static int fail(int code, const char *message, const char *detail)
{
    if(detail) fprintf(stderr, "ogg2wav: %s: %s\n", message, detail);
    else fprintf(stderr, "ogg2wav: %s\n", message);
    return code;
}

static const char *vorbis_error_text(int error)
{
    switch(error) {
    case VORBIS_outofmem: return "out of memory";
    case VORBIS_unexpected_eof: return "unexpected end of the stream";
    case VORBIS_invalid_setup: return "invalid setup header";
    case VORBIS_invalid_stream: return "invalid stream";
    case VORBIS_missing_capture_pattern: return "missing Ogg capture pattern (not an Ogg stream?)";
    case VORBIS_invalid_stream_structure_version: return "invalid Ogg stream structure version";
    case VORBIS_continued_packet_flag_invalid: return "invalid continued-packet flag";
    case VORBIS_incorrect_stream_serial_number: return "incorrect stream serial number";
    case VORBIS_invalid_first_page: return "invalid first page (not a Vorbis stream?)";
    case VORBIS_bad_packet_type: return "bad packet type";
    case VORBIS_cant_find_last_page: return "cannot find the last page";
    case VORBIS_seek_failed: return "seek failed";
    case VORBIS_ogg_skeleton_not_supported: return "Ogg Skeleton streams are not supported";
    case VORBIS_feature_not_supported: return "unsupported Vorbis feature (floor 0?)";
    case VORBIS_too_many_channels: return "too many channels";
    case VORBIS_file_open_failure: return "file open failure";
    default: return "decoding error";
    }
}

static void put16(unsigned char *p, unsigned v)
{
    p[0] = (unsigned char)(v & 0xFF);
    p[1] = (unsigned char)((v >> 8) & 0xFF);
}

static void put32(unsigned char *p, uint32_t v)
{
    p[0] = (unsigned char)(v & 0xFF);
    p[1] = (unsigned char)((v >> 8) & 0xFF);
    p[2] = (unsigned char)((v >> 16) & 0xFF);
    p[3] = (unsigned char)((v >> 24) & 0xFF);
}

/* all of standard input (binary) into one buffer */
static unsigned char *read_stdin(size_t *size_out)
{
    size_t capacity = 1 << 20, size = 0;
    unsigned char *data = malloc(capacity);
    if(!data) return NULL;
    for(;;) {
        size_t n;
        if(size == capacity) {
            unsigned char *bigger;
            if(capacity > INPUT_LIMIT) break;
            capacity *= 2;
            bigger = realloc(data, capacity);
            if(!bigger) { free(data); return NULL; }
            data = bigger;
        }
        n = fread(data + size, 1, capacity - size, stdin);
        size += n;
        if(n == 0) break;
    }
    if(ferror(stdin) || size > INPUT_LIMIT) { free(data); return NULL; }
    *size_out = size;
    return data;
}

#ifdef _WIN32
int wmain(int argc, wchar_t **argv)
#else
int main(int argc, char **argv)
#endif
{
    const path_char *output_path;
    unsigned char *input, header[44];
    size_t input_size = 0, frames = 0, capacity;
    short *pcm;
    stb_vorbis *vorbis;
    stb_vorbis_info info;
    int error = 0;
    uint64_t data_bytes;
    FILE *output;

    if(argc != 2) {
        fprintf(stderr, "Usage: ogg2wav <output.wav> < input.ogg\n"
                        "Decodes an Ogg Vorbis stream from standard input to a 16-bit PCM WAV file.\n");
        return 1;
    }
    output_path = argv[1];

#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
#endif
    input = read_stdin(&input_size);
    if(!input) return fail(2, "cannot read standard input", "read error, out of memory or over 2 GiB");
    if(input_size == 0) { free(input); return fail(2, "no input on standard input", NULL); }

    vorbis = stb_vorbis_open_memory(input, (int)input_size, &error, NULL);
    if(!vorbis) { free(input); return fail(2, "not a decodable Ogg Vorbis stream", vorbis_error_text(error)); }
    info = stb_vorbis_get_info(vorbis);
    if(info.channels < 1 || info.channels > 0xFFFF / 2 || info.sample_rate < 1) {
        stb_vorbis_close(vorbis);
        free(input);
        return fail(2, "unsupported stream", "bad channel count or sample rate");
    }

    capacity = 65536;
    pcm = malloc(capacity * (size_t)info.channels * sizeof(short));
    if(!pcm) { stb_vorbis_close(vorbis); free(input); return fail(2, "out of memory", NULL); }
    for(;;) {
        int n;
        if(capacity - frames < 4096) {
            short *bigger = realloc(pcm, capacity * 2 * (size_t)info.channels * sizeof(short));
            if(!bigger) { free(pcm); stb_vorbis_close(vorbis); free(input); return fail(2, "out of memory", NULL); }
            pcm = bigger;
            capacity *= 2;
        }
        n = stb_vorbis_get_samples_short_interleaved(vorbis, info.channels, pcm + frames * (size_t)info.channels,
                                                     4096 * info.channels);
        if(n <= 0) break;
        frames += (size_t)n;
    }
    error = stb_vorbis_get_error(vorbis);
    stb_vorbis_close(vorbis);
    free(input);
    if(error != VORBIS__no_error) {
        free(pcm);
        /* the end of the data in the middle of a page */
        if(error == VORBIS_missing_capture_pattern || error == VORBIS_unexpected_eof)
            return fail(2, "decoding failed", "the stream is truncated or corrupt");
        return fail(2, "decoding failed", vorbis_error_text(error));
    }

    data_bytes = (uint64_t)frames * (uint64_t)info.channels * 2;
    if(data_bytes > 0xFFFFFFFFu - 36) { free(pcm); return fail(2, "decoded audio is too long for a WAV file", NULL); }

    memcpy(header, "RIFF", 4);
    put32(header + 4, (uint32_t)(36 + data_bytes));
    memcpy(header + 8, "WAVEfmt ", 8);
    put32(header + 16, 16);                                          /* fmt chunk size */
    put16(header + 20, 1);                                           /* integer PCM */
    put16(header + 22, (unsigned)info.channels);
    put32(header + 24, info.sample_rate);
    put32(header + 28, info.sample_rate * (uint32_t)info.channels * 2); /* bytes per second */
    put16(header + 32, (unsigned)info.channels * 2);                 /* block align */
    put16(header + 34, 16);                                          /* bits per sample */
    memcpy(header + 36, "data", 4);
    put32(header + 40, (uint32_t)data_bytes);

    /* samples are host-order shorts: swap them on a big-endian host */
    {
        const uint16_t probe = 1;
        if(*(const unsigned char *)&probe == 0) {
            size_t i, count = frames * (size_t)info.channels;
            for(i = 0; i < count; i++) {
                uint16_t s = (uint16_t)pcm[i];
                pcm[i] = (short)(uint16_t)((s >> 8) | (s << 8));
            }
        }
    }

    output = open_output(output_path);
    if(!output) { free(pcm); return fail(3, "cannot create the output file", strerror(errno)); }
    {
        int ok = fwrite(header, 1, sizeof header, output) == sizeof header &&
                 (data_bytes == 0 || fwrite(pcm, 1, (size_t)data_bytes, output) == (size_t)data_bytes);
        int saved = errno;
        if(fclose(output) != 0 && ok) { ok = 0; saved = errno; }
        free(pcm);
        if(!ok) {
            remove_output(output_path);
            return fail(3, "cannot write the output file", strerror(saved));
        }
    }
    return 0;
}
