/*
RASTERIZER_XBOX_VERTEX_SHADERS.H
*/

#ifndef __RASTERIZER_XBOX_VERTEX_SHADERS_H
#define __RASTERIZER_XBOX_VERTEX_SHADERS_H
#pragma once

/* ---------- constants */

enum
{
	NUMBER_OF_VERTEX_SHADERS = 67
};

/* ---------- structures */

struct vertex_shader_entry
{
	void const *declaration;
	void const *code;
	unsigned long handle;
	long instruction_count;
};

/* ---------- prototypes/RASTERIZER_XBOX_VERTEX_SHADERS_INITIALIZE.C */

boolean rasterizer_vertex_shaders_initialize(
	void);
void rasterizer_vertex_shaders_dispose(
	void);

/* ---------- globals */

extern struct vertex_shader_entry vertex_shader_table[NUMBER_OF_VERTEX_SHADERS];

#endif // __RASTERIZER_XBOX_VERTEX_SHADERS_H
