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
};

struct explosion
{
	float position[3];
	float age;
	uint8_t kind, seed, live;
};

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
	uint8_t explosion_seed;
	surface_t fireball;
} fx;

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

void n64_particles_load(const char *map_name)
{
	char path[48];
	uint8_t header[32];
	FILE *file;

	fx_free();
	memset(fx.particles, 0, sizeof(fx.particles));
	fx.live = 0;
	memset(fx.explosions, 0, sizeof(fx.explosions));
	fx.explosion_effect_count = 0;
	if (!fx.fireball.buffer)
		make_fireball();
	snprintf(path, sizeof(path), "fx/%s.fx", map_name);
	fx.rom = dfs_rom_addr(path);
	snprintf(path, sizeof(path), "rom:/fx/%s.fx", map_name);
	file = fopen(path, "rb");
	if (!file || !fx.rom)
	{
		if (file)
			fclose(file);
		debugf("n64: no particles for %s\n", map_name);
		fx.rom = 0;
		return;
	}
	if (fread(header, 1, sizeof(header), file) == sizeof(header) && !memcmp(header, "N64F", 4))
	{
		int count = header[8] << 8 | header[9];
		uint32_t index_offset = (uint32_t)header[12] << 24 | header[13] << 16 | header[14] << 8 | header[15];
		uint32_t atlas_offset = (uint32_t)header[16] << 24 | header[17] << 16 | header[18] << 8 | header[19];

		fx.index = malloc(sizeof(struct fx_index) * (count ? count : 1));
		fseek(file, index_offset, SEEK_SET);
		if (fx.index && (int)fread(fx.index, sizeof(struct fx_index), count, file) == count)
			fx.index_count = count;
		fx.atlas = surface_alloc(FMT_I4, ATLAS_COLUMNS * CELL, ATLAS_BYTES * 2 / (ATLAS_COLUMNS * CELL));
		fseek(file, atlas_offset, SEEK_SET);
		if (fx.atlas.buffer)
		{
			fread(fx.atlas.buffer, 1, ATLAS_BYTES, file);
			data_cache_hit_writeback(fx.atlas.buffer, ATLAS_BYTES);
		}
		/* each cell's colour, after the atlas */
		fread(fx.cell_colours, 4, CELLS, file);
		/* the explosions' list (version 2: bytes 20-25 of the header) */
		{
			uint32_t explosions_offset = (uint32_t)header[20] << 24 | header[21] << 16 | header[22] << 8 | header[23];
			int explosions = header[24] << 8 | header[25];

			if (explosions_offset && explosions > 0)
			{
				if (explosions > MAXIMUM_EXPLOSION_EFFECTS)
					explosions = MAXIMUM_EXPLOSION_EFFECTS;
				fseek(file, explosions_offset, SEEK_SET);
				fx.explosion_effect_count = (int)fread(fx.explosion_effects, 4, explosions, file);
			}
		}
	}
	fclose(file);
	debugf("n64: particles for %d effects of %s, %d explosions\n", fx.index_count, map_name,
		fx.explosion_effect_count);
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

	if (kind < 0 || kind >= N64_EXPLOSION_KINDS)
		return;
	/* (full: the oldest goes) */
	fx.next_explosion = (fx.next_explosion + 1) % MAXIMUM_EXPLOSIONS;
	memcpy(e->position, position, sizeof(e->position));
	e->age = 0.f;
	e->kind = (uint8_t)kind;
	e->seed = fx.explosion_seed;
	fx.explosion_seed += 37;
	e->live = 1;
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

			free(fx.cache[slot].data);
			fx.cache[slot].data = memalign(16, size);
			if (!fx.cache[slot].data)
				return NULL;
			data_cache_hit_writeback_invalidate(fx.cache[slot].data, size);
			dma_read(fx.cache[slot].data, fx.rom + entry->offset, size);
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
	p->fade_in = spawn->fade_in;
	p->fade_out = spawn->fade_out;
	p->radius = spawn->radius;
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

void n64_particles_update(float dt)
{
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

		if (e->live && (e->age += n64_min_f(dt, 0.25f)) >= SMOKE_START + SMOKE_TIME)
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
			uint32_t r = ((p->color >> 24) * hue[0]) / 255, g = (((p->color >> 16) & 0xFF) * hue[1]) / 255;
			uint32_t b = (((p->color >> 8) & 0xFF) * hue[2]) / 255;

			d->color = r << 24 | g << 16 | b << 8 | (uint32_t)(alpha * 255.f);
		}
		d->cell = p->cell;
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
	for (int i = 0; i < count; i++)
	{
		struct drawn const *d = &drawn[i];
		/* (one clean soft glow for every particle, in its own colour: Halo's
		pictures at 16x16 were noise) */
		int s = 0, t = 0;

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
};

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
		const struct explosion_look *look = &explosion_looks[e->kind];
		float r = look->radius;

		if (!e->live)
			continue;
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
			if (part == 0)
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
			p->flip = (uint8_t)(hash2(part, 5, e->seed) * 4.f) & 3;
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
	for (int i = 0; i < count; i++)
	{
		struct puff const *p = &puffs[i];
		int s0 = p->flip & 1 ? FIREBALL : 0, t0 = p->flip & 2 ? FIREBALL : 0;

		rdpq_set_prim_color(RGBA32(p->prim[0], p->prim[1], p->prim[2], p->prim[3]));
		rdpq_set_env_color(RGBA32(p->env[0], p->env[1], p->env[2], 0xFF));
		rdpq_set_prim_depth_raw(p->depth, 0);
		rdpq_texture_rectangle_scaled(TILE0, p->x0, p->y0, p->x1, p->y1, s0, t0, FIREBALL - s0, FIREBALL - t0);
	}
	rdpq_mode_zoverride(false, 0.f, 0);
}

void n64_particles_draw(const struct n64_particle_view *view)
{
	draw_particles(view);
	/* (explosions are the world's: not in the first-person pass) */
	if (!view->first_person)
		draw_explosions(view);
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
