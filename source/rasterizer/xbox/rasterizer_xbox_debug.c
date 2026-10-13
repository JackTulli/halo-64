/*
RASTERIZER_XBOX_DEBUG.C
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/hud_draw.h"
#include "real_math.h"
/* The January object retains out-of-line copies of the D3D inline wrappers.
 * The stock XDK definition of D3DINLINE (static __forceinline) reproduces all
 * eight wrappers, including IDirect3DDevice8_SetRenderState's 0x220-byte body.
 * Do not replace them with handwritten Microsoft dispatchers or override the
 * XDK's inline policy: taking an address or weakening __forceinline changes
 * their emitted ABI and code shape.
 *
 * The emitted owners are D3DDevice_SetRenderState and the corresponding
 * IDirect3DDevice8 render-state, vertex-data, begin, and end wrappers.
 */
#include "rasterizer/rasterizer.h"
#include <xtl.h>

#include "rasterizer/xbox/rasterizer_xbox.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct rasterizer_xbox_debug_options_prefix
{
	byte reserved00[0x25];
	boolean debug_geometry;
	byte reserved26[0x2E];
	long zbias;
};

struct pixel_shader_definition
{
	unsigned long alpha_inputs[8];
	unsigned long final_combiner_inputs_abcd;
	unsigned long final_combiner_inputs_efg;
	unsigned long constant_0[8];
	unsigned long constant_1[8];
	unsigned long alpha_outputs[8];
	unsigned long rgb_inputs[8];
	unsigned long compare_mode;
	unsigned long final_combiner_constant_0;
	unsigned long final_combiner_constant_1;
	unsigned long rgb_outputs[8];
	unsigned long combiner_count;
	unsigned long texture_modes;
	unsigned long dot_mapping;
	unsigned long input_texture;
	unsigned long c0_mapping;
	unsigned long c1_mapping;
	unsigned long final_combiner_constants;
};

/* ---------- prototypes */

/* ---------- globals */

extern struct rasterizer_xbox_debug_options_prefix rasterizer_debug_options;
extern struct rasterizer_window_begin_parameters global_window_parameters;

/* ---------- public code */

void rasterizer_debug_drawing_begin(
	boolean opaque)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		19,
		global_d3d_device);

	if (rasterizer_debug_options.debug_geometry)
	{
		rasterizer_set_vertex_shader_permutation(0, 9, 0);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			D3DCULL_NONE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZFUNC,
			D3DCMP_LESSEQUAL);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZENABLE,
			D3DZB_TRUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZBIAS,
			rasterizer_debug_options.zbias);

		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 0;
		pixel_shader.combiner_count = 1;
		pixel_shader.final_combiner_inputs_abcd = 4;

		if (opaque)
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHABLENDENABLE,
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHATESTENABLE,
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				TRUE);
			rasterizer_set_pixel_shader(&pixel_shader);
		}
		else
		{
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHABLENDENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ALPHATESTENABLE,
				FALSE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_SRCBLEND,
				D3DBLEND_SRCALPHA);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_DESTBLEND,
				D3DBLEND_INVSRCALPHA);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_BLENDOP,
				D3DBLENDOP_ADD);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_ZWRITEENABLE,
				FALSE);
			pixel_shader.final_combiner_inputs_efg = 0x1400;
			rasterizer_set_pixel_shader(&pixel_shader);
		}
	}

	return;
}

void rasterizer_debug_drawing_end(
	void)
{
	return;
}

void _rasterizer_debug_immediate_begin(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		81,
		global_d3d_device);

	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_CULLMODE,
		D3DCULL_NONE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ALPHABLENDENABLE,
		FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ALPHATESTENABLE,
		FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZFUNC,
		D3DCMP_LESSEQUAL);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZENABLE,
		D3DZB_TRUE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZWRITEENABLE,
		TRUE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZBIAS,
		rasterizer_debug_options.zbias);

	rasterizer_set_vertex_shader_permutation(0, 9, 0);

	csmemset(&pixel_shader, 0, sizeof(pixel_shader));
	pixel_shader.texture_modes = 0;
	pixel_shader.combiner_count = 1;
	pixel_shader.final_combiner_inputs_abcd = 4;
	rasterizer_set_pixel_shader(&pixel_shader);

	return;
}

void _rasterizer_debug_immediate_end(
	void)
{
	return;
}

void _rasterizer_debug_immediate_line(
	real_point3d const *p0,
	real_point3d const *p1,
	real_rgb_color const *color0,
	real_rgb_color const *color1)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		116,
		p0 && p1 && color0);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		117,
		global_d3d_device);

	IDirect3DDevice8_Begin(
		global_d3d_device,
		D3DPT_LINELIST);
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		9,
		color0->red,
		color0->green,
		color0->blue,
		1.0f);
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		0,
		p0->x,
		p0->y,
		p0->z,
		1.0f);
	if (color1)
	{
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			9,
			color1->red,
			color1->green,
			color1->blue,
			1.0f);
	}
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		0,
		p1->x,
		p1->y,
		p1->z,
		1.0f);
	IDirect3DDevice8_End(
		global_d3d_device);

	return;
}

void _rasterizer_debug_immediate_triangle(
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2,
	real_rgb_color const *color0,
	real_rgb_color const *color1,
	real_rgb_color const *color2)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		139,
		p0 && p1 && p2 && color0);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		140,
		global_d3d_device);

	IDirect3DDevice8_Begin(
		global_d3d_device,
		D3DPT_TRIANGLELIST);
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		9,
		color0->red,
		color0->green,
		color0->blue,
		1.0f);
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		0,
		p0->x,
		p0->y,
		p0->z,
		1.0f);
	if (color1)
	{
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			9,
			color1->red,
			color1->green,
			color1->blue,
			1.0f);
	}
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		0,
		p1->x,
		p1->y,
		p1->z,
		1.0f);
	if (color2)
	{
		IDirect3DDevice8_SetVertexData4f(
			global_d3d_device,
			9,
			color2->red,
			color2->green,
			color2->blue,
			1.0f);
	}
	IDirect3DDevice8_SetVertexData4f(
		global_d3d_device,
		0,
		p2->x,
		p2->y,
		p2->z,
		1.0f);
	IDirect3DDevice8_End(
		global_d3d_device);

	return;
}

void _rasterizer_debug_immediate_begin_screenspace(
	void)
{
	real vertex_constants[5][4];
	short window_height;
	short window_width;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		167,
		global_d3d_device);

	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_CULLMODE,
		D3DCULL_NONE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ALPHABLENDENABLE,
		FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ALPHATESTENABLE,
		FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZENABLE,
		D3DZB_FALSE);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_ZBIAS,
		0);

	rasterizer_set_vertex_shader_permutation(4, 8, 0);

	window_width = global_window_parameters.camera.viewport_bounds.x1 -
		global_window_parameters.camera.viewport_bounds.x0;
	window_height = global_window_parameters.camera.viewport_bounds.y1 -
		global_window_parameters.camera.viewport_bounds.y0;

	vertex_constants[0][0] = 2.0f / window_width;
	vertex_constants[0][1] = 0.0f;
	vertex_constants[0][2] = 0.0f;
	vertex_constants[0][3] = -1.0f - 1.0f / window_width;
	vertex_constants[1][0] = 0.0f;
	vertex_constants[1][1] = -2.0f / window_height;
	vertex_constants[1][2] = 0.0f;
	vertex_constants[1][3] = 1.0f / window_height + 1.0f;
	vertex_constants[2][0] = 0.0f;
	vertex_constants[2][1] = 0.0f;
	vertex_constants[2][2] = 0.0f;
	vertex_constants[2][3] = 0.5f;
	vertex_constants[3][0] = 0.0f;
	vertex_constants[3][1] = 0.0f;
	vertex_constants[3][2] = 0.0f;
	vertex_constants[3][3] = 1.0f;
	vertex_constants[4][0] = 1.0f;
	vertex_constants[4][1] = 1.0f;
	vertex_constants[4][2] = 0.0f;
	vertex_constants[4][3] = 1.0f;

	IDirect3DDevice8_SetVertexShaderConstant(
		global_d3d_device,
		-68,
		vertex_constants,
		5);

	csmemset(&pixel_shader, 0, sizeof(pixel_shader));
	pixel_shader.texture_modes = 0;
	pixel_shader.combiner_count = 1;
	pixel_shader.final_combiner_inputs_abcd = 4;
	rasterizer_set_pixel_shader(&pixel_shader);

	return;
}

void _rasterizer_debug_immediate_end_screenspace(
	void)
{
	return;
}

void _rasterizer_debug_immediate_line_screenspace(
	point2d const *p0,
	point2d const *p1,
	real_rgb_color const *color0,
	real_rgb_color const *color1)
{
	pixel32 pixel0;
	pixel32 pixel1;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		221,
		p0 && p1 && color0);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		222,
		global_d3d_device);

	pixel0 = real_rgb_color_to_pixel32(color0);
	if (color1)
		pixel1 = real_rgb_color_to_pixel32(color1);

	IDirect3DDevice8_Begin(
		global_d3d_device,
		D3DPT_LINESTRIP);
	IDirect3DDevice8_SetVertexDataColor(
		global_d3d_device,
		9,
		pixel0);
	IDirect3DDevice8_SetVertexData2s(
		global_d3d_device,
		0,
		p0->x,
		p0->y);
	if (color1)
	{
		IDirect3DDevice8_SetVertexDataColor(
			global_d3d_device,
			9,
			pixel1);
	}
	IDirect3DDevice8_SetVertexData2s(
		global_d3d_device,
		0,
		p1->x,
		p1->y);
	IDirect3DDevice8_End(
		global_d3d_device);

	return;
}

void _rasterizer_debug_immediate_linestrip_screenspace(
	point2d const *points,
	short point_count,
	real_rgb_color const *color)
{
	boolean success;
	pixel32 pixel;
	short point_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		249,
		points && color && point_count>1);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_debug.c",
		250,
		global_d3d_device);

	pixel = real_rgb_color_to_pixel32(color);

	IDirect3DDevice8_Begin(
		global_d3d_device,
		D3DPT_LINESTRIP);
	IDirect3DDevice8_SetVertexDataColor(
		global_d3d_device,
		9,
		pixel);
	success = TRUE;

	for (point_index = 0; point_index < point_count; point_index++)
	{
		if (IDirect3DDevice8_SetVertexData2s(
			global_d3d_device,
			0,
			points[point_index].x,
			points[point_index].y) >= 0 && success)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				0,
				"IDirect3DDevice8_SetVertexData2s(global_d3d_device, VSDE_VERTEX, points[point_index].x, points[point_index].y)");
		}
	}

	if (IDirect3DDevice8_End(
		global_d3d_device) >= 0 && success)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			0,
			"IDirect3DDevice8_End(global_d3d_device)");
	}

	return;
}

/* ---------- private code */
