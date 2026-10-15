/*
N64_PARTICLES.C

Halo's particles, simply: what an effect would have made (a muzzle flash,
sparks, dust, blood, an explosion's fireball) as a small pool of sprites
that fly in straight lines (gravity and drag their only physics), grow,
fade in and out and are drawn as rectangles facing the camera.

The effects' particle lists are not in the map: tools/n64/effects.py
writes each map's as records of its own in rom:/fx/<map>.fx, with a 128x64
I4 atlas of the particles' pictures (16x16 each, tinted by the particle's
colour). The index of the records and the atlas stay in RAM; a record is
read from the cartridge when its effect plays, into a small cache
(n64_fx_event, asked for by source/effects/effects.c, HALO_N64).

Drawing (n64_particles_draw, from n64_render.c): each particle is placed
on the screen by the view's camera and drawn as a textured rectangle with
the depth tiny3d's triangles would have there (rdpq's Z-override: tested,
not written), far ones first; the first-person ones (a muzzle flash on the
player's own gun) are kept in the camera's space and drawn after the
first-person weapon.

Explosions (grenades', rockets', tank shells', the needler's -- the effects
the converter lists in the file) are the N64's own: n64_explosion, from the
effect code where the effect begins, makes a flash, a fireball of puffs
that swell, rise and cool from white through orange to dark red, and a
little smoke that rises after it, each puff a billowy blob (made here, 32x32
I8) coloured hot in its middle and cooler at its edge (the combiner mixes
the two colours by the texel), all drawn like the particles.
*/

#include <libdragon.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"
#include "n64_sniper_ribbon.h"
#include "n64_fx_format.h"

#define MAXIMUM_PARTICLES 128
#define CELL 16
#define CELLS 32
#define ATLAS_COLUMNS 8
#define ATLAS_BYTES 4096
#define CACHE_ENTRIES 12
#define SPAWNS_PER_SECOND 300.f
/* a particle's largest half-size in pixels, in a full-screen view */
#define MAXIMUM_PIXELS 20.f
#define SPAWN_BURST 64.f
#define MAXIMUM_EXPLOSIONS 8
#define MAXIMUM_EXPLOSION_EFFECTS 16
#define FIREBALL 32
/* an explosion's parts: the flash, the fireball's puffs, the smoke's */
#define FIRE_PUFFS 7
#define SMOKE_PUFFS 5
#define EXPLOSION_PUFFS (1 + FIRE_PUFFS + SMOKE_PUFFS)
/* seconds: the flash, the fireball, the smoke's start and its life */
#define FLASH_TIME 0.12f
#define FIRE_TIME 0.75f
#define SMOKE_START 0.3f
#define SMOKE_TIME 1.3f

/* how each kind of explosion looks: the fireball's radius (world units),
the colours from the flash to the coolest fire, the smoke's, and how much
smoke (0-1) */
static const struct explosion_look
{
	float radius;
	uint8_t flash[3], hot[3], warm[3], cool[3], smoke[3];
	float smoke_amount;
} explosion_looks[N64_EXPLOSION_KINDS] = {
	/* frag grenade */
	{1.1f, {255, 250, 225}, {255, 214, 120}, {250, 128, 40}, {140, 40, 18}, {64, 60, 56}, 1.f},
	/* plasma grenade: blue */
	{1.0f, {235, 245, 255}, {150, 210, 255}, {70, 130, 255}, {36, 46, 170}, {72, 82, 112}, 0.25f},
	/* rocket */
	{1.35f, {255, 250, 225}, {255, 210, 110}, {250, 120, 36}, {130, 36, 16}, {60, 56, 52}, 1.f},
	/* tank shell */
	{1.6f, {255, 250, 225}, {255, 205, 100}, {245, 112, 32}, {120, 32, 14}, {58, 54, 50}, 1.f},
	/* the needler's: pink */
	{0.6f, {255, 235, 250}, {255, 160, 235}, {230, 80, 205}, {110, 28, 100}, {80, 62, 80}, 0.15f},
	/* PC Fuel Rod: green plasma, distinct from a rocket or plasma grenade. */
	{1.15f, {245, 255, 210}, {210, 255, 110}, {100, 230, 36}, {38, 100, 20}, {58, 78, 45}, 0.3f},
};

struct explosion
{
	float position[3];
	float age;
	uint8_t kind, seed, live, paint;
};
/* Paint uses the old padding byte; both pools keep their original size. */
typedef char explosion_pool_size_check[sizeof(struct explosion) == 20 ? 1 : -1];

struct particle
{
	float position[3], velocity[3];
	float age, life, fade_in, fade_out;
	float radius, end_scale;
	float gravity, drag;
	uint32_t color; /* RGBA */
	uint8_t cell, flags;
	int8_t local_player;
	uint8_t live;
};

struct fx_index
{
	uint16_t tag_index, size;
	uint32_t offset;
};

static struct
{
	struct particle particles[MAXIMUM_PARTICLES];
	int next;
	int live;
	/* the map's records: the index, the file's cartridge address, the atlas */
	struct fx_index *index;
	int index_count;
	uint32_t rom;
	/* Inclusive lower / exclusive upper record-file bounds; no bank heap copy. */
	uint32_t record_min, record_max;
	surface_t atlas;
	uint8_t cell_colours[CELLS][4];
	struct
	{
		int tag_index;
		uint8_t *data;
		unsigned long used;
	} cache[CACHE_ENTRIES];
	unsigned long clock;
	/* the spawn budget, and its share (half with three or four views: each
	particle is drawn in every view) */
	float tokens, budget;
	/* the menu's setting */
	int disabled;
	/* statistics */
	unsigned long spawned, drawn, reads, refused;
	/* explosions: the map's explosion effects (tag index, kind), those
	going on, the fireball's blob */
	struct { uint16_t tag_index; uint8_t kind, pad; } explosion_effects[MAXIMUM_EXPLOSION_EFFECTS];
	int explosion_effect_count;
	struct explosion explosions[MAXIMUM_EXPLOSIONS];
	int next_explosion;
	uint8_t explosion_seed, paint_splash_budget;
	surface_t fireball;
} fx;

/* Eight short, independently fading shot segments: 224 bytes, no heap. */
static struct { float start[3], end[3], age; } sniper_ribbons[N64_SNIPER_RIBBONS];
static int next_sniper_ribbon;
/* I4: soft across the ribbon, tapered at both ends, no opaque card. */
static unsigned char sniper_mask[32] __attribute__((aligned(16))) = {
    0,0,0,0,0,0,0,0,
    0x01,0x35,0x79,0xBC,0xCB,0x97,0x53,0x10,
    0x01,0x35,0x79,0xBC,0xCB,0x97,0x53,0x10,
    0,0,0,0,0,0,0,0
};
static void reset_sniper_ribbons(void)
{
    for (int i = 0; i < N64_SNIPER_RIBBONS; i++) sniper_ribbons[i].age = N64_SNIPER_LIFE;
    next_sniper_ribbon = 0;
}
void n64_sniper_ribbon(const float *start, const float *end)
{
    if (fx.disabled) return;
    int index = next_sniper_ribbon;
    if (!sniper_ribbon_segment(start, end, sniper_ribbons[index].start, sniper_ribbons[index].end)) return;
    sniper_ribbons[index].age = 0.f;
    next_sniper_ribbon = (index + 1) % N64_SNIPER_RIBBONS;
}

static void make_fireball(void);
static int enhanced_fx;
static unsigned char flame_cell;
void n64_particles_set_enhanced(int on) { enhanced_fx = !!on; }
int n64_particles_enhanced(void) { return enhanced_fx; }

/* the explosions' fireball, made at boot (n64_main.c): made with the first
map it was kept above that map's image (n64_anim.c) */
void n64_particles_reserve(void)
{
	if (!fx.fireball.buffer)
		make_fireball();
}

static void fx_free(void)
{
	for (int i = 0; i < CACHE_ENTRIES; i++)
	{
		free(fx.cache[i].data);
		fx.cache[i].data = NULL;
		fx.cache[i].tag_index = -1;
	}
	free(fx.index);
	fx.index = NULL;
	fx.index_count = 0;
	if (fx.atlas.buffer)
		surface_free(&fx.atlas);
	fx.rom = 0;
	fx.record_min = fx.record_max = 0;
	fx.explosion_effect_count = 0;
	memset(fx.cell_colours, 0, sizeof(fx.cell_colours));
}

/* the map's effects given back (the fireball stays): at the end of a game
(n64_game_free_map), as n64_hud_release; the RSP and the RDP done with the
atlas first */
void n64_particles_release(void)
{
	rspq_wait();
	fx_free();
	flame_cell = 0;
	reset_sniper_ribbons();
	memset(fx.particles, 0, sizeof(fx.particles));
	fx.live = 0;
	memset(fx.explosions, 0, sizeof(fx.explosions));
}

/* a small hash: 0..1 */
static float hash2(int x, int y, int seed)
{
	unsigned n = (unsigned)x * 374761393u + (unsigned)y * 668265263u + (unsigned)seed * 2147483647u;

	n = (n ^ (n >> 13)) * 1274126177u;
	return (float)((n ^ (n >> 16)) & 0xFFFF) / 65535.f;
}

/* value noise at a scale, bilinear */
static float value_noise(float x, float y, int seed)
{
	int ix = (int)floorf(x), iy = (int)floorf(y);
	float fx_ = x - ix, fy = y - iy;
	float a = hash2(ix, iy, seed), b = hash2(ix + 1, iy, seed);
	float c = hash2(ix, iy + 1, seed), d = hash2(ix + 1, iy + 1, seed);

	fx_ = fx_ * fx_ * (3.f - 2.f * fx_);
	fy = fy * fy * (3.f - 2.f * fy);
	return (a + (b - a) * fx_) + ((c + (d - c) * fx_) - (a + (b - a) * fx_)) * fy;
}

/* the fireball's puff: a few soft lumps, roughened by noise, I8 (its
intensity the alpha and the mix of the puff's colours) */
static void make_fireball(void)
{
	static const float lumps[7][3] = {
		{0.f, 0.f, 0.62f}, {0.30f, 0.12f, 0.38f}, {-0.27f, 0.20f, 0.36f}, {0.06f, -0.32f, 0.38f},
		{-0.24f, -0.24f, 0.32f}, {0.32f, -0.20f, 0.30f}, {-0.04f, 0.34f, 0.32f},
	};

	fx.fireball = surface_alloc(FMT_I8, FIREBALL, FIREBALL);
	if (!fx.fireball.buffer)
		return;
	for (int y = 0; y < FIREBALL; y++)
	{
		uint8_t *row = (uint8_t *)fx.fireball.buffer + y * fx.fireball.stride;

		for (int x = 0; x < FIREBALL; x++)
		{
			float u = (x + 0.5f) / (FIREBALL / 2) - 1.f, v = (y + 0.5f) / (FIREBALL / 2) - 1.f;
			float value = 0.f, noise;

			for (int i = 0; i < 7; i++)
			{
				float du = u - lumps[i][0], dv = v - lumps[i][1];
				float d = (du * du + dv * dv) / (lumps[i][2] * lumps[i][2]);

				if (d < 1.f)
					value += (1.f - d) * (1.f - d);
			}
			noise = value_noise(x * 0.25f, y * 0.25f, 1) * 0.6f + value_noise(x * 0.5f, y * 0.5f, 2) * 0.4f;
			value *= 0.55f + 0.7f * noise;
			/* (nothing at the square's edge) */
			value *= n64_max_f(0.f, 1.f - (u * u + v * v)) * 1.6f;
			row[x] = value >= 1.f ? 255 : value <= 0.f ? 0 : (uint8_t)(value * 255.f);
		}
	}
	data_cache_hit_writeback(fx.fireball.buffer, fx.fireball.stride * FIREBALL);
}

/* A failed load publishes no index, atlas, bank address or explosion list. */
static int fx_file_bytes(FILE *file, uint32_t *bytes)
{
    long end;
    if (fseek(file, 0, SEEK_END) || (end = ftell(file)) < 0 || (uint64_t)end > 0x7fffffffu) return 0;
    *bytes = (uint32_t)end;
    return !fseek(file, 0, SEEK_SET);
}

static int fx_read_at(FILE *file, uint32_t offset, void *out, size_t bytes)
{
    return !fseek(file, (long)offset, SEEK_SET) && fread(out, 1, bytes, file) == bytes;
}

static int fx_rom_span(uint32_t rom, uint32_t offset, uint32_t size)
{
    uint32_t padded = (size + 15u) & ~15u;
    if (!rom || size > 65535u || offset > UINT32_MAX - rom) return 0;
    return padded && padded - 1u <= UINT32_MAX - (rom + offset);
}

static int fx_bank_validate(FILE *file, uint32_t identity, uint32_t *minimum, uint32_t *maximum)
{
    uint8_t header[32], scratch[256];
    uint32_t bytes, crc = 0xffffffffu;
    if (!fx_file_bytes(file, &bytes) || !fx_read_at(file, 0, header, sizeof(header)) ||
        !n64_fx_bank_header(header, bytes, identity, minimum, maximum)) return 0;
    for (uint32_t offset = *minimum; offset < *maximum;) {
        uint32_t count = *maximum - offset;
        if (count > sizeof(scratch)) count = sizeof(scratch);
        if (fread(scratch, 1, count, file) != count) return 0;
        crc = n64_fx_crc32_update(crc, scratch, count);
        offset += count;
    }
    return (crc ^ 0xffffffffu) == identity;
}

/* Record shape/table checks stream through bounded stack storage. No record
   allocation is added to map loading, even for the largest bank record. */
static int fx_file_record(FILE *file, uint32_t offset, uint32_t bytes)
{
    uint8_t head[4], scratch[256];
    uint32_t events, emitters, first, previous = 0, seen = 0;
    if (!fx_read_at(file, offset, head, sizeof(head))) return 0;
    events = n64_fx_be16(head); emitters = n64_fx_be16(head + 2);
    first = (4u + 2u * (events + 1u) + 3u) & ~3u;
    if (bytes < 8 || first > bytes || bytes - first != emitters * N64_FX_EMITTER_BYTES) return 0;
    for (uint32_t remaining = 2u * (events + 1u); remaining;) {
        uint32_t count = remaining > sizeof(scratch) ? sizeof(scratch) : remaining;
        if (fread(scratch, 1, count, file) != count) return 0;
        for (uint32_t i = 0; i < count; i += 2) {
            uint32_t current = n64_fx_be16(scratch + i);
            if ((!seen && current) || current < previous || current > emitters) return 0;
            previous = current; seen++;
        }
        remaining -= count;
    }
    return previous == emitters;
}

void n64_particles_load(const char *map_name)
{
    char path[48];
    uint8_t header[32], colours[CELLS][4], explosions[MAXIMUM_EXPLOSION_EFFECTS][4];
    struct n64_fx_layout layout;
    struct fx_index *index = NULL;
    surface_t atlas = {0};
    FILE *file = NULL, *bank = NULL, *records;
    uint32_t map_bytes, rom, record_min, record_max;
    int ok = 0;

    reset_sniper_ribbons();
    n64_paint_reset(map_name);
    fx_free();
    flame_cell = 0;
    memset(fx.particles, 0, sizeof(fx.particles)); fx.live = 0;
    memset(fx.explosions, 0, sizeof(fx.explosions)); fx.paint_splash_budget = 4;
    if (!fx.fireball.buffer) make_fireball();

    if (!map_name || snprintf(path, sizeof(path), "fx/%s.fx", map_name) >= (int)sizeof(path)) goto done;
    rom = dfs_rom_addr(path);
    if (snprintf(path, sizeof(path), "rom:/fx/%s.fx", map_name) >= (int)sizeof(path)) goto done;
    file = fopen(path, "rb");
    if (!file || !rom || !fx_file_bytes(file, &map_bytes) ||
        !fx_read_at(file, 0, header, sizeof(header)) || !n64_fx_map_header(header, map_bytes, &layout)) goto done;

    records = file; record_min = layout.record_min; record_max = layout.record_max;
    if (layout.version == 3) {
        rom = dfs_rom_addr("fx/records.bank");
        bank = fopen("rom:/fx/records.bank", "rb");
        if (!bank || !rom || !fx_bank_validate(bank, layout.identity, &record_min, &record_max)) goto done;
        records = bank;
    }
    index = malloc(sizeof(struct fx_index) * (layout.count ? layout.count : 1));
    if (!index) goto done;
    for (int i = 0; i < layout.count; i++) {
        uint8_t entry[8];
        if (!fx_read_at(file, layout.index_offset + (uint32_t)i * 8, entry, sizeof(entry))) goto done;
        index[i].tag_index = n64_fx_be16(entry); index[i].size = n64_fx_be16(entry + 2);
        index[i].offset = n64_fx_be32(entry + 4);
        if ((i && index[i].tag_index <= index[i - 1].tag_index) ||
            !n64_fx_record_span(record_min, record_max, index[i].offset, index[i].size) ||
            !fx_rom_span(rom, index[i].offset, index[i].size) ||
            !fx_file_record(records, index[i].offset, index[i].size)) goto done;
    }
    atlas = surface_alloc(FMT_I4, ATLAS_COLUMNS * CELL, ATLAS_BYTES * 2 / (ATLAS_COLUMNS * CELL));
    if (!atlas.buffer || !fx_read_at(file, layout.atlas_offset, atlas.buffer, ATLAS_BYTES) ||
        !fx_read_at(file, layout.atlas_offset + ATLAS_BYTES, colours, sizeof(colours))) goto done;
    if (layout.explosions && !fx_read_at(file, layout.explosions_offset, explosions, (size_t)layout.explosions * 4)) goto done;
    for (int i = 0; i < layout.explosions; i++)
        if (explosions[i][2] >= N64_EXPLOSION_KINDS || explosions[i][3] ||
            (i && n64_fx_be16(explosions[i]) <= n64_fx_be16(explosions[i - 1]))) goto done;

    /* No persistent bank allocation and no visible half-loaded map state. */
    data_cache_hit_writeback(atlas.buffer, ATLAS_BYTES);
    fx.index = index; index = NULL;
    fx.atlas = atlas; memset(&atlas, 0, sizeof(atlas));
    memcpy(fx.cell_colours, colours, sizeof(colours));
    for (int i = 0; i < layout.explosions; i++) {
        fx.explosion_effects[i].tag_index = n64_fx_be16(explosions[i]);
        fx.explosion_effects[i].kind = explosions[i][2]; fx.explosion_effects[i].pad = 0;
    }
    fx.explosion_effect_count = layout.explosions;
    fx.record_min = record_min; fx.record_max = record_max;
    flame_cell = layout.flame_cell;
    fx.rom = rom;
    fx.index_count = layout.count;
    ok = 1;
done:
    if (bank) fclose(bank);
    if (file) fclose(file);
    free(index);
    if (atlas.buffer) surface_free(&atlas);
    if (!ok) fx_free();
    debugf("n64: particles for %d effects of %s, %d explosions%s\n", fx.index_count,
        map_name ? map_name : "(invalid)", fx.explosion_effect_count, ok ? "" : " (invalid or missing FX data)");
}

int n64_explosion_kind(int effect_tag_index)
{
	for (int i = 0; i < fx.explosion_effect_count; i++)
	{
		if (fx.explosion_effects[i].tag_index == effect_tag_index)
			return fx.explosion_effects[i].kind < N64_EXPLOSION_KINDS ? fx.explosion_effects[i].kind : -1;
	}
	return -1;
}

void n64_explosion(const float position[3], int kind)
{
	struct explosion *e = &fx.explosions[fx.next_explosion];

	if (fx.disabled || kind < 0 || kind >= N64_EXPLOSION_KINDS)
		return;
	if (n64_paint_enabled())
	{
		n64_paint_splash(position, explosion_looks[kind].radius, -1);
		return;
	}
	/* (full: the oldest goes) */
	fx.next_explosion = (fx.next_explosion + 1) % MAXIMUM_EXPLOSIONS;
	memcpy(e->position, position, sizeof(e->position));
	e->age = 0.f;
	e->kind = (uint8_t)kind;
	e->seed = fx.explosion_seed;
	fx.explosion_seed += 37;
	e->live = 1;
	e->paint = 0;
}

/* Authored impact leaves only: firing, muzzle, Flame jet/pilot and ambient
   effects never enter this path. Called once per effect event, not per drop. */
int n64_paint_effect(const char *name)
{
	const char *leaf;
	if (!name || (strncmp(name, "weapons\\", 8) && strncmp(name, "vehicles\\", 9))) return 0;
	leaf = strrchr(name, '\\');
	if (!leaf) return 0;
	leaf++;
	if (strstr(leaf, "impact")) return 1;
	if (strstr(leaf, "explosion") || (strstr(leaf, "detonat") && strcmp(leaf, "detonation start"))) return 2;
	return 0;
}

void n64_paint_splash(const float position[3], float radius, int color)
{
	struct explosion *e;
	unsigned char size;
	if (fx.disabled || !n64_paint_enabled() || !position || !isfinite(radius) || radius <= 0.f) return;
	for (int k = 0; k < 3; k++) if (!isfinite(position[k])) return;
	/* Paint reuses kind as a quantized size. It is never an explosion index. */
	size = (unsigned char)(n64_min_f(2.5f, n64_max_f(.125f, radius)) * 32.f + .5f);
	/* A surface decal can follow its effect in the same update. Merge that
   pair once here, before any per-view/per-drop work, and adopt its ink hue. */
	for (int i = 0; i < MAXIMUM_EXPLOSIONS; i++)
	{
		float distance = 0.f;
		e = &fx.explosions[i];
		if (!e->live || !e->paint || e->age > .05f) continue;
		for (int k = 0; k < 3; k++) { float d = e->position[k] - position[k]; distance += d * d; }
		if (distance > .0625f) continue;
		if (color >= 0 && color < 6) e->paint = (unsigned char)(color + 1);
		if (size > e->kind) e->kind = size;
		return;
	}
	if (fx.paint_splash_budget <= 0) return;
	fx.paint_splash_budget--;
	e = &fx.explosions[fx.next_explosion];
	fx.next_explosion = (fx.next_explosion + 1) % MAXIMUM_EXPLOSIONS;
	memcpy(e->position, position, sizeof(e->position));
	e->age = 0.f;
	e->kind = size;
	e->seed = fx.explosion_seed;
	fx.explosion_seed += 37;
	e->live = 1;
	e->paint = (unsigned char)((color >= 0 && color < 6 ? color : e->seed % 6) + 1);
}

/* an effect's record (by binary search of the index; read from the
cartridge into the cache), or NULL */
static const uint8_t *fx_record(int tag_index)
{
	int low = 0, high = fx.index_count - 1, slot = 0;

	fx.clock++;
	for (int i = 0; i < CACHE_ENTRIES; i++)
	{
		if (fx.cache[i].data && fx.cache[i].tag_index == tag_index)
		{
			fx.cache[i].used = fx.clock;
			return fx.cache[i].data;
		}
	}
	/* (a free entry, else the least recently used) */
	for (int i = 0; i < CACHE_ENTRIES; i++)
	{
		if (!fx.cache[i].data)
		{
			slot = i;
			break;
		}
		if (fx.cache[i].used < fx.cache[slot].used)
			slot = i;
	}
	while (low <= high)
	{
		int middle = (low + high) / 2;
		struct fx_index const *entry = &fx.index[middle];

		if (entry->tag_index == tag_index)
		{
			int size = (entry->size + 15) & ~15;

			if (!n64_fx_record_span(fx.record_min, fx.record_max, entry->offset, entry->size) ||
				!fx_rom_span(fx.rom, entry->offset, entry->size)) return NULL;
			free(fx.cache[slot].data);
			fx.cache[slot].tag_index = -1;
			fx.cache[slot].data = memalign(16, size);
			if (!fx.cache[slot].data)
				return NULL;
			data_cache_hit_writeback_invalidate(fx.cache[slot].data, size);
			dma_read(fx.cache[slot].data, fx.rom + entry->offset, size);
			if (!n64_fx_record_memory(fx.cache[slot].data, entry->size)) {
				free(fx.cache[slot].data); fx.cache[slot].data = NULL;
				return NULL;
			}
			fx.cache[slot].tag_index = tag_index;
			fx.cache[slot].used = fx.clock;
			fx.reads++;
			return fx.cache[slot].data;
		}
		if (entry->tag_index < tag_index)
			low = middle + 1;
		else
			high = middle - 1;
	}
	return NULL;
}

const struct n64_fx_emitter *n64_fx_event(int effect_tag_index, int event_index, int *count)
{
	const uint8_t *record;
	const uint16_t *head;
	int events, first, last;

	*count = 0;
	if (!fx.index_count || !(record = fx_record(effect_tag_index)))
		return NULL;
	head = (const uint16_t *)record;
	events = head[0];
	if (event_index < 0 || event_index >= events)
		return NULL;
	first = head[2 + event_index];
	last = head[3 + event_index];
	*count = last - first;
	return (const struct n64_fx_emitter *)(record + ((4 + 2 * (events + 1) + 3) & ~3)) + first;
}

void n64_particle_spawn(const struct n64_particle_spawn *spawn)
{
	struct particle *p = &fx.particles[fx.next];

	if (fx.disabled)
		return;
	/* (a budget: ~300 a second, bursts of up to 64 -- a muzzle flash or an
	impact gets through, a stream cannot fill the pool) */
	if (fx.tokens < 1.f)
	{
		fx.refused++;
		return;
	}
	fx.tokens -= 1.f;

	/* (full: the oldest goes) */
	fx.next = (fx.next + 1) % MAXIMUM_PARTICLES;
	if (!p->live)
		fx.live++;
	memcpy(p->position, spawn->position, sizeof(p->position));
	memcpy(p->velocity, spawn->velocity, sizeof(p->velocity));
	p->age = 0.f;
	/* (a muzzle flash lives ~30 ms, less than a frame: at least 80, so that
	it is drawn at all) */
	p->life = spawn->life > 0.08f ? spawn->life : 0.08f;
	if (enhanced_fx && p->life < 0.11f) p->life = 0.11f;
	p->fade_in = spawn->fade_in;
	p->fade_out = spawn->fade_out;
	p->radius = spawn->radius;
	if (enhanced_fx && (spawn->flags & N64_PARTICLE_FIRST_PERSON)) p->radius *= 1.08f;
	p->end_scale = spawn->end_scale;
	p->gravity = spawn->gravity;
	p->drag = spawn->drag;
	p->color = spawn->color;
	p->cell = spawn->cell;
	p->flags = spawn->flags;
	p->local_player = spawn->local_player;
	p->live = 1;
	fx.spawned++;
}

/* Follow the real damaging projectile. Xbox's ribbon contrails are absent
from the native renderer, so a bounded, expanding fire trail makes the PC
Flamethrower readable. It shares the existing particle budget in every view. */
void n64_flame_trail(const float *position, const float *velocity)
{
	struct n64_particle_spawn p = {0};
	for (int k = 0; k < 3; k++)
	{
		p.position[k] = position[k];
		p.velocity[k] = velocity[k] * 3.f;
	}
	p.velocity[2] += 0.12f;
	p.radius = 0.055f;
	p.end_scale = enhanced_fx ? 3.2f : 2.f;
	p.life = enhanced_fx ? 0.32f : 0.2f;
	p.fade_in = 0.025f;
	p.fade_out = 0.18f;
	p.drag = 2.f;
	p.color = 0xFFB040D0;
	p.cell = flame_cell;
	p.flags = N64_PARTICLE_FLAME;
	p.local_player = -1;
	n64_particle_spawn(&p);
}

void n64_particles_update(float dt)
{
	/* At most four new bursts per simulation/render update, shared by views. */
	fx.paint_splash_budget = 4;
	n64_paint_update(dt);
	/* Age even if the sprite pool is empty. No simulation tick/RNG changes. */
	if (isfinite(dt) && dt > 0.f)
		for (int i = 0; i < N64_SNIPER_RIBBONS; i++)
			if (sniper_ribbons[i].age < N64_SNIPER_LIFE) sniper_ribbons[i].age += dt;
	static float log_time;

	/* (port.json flag "debug_particles": the counts every five seconds) */
	log_time += dt;
	if (log_time >= 5.f)
	{
		log_time = 0.f;
		if (n64_debug_flag("debug_particles"))
			debugf("particles: %lu made, %lu refused, %lu drawn, %lu records read, %d live\n", fx.spawned,
				fx.refused, fx.drawn, fx.reads, fx.live);
	}
	fx.tokens += dt * SPAWNS_PER_SECOND * fx.budget;
	if (fx.tokens > SPAWN_BURST * fx.budget)
		fx.tokens = SPAWN_BURST * fx.budget;
	for (int i = 0; i < MAXIMUM_EXPLOSIONS && dt > 0.f; i++)
	{
		struct explosion *e = &fx.explosions[i];

		if (e->live && (e->age += n64_min_f(dt, 0.25f)) >= (e->paint ? .48f : SMOKE_START + SMOKE_TIME))
			e->live = 0;
	}
	if (!fx.live || dt <= 0.f)
		return;
	if (dt > 0.25f)
		dt = 0.25f;
	for (int i = 0; i < MAXIMUM_PARTICLES; i++)
	{
		struct particle *p = &fx.particles[i];
		float keep;

		if (!p->live)
			continue;
		p->age += dt;
		if (p->age >= p->life)
		{
			p->live = 0;
			fx.live--;
			continue;
		}
		if (!(p->flags & N64_PARTICLE_FIRST_PERSON))
			p->velocity[2] -= p->gravity * dt;
		keep = 1.f - p->drag * dt;
		if (keep < 0.f)
			keep = 0.f;
		for (int k = 0; k < 3; k++)
		{
			p->velocity[k] *= keep;
			p->position[k] += p->velocity[k] * dt;
		}
	}
}

struct drawn
{
	float z;
	float x0, y0, x1, y1;
	uint32_t color;
	uint16_t depth;
	uint8_t cell;
	uint8_t flags;
};

static int compare_drawn(const void *a, const void *b)
{
	float za = ((const struct drawn *)a)->z, zb = ((const struct drawn *)b)->z;

	return za < zb ? 1 : za > zb ? -1 : 0;
}

static void draw_particles(const struct n64_particle_view *view)
{
	static struct drawn drawn[MAXIMUM_PARTICLES];
	int count = 0;
	float m22, m32;

	if (!fx.live || !fx.atlas.buffer)
		return;
	/* tiny3d's depth: OpenGL's projection, then (ndc / 2 + 1/2) x 0x7FFF */
	m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);
	for (int i = 0; i < MAXIMUM_PARTICLES; i++)
	{
		struct particle const *p = &fx.particles[i];
		struct drawn *d = &drawn[count];
		float x, y, z, radius, alpha, scale, depth;
		int first_person = p->flags & N64_PARTICLE_FIRST_PERSON;

		if (!p->live || first_person != (view->first_person ? N64_PARTICLE_FIRST_PERSON : 0))
			continue;
		if (first_person ? p->local_player != view->local_player :
			(p->flags & N64_PARTICLE_THIRD_PERSON_ONLY) && p->local_player == view->local_player)
			continue;
		if (first_person)
		{
			x = p->position[0];
			y = p->position[1];
			z = p->position[2];
		}
		else
		{
			float dx = p->position[0] - view->eye[0];
			float dy = p->position[1] - view->eye[1];
			float dz = p->position[2] - view->eye[2];

			x = dx * view->right[0] + dy * view->right[1] + dz * view->right[2];
			y = dx * view->up[0] + dy * view->up[1] + dz * view->up[2];
			z = dx * view->forward[0] + dy * view->forward[1] + dz * view->forward[2];
		}
		if (z < view->z_near * 2.f || (view->fog_end > 0.f && z > view->fog_end))
			continue;
		/* (half again Halo's size: fewer, larger particles) */
		radius = p->radius * 1.5f * (1.f + (p->end_scale - 1.f) * (p->age / p->life));
		x *= view->x_scale;
		scale = view->pixels_per_unit / z;
		d->x0 = view->center_x + x * scale;
		d->y0 = view->center_y - y * scale;
		radius *= scale;
		/* (no bigger than MAXIMUM_PIXELS across a half: a flash right at the
		eye filled the view) */
		if (radius > MAXIMUM_PIXELS * view->pixels_per_unit / 207.f)
			radius = MAXIMUM_PIXELS * view->pixels_per_unit / 207.f;
		if (radius < 0.4f || d->x0 + radius < view->x0 || d->x0 - radius > view->x1 ||
			d->y0 + radius < view->y0 || d->y0 - radius > view->y1)
			continue;
		/* fading out over its whole life */
		alpha = 1.f - p->age / p->life;
		/* Use the source's fade-in/out windows for a smooth flame envelope. */
		if (enhanced_fx)
		{
			alpha = 1.f;
			if (p->fade_in > 0.f && p->age < p->fade_in) alpha *= p->age / p->fade_in;
			if (p->fade_out > 0.f && p->life - p->age < p->fade_out)
				alpha *= (p->life - p->age) / p->fade_out;
			if (p->fade_out <= 0.f) alpha *= 1.f - p->age / p->life;
		}
		if (view->fog_end > 0.f && z > view->fog_start)
			alpha *= 1.f - (z - view->fog_start) / (view->fog_end - view->fog_start);
		alpha *= (p->flags & N64_PARTICLE_ADDITIVE) ? 1.f : (p->color & 0xFF) * (1.f / 255.f);
		if (alpha < 0.02f)
			continue;
		/* (a hair nearer than the surface it sits on) */
		depth = (-m22 + m32 / (z * 0.995f)) * 0.5f + 0.5f;
		if (depth < 0.f)
			depth = 0.f;
		d->depth = depth >= 1.f ? 0x7FFF : (uint16_t)(depth * 0x7FFF);
		d->x1 = d->x0 + radius;
		d->y1 = d->y0 + radius;
		d->x0 -= radius;
		d->y0 -= radius;
		if (view->chunky)
		{
			/* (the view is drawn half as wide, then widened) */
			d->x0 = view->x0 + (d->x0 - view->x0) * view->chunky_scale;
			d->x1 = view->x0 + (d->x1 - view->x0) * view->chunky_scale;
		}
		d->z = z;
		{
			/* the tint times the cell's own colour */
			uint8_t const *hue = fx.cell_colours[p->cell & (CELLS - 1)];
			uint32_t r = n64_div255((p->color >> 24) * hue[0]), g = n64_div255(((p->color >> 16) & 0xFF) * hue[1]);
			uint32_t b = n64_div255(((p->color >> 8) & 0xFF) * hue[2]);

			d->color = r << 24 | g << 16 | b << 8 | (uint32_t)(alpha * 255.f);
			if (p->flags & N64_PARTICLE_FLAME)
			{
				float cool = p->age / p->life;
				d->color = 0xFF000000 | (uint32_t)(242.f - 82.f * cool) << 16 |
					(uint32_t)(190.f - 150.f * cool) << 8 | (uint32_t)(alpha * 255.f);
			}
		}
		d->cell = p->cell;
		d->flags = p->flags;
		count++;
	}
	if (!count)
		return;
	qsort(drawn, count, sizeof(drawn[0]), compare_drawn);
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	/* (the colour flat, the glow's shape in the alpha: colour times the
	texture as well darkened every edge into a ring) */
	rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_zbuf(true, false);
	rdpq_mode_zoverride(true, 0.f, 0);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_tex_upload(TILE0, &fx.atlas, NULL);
	int last_flame = 0;
	for (int i = 0; i < count; i++)
	{
		struct drawn const *d = &drawn[i];
		/* (one clean soft glow for every particle, in its own colour: Halo's
		pictures at 16x16 were noise) */
		int s = 0, t = 0;
		int flame = !!(d->flags & N64_PARTICLE_FLAME);
		if (flame != last_flame)
		{
			rdpq_sync_pipe();
			if (flame)
			{
				rdpq_mode_combiner(RDPQ_COMBINER1((PRIM, ENV, TEX0, ENV), (TEX0, 0, PRIM, 0)));
				rdpq_set_env_color(RGBA32(200, 32, 0, 255));
			}
			else rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
			last_flame = flame;
		}
		if (flame)
		{
			s = (d->cell % ATLAS_COLUMNS) * CELL;
			t = (d->cell / ATLAS_COLUMNS) * CELL;
		}

		rdpq_set_prim_color(RGBA32(d->color >> 24, (d->color >> 16) & 0xFF, (d->color >> 8) & 0xFF, d->color & 0xFF));
		rdpq_set_prim_depth_raw(d->depth, 0);
		rdpq_texture_rectangle_scaled(TILE0, d->x0, d->y0, d->x1, d->y1, s, t, s + CELL, t + CELL);
	}
	rdpq_mode_zoverride(false, 0.f, 0);
	fx.drawn += count;
}

struct puff
{
	float z, x0, y0, x1, y1;
	uint8_t prim[4], env[3];
	uint8_t flip;
	uint16_t depth;
	uint8_t paint;
};
typedef char puff_size_check[sizeof(struct puff) == 32 ? 1 : -1];

static int compare_puffs(const void *a, const void *b)
{
	float za = ((const struct puff *)a)->z, zb = ((const struct puff *)b)->z;

	return za < zb ? 1 : za > zb ? -1 : 0;
}

static void mix(const uint8_t *a, const uint8_t *b, float t, uint8_t *out)
{
	for (int k = 0; k < 3; k++)
		out[k] = (uint8_t)(a[k] + (b[k] - a[k]) * t);
}

/* the fire's colour as it cools: 0 the flash's, 1 the coolest */
static void fire_colour(const struct explosion_look *look, float t, uint8_t *out)
{
	if (t < 0.2f)
		mix(look->flash, look->hot, t / 0.2f, out);
	else if (t < 0.5f)
		mix(look->hot, look->warm, (t - 0.2f) / 0.3f, out);
	else
		mix(look->warm, look->cool, n64_min_f(1.f, (t - 0.5f) / 0.5f), out);
}

/* One coherent ink color: a quick irregular splat and six falling drops.
   No smoke, white flash, hot core or color cycling in Paintball impacts. */
static int paint_puff(const struct explosion *e, int part, const float direction[3],
	float size, float offset[3], float *radius, float *alpha, struct puff *p)
{
	float r = e->kind * (1.f / 32.f), t;
	const unsigned char *color;
	if (!e->paint || e->paint > 6 || part < 0 || part > 6 || !isfinite(e->age) || e->age < 0.f) return 0;
	if (part == 0)
	{
		t = e->age / .14f;
		if (t >= 1.f) return 0;
		*radius = r * (.48f + .32f * t);
		*alpha = .95f * (1.f - t);
	}
	else
	{
		t = e->age / .48f;
		if (t >= 1.f) return 0;
		for (int k = 0; k < 3; k++) offset[k] = direction[k] * r * (.12f + 1.8f * t);
		offset[2] += r * (.45f * t - 1.5f * t * t);
		*radius = r * .085f * size * (1.f - .35f * t);
		*alpha = .95f * (1.f - t * t);
	}
	color = n64_paint_color(e->paint - 1);
	memcpy(p->prim, color, 3);
	memcpy(p->env, color, 3);
	return 1;
}

static void draw_explosions(const struct n64_particle_view *view)
{
	static struct puff puffs[MAXIMUM_EXPLOSIONS * EXPLOSION_PUFFS];
	int count = 0;
	/* (an explosion at the eye fills most of the view, not more) */
	float limit = (view->y1 - view->y0) * 0.8f;
	float m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	float m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);

	if (!fx.fireball.buffer)
		return;
	/* (testing, port.json flag "debug_explosions": one every two seconds
	ahead of the first view, each kind in turn) */
	if (view->local_player == 0 && n64_debug_flag("debug_explosions"))
	{
		static unsigned long long last;
		static int kind;
		unsigned long long now = get_ticks();

		if (!last || now - last > 2ull * TICKS_PER_SECOND)
		{
			float at[3];

			for (int k = 0; k < 3; k++)
				at[k] = view->eye[k] + view->forward[k] * 6.f - view->up[k] * 0.5f;
			n64_explosion(at, kind);
			kind = (kind + 1) % N64_EXPLOSION_KINDS;
			last = now;
		}
	}
	for (int i = 0; i < MAXIMUM_EXPLOSIONS; i++)
	{
		struct explosion const *e = &fx.explosions[i];
		const struct explosion_look *look;
		float r;

		if (!e->live)
			continue;
		look = e->paint ? NULL : &explosion_looks[e->kind];
		r = e->paint ? e->kind * (1.f / 32.f) : look->radius;
		for (int part = 0; part < EXPLOSION_PUFFS; part++)
		{
			struct puff *p = &puffs[count];
			/* the puff's own direction, size and turn, from the explosion's seed */
			float d[3] = {hash2(part, 1, e->seed) * 2.f - 1.f, hash2(part, 2, e->seed) * 2.f - 1.f,
				hash2(part, 3, e->seed) * 1.4f - 0.4f};
			float length = sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) + 1e-3f;
			float size = 0.75f + 0.5f * hash2(part, 4, e->seed);
			float offset[3] = {0.f, 0.f, 0.f}, radius, alpha, t;
			float x, y, z, scale, depth, dx, dy, dz;

			for (int k = 0; k < 3; k++)
				d[k] /= length;
			p->paint = e->paint;
			if (e->paint)
			{
				if (!paint_puff(e, part, d, size, offset, &radius, &alpha, p)) continue;
			}
			else if (part == 0)
			{
				/* the flash */
				if (e->age >= FLASH_TIME)
					continue;
				t = e->age / FLASH_TIME;
				radius = r * 1.7f * (0.6f + 0.4f * t);
				alpha = 0.95f * (1.f - t);
				memcpy(p->prim, look->flash, 3);
				memcpy(p->env, look->hot, 3);
			}
			else if (part <= FIRE_PUFFS)
			{
				/* the fireball: puffs swelling outwards and up as they cool */
				t = e->age / FIRE_TIME;
				if (t >= 1.f)
					continue;
				for (int k = 0; k < 3; k++)
					offset[k] = d[k] * r * (0.2f + 0.45f * t);
				offset[2] += r * 0.45f * t;
				radius = r * 0.55f * size * (0.5f + 0.75f * sqrtf(t));
				fire_colour(look, t, p->prim);
				fire_colour(look, n64_min_f(1.f, t + 0.35f), p->env);
				alpha = t < 0.45f ? 1.f : 1.f - (t - 0.45f) / 0.55f;
			}
			else
			{
				/* the smoke: rising and spreading after the fire */
				t = (e->age - SMOKE_START) / SMOKE_TIME;
				if (t <= 0.f || t >= 1.f || look->smoke_amount <= 0.f)
					continue;
				for (int k = 0; k < 3; k++)
					offset[k] = d[k] * r * 0.45f;
				offset[2] += r * (0.4f + 1.4f * t);
				radius = r * 0.6f * size * (0.7f + 0.9f * t);
				for (int k = 0; k < 3; k++)
				{
					p->prim[k] = (uint8_t)n64_min_f(255.f, look->smoke[k] * 1.3f);
					p->env[k] = (uint8_t)(look->smoke[k] * 0.75f);
				}
				alpha = look->smoke_amount * 0.6f * (t < 0.15f ? t / 0.15f : 1.f - (t - 0.15f) / 0.85f);
			}
			/* placed like a particle */
			dx = e->position[0] + offset[0] - view->eye[0];
			dy = e->position[1] + offset[1] - view->eye[1];
			dz = e->position[2] + offset[2] - view->eye[2];
			x = dx * view->right[0] + dy * view->right[1] + dz * view->right[2];
			y = dx * view->up[0] + dy * view->up[1] + dz * view->up[2];
			z = dx * view->forward[0] + dy * view->forward[1] + dz * view->forward[2];
			if (z < view->z_near * 2.f || (view->fog_end > 0.f && z > view->fog_end))
				continue;
			x *= view->x_scale;
			scale = view->pixels_per_unit / z;
			radius *= scale;
			if (radius > limit)
				radius = limit;
			p->x0 = view->center_x + x * scale - radius;
			p->x1 = view->center_x + x * scale + radius;
			p->y0 = view->center_y - y * scale - radius;
			p->y1 = view->center_y - y * scale + radius;
			if (radius < 0.5f || p->x1 < view->x0 || p->x0 > view->x1 || p->y1 < view->y0 || p->y0 > view->y1)
				continue;
			if (view->fog_end > 0.f && z > view->fog_start)
				alpha *= 1.f - (z - view->fog_start) / (view->fog_end - view->fog_start);
			if (alpha < 0.02f)
				continue;
			p->prim[3] = (uint8_t)(n64_min_f(alpha, 1.f) * 255.f);
			/* (its depth a little nearer than its middle: the ground it sits
			on does not cut it in half) */
			depth = n64_max_f(z - radius / scale * 0.6f, view->z_near * 2.f);
			depth = (-m22 + m32 / depth) * 0.5f + 0.5f;
			p->depth = depth >= 1.f ? 0x7FFF : depth <= 0.f ? 0 : (uint16_t)(depth * 0x7FFF);
			if (view->chunky)
			{
				p->x0 = view->x0 + (p->x0 - view->x0) * view->chunky_scale;
				p->x1 = view->x0 + (p->x1 - view->x0) * view->chunky_scale;
			}
			p->z = z;
			p->flip = ((uint8_t)(hash2(part, 5, e->seed) * 4.f) & 3) |
				(e->paint ? (e->seed % 3) << 2 : 0);
			count++;
		}
	}
	if (!count)
		return;
	qsort(puffs, count, sizeof(puffs[0]), compare_puffs);
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	/* the colour from the edge's (ENV) to the middle's (PRIM) by the texel,
	the texel times the puff's alpha the alpha */
	rdpq_mode_combiner(RDPQ_COMBINER1((PRIM, ENV, TEX0, ENV), (TEX0, 0, PRIM, 0)));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_zbuf(true, false);
	rdpq_mode_zoverride(true, 0.f, 0);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_tex_upload(TILE0, &fx.fireball, NULL);
	/* The three immutable 32x32 I4 splats share TMEM with the 32x32 I8
	   fireball: 1024 + 3*512 = 2560 bytes, below the RDP's 4096 bytes. */
	int paint_textures = 0;
	for (int i = 0; i < count; i++) if (puffs[i].paint) { paint_textures = 1; break; }
	if (paint_textures)
		for (int pattern = 0; pattern < 3; pattern++)
		{
			const unsigned char *data = n64_paint_mask(pattern);
			if (data)
			{
				surface_t mask = surface_make_linear((void *)data, FMT_I4, 32, 32);
				rdpq_tex_upload(TILE1 + pattern, &mask, &(rdpq_texparms_t){.tmem_addr = 1024 + pattern * 512});
			}
		}
	for (int i = 0; i < count; i++)
	{
		struct puff const *p = &puffs[i];
		int s0 = p->flip & 1 ? FIREBALL : 0, t0 = p->flip & 2 ? FIREBALL : 0;
		int tile = p->paint ? TILE1 + ((p->flip >> 2) % 3) : TILE0;
		if (p->paint && !n64_paint_mask((p->flip >> 2) % 3)) continue;

		rdpq_set_prim_color(RGBA32(p->prim[0], p->prim[1], p->prim[2], p->prim[3]));
		rdpq_set_env_color(RGBA32(p->env[0], p->env[1], p->env[2], 0xFF));
		rdpq_set_prim_depth_raw(p->depth, 0);
		rdpq_texture_rectangle_scaled(tile, p->x0, p->y0, p->x1, p->y1, s0, t0, FIREBALL - s0, FIREBALL - t0);
	}
	rdpq_mode_zoverride(false, 0.f, 0);
}

static void draw_sniper_ribbons(const struct n64_particle_view *view)
{
    int started = 0;
    for (int i = 0; i < N64_SNIPER_RIBBONS; i++)
    {
        float projected[N64_SNIPER_CLIP][6];
        float age = sniper_ribbons[i].age;
        if (age >= N64_SNIPER_LIFE) continue;
        int count = sniper_ribbon_geometry(view, sniper_ribbons[i].start, sniper_ribbons[i].end, projected);
        if (count < 3) continue;
        if (!started)
        {
            surface_t mask = surface_make_linear(sniper_mask, FMT_I4, 16, 4);
            data_cache_hit_writeback(sniper_mask, sizeof(sniper_mask));
            rdpq_sync_pipe();
            rdpq_set_mode_standard();
            rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
            rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
            rdpq_mode_zbuf(true, false);
            rdpq_mode_persp(true);
            rdpq_mode_filter(FILTER_BILINEAR);
            rdpq_tex_upload(TILE0, &mask, NULL);
            started = 1;
        }
        float alpha = 1.f - age / N64_SNIPER_LIFE;
        if (view->fog_end > view->fog_start)
        {
            float z = 1.f / projected[0][5];
            if (z > view->fog_end) continue;
            if (z > view->fog_start) alpha *= 1.f - (z - view->fog_start) / (view->fog_end - view->fog_start);
        }
        /* Pale vapor, deliberately much quieter than impact flashes. */
        rdpq_set_prim_color(RGBA32(205, 225, 235, (int)(alpha * alpha * 76.f)));
        for (int v = 1; v < count - 1; v++) rdpq_triangle(&TRIFMT_ZBUF_TEX, projected[0], projected[v], projected[v + 1]);
    }
}

void n64_particles_draw(const struct n64_particle_view *view)
{
	if (fx.disabled) return;
	draw_particles(view);
	/* (explosions are the world's: not in the first-person pass) */
	if (!view->first_person)
	{
		draw_explosions(view);
		draw_sniper_ribbons(view);
	}
}

void n64_particles_statistics(unsigned long *spawned, unsigned long *drawn, unsigned long *reads, int *live)
{
	*spawned = fx.spawned;
	*drawn = fx.drawn;
	*reads = fx.reads;
	*live = fx.live;
}

void n64_particles_set_view_count(int views)
{
	fx.budget = views > 2 ? 0.5f : 1.f;
}

void n64_particles_set_enabled(int on)
{
	fx.disabled = !on;
}
