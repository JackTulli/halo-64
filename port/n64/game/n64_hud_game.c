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

void rasterizer_psuedo_dynamic_screen_quad_draw(
	struct rasterizer_dynamic_screen_geometry_parameters *parameters,
	struct dynamic_screen_vertex *vertices)
{
	struct bitmap_data *bitmap = parameters->map[0];
	struct n64_hud_quad quad;
	real origin_x, origin_y;
	real texel_u, texel_v;
	short i;

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

static void draw_text(
	rectangle2d const *bounds,
	char const *text)
{
	rectangle2d window = render.camera.window_bounds;
	short x, y;

	if (!bounds)
		bounds = &window;
	x = (short)hud_x(bounds->x0);
	y = (short)hud_y(bounds->y0);
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
		draw_text(bounds, text);
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
