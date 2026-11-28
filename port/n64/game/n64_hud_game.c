/*
N64_HUD_GAME.C

The HUD as the game draws it: the rasterizer calls interface/hud*.c,
motion_sensor.c and game_engine.c make, turned into the N64 HUD's (n64.h,
port/n64/src/n64_hud.c). The game lays the HUD out on a 640x480 screen;
the N64 shows 320x240, and the HUD's bitmaps are half size
(tools/n64/hud.py), so positions and texels both halve (n64_hud.c doubles
the texels across for the full-width set of a HUD 640 across).

A bitmap the converter exported carries its sprite's number + 1 in
pixels_offset; the others have none, and the game skips them
(_texture_cache_bitmap_get_hardware_format returns NULL).
*/

#include "cseries.h"

#include "bitmaps/bitmap_group.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"

#include "../src/n64.h"
#include "../src/n64_score_table.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* motion_sensor.c's (the sensor's centre and its scale) */
extern real_point2d center_point;
extern real scale[];

/* interface/hud_draw.c's meter record (the meter shader's inputs) */
struct n64_meter_parameters
{
	pixel32 gradient_min_color;
	pixel32 gradient_max_color;
	pixel32 background_color;
	pixel32 flash_color;
	boolean flash_color_is_negative;
	boolean tint_mode_2;
	byte pad12[2];
	pixel32 tint_color;
	real gradient;
};

/* the game's 640x480 layout in the N64's pixels (half, at 320x240): across,
squeezed about the view's middle with the anamorphic picture
(n64_render_hud_squeeze); without it, the same scaling as before to the bit */
static real hud_x(
	real x)
{
	real scale = n64_render_layout_scale_x();
	real squeeze = n64_render_hud_squeeze();
	real middle;

	if (squeeze == 1.f)
		return x * scale;
	middle = (render.camera.viewport_bounds.x0 + render.camera.viewport_bounds.x1) * 0.5f * scale;
	return middle + (x * scale - middle) * squeeze;
}

static real hud_y(
	real y)
{
	return y * n64_render_layout_scale_y();
}

void *_texture_cache_bitmap_get_hardware_format(
	struct bitmap_data *bitmap,
	boolean block,
	boolean load)
{
	(void)block;
	(void)load;
	return bitmap && bitmap->pixels_offset > 0 ? bitmap : NULL;
}

/* the view's HUD began before interface_draw_screen (n64_render_game.c) */
void rasterizer_hud_begin(
	void)
{
	return;
}

void rasterizer_hud_end(
	void)
{
	return;
}

/* hud_draw.c's _shader_framebuffer_blend_function_multiply (its own enum),
a multitexture overlay's first map times its second */
#define MAP_BLEND_MULTIPLY 1

/* a piece of a strip of a map: the screen's rectangle, the map's (in its
sprite's texels) */
static void masked_map_piece(
	short sprite,
	real x0, real y0, real x1, real y1,
	real s0, real t0, real s1, real t1,
	unsigned long color)
{
	struct n64_hud_quad quad;

	memset(&quad, 0, sizeof(quad));
	quad.sprite = sprite;
	quad.x[0] = quad.x[3] = x0;
	quad.x[1] = quad.x[2] = x1;
	quad.y[0] = quad.y[1] = y0;
	quad.y[2] = quad.y[3] = y1;
	quad.s[0] = quad.s[3] = s0;
	quad.s[1] = quad.s[2] = s1;
	quad.t[0] = quad.t[1] = t0;
	quad.t[2] = quad.t[3] = t1;
	quad.color = color;
	n64_hud_quad(&quad);
}

/* A multitexture overlay whose first map masks its second -- the sniper's
angle ticks: a fading strip over ticks that scroll with the aim, faded in
by their effectors only when zoomed. The N64's HUD draws one map a quad:
drawing the first alone left two white bars on the screen, zoomed or not.
The second is drawn instead, scrolled and faded as Halo has it (its
texture coordinates are the first's times its scale plus its offset), in
two pieces where the strip passes its map's end (it wraps); the mask's
fading ends are left out */
static void draw_masked_map(
	struct rasterizer_dynamic_screen_geometry_parameters *parameters,
	struct dynamic_screen_vertex *vertices)
{
	struct bitmap_data *bitmap = parameters->map[1];
	real fade = parameters->map_fade[1] ? *parameters->map_fade[1] : 1.f;
	real origin_x, origin_y, x0, y0, x1, y1, u0, v0, u1, v1, k, texel_u, texel_v;
	real base_u = parameters->map_texture_scale[0].i ? parameters->map_texture_scale[0].i : 1.f;
	real base_v = parameters->map_texture_scale[0].j ? parameters->map_texture_scale[0].j : 1.f;
	real offset_u = parameters->map_offset[1] ? parameters->map_offset[1]->x : 0.f;
	real offset_v = parameters->map_offset[1] ? parameters->map_offset[1]->y : 0.f;
	/* (the element's colour, or its effectors' tint; the ticks' blue is
	their bitmap's own, kept in colour on the cartridge: tools/n64/hud.py) */
	unsigned long rgb = vertices[0].color & 0xFFFFFFu, alpha, color;

	if (!bitmap || bitmap->pixels_offset <= 0 || fade <= 0.f)
		return;
	origin_x = render.camera.viewport_bounds.x0 + (parameters->offset ? parameters->offset->i : 0.f);
	origin_y = render.camera.viewport_bounds.y0 + (parameters->offset ? parameters->offset->j : 0.f);
	x0 = hud_x(origin_x + vertices[0].position.x);
	y0 = hud_y(origin_y + vertices[0].position.y);
	x1 = hud_x(origin_x + vertices[2].position.x);
	y1 = hud_y(origin_y + vertices[2].position.y);
	u0 = vertices[0].texture_coordinates.x * base_u * parameters->map_scale[1].i + offset_u;
	v0 = vertices[0].texture_coordinates.y * base_v * parameters->map_scale[1].j + offset_v;
	u1 = vertices[2].texture_coordinates.x * base_u * parameters->map_scale[1].i + offset_u;
	v1 = vertices[2].texture_coordinates.y * base_v * parameters->map_scale[1].j + offset_v;
	if (v1 < v0 || v1 - v0 > 1.f || u1 - u0 > 1.f || y1 <= y0)
		return;
	/* (within the map's first turn) */
	k = floorf(v0);
	v0 -= k;
	v1 -= k;
	k = floorf(u0 < u1 ? u0 : u1);
	u0 -= k;
	u1 -= k;
	/* (in its sprite's texels: half size, as the HUD's bitmaps are) */
	texel_u = (TEST_FLAG(bitmap->flags, 4) ? 1.f : bitmap->width) * 0.5f *
		(parameters->map_texture_scale[1].i ? parameters->map_texture_scale[1].i : 1.f);
	texel_v = (TEST_FLAG(bitmap->flags, 4) ? 1.f : bitmap->height) * 0.5f *
		(parameters->map_texture_scale[1].j ? parameters->map_texture_scale[1].j : 1.f);
	if (parameters->map_tint[1])
	{
		rgb = (unsigned long)PIN(parameters->map_tint[1]->red * 255.f, 0.f, 255.f) << 16 |
			(unsigned long)PIN(parameters->map_tint[1]->green * 255.f, 0.f, 255.f) << 8 |
			(unsigned long)PIN(parameters->map_tint[1]->blue * 255.f, 0.f, 255.f);
	}
	alpha = (unsigned long)PIN((real)(vertices[0].color >> 24) * PIN(fade, 0.f, 1.f), 0.f, 255.f);
	color = alpha << 24 | rgb;
	if (v1 <= 1.f)
		masked_map_piece((short)(bitmap->pixels_offset - 1), x0, y0, x1, y1,
			u0 * texel_u, v0 * texel_v, u1 * texel_u, v1 * texel_v, color);
	else
	{
		real split = y0 + (y1 - y0) * (1.f - v0) / (v1 - v0);

		masked_map_piece((short)(bitmap->pixels_offset - 1), x0, y0, x1, split,
			u0 * texel_u, v0 * texel_v, u1 * texel_u, texel_v, color);
		masked_map_piece((short)(bitmap->pixels_offset - 1), x0, split, x1, y1,
			u0 * texel_u, 0.f, u1 * texel_u, (v1 - 1.f) * texel_v, color);
	}
}

void rasterizer_psuedo_dynamic_screen_quad_draw(
	struct rasterizer_dynamic_screen_geometry_parameters *parameters,
	struct dynamic_screen_vertex *vertices)
{
	struct bitmap_data *bitmap = parameters->map[0];
	struct n64_hud_quad quad;
	real origin_x, origin_y;
	real texel_u, texel_v;
	short i;

	if (parameters->map[1] && parameters->map0_to_1_blend_function == MAP_BLEND_MULTIPLY)
	{
		draw_masked_map(parameters, vertices);
		return;
	}
	if (!bitmap || bitmap->pixels_offset <= 0)
		return;
	quad.sprite = (short)(bitmap->pixels_offset - 1);
	origin_x = render.camera.viewport_bounds.x0 + (parameters->offset ? parameters->offset->i : 0.f);
	origin_y = render.camera.viewport_bounds.y0 + (parameters->offset ? parameters->offset->j : 0.f);
	/* linear (interface) bitmaps are addressed in texels, swizzled ones from
	0 to 1; either way scaled by map_texture_scale first */
	if (TEST_FLAG(bitmap->flags, 4)) /* _bitmap_linear_bit */
	{
		texel_u = 1.f;
		texel_v = 1.f;
	}
	else
	{
		texel_u = bitmap->width;
		texel_v = bitmap->height;
	}
	texel_u *= parameters->map_texture_scale[0].i ? parameters->map_texture_scale[0].i : 1.f;
	texel_v *= parameters->map_texture_scale[0].j ? parameters->map_texture_scale[0].j : 1.f;
	for (i = 0; i < 4; i++)
	{
		quad.x[i] = hud_x(origin_x + vertices[i].position.x);
		quad.y[i] = hud_y(origin_y + vertices[i].position.y);
		quad.s[i] = vertices[i].texture_coordinates.x * texel_u * 0.5f;
		quad.t[i] = vertices[i].texture_coordinates.y * texel_v * 0.5f;
	}
	quad.color = vertices[0].color;
	quad.meter = parameters->meter_parameters != NULL;
	if (quad.meter)
	{
		struct n64_meter_parameters const *meter = parameters->meter_parameters;

		quad.minimum_color = meter->gradient_min_color;
		quad.maximum_color = meter->gradient_max_color;
		quad.flash_color = meter->flash_color_is_negative ? meter->flash_color & 0xFF000000 : meter->flash_color;
		quad.background_color = meter->background_color;
		quad.tint_color = meter->tint_color;
	}
	n64_hud_quad(&quad);
	return;
}

/* ---------- text */

/* The native actor scan owns eligibility and the selected weapon. This only
labels its real hold-to-pick-up action in the current split-screen viewport;
B remains Xbox X in every preset, including both Golden schemes. */
extern short n64_hud_original_action_icon(short x, short baseline, pixel32 color, short *bottom);
/* HUD STYLE 64 (n64_hud64_game.c): 1 when it takes the prompt, a string */
extern int n64_hud64_game_prompt(char const *name, int swap);
extern int n64_hud64_game_collect_string(wchar_t const *string);
void n64_hud_game_weapon_prompt(char const *name, int swap)
{
	real virtual_x = render.camera.viewport_bounds.x0 + 16;
	real virtual_y = render.camera.viewport_bounds.y0 +
		(render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0) * 0.64f;
	int x = (int)hud_x(virtual_x), y = (int)hud_y(virtual_y);
	real text_scale = n64_render_layout_scale_x() * n64_render_hud_squeeze();
	short icon_x, after_icon, icon_bottom = (short)virtual_y;
	int name_y = y + 16;

	if (n64_hud64_game_prompt(name, swap))
		return;
	/* Keep the existing HOLD / action / weapon flow and the original font.
	   Pixel advance is measured after the existing lazy font load. */
	n64_hud_text(x, y, 0xFF8CC0FFu, "HOLD ");
	if (name_y < y + n64_hud_text_height() + 2)
		name_y = y + n64_hud_text_height() + 2;
	icon_x = (short)(virtual_x + n64_hud_text_width("HOLD ") / text_scale);
	after_icon = n64_hud_original_action_icon(icon_x, (short)(virtual_y + 24), 0xFF8CC0FFu, &icon_bottom);
	if (after_icon > icon_x)
	{
		real bottom_y = hud_y(icon_bottom);
		int below_icon = (int)bottom_y;
		if ((real)below_icon < bottom_y) below_icon++;
		if (name_y < below_icon + 2) name_y = below_icon + 2;
		n64_hud_text((int)hud_x(after_icon + 3), y, 0xFF8CC0FFu,
			swap ? " TO SWAP FOR" : " TO PICK UP");
	}
	else
		n64_hud_text((int)hud_x(icon_x), y, 0xFF8CC0FFu,
			swap ? "B TO SWAP FOR" : "B TO PICK UP");
	/* Follow the actual authored icon and original font at every HUD scale. */
	n64_hud_text(x, name_y, 0xFFFFFFFFu, name);
}

static void draw_text(
	rectangle2d const *bounds,
	char const *text)
{
	short x, y;

	/* (Halo's text bounds are the view's -- its window bounds moved by the
	view's corner, as for its bitmaps: the view's corner added, else in 2
	to 4 players' views but the first every text fell inside the first
	view's corner and was cut away by its own view's scissor) */
	if (bounds)
	{
		x = (short)hud_x(bounds->x0 + render.camera.viewport_bounds.x0);
		y = (short)hud_y(bounds->y0 + render.camera.viewport_bounds.y0);
	}
	else
	{
		x = (short)hud_x(render.camera.window_bounds.x0);
		y = (short)hud_y(render.camera.window_bounds.y0);
	}
	n64_hud_text(x, y, 0xFFFFFFFF, text);
}

void rasterizer_draw_string(
	rectangle2d const *bounds,
	rectangle2d const *clip,
	point2d *cursor_reference,
	short height_adjust,
	char const *string)
{
	(void)clip;
	(void)cursor_reference;
	(void)height_adjust;
	if (string && *string)
		draw_text(bounds, string);
	return;
}

/* where the last text drawn on a HUD line ended (Halo's virtual pixels):
the next piece of that line, and its button icon, start there
(hud_messaging.c render_state_bitmap); reset at each state message;
"button" when a button was placed there since */
static struct { short x, y, dy, button; } text_pen = {0, SHORT_MIN, 0, 0};

void n64_hud_text_pen_reset(void)
{
	text_pen.y = SHORT_MIN;
	text_pen.dy = 0;
	text_pen.button = 0;
}

short n64_hud_text_pen_wrapped(void)
{
	/* (how far down the state line's last piece went when it was
	wrapped: the messages under the line move down as far) */
	return text_pen.y != SHORT_MIN ? text_pen.dy : 0;
}

short n64_hud_text_pen(short y)
{
	/* (a button after a wrapped piece stays where Halo put it) */
	if (y != text_pen.y || text_pen.dy)
		return SHORT_MIN;
	text_pen.button = 1;
	return text_pen.x;
}

void rasterizer_draw_unicode_string(
	rectangle2d const *bounds,
	rectangle2d const *clip,
	point2d *cursor_reference,
	short height_adjust,
	wchar_t const *string)
{
	char text[128];
	short i;

	(void)clip;
	(void)cursor_reference;
	(void)height_adjust;
	if (!string || n64_hud64_game_collect_string(string))
		return;
	for (i = 0; i < (short)sizeof(text) - 1 && string[i]; i++)
		text[i] = string[i] < 128 ? (char)string[i] : '?';
	text[i] = 0;
	if (i)
	{
		/* (Halo lays a state message out piece by piece, the first line of
		each indented to where the last piece ended: draw_string's initial
		indent; without it every piece started at the line's start, one
		over the other) */
		extern short draw_string_initial_indent(void);
		rectangle2d indented = bounds ? *bounds : render.camera.window_bounds;
		/* (no bounds: the window's, made the view's as Halo's are -- the
		text drawer adds the view's corner) */
		if (!bounds)
			offset_rectangle2d(&indented, -render.camera.viewport_bounds.x0, -render.camera.viewport_bounds.y0);
		real text_scale = n64_render_layout_scale_x() * n64_render_hud_squeeze();
		short indent = draw_string_initial_indent();

		indented.x0 += indent;
		/* (the N64's font draws wider or narrower than Halo's font
		measured -- about twice as wide at 320 across: a piece continuing a
		line starts where the last piece as drawn ended, not where Halo's
		measure put it; after a button, where Halo put it, past the button
		placed at the pen) */
		short line_y = indented.y0, dy = 0;
		char *piece = text;
		real width = n64_hud_text_width(piece) / text_scale;
		short right = (short)(render.camera.window_bounds.x1 - render.camera.viewport_bounds.x0);

		if (indent > 0 && line_y == text_pen.y)
		{
			/* (on a line this drawer broke, after the last piece alone:
			Halo's indent measures the line unbroken) */
			if (!text_pen.button || text_pen.x > indented.x0 || text_pen.dy > 0)
				indented.x0 = text_pen.x;
			dy = text_pen.dy;
			/* (past the view's right edge -- 3 and 4 players' views: the
			piece on a new line under the last, without its leading
			spaces) */
			if (bounds && indented.x0 + width > right && indented.x0 > bounds->x0)
			{
				indented.x0 = bounds->x0;
				dy += (short)(n64_hud_text_height() / n64_render_layout_scale_y() + 2.f);
				while (*piece == ' ')
					piece++;
				width = n64_hud_text_width(piece) / text_scale;
			}
		}
		indented.y0 += dy;
		draw_text(&indented, piece);
		/* (measured again: the first text drawn loads the font, and before
		that the width was a guess) */
		width = n64_hud_text_width(piece) / text_scale;
		text_pen.x = (short)(indented.x0 + width + 0.5f);
		text_pen.y = line_y;
		text_pen.dy = dy;
		text_pen.button = 0;
	}
	return;
}

/* ---------- the scores' table at a game's end (n64_score_table.h) */

/* the menu's colours (the pause screen's, n64_hud.c), opaque: nothing of
the view's HUD under the table shows through. The panel, the text, the
headings, the view's own row, the teams' text and rows */
#define TABLE_PANEL 0xFF001028u
#define TABLE_TEXT 0xFF8CC0FFu
#define TABLE_BRIGHT 0xFFFFFFFFu
#define TABLE_DIM 0xFF42648Eu
#define TABLE_BAND 0xFF0C274Cu
static const unsigned long table_team_text[2] = { 0xFFD03838u, 0xFF3C6CE0u };
static const unsigned long table_team_band[2] = { 0xFF4E1F2Eu, 0xFF16326Du };

static unsigned long table_fade(
	unsigned long argb,
	real alpha)
{
	real a = (real)(argb >> 24) * PIN(alpha, 0.f, 1.f);

	return ((unsigned long)a << 24) | (argb & 0xFFFFFFu);
}

static real table_width(
	const struct n64_table_layout *l,
	const char *text)
{
	return n64_hud_text_width(text) * l->squeeze;
}

/* a cell's text from x, or ending at x (right); y the row's top; cut to
limit pixels (0: none) */
static void table_text(
	const struct n64_table_layout *l,
	real alpha,
	real x,
	int y,
	int right,
	unsigned long color,
	const char *text,
	real limit)
{
	char cut[16];

	if (!text[0])
		return;
	if (limit > 0.f && table_width(l, text) > limit + 0.01f)
	{
		int n;

		snprintf(cut, sizeof(cut), "%s", text);
		for (n = (int)strlen(cut); n > 0 && table_width(l, cut) > limit + 0.01f; n--)
			cut[n - 1] = 0;
		text = cut;
	}
	if (right)
		x -= table_width(l, text);
	n64_hud_text_squeezed((int)x, y + l->toff, table_fade(color, alpha), text, l->squeeze);
}

static void table_fill(
	real alpha,
	real x0,
	int y0,
	real x1,
	int y1,
	unsigned long color)
{
	n64_hud_fill(x0, (float)y0, x1, (float)y1, table_fade(color, alpha));
}

/* the columns' widths (the widest text of each); the table's width */
static real table_measure(
	const struct n64_table *t,
	struct n64_table_layout *l)
{
	real total = l->bar > 0.f ? l->bar + l->gap * 0.5f : 0.f;
	int i, k;

	for (i = 0; l->columns[i] >= 0; i++)
	{
		int c = l->columns[i];

		l->width[c] = l->headings ? table_width(l, t->headings[c]) : 0.f;
		for (k = 0; k < t->count; k++)
			l->width[c] = MAX(l->width[c], table_width(l, t->rows[k].cell[c]));
		if (l->grouped && c == N64_TABLE_SCORE)
			for (k = 0; k < 2; k++)
				l->width[c] = MAX(l->width[c], table_width(l, t->team_score[k]));
		total += l->width[c] + (i ? l->gap : 0.f);
	}
	/* (a team's name spans the place and name columns) */
	for (k = 0; l->grouped && k < 2; k++)
	{
		real over = table_width(l, t->team_name[k]) - (l->width[N64_TABLE_PLACE] + l->gap + l->width[N64_TABLE_NAME]);

		if (over > 0.f)
		{
			l->width[N64_TABLE_NAME] += over;
			total += over;
		}
	}
	return total;
}

static void table_columns(
	struct n64_table_layout *l,
	const signed char *columns)
{
	int i;

	for (i = 0; columns[i] >= 0; i++)
		l->columns[i] = columns[i];
	l->columns[i] = -1;
}

/* the widest line: the columns, the title, the teams' line with PRESS A */
static real table_content(
	const struct n64_table *t,
	struct n64_table_layout *l,
	const char *title,
	real *total)
{
	real content, space = table_width(l, " ");

	*total = table_measure(t, l);
	content = *total;
	if (l->title)
		content = MAX(content, table_width(l, title));
	if (l->team_line)
		content = MAX(content, table_width(l, t->team_name[0]) + table_width(l, t->team_score[0]) +
			table_width(l, t->team_name[1]) + table_width(l, t->team_score[1]) + table_width(l, "PRESS A") +
			l->gap * 2.f + 2.f * MAX(l->gap * 0.5f, space));
	return content;
}

/* the view's layout. Down first: rows 3 to 0 px apart (the font's tallest
glyph and lowest packed), with 3 px of padding, then 1; while the game ends,
then without the headings, then without the title too; after it, a team
game, the teams grouped, then their totals on one line with PRESS A, then
that without the headings; free for all, then without the headings. Then
across, for those rows: the most columns at the widest spacing that fit
(while the game ends, then without the headings); then the text squeezed,
not below 0.7; at last the name column cut to the room. Two texts on a line
are never closer than a space. The panel is as wide as its widest line */
static void table_layout(
	const struct n64_table *t,
	struct n64_table_layout *l,
	const char *title)
{
	static const signed char in_game[] = { N64_TABLE_PLACE, N64_TABLE_NAME, N64_TABLE_SCORE, -1 };
	static const signed char all[] = { N64_TABLE_PLACE, N64_TABLE_NAME, N64_TABLE_SCORE, N64_TABLE_KILLS,
		N64_TABLE_ASSISTS, N64_TABLE_DEATHS, -1 };
	static const signed char no_assists[] = { N64_TABLE_PLACE, N64_TABLE_NAME, N64_TABLE_SCORE, N64_TABLE_KILLS,
		N64_TABLE_DEATHS, -1 };
	static const real spacings[3][2] = { { 6.f, 8.f }, { 4.f, 5.f }, { 2.f, 3.f } };
	const signed char *sets[2];
	rectangle2d w = render.camera.window_bounds, v = render.camera.viewport_bounds;
	real hx = n64_render_layout_scale_x() * 2.f * n64_render_hud_squeeze();
	int vu = n64_render_layout_scale_y() > 0.75f ? 2 : 1;
	int inset = n64_hud_inset() * 2, asc, desc, cap, set_count, s, sp, step, i, lines, fit = 0, mode, modes, extra, pass;
	int squeezable;
	int has_title = title && title[0];
	real ux0, ux1, uy0, uy1, avail_w, avail_h, total = 0.f, content = 0.f, cx, x, gap = 0.f;

	n64_hud_text(0, 0, 0, "");
	/* (the font's tallest glyph above the baseline, its lowest below; without
	the HUD font, the built-in one: it cannot be squeezed) */
	squeezable = n64_hud_text_metrics(&asc, &desc, &cap);
	/* the room: the view's window less the HUD inset where the view meets
	the screen's edge, and a margin where it meets another view */
	ux0 = w.x0 + (v.x0 <= 0 ? inset : 6);
	ux1 = w.x1 - (v.x1 >= 640 ? inset : 6);
	uy0 = w.y0 + (v.y0 <= 0 ? inset : 4);
	uy1 = w.y1 - (v.y1 >= 480 ? inset : 4);
	/* (a pixel less: the panel's edges on whole pixels stay inside) */
	avail_w = hud_x(ux1) - hud_x(ux0) - 1.f;
	avail_h = hud_y(uy1) - hud_y(uy0);
	cx = (hud_x(ux0) + hud_x(ux1)) * 0.5f;

	/* down */
	l->squeeze = 1.f;
	l->headings = 1;
	l->title = has_title;
	l->grouped = t->postgame && t->teams;
	l->team_line = 0;
	l->rule = (short)vu;
	modes = t->postgame ? (t->teams ? 3 : 2) : 3;
	for (mode = 0; mode < modes && !fit; mode++)
	{
		if (!t->postgame)
		{
			l->headings = mode == 0;
			l->title = mode == 2 ? 0 : has_title;
		}
		else
		{
			l->headings = mode < modes - 1;
			l->grouped = t->teams && mode == 0;
			l->team_line = t->teams && mode >= 1;
		}
		for (pass = 0; pass < 2 && !fit; pass++)
		{
			l->padv = (short)((pass ? 1 : 3) * vu);
			for (extra = 3; extra >= 0 && !fit; extra--)
			{
				l->pitch = (short)(cap + desc + extra);
				lines = l->title + l->team_line + l->headings + t->count + (l->grouped ? 2 : 0);
				/* (PRESS A: under the table; or on the teams' line) */
				l->prompt_line = t->postgame && !l->team_line;
				l->height = (short)(lines * l->pitch + (l->headings ? l->rule : 0) + 2 * l->padv);
				fit = l->height + (l->prompt_line ? l->pitch + l->padv : 0) <= avail_h;
			}
		}
	}

	/* across (after the game without the headings, no assists: four numbers
	without their headings read worse than three) */
	sets[0] = !t->postgame ? in_game : l->headings ? all : no_assists;
	sets[1] = t->postgame ? no_assists : in_game;
	set_count = t->postgame && l->headings ? 2 : 1;
	l->bar = l->grouped ? 3.f * hx : 0.f;
	fit = 0;
	for (step = 0; step < 2 && !fit; step++)
	{
		for (s = 0; s < set_count && !fit; s++)
		{
			for (sp = 0; sp < 3 && !fit; sp++)
			{
				table_columns(l, sets[s]);
				l->pad = spacings[sp][0] * hx;
				gap = spacings[sp][1] * hx;
				l->gap = MAX(gap, table_width(l, " "));
				content = table_content(t, l, title, &total);
				fit = content + 2.f * l->pad <= avail_w;
			}
		}
		/* (while the game ends, the headings give way; they also free a line) */
		if (fit || t->postgame || !l->headings)
			break;
		l->headings = 0;
		l->height = (short)(l->height - l->pitch - l->rule);
	}
	for (i = 0; i < 4 && !fit && squeezable; i++)
	{
		/* (the last set and spacing, the text squeezed; a space squeezes too) */
		l->squeeze = MAX(0.7f, l->squeeze * (avail_w - 2.f * l->pad) / content);
		l->gap = MAX(gap, table_width(l, " "));
		content = table_content(t, l, title, &total);
		fit = content + 2.f * l->pad <= avail_w || l->squeeze <= 0.7f;
	}
	/* (still wider at the squeeze's limit: the name column, and the title,
	cut to the room; the panel never leaves it) */
	if (content + 2.f * l->pad > avail_w)
	{
		real over = MIN(total + 2.f * l->pad - avail_w, l->width[N64_TABLE_NAME]);

		if (over > 0.f)
		{
			l->width[N64_TABLE_NAME] -= over;
			total -= over;
		}
		content = avail_w - 2.f * l->pad;
	}
	/* (the text centred in its row: the tallest glyph's top to the lowest's
	bottom inside it, at every pitch) */
	l->toff = (short)((l->pitch - (cap + desc) + 1) / 2 - (asc - cap));
	l->y0 = (short)((hud_y(uy0) + hud_y(uy1) - (real)(l->height + (l->prompt_line ? l->pitch + l->padv : 0))) * 0.5f);
	if (l->y0 < (short)hud_y(uy0))
		l->y0 = (short)hud_y(uy0);
	l->x0 = cx - content * 0.5f - l->pad;
	l->x1 = cx + content * 0.5f + l->pad;
	x = cx - total * 0.5f + (l->bar > 0.f ? l->bar + l->gap * 0.5f : 0.f);
	for (i = 0; l->columns[i] >= 0; i++)
	{
		l->x[(int)l->columns[i]] = x;
		x += l->width[(int)l->columns[i]] + l->gap;
	}
}

void n64_hud_score_table(
	struct n64_table *t,
	const char *title,
	long view_player_index,
	float alpha,
	float prompt_alpha)
{
	struct n64_table_layout *l;
	rectangle2d w = render.camera.window_bounds;
	int view = render.local_player_index >= 0 && render.local_player_index < N64_TABLE_VIEWS - 1 ?
		render.local_player_index : N64_TABLE_VIEWS - 1;
	int has_title = title && title[0], i, k, y, team;
	long key[8];
	real cx;

	/* the view's layout, measured once for the table and view as they are */
	key[0] = t->generation;
	key[1] = ((long)n64_render_screen_width() << 16) | n64_render_screen_height();
	key[2] = ((long)w.x0 << 16) | (w.y0 & 0xFFFF);
	key[3] = ((long)w.x1 << 16) | (w.y1 & 0xFFFF);
	key[4] = (long)(n64_render_hud_squeeze() * 1000.f);
	key[5] = n64_hud_inset();
	key[6] = has_title ? (long)n64_hud_text_width(title) : -1;
	key[7] = (long)(n64_render_layout_scale_x() * 1000.f);
	l = &t->layouts[view];
	if (memcmp(l->key, key, sizeof(key)))
	{
		memcpy(l->key, key, sizeof(key));
		table_layout(t, l, title);
	}
	cx = (l->x0 + l->x1) * 0.5f;

	/* (the panel and its bands opaque while the table shows; its text fades in
	with Halo's alpha) */
	table_fill(1.f, l->x0, l->y0, l->x1, l->y0 + l->height, TABLE_PANEL);
	y = l->y0 + l->padv;
	if (l->title)
	{
		real limit = l->x1 - l->x0 - 2.f * l->pad;

		table_text(l, alpha, cx - MIN(table_width(l, title), limit) * 0.5f, y, 0, TABLE_BRIGHT, title, limit);
		y += l->pitch;
	}
	if (l->team_line)
	{
		/* (the teams' totals on one line, PRESS A at its end) */
		real x = l->x0 + l->pad;

		for (k = 0; k < 2; k++)
		{
			table_text(l, alpha, x, y, 0, table_team_text[t->team_index[k] & 1], t->team_name[k], 0.f);
			x += table_width(l, t->team_name[k]) + MAX(l->gap * 0.5f, table_width(l, " "));
			table_text(l, alpha, x, y, 0, table_team_text[t->team_index[k] & 1], t->team_score[k], 0.f);
			x += table_width(l, t->team_score[k]) + l->gap;
		}
		if (prompt_alpha > 0.f)
			table_text(l, prompt_alpha, l->x1 - l->pad, y, 1, TABLE_TEXT, "PRESS A", 0.f);
		y += l->pitch;
	}
	if (l->headings)
	{
		for (i = 0; l->columns[i] >= 0; i++)
		{
			int c = l->columns[i];

			table_text(l, alpha, c >= N64_TABLE_SCORE ? l->x[c] + l->width[c] : l->x[c], y, c >= N64_TABLE_SCORE,
				TABLE_DIM, t->headings[c], c == N64_TABLE_NAME ? l->width[c] : 0.f);
		}
		/* (the rule under the headings' row, clear of its descenders) */
		y += l->pitch;
		table_fill(1.f, l->x0 + l->pad, y, l->x1 - l->pad, y + l->rule, TABLE_DIM);
		y += l->rule;
	}
	for (team = 0; team < (l->grouped ? 2 : 1); team++)
	{
		int team_index = l->grouped ? t->team_index[team] : -1;

		if (l->grouped)
		{
			table_fill(1.f, l->x0, y, l->x1, y + l->pitch, table_team_band[team_index & 1]);
			table_text(l, alpha, l->x[N64_TABLE_PLACE], y, 0, TABLE_BRIGHT, t->team_name[team],
				l->width[N64_TABLE_PLACE] + l->gap + l->width[N64_TABLE_NAME]);
			table_text(l, alpha, l->x[N64_TABLE_SCORE] + l->width[N64_TABLE_SCORE], y, 1, TABLE_BRIGHT,
				t->team_score[team], 0.f);
			y += l->pitch;
		}
		for (k = 0; k < t->count; k++)
		{
			const struct n64_table_row *r = &t->rows[k];
			/* (the view's own player; after the game, the players on this N64) */
			int own = t->postgame ? r->local : r->player_index == view_player_index;
			unsigned long color = r->team >= 0 && !l->grouped ? table_team_text[r->team & 1] :
				own ? TABLE_BRIGHT : TABLE_TEXT;

			if (l->grouped && r->team != team_index)
				continue;
			if (own)
				table_fill(1.f, l->x0, y, l->x1, y + l->pitch, TABLE_BAND);
			if (l->grouped)
				table_fill(1.f, l->x0 + l->pad, y + 1, l->x0 + l->pad + l->bar, y + l->pitch - 1,
					table_team_text[team_index & 1]);
			for (i = 0; l->columns[i] >= 0; i++)
			{
				int c = l->columns[i];

				table_text(l, alpha, c >= N64_TABLE_SCORE ? l->x[c] + l->width[c] : l->x[c], y, c >= N64_TABLE_SCORE,
					t->postgame && ((r->best >> c) & 1) ? TABLE_BRIGHT : color, r->cell[c],
					c == N64_TABLE_NAME ? l->width[c] : 0.f);
			}
			y += l->pitch;
		}
	}
	if (l->prompt_line && prompt_alpha > 0.f)
	{
		/* (on a panel of its own: readable over a bright sky; its room is kept
		while it is hidden, so the table does not move) */
		real width = table_width(l, "PRESS A");

		y = l->y0 + l->height + l->padv;
		table_fill(prompt_alpha, cx - width * 0.5f - l->pad, y - 1, cx + width * 0.5f + l->pad, y + l->pitch, TABLE_PANEL);
		table_text(l, prompt_alpha, cx - width * 0.5f, y, 0, TABLE_TEXT, "PRESS A", 0.f);
	}
}

void rasterizer_text_set_shadow_color(pixel32 shadow_color) { (void)shadow_color; return; }
void rasterizer_text_cache_flush(void) { return; }

/* ---------- the motion sensor: blips in the sensor's -1..1 space (the Xbox
drew them into a texture it laid over the sensor) */

#define MOTION_SENSOR_RADIUS 32.f

void rasterizer_hud_motion_sensor_blip_begin(void) { return; }

void rasterizer_hud_motion_sensor_blip_draw(
	real_point2d const *position,
	real intensity,
	real size,
	real_rgb_color const *color,
	boolean large_blip)
{
	real radius = MOTION_SENSOR_RADIUS * scale[0];
	real x = center_point.x + render.camera.viewport_bounds.x0 + position->x * -0.03125f * radius;
	real y = center_point.y + render.camera.viewport_bounds.y0 + position->y * 0.03125f * radius;
	unsigned long argb =
		((unsigned long)PIN(color->red * intensity * 255.f, 0.f, 255.f) << 16) |
		((unsigned long)PIN(color->green * intensity * 255.f, 0.f, 255.f) << 8) |
		(unsigned long)PIN(color->blue * intensity * 255.f, 0.f, 255.f);

	(void)large_blip;
	{
		/* (its size in 320x240's pixels, then the screen's) */
		real pixels = MAX(2.f, size * 0.0625f * radius);

		n64_hud_blip(hud_x(x), hud_y(y), pixels * n64_render_layout_scale_x() * 2.f * n64_render_hud_squeeze(),
			pixels * n64_render_layout_scale_y() * 2.f, argb);
	}
	return;
}

void rasterizer_hud_motion_sensor_blip_end(
	real_point2d const *center,
	real sweep)
{
	(void)center;
	(void)sweep;
	return;
}
