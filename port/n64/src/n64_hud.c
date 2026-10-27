/*
N64_HUD.C

The HUD's drawing (n64.h): the game's own HUD code (interface/hud*.c,
motion_sensor.c, game_engine.c) lays out its bitmaps, meters, numbers and
text; port/n64/game/n64_hud_game.c hands each quad here. The bitmaps are
half-size sprites on the cartridge (tools/n64/hud.py), loaded on first use;
with the HUD 640 across (high detail's, HUD RESOLUTION's) the full-width set
beside them where there is room (n64_hud_set_wide), drawn a texel a pixel
across.

An upright quad is a blit (rdpq handles bitmaps larger than TMEM by parts);
a rotated one is textured triangles. A meter's texture is made on the CPU,
only the part of the sprite the quad shows, and kept until its value or
colours change.

Halo's HUD code costs the N64 several milliseconds a view, so each view's
HUD is recorded (an rspq block) and the game draws it every other frame,
the views taking turns; in between the recording is shown again. Whatever
a recording uses stays in memory: the sprites until the next map, the
meters' textures in fixed slots. A recording is begun with room on the
heap, models dropped from the cache for it if need be; without it the view
is drawn as it goes, unrecorded, that frame.
*/

#include <libdragon.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "n64.h"

#define MAXIMUM_SPRITES 256
#define MAXIMUM_VIEWS 4
#define HUD_INTERVAL 2
#define METER_CACHE 12
/* a meter texture's slot: 128x32 RGBA16 (the parts of meters shown are
smaller) */
#define METER_SLOT_WIDTH 128
#define METER_SLOT_HEIGHT 32
#define BLIP_SIZE 16

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
/* uploads of a sprite's rectangle kept as recordings (below) */
#define UPLOAD_BLOCKS 48
/* the heap a view's recording is begun with (n64_render_make_room): its
commands come in pieces that double in size (rdpq's up to ~16 KB), and the
sprites and uploads it loads come out of it too */
#define RECORDING_ROOM_PIECE (18 * 1024)
#define RECORDING_ROOM_PIECES 3

/* the float helpers the quads need, inline: newlib's fminf and fmaxf asked
__fpclassifyf about every argument, and floorf and ceilf were calls */
static inline float min_f(float a, float b) { return a < b ? a : b; }
static inline float max_f(float a, float b) { return a > b ? a : b; }
static inline int floor_int(float x) { int i = (int)x; return (float)i > x ? i - 1 : i; }
static inline int ceil_int(float x) { int i = (int)x; return (float)i < x ? i + 1 : i; }

enum
{
	_mode_none,
	_mode_over,      /* colour * texture over the framebuffer */
	_mode_meter,     /* a meter's texture (opaque or cut out) */
	_mode_meter_glow, /* a meter's texture, partly over (it keeps most of the framebuffer) */
	_mode_blip,      /* motion sensor blips */
};

static struct
{
	char map_name[32];
	/* rom:/hud/<map>.list: 16 characters a sprite */
	char *names;
	int name_count;
	/* rom:/hud/<map>-wide.list, the full-width set (the same numbers),
	read when first asked for; the memory it all loads into (its first
	line); whether it is drawn from (n64_hud_set_wide) */
	char *wide_names;
	int wide_name_count;
	unsigned long wide_bytes;
	int wide;
	sprite_t *sprites[MAXIMUM_SPRITES];
	int missing[MAXIMUM_SPRITES];
	/* a loaded sprite's set: full width (twice the half-size set's texels
	across) */
	unsigned char sprite_wide[MAXIMUM_SPRITES];
	int x0, y0, x1, y1;
	int view_count;
	int mode;
	surface_t blip;
	rspq_block_t *recordings[MAXIMUM_VIEWS];
	unsigned long view_frames[MAXIMUM_VIEWS];
	int view;      /* the view begun */
	int recording; /* the view being recorded, or -1 */
	int live;      /* the view drawn as it goes, unrecorded (no room for a recording) */
	/* statistics: microseconds in the HUD, its quads and meters, every 240
	views; one view's quads are logged at view 600 */
	unsigned long begin_ticks, ticks, quad_ticks, meter_ticks, quads, views;
	int dump;
	/* what the last sprite quad left in TMEM (tile 0): its pixels and
	rectangle; NULL when anything else has been loaded since */
	const void *tmem_pixels;
	int tmem_s0, tmem_t0, tmem_s1, tmem_t1;
	/* statistics: quads drawn by the fast path, uploads it skipped */
	unsigned long fast_quads, skipped_uploads;
	/* the texture filter set: point sampling for the quads drawn at their
	sprite's own size, bilinear for the rest */
	int point;
	/* n64_hud_inset; -1 before it is first asked for */
	int inset;
} hud = {.inset = -1};

/* A quad's texture upload was rdpq_tex_upload_sub's work on the CPU every
time the HUD was recorded: the texture loader's plans and a dozen commands.
Each sprite rectangle uploaded is kept as a recording of its own (made once
the HUD's recording is done: blocks don't nest while recording) and called
from then on -- the same commands. They stay until the map ends
(n64_hud_release), with the HUD recordings that call them. */
static struct upload_block
{
	surface_t surface;
	int s0, t0, s1, t1;
	rspq_block_t *block; /* NULL until recorded */
} uploads[UPLOAD_BLOCKS];
static int upload_count;

static struct meter_entry
{
	int sprite;
	int s0, t0, width, height, step;
	unsigned long minimum_color, maximum_color, flash_color, background_color, tint_color;
	surface_t slot, surface;
	unsigned long last_used;
} meters[METER_CACHE];
static unsigned long meter_clock;

/* a soft round dot (Halo's blip bitmap is one), intensity 8 bits */
static void make_blip(void)
{
	hud.blip = surface_alloc(FMT_I8, BLIP_SIZE, BLIP_SIZE);
	for (int y = 0; y < BLIP_SIZE; y++)
	{
		uint8_t *row = (uint8_t *)hud.blip.buffer + y * hud.blip.stride;

		for (int x = 0; x < BLIP_SIZE; x++)
		{
			float dx = (x + 0.5f) / (BLIP_SIZE / 2) - 1.f, dy = (y + 0.5f) / (BLIP_SIZE / 2) - 1.f;
			float d = 1.f - sqrtf(dx * dx + dy * dy);

			row[x] = d <= 0.f ? 0 : d >= 0.5f ? 255 : (uint8_t)(d * 510.f);
		}
	}
	data_cache_hit_writeback(hud.blip.buffer, hud.blip.stride * BLIP_SIZE);
}

/* the HUD's own font (Halo's small one, for its text) */
static struct n64_font *hud_font;
static int hud_font_loaded;

/* what the HUD keeps from its first map on -- the meters' texture slots
(96 KB), the blip, its font -- made at boot (n64_main.c), at the heap's
bottom: made with the first map, above its image, they split the heap a
later, bigger map needs in one piece (n64_anim.c) */
void n64_hud_reserve(void)
{
	for (int i = 0; i < METER_CACHE; i++)
	{
		if (!meters[i].slot.buffer)
			meters[i].slot = surface_alloc(FMT_RGBA16, METER_SLOT_WIDTH, METER_SLOT_HEIGHT);
		meters[i].sprite = -1;
	}
	if (!hud.blip.buffer)
		make_blip();
	if (!hud_font_loaded)
	{
		hud_font = n64_font_load("small");
		hud_font_loaded = 1;
	}
}

/* what the HUD made for its map -- the sprites, the recordings and their
uploads, the sprites' names -- given back (the slots, the blip and the font
stay): at the end of a game (n64_game_free_map), so that none of it is left
above the memory the next map's image wants in one piece, and before a new
map's. The RSP and the RDP done with them first, their recordings freed
before it returns */
static void release_sprites(void)
{
	rspq_wait();
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (hud.sprites[i])
			sprite_free(hud.sprites[i]);
		hud.sprites[i] = NULL;
		hud.missing[i] = 0;
		hud.sprite_wide[i] = 0;
	}
	for (int i = 0; i < MAXIMUM_VIEWS; i++)
	{
		if (hud.recordings[i])
			rdpq_call_deferred((void (*)(void *))rspq_block_free, hud.recordings[i]);
		hud.recordings[i] = NULL;
	}
	for (int i = 0; i < upload_count; i++)
	{
		if (uploads[i].block)
			rdpq_call_deferred((void (*)(void *))rspq_block_free, uploads[i].block);
	}
	upload_count = 0;
	for (int i = 0; i < METER_CACHE; i++)
	{
		surface_t slot = meters[i].slot;

		memset(&meters[i], 0, sizeof(meters[i]));
		meters[i].sprite = -1;
		meters[i].slot = slot;
	}
	hud.tmem_pixels = NULL;
	/* (the deferred frees made) */
	rspq_wait();
}

void n64_hud_release(void)
{
	release_sprites();
	free(hud.names);
	hud.names = NULL;
	hud.name_count = 0;
	free(hud.wide_names);
	hud.wide_names = NULL;
	hud.wide_name_count = 0;
	hud.wide_bytes = 0;
	hud.wide = 0;
}

/* a list of sprite names (16 characters a line) read into names; the
count. A first line "#<bytes>" (the full-width set's) into bytes */
static int read_list(const char *path, char **names, unsigned long *bytes)
{
	char line[32];
	int count = 0;
	FILE *file = fopen(path, "r");

	*names = NULL;
	if (!file)
		return 0;
	*names = malloc(MAXIMUM_SPRITES * 16);
	while (*names && count < MAXIMUM_SPRITES && fgets(line, sizeof(line), file))
	{
		if (line[0] == '#' && bytes)
			*bytes = strtoul(line + 1, NULL, 10);
		else if (strlen(line) >= 16)
			memcpy(*names + 16 * count++, line, 16);
	}
	fclose(file);
	return count;
}

void n64_hud_set_map(const char *map_name)
{
	n64_hud_release();
	for (int i = 0; i < METER_CACHE; i++)
	{
		if (!meters[i].slot.buffer)
			meters[i].slot = surface_alloc(FMT_RGBA16, METER_SLOT_WIDTH, METER_SLOT_HEIGHT);
	}
	if (!hud.blip.buffer)
		make_blip();
	snprintf(hud.map_name, sizeof(hud.map_name), "%s", map_name);
	/* the sprites' names, in number order (tools/n64/hud.py) */
	{
		char path[64];

		snprintf(path, sizeof(path), "rom:/hud/%s.list", map_name);
		hud.name_count = read_list(path, &hud.names, NULL);
	}
}

/* The HUD's sprites for a display 640 across (on): the full-width set,
where the map has one and room is at least the memory all of it loads into
(the caller's room check, as the display's: the heap less the models'
room), else the half-size set as at 320 across. The sprites loaded so far,
and all made from them, are given back when the set changes */
void n64_hud_set_wide(int on, unsigned long room)
{
	if (on && !hud.wide_names && hud.name_count)
	{
		char path[64];

		snprintf(path, sizeof(path), "rom:/hud/%s-wide.list", hud.map_name);
		hud.wide_name_count = read_list(path, &hud.wide_names, &hud.wide_bytes);
	}
	on = on && hud.wide_name_count == hud.name_count && hud.wide_bytes && hud.wide_bytes <= room;
	if (!on && hud.wide_names)
	{
		free(hud.wide_names);
		hud.wide_names = NULL;
		hud.wide_name_count = 0;
		hud.wide_bytes = 0;
	}
	if (on == hud.wide)
		return;
	release_sprites();
	hud.wide = on;
	debugf("n64: HUD sprites %s (%lu KB of room)\n", on ? "full width" : "half size", room / 1024);
}

/* the memory a sprite file loads into: a compressed asset's header
(libdragon's "DCA5": its flags, then the compressed size, the original size
and the in-place margin as LEB128 varints) gives it, else the file's size */
static unsigned long sprite_bytes(FILE *file)
{
	uint8_t header[16];
	unsigned long fields[3] = {0, 0, 0};

	if (fread(header, 1, sizeof(header), file) == sizeof(header) && !memcmp(header, "DCA", 3))
	{
		const uint8_t *p = header + 5;

		for (int f = 0; f < 3; f++)
		{
			for (int shift = 0; p < header + sizeof(header); shift += 7)
			{
				uint8_t b = *p++;

				fields[f] |= (unsigned long)(b & 0x7F) << shift;
				if (!(b & 0x80))
					break;
			}
		}
		return fields[1] + fields[2];
	}
	fseek(file, 0, SEEK_END);
	return (unsigned long)ftell(file);
}

static sprite_t *hud_sprite(int number)
{
	char path[64];

	if (number < 0 || number >= MAXIMUM_SPRITES || hud.missing[number])
		return NULL;
	if (!hud.sprites[number])
	{
		int found = 0;

		/* the full-width set's sprite where it is in use, else -- or
		without room for it -- the half-size set's */
		for (int wide = hud.wide; wide >= 0 && !hud.sprites[number]; wide--)
		{
			const char *names = wide ? hud.wide_names : hud.names;
			FILE *file;
			int room;

			if (!names || number >= (wide ? hud.wide_name_count : hud.name_count))
				continue;
			snprintf(path, sizeof(path), "rom:/hud/%.16s.sprite", names + number * 16);
			file = fopen(path, "rb");
			if (!file)
				continue;
			found = 1;
			/* (asset_load doesn't check its allocation: the sprite's room
			is looked for first, its quads left out until there is some --
			and the room a recording began with kept for it: its chunks are
			made as it goes) */
			room = n64_heap_has(sprite_bytes(file) + 1024 +
				(hud.recording >= 0 ? RECORDING_ROOM_PIECE * RECORDING_ROOM_PIECES : 0));
			fclose(file);
			if (!room)
				continue;
			hud.sprites[number] = sprite_load(path);
			hud.sprite_wide[number] = wide && hud.sprites[number];
		}
		if (!found)
		{
			hud.missing[number] = 1;
			return NULL;
		}
		if (hud.sprites[number] && n64_debug_flag("debug_hud_sprites"))
			debugf("hud sprite %d: %dx%d format %d%s\n", number, hud.sprites[number]->width,
				hud.sprites[number]->height, (int)sprite_get_format(hud.sprites[number]),
				hud.sprite_wide[number] ? " (full width)" : "");
	}
	return hud.sprites[number];
}

/* Acquire through the existing per-map shared cache. A successful
   preflight and subsequent draw reuse one sprite allocation and owner. */
int n64_hud_ensure_sprite(int number)
{
	return hud_sprite(number) != NULL;
}

void n64_render_hud_phase(void);

/* the views' recordings dropped (the display changed size: high detail), each
recorded again the next time its view is drawn */
void n64_hud_forget_recordings(void)
{
	n64_render_park_rdp_output();
	for (int i = 0; i < MAXIMUM_VIEWS; i++)
	{
		if (hud.recordings[i])
			rdpq_call_deferred((void (*)(void *))rspq_block_free, hud.recordings[i]);
		hud.recordings[i] = NULL;
	}
}

void n64_hud_set_view_count(int count)
{
	hud.view_count = count;
}

void n64_hud_set_inset(int pixels)
{
	hud.inset = pixels < 0 ? 0 : pixels;
}

int n64_hud_inset(void)
{
	if (hud.inset < 0)
	{
		char text[16];

		hud.inset = n64_debug_text("debug_hud_inset", text, sizeof(text)) ? atoi(text) : 0;
		if (hud.inset < 0)
			hud.inset = 0;
	}
	return hud.inset;
}

int n64_hud_begin(int view, int x0, int y0, int x1, int y1)
{
	n64_render_hud_phase();
	/* (the view laid out in the 3D's pixels, its HUD in the display's) */
	n64_render_hud_rect(&x0, &y0, &x1, &y1);
	hud.x0 = x0;
	hud.y0 = y0;
	hud.x1 = x1;
	hud.y1 = y1;
	hud.mode = _mode_none;
	hud.tmem_pixels = NULL;
	hud.begin_ticks = TICKS_READ();
	hud.recording = -1;
	hud.live = 0;
	hud.view = view;
	if (view < 0 || view >= MAXIMUM_VIEWS)
		return 0;
	/* with three or four views, each view's every sixth frame (the user's
	choice: the HUD's code is ~5 ms a recording, and four views made one a
	frame), the views' turns spread over the six (four views: frames 0, 1,
	3 and 4 of each six, not four in a row); with two, each view's every
	third */
	{
		int interval = n64_golden_selected(view) ? 1 : hud.view_count > 2 ? 6 : hud.view_count == 2 ? 3 : HUD_INTERVAL;
		int turn = hud.view_count > 0 ? view * interval / hud.view_count : view;

		if (hud.recordings[view] && (hud.view_frames[view]++ + turn) % interval)
			return 0;
	}
	/* the view's last recording let go first (its memory back once the RSP
	is past it), then room made for the new one: the model cache fills the
	heap down to its margin, and a recording that ran out of memory halted
	the game ("Out of memory" in __rdpq_block_next_buffer). Without room the
	view's HUD is drawn as it goes this time, unrecorded (and then nothing
	would move the RSP's RDP output off the old recording: parked) */
	if (hud.recordings[view])
	{
		n64_render_park_rdp_output();
		rdpq_call_deferred((void (*)(void *))rspq_block_free, hud.recordings[view]);
		hud.recordings[view] = NULL;
	}
	hud.dump = hud.views == 600 || hud.views == 601;
	n64_profile_zone = 1;
	if (n64_render_make_room(RECORDING_ROOM_PIECE, RECORDING_ROOM_PIECES))
	{
		hud.recording = view;
		rspq_block_begin();
	}
	else
		hud.live = 1;
	rdpq_sync_pipe();
	rdpq_set_scissor(x0, y0, x1, y1);
	return 1;
}

void n64_hud_end(void)
{
	n64_profile_zone = 0;
	if (hud.recording >= 0)
	{
		rspq_block_t *recording = rspq_block_end();

		/* the last recording may still be in the RSP's queue */
		if (hud.recordings[hud.recording])
			rdpq_call_deferred((void (*)(void *))rspq_block_free, hud.recordings[hud.recording]);
		hud.recordings[hud.recording] = recording;
		rspq_block_run(recording);
		hud.recording = -1;
		/* the uploads this recording made directly, recorded for the next
		(while the heap has room for one: else at a later recording) */
		for (int i = 0; i < upload_count; i++)
		{
			if (!uploads[i].block)
			{
				if (!n64_heap_has(2048))
					break;
				rspq_block_begin();
				rdpq_tex_upload_sub(TILE0, &uploads[i].surface, NULL, uploads[i].s0, uploads[i].t0, uploads[i].s1, uploads[i].t1);
				uploads[i].block = rspq_block_end();
			}
		}
		hud.tmem_pixels = NULL;
	}
	else if (hud.live)
	{
		/* (drawn already; recorded at the view's next frame, room allowing) */
		hud.live = 0;
		hud.tmem_pixels = NULL;
	}
	else if (hud.view >= 0 && hud.view < MAXIMUM_VIEWS && hud.recordings[hud.view])
		rspq_block_run(hud.recordings[hud.view]);
	hud.ticks += TICKS_READ() - hud.begin_ticks;
	if (!(++hud.views % 240))
	{
		extern unsigned long n64_emulated_count;
		static unsigned long emulated;

		if (n64_debug_flag("debug_stats"))
		{
			debugf("hud: %lu fast quads, %lu uploads skipped\n", hud.fast_quads, hud.skipped_uploads);
			debugf("hud: %lu us per view (quads %lu, meters %lu), %lu quads; %lu emulated instructions per view\n",
				TICKS_TO_US(hud.ticks) / 240, TICKS_TO_US(hud.quad_ticks) / 240, TICKS_TO_US(hud.meter_ticks) / 240,
				hud.quads / 240, (n64_emulated_count - emulated) / 240);
		}
		hud.fast_quads = hud.skipped_uploads = 0;
		emulated = n64_emulated_count;
		hud.ticks = hud.quad_ticks = hud.meter_ticks = hud.quads = 0;
	}
	n64_render_hud_end();
}

static unsigned channel(unsigned long color, int shift)
{
	return (color >> shift) & 0xFF;
}

/* (the mode's changes one batch, rdpq_mode_begin's: the RSP works the render
mode out once, at the end, where each change did it again, every time a
recording is shown -- the same mode in the end) */
static void set_mode(int mode)
{
	if (hud.mode == mode)
		return;
	rdpq_sync_pipe();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_filter(hud.point ? FILTER_POINT : FILTER_BILINEAR);
	switch (mode)
	{
	case _mode_over:
		rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, 0, PRIM, 0), (TEX0, 0, PRIM, 0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		break;
	case _mode_meter:
		rdpq_mode_combiner(RDPQ_COMBINER_TEX);
		rdpq_mode_alphacompare(1);
		break;
	case _mode_meter_glow:
		rdpq_mode_combiner(RDPQ_COMBINER_TEX);
		rdpq_mode_alphacompare(1);
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY_CONST);
		break;
	case _mode_blip:
		/* the dot's intensity times the blip's as alpha */
		rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		break;
	}
	rdpq_mode_end();
	if (mode == _mode_meter_glow)
		rdpq_set_fog_color(RGBA32(0, 0, 0, 0xB0));
	hud.mode = mode;
}

/* point sampling for a quad drawn at its sprite's own size: the HUD's pixels
as they are (bilinear filtering at 1:1 blurred them by half a texel and mixed
in the transparent texels around each shape: dark fringes); bilinear for a
scaled or rotated one */
static void set_filter(int point)
{
	if (hud.point == point)
		return;
	hud.point = point;
	if (hud.mode != _mode_none)
	{
		rdpq_sync_pipe();
		rdpq_mode_filter(point ? FILTER_POINT : FILTER_BILINEAR);
	}
}

/* a quad's mode and filter: a new mode is set with the filter the quad wants
(set after the mode, a filter that changed as well was a second pipe sync and
render mode change, nothing drawn in between) */
static __attribute__((noinline)) void set_state(int mode, int point)
{
	if (hud.mode == mode)
	{
		set_filter(point);
		return;
	}
	hud.point = point;
	set_mode(mode);
}

/* a colour Halo adds (colour * texture + framebuffer * (1 - alpha * texture
alpha)) as one the RDP blends over the framebuffer (its additive blending
wraps around): mostly opaque colours keep their alpha (the colour brightened
by 1 / alpha, so that colour * alpha is Halo's); mostly transparent ones, a
glow, take their brightest channel as alpha (the colour brightened to full) */
static color_t over_color(unsigned long argb)
{
	unsigned a = argb >> 24, r = channel(argb, 16), g = channel(argb, 8), b = channel(argb, 0);

	if (a < 0x80)
	{
		a = r > g ? r : g;
		a = a > b ? a : b;
	}
	if (!a)
		return RGBA32(0, 0, 0, 0);
	return RGBA32(MIN(255u, r * 255 / a), MIN(255u, g * 255 / a), MIN(255u, b * 255 / a), a);
}

/* the part of a meter's sprite a quad shows, as Halo's meter shader colours
it (n64.h), in RGBA16. Halo adds it to the framebuffer times the tint's
alpha (full texels) or the background's (empty ones): a meter keeping half
or more of the framebuffer is drawn partly over it, the rest opaque; an
empty part keeping half or more is cut out, as is the outside of the shape.
RGBA16 has one bit of alpha, and Halo's shape is the meter's opacity as well
as its outline (the shield's ~0.4-0.7): a texel of at least half the meter's
own strongest is drawn, at least three quarters as bright as that, the rest
cut out (Halo's colour times the shape, opaque, drew the faint edges as dark
rims) but coloured all the same, for the bilinear filtering of a scaled
meter */
/* (n64_div255: a meter's texture made again -- its level changed -- has
nine of them a texel) */
#define div255 n64_div255

static surface_t *meter_surface(const struct n64_hud_quad *quad, sprite_t *sprite, int s0, int t0, int width, int height,
	int step)
{
	struct meter_entry *entry, *oldest = &meters[0];
	surface_t source = sprite_get_pixels(sprite);
	unsigned minimum_alpha = quad->minimum_color >> 24, maximum_alpha = quad->flash_color >> 24;
	unsigned scale = channel(quad->tint_color, 0);
	unsigned empty_alpha = quad->background_color >> 24;
	unsigned strongest = 0;
	uint16_t empty;

	meter_clock++;
	for (int i = 0; i < METER_CACHE; i++)
	{
		struct meter_entry *m = &meters[i];

		if (m->sprite == quad->sprite && m->s0 == s0 && m->t0 == t0 && m->width == width && m->height == height &&
			m->step == step &&
			m->minimum_color == quad->minimum_color && m->maximum_color == quad->maximum_color &&
			m->flash_color == quad->flash_color && m->background_color == quad->background_color &&
			m->tint_color == quad->tint_color)
		{
			m->last_used = meter_clock;
			return &m->surface;
		}
		if (m->last_used < oldest->last_used)
			oldest = m;
	}
	entry = oldest;
	if (!entry->slot.buffer)
		return NULL;
	width = MIN(width, METER_SLOT_WIDTH);
	height = MIN(height, METER_SLOT_HEIGHT);
	entry->surface = surface_make_sub(&entry->slot, 0, 0, width, height);
	entry->sprite = quad->sprite;
	entry->s0 = s0;
	entry->t0 = t0;
	entry->width = width;
	entry->height = height;
	entry->step = step;
	entry->minimum_color = quad->minimum_color;
	entry->maximum_color = quad->maximum_color;
	entry->flash_color = quad->flash_color;
	entry->background_color = quad->background_color;
	entry->tint_color = quad->tint_color;
	entry->last_used = meter_clock;
	/* the empty part: the background where it covers (its alpha is what
	the framebuffer keeps), else nothing */
	if (empty_alpha < 0x80)
	{
		unsigned r = div255(channel(quad->background_color, 16) * scale);
		unsigned g = div255(channel(quad->background_color, 8) * scale);
		unsigned b = div255(channel(quad->background_color, 0) * scale);

		empty = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1;
	}
	else
		empty = 0;
	for (int y = 0; y < height; y++)
	{
		const uint8_t *in = (const uint8_t *)source.buffer + (t0 + y) * source.stride + s0 * 2;

		for (int x = 0; x < width; x++)
			strongest = in[x * step * 2 + 1] > strongest ? in[x * step * 2 + 1] : strongest;
	}
	/* A texel's colour depends on its order and shape alone (the quad's
	colours, the strongest and the scale are the meter's): a texel the same
	as the one before it, or the one above it, takes that one's colour, and a
	meter's texels are runs of a few (a bar's order changes along it, its
	shape at its edges) -- worked out once each, not nine divisions a texel.
	A row is made in cached memory, then copied into the slot (uncached)
	whole words at a time, its width exactly */
	{
		typedef uint16_t __attribute__((may_alias)) texel_pair;
		uint16_t rows[2][METER_SLOT_WIDTH] __attribute__((aligned(8)));
		const texel_pair *above = NULL;

		for (int y = 0; y < height; y++)
		{
			const uint8_t *in = (const uint8_t *)source.buffer + (t0 + y) * source.stride + s0 * 2;
			const texel_pair *pairs = (const texel_pair *)in;
			uint16_t *made = rows[y & 1], *made_above = rows[(y & 1) ^ 1];
			uint16_t *out = (uint16_t *)((uint8_t *)entry->surface.buffer + y * entry->surface.stride);
			unsigned last = 0x10000, last_made = 0;
			int x;

			for (x = 0; x < width; x++)
			{
				unsigned pair = pairs[x * step];

				if (pair == last)
				{
					made[x] = last_made;
					continue;
				}
				last = pair;
				if (above && above[x * step] == pair)
				{
					made[x] = last_made = made_above[x];
					continue;
				}
				{
					unsigned order = in[x * step * 2], shape = in[x * step * 2 + 1];
					int covered = shape && shape * 2 >= strongest;
					unsigned k = div255((shape * 4 >= strongest * 3 ? shape : strongest * 3 / 4) * scale);

					if (covered && order >= maximum_alpha)
						made[x] = empty;
					else
					{
						unsigned r = div255(channel(quad->minimum_color, 16) * (255 - order) + channel(quad->maximum_color, 16) * order);
						unsigned g = div255(channel(quad->minimum_color, 8) * (255 - order) + channel(quad->maximum_color, 8) * order);
						unsigned b = div255(channel(quad->minimum_color, 0) * (255 - order) + channel(quad->maximum_color, 0) * order);

						if (order >= minimum_alpha)
						{
							r = MIN(255u, r + channel(quad->flash_color, 16));
							g = MIN(255u, g + channel(quad->flash_color, 8));
							b = MIN(255u, b + channel(quad->flash_color, 0));
						}
						r = div255(r * k);
						g = div255(g * k);
						b = div255(b * k);
						made[x] = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | (covered && order < maximum_alpha);
					}
					last_made = made[x];
				}
			}
			memcpy(out, made, width * sizeof(*made));
			above = pairs;
		}
	}
	data_cache_hit_writeback(entry->surface.buffer, entry->surface.stride * entry->surface.height);
	return &entry->surface;
}

/* a rotated quad: textured triangles, the bitmap loaded into TMEM in strips
of rows (with one row more for the filtering), each strip drawn as the part
of the quad its rows cover (the quad maps the bitmap affinely) */
static void draw_rotated(const struct n64_hud_quad *quad, const surface_t *surface)
{
	tex_format_t format = surface_get_format(surface);
	int row_bytes = (TEX_FORMAT_PIX2BYTES(format, surface->width) + 7) & ~7;
	int strip = 4096 / row_bytes - 1;
	float ds = quad->s[1] - quad->s[0], dt = quad->t[3] - quad->t[0];
	float t_low = min_f(quad->t[0], quad->t[3]), t_high = max_f(quad->t[0], quad->t[3]);

	if (strip < 1 || fabsf(dt) < 1e-3f || fabsf(ds) < 1e-3f)
		return;
	for (int t0 = floor_int(max_f(t_low, 0)); t0 < ceil_int(min_f(t_high, surface->height)); t0 += strip)
	{
		int t1 = MIN(t0 + strip, surface->height);
		/* the strip's rows as fractions along the quad's left edge (v0 to v3) */
		float a = (max_f(t0, t_low) - quad->t[0]) / dt, b = (min_f(t1, t_high) - quad->t[0]) / dt;
		float v[4][5];

		rdpq_tex_upload_sub(TILE0, surface, NULL, 0, t0, surface->width, MIN(t1 + 1, surface->height));
		for (int i = 0; i < 4; i++)
		{
			/* corners 0 and 3 on the left edge (v0 to v3), 1 and 2 on the
			right (v1 to v2); 0 and 1 at fraction a, 2 and 3 at b */
			float f = i < 2 ? a : b;
			int from = i == 0 || i == 3 ? 0 : 1, to = from ? 2 : 3;

			v[i][0] = quad->x[from] + (quad->x[to] - quad->x[from]) * f;
			v[i][1] = quad->y[from] + (quad->y[to] - quad->y[from]) * f;
			v[i][2] = quad->s[from] + (quad->s[to] - quad->s[from]) * f;
			v[i][3] = quad->t[from] + (quad->t[to] - quad->t[from]) * f;
			v[i][4] = 1.0f;
		}
		rdpq_triangle(&TRIFMT_TEX, v[0], v[1], v[2]);
		rdpq_triangle(&TRIFMT_TEX, v[0], v[2], v[3]);
	}
}

void n64_hud_quad(const struct n64_hud_quad *in)
{
	sprite_t *sprite = hud_sprite(in->sprite);
	unsigned long start = TICKS_READ();
	const struct n64_hud_quad *quad = in;
	struct n64_hud_quad full;
	surface_t surface;
	float s0, t0, s1, t1;
	int is0, it0, is1, it1;
	int upright, drawn_upright, wide = sprite && hud.sprite_wide[in->sprite];
	int mode = _mode_over, point = hud.point;
	float scale_x = 0.f, scale_y = 0.f;

	/* (the game addresses the half-size set's texels; the full-width set
	has twice as many across) */
	if (wide)
	{
		full = *in;
		for (int i = 0; i < 4; i++)
			full.s[i] *= 2.f;
		quad = &full;
	}

	hud.quads++;
	if (hud.dump)
		debugf("hud quad %d: %d,%d %d,%d %d,%d %d,%d st %d,%d-%d,%d color %08lx meter %d %08lx %08lx %08lx %08lx %08lx%s\n",
			quad->sprite, (int)quad->x[0], (int)quad->y[0], (int)quad->x[1], (int)quad->y[1],
			(int)quad->x[2], (int)quad->y[2], (int)quad->x[3], (int)quad->y[3],
			(int)quad->s[0], (int)quad->t[0], (int)quad->s[2], (int)quad->t[2], quad->color, quad->meter,
			quad->minimum_color, quad->maximum_color, quad->flash_color, quad->background_color, quad->tint_color,
			sprite ? "" : " (missing)");
	if (!sprite)
		return;
	surface = sprite_get_pixels(sprite);

	/* v0 top left, v1 top right, v2 bottom right, v3 bottom left */
	upright = fabsf(quad->y[0] - quad->y[1]) < 0.5f && fabsf(quad->x[1] - quad->x[2]) < 0.5f &&
		quad->x[1] > quad->x[0] && quad->y[2] > quad->y[1];
	s0 = min_f(quad->s[0], quad->s[2]);
	s1 = max_f(quad->s[0], quad->s[2]);
	t0 = min_f(quad->t[0], quad->t[2]);
	t1 = max_f(quad->t[0], quad->t[2]);
	is0 = floor_int(max_f(s0, 0));
	it0 = floor_int(max_f(t0, 0));
	is1 = ceil_int(min_f(s1, surface.width));
	it1 = ceil_int(min_f(t1, surface.height));
	if (is1 <= is0 || it1 <= it0)
		return;
	if (quad->meter)
	{
		unsigned long meter_start = TICKS_READ();

		/* the meter's own texture holds just the part shown (a full-width
		part wider than a slot: every other column, the half-size set's) */
		int step = is1 - is0 > METER_SLOT_WIDTH ? 2 : 1;
		surface_t *meter = meter_surface(quad, sprite, is0, it0, (is1 - is0) / step, it1 - it0, step);

		if (!meter)
			return;
		surface = *meter;
		hud.meter_ticks += TICKS_READ() - meter_start;
		s0 = (s0 - is0) / step;
		s1 = (s1 - is0) / step;
		t0 -= it0;
		t1 -= it0;
		is1 = MIN(is1 - is0, surface.width);
		it1 = MIN(it1 - it0, surface.height);
		is0 = it0 = 0;
		mode = quad->tint_color >> 24 >= 0x80 ? _mode_meter_glow : _mode_meter;
	}

	/* the filter the quad wants, set with its mode (set_state); a rotated
	quad's bilinear, a meter not drawn keeping the one in force. (Point
	sampled at the screen's own multiple of the sprites' size: the
	half-size set's 1 at 320x240, 2 at 640x480; the full-width set's 1
	across at 640 -- whole texels to whole pixels) */
	drawn_upright = upright && s1 > s0 && t1 > t0;
	if (drawn_upright)
	{
		float native_x = n64_render_layout_scale_x() * (wide ? 1.f : 2.f);
		float native_y = n64_render_layout_scale_y() * 2.f;

		scale_x = (quad->x[1] - quad->x[0]) / (s1 - s0);
		scale_y = (quad->y[2] - quad->y[1]) / (t1 - t0);
		point = native_x == (float)(int)native_x &&
			fabsf(scale_x - native_x) < 0.02f && fabsf(scale_y - native_y) < 0.02f;
	}
	else if (!quad->meter)
		point = 0;
	set_state(mode, point);
	if (!quad->meter)
		rdpq_set_prim_color(over_color(quad->color));

	if (drawn_upright)
	{
		int width = is1 - is0, height = it1 - it0;
		int flip_x = quad->s[0] > quad->s[2], flip_y = quad->t[0] > quad->t[2];
		int row_bytes = (TEX_FORMAT_PIX2BYTES(surface_get_format(&surface), width) + 7) & ~7;

		/* a rectangle of the sprite that fits TMEM, unflipped: one upload
		(none when the last quad's is still there) and one scaled rectangle
		-- rdpq_tex_blit planned its strips on the CPU for every quad */
		if (!flip_x && !flip_y && !quad->meter && row_bytes * height <= 4096)
		{
			if (hud.tmem_pixels != surface.buffer || hud.tmem_s0 != is0 || hud.tmem_t0 != it0 ||
				hud.tmem_s1 != is1 || hud.tmem_t1 != it1)
			{
				struct upload_block *upload = NULL;

				for (int i = 0; i < upload_count; i++)
				{
					if (uploads[i].surface.buffer == surface.buffer && uploads[i].s0 == is0 && uploads[i].t0 == it0 &&
						uploads[i].s1 == is1 && uploads[i].t1 == it1)
					{
						upload = &uploads[i];
						break;
					}
				}
				if (upload && upload->block)
					rspq_block_run(upload->block);
				else
				{
					rdpq_tex_upload_sub(TILE0, &surface, NULL, is0, it0, is1, it1);
					if (!upload && upload_count < UPLOAD_BLOCKS && hud.recording >= 0)
					{
						upload = &uploads[upload_count++];
						upload->surface = surface;
						upload->s0 = is0;
						upload->t0 = it0;
						upload->s1 = is1;
						upload->t1 = it1;
						upload->block = NULL;
					}
				}
				hud.tmem_pixels = surface.buffer;
				hud.tmem_s0 = is0;
				hud.tmem_t0 = it0;
				hud.tmem_s1 = is1;
				hud.tmem_t1 = it1;
			}
			else
				hud.skipped_uploads++;
			rdpq_texture_rectangle_scaled(TILE0, quad->x[0], quad->y[0],
				quad->x[0] + width * scale_x, quad->y[0] + height * scale_y, is0, it0, is1, it1);
			hud.fast_quads++;
		}
		else
		{
			hud.tmem_pixels = NULL;
			rdpq_tex_blit(&surface, quad->x[0], quad->y[0], &(rdpq_blitparms_t){
				.s0 = is0, .t0 = it0, .width = width, .height = height,
				.scale_x = scale_x, .scale_y = scale_y, .flip_x = flip_x, .flip_y = flip_y,
			});
		}
	}
	else if (!quad->meter)
	{
		hud.tmem_pixels = NULL;
		draw_rotated(quad, &surface);
	}
	hud.quad_ticks += TICKS_READ() - start;
}

void n64_hud_text(int x, int y, unsigned long color, const char *text)
{
	struct n64_font *font;

	if (hud.dump)
		debugf("hud text %d,%d: %s\n", x, y, text);
	hud.mode = _mode_none;
	hud.tmem_pixels = NULL;
	/* Halo's small font (the menu's), from the top of the text's box */
	if (!hud_font_loaded)
	{
		hud_font = n64_font_load("small");
		hud_font_loaded = 1;
	}
	font = hud_font;
	if (!font)
	{
		rdpq_sync_pipe();
		rdpq_text_print(&(rdpq_textparms_t){0}, FONT_BUILTIN_DEBUG_MONO, x, y + 8, text);
		return;
	}
	/* (squeezed across with the anamorphic picture, as the rest of the HUD) */
	n64_text_draw_scaled(font, (float)x, y + n64_font_ascent(font), color, text, n64_render_hud_squeeze());
}

/* The same small-font advance used by the existing HUD text renderer.
   Call after n64_hud_text so its existing lazy font load has completed. */
float n64_hud_text_width(const char *text)
{
	return hud_font ? n64_text_width(hud_font, text) * n64_render_hud_squeeze() :
		(int)strlen(text) * 8;
}

int n64_hud_text_height(void)
{
	return hud_font ? n64_font_height(hud_font) : 8;
}

void n64_hud_blip(float x, float y, float width, float height, unsigned long color)
{
	float scale_x = width / BLIP_SIZE, scale_y = height / BLIP_SIZE;

	if (hud.dump)
		debugf("hud blip %d,%d size %d color %08lx\n", (int)x, (int)y, (int)width, color);
	if (!(color & 0xF0F0F0) || !hud.blip.buffer)
		return;
	set_state(_mode_blip, 0);
	rdpq_set_prim_color(over_color(color & 0xFFFFFF));
	/* what rdpq_tex_blit drew (its one strip: the dot's upload, then the
	scaled rectangle), the dot uploaded once for all the blips in a row */
	if (hud.tmem_pixels != hud.blip.buffer)
	{
		rdpq_tex_upload(TILE0, &hud.blip, NULL);
		hud.tmem_pixels = hud.blip.buffer;
		hud.tmem_s0 = hud.tmem_t0 = 0;
		hud.tmem_s1 = hud.tmem_t1 = BLIP_SIZE;
	}
	rdpq_texture_rectangle_scaled(TILE0, x - width * 0.5f, y - height * 0.5f,
		x - width * 0.5f + BLIP_SIZE * scale_x, y - height * 0.5f + BLIP_SIZE * scale_y, 0, 0, BLIP_SIZE, BLIP_SIZE);
}

/* ---------- the pause menu (n64.h): its scoreboard over the frame, in the
menu's colours, laid out to its rows' text (Halo's small font, every row's
name measured), squeezed with the anamorphic picture as the HUD is */

#define PAUSE_BACKDROP 0x70000000u
#define PAUSE_PANEL 0xC0001028u
#define PAUSE_FRAME 0xFF8CC0FFu
#define PAUSE_CHOSEN 0xA0306CB8u
#define PAUSE_HUMAN 0x40306CB8u
#define PAUSE_TEXT 0xFF8CC0FFu
#define PAUSE_BRIGHT 0xFFFFFFFFu
#define PAUSE_DIM 0xC05880B0u

static const unsigned long pause_team_colors[2] = { 0xFFD03838u, 0xFF3C6CE0u };

static float pause_middle, pause_squeeze;

static float pause_x(float x)
{
	return pause_middle + (x - pause_middle) * pause_squeeze;
}

static void pause_fill(float x0, float y0, float x1, float y1, unsigned long argb)
{
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_set_prim_color(RGBA32((argb >> 16) & 0xFF, (argb >> 8) & 0xFF, argb & 0xFF, argb >> 24));
	rdpq_fill_rectangle(x0, y0, x1, y1);
}

static void pause_box(float x0, int y0, float x1, int y1, unsigned long argb)
{
	pause_fill(pause_x(x0), y0, pause_x(x1), y1, argb);
}

/* a frame's four sides (not overlapping: they are blended) */
static void pause_frame(float x0, int y0, float x1, int y1, int line, unsigned long argb)
{
	pause_box(x0, y0, x1, y0 + line, argb);
	pause_box(x0, y1 - line, x1, y1, argb);
	pause_box(x0, y0 + line, x0 + line, y1 - line, argb);
	pause_box(x1 - line, y0 + line, x1, y1 - line, argb);
}

enum { PAUSE_LEFT, PAUSE_RIGHT, PAUSE_CENTRE };

/* text from x, ending at x or about x; y its baseline */
static void pause_text(float x, int y, int align, unsigned long color, const char *text)
{
	float width = (float)n64_text_width(hud_font, text);

	if (align == PAUSE_RIGHT)
		x -= width;
	else if (align == PAUSE_CENTRE)
		x -= width * 0.5f;
	n64_text_draw_scaled(hud_font, pause_x(x), y, color, text, pause_squeeze);
}

void n64_hud_pause(const struct n64_pause_screen *screen)
{
	int width = n64_render_screen_width(), height = n64_render_screen_height();
	/* (480 lines: two pixels where 240 have one; the font stays itself) */
	int unit = height >= 480 ? 2 : 1;
	int pad = 6 * unit, gap = 8 * unit;
	int rows = screen->row_count, cap, pitch, total;
	int name_width, score_width, kills_width, deaths_width, marker, button_width, inner;
	int box_height, top, y, i;
	float x0, x1, name_x, score_x, kills_x, deaths_x;
	char number[12];

	if (!hud_font_loaded)
	{
		hud_font = n64_font_load("small");
		hud_font_loaded = 1;
	}
	if (rows > N64_SCORE_ROWS)
		rows = N64_SCORE_ROWS;
	hud.mode = _mode_none;
	hud.tmem_pixels = NULL;
	pause_middle = width * 0.5f;
	pause_squeeze = n64_render_hud_squeeze();
	rdpq_sync_pipe();
	rdpq_set_scissor(0, 0, width, height);
	/* the game behind, darkened (the whole picture, wide or not) */
	pause_fill(0, 0, width, height, PAUSE_BACKDROP);

	/* the columns as wide as their widest text: the names (11 letters at
	most), the numbers under their headings */
	cap = n64_font_cap_height(hud_font);
	name_width = n64_text_width(hud_font, "NAME");
	score_width = n64_text_width(hud_font, "SCORE");
	kills_width = n64_text_width(hud_font, "KILLS");
	deaths_width = n64_text_width(hud_font, "DEATHS");
	for (i = 0; i < rows; i++)
	{
		const struct n64_score_row *row = &screen->rows[i];

		name_width = MAX(name_width, n64_text_width(hud_font, row->name));
		score_width = MAX(score_width, n64_text_width(hud_font, row->score));
		snprintf(number, sizeof(number), "%d", row->kills);
		kills_width = MAX(kills_width, n64_text_width(hud_font, number));
		snprintf(number, sizeof(number), "%d", row->deaths);
		deaths_width = MAX(deaths_width, n64_text_width(hud_font, number));
	}
	marker = cap + 3 * unit;
	button_width = n64_text_width(hud_font, "MAIN MENU") + 2 * pad;
	inner = marker + name_width + 3 * gap + score_width + kills_width + deaths_width;
	inner = MAX(inner, 2 * button_width + 2 * gap + n64_text_width(hud_font, "START RESUME"));
	inner = MAX(inner, n64_text_width(hud_font, screen->note ? screen->note : ""));
	x0 = (width - inner) * 0.5f - pad;
	x1 = (width + inner) * 0.5f + pad;

	/* from the top: the title (and its note), the columns' headings over a
	rule, a row each (every row's text in capitals: rows a cap height and a
	little apart), the button; closer rows if they don't fit */
	pitch = cap + 2 * unit;
	for (;;)
	{
		box_height = pitch + 4 * unit;
		total = pad + pitch * (screen->note ? 2 : 1) + 2 * unit + pitch + unit + 3 * unit + rows * pitch +
			4 * unit + box_height + pad;
		if (total <= height - 4 * unit || pitch <= cap + 1)
			break;
		pitch--;
	}
	top = (height - total) / 2;
	pause_box(x0, top, x1, top + total, PAUSE_PANEL);
	pause_frame(x0, top, x1, top + total, unit, PAUSE_FRAME);
	y = top + pad;
	pause_text(pause_middle, y + cap, PAUSE_CENTRE, PAUSE_BRIGHT, screen->title);
	y += pitch;
	if (screen->note)
	{
		pause_text(pause_middle, y + cap, PAUSE_CENTRE, PAUSE_TEXT, screen->note);
		y += pitch;
	}
	y += 2 * unit;

	name_x = x0 + pad + marker;
	score_x = name_x + name_width + gap + score_width;
	kills_x = score_x + gap + kills_width;
	deaths_x = kills_x + gap + deaths_width;
	pause_text(name_x, y + cap, PAUSE_LEFT, PAUSE_DIM, "NAME");
	pause_text(score_x, y + cap, PAUSE_RIGHT, PAUSE_DIM, "SCORE");
	pause_text(kills_x, y + cap, PAUSE_RIGHT, PAUSE_DIM, "KILLS");
	pause_text(deaths_x, y + cap, PAUSE_RIGHT, PAUSE_DIM, "DEATHS");
	y += pitch;
	pause_box(x0 + pad, y, x1 - pad, y + unit, 0x808CC0FFu);
	y += unit + 3 * unit;

	for (i = 0; i < rows; i++)
	{
		const struct n64_score_row *row = &screen->rows[i];
		unsigned long color = row->human ? PAUSE_BRIGHT : PAUSE_TEXT;

		/* (the players on the controllers on a band of their own) */
		if (row->human)
			pause_box(x0 + pad - 2 * unit, y - unit, x1 - pad + 2 * unit, y + pitch - unit, PAUSE_HUMAN);
		if (row->team >= 0)
			pause_box(x0 + pad, y, x0 + pad + cap, y + cap, pause_team_colors[row->team & 1]);
		pause_text(name_x, y + cap, PAUSE_LEFT, color, row->name);
		pause_text(score_x, y + cap, PAUSE_RIGHT, color, row->score);
		snprintf(number, sizeof(number), "%d", row->kills);
		pause_text(kills_x, y + cap, PAUSE_RIGHT, color, number);
		snprintf(number, sizeof(number), "%d", row->deaths);
		pause_text(deaths_x, y + cap, PAUSE_RIGHT, color, number);
		y += pitch;
	}
	y += 4 * unit;

	/* The native blue buttons now expose Controls without leaving the match. */
	for (i = 0; i < 2; i++)
	{
		float bx = x0 + pad + i * (button_width + gap);
		if (screen->selected == i)
		{
			pause_box(bx, y, bx + button_width, y + box_height, PAUSE_CHOSEN);
			pause_frame(bx, y, bx + button_width, y + box_height, unit, PAUSE_FRAME);
		}
		pause_text(bx + button_width * 0.5f, y + (box_height + cap) / 2, PAUSE_CENTRE,
			screen->selected == i ? PAUSE_BRIGHT : PAUSE_TEXT, i ? "MAIN MENU" : "CONTROLS");
	}
	pause_text(x1 - pad, y + (box_height + cap) / 2, PAUSE_RIGHT, PAUSE_DIM, "START RESUME");
	hud.tmem_pixels = NULL;
}

/* Condense a long preset name inside its own column, using the same font. */
static void pause_text_fit(float x, int y, int align, unsigned long color, const char *text, int maximum)
{
	int width = n64_text_width(hud_font, text);
	float scale = width > maximum && width > 0 ? (float)maximum / width : 1.f;
	float shown = width * scale;
	if (align == PAUSE_RIGHT) x -= shown;
	else if (align == PAUSE_CENTRE) x -= shown * 0.5f;
	n64_text_draw_scaled(hud_font, pause_x(x), y, color, text, pause_squeeze * scale);
}

void n64_hud_pause_controls(const int *ports, int count, const int *selected_rows)
{
	static const char *const names[CONTROL_COUNT] = {"SCHEME", "CROUCH", "LOOK", "FIRE", "STICK", "C LOOK", "RUMBLE"};
	static const char *const schemes[N64_SCHEME_COUNT] = {"STANDARD", "BUMPER JUMPER", "GOLDENEYE", "NORTH", "GOLDEN STYLE",
		"D-PAD MOVE"};
	int width = n64_render_screen_width(), height = n64_render_screen_height();
	int unit = height >= 480 ? 2 : 1, cap, i, r;
	int columns = count > 1 ? 2 : 1, rows = count > 2 ? 2 : 1;
	int top = 23 * unit, bottom = height - 27 * unit;
	if (count < 1 || count > 4) return;
	if (!hud_font_loaded) { hud_font = n64_font_load("small"); hud_font_loaded = 1; }
	hud.mode = _mode_none;
	hud.tmem_pixels = NULL;
	pause_middle = width * 0.5f;
	pause_squeeze = n64_render_hud_squeeze();
	rdpq_sync_pipe();
	rdpq_set_scissor(0, 0, width, height);
	pause_fill(0, 0, width, height, PAUSE_BACKDROP);
	cap = n64_font_cap_height(hud_font);
	/* Per-player headings identify this page; leave the top clear for the
	user's optional FPS counter, which also appears in production. */
	for (i = 0; i < count; i++)
	{
		int port = ports[i], x0 = (i % columns) * width / columns + 4 * unit;
		int x1 = (i % columns + 1) * width / columns - 4 * unit;
		int y0 = top + (i / columns) * (bottom - top) / rows;
		int y1 = top + (i / columns + 1) * (bottom - top) / rows - 3 * unit;
		int pitch = (y1 - y0 - cap - 7 * unit) / CONTROL_COUNT;
		const struct n64_controls *controls;
		char text[48];
		if (port < 0 || port > 3) continue;
		controls = &n64_controls[port];
		pause_box(x0, y0, x1, y1, PAUSE_PANEL);
		pause_frame(x0, y0, x1, y1, unit, PAUSE_FRAME);
		snprintf(text, sizeof(text), "PLAYER %d  -  PORT %d", i + 1, port + 1);
		pause_text_fit((x0 + x1) * 0.5f, y0 + cap + 2 * unit, PAUSE_CENTRE, PAUSE_BRIGHT, text, x1 - x0 - 8 * unit);
		for (r = 0; r < CONTROL_COUNT; r++)
		{
			int y = y0 + cap + 5 * unit + r * pitch, selected = selected_rows[port] == r;
			const char *value = text;
			if (selected) pause_box(x0 + 2 * unit, y - unit, x1 - 2 * unit, y + pitch - unit, PAUSE_CHOSEN);
			switch (r)
			{
			case CONTROL_SCHEME: value = schemes[controls->scheme < N64_SCHEME_COUNT ? controls->scheme : 0]; break;
			case CONTROL_CROUCH: value = controls->toggle_crouch ? "TOGGLE" : "HOLD"; break;
			case CONTROL_LOOK: value = controls->inverted ? "INVERTED" : "NORMAL"; break;
			case CONTROL_FIRE: value = !controls->swap_fire ? "Z" : controls->scheme == N64_SCHEME_BUMPER_JUMPER ? "A" :
				controls->scheme == N64_SCHEME_GOLDEN_STYLE ? "D-LEFT" : "R"; break;
			case CONTROL_STICK_SPEED: snprintf(text, sizeof(text), "%d", 5 + controls->stick_speed); break;
			case CONTROL_C_SPEED: snprintf(text, sizeof(text), "%d", 5 + controls->c_speed); break;
			default: value = controls->no_rumble ? "OFF" : "ON"; break;
			}
			pause_text_fit(x0 + 4 * unit, y + cap, PAUSE_LEFT, selected ? PAUSE_BRIGHT : PAUSE_TEXT, names[r], (x1 - x0 - 12 * unit) / 2);
			pause_text_fit(x1 - 4 * unit, y + cap, PAUSE_RIGHT, selected ? PAUSE_BRIGHT : PAUSE_TEXT, value, (x1 - x0 - 12 * unit) / 2);
		}
	}
	pause_text_fit(pause_middle, height - 15 * unit, PAUSE_CENTRE, PAUSE_TEXT, "OWN PAD: UP/DOWN ROW  LEFT/RIGHT CHANGE", width - 12 * unit);
	pause_text_fit(pause_middle, height - 5 * unit, PAUSE_CENTRE, PAUSE_DIM, "A CYCLE   B BACK   START RESUME", width - 12 * unit);
	hud.tmem_pixels = NULL;
}
