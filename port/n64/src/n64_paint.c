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
#define DECAL_SLOTS 4
#define DECAL_BYTES (PAINT_CELL * PAINT_CELL / 2 + 32)
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
	if (fseek(file, 0, SEEK_END) || (size = ftell(file)) < 12 || fseek(file, 0, SEEK_SET) ||
		fread(header, 1, 12, file) != 12 || memcmp(header, "NDEC", 4) || read_be32(header + 4) != 1) { fclose(file); return -1; }
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
	for (int i = 0; i < DECAL_SLOTS; i++)
	{
		if (!decal_valid[i]) { slot = i; break; }
		if (decal_use[i] < decal_use[slot]) slot = i;
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
	if (count < 3 || count > N64_PAINT_VERTICES || !name || !color) return;
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
	if (count < 3 || count > PAINT_CLIP_VERTICES) return 0;
	for (int i = 0; i < count; i++)
	{
		float *a = in[i], *b = in[(i + 1) % count];
		float da = a[0] * plane[0] + a[1] * plane[1] + a[2] * plane[2] + plane[3];
		float db = b[0] * plane[0] + b[1] * plane[1] + b[2] * plane[2] + plane[3];
		if (!isfinite(da) || !isfinite(db)) return 0;
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
	if (count < 3 || count > N64_PAINT_VERTICES) return 0;
	memcpy(out, in, count * 5 * sizeof(float));
	for (int p = 0; p < 5 && count >= 3; p++)
	{
		count = clip_plane(out, count, planes[p], temporary);
		memcpy(out, temporary, count * 5 * sizeof(float));
	}
	return count;
}

void n64_paint_draw(const struct n64_particle_view *view)
{
	float m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	float m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);
	/* Diagnostic only: distinguish depth rejection from missing/offscreen
	   marks. Production packages forbid all debug flags. */
	if (paint_no_depth < 0) paint_no_depth = n64_debug_flag("debug_paint_no_depth");
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	rdpq_mode_zbuf(!paint_no_depth, false);
	rdpq_mode_zmode(paint_no_depth ? ZMODE_STANDARD : ZMODE_DECAL);
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	for (int i = 0; i < PAINT_MARKS; i++)
	{
		const struct paint_mark *mark = &marks[(next_mark + i) % PAINT_MARKS];
		float camera[N64_PAINT_VERTICES][5], clipped[PAINT_CLIP_VERTICES][5];
		float projected[PAINT_CLIP_VERTICES][6];
		float alpha = mark->fade > 0.f && mark->age > mark->lifetime - mark->fade ? (mark->lifetime - mark->age) / mark->fade : 1.f;
		float min_z = view->z_far;
		int count;
		surface_t surface;
		if (!mark->count) continue;
		if (mark->pattern != 255 && (!n64_paint_enabled() || !masks_loaded)) continue;
		if (mark->pattern == 255 && (mark->native_slot >= DECAL_SLOTS || !decal_valid[mark->native_slot])) continue;
		for (int v = 0; v < mark->count; v++)
		{
			float d[3];
			for (int k = 0; k < 3; k++) d[k] = mark->v[v].position[k] - view->eye[k];
			for (int k = 0; k < 3; k++) camera[v][k] = d[0] * (k == 0 ? view->right[0] : k == 1 ? view->up[0] : view->forward[0]) +
				d[1] * (k == 0 ? view->right[1] : k == 1 ? view->up[1] : view->forward[1]) +
				d[2] * (k == 0 ? view->right[2] : k == 1 ? view->up[2] : view->forward[2]);
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
			unsigned char *texture = decal_textures[mark->native_slot];
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, 0, PRIM, 0), (TEX0, 0, PRIM, 0)));
			rdpq_set_prim_color(RGBA32(mark->rgba[0], mark->rgba[1], mark->rgba[2], (int)(alpha * mark->rgba[3])));
			rdpq_tex_upload_tlut((uint16_t *)(texture + PAINT_CELL * PAINT_CELL / 2), 0, 16);
			surface = surface_make_linear(texture, FMT_CI4, PAINT_CELL, PAINT_CELL);
		}
		else
		{
			rdpq_mode_tlut(TLUT_NONE);
			rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
			rdpq_set_prim_color(RGBA32(colors[mark->color][0], colors[mark->color][1], colors[mark->color][2], (int)(alpha * 230.f)));
			surface = surface_make_linear(masks + mark->pattern * PAINT_CELL * PAINT_CELL / 2, FMT_I4, PAINT_CELL, PAINT_CELL);
		}
		rdpq_tex_upload(TILE0, &surface, NULL);
		for (int v = 1; v < count - 1; v++) rdpq_triangle(&TRIFMT_ZBUF_TEX, projected[0], projected[v], projected[v + 1]);
	}
	rdpq_mode_tlut(TLUT_NONE);
}
