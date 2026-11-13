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
#include <sys/stat.h>
#include <unistd.h>

#include "n64.h"

#define MAXIMUM_SPRITES 256
/* The PC maps' HUD: packed sprites (rom:/hud/<map>.subs), the meters'
places by size, the critical sprites first with a reserve on the heap and
the arena -- on the cartridges whose maps have HUD images larger than
256x256 for the converter to pack (tools/n64/hud.py): the PC Expansion's
and the Custom Edition's (the PC's HUD bitmaps); there, only on the maps
whose HUD the converter packed (n64_hud_packed). The Xbox maps -- every
cartridge's -- have none. On the other cartridges their HUD is v0.6.6's;
on these two a map with no packed sprite has v0.6.6's HUD but for one rule
(hud_sprite): a view whose recording left an image out is drawn live the
next time, and an image left out of a live view is asked for again
HUD_RETRY_VIEWS views later */
#if defined(HALO_N64_PC_EXPANSION) || defined(HALO_N64_CUSTOM_MAPS)
#define HUD_PACKING 1
#else
#define HUD_PACKING 0
#endif
/* packed sprites a map may have (rom:/hud/<map>.subs) */
#define MAXIMUM_PACKED_SPRITES 64
/* (v0.6.6's meter slots, without HUD_PACKING) */
#define METER_CACHE 12
#define MAXIMUM_VIEWS 4
#define HUD_INTERVAL 2
/* the meters' textures: RGBA16, at most 128x32 (the part of a meter
shown), each in a place of the meter block made at boot (n64_hud_reserve):
12 places of 8 KB, or with the half-size sprites places by size for the
most meter textures the player count's HUDs draw at once (meter_layout) */
#define METER_SLOT_WIDTH 128
#define METER_SLOT_HEIGHT 32
#define METER_BLOCK (12 * METER_SLOT_WIDTH * METER_SLOT_HEIGHT * 2)
#define MAXIMUM_METER_PLACES 36
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
/* the largest image (the room asset_load takes for it, and 1 KB) that
decides on the whole heap's room: a 128x128 image at 2 bytes a texel -- its
texels, its sprite headers and the room left for decompressing it where it
lies, within 1 KB -- and 1 KB. A larger one, a 256x256 image of 64 or
128 KB, decides on the room the map had before the converter's savings
(n64_heap_has_held, n64_heap_free_held): those bytes are not counted for
it, as for the display's other choices. Smaller images keep the whole
heap's room: where the model cache has filled the bytes given back, the
free total less them leaves a small image no room that it has before
the savings (in a test match on a Custom Edition map, four of its 1 to
5 KB images) */
#define HUD_LARGE_IMAGE (128 * 128 * 2 + 1024 + 1024)
/* the heap a HUD sprite leaves free for the game besides its own bytes: for
the HUD's critical sprites (hud_n64_load_critical_first) the most the game
itself took in play with no HUD sprites at all, 27088 bytes (Death Island,
one player, eight bots, 2400 game frames), and at least 15% more; for the
rest of the HUD 8 KB more, so that a critical sprite asked for later finds
room the rest did not take. Without that much after it, a sprite is left
out, as without room -- except where a weapon's critical sprites take the
memory of the weapon they replace (swap_critical) */
#define HUD_HEAP_RESERVE (32 * 1024)
#define HUD_HEAP_RESERVE_REST (40 * 1024)
/* the HUD views after which a critical sprite left out is asked for again */
#define HUD_RETRY_VIEWS_CRITICAL 4
/* the HUD views after which a sprite left out for want of room is asked for
again */
#define HUD_RETRY_VIEWS 32
/* (the custom cartridge) the HUD views, from its first such try, for which
a critical sprite wanted and missing keeps the local player's view models
out where its last try did not find too few bytes free -- it was left out
of a recording, or found no piece its size -- (n64_hud_view_model_room);
the critical pass asks for it again every HUD_RETRY_VIEWS_CRITICAL views */
#define CRITICAL_HOLD_VIEWS (2 * HUD_RETRY_VIEWS)
/* the largest image (the room asset_load takes for it and 1 KB) for which a
view's HUD may push models out (n64_render_push_out_for_hud): a 128x128
image at 2 bytes a texel -- its texels, its sprite headers and the room left
for decompressing it where it lies, within 1 KB (the PC Expansion maps'
three such images take 32,938-32,961 bytes) -- and 1 KB. The HUD's 256x256
images (64 or 128 KB), which v0.6.6 too loads only where the heap has room
for them, do not: on Death Island the push-out dropped the ground weapons'
models again and again for them */
#define PUSH_OUT_LARGEST HUD_LARGE_IMAGE
/* the push-outs an image may ask for in a map, counted for the image across
its views, whatever each one did (an image whose push-out pushed models out
and still found no room asks no more) */
#define PUSH_OUT_TRIES 3

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
	/* by view, the room the last image without room asked for (hud_sprite),
	sought before that view's next recording (n64_render_push_out_for_hud),
	and the image's number; 0: none */
	unsigned long push_need[MAXIMUM_VIEWS], push_image[MAXIMUM_VIEWS];
	int push_pieces[MAXIMUM_VIEWS];
	int push_sprite[MAXIMUM_VIEWS];
	/* an image whose push-out pushed models out and still found no room, or
	that asked PUSH_OUT_TRIES times: not asked for again until the map's end
	(release_sprites); the push-outs it asked for */
	unsigned char push_gave_up[MAXIMUM_SPRITES];
	unsigned char push_tries[MAXIMUM_SPRITES];
#if HUD_PACKING
	/* hud.views when each sprite was last left out for want of room (+1,
	its low 16 bits; 0 never): asked for again HUD_RETRY_VIEWS views later */
	unsigned short refused_at[MAXIMUM_SPRITES];
	/* a view whose recording left a sprite out for want of room: drawn as it
	goes the next time, where the sprite needs no recording's room */
	unsigned char live_next[MAXIMUM_VIEWS];
	/* the critical pass under way (n64_hud_critical_pass), its number (0 to
	65534), and the pass each sprite was last asked for in (+1; 0
	never): a sprite asked for in this pass or the views' last is critical, kept
	with HUD_HEAP_RESERVE; the rest with HUD_HEAP_RESERVE_REST. The bytes
	each sprite loaded into, or wants (a critical sprite this pass left out
	for want of room: wanted); a swap under way (swap_critical), and
	hud.views at the last that failed (+1; 0 none) */
	int critical_pass;
	unsigned short pass;
	unsigned short asked[MAXIMUM_SPRITES];
	unsigned short held[MAXIMUM_SPRITES]; /* in 16 bytes */
	int wanted_count;
	int swapping;
	unsigned short swap_failed_at;
	/* each sprite's SPRITE_ flags: wanted (above), loaded into the arena
	(arena_load), decoration (the critical pass found it an outline or a
	meter's frame: the owner's tiers, the first given back for a critical
	one); and the sprite whose room in the arena the pass found taken by
	sprites no longer critical (+1: a refill at the pass's end,
	arena_refill) */
	unsigned char flags[MAXIMUM_SPRITES];
	int arena_refill;
	/* the critical pass each sprite was last marked in as a sprite of a
	weapon some player carries, held or not (+1; 0 never): never given back
	for another critical sprite */
	unsigned short carried[MAXIMUM_SPRITES];
	/* rom:/hud/<map>.subs (tools/n64/hud.py pack): sprites cropped to one
	sprite rectangle of a large image (in its half-size texels), and whether
	an image has any */
	struct { short image, sprite, s0, t0, s1, t1; } packed[MAXIMUM_PACKED_SPRITES];
	int packed_count;
	unsigned char has_packed[MAXIMUM_SPRITES];
	/* the packed sprite an image is drawn from whole (the digits', the
	motion sensor's: its box of what is not clear), +1; 0 none */
	unsigned char whole[MAXIMUM_SPRITES];
#ifdef HALO_N64_CUSTOM_MAPS
	/* the free bytes each sprite not loaded needs (in 16 bytes; 0: none)
	where a try left it out -- SPRITE_FREE_SHORT where the last found too
	few bytes free for it over its reserve -- and hud.views when it was
	first so (+1, its low 16 bits): the room the local player's view models
	leave the critical ones on a map whose HUD is packed
	(n64_hud_view_model_room) */
	unsigned short short_of[MAXIMUM_SPRITES];
	unsigned short short_at[MAXIMUM_SPRITES];
	/* a critical pass has run on this map (n64_hud_view_models_wait) */
	int passed;
#endif
} hud = {.inset = -1, .recording = -1};

#define SPRITE_WANTED 1
#define SPRITE_IN_ARENA 2
#define SPRITE_DECORATION 4
#define SPRITE_FREE_SHORT 8
static inline int sprite_flag(int number, int flag) { return hud.flags[number] & flag; }
static inline void sprite_set(int number, int flag) { hud.flags[number] |= (unsigned char)flag; }
static inline void sprite_clear(int number, int flag) { hud.flags[number] &= (unsigned char)~flag; }
/* the bytes a sprite holds or wants (hud.held, in 16 bytes) */
static inline unsigned long held_bytes(int number) { return (unsigned long)hud.held[number] << 4; }
static inline void set_held(int number, unsigned long bytes) { hud.held[number] = (unsigned short)((bytes + 15) >> 4); }
/* a view whose recording left a sprite out: drawn live the next time */
static inline int live_next_pending(int view) { return hud.live_next[view]; }
static inline int live_next_take(int view)
{
	if (!hud.live_next[view])
		return 0;
	hud.live_next[view] = 0;
	return 1;
}
#else
} hud = {.inset = -1};

#define live_next_pending(view) 0
#define live_next_take(view) 0
#endif

/* the HUD's push-out requests let go: every view's (the views laid out
again: a view's request is for its own room), and with sprites set every
image's tries and give-up too (the map's end: the next map's image of that
number is another). Both HUD loaders' release_sprites and
n64_hud_set_view_count */
static void push_out_reset(int sprites)
{
	for (int i = 0; i < MAXIMUM_VIEWS; i++)
	{
		hud.push_need[i] = 0;
		hud.push_image[i] = 0;
		hud.push_pieces[i] = 0;
		hud.push_sprite[i] = 0;
	}
	if (sprites)
	{
		memset(hud.push_gave_up, 0, sizeof(hud.push_gave_up));
		memset(hud.push_tries, 0, sizeof(hud.push_tries));
	}
}

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

#if HUD_PACKING
static struct meter_entry
{
	int sprite;
	int s0, t0, width, height, step;
	unsigned long minimum_color, maximum_color, flash_color, background_color, tint_color;
	surface_t surface;
	uint8_t *place; /* in the meter block (uncached) */
	unsigned long bytes; /* the place's */
	unsigned long last_used;
	unsigned long drawn; /* hud_frame when last drawn */
} meters[MAXIMUM_METER_PLACES];
static unsigned long meter_clock;
/* the HUD's frames (n64_hud_set_view_count, each frame); the meter block
(uncached), its places' layout (meter_layout's number, -1 none yet), how
many places, and the bytes they take from its start */
static unsigned long hud_frame;
static uint8_t *meter_block;
static int meter_layout_in_use = -1, meter_places;
static unsigned long meter_bytes;
/* a meter texture too large for the places by size: the 8 KB places for the
rest of the map */
static int meter_too_large;
/* The arena: the meter block past the meter places with one, two or three
players on a map whose large HUD images are packed (the PC maps) and the
half-size sprites, where the HUD's critical sprites load (arena_load) --
memory no game allocation takes, and none of the heap. With one player
the places leave 55296 bytes, with two 49152, with three 24576 (a fourth
player's places); the most the critical sprites need at once on the PC
maps is 51074 bytes with one player (the sniper rifle zoomed: its reticle
is 12 sprites), at most 40754 with two and 49674 with three (the players'
own unit HUDs and weapons, shared sprites once): with three the arena
holds what fits, the rest is drawn from the heap. Its
start and end (cached addresses), and the top of what is loaded */
static uint8_t *arena_start, *arena_end, *arena_top;
/* (HUD STYLE 64) the bytes at the meter block's end lent to the style's
module while it is in (n64_hud_lend_slots), 0 when none: Halo draws none of
the meters the places are made for under the style -- the shield, the health
and the weapon's are the style's -- so the places are then two of 8 KB at
the block's start, for any meter Halo still draws (a vehicle's), with no
arena: the critical sprites load on the heap, with their reserve */
static unsigned long meter_lent;

/* The places by size: the most meter textures one player's HUD draws at
once, over every weapon and every vehicle seat with every weapon it may
hold on the PC and Xbox maps, from their tags as hud_unit.c and
hud_weapon.c draw them -- the unit's shield (three layers with the
overshield), health and flashlight meters, a Warthog driver's two other
seats' health, the weapon's meters: 8 with one player, 9 a player in
split screen. With one player one of them may be larger than 4352 bytes
(rows of whole 8 bytes: the assault rifle's bullet row is 114x27, 6264
bytes; the sniper rifle's 107x22), the others are at most 4352 -- two
places of 6272 (a bullet row's new texture at each shot beside the one
still drawn) and 7 of 4352, a small texture taking a large place where
it is free; in split
screen each is at most 4096 bytes, at most 3 of a player's are over
2048, and at most 8 are not. With the full-width sprites (twice as wide),
the 12 places of 8 KB. 0: those; 1-4: the player count's; 5: the two of
8 KB the block keeps while HUD STYLE 64 has its end (meter_lent) */
static int meter_layout(void)
{
	int arena = hud.packed_count > 0 && hud.view_count <= 3;

	if (meter_lent)
		return 5;
	if (!hud.packed_count || hud.wide || meter_too_large || hud.view_count < 1 || hud.view_count > 4)
		return 0;
	return hud.view_count + (arena ? 8 : 0);
}

static void place_meters(int layout)
{
	static const struct { unsigned short bytes, count[6]; } kinds[] =
	{
		{8192, {12, 0, 0, 0, 0, 2}},
		{6272, {0, 2, 0, 0, 0, 0}},
		{4352, {0, 7, 0, 0, 0, 0}},
		{4096, {0, 0, 6, 9, 12, 0}},
		{2048, {0, 0, 12, 18, 24, 0}},
	};
	uint8_t *at = meter_block;
	int places = layout & 7;

	meter_places = 0;
	for (unsigned k = 0; k < sizeof(kinds) / sizeof(kinds[0]); k++)
	{
		for (int i = 0; i < kinds[k].count[places] && meter_places < MAXIMUM_METER_PLACES; i++)
		{
			memset(&meters[meter_places], 0, sizeof(meters[0]));
			meters[meter_places].sprite = -1;
			meters[meter_places].place = at;
			meters[meter_places].bytes = kinds[k].bytes;
			at += kinds[k].bytes;
			meter_places++;
		}
	}
	meter_bytes = at - meter_block;
	meter_layout_in_use = layout;
	arena_start = arena_top = CachedAddr(meter_block + meter_bytes);
	arena_end = layout & 8 ? CachedAddr(meter_block + METER_BLOCK) : arena_start;
}
#else
static struct meter_entry
{
	int sprite;
	int s0, t0, width, height, step;
	unsigned long minimum_color, maximum_color, flash_color, background_color, tint_color;
	surface_t slot, surface;
	unsigned long last_used;
} meters[METER_CACHE];
static unsigned long meter_clock;
#endif

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

#if HUD_PACKING
/* what the HUD keeps from its first map on -- the meters' texture slots
(96 KB), the blip, its font -- made at boot (n64_main.c), at the heap's
bottom: made with the first map, above its image, they split the heap a
later, bigger map needs in one piece (n64_anim.c) */
void n64_hud_reserve(void)
{
	if (!meter_block)
	{
		meter_block = malloc_uncached_aligned(64, METER_BLOCK);
		if (meter_block)
			place_meters(0);
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
static void drop_made(void);
static void check_layout(void);
static int is_critical(int number);

static void release_sprites(void)
{
	rspq_wait();
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (hud.sprites[i])
			sprite_free(hud.sprites[i]);
		hud.sprites[i] = NULL;
		hud.missing[i] = 0;
		hud.refused_at[i] = 0;
		hud.sprite_wide[i] = 0;
		hud.asked[i] = 0;
		hud.held[i] = 0;
		hud.flags[i] = 0;
		hud.carried[i] = 0;
#ifdef HALO_N64_CUSTOM_MAPS
		hud.short_of[i] = 0;
		hud.short_at[i] = 0;
#endif
	}
	memset(hud.live_next, 0, sizeof(hud.live_next));
#ifdef HALO_N64_CUSTOM_MAPS
	hud.passed = 0;
#endif
	push_out_reset(1);
	hud.swap_failed_at = 0;
	arena_top = arena_start;
	hud.arena_refill = 0;
	drop_made();
}

/* what the HUD made from its sprites -- the views' recordings, the
uploads, the meters' textures -- given back (made again as the HUD is
drawn); the RSP and the RDP done with them */
static void drop_made(void)
{
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
	for (int i = 0; i < meter_places; i++)
	{
		meters[i].sprite = -1;
		meters[i].last_used = 0;
	}
	hud.tmem_pixels = NULL;
	/* (the deferred frees made) */
	rspq_wait();
}

/* drop_made in play -- a swap, an arena refill, a new meter layout: the
RDP's output parked first (n64_render_park_rdp_output), so that no
recording freed is still where the RSP writes (the map change's
release_sprites frees them as v0.6.6 did) */
static void drop_made_in_play(void)
{
	n64_render_park_rdp_output();
	drop_made();
}

/* the arena's sprites from at up forgotten (they own no memory of the
heap: sprite_load_buf), the top back to at; what was made from them given
back first (drop_made). Laid out as meter places again, the arena's
cache lines are written back and dropped (the places are written
uncached) */
static void forget_arena(uint8_t *at)
{
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (!sprite_flag(i, SPRITE_IN_ARENA) || (uint8_t *)hud.sprites[i] < at)
			continue;
		hud.sprites[i] = NULL;
		hud.sprite_wide[i] = 0;
		hud.held[i] = 0;
		sprite_clear(i, SPRITE_IN_ARENA);
	}
	if (arena_end > arena_start)
		data_cache_hit_writeback_invalidate(at, arena_top - at);
	arena_top = at;
}

/* a critical sprite of the half-size set loaded into the arena, where it
fits: the asset's in-place buffer (asset_loadf_into: the compressed data
read in at its end and unpacked over it), then sprite_load_buf over the
same memory, which writes the data cache back for the RDP. 1 if loaded */
static int arena_load(int number)
{
	char path[64];
	int sz, size;
	FILE *file;

	if (arena_end <= arena_start || !hud.names || number >= hud.name_count || hud.wide)
		return 0;
	snprintf(path, sizeof(path), "rom:/hud/%.16s.sprite", hud.names + number * 16);
	file = fopen(path, "rb");
	if (!file)
		return 0;
	/* (sz in: the file's size, which the asset header is checked against) */
	{
		struct stat st;

		sz = fstat(fileno(file), &st) == 0 ? (int)st.st_size : -1;
	}
	size = (int)(arena_end - arena_top);
	if (sz <= 0 || !asset_loadf_into(file, &sz, arena_top, &size))
	{
		fclose(file);
		return 0;
	}
	fclose(file);
	/* (the sprite keeps its unpacked bytes, sz: the in-place margin past
	them is free again) */
	hud.sprites[number] = sprite_load_buf(arena_top, sz);
	hud.sprite_wide[number] = 0;
	sprite_set(number, SPRITE_IN_ARENA);
	size = (sz + 15) & ~15;
	set_held(number, (unsigned long)size);
	arena_top += size;
	return 1;
}

/* whether the arena holds a sprite no view's last pass asked for */
static int arena_stale(void)
{
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (sprite_flag(i, SPRITE_IN_ARENA) && !is_critical(i))
			return 1;
	}
	return 0;
}

/* The arena refilled at the critical pass's end, where a critical sprite
found no room in it while it held sprites no view's last pass asked for
(a weapon's that was replaced): those from the lowest of them up are
forgotten -- with the critical sprites above it -- and every critical
sprite not loaded is loaded again, in number order. One that still does
not fit is drawn from the heap (hud_sprite). The RSP and the RDP are done
with what was made from them first (drop_made) */
static void arena_refill(void)
{
	uint8_t *lowest = arena_top;
	unsigned long start = TICKS_READ(), bytes = 0;
	int forgotten = 0, loaded = 0, cause = hud.arena_refill - 1;

	hud.arena_refill = 0;
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (sprite_flag(i, SPRITE_IN_ARENA) && !is_critical(i) && (uint8_t *)hud.sprites[i] < lowest)
			lowest = (uint8_t *)hud.sprites[i];
	}
	if (lowest == arena_top)
		return;
	drop_made_in_play();
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
		forgotten += sprite_flag(i, SPRITE_IN_ARENA) && (uint8_t *)hud.sprites[i] >= lowest;
	forget_arena(lowest);
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (hud.sprites[i] || hud.missing[i] || !is_critical(i))
			continue;
		if (arena_load(i))
			loaded++, bytes += held_bytes(i);
	}
	debugf("n64: HUD arena refill (s%d found no room): %d sprites forgotten, %d loaded (%lu B), %lu us\n", cause,
		forgotten, loaded, bytes, (unsigned long)TICKS_TO_US(TICKS_READ() - start));
}


/* the meter places laid out again where the player count or the sprite set
changed them (meter_layout): what was made from the old ones given back
first, the RSP and the RDP done with it */
static void check_layout(void)
{
	int layout = meter_layout();

	if (!meter_block || layout == meter_layout_in_use)
		return;
	drop_made_in_play();
	forget_arena(arena_start);
	place_meters(layout);
}

/* (HUD STYLE 64, n64_hud64_stub.c) the meter block past its first keep
places of 8 KB lent to the style's module: what was made in the block given
back first (the views' recordings with it), the arena's sprites forgotten,
the places laid out as two of 8 KB; none of the lent memory in the data
cache. Its memory (cached) and *bytes; NULL without the block, or lent
already */
void *n64_hud_lend_slots(int keep, unsigned long *bytes)
{
	unsigned long kept = (unsigned long)keep * METER_SLOT_WIDTH * METER_SLOT_HEIGHT * 2;
	void *cached;

	*bytes = 0;
	if (!meter_block || meter_lent || keep < 1 || kept >= METER_BLOCK)
		return NULL;
	drop_made_in_play();
	forget_arena(arena_start);
	meter_lent = METER_BLOCK - kept;
	place_meters(meter_layout());
	cached = CachedAddr(meter_block + kept);
	data_cache_hit_invalidate(cached, meter_lent);
	*bytes = meter_lent;
	return cached;
}

/* (HUD STYLE 64) the lent memory back: the RDP done with what the style
put there, its cache lines written back and dropped (the places are written
uncached), the places laid out again for the player count (check_layout:
what was made from the sprites dropped). Where that layout has the arena,
the sprites loaded on the heap meanwhile -- the critical pass's, with no
arena -- are given back too: the next pass loads them into the arena, so
that the game's own HUD takes the heap it takes without the style */
void n64_hud_return_slots(void)
{
	if (!meter_lent)
		return;
	rspq_wait();
	data_cache_hit_writeback_invalidate(CachedAddr(meter_block + METER_BLOCK - meter_lent), meter_lent);
	meter_lent = 0;
	check_layout();
	for (int i = 0; arena_end > arena_start && i < MAXIMUM_SPRITES; i++)
	{
		if (!hud.sprites[i])
			continue;
		sprite_free(hud.sprites[i]);
		hud.sprites[i] = NULL;
		hud.sprite_wide[i] = 0;
		hud.held[i] = 0;
	}
}

/* the passes since a mark was made (hud.pass + 1 then: 1 to 65535; the
pass wraps at 65535, so that no mark is 0, "never") */
static unsigned pass_age(unsigned short mark)
{
	unsigned now = hud.pass + 1u;

	return now >= mark ? now - mark : now + 65535u - mark;
}

/* a sprite asked for in this critical pass or in one of the views' last
(a pass a view) */
static int is_critical(int number)
{
	return hud.asked[number] && pass_age(hud.asked[number]) <= (unsigned)(hud.view_count > 1 ? hud.view_count : 1);
}

/* a sprite of a weapon a player carries, marked in this critical pass or
one of the views' last */
static int is_carried(int number)
{
	return hud.carried[number] && pass_age(hud.carried[number]) <= (unsigned)(hud.view_count > 1 ? hud.view_count : 1);
}

/* whether the heap has a sprite's room: its bytes in one piece and, while
a recording is open, the recording's room, with the reserve besides. On the
PC Expansion cartridge an image of at most PUSH_OUT_LARGEST has the whole
heap's room, the given bytes too, and the recording's room is looked for
as the pieces n64_render_make_room begins it with, besides the image's
piece: its chunks are made one at a time. With the view model kept on the
big maps the free room is seldom one piece: on Infinity, in one program
layout, the HUD's small images found none for 2,400 frames. A larger image,
which the HUD's push-out does not serve either, decides on the room the
map had before the converter's savings (n64_heap_has_held,
n64_heap_free_held), on every cartridge; a small image on the other
cartridges keeps the one-piece test on the whole heap */
static unsigned long hud_free(unsigned long bytes)
{
#ifdef HALO_N64_PC_EXPANSION
	if (bytes <= PUSH_OUT_LARGEST)
		return n64_heap_free();
#else
	if (bytes <= HUD_LARGE_IMAGE)
		return n64_heap_free();
#endif
	return n64_heap_free_held();
}

static int hud_room_now(unsigned long bytes, unsigned long reserve)
{
	int pieces = hud.recording >= 0 ? RECORDING_ROOM_PIECES : 0;

#ifdef HALO_N64_PC_EXPANSION
	if (bytes <= PUSH_OUT_LARGEST)
		return n64_heap_has_pieces(bytes, pieces, RECORDING_ROOM_PIECE) && n64_heap_free() >= bytes + reserve;
#else
	if (bytes <= HUD_LARGE_IMAGE)
		return n64_heap_has(bytes + (unsigned long)pieces * RECORDING_ROOM_PIECE) && n64_heap_free() >= bytes + reserve;
#endif
	return n64_heap_has_held(bytes + (unsigned long)pieces * RECORDING_ROOM_PIECE) && n64_heap_free_held() >= bytes + reserve;
}

/* (with the projectiles' module given back first where there is none,
n64_particles.c: one call of hud_room_now) */
static int hud_room(unsigned long bytes, unsigned long reserve)
{
	for (int tries = 0;; tries++)
		if (hud_room_now(bytes, reserve))
			return 1;
		else if (tries || !n64_projectiles_give_back())
			return 0;
}

static sprite_t *hud_sprite(int number);

/* Critical sprites the heap has no room for (too few bytes over
HUD_HEAP_RESERVE, or no piece large enough), while the HUD holds sprites
of the heap that are not critical, take their memory: first those of
weapons no player carries any more (critical once, asked for in none of
the views' last passes, carried by none), then decoration, then the rest
of the HUD (the owner's tiers), each tier in sprite number order, until
they give the bytes wanted; never a
sprite of a weapon a player carries, held or not. Those are given back and the new ones loaded in
their place with no reserve -- the HUD then holds no more than it did,
or no more than the heap has over HUD_HEAP_RESERVE. In a swap a sprite
is loaded by swap_load, with no 1 KB margin, and no retry window holds
it back (hud_sprite). If any new one does not load, the new ones are given
back and the old ones loaded again, the largest first, and no swap is
tried again for HUD_RETRY_VIEWS views. An old one loads again only where
its piece and about 40 bytes beside it are free (memalign(16) asks malloc
for the sprite + 32 bytes): where small allocations took the slack beside
it, it stays out until it is asked for again (it is not critical). The RSP and the RDP are done
with what was made from them first (drop_made_in_play) */
static void swap_critical(void)
{
	unsigned char old[MAXIMUM_SPRITES];
	unsigned long freed = 0, wanted = 0, free_bytes, need;
	int count = 0, loaded = 0, reloaded = 0, failed = 0;

	/* (the projectiles' module given back first, n64_particles.c) */
	n64_projectiles_give_back();
	free_bytes = n64_heap_free();

	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		old[i] = 0;
		wanted += sprite_flag(i, SPRITE_WANTED) ? held_bytes(i) : 0;
	}
	need = wanted > (free_bytes > HUD_HEAP_RESERVE ? free_bytes - HUD_HEAP_RESERVE : 0) ?
		wanted - (free_bytes > HUD_HEAP_RESERVE ? free_bytes - HUD_HEAP_RESERVE : 0) : 0;
	/* (the heap's total has room, but a wanted sprite finds no piece of
	it large enough: the need is all the bytes wanted) */
	for (int i = 0; i < MAXIMUM_SPRITES && need < wanted; i++)
	{
		if (sprite_flag(i, SPRITE_WANTED) && !n64_heap_has(held_bytes(i)))
			need = wanted;
	}
	for (int tier = 0; tier < 3 && freed < need; tier++)
	{
		for (int i = 0; i < MAXIMUM_SPRITES && freed < need; i++)
		{
			if (!hud.sprites[i] || sprite_flag(i, SPRITE_IN_ARENA | SPRITE_WANTED) || is_critical(i) || is_carried(i) ||
				old[i] || (tier == 0 && !hud.asked[i]) ||
				(tier == 1 && (hud.asked[i] || !sprite_flag(i, SPRITE_DECORATION))))
				continue;
			old[i] = 1;
			freed += held_bytes(i);
			count++;
		}
	}
	if (!count || freed < need ||
		(hud.swap_failed_at && (unsigned short)(hud.views + 1 - hud.swap_failed_at) < HUD_RETRY_VIEWS))
		return;
	rspq_wait();
	drop_made_in_play();
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (!old[i])
			continue;
		sprite_free(hud.sprites[i]);
		hud.sprites[i] = NULL;
		hud.sprite_wide[i] = 0;
	}
	hud.swapping = 1;
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (!sprite_flag(i, SPRITE_WANTED))
			continue;
		hud.refused_at[i] = 0;
		/* (a new one that loads is no longer wanted: old[i] = 3 marks it,
		for the rollback) */
		if (hud_sprite(i))
		{
			loaded++;
			old[i] = 3;
		}
		else
			failed = 1;
	}
	if (failed)
	{
		for (int i = 0; i < MAXIMUM_SPRITES; i++)
		{
			if (old[i] != 3 || !hud.sprites[i])
				continue;
			sprite_free(hud.sprites[i]);
			hud.sprites[i] = NULL;
			hud.sprite_wide[i] = 0;
			hud.held[i] = 0;
			hud.refused_at[i] = (unsigned short)(hud.views + 1);
		}
		/* the old ones loaded again (each keeps its bytes in hud.held until
		then), the largest first, so that a smaller one does not take the
		piece a larger one left; one that finds no room is left out */
		for (int left = count; left > 0; left--)
		{
			int best = -1;

			for (int i = 0; i < MAXIMUM_SPRITES; i++)
			{
				if (old[i] == 1 && (best < 0 || hud.held[i] > hud.held[best]))
					best = i;
			}
			old[best] = 2;
			if (hud_sprite(best))
				reloaded++;
			else
				hud.held[best] = 0;
		}
		hud.swap_failed_at = (unsigned short)(hud.views + 1);
	}
	else
	{
		for (int i = 0; i < MAXIMUM_SPRITES; i++)
		{
			if (old[i] == 1)
				hud.held[i] = 0;
		}
	}
	hud.swapping = 0;
	if (failed)
		debugf("n64: HUD swap: %d sprites (%lu B) given back for %d (%lu B), %d loaded: %d of the %d old ones loaded again\n",
			count, freed, hud.wanted_count, wanted, loaded, reloaded, count);
	else
		debugf("n64: HUD swap: %d sprites (%lu B) given back for %d (%lu B), %d loaded\n", count, freed, hud.wanted_count,
			wanted, loaded);
}

/* whether this map's HUD is packed: the critical pass (n64_render_game.c),
and with it the reserve, the swap and the meter places by size, only there;
a map with no packed sprite -- the Xbox maps, on every cartridge, and the
Custom Edition maps the converter did not pack -- has v0.6.6's HUD, but for
live_next and the HUD_RETRY_VIEWS window (hud_sprite; it has no critical
pass) */
int n64_hud_packed(void)
{
	return hud.packed_count > 0;
}

#ifdef HALO_N64_CUSTOM_MAPS
/* HUD first (the HUD's tiers: the critical readouts before every model),
on a map whose HUD is packed -- where the critical pass loads the critical
sprites before the rest of the HUD: the free bytes the critical sprites
wanted and missing need -- each sprite asked for in the critical pass that
is not loaded and that a try left out (short_of): where the last try found
too few bytes free for it, until it loads; else (it was left out of a
recording, or found no piece its size) for CRITICAL_HOLD_VIEWS -- of those
that need at most limit. The local player's view models leave them free
(n64_render.c load_model) and give way for them (view_models_give_way) */
unsigned long n64_hud_view_model_room(unsigned long limit)
{
	unsigned long room = 0;

	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		unsigned long need = (unsigned long)hud.short_of[i] << 4;

		if (need && need <= limit && !hud.sprites[i] && !hud.missing[i] && is_critical(i) &&
			(sprite_flag(i, SPRITE_FREE_SHORT) || (unsigned short)(hud.views + 1 - hud.short_at[i]) < CRITICAL_HOLD_VIEWS))
			room += need;
	}
	return room;
}

/* whether no critical pass has run on this map yet: the first comes after
the first-person weapon's models are asked for, and the local player's
view models wait for it (n64_render.c load_model) */
int n64_hud_view_models_wait(void)
{
	return !hud.passed;
}

/* a view model gave way for the critical sprites wanted and missing: they
are asked for again in the views' next HUD, not HUD_RETRY_VIEWS later, and
keep the view models out for CRITICAL_HOLD_VIEWS from now at least -- a
recording that leaves one out does not answer for the view models */
void n64_hud_view_models_gave_way(void)
{
	for (int i = 0; i < MAXIMUM_SPRITES; i++)
	{
		if (hud.short_of[i] && !hud.sprites[i] && is_critical(i))
		{
			hud.refused_at[i] = 0;
			hud.short_at[i] = (unsigned short)(hud.views + 1);
		}
	}
}
#endif

/* (the critical pass, hud.c) the sprites asked for meanwhile are critical;
at its end, those it left out for want of room swapped in for a replaced
weapon's (swap_critical) */
void n64_hud_critical_pass(int on)
{
	hud.critical_pass = on;
#ifdef HALO_N64_CUSTOM_MAPS
	hud.passed |= on;
#endif
	if (on)
	{
		check_layout();
		if (++hud.pass == 65535)
			hud.pass = 0;
		return;
	}
	if (hud.arena_refill)
		arena_refill();
	if (hud.wanted_count)
		swap_critical();
	for (int i = 0; hud.wanted_count && i < MAXIMUM_SPRITES; i++)
	{
		if (sprite_flag(i, SPRITE_WANTED) && !hud.sprites[i])
			hud.held[i] = 0;
		sprite_clear(i, SPRITE_WANTED);
	}
	hud.wanted_count = 0;
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
	hud.packed_count = 0;
	memset(hud.has_packed, 0, sizeof(hud.has_packed));
	memset(hud.whole, 0, sizeof(hud.whole));
	/* (HUD STYLE 64's textures with it, as in the version below) */
	n64_hud64_release();
}
#else
/* The meters' slots are one block (meter_block, uncached), each slot a part
of it, so that HUD STYLE 64 can borrow the block's last slots for its module
and textures while it is in, where nothing else of the heap moves
(n64_hud_lend_slots, n64_hud64_stub.c), and have them back as slots, with
nothing to allocate, when it goes (n64_hud_return_slots). Without the block
(no room at boot) each slot is its own surface, as before, and nothing is
lent */
#define METER_SLOT_BYTES (METER_SLOT_WIDTH * METER_SLOT_HEIGHT * 2)
static uint8_t *meter_block;
static int meter_lent;

static void meter_slots_from_block(void)
{
	for (int i = 0; i < METER_CACHE; i++)
	{
		memset(&meters[i], 0, sizeof(meters[i]));
		meters[i].sprite = -1;
		if (i < METER_CACHE - meter_lent)
			meters[i].slot = surface_make(meter_block + i * METER_SLOT_BYTES, FMT_RGBA16, METER_SLOT_WIDTH,
				METER_SLOT_HEIGHT, METER_SLOT_WIDTH * 2);
	}
}

/* (HUD STYLE 64) the block's slots past keep, lent: the views' recordings
dropped (they may draw the slots' textures), the RDP done with them, none of
their memory in the data cache; their memory (cached) and its bytes. NULL
without the block, or lent already */
void *n64_hud_lend_slots(int keep, unsigned long *bytes)
{
	void *cached;

	*bytes = 0;
	if (!meter_block || meter_lent || keep < 1 || keep >= METER_CACHE)
		return NULL;
	n64_hud_forget_recordings();
	rspq_wait();
	meter_lent = METER_CACHE - keep;
	meter_slots_from_block();
	*bytes = (unsigned long)meter_lent * METER_SLOT_BYTES;
	cached = CachedAddr(meter_block + keep * METER_SLOT_BYTES);
	data_cache_hit_invalidate(cached, *bytes);
	return cached;
}

/* (HUD STYLE 64) the lent slots back: the views' recordings dropped, the RDP
done with what the style put there, its cache lines written back and dropped
(the slots are written uncached) */
void n64_hud_return_slots(void)
{
	if (!meter_lent)
		return;
	n64_hud_forget_recordings();
	rspq_wait();
	data_cache_hit_writeback_invalidate(CachedAddr(meter_block + (METER_CACHE - meter_lent) * METER_SLOT_BYTES),
		(unsigned long)meter_lent * METER_SLOT_BYTES);
	meter_lent = 0;
	meter_slots_from_block();
}

/* what the HUD keeps from its first map on -- the meters' texture slots
(96 KB), the blip, its font -- made at boot (n64_main.c), at the heap's
bottom: made with the first map, above its image, they split the heap a
later, bigger map needs in one piece (n64_anim.c) */
void n64_hud_reserve(void)
{
	if (!meter_block)
	{
		/* (whole cache lines, none in the data cache: n64_main.c's
		malloc_uncached_aligned) */
		meter_block = malloc_uncached_aligned(64, METER_CACHE * METER_SLOT_BYTES);
		if (meter_block)
			meter_slots_from_block();
	}
	for (int i = 0; i < METER_CACHE; i++)
	{
		if (!meters[i].slot.buffer && !meter_block)
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
	push_out_reset(1);
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
	/* (HUD STYLE 64's textures, when its module is loaded: the RDP done
	with them first, n64_hud64_stub.c. With ORIGINAL nothing waits here:
	the sprites' frees stay deferred, and the heap is laid out as without
	the style) */
	n64_hud64_release();
}

/* (the meter places by size: the PC maps' cartridges only) */
static inline void check_layout(void)
{
}
#endif

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

#if HUD_PACKING
/* the packed sprites (hud.packed), a line "<image> <sprite> <s0> <t0> <s1>
<t1> [<whole>]" each, where the map has them */
static void read_packed(const char *path)
{
	char line[64];
	FILE *file = fopen(path, "r");

	while (file && hud.packed_count < MAXIMUM_PACKED_SPRITES && fgets(line, sizeof(line), file))
	{
		int image, sprite, s0, t0, s1, t1, whole = 0;

		if (sscanf(line, "%d %d %d %d %d %d %d", &image, &sprite, &s0, &t0, &s1, &t1, &whole) < 6 ||
			image < 0 || image >= hud.name_count || sprite < 0 || sprite >= hud.name_count ||
			s1 <= s0 || t1 <= t0)
			continue;
		hud.packed[hud.packed_count].image = image;
		hud.packed[hud.packed_count].sprite = sprite;
		hud.packed[hud.packed_count].s0 = s0;
		hud.packed[hud.packed_count].t0 = t0;
		hud.packed[hud.packed_count].s1 = s1;
		hud.packed[hud.packed_count].t1 = t1;
		hud.packed_count++;
		hud.has_packed[image] = 1;
		if (whole)
			hud.whole[image] = (unsigned char)hud.packed_count;
	}
	if (file)
		fclose(file);
}

/* the packed sprite of image number holding its half-size texels s0..s1,
t0..t1, or -1 */
static int packed_sprite(int number, float s0, float t0, float s1, float t1)
{
	if (number < 0 || number >= MAXIMUM_SPRITES || !hud.has_packed[number])
		return -1;
	for (int i = 0; i < hud.packed_count; i++)
	{
		if (hud.packed[i].image == number &&
			s0 >= hud.packed[i].s0 - 0.01f && s1 <= hud.packed[i].s1 + 0.01f &&
			t0 >= hud.packed[i].t0 - 0.01f && t1 <= hud.packed[i].t1 + 0.01f)
			return i;
	}
	return -1;
}

void n64_hud_set_map(const char *map_name)
{
	n64_hud_release();
	meter_too_large = 0;
	if (!meter_block)
	{
		meter_block = malloc_uncached_aligned(64, METER_BLOCK);
		if (meter_block)
			place_meters(0);
	}
	if (!hud.blip.buffer)
		make_blip();
	snprintf(hud.map_name, sizeof(hud.map_name), "%s", map_name);
	/* the sprites' names, in number order (tools/n64/hud.py) */
	{
		char path[64];

		snprintf(path, sizeof(path), "rom:/hud/%s.list", map_name);
		hud.name_count = read_list(path, &hud.names, NULL);
		snprintf(path, sizeof(path), "rom:/hud/%s.subs", map_name);
		read_packed(path);
	}
}
#else
void n64_hud_set_map(const char *map_name)
{
	n64_hud_release();
	for (int i = 0; i < METER_CACHE; i++)
	{
		/* (the block's slots are there, or lent to HUD STYLE 64) */
		if (!meters[i].slot.buffer && !meter_block)
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
#endif

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
	on = on && hud.wide_name_count == hud.name_count && hud.wide_bytes && room;
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
	debugf("n64: HUD sprites %s (%lu KB past the models' room)\n", on ? "full width" : "half size", room / 1024);
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

#if HUD_PACKING
/* a sprite loaded in a swap (swap_critical), or in the critical pass where
only the 1 KB margin keeps it out (hud_sprite): its buffer allocated first,
of the size the asset says it needs (asset_loadf_into with no buffer),
then the file read into it, as the arena's (arena_load); the sprite owns
the buffer, as sprite_load's. memalign answers NULL where the heap has no
piece for it -- asset_load would stop the game -- so no margin is kept
for it; memalign(16) itself asks malloc for the sprite + 32 bytes. No
stdio buffer: the file is not read through it. NULL where there is no
room */
static sprite_t *swap_load(const char *path)
{
	FILE *file = fopen(path, "rb");
	struct stat st;
	int sz, size = 0;
	void *buf = NULL;
	sprite_t *sprite = NULL;

	if (!file)
		return NULL;
	sz = fstat(fileno(file), &st) == 0 ? (int)st.st_size : -1;
	if (sz > 0 && !asset_loadf_into(file, &sz, NULL, &size) && size > 0 && (buf = memalign(16, size)) &&
		lseek(fileno(file), 0, SEEK_SET) == 0 && asset_loadf_into(file, &sz, buf, &size))
	{
		sprite = sprite_load_buf(buf, sz);
		if ((void *)sprite != buf)
			free(buf);
		else
			sprite->flags |= SPRITE_FLAGS_OWNEDBUFFER;
	}
	else if (buf)
		free(buf);
	fclose(file);
	return sprite;
}

static sprite_t *hud_sprite(int number)
{
	char path[64];

	if (number < 0 || number >= MAXIMUM_SPRITES || hud.missing[number])
		return NULL;
	/* (a sprite left out for want of room as the HUD was drawn live is
	asked for again only HUD_RETRY_VIEWS views later: each ask opens its
	files and walks the heap's totals; one left out while a recording was
	made, at its view's next drawing, live) */
	if (hud.critical_pass)
		hud.asked[number] = (unsigned short)(hud.pass + 1);
	if (!hud.sprites[number] && hud.refused_at[number] && !hud.swapping &&
		(unsigned short)(hud.views + 1 - hud.refused_at[number]) <
		(hud.critical_pass ? HUD_RETRY_VIEWS_CRITICAL : HUD_RETRY_VIEWS))
		return NULL;
	/* (a critical sprite asked for in the critical pass, outside a
	recording: the arena's, where it fits; where sprites no longer critical
	hold its room, loaded at the pass's end (arena_refill); else the heap's) */
	if (!hud.sprites[number] && hud.critical_pass && hud.recording < 0 && arena_end > arena_start)
	{
		if (arena_load(number))
			return hud.sprites[number];
		if (arena_stale())
		{
			if (!hud.arena_refill)
				hud.arena_refill = number + 1;
			return NULL;
		}
	}
	if (!hud.sprites[number])
	{
		int found = 0, critical = is_critical(number);
		unsigned long asked = 0;
		int asked_pieces = hud.recording >= 0 ? RECORDING_ROOM_PIECES : 0;

		/* the full-width set's sprite where it is in use, else -- or
		without room for it -- the half-size set's */
		for (int wide = hud.wide; wide >= 0 && !hud.sprites[number]; wide--)
		{
			const char *names = wide ? hud.wide_names : hud.names;
			FILE *file;
			unsigned long bytes, reserve;
			int room, exact;

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
			made as it goes; and HUD_HEAP_RESERVE left on the heap besides
			the sprite. In a swap the file is closed first -- its stdio buffer
			(1 KB, from sprite_bytes' read) can sit in the very piece a sprite
			given back left -- and the sprite is loaded by swap_load, whose
			own allocation is the check. So is a critical sprite asked for in
			the critical pass, outside a recording, that the heap's free bytes
			have room for over the reserve where no piece of its size + 1 KB
			is free: the 1 KB is for asset_load's unchecked allocation. The
			room is the map's before the converter's savings: the bytes its
			image left out, the heap's once the map's display is made, are not
			counted (n64_heap_has_held, n64_heap_free_held): the HUD is part of
			the display) */
			bytes = sprite_bytes(file) + 1024;
			if (hud.swapping)
			{
				fclose(file);
				file = NULL;
			}
			reserve = !hud.packed_count ? 0 : critical ? HUD_HEAP_RESERVE : HUD_HEAP_RESERVE_REST;
			asked = bytes;
			room = hud.swapping ? hud.recording < 0 : hud_room(bytes, reserve);
			exact = !room && !hud.swapping && critical && hud.critical_pass && hud.recording < 0 &&
				hud_free(bytes) >= bytes + reserve;
			if (file)
				fclose(file);
			if (exact)
				room = (hud.sprites[number] = swap_load(path)) != NULL;
			/* (a critical sprite left out in the critical pass, outside a
			recording: wanted, for swap_critical at the pass's end) */
			if (!room && critical && hud.critical_pass && hud.recording < 0 && !sprite_flag(number, SPRITE_WANTED) &&
				!hud.swapping)
			{
				sprite_set(number, SPRITE_WANTED);
				hud.wanted_count++;
				set_held(number, bytes);
			}
			if (!room)
				continue;
			if (!hud.sprites[number])
				hud.sprites[number] = hud.swapping ? swap_load(path) : sprite_load(path);
			hud.sprite_wide[number] = wide && hud.sprites[number];
			set_held(number, hud.sprites[number] ? bytes : 0);
			if (hud.sprites[number])
				hud.refused_at[number] = 0;
		}
		if (!found)
		{
			hud.missing[number] = 1;
			return NULL;
		}
		if (hud.sprites[number] && sprite_flag(number, SPRITE_WANTED))
		{
			sprite_clear(number, SPRITE_WANTED);
			hud.wanted_count--;
		}
		/* (none had room: the smallest set's room asked for before this
		view's next recording, n64_hud_begin -- of the images refused, the
		one that asks least, which the push-out may make room for where a
		larger one could not; only an image of at most PUSH_OUT_LARGEST) */
		if (!hud.sprites[number] && asked && asked <= PUSH_OUT_LARGEST && !hud.push_gave_up[number] && !hud.swapping &&
			hud.view >= 0 && hud.view < MAXIMUM_VIEWS &&
			(!hud.push_need[hud.view] || asked + (unsigned long)asked_pieces * RECORDING_ROOM_PIECE < hud.push_need[hud.view]))
		{
			hud.push_need[hud.view] = asked + (unsigned long)asked_pieces * RECORDING_ROOM_PIECE;
			hud.push_image[hud.view] = asked;
			hud.push_pieces[hud.view] = asked_pieces;
			hud.push_sprite[hud.view] = number;
		}
		/* (left out of the view's recording: the view drawn live the next
		time, where the image needs no recording's room; left out of a live
		view: asked for again HUD_RETRY_VIEWS views later, in a packed map's
		critical pass HUD_RETRY_VIEWS_CRITICAL. On every map: with the room the
		map image's savings leave, a view's recording finds its room where
		v0.6.7 drew the view live, and an image beside that room seldom finds
		its own -- on the Custom Edition cartridge the image and the
		recording's room in one piece, on the PC Expansion cartridge the
		image's piece and the recording's pieces besides it (hud_room)) */
		if (!hud.sprites[number] && hud.recording >= 0 && hud.recording < MAXIMUM_VIEWS)
			hud.live_next[hud.recording] = 1;
		else if (!hud.sprites[number])
			hud.refused_at[number] = (unsigned short)(hud.views + 1);
#ifdef HALO_N64_CUSTOM_MAPS
		/* (the bytes it was left out for, and whether for too few bytes
		free -- a live try's answer: a recording's keeps the last -- the room
		the view models leave it, n64_hud_view_model_room) */
		if (hud.sprites[number] || !asked)
		{
			hud.short_of[number] = hud.short_at[number] = 0;
			sprite_clear(number, SPRITE_FREE_SHORT);
		}
		else if (!hud.swapping)
		{
			hud.short_of[number] = (unsigned short)((asked + 15) >> 4);
			if (!hud.short_at[number])
				hud.short_at[number] = (unsigned short)(hud.views + 1);
			if (hud.recording < 0)
			{
				sprite_clear(number, SPRITE_FREE_SHORT);
				if (hud_free(asked) < asked + (critical ? HUD_HEAP_RESERVE : HUD_HEAP_RESERVE_REST))
					sprite_set(number, SPRITE_FREE_SHORT);
			}
		}
#endif
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
	/* (an image drawn whole from its packed box: that sprite) */
	if (number >= 0 && number < MAXIMUM_SPRITES && hud.whole[number])
		number = hud.packed[hud.whole[number] - 1].sprite;
	return hud_sprite(number) != NULL;
}

/* the sprite a quad of image number at its half-size texel (s, t) draws
from -- the packed sprite holding the point, where there is one -- loaded
where there is room (the HUD's critical sprites, hud.c) */
int n64_hud_ensure_sprite_at(int number, float s, float t)
{
	int packed = packed_sprite(number, s, t, s, t);

	return packed >= 0 ? hud_sprite(hud.packed[packed].sprite) != NULL : n64_hud_ensure_sprite(number);
}

/* the sprite a quad of image number at (s, t) draws from marked a carried
weapon's (the critical pass: swap_critical never gives it back) */
void n64_hud_mark_carried_at(int number, float s, float t)
{
	int packed = packed_sprite(number, s, t, s, t);

	if (packed >= 0)
		number = hud.packed[packed].sprite;
	else if (number >= 0 && number < MAXIMUM_SPRITES && hud.whole[number])
		number = hud.packed[hud.whole[number] - 1].sprite;
	if (number >= 0 && number < MAXIMUM_SPRITES)
		hud.carried[number] = (unsigned short)(hud.pass + 1);
}

/* the sprite a quad of image number at (s, t) draws from marked decoration
(the critical pass: swap_critical gives it back first) */
void n64_hud_mark_decoration_at(int number, float s, float t)
{
	int packed = packed_sprite(number, s, t, s, t);

	if (packed >= 0)
		number = hud.packed[packed].sprite;
	else if (number >= 0 && number < MAXIMUM_SPRITES && hud.whole[number])
		number = hud.packed[hud.whole[number] - 1].sprite;
	if (number >= 0 && number < MAXIMUM_SPRITES)
		sprite_set(number, SPRITE_DECORATION);
}

/* a quad drawn upright, its texels along its edges (s across, t down) */
static int axis_aligned(const struct n64_hud_quad *q)
{
	return fabsf(q->y[0] - q->y[1]) < 0.5f && fabsf(q->y[2] - q->y[3]) < 0.5f &&
		fabsf(q->x[0] - q->x[3]) < 0.5f && fabsf(q->x[1] - q->x[2]) < 0.5f &&
		fabsf(q->s[0] - q->s[3]) < 0.01f && fabsf(q->s[1] - q->s[2]) < 0.01f &&
		fabsf(q->t[0] - q->t[1]) < 0.01f && fabsf(q->t[2] - q->t[3]) < 0.01f &&
		fabsf(q->s[1] - q->s[0]) > 0.01f && fabsf(q->t[3] - q->t[0]) > 0.01f;
}

/* an axis-aligned quad cut to the texels s0..s1, t0..t1 of its image, in
those texels' own (from s0, t0); 0 where none of it is inside */
static int cut_quad(const struct n64_hud_quad *in, struct n64_hud_quad *out, float s0, float t0, float s1, float t1)
{
	float sa = in->s[0], sb = in->s[1], ta = in->t[0], tb = in->t[3];
	float slo = max_f(min_f(sa, sb), s0), shi = min_f(max_f(sa, sb), s1);
	float tlo = max_f(min_f(ta, tb), t0), thi = min_f(max_f(ta, tb), t1);
	float na, nb, ma, mb;

	if (shi <= slo || thi <= tlo)
		return 0;
	na = sa < sb ? slo : shi;
	nb = sa < sb ? shi : slo;
	ma = ta < tb ? tlo : thi;
	mb = ta < tb ? thi : tlo;
	*out = *in;
	out->x[0] = out->x[3] = in->x[0] + (na - sa) / (sb - sa) * (in->x[1] - in->x[0]);
	out->x[1] = out->x[2] = in->x[0] + (nb - sa) / (sb - sa) * (in->x[1] - in->x[0]);
	out->y[0] = out->y[1] = in->y[0] + (ma - ta) / (tb - ta) * (in->y[3] - in->y[0]);
	out->y[2] = out->y[3] = in->y[0] + (mb - ta) / (tb - ta) * (in->y[3] - in->y[0]);
	out->s[0] = out->s[3] = na - s0;
	out->s[1] = out->s[2] = nb - s0;
	out->t[0] = out->t[1] = ma - t0;
	out->t[2] = out->t[3] = mb - t0;
	return 1;
}
#else
static sprite_t *hud_sprite(int number)
{
	char path[64];

	if (number < 0 || number >= MAXIMUM_SPRITES || hud.missing[number])
		return NULL;
	if (!hud.sprites[number])
	{
		int found = 0;
		unsigned long need = 0, image = 0;
		unsigned long recording_room = hud.recording >= 0 ? RECORDING_ROOM_PIECE * RECORDING_ROOM_PIECES : 0;

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
			made as it goes. For an image larger than HUD_LARGE_IMAGE the
			room is the map's before the converter's savings: the bytes its
			image left out, the heap's once the map's display is made, are not
			counted (n64_heap_has_held)) */
			image = sprite_bytes(file) + 1024;
			need = image + recording_room;
			room = image > HUD_LARGE_IMAGE ? n64_heap_has_held(need) : n64_heap_has(need);
			/* (the projectiles' module given back first, n64_particles.c) */
			if (!room && n64_projectiles_give_back())
				room = image > HUD_LARGE_IMAGE ? n64_heap_has_held(need) : n64_heap_has(need);
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
		/* (none had room: the smallest set's room asked for before this
		view's next recording, n64_hud_begin -- of the images refused, the
		one that asks least, which the push-out may make room for where a
		larger one could not; only an image of at most PUSH_OUT_LARGEST) */
		if (!hud.sprites[number] && need - recording_room <= PUSH_OUT_LARGEST && !hud.push_gave_up[number] &&
			hud.view >= 0 && hud.view < MAXIMUM_VIEWS &&
			(!hud.push_need[hud.view] || need < hud.push_need[hud.view]))
		{
			hud.push_need[hud.view] = need;
			hud.push_image[hud.view] = image;
			hud.push_pieces[hud.view] = recording_room ? RECORDING_ROOM_PIECES : 0;
			hud.push_sprite[hud.view] = number;
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
#endif

void n64_render_hud_phase(void);

/* HUD STYLE 64 drew with its own RDP modes, colours and TMEM loads
(n64_hud64.c): the render state this file keeps (set_state, the TMEM's
pixels) no longer holds -- anything drawn after it in the view sets it
again */
void n64_hud_state_lost(void)
{
	hud.mode = _mode_none;
	hud.tmem_pixels = NULL;
}

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

#if HUD_PACKING
/* (each frame, render_frame) */
void n64_hud_set_view_count(int count)
{
	if (count != hud.view_count)
		push_out_reset(0);
	hud.view_count = count;
	hud_frame++;
}
#else
void n64_hud_set_view_count(int count)
{
	if (count != hud.view_count)
		push_out_reset(0);
	hud.view_count = count;
}
#endif

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
	check_layout();
	/* with three or four views, each view's every sixth frame (the user's
	choice: the HUD's code is ~5 ms a recording, and four views made one a
	frame), the views' turns spread over the six (four views: frames 0, 1,
	3 and 4 of each six, not four in a row); with two, each view's every
	third */
	{
		int interval = n64_golden_selected(view) ? 1 : hud.view_count > 2 ? 6 : hud.view_count == 2 ? 3 : HUD_INTERVAL;
		int turn = hud.view_count > 0 ? view * interval / hud.view_count : view;

		/* (a view drawn live for live_next keeps its turn) */
		if (hud.recordings[view] && live_next_pending(view))
			hud.view_frames[view]++;
		if (hud.recordings[view] && !live_next_pending(view) && (hud.view_frames[view]++ + turn) % interval)
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
	/* (an image that found no room in this view's last recording: room for
	it made now, lower-tier models pushed out where they may be, before this
	one begins) */
	if (hud.push_need[view])
	{
		int sprite = hud.push_sprite[view];

		if (n64_render_push_out_for_hud(hud.push_image[view], hud.push_pieces[view], RECORDING_ROOM_PIECE) < 0 ||
			++hud.push_tries[sprite] >= PUSH_OUT_TRIES)
			hud.push_gave_up[sprite] = 1;
		hud.push_need[view] = 0;
	}
	if (live_next_take(view))
		hud.live = 1;
	else if (n64_render_make_room(RECORDING_ROOM_PIECE, RECORDING_ROOM_PIECES))
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

/* (HUD STYLE 64's module: the view's rectangle, for its scissor over Halo's
tracker -- n64_hud64_game.c) */
void n64_hud_view_rect(int rect[4])
{
	rect[0] = hud.x0;
	rect[1] = hud.y0;
	rect[2] = hud.x1;
	rect[3] = hud.y1;
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
#if HUD_PACKING
	struct meter_entry *entry = NULL;
	surface_t source = sprite_get_pixels(sprite);
	unsigned long need;
	unsigned minimum_alpha = quad->minimum_color >> 24, maximum_alpha = quad->flash_color >> 24;
	unsigned scale = channel(quad->tint_color, 0);
	unsigned empty_alpha = quad->background_color >> 24;
	unsigned strongest = 0;
	uint16_t empty;

	meter_clock++;
	for (int i = 0; i < meter_places; i++)
	{
		struct meter_entry *m = &meters[i];

		if (m->sprite == quad->sprite && m->s0 == s0 && m->t0 == t0 && m->width == width && m->height == height &&
			m->step == step &&
			m->minimum_color == quad->minimum_color && m->maximum_color == quad->maximum_color &&
			m->flash_color == quad->flash_color && m->background_color == quad->background_color &&
			m->tint_color == quad->tint_color)
		{
			m->last_used = meter_clock;
			m->drawn = hud_frame;
			return &m->surface;
		}
	}
	/* A new texture goes in a place no view still draws -- not drawn within
	the views' recording interval and a frame more -- the smallest such that
	it fits (rows of whole 8 bytes), the least recently used of those; a
	small texture takes a larger place where the small ones are all drawn
	still. Only where every place it fits is still drawn, the least recently
	used of them. None fits: the 8 KB places from the next view on
	(meter_layout), the meter left out until then */
	width = MIN(width, METER_SLOT_WIDTH);
	height = MIN(height, METER_SLOT_HEIGHT);
	need = (unsigned long)((width * 2 + 7) & ~7) * height;
	{
		unsigned long window = (unsigned long)(hud.view_count > 2 ? 6 : hud.view_count == 2 ? 3 : HUD_INTERVAL) + 1;
		struct meter_entry *oldest = NULL;

		for (int i = 0; i < meter_places; i++)
		{
			struct meter_entry *m = &meters[i];

			if (m->bytes < need)
				continue;
			if (!oldest || m->last_used < oldest->last_used)
				oldest = m;
			if ((m->sprite < 0 || hud_frame - m->drawn > window) && (!entry || m->bytes < entry->bytes ||
				(m->bytes == entry->bytes && m->last_used < entry->last_used)))
				entry = m;
		}
		if (!entry || !hud.packed_count)
			entry = oldest;
	}
	if (!entry)
	{
		if (meter_layout_in_use)
			meter_too_large = 1;
		return NULL;
	}
	entry->surface = surface_make(entry->place, FMT_RGBA16, width, height, (width * 2 + 7) & ~7);
	entry->drawn = hud_frame;
#else
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
		/* (only an entry with a slot: HUD STYLE 64 may have the block's last
		slots) */
		if (m->slot.buffer && (!oldest->slot.buffer || m->last_used < oldest->last_used))
			oldest = m;
	}
	entry = oldest;
	if (!entry->slot.buffer)
		return NULL;
	width = MIN(width, METER_SLOT_WIDTH);
	height = MIN(height, METER_SLOT_HEIGHT);
	entry->surface = surface_make_sub(&entry->slot, 0, 0, width, height);
#endif
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
	A row is made in cached memory, then copied into the place (uncached)
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
#if HUD_PACKING
	struct n64_hud_quad cropped;
	int packed = in->sprite >= 0 && in->sprite < MAXIMUM_SPRITES && hud.has_packed[in->sprite] ?
		packed_sprite(in->sprite,
			min_f(min_f(in->s[0], in->s[1]), min_f(in->s[2], in->s[3])),
			min_f(min_f(in->t[0], in->t[1]), min_f(in->t[2], in->t[3])),
			max_f(max_f(in->s[0], in->s[1]), max_f(in->s[2], in->s[3])),
			max_f(max_f(in->t[0], in->t[1]), max_f(in->t[2], in->t[3]))) : -1;

	/* (a quad inside a packed sprite's rectangle drawn from it, in its own
	texels) */
	if (packed >= 0)
	{
		cropped = *in;
		cropped.sprite = hud.packed[packed].sprite;
		for (int i = 0; i < 4; i++)
		{
			cropped.s[i] -= hud.packed[packed].s0;
			cropped.t[i] -= hud.packed[packed].t0;
		}
		in = &cropped;
	}
	/* (an image drawn whole from its packed box: an upright quad cut to the
	box, the rest of it clear; a turned one from the image's own sprite) */
	else if (in->sprite >= 0 && in->sprite < MAXIMUM_SPRITES && hud.whole[in->sprite] && axis_aligned(in))
	{
		int box = hud.whole[in->sprite] - 1;

		if (!cut_quad(in, &cropped, hud.packed[box].s0, hud.packed[box].t0, hud.packed[box].s1, hud.packed[box].t1))
			return;
		cropped.sprite = hud.packed[box].sprite;
		in = &cropped;
	}
#endif
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
