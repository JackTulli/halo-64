/*
N64_RENDER.C

The N64 renderer (n64.h): tiny3d on the RSP, libdragon's rdpq for the RDP.

The level is drawn from its converted render geometry (tools/n64/level.py:
rom:/levels/<map>.lvl): batches of at most 70 vertices with their
triangles, grouped by BSP cluster; a cluster outside a view's frustum is
skipped. Vertices are stored in the game's world units * 64 (the "N64
units" everything here is drawn in), already lit.
*/

#include <libdragon.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <rspq_constants.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>

#include "n64.h"

/* the screen: the game's 320x240, or the menu's 640x480 interlaced
(n64_render_set_menu_display), or the game's in more detail (Settings: 3D
RESOLUTION HIGH DETAIL, 640x240 at 240p, or ULTRA DETAIL, 480i where the map
leaves room: n64_render_set_high_detail) */
static int screen_width = 320, screen_height = 240;
enum { DISPLAY_GAME, DISPLAY_MENU, DISPLAY_GAME_HIGH };
static int display_mode = DISPLAY_GAME;
/* the detail asked for (N64_DETAIL_HIGH, N64_DETAIL_ULTRA, 0), tried for
this game yet, the frames left to say it didn't fit; the anamorphic picture
(Settings: WIDESCREEN) */
static int want_high_detail, tried_high_detail, notice_high_detail_frames;
static int anamorphic;
/* Settings: ANTI-ALIASING (the RDP's coverage on opaque surfaces, which the
VI's filter smooths the edges with) and DEDITHER (the VI rebuilding colour
from the 16-bit picture's dither; libdragon takes it at 320 across only
with the VI's anti-aliasing filter, which is on either way: without the
RDP's coverage it leaves the picture as it is) */
static int want_antialias, want_dedither;
/* Settings: HUD RESOLUTION HIGH DETAIL (n64_render_set_hud_detail): the
game's display at 640x240 at 240p, as high detail's, while its 3D stays at
320x240, drawn into a picture of its own (hud3d, with game_zbuf), and each
view's doubled across into the display before its HUD and text are drawn
there (hud_display_phase). The views are laid out in the 3D's pixels
(SCREEN_WIDTH is the 3D's while it is drawn), the HUD in the display's. */
static int want_hud_detail;
static surface_t hud3d;
/* the frame's display buffer; whether it is the RDP's target now (the
HUD's phase) rather than hud3d (the 3D's); the views already doubled into
it; the display's pixels for one of the 3D's; frames still to clear the
display first (a new display, or new views: what no view covers is black)
and the views it was cleared for */
static surface_t *hud_display;
static int hud_on_display, hud_views_doubled, hud_scale_x, hud_scale_y;
static int hud_clear_frames, hud_cleared_views;

static filter_options_t display_filters(void)
{
	return want_dedither ? FILTERS_RESAMPLE_ANTIALIAS_DEDITHER : FILTERS_RESAMPLE_ANTIALIAS;
}
#define SCREEN_WIDTH screen_width
#define SCREEN_HEIGHT screen_height
#define MAXIMUM_VIEWS 4
#define WORLD_SCALE 64.0f
/* an object smaller than this on screen is drawn in its far version */
#define FAR_PIXELS 24.0f
#ifndef GUARD_BAND_SCALE
#define GUARD_BAND_SCALE 4
#endif

enum
{
	MATERIAL_ALPHA_TEST = 1,
	MATERIAL_TWO_SIDED = 2,
	MATERIAL_TRANSPARENT = 4,
	MATERIAL_UNTEXTURED = 8,
	/* a model texture whose palette's upper half is a player's colour
	(tools/n64/level.py split_palette) */
	MATERIAL_CHANGE_COLOR = 16,
	/* a transparent model shader drawn as a cut-out glow, unlit */
	MATERIAL_GLOW = 32,
	/* a sky's batch: drawn first, without depth (tools/n64/ui.py) */
	MATERIAL_SKY = 64,
	/* natural terrain: the texture is a detail map about grey, the
	vertices carry the base map's colour (tools/n64/level.py) */
	MATERIAL_DETAIL = 128,
	/* a greyscale texture stretched to its full range, its vertices' light
	the stretch undone: Halo's doubling of texture x light in a second
	cycle (in the texture it turned bright textures white, on the vertices
	bright light dark) */
	MATERIAL_DOUBLE = 256,
	/* a natural material's detail map, drawn over it (record_overlay) */
	MATERIAL_OVERLAY = 512,
	/* Halo's water: untextured, one flat colour on its vertices, blended at
	WATER_ALPHA (tools/n64/level.py) */
	MATERIAL_WATER = 1024,
	/* a model glow of one emitted colour (a teleporter's energy), drawn
	translucent at GLOW_ALPHA (tools/n64/models.py glow_texture) */
	MATERIAL_BLEND = 2048,
	/* natural terrain whose colour map is in tiles, a batch's own with the
	light baked in (tools/n64/terrain_tiles.py): drawn tile x (0.5 +
	detail), with MATERIAL_DETAIL's texture its detail map */
	MATERIAL_TILED = 4096,
};

/* the water's opacity (MATERIAL_WATER) */
#define WATER_ALPHA 128
/* a translucent glow's (MATERIAL_BLEND) */
#define GLOW_ALPHA 112

/* ---------- the level file */

struct level_header
{
	char magic[4];
	uint32_t version;
	float scale;
	uint16_t texture_count, material_count, cluster_count, batch_count;
	uint32_t textures_offset, materials_offset;
	uint32_t clusters_offset, batches_offset;
	uint32_t vertices_offset, vertices_size;
	uint32_t indices_offset, indices_size;
	/* the clusters' owners: BSP cluster indices (level_cluster) */
	uint32_t owners_offset;
	uint16_t owner_total, pad;
	/* (version 5, a streamed level: its chunks, level_chunk) */
	uint32_t stream_offset;
};

/* A streamed level (version 5: a campaign level, too big to keep in memory
whole -- tools/n64/level.py) keeps its clusters' batches in chunks of near
ones, ~16 KB each: a chunk's bounds, its batches, and where their vertices
and indices are in the file (from vertices_offset and indices_offset). A
chunk is read when it comes into view and dropped, least recently drawn
first, past LEVEL_CACHE_BYTES (stream_chunk). At stream_offset: the chunk
count, each cluster's first chunk and chunk count, the chunks */
struct level_cluster_chunks
{
	uint16_t first, count;
};

struct level_chunk
{
	float minimum[3], maximum[3];
	uint16_t first_batch, batch_count;
	uint32_t vertex_offset, vertex_size;
	uint32_t index_offset, index_size;
};

/* a streamed level's chunks in memory at most (its slots, made when it
loads), and read in one frame at most (the rest the frames after: ~2 ms an
8 KB chunk); the heap the slots leave free (the HUD's recordings, the
game's own) */
#define LEVEL_CACHE_BYTES (320 * 1024)
#define LEVEL_READS_PER_FRAME 3
#define LEVEL_HEAP_MARGIN (128 * 1024)

struct level_texture
{
	char name[24];
	uint16_t width, height;
};

struct level_material
{
	int16_t texture;
	uint16_t flags;
};

/* a BSP cluster's surfaces, or those several clusters share (where they
cross between them): drawn when any of its owners -- BSP clusters -- is in
the eye's potentially visible set (none: always) */
struct level_cluster
{
	float minimum[3], maximum[3];
	uint16_t first_batch, batch_count;
	uint16_t first_owner, owner_count;
};

struct level_batch
{
	uint16_t material;
	uint8_t vertex_count, triangle_count;
	uint32_t vertex_pair_offset;
	uint32_t index_offset;
	int16_t minimum[3], maximum[3];
} __attribute__((packed));

static struct
{
	uint8_t *data;
	struct level_header *header;
	struct level_texture *textures;
	struct level_material *materials;
	struct level_cluster *clusters;
	uint16_t *owners;
	struct level_batch *batches;
	T3DVertPacked *vertices;
	/* each batch's triangle strips (draw_batch), chunks of tiny3d index
	buffers converted in place */
	uint8_t *indices;
	/* Large maps keep tables in RAM and stream visible geometry into a
	bounded cache. A fence precedes reuse while the RSP may still read it. */
	uint32_t geometry_rom, geometry_used;
	uint8_t *geometry_cache;
	uint32_t *geometry_offsets, *index_sizes;
	sprite_t **sprites;
	/* a view's batches in view, then in material order (draw_level) */
	uint16_t *visible, *sorted;
	uint16_t *material_starts;
	/* each material's render mode and texture, recorded once without fog
	and once with (set_material) */
	rspq_block_t **material_blocks[2];
	/* a big cluster's batches in groups near one another, each tested
	before its batches (build_level_groups); by cluster, its first group
	and their count (0: its batches tested one by one) */
	struct level_group *groups;
	uint16_t *group_batches;
	uint16_t *cluster_first_group, *cluster_group_count;
	/* by cluster, the box its batches' own boxes make together (N64 units,
	frustum_planes_inside) */
	int16_t (*cluster_boxes)[6];
	/* a streamed level (version 5): the file, its chunks, each chunk's data
	in memory (NULL: not read) and the frame it was last drawn in, each
	batch's chunk, the bytes in memory and read */
	FILE *stream;
	int chunk_count;
	struct level_cluster_chunks *cluster_chunks;
	struct level_chunk *chunks;
	uint8_t **resident;
	unsigned long *resident_drawn;
	uint16_t *batch_chunk;
	unsigned long resident_bytes, stream_reads;
	int stream_reads_this_frame;
	/* the chunks' memory: slots of the largest chunk's size, made when the
	level loads (stream_chunk), and each slot's chunk (-1: free) */
	uint8_t *slots;
	unsigned long slot_size;
	int slot_count;
	/* chunks in view but not in memory, this frame and the last (the FPS
	counter's) */
	int stream_waiting, stream_waited;
	int16_t *slot_chunk;
	int16_t *chunk_slot;
} level;

struct level_group
{
	float minimum[3], maximum[3];
	uint16_t first, count; /* in level.group_batches */
};

/* ---------- natural terrain's colour map in tiles */

/* Halo's natural ground is one big colour map (Blood Gulch's spans the
map) under detail maps repeating every metre or two. The RDP takes two
textures a pixel, so the colour map had been sampled onto the vertices,
seven units apart -- the grass, dirt and paths blurred into beige. Now each
terrain batch has a tile of the map of its own, its light baked in
(tools/n64/terrain_tiles.py, rom:/levels/<map>.tiles), drawn under one
level of its detail map (record_material, bind_tile). Small tiles, and the
big ones a quarter a side, stay in memory; a big tile is read into a slot
when the eye comes within TILE_NEAR_UNITS of its batch, TILE_READS_PER_FRAME
a frame at most (2 KB, ~0.5 ms), the one drawn longest ago giving its slot
up */
struct tiles_header
{
	char magic[4];
	uint32_t version;
	uint16_t entry_count, detail_level;
	uint32_t entries_offset;
	uint32_t resident_offset, resident_size;
	uint32_t big_offset, big_largest;
};

struct tile_entry
{
	uint16_t batch;
	/* the resident tile's shifts (a tile texel is 1 << shift detail
	texels) and size (log2 width << 4 | log2 height), then the big one's
	(size 0: none) */
	uint8_t shift_s, shift_t, size;
	uint8_t big_shift_s, big_shift_t, big_size;
	uint32_t offset, big_offset;
};

#define TILE_SLOTS 24
#define TILE_NEAR_UNITS 18.0f
#define TILE_READS_PER_FRAME 2
/* the heap the slots leave free at least (else fewer slots, or none: the
small tiles alone) */
#define TILE_HEAP_MARGIN (160 * 1024)

static struct
{
	/* the header and entries; the resident tiles; each level batch's entry
	(-1: none) */
	uint8_t *data;
	struct tiles_header *header;
	struct tile_entry *entries;
	uint8_t *resident;
	int16_t *batch_entry;
	/* the tile's place in TMEM: past the detail map's level */
	int tmem;
	float near_squared;
	/* the big tiles: the file, the slots (each slot's entry, each entry's
	slot, -1: none) and the frame each was last drawn in */
	FILE *file;
	uint8_t *slots;
	unsigned long slot_size;
	int slot_count;
	int16_t *slot_entry, *entry_slot;
	unsigned long *entry_drawn;
	int reads_this_frame;
	/* big tiles drawn, and near batches still waiting for theirs: this
	frame's and the last's (the FPS counter's) */
	int big_drawn, waiting, big_drawn_last, waiting_last;
} tiles;

/* Division first: malformed file offsets/counts cannot wrap validation. */
static int file_span(unsigned long bytes, unsigned long offset, unsigned long count, unsigned long width)
{
	return width && offset <= bytes && count <= (bytes - offset) / width;
}

static int convert_level_strips(uint8_t *strip, unsigned long bytes)
{
	for (;;)
	{
		if (bytes < 8) return 0;
		uint16_t count = ((uint16_t *)strip)[0], more = ((uint16_t *)strip)[1];
		unsigned long step = 8 + ((count * 2UL + 7) & ~7UL);
		if (step > bytes) return 0;
		t3d_indexbuffer_convert((int16_t *)(strip + 8), count);
		if (!more) return 1;
		strip += step; bytes -= step;
	}
}

static void unload_tiles(void)
{
	free(tiles.data);
	free(tiles.resident);
	free(tiles.batch_entry);
	free(tiles.entry_slot);
	free(tiles.entry_drawn);
	free(tiles.slots);
	free(tiles.slot_entry);
	if (tiles.file) fclose(tiles.file);
	memset(&tiles, 0, sizeof(tiles));
}

static unsigned long tile_bytes(unsigned code)
{
	unsigned w = code >> 4, h = code & 15;
	/* The converter's 4..64 sides, at most1024 RGBA16 texels. */
	return w >= 2 && w <= 6 && h >= 2 && h <= 6 && w + h <= 10 ? 2UL << (w + h) : 0;
}

/* ---------- the models file (tools/n64/models.py) */

#define MODEL_SCALE 1024.0f
#define MATRICES_PER_FRAME 640
#define FRAMES_IN_FLIGHT 3

struct models_header
{
	char magic[4];
	uint32_t version;
	uint16_t texture_count, model_count, region_count, permutation_count;
	uint16_t geometry_count, batch_count, segment_count, pad;
	uint32_t textures_offset, models_offset, regions_offset, permutations_offset;
	uint32_t geometries_offset, batches_offset, segments_offset;
	uint32_t vertices_offset, indices_offset;
};

struct model_record
{
	uint16_t tag_index;
	uint8_t node_count, region_count;
	uint16_t first_region, pad;
	uint16_t first_batch, batch_count;
	/* the model's vertices, then its indices: rom:/mdl/<data_name>.bin
	(shared by content) */
	char data_name[16];
	uint32_t vertex_bytes, index_bytes;
	/* its far version's batches (after first_batch), or none */
	uint16_t far_first, far_count;
};
struct region_record { uint16_t permutation_count, first_permutation; };
struct geometry_record { uint16_t first_batch, batch_count; };
struct model_batch
{
	int16_t texture;
	uint16_t flags;
	uint8_t segment_count, triangle_count;
	uint16_t first_segment;
	uint32_t index_offset;
};
struct model_segment { uint8_t node, slot, count, pad; uint32_t vertex_pair_offset; };

/* Models and their textures are read from the cartridge when first drawn
and dropped, least recently drawn first, past this size (a model drawn in
the last frames in flight stays). A texture stays while a loaded model
uses it: each batch's texture and render mode are recorded with it. */
#define MODEL_CACHE_BYTES (448 * 1024)
/* heap a model load leaves free (the game's own allocations, rspq's) */
#define MODEL_HEAP_MARGIN (96 * 1024)

struct loaded_model
{
	/* its vertices and indices; each batch's setup (render mode, texture)
	and its loads (node matrices from the segment the object sets) and
	triangles, recorded once */
	uint8_t *data;
	rspq_block_t **setups, **blocks;
	unsigned long last_drawn;
	/* the frame it was last drawn in a first-person view (0: never) */
	unsigned long first_person_drawn;
};

static struct
{
	uint8_t *data; /* the tables */
	struct models_header *header;
	struct level_texture *textures;
	struct model_record *models;
	struct region_record *regions;
	int16_t *permutations;
	struct geometry_record *geometries;
	struct model_batch *batches;
	struct model_segment *segments;
	char path[64], map_name[32];
	struct loaded_model *loaded;
	unsigned long loaded_bytes;
	sprite_t **sprites;
	unsigned short *texture_users;
	unsigned long *texture_size;
	unsigned long *texture_allocation;
	unsigned long texture_bytes;
	/* models that found no room, this frame and the last (drawn as nothing:
	the FPS counter's) */
	int missing, missing_last;
	/* tag index -> model record, or NULL */
	struct model_record **by_tag;
	int tag_count;
	unsigned long reads, texture_reads;
	/* by batch: the batches alike from it on (RUN_LENGTH) and whether a run
	starts there (RUN_HEAD) -- see mark_batch_runs */
	uint8_t *runs;
} models;

#define RUN_HEAD 0x80
#define RUN_LENGTH 0x7F

#define SEGMENT_NODE_MATRICES T3D_SEGMENT_1

/* debug switches: files on the cartridge (port.json "flags") */
static int debug_overview;
/* debugging: wait for the RSP and RDP after each part of a view, so that
the parts' times include the hardware's (and the frame is slower) */
static int debug_gpu_time;
/* debugging (port.json flag "debug_rdpq_log=<frame>", with "debug_rdpq"):
the RDP commands of that frame and the next in the log, with where they
are in memory and markers at the frame's start, its depth clears and its
end */
static unsigned long debug_rdpq_log_frame;

static void gpu_time_sync(void)
{
	if (debug_gpu_time)
	{
		rdpq_fence();
		rspq_wait();
	}
}

/* the parts of a view, timed: the time since the last change goes to the
part that was under way (with debug_gpu_time, once the RSP and RDP are
done with it) */
enum { PHASE_NONE, PHASE_LEVEL, PHASE_OBJECTS, PHASE_FIRST_PERSON, PHASE_HUD };
static void render_phase(int phase);
static int debug_untextured;
/* measuring: the level's front faces culled (what transforming and
rejecting its triangles costs) */
static int debug_cull_front;
/* measuring: 1, the level's vertices without its triangles; 2, its
materials without its batches */
static int debug_level_parts;
/* measuring the fog: 1, nothing left out past it; 2, no fog; 3, fog far
away */
static int debug_fog_test;

static struct
{
	T3DViewport viewports[MAXIMUM_VIEWS];
	int view_count;
	surface_t *surface;
	unsigned long frame_index;
	unsigned long triangles, batches, clusters, objects;
	/* microseconds of CPU in the level, objects and first-person models,
	and frames, over the statistics window */
	unsigned long long level_ticks, object_ticks, first_person_ticks, draw_ticks, window_start;
	unsigned long long hud_ticks;
	int phase;
	unsigned long phase_start;
	int first_person;
	/* the view's eye (world units) and how many pixels a unit at distance
	one covers, for leaving out objects too small to see */
	float eye[3], pixels_per_unit;
	color_t clear_color;
	T3DMat4FP *matrices[FRAMES_IN_FLIGHT];
	int matrix_count;
	T3DViewport *viewport;
	int current_texture;
	float fov;
	/* each view's rectangle, for the crosshairs drawn after the 3D */
	short rects[MAXIMUM_VIEWS][4];
	/* the FPS counter: frames shown in the last second */
	unsigned long long fps_window_start;
	int fps_frames;
	int fps;
	int hide_counter;
	/* chunky mode's level (0 off, 1 medium, 2 chunky): as asked
	(n64_render_set_chunky), and in this frame; whether the current view's
	narrower picture is still to be widened */
	int want_chunky, chunky;
	int unstretched;
	/* the current view's width over its height */
	float aspect;
	/* the views this frame (n64_render_set_view_count) */
	int view_total;
	/* the draw distance and its fog (n64_render_set_fog) */
	int fog;
	color_t fog_color;
	float fog_start, fog_end;
	/* the current view's camera (for its particles): its axes, near and far
	planes, its local player, whether its world particles are drawn */
	float right[3], up[3], forward[3];
	float z_near, z_far;
	int local_player;
	int world_particles_drawn;
} renderer;

/* the game's last tick, in microseconds (n64_game.c), for the counter */
unsigned long n64_last_tick_us;
/* the CPU's time in the last frame's drawing, from the frame's start to its
end, and the RSP/RDP queue's wait inside n64_render_frame_end */
static unsigned long render_start_ticks, render_cpu_us, render_wait_us;

void n64_render_set_clear_color(unsigned long rgb)
{
	renderer.clear_color = RGBA32((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF, 0xFF);
}

static void load_models(const char *map_name);
static void flush_draws(void);

static int view_model_quality;
/* Settings: VIEW MODEL "OFF IN SPLIT": the low models, not drawn with three
or four views. Drawing them was the dearest part of a four-player frame
(Blood Gulch in ares: 16.8 fps drawn, 24.6 posed but not drawn) -- posed
all the same: the game takes a local player's shots from its first-person
weapon's markers */
static int view_models_hidden_in_split;

void n64_render_set_view_model_quality(int quality)
{
	view_models_hidden_in_split = quality == 3;
	view_model_quality = quality < 0 || quality == 3 ? 0 : quality > 2 ? 2 : quality;
}

int n64_render_view_models_hidden(void)
{
	return view_models_hidden_in_split && renderer.view_total > 2;
}

/* the game's views (objects.c: with three or four, every biped's pose
every other tick) */
int n64_view_total;

void n64_render_set_view_count(int count)
{
	renderer.view_total = count;
	n64_view_total = count;
	n64_particles_set_view_count(count);
}

void n64_render_set_fog(unsigned long rgb, float start, float end)
{
	/* (measuring, "debug_fog_test": 2 no fog, 3 fog far away) */
	if (debug_fog_test == 2)
		end = 0.f;
	if (debug_fog_test == 3 || debug_fog_test == 5)
	{
		start = 1000.f;
		end = 2000.f;
	}
	renderer.fog = end > 0.f;
	renderer.fog_color = RGBA32((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF, 0xFF);
	renderer.fog_start = start;
	renderer.fog_end = end;
}

/* ---------- the sky behind a view (n64_render_set_sky) */

static const float sky_stop_degrees[N64_SKY_STOPS] = {0.f, 15.f, 30.f, 50.f, 90.f};
static float sky_colors[N64_SKY_STOPS][3];
static int sky_set;

void n64_render_set_sky(const unsigned long *colors)
{
	sky_set = colors != NULL;
	for (int i = 0; colors && i < N64_SKY_STOPS; i++)
	{
		sky_colors[i][0] = ((colors[i] >> 16) & 0xFF) / 255.f;
		sky_colors[i][1] = ((colors[i] >> 8) & 0xFF) / 255.f;
		sky_colors[i][2] = (colors[i] & 0xFF) / 255.f;
	}
}

/* the sky's colour at a height above the horizon (degrees) */
static void sky_color_at(float degrees, float *rgb)
{
	int i = 0;
	float f;

	if (degrees <= sky_stop_degrees[0])
		degrees = sky_stop_degrees[0];
	while (i < N64_SKY_STOPS - 2 && degrees > sky_stop_degrees[i + 1])
		i++;
	f = (degrees - sky_stop_degrees[i]) / (sky_stop_degrees[i + 1] - sky_stop_degrees[i]);
	if (f > 1.f)
		f = 1.f;
	for (int k = 0; k < 3; k++)
		rgb[k] = sky_colors[i][k] + (sky_colors[i + 1][k] - sky_colors[i][k]) * f;
}

/* The sky from the horizon up, in strips of the view whose rows take the
sky's colour at their height (the eye's up is the world's, but for a
vehicle's tilt). Below the horizon the frame is cleared to the fog's
colour, the sky's at the horizon (the game's table: n64_render_game.c),
so the level fades into it where it ends */
#define SKY_STRIPS 12

static int chunky_width(int width);

static void draw_sky(const struct n64_view *view)
{
	float x0 = (float)view->x0, x1 = (float)(view->x0 + chunky_width(view->x1 - view->x0));
	float y0 = (float)view->y0, y1 = (float)view->y1;
	float middle = (y0 + y1) * 0.5f;
	float rows[SKY_STRIPS + 1], degrees[SKY_STRIPS + 1], colors[SKY_STRIPS + 1][3];

	if (!sky_set || !renderer.fog || renderer.pixels_per_unit <= 0.f)
		return;
	for (int k = 0; k <= SKY_STRIPS; k++)
	{
		/* the row's direction: forward, and up by its distance from the
		view's middle over the pixels a unit away */
		float t = (middle - (y0 + (y1 - y0) * k / SKY_STRIPS)) / renderer.pixels_per_unit;
		float sine = (renderer.forward[2] + renderer.up[2] * t) / sqrtf(1.f + t * t);

		rows[k] = y0 + (y1 - y0) * k / SKY_STRIPS;
		degrees[k] = asinf(sine > 1.f ? 1.f : sine < -1.f ? -1.f : sine) * (180.f / (float)M_PI);
		sky_color_at(degrees[k], colors[k]);
	}
	rdpq_mode_push();
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
	rdpq_mode_dithering(DITHER_SQUARE_SQUARE);
	for (int k = 0; k < SKY_STRIPS; k++)
	{
		float *a = colors[k], *b = colors[k + 1];
		float v0[6] = {x0, rows[k], a[0], a[1], a[2], 1.f};
		float v1[6] = {x1, rows[k], a[0], a[1], a[2], 1.f};
		float v2[6] = {x0, rows[k + 1], b[0], b[1], b[2], 1.f};
		float v3[6] = {x1, rows[k + 1], b[0], b[1], b[2], 1.f};

		/* (all below the horizon: the clear colour is the sky there) */
		if (degrees[k] <= 0.f && degrees[k + 1] <= 0.f)
			continue;
		rdpq_triangle(&TRIFMT_SHADE, v0, v1, v2);
		rdpq_triangle(&TRIFMT_SHADE, v1, v3, v2);
	}
	rdpq_mode_pop();
}

/* the view's fog for its current projection (tiny3d measures it in the
projection's depth) */
static void apply_fog(void)
{
	if (!renderer.fog)
		return;
	t3d_fog_set_range(renderer.fog_start * WORLD_SCALE, renderer.fog_end * WORLD_SCALE);
	t3d_fog_set_enabled(true);
	rdpq_set_fog_color(renderer.fog_color);
}

/* 0 the whole width, 1 medium (the 3D at four fifths of the view's width:
256 of 320, then widened), 2 chunky (half, each pixel doubled) */
void n64_render_set_chunky(int level)
{
	renderer.want_chunky = level < 0 ? 0 : level > 2 ? 2 : level;
}

/* the width the current level draws a view's 3D at */
static int chunky_width(int width)
{
	return renderer.chunky == 2 ? width / 2 : renderer.chunky == 1 ? width * 4 / 5 : width;
}

void n64_render_show_counter(int on)
{
	renderer.hide_counter = !on;
}

/* chunky mode: a view's 3D is drawn into the left half of its rectangle,
then each of its pixels doubled across the whole rectangle, before its HUD
(drawn at full width); medium, into its left four fifths (256 of 320), then
widened by a quarter (point sampled: a column in four shown twice). The RDP
widens it in place, in strips from the right: a strip's pixels land at or
right of where they are read, over strips already widened */
static void stretch_view(void)
{
	const short *rect;
	surface_t picture;
	int x0, y0, x1, y1, half;
	float widen;

	if (!renderer.unstretched)
		return;
	renderer.unstretched = 0;
	rect = renderer.rects[renderer.view_count - 1];
	x0 = rect[0], y0 = rect[1], x1 = rect[2], y1 = rect[3];
	/* the width drawn, and how much wider it is shown (chunky 2, medium
	1.25): each strip still lands at or right of where it was read */
	half = chunky_width(x1 - x0);
	if (half <= 0)
		return;
	widen = renderer.chunky == 2 ? 2.f : (float)(x1 - x0) / (float)half;
	/* the RDP's writes of the 3D reach memory before it reads them back
	(the pipeline drained; a full sync would also stall the RSP) */
	rdpq_sync_pipe();
	rdpq_set_scissor(x0, y0, x1, y1);
	rdpq_set_mode_standard();
	rdpq_mode_filter(FILTER_POINT);
	picture = surface_make_sub(renderer.surface, 0, 0, renderer.surface->width, renderer.surface->height);
	for (int s = x0 + half; s > x0;)
	{
		int width = s - x0 < 32 ? s - x0 : 32;

		s -= width;
		rdpq_tex_blit(&picture, x0 + widen * (s - x0), y0, &(rdpq_blitparms_t){
			.s0 = s, .t0 = y0, .width = width, .height = y1 - y0, .scale_x = widen,
		});
	}
	renderer.current_texture = -2;
}

/* HUD RESOLUTION HIGH DETAIL: the 3D's pixels in a rectangle of hud3d
doubled across into the display (point sampled, as stretch_view widens) */
static void hud_double(int x0, int y0, int x1, int y1)
{
	if (x1 <= x0 || y1 <= y0)
		return;
	rdpq_set_scissor(x0 * hud_scale_x, y0 * hud_scale_y, x1 * hud_scale_x, y1 * hud_scale_y);
	rdpq_tex_blit(&hud3d, x0 * hud_scale_x, y0 * hud_scale_y, &(rdpq_blitparms_t){
		.s0 = x0, .t0 = y0, .width = x1 - x0, .height = y1 - y0,
		.scale_x = (float)hud_scale_x, .scale_y = (float)hud_scale_y,
	});
}

/* HUD RESOLUTION HIGH DETAIL: the display the RDP's target, for a view's
HUD (and the frame's last 2D), the views drawn since the last time doubled
across into it first (a frame of none: all of hud3d, its clear colour) */
static void hud_display_phase(void)
{
	int first = !hud_display;

	if (!hud3d.buffer || hud_on_display)
		return;
	if (first)
	{
		/* (the frame's buffer: waited for here, its wait counted) */
		unsigned long before = TICKS_READ();

		hud_display = display_get();
		render_wait_us += TICKS_TO_US(TICKS_READ() - before);
	}
	/* (hud3d's last pixels in memory before they are read: stretch_view) */
	rdpq_sync_pipe();
	rdpq_set_color_image(hud_display);
	if (first && hud_clear_frames)
	{
		/* (each of the display's two buffers once, at the frame's first
		HUD phase: black where no view is doubled; a later phase of the
		same frame would clear what its views and HUD drew) */
		hud_clear_frames--;
		rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
		rdpq_fill_rectangle(0, 0, hud_display->width, hud_display->height);
	}
	rdpq_set_mode_standard();
	rdpq_mode_filter(FILTER_POINT);
	if (!renderer.view_count)
		hud_double(0, 0, hud3d.width, hud3d.height);
	for (int v = hud_views_doubled; v < renderer.view_count; v++)
		hud_double(renderer.rects[v][0], renderer.rects[v][1], renderer.rects[v][2], renderer.rects[v][3]);
	hud_views_doubled = renderer.view_count;
	renderer.current_texture = -2;
	renderer.surface = hud_display;
	screen_width = hud_display->width;
	screen_height = hud_display->height;
	hud_on_display = 1;
}

/* and hud3d the target again, for the next view's 3D */
static void hud_3d_phase(void)
{
	if (!hud3d.buffer || !hud_on_display)
		return;
	rdpq_sync_pipe();
	rdpq_set_color_image(&hud3d);
	renderer.surface = &hud3d;
	screen_width = hud3d.width;
	screen_height = hud3d.height;
	hud_on_display = 0;
}

void n64_render_hud_end(void)
{
	hud_3d_phase();
}

/* a view's rectangle, laid out in the 3D's pixels, in the display's (where
its HUD is drawn): the same unless the HUD is at 640x240 over the 3D's
320x240 */
void n64_render_hud_rect(int *x0, int *y0, int *x1, int *y1)
{
	if (!hud3d.buffer)
		return;
	*x0 *= hud_scale_x;
	*x1 *= hud_scale_x;
	*y0 *= hud_scale_y;
	*y1 *= hud_scale_y;
}

/* a scene's models without a level (the menu's: levels/<name>.mdl) */
void n64_render_load_scene(const char *name)
{
	n64_render_unload_level();
	load_models(name);
}

/* how far the CPU may run ahead of the RSP: each of the low-priority
queue's two buffers, in words (libdragon's is 512, patched to be ours:
port/n64/devtools/libdragon-rspq-buffer.patch). With 512 the CPU waited
for the RSP mid-frame instead of going on to the next ticks: 2048 (16 KB
both) let it, 4096 was no faster then -- with the level drawn as strips
(more commands a frame) 4096 is: Blood Gulch 4 players 12.5 -> 13.2 fps,
16 KB more. */
int rspq_lowpri_buffer_words = 0x1000;

/* ---------- the display: the game's 320x240, or the menu's 640x480
interlaced (n64_render_set_menu_display) */

/* the game's: three 320x240 buffers (and the z-buffer, which libdragon
puts at the top of the heap the first time a frame asks for it) */
#define GAME_DISPLAY_BYTES (4 * 320 * 240 * 2 + 1024) /* three buffers and the z-buffer */

/* the game's 320x240 z-buffer, its own, made with the buffers (below) */
static surface_t game_zbuf;
static int home_in_use;

/* the game's z-buffer when the heap had no 150 KB in one piece for it (issue
3: "no memory for the z-buffer" between the menu and the campaign's level,
on a cartridge built from another copy of the game): the model cache's
oldest models dropped, then the maps' home if no map is in it (between the
menu and the level none is; the level's load closes the display before its
image goes there, n64_render_prepare_map_image) */
static surface_t game_zbuf_made_room(void)
{
	unsigned long size = 320 * 240 * 2 + 64, available;
	surface_t zbuf = {0};
	void *home;

	if (n64_render_make_room(size, 1))
		zbuf = surface_alloc(FMT_RGBA16, 320, 240);
	home = n64_map_home_free_part(&available);
	if (!zbuf.buffer && home && available >= size)
	{
		n64_display_lend(home, available);
		zbuf = surface_alloc(FMT_RGBA16, 320, 240);
		n64_display_lend(NULL, 0);
		if (zbuf.buffer)
			home_in_use = 1;
	}
	debugf("n64: no room for the z-buffer on the heap (%lu KB free); %s\n", n64_heap_free() / 1024,
		!zbuf.buffer ? "none found" : home_in_use ? "made in the maps' home" : "made by dropping models");
	return zbuf;
}

/* The game's display, and its z-buffer made right after its three buffers,
in the same place (the menu keeps that place for them: GAME_DISPLAY_BYTES).
libdragon's display_get_zbuf took it from the heap's top or, failing that,
the heap -- and failing both gave a z-buffer at address 0 without a word:
the loading frame after the menu, drawn while the menu's things (its logo,
its scene) were still in memory, cleared and drew its depth over the first
150 KB of RAM, the game's code (every map then crashed in the first
function it ran from there, bipeds_dispose_from_old_map) */
static void game_display_init(void)
{
	static int first = 1;
	unsigned long available;
	void *home = n64_map_home_free_part(&available);
	int buffers = available == 0 ? 2 : 3;
	if (first && home && available >= GAME_DISPLAY_BYTES)
	{
		n64_display_lend(home, available);
		home_in_use = 1;
	}
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, buffers, GAMMA_NONE, display_filters());
	if (game_zbuf.buffer)
		surface_free(&game_zbuf);
	game_zbuf = surface_alloc(FMT_RGBA16, 320, 240);
	n64_display_lend(NULL, 0);
	if (!game_zbuf.buffer)
		game_zbuf = game_zbuf_made_room();
	first = 0;
	screen_width = 320;
	screen_height = 240;
	/* (the HUD 320 across: the half-size sprites) */
	n64_hud_set_wide(0, 0);
}

/* the display closed, the game's z-buffer with it */
static void game_display_close(void)
{
	display_close();
	if (game_zbuf.buffer)
		surface_free(&game_zbuf);
	game_zbuf = (surface_t){0};
	if (hud3d.buffer)
		surface_free(&hud3d);
	hud3d = (surface_t){0};
}

/* while the menu runs, the place the game's buffers had, kept for them (so
that a game's heap is laid out as it was) */
static void *game_display_place;

/* the menu's z-buffer, its own (libdragon's display_get_zbuf would take it
from the heap's top): with its buffers in the maps' home, which has no map
while the menu runs (n64_display_lend) */
static surface_t menu_zbuf;
/* display buffers in the maps' home now: no map may go there */

int n64_render_home_in_use(void)
{
	return home_in_use;
}

void n64_render_prepare_map_image(void)
{
	rspq_wait();
	game_display_close();
	home_in_use = 0;
	n64_render_unload_level();
}

void n64_render_finish_map_image(void)
{
	game_display_init();
	display_mode = DISPLAY_GAME;
}

void n64_render_set_menu_display(int on)
{
	heap_stats_t heap;

	on = on != 0;
	if (on == (display_mode == DISPLAY_MENU))
		return;
	/* the RSP and the RDP done with the buffers, the deferred calls made
	(the last frame given to the display) */
	rspq_wait();
	/* (frees the buffers and the z-buffer; black until the next frame) */
	game_display_close();
	if (menu_zbuf.buffer)
		surface_free(&menu_zbuf);
	menu_zbuf = (surface_t){0};
	home_in_use = 0;
	if (on)
	{
		unsigned long home_size;
		/* All borrowed display buffers have closed. Give the title the
		unused tail of a large map back before its transient allocations. */
		n64_map_boot_complete();
		void *home = n64_map_home_free_part(&home_size);

		game_display_place = malloc(GAME_DISPLAY_BYTES);
		/* each frame shown as both fields (INTERLACE_FULL: a whole 640x480
		picture), two buffers: 2 x 600 KB, and the z-buffer's 600 KB -- in
		the maps' home, which has no map now, else from the heap. (From the
		heap they and what the menu made after them left blocks in the
		middle of the memory the next map wanted: Sidewinder's, Blood
		Gulch's.) */
		if (home && home_size >= 3 * (640 * 480 * 2 + 64))
		{
			n64_display_lend(home, home_size);
			home_in_use = 1;
		}
		display_init((resolution_t){.width = 640, .height = 480, .interlaced = INTERLACE_FULL}, DEPTH_16_BPP, 2,
			GAMMA_NONE, display_filters());
		if (home_in_use)
		{
			menu_zbuf = surface_alloc(FMT_RGBA16, 640, 480);
			n64_display_lend(NULL, 0);
		}
		{
			/* (testing, "debug_heap_dump": what the game left behind, past
			the maps' home) */
			unsigned long home_size;
			char *home = n64_map_home_free_part(&home_size);

			n64_heap_dump("at the menu", home ? home + home_size : NULL);
		}
		screen_width = 640;
		screen_height = 480;
		display_mode = DISPLAY_MENU;
		/* (the menu's scene as it is: the game sets its own) */
		anamorphic = 0;
	}
	else
	{
		free(game_display_place);
		game_display_place = NULL;
		game_display_init();
		display_mode = DISPLAY_GAME;
		/* (high detail, if asked, once the map is in:
		n64_render_frame_begin) */
		tried_high_detail = 0;
	}
	sys_get_heap_stats(&heap);
	debugf("n64: display %dx%d%s, heap %d KB free\n", screen_width, screen_height, on ? " interlaced" : "",
		heap.free / 1024);
}

/* how far the game's views show anything: the fog's end where there is
fog, else the far plane (n64_render_game.c's 200) -- effects.c leaves out
what no view could show */
float n64_render_visible_range(void)
{
	return renderer.fog ? renderer.fog_end : 200.f;
}

int n64_render_screen_width(void)
{
	return screen_width;
}

/* ---------- high detail in the game, and the anamorphic picture */

void n64_render_set_high_detail(int level)
{
	want_high_detail = level >= N64_DETAIL_ULTRA ? N64_DETAIL_ULTRA : level > 0 ? N64_DETAIL_HIGH : 0;
}

void n64_render_set_hud_detail(int on)
{
	want_hud_detail = on != 0;
}

void n64_render_set_anamorphic(int on)
{
	anamorphic = on != 0;
}

/* Settings: DEDITHER, from the next display made (the game's at a map's
start, the menu's when it comes back) */
void n64_render_set_dedither(int on)
{
	want_dedither = on != 0;
}

/* the game lays its views and HUD out on a 640x480 screen: the N64's
pixels for one of its units, across and down */
float n64_render_layout_scale_x(void)
{
	return screen_width * (1.f / 640.f);
}

float n64_render_layout_scale_y(void)
{
	return screen_height * (1.f / 480.f);
}

/* the HUD's squeeze about each view's middle: with the anamorphic picture
(a 16:9 picture squeezed into the 4:3 one, the television stretching it
back) the HUD keeps its proportions and its 4:3 place in the middle; the
game's own projections onto the screen (nav points) land where the wider
3D puts them */
float n64_render_hud_squeeze(void)
{
	return anamorphic ? 0.75f : 1.f;
}

/* the width a pixel shows at (the screen 4:3 whatever its size: 512x480's
are wider), and with the anamorphic picture a third wider again */
static float display_pixel_aspect(void)
{
	return (640.f / screen_width) * (screen_height / 480.f) * (anamorphic ? 4.f / 3.f : 1.f);
}

static void drop_all_models(void);
static void record_model_modes(void);

/* room for count blocks of the display (its buffers and z-buffer, 16 bits
a pixel) and the models the game draws (the cache is emptied first: it
refills) */
#define ROOM_FOR_MODELS (448 * 1024)

static int display_fits(int width, int height, int count)
{
	unsigned long size = (unsigned long)width * height * 2 + 256;
	void *blocks[5] = {NULL, NULL, NULL, NULL, NULL};
	int fits = 1;

	count = count < 4 ? count : 4;
	for (int i = 0; i < count && fits; i++)
		fits = (blocks[i] = malloc(size)) != NULL;
	if (fits)
		fits = (blocks[4] = malloc(ROOM_FOR_MODELS)) != NULL;
	for (int i = 0; i < 5; i++)
		free(blocks[i]);
	return fits;
}

/* the room for the HUD's full-width sprites once a display is made: the
heap less the models' room (as display_fits leaves it) */
static unsigned long hud_sprite_room(void)
{
	unsigned long free_bytes = n64_heap_free();

	return free_bytes > ROOM_FOR_MODELS ? free_bytes - ROOM_FOR_MODELS : 0;
}

/* HUD RESOLUTION HIGH DETAIL: the display's two 640x240 buffers (300 KB
each), then the 3D's 320x240 picture and its z-buffer (150 KB each): in the
maps' home past the map's image, each that still fits there
(n64_display_lend), the others from the heap, with room left for the models
(as display_fits). 1 if made. */
static int try_hud_detail(void)
{
	int width = 320, height = 240;
	unsigned long sizes[4] = {640 * 240 * 2 + 256, 640 * 240 * 2 + 256,
		(unsigned long)width * height * 2 + 256, (unsigned long)width * height * 2 + 256};
	void *blocks[5] = {NULL, NULL, NULL, NULL, NULL};
	unsigned long home_size, room;
	void *home = n64_map_home_free_part(&home_size);
	int fits = 1, in_home = 0;
	heap_stats_t heap;

	room = home ? home_size : 0;
	for (int i = 0; i < 4 && fits; i++)
	{
		if (sizes[i] <= room)
			room -= sizes[i], in_home++;
		else
			fits = (blocks[i] = malloc(sizes[i])) != NULL;
	}
	if (fits)
		fits = (blocks[4] = malloc(ROOM_FOR_MODELS)) != NULL;
	for (int i = 0; i < 5; i++)
		free(blocks[i]);
	if (!fits)
		return 0;
	if (in_home)
	{
		n64_display_lend(home, home_size);
		home_in_use = 1;
	}
	/* (240p, as high detail: no interlaced picture for the HUD; 480i is
	ultra detail's alone) */
	display_init((resolution_t){.width = 640, .height = 240, .interlaced = INTERLACE_OFF}, DEPTH_16_BPP, 2,
		GAMMA_NONE, display_filters());
	hud3d = surface_alloc(FMT_RGBA16, width, height);
	game_zbuf = surface_alloc(FMT_RGBA16, width, height);
	n64_display_lend(NULL, 0);
	if (!hud3d.buffer || !game_zbuf.buffer)
	{
		game_display_close();
		home_in_use = 0;
		return 0;
	}
	screen_width = width;
	screen_height = height;
	hud_scale_x = 640 / width;
	hud_scale_y = 240 / height;
	hud_clear_frames = 2;
	display_mode = DISPLAY_GAME_HIGH;
	n64_hud_forget_recordings();
	/* (the HUD 640 across: its full-width sprites, where there is room) */
	n64_hud_set_wide(1, hud_sprite_room());
	sys_get_heap_stats(&heap);
	debugf("n64: HUD 640x240 over 3D %dx%d, %d of 4 surfaces in the maps' home, heap %d KB free\n", width, height,
		in_home, heap.free / 1024);
	return 1;
}

/* HUD RESOLUTION HIGH DETAIL asked for (over a 320x240 3D): whether the
memory it needs is there at all, the game's display (if on the heap) and the
models' cache counted as freed (try_hud_detail then says whether it fits in
the pieces there are). Without it nothing is touched, the game as without
the setting (the display closed and made again on a map that left little
room found no z-buffer: "no memory for the z-buffer", the PC cartridge's
Infinity) */
static int hud_detail_may_fit(void)
{
	unsigned long home_size;
	unsigned long needed = 2 * (640UL * 240 * 2 + 256) + 2 * (320UL * 240 * 2 + 256) + ROOM_FOR_MODELS;

	n64_map_home_free_part(&home_size);
	return n64_heap_free() + models.loaded_bytes + models.texture_bytes + (home_in_use ? 0 : GAME_DISPLAY_BYTES) +
		home_size >= needed;
}

/* The game's display in more detail, the first of these the map leaves
room for. Ultra detail starts at 480i -- 640x480 (two buffers of 600 KB and
the z-buffer's 600 KB, where 320x240 took 600 KB in all), else 512x480 (480
KB each, stretched to the same 4:3), each frame shown as both fields
(INTERLACE_FULL, as the menu); high detail at Perfect Dark's high
resolution, 640x240 at 240p: twice the pixels across, the same lines (300
KB each, three buffers or two), else 520x240, else 480x240. Neither
fitting: 320x240 as it was and a word on the screen. */
#define FIRST_HIGH_DETAIL_MODE 2
static void try_high_detail(void)
{
	static const struct
	{
		short width, height, interlaced, buffers;
	} modes[] = {
		{640, 480, 1, 2}, {512, 480, 1, 2}, {640, 240, 0, 3}, {640, 240, 0, 2},
		{520, 240, 0, 3}, {520, 240, 0, 2}, {480, 240, 0, 3}, {480, 240, 0, 2},
	};
	heap_stats_t heap;

	tried_high_detail = 1;
	if (want_hud_detail && !want_high_detail && !hud_detail_may_fit())
	{
		/* (no word on the screen: the setting is on by default, and the
		HUD as it was is no fault -- its help says the map may leave no
		room) */
		char pieces[200];

		n64_heap_describe(pieces, sizeof(pieces));
		debugf("n64: no room for the HUD at 640x240 on this map, the display kept (heap %lu KB free, models %lu KB: %s)\n",
			n64_heap_free() / 1024, (models.loaded_bytes + models.texture_bytes) / 1024, pieces);
		return;
	}
	rspq_wait();
	game_display_close();
	drop_all_models();
	/* (the models' memory freed: their releases were deferred) */
	rspq_wait();
	/* (the HUD at 640x240 over the 3D's 320x240; high and ultra detail have
	their HUD at their own resolution already) */
	if (want_hud_detail && !want_high_detail && try_hud_detail())
		return;
	for (int i = want_high_detail == N64_DETAIL_ULTRA ? 0 : FIRST_HIGH_DETAIL_MODE;
		want_high_detail && i < (int)(sizeof(modes) / sizeof(modes[0])); i++)
	{
		/* the buffers in the maps' home past the map's image, as many as
		fit there (a smaller map leaves room), the rest and the z-buffer
		from the heap */
		unsigned long surface = (unsigned long)modes[i].width * modes[i].height * 2 + 64, home_size;
		void *home = n64_map_home_free_part(&home_size);
		int in_home = home ? (int)(home_size / surface) : 0;

		if (in_home > modes[i].buffers)
			in_home = modes[i].buffers;
		if (display_fits(modes[i].width, modes[i].height, modes[i].buffers + 1 - in_home))
		{
			if (in_home)
			{
				n64_display_lend(home, home_size);
				home_in_use = 1;
			}
			display_init((resolution_t){.width = modes[i].width, .height = modes[i].height,
				.interlaced = modes[i].interlaced ? INTERLACE_FULL : INTERLACE_OFF},
				DEPTH_16_BPP, modes[i].buffers, GAMMA_NONE, display_filters());
			n64_display_lend(NULL, 0);
			screen_width = modes[i].width;
			screen_height = modes[i].height;
			display_mode = DISPLAY_GAME_HIGH;
			n64_hud_forget_recordings();
			/* (a HUD 640 across, at 240 lines or 480: the full-width
			sprites, where there is room) */
			n64_hud_set_wide(modes[i].width == 640, hud_sprite_room());
			sys_get_heap_stats(&heap);
			debugf("n64: %s detail %dx%d%s, %d buffers (%d in the maps' home), heap %d KB free\n",
				want_high_detail == N64_DETAIL_ULTRA ? "ultra" : "high", screen_width, screen_height,
				modes[i].interlaced ? " interlaced" : "", modes[i].buffers, in_home, heap.free / 1024);
			return;
		}
	}
	game_display_init();
	if (want_high_detail)
		notice_high_detail_frames = 150;
	{
		/* (what the heap had: its free pieces) */
		char pieces[200];

		n64_heap_describe(pieces, sizeof(pieces));
		debugf("n64: no room for %s on this map (heap %lu KB free: %s)\n",
			want_high_detail == N64_DETAIL_ULTRA ? "ultra detail" : want_high_detail ? "high detail" : "the HUD at 640x240",
			n64_heap_free() / 1024, pieces);
	}
}

int n64_render_screen_height(void)
{
	return screen_height;
}

void n64_render_initialize(void)
{
	char setting[16];

	/* (port.json flag "debug_rspq_words=<n>": another size) */
	if (n64_debug_text("debug_rspq_words", setting, sizeof(setting)))
		rspq_lowpri_buffer_words = atoi(setting);
	debugf("n64: rspq buffers of %d words\n", rspq_lowpri_buffer_words);
	renderer.clear_color = RGBA32(96, 112, 140, 0xFF);
	game_display_init();
	rdpq_init();
	/* (port.json flag "debug_rdpq": libdragon's RDP validator, every command
	the RDP runs checked on the CPU -- slow -- and the first that would hang
	a console stopped on, disassembled) */
	if (n64_debug_flag("debug_rdpq"))
		rdpq_debug_start();
	t3d_init((T3DInitParams){});
	for (int i = 0; i < MAXIMUM_VIEWS; i++)
	{
		/* the camera and projection matrices in a ring the RSP reads from
		while the CPU goes on: two per frame (the world's, the first-person
		weapon's) for each frame in flight and one more. Unbuffered, tiny3d
		kept one copy that each attach overwrote, and once the CPU ran
		ahead of the RSP the world's later batches and objects were drawn
		with the weapon's 16-unit far plane: distant terrain came and went */
		renderer.viewports[i] = t3d_viewport_create_buffered(2 * (FRAMES_IN_FLIGHT + 1));
		/* triangles reaching past four times the view are clipped (the
		RDP's scissor trims the rest): Halo's big floors and walls near the
		eye made the RSP clip a lot with tiny3d's default of two */
		renderer.viewports[i].guardBandScale = GUARD_BAND_SCALE;
	}
	{
		/* measuring (port.json flag "debug_guard=<1-15>"): another guard band */
		char text[16];

		if (n64_debug_text("debug_guard", text, sizeof(text)) && atoi(text) > 0)
		{
			for (int i = 0; i < MAXIMUM_VIEWS; i++)
				renderer.viewports[i].guardBandScale = atoi(text) & 15;
		}
		debug_cull_front = n64_debug_flag("debug_cull_front");
		if (n64_debug_text("debug_level_parts", text, sizeof(text)))
			debug_level_parts = atoi(text);
		if (n64_debug_text("debug_fog_test", text, sizeof(text)))
			debug_fog_test = atoi(text);
	}
	for (int i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		renderer.matrices[i] = malloc_uncached(sizeof(T3DMat4FP) * MATRICES_PER_FRAME);
		if (!renderer.matrices[i]) n64_fatal("no memory for renderer frame matrices");
	}
	rdpq_text_register_font(FONT_BUILTIN_DEBUG_MONO, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
	/* the models' render modes, recorded now (the debug flags read): made
	with the first model a game loaded they stayed for good wherever the
	heap had room then, in the middle of what the next map and the menu
	wanted */
	record_model_modes();
	{
		FILE *flag;

		if ((flag = fopen("rom:/debug_overview", "rb"))) { debug_overview = 1; fclose(flag); }
		debug_gpu_time = n64_debug_flag("debug_gpu_time");
		if ((flag = fopen("rom:/debug_untextured", "rb"))) { debug_untextured = 1; fclose(flag); }
	}
	{
		char text[16];

		if (n64_debug_flag("debug_rdpq") && n64_debug_text("debug_rdpq_log", text, sizeof(text)))
			debug_rdpq_log_frame = strtoul(text, NULL, 10);
	}
}

/* a model texture: read when a model that uses it is loaded, freed (once
the RSP is done with it) when the last such model is dropped */
static sprite_t *acquire_texture(int texture)
{
	char path[64];

	if (texture < 0 || texture >= models.header->texture_count)
		return NULL;
	if (!models.sprites[texture])
	{
		snprintf(path, sizeof(path), "rom:/tex/%.24s.sprite", models.textures[texture].name);
		models.sprites[texture] = sprite_load(path);
		if (!models.sprites[texture])
			return NULL;
		surface_t pixels = sprite_get_pixels(models.sprites[texture]);

		models.texture_size[texture] = pixels.stride * pixels.height + 512;
		models.texture_bytes += models.texture_size[texture];
		models.texture_reads++;
	}
	models.texture_users[texture]++;
	return models.sprites[texture];
}

static void release_texture(int texture)
{
	if (texture < 0 || texture >= models.header->texture_count || !models.sprites[texture] ||
		--models.texture_users[texture])
		return;
	rdpq_call_deferred((void (*)(void *))sprite_free, models.sprites[texture]);
	models.sprites[texture] = NULL;
	models.texture_bytes -= models.texture_size[texture];
}

/* a model's data and recordings, freed once the RSP is done with them */
struct model_release
{
	uint8_t *data;
	rspq_block_t **setups, **blocks;
	int count;
};

static void release_model_contents(struct model_release *release)
{
	for (int i = 0; i < release->count; i++)
	{
		if (release->setups && release->setups[i])
			rspq_block_free(release->setups[i]);
		if (release->blocks && release->blocks[i])
			rspq_block_free(release->blocks[i]);
	}
	free(release->setups);
	free(release->blocks);
	free(release->data);
}

static void release_model(void *argument)
{
	struct model_release *release = argument;

	release_model_contents(release);
	free(release);
}

/* After a block runs, libdragon writes the RDP commands that follow into the
room left at the end of the block's RDP buffer, until it is full (rdpq.c,
__rdpq_block_end) -- so a block whose commands the RDP has finished can
still be where the RSP writes the next ones. rspq_wait() moves the writing
back to libdragon's own buffers before anything is freed; rdpq_call_deferred
(the model cache's and the HUD's frees) does not. Running this block, made
once and never freed, just before such a free moves the writing into its
room instead. Its end state leaves rdpq's tracking at "unknown" but for the
pipe: every later sync is still made. (GitHub issues 6, 7 and 11: consoles'
RDP crashes and garbage executed; a freed block still in use is one way to
them) */
static rspq_block_t *rdp_output_park;

void n64_render_park_rdp_output(void)
{
	/* (port.json flag "debug_no_park": not parked, as before, for tests) */
	if (n64_debug_flag("debug_no_park"))
		return;
	if (!rdp_output_park)
	{
		rspq_block_begin();
		rdpq_sync_pipe();
		rdp_output_park = rspq_block_end();
	}
	rspq_block_run(rdp_output_park);
}

static void drop_model(int index)
{
	struct loaded_model *loaded = &models.loaded[index];
	struct model_record *model = &models.models[index];
	struct model_release fallback, *release;

	if (!loaded->data)
		return;
	release = malloc(sizeof(*release));
	if (!release)
		release = &fallback;
	release->data = loaded->data;
	release->setups = loaded->setups;
	release->blocks = loaded->blocks;
	release->count = model->far_first + model->far_count;
	if (release == &fallback)
	{
		/* Reclaim the payload before texture releases need deferred-call
		metadata. This path is outside block recording, like the deferred path. */
		rspq_wait();
		release_model_contents(release);
	}
	else
	{
		n64_render_park_rdp_output();
		rdpq_call_deferred(release_model, release);
	}
	for (int b = 0; b < model->far_first + model->far_count; b++)
		release_texture(models.batches[model->first_batch + b].texture);
	models.loaded_bytes -= model->vertex_bytes + model->index_bytes;
	loaded->data = NULL;
	loaded->setups = NULL;
	loaded->blocks = NULL;
}

/* every model in the cache dropped (the RSP done with them:
try_high_detail) */
static void drop_all_models(void)
{
	if (!models.header || !models.loaded)
		return;
	for (int i = 0; i < models.header->model_count; i++)
		drop_model(i);
}

/* whether count blocks of size bytes can be had at once */
static int heap_has_room(unsigned long size, int count)
{
	void *blocks[8];
	int have = 0, room;

	count = count < 8 ? count : 8;
	while (have < count && (blocks[have] = malloc(size)) != NULL)
		have++;
	room = have == count;
	while (have > 0)
		free(blocks[--have]);
	return room;
}

/* the most of the model cache n64_render_make_room drops in one call */
#define MAKE_ROOM_LIMIT (160 * 1024)

/* room on the heap for something the HUD is about to allocate (its
recording), count blocks of size bytes to be had at once. The model cache
fills the heap down to its margin, and what comes after it (the HUD's
recordings and uploads, its sprites, the bots' paths) could use that up: the
HUD ran out of memory recording (__rdpq_block_next_buffer). Short of room,
the models drawn longest ago are dropped (none drawn in the frames still in
flight: their memory comes back once the RSP is done with them), up to
MAKE_ROOM_LIMIT of them a call; 0 if that isn't enough. Not while recording
a block (rspq_wait) */
int n64_render_make_room(unsigned long size, int count)
{
	unsigned long dropped = 0;

	while (!heap_has_room(size, count))
	{
		unsigned long round = 0;

		if (!models.header || !models.loaded)
			return 0;
		/* (a round's models, then one wait for the RSP) */
		while (round < size && dropped + round < MAKE_ROOM_LIMIT)
		{
			int oldest = -1;

			for (int i = 0; i < models.header->model_count; i++)
			{
				if (models.loaded[i].data && models.loaded[i].last_drawn + FRAMES_IN_FLIGHT < renderer.frame_index &&
					(oldest < 0 || models.loaded[i].last_drawn < models.loaded[oldest].last_drawn))
					oldest = i;
			}
			if (oldest < 0)
				break;
			round += models.models[oldest].vertex_bytes + models.models[oldest].index_bytes;
			drop_model(oldest);
		}
		if (!round)
			return 0;
		dropped += round;
		rspq_wait();
	}
	return 1;
}

/* the render modes of model batches, recorded once each: textured (the
doubling is in the texture), shaded by the baked sky light, one RDP cycle;
glows unlit and cut out; untextured; textured and cut out where the
texture's alpha is under half (Halo's alpha-tested model shaders: a snow
pine's needles, MATERIAL_ALPHA_TEST); glows of one colour translucent, over
the rest (their order last: MATERIAL_BLEND) */
enum { MODEL_MODE_TEXTURED, MODEL_MODE_GLOW, MODEL_MODE_UNTEXTURED, MODEL_MODE_CUTOUT, MODEL_MODE_BLEND,
	MODEL_MODES_WITH_DEPTH,
	/* the same, for skies: no depth */
	NUMBER_OF_MODEL_MODES = MODEL_MODES_WITH_DEPTH * 2 };
/* Keep every existing mode/key unchanged. Cloaked units need only three
extra modes with and without fog; skies never inherit unit camouflage. */
enum { MODEL_CLOAK_TEXTURED, MODEL_CLOAK_GLOW, MODEL_CLOAK_UNTEXTURED,
	MODEL_CLOAK_KINDS, MODEL_CLOAK_FIRST = 2 * NUMBER_OF_MODEL_MODES,
	MODEL_MODE_COUNT = MODEL_CLOAK_FIRST + 2 * MODEL_CLOAK_KINDS };
static rspq_block_t *model_modes[MODEL_MODE_COUNT];
typedef char verify_model_mode_sort_bits[MODEL_MODE_COUNT <= 32 ? 1 : -1];

static int model_mode(sprite_t *sprite, int flags, unsigned char opacity)
{
	int mode = !sprite ? MODEL_MODE_UNTEXTURED : (flags & MATERIAL_BLEND) ? MODEL_MODE_BLEND :
		(flags & MATERIAL_GLOW) ? MODEL_MODE_GLOW : (flags & MATERIAL_ALPHA_TEST) ? MODEL_MODE_CUTOUT :
		MODEL_MODE_TEXTURED;

	if (opacity < 255 && !(flags & MATERIAL_SKY))
		return MODEL_CLOAK_FIRST + (renderer.fog ? MODEL_CLOAK_KINDS : 0) +
			(!sprite ? MODEL_CLOAK_UNTEXTURED : flags & (MATERIAL_GLOW | MATERIAL_BLEND) ?
			MODEL_CLOAK_GLOW : MODEL_CLOAK_TEXTURED);
	if (flags & MATERIAL_SKY)
		mode += MODEL_MODES_WITH_DEPTH;
	return renderer.fog ? mode + NUMBER_OF_MODEL_MODES : mode;
}

static void record_model_modes(void)
{
	for (int mode = 0; mode < MODEL_MODE_COUNT; mode++)
	{
		if (model_modes[mode])
			continue;
		int kind = mode % MODEL_MODES_WITH_DEPTH;
		int depth = mode % NUMBER_OF_MODEL_MODES < MODEL_MODES_WITH_DEPTH;
		int cloak = mode >= MODEL_CLOAK_FIRST;
		int cloak_kind = (mode - MODEL_CLOAK_FIRST) % MODEL_CLOAK_KINDS;

		rspq_block_begin();
		rdpq_mode_begin();
		rdpq_set_mode_standard();
		rdpq_mode_zbuf((cloak || depth) && debug_fog_test != 4, cloak ? false : depth);
		rdpq_mode_persp(true);
		rdpq_mode_fog((cloak ? mode >= MODEL_CLOAK_FIRST + MODEL_CLOAK_KINDS :
			mode >= NUMBER_OF_MODEL_MODES) ? RDPQ_FOG_STANDARD : 0);
		rdpq_mode_filter(FILTER_BILINEAR);
		if (cloak)
		{
			/* Keep authored texture coverage and shaded armour, using PRIM
			alpha for translucency; SHADE alpha remains available to fog. */
			if (cloak_kind == MODEL_CLOAK_UNTEXTURED)
				rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, SHADE), (0, 0, 0, PRIM)));
			else if (cloak_kind == MODEL_CLOAK_GLOW)
				rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, TEX0), (TEX0, 0, PRIM, 0)));
			else
				rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, 0, SHADE, 0), (TEX0, 0, PRIM, 0)));
			rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		}
		else if (kind == MODEL_MODE_GLOW)
		{
			rdpq_mode_combiner(RDPQ_COMBINER_TEX);
			rdpq_mode_alphacompare(128);
		}
		else if (kind == MODEL_MODE_TEXTURED)
			rdpq_mode_combiner(RDPQ_COMBINER_TEX_SHADE);
		else if (kind == MODEL_MODE_CUTOUT)
		{
			rdpq_mode_combiner(RDPQ_COMBINER_TEX_SHADE);
			rdpq_mode_alphacompare(128);
		}
		else if (kind == MODEL_MODE_BLEND)
		{
			/* the texture's colour at GLOW_ALPHA (the shade's alpha is the
			fog), blended (the RDP cannot add: its blender wraps), tested
			against the depth but not written; a texel without alpha not
			drawn (the powerups' energy is a pattern, the rest of its
			texture adds nothing in Halo) */
			rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, TEX0), (TEX0, 0, PRIM, 0)));
			rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
			rdpq_mode_zbuf(depth && debug_fog_test != 4, false);
		}
		else
			rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
		/* (Settings: ANTI-ALIASING, on the opaque surfaces) */
		if (want_antialias && !cloak && (kind == MODEL_MODE_TEXTURED || kind == MODEL_MODE_CUTOUT ||
			kind == MODEL_MODE_UNTEXTURED))
			rdpq_mode_antialias(AA_STANDARD);
		rdpq_mode_end();
		if (!cloak && kind == MODEL_MODE_BLEND)
			rdpq_set_prim_color(RGBA32(255, 255, 255, GLOW_ALPHA));
		model_modes[mode] = rspq_block_end();
	}
}

/* Settings: ANTI-ALIASING. The render modes recorded with the other setting
are dropped, recorded again when next used (record_model_modes, set_material) */
void n64_render_set_antialias(int on)
{
	on = on != 0;
	if (on == want_antialias)
		return;
	want_antialias = on;
	rspq_wait();
	for (int mode = 0; mode < MODEL_MODE_COUNT; mode++)
	{
		if (model_modes[mode])
			rspq_block_free(model_modes[mode]);
		model_modes[mode] = NULL;
	}
	record_model_modes();
	for (int fog = 0; level.data && fog < 2; fog++)
	{
		for (int m = 0; level.material_blocks[fog] && m < level.header->material_count; m++)
		{
			if (level.material_blocks[fog][m])
				rspq_block_free(level.material_blocks[fog][m]);
			level.material_blocks[fog][m] = NULL;
		}
	}
}

/* A palette's load (LOAD_TLUT) then drawing, with no SYNC_LOAD between:
libdragon's own font code hung real consoles' RDPs that way (its commit
e2e2dc0, "workaround for crash related to missing sync"; its validator and
the emulators say nothing). Every palette load here is followed by one
(issues 6 and 7: "RDP crashed" on consoles at explosions and plasma fire,
whose scorch marks are paletted, n64_paint.c) */
static void upload_sprite(sprite_t *sprite, const rdpq_texparms_t *parms)
{
	tex_format_t format = sprite_get_format(sprite);

	rdpq_sprite_upload(TILE0, sprite, parms);
	if (format == FMT_CI4 || format == FMT_CI8)
		rdpq_sync_load();
}

/* a batch's texture (and the RSP's culling: Halo's two-sided shaders are
drawn without back-face culling), recorded */
static rspq_block_t *record_setup(sprite_t *sprite, int flags)
{
	int two_sided = (flags & MATERIAL_TWO_SIDED) != 0;

	rspq_block_begin();
	if (sprite)
	{
		upload_sprite(sprite, &(rdpq_texparms_t){
			.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE,
		});
	}
	t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH | (two_sided ? 0 : T3D_FLAG_CULL_BACK) |
		(sprite ? T3D_FLAG_TEXTURED : 0));
	return rspq_block_end();
}

/* A cartridge file read in big pieces at known places (a model, the level's
streamed chunks, its big tiles), without stdio's buffer. newlib gives a file
a 1 KB buffer at its first read; then a read of n bytes was n/1024 DMAs of
1 KB into the buffer, each copied on into the caller's memory, and a seek
out of the buffer a DMA of the 1 KB block around the new place. Unbuffered
(as libdragon's MPEG-1 player reads), a read is one DMA straight into the
caller's memory (DragonFS's dfs_read: through its 512-byte bounce buffer
only where the memory and the file's place differ in their 2-byte phase) and
a seek only sets the place: the same bytes, without the copying, the 1 KB
DMAs' set-ups, the bytes read for nothing and the buffer on the heap */
static FILE *open_direct(const char *path)
{
	FILE *file = fopen(path, "rb");

	if (file)
		setvbuf(file, NULL, _IONBF, 0);
	return file;
}

/* a model's data in memory (read from the cartridge, its textures read and
its batches recorded the first time), or 0 */
static unsigned long model_texture_allocation(int texture)
{
	if (!models.texture_allocation[texture])
	{
		char path[64];
		int size, allocation = 0;
		snprintf(path, sizeof(path), "rom:/tex/%.24s.sprite", models.textures[texture].name);
		FILE *file = fopen(path, "rb");
		if (file)
		{
			fseek(file, 0, SEEK_END); size = ftell(file); fseek(file, 0, SEEK_SET);
			/* Public API's size-only probe: no decompression or allocation. */
			asset_loadf_into(file, &size, NULL, &allocation);
			fclose(file);
		}
		models.texture_allocation[texture] = allocation > 0 ? (unsigned long)allocation + 64 : 4096;
	}
	return models.texture_allocation[texture];
}

static unsigned long model_allocation(struct model_record const *model)
{
	int count = model->far_first + model->far_count;
	unsigned long needed = model->vertex_bytes + model->index_bytes + count * 768 + MODEL_HEAP_MARGIN;
	for (int b = 0; b < count; b++)
	{
		int texture = models.batches[model->first_batch + b].texture;
		if (texture < 0 || texture >= models.header->texture_count || models.sprites[texture]) continue;
		int previous;
		for (previous = 0; previous < b; previous++)
			if (models.batches[model->first_batch + previous].texture == texture) break;
		if (previous == b) needed += model_texture_allocation(texture);
	}
	return needed;
}

/* a first-person model (the arms, the weapon) drawn as one lately. On the
custom cartridge: a Custom Edition map (Foundry: its warehouse is machine
models, ~1.5 MB of them) filled the heap with world models drawn in the
frames in flight, and the view models, drawn last, found no room. There
they are kept once loaded, take room first (n64_render_frame_begin) and may
push out a world model from an earlier frame */
#define FIRST_PERSON_FRAMES 60
static int first_person_model(int index)
{
#ifdef HALO_N64_CUSTOM_MAPS
	unsigned long drawn = models.loaded[index].first_person_drawn;

	return drawn && drawn + FIRST_PERSON_FRAMES >= renderer.frame_index;
#else
	(void)index;
	return 0;
#endif
}

static int load_model(int index)
{
	struct model_record *model = &models.models[index];
	struct loaded_model *loaded = &models.loaded[index];
	unsigned long size = model->vertex_bytes + model->index_bytes;
	int priority = first_person_model(index);
	FILE *file;

	loaded->last_drawn = renderer.frame_index;
	if (loaded->data)
		return 1;
	/* room: drop the models drawn longest ago, none drawn in the frames
	still in flight (their textures go with the last model using them),
	past the cache's size or while the heap is short: the model, its
	recordings (~1 KB a batch), its textures (~2 KB each) and a margin, and
	the model's data in one piece */
	for (;;)
	{
		/* Count each shared texture once, using its actual asset buffer size.
		Painted-panel subdivisions reuse textures across many batches. */
		unsigned long needed = model_allocation(model);
		/* (the total alone passed on a heap cut up by models loaded and
		dropped: memalign then found no piece the model's size, nothing was
		dropped for it, and the model was drawn as nothing frame after
		frame -- the PC cartridge's rocket launcher, 38-43 KB. The piece
		memalign(16, size) needs: newlib's dlmalloc asks malloc for
		request2size(size) + 16 + MINSIZE (16) bytes, a chunk of
		(size + 51) & ~7 -- what malloc(size + 40) takes too) */
		int short_of_heap = n64_heap_free() < needed || !n64_heap_has(size + 40);

		if (models.loaded_bytes + models.texture_bytes + size <= MODEL_CACHE_BYTES && !short_of_heap)
			break;
		int oldest = -1;

		for (int i = 0; i < models.header->model_count; i++)
		{
			if (!models.loaded[i].data || i == index || first_person_model(i))
				continue;
			if ((priority ? models.loaded[i].last_drawn < renderer.frame_index :
				models.loaded[i].last_drawn + FRAMES_IN_FLIGHT < renderer.frame_index) &&
				(oldest < 0 || models.loaded[i].last_drawn < models.loaded[oldest].last_drawn))
				oldest = i;
		}
		if (oldest < 0)
		{
			if (short_of_heap)
			{
				models.missing++;
				return 0;
			}
			break;
		}
		drop_model(oldest);
		if (short_of_heap)
			rspq_wait(); /* the dropped model's memory comes back once the RSP is done with it */
	}
	loaded->data = memalign(16, size);
	loaded->setups = calloc(model->far_first + model->far_count, sizeof(rspq_block_t *));
	loaded->blocks = calloc(model->far_first + model->far_count, sizeof(rspq_block_t *));
	char path[48];

	snprintf(path, sizeof(path), "rom:/mdl/%.16s.bin", model->data_name);
	file = loaded->data && loaded->setups && loaded->blocks ? open_direct(path) : NULL;
	if (!file || fread(loaded->data, 1, size, file) != size)
	{
		if (file)
			fclose(file);
		free(loaded->data);
		free(loaded->setups);
		free(loaded->blocks);
		loaded->data = NULL;
		loaded->setups = NULL;
		loaded->blocks = NULL;
		models.missing++;
		return 0;
	}
	fclose(file);
	data_cache_hit_writeback_invalidate(loaded->data, size);
	models.loaded_bytes += size;
	models.reads++;
	for (int b = 0; b < model->far_first + model->far_count; b++)
	{
		struct model_batch *batch = &models.batches[model->first_batch + b];
		sprite_t *sprite = acquire_texture(batch->texture);
		int run = models.runs ? models.runs[model->first_batch + b] : RUN_HEAD | 1;

		/* (a run's batches all in its first's block: the rest have none,
		mark_batch_runs) */
		if (!(run & RUN_HEAD))
			continue;
		record_model_modes();
		loaded->setups[b] = record_setup(debug_untextured ? NULL : sprite, batch->flags);
		rspq_block_begin();
		/* Each segment's vertices are loaded under its node's matrix times
		the camera's. flush_draws moves tiny3d's matrix stack up one past the
		camera's for all its blocks, and a block sets each node there, the
		camera's multiplied in (t3d_matrix_set, as tiny3d's own model drawing
		does): the same matrices as a push and a pop around each segment,
		without the pop -- a matrix read back, normalised and multiplied by
		the projection on the RSP, ~170 of them a view for the first-person
		arms alone. A node already in force (the segment before it in the
		block was its) is not set again: the RSP's matrices are what they
		were (none at the block's start: whatever the block before left) */
		int node = -1;

		for (int k = 0; k < (run & RUN_LENGTH) && b + k < model->far_first + model->far_count; k++)
		{
			struct model_batch *part = batch + k;
			uint8_t *index = loaded->data + model->vertex_bytes + part->index_offset;

			for (int g = 0; g < part->segment_count; g++)
			{
				struct model_segment *segment = &models.segments[part->first_segment + g];

				if (segment->node != node)
				{
					t3d_matrix_set(t3d_segment_address(SEGMENT_NODE_MATRICES, (void *)(segment->node * sizeof(T3DMat4FP))), true);
					node = segment->node;
				}
				t3d_vert_load((T3DVertPacked *)loaded->data + segment->vertex_pair_offset, segment->slot, segment->count);
			}
			for (int t = 0; t < part->triangle_count; t++, index += 3)
				t3d_tri_draw(index[0], index[1], index[2]);
			t3d_tri_sync();
		}
		loaded->blocks[b] = rspq_block_end();
	}
	return 1;
}

/* The converter splits a part of a model into several batches where it
outgrows the vertex cache (tools/n64/models.py): the same texture and
flags one after another. Each was queued, sorted, set up and called on
its own; a run of them is recorded as one block (the same commands one
after the other) and drawn as one. Runs end where a geometry (a region's
permutation) or a model's far version starts or ends, so that a range of
batches is always whole runs: from a range's first batch, each run's
length leads to the next run's first. */
static void mark_batch_runs(void)
{
	int count = models.header->batch_count;
	uint8_t *edge = calloc(count + 1, 1);

	free(models.runs);
	models.runs = malloc(count ? count : 1);
	if (!edge || !models.runs)
	{
		/* (no runs: each batch its own) */
		for (int b = 0; models.runs && b < count; b++)
			models.runs[b] = RUN_HEAD | 1;
		free(edge);
		return;
	}
	for (int g = 0; g < models.header->geometry_count; g++)
	{
		struct geometry_record *geometry = &models.geometries[g];

		if (geometry->first_batch < count)
			edge[geometry->first_batch] = 1;
		if (geometry->first_batch + geometry->batch_count <= count)
			edge[geometry->first_batch + geometry->batch_count] = 1;
	}
	for (int m = 0; m < models.header->model_count; m++)
	{
		struct model_record *model = &models.models[m];
		int marks[3] = {model->first_batch, model->first_batch + model->far_first,
			model->first_batch + model->far_first + model->far_count};

		for (int i = 0; i < 3; i++)
		{
			if (marks[i] <= count)
				edge[marks[i]] = 1;
		}
	}
	/* the length from each batch to its run's end, from the last back */
	for (int b = count - 1; b >= 0; b--)
	{
		struct model_batch *batch = &models.batches[b], *next = batch + 1;
		int length = 1;

		if (b + 1 < count && !edge[b + 1] && next->texture == batch->texture && next->flags == batch->flags &&
			(models.runs[b + 1] & RUN_LENGTH) < RUN_LENGTH)
		{
			length = (models.runs[b + 1] & RUN_LENGTH) + 1;
		}
		models.runs[b] = (uint8_t)length;
	}
	/* a run's first: a range's first, or where the length didn't go on */
	for (int b = 0; b < count; b++)
	{
		if (b == 0 || edge[b] || (models.runs[b - 1] & RUN_LENGTH) != (models.runs[b] & RUN_LENGTH) + 1)
			models.runs[b] |= RUN_HEAD;
	}
	free(edge);
}

static void load_models(const char *map_name)
{
	struct models_header header;
	FILE *file;

	if (models.data)
	{
		rspq_wait();
		for (int i = 0; models.loaded && i < models.header->model_count; i++)
			drop_model(i);
		rspq_wait();
		free(models.sprites);
		free(models.texture_users);
		free(models.texture_size);
		free(models.texture_allocation);
		free(models.loaded);
		free(models.by_tag);
		free(models.runs);
		free(models.data);
		memset(&models, 0, sizeof(models));
	}
	if (!map_name)
		return;
	snprintf(models.path, sizeof(models.path), "rom:/levels/%s.mdl", map_name);
	snprintf(models.map_name, sizeof(models.map_name), "%s", map_name);
	file = fopen(models.path, "rb");
	if (!file)
	{
		debugf("n64: no models %s\n", models.path);
		return;
	}
	/* Tables are required; don't publish partially read model metadata. */
	if (fseek(file, 0, SEEK_END)) { fclose(file); return; }
	long file_size = ftell(file);
	if (file_size < (long)sizeof(header) || fseek(file, 0, SEEK_SET) ||
		fread(&header, 1, sizeof(header), file) != sizeof(header) ||
		memcmp(header.magic, "H64O", 4) || header.version != 4 ||
		header.vertices_offset < sizeof(header) || header.vertices_offset > (unsigned long)file_size ||
		!file_span(header.vertices_offset, header.textures_offset, header.texture_count, sizeof(struct level_texture)) ||
		!file_span(header.vertices_offset, header.models_offset, header.model_count, sizeof(struct model_record)) ||
		!file_span(header.vertices_offset, header.regions_offset, header.region_count, sizeof(struct region_record)) ||
		!file_span(header.vertices_offset, header.permutations_offset, header.permutation_count, sizeof(int16_t)) ||
		!file_span(header.vertices_offset, header.geometries_offset, header.geometry_count, sizeof(struct geometry_record)) ||
		!file_span(header.vertices_offset, header.batches_offset, header.batch_count, sizeof(struct model_batch)) ||
		!file_span(header.vertices_offset, header.segments_offset, header.segment_count, sizeof(struct model_segment)))
	{
		fclose(file);
		debugf("n64: invalid/short model table %s\n", models.path);
		return;
	}
	models.data = memalign(16, header.vertices_offset);
	if (!models.data || fseek(file, 0, SEEK_SET) || fread(models.data, 1, header.vertices_offset, file) != header.vertices_offset)
	{
		free(models.data); models.data = NULL;
		fclose(file);
		debugf("n64: unavailable model table %s\n", models.path);
		return;
	}
	fclose(file);
	models.header = (struct models_header *)models.data;
	models.textures = (struct level_texture *)(models.data + models.header->textures_offset);
	models.models = (struct model_record *)(models.data + models.header->models_offset);
	models.regions = (struct region_record *)(models.data + models.header->regions_offset);
	models.permutations = (int16_t *)(models.data + models.header->permutations_offset);
	models.geometries = (struct geometry_record *)(models.data + models.header->geometries_offset);
	models.batches = (struct model_batch *)(models.data + models.header->batches_offset);
	models.segments = (struct model_segment *)(models.data + models.header->segments_offset);
	models.sprites = calloc(models.header->texture_count, sizeof(sprite_t *));
	models.texture_users = calloc(models.header->texture_count, sizeof(unsigned short));
	models.texture_size = calloc(models.header->texture_count, sizeof(unsigned long));
	models.texture_allocation = calloc(models.header->texture_count, sizeof(unsigned long));
	models.loaded = calloc(models.header->model_count, sizeof(struct loaded_model));
	if ((!models.sprites && models.header->texture_count) || (!models.texture_users && models.header->texture_count) ||
		(!models.texture_size && models.header->texture_count) || (!models.texture_allocation && models.header->texture_count) ||
		(!models.loaded && models.header->model_count))
	{
		debugf("n64: no memory for model cache tables\n");
		load_models(NULL);
		return;
	}
	/* All following table-indexed paths rely on these source ranges. */
	for (int i = 0; i < models.header->model_count; i++)
	{
		const struct model_record *m = &models.models[i];
		if (m->first_region + m->region_count > models.header->region_count ||
			m->first_batch + m->far_first + m->far_count > models.header->batch_count ||
			m->vertex_bytes > ULONG_MAX - m->index_bytes)
		{ load_models(NULL); return; }
	}
	for (int b = 0; b < models.header->batch_count; b++)
	{
		const struct model_batch *part = &models.batches[b];
		if (part->first_segment + part->segment_count > models.header->segment_count ||
			part->texture < -1 || part->texture >= models.header->texture_count)
		{ load_models(NULL); return; }
	}
	models.tag_count = 0;
	for (int i = 0; i < models.header->model_count; i++)
	{
		if (models.models[i].tag_index + 1 > models.tag_count)
			models.tag_count = models.models[i].tag_index + 1;
	}
	models.by_tag = calloc(models.tag_count ? models.tag_count : 1, sizeof(struct model_record *));
	if (!models.by_tag) { load_models(NULL); return; }
	for (int i = 0; i < models.header->model_count; i++)
		models.by_tag[models.models[i].tag_index] = &models.models[i];
	mark_batch_runs();
	debugf("n64: %d models, %ld KB of tables\n", models.header->model_count, (long)header.vertices_offset / 1024);
}

void n64_render_unload_level(void)
{
	if (level.data)
	{
		rspq_wait();
		for (int fog = 0; fog < 2; fog++)
		{
			for (int m = 0; level.material_blocks[fog] && m < level.header->material_count; m++)
			{
				if (level.material_blocks[fog][m])
					rspq_block_free(level.material_blocks[fog][m]);
			}
			free(level.material_blocks[fog]);
		}
		free(level.visible);
		free(level.sorted);
		free(level.material_starts);
		free(level.groups);
		free(level.group_batches);
		free(level.cluster_first_group);
		free(level.cluster_group_count);
		free(level.cluster_boxes);
		for (int i = 0; i < level.header->texture_count; i++)
		{
			if (level.sprites && level.sprites[i])
				sprite_free(level.sprites[i]);
		}
		free(level.sprites);
		if (level.stream)
		{
			free(level.slots);
			free(level.slot_chunk);
			free(level.chunk_slot);
			free(level.resident);
			free(level.resident_drawn);
			free(level.batch_chunk);
			fclose(level.stream);
		}
		unload_tiles();
		free(level.geometry_cache);
		free(level.geometry_offsets);
		free(level.index_sizes);
		free(level.data);
		memset(&level, 0, sizeof(level));
	}
}

/* Level clusters are the BSP's: a map of one cluster (Hang 'em High,
Longest, Prisoner: 140-470 batches) or a few big ones had each batch
tested against each view's frustum and fog, every frame. A cluster of more
than GROUPED_CLUSTER batches is split here, by halves along its longest
side, into groups of at most GROUP_BATCHES lying near one another, each
with its bounds: draw_level tests a group first, then its batches only if
it is partly in view. A box out of view or past the fog holds nothing in
view; one wholly inside holds batches wholly inside: the batches drawn are
the same (and their order comes from the material sort after). */
#define GROUPED_CLUSTER 32
#define GROUP_BATCHES 16

static int group_axis;

/* the level's terrain tiles (rom:/levels/<map>.tiles beside its .lvl), if
it has any: their table and the resident tiles, and slots for the big ones
as the heap allows */
static void load_tiles(const char *level_path)
{
	char path[64];
	struct tiles_header header;
	size_t length = strlen(level_path), table;
	FILE *file;
	long file_size;

	if (length < 4 || length + 3 > sizeof(path)) return;
	memcpy(path, level_path, length - 4);
	strcpy(path + length - 4, ".tiles");
	file = open_direct(path);
	if (!file) return;
	if (fseek(file, 0, SEEK_END) || (file_size = ftell(file)) < (long)sizeof(header) ||
		fseek(file, 0, SEEK_SET) || fread(&header, 1, sizeof(header), file) != sizeof(header) ||
		memcmp(header.magic, "H64T", 4) || header.version != 1 || !header.entry_count ||
		header.entry_count > INT16_MAX || header.entry_count > level.header->batch_count ||
		header.detail_level != 1 || header.entries_offset < sizeof(header) || (header.entries_offset & 3) ||
		!file_span((unsigned long)file_size, header.entries_offset, header.entry_count, sizeof(struct tile_entry)) ||
		header.big_largest > 2048 ||
		!file_span((unsigned long)file_size, header.resident_offset, header.resident_size, 1) ||
		header.big_offset > (unsigned long)file_size)
		goto failed;
	table = header.entries_offset + header.entry_count * sizeof(struct tile_entry);
	if (header.resident_offset < table || header.big_offset < header.resident_offset + header.resident_size)
		goto failed;
	tiles.data = malloc(table);
	tiles.resident = memalign(16, header.resident_size ? header.resident_size : 16);
	tiles.batch_entry = malloc((level.header->batch_count ? level.header->batch_count : 1) * sizeof(int16_t));
	tiles.entry_slot = malloc(header.entry_count * sizeof(int16_t));
	tiles.entry_drawn = calloc(header.entry_count, sizeof(unsigned long));
	if (!tiles.data || !tiles.resident || !tiles.batch_entry || !tiles.entry_slot || !tiles.entry_drawn)
		goto failed;
	if (fseek(file, 0, SEEK_SET) || fread(tiles.data, 1, table, file) != table ||
		fseek(file, header.resident_offset, SEEK_SET) ||
		fread(tiles.resident, 1, header.resident_size, file) != header.resident_size)
		goto failed;
	tiles.entries = (struct tile_entry *)(tiles.data + header.entries_offset);
	for (int b = 0; b < level.header->batch_count; b++) tiles.batch_entry[b] = -1;
	for (int e = 0; e < header.entry_count; e++)
	{
		const struct tile_entry *entry = &tiles.entries[e];
		unsigned long small = tile_bytes(entry->size), big = tile_bytes(entry->big_size);
		if (entry->batch >= level.header->batch_count || tiles.batch_entry[entry->batch] >= 0 ||
			!small || entry->shift_s > 10 || entry->shift_t > 10 || (entry->offset & 15) ||
			!file_span(header.resident_size, entry->offset, small, 1) ||
			(entry->big_size && (!big || big > header.big_largest || entry->big_shift_s > 10 ||
			entry->big_shift_t > 10 || (entry->big_offset & 15) ||
			!file_span((unsigned long)file_size - header.big_offset, entry->big_offset, big, 1))))
			goto failed;
		tiles.batch_entry[entry->batch] = (int16_t)e;
		tiles.entry_slot[e] = -1;
	}
	/* Publish only complete metadata/pixels. Optional near-slot OOM keeps
	   the already validated resident tiles, as upstream intended. */
	tiles.header = (struct tiles_header *)tiles.data;
	data_cache_hit_writeback(tiles.resident, header.resident_size);
	/* (the tile in TMEM's second half: the detail map's level drawn under it
	-- 512 bytes, or 2 KB for a grain the texture picker chose, drawn whole:
	tools/n64/level.py detail_texture -- in the first) */
	tiles.tmem = 2048;
	tiles.near_squared = TILE_NEAR_UNITS * WORLD_SCALE * TILE_NEAR_UNITS * WORLD_SCALE;
	tiles.slot_size = (header.big_largest + 15) & ~15UL;
	for (tiles.slot_count = header.big_largest ? TILE_SLOTS : 0; tiles.slot_count >= 6; tiles.slot_count -= 6)
	{
		if (n64_heap_has(tiles.slot_count * tiles.slot_size + TILE_HEAP_MARGIN) &&
			(tiles.slots = memalign(16, tiles.slot_count * tiles.slot_size)) != NULL)
			break;
	}
	if (!tiles.slots) tiles.slot_count = 0;
	if (tiles.slot_count)
	{
		tiles.slot_entry = malloc(tiles.slot_count * sizeof(int16_t));
		if (!tiles.slot_entry)
		{
			free(tiles.slots); tiles.slots = NULL; tiles.slot_count = 0;
		}
	}
	for (int i = 0; i < tiles.slot_count; i++) tiles.slot_entry[i] = -1;
	if (tiles.slot_count) tiles.file = file;
	else fclose(file);
	debugf("n64: terrain tiles: %d (%lu KB in memory), %d slots of %lu bytes for the near ones\n",
		header.entry_count, (unsigned long)header.resident_size / 1024, tiles.slot_count, tiles.slot_size);
	return;
failed:
	debugf("n64: terrain tiles unavailable (invalid/short file or no memory)\n");
	fclose(file);
	unload_tiles();
}

static int compare_batch_centers(const void *a, const void *b)
{
	const struct level_batch *x = &level.batches[*(const uint16_t *)a], *y = &level.batches[*(const uint16_t *)b];

	return (x->minimum[group_axis] + x->maximum[group_axis]) - (y->minimum[group_axis] + y->maximum[group_axis]);
}

static void split_level_group(uint16_t *list, int count, int *group_count, int maximum_groups)
{
	int low[3] = {INT16_MAX, INT16_MAX, INT16_MAX}, high[3] = {INT16_MIN, INT16_MIN, INT16_MIN};

	for (int i = 0; i < count; i++)
	{
		const struct level_batch *batch = &level.batches[list[i]];

		for (int k = 0; k < 3; k++)
		{
			int center = batch->minimum[k] + batch->maximum[k];

			low[k] = center < low[k] ? center : low[k];
			high[k] = center > high[k] ? center : high[k];
		}
	}
	if (count > GROUP_BATCHES && *group_count + 2 <= maximum_groups)
	{
		int spread[3] = {high[0] - low[0], high[1] - low[1], high[2] - low[2]};

		group_axis = spread[0] >= spread[1] && spread[0] >= spread[2] ? 0 : spread[1] >= spread[2] ? 1 : 2;
		qsort(list, count, sizeof(list[0]), compare_batch_centers);
		split_level_group(list, count / 2, group_count, maximum_groups);
		split_level_group(list + count / 2, count - count / 2, group_count, maximum_groups);
		return;
	}
	{
		struct level_group *group = &level.groups[(*group_count)++];

		group->first = (uint16_t)(list - level.group_batches);
		group->count = (uint16_t)count;
		for (int k = 0; k < 3; k++)
		{
			group->minimum[k] = 1e9f;
			group->maximum[k] = -1e9f;
		}
		for (int i = 0; i < count; i++)
		{
			const struct level_batch *batch = &level.batches[list[i]];

			for (int k = 0; k < 3; k++)
			{
				group->minimum[k] = n64_min_f(group->minimum[k], batch->minimum[k] * (1.f / WORLD_SCALE));
				group->maximum[k] = n64_max_f(group->maximum[k], batch->maximum[k] * (1.f / WORLD_SCALE));
			}
		}
	}
}

/* each cluster's batches' box (none, or a cluster whose batches are not all
in the file -- its minimum past its maximum: every plane tested). (Made once
a level, kept small) */
static __attribute__((cold, noinline)) void build_cluster_boxes(void)
{
	int batch_count = level.header->batch_count, cluster_count = level.header->cluster_count;

	level.cluster_boxes = malloc((cluster_count ? cluster_count : 1) * sizeof(level.cluster_boxes[0]));
	for (int c = 0; level.cluster_boxes && c < cluster_count; c++)
	{
		struct level_cluster *cluster = &level.clusters[c];
		int16_t *box = level.cluster_boxes[c];

		box[0] = box[1] = box[2] = INT16_MAX;
		box[3] = box[4] = box[5] = INT16_MIN;
		for (int b = cluster->first_batch; cluster->first_batch + cluster->batch_count <= batch_count &&
			b < cluster->first_batch + cluster->batch_count; b++)
		{
			/* (read as draw_level reads them, halfwords) */
			const int16_t *minimum = level.batches[b].minimum, *maximum = level.batches[b].maximum;

			for (int k = 0; k < 3; k++)
			{
				box[k] = minimum[k] < box[k] ? minimum[k] : box[k];
				box[3 + k] = maximum[k] > box[3 + k] ? maximum[k] : box[3 + k];
			}
		}
	}
}

static void build_level_groups(void)
{
	int batch_count = level.header->batch_count, cluster_count = level.header->cluster_count;
	int maximum_groups = batch_count / 4 + cluster_count + 1, group_count = 0;

	build_cluster_boxes();
	level.groups = malloc(maximum_groups * sizeof(struct level_group));
	level.group_batches = malloc((batch_count ? batch_count : 1) * sizeof(uint16_t));
	level.cluster_first_group = calloc(cluster_count ? cluster_count : 1, sizeof(uint16_t));
	level.cluster_group_count = calloc(cluster_count ? cluster_count : 1, sizeof(uint16_t));
	if (!level.groups || !level.group_batches || !level.cluster_first_group || !level.cluster_group_count)
	{
		/* (no groups: every batch tested, as before) */
		free(level.groups);
		free(level.group_batches);
		free(level.cluster_first_group);
		free(level.cluster_group_count);
		level.groups = NULL;
		level.group_batches = level.cluster_first_group = level.cluster_group_count = NULL;
		return;
	}
	for (int c = 0; c < cluster_count; c++)
	{
		struct level_cluster *cluster = &level.clusters[c];
		int first_group = group_count;

		if (cluster->batch_count <= GROUPED_CLUSTER || cluster->first_batch + cluster->batch_count > batch_count)
			continue;
		for (int i = 0; i < cluster->batch_count; i++)
			level.group_batches[cluster->first_batch + i] = (uint16_t)(cluster->first_batch + i);
		split_level_group(level.group_batches + cluster->first_batch, cluster->batch_count, &group_count, maximum_groups);
		level.cluster_first_group[c] = (uint16_t)first_group;
		level.cluster_group_count[c] = (uint16_t)(group_count - first_group);
	}
	debugf("n64: level batches in %d groups\n", group_count);
}

int n64_render_load_level(const char *map_name, int low)
{
	char path[64];
	FILE *file = NULL;
	long size;

	n64_render_unload_level();
	debugf("n64: loading level %s%s, %lu KB of heap left\n", map_name, low ? " (low)" : "", n64_heap_free() / 1024);
	if (low)
	{
		snprintf(path, sizeof(path), "rom:/levels/%s-low.lvl", map_name);
		file = open_direct(path);
	}
	if (!file)
	{
		snprintf(path, sizeof(path), "rom:/levels/%s.lvl", map_name);
		file = open_direct(path);
	}
	if (!file)
	{
		debugf("n64: no level geometry %s\n", path);
		return 0;
	}
	if (fseek(file, 0, SEEK_END) || (size = ftell(file)) < 0 || fseek(file, 0, SEEK_SET))
		n64_fatal("cannot seek native level geometry");
	struct level_header disk_header;
	if (size < (long)sizeof(disk_header) || fread(&disk_header, 1, sizeof(disk_header), file) != sizeof(disk_header) ||
		memcmp(disk_header.magic, "H64L", 4) || (disk_header.version != 4 && disk_header.version != 5) ||
		disk_header.vertices_offset < sizeof(disk_header) || disk_header.vertices_offset > (unsigned long)size ||
		disk_header.indices_offset > (unsigned long)size || disk_header.indices_size > (unsigned long)size - disk_header.indices_offset)
		n64_fatal("invalid native level geometry header");
	if (!file_span(disk_header.vertices_offset, disk_header.textures_offset, disk_header.texture_count, sizeof(struct level_texture)) ||
		!file_span(disk_header.vertices_offset, disk_header.materials_offset, disk_header.material_count, sizeof(struct level_material)) ||
		!file_span(disk_header.vertices_offset, disk_header.clusters_offset, disk_header.cluster_count, sizeof(struct level_cluster)) ||
		!file_span(disk_header.vertices_offset, disk_header.batches_offset, disk_header.batch_count, sizeof(struct level_batch)) ||
		!file_span(disk_header.vertices_offset, disk_header.owners_offset, disk_header.owner_total, sizeof(uint16_t)) ||
		!file_span((unsigned long)size, disk_header.vertices_offset, disk_header.vertices_size, 1))
		n64_fatal("invalid native level table ranges");
	/* Campaign v5 keeps upstream's chunk cache. Multiplayer v4 keeps the
	   reviewed PC 64KiB DMA cache only when the whole map would crowd heap. */
	int streamed = disk_header.version == 4 && (n64_heap_free() < 512 * 1024 || (unsigned long)size > n64_heap_free() - 512 * 1024);
	unsigned long resident = streamed || disk_header.version == 5 ? disk_header.vertices_offset : (unsigned long)size;
	level.data = memalign(16, resident);
	if (!level.data) n64_fatal("no memory for native level tables");
	if (fseek(file, 0, SEEK_SET) || fread(level.data, 1, resident, file) != resident) n64_fatal("short native level geometry read");
	/* the RSP reads the vertices by DMA */
	data_cache_hit_writeback_invalidate(level.data, resident);

	level.header = (struct level_header *)level.data;
	if (level.header->version == 5)
	{
		if (!file_span(resident, level.header->stream_offset, 1, 4) ||
			!file_span(resident, level.header->stream_offset + 4, level.header->cluster_count, sizeof(struct level_cluster_chunks)))
			n64_fatal("invalid native campaign stream table");
		uint8_t *stream = level.data + level.header->stream_offset;

		level.stream = file;
		level.chunk_count = (int)*(uint32_t *)stream;
		level.cluster_chunks = (struct level_cluster_chunks *)(stream + 4);
		level.chunks = (struct level_chunk *)(stream + 4 + level.header->cluster_count * sizeof(struct level_cluster_chunks));
		unsigned long chunk_offset = level.header->stream_offset + 4 + level.header->cluster_count * sizeof(struct level_cluster_chunks);
		if (level.chunk_count < 0 || level.chunk_count > INT16_MAX ||
			!file_span(resident, chunk_offset, level.chunk_count, sizeof(struct level_chunk)))
			n64_fatal("invalid native campaign chunks");
		level.resident = calloc(level.chunk_count ? level.chunk_count : 1, sizeof(uint8_t *));
		level.resident_drawn = calloc(level.chunk_count ? level.chunk_count : 1, sizeof(unsigned long));
		level.batch_chunk = malloc((level.header->batch_count ? level.header->batch_count : 1) * sizeof(uint16_t));
		level.chunk_slot = malloc((level.chunk_count ? level.chunk_count : 1) * sizeof(int16_t));
		if (!level.resident || !level.resident_drawn || !level.batch_chunk || !level.chunk_slot)
			n64_fatal("no memory for campaign chunk tables");
		for (int k = 0; k < level.chunk_count; k++)
		{
			if (!file_span(level.header->vertices_size, level.chunks[k].vertex_offset, level.chunks[k].vertex_size, 1) ||
				!file_span(level.header->indices_size, level.chunks[k].index_offset, level.chunks[k].index_size, 1) ||
				level.chunks[k].first_batch + level.chunks[k].batch_count > level.header->batch_count ||
				level.chunks[k].vertex_size > ULONG_MAX - level.chunks[k].index_size - 15)
				n64_fatal("invalid native campaign chunk range");
			unsigned long chunk_size = level.chunks[k].vertex_size + level.chunks[k].index_size;

			if (chunk_size > level.slot_size)
				level.slot_size = chunk_size;
			level.chunk_slot[k] = -1;
		}
		/* the chunks' memory, before the models' cache can have it: as many
		slots as LEVEL_CACHE_BYTES holds, fewer if the heap has not got them
		(the model cache took what was free past its margin, and the chunks
		that found no room were never drawn: whole parts of a30 missing) */
		level.slot_size = (level.slot_size + 15) & ~15UL;
		for (level.slot_count = level.slot_size ? (int)(LEVEL_CACHE_BYTES / level.slot_size) : 0;
			level.slot_count >= 8; level.slot_count -= 4)
		{
			if (n64_heap_has(level.slot_count * level.slot_size + LEVEL_HEAP_MARGIN) &&
				(level.slots = memalign(16, level.slot_count * level.slot_size)) != NULL)
				break;
		}
		if (!level.slots)
			level.slot_count = 0;
		level.slot_chunk = malloc((level.slot_count ? level.slot_count : 1) * sizeof(int16_t));
		if (!level.slot_chunk) n64_fatal("no memory for campaign slot table");
		for (int i = 0; i < level.slot_count; i++)
			level.slot_chunk[i] = -1;
		debugf("n64: level streamed: %d chunks, %d slots of %lu bytes\n", level.chunk_count, level.slot_count,
			level.slot_size);
	}
	else
		fclose(file);
	/* (its terrain's tiles: before the models' cache takes the heap) */
	load_tiles(path);
	level.textures = (struct level_texture *)(level.data + level.header->textures_offset);
	level.materials = (struct level_material *)(level.data + level.header->materials_offset);
	level.clusters = (struct level_cluster *)(level.data + level.header->clusters_offset);
	level.owners = (uint16_t *)(level.data + level.header->owners_offset);
	level.batches = (struct level_batch *)(level.data + level.header->batches_offset);
	if (level.stream)
	{
		memset(level.batch_chunk, 0xff, level.header->batch_count * sizeof(uint16_t));
		for (int k = 0; k < level.chunk_count; k++)
		{
			for (int b = level.chunks[k].first_batch; b < level.chunks[k].first_batch + level.chunks[k].batch_count; b++)
				level.batch_chunk[b] = (uint16_t)k;
		}
		for (int b = 0; b < level.header->batch_count; b++)
			if (level.batch_chunk[b] == UINT16_MAX) n64_fatal("unmapped native campaign batch");
	}
	else if (streamed)
	{
		level.geometry_rom = dfs_rom_addr(path + 5);
		level.geometry_cache = memalign(16, 64 * 1024);
		level.geometry_offsets = malloc(level.header->batch_count * sizeof(uint32_t));
		level.index_sizes = malloc(level.header->batch_count * sizeof(uint32_t));
		if (!level.geometry_rom || !level.geometry_cache || !level.geometry_offsets || !level.index_sizes)
			n64_fatal("no memory for the native geometry stream cache");
		memset(level.geometry_offsets, 0xff, level.header->batch_count * sizeof(uint32_t));
		for (int b = 0; b < level.header->batch_count; b++)
		{
			unsigned long next = b + 1 < level.header->batch_count ? level.batches[b + 1].index_offset : level.header->indices_size;
			if (next <= level.batches[b].index_offset || next > level.header->indices_size)
				n64_fatal("invalid native level strip order");
			level.index_sizes[b] = next - level.batches[b].index_offset;
		}
		debugf("n64: level geometry streamed: %lu KB tables, 64 KB cache\n", resident / 1024);
	}
	else
	{
		level.vertices = (T3DVertPacked *)(level.data + level.header->vertices_offset);
		level.indices = level.data + level.header->indices_offset;
	}
	level.visible = malloc(level.header->batch_count * sizeof(uint16_t));
	level.sorted = malloc(level.header->batch_count * sizeof(uint16_t));
	level.material_starts = malloc((level.header->material_count + 1) * sizeof(uint16_t));
	if (!level.visible || !level.sorted || !level.material_starts)
		n64_fatal("no memory for native level draw tables");
	/* (a streamed level is culled by its chunks instead) */
	if (!level.stream)
		build_level_groups();
	/* the strips' indices made into tiny3d's vertex cache addresses, once
	(the RSP reads them by DMA: written back after) -- recorded blocks of
	the triangles cost Sidewinder 275 KB, the indices ~40 */
	for (int b = 0; b < level.header->batch_count && !level.stream && !streamed; b++)
	{
		unsigned long start = level.batches[b].index_offset;
		unsigned long end = b + 1 < level.header->batch_count ? level.batches[b + 1].index_offset : level.header->indices_size;
		if (end <= start || end > level.header->indices_size || !convert_level_strips(level.indices + start, end - start))
			n64_fatal("invalid resident level strip");
	}
	if (!level.stream && !streamed) data_cache_hit_writeback(level.indices, level.header->indices_size);
	debugf("n64: level strips ready, %lu KB of heap left\n", n64_heap_free() / 1024);
	level.sprites = calloc(level.header->texture_count ? level.header->texture_count : 1, sizeof(sprite_t *));
	if (!level.sprites) n64_fatal("no memory for native level texture table");
	for (int i = 0; i < level.header->texture_count; i++)
	{
		snprintf(path, sizeof(path), "rom:/tex/%.24s.sprite", level.textures[i].name);
		level.sprites[i] = sprite_load(path);
	}
	/* the objects' models (with low, their lighter set): the set made with
	the view models' quality first, the plain one if there is none */
	{
		char name[48];

		/* (port.json flag "debug_view_model=<0-2>": the quality without
		the menu) */
		if (n64_debug_text("debug_view_model", name, sizeof(name)))
			n64_render_set_view_model_quality(atoi(name));
		if (view_model_quality)
		{
			snprintf(name, sizeof(name), "%s%s-fp%d", map_name, low ? "-low" : "", view_model_quality);
			load_models(name);
		}
		if (!models.data)
		{
			snprintf(name, sizeof(name), "%s%s", map_name, low ? "-low" : "");
			load_models(name);
		}
		if (!models.data)
			load_models(map_name);
	}
	debugf("n64: level %s: %d clusters, %d batches, %lu KB, %lu KB of heap left\n", map_name,
		level.header->cluster_count, level.header->batch_count, (unsigned long)size / 1024,
		n64_heap_free() / 1024);
	return 1;
}

/* the tinted palettes made this frame (use_setup) */
static int palette_count;

/* the frame's z-buffer (the one attached with its colour buffer) */
static const surface_t *frame_zbuf;

static int debug_rdpq_logging;

static void debug_rdpq_mark(const char *what)
{
	if (debug_rdpq_logging)
		rdpq_debug_log_msg(what);
}

/* After a SYNC_FULL (rdpq_detach, rdpq_fence) the RSP holds back the RDP
commands that follow it, and the next block's buffer switch
(RSPQCmd_RdpSetBuffer) lets them go as soon as DP_STATUS_BUSY drops. On
consoles that came too soon: the crash screens of GitHub issues 6, 7, 11,
13 and 14 show the RDP stopped right after a SYNC_FULL, before its first
held command, with the next block's buffer pending -- the RDP hang
libdragon's SYNC_FULL handling is there to avoid, and this renderer asks
for a SYNC_FULL at each depth reset (the frame's start and each view's
first person), with a block soon after. rspq's TEST_WRITE_STATUS
(RSPQ_CMD_TEST_WRITE_STATUS, rspq_internal.h) makes the RSP wait until the
CPU has taken the SYNC_FULL's interrupt, as libdragon's own SYNC_FULL
command waits for the one before it. Emulators do not hang there */
#define RSPQ_CMD_TEST_WRITE_STATUS 0x08

void n64_render_wait_sync_full(void)
{
	rspq_write(0, RSPQ_CMD_TEST_WRITE_STATUS, 0, SP_STATUS_SIG_RDPSYNCFULL);
}

/* The depth reset where the RDP is drawing: a fill of the z-buffer, held
to the scissor (the whole screen at the frame's start, a view for its
first-person weapon). tiny3d's t3d_screen_clear_depth (rdpq_clear_z)
cleared the whole buffer with the RSP after an RDP fence -- the RSP waited
for the RDP to finish all it had been given first, and again in each view's
first person: four stalls and four full-screen clears a frame with four
views. The fill comes after what was drawn before it in the RDP's own
order, and needs no fence (rdpq_detach's SYNC_FULL stays: libdragon's
__rdpq_clear_z found the RDP needs it after drawing to the z-buffer) */
static void clear_depth(void)
{
	uint32_t old_config;

	if (!frame_zbuf)
	{
		t3d_screen_clear_depth();
		n64_render_wait_sync_full();
		return;
	}
	debug_rdpq_mark("clear_depth");
	old_config = rdpq_config_disable(RDPQ_CFG_AUTOSCISSOR);
	rdpq_attach(frame_zbuf, NULL);
	rdpq_mode_push();
	rdpq_set_mode_fill(color_from_packed16(0xFFFC));
	rdpq_fill_rectangle(0, 0, frame_zbuf->width, frame_zbuf->height);
	rdpq_mode_pop();
	rdpq_detach();
	n64_render_wait_sync_full();
	rdpq_config_set(old_config);
}

void n64_render_frame_begin(void)
{
	unsigned long before_display = TICKS_READ();

	renderer.chunky = renderer.want_chunky;
	renderer.unstretched = 0;
	/* the uncached blocks' reserve, if a block took it: room made for it
	(the model cache drops what it must) and taken again */
	if (n64_uncached_reserve_missing() && display_mode != DISPLAY_MENU)
	{
		n64_render_make_room(N64_UNCACHED_RESERVE, 1);
		if (n64_uncached_reserve_take())
			debugf("n64: the uncached blocks' reserve was spent (%d so far), taken again\n",
				n64_uncached_reserve_spent());
	}
	if ((want_high_detail || want_hud_detail) && display_mode == DISPLAY_GAME && !tried_high_detail && level.data)
		try_high_detail();
	if (hud3d.buffer)
	{
		/* (the HUD at 640x240: the 3D into hud3d first, and the display's
		buffer taken only for the HUD, hud_display_phase -- with two
		buffers, the last frame still waiting to be shown while this one's
		3D is drawn) */
		hud_display = NULL;
		renderer.surface = &hud3d;
		screen_width = hud3d.width;
		screen_height = hud3d.height;
		hud_on_display = 0;
		hud_views_doubled = 0;
		if (renderer.view_total != hud_cleared_views)
			hud_clear_frames = 2;
		hud_cleared_views = renderer.view_total;
	}
	else
		renderer.surface = display_get();
	render_wait_us = TICKS_TO_US(TICKS_READ() - before_display);
	render_start_ticks = TICKS_READ();
	frame_zbuf = menu_zbuf.buffer ? &menu_zbuf :
		(display_mode == DISPLAY_GAME || hud3d.buffer) && game_zbuf.buffer ? &game_zbuf : display_get_zbuf();
	/* (a z-buffer without memory is one at address 0, over the code: stop
	and say so instead) */
	if (!frame_zbuf || !frame_zbuf->buffer)
	{
		char pieces[200], message[300];
		unsigned long largest = n64_heap_describe(pieces, sizeof(pieces));

		snprintf(message, sizeof(message), "no memory for the z-buffer: %lu KB free, largest %lu KB; %s",
			n64_heap_free() / 1024, largest / 1024, pieces);
		n64_fatal(message);
	}
	if (debug_rdpq_log_frame)
	{
		/* (rdpq_debug_log counts: on once, off once) */
		int logging = renderer.frame_index + 1 - debug_rdpq_log_frame < 2;

		if (logging != debug_rdpq_logging)
			rdpq_debug_log(logging);
		debug_rdpq_logging = logging;
	}
	rdpq_attach(renderer.surface, frame_zbuf);
	debug_rdpq_mark("frame begin");
	t3d_frame_start();
	/* past the draw distance: the fog's colour */
	t3d_screen_clear_color(renderer.fog ? renderer.fog_color : renderer.clear_color);
	clear_depth();
	renderer.view_count = 0;
	renderer.frame_index++;
	level.stream_reads_this_frame = 0;
	level.stream_waited = level.stream_waiting;
	level.stream_waiting = 0;
	tiles.reads_this_frame = 0;
	models.missing_last = models.missing;
	models.missing = 0;
	tiles.big_drawn_last = tiles.big_drawn;
	tiles.waiting_last = tiles.waiting;
	tiles.big_drawn = tiles.waiting = 0;
	renderer.matrix_count = 0;
	palette_count = 0;
#ifdef HALO_N64_CUSTOM_MAPS
	/* the view models first, before the world's take the heap */
	if (models.header && models.loaded && display_mode == DISPLAY_GAME)
	{
		for (int i = 0; i < models.header->model_count; i++)
		{
			if (first_person_model(i))
				load_model(i);
		}
	}
#endif
}

static void record_material(int index);

/* a material's mode and texture: recorded the first time it is used (the
CPU's work of working out the RDP's modes and texture upload, once) */
static void set_material(int index)
{
	int fog = renderer.fog != 0;

	if (!level.material_blocks[fog])
		level.material_blocks[fog] = calloc(level.header->material_count, sizeof(rspq_block_t *));
	if (!level.material_blocks[fog])
	{
		/* Same native commands without a recording when its table cannot
		   be allocated. Never dereference the failed allocation. */
		record_material(index);
		return;
	}
	if (!level.material_blocks[fog][index])
	{
		rspq_block_begin();
		record_material(index);
		level.material_blocks[fog][index] = rspq_block_end();
	}
	rspq_block_run(level.material_blocks[fog][index]);
}

/* a natural material's detail map over what its own pass drew (that pass
brightened by the overlay's mean, tools/n64/level.py): the framebuffer
darkened by the texel, its mipmaps interpolated; on the same triangles,
compared as decals; with the fog it fades to nothing with distance */
/* TERRAIN DETAIL (the SETTINGS page): how much of the natural terrain's
detail is drawn -- 0 full: the tiled ground's detail at every distance
(MATERIAL_TILED), the other detail maps' mipmaps down to 16x16
(detail_upload), and rock and earth's detail drawn over them
(record_overlay); 1 high: the tiled ground's detail faded out a few units
away, before it would shimmer; 2 medium: high without the overlays (a
second pass of every rock and earth batch); 3 low: no detail maps, the
ground's colour alone. (The modes kept for tracking down the black smudges
consoles showed on distant detailed ground -- every mipmap, no mipmaps --
went with the user's choice of these four: HISTORY.md, 13-15 October) */
enum { TERRAIN_FULL, TERRAIN_HIGH, TERRAIN_MEDIUM, TERRAIN_LOW };
static int terrain_detail;

void n64_render_set_terrain_detail(int mode)
{
	terrain_detail = mode >= 0 && mode <= TERRAIN_LOW ? mode : 0;
}

/* The detail maps are I4, 64x64, with mipmaps down to 4x4 (mksprite's BOX);
rdpq_sprite_upload uploads them all and has the RDP sample down to 8x8. Its
8x8 level has rows of 4 bytes, half a TMEM word (16 texels fill one): such a
level is loaded as I8 by LOAD_TILE into rows padded to the word, and that is
the one odd case on the path the smudges took -- far away, where the small
levels are sampled, never up close. The levels are uploaded here down to
smallest texels a side only, the RDP told of those alone; returns how many
(rdpq_mode_mipmap's count, the base level with them) */
#define DETAIL_SMALLEST 16

static int detail_upload(sprite_t *sprite, int smallest)
{
	rdpq_texparms_t parms = {.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE};
	surface_t pixels = sprite_get_pixels(sprite);
	int levels = 1;

	rdpq_tex_multi_begin();
	rdpq_tex_upload(TILE0, &pixels, &parms);
	for (int i = 1; i < 8; i++)
	{
		pixels = sprite_get_lod_pixels(sprite, i);
		if (!pixels.buffer || pixels.width < smallest || pixels.height < smallest)
			break;
		parms.s.scale_log++;
		parms.t.scale_log++;
		rdpq_tex_upload((rdpq_tile_t)((TILE0 + i) & 7), &pixels, &parms);
		levels++;
	}
	rdpq_tex_multi_end();
	return levels;
}

/* the mipmaps' blending for a detail map uploaded with this many levels
(detail_upload): none for one alone */
static void detail_mipmap(int levels)
{
	if (levels > 1)
		rdpq_mode_mipmap(MIPMAP_INTERPOLATE, levels);
	else
		rdpq_mode_mipmap(MIPMAP_NONE, 0);
}

static void record_overlay(struct level_material *material)
{
	rdpq_sync_pipe();
	rdpq_sync_tile();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_zbuf(true, false);
	rdpq_mode_zmode(ZMODE_DECAL);
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	/* (no alpha dither: the darkening is the alpha) */
	rdpq_mode_dithering(DITHER_SQUARE_NONE);
	if (n64_debug_flag("debug_overlay_alpha"))
	{
		/* (measuring: the overlay's fog alpha drawn as grey) */
		rdpq_mode_combiner(RDPQ_COMBINER1((1, 0, SHADE_ALPHA, 0), (0, 0, 0, 1)));
	}
	else
	{
		/* The darkening as the most ordinary of blends: black over the
		framebuffer at an alpha of 1 - texel (rdpq's multiply blender: the
		framebuffer times the texel), faded out by the fog (the shade's
		alpha is its visibility, 1 near). Real consoles drew black smudges
		on this pass away from the eye that no emulator shows (ares,
		parallel-RDP); it was a formula of its own (the framebuffer as the
		blender's first input, times the alpha) in two cycles (the mipmaps
		blended): both gone, in case either was the console's trouble */
		if (renderer.fog)
			rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, 0), (1, TEX0, SHADE, 0)));
		else
			rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, 0), (1, TEX0, ENV, 0)));
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	}
	rdpq_mode_end();
	rdpq_set_env_color(RGBA32(0, 0, 0, 255));
	/* the levels down to 16x16, blended (two cycles, as rdpq does with a
	sprite's mipmaps); an overlay of 4 KB (the texture picker's) has none */
	detail_mipmap(detail_upload(level.sprites[material->texture], DETAIL_SMALLEST));
	t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH | T3D_FLAG_TEXTURED |
		(debug_cull_front ? T3D_FLAG_CULL_FRONT : T3D_FLAG_CULL_BACK));
}

/* a material drawn with its batches' terrain tiles (bind_tile) */
static int material_tiled(const struct level_material *material)
{
	return (material->flags & MATERIAL_TILED) && tiles.data && material->texture >= 0 &&
		level.sprites[material->texture] && !debug_untextured;
}

static void record_material(int index)
{
	struct level_material *material = &level.materials[index];
	int tiled = material_tiled(material);
	int textured = tiled || (material->texture >= 0 && level.sprites[material->texture] && !debug_untextured &&
		/* (TERRAIN DETAIL low: the terrain's colour alone) */
		!(terrain_detail == TERRAIN_LOW && (material->flags & MATERIAL_DETAIL)));

	if (material->flags & MATERIAL_OVERLAY)
	{
		if (textured)
			record_overlay(material);
		return;
	}
	rdpq_sync_pipe();
	rdpq_sync_tile();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_zbuf(debug_fog_test != 4, true);
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_mode_dithering(DITHER_SQUARE_SQUARE);
	/* (rdpq keeps the shade's alpha, the fog, out of the standard
	combiners' alpha) */
	rdpq_mode_fog(renderer.fog ? RDPQ_FOG_STANDARD : 0);
	/* Halo's environment shader doubles texture x light; the converter
	doubled the textures, so one combiner cycle does */
	if (material->flags & MATERIAL_WATER)
	{
		/* water: the vertices' colour at a constant opacity (the shade's
		alpha is the fog) */
		rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, SHADE), (0, 0, 0, ENV)));
	}
	else if (tiled)
	{
		/* tiled terrain: the batch's tile (TILE1, bind_tile: colour x
		light) x (0.5 + detail), the detail's texel (TILE0) faded to grey,
		0.5, by the LOD fraction: 0 where a detail texel covers a pixel or
		more, 1 where two share one. There are no mipmaps -- the RDP works
		the fraction out for a combiner that asks for it all the same, and
		with the triangles' max level 1 it rises once, over that octave
		(further on the detail would only shimmer): TERRAIN DETAIL high and
		medium. Full: the detail at every distance; low: the tile alone */
		if (terrain_detail == TERRAIN_LOW)
			rdpq_mode_combiner(RDPQ_COMBINER2((0, 0, 0, 0), (0, 0, 0, 1), (0, 0, 0, TEX1), (0, 0, 0, 1)));
		else if (terrain_detail == TERRAIN_FULL)
			rdpq_mode_combiner(RDPQ_COMBINER2((0, 0, 0, TEX0), (0, 0, 0, 1),
				(COMBINED, ENV, TEX1, TEX1), (0, 0, 0, 1)));
		else
			rdpq_mode_combiner(RDPQ_COMBINER2((ENV, TEX0, LOD_FRAC, TEX0), (0, 0, 0, 1),
				(COMBINED, ENV, TEX1, TEX1), (0, 0, 0, 1)));
		rdpq_mode_mipmap(MIPMAP_NONE, 0);
		if (terrain_detail == TERRAIN_HIGH || terrain_detail == TERRAIN_MEDIUM)
			__rdpq_mode_change_som(SOMX_NUMLODS_MASK, 1ULL << SOMX_NUMLODS_SHIFT);
	}
	else if (textured && (material->flags & MATERIAL_DETAIL))
	{
		/* terrain: light x (0.5 + detail), the detail map's mipmaps
		interpolated by rdpq in a first pass (they fade to grey in the
		distance) */
		rdpq_mode_combiner(RDPQ_COMBINER1((TEX0, ENV, SHADE, SHADE), (0, 0, 0, 1)));
	}
	else if (textured && (material->flags & MATERIAL_DOUBLE))
		rdpq_mode_combiner(RDPQ_COMBINER2((TEX0, 0, SHADE, 0), (0, 0, 0, TEX0),
			(COMBINED, 0, ENV, COMBINED), (0, 0, 0, COMBINED)));
	else if (textured)
		rdpq_mode_combiner(RDPQ_COMBINER_TEX_SHADE);
	else
		rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
	if (material->flags & MATERIAL_TRANSPARENT)
	{
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		rdpq_mode_zbuf(true, false);
	}
	else if (material->flags & MATERIAL_ALPHA_TEST)
	{
		rdpq_mode_alphacompare(128);
	}
	/* The fog in the blender's first pass. In two cycles (a detail map's
	mipmaps, the doubling combiner) rdpq put it in the second, alone, and
	there a console reads the shade's alpha -- the fog -- of the next pixel
	(libdragon's validator: a hardware bug no emulator shows; these are the
	materials real consoles smudged). With a second pass of its own -- the
	colour as it comes, in x 0 + in x 1 (in x a + in x (1 - a) encodes as
	all zeros, which rdpq_mode_blender takes for no blender) -- rdpq keeps the
	fog in the first. (Only where it is two cycles anyway: fog and a
	blender together are two cycles) */
	/* (Settings: ANTI-ALIASING, on the opaque surfaces: the blender's second
	step is then the anti-aliasing's, the fog kept in the first, so the
	pass-through below is not needed) */
	int antialias = want_antialias && !(material->flags & (MATERIAL_TRANSPARENT | MATERIAL_WATER));

	if (antialias)
		rdpq_mode_antialias(AA_STANDARD);
	else if (renderer.fog && textured && !(material->flags & MATERIAL_TRANSPARENT) &&
		(material->flags & (MATERIAL_DOUBLE | MATERIAL_DETAIL)))
		rdpq_mode_blender(RDPQ_BLENDER((IN_RGB, ZERO, IN_RGB, ONE)));
	rdpq_mode_end();
	if (material->flags & MATERIAL_WATER)
		rdpq_set_env_color(RGBA32(255, 255, 255, WATER_ALPHA));
	else if (textured && (material->flags & MATERIAL_DETAIL))
		rdpq_set_env_color(RGBA32(128, 128, 128, 255));
	else if (textured && (material->flags & MATERIAL_DOUBLE))
		rdpq_set_env_color(RGBA32(255, 255, 255, 255));
	if (tiled)
	{
		/* the detail map's level the tiles are drawn under, in TMEM before
		them (bind_tile); a grain without mipmaps (the texture picker's) is
		drawn whole, the batches' coordinates in its texels */
		surface_t pixels = sprite_get_lod_pixels(level.sprites[material->texture], tiles.header->detail_level);

		if (!pixels.buffer)
			pixels = sprite_get_pixels(level.sprites[material->texture]);

		rdpq_tex_upload(TILE0, &pixels, &(rdpq_texparms_t){
			.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE,
		});
	}
	else if (textured && (material->flags & MATERIAL_DETAIL))
	{
		/* a detail map: its levels down to 16x16 (detail_upload), blended */
		detail_mipmap(detail_upload(level.sprites[material->texture], DETAIL_SMALLEST));
	}
	else if (textured)
	{
		upload_sprite(level.sprites[material->texture], &(rdpq_texparms_t){
			.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE,
		});
	}
	t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH |
		(textured ? T3D_FLAG_TEXTURED : 0) |
		(debug_cull_front ? T3D_FLAG_CULL_FRONT : (material->flags & MATERIAL_TWO_SIDED) ? 0 : T3D_FLAG_CULL_BACK));
}

/* a box (world units) wholly past the fog's end, where it would be drawn
in the fog's colour alone */
static int beyond_fog(float x0, float y0, float z0, float x1, float y1, float z1)
{
	float dx, dy, dz;

	if (!renderer.fog || debug_fog_test == 1)
		return 0;
	dx = renderer.eye[0] < x0 ? x0 - renderer.eye[0] : renderer.eye[0] > x1 ? renderer.eye[0] - x1 : 0.f;
	dy = renderer.eye[1] < y0 ? y0 - renderer.eye[1] : renderer.eye[1] > y1 ? renderer.eye[1] - y1 : 0.f;
	dz = renderer.eye[2] < z0 ? z0 - renderer.eye[2] : renderer.eye[2] > z1 ? renderer.eye[2] - z1 : 0.f;
	return dx * dx + dy * dy + dz * dz > renderer.fog_end * renderer.fog_end;
}

/* a level batch: its vertices, then its strips (tiny3d reads their indices
by DMA into the vertex cache's free end: the converter leaves six slots) */
/* a streamed level's chunk made ready to draw: in memory already, or read
now (its strips converted for tiny3d) into a free slot if this frame may
still read one -- the chunk drawn longest ago gives its slot up when none is
free, unless it was drawn in the frames still in flight. 0: not this frame */
static int stream_chunk(int k)
{
	struct level_chunk *chunk = &level.chunks[k];
	unsigned long size = chunk->vertex_size + chunk->index_size;
	uint8_t *block;
	int slot = -1;

	if (level.resident[k])
	{
		level.resident_drawn[k] = renderer.frame_index;
		return 1;
	}
	if (!size || size > level.slot_size || level.stream_reads_this_frame >= LEVEL_READS_PER_FRAME)
		return 0;
	for (int i = 0; i < level.slot_count && slot < 0; i++)
	{
		if (level.slot_chunk[i] < 0)
			slot = i;
	}
	if (slot < 0)
	{
		int oldest = -1;

		for (int i = 0; i < level.slot_count; i++)
		{
			int other = level.slot_chunk[i];

			if (level.resident_drawn[other] + FRAMES_IN_FLIGHT < renderer.frame_index &&
				(oldest < 0 || level.resident_drawn[other] < level.resident_drawn[level.slot_chunk[oldest]]))
				oldest = i;
		}
		if (oldest < 0)
			return 0;
		slot = oldest;
		level.resident[level.slot_chunk[slot]] = NULL;
		level.chunk_slot[level.slot_chunk[slot]] = -1;
		level.resident_bytes -= level.slot_size;
		level.slot_chunk[slot] = -1;
	}
	block = level.slots + slot * level.slot_size;
	/* Count a failed read too: corrupt/unavailable media cannot make a
	   frame repeatedly retry unbounded chunks. Don't publish partial data. */
	level.stream_reads_this_frame++;
	/* (the slot may hold its last chunk in the CPU's data cache: the read
	is a DMA to RDRAM, and the conversion below must see the new data, as
	the geometry stream does before its dma_read) */
	data_cache_hit_writeback_invalidate(block, size);
	if (fseek(level.stream, level.header->vertices_offset + chunk->vertex_offset, SEEK_SET) ||
		fread(block, 1, chunk->vertex_size, level.stream) != chunk->vertex_size ||
		fseek(level.stream, level.header->indices_offset + chunk->index_offset, SEEK_SET) ||
		fread(block + chunk->vertex_size, 1, chunk->index_size, level.stream) != chunk->index_size)
		return 0;
	for (int b = chunk->first_batch; b < chunk->first_batch + chunk->batch_count; b++)
	{
		unsigned long start = level.batches[b].index_offset;
		unsigned long end = b + 1 < chunk->first_batch + chunk->batch_count ? level.batches[b + 1].index_offset : chunk->index_offset + chunk->index_size;
		if (start < chunk->index_offset || end <= start || end > chunk->index_offset + chunk->index_size)
			return 0;
		if (!convert_level_strips(block + chunk->vertex_size + start - chunk->index_offset, end - start))
			return 0;
	}
	/* (the RSP reads them by DMA) */
	data_cache_hit_writeback(block, size);
	level.resident[k] = block;
	level.resident_drawn[k] = renderer.frame_index;
	level.slot_chunk[slot] = (int16_t)k;
	level.chunk_slot[k] = (int16_t)slot;
	level.resident_bytes += level.slot_size;
	level.stream_reads++;
	return 1;
}

/* a terrain batch's big tile made ready: in a slot already, or read into
one now if this frame may (the one drawn longest ago gives its slot up,
unless it was drawn in the frames still in flight). NULL: not this frame */
static uint8_t *stream_tile(int e)
{
	const struct tile_entry *entry = &tiles.entries[e];
	unsigned long bytes = 2UL << ((entry->big_size >> 4) + (entry->big_size & 15));
	int slot = tiles.entry_slot[e];
	uint8_t *block;

	if (slot >= 0)
	{
		tiles.entry_drawn[e] = renderer.frame_index;
		return tiles.slots + slot * tiles.slot_size;
	}
	if (!tiles.slot_count || bytes > tiles.slot_size || tiles.reads_this_frame >= TILE_READS_PER_FRAME)
		return NULL;
	for (int i = 0; i < tiles.slot_count && slot < 0; i++)
	{
		if (tiles.slot_entry[i] < 0)
			slot = i;
	}
	if (slot < 0)
	{
		int oldest = -1;

		for (int i = 0; i < tiles.slot_count; i++)
		{
			int other = tiles.slot_entry[i];

			if (tiles.entry_drawn[other] + FRAMES_IN_FLIGHT < renderer.frame_index &&
				(oldest < 0 || tiles.entry_drawn[other] < tiles.entry_drawn[tiles.slot_entry[oldest]]))
				oldest = i;
		}
		if (oldest < 0)
			return NULL;
		slot = oldest;
		tiles.entry_slot[tiles.slot_entry[slot]] = -1;
		tiles.slot_entry[slot] = -1;
	}
	block = tiles.slots + slot * tiles.slot_size;
	tiles.reads_this_frame++;
	if (fseek(tiles.file, tiles.header->big_offset + entry->big_offset, SEEK_SET))
		return NULL;
	if (fread(block, 1, bytes, tiles.file) != bytes)
		return NULL;
	/* (the RDP reads it) */
	data_cache_hit_writeback(block, bytes);
	tiles.slot_entry[slot] = (int16_t)e;
	tiles.entry_slot[e] = (int16_t)slot;
	tiles.entry_drawn[e] = renderer.frame_index;
	return block;
}

/* a batch's box within TILE_NEAR_UNITS of the eye (the view's, in the
batch's own units: draw_level's fog_eye) */
static int fog_eye[3];

static inline int batch_near(const struct level_batch *batch)
{
	float d2 = 0.f;

	for (int k = 0; k < 3; k++)
	{
		int d = fog_eye[k] < batch->minimum[k] ? batch->minimum[k] - fog_eye[k] :
			fog_eye[k] > batch->maximum[k] ? fog_eye[k] - batch->maximum[k] : 0;

		d2 += (float)d * (float)d;
	}
	return d2 < tiles.near_squared;
}

/* a tiled terrain batch's tile into TMEM (TILE1, after the detail map's
level): its big one near the eye if it is in memory or can be read now,
else the small one -- each addressed by the batch's detail coordinates
shifted right, wrapping (tools/n64/terrain_tiles.py) */
static void bind_tile(int b)
{
	int e = tiles.batch_entry[b];
	const struct tile_entry *entry;
	uint8_t *pixels;
	int shift_s, shift_t, size;
	surface_t surface;

	if (e < 0)
		return;
	entry = &tiles.entries[e];
	pixels = tiles.resident + entry->offset;
	shift_s = entry->shift_s;
	shift_t = entry->shift_t;
	size = entry->size;
	if (entry->big_size && batch_near(&level.batches[b]))
	{
		uint8_t *big = stream_tile(e);

		if (big)
		{
			pixels = big;
			shift_s = entry->big_shift_s;
			shift_t = entry->big_shift_t;
			size = entry->big_size;
			tiles.big_drawn++;
		}
		else
			tiles.waiting++;
	}
	surface = surface_make(pixels, FMT_RGBA16, 1 << (size >> 4), 1 << (size & 15), 2 << (size >> 4));
	rdpq_tex_upload(TILE1, &surface, &(rdpq_texparms_t){
		.tmem_addr = tiles.tmem,
		.s = {.scale_log = shift_s, .repeats = REPEAT_INFINITE},
		.t = {.scale_log = shift_t, .repeats = REPEAT_INFINITE},
	});
}

static struct
{
    unsigned long requests, hits, misses, wraps, bytes;
    unsigned long long dma_ticks, convert_ticks, wait_ticks;
} geometry_profile;
static int geometry_profile_enabled = -1;

static void draw_batch(int b)
{
	const struct level_batch *batch = &level.batches[b];
	T3DVertPacked *vertices = NULL;
	uint8_t *chunk = NULL;

	if (level.geometry_cache)
	{
		unsigned long profile_start = 0;
		if (geometry_profile_enabled < 0)
			geometry_profile_enabled = n64_debug_flag("debug_geometry_profile");
		if (geometry_profile_enabled)
		{
			geometry_profile.requests++;
			if (level.geometry_offsets[b] != 0xffffffff) geometry_profile.hits++;
			else geometry_profile.misses++;
		}
		unsigned long vertex_bytes = ((batch->vertex_count + 1) / 2) * sizeof(T3DVertPacked);
		unsigned long index_bytes = level.index_sizes[b];
		unsigned long bytes = (vertex_bytes + index_bytes + 15) & ~15UL;
		if (bytes > 64 * 1024) n64_fatal("native geometry batch exceeds stream cache");
		if (level.geometry_offsets[b] == 0xffffffff)
		{
			if (level.geometry_used + bytes > 64 * 1024)
			{
				if (geometry_profile_enabled) profile_start = TICKS_READ();
				rspq_wait();
				if (geometry_profile_enabled)
				{
					geometry_profile.wraps++;
					geometry_profile.wait_ticks += TICKS_READ() - profile_start;
				}
				level.geometry_used = 0;
				memset(level.geometry_offsets, 0xff, level.header->batch_count * sizeof(uint32_t));
			}
			uint8_t *to = level.geometry_cache + level.geometry_used;
			data_cache_hit_writeback_invalidate(to, bytes);
			if (geometry_profile_enabled) profile_start = TICKS_READ();
			dma_read(to, level.geometry_rom + level.header->vertices_offset + batch->vertex_pair_offset * sizeof(T3DVertPacked), vertex_bytes);
			dma_read(to + vertex_bytes, level.geometry_rom + level.header->indices_offset + batch->index_offset, index_bytes);
			if (geometry_profile_enabled)
			{
				geometry_profile.bytes += vertex_bytes + index_bytes;
				geometry_profile.dma_ticks += TICKS_READ() - profile_start;
				profile_start = TICKS_READ();
			}
			if (!convert_level_strips(to + vertex_bytes, index_bytes))
				n64_fatal("invalid streamed level strip");
			data_cache_hit_writeback(to + vertex_bytes, index_bytes);
			if (geometry_profile_enabled) geometry_profile.convert_ticks += TICKS_READ() - profile_start;
			level.geometry_offsets[b] = level.geometry_used;
			level.geometry_used += bytes;
		}
		vertices = (T3DVertPacked *)(level.geometry_cache + level.geometry_offsets[b]);
		chunk = (uint8_t *)vertices + vertex_bytes;
	}
	else if (level.stream)
	{
		int k = level.batch_chunk[b];
		struct level_chunk *data = &level.chunks[k];

		vertices = (T3DVertPacked *)(level.resident[k] + batch->vertex_pair_offset * sizeof(T3DVertPacked) -
			data->vertex_offset);
		chunk = level.resident[k] + data->vertex_size + (batch->index_offset - data->index_offset);
	}
	else
	{
		vertices = level.vertices + batch->vertex_pair_offset;
		chunk = level.indices + batch->index_offset;
	}
	t3d_vert_load(vertices, 0, batch->vertex_count);
	/* (measuring, "debug_level_parts=1": the vertices alone) */
	while (debug_level_parts != 1)
	{
		uint16_t count = ((uint16_t *)chunk)[0], more = ((uint16_t *)chunk)[1];

		/* the last strip with the triangles' sync: tiny3d's strip command
		runs on into its sync command (T3DCmd_TriDraw_Strip), one RSP
		command fewer a batch */
		if (!more)
		{
			t3d_tri_draw_strip_and_sync((int16_t *)(chunk + 8), count);
			return;
		}
		t3d_tri_draw_strip((int16_t *)(chunk + 8), count);
		chunk += 8 + ((count * 2 + 7) & ~7);
	}
	t3d_tri_sync();
}

/* a box wholly inside the frustum (its corner nearest each plane's
outside within it) */
static int frustum_contains(const T3DFrustum *frustum, const T3DVec3 *minimum, const T3DVec3 *maximum)
{
	for (int i = 0; i < 6; i++)
	{
		const float *plane = frustum->planes[i].v;
		float d = plane[3];

		for (int k = 0; k < 3; k++)
			d += plane[k] * (plane[k] >= 0.f ? minimum->v[k] : maximum->v[k]);
		if (d < 0.f)
			return 0;
	}
	return 1;
}

/* a box wholly nearer than the fog's end (or no fog) */
static int within_fog(float x0, float y0, float z0, float x1, float y1, float z1)
{
	float dx, dy, dz;

	if (!renderer.fog || debug_fog_test == 1)
		return 1;
	dx = n64_max_f(fabsf(renderer.eye[0] - x0), fabsf(x1 - renderer.eye[0]));
	dy = n64_max_f(fabsf(renderer.eye[1] - y0), fabsf(y1 - renderer.eye[1]));
	dz = n64_max_f(fabsf(renderer.eye[2] - z0), fabsf(z1 - renderer.eye[2]));
	return dx * dx + dy * dy + dz * dz <= renderer.fog_end * renderer.fog_end;
}

/* beyond_fog for a batch, in the batch's own units (world units x
WORLD_SCALE, 16 bits): the eye's position in them made once a view
(draw_level; fog_eye, by bind_tile). The six conversions and products a
batch took to world units were 2% of a 4-player frame */
static float fog_end_squared_units;

static inline int batch_beyond_fog(const int16_t *minimum, const int16_t *maximum)
{
	int d[3];
	float dx, dy, dz;

	if (!renderer.fog || debug_fog_test == 1)
		return 0;
	for (int k = 0; k < 3; k++)
		d[k] = fog_eye[k] < minimum[k] ? minimum[k] - fog_eye[k] : fog_eye[k] > maximum[k] ? fog_eye[k] - maximum[k] : 0;
	dx = (float)d[0];
	dy = (float)d[1];
	dz = (float)d[2];
	return dx * dx + dy * dy + dz * dz > fog_end_squared_units;
}

/* The frustum's planes (a bit each) a box of batches lies wholly inside by
t3d_frustum_vs_aabb_s16's own sums: all eight of its corners pass that
test's comparison, in the same float operations (the library is built
without reassociation, so they are these). A batch inside the box passes
the same comparison at each such plane: every product, sum and difference
is rounded monotonically, so none of its corners comes out worse than the
box's worst. Those planes are left out of its test (batch_in_frustum): the
same batches in view, where a cluster or group crossing one plane had each
of its batches tested against all six */
static unsigned frustum_planes_inside(const T3DFrustum *frustum, const float *minimum, const float *maximum)
{
	unsigned inside = 0;

	for (int i = 0; i < 6; i++)
	{
		const float *plane = frustum->planes[i].v;
		int passed = 0;

		for (int corner = 0; corner < 8; corner++)
		{
			float p0 = plane[0] * (corner & 1 ? maximum : minimum)[0];
			float p1 = plane[1] * (corner & 2 ? maximum : minimum)[1];
			float p2AndW = -plane[3] - plane[2] * (corner & 4 ? maximum : minimum)[2];

			passed += p0 + p1 > p2AndW;
		}
		if (passed == 8)
			inside |= 1u << i;
	}
	return inside;
}

/* t3d_frustum_vs_aabb_s16 (tiny3d's t3dmath.c, the same sums) at the planes
not in inside (frustum_planes_inside: those it would pass). (Called, as
tiny3d's was: inlined twice it took a kilobyte) */
static __attribute__((noinline)) int batch_in_frustum(const T3DFrustum *frustum, const int16_t min[3], const int16_t max[3], unsigned inside)
{
	float min0 = min[0], max0 = max[0], min1 = min[1], max1 = max[1], min2 = min[2], max2 = max[2];

	for (int i = 0; i < 6; i++)
	{
		const float *plane = frustum->planes[i].v;

		if (inside & (1u << i))
			continue;
		float p0Min = plane[0] * min0;
		float p0Max = plane[0] * max0;
		float p1Min = plane[1] * min1;
		float p1Max = plane[1] * max1;
		float p2MinAndW = -plane[3] - plane[2] * min2;

		if (p0Min + p1Min > p2MinAndW) continue;
		if (p0Max + p1Min > p2MinAndW) continue;
		if (p0Min + p1Max > p2MinAndW) continue;
		if (p0Max + p1Max > p2MinAndW) continue;

		float p2MaxAndW = -plane[3] - plane[2] * max2;

		if (p0Min + p1Min > p2MaxAndW) continue;
		if (p0Max + p1Min > p2MaxAndW) continue;
		if (p0Min + p1Max > p2MaxAndW) continue;
		if (p0Max + p1Max > p2MaxAndW) continue;
		return 0;
	}
	return 1;
}

static void draw_level(T3DViewport *viewport, const struct n64_view *view)
{
	int visible_count = 0;
	int material_count;
	int tiled;
	static int no_level = -1;

	/* (a debug flag looked up once) */
	if (no_level < 0)
		no_level = n64_debug_flag("debug_no_level");
	if (!level.data || no_level)
		return;
	for (int k = 0; k < 3; k++)
		fog_eye[k] = (int)floorf(renderer.eye[k] * WORLD_SCALE + 0.5f);
	fog_end_squared_units = renderer.fog_end * WORLD_SCALE * renderer.fog_end * WORLD_SCALE;
	material_count = level.header->material_count;
	/* the batches in view (the eye's potentially visible set of clusters,
	the frustum)... */
	for (int c = 0; c < level.header->cluster_count; c++)
	{
		struct level_cluster *cluster = &level.clusters[c];
		T3DVec3 minimum = {{cluster->minimum[0] * WORLD_SCALE, cluster->minimum[1] * WORLD_SCALE, cluster->minimum[2] * WORLD_SCALE}};
		T3DVec3 maximum = {{cluster->maximum[0] * WORLD_SCALE, cluster->maximum[1] * WORLD_SCALE, cluster->maximum[2] * WORLD_SCALE}};

		if (!cluster->batch_count)
			continue;
		/* out of the eye's potentially visible set: none of its owners in
		it (one past the BSP's own clusters counts as in it; surfaces no
		cluster listed have none and are always drawn) */
		if (view->visible_clusters && cluster->owner_count)
		{
			int seen = 0;

			for (int k = cluster->first_owner; k < cluster->first_owner + cluster->owner_count && !seen; k++)
			{
				int owner = level.owners[k];

				seen = owner >= view->cluster_count || (view->visible_clusters[owner >> 5] & (1UL << (owner & 31)));
			}
			if (!seen)
				continue;
		}
		if (!t3d_frustum_vs_aabb(&viewport->viewFrustum, &minimum, &maximum) ||
			beyond_fog(cluster->minimum[0], cluster->minimum[1], cluster->minimum[2],
				cluster->maximum[0], cluster->maximum[1], cluster->maximum[2]))
			continue;
		renderer.clusters++;
		/* a streamed level's cluster: its chunks in view (each in memory, or
		read now if it may be), their batches as a group's */
		if (level.stream)
		{
			struct level_cluster_chunks *own = &level.cluster_chunks[c];

			for (int k = own->first; k < own->first + own->count; k++)
			{
				struct level_chunk *chunk = &level.chunks[k];
				T3DVec3 chunk_minimum = {{chunk->minimum[0] * WORLD_SCALE, chunk->minimum[1] * WORLD_SCALE, chunk->minimum[2] * WORLD_SCALE}};
				T3DVec3 chunk_maximum = {{chunk->maximum[0] * WORLD_SCALE, chunk->maximum[1] * WORLD_SCALE, chunk->maximum[2] * WORLD_SCALE}};
				int inside;

				if (!t3d_frustum_vs_aabb(&viewport->viewFrustum, &chunk_minimum, &chunk_maximum) ||
					beyond_fog(chunk->minimum[0], chunk->minimum[1], chunk->minimum[2],
						chunk->maximum[0], chunk->maximum[1], chunk->maximum[2]))
					continue;
				if (!stream_chunk(k))
				{
					level.stream_waiting++;
					continue;
				}
				inside = frustum_contains(&viewport->viewFrustum, &chunk_minimum, &chunk_maximum) &&
					within_fog(chunk->minimum[0], chunk->minimum[1], chunk->minimum[2],
						chunk->maximum[0], chunk->maximum[1], chunk->maximum[2]);
				for (int b = chunk->first_batch; b < chunk->first_batch + chunk->batch_count; b++)
				{
					struct level_batch *batch = &level.batches[b];

					if (inside || (t3d_frustum_vs_aabb_s16(&viewport->viewFrustum, batch->minimum, batch->maximum) &&
						!batch_beyond_fog(batch->minimum, batch->maximum)))
						level.visible[visible_count++] = (uint16_t)b;
				}
			}
			continue;
		}
		/* a cluster wholly in view and in the fog's reach: its batches
		without a test each (those were 4% of a 4-player frame) */
		if (frustum_contains(&viewport->viewFrustum, &minimum, &maximum) &&
			within_fog(cluster->minimum[0], cluster->minimum[1], cluster->minimum[2],
				cluster->maximum[0], cluster->maximum[1], cluster->maximum[2]))
		{
			for (int b = cluster->first_batch; b < cluster->first_batch + cluster->batch_count; b++)
				level.visible[visible_count++] = (uint16_t)b;
			continue;
		}
		/* a big cluster partly in view: its groups first, then the
		batches of those partly in view (build_level_groups) */
		if (level.cluster_group_count && level.cluster_group_count[c])
		{
			int first_group = level.cluster_first_group[c];

			for (int g = first_group; g < first_group + level.cluster_group_count[c]; g++)
			{
				struct level_group *group = &level.groups[g];
				T3DVec3 group_minimum = {{group->minimum[0] * WORLD_SCALE, group->minimum[1] * WORLD_SCALE, group->minimum[2] * WORLD_SCALE}};
				T3DVec3 group_maximum = {{group->maximum[0] * WORLD_SCALE, group->maximum[1] * WORLD_SCALE, group->maximum[2] * WORLD_SCALE}};
				uint16_t *list = level.group_batches + group->first;

				if (!t3d_frustum_vs_aabb(&viewport->viewFrustum, &group_minimum, &group_maximum) ||
					beyond_fog(group->minimum[0], group->minimum[1], group->minimum[2],
						group->maximum[0], group->maximum[1], group->maximum[2]))
					continue;
				if (frustum_contains(&viewport->viewFrustum, &group_minimum, &group_maximum) &&
					within_fog(group->minimum[0], group->minimum[1], group->minimum[2],
						group->maximum[0], group->maximum[1], group->maximum[2]))
				{
					for (int i = 0; i < group->count; i++)
						level.visible[visible_count++] = list[i];
					continue;
				}
				/* (the group's box is its batches' boxes together, exactly:
				split_level_group's quarters of a 64th) */
				unsigned inside = frustum_planes_inside(&viewport->viewFrustum, group_minimum.v, group_maximum.v);

				for (int i = 0; i < group->count; i++)
				{
					struct level_batch *batch = &level.batches[list[i]];

					if (batch_in_frustum(&viewport->viewFrustum, batch->minimum, batch->maximum, inside) &&
						!batch_beyond_fog(batch->minimum, batch->maximum))
						level.visible[visible_count++] = list[i];
				}
			}
			continue;
		}
		unsigned inside = 0;

		if (level.cluster_boxes && level.cluster_boxes[c][0] <= level.cluster_boxes[c][3])
		{
			const int16_t *box = level.cluster_boxes[c];
			float box_minimum[3] = {box[0], box[1], box[2]}, box_maximum[3] = {box[3], box[4], box[5]};

			inside = frustum_planes_inside(&viewport->viewFrustum, box_minimum, box_maximum);
		}
		for (int b = cluster->first_batch; b < cluster->first_batch + cluster->batch_count; b++)
		{
			struct level_batch *batch = &level.batches[b];

			if (batch_in_frustum(&viewport->viewFrustum, batch->minimum, batch->maximum, inside) &&
				!batch_beyond_fog(batch->minimum, batch->maximum))
				level.visible[visible_count++] = (uint16_t)b;
		}
	}
	/* ...drawn a material at a time: changing material runs rdpq's RSP
	code in place of tiny3d's and back, which cost more than the batch
	itself when the materials alternated cluster by cluster */
	memset(level.material_starts, 0, (material_count + 1) * sizeof(uint16_t));
	for (int i = 0; i < visible_count; i++)
		level.material_starts[level.batches[level.visible[i]].material + 1]++;
	for (int m = 0; m < material_count; m++)
		level.material_starts[m + 1] += level.material_starts[m];
	for (int i = 0; i < visible_count; i++)
		level.sorted[level.material_starts[level.batches[level.visible[i]].material]++] = level.visible[i];
	/* (material m's batches end at material_starts[m] now); the opaque
	materials first, then the overlays over them (natural materials'
	detail), then the transparent ones. The level's vertices are in the
	world's place: loaded under the camera's matrix as the viewport left it
	(an identity pushed on it made the same numbers on the RSP, its 16.16
	product by 1 and 0 exact, at a matrix push and pop a view) */
	for (int pass = 0; pass < 3; pass++)
	{
		for (int m = 0; m < material_count; m++)
		{
			int first = m ? level.material_starts[m - 1] : 0;
			int flags = level.materials[m].flags;
			int material_pass = flags & MATERIAL_TRANSPARENT ? 2 : flags & MATERIAL_OVERLAY ? 1 : 0;

			if (first == level.material_starts[m] || material_pass != pass)
				continue;
			if (pass == 1 && (debug_untextured || level.materials[m].texture < 0 ||
				!level.sprites[level.materials[m].texture] || n64_debug_flag("debug_no_overlay") ||
				terrain_detail == TERRAIN_MEDIUM || terrain_detail == TERRAIN_LOW))
				continue;
			set_material(m);
			tiled = material_tiled(&level.materials[m]);
			for (int i = first; i < level.material_starts[m]; i++)
			{
				int b = level.sorted[i];

				if (tiled)
					bind_tile(b);
				/* (measuring, "debug_level_parts=2": the materials alone) */
				if (debug_level_parts != 2)
					draw_batch(b);
				renderer.triangles += level.batches[b].triangle_count;
				renderer.batches++;
			}
		}
	}
}

/* the current view as the particles and the paint project (n64_particles.c,
n64_paint.c): the world's, or the first-person weapon's */
static void particle_view(struct n64_particle_view *view, int first_person)
{
	short const *rect = renderer.rects[renderer.view_count - 1];

	for (int k = 0; k < 3; k++)
	{
		view->eye[k] = renderer.eye[k];
		view->right[k] = renderer.right[k];
		view->up[k] = renderer.up[k];
		view->forward[k] = renderer.forward[k];
	}
	view->pixels_per_unit = renderer.pixels_per_unit;
	/* (the pixels per unit are the height's; across, as wide as a pixel
	shows) */
	view->x_scale = 1.f / display_pixel_aspect();
	view->x0 = rect[0];
	view->y0 = rect[1];
	view->x1 = rect[2];
	view->y1 = rect[3];
	view->center_x = (rect[0] + rect[2]) * 0.5f;
	view->center_y = (rect[1] + rect[3]) * 0.5f;
	view->z_near = first_person ? 0.01171875f : renderer.z_near;
	view->z_far = first_person ? 16.f : renderer.z_far;
	view->fog_start = renderer.fog && !first_person ? renderer.fog_start : 0.f;
	view->fog_end = renderer.fog && !first_person ? renderer.fog_end : 0.f;
	view->local_player = renderer.local_player;
	view->first_person = first_person;
	view->chunky = renderer.unstretched;
	view->chunky_scale = renderer.unstretched ?
		(float)chunky_width(rect[2] - rect[0]) / (float)(rect[2] - rect[0]) : 1.f;
}

void n64_render_view(const struct n64_view *view)
{
	T3DViewport *viewport;
	T3DVec3 eye, target, up;
	uint8_t white[4] = {0xFF, 0xFF, 0xFF, 0xFF};

	/* (the audio fed between views: a 4-player frame is long) */
	n64_sound_poll();
	if (renderer.view_count >= MAXIMUM_VIEWS)
		return;
	flush_draws();
	stretch_view();
	hud_3d_phase();
	renderer.rects[renderer.view_count][0] = view->x0;
	renderer.rects[renderer.view_count][1] = view->y0;
	renderer.rects[renderer.view_count][2] = view->x1;
	renderer.rects[renderer.view_count][3] = view->y1;
	viewport = &renderer.viewports[renderer.view_count++];
	if (!(renderer.frame_index % 30) && n64_debug_flag("debug_stats"))
	{
		debugf("view %d: at %.2f %.2f %.2f forward %.2f %.2f %.2f up %.2f %.2f %.2f fov %.2f rect %d,%d-%d,%d\n",
			renderer.view_count - 1, view->position[0], view->position[1], view->position[2],
			view->forward[0], view->forward[1], view->forward[2], view->up[0], view->up[1], view->up[2],
			view->vertical_field_of_view, view->x0, view->y0, view->x1, view->y1);
	}
	t3d_viewport_set_area(viewport, view->x0, view->y0, chunky_width(view->x1 - view->x0), view->y1 - view->y0);
	renderer.unstretched = renderer.chunky != 0;
	/* the view's shape on the screen (in chunky mode drawn half as wide) */
	renderer.aspect = (float)(view->x1 - view->x0) / (float)(view->y1 - view->y0) * display_pixel_aspect();
	/* (the far plane stays the game's: tiny3d does not clip at it, so big
	triangles across a near far plane came and went as the eye moved; what
	lies wholly past the fog's end is left out instead, beyond_fog) */
	t3d_viewport_set_perspective(viewport, view->vertical_field_of_view, renderer.aspect,
		view->z_near * WORLD_SCALE, view->z_far * WORLD_SCALE);
	renderer.fov = view->vertical_field_of_view;
	renderer.z_near = view->z_near;
	renderer.z_far = view->z_far;
	renderer.local_player = view->local_player_index;
	renderer.world_particles_drawn = 0;
	renderer.eye[0] = view->position[0];
	renderer.eye[1] = view->position[1];
	renderer.eye[2] = view->position[2];
	renderer.pixels_per_unit = (view->y1 - view->y0) * 0.5f / tanf(view->vertical_field_of_view * 0.5f);
	eye = (T3DVec3){{view->position[0] * WORLD_SCALE, view->position[1] * WORLD_SCALE, view->position[2] * WORLD_SCALE}};
	target = (T3DVec3){{eye.v[0] + view->forward[0] * WORLD_SCALE, eye.v[1] + view->forward[1] * WORLD_SCALE,
		eye.v[2] + view->forward[2] * WORLD_SCALE}};
	up = (T3DVec3){{view->up[0], view->up[1], view->up[2]}};
	if (debug_overview && renderer.view_count == 1)
	{
		/* the first view looks down on the whole level from above */
		eye = (T3DVec3){{0, -12 * WORLD_SCALE, 30 * WORLD_SCALE}};
		target = (T3DVec3){{0, 0, 0}};
		up = (T3DVec3){{0, 0, 1}};
	}
	t3d_viewport_look_at(viewport, &eye, &target, &up);
	/* Paint and particles must project through the same orthonormal camera
	   as world geometry. Incoming forward/up need not be perpendicular
	   (notably pitched debug/chase views); Tiny3D corrects them here. */
	for (int k = 0; k < 3; k++)
	{
		renderer.right[k] = viewport->matCamera.m[k][0];
		renderer.up[k] = viewport->matCamera.m[k][1];
		renderer.forward[k] = -viewport->matCamera.m[k][2];
		renderer.eye[k] = eye.v[k] / WORLD_SCALE;
	}
	{
		/* the frustum's far plane from the eye and its direction: tiny3d
		takes it as the difference of two projection rows that agree to
		four digits with Halo's near plane, so in floats it landed anywhere
		and moved as the eye turned -- the distant level and objects came
		and went from frame to frame */
		float fx = target.v[0] - eye.v[0], fy = target.v[1] - eye.v[1], fz = target.v[2] - eye.v[2];
		float length = sqrtf(fx * fx + fy * fy + fz * fz);
		float far = (renderer.fog && renderer.fog_end < view->z_far ? renderer.fog_end : view->z_far) * WORLD_SCALE;

		if (length > 0.f)
		{
			fx /= length, fy /= length, fz /= length;
			viewport->viewFrustum.planes[5] = (T3DVec4){{-fx, -fy, -fz, fx * eye.v[0] + fy * eye.v[1] + fz * eye.v[2] + far}};
		}
	}
	t3d_viewport_attach(viewport);
	/* the vertex colours are the lighting: no lights, a white ambient --
	set with tiny3d's other settings for the view, before the RDP's (the
	fog's colour, the sky, the level's first material): each change between
	tiny3d's commands and rdpq's has the RSP swap the one's code and state
	for the other's */
	t3d_light_set_ambient(white);
	t3d_light_set_count(0);
	apply_fog();
	draw_sky(view);
	rdpq_set_env_color(RGBA32(0xFF, 0xFF, 0xFF, 0xFF));
	render_phase(PHASE_LEVEL);
	draw_level(viewport, view);
	{
		/* the game's decals (bullet holes, blood, scorch marks) and paint,
		on the level before the objects: n64_paint.c draws them a little
		toward the eye, and the objects in front of them then cover them
		(after the objects, the pull showed them over a player against the
		wall) */
		struct n64_particle_view marks;

		particle_view(&marks, 0);
		n64_paint_draw(&marks);
	}
	render_phase(PHASE_OBJECTS);
	renderer.first_person = 0;
	renderer.viewport = viewport;
	renderer.current_texture = -2;
}

/* models: textured, shaded by their baked sky light, one RDP cycle. Halo's
two-sided shaders (tools/n64/models.py) are drawn without back-face
culling. A player's colour is the upper half of a texture's palette times
that colour (Halo tints where the multipurpose map says): a palette of its
own, made for the draw, in a ring that lasts the frames in flight. */
#define PALETTES_PER_FRAME 256

static unsigned long current_color;
static uint16_t palettes[FRAMES_IN_FLIGHT][PALETTES_PER_FRAME][16] __attribute__((aligned(16)));

static uint16_t *tinted_palette(sprite_t *sprite, unsigned long color)
{
	uint16_t *source = sprite_get_palette(sprite), *palette;
	unsigned r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = color & 0xFF;

	if (!source || palette_count >= PALETTES_PER_FRAME)
		return NULL;
	palette = palettes[renderer.frame_index % FRAMES_IN_FLIGHT][palette_count++];
	for (int i = 0; i < 16; i++)
	{
		uint16_t c = source[i];

		if (i >= 8)
		{
			unsigned cr = n64_div255(((c >> 11) & 31) * r), cg = n64_div255(((c >> 6) & 31) * g);
			unsigned cb = n64_div255(((c >> 1) & 31) * b);

			c = (uint16_t)((cr << 11) | (cg << 6) | (cb << 1) | (c & 1));
		}
		palette[i] = c;
	}
	data_cache_hit_writeback(palette, sizeof(uint16_t) * 16);
	return palette;
}

/* a batch's render mode and texture, unless they are the ones in force; a
player's colour on a MATERIAL_CHANGE_COLOR texture (color 0xRRGGBB; white
leaves it as it is) */
static int current_model_mode = -1;
static int current_model_opacity = -1;

static void use_setup(struct loaded_model *loaded, int b, struct model_batch *batch, unsigned long color)
{
	/* (the texture and the flags: model batches use flags up to MATERIAL_BLEND) */
	int key = (batch->texture << 12) | (batch->flags & 0xFFF);
	sprite_t *sprite = debug_untextured || batch->texture < 0 ? NULL : models.sprites[batch->texture];
	unsigned char opacity = (unsigned char)(color >> 24);
	int mode = model_mode(batch->texture >= 0 ? sprite : NULL, batch->flags, opacity);
	color &= 0xFFFFFF;

	if (!(batch->flags & MATERIAL_CHANGE_COLOR))
		color = 0xFFFFFF;
	if (renderer.current_texture == -2)
		current_model_mode = -1;
	if (mode != current_model_mode)
	{
		rspq_block_run(model_modes[mode]);
		current_model_mode = mode;
		current_model_opacity = -1;
	}
	if (mode >= MODEL_CLOAK_FIRST)
	{
		int alpha = batch->flags & MATERIAL_BLEND ? (opacity * GLOW_ALPHA + 127) / 255 : opacity;
		if (alpha != current_model_opacity)
		{
			rdpq_set_prim_color(RGBA32(255, 255, 255, alpha));
			current_model_opacity = alpha;
		}
	}
	if (key != renderer.current_texture || (color == 0xFFFFFF && current_color != 0xFFFFFF))
	{
		rspq_block_run(loaded->setups[b]);
		renderer.current_texture = key;
		current_color = 0xFFFFFF;
	}
	if (color != current_color && sprite)
	{
		uint16_t *palette = tinted_palette(sprite, color);

		if (palette)
		{
			rdpq_tex_upload_tlut(palette, 0, 16);
			/* (upload_sprite) */
			rdpq_sync_load();
		}
		current_color = color;
	}
}

/* the current view's particles: the world's (with the world's projection,
after its objects) or the first-person ones (after the weapon) */
static void draw_particles(int first_person)
{
	struct n64_particle_view view;

	if (!renderer.view_count)
		return;
	particle_view(&view, first_person);
	n64_particles_draw(&view);
	/* (their mode and TMEM -- the whole of it, the palettes' half too --
	are the models' no longer) */
	current_model_mode = -1;
	renderer.current_texture = -2;
	current_color = 0xFFFFFFFF;
}

void n64_render_view_particles(void)
{
	flush_draws();
	if (!renderer.world_particles_drawn)
	{
		draw_particles(0);
		renderer.world_particles_drawn = 1;
	}
	else if (renderer.first_person)
		draw_particles(1);
}

void n64_render_first_person_begin(void)
{
	T3DViewport *viewport = renderer.viewport;

	if (!viewport)
		return;
	/* the world's objects first, with the world's projection, and its
	particles */
	flush_draws();
	if (!renderer.world_particles_drawn)
	{
		draw_particles(0);
		renderer.world_particles_drawn = 1;
	}
	/* the game's first-person near and far planes (rasterizer.c), and the
	weapon in front of whatever the world put in this view's depth */
	t3d_viewport_set_perspective(viewport, renderer.fov, renderer.aspect, 0.01171875f * WORLD_SCALE, 16.0f * WORLD_SCALE);
	t3d_viewport_attach(viewport);
	apply_fog();
	/* (the view's own rectangle: the viewport's scissor) */
	clear_depth();
	renderer.current_texture = -2;
	renderer.first_person = 1;
	render_phase(PHASE_FIRST_PERSON);
}

static void render_object(const struct n64_object *object);

/* the node matrices converted this frame, by object */
#define CONVERTED_SLOTS 256
static struct converted_matrices
{
	unsigned long frame, key;
	T3DMat4FP *matrices;
} converted[CONVERTED_SLOTS];

/* (render_object's culling passed: the object is in the view) */
static int object_in_view;

int n64_render_object(const struct n64_object *object)
{
	object_in_view = 0;
	render_object(object);
	return object_in_view;
}

/* (the order render_object reads the node matrices in: NULL, their own) */
static const short *object_node_remap;

/* (the first-person models: their node matrices read through the model's
remapping table from the animation graph's, where the game copied them
into the model's order first, 52 bytes a node, a memcpy each, every frame,
for the renderer to read once) */
int n64_render_object_remapped(const struct n64_object *object, const short *node_remap)
{
	int in_view;

	object_node_remap = node_remap;
	in_view = n64_render_object(object);
	object_node_remap = NULL;
	return in_view;
}

static void render_phase(int phase)
{
	unsigned long now;

	gpu_time_sync();
	now = TICKS_READ();
	switch (renderer.phase)
	{
	case PHASE_LEVEL: renderer.level_ticks += now - renderer.phase_start; break;
	case PHASE_OBJECTS: renderer.object_ticks += now - renderer.phase_start; break;
	case PHASE_FIRST_PERSON: renderer.first_person_ticks += now - renderer.phase_start; break;
	case PHASE_HUD: renderer.hud_ticks += now - renderer.phase_start; break;
	}
	renderer.phase = phase;
	renderer.phase_start = now;
}

void n64_render_flush(void)
{
	flush_draws();
	stretch_view();
}

void n64_render_hud_phase(void)
{
	flush_draws();
	stretch_view();
	hud_display_phase();
	render_phase(PHASE_HUD);
}

/* A view's object batches are queued and drawn sorted by their render
mode, texture and colour (skies first): each change of mode or texture
runs rdpq's RSP code in place of tiny3d's and back, which cost more than
the batches themselves when every object changed it a few times. */
#define MAXIMUM_QUEUED_DRAWS 768

static struct queued_draw
{
	uint32_t order;
	uint32_t color;
	T3DMat4FP *matrices;
	uint16_t model, batch;
} queued_draws[MAXIMUM_QUEUED_DRAWS];
static uint16_t queued_order[MAXIMUM_QUEUED_DRAWS];
static int queued_count;

static void flush_draws(void);

/* the batches drawn from a run's first batch (global index): its run */
static inline int batch_run_length(int b)
{
	int length = models.runs ? models.runs[b] & RUN_LENGTH : 1;

	return length ? length : 1;
}

static void queue_draw(int model_index, int b, unsigned long color, T3DMat4FP *matrices)
{
	struct model_record *model = &models.models[model_index];
	struct model_batch *batch = &models.batches[model->first_batch + b];
	sprite_t *sprite = debug_untextured || batch->texture < 0 ? NULL : models.sprites[batch->texture];
	int mode = model_mode(batch->texture >= 0 ? sprite : NULL, batch->flags, (unsigned char)(color >> 24));
	struct queued_draw *draw;

	if (queued_count == MAXIMUM_QUEUED_DRAWS)
		flush_draws();
	draw = &queued_draws[queued_count];
	draw->order = ((uint32_t)!(batch->flags & MATERIAL_SKY) << 31) | ((uint32_t)mode << 26) |
		((uint32_t)(batch->texture & 0xFFFF) << 10) | (batch->flags & 0x3FF);
	/* Alpha lives in the previously unused byte: no larger resident queue. */
	draw->color = (color & 0xFF000000u) |
		(batch->flags & MATERIAL_CHANGE_COLOR ? color & 0xFFFFFF : 0xFFFFFF);
	draw->matrices = matrices;
	draw->model = (uint16_t)model_index;
	draw->batch = (uint16_t)b;
	queued_order[queued_count] = (uint16_t)queued_count;
	queued_count++;
}

/* x goes before y: by mode and texture, colour, then node matrices (an
object's batches together) */
static inline int draw_before(const struct queued_draw *x, const struct queued_draw *y)
{
	if (x->order != y->order)
		return x->order < y->order;
	if (x->color != y->color)
		return x->color < y->color;
	return x->matrices < y->matrices;
}

/* The queue in that order: runs of eight by insertion, then merged (newlib's
qsort called its comparison through a pointer and swapped bytes at a time:
several hundred draws a view, sorted twice a view). Stable: draws alike
keep the order they were queued in. */
static void sort_draws(void)
{
	static uint16_t scratch[MAXIMUM_QUEUED_DRAWS];
	uint16_t *from = queued_order, *to = scratch;
	int n = queued_count;

	for (int start = 0; start < n; start += 8)
	{
		int end = start + 8 < n ? start + 8 : n;

		for (int i = start + 1; i < end; i++)
		{
			uint16_t index = from[i];
			const struct queued_draw *draw = &queued_draws[index];
			int j = i;

			while (j > start && draw_before(draw, &queued_draws[from[j - 1]]))
			{
				from[j] = from[j - 1];
				j--;
			}
			from[j] = index;
		}
	}
	for (int width = 8; width < n; width *= 2)
	{
		for (int low = 0; low < n; low += 2 * width)
		{
			int middle = low + width < n ? low + width : n;
			int high = low + 2 * width < n ? low + 2 * width : n;
			int a = low, b = middle, k = low;

			while (a < middle && b < high)
				to[k++] = draw_before(&queued_draws[from[b]], &queued_draws[from[a]]) ? from[b++] : from[a++];
			while (a < middle)
				to[k++] = from[a++];
			while (b < high)
				to[k++] = from[b++];
		}
		uint16_t *swap = from;

		from = to;
		to = swap;
	}
	if (from != queued_order)
		memcpy(queued_order, from, n * sizeof(queued_order[0]));
}

static void flush_draws(void)
{
	T3DMat4FP *current_matrices = NULL;

	if (!queued_count)
		return;
	sort_draws();
	for (int i = 0; i < queued_count; i++)
	{
		struct queued_draw *draw = &queued_draws[queued_order[i]];
		struct model_record *model = &models.models[draw->model];
		struct loaded_model *loaded = &models.loaded[draw->model];
		struct model_batch *batch = &models.batches[model->first_batch + draw->batch];

		if (!loaded->blocks || !loaded->blocks[draw->batch])
			continue;
		/* rdpq's part first (the mode, the texture), then tiny3d's (the
		node matrices' segment, which only the batch's block reads, and the
		block): the first draw after rdpq's commands -- the paint's, the
		first person's depth reset -- no longer goes to tiny3d's code and
		back before them */
		use_setup(loaded, draw->batch, batch, draw->color);
		if (!current_matrices)
		{
			/* the blocks' place on tiny3d's matrix stack, one past the
			camera's (load_model); the camera's back in force after them */
			t3d_matrix_push_pos(1);
		}
		if (draw->matrices != current_matrices)
		{
			t3d_segment_set(SEGMENT_NODE_MATRICES, draw->matrices);
			current_matrices = draw->matrices;
		}
		rspq_block_run(loaded->blocks[draw->batch]);
		renderer.triangles += batch->triangle_count;
	}
	if (current_matrices)
		t3d_matrix_pop(1);
	queued_count = 0;
}

static void render_object(const struct n64_object *object)
{
	struct model_record *model;
	T3DMat4FP *matrices;
	T3DVec3 center;
	int node_count;
	int far;
	/* (debug flags looked up once: asked for each object in each view) */
	static int no_objects = -1, first_person_far = -1;

	if (no_objects < 0)
	{
		no_objects = n64_debug_flag("debug_no_objects");
		/* (measuring, "debug_fp_far": the first-person models' far versions
		with three or four views) */
		first_person_far = n64_debug_flag("debug_fp_far");
	}
	if (no_objects)
		return;
	if (!models.data || !renderer.viewport || object->model_tag_index < 0 ||
		object->model_tag_index >= models.tag_count || !(model = models.by_tag[object->model_tag_index]))
		return;
	far = 0;
	if (renderer.first_person && first_person_far && renderer.view_total > 2)
		far = model->far_count != 0;
	if (!renderer.first_person)
	{
		/* smaller than a pixel and a half across: not drawn; smaller than
		FAR_PIXELS: its far version, if it has one -- its size in pixels,
		2 r ppu / distance, compared squared (no square root or division,
		for each object in each view) */
		float dx = object->center[0] - renderer.eye[0], dy = object->center[1] - renderer.eye[1];
		float dz = object->center[2] - renderer.eye[2];
		float distance_squared = dx * dx + dy * dy + dz * dz;
		float radius = object->radius;

		/* wholly past the fog's end (before the frustum: the cheaper test) */
		if (renderer.fog && distance_squared > (renderer.fog_end + radius) * (renderer.fog_end + radius))
			return;
		if (distance_squared > radius * radius)
		{
			/* (a quarter of the screen: the far versions sooner) */
			float size = radius * 2.0f * renderer.pixels_per_unit * (renderer.view_total > 2 ? 0.6f : 1.f);
			float far_pixels = FAR_PIXELS * (screen_height * (1.f / 240.f));

			size *= size;
			if (size < 1.5f * 1.5f * distance_squared)
				return;
			far = model->far_count && size < far_pixels * far_pixels * distance_squared;
		}
	}
	center = (T3DVec3){{object->center[0] * WORLD_SCALE, object->center[1] * WORLD_SCALE, object->center[2] * WORLD_SCALE}};
	if (!t3d_frustum_vs_sphere(&renderer.viewport->viewFrustum, &center, object->radius * WORLD_SCALE + 8.0f))
		return;
	object_in_view = 1;
	node_count = object->node_count < model->node_count ? object->node_count : model->node_count;
	if (node_count <= 0 || renderer.matrix_count + model->node_count > MATRICES_PER_FRAME)
		return;
	if (renderer.first_person)
		models.loaded[model - models.models].first_person_drawn = renderer.frame_index;
	if (!load_model(model - models.models))
		return;
	/* an object seen in an earlier view of this frame has its matrices */
	matrices = NULL;
	if (object->key)
	{
		for (int probe = 0; probe < 8; probe++)
		{
			struct converted_matrices *entry = &converted[(object->key * 2654435761u + probe) % CONVERTED_SLOTS];

			if (entry->frame == renderer.frame_index && entry->key == object->key)
			{
				matrices = entry->matrices;
				break;
			}
		}
	}
	if (matrices)
		goto converted_already;
	matrices = renderer.matrices[renderer.frame_index % FRAMES_IN_FLIGHT] + renderer.matrix_count;
	renderer.matrix_count += model->node_count;
	if (object->key)
	{
		for (int probe = 0; probe < 8; probe++)
		{
			struct converted_matrices *entry = &converted[(object->key * 2654435761u + probe) % CONVERTED_SLOTS];

			if (entry->frame != renderer.frame_index)
			{
				entry->frame = renderer.frame_index;
				entry->key = object->key;
				entry->matrices = matrices;
				break;
			}
		}
	}
	for (int n = 0; n < node_count; n++)
	{
		const float *m = object->node_matrices + (object_node_remap ? object_node_remap[n] : n) * 13;
		/* The matrix in tiny3d's 16.16 fixed point: t3d_mat4_to_fixed_3x4
		multiplied each number by 65536 before truncating it; here that is in
		the scales already (the node's times WORLD_SCALE / MODEL_SCALE, the
		position's WORLD_SCALE). A power of two scales a float exactly and
		rounds with it, so each product is the same number as before, 12
		multiplications a node fewer (and its copy through the stack and the
		call): where a product had been too small for a float's exponent its
		fixed point is 0 either way */
		float axes = m[0] * (WORLD_SCALE / MODEL_SCALE * 65536.0f);
		float rows[4][3] = {
			{m[1] * axes, m[2] * axes, m[3] * axes},
			{m[4] * axes, m[5] * axes, m[6] * axes},
			{m[7] * axes, m[8] * axes, m[9] * axes},
			{m[10] * (WORLD_SCALE * 65536.0f), m[11] * (WORLD_SCALE * 65536.0f), m[12] * (WORLD_SCALE * 65536.0f)},
		};

		if (!renderer.first_person && n < 64 && object->head_node >= 0 && object->head_node < node_count &&
			(object->head_mask[n >> 5] & (1UL << (n & 31))))
		{
			/* Render-only 1.8x head hierarchy, about its animated pivot. Head
			and helmet vertices share the transform; gameplay matrices stay intact. */
			const float *head = object->node_matrices + object->head_node * 13;
			for (int axis = 0; axis < 3; axis++)
			{
				for (int k = 0; k < 3; k++) rows[axis][k] *= 1.8f;
				rows[3][axis] = (head[10 + axis] + (m[10 + axis] - head[10 + axis]) * 1.8f) * (WORLD_SCALE * 65536.0f);
			}
		}
		/* (as t3d_mat4_to_fixed_3x4 packs them: the integer halves, then the
		fractions, a row at a time; the last column 0, 0, 0, 1 -- written as
		words that may alias the matrix's halves, which matrices[0]'s copy
		below reads) */
		for (int y = 0; y < 4; y++)
		{
			typedef uint64_t __attribute__((may_alias)) fixed_words;
			uint32_t fixed0 = (uint32_t)(int32_t)rows[y][0];
			uint32_t fixed1 = (uint32_t)(int32_t)rows[y][1];
			uint32_t fixed2 = (uint32_t)(int32_t)rows[y][2];
			uint64_t integers = (fixed0 & 0xFFFF0000) | (fixed1 >> 16);
			uint64_t fractions = (fixed0 << 16) | (fixed1 & 0xFFFF);

			integers <<= 32;
			integers |= (fixed2 & 0xFFFF0000) | (y == 3);
			fractions <<= 32;
			fractions |= fixed2 << 16;
			*(fixed_words *)matrices[n].m[y].i = integers;
			*(fixed_words *)matrices[n].m[y].f = fractions;
		}
	}
	/* nodes the game does not have (a model with more nodes than its
	animation drives) keep the first node's matrix */
	for (int n = node_count; n < model->node_count; n++)
		matrices[n] = matrices[0];
converted_already:
	renderer.objects++;
	unsigned long color = ((unsigned long)object->tint[3] << 24) | ((unsigned long)object->tint[0] << 16) |
		((unsigned long)object->tint[1] << 8) | object->tint[2];

	/* (a run of batches alike drawn as one: its first's block holds them
	all, mark_batch_runs) */
	if (far)
	{
		for (int b = model->far_first; b < model->far_first + model->far_count;
			b += batch_run_length(model->first_batch + b))
		{
			queue_draw(model - models.models, b, color, matrices);
		}
		return;
	}
	for (int r = 0; r < model->region_count; r++)
	{
		struct region_record *region = &models.regions[model->first_region + r];
		int permutation = r < object->region_count ? object->region_permutations[r] : 0;
		int geometry_index;
		struct geometry_record *geometry;

		if (!region->permutation_count)
			continue;
		if (permutation >= region->permutation_count)
			permutation = 0;
		geometry_index = models.permutations[region->first_permutation + permutation];
		if (geometry_index < 0)
			continue;
		geometry = &models.geometries[geometry_index];
		for (int b = geometry->first_batch; b < geometry->first_batch + geometry->batch_count; b += batch_run_length(b))
			queue_draw(model - models.models, b - model->first_batch, color, matrices);
	}
}

/* without the game's HUD (debug_no_hud), a crosshair in the middle of
each view; always the line between the views */
static void draw_crosshairs(void)
{
	rdpq_sync_pipe();
	rdpq_set_scissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	rdpq_set_mode_fill(RGBA32(0x60, 0xD0, 0xFF, 0xFF));
	for (int i = 0; i < renderer.view_count && n64_debug_flag("debug_no_hud"); i++)
	{
		int x0 = renderer.rects[i][0], y0 = renderer.rects[i][1], x1 = renderer.rects[i][2], y1 = renderer.rects[i][3];
		int x, y;

		n64_render_hud_rect(&x0, &y0, &x1, &y1);
		x = (x0 + x1) / 2;
		y = (y0 + y1) / 2;

		rdpq_fill_rectangle(x - 6, y, x - 2, y + 1);
		rdpq_fill_rectangle(x + 2, y, x + 6, y + 1);
		rdpq_fill_rectangle(x, y - 5, x + 1, y - 1);
		rdpq_fill_rectangle(x, y + 2, x + 1, y + 6);
	}
	if (renderer.view_count == 2)
	{
		/* the line between the two views: two of the 3D's lines, in the
		display's pixels (n64_render_hud_rect) */
		int x0 = 0, y0 = renderer.rects[1][1] - 1, x1 = 0, y1 = renderer.rects[1][1] + 1;

		n64_render_hud_rect(&x0, &y0, &x1, &y1);
		rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));
		rdpq_fill_rectangle(0, y0, SCREEN_WIDTH, y1);
	}
}

static void draw_counter(void)
{
	unsigned long long now = get_ticks();

	if (!renderer.fps_window_start)
		renderer.fps_window_start = now;
	renderer.fps_frames++;
	if (now - renderer.fps_window_start >= TICKS_PER_SECOND)
	{
		renderer.fps = (int)((unsigned long long)renderer.fps_frames * TICKS_PER_SECOND * 10 / (now - renderer.fps_window_start));
		renderer.fps_frames = 0;
		renderer.fps_window_start = now;
	}
	if (notice_high_detail_frames)
	{
		/* (high or ultra detail asked for, but the map left no room for it) */
		notice_high_detail_frames--;
		rdpq_sync_pipe();
		rdpq_set_scissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 24, SCREEN_HEIGHT / 2,
			"NOT ENOUGH MEMORY FOR %s DETAIL ON THIS MAP", want_high_detail == N64_DETAIL_ULTRA ? "ULTRA" : "HIGH");
	}
	if (renderer.hide_counter)
		return;
	rdpq_sync_pipe();
	rdpq_set_scissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, 12, "%d.%d FPS tick %lu.%lu draw %lu.%lu wait %lu.%lu",
		renderer.fps / 10, renderer.fps % 10, n64_last_tick_us / 1000, (n64_last_tick_us % 1000) / 100,
		render_cpu_us / 1000, (render_cpu_us % 1000) / 100, render_wait_us / 1000, (render_wait_us % 1000) / 100);
	/* the 3D's resolution (narrower with medium and chunky: the screen's
	after it), 480 lines interlaced */
	if (hud3d.buffer)
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, 22, "3D %dx%d, HUD %dx%d",
			chunky_width(hud3d.width), hud3d.height, SCREEN_WIDTH, SCREEN_HEIGHT);
	else if (chunky_width(SCREEN_WIDTH) != SCREEN_WIDTH)
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, 22, "3D %dx%d ON %dx%d", chunky_width(SCREEN_WIDTH),
			SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
	else
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, 22, "3D %dx%d%s", SCREEN_WIDTH, SCREEN_HEIGHT,
			SCREEN_HEIGHT > 240 ? " INTERLACED" : "");
	int line = 32;
	char text[80];

	/* a streamed level: its chunks in memory (of its slots), those in view
	but not in memory yet, the heap, the models' cache and the models that
	found no room (drawn as nothing) */
	if (level.stream)
	{
		int used = 0;

		for (int i = 0; i < level.slot_count; i++)
			used += level.slot_chunk[i] >= 0;
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, line, "LEVEL %d/%d WAITING %d HEAP %lu KB", used,
			level.slot_count, level.stream_waited, n64_heap_free() / 1024);
		line += 10;
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, line, "MODELS %lu KB MISSING %d",
			(models.loaded_bytes + models.texture_bytes) / 1024, models.missing_last);
		line += 10;
	}
	/* the terrain's tiles: big ones drawn (of the slots), near batches
	still waiting for theirs */
	if (tiles.data)
	{
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, line, "GROUND TILES NEAR %d/%d WAITING %d",
			tiles.big_drawn_last, tiles.slot_count, tiles.waiting_last);
		line += 10;
	}
	/* the game's (a campaign's objects: n64_campaign.c) */
	if (n64_game_debug_line(text, sizeof(text)))
	{
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, line, "%s", text);
		line += 10;
	}
	if (n64_game_debug_line_2(text, sizeof(text)))
		rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 8, line, "%s", text);
}

void n64_profile_poll(void);

void n64_render_frame_end(void)
{
	n64_sound_poll();
	/* (measuring, "debug_fog_test=5": each frame's counts) */
	if (debug_fog_test == 5)
	{
		static unsigned long last_batches, last_objects, last_triangles;

		debugf("frame %lu: %lu batches, %lu objects, %lu triangles\n", renderer.frame_index,
			renderer.batches - last_batches, renderer.objects - last_objects, renderer.triangles - last_triangles);
		last_batches = renderer.batches;
		last_objects = renderer.objects;
		last_triangles = renderer.triangles;
	}
	flush_draws();
	stretch_view();
	hud_display_phase();
	render_phase(PHASE_NONE);
	n64_profile_poll();
	draw_crosshairs();
	draw_counter();
	debug_rdpq_mark("frame end");
	/* (the display shown: with the HUD at 640x240, not hud3d, attached) */
	if (hud3d.buffer)
		rdpq_detach_cb((void (*)(void *))display_show, hud_display);
	else
		rdpq_detach_show();
	n64_render_wait_sync_full();
	render_cpu_us = TICKS_TO_US(TICKS_READ() - render_start_ticks);
	renderer.draw_ticks += TICKS_READ() - render_start_ticks;
	if (!(renderer.frame_index % 120))
	{
		unsigned long n64_stack_used(void);
		heap_stats_t heap;
		unsigned long long now = get_ticks();

		/* (the statistics, with "debug_stats": formatting them, the heap's
		walk and the stack's scan cost the frame they were printed in) */
		int stats = n64_debug_flag("debug_stats");

		if (renderer.window_start && stats)
		{
			debugf("frames: %lu.%lu fps; per frame: draw %llu us (level %llu, objects %llu, first person %llu, hud %llu), tick %lu us\n",
				(unsigned long)(120ULL * TICKS_PER_SECOND * 10 / (now - renderer.window_start) / 10),
				(unsigned long)(120ULL * TICKS_PER_SECOND * 10 / (now - renderer.window_start) % 10),
				TICKS_TO_US(renderer.draw_ticks) / 120, TICKS_TO_US(renderer.level_ticks) / 120,
				TICKS_TO_US(renderer.object_ticks) / 120, TICKS_TO_US(renderer.first_person_ticks) / 120,
				TICKS_TO_US(renderer.hud_ticks) / 120, n64_last_tick_us);
		}
		renderer.window_start = now;
		renderer.draw_ticks = renderer.level_ticks = renderer.object_ticks = renderer.first_person_ticks = 0;
		renderer.hud_ticks = 0;
		if (stats)
		{
			extern unsigned long n64_exceptions_emulated;
			static unsigned long exceptions_before;

			sys_get_heap_stats(&heap);
			debugf("render: frame %lu, per frame %lu clusters, %lu batches, %lu objects, %lu triangles; heap %d KB free, stack %lu KB used; "
				"models %lu KB (%lu reads), textures %lu KB (%lu reads); %lu exceptions emulated\n",
				renderer.frame_index, renderer.clusters / 120, renderer.batches / 120, renderer.objects / 120,
				renderer.triangles / 120, heap.free / 1024, n64_stack_used() / 1024,
				models.loaded_bytes / 1024, models.reads, models.texture_bytes / 1024, models.texture_reads,
				n64_exceptions_emulated - exceptions_before);
			if (level.stream)
				debugf("render: level clusters %lu KB in memory, %lu read\n", level.resident_bytes / 1024,
					level.stream_reads);
			exceptions_before = n64_exceptions_emulated;
		}
		if (geometry_profile_enabled > 0)
		{
			debugf("geometry/120frames: requests %lu hit %lu miss %lu wrap %lu bytes %lu; dma %llu us convert/writeback %llu us wrap-wait %llu us\n",
				geometry_profile.requests, geometry_profile.hits, geometry_profile.misses,
				geometry_profile.wraps, geometry_profile.bytes,
				TICKS_TO_US(geometry_profile.dma_ticks), TICKS_TO_US(geometry_profile.convert_ticks),
				TICKS_TO_US(geometry_profile.wait_ticks));
			memset(&geometry_profile, 0, sizeof(geometry_profile));
		}
		renderer.clusters = renderer.batches = renderer.triangles = renderer.objects = 0;
	}
}
