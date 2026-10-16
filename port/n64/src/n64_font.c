/*
N64_FONT.C

Halo's fonts (tools/n64/ui.py: rom:/fonts/<name>.sprite, an I8 atlas of
the glyphs' 8-bit coverage, and <name>.glyphs, where each glyph is), drawn
as Halo's draw_string does: a glyph at the cursor less its origin, the
cursor on the baseline, moved on by the glyph's width.
*/

#include <libdragon.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"

#define FIRST_CHARACTER 32
#define CHARACTER_COUNT 95

struct glyph
{
	uint16_t x, y;
	uint8_t width, height;
	int8_t origin_x, origin_y;
	uint8_t advance;
	uint8_t present;
};

struct n64_font
{
	sprite_t *atlas;
	short ascent, descent;
	struct glyph glyphs[CHARACTER_COUNT];
};

struct n64_font *n64_font_load(const char *name)
{
	char path[64];
	struct n64_font *font;
	FILE *file;
	uint8_t header[10];

	font = n64_menu_alloc(sizeof(*font));
	if (!font)
		return NULL;
	memset(font, 0, sizeof(*font));
	snprintf(path, sizeof(path), "rom:/fonts/%s.sprite", name);
	font->atlas = n64_menu_sprite_load(path);
	snprintf(path, sizeof(path), "rom:/fonts/%s.glyphs", name);
	file = fopen(path, "rb");
	if (!font->atlas || !file || fread(header, 1, sizeof(header), file) != sizeof(header) || memcmp(header, "H64F", 4))
	{
		if (file)
			fclose(file);
		if (font->atlas)
			n64_menu_sprite_free(font->atlas);
		n64_menu_free(font);
		debugf("n64: no font %s\n", name);
		return NULL;
	}
	font->ascent = (int16_t)((header[4] << 8) | header[5]);
	font->descent = (int16_t)((header[6] << 8) | header[7]);
	for (int i = 0, count = (header[8] << 8) | header[9]; i < count; i++)
	{
		uint8_t raw[12];
		int code;

		if (fread(raw, 1, sizeof(raw), file) != sizeof(raw))
			break;
		code = (raw[0] << 8) | raw[1];
		if (code < FIRST_CHARACTER || code >= FIRST_CHARACTER + CHARACTER_COUNT)
			continue;
		struct glyph *g = &font->glyphs[code - FIRST_CHARACTER];

		g->x = (raw[2] << 8) | raw[3];
		g->y = (raw[4] << 8) | raw[5];
		g->width = raw[6];
		g->height = raw[7];
		g->origin_x = (int8_t)raw[8];
		g->origin_y = (int8_t)raw[9];
		g->advance = raw[10];
		g->present = 1;
	}
	fclose(file);
	return font;
}

void n64_font_free(struct n64_font *font)
{
	if (!font)
		return;
	n64_menu_sprite_free(font->atlas);
	n64_menu_free(font);
}

int n64_font_height(const struct n64_font *font)
{
	return font ? font->ascent + font->descent : 8;
}

int n64_font_ascent(const struct n64_font *font)
{
	return font ? font->ascent : 8;
}

/* how far a capital rises above the baseline (H's), less than the ascent,
which leaves room for accents */
int n64_font_cap_height(const struct n64_font *font)
{
	const struct glyph *h;

	if (!font)
		return 8;
	h = &font->glyphs['H' - FIRST_CHARACTER];
	return h->present && h->origin_y > 0 ? h->origin_y : font->ascent;
}

int n64_text_width(const struct n64_font *font, const char *text)
{
	int width = 0;

	if (!font)
		return (int)strlen(text) * 8;
	for (; *text; text++)
	{
		int c = (unsigned char)*text;

		if (c >= FIRST_CHARACTER && c < FIRST_CHARACTER + CHARACTER_COUNT)
			width += font->glyphs[c - FIRST_CHARACTER].advance;
	}
	return width;
}

/* text with its baseline at y; color 0xAARRGGBB */
void n64_text_draw(const struct n64_font *font, int x, int y, unsigned long color, const char *text)
{
	n64_text_draw_scaled(font, (float)x, y, color, text, 1.f);
}

/* text squeezed across by x_scale (the anamorphic picture's 3/4: a wide
screen stretches it back), its glyphs filtered; at 1, as it always was */
void n64_text_draw_scaled(const struct n64_font *font, float x, int y, unsigned long color, const char *text,
	float x_scale)
{
	surface_t atlas;
	int squeezed = x_scale != 1.f;

	if (!font)
	{
		rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, x, y, text);
		return;
	}
	atlas = sprite_get_pixels(font->atlas);
	rdpq_sync_pipe();
	rdpq_set_mode_standard();
	/* the colour, the glyph's coverage as alpha */
	rdpq_mode_combiner(RDPQ_COMBINER1((0, 0, 0, PRIM), (TEX0, 0, PRIM, 0)));
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_mode_filter(squeezed ? FILTER_BILINEAR : FILTER_POINT);
	rdpq_set_prim_color(RGBA32((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, color >> 24));
	/* rdpq_tex_blit loaded each glyph on its own, the texture loader's work
	on the CPU per character. A band of the atlas's rows (as many as TMEM
	holds) is loaded instead, and each glyph in it drawn as the blit drew
	it -- the same rectangle and texels, point sampled at whole pixels --
	the next band loaded when a glyph lies outside it */
	int band_rows = 4096 / ((TEX_FORMAT_PIX2BYTES(surface_get_format(&atlas), atlas.width) + 7) & ~7);
	int band_top = -1, band_bottom = -1;

	for (; *text; text++)
	{
		int c = (unsigned char)*text;
		const struct glyph *g;

		if (c < FIRST_CHARACTER || c >= FIRST_CHARACTER + CHARACTER_COUNT)
			continue;
		g = &font->glyphs[c - FIRST_CHARACTER];
		if (g->present && g->width && g->height)
		{
			if (g->height > band_rows)
			{
				band_top = band_bottom = -1;
				rdpq_tex_blit(&atlas, x - g->origin_x * x_scale, y - g->origin_y, &(rdpq_blitparms_t){
					.s0 = g->x, .t0 = g->y, .width = g->width, .height = g->height,
					.scale_x = squeezed ? x_scale : 0.f,
				});
			}
			else
			{
				if (g->y < band_top || g->y + g->height > band_bottom)
				{
					band_top = g->y;
					band_bottom = band_top + band_rows < atlas.height ? band_top + band_rows : atlas.height;
					rdpq_tex_upload_sub(TILE0, &atlas, NULL, 0, band_top, atlas.width, band_bottom);
				}
				if (squeezed)
					rdpq_texture_rectangle_scaled(TILE0, x - g->origin_x * x_scale, y - g->origin_y,
						x + (g->width - g->origin_x) * x_scale, y - g->origin_y + g->height,
						g->x, g->y, g->x + g->width, g->y + g->height);
				else
					rdpq_texture_rectangle(TILE0, x - g->origin_x, y - g->origin_y,
						x - g->origin_x + g->width, y - g->origin_y + g->height, g->x, g->y);
			}
		}
		x += g->advance * x_scale;
	}
}
