/*
N64_PROJECTILES_FULL.C

PROJECTILES FULL's long contrails: a module of its own, rom:/projectiles_full.dso
(tools/n64_build.py), loaded by the program (n64_particles.c) with the
projectiles' module only while the setting is FULL and the heap has its room
besides the model cache's margin, and given back with it. So its code and its
pieces are memory only at FULL; at LIGHT and HEAVY it takes none. The
projectiles' module (n64_projectiles.c, n64_projectiles_game.c) hands it each
move of a projectile with a long trail and calls its update and its drawing.

The contrail tags are not on the cartridge, so their values are here, from the
stock tags: a rocket's smoke (weapons\rocket launcher\exhaust, alpha blended:
orange at the motor, then grey, widening from 0.15 to 0.5 world units as it
fades, gone in 1.4-3.4 s), the charged plasma shot's trail (weapons\plasma
rifle\plasma charged bolt, added: 0.5 world units wide at the shot, 0.2 after
0.125 s, gone in 1.1-1.3 s) and a needle's (weapons\needler\needler, added:
0.04 wide, gone in 0.5-0.65 s). All three fade where they are seen along their
length (the tags' "fade when perpendicular", which Halo takes at each point):
a shooter's own trail is seen broadside near the gun and end-on at the shot, so
its near part shows and it is not drawn as a blob at the shot. A projectile's
moves join one piece for its style's span, then start the next (a homing shot's
path bends); each piece is a ribbon with the sniper's mask, its width and colour
from its style's keys at its age, its fade and fog at each vertex. 64 pieces: a
needler's burst (~10 needles' trails alive, 3-4 pieces each) and two rockets'
smoke (~10 each); at most 32 drawn a frame, all views together, newest first.
Visual only: no game state, no random numbers.
*/

#include <libdragon.h>
#include <math.h>
#include <string.h>

#include "n64.h"
#include "n64_projectiles_module.h"

/* (the game's, source/tag_files/tag_files.h; a datum index's absolute index,
source/cseries/cseries.h) */
char *tag_get_name(long tag_index);
#define DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index) ((datum_index) & 0xFFFF)

#define HOT __attribute__((optimize("O2")))

/* (the sniper's clipping geometry, for a piece the quick one below does not
take: one end behind the near plane or past the fog, or far off the view) */
#define N64_RIBBON_HALF_MAX 8.f
#include "n64_sniper_ribbon.h"

#define PROJECTILE_PIECES 64
#define PROJECTILE_PIECES_DRAWN 32
/* (the slots a key may use, from its own) */
#define PIECE_PROBES 4
/* (pixels past the view's edges a quick piece may reach: the RDP's scissor
cuts it there) */
#define PIECE_GUARD 128.f

struct projectile_piece
{
	float start[3], end[3];
	/* seconds since the piece began */
	float age;
	/* the projectile's absolute object index + 1, its style (1-3; 0: none) */
	uint16_t key;
	uint8_t style, spare;
};
/* the pieces: this module's own memory, given back with it */
static struct
{
	struct projectile_piece piece[PROJECTILE_PIECES];
	/* a projectile's newest piece + 1, by its key */
	uint8_t slot[PROJECTILE_PIECES];
	uint8_t next;
} pieces;
/* a style: a piece's span (seconds), its life, and its keys -- from t
seconds, the half width (world units) and 0xRRGGBBAA, linear between */
struct piece_key { float t, half; uint32_t rgba; };
struct piece_style { float span, life; int keys; struct piece_key key[4]; };
static const struct piece_style piece_styles[3] =
{
	/* rocket smoke: orange (251, 188, 30), then grey (209, 209, 216) at
	0.63 (state 1's 0.75, its bitmap's 0.89 at most), (128, 128, 142) at 0.19,
	gone at 2.4 s */
	{ 0.25f, 2.4f, 4, { { 0.f, 0.075f, 0xFBBC1EE6 }, { 0.08f, 0.1f, 0xD1D1D8A0 }, { 1.2f, 0.16f, 0x80808E30 },
		{ 2.4f, 0.25f, 0x68686C00 } } },
	/* the charged shot's: its glow's green lifted a quarter (added in
	Halo), whole, 0.85 at 0.125 s, gone at 1.2 s */
	{ 0.1f, 1.2f, 3, { { 0.f, 0.25f, 0x8BFE7AFF }, { 0.125f, 0.1f, 0x8BFE7AD9 }, { 1.2f, 0.025f, 0x8BFE7A00 } } },
	/* a needle's: its bitmap's dim magenta (124, 2, 137) lifted halfway
	(added in Halo, at most 0.33 bright), faint, gone at 0.575 s */
	{ 0.15f, 0.575f, 2, { { 0.f, 0.02f, 0xBE81C45A }, { 0.575f, 0.004f, 0xBE81C400 } } },
};

/* a definition's style, by its tag's name, found once a load (1 a rocket's
smoke, 2 the charged plasma shot's, 3 a needle's, the multiplayer one too;
0 none): the definition's absolute index + 1 in bits 2-31, the style in 0-1,
16 places by the index */
static unsigned long styles[16];

static int style_of(long definition_index)
{
	unsigned long index = (unsigned long)DATUM_INDEX_TO_ABSOLUTE_INDEX(definition_index);
	unsigned long *at = &styles[index & 15];

	if ((*at >> 2) != index + 1)
	{
		char const *name = tag_get_name(definition_index);

		*at = (index + 1) << 2 | (!name ? 0 : !strcmp(name, "weapons\\rocket launcher\\rocket") ? 1 :
			!strcmp(name, "weapons\\plasma rifle\\charged bolt") ? 2 :
			!strcmp(name, "weapons\\needler\\needle") || !strcmp(name, "weapons\\needler\\mp_needle") ? 3 : 0);
	}
	return (int)(*at & 3);
}

/* a projectile's move this tick, in its style's long trail if it has one */
static void record(const float *from, const float *to, int key, long definition_index)
{
	int style = style_of(definition_index);

	if (!style)
		return;
	int index, take = -1;
	struct projectile_piece *piece;

	/* (PIECE_PROBES slots from its key's, as the projectiles' module's
	segments: keys 64 apart share a slot; a slot to take is free, or its
	piece past its style's span, which takes no more moves) */
	for (int j = 0; j < PIECE_PROBES; j++)
	{
		int slot = (key + j) & (PROJECTILE_PIECES - 1);

		index = pieces.slot[slot] - 1;
		piece = &pieces.piece[index < 0 ? 0 : index];
		if (index >= 0 && piece->key == (uint16_t)key && piece->style == style &&
			piece->age < piece_styles[style - 1].span && !memcmp(from, piece->end, sizeof(piece->end)))
		{
			memcpy(piece->end, to, sizeof(piece->end));
			return;
		}
		if (take < 0 && (index < 0 || !piece->style || piece->age >= piece_styles[piece->style - 1].span))
			take = slot;
	}
	index = pieces.next;
	pieces.next = (uint8_t)((index + 1) & (PROJECTILE_PIECES - 1));
	piece = &pieces.piece[index];
	memcpy(piece->start, from, sizeof(piece->start));
	memcpy(piece->end, to, sizeof(piece->end));
	piece->age = 0.f;
	piece->key = (uint16_t)key;
	piece->style = (uint8_t)style;
	pieces.slot[take >= 0 ? take : key & (PROJECTILE_PIECES - 1)] = (uint8_t)(index + 1);
}

/* a frame of dt seconds: the pieces a frame older */
static void update(float dt)
{
	for (int i = 0; i < PROJECTILE_PIECES; i++)
		pieces.piece[i].age += dt;
}

/* (the pieces' mode and mask, as the sniper's: HOT, so libdragon's mode
helpers inline) */
HOT static __attribute__((noinline)) void pieces_mode(void)
{
	surface_t mask = surface_make_linear(n64_sniper_mask, FMT_I4, 16, 4);

	data_cache_hit_writeback(n64_sniper_mask, sizeof(n64_sniper_mask));
	rdpq_sync_pipe();
	rdpq_mode_begin();
	rdpq_set_mode_standard();
	rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, SHADE, 0)));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_zbuf(true, false);
	rdpq_mode_persp(true);
	rdpq_mode_filter(FILTER_BILINEAR);
	rdpq_mode_end();
	rdpq_tex_upload(TILE0, &mask, NULL);
}

/* a piece's vertices: the sniper's (XYZ, ST, INV_W), then the shade (RGBA, 0-1;
only its alpha is used: the fade and fog at the vertex) */
static const rdpq_trifmt_t piece_format =
{
	.pos_offset = 0, .z_offset = 2, .tex_offset = 3, .tex_tile = TILE0, .shade_offset = 6,
};

/* how far a piece of direction d (|d|^2 = d2) is seen across at the point p,
both in the view's frame: the sine of its angle to the line of sight to p --
1 broadside, 0 along the sight (the tags' "fade when perpendicular", taken at
each point as Halo does) */
static float piece_seen(const float d[3], float d2, const float p[3])
{
	float p2 = p[0] * p[0] + p[1] * p[1] + p[2] * p[2], dp = d[0] * p[0] + d[1] * p[1] + d[2] * p[2];

	return d2 > 0.f && p2 > 0.f ? sqrtf(n64_max_f(1.f - dp * dp / (d2 * p2), 0.f)) : 0.f;
}

/* a piece's quad: both ends in front of the near plane and short of the far
cut, its four corners within PIECE_GUARD of the view -- no clipping, the RDP's
scissor cuts at the view's edge (the common case: one projection an end, no
plane tests); else the sniper's clipping geometry. Out as the sniper's (XYZ,
ST, INV_W); *quick_out 1 for the quick quad (corners 1 and 2 at the end, 0 and
3 at the start), 0 for the clipped one */
static int piece_geometry(const struct n64_particle_view *view, const float start[3], const float end[3], float half,
	const float camera[2][3], float out[N64_SNIPER_CLIP][6], int *quick_out)
{
	float x[2], y[2], inv[2], w[2];
	float px = view->pixels_per_unit * view->x_scale, py = view->pixels_per_unit;
	float near = view->z_near * 1.01f;
	float far = view->fog_end > near && view->fog_end < view->z_far ? view->fog_end : view->z_far;
	int quick = px > 0.f && py > 0.f && near > 0.f;

	*quick_out = 0;

	for (int i = 0; i < 2; i++)
		quick &= camera[i][2] >= near && camera[i][2] <= far;
	if (!quick)
		return sniper_ribbon_geometry(view, start, end, half, out);
	for (int i = 0; i < 2; i++)
	{
		inv[i] = 1.f / camera[i][2];
		x[i] = view->center_x + camera[i][0] * px * inv[i];
		y[i] = view->center_y - camera[i][1] * py * inv[i];
		w[i] = n64_min_f(n64_max_f(py * half * inv[i], 0.5f), N64_RIBBON_HALF_MAX);
	}
	float dx = x[1] - x[0], dy = y[1] - y[0], screen = sqrtf(dx * dx + dy * dy);

	if (!(screen >= 0.8f))
		return 0;
	float nx = -dy / screen, ny = dx / screen;
	float m22 = (view->z_far + view->z_near) / (view->z_near - view->z_far);
	float m32 = -2.f * view->z_far * view->z_near / (view->z_far - view->z_near);

	for (int v = 0; v < 4; v++)
	{
		int at = v == 1 || v == 2;
		float side = v < 2 ? 1.f : -1.f, cx = x[at] + nx * w[at] * side, cy = y[at] + ny * w[at] * side;

		if (cx < view->x0 - PIECE_GUARD || cx > view->x1 + PIECE_GUARD || cy < view->y0 - PIECE_GUARD ||
			cy > view->y1 + PIECE_GUARD)
			return sniper_ribbon_geometry(view, start, end, half, out);
		out[v][0] = view->chunky ? view->x0 + (cx - view->x0) * view->chunky_scale : cx;
		out[v][1] = cy;
		out[v][2] = n64_max_f(0.f, n64_min_f(1.f, (-m22 + m32 * inv[at]) * .5f + .5f));
		out[v][3] = at ? 15.f : 0.f;
		out[v][4] = v < 2 ? 0.f : 3.f;
		out[v][5] = inv[at];
	}
	*quick_out = 1;
	return 4;
}

/* the long trails in a view, newest first, at most the view's share of
PROJECTILE_PIECES_DRAWN; *mode 3 (this module's mode) once one is drawn */
static void draw(const struct n64_particle_view *view, int views, int *mode)
{
	int share = PROJECTILE_PIECES_DRAWN / views, drawn = 0;
	float fog_per = view->fog_end > view->fog_start ? 1.f / (view->fog_end - view->fog_start) : 0.f;
	/* (a clipped vertex's place in the view's frame, back from the screen) */
	float inv_px = view->pixels_per_unit * view->x_scale > 0.f ? 1.f / (view->pixels_per_unit * view->x_scale) : 0.f;
	float inv_py = view->pixels_per_unit > 0.f ? 1.f / view->pixels_per_unit : 0.f;
	float inv_chunky = view->chunky && view->chunky_scale > 0.f ? 1.f / view->chunky_scale : 1.f;

	for (int k = 1; k <= PROJECTILE_PIECES && drawn < share; k++)
	{
		struct projectile_piece const *piece =
			&pieces.piece[(pieces.next + PROJECTILE_PIECES - k) & (PROJECTILE_PIECES - 1)];

		if (!piece->style)
			continue;
		struct piece_style const *style = &piece_styles[piece->style - 1];
		float age = piece->age;

		if (age >= style->life)
			continue;
		int n = 1;

		while (n < style->keys - 1 && age >= style->key[n].t)
			n++;
		struct piece_key const *a = &style->key[n - 1], *b = &style->key[n];
		float f = n64_min_f(n64_max_f((age - a->t) / (b->t - a->t), 0.f), 1.f);
		float channel[4];

		for (int c = 0; c < 4; c++)
		{
			float from = (float)((a->rgba >> (24 - 8 * c)) & 0xFF), to = (float)((b->rgba >> (24 - 8 * c)) & 0xFF);

			channel[c] = from + (to - from) * f;
		}
		if (channel[3] < 8.f)
			continue;
		/* (in the view's frame; how far it is seen across at each end, a
		vertex's alpha that times its fog: a shooter's own trail shows near the
		gun, where it is seen across, and fades toward the shot, which it
		follows along the sight) */
		float camera[2][3], d[3], d2 = 0.f, seen[2];

		for (int i = 0; i < 2; i++)
		{
			const float *point = i ? piece->end : piece->start;

			for (int c = 0; c < 3; c++)
			{
				const float *basis = c == 0 ? view->right : c == 1 ? view->up : view->forward;

				camera[i][c] = (point[0] - view->eye[0]) * basis[0] + (point[1] - view->eye[1]) * basis[1] +
					(point[2] - view->eye[2]) * basis[2];
			}
		}
		float dp[2] = { 0.f, 0.f };

		for (int c = 0; c < 3; c++)
		{
			d[c] = camera[1][c] - camera[0][c];
			d2 += d[c] * d[c];
			dp[0] += d[c] * camera[0][c];
			dp[1] += d[c] * camera[1][c];
		}
		for (int i = 0; i < 2; i++)
			seen[i] = channel[3] * piece_seen(d, d2, camera[i]);
		/* (seen across most at its point nearest the eye: dropped when both
		ends are too faint and that point is not between them) */
		if (seen[0] < 8.f && seen[1] < 8.f && !(dp[0] < 0.f && dp[1] > 0.f))
			continue;
		float projected[N64_SNIPER_CLIP][6];
		int quick, count = piece_geometry(view, piece->start, piece->end, a->half + (b->half - a->half) * f,
			(const float (*)[3])camera, projected, &quick);

		if (count < 3)
			continue;
		/* (a quick quad's corners take their end's fade and fog; a clipped
		one's vertices their own, from their place in the view's frame) */
		float vertex[N64_SNIPER_CLIP][10], most = 0.f, ends[2];

		if (quick)
			for (int i = 0; i < 2; i++)
			{
				float z = camera[i][2];

				ends[i] = fog_per > 0.f && z > view->fog_start ?
					seen[i] * n64_max_f(1.f - (z - view->fog_start) * fog_per, 0.f) : seen[i];
			}
		for (int v = 0; v < count; v++)
		{
			float alpha;

			if (quick)
				alpha = ends[v == 1 || v == 2];
			else
			{
				float z = 1.f / projected[v][5];
				float x = view->chunky ? view->x0 + (projected[v][0] - view->x0) * inv_chunky : projected[v][0];
				float p[3] = { (x - view->center_x) * z * inv_px, (view->center_y - projected[v][1]) * z * inv_py, z };

				alpha = channel[3] * piece_seen(d, d2, p);
				if (fog_per > 0.f && z > view->fog_start)
					alpha *= n64_max_f(1.f - (z - view->fog_start) * fog_per, 0.f);
			}
			memcpy(vertex[v], projected[v], sizeof(projected[v]));
			vertex[v][6] = vertex[v][7] = vertex[v][8] = 1.f;
			/* (the RSP's setup truncates: half a step up) */
			vertex[v][9] = n64_min_f((alpha + 0.5f) * (1.f / 255.f), 1.f);
			most = n64_max_f(most, alpha);
		}
		if (most < 8.f)
			continue;
		if (*mode != 3)
		{
			pieces_mode();
			*mode = 3;
		}
		rdpq_set_prim_color(RGBA32((int)channel[0], (int)channel[1], (int)channel[2], 255));
		for (int v = 1; v < count - 1; v++)
			rdpq_triangle(&piece_format, vertex[0], vertex[v], vertex[v + 1]);
		drawn++;
	}
}

const struct n64_projectiles_full_entries n64_projectiles_full_entries =
{
	record,
	update,
	draw,
};
