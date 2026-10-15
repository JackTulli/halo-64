/* N64F v1/v2 store effect records in each map file. N64F v3 keeps the same
   map metadata and addresses shared records in an identity-checked N64B bank.
   These byte-level helpers validate headers, padded record spans and tables. */
#ifndef N64_FX_FORMAT_H
#define N64_FX_FORMAT_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define N64_FX_HEADER_BYTES 32u
#define N64_FX_ATLAS_BYTES 4096u
#define N64_FX_COLOUR_BYTES 128u
#define N64_FX_EMITTER_BYTES 80u

struct n64_fx_layout {
    uint32_t version, index_offset, atlas_offset, explosions_offset, identity;
    uint32_t record_min, record_max;
    uint16_t count, cells, explosions;
    uint8_t flame_cell;
};

static uint16_t n64_fx_be16(const uint8_t *p)
{ return (uint16_t)((uint16_t)p[0] << 8 | p[1]); }
static uint32_t n64_fx_be32(const uint8_t *p)
{ return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3]; }
static int n64_fx_span(uint32_t bytes, uint32_t offset, uint32_t size)
{ return offset <= bytes && size <= bytes - offset; }

static int n64_fx_map_header(const uint8_t h[32], uint32_t bytes, struct n64_fx_layout *out)
{
    struct n64_fx_layout m;
    uint32_t atlas_end, index_end;
    if (bytes < 32 || memcmp(h, "N64F", 4)) return 0;
    memset(&m, 0, sizeof(m));
    m.version = n64_fx_be32(h + 4);
    m.count = n64_fx_be16(h + 8); m.cells = n64_fx_be16(h + 10);
    m.index_offset = n64_fx_be32(h + 12); m.atlas_offset = n64_fx_be32(h + 16);
    m.explosions_offset = n64_fx_be32(h + 20); m.explosions = n64_fx_be16(h + 24);
    m.flame_cell = h[26]; m.identity = n64_fx_be32(h + 28);
    if (m.version < 1 || m.version > 3 || m.cells > 32 || m.flame_cell >= 32 || h[27]) return 0;
    if (m.version < 3 && m.identity) return 0;
    if (m.index_offset < 32 || (m.index_offset & 3) ||
        !n64_fx_span(bytes, m.index_offset, (uint32_t)m.count * 8)) return 0;
    index_end = m.index_offset + (uint32_t)m.count * 8;
    if ((m.atlas_offset & 15) || m.atlas_offset < index_end ||
        !n64_fx_span(bytes, m.atlas_offset, N64_FX_ATLAS_BYTES + N64_FX_COLOUR_BYTES)) return 0;
    atlas_end = m.atlas_offset + N64_FX_ATLAS_BYTES + N64_FX_COLOUR_BYTES;
    if (m.explosions > 16) return 0;
    if (m.version == 1 && (m.explosions_offset || m.explosions)) return 0;
    if (m.explosions_offset) {
        if ((m.explosions_offset & 3) || m.explosions_offset < atlas_end ||
            !n64_fx_span(bytes, m.explosions_offset, (uint32_t)m.explosions * 4)) return 0;
    } else if (m.explosions) return 0;
    if (m.version == 3) {
        if (m.explosions_offset != atlas_end || bytes != atlas_end + (uint32_t)m.explosions * 4) return 0;
        /* The bank supplies these only after its own validation. */
        m.record_min = m.record_max = 0;
    } else {
        m.record_min = atlas_end;
        m.record_max = m.explosions_offset ? m.explosions_offset : bytes;
    }
    *out = m;
    return 1;
}

/* The CRC covers all payload bytes, including every inter-record/trailing pad. */
static int n64_fx_bank_header(const uint8_t h[32], uint32_t bytes, uint32_t identity,
    uint32_t *payload_min, uint32_t *payload_max)
{
    uint32_t count;
    if (bytes < 48 || memcmp(h, "N64B", 4) || n64_fx_be32(h + 4) != 1 ||
        n64_fx_be32(h + 8) != identity || n64_fx_be32(h + 12) != bytes ||
        n64_fx_be32(h + 16) != 32 || n64_fx_be32(h + 24) != bytes - 32 ||
        n64_fx_be32(h + 28) || ((bytes - 32) & 15)) return 0;
    count = n64_fx_be32(h + 20);
    if (!count || count > (bytes - 32) / 16) return 0;
    *payload_min = 32; *payload_max = bytes;
    return 1;
}

static uint32_t n64_fx_crc32_update(uint32_t crc, const uint8_t *data, size_t bytes)
{
    while (bytes--) {
        crc ^= *data++;
        for (unsigned i = 0; i < 8; i++) crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1)));
    }
    return crc;
}

static int n64_fx_record_span(uint32_t minimum, uint32_t maximum, uint32_t offset, uint32_t bytes)
{
    uint32_t padded;
    if (bytes < 8 || bytes > 65535 || (offset & 15) || minimum > maximum) return 0;
    padded = (bytes + 15) & ~15u;
    return offset >= minimum && n64_fx_span(maximum, offset, padded);
}

static int n64_fx_record_memory(const uint8_t *record, uint32_t bytes)
{
    uint32_t events, emitters, first, previous = 0;
    if (!record || bytes < 8) return 0;
    events = n64_fx_be16(record); emitters = n64_fx_be16(record + 2);
    first = (4 + 2 * (events + 1) + 3) & ~3u;
    if (first > bytes || bytes - first != emitters * N64_FX_EMITTER_BYTES) return 0;
    for (uint32_t i = 0; i <= events; i++) {
        uint32_t current = n64_fx_be16(record + 4 + 2 * i);
        if ((!i && current) || current < previous || current > emitters) return 0;
        previous = current;
    }
    return previous == emitters;
}
#endif
