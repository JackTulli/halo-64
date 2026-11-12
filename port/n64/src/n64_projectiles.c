/*
N64_PROJECTILES.C

The projectiles' tracers, trails and glows: a module, rom:/projectiles.dso
(tools/n64_build.py), loaded by the program (n64_particles.c) only while the
setting is LIGHT or FULL and the heap has its room, and given back first when
anything else needs memory. Its code, its looks and its segments are memory only
while it is in; the program keeps the setting, the per-tick hook
(source/items/projectiles.c) and the dots, with their own drawing.

A bullet or a bolt has no model, and Halo draws it with what it attaches: a
contrail (the tracer) and a light volume (the glowing head of a plasma bolt,
a needle, a rocket's exhaust). The N64 makes neither, so nothing showed
where fire went. Each tick's move of a projectile with such a look comes
here (the entry segment): one segment a projectile a frame, the moves of the
frame's ticks joined, drawn in the world pass with the RDP's fill
rectangles (no triangles): a tracer's trail, its move since the last frame
as Halo's tracer contrail (PROJECTILE_TRACER_FRAMES), thin; a FULL bolt
trail kept for 0.125 s (bolt_life) and fading as it ages; a light volume's
glow, in the frame after the tick that made it, wide and its length behind
the projectile. Visual only: no game state, no random numbers.
*/

#include <libdragon.h>
#include <math.h>
#include <string.h>

#include "n64.h"
#include "n64_projectiles_module.h"

/* (compiled for size, -Os, as a module; the per-bar work for speed) */
#define HOT __attribute__((optimize("O2")))

#define PROJECTILE_SEGMENTS 64
#define PROJECTILE_SLOTS 64
/* a trail's life, in frames. A tracer's: Halo's, one frame. Each point of
Halo's tracer contrail is drawn in one frame (the assault rifle's tracer
contrail tag goes from yellow to black in 0.010-0.0125 s, under a frame,
and its additive blend draws no black; contrails.c,
contrail_update_points), so its tracer on the screen is the round's move
since the last frame: solid over its newer half (the frame's newer
points) and fading to nothing over its older half. (4 keeps each move for
4 frames, fading with age: a round's whole path at its end; a
compile-time option.) A FULL bolt trail's: as long as Halo's, 0.125 s
(the stock tag weapons\plasma rifle\plasma's contrail: its points fade
from white to nothing in 0.125 s; the contrail tags are not on the
cartridge), in frames of the measured frame time: 4 frames at 30 a
second, 2 at 15, 1 at 10 and below (projectile_bolt_life), at most
PROJECTILE_BOLT_FRAMES */
#define PROJECTILE_TRACER_FRAMES 1
#define PROJECTILE_BOLT_FRAMES 4
#define PROJECTILE_BOLT_SECONDS 0.125f
/* The ring's order: segments are made at block->next, one after another,
with frames 0, and every frame ages them all alike (at most 15; an unused
segment is 15). So from the newest (next - 1) to the oldest, frames never
falls: past a segment older than a life, every one is; past one at 15,
every one is 15. The draw and the ageing stop there, and a trail's search
for its older moves stops past the frame it looks for. */
#define PROJECTILE_LIFE_MOST (PROJECTILE_BOLT_FRAMES > PROJECTILE_TRACER_FRAMES ? \
	PROJECTILE_BOLT_FRAMES : PROJECTILE_TRACER_FRAMES)
/* a trail's bars on the screen: as many for a whole fade (fewer for less:
one a frame of age), at most as many for its cost */
#define PROJECTILE_FADE_BARS 4
#define PROJECTILE_LINE_BARS 48
/* the tracers' and trails' bars a frame, all views (each view its share):
past it a trail's oldest bars are dropped, its head kept -- a tracer keeps
at least PROJECTILE_HEAD_BARS of its newest, a bolt's trail none (its glow
stays). Far above a fight's (bar case 4 players ~25 a frame, plasma FULL
~50): it caps a storm's cost, ~7 ms of bars at LIGHT and ~14 at FULL
without it */
#define PROJECTILE_BAR_BUDGET 256
#define PROJECTILE_HEAD_BARS 4

struct projectile_segment
{
	float start[3], end[3];
	/* RGB; the top byte its parts (bit 0 the trail, bit 1 the glow) and
	the glow's length in 1/32 world units (bits 2-7) */
	uint32_t color;
	/* the projectile's absolute object index + 1 (bits 0-12); a solid
	glow's owner, its local player + 1 (bits 13-15; 0: none) */
	uint16_t key;
	/* the glow's radius, 1/256 world units */
	uint8_t radius;
	/* frames since the tick that made it (a glow drawn at 1, a trail's
	piece from 1 to its life; at most 15), and the glow's opacity, the light
	volume's far alpha in 1/20 (at most 14; 15: solid, alpha 1) */
	uint8_t frames : 4, opacity : 4;
};

/* the segments: the module's own memory, given back with it */
static struct projectile_block
{
	struct projectile_segment segments[PROJECTILE_SEGMENTS];
	/* a projectile's segment of this frame + 1, by its key */
	uint8_t slot[PROJECTILE_SLOTS];
	uint8_t next;
} block;
/* a FULL bolt trail's life in frames, from the frame time smoothed over
about 8 frames (n64_particles_update's): it moves by one only where 0.125 s
in frames is 0.75 past it, so it does not flicker between two */
static uint8_t bolt_life;
/* the map's mode: 1 LIGHT, 2 FULL */
int n64_projectiles_map_mode;
typedef char projectile_segment_size_check[sizeof(struct projectile_segment) == 32 ? 1 : -1];
/* (a trail's older moves are marked in 64 bits, one a segment) */
typedef char projectile_segments_check[PROJECTILE_SEGMENTS <= 64 ? 1 : -1];
/* the segments new, after the module's load (an unused segment is 15
frames old: never drawn) */
static void begin(int mode, float frame_time)
{
	n64_projectiles_map_mode = mode;
	n64_projectiles_looks_clear();
	memset(block.segments, 0xFF, sizeof(block.segments));
	memset(block.slot, 0, sizeof(block.slot));
	block.next = 0;
	/* (the life nearest 0.125 s in frames) */
	for (bolt_life = 1; bolt_life < PROJECTILE_BOLT_FRAMES && PROJECTILE_BOLT_SECONDS / frame_time > bolt_life + 0.5f;)
		bolt_life++;
}

/* the slots a key may use, from its own: PROJECTILE_PROBES (keys 64
apart, objects' indices 64 apart, share a slot) */
#define PROJECTILE_PROBES 4

void n64_projectiles_segment_record(const float *from, const float *to, unsigned long color, int radius, int opacity, int key)
{
	struct projectile_block *block_ = &block;
	struct projectile_segment *segment = NULL;
	int index, free_slot = -1;

	/* (a later tick's move in the same frame joins its segment -- the
	trail's piece from where it was at the last frame, the glow where it is
	now -- only where the move starts where the segment ends: an object's
	index is used again at once, in the next tick of the same frame, by
	another projectile, which starts a segment of its own) */
	for (int j = 0; j < PROJECTILE_PROBES; j++)
	{
		int slot = (key + j) & (PROJECTILE_SLOTS - 1);
		struct projectile_segment *at;

		index = block_->slot[slot] - 1;
		at = &block_->segments[index < 0 ? 0 : index];
		if (index >= 0 && !at->frames && at->key == (uint16_t)key)
		{
			float gap = 0.f;

			for (int k = 0; k < 3; k++)
				gap += (from[k] - at->end[k]) * (from[k] - at->end[k]);
			if (gap <= 0.0001f)
			{
				segment = at;
				break;
			}
		}
		/* (a slot to take: free, or its segment from an earlier frame) */
		if (free_slot < 0 && (index < 0 || at->frames))
			free_slot = slot;
	}
	if (!segment)
	{
		index = block_->next;
		block_->next = (uint8_t)((index + 1) % PROJECTILE_SEGMENTS);
		block_->slot[free_slot >= 0 ? free_slot : key & (PROJECTILE_SLOTS - 1)] = (uint8_t)(index + 1);
		segment = &block_->segments[index];
		memcpy(segment->start, from, sizeof(segment->start));
		segment->key = (uint16_t)key;
		segment->frames = 0;
	}
	memcpy(segment->end, to, sizeof(segment->end));
	segment->color = (uint32_t)color;
	segment->radius = (uint8_t)radius;
	segment->opacity = (uint8_t)(opacity < 0 ? 0 : opacity > 15 ? 15 : opacity);
}

/* a frame of dt seconds: the trails' life from the smoothed frame time, the
segments a frame older */
static void update(float dt, float frame_time)
{
	if (!(isfinite(dt) && dt > 0.f))
		return;
	float frames = PROJECTILE_BOLT_SECONDS / frame_time;

	if (frames > bolt_life + 0.75f && bolt_life < PROJECTILE_BOLT_FRAMES)
		bolt_life++;
	else if (frames < bolt_life - 0.75f && bolt_life > 1)
		bolt_life--;
	/* (newest first, to the first at 15: the ring's order, below) */
	for (int k = 1; k <= PROJECTILE_SEGMENTS; k++)
	{
		struct projectile_segment *segment =
			&block.segments[(block.next + PROJECTILE_SEGMENTS - k) % PROJECTILE_SEGMENTS];

		if (segment->frames == 15)
			break;
		segment->frames++;
	}
}

/* ---------- the drawing (the program's dots draw their own) */

static struct { float depth_a, depth_b, fog_start, fog_per; } projectile_view;

/* a point on the view's screen, its distance and its inverse; 0 behind the
near plane */
HOT static int projectile_point(const struct n64_particle_view *view, const float point[3], float out[2],
	float *z, float *per_z)
{
	float d[3], s;

	for (int k = 0; k < 3; k++)
		d[k] = point[k] - view->eye[k];
	*z = d[0] * view->forward[0] + d[1] * view->forward[1] + d[2] * view->forward[2];
	if (!(*z > view->z_near * 2.f))
		return 0;
	*per_z = 1.f / *z;
	s = view->pixels_per_unit * *per_z;
	out[0] = view->center_x + (d[0] * view->right[0] + d[1] * view->right[1] + d[2] * view->right[2]) * view->x_scale * s;
	out[1] = view->center_y - (d[0] * view->up[0] + d[1] * view->up[1] + d[2] * view->up[2]) * s;
	return isfinite(out[0] + out[1]);
}

/* the prim colour last set for the projectiles, its alpha's top 5 bits
(projectile_begin: none), and the prim depth last set (draw: none) */
static uint32_t projectile_prim;
static int projectile_prim_set;
static uint32_t projectile_z;

/* the RDP's colour set: rgb with alpha a (0-255), prim its key (not HOT:
libdragon's inline colour packing, color_to_packed32, inlines only into a
function of the file's own optimisation, not into the O2 ones) */
static __attribute__((noinline)) void projectile_color_set(uint32_t rgb, int a, uint32_t prim)
{
	rdpq_set_prim_color(RGBA32(rgb >> 16, (rgb >> 8) & 0xFF, rgb & 0xFF, a));
	projectile_prim = prim;
	projectile_prim_set = 1;
}

/* the RDP's colour rgb with alpha a (0-255), set again only where it changes */
HOT static inline void projectile_color(uint32_t rgb, int a)
{
	/* (the RDP's blender reads an alpha's top 5 bits: alphas that share
	them draw alike, so the colour is set again only where they change) */
	uint32_t prim = (rgb & 0xFFFFFF) << 5 | ((uint32_t)a >> 3 & 0x1F);

	if (!projectile_prim_set || prim != projectile_prim)
		projectile_color_set(rgb, a, prim);
}

/* the RDP's prim depth, set again only where it changes */
HOT static void projectile_set_depth(uint16_t z)
{
	if (z != projectile_z)
	{
		rdpq_set_prim_depth_raw(z, 0);
		projectile_z = z;
	}
}

/* a rectangle from x0, y0 to x1, y1 in drawn pixels */
HOT static void projectile_fill(float x0, float y0, float x1, float y1, uint32_t rgb, float alpha)
{
	projectile_color(rgb, (int)(alpha * 255.f));
	rdpq_fill_rectangle(x0, y0, x1, y1);
}

/* a trail's bars laid out (trail), sent to the RDP a chunk at a time: their
values first, then the commands, so no bar's values are kept across a call */
#define PROJECTILE_CHUNK 32
struct projectile_bar { float x0, y0, x1, y1; uint16_t z; int16_t a; };

HOT static __attribute__((noinline)) void projectile_send(const struct projectile_bar *bars, int count, uint32_t rgb)
{
	for (int i = 0; i < count; i++)
	{
		projectile_set_depth(bars[i].z);
		projectile_color(rgb, bars[i].a);
		rdpq_fill_rectangle(bars[i].x0, bars[i].y0, bars[i].x1, bars[i].y1);
	}
}

/* a rectangle from x0, y0 to x1, y1 on the view's screen, if on it (in
CHUNKY and MEDIUM narrower where drawn); one copy (a glow's squares) */
HOT static __attribute__((noinline)) void projectile_bar(const struct n64_particle_view *view, float x0, float y0, float x1, float y1, uint32_t rgb,
	float alpha)
{
	if (x1 < view->x0 || x0 >= view->x1 || y1 < view->y0 || y0 >= view->y1)
		return;
	if (view->chunky)
	{
		x0 = view->x0 + (x0 - view->x0) * view->chunky_scale;
		x1 = view->x0 + (x1 - view->x0) * view->chunky_scale;
		/* (at least a drawn pixel wide: the RDP draws no narrower
		rectangle -- a small glow's bright middle) */
		if (x1 - x0 < 1.f)
		{
			float middle = (x0 + x1) * 0.5f;

			x0 = middle - 0.5f;
			x1 = middle + 0.5f;
		}
	}
	projectile_fill(x0, y0, x1, y1, rgb, alpha);
}

/* a square of half-size half pixels at x, y on the view's screen, if on it */
HOT static void projectile_square(const struct n64_particle_view *view, float x, float y, float half, uint32_t rgb,
	float alpha)
{
	projectile_bar(view, x - half, y - half, x + half, y + half, rgb, alpha);
}

/* the mode, set where it is the first thing drawn */
HOT static void projectile_begin(int *mode)
{
	if (*mode)
		return;
	projectile_prim_set = 0;
	rdpq_sync_pipe();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_zbuf(true, false);
	rdpq_mode_zoverride(true, 0.f, 0);
	rdpq_mode_end();
	*mode = 1;
}

/* a distance's fog (1 none, 0 all), and the prim depth from its inverse */
HOT static float projectile_fog(float z)
{
	return projectile_view.fog_per > 0.f && z > projectile_view.fog_start ?
		1.f - (z - projectile_view.fog_start) * projectile_view.fog_per : 1.f;
}

HOT static float projectile_depth(float per_z)
{
	float depth = projectile_view.depth_a - projectile_view.depth_b * per_z;

	return depth >= 1.f ? 1.f : depth <= 0.f ? 0.f : depth;
}

/* a point's place in the view and its fog, the mode set where it is the
first one drawn, and its depth; 0: not drawn (behind, past the fog, off the
view) */
HOT static int projectile_place(const struct n64_particle_view *view, const float point[3], float head[2], float *alpha,
	float *scale, int *mode)
{
	float z, per_z;

	if (!projectile_point(view, point, head, &z, &per_z) || (view->fog_end > 0.f && z >= view->fog_end) ||
		head[0] < view->x0 - 28.f || head[0] > view->x1 + 28.f || head[1] < view->y0 - 28.f || head[1] > view->y1 + 28.f)
		return 0;
	*alpha = projectile_fog(z);
	*scale = view->pixels_per_unit * per_z;
	projectile_begin(mode);
	projectile_set_depth((uint16_t)(projectile_depth(per_z) * 0x7FFF));
	return 1;
}

/* a row of squares to the head from the point units behind it along the
segment, at most cap pixels: count squares of size pixels (two more where
grow, to the head), each alpha from a fifth to whole at the head */
HOT static void row(const struct n64_particle_view *view, struct projectile_segment const *segment,
	const float head[2], float units, float cap, int count, float size, int grow, uint32_t rgb, float alpha)
{
	float d[3], length2 = 0.f, tail[2], z, per_z, keep;

	for (int k = 0; k < 3; k++)
	{
		d[k] = segment->end[k] - segment->start[k];
		length2 += d[k] * d[k];
	}
	keep = length2 > units * units ? units / sqrtf(length2) : 1.f;
	for (int k = 0; k < 3; k++)
		d[k] = segment->end[k] - d[k] * keep;
	if (!projectile_point(view, d, tail, &z, &per_z))
		tail[0] = head[0], tail[1] = head[1];
	float lx = head[0] - tail[0], ly = head[1] - tail[1], length = sqrtf(lx * lx + ly * ly);
	if (length > cap)
	{
		lx *= cap / length;
		ly *= cap / length;
	}
	if (count < 1)
		count = 1;
	for (int n = 1; n <= count; n++)
	{
		float t = (float)n / (float)count;

		projectile_square(view, head[0] - lx * (1.f - t), head[1] - ly * (1.f - t),
			(size + (grow ? 2.f * t : 0.f)) * 0.5f, rgb, alpha * (grow ? 0.2f + 0.8f * t * t : 0.5f + 0.5f * t));
	}
}

/* a trail: the line from start (alpha a) to end (alpha b, reached at knee
of the way and kept to the end), cut to the view's depth (in front of the near plane, short of the fog's end), as bars
along its longer axis on the screen, each at most a pixel off the line
across it (more past PROJECTILE_LINE_BARS bars a part: a line over 48
pixels across steps), at least one a frame of its fade (PROJECTILE_FADE_BARS
for a whole fade): a solid line in few of the RDP's rectangles where it is
near level or upright. Each bar has its own alpha, fog and depth (the
depth is linear along the line on the screen). At most *bars bars, the
newest (the oldest dropped first: the head is kept); *bars then the bars
laid out. 0: not drawn */
HOT static int trail(const struct n64_particle_view *view, const float start[3], const float end[3],
	float alpha_a, float alpha_b, float knee, float width, uint32_t rgb, int *mode, int *bars)
{
	int most = *bars;

	*bars = 0;
	float p[2][3], z[2], s[2][2], range[2] = { 0.f, 1.f }, near = view->z_near * 2.f * 1.01f, far = view->fog_end > 0.f ? view->fog_end : view->z_far;

	for (int i = 0; i < 2; i++)
	{
		const float *q = i ? end : start;

		z[i] = 0.f;
		for (int k = 0; k < 3; k++)
		{
			p[i][k] = q[k];
			z[i] += (q[k] - view->eye[k]) * view->forward[k];
		}
	}
	if (!isfinite(z[0] + z[1]) || (z[0] < near && z[1] < near) || (z[0] >= far && z[1] >= far))
		return 0;
	/* (cut where it leaves the view's depth) */
	for (int i = 0; i < 2; i++)
	{
		float bound = z[i] < near ? near : z[i] >= far ? far * 0.999f : z[i];

		if (bound != z[i])
		{
			float t = (bound - z[1 - i]) / (z[i] - z[1 - i]);

			for (int k = 0; k < 3; k++)
				p[i][k] = p[1 - i][k] + (p[i][k] - p[1 - i][k]) * t;
			range[i] = range[1 - i] + (range[i] - range[1 - i]) * t;
			z[i] = bound;
		}
	}
	float per_z[2];

	if (!projectile_point(view, p[0], s[0], &z[0], &per_z[0]) || !projectile_point(view, p[1], s[1], &z[1], &per_z[1]))
		return 0;
	if ((s[0][0] < view->x0 && s[1][0] < view->x0) || (s[0][0] >= view->x1 && s[1][0] >= view->x1) ||
		(s[0][1] < view->y0 && s[1][1] < view->y0) || (s[0][1] >= view->y1 && s[1][1] >= view->y1))
		return 0;
	/* (CHUNKY and MEDIUM draw the view narrower: the line's ends in drawn
	pixels, so its bars are laid out, and kept at least a pixel wide, in
	the pixels drawn -- the RDP draws no rectangle under a pixel wide) */
	float xs = view->chunky ? view->chunky_scale : 1.f, right = view->x0 + (view->x1 - view->x0) * xs;

	if (view->chunky)
		for (int i = 0; i < 2; i++)
			s[i][0] = view->x0 + (s[i][0] - view->x0) * xs;
	float dx = s[1][0] - s[0][0], dy = s[1][1] - s[0][1], along = fabsf(dx), across = fabsf(dy);
	int level = along >= across;
	float fog[2] = { projectile_fog(z[0]), projectile_fog(z[1]) };
	float depth[2] = { projectile_depth(per_z[0]), projectile_depth(per_z[1]) };
	float size[2] = { n64_min_f(n64_max_f(width * view->pixels_per_unit * per_z[0], 1.f), 2.f),
		n64_min_f(n64_max_f(width * view->pixels_per_unit * per_z[1], 1.f), 2.f) };
	int count;

	if (!level)
	{
		float swap = along;

		along = across;
		across = swap;
	}
	/* (two parts on the screen: the fade, to where the alpha reaches b, and
	the solid rest; each part's bars at most a pixel off the line across it
	and at least a pixel long (the RDP draws no narrower rectangle), the
	fade's at least PROJECTILE_FADE_BARS for a whole fade, the solid part's
	one where it is level or upright) */
	float split = range[1] > range[0] ? (knee - range[0]) / (range[1] - range[0]) : 1.f, per_knee = 1.f / knee;
	float part[3] = { 0.f, split < 0.f ? 0.f : split > 1.f ? 1.f : split, 1.f };
	int counts[2], total;

	for (int k = 0; k < 2; k++)
	{
		float share = part[k + 1] - part[k];
		float a0 = range[0] + (range[1] - range[0]) * part[k], a1 = range[0] + (range[1] - range[0]) * part[k + 1];
		float fade = k ? 0.f : fabsf(alpha_b - alpha_a) * ((a1 < knee ? a1 : knee) - (a0 < knee ? a0 : knee)) * per_knee;

		/* (the fade's bars, rounded up: an integer ceiling, no libm call) */
		float need = fade * PROJECTILE_FADE_BARS - 0.001f;
		int needed = (int)need + ((float)(int)need < need);

		count = share > 0.f ? (int)(across * share) + 1 : 0;
		if (count && count < needed)
			count = needed;
		if (count > (int)(along * share))
			count = (int)(along * share);
		if (count > PROJECTILE_LINE_BARS)
			count = PROJECTILE_LINE_BARS;
		if (share > 0.f && count < 1)
			count = 1;
		counts[k] = count;
	}
	total = counts[0] + counts[1];
	/* (past the budget: the first bars, the line's oldest, left out) */
	int skip = total > most ? total - most : 0;

	*bars = total - skip;
	projectile_begin(mode);
	/* (part by part, each bar's values from the line's, with no division a
	bar; the line's two ends a half width longer, each on its own side,
	along its longer axis) */
	float span = range[1] - range[0], dalpha = alpha_b - alpha_a, dfog = fog[1] - fog[0];
	float dsize = size[1] - size[0], ddepth = depth[1] - depth[0];
	int n = 0, queued = 0;
	struct projectile_bar chunk[PROJECTILE_CHUNK];

	for (int k = 0; k < 2; k++)
	{
		float step = counts[k] ? (part[k + 1] - part[k]) / (float)counts[k] : 0.f;

		for (int m = 0; m < counts[k]; m++, n++)
		{
			float t0 = part[k] + step * (float)m;
			float t1 = part[k] + step * (float)(m + 1), t = (t0 + t1) * 0.5f;
			float along_line = (range[0] + span * t) * per_knee;
			float alpha = (alpha_a + dalpha * (along_line < 1.f ? along_line : 1.f)) * (fog[0] + dfog * t);
			float half = (size[0] + dsize * t) * 0.5f, half_x = half * xs;
			float x0, x1, y0, y1;

			if (n < skip || alpha < 1.f / 32.f)
				continue;
			/* (along the longer axis the bars meet end to end: none drawn
			over another, so a faded line keeps one alpha) */
			if (level)
			{
				float a = s[0][0] + dx * t0, b = s[0][0] + dx * t1, c = s[0][1] + dy * t;

				if (dx >= 0.f)
				{
					x0 = n == 0 ? a - half_x : a;
					x1 = n == total - 1 ? b + half_x : b;
				}
				else
				{
					x0 = n == total - 1 ? b - half_x : b;
					x1 = n == 0 ? a + half_x : a;
				}
				y0 = c - half;
				y1 = c + half;
			}
			else
			{
				float a = s[0][1] + dy * t0, b = s[0][1] + dy * t1, c = s[0][0] + dx * t;

				if (dy >= 0.f)
				{
					y0 = n == 0 ? a - half : a;
					y1 = n == total - 1 ? b + half : b;
				}
				else
				{
					y0 = n == total - 1 ? b - half : b;
					y1 = n == 0 ? a + half : a;
				}
				if (half_x < 0.5f)
					half_x = 0.5f;
				x0 = c - half_x;
				x1 = c + half_x;
			}
			/* (a bar off the view sets nothing) */
			if (x1 < view->x0 || x0 >= right || y1 < view->y0 || y0 >= view->y1)
				continue;
			chunk[queued].x0 = x0;
			chunk[queued].y0 = y0;
			chunk[queued].x1 = x1;
			chunk[queued].y1 = y1;
			chunk[queued].z = (uint16_t)((depth[0] + ddepth * t) * 0x7FFF);
			chunk[queued].a = (int16_t)(int)(alpha * 255.f);
			if (++queued == PROJECTILE_CHUNK)
			{
				projectile_send(chunk, queued, rgb);
				queued = 0;
			}
		}
	}
	projectile_send(chunk, queued, rgb);
	return 1;
}

/* a view's segments, newest first, at most the view's share of the
setting's limit (a trail counts as one); none past the fog */
HOT static void draw(const struct n64_particle_view *view, int views, int *mode)
{
	int limit = (n64_projectiles_map_mode == 2 ? 32 : 16) / views, drawn = 0;
	int budget = PROJECTILE_BAR_BUDGET / views;
	float head[2], alpha, scale;
	/* the segments drawn as a trail's older moves, by slot */
	uint64_t used = 0;
	/* (1 / a life of 1 to 4 frames) */
	static const float per_life[PROJECTILE_LIFE_MOST + 1] = { 0.f, 1.f, 0.5f, 1.f / 3.f, 0.25f };

	/* (this view's constants; the RDP's colour set again at the first bar,
	whatever the program's dots set) */
	projectile_view.depth_a = (view->z_far + view->z_near) / (view->z_far - view->z_near) * 0.5f + 0.5f;
	projectile_view.depth_b = view->z_far * view->z_near / (view->z_far - view->z_near) / 0.995f;
	projectile_view.fog_start = view->fog_start;
	projectile_view.fog_per = view->fog_end > view->fog_start ? 1.f / (view->fog_end - view->fog_start) : 0.f;
	projectile_prim_set = 0;
	projectile_z = ~0UL;
	for (int k = 1; k <= PROJECTILE_SEGMENTS && drawn < limit; k++)
	{
		int at = (block.next + PROJECTILE_SEGMENTS - k) % PROJECTILE_SEGMENTS, shown = 0;
		struct projectile_segment const *segment = &block.segments[at];
		uint32_t parts = segment->color >> 24;
		int frames = segment->frames;

		/* (a glow alone is drawn in the frame after its tick) */
		int life = !(parts & 1) ? 1 : (parts & 2) ? bolt_life : PROJECTILE_TRACER_FRAMES;

		/* (the ring's order: every one after it older still) */
		if (frames > PROJECTILE_LIFE_MOST)
			break;
		if (frames < 1 || frames > life || ((used >> at) & 1))
			continue;
		/* the trail: a round's moves of its last life frames (a tracer's 1,
		a FULL bolt's 4), one line (a round flies straight) from the start of
		its oldest kept move to its head. A tracer's one frame: solid over
		its newer half, fading to nothing over its older half, as Halo's
		contrail. Kept moves fade with age (whole at the head, nothing life
		frames behind it): a round's older moves are the segments with its
		key, each a frame older, that end where the newer one starts (an
		object's index is used again). 1-2 pixels wide (Halo's tracer
		contrail is 0.05 world units wide); a tracer in the contrail's
		colour, a bolt's white, as Halo's plasma contrails (the contrail tags
		are not on the cartridge) */
		if (parts & 1)
		{
			struct projectile_segment const *tail = segment;

			for (int j = k + 1; life > 1 && j <= PROJECTILE_SEGMENTS; j++)
			{
				int older_at = (block.next + PROJECTILE_SEGMENTS - j) % PROJECTILE_SEGMENTS;
				struct projectile_segment const *older = &block.segments[older_at];
				float gap = 0.f;

				/* (the ring's order: none after it a frame older than the tail) */
				if (older->frames > tail->frames + 1 || older->frames > life)
					break;
				if (older->key != segment->key || older->frames != tail->frames + 1 ||
					!((older->color >> 24) & 1))
					continue;
				for (int c = 0; c < 3; c++)
					gap += (older->end[c] - tail->start[c]) * (older->end[c] - tail->start[c]);
				if (gap > 0.01f)
					continue;
				tail = older;
				used |= 1ULL << older_at;
			}
			/* (the view's bar budget: a tracer keeps its newest bars, a
			bolt's trail none past it; a bolt's trail of one frame, at a low
			frame rate, still fades from its tail to its head, not a
			tracer's two halves) */
			int tracer = !(parts & 2), bars = budget > 0 ? budget : tracer ? PROJECTILE_HEAD_BARS : 0;

			if (bars > 0 && tracer && life == 1)
				shown = trail(view, segment->start, segment->end, 0.f, 1.f, 0.5f, 0.05f,
					parts & 2 ? 0xFFFFFF : segment->color & 0xFFFFFF, mode, &bars);
			else if (bars > 0)
				shown = trail(view, tail->start, segment->end, 1.f - (float)tail->frames * per_life[life],
					1.f - (float)(frames - 1) * per_life[life], 1.f, 0.05f,
					parts & 2 ? 0xFFFFFF : segment->color & 0xFFFFFF, mode, &bars);
			else
				bars = 0;
			budget -= bars;
		}
		/* the glow, in the frame after its tick: two squares along the light
		volume's length, the one at the head wider (its size + 2 pixels),
		then its bright middle; its size three times the volume's far radius
		at its distance, at least 2 pixels; at most 6 for a see-through
		volume (far alpha under 1: the plasma rifle's, the Ghost's, the
		Banshee's and the plasma pistol's normal bolts, needles, rockets),
		whose glow at the player's own crosshair would hide the target; for a
		solid one (1: the charged plasma pistol shot, the fuel rod) 120
		pixels a world unit of its radius, 6-16, so a charged shot coming at
		the player reads as the big, solid ball it is in Halo. In its owner's
		own view a solid glow is drawn as a see-through one while its size
		would pass 6 pixels (a charged shot's, to about 15 world units in one
		view): the player's own shot does not hide the target. Its opacity
		from the far alpha (0.35: 0.6; from 0.6: whole) */
		if ((parts & 2) && frames == 1 && projectile_place(view, segment->end, head, &alpha, &scale, mode))
		{
			float radius = segment->radius * (1.f / 256.f), full = radius * 3.f * scale;
			int solid = segment->opacity == 15 && !(full > 6.f && (segment->key >> 13) == view->local_player + 1);
			uint32_t rgb = segment->color & 0xFFFFFF;

			float size = n64_min_f(n64_max_f(full, 2.f), solid ? n64_min_f(n64_max_f(radius * 120.f, 6.f), 16.f) : 6.f);

			row(view, segment, head, (parts >> 2) * (1.f / 32.f), 20.f, 2, size, 1, rgb,
				alpha * (solid ? 1.f : segment->opacity == 15 ? 0.6f : n64_min_f(segment->opacity * (0.6f / 7.f), 1.f)));
			projectile_square(view, head[0], head[1], size * 0.25f, rgb | 0x808080, alpha);
			shown = 1;
		}
		drawn += shown;
	}
}

const struct n64_projectiles_entries n64_projectiles_entries =
{
	begin,
	n64_projectiles_moved,
	update,
	draw,
};
