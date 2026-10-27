/* Bounded, surface-clipped, perspective-correct paint and native decals. */
#include <libdragon.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "n64.h"

#define PAINT_MARKS 32
#define PAINT_CELL 32
#define PAINT_LIFE 30.f
/* A convex twelve-vertex mark can gain one vertex at each of five planes. */
#define PAINT_CLIP_VERTICES (N64_PAINT_VERTICES + 6)
static struct paint_mark
{
	struct n64_paint_vertex v[N64_PAINT_VERTICES];
	float age;
	unsigned char count, pattern, color;
	unsigned char native_slot;
	float lifetime, fade;
	unsigned char rgba[4];
} marks[PAINT_MARKS];
typedef char decal_mark_size_check[sizeof(struct paint_mark) == 260 ? 1 : -1];
typedef char decal_mark_native_offset_check[offsetof(struct paint_mark, native_slot) == 247 ? 1 : -1];
typedef char decal_mark_life_offset_check[offsetof(struct paint_mark, lifetime) == 248 ? 1 : -1];
typedef char decal_mark_rgba_offset_check[offsetof(struct paint_mark, rgba) == 256 ? 1 : -1];
static int next_mark;
static unsigned char masks[3 * PAINT_CELL * PAINT_CELL / 2] __attribute__((aligned(16)));
static int masks_loaded;
static int paint_no_depth = -1;
/* Settings: DECALS (the game's own marks: pattern 255) */
static int decals_on = 1;
/* A mark is drawn a little toward the eye, along each vertex's ray (the
same place on the screen, nearer in depth), and tested against the depth
as any surface is. It lies on the exact plane of the surface it marks, but
that surface was drawn from vertices at 1/64 of a unit (tools/n64/level.py)
and from a depth buffer that tells surfaces apart by about d^2 / (near x
32768) at d units -- 5 cm at 10 with Halo's near plane of 1/16. In the
RDP's decal mode (equal depths only) the marks came and went with the angle
and lost pieces where the two disagreed. So: PAINT_PULL_MINIMUM units, past
the vertices' rounding, and PAINT_PULL_STEPS of the depth buffer's steps
at the mark's distance. (Drawn before the objects: n64_render.c, so the
objects in front of a mark still cover it) */
#define PAINT_PULL_MINIMUM 0.012f
#define PAINT_PULL_STEPS 3.f

void n64_paint_set_decals(int on)
{
	decals_on = on != 0;
	if (!decals_on)
		for (int i = 0; i < PAINT_MARKS; i++)
			if (marks[i].pattern == 255)
				marks[i].count = 0;
}
/* The game's own decals' textures (tools/n64/decal_bank.py, version 2): IA8
32x32 -- the ink's brightness and coverage at sixteen levels each, as the
paint's masks have -- then its tint (r, g, b) and whether it darkens. (The
first bank's were CI4 with one bit of alpha, dithered: blood was a scatter
of dots, a bullet hole a few black ones.) DECAL_SLOTS of them in memory: a
plasma shot alone takes three (the burn and its two glows), and with four
every new kind of mark threw one out with the marks that used it */
#define DECAL_SLOTS 8
#define DECAL_BYTES (PAINT_CELL * PAINT_CELL + 16)
#define DECAL_BANK_VERSION 2
static unsigned char decal_textures[DECAL_SLOTS][DECAL_BYTES] __attribute__((aligned(16)));
static unsigned int decal_keys[DECAL_SLOTS], decal_use[DECAL_SLOTS], decal_clock, map_hash;
static unsigned char decal_valid[DECAL_SLOTS];
/* Diagnostics stay out of the saved game and never affect either RNG. */
unsigned long n64_decal_cache_misses, n64_decal_cache_replacements, n64_decal_cache_waits;
static const unsigned char colors[6][3] = {
	{255, 65, 195}, {50, 220, 255}, {255, 220, 45},
	{100, 255, 65}, {255, 95, 35}, {160, 100, 255}
};

const unsigned char *n64_paint_mask(int pattern)
{
	return masks_loaded && pattern >= 0 && pattern < 3 ? masks + pattern * PAINT_CELL * PAINT_CELL / 2 : NULL;
}
const unsigned char *n64_paint_color(int color) { return colors[color >= 0 && color < 6 ? color : 0]; }
int n64_paint_next_color(void) { return next_mark % 6; }

static unsigned int hash_text(unsigned int hash, const char *text)
{
	for (int i = 0; text && text[i] && i < 512; i++) hash = (hash ^ (unsigned char)text[i]) * 16777619u;
	return hash;
}

void n64_paint_reset(const char *map_name)
{
	/* Map transitions can reuse a previously uploaded cache slot too. */
	for (int i = 0; i < DECAL_SLOTS; i++) if (decal_valid[i]) { rspq_wait(); break; }
	memset(marks, 0, sizeof(marks));
	next_mark = 0;
	memset(decal_valid, 0, sizeof(decal_valid));
	memset(decal_use, 0, sizeof(decal_use));
	map_hash = (hash_text(2166136261u, map_name) ^ '|') * 16777619u;
	decal_clock = 0;
	n64_decal_cache_misses = n64_decal_cache_replacements = n64_decal_cache_waits = 0;
	if (!masks_loaded)
	{
		FILE *file = fopen("rom:/paint/masks.i4", "rb");
		if (file)
		{
			masks_loaded = fread(masks, 1, sizeof(masks), file) == sizeof(masks);
			fclose(file);
			data_cache_hit_writeback_invalidate(masks, sizeof(masks));
		}
	}
}

void n64_paint_submit(const struct n64_paint_vertex *v, int count, int pattern)
{
	struct paint_mark *mark;
	if (count < 3 || count > N64_PAINT_VERTICES || !n64_paint_enabled()) return;
	mark = &marks[next_mark];
	memcpy(mark->v, v, count * sizeof(*v));
	mark->count = count;
	mark->pattern = pattern >= 0 && pattern < 3 ? pattern : 0;
	mark->color = next_mark % 6;
	mark->age = 0.f;
	mark->lifetime = PAINT_LIFE;
	mark->fade = 5.f;
	next_mark = (next_mark + 1) % PAINT_MARKS;
}

static unsigned int read_be32(const unsigned char *p)
{
	return (unsigned int)p[0] << 24 | (unsigned int)p[1] << 16 | (unsigned int)p[2] << 8 | p[3];
}

static int load_material(unsigned int key)
{
	unsigned char header[12], record[8];
	unsigned int count, offset = 0;
	long size;
	int slot = 0, found = 0;
	FILE *file;
	for (int i = 0; i < DECAL_SLOTS; i++)
		if (decal_valid[i] && decal_keys[i] == key) { decal_use[i] = ++decal_clock; return i; }
	n64_decal_cache_misses++;
	file = fopen("rom:/decals/materials.bank", "rb");
	if (!file) return -1;
	/* (without stdio's buffer, as n64_render.c's open_direct: each step of
	the search below read the 1 KB around its 8 bytes, the texture came in
	1 KB pieces copied on, and the buffer was made and freed each time) */
	setvbuf(file, NULL, _IONBF, 0);
	if (fseek(file, 0, SEEK_END) || (size = ftell(file)) < 12 || fseek(file, 0, SEEK_SET) ||
		fread(header, 1, 12, file) != 12 || memcmp(header, "NDEC", 4) || read_be32(header + 4) != DECAL_BANK_VERSION) { fclose(file); return -1; }
	count = read_be32(header + 8);
	if (!count || count > 4096 || 12u + count * 8u > (unsigned long)size) { fclose(file); return -1; }
	for (unsigned int low = 0, high = count; low < high; )
	{
		unsigned int mid = low + (high - low) / 2, candidate;
		if (fseek(file, 12u + mid * 8u, SEEK_SET) || fread(record, 1, 8, file) != 8) break;
		candidate = read_be32(record);
		if (candidate < key) low = mid + 1;
		else if (candidate > key) high = mid;
		else { offset = read_be32(record + 4); found = 1; break; }
	}
	if (!found || offset < 12u + count * 8u || offset > (unsigned long)size ||
		(unsigned long)size - offset < DECAL_BYTES) { fclose(file); return -1; }
	/* a free slot, else the one fewest marks still use (a glow's, gone in a
	second or two, before a bullet hole's), the least recently used of
	those */
	int live[DECAL_SLOTS] = {0};
	for (int i = 0; i < PAINT_MARKS; i++)
		if (marks[i].count && marks[i].pattern == 255 && marks[i].native_slot < DECAL_SLOTS)
			live[marks[i].native_slot]++;
	for (int i = 0; i < DECAL_SLOTS; i++)
	{
		if (!decal_valid[i]) { slot = i; break; }
		if (live[i] < live[slot] || (live[i] == live[slot] && decal_use[i] < decal_use[slot])) slot = i;
	}
	/* CPU overwrite must wait for earlier queued texture/palette DMA. A
	   queued RDP pipe sync alone cannot protect this source RAM. */
	if (decal_valid[slot])
	{
		n64_decal_cache_replacements++;
		n64_decal_cache_waits++;
		rspq_wait();
		for (int i = 0; i < PAINT_MARKS; i++)
			if (marks[i].pattern == 255 && marks[i].native_slot == slot) marks[i].count = 0;
	}
	decal_valid[slot] = 0;
	if (fseek(file, offset, SEEK_SET) || fread(decal_textures[slot], 1, DECAL_BYTES, file) != DECAL_BYTES) { fclose(file); return -1; }
	fclose(file);
	data_cache_hit_writeback_invalidate(decal_textures[slot], DECAL_BYTES);
	decal_keys[slot] = key;
	decal_valid[slot] = 1;
	decal_use[slot] = ++decal_clock;
	return slot;
}

void n64_decal_submit(const struct n64_paint_vertex *v, int count, const char *name,
	float lifetime, float fade, const unsigned char color[4])
{
	struct paint_mark *mark;
	int slot;
	if (!decals_on || count < 3 || count > N64_PAINT_VERTICES || !name || !color) return;
	slot = load_material(hash_text(map_hash, name));
	if (slot < 0) return;
	mark = &marks[next_mark];
	memcpy(mark->v, v, count * sizeof(*v));
	mark->count = count;
	mark->pattern = 255;
	mark->native_slot = slot;
	mark->age = 0.f;
	mark->lifetime = isfinite(lifetime) && lifetime > 0.f ? n64_min_f(lifetime, PAINT_LIFE) : PAINT_LIFE;
	mark->fade = isfinite(fade) && fade > 0.f ? n64_min_f(fade, mark->lifetime) : n64_min_f(5.f, mark->lifetime);
	memcpy(mark->rgba, color, 4);
	next_mark = (next_mark + 1) % PAINT_MARKS;
}

void n64_paint_update(float dt)
{
	extern void n64_paint_budget_reset(void);
	n64_paint_budget_reset();
	for (int i = 0; i < PAINT_MARKS; i++)
		if (marks[i].count && (marks[i].age += dt) >= marks[i].lifetime) marks[i].count = 0;
}

/* Clip before perspective division. RDPQ's screen-coordinate clamp cannot
   preserve edge slopes/UVs when a close, grazing mark exceeds its range. */
static int clip_plane(float in[][5], int count, const float plane[4], float out[][5])
{
	int n = 0;
	float distance[PAINT_CLIP_VERTICES];
	if (count < 3 || count > PAINT_CLIP_VERTICES) return 0;
	/* (each vertex's distance worked out once -- an edge's end is the next
	edge's start -- and any not finite none of the mark, wherever it is; the
	next vertex without a division, the remainder's) */
	for (int i = 0; i < count; i++)
	{
		distance[i] = in[i][0] * plane[0] + in[i][1] * plane[1] + in[i][2] * plane[2] + plane[3];
		if (!isfinite(distance[i])) return 0;
	}
	for (int i = 0; i < count; i++)
	{
		int next = i + 1 == count ? 0 : i + 1;
		float *a = in[i], *b = in[next];
		float da = distance[i], db = distance[next];
		if (da >= 0.f) { if (n == PAINT_CLIP_VERTICES) return 0; memcpy(out[n++], a, 5 * sizeof(float)); }
		if ((da < 0.f) != (db < 0.f))
		{
			float t = da / (da - db);
			if (n == PAINT_CLIP_VERTICES) return 0;
			for (int k = 0; k < 5; k++) out[n][k] = a[k] + (b[k] - a[k]) * t;
			n++;
		}
	}
	return n;
}

static int clip_view(float in[][5], int count, const struct n64_particle_view *view, float out[][5])
{
	float temporary[PAINT_CLIP_VERTICES][5];
	float px = view->pixels_per_unit * view->x_scale, py = view->pixels_per_unit;
	const float planes[5][4] = {
		{0.f, 0.f, 1.f, -view->z_near * 1.01f},
		{px, 0.f, view->center_x - view->x0, 0.f},
		{-px, 0.f, view->x1 - view->center_x, 0.f},
		{0.f, -py, view->center_y - view->y0, 0.f},
		{0.f, py, view->y1 - view->center_y, 0.f}
	};
	float (*from)[5] = in, (*to)[5] = out;
	if (count < 3 || count > N64_PAINT_VERTICES) return 0;
	/* (the planes take turns between out and temporary, the copies between
	them left out: the fifth plane's vertices end in out -- and fewer than
	three on the way are none to the caller) */
	for (int p = 0; p < 5 && count >= 3; p++)
	{
		count = clip_plane(from, count, planes[p], to);
		from = to;
		to = to == out ? temporary : out;
	}
	return count;
}

void n64_paint_draw(const struct n64_particle_view *view)
{
	float m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	float m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);
	/* (the depth buffer's step at distance d is about d^2 times this) */
	float pull_rate = PAINT_PULL_STEPS / (n64_max_f(view->z_near, 0.001f) * 32768.f);
	/* (the combiner and the texture in force, set again only when a mark
	wants others: each combiner set waited for the RDP's pipeline and worked
	the render mode out again, each upload loaded TMEM again -- consecutive
	marks are mostly one kind of decal) */
	int combiner = -1;
	const void *loaded = NULL;
	/* Diagnostic only: distinguish depth rejection from missing/offscreen
	   marks. Production packages forbid all debug flags. */
	if (paint_no_depth < 0) paint_no_depth = n64_debug_flag("debug_paint_no_depth");
	rdpq_sync_pipe();
	/* (one batch of mode changes: the RSP works the render mode out once,
	every view) */
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_zbuf(!paint_no_depth, false);
	rdpq_mode_zmode(ZMODE_STANDARD);
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_end();
	for (int i = 0; i < PAINT_MARKS; i++)
	{
		const struct paint_mark *mark = &marks[(next_mark + i) % PAINT_MARKS];
		float camera[N64_PAINT_VERTICES][5], clipped[PAINT_CLIP_VERTICES][5];
		float projected[PAINT_CLIP_VERTICES][6];
		float alpha = mark->fade > 0.f && mark->age > mark->lifetime - mark->fade ? (mark->lifetime - mark->age) / mark->fade : 1.f;
		float min_z = view->z_far;
		int count, behind;
		surface_t surface;
		if (!mark->count) continue;
		if (mark->pattern != 255 && (!n64_paint_enabled() || !masks_loaded)) continue;
		if (mark->pattern == 255 && (!decals_on || mark->native_slot >= DECAL_SLOTS ||
			!decal_valid[mark->native_slot])) continue;
		for (int v = 0; v < mark->count; v++)
		{
			float d[3];
			for (int k = 0; k < 3; k++) d[k] = mark->v[v].position[k] - view->eye[k];
			for (int k = 0; k < 3; k++) camera[v][k] = d[0] * (k == 0 ? view->right[0] : k == 1 ? view->up[0] : view->forward[0]) +
				d[1] * (k == 0 ? view->right[1] : k == 1 ? view->up[1] : view->forward[1]) +
				d[2] * (k == 0 ? view->right[2] : k == 1 ? view->up[2] : view->forward[2]);
		}
		/* A mark wholly behind the eye (no vertex in front of it) is left
		out before its pull and clipping: the pull scales a vertex by 0.5-1
		(or makes it NaN), so none comes in front of the near plane (its
		distance z_near x 1.01, more than nothing), whose clipping left none
		of the mark -- drawn nothing, as before */
		behind = view->z_near > 0.f;
		for (int v = 0; v < mark->count && behind; v++)
			behind = camera[v][2] <= 0.f;
		if (behind) continue;
		for (int v = 0; v < mark->count; v++)
		{
			float distance, pull;
			/* toward the eye along the vertex's ray (PAINT_PULL_MINIMUM) */
			distance = sqrtf(camera[v][0] * camera[v][0] + camera[v][1] * camera[v][1] + camera[v][2] * camera[v][2]);
			if (!paint_no_depth && distance > 0.f)
			{
				pull = PAINT_PULL_MINIMUM + distance * distance * pull_rate;
				pull = n64_max_f(0.5f, (distance - pull) / distance);
				for (int k = 0; k < 3; k++) camera[v][k] *= pull;
			}
			camera[v][3] = mark->v[v].uv[0]; camera[v][4] = mark->v[v].uv[1];
			if (camera[v][2] < min_z) min_z = camera[v][2];
		}
		if (view->fog_end > 0.f && min_z > view->fog_end) continue;
		if (view->fog_end > view->fog_start && min_z > view->fog_start)
			alpha *= 1.f - (min_z - view->fog_start) / (view->fog_end - view->fog_start);
		count = clip_view(camera, mark->count, view, clipped);
		if (count < 3) continue;
		for (int v = 0; v < count; v++)
		{
			float inv = 1.f / clipped[v][2];
			projected[v][0] = view->center_x + clipped[v][0] * view->pixels_per_unit * view->x_scale * inv;
			projected[v][1] = view->center_y - clipped[v][1] * view->pixels_per_unit * inv;
			if (view->chunky) projected[v][0] = view->x0 + (projected[v][0] - view->x0) * view->chunky_scale;
			projected[v][2] = n64_max_f(0.f, n64_min_f(1.f, (-m22 + m32 * inv) * 0.5f + 0.5f));
			projected[v][3] = clipped[v][3] * (PAINT_CELL - 1);
			projected[v][4] = clipped[v][4] * (PAINT_CELL - 1);
			projected[v][5] = inv;
		}
		if (mark->pattern == 255)
		{
			/* the ink's brightness x its tint x the decal's colour, its coverage
			x the decal's intensity and fade; a darkening decal's (Halo's
			multiply: the colour fades the texture toward the neutral) has
			its colour set how strongly */
			unsigned char *texture = decal_textures[mark->native_slot];
			const unsigned char *tint = texture + PAINT_CELL * PAINT_CELL;
			int a = (int)(alpha * mark->rgba[3]);
			int rgb[3];

			for (int k = 0; k < 3; k++)
				rgb[k] = tint[3] ? tint[k] : tint[k] * mark->rgba[k] / 255;
			if (tint[3])
				a = a * (mark->rgba[0] + mark->rgba[1] + mark->rgba[2]) / 765;
			if (combiner != 1)
				rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, 0, PRIM, 0), (TEX0, 0, PRIM, 0)));
			combiner = 1;
			rdpq_set_prim_color(RGBA32(rgb[0], rgb[1], rgb[2], a));
			surface = surface_make_linear(texture, FMT_IA8, PAINT_CELL, PAINT_CELL);
		}
		else
		{
			if (combiner != 0)
				rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
			combiner = 0;
			rdpq_set_prim_color(RGBA32(colors[mark->color][0], colors[mark->color][1], colors[mark->color][2], (int)(alpha * 230.f)));
			surface = surface_make_linear(masks + mark->pattern * PAINT_CELL * PAINT_CELL / 2, FMT_I4, PAINT_CELL, PAINT_CELL);
		}
		/* (a decal's texture and a mask are each their own memory, one format
		each: the same buffer is the same upload) */
		if (surface.buffer != loaded)
			rdpq_tex_upload(TILE0, &surface, NULL);
		loaded = surface.buffer;
		for (int v = 1; v < count - 1; v++) rdpq_triangle(&TRIFMT_ZBUF_TEX, projected[0], projected[v], projected[v + 1]);
	}
}
