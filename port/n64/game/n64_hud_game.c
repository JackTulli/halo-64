/*
N64_HUD_GAME.C

The HUD as the game draws it: the rasterizer calls interface/hud*.c,
motion_sensor.c and game_engine.c make, turned into the N64 HUD's (n64.h,
port/n64/src/n64_hud.c). The game lays the HUD out on a 640x480 screen;
the N64 shows 320x240, and the HUD's bitmaps are half size
(tools/n64/hud.py), so positions and texels both halve.

A bitmap the converter exported carries its sprite's number + 1 in
pixels_offset; the others have none, and the game skips them
(_texture_cache_bitmap_get_hardware_format returns NULL).
*/

#include "cseries.h"

#include "bitmaps/bitmap_group.h"
#include "rasterizer/rasterizer.h"
#include "render/render.h"

#include "../src/n64.h"

#include <math.h>
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
void n64_hud_game_weapon_prompt(char const *name, int swap)
{
	real virtual_x = render.camera.viewport_bounds.x0 + 16;
	real virtual_y = render.camera.viewport_bounds.y0 +
		(render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0) * 0.64f;
	int x = (int)hud_x(virtual_x), y = (int)hud_y(virtual_y);
	real text_scale = n64_render_layout_scale_x() * n64_render_hud_squeeze();
	short icon_x, after_icon, icon_bottom = (short)virtual_y;
	int name_y = y + 16;
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
	if (!string)
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
