/*
N64_HUD64.C

HUD STYLE 64 (Settings): a HUD redrawn for the N64 -- Halo CE's HUD as N64
pixel art. It draws from tables (n64_hud64_data.h, written by
tools/n64/hud64/hud64_gen.py from the design, tools/n64/hud64/hud64_design.py)
and from the textures in rom:/hud64/c*.bin (assets/hud64, written with the
tables), a file a context (set, 640x240, 1-2 or 3-4 players), so that a game
loads only what it draws: every position, size, colour and glyph is in the
tables, so that another design can replace this one by writing its own
tables. This file only holds the steps of drawing.

The game's HUD code still runs (port/n64/game/n64_hud64_game.c): it hands
over the values (ammunition, grenades, shield, health, score, messages, the
pickup prompt) and draws, as before, what the design does not cover: the
crosshairs and the motion tracker (Halo's own in either style), nav points,
damage arrows, vehicles' panels, scopes.

Modes, as the design draws them:
- 320x240: the 240-line set, 1:1;
- 640x240 (HIGH DETAIL, and HUD RESOLUTION HIGH DETAIL over a 320x240 3D):
  the 240-line set's graphics doubled across, its text from 640-wide faces
  with a 1-pixel outline, drawn after the graphics;
- 640x480 (ULTRA DETAIL): the 480 set;
- WIDESCREEN: the same with the sets drawn 75% wide.
High detail's narrower fallbacks (520x240, 480x240) and ultra's 512x480,
which the design does not show, take the 75%-wide sets (their pixels are a
third wider than 640x240's).

Texts outline by offset draws: a 640-wide glyph is drawn at its neighbour
offsets in the outline colour, then itself. Meters (shield, health, rounds,
score bars) are order maps (I8): the RDP's alpha compare keeps the texels
whose order is within the value, so a meter needs no texture of its own.
*/

#include <libdragon.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "n64.h"
#include "n64_hud64_data.h"

/* (the program's: this frame shows Halo's own small range label --
n64_hud64_stub.c) */
extern int n64_hud64_label_halo;

#define HUD64_MAXIMUM_TOKENS 24
#define HUD64_MAXIMUM_LINES 6
#define HUD64_MAXIMUM_DEFERRED 96

/* Python's floor division and round (half to even), which the design's
layout uses */
static inline int floordiv(int a, int b)
{
	int q = a / b;
	return (a % b != 0 && ((a < 0) != (b < 0))) ? q - 1 : q;
}

static int round_even(float v)
{
	int r = (int)floorf(v + 0.5f);

	if ((float)r - v == 0.5f && (r & 1))
		r--;
	return r;
}

/* ---------- the texture groups */

#define HUD64_CONTEXTS 16

/* (the module compiled for size (-Os, tools/n64_build.py) -- its image is heap a map gives it -- but its
per-draw paths, run for every texture, glyph and meter part a frame, and the range label's placement, run every
frame at 1-2 players, at -O2: the -Os build cost 0.15 fps at 1p HUD 640x240) */
#define HOT __attribute__((optimize("O2")))
/* (the outline cells, made when a context's textures load: below) */
static void build_all_outlines(void);
static void release_outlines(void);
static rspq_block_t *outline_upload(const surface_t *surface);
static unsigned long outline_bytes(void);

static struct
{
	void *data[HUD64_CONTEXTS];
	unsigned long bytes[HUD64_CONTEXTS];
	int failed[HUD64_CONTEXTS];
	surface_t textures[HUD64_TEXTURE_COUNT];
	/* each texture's upload to TMEM recorded once, when it loads
	(measured at 640x480 with 2 players: rdpq_tex_upload's commands built
	again for every draw cost 1.6 ms of a recorded view's 3.3) */
	rspq_block_t *uploads[HUD64_TEXTURE_COUNT];
} hud64;

/* a texture's TMEM upload as a block (made outside any block recording:
when its context loads) */
static rspq_block_t *upload_block(const surface_t *surface)
{
	rspq_block_begin();
	rdpq_tex_upload(TILE0, surface, NULL);
	return rspq_block_end();
}

/* (the setting, n64_hud_set_style and n64_hud_style: the resident part's,
port/n64/game/n64_hud64_stub.c) */

static const tex_format_t formats[4] = {FMT_I4, FMT_IA4, FMT_I8, FMT_IA8};

/* a context's file read whole (its textures point into it); 1 if loaded.
Context: set * 4 + 640x240 * 2 + 3-4 players */
static int load_group(int group)
{
	FILE *file;
	unsigned long size;
	uint8_t *data;
	char path[40];

	if (hud64.data[group])
		return 1;
	if (hud64.failed[group])
		return 0;
	snprintf(path, sizeof(path), HUD64_CONTEXT_FILE, group / 4, (group / 2) & 1, group & 1);
	file = fopen(path, "rb");
	if (!file)
	{
		debugf("hud64: no %s\n", path);
		hud64.failed[group] = 1;
		return 0;
	}
	fseek(file, 0, SEEK_END);
	size = (unsigned long)ftell(file);
	fseek(file, 0, SEEK_SET);
	/* (room for it that leaves the style's margin free, the model cache's
	oldest dropped first if short; without it the game's own HUD on this
	map: n64_hud64_stub.c) */
	if (!n64_render_room_leaving(size, n64_hud64_heap_margin()))
	{
		fclose(file);
		hud64.failed[group] = 1;
		debugf("hud64: %s (%lu KB) would leave less than %lu KB free (%lu KB free): the game's own HUD\n", path,
			size / 1024, n64_hud64_heap_margin() / 1024, n64_heap_free() / 1024);
		return 0;
	}
	data = memalign(16, size);
	if (!data)
	{
		fclose(file);
		debugf("hud64: no memory for %s (%lu bytes)\n", path, size);
		return 0;
	}
	/* (a file from other tables -- not written in the same run of
	hud64_gen.py as n64_hud64_data.h -- would draw the wrong texels) */
	if (fread(data, 1, size, file) != size || memcmp(data, "H64T", 4) || (data[4] << 8 | data[5]) != HUD64_TABLES_ID)
	{
		fclose(file);
		free(data);
		hud64.failed[group] = 1;
		debugf("hud64: %s unreadable or not from these tables\n", path);
		return 0;
	}
	fclose(file);
	data_cache_hit_writeback_invalidate(data, size);
	{
		int count = data[6] << 8 | data[7];

		for (int i = 0; i < count; i++)
		{
			const uint8_t *e = data + 8 + 16 * i;
			int id = e[0] << 8 | e[1], format = e[2];
			int width = e[4] << 8 | e[5], height = e[6] << 8 | e[7], stride = e[8] << 8 | e[9];
			unsigned long offset = (unsigned long)e[12] << 24 | e[13] << 16 | e[14] << 8 | e[15];

			/* (a texture another loaded context holds already keeps that
			copy and its upload block: the same pixels) */
			if (id < HUD64_TEXTURE_COUNT && format < 4 && !hud64.textures[id].buffer)
			{
				hud64.textures[id] = surface_make(data + offset, formats[format], width, height, stride);
				hud64.uploads[id] = upload_block(&hud64.textures[id]);
			}
		}
	}
	hud64.data[group] = data;
	hud64.bytes[group] = size;
	build_all_outlines();
	{
		heap_stats_t heap;

		sys_get_heap_stats(&heap);
		debugf("hud64: %s loaded, %lu bytes; heap %d KB free\n", path, size, heap.free / 1024);
	}
	return 1;
}

/* the groups given back (end of a map: n64_hud_release); the RDP done with
them first (the caller waited) */
void n64_hud64_release(void)
{
	release_outlines();
	for (int i = 0; i < HUD64_TEXTURE_COUNT; i++)
	{
		if (hud64.uploads[i])
			rspq_block_free(hud64.uploads[i]);
		hud64.uploads[i] = NULL;
	}
	for (int g = 0; g < HUD64_CONTEXTS; g++)
	{
		free(hud64.data[g]);
		hud64.data[g] = NULL;
		hud64.bytes[g] = 0;
	}
	memset(hud64.textures, 0, sizeof(hud64.textures));
}

/* (n64_hud64_game.c: Halo's tracker foreground cut short) the next draws
kept in this rectangle */
void n64_hud64_scissor(int x0, int y0, int x1, int y1)
{
	rdpq_sync_pipe();
	rdpq_set_scissor(x0, y0, x1, y1);
}

unsigned long n64_hud64_resident_bytes(void)
{
	unsigned long bytes = 0;

	for (int g = 0; g < HUD64_CONTEXTS; g++)
		bytes += hud64.bytes[g];
	return bytes + outline_bytes();
}

/* ---------- the mode: the set, the canvas the design lays out on, the
scale across */

struct mode
{
	const struct hud64_set *set;
	int set_index;
	int canvas_w, canvas_h; /* the design's layout canvas */
	int xs;                 /* screen pixels across a canvas pixel */
};

static void current_mode(struct mode *m)
{
	int w, h, wide = n64_render_hud_squeeze() != 1.f;

	/* (the display the HUD is drawn in: with HUD RESOLUTION HIGH DETAIL,
	640x240, not the 3D's, which is the screen before the HUD's phase) */
	n64_render_hud_size(&w, &h);

	if (h >= 480)
	{
		m->set_index = wide || w != 640 ? HUD64_SET_480WS : HUD64_SET_480;
		m->canvas_w = w;
		m->canvas_h = h;
		m->xs = 1;
	}
	else if (w >= 640)
	{
		m->set_index = wide ? HUD64_SET_240WS : HUD64_SET_240;
		m->canvas_w = w / 2;
		m->canvas_h = h;
		m->xs = 2;
	}
	else if (w > 320)
	{
		m->set_index = HUD64_SET_240WS;
		m->canvas_w = w / 2;
		m->canvas_h = h;
		m->xs = 2;
	}
	else
	{
		m->set_index = wide ? HUD64_SET_240WS : HUD64_SET_240;
		m->canvas_w = w;
		m->canvas_h = h;
		m->xs = 1;
	}
	m->set = &hud64_sets[m->set_index];
}

/* before a view's HUD (not while one is recorded): the textures of its
context (the mode's set, 640x240's faces, the player count's arrangement)
in memory. 1 if the 64 style can be drawn now; 0 and the game draws its own */
static int label_halo(int views);

int n64_hud64_prepare(int views)
{
	const struct hud64_arrangement *ar = &hud64_arrangements[views < 1 ? 1 : views > 4 ? 4 : views];
	struct mode m;

	if (!n64_hud_style())
		return 0;
	current_mode(&m);
	/* (before Halo's HUD, which cuts its own small range label
	out of the tracker unless this frame shows it -- label_halo) */
	n64_hud64_label_halo = ar->sensor_label ? label_halo(views) : 0;
	return load_group(m.set_index * 4 + (m.xs > 1 && m.set->s == 1) * 2 + ar->split);
}

/* ---------- drawing: one texture or cell, as the design blits it */

enum { MODE_NONE, MODE_IA, MODE_IA_CUT, MODE_I, MODE_METER };

static struct
{
	struct mode m;
	int rdp_mode;
	const void *tmem; /* the texture in TMEM (NULL: unknown) */
	uint32_t prim, env;
	int prim_valid, env_valid;
	int threshold, fog;
	/* the view's scissor (n64_hud_begin's), screen pixels */
	int clip[4];
	/* 640x240: glyphs drawn after the view's graphics */
	struct deferred { uint8_t face; char ch; int16_t x, y; uint32_t colour; } deferred[HUD64_MAXIMUM_DEFERRED];
	int deferred_count;
	/* the tracker box's left and right before rounding (tracker_box), the
	set's pixels: the range label's test against the tracker's circle */
	float trk_l, trk_r;
} draw;

HOT static void set_rdp_mode(int mode)
{
	if (draw.rdp_mode == mode)
		return;
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	rdpq_mode_filter(FILTER_POINT);
	switch (mode)
	{
	case MODE_IA:
		/* colour env + (prim - env) * texel intensity, alpha texel's * prim's */
		rdpq_mode_combiner(RDPQ_COMBINER1((PRIM, ENV, TEX0, ENV), (TEX0, 0, PRIM, 0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		break;
	case MODE_IA_CUT:
		/* a texture of 1-bit alpha at full alpha (IA4): its texels written as
		they are, the rest left (the RDP's blender keeps 1/32 of the
		framebuffer even at full alpha) */
		rdpq_mode_combiner(RDPQ_COMBINER1((PRIM, ENV, TEX0, ENV), (TEX0, 0, PRIM, 0)));
		rdpq_mode_alphacompare(1);
		break;
	case MODE_I:
		/* the primitive's colour, the texel as alpha */
		rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		break;
	case MODE_METER:
		/* the primitive's colour where the order map passes the threshold,
		at the fog colour's alpha */
		rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (0, 0, 0, TEX0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
		draw.threshold = -1;
		draw.fog = -1;
		break;
	}
	draw.rdp_mode = mode;
}

HOT static color_t rgba(uint32_t rgb, int alpha)
{
	return RGBA32((rgb >> 16) & 255, (rgb >> 8) & 255, rgb & 255, alpha);
}

HOT static void set_colours(uint32_t prim, int alpha, uint32_t env)
{
	uint32_t p = (prim & 0xFFFFFF) | (uint32_t)alpha << 24;

	if (!draw.prim_valid || p != draw.prim)
	{
		rdpq_set_prim_color(rgba(prim, alpha));
		draw.prim = p;
		draw.prim_valid = 1;
	}
	if (!draw.env_valid || env != draw.env)
	{
		rdpq_set_env_color(rgba(env, 255));
		draw.env = env;
		draw.env_valid = 1;
	}
}

HOT static const surface_t *texture(int id)
{
	if (id >= HUD64_TEXTURE_COUNT || !hud64.textures[id].buffer)
		return NULL;
	return &hud64.textures[id];
}

HOT static void upload(const surface_t *surface)
{
	rspq_block_t *block = NULL;

	if (draw.tmem == surface->buffer)
		return;
	/* (the upload recorded when the texture loaded: a context's texture or
	an outline sheet) */
	if (surface >= hud64.textures && surface < hud64.textures + HUD64_TEXTURE_COUNT)
		block = hud64.uploads[surface - hud64.textures];
	else
		block = outline_upload(surface);
	if (block)
		rspq_block_run(block);
	else
		rdpq_tex_upload(TILE0, surface, NULL);
	draw.tmem = surface->buffer;
}

/* a texture's rectangle (s0, t0, w, h in texels) at canvas x, y: scale
texels per pixel down, the mode's scale across; mirrored as libdragon's
blits mirror */
HOT static void rectangle(int x, int y, int s0, int t0, int w, int h, int scale, int flip_x, int flip_y)
{
	int sx = scale * draw.m.xs, sy = scale;
	float x0 = (float)(x * draw.m.xs), y0 = (float)y, x1 = x0 + w * sx, y1 = y0 + h * sy;

	if (sx == 1 && sy == 1)
	{
		if (flip_x)
			x0 = (float)(x + w - 1), x1 = (float)(x - 1);
		if (flip_y)
			y0 = (float)(y + h - 1), y1 = (float)(y - 1);
		rdpq_texture_rectangle(TILE0, x0, y0, x1, y1, s0, t0);
		return;
	}
	if (flip_x)
	{
		float t = x0;
		x0 = x1;
		x1 = t;
	}
	if (flip_y)
	{
		float t = y0;
		y0 = y1;
		y1 = t;
	}
	rdpq_texture_rectangle_scaled(TILE0, x0, y0, x1, y1, s0, t0, s0 + w, t0 + h);
}

HOT static int ia_mode(const surface_t *t, int alpha)
{
	return surface_get_format(t) == FMT_IA4 && alpha == 255 ? MODE_IA_CUT : MODE_IA;
}

/* the design's tex(): I4/I8 textures colour prim, alpha the texel; IA ones
env + (prim - env) * intensity (env defaults to prim) */
HOT static void draw_ref(const struct hud64_ref *ref, int x, int y, uint32_t prim, int has_env, uint32_t env, int alpha,
	int flip_x, int flip_y)
{
	const surface_t *t = texture(ref->tex);

	if (!t)
		return;
	if (surface_get_format(t) == FMT_I4 || surface_get_format(t) == FMT_I8)
		set_rdp_mode(MODE_I), set_colours(prim, alpha, draw.env);
	else
		set_rdp_mode(ia_mode(t, alpha)), set_colours(prim, alpha, has_env ? env : prim);
	upload(t);
	/* (a part of a stacked texture, hud64_gen.py _merge: its rows t0..t0 + h) */
	rectangle(x + ref->dx * ref->scale, y + ref->dy * ref->scale, 0, ref->t0, t->width, ref->h ? ref->h : t->height,
		ref->scale, flip_x, flip_y);
}

HOT static void draw_cell(const struct hud64_cell *c, int x, int y, uint32_t prim, uint32_t env, int alpha)
{
	const surface_t *t = texture(c->tex);

	if (!t)
		return;
	set_rdp_mode(ia_mode(t, alpha));
	set_colours(prim, alpha, env);
	upload(t);
	rectangle(x, y, c->x, c->y, c->w, c->h, c->scale, 0, 0);
}

static void ref_size(const struct hud64_ref *ref, int *w, int *h)
{
	const surface_t *t = texture(ref->tex);

	*w = t ? t->width * ref->scale : 0;
	*h = t ? (ref->h ? ref->h : t->height) * ref->scale : 0;
}

/* a colour blended toward another by f of 255 -- HUD 64's own colour
toward a Halo flash's colour by Halo's factor */
HOT static uint32_t blend(uint32_t c, uint32_t target, int f)
{
	uint32_t out = 0;

	for (int sh = 0; sh <= 16; sh += 8)
	{
		int a = (int)((c >> sh) & 255), b = (int)((target >> sh) & 255);

		out |= (uint32_t)(a + ((b - a) * f + (b > a ? 127 : -127)) / 255) << sh;
	}
	return out;
}

/* the vitals' 3-tone ramp: a meter colour's tint, 30% toward white,
and its shade, 65% of it -- per channel */
HOT static uint32_t ramp_colour(uint32_t c, int tint)
{
	uint32_t out = 0;

	for (int sh = 0; sh <= 16; sh += 8)
	{
		int v = (int)((c >> sh) & 255);

		v = tint ? v + ((255 - v) * 30 + 50) / 100 : (v * 65 + 50) / 100;
		out |= (uint32_t)v << sh;
	}
	return out;
}

/* (a meter drawn in one colour throughout -- the damage ghost: Halo's
flash colour added to every texel of the held range) */
static int meter_flat;

/* a meter: its order map's texels within value, in colour lit, at alpha; its
tint and shade parts (each texel its own order) in lit's tint and shade;
its top edge (the shield's) in highlight */
HOT static void draw_meter(int key, int x, int y, float value, uint32_t lit, int alpha, uint32_t highlight)
{
	const struct hud64_meter *meter = &draw.m.set->meter[key];
	int v = (int)floorf(value * 255.f + 1e-3f);

	if (v <= 0)
		return;
	if (v > 255)
		v = 255;
	set_rdp_mode(MODE_METER);
	if (256 - v != draw.threshold)
	{
		rdpq_mode_alphacompare(256 - v);
		draw.threshold = 256 - v;
	}
	if (alpha != draw.fog)
	{
		rdpq_set_fog_color(RGBA32(0, 0, 0, alpha));
		draw.fog = alpha;
	}
	for (int part = 0; part < 4; part++)
	{
		/* (in turn: the body, the tint, the shade -- which may draw over the
		body where its texels' order is later, as the design's ramp does --
		the top edge) */
		const struct hud64_ref *ref = part == 0 ? &meter->body : part == 1 ? &meter->tint :
			part == 2 ? &meter->shade : &meter->highlight;
		const surface_t *t = texture(ref->tex);

		if (!t)
			continue;
		set_colours(meter_flat || part == 0 ? lit : part == 1 ? ramp_colour(lit, 1) : part == 2 ? ramp_colour(lit, 0) :
			highlight, 255, draw.env);
		upload(t);
		/* (a shield meter at 640 across: a texel a screen column, hud64_gen.py WIDE_METERS) */
		/* (a part of a stacked texture, hud64_gen.py _merge: its rows t0..t0 + h) */
		if (key == HUD64_METER_SHIELD_W || key == HUD64_METER_SHIELD_S_W)
		{
			int x0 = x * draw.m.xs + ref->dx, y0 = y + ref->dy;

			rdpq_texture_rectangle(TILE0, x0, y0, x0 + t->width, y0 + (ref->h ? ref->h : t->height), 0, ref->t0);
		}
		else
			rectangle(x + ref->dx * ref->scale, y + ref->dy * ref->scale, 0, ref->t0, t->width,
				ref->h ? ref->h : t->height, ref->scale, 0, 0);
	}
}

/* the ticks a round grid of n draws to value, as draw_meter keeps them (tick
j of n is stored at 256 - j * 255 / n) */
static int ticks_drawn(int n, float value)
{
	int v = (int)floorf(value * 255.f + 1e-3f), k = 0;

	for (int j = 1; j <= n; j++)
		k += j * 255 / n <= v;
	return k;
}

/* a texture drawn shorter by dx texels and h texels tall (its top rows):
its columns to the seam, then its right end moved left by dx (a panel or a
plate whose middle only its length changes, hud64_gen.py) */
HOT static void draw_ref_short(const struct hud64_ref *ref, int x, int y, uint32_t prim, int alpha, int dx, int cut, int h)
{
	const surface_t *t = texture(ref->tex);

	if (!t)
		return;
	if (dx == 0 && h == t->height)
	{
		draw_ref(ref, x, y, prim, 0, 0, alpha, 0, 0);
		return;
	}
	if (surface_get_format(t) == FMT_I4 || surface_get_format(t) == FMT_I8)
		set_rdp_mode(MODE_I), set_colours(prim, alpha, draw.env);
	else
		set_rdp_mode(ia_mode(t, alpha)), set_colours(prim, alpha, prim);
	upload(t);
	if (dx == 0)
	{
		rectangle(x, y, 0, 0, t->width, h, ref->scale, 0, 0);
		return;
	}
	rectangle(x, y, 0, 0, cut, h, ref->scale, 0, 0);
	rectangle(x + cut * ref->scale, y, cut + dx, 0, t->width - cut - dx, h, ref->scale, 0, 0);
}

/* the plate under a round grid, sized to the slots the magazine draws (a
2-round rocket launcher does not get the assault rifle's plate). g: the plate's geometry from the tables (hud64_gen.py _plate): the
ticks, columns, rows, the ticks' pitch across and down, the seam, the shift
that leaves the design's margin past the grid's last column, the margin;
texels.
The full plate's top rows, a row of ticks fewer each a pitch down fewer;
under one row's worth, its left part to the seam and its right end moved
left by the unused columns */
static void draw_plate(int key, const int16_t *g, int x, int y, int slots)
{
	const struct hud64_ref *ref = &draw.m.set->ref[key];
	const surface_t *t = texture(ref->tex);
	int cols, rows;

	if (!t || slots <= 0)
		return;
	cols = slots < g[1] ? slots : g[1];
	rows = (slots + g[1] - 1) / g[1];
	draw_ref_short(ref, x, y, HUD64_PLATE, 255, g[6] + (g[1] - cols) * g[3], g[5], t->height - (g[2] - rows) * g[4]);
}

/* ---------- text */

HOT static const struct hud64_face *face(int key, int wide)
{
	int i = wide ? draw.m.set->wface[key] : draw.m.set->face[key];

	return i < HUD64_FACE_COUNT ? &hud64_faces[i] : NULL;
}

HOT static const struct hud64_glyph *glyph_of(const struct hud64_face *f, char ch)
{
	unsigned c = (unsigned char)ch;

	if (c < 32 || c >= 128 || !f->map[c - 32])
		return NULL;
	return &f->glyphs[f->map[c - 32] - 1];
}

/* ---------- outlines: the 640-wide faces' navy outline is the glyph drawn
at its neighbour offsets (the face's ring of offsets, the fewest that give
the design's outline). Those offset draws are made once, when the face's
textures load, into outline cells (the ring alone, without the glyph's own
pixels), so that a glyph's outline is one draw on the RDP, not up to 14 --
the same pixels, at a ninth of the fill (480i's text made two players
0.37 fps slower than ORIGINAL). The stored atlases stay the design's: the
glyph alone. */

#define HUD64_OUTLINE_SHEETS 16
#define HUD64_OUTLINE_GLYPHS 48
#define HUD64_OUTLINE_WIDTH 128

static struct
{
	uint8_t built[HUD64_FACE_COUNT];
	/* each built face's cells (sheet, x, y), on the heap with the sheets */
	struct outline_cell { uint8_t sheet, x, y; } *cell[HUD64_FACE_COUNT];
	surface_t sheets[HUD64_OUTLINE_SHEETS];
	rspq_block_t *uploads[HUD64_OUTLINE_SHEETS];
	int sheet_count;
	unsigned long bytes;
} outlines;

static int glyph_alpha(const surface_t *t, int x, int y)
{
	uint8_t b = ((const uint8_t *)t->buffer)[y * t->stride + x / 2];
	tex_format_t format = surface_get_format(t);

	if (format == FMT_IA4)
		return ((x & 1) ? b : b >> 4) & 1;
	if (format == FMT_IA8)
		return (((const uint8_t *)t->buffer)[y * t->stride + x] & 15) >= 8;
	return 0;
}

/* a face's outline cells, its glyphs packed in rows of sheets (IA4, 128
wide, at most 4 KB: one TMEM load); 1 if built */
static int build_outlines(int fi)
{
	const struct hud64_face *f = &hud64_faces[fi];
	int ry = f->ry, first = outlines.sheet_count;
	int n = f->glyph_count < HUD64_OUTLINE_GLYPHS ? f->glyph_count : HUD64_OUTLINE_GLYPHS;
	int heights[HUD64_OUTLINE_SHEETS] = {0}, sheet = -1, x = 0, y = 0, shelf = 0;

	if (outlines.built[fi])
		return 1;
	for (int g = 0; g < n; g++)
	{
		if (!texture(f->glyphs[g].tex))
			return 0;
	}
	if (!outlines.cell[fi] && !(outlines.cell[fi] = malloc(n * sizeof(struct outline_cell))))
		return 0;
	/* (the packing: rows of cells, a new sheet when 64 rows are full) */
	for (int pass = 0; pass < 2; pass++)
	{
		sheet = -1;
		x = y = shelf = 0;
		for (int g = 0; g < n; g++)
		{
			const struct hud64_glyph *gl = &f->glyphs[g];
			int w = gl->w + 2, h = gl->h + 2 * ry;

			if (sheet < 0 || x + w > HUD64_OUTLINE_WIDTH)
				x = 0, y += shelf, shelf = 0;
			if (sheet < 0 || y + h > 4096 / (HUD64_OUTLINE_WIDTH / 2))
			{
				if (first + sheet + 1 >= HUD64_OUTLINE_SHEETS)
					return 0;
				sheet++;
				x = y = shelf = 0;
			}
			if (pass == 0)
			{
				if (y + h > heights[sheet])
					heights[sheet] = y + h;
			}
			else
			{
				const surface_t *t = texture(gl->tex);
				const surface_t *out = &outlines.sheets[first + sheet];

				for (int py = 0; py < gl->h; py++)
				{
					for (int px = 0; px < gl->w; px++)
					{
						if (!glyph_alpha(t, gl->x + px, gl->y + py))
							continue;
						for (int k = 0; k < f->ring_count; k++)
						{
							int ox = x + 1 + px + f->ring[k][0], oy = y + ry + py + f->ring[k][1];
							uint8_t *b = (uint8_t *)out->buffer + oy * out->stride + ox / 2;

							*b |= (ox & 1) ? 0x0F : 0xF0;
						}
					}
				}
				/* (the ring alone: the glyph's own pixels left out) */
				for (int py = 0; py < gl->h; py++)
				{
					for (int px = 0; px < gl->w; px++)
					{
						int ox = x + 1 + px, oy = y + ry + py;
						uint8_t *b = (uint8_t *)out->buffer + oy * out->stride + ox / 2;

						if (glyph_alpha(t, gl->x + px, gl->y + py))
							*b &= (ox & 1) ? 0xF0 : 0x0F;
					}
				}
				outlines.cell[fi][g].sheet = (uint8_t)(first + sheet);
				outlines.cell[fi][g].x = (uint8_t)x;
				outlines.cell[fi][g].y = (uint8_t)y;
			}
			x += w;
			shelf = h > shelf ? h : shelf;
		}
		if (pass == 0)
		{
			for (int i = 0; i <= sheet; i++)
			{
				unsigned long bytes = (HUD64_OUTLINE_WIDTH / 2) * heights[i];
				void *data;

				if (!n64_heap_has(bytes + 4096))
					n64_render_make_room(bytes + 4096, 1);
				data = memalign(16, bytes);
				if (!data)
				{
					for (int j = 0; j < i; j++)
					{
						free(outlines.sheets[first + j].buffer);
						outlines.sheets[first + j] = (surface_t){0};
						outlines.bytes -= (HUD64_OUTLINE_WIDTH / 2) * heights[j];
					}
					return 0;
				}
				memset(data, 0, bytes);
				outlines.sheets[first + i] = surface_make(data, FMT_IA4, HUD64_OUTLINE_WIDTH, heights[i],
					HUD64_OUTLINE_WIDTH / 2);
				outlines.bytes += bytes;
			}
		}
	}
	for (int i = 0; i <= sheet; i++)
	{
		data_cache_hit_writeback(outlines.sheets[first + i].buffer,
			outlines.sheets[first + i].stride * outlines.sheets[first + i].height);
		if (!outlines.uploads[first + i])
			outlines.uploads[first + i] = upload_block(&outlines.sheets[first + i]);
	}
	outlines.sheet_count = first + sheet + 1;
	outlines.built[fi] = 1;
	return 1;
}

/* every outlined face whose textures are in memory */
static void build_all_outlines(void)
{
	for (int fi = 0; fi < HUD64_FACE_COUNT; fi++)
	{
		if (hud64_faces[fi].kind && !outlines.built[fi])
			build_outlines(fi);
	}
}

static unsigned long outline_bytes(void)
{
	return outlines.bytes;
}

/* an outline sheet's upload block (NULL: not a sheet) */
static rspq_block_t *outline_upload(const surface_t *surface)
{
	if (surface >= outlines.sheets && surface < outlines.sheets + HUD64_OUTLINE_SHEETS)
		return outlines.uploads[surface - outlines.sheets];
	return NULL;
}

static void release_outlines(void)
{
	for (int i = 0; i < outlines.sheet_count; i++)
	{
		if (outlines.uploads[i])
			rspq_block_free(outlines.uploads[i]);
		free(outlines.sheets[i].buffer);
	}
	for (int i = 0; i < HUD64_FACE_COUNT; i++)
		free(outlines.cell[i]);
	memset(&outlines, 0, sizeof(outlines));
}

/* ---------- glyphs: drawn in runs (a word list, a number, a view's
640x240 text): a run's outlines first, then its glyphs, unless an outline
would touch an earlier glyph -- then each glyph's outline and the glyph in
turn, the design's order */

struct run_glyph
{
	const struct hud64_face *f;
	const struct hud64_glyph *g;
	int16_t x, y;
	uint32_t colour;
	uint8_t alpha;
};

static struct
{
	struct run_glyph g[HUD64_MAXIMUM_DEFERRED];
	int count;
	int screen; /* (x in the screen's pixels: 640x240's deferred text) */
} run;

/* a dimmed count (the grenade type not selected, or none left): dark digits
-- the unlit slots' dark -- with a light rim, the panel's rim colour; light
digits in a dark rim dimmed to grey had too little contrast to read (a
luminance contrast of 0.16-0.30 where text needs 0.30) */
HOT static uint32_t ring_colour(uint32_t colour)
{
	return (colour & 0xFFFFFF) == HUD64_DIM ? HUD64_OUTLINE : HUD64_SHADOW;
}

HOT static uint32_t fill_colour(uint32_t colour)
{
	return (colour & 0xFFFFFF) == HUD64_DIM ? HUD64_PLATE : colour;
}

HOT static void outline_draw(const struct run_glyph *r)
{
	int fi = (int)(r->f - hud64_faces), gi = (int)(r->g - r->f->glyphs);
	const surface_t *t;

	if (!outlines.built[fi])
	{
		/* (no room for the outline cells: the offset draws themselves) */
		const surface_t *a = texture(r->g->tex);

		if (!a)
			return;
		set_rdp_mode(ia_mode(a, r->alpha));
		upload(a);
		set_colours(ring_colour(r->colour), r->alpha, ring_colour(r->colour));
		for (int k = 0; k < r->f->ring_count; k++)
			rectangle(r->x + r->f->ring[k][0], r->y + r->f->ring[k][1], r->g->x, r->g->y, r->g->w, r->g->h, 1, 0, 0);
		return;
	}
	t = &outlines.sheets[outlines.cell[fi][gi].sheet];
	set_rdp_mode(ia_mode(t, r->alpha));
	upload(t);
	set_colours(ring_colour(r->colour), r->alpha, ring_colour(r->colour));
	rectangle(r->x - 1, r->y - r->f->ry, outlines.cell[fi][gi].x, outlines.cell[fi][gi].y, r->g->w + 2,
		r->g->h + 2 * r->f->ry, 1, 0, 0);
}

HOT static void glyph_draw(const struct run_glyph *r)
{
	const surface_t *t = texture(r->g->tex);

	if (!t)
		return;
	set_rdp_mode(ia_mode(t, r->alpha));
	upload(t);
	if (r->f->kind == 0)
	{
		/* a 320-wide face's cell holds its haze: one draw */
		set_colours(fill_colour(r->colour), r->alpha, ring_colour(r->colour));
		rectangle(r->x - r->f->padx, r->y - r->f->pady, r->g->x, r->g->y, r->g->w, r->g->h, 1, 0, 0);
		return;
	}
	set_colours(fill_colour(r->colour), r->alpha, fill_colour(r->colour));
	rectangle(r->x, r->y, r->g->x, r->g->y, r->g->w, r->g->h, 1, 0, 0);
}

HOT static void run_flush(void)
{
	int xs = draw.m.xs, apart = 1;

	if (!run.count)
		return;
	if (run.screen)
		draw.m.xs = 1;
	/* (does an outline touch an earlier glyph?) */
	for (int j = 1; j < run.count && apart; j++)
	{
		const struct run_glyph *b = &run.g[j];

		if (!b->f->kind)
			continue;
		for (int i = 0; i < j && apart; i++)
		{
			const struct run_glyph *a = &run.g[i];

			if (b->x - 1 < a->x + a->g->w && a->x < b->x + b->g->w + 1 &&
				b->y - b->f->ry < a->y + a->g->h && a->y < b->y + b->g->h + b->f->ry)
				apart = 0;
		}
	}
	if (apart)
	{
		/* the outlines, sheet by sheet (one TMEM load a sheet), then the glyphs */
		for (int sheet = -1; sheet < outlines.sheet_count; sheet++)
		{
			for (int i = 0; i < run.count; i++)
			{
				const struct run_glyph *r = &run.g[i];
				int fi = (int)(r->f - hud64_faces);

				if (r->f->kind && (outlines.built[fi] ? outlines.cell[fi][r->g - r->f->glyphs].sheet : -1) == sheet)
					outline_draw(r);
			}
		}
		for (int i = 0; i < run.count; i++)
			glyph_draw(&run.g[i]);
	}
	else
	{
		for (int i = 0; i < run.count; i++)
		{
			if (run.g[i].f->kind)
				outline_draw(&run.g[i]);
			glyph_draw(&run.g[i]);
		}
	}
	draw.m.xs = xs;
	run.count = 0;
	run.screen = 0;
}

HOT static void run_add(const struct hud64_face *f, char ch, int x, int top, uint32_t colour, int alpha, int screen)
{
	const struct hud64_glyph *g = f ? glyph_of(f, ch) : NULL;

	if (!g)
		return;
	if (run.count && run.screen != screen)
		run_flush();
	if (run.count == HUD64_MAXIMUM_DEFERRED)
		run_flush();
	run.screen = screen;
	run.g[run.count++] = (struct run_glyph){f, g, (int16_t)x, (int16_t)top, colour, (uint8_t)alpha};
}

HOT static void glyph(int fkey, char ch, int x, int top, uint32_t colour, int alpha)
{
	if (draw.m.xs > 1)
	{
		/* (640x240: drawn after the view's graphics, from the 640-wide face) */
		if (draw.deferred_count < HUD64_MAXIMUM_DEFERRED)
		{
			struct deferred *d = &draw.deferred[draw.deferred_count++];

			d->face = (uint8_t)fkey;
			d->ch = ch;
			d->x = (int16_t)x;
			d->y = (int16_t)top;
			d->colour = (colour & 0xFFFFFF) | (uint32_t)alpha << 24;
		}
		return;
	}
	run_add(face(fkey, 0), ch, x, top, colour, alpha, 0);
}

/* a glyph at x in the canvas's pixels (at 640x240 not a whole set pixel:
the range label, placed a canvas pixel at a time); deferred as glyph's, its
face marked (bit 7) */
HOT static void glyph_canvas(int fkey, char ch, int x, int top, uint32_t colour, int alpha)
{
	if (draw.m.xs > 1)
	{
		if (draw.deferred_count < HUD64_MAXIMUM_DEFERRED)
		{
			struct deferred *d = &draw.deferred[draw.deferred_count++];

			d->face = (uint8_t)(fkey | 0x80);
			d->ch = ch;
			d->x = (int16_t)x;
			d->y = (int16_t)top;
			d->colour = (colour & 0xFFFFFF) | (uint32_t)alpha << 24;
		}
		return;
	}
	run_add(face(fkey, 0), ch, x, top, colour, alpha, 0);
}

HOT static void flush_deferred(void)
{
	run_flush();
	for (int i = 0; i < draw.deferred_count; i++)
	{
		const struct deferred *d = &draw.deferred[i];

		run_add(face(d->face & 0x7F, 1), d->ch, d->face & 0x80 ? d->x : d->x * draw.m.xs, d->y, d->colour & 0xFFFFFF,
			d->colour >> 24, 1);
	}
	run_flush();
	draw.deferred_count = 0;
}

enum { FACE_MSG, FACE_N8, FACE_N7, FACE_LBL };
enum { TOKEN_WORD, TOKEN_BUTTON };

struct token
{
	const char *text;
	uint8_t length, kind;
	uint32_t colour;
};

HOT static int word_width(const struct hud64_face *f, const char *text, int length)
{
	int width = 0, n = 0;

	for (int i = 0; i < length; i++)
	{
		const struct hud64_glyph *g = glyph_of(f, text[i]);

		if (g)
			width += g->adv, n++;
	}
	return n ? width + f->gap * (n - 1) : 0;
}

HOT static int token_width(const struct hud64_face *f, const struct token *t)
{
	const struct hud64_set *set = draw.m.set;

	if (t->kind == TOKEN_BUTTON)
		return set->layout.button_adv;
	return word_width(f, t->text, t->length);
}

HOT static int tokens_width(const struct hud64_face *f, const struct token *t, int n)
{
	int width = 0;

	for (int i = 0; i < n; i++)
		width += token_width(f, &t[i]);
	return width + f->space * (n > 1 ? n - 1 : 0);
}

/* the condensed 320-wide face's 1-pixel strokes need full brightness (the
design's draw_tokens and draw_num) */
HOT static uint32_t text_colour(uint32_t colour)
{
	/* (the 75% set's 1-pixel strokes at full brightness, to keep their
	contrast; a dimmed
	grenade count stays dim: it is meant to read as not selected) */
	return draw.m.set->ws && draw.m.set->s == 1 && colour != HUD64_DIM ? HUD64_WHITE : colour;
}

HOT static int draw_tokens(int x, int base, const struct token *tokens, int n, int alpha)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_face *f = face(FACE_MSG, 0);

	for (int i = 0; i < n; i++)
	{
		const struct token *t = &tokens[i];
		uint32_t colour = text_colour(t->colour);

		if (i)
			x += f->space;
		if (t->kind == TOKEN_BUTTON)
		{
			int bw, bh;

			run_flush();
			ref_size(&set->ref[HUD64_REF_BUTTON], &bw, &bh);
			draw_ref(&set->ref[HUD64_REF_BUTTON], x, base - f->cap + floordiv(f->cap - (bh - set->s), 2), HUD64_BUTTON,
				1, HUD64_SHADOW, alpha, 0, 0);
			x += set->layout.button_adv;
			continue;
		}
		{
			int n_drawn = 0;

			for (int k = 0; k < t->length; k++)
			{
				const struct hud64_glyph *g = glyph_of(f, t->text[k]);

				if (!g)
					continue;
				glyph(FACE_MSG, t->text[k], x, base - g->oy, colour, alpha);
				x += g->adv + f->gap;
				n_drawn++;
			}
			if (n_drawn)
				x -= f->gap;
		}
	}
	run_flush();
	return x;
}

/* digits from top (the design's draw_num); returns the end */
HOT static int draw_number(int fkey, int x, int top, int value, uint32_t colour, int gap, int use_gap)
{
	const struct hud64_face *f = face(fkey, 0);
	char text[12];

	colour = text_colour(colour);
	if (!use_gap)
		gap = f->gap;
	snprintf(text, sizeof(text), "%d", value < 0 ? 0 : value);
	for (char *p = text; *p; p++)
	{
		const struct hud64_glyph *g = glyph_of(f, *p);

		if (!g)
			continue;
		glyph(fkey, *p, x, top, colour, 255);
		x += g->adv + gap;
	}
	run_flush();
	return x - gap;
}

HOT static void draw_icon(int key, int x, int y, uint32_t colour)
{
	draw_cell(&draw.m.set->icon[key], x, y, colour, HUD64_SHADOW, 255);
}

/* the round grid: the plate, the slots the magazine holds and the rounds
lit -- a tick a round in every arrangement (60 ticks; a magazine of more
than 60 rounds, which no Xbox weapon has, in proportion: n64_hud64_game.c).
A magazine that would light no tick draws one, lit while it holds a round
(a loaded weapon never shows an empty readout). g: the grid's plate
geometry, its first entry the ticks */
static void round_grid(int meter, int plate, const int16_t *g, int x, int y, const struct n64_hud64_state *st)
{
	float max_v = st->loaded_max / 60.f, lit_v = st->loaded / 60.f;
	int slots = ticks_drawn(g[0], max_v);

	if (slots == 0 && st->loaded_max > 0)
	{
		float one = (float)(255 / g[0]) / 255.f;

		slots = 1;
		max_v = one;
		lit_v = st->loaded > 0 ? one : 0.f;
	}
	draw_plate(plate, g, x, y, slots);
	draw_meter(meter, x, y, max_v, HUD64_TICK_OFF, HUD64_ALPHA_SLOTS, HUD64_TICK_OFF);
	draw_meter(meter, x, y, lit_v, HUD64_TICK_ON, 255, HUD64_TICK_ON);
}

/* a battery weapon's heat (ORIGINAL shows it under the battery): a bar over
the round grid's first row on its one-row plate, filled from the left (a
battery shows its percent, not a round grid), in ORIGINAL's colours along
its length, blue to purple to red (hud64_heat_gradient) -- the meter drawn once a stop, through a scissor
over that stop's stretch of the bar */
static void heat_bar(int meter, int plate, const int16_t *g, const int16_t *at, int x, int y,
	const struct n64_hud64_state *st)
{
	const struct hud64_meter *mt = &draw.m.set->meter[meter];
	int scale = mt->body.scale, bx = x + at[0] * scale, by = y + at[1] * scale, w, h;

	draw_plate(plate, g, x, y, g[1]);
	draw_meter(meter, bx, by, 1.f, HUD64_TICK_OFF, HUD64_ALPHA_SLOTS, HUD64_TICK_OFF);
	ref_size(&mt->body, &w, &h);
	for (int k = 0; k < HUD64_HEAT_STOPS; k++)
	{
		int c0 = (bx + w * k / HUD64_HEAT_STOPS) * draw.m.xs, c1 = (bx + w * (k + 1) / HUD64_HEAT_STOPS) * draw.m.xs;

		rdpq_sync_pipe();
		rdpq_set_scissor(c0 > draw.clip[0] ? c0 : draw.clip[0], draw.clip[1], c1 < draw.clip[2] ? c1 : draw.clip[2],
			draw.clip[3]);
		draw_meter(meter, bx, by, st->heat, hud64_heat_gradient[k], 255, hud64_heat_gradient[k]);
	}
	rdpq_sync_pipe();
	rdpq_set_scissor(draw.clip[0], draw.clip[1], draw.clip[2], draw.clip[3]);
}

/* the bar under the
lemniscate's card, its plate cut to the card's width from its right end
(its ends kept), the bar lit over the cut plate's inside only: a vehicle
gun's heat in ORIGINAL's colours as before (red stays the overheat's), or
(blue) its main recharge in Halo blue, filling from the left, full when the
gun can fire -- the grenade slot's rule (n64_hud64_game.c main_cooldown) */
static void card_bar(int meter, int plate, const int16_t *g, const int16_t *at, int x, int y, int card_w,
	const struct n64_hud64_state *st, int blue)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_meter *mt = &set->meter[meter];
	const struct hud64_ref *pr = &set->ref[plate];
	const surface_t *t = texture(pr->tex);
	int scale = mt->body.scale, bx = x + at[0] * scale, by = y + at[1] * scale, w, h, ps = pr->scale, pw, pdx;
	float cut, v;

	if (!t)
		return;
	pw = t->width * ps;
	pdx = (pw - card_w + ps - 1) / ps;
	if (pdx < g[6])
		pdx = g[6];
	draw_ref_short(pr, x, y, HUD64_PLATE, 255, pdx, g[5], t->height - (g[2] - 1) * g[4]);
	ref_size(&mt->body, &w, &h);
	cut = (float)(pw - pdx * ps - 2 * at[0] * scale) / (float)w;
	cut = cut < 0.f ? 0.f : cut > 1.f ? 1.f : cut;
	draw_meter(meter, bx, by, cut, HUD64_TICK_OFF, HUD64_ALPHA_SLOTS, HUD64_TICK_OFF);
	if (blue)
	{
		draw_meter(meter, bx, by, st->cooldown_fill / 255.f * cut, HUD64_BLUE, 255, HUD64_BLUE);
		return;
	}
	v = st->heat * cut;
	for (int k = 0; k < HUD64_HEAT_STOPS; k++)
	{
		int c0 = (bx + (int)(w * cut) * k / HUD64_HEAT_STOPS) * draw.m.xs;
		int c1 = (bx + (int)(w * cut) * (k + 1) / HUD64_HEAT_STOPS) * draw.m.xs;

		rdpq_sync_pipe();
		rdpq_set_scissor(c0 > draw.clip[0] ? c0 : draw.clip[0], draw.clip[1], c1 < draw.clip[2] ? c1 : draw.clip[2],
			draw.clip[3]);
		draw_meter(meter, bx, by, v, hud64_heat_gradient[k], 255, hud64_heat_gradient[k]);
	}
	rdpq_sync_pipe();
	rdpq_set_scissor(draw.clip[0], draw.clip[1], draw.clip[2], draw.clip[3]);
}

/* the grenade panel: both types with their counts, the design's panel;
the type a throw uses lit, the other dimmed, and a type with none dimmed
with its 0 (the selected type alone would hide the grenades in reserve) */
static void grenades(const struct n64_hud64_state *st, int fill, int edge, int gx, int Tp, int fkey, int icon_x,
	int frag_y, int plasma_y, int count_dx, int count_y, int plasma_dx, int pnum_dx)
{
	const struct hud64_set *set = draw.m.set;
	int frag_lit = st->grenade_type != 1 && st->frags > 0, plasma_lit = st->grenade_type == 1 && st->plasmas > 0, x;

	draw_ref(&set->ref[fill], gx, Tp, st->grenade_fill ? (uint32_t)(st->grenade_fill & 0xFFFFFF) : HUD64_BLUE, 0, 0, 255, 0, 0);
	draw_ref(&set->ref[edge], gx, Tp, st->grenade_edge ? (uint32_t)(st->grenade_edge & 0xFFFFFF) : HUD64_OUTLINE, 0, 0,
		HUD64_ALPHA_EDGE, 0, 0);
	/* (a type out, a switch to it or a throw with none: its slot flashes
	white twice -- n64_hud64_game.c grenade_flash) */
	uint32_t frag_c = frag_lit ? HUD64_NUMBER : HUD64_DIM, plasma_c = plasma_lit ? HUD64_PLASMA : HUD64_DIM;
	uint32_t fnum_c = frag_lit ? HUD64_NUMBER : HUD64_DIM, pnum_c = plasma_lit ? HUD64_NUMBER : HUD64_DIM;

	if (st->grenade_flash & 2)
		plasma_c = pnum_c = HUD64_WHITE;
	if (st->grenade_flash & 1)
		frag_c = fnum_c = HUD64_WHITE;
	draw_icon(HUD64_ICON_FRAG, icon_x, frag_y, frag_c);
	x = draw_number(fkey, icon_x + count_dx, count_y, st->frags, fnum_c, 0, 0);
	draw_icon(HUD64_ICON_PLASMA, x + plasma_dx, plasma_y, plasma_c);
	draw_number(fkey, x + pnum_dx, count_y, st->plasmas, pnum_c, 0, 0);
}

/* the seat's secondary function where the grenade panel is, that panel's place and size: dark
while the secondary recharges, its blue filling from the left as far as
the recharge has gone (the game's fill, from the weapon's own magazine
state: n64_hud64_game.c secondary_seat); then the grenade button's letter
in Halo's button green and the secondary's icon (a charge "*" in the
numeral face, or the round icon), dimmed until it is ready */
static void secondary_slot(const struct n64_hud64_state *st, int fill, int edge, int gx, int Tp, int fkey, int x, int ny)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_ref *panel = &set->ref[fill];
	const struct hud64_face *msg = face(FACE_MSG, 0), *num = face(fkey, 0);
	const struct hud64_glyph *letter = glyph_of(msg, st->secondary_button);
	int ready = st->secondary_fill >= 255, w, h;
	uint32_t lit = st->secondary_icon ? HUD64_NUMBER : HUD64_PLASMA;

	if (ready)
		draw_ref(panel, gx, Tp, HUD64_BLUE, 0, 0, 255, 0, 0);
	else
	{
		int c0, c1;

		ref_size(panel, &w, &h);
		draw_ref(panel, gx, Tp, HUD64_TICK_OFF, 0, 0, 255, 0, 0);
		/* (the blue through a scissor over the part gone, as heat_bar draws) */
		c0 = gx * draw.m.xs;
		c1 = (gx + w * st->secondary_fill / 255) * draw.m.xs;
		if (c1 > c0)
		{
			rdpq_sync_pipe();
			rdpq_set_scissor(c0 > draw.clip[0] ? c0 : draw.clip[0], draw.clip[1], c1 < draw.clip[2] ? c1 : draw.clip[2],
				draw.clip[3]);
			draw_ref(panel, gx, Tp, HUD64_BLUE, 0, 0, 255, 0, 0);
			rdpq_sync_pipe();
			rdpq_set_scissor(draw.clip[0], draw.clip[1], draw.clip[2], draw.clip[3]);
		}
	}
	draw_ref(&set->ref[edge], gx, Tp, HUD64_OUTLINE, 0, 0, HUD64_ALPHA_EDGE, 0, 0);
	if (letter)
	{
		glyph(FACE_MSG, st->secondary_button, x, ny + (num->cap - msg->cap) / 2, ready ? HUD64_BUTTON : HUD64_DIM, 255);
		x += letter->adv + 2 * set->s;
	}
	if (st->secondary_icon)
	{
		const struct hud64_cell *c = &set->icon[HUD64_ICON_ROUND];

		draw_icon(HUD64_ICON_ROUND, x, ny + (num->cap - c->h * c->scale) / 2, ready ? lit : HUD64_DIM);
	}
	else
		glyph(fkey, '*', x, ny, ready ? lit : HUD64_DIM, 255);
}

/* ---------- the elements (the design's compose_full and compose_4p); each
variant its own part, laid out from its table */

/* unlimited ammunition: the panel shortened to the lemniscate,
the panel's middle columns left out (its ends kept); the glyph in the
count's place; the weapon's heat, if it has one, as before. The texels left
out are the tables' (hud64_gen.py _inf_dx): the length whose blue
balances best about the glyph, measured on the textures. (A panel of the
glyph's advance, its margin before it twice, and the slant's run -- a third
of the panel's height -- more puts the card's middle half that run right of
the glyph's: more blue right of the glyph than left.) */
static int infinite_panel(int panel, int edge, int pw, int ph, int L, int Tp, int dx)
{
	const struct hud64_set *set = draw.m.set;
	int sc = set->ref[panel].scale, cut = pw / 3 / sc;

	/* (its height in texels, as draw_ref_short takes it: the panel's height is the set's pixels -- at 480 lines,
	where a texel is 2 pixels, the panel's 24 drew 24 texels of a 12-texel texture, the clamped last row a solid
	block under the card) */
	draw_ref_short(&set->ref[panel], L, Tp, HUD64_BLUE, 255, dx, cut, ph / sc);
	draw_ref_short(&set->ref[edge], L, Tp, HUD64_OUTLINE, HUD64_ALPHA_EDGE, dx, cut, ph / set->ref[edge].scale);
	return pw - dx * sc;
}

static void ammo_full(const struct n64_hud64_state *st, int L, int Tp)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_layout *ly = &set->layout;
	int ny = Tp + ly->full_num_dy, x, gx;

	if (st->show_ammo && st->ammo_infinite)
	{
		int card_w = infinite_panel(HUD64_REF_WEAPON, HUD64_REF_WEAPON_EDGE, set->panel[0][0], set->panel[0][1], L, Tp,
			ly->full_inf_dx);

		glyph(FACE_N8, '~', L + ly->full_num_dx, ny, HUD64_NUMBER, 255);
		if (st->ammo_kind == N64_HUD64_BATTERY && st->show_heat)
			card_bar(HUD64_METER_HEAT, HUD64_REF_PLATE, ly->full_plate, ly->full_heat, L, Tp + ly->full_ticks_dy, card_w,
				st, 0);
		else if (st->show_cooldown)
			card_bar(HUD64_METER_HEAT, HUD64_REF_PLATE, ly->full_plate, ly->full_heat, L, Tp + ly->full_ticks_dy, card_w,
				st, 1);
	}
	else if (st->show_ammo)
		draw_ref(&set->ref[HUD64_REF_WEAPON], L, Tp, HUD64_BLUE, 0, 0, 255, 0, 0);
	if (st->show_ammo && !st->ammo_infinite)
		draw_ref(&set->ref[HUD64_REF_WEAPON_EDGE], L, Tp, HUD64_OUTLINE, 0, 0, HUD64_ALPHA_EDGE, 0, 0);
	if (st->show_ammo && !st->ammo_infinite && st->ammo_kind != N64_HUD64_NO_AMMO)
	{
		int ty = Tp + ly->full_ticks_dy;

		x = draw_number(FACE_N8, L + ly->full_num_dx, ny, st->total, HUD64_NUMBER, 0, 0);
		/* (a battery: its unit after its percent, the face's gap on, so
		that the number reads as a percent, not as a count) */
		if (st->ammo_kind == N64_HUD64_BATTERY)
			glyph(FACE_N8, '%', x + face(FACE_N8, 0)->gap, ny, HUD64_NUMBER, 255);
		if (st->ammo_kind == N64_HUD64_ROUNDS)
		{
			draw_icon(HUD64_ICON_TIMES, x + ly->full_times_dx, ny + ly->full_icon_dy, HUD64_NUMBER);
			draw_icon(HUD64_ICON_ROUND, x + ly->full_round_dx, ny + ly->full_icon_dy, HUD64_NUMBER);
		}
		/* (a battery: its percent alone, no round grid) */
		if (st->ammo_kind == N64_HUD64_ROUNDS)
			round_grid(HUD64_METER_TICKS, HUD64_REF_PLATE, ly->full_plate, L, ty, st);
		else if (st->show_heat)
			heat_bar(HUD64_METER_HEAT, HUD64_REF_PLATE, ly->full_plate, ly->full_heat, L, ty, st);
	}
	gx = L + ly->full_gren_dx;
	if (!st->show_grenades)
	{
		if (st->show_secondary)
			secondary_slot(st, HUD64_REF_GRENADE, HUD64_REF_GRENADE_EDGE, gx, Tp, FACE_N8, gx + ly->full_frag_dx, ny);
		return;
	}
	grenades(st, HUD64_REF_GRENADE, HUD64_REF_GRENADE_EDGE, gx, Tp, FACE_N8, gx + ly->full_frag_dx, ny,
		ny + ly->full_plasma_dy, ly->full_gnum_dx, ny, ly->full_plasma_dx, ly->full_pnum_dx);
}

/* 3-4 players' readout (the design's top_left): a part of its own */
static void ammo_split(const struct n64_hud64_state *st, int L, int Tp, int R)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_layout *ly = &set->layout;
	int ny = Tp + ly->split_num_dy, gx;
	/* (the ammunition count a unit clear of the frag icon, the grenade
	panel a unit wider -- unless that brings it closer to the shield panel
	than the panels' gap, in a view narrower than split_gren_room (HUD
	INSET 20, 4:3): there the tighter spacing and the narrower panel) */
	int fallback = R - L < ly->split_gren_room;

	if (st->show_ammo && st->ammo_infinite)
	{
		int card_w = infinite_panel(HUD64_REF_AMMO4, HUD64_REF_AMMO4_EDGE, set->panel[2][0], set->panel[2][1], L, Tp,
			ly->split_inf_dx);

		glyph(FACE_N7, '~', L + ly->split_num_dx, ny, HUD64_NUMBER, 255);
		if (st->ammo_kind == N64_HUD64_BATTERY && st->show_heat)
			card_bar(HUD64_METER_HEAT4, HUD64_REF_PLATE4, ly->split_plate, ly->split_heat, L, Tp + ly->split_ticks_dy,
				card_w, st, 0);
		else if (st->show_cooldown)
			card_bar(HUD64_METER_HEAT4, HUD64_REF_PLATE4, ly->split_plate, ly->split_heat, L, Tp + ly->split_ticks_dy,
				card_w, st, 1);
	}
	else if (st->show_ammo)
		draw_ref(&set->ref[HUD64_REF_AMMO4], L, Tp, HUD64_BLUE, 0, 0, 255, 0, 0);
	if (st->show_ammo && !st->ammo_infinite)
		draw_ref(&set->ref[HUD64_REF_AMMO4_EDGE], L, Tp, HUD64_OUTLINE, 0, 0, HUD64_ALPHA_EDGE, 0, 0);
	if (st->show_ammo && !st->ammo_infinite && st->ammo_kind != N64_HUD64_NO_AMMO)
	{
		int ty = Tp + ly->split_ticks_dy;

		int x = draw_number(FACE_N7, L + ly->split_num_dx, ny, st->total, HUD64_NUMBER, ly->split_num_gap, 1);

		if (st->ammo_kind == N64_HUD64_BATTERY)
			glyph(FACE_N7, '%', x + ly->split_num_gap, ny, HUD64_NUMBER, 255);
		if (st->ammo_kind == N64_HUD64_ROUNDS)
			round_grid(HUD64_METER_TICKS4, HUD64_REF_PLATE4, ly->split_plate, L, ty, st);
		else if (st->show_heat)
			heat_bar(HUD64_METER_HEAT4, HUD64_REF_PLATE4, ly->split_plate, ly->split_heat, L, ty, st);
	}
	gx = L + ly->split_gren_dx;
	if (!st->show_grenades)
	{
		if (st->show_secondary)
			secondary_slot(st, fallback ? HUD64_REF_GREN4_E : HUD64_REF_GREN4, fallback ? HUD64_REF_GREN4_E_EDGE :
				HUD64_REF_GREN4_EDGE, gx, Tp, FACE_N7, gx + (fallback ? ly->split_frag_dx_e : ly->split_frag_dx), ny);
		return;
	}
	grenades(st, fallback ? HUD64_REF_GREN4_E : HUD64_REF_GREN4, fallback ? HUD64_REF_GREN4_E_EDGE : HUD64_REF_GREN4_EDGE,
		gx, Tp, FACE_N7, gx + (fallback ? ly->split_frag_dx_e : ly->split_frag_dx), Tp + ly->split_frag_dy,
		Tp + ly->split_plasma_dy, ly->split_gnum_dx, ny, ly->split_plasma_dx, ly->split_pnum_dx);
}

static void shield(const struct n64_hud64_state *st, int R, int Tp, int split)
{
	const struct hud64_set *set = draw.m.set;
	int sx0 = R - (split ? set->shield_s_size[0] : set->shield_size[0]);

	/* (Halo's low-shield blink -- the frame's outline and tray blended
	toward the tag's flashing colour by Halo's factor, the badge's outline
	with them) */
	uint32_t outline = st->shield_flash ? blend(HUD64_OUTLINE, st->shield_flash_colour, st->shield_flash) : HUD64_OUTLINE;
	uint32_t tray = st->shield_flash ? blend(HUD64_BLUE, st->shield_flash_colour, st->shield_flash) : HUD64_BLUE;

	draw_ref(&set->ref[split ? HUD64_REF_SHIELD_S_FRAME : HUD64_REF_SHIELD_FRAME], sx0, Tp, outline, 1, tray, 255, 0, 0);
	/* (640 across: the shield's own meter there, a column a pixel, so that
	the recharge moves a pixel at a time, not 2) */
	int meter = split ? HUD64_METER_SHIELD_S : HUD64_METER_SHIELD;

	if (draw.m.xs == 2 && texture(set->meter[split ? HUD64_METER_SHIELD_S_W : HUD64_METER_SHIELD_W].body.tex))
		meter = split ? HUD64_METER_SHIELD_S_W : HUD64_METER_SHIELD_W;
	/* the shield and the overshield: as Halo's
	hud_unit.c draws it, a meter a level over the shield, filled to the
	level's part, in Halo's overcharge colours; the meter's top edge stays the
	design's. Each level's damage ghost first: the held part in the
	level's colour plus Halo's flash colour by its factor (white while held),
	flat; the level's meter over it leaves the lost part showing */
	float held = st->shield_ghost ? st->shield_held : 0.f;

	for (int level = 0; level < 5 && (level == 0 || st->shield > (float)level || held > (float)level); level++)
	{
		uint32_t c = level ? hud64_overcharge[level] : HUD64_BLUE;
		float v = st->shield - (float)level, h = held - (float)level;

		v = v < 0.f ? 0.f : v > 1.f ? 1.f : v;
		h = h < 0.f ? 0.f : h > 1.f ? 1.f : h;
		if (st->shield_ghost && h > v)
		{
			uint32_t g = 0;

			for (int sh = 0; sh <= 16; sh += 8)
			{
				int a = (int)((c >> sh) & 255) + (int)((st->shield_ghost_colour >> sh) & 255) * st->shield_ghost / 255;

				g |= (uint32_t)(a > 255 ? 255 : a) << sh;
			}
			meter_flat = 1;
			draw_meter(meter, sx0, Tp, h, g, HUD64_ALPHA_SHIELD, g);
			meter_flat = 0;
		}
		draw_meter(meter, sx0, Tp, level ? st->shield - (float)level : st->shield, c, HUD64_ALPHA_SHIELD, HUD64_OUTLINE);
	}
	/* (the badge left of the bar at 1-2 players -- its
	outline, its unlit slot, and lit at any shield its fill and crest in
	each level's colour in turn, as the meter's levels) */
	if (!split)
	{
		draw_ref(&set->ref[HUD64_REF_BADGE_EDGE], sx0, Tp, outline, 0, 0, HUD64_ALPHA_EDGE, 0, 0);
		draw_ref(&set->ref[HUD64_REF_BADGE_SLOT], sx0, Tp, HUD64_BLUE, 0, 0, HUD64_ALPHA_BADGE_SLOT, 0, 0);
		if ((int)floorf(st->shield * 255.f + 1e-4f) > 0)
			for (int level = 0; level < 5 && (level == 0 || st->shield > (float)level); level++)
			{
				uint32_t c = level ? hud64_overcharge[level] : HUD64_BLUE, crest = 0;

				for (int sh = 0; sh <= 16; sh += 8)
				{
					int v = (int)((c >> sh) & 255);

					crest |= (uint32_t)(v + ((255 - v) * 25 + 50) / 100) << sh;
				}
				draw_ref(&set->ref[HUD64_REF_BADGE_FILL], sx0, Tp, c, 0, 0, HUD64_ALPHA_SHIELD, 0, 0);
				draw_ref(&set->ref[HUD64_REF_BADGE_CREST], sx0, Tp, crest, 0, 0, HUD64_ALPHA_SHIELD, 0, 0);
			}
	}
	/* (the pips -- the gauge's frame, its empty slots,
	then the lit cells over them, Halo's count: the cells times the health,
	rounded to the nearest, a tie to even, as hud_unit.c's fast_ftol_C) */
	/* (Halo's health-panel flash, 1 player -- the gauge's outline blended
	toward the tag's flashing colour by Halo's factor) */
	draw_ref(&set->ref[split ? HUD64_REF_HEALTH_S_FRAME : HUD64_REF_HEALTH_FRAME], sx0, Tp,
		st->health_flash ? blend(HUD64_OUTLINE, st->health_flash_colour, st->health_flash) : HUD64_OUTLINE, 1,
		HUD64_BLUE, 255, 0, 0);
	draw_ref(&set->ref[split ? HUD64_REF_HEALTH_S_SLOTS : HUD64_REF_HEALTH_SLOTS], sx0, Tp, HUD64_PIP_EMPTY, 0, 0,
		HUD64_ALPHA_PIP_EMPTY, 0, 0);
	/* (the health in Halo's colour for its level -- yellow between the
	unit HUD's cutoffs, red under them, as ORIGINAL; the design's above) */
	uint32_t health = st->health_colour ? (uint32_t)(st->health_colour & 0xFFFFFF) : HUD64_BLUE;
	int pips = split ? set->layout.split_pips : set->layout.full_pips;
	/* (exactly: the fraction v - floor(v) is exact in float) */
	float pv = (float)pips * st->health;
	int lit = (int)floorf(pv);
	float pf = pv - (float)lit;

	if (pf > 0.5f || (pf == 0.5f && (lit & 1)))
		lit++;

	if (lit > pips)
		lit = pips;
	/* (each cell's texels at its count from the right: a value of lit / 255
	lights the rightmost lit cells) */
	if (lit > 0)
		draw_meter(split ? HUD64_METER_HEALTH_S : HUD64_METER_HEALTH, sx0, Tp, ((float)lit + 0.5f) / 255.f, health,
			HUD64_ALPHA_HEALTH, health);
	if (!split)
		draw_icon(HUD64_ICON_CROSS, sx0 + set->layout.full_cross_dx, Tp + set->layout.full_cross_dy,
			st->health_colour ? health : HUD64_OUTLINE);
}

/* Halo's motion tracker, which Halo draws with either style (ORIGINAL's
tracker art, which the player knows), its box in the canvas's
pixels as the port places Halo's HUD (hud64_gen.py trk_*): its corner a
fixed distance in from the view's edges (more from a screen edge) and
HUD INSET, its size Halo's (smaller from 2 players), squeezed to 0.75
about the view's middle in WIDESCREEN. The design's parts keep out of it */
static void tracker_box(int x0, int y0, int x1, int y1, int views, int inset, int box[4])
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_layout *ly = &set->layout;
	int size = ly->trk_size[views > 1], s = set->s;
	int left = x0 + (x0 == 0 ? ly->trk_edge[0] : ly->trk_edge[1]) + inset * s;
	int bottom = y1 - (y1 >= draw.m.canvas_h ? ly->trk_edge[2] : ly->trk_edge[3]) - inset * s;
	float l = (float)left, r = (float)(left + size);
	/* the canvas against the set's design width: below 1 in a narrow frame
	(ULTRA's 512x480, HIGH DETAIL's 520- and 480-wide fallbacks, which take
	the 75%-wide sets) */
	float k = (float)draw.m.canvas_w / (320.f * s), squeeze = n64_render_hud_squeeze();

	(void)y0;
	/* as Halo places its HUD (n64_hud_game.c hud_x): scaled across with the
	frame from its left edge -- in a narrow frame -- then, WIDESCREEN,
	squeezed about the view's middle (a narrow frame squeezed instead of
	scaled would put the box 30 pixels right of Halo's tracker at 512x480) */
	if (k < 0.999f)
	{
		l = x0 + (l - x0) * k;
		r = l + size * k;
	}
	if (squeeze != 1.f)
	{
		float c = (x0 + x1) * 0.5f;

		l = c + (l - c) * squeeze;
		r = c + (r - c) * squeeze;
	}
	draw.trk_l = l, draw.trk_r = r;
	box[0] = (int)floorf(l + 0.5f);
	box[2] = (int)ceilf(r);
	box[3] = bottom;
	box[1] = bottom - size;
}

/* the tracker's range label, at Halo's place but never over the tracker's
circle: the label's own smaller face, its text where Halo's small label is (its left the tracker box's left, its baseline the box's last row),
then moved the shortest way down, left or both until none of its pixels --
outline and haze -- is on Halo's tracker circle, the plate's edge as drawn:
an ellipse from the box (trk_plate: its centre a little above the box's
middle, its radii half the box and a little more), each pixel tested at its
middle against the face's rows of ink (lbl_ink; 640x240 the 640-wide face's,
lbl_wink). It keeps on the screen and in its own view; across it moves on the
set's grid. Where no place clears the circle it stays at Halo's place */
HOT static int label_clear(const int16_t *ink, int X, int Y, const float c[4], const int v[4])
{
	for (int i = 0; i < ink[1]; i++)
	{
		int a = ink[2 + 2 * i], b = ink[3 + 2 * i], y = Y + ink[0] + i;
		float xn, u, w;

		if (a > b)
			continue;
		a += X, b += X;
		if (y < v[1] || y >= v[3] || a < v[0] || b >= v[2])
			return 0;
		/* (the row's pixel nearest the centre) */
		xn = c[0] - 0.5f;
		xn = xn < (float)a ? (float)a : xn > (float)b ? (float)b : xn;
		u = (xn + 0.5f - c[0]) / c[2], w = ((float)y + 0.5f - c[1]) / c[3];
		if (u * u + w * w <= 1.f)
			return 0;
	}
	return 1;
}

HOT static int label_place(const int box[4], int views, int vx0, int vy0, int vx1, int vy1, int *x, int *top, int *mdx, int *mdy)
{
	const struct hud64_set *set = draw.m.set;
	const struct hud64_layout *ly = &set->layout;
	const struct hud64_face *f = face(FACE_LBL, 0);
	int xs = draw.m.xs, top0 = box[3] - f->cap, best = -1, bx = 0, by = 0, v[4], x0;
	const int16_t *ink = xs > 1 ? ly->lbl_wink : ly->lbl_ink;
	float l = draw.trk_l * xs, r = draw.trk_r * xs, c[4];
	/* (the circle's constants: players, then the frame -- 0 full, 1 the
	640x240 canvas or a narrow frame, 2 a narrow WIDESCREEN frame) */
	int variant = set->s == 1 ? xs > 1 :
		draw.m.canvas_w < 320 * set->s ? (n64_render_hud_squeeze() != 1.f ? 2 : 1) : 0;
	const int16_t *k = &ly->trk_plate[((views > 1) * 3 + variant) * 4];

	/* (Halo's small label: its left the first canvas pixel at or right of the
	tracker's unrounded left -- Halo draws its HUD at the canvas's pixels) */
	x0 = (int)ceilf(l - 0.001f);
	c[0] = (l + r) * 0.5f + k[0] * 0.125f;
	c[1] = (float)(box[1] + box[3]) * 0.5f + k[1] * 0.125f;
	c[2] = (r - l) * 0.5f + k[2] * 0.125f;
	c[3] = (float)(box[3] - box[1]) * 0.5f + k[3] * 0.125f;
	/* (the view on the screen, the canvas's pixels) */
	v[0] = vx0 * xs < 0 ? 0 : vx0 * xs;
	v[1] = vy0 < 0 ? 0 : vy0;
	v[2] = vx1 * xs > draw.m.canvas_w * xs ? draw.m.canvas_w * xs : vx1 * xs;
	v[3] = vy1 > draw.m.canvas_h ? draw.m.canvas_h : vy1;
	for (int dy = 0; dy < 40 && (best < 0 || dy * dy <= best); dy++)
		for (int dx = 0; dx < 40; dx++)
		{
			int d2 = dx * dx + dy * dy;

			if (best >= 0 && d2 > best)
				break;
			/* (a tie: the smaller move across) */
			if ((best < 0 || d2 < best || dx < bx) && label_clear(ink, x0 - dx, top0 + dy, c, v))
				best = d2, bx = dx, by = dy;
		}
	if (best < 0)
		bx = by = 0;
	/* (x in the canvas's pixels: at 640x240 the label's glyphs are placed a
	canvas pixel at a time, glyph_canvas) */
	*x = x0 - bx, *top = top0 + by, *mdx = -bx, *mdy = by;
	return best >= 0;
}

/* a frame -- the mode's set and
canvas, the player count, HUD INSET, WIDESCREEN -- where a 1-2p view has no
clear place for the range label (ULTRA 512x480, 2 players, 4:3, HUD INSET
0: the tracker on the view's border) shows Halo's own small label in every
view instead, at Halo's place and off the circle; the 64 style draws none
there. From the same search as label_place, kept until the frame changes */
HOT static int label_halo(int views)
{
	static int key[7] = {-1, -1, -1, -1, -1, -1, -1}, result;
	int k[7], inset = n64_hud_inset(), same = 1;
	struct mode m;

	current_mode(&m);
	k[0] = m.set_index, k[1] = m.canvas_w, k[2] = m.canvas_h, k[3] = m.xs, k[4] = views, k[5] = inset;
	k[6] = (int)(n64_render_hud_squeeze() * 1000.f);
	for (int i = 0; i < 7; i++)
		same = same && k[i] == key[i];
	if (same)
		return result;
	for (int i = 0; i < 7; i++)
		key[i] = k[i];
	result = 0;
	draw.m = m;
	if (views > 2 || !face(FACE_LBL, 0))
		return result;
	for (int v = 0; v < views && !result; v++)
	{
		int y0 = v * m.canvas_h / views, y1 = (v + 1) * m.canvas_h / views, box[4], x, top, dx, dy;

		tracker_box(0, y0, m.canvas_w, y1, views, inset, box);
		result = !label_place(box, views, 0, y0, m.canvas_w, y1, &x, &top, &dx, &dy);
	}
	return result;
}

HOT static void range_label(const int box[4], int views, int vx0, int vy0, int vx1, int vy1)
{
	static const char text[] = "15m";
	const struct hud64_face *f = face(FACE_LBL, 0);
	int x, top, dx, dy;

	if (!f)
		return;
	label_place(box, views, vx0, vy0, vx1, vy1, &x, &top, &dx, &dy);
	for (int i = 0; text[i]; i++)
	{
		const struct hud64_glyph *g = glyph_of(f, text[i]);

		if (!g)
			continue;
		glyph_canvas(FACE_LBL, text[i], x, top, text_colour(HUD64_HELP), 255);
		x += (g->adv + f->gap) * draw.m.xs;
	}
}

/* the score (the design's draw_score): a part of its own; its width */
static int score(const struct n64_hud64_state *st, int R, int bottom, int words)
{
	static const char *const labels[3] = {"YOU", "1ST", "2ND"};
	const struct hud64_set *set = draw.m.set;
	const struct hud64_layout *ly = &set->layout;
	const struct hud64_face *f = face(FACE_MSG, 0);
	int lw = 0, nw = word_width(f, "88", 2), w, x0;

	if (words)
	{
		for (int i = 0; i < 3; i++)
		{
			int k = word_width(f, labels[i], 3);
			lw = k > lw ? k : lw;
		}
		lw += f->space;
	}
	w = lw + nw;
	x0 = R - ly->score_dx - w;
	for (int k = 0; k < 2; k++)
	{
		int value = k ? st->other : st->score;
		int base = bottom - ly->score_base_dy - (1 - k) * (ly->score_row_h + ly->score_row_gap);
		int tx = x0 + w - nw, by = base + ly->score_bar_dy;
		char text[8];
		struct token t = {text, 0, TOKEN_WORD, HUD64_NUMBER};

		if (words)
		{
			struct token label = {k ? labels[st->other_leads ? 1 : 2] : labels[0], 3, TOKEN_WORD, HUD64_HELP};

			draw_tokens(x0, base, &label, 1, 255);
		}
		snprintf(text, sizeof(text), "%d", value < 0 ? 0 : value > 999 ? 999 : value);
		t.length = (uint8_t)strlen(text);
		draw_tokens(tx + nw - tokens_width(f, &t, 1), base, &t, 1, 255);
		draw_ref(&set->ref[HUD64_REF_BAR_SLOT], tx, by, HUD64_BLUE, 0, 0, 255, 0, 0);
		draw_meter(HUD64_METER_BAR, tx, by, st->limit > 0 ? (float)value / st->limit : 0.f, k ? HUD64_LEADER : HUD64_BLUE,
			HUD64_ALPHA_BAR, k ? HUD64_LEADER : HUD64_BLUE);
	}
	return w;
}

/* ---------- messages: each a list of forms, the longest that fits one
line taken (verb and object kept); else the last wrapped at its words */

struct form
{
	struct token tokens[HUD64_MAXIMUM_TOKENS];
	int count;
};

struct message
{
	struct form forms[2];
	int form_count;
	int alpha;
};

struct line
{
	const struct token *tokens;
	int count, alpha;
};

/* the text's words as tokens, '\1' switching to the name colour and back */
static void words_into(struct form *f, const char *text, uint32_t colour, uint32_t other)
{
	const char *p = text;

	while (*p && f->count < HUD64_MAXIMUM_TOKENS)
	{
		const char *start;

		while (*p == ' ' || *p == '\1' || *p == '\2')
		{
			if (*p == '\1')
			{
				uint32_t c = colour;
				colour = other;
				other = c;
			}
			else if (*p == '\2')
				f->tokens[f->count++] = (struct token){"", 0, TOKEN_BUTTON, colour};
			p++;
			if (f->count >= HUD64_MAXIMUM_TOKENS)
				return;
		}
		if (!*p)
			break;
		start = p;
		while (*p && *p != ' ' && *p != '\1' && *p != '\2')
			p++;
		f->tokens[f->count++] = (struct token){start, (uint8_t)(p - start), TOKEN_WORD, colour};
	}
}

static void token_into(struct form *f, struct token t)
{
	if (f->count < HUD64_MAXIMUM_TOKENS)
		f->tokens[f->count++] = t;
}

/* the pickup prompt's forms ("HOLD [B] TO PICK UP <weapon>", then
"HOLD [B] PICK UP <short name>", the last wrapped at its words where neither
fits): its own table part. 3-4 players start from the short form */
static const char *const prompt_words[2][2] = {{"TO PICK UP", "PICK UP"}, {"TO SWAP FOR", "SWAP FOR"}};

static void prompt_message(struct message *m, const struct n64_hud64_state *st, int first_form)
{
	const struct hud64_weapon_name *known = NULL;
	struct token hold = {"HOLD", 4, TOKEN_WORD, HUD64_HELP}, button = {"", 0, TOKEN_BUTTON, HUD64_HELP};

	for (unsigned i = 0; i < sizeof(hud64_weapon_names) / sizeof(hud64_weapon_names[0]); i++)
	{
		if (!strcmp(st->prompt, hud64_weapon_names[i].name))
			known = &hud64_weapon_names[i];
	}
	m->form_count = 0;
	for (int k = first_form; k < 2; k++)
	{
		struct form *f = &m->forms[m->form_count];

		f->count = 0;
		token_into(f, hold);
		token_into(f, button);
		words_into(f, prompt_words[st->prompt_swap != 0][k], HUD64_HELP, HUD64_HELP);
		words_into(f, k == 1 && known ? known->short_name : st->prompt, HUD64_NUMBER, HUD64_NUMBER);
		m->form_count++;
	}
	m->alpha = 255;
}

/* a game message (a kill, an item): as it is, then without a first "YOU" */
static void text_message(struct message *m, const char *text, int alpha)
{
	struct form *f = &m->forms[0];

	f->count = 0;
	words_into(f, text, HUD64_HELP, HUD64_NUMBER);
	m->form_count = 1;
	if (f->count > 1 && f->tokens[0].length == 3 && !memcmp(f->tokens[0].text, "YOU", 3))
	{
		m->forms[1] = *f;
		memmove(m->forms[1].tokens, m->forms[1].tokens + 1, (f->count - 1) * sizeof(struct token));
		m->forms[1].count--;
		m->form_count = 2;
	}
	m->alpha = alpha;
}

static int layout_messages(const struct message *msgs, int n, int avail, struct line *lines, int maximum)
{
	const struct hud64_face *f = face(FACE_MSG, 0);
	int count = 0;

	for (int i = 0; i < n; i++)
	{
		const struct message *m = &msgs[i];
		int k;

		for (k = 0; k < m->form_count; k++)
		{
			if (tokens_width(f, m->forms[k].tokens, m->forms[k].count) <= avail)
			{
				if (count < maximum)
					lines[count++] = (struct line){m->forms[k].tokens, m->forms[k].count, m->alpha};
				break;
			}
		}
		if (k < m->form_count || !m->form_count)
			continue;
		{
			const struct form *last = &m->forms[m->form_count - 1];
			int start = 0;

			for (int t = 0; t < last->count; t++)
			{
				if (t > start && tokens_width(f, last->tokens + start, t - start + 1) > avail)
				{
					if (count < maximum)
						lines[count++] = (struct line){last->tokens + start, t - start, m->alpha};
					start = t;
				}
			}
			if (count < maximum)
				lines[count++] = (struct line){last->tokens + start, last->count - start, m->alpha};
		}
	}
	return count;
}

/* lines stacked up from B, the newest last (message_lines) */
static void message_lines(const struct line *lines, int n, int x, int B)
{
	const struct hud64_layout *ly = &draw.m.set->layout;
	int lift = 0;

	if (n)
	{
		const struct line *last = &lines[n - 1];

		for (int i = 0; i < last->count; i++)
		{
			if (last->tokens[i].kind == TOKEN_BUTTON)
				lift = ly->lift;
		}
	}
	for (int k = 0; k < n; k++)
		draw_tokens(x, B - ly->msg_base_dy - lift - (n - 1 - k) * ly->pitch, lines[k].tokens, lines[k].count,
			lines[k].alpha);
}

/* the view's message lines, at most the arrangement's: the prompt's first
(the design's order: the prompt, then the kill line), then the newest of
the game's messages, oldest first. The game keeps up to four; the design
shows two lines */
static int view_lines(const struct n64_hud64_state *st, struct message *msgs, struct line *lines, int avail,
	const struct hud64_arrangement *ar, int keep)
{
	struct line more[HUD64_MAXIMUM_LINES];
	int n = 0, m, n_msgs = 0;

	keep = keep < HUD64_MAXIMUM_LINES ? keep : HUD64_MAXIMUM_LINES;

	if (st->prompt[0] || st->state_text[0])
	{
		if (st->prompt[0])
			prompt_message(&msgs[0], st, ar->prompt_first_form);
		else
			text_message(&msgs[0], st->state_text, 255);
		n = layout_messages(msgs, 1, avail, lines, keep);
		n_msgs = 1;
	}
	if (n >= keep)
		return n;
	for (int i = 0; i < st->message_count && i < N64_HUD64_MESSAGES; i++)
		text_message(&msgs[n_msgs + i], st->messages[i].text, st->messages[i].alpha);
	m = layout_messages(msgs + n_msgs, st->message_count < N64_HUD64_MESSAGES ? st->message_count : N64_HUD64_MESSAGES,
		avail, more, HUD64_MAXIMUM_LINES);
	for (int k = m > keep - n ? m - (keep - n) : 0; k < m; k++)
		lines[n++] = more[k];
	return n;
}

/* ---------- a view */

void n64_hud64_draw(const struct n64_hud64_state *st, int x0, int y0, int x1, int y1, int views)
{
	struct message msgs[N64_HUD64_MESSAGES + 1];
	struct line lines[HUD64_MAXIMUM_LINES];
	/* the player count's arrangement: which variant of each part */
	const struct hud64_arrangement *ar = &hud64_arrangements[views < 1 ? 1 : views > 4 ? 4 : views];
	const struct hud64_set *set;
	const struct hud64_layout *ly;
	const struct hud64_face *f;
	int W, H, inset = n64_hud_inset(), mx, my, L, Tp, R, B, n, box[4];

	current_mode(&draw.m);
	draw.rdp_mode = MODE_NONE;
	draw.prim_valid = draw.env_valid = 0;
	draw.tmem = NULL;
	draw.deferred_count = 0;
	draw.clip[0] = x0, draw.clip[1] = y0, draw.clip[2] = x1, draw.clip[3] = y1;
	set = draw.m.set;
	ly = &set->layout;
	f = face(FACE_MSG, 0);
	W = draw.m.canvas_w;
	H = draw.m.canvas_h;
	/* (the view in the canvas's pixels) */
	x0 /= draw.m.xs;
	x1 /= draw.m.xs;
	/* the design's rectangle: 3% from the screen's edges plus HUD INSET at
	the outer edges only, 5 pixels (240 lines) from the inner ones */
	mx = round_even(W * 0.03f + inset * W / 320.f);
	my = round_even(H * 0.03f + inset * H / 240.f);
	L = x0 == 0 ? mx : x0 + ly->gap;
	Tp = y0 == 0 ? my : y0 + ly->gap;
	R = x1 >= W ? W - mx : x1 - ly->gap;
	B = y1 >= H ? H - my : y1 - ly->gap;
	/* Halo's motion tracker (Halo draws it) */
	tracker_box(x0, y0, x1, y1, views, inset, box);
	rdpq_sync_pipe();
	if (!ar->split)
	{
		/* 1-2 players (compose_full): the score bottom right, the messages
		between Halo's tracker and the score; the design's range label for
		Halo's small one (the design's face reads better at 320x240), above
		the tracker */
		int sc_w, tx;
		static const char *const labels[3] = {"YOU", "1ST", "2ND"};

		if (st->show_ammo || st->show_grenades)
			ammo_full(st, L, Tp);
		if (st->show_shield)
			shield(st, R, Tp, 0);
		/* (not where the frame shows Halo's own -- label_halo) */
		if (st->show_sensor && ar->sensor_label && !n64_hud64_label_halo)
			range_label(box, views, x0, y0, x1, y1);
		/* (the messages keep their place without the score) */
		sc_w = word_width(f, "88", 2);
		if (ar->score_words)
		{
			int k = 0;

			for (int i = 0; i < 3; i++)
			{
				int w = word_width(f, labels[i], 3);
				k = w > k ? w : k;
			}
			sc_w += k + f->space;
		}
		if (st->show_score)
			score(st, R, B, ar->score_words);
		tx = box[2] + ly->full_msg_dx;
		n = view_lines(st, msgs, lines, R - sc_w - ly->full_msg_gap - tx, ar, ar->message_lines);
		message_lines(lines, n, tx, B);
	}
	else
	{
		/* 3-4 players: the tracker (Halo's) bottom left, as ORIGINAL places
		it, the score bottom right on the tracker's line, the messages
		between them up from that line -- as many lines as stay below the
		aim zone (the view's reticle box, 16% of its height, and 8 lines of
		240 under it: no message over the aim), at most the arrangement's
		(the tracker and the score kept low, where ORIGINAL has them) */
		int bottom = box[3], vy = (y0 + y1) / 2, clear = vy + (int)lroundf((y1 - y0) * 0.08f) + ly->aim_clear;
		int top = bottom - ly->msg_base_dy - f->cap - set->s, keep = 0, tx = box[2] + ly->full_msg_dx, sc_w;

		while (keep < ar->message_lines && top - keep * ly->pitch >= clear)
			keep++;
		if (st->show_ammo || st->show_grenades)
			ammo_split(st, L, Tp, R);
		if (st->show_shield)
			shield(st, R, Tp, 1);
		sc_w = word_width(f, "88", 2);
		if (st->show_score)
			score(st, R, bottom, ar->score_words);
		n = view_lines(st, msgs, lines, R - ly->score_dx - sc_w - ly->full_msg_gap - tx, ar, keep);
		message_lines(lines, n, tx, bottom);
	}
	flush_deferred();
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	/* (the game's HUD drawer keeps its last render state and TMEM load:
	v0.6.5's set_state; dropped, as what this view drew changed them) */
	n64_hud_state_lost();
}
