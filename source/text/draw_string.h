/*
DRAW_STRING.H

header included in hcex build.
*/

#ifndef __DRAW_STRING_H
#define __DRAW_STRING_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct bitmap_data;
struct font_character;
struct font_header;
struct parse_string_state;

typedef void (*draw_character_proc)(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *character,
	pixel32 color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy);

/* ---------- prototypes/DRAW_STRING.C */

void draw_string_initialize(
	void);
void draw_string_initialize_for_new_map(
	void);
void draw_string_dispose_from_old_map(
	void);
void draw_string_dispose(
	void);

char *draw_string_get_string(
	short index);
void draw_string_set_tab_stops(
	short const *tab_stops,
	short count);
void draw_string_set_indents(
	short initial_indent,
	short paragraph_indent);
void draw_string_set_color(
	real_argb_color const *color);
void draw_string_get_color(
	real_argb_color *color);
void draw_string_set_font(
	long font_index);
void draw_string_set_format(
	short style,
	short justification,
	unsigned long flags);
void draw_string_set_draw_mode(
	long font_index,
	short style,
	short justification,
	unsigned long flags,
	real_argb_color const *color);
void draw_string_set_highlight(
	short start,
	short end);
void draw_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	char const *string);
void draw_unicode_string(
	draw_character_proc draw_character,
	rectangle2d const *bounds,
	point2d *cursor_reference,
	rectangle2d const *clip,
	short height_adjust,
	wchar_t const *string);
void draw_string_compute_bounds(
	rectangle2d const *bounds,
	char const *string,
	rectangle2d *text_bounds,
	rectangle2d *cursor_bounds);
void draw_unicode_string_compute_bounds(
	rectangle2d const *bounds,
	wchar_t const *string,
	rectangle2d *text_bounds,
	rectangle2d *cursor_bounds);
short draw_string_pick(
	rectangle2d const *bounds,
	char const *string,
	point2d const *point);
void bitmap_draw_string(
	struct bitmap_data *bitmap,
	rectangle2d const *bounds,
	rectangle2d const *clip,
	char const *string);

/* ---------- globals */

/* ---------- public code */

#endif // __DRAW_STRING_H
