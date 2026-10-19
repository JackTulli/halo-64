// invader-lite: the FLAC / Ogg Vorbis entry points of libinvader without
// libFLAC or libvorbis (not linked). Encoding and decoding throw;
// ogg_vorbis_sample_count reads the Ogg pages directly. Decode Ogg Vorbis
// to a WAV first with ogg2wav (or ffmpeg).
#include <invader/sound/sound_encoder.hpp>
#include <invader/sound/sound_reader.hpp>
#include <invader/printf.hpp>
#include <stdexcept>
#include <cstring>

namespace Invader::SoundEncoder {
    std::vector<std::byte> encode_to_flac(const std::vector<std::byte> &, std::size_t, std::uint32_t, std::uint32_t, std::uint32_t) {
        throw std::runtime_error("invader-lite: FLAC encoding is not available");
    }
    std::vector<std::byte> encode_to_ogg_vorbis_vbr(const std::vector<std::byte> &, std::size_t, std::uint32_t, std::uint32_t, float) {
        throw std::runtime_error("invader-lite: Ogg Vorbis encoding is not available");
    }
    std::vector<std::byte> encode_to_ogg_vorbis_cbr(const std::vector<std::byte> &, std::size_t, std::uint32_t, std::uint32_t, std::uint16_t) {
        throw std::runtime_error("invader-lite: Ogg Vorbis encoding is not available");
    }
}

namespace Invader::SoundReader {
    Sound sound_from_ogg_file(const std::filesystem::path &) {
        throw std::runtime_error("invader-lite: Ogg Vorbis decoding is not available (decode with ffmpeg)");
    }
    Sound sound_from_ogg(const std::byte *, std::size_t) {
        throw std::runtime_error("invader-lite: Ogg Vorbis decoding is not available (decode with ffmpeg)");
    }
    Sound sound_from_flac_file(const std::filesystem::path &) {
        throw std::runtime_error("invader-lite: FLAC decoding is not available");
    }
    Sound sound_from_flac(const std::byte *, std::size_t) {
        throw std::runtime_error("invader-lite: FLAC decoding is not available");
    }

    // ov_pcm_total * channels: the last page's granule position of the first
    // logical stream (samples per channel) times the identification header's
    // channel count
    std::size_t ogg_vorbis_sample_count(const std::byte *data, std::size_t size) {
        const auto *d = reinterpret_cast<const unsigned char *>(data);
        std::size_t pos = 0;
        std::uint32_t serial = 0;
        bool have_serial = false;
        int channels = 0;
        std::uint64_t granule = 0;
        while(pos + 27 <= size) {
            if(std::memcmp(d + pos, "OggS", 4) != 0) {
                eprintf_error("Invalid ogg vorbis input");
                throw std::exception();
            }
            std::uint64_t g = 0;
            for(int i = 7; i >= 0; i--) g = (g << 8) | d[pos + 6 + i];
            std::uint32_t s = static_cast<std::uint32_t>(d[pos + 14]) | (static_cast<std::uint32_t>(d[pos + 15]) << 8) |
                              (static_cast<std::uint32_t>(d[pos + 16]) << 16) | (static_cast<std::uint32_t>(d[pos + 17]) << 24);
            std::size_t segments = d[pos + 26];
            if(pos + 27 + segments > size) break;
            std::size_t body = 0;
            for(std::size_t i = 0; i < segments; i++) body += d[pos + 27 + i];
            std::size_t start = pos + 27 + segments;
            if(start + body > size) break;
            if(!have_serial) {
                serial = s;
                have_serial = true;
                if(body >= 12 && d[start] == 1 && std::memcmp(d + start + 1, "vorbis", 6) == 0) {
                    channels = d[start + 11];
                }
            }
            if(s == serial && g != 0xFFFFFFFFFFFFFFFFULL) granule = g;
            pos = start + body;
        }
        if(channels <= 0) {
            eprintf_error("Invalid ogg vorbis input");
            throw std::exception();
        }
        return static_cast<std::size_t>(granule) * static_cast<std::size_t>(channels);
    }
}
