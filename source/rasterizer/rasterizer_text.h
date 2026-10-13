/*
RASTERIZER_TEXT.H
*/

#ifndef __RASTERIZER_TEXT_H
#define __RASTERIZER_TEXT_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct bitmap_data;
struct dynamic_screen_vertex;
struct font_character;
struct font_header;
struct parse_string_state;
struct rasterizer_dynamic_screen_geometry_parameters;

/* ---------- prototypes/RASTERIZER_TEXT.C */

void lock_rasterizer_text_data(
	void);
void unlock_rasterizer_text_data(
	void);
boolean rasterizer_text_cache_initialize(
	void);
struct bitmap_data *hardware_character_cache_get_bitmap(
	void);
void rasterizer_text_draw_character(
	struct dynamic_screen_vertex const *vertices);
void rasterizer_draw_character(
	struct parse_string_state *state,
	struct font_header *font,
	struct font_character *font_character,
	unsigned long color,
	short x0,
	short y0,
	short x,
	short y,
	short dx,
	short dy);
void rasterizer_text_begin(
	struct rasterizer_dynamic_screen_geometry_parameters const *parameters);
void rasterizer_text_end(
	void);

#endif // __RASTERIZER_TEXT_H
