/*
N64_DYNAMIC.C

The geometry Halo builds each frame for a multiplayer game: King of the
Hill's band round the hill (game_engine_king.c) and the cloth of Capture the
Flag's flags (objects/widgets/flags.c). Halo fills its rasterizer's dynamic
buffers with it, which the N64 does not build (their stand-ins draw nothing:
n64_stubs_generated.c); the game hands it here instead as a grid of world
points, and it is drawn in the view after the objects and before the
particles, as Halo draws it (render.c: post_rasterize_objects after the
level's transparent geometry, before the particles).

The shaders' looks are the converter's (tools/n64/dynamic.py: fx/<map>.dyn),
read when a game that draws them starts, only the kind it draws (the hill's
in King of the Hill, the flags' in Capture the Flag), and given back with the
map (n64_game_free_map). A shader with no look here (the file left out, no
room for it) is drawn flat in the colour the game gives.

A grid is drawn by tiny3d, as the level is: its points, in the world's
place, packed once a frame into tiny3d's vertices (the frame's views load the
same ones under their cameras; tiny3d clips at the near plane), into a ring
of a few frames' room, made as the game starts (n64_dynamic_reserve) and
given back with the looks. rdpq's own triangles, cut on the CPU, cost tens
of microseconds each (the RSP's setup): a flag's cloth a view took
milliseconds.
*/

#include <libdragon.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <t3d/t3d.h>

#include "n64.h"

#define DYNAMIC_LOOKS 4
/* the frames a ring keeps (n64_render.c's frames in flight) */
#define DYNAMIC_FRAMES 3
/* the grids a frame draws at most (a band; two flags) */
#define DYNAMIC_GRIDS 4
/* tiny3d's vertex cache */
#define DYNAMIC_LOAD 70
/* world units to tiny3d's (n64_render.c WORLD_SCALE) */
#define DYNAMIC_SCALE 64.f
/* (a few grids a view: the file is compiled for size, as the modules are;
the program's code is memory every game has less of) */
#define SMALL __attribute__((optimize("Os")))
/* TMEM's 4 KB */
#define DYNAMIC_TEXELS_BYTES 4096
/* the band's strength over the shader's (n64_dynamic_grid) */
#define DYNAMIC_BAND_GAIN 2.0f

/* Halo's periodic functions (math/periodic_functions.c): the looks'
animations take their values from them, as the shader's do */
extern float periodic_function_evaluate(short function_type, float time);

static struct dynamic_look
{
	long shader_index;
	uint8_t kind, format, u_function, flicker_function;
	uint8_t color[4];
	float u_period, u_scale, flicker_period, flicker_lower;
	surface_t texels;
} looks[DYNAMIC_LOOKS];
static int look_count;

static uint16_t dynamic_be16(const uint8_t *p)
{
	return (uint16_t)(p[0] << 8 | p[1]);
}

static uint32_t dynamic_be32(const uint8_t *p)
{
	return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3];
}

static float dynamic_float(const uint8_t *p)
{
	uint32_t bits = dynamic_be32(p);
	float value;

	memcpy(&value, &bits, sizeof(value));
	return isfinite(value) ? value : 0.f;
}

static const struct dynamic_look *find_look(long shader_index)
{
	/* (a tag index: its datum's low half) */
	for (int i = 0; i < look_count; i++)
		if (looks[i].shader_index == (shader_index & 0xFFFF))
			return &looks[i];
	return NULL;
}

/* the frames' vertices: DYNAMIC_FRAMES frames of `frame_pairs` pairs (each
grid's written back from the data cache as it is made: the RSP reads them),
the frame the slot in use was made in, the vertices of it used, and the
frame's grids made so far (their keys, their first vertices) */
static struct
{
	T3DVertPacked *pairs;
	int frame_pairs;
	unsigned long frame;
	int used;
	struct
	{
		unsigned long key;
		int first;
	} grids[DYNAMIC_GRIDS];
	int grid_count;
	/* the boxes of the frame's grids (n64_dynamic_grid's view test: made
	by the frame's first view to test the grid, the other views use them) */
	struct
	{
		unsigned long key, frame;
		float low[3], high[3];
	} boxes[DYNAMIC_GRIDS];
	int box_next;
} ring;

__attribute__((cold)) void n64_dynamic_release(void)
{
	if (!look_count && !ring.pairs)
		return;
	/* (the RSP and the RDP done with the texels and the vertices first) */
	rspq_wait();
	for (int i = 0; i < look_count; i++)
		surface_free(&looks[i].texels);
	memset(looks, 0, sizeof(looks));
	look_count = 0;
	free(ring.pairs);
	memset(&ring, 0, sizeof(ring));
}

__attribute__((cold)) int n64_dynamic_reserve(int vertices)
{
	int pairs = (vertices + 1) / 2;

	if (ring.pairs && ring.frame_pairs >= pairs)
		return 1;
	if (ring.pairs)
	{
		rspq_wait();
		free(ring.pairs);
	}
	memset(&ring, 0, sizeof(ring));
	/* (whole data cache lines: a write-back touches nothing else) */
	ring.pairs = memalign(16, sizeof(T3DVertPacked) * DYNAMIC_FRAMES * pairs);
	if (!ring.pairs)
		return 0;
	ring.frame_pairs = pairs;
	ring.frame = ~0UL;
	return 1;
}

SMALL __attribute__((cold)) int n64_dynamic_load(const char *map_name, int kind)
{
	char path[48];
	uint8_t header[16], entry[32];
	FILE *file;
	int loaded = 0;

	if (!map_name || snprintf(path, sizeof(path), "rom:/fx/%s.dyn", map_name) >= (int)sizeof(path))
		return 0;
	file = fopen(path, "rb");
	if (!file)
		return 0;
	if (fread(header, 1, sizeof(header), file) == sizeof(header) && !memcmp(header, "N64D", 4) &&
		dynamic_be16(header + 4) == 1)
	{
		int count = dynamic_be16(header + 6);

		for (int i = 0; i < count && look_count < DYNAMIC_LOOKS; i++)
		{
			struct dynamic_look *look = &looks[look_count];
			int format, width, height;
			unsigned long bytes;

			/* (one of the kind, not already read: a game again on the map) */
			if (fseek(file, 16 + 32 * i, SEEK_SET) || fread(entry, 1, sizeof(entry), file) != sizeof(entry) ||
				entry[2] != kind || find_look((long)dynamic_be16(entry)))
				continue;
			format = entry[3];
			width = entry[4];
			height = entry[5];
			bytes = format ? (unsigned long)width * height * 2 : (unsigned long)width * height / 2;
			/* (I4 or RGBA16, a power of two across (the band's repeats), in
			TMEM whole) */
			if (format > 1 || width < 8 || height < 1 || (width & (width - 1)) || bytes > DYNAMIC_TEXELS_BYTES)
				continue;
			look->texels = surface_alloc(format ? FMT_RGBA16 : FMT_I4, width, height);
			if (!look->texels.buffer)
				break;
			if (look->texels.stride * height != bytes || fseek(file, (long)dynamic_be32(entry + 28), SEEK_SET) ||
				fread(look->texels.buffer, 1, bytes, file) != bytes)
			{
				surface_free(&look->texels);
				continue;
			}
			look->shader_index = (long)dynamic_be16(entry);
			look->kind = entry[2];
			look->format = (uint8_t)format;
			look->u_function = entry[6];
			look->flicker_function = entry[7];
			memcpy(look->color, entry + 8, 4);
			look->u_period = dynamic_float(entry + 12);
			look->u_scale = dynamic_float(entry + 16);
			look->flicker_period = dynamic_float(entry + 20);
			look->flicker_lower = dynamic_float(entry + 24);
			look_count++;
			loaded++;
		}
	}
	fclose(file);
	{
		char line[64];

		snprintf(line, sizeof(line), "n64: %d dynamic look(s) of kind %d loaded", loaded, kind);
		n64_log(line);
	}
	return loaded;
}

/* ---------- drawing */

/* a value rounded to the nearest whole number */
static inline int16_t dynamic_round(float x)
{
	return (int16_t)(x >= 0.f ? x + 0.5f : x - 0.5f);
}

/* one of a frame's vertices: a world point at (s, t) texels (tiny3d's 10.5) */
SMALL static void pack(int index, const float *p, float s, float t)
{
	T3DVertPacked *pair = &ring.pairs[index >> 1];
	int16_t *position = index & 1 ? pair->posB : pair->posA;
	int16_t *st = index & 1 ? pair->stB : pair->stA;

	for (int k = 0; k < 3; k++)
		position[k] = dynamic_round(p[k] * DYNAMIC_SCALE);
	st[0] = dynamic_round(s * 32.f);
	st[1] = dynamic_round(t * 32.f);
	if (index & 1)
		pair->rgbaB = 0xFFFFFFFF, pair->normB = 0;
	else
		pair->rgbaA = 0xFFFFFFFF, pair->normA = 0;
}

/* each cell's triangles by its tesselation (flags.c: both, none, top left,
bottom left, top right, bottom right), corners 0 (x, y), 1 (x + 1, y),
2 (x, y + 1), 3 (x + 1, y + 1); 0xFF: no more */
static const uint8_t cell_triangles[6][6] = {
	{0, 1, 2, 2, 1, 3},
	{0xFF},
	{0, 2, 1, 0xFF},
	{0, 2, 3, 0xFF},
	{0, 3, 1, 0xFF},
	{2, 3, 1, 0xFF},
};

/* the RDP's mode for a look (NULL: flat), its texture, and its colours: the
fog's and the fade's; a cloth (with cells) or the band */
SMALL static __attribute__((noinline)) void set_mode(const struct dynamic_look *look, int cloth, const uint8_t color[4],
	float fade, unsigned long fog_rgba)
{
	rdpq_sync_pipe();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	if (cloth)
	{
		/* a flag's cloth: its texture, cut out where its alpha is clear and
		written to the depth (a cloth folds over itself), toward the fog's
		colour as it is far (the environment colour, by the primitive's
		alpha); with no look, solid in its team's colour, written to the
		depth too */
		if (look && look->format)
		{
			rdpq_mode_combiner(RDPQ_COMBINER1((ENV, TEX0, PRIM_ALPHA, TEX0), (0, 0, 0, TEX0)));
			rdpq_mode_alphacompare(128);
		}
		else
			rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
		rdpq_mode_zbuf(true, true);
	}
	else
	{
		/* the band: its hue, the texel its alpha (Halo adds the shader's
		colour to the frame: the RDP's additive blend wraps, so the hue is
		blended in as much as the texel is bright); faded in the fog */
		if (look)
			rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
		else
			rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
		rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
		rdpq_mode_zbuf(true, false);
	}
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_mode_end();
	if (look && (!cloth || look->format))
	{
		/* (the band's texture repeats along it; a flag's is clamped) */
		rdpq_texparms_t parms = {.s.repeats = look->format ? 1 : REPEAT_INFINITE};

		rdpq_tex_upload(TILE0, &look->texels, &parms);
	}
	if (cloth && look && look->format)
	{
		rdpq_set_env_color(color_from_packed32((uint32_t)fog_rgba));
		rdpq_set_prim_color(RGBA32(0xFF, 0xFF, 0xFF, (int)((1.f - fade) * 255.f)));
	}
	else
	{
		rdpq_set_prim_color(RGBA32(color[0], color[1], color[2], (int)(color[3] * fade)));
	}
}

/* a box (least and greatest x, y and z) all past one of the view's sides or
before its near plane: no part of it can be seen in the view. Each side is a
plane through the eye: the screen's x0..x1 and y0..y1 at depth z, as x / z and
y / z are drawn (16 pixels to spare round the view: tiny3d's projection and
this one need not agree to the pixel), so a point behind the eye is tested
too. The box is past a plane when its corner farthest in along the plane's
normal is (its centre's distance plus its half sizes along the normal) */
static int box_outside(const struct n64_particle_view *view, const float *low, const float *high)
{
	float x = view->x_scale * view->pixels_per_unit, y = view->pixels_per_unit;
	float sides[4] = {view->x0 - view->center_x - 16.f, view->x1 - view->center_x + 16.f,
		view->center_y - view->y0 + 16.f, view->center_y - view->y1 - 16.f};
	float centre[3], half[3];

	for (int k = 0; k < 3; k++)
	{
		centre[k] = (low[k] + high[k]) * 0.5f - view->eye[k];
		half[k] = (high[k] - low[k]) * 0.5f;
	}
	for (int plane = 0; plane < 5; plane++)
	{
		float normal[3], distance = 0.f, reach = 0.f;

		for (int k = 0; k < 3; k++)
		{
			/* (inside: x >= left z, x <= right z, y <= top z, y >= bottom z,
			z >= near) */
			normal[k] = plane == 0 ? view->right[k] * x - sides[0] * view->forward[k] :
				plane == 1 ? sides[1] * view->forward[k] - view->right[k] * x :
				plane == 2 ? sides[2] * view->forward[k] - view->up[k] * y :
				plane == 3 ? view->up[k] * y - sides[3] * view->forward[k] : view->forward[k];
			distance += normal[k] * centre[k];
			reach += fabsf(normal[k]) * half[k];
		}
		if (plane == 4)
			distance -= view->z_near;
		if (distance + reach < 0.f)
			return 1;
	}
	return 0;
}

int n64_dynamic_in_view(const float *low, const float *high)
{
	struct n64_particle_view view;
	unsigned long fog_rgba, frame;

	return n64_render_dynamic_view(&view, &fog_rgba, &frame) && !box_outside(&view, low, high);
}

SMALL void n64_dynamic_grid(unsigned long key, long shader_index, const float *points, int stride, int columns, int rows,
	const float *u, const float *v, const short *cells, float seconds, unsigned long rgba)
{
	struct n64_particle_view view;
	const struct dynamic_look *look;
	unsigned long fog_rgba, frame;
	float u_offset = 0.f, width = 1.f, height = 1.f, nearest = 1e30f, fade = 1.f;
	uint8_t color[4];
	/* a repeating look's cells each have their corners of their own (their s
	near 0: tiny3d's and the RDP's texture coordinates are small numbers);
	else the grid's columns share them, each padded to a pair */
	int repeating, column_vertices, first = -1;

	if (columns < 2 || rows < 2 || !points || !u || !v || !ring.pairs)
		return;
	if (!n64_render_dynamic_view(&view, &fog_rgba, &frame))
		return;
	{
		/* the view's test, first: a grid whose box (its points' least and
		greatest x, y and z) no part of the view can show costs the view
		nothing more; one in the view even in part is drawn whole (its
		triangles lie within its points, its points within the box). The box
		is made by the frame's first view to test the grid, and every point
		is checked in tiny3d's reach then */
		const uint8_t *point = (const uint8_t *)points;
		float *low = NULL, *high = NULL;

		for (int b = 0; b < DYNAMIC_GRIDS; b++)
			if (ring.boxes[b].key == key && ring.boxes[b].frame == frame)
			{
				low = ring.boxes[b].low;
				high = ring.boxes[b].high;
			}
		if (!low)
		{
			int b = ring.box_next++ % DYNAMIC_GRIDS;

			ring.boxes[b].key = key;
			ring.boxes[b].frame = ~0UL;
			low = ring.boxes[b].low;
			high = ring.boxes[b].high;
			for (int k = 0; k < 3; k++)
			{
				low[k] = 1e30f;
				high[k] = -1e30f;
			}
			for (int i = 0; i < columns * rows; i++, point += stride)
			{
				const float *p = (const float *)point;

				/* (a NaN fails this too) */
				if (!(fabsf(p[0]) < 500.f && fabsf(p[1]) < 500.f && fabsf(p[2]) < 500.f))
					return;
				for (int k = 0; k < 3; k++)
				{
					low[k] = p[k] < low[k] ? p[k] : low[k];
					high[k] = p[k] > high[k] ? p[k] : high[k];
				}
			}
			ring.boxes[b].frame = frame;
		}
		if (box_outside(&view, low, high))
			return;
	}
	look = find_look(shader_index);
	repeating = look && !look->format;
	column_vertices = repeating ? 2 * rows : rows + (rows & 1);
	/* (a load holds a repeating look's cell column, or two of a grid's
	columns) */
	if ((repeating ? 1 : 2) * column_vertices > DYNAMIC_LOAD ||
		(columns - repeating) * column_vertices > 2 * ring.frame_pairs)
		return;
	color[0] = (uint8_t)(rgba >> 24);
	color[1] = (uint8_t)(rgba >> 16);
	color[2] = (uint8_t)(rgba >> 8);
	color[3] = (uint8_t)rgba;
	if (look)
	{
		memcpy(color, look->color, 4);
		width = (float)look->texels.width;
		height = (float)look->texels.height;
		/* the scroll along the texture's width (its first map's u
		animation), and the flicker of its brightness (its last stage's
		constant colour) */
		if (look->u_function && look->u_period > 0.f)
			u_offset = look->u_scale * periodic_function_evaluate(look->u_function, seconds / look->u_period);
		if (look->flicker_function && look->flicker_period > 0.f)
		{
			float wave = periodic_function_evaluate(look->flicker_function, seconds / look->flicker_period);

			color[3] = (uint8_t)(color[3] * (look->flicker_lower + (1.f - look->flicker_lower) * wave));
		}
		/* (the band as Halo draws it reads faintly at 320x240: twice as strong,
		capped at full, so its flicker's dim phase is 74% of its peak, not the
		original's 52%; its hue kept) */
		if (!look->format)
			color[3] = (uint8_t)fminf(255.f, color[3] * DYNAMIC_BAND_GAIN);
	}
	{
		/* the grid's nearest point, for the fog (as the sniper's trail's:
		what is past the fog's end is not drawn) */
		const uint8_t *point = (const uint8_t *)points;

		for (int i = 0; i < columns * rows; i++, point += stride)
		{
			const float *p = (const float *)point;
			float z = (p[0] - view.eye[0]) * view.forward[0] + (p[1] - view.eye[1]) * view.forward[1] +
				(p[2] - view.eye[2]) * view.forward[2];

			if (z < nearest)
				nearest = z;
		}
		if (view.fog_end > view.fog_start)
		{
			if (nearest >= view.fog_end)
				return;
			if (nearest > view.fog_start)
				fade = 1.f - (nearest - view.fog_start) / (view.fog_end - view.fog_start);
		}
	}
	/* the frame's vertices: made by the frame's first view to draw the grid */
	if (frame != ring.frame)
	{
		ring.frame = frame;
		ring.used = 0;
		ring.grid_count = 0;
	}
	for (int g = 0; g < ring.grid_count; g++)
		if (ring.grids[g].key == key)
			first = ring.grids[g].first;
	if (first < 0)
	{
		int base_index = (int)(frame % DYNAMIC_FRAMES) * ring.frame_pairs * 2;
		int needed = (columns - repeating) * column_vertices;

		if (ring.grid_count == DYNAMIC_GRIDS || ring.used + needed > 2 * ring.frame_pairs)
			return;
		first = base_index + ring.used;
		ring.grids[ring.grid_count].key = key;
		ring.grids[ring.grid_count++].first = first;
		ring.used += needed;
		for (int c = 0; c < columns - repeating; c++)
		{
			const uint8_t *column = (const uint8_t *)points + (size_t)stride * rows * c;
			int index = first + c * column_vertices;

			if (repeating)
			{
				/* (a cell's s, moved by whole repeats near 0) */
				float s0 = (u[c] + u_offset) * width, s1 = (u[c + 1] + u_offset) * width;
				float base = floorf(fminf(s0, s1) / width) * width;

				for (int r = 0; r < rows; r++)
				{
					pack(index + r, (const float *)(column + (size_t)stride * r), s0 - base, v[r] * height);
					pack(index + rows + r, (const float *)(column + (size_t)stride * (rows + r)), s1 - base,
						v[r] * height);
				}
			}
			else
			{
				for (int r = 0; r < rows; r++)
					pack(index + r, (const float *)(column + (size_t)stride * r), u[c] * width, v[r] * height);
				if (rows & 1)
					pack(index + rows, (const float *)(column + (size_t)stride * (rows - 1)), u[c] * width,
						v[rows - 1] * height);
			}
		}
		data_cache_hit_writeback(&ring.pairs[first >> 1], (unsigned long)needed / 2 * sizeof(T3DVertPacked));
	}
	n64_render_dynamic_begin();
	/* (a cloth passes its cells, the band none) */
	set_mode(look, cells != NULL, color, fade, fog_rgba);
	t3d_state_set_drawflags(T3D_FLAG_DEPTH | (look ? T3D_FLAG_TEXTURED : 0));
	{
		/* the cells, a load of tiny3d's cache at a time: a repeating look's
		cell columns each whole, a grid's columns with the last of a load
		loaded again first in the next */
		int step = repeating ? DYNAMIC_LOAD / column_vertices : DYNAMIC_LOAD / column_vertices - 1;
		int cell_columns = columns - 1;

		for (int c0 = 0; c0 < cell_columns; c0 += step)
		{
			int count = cell_columns - c0 < step ? cell_columns - c0 : step;
			int loaded = repeating ? count * column_vertices : (count + 1) * column_vertices;

			t3d_vert_load(&ring.pairs[(first + c0 * column_vertices) >> 1], 0, loaded);
			for (int c = 0; c < count; c++)
			{
				/* (a cell's corners in the cache: its column's, then the next
				column's) */
				int left = c * column_vertices, right = repeating ? left + rows : left + column_vertices;

				for (int r = 0; r < rows - 1; r++)
				{
					int code = cells ? cells[(c0 + c) * (rows - 1) + r] : 0;
					const uint8_t *list = code >= 0 && code < 6 ? cell_triangles[code] : cell_triangles[1];
					int corner[4] = {left + r, right + r, left + r + 1, right + r + 1};

					for (int k = 0; k < 6 && list[k] != 0xFF; k += 3)
						t3d_tri_draw(corner[list[k]], corner[list[k + 1]], corner[list[k + 2]]);
				}
			}
			t3d_tri_sync();
		}
	}
}
