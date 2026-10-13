/*
RASTERIZER_XBOX_VERTEX_SHADERS_RUNTIME.C

symbols in this file:
00168350 0010:
	_code_00168350 (0000)
00168360 0010:
	_code_00168360 (0000)
00168370 0010:
	_code_00168370 (0000)
00168380 0010:
	_code_00168380 (0000)
00168390 0560:
	_rasterizer_set_vertex_shader_permutation (0000)
0029C2F8 0528:
	_rdata_0029c2f8 (0000)
0029C820 0043:
	??_C@_0ED@PALLOMFH@IDirect3DDevice8_SetVertexShader@ (0000)
0029C868 0054:
	??_C@_0FE@NPNOIOBA@IDirect3DDevice8_SelectVertexSha@ (0000)
0029C8C0 0052:
	??_C@_0FC@JAKGKFDP@IDirect3DDevice8_LoadVertexShade@ (0000)
0029C918 0049:
	??_C@_0EJ@GJBDGHE@IDirect3DDevice8_SelectVertexSha@ (0000)
0029C964 0026:
	??_C@_0CG@FJEBEMLC@?$CD?$CD?$CD?5ERROR?5vertex?5shader?5was?5not?5@ (0000)
0029C98C 0038:
	??_C@_0DI@ENBGJMAN@?$CD?$CD?$CD?5ERROR?5packed?5vertex?5shaders?5@ (0000)
0029C9C8 0094:
	??_C@_0JE@FLCOGNAM@IDirect3DDevice8_GetVertexShader@ (0000)
0029CA60 004b:
	??_C@_0EL@IDNOJKOP@translation_table?$FLvertex_type?$CKpe@ (0000)
0029CAAC 003c:
	??_C@_0DM@HGGNMEDI@permutation_index?$DO?$DN0?5?$CG?$CG?5permutat@ (0000)
0029CAE8 0024:
	??_C@_0CE@IOILLCAH@?$CD?$CD?$CD?5ERROR?5unsupported?5vertex?5sha@ (0000)
0029CB10 0047:
	??_C@_0EH@NIDAECGF@vertex_shader_index?$DO?$DN0?5?$CG?$CG?5vertex@ (0000)
0029CB58 0048:
	??_C@_0EI@JLPCJIEP@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0030D3B8 001c:
	_data_0030d3b8 (0000)
*/

/* ---------- headers */

#include "rasterizer/rasterizer_frame_statistics.h"
#include "cseries.h"
#include "cseries/errors.h"
#include "rasterizer_xbox_vertex_shaders.h"
#include <xtl.h>

/* ---------- constants */

enum
{
	NUMBER_OF_RASTERIZER_VERTEX_TYPES = 12,
};

/* ---------- macros */

/* ---------- structures */

struct packed_vertex_shader
{
	short vertex_shader_index;
	boolean loaded;
	byte pad03;
	unsigned long offset;
};

struct rasterizer_vertex_shader_runtime_globals
{
	short current_vertex_shader_index;
	short pad02;
	struct packed_vertex_shader packed_shaders[3];
};

struct rasterizer_vertex_shader_debug_options
{
	byte reserved00[2];
	short vertex_shader_statistics;
};

/* ---------- prototypes */

void rasterizer_error(
	long error_result,
	char const *format,
	...);

/* ---------- globals */

short const vertex_shader_translation_tables[] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 4, 3, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, 38, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	65, 2, 12, 66, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, 56, -1, -1, -1, -1, -1,
	16, 16, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	49, 49, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	29, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	40, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	21, 41, 59, 21, 41, 59, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	58, 58, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	26, 42, 44, 26, 42, 44, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	51, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	6, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	8, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	37, 7, 54, 55, 53, 52, 37, 7, 54, 55, 53, 52,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 17, 10, 9, 27, 17, 10, 9, 27,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 64, 64, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 39, 39, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 13, 13, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 5, 5, -1, -1, -1, -1, -1, -1,
	24, 48, 34, 19, 35, -1, 24, 48, 34, 19, 35, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	47, 60, 57, 45, 62, 31, 47, 60, 57, 45, 62, 31,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	20, 20, -1, -1, 18, 18, -1, -1, -1, -1, -1, -1,
	23, 23, -1, -1, 14, 14, -1, -1, -1, -1, -1, -1,
	46, 46, -1, -1, 30, 30, -1, -1, -1, -1, -1, -1,
	28, 43, 61, 28, 43, 61, -1, -1, -1, -1, -1, -1,
	63, 36, -1, 63, 36, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	25, 25, -1, -1, 50, 50, -1, -1, -1, -1, -1, -1,
	22, 22, -1, -1, 32, 32, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 15, 15, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, 11,
};

struct rasterizer_vertex_shader_runtime_globals vertex_shader_runtime_globals =
{
	NONE,
	0,
	{
		{ 10, FALSE, 0, 0 },
		{ 9, FALSE, 0, 0 },
		{ 27, FALSE, 0, 0 },
	},
};

extern struct rasterizer_vertex_shader_debug_options rasterizer_debug_options;
extern void *global_d3d_device;

/* ---------- public code */

void rasterizer_set_vertex_shader_permutation(
	short vertex_shader_index,
	short vertex_type,
	short permutation_index)
{
	boolean success;
	short const *translation_table;
	short permutation_count;
	short packed_shader_index;
	long offset;
	unsigned long handle;

	success = TRUE;
	translation_table = NULL;
	permutation_count = 1;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
		135,
		vertex_shader_index>=0 && vertex_shader_index<NUMBER_OF_VERTEX_SHADERS);

	switch (vertex_shader_index)
	{
	case 0: translation_table = vertex_shader_translation_tables + 0x000 / sizeof(short); break;
	case 1: translation_table = vertex_shader_translation_tables + 0x018 / sizeof(short); break;
	case 4: permutation_count = 2; translation_table = vertex_shader_translation_tables + 0x030 / sizeof(short); break;
	case 38: translation_table = vertex_shader_translation_tables + 0x060 / sizeof(short); break;
	case 65: translation_table = vertex_shader_translation_tables + 0x078 / sizeof(short); permutation_count = 4; break;
	case 56: translation_table = vertex_shader_translation_tables + 0x0D8 / sizeof(short); break;
	case 16: translation_table = vertex_shader_translation_tables + 0x0F0 / sizeof(short); break;
	case 49: translation_table = vertex_shader_translation_tables + 0x108 / sizeof(short); break;
	case 29: translation_table = vertex_shader_translation_tables + 0x120 / sizeof(short); break;
	case 40: translation_table = vertex_shader_translation_tables + 0x138 / sizeof(short); break;
	case 21: translation_table = vertex_shader_translation_tables + 0x150 / sizeof(short); permutation_count = 3; break;
	case 58: translation_table = vertex_shader_translation_tables + 0x198 / sizeof(short); break;
	case 42: translation_table = vertex_shader_translation_tables + 0x1B0 / sizeof(short); permutation_count = 3; break;
	case 51: translation_table = vertex_shader_translation_tables + 0x1F8 / sizeof(short); break;
	case 6: translation_table = vertex_shader_translation_tables + 0x210 / sizeof(short); break;
	case 8: translation_table = vertex_shader_translation_tables + 0x228 / sizeof(short); break;
	case 37: translation_table = vertex_shader_translation_tables + 0x240 / sizeof(short); permutation_count = 6; break;
	case 10: translation_table = vertex_shader_translation_tables + 0x2D0 / sizeof(short); permutation_count = 4; break;
	case 64: translation_table = vertex_shader_translation_tables + 0x330 / sizeof(short); break;
	case 39: translation_table = vertex_shader_translation_tables + 0x348 / sizeof(short); break;
	case 13: translation_table = vertex_shader_translation_tables + 0x360 / sizeof(short); break;
	case 5: translation_table = vertex_shader_translation_tables + 0x378 / sizeof(short); break;
	case 24: translation_table = vertex_shader_translation_tables + 0x390 / sizeof(short); permutation_count = 6; break;
	case 20: translation_table = vertex_shader_translation_tables + 0x420 / sizeof(short); break;
	case 23: translation_table = vertex_shader_translation_tables + 0x438 / sizeof(short); break;
	case 46: translation_table = vertex_shader_translation_tables + 0x450 / sizeof(short); break;
	case 43: translation_table = vertex_shader_translation_tables + 0x468 / sizeof(short); permutation_count = 3; break;
	case 25: translation_table = vertex_shader_translation_tables + 0x4B0 / sizeof(short); break;
	case 22: translation_table = vertex_shader_translation_tables + 0x4C8 / sizeof(short); break;
	case 15: translation_table = vertex_shader_translation_tables + 0x4E0 / sizeof(short); break;
	case 33: translation_table = vertex_shader_translation_tables + 0x4F8 / sizeof(short); permutation_count = 2; break;
	default:
		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
			876,
			FALSE,
			"### ERROR unsupported vertex shader");
	}

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
		879,
		vertex_type>=0 && vertex_type<NUMBER_OF_RASTERIZER_VERTEX_TYPES);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
		880,
		permutation_index>=0 && permutation_index<permutation_count);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
		881,
		translation_table[vertex_type*permutation_count + permutation_index]!=NONE);

	vertex_shader_index =
		translation_table[vertex_type * permutation_count + permutation_index];
	if (vertex_shader_index != vertex_shader_runtime_globals.current_vertex_shader_index)
	{
		if (vertex_shader_runtime_globals.packed_shaders[1].offset == 0)
		{
			offset = 0;
			for (packed_shader_index = 0; packed_shader_index < 3; packed_shader_index++)
			{
				long size;

				if (IDirect3DDevice8_GetVertexShaderSize(
					global_d3d_device,
					vertex_shader_table[vertex_shader_runtime_globals.packed_shaders[packed_shader_index].vertex_shader_index].handle,
					&size) >= 0 && success)
					success = TRUE;
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_GetVertexShaderSize(global_d3d_device, vertex_shader_table[packed_shaders[packed_shader_index].vertex_shader_index].handle, &size)");
				}
				vertex_shader_runtime_globals.packed_shaders[packed_shader_index].offset = offset;
				offset += size;
			}
			match_vassert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
				930,
				offset <= 136,
				"### ERROR packed vertex shaders don't fit in GPU memory");
		}

		for (packed_shader_index = 0; packed_shader_index < 3; packed_shader_index++)
		{
			if (vertex_shader_index ==
				vertex_shader_runtime_globals.packed_shaders[packed_shader_index].vertex_shader_index)
				break;
		}

		if (packed_shader_index < 3)
		{
			if (vertex_shader_runtime_globals.packed_shaders[packed_shader_index].loaded)
			{
				if (IDirect3DDevice8_SelectVertexShader(
					global_d3d_device,
					0,
					vertex_shader_runtime_globals.packed_shaders[packed_shader_index].offset) >= 0 && success)
				{
					success = TRUE;
					goto set_current_vertex_shader;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SelectVertexShader(global_d3d_device, 0L, (UINT)offset)");
					goto set_current_vertex_shader;
				}
			}
			else
			{
				offset = vertex_shader_runtime_globals.packed_shaders[packed_shader_index].offset;
				handle =
					vertex_shader_table[
						vertex_shader_runtime_globals.packed_shaders[packed_shader_index].vertex_shader_index].handle;
				match_vassert(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
					956,
					handle != 0,
					"### ERROR vertex shader was not valid");
				if (IDirect3DDevice8_LoadVertexShader(global_d3d_device, handle, offset) >= 0 && success)
					success = TRUE;
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_LoadVertexShader(global_d3d_device, (DWORD)handle, (UINT)offset)");
				}
				if (IDirect3DDevice8_SelectVertexShader(global_d3d_device, handle, offset) >= 0 && success)
				{
					success = TRUE;
					vertex_shader_runtime_globals.packed_shaders[packed_shader_index].loaded = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						0,
						"IDirect3DDevice8_SelectVertexShader(global_d3d_device, (DWORD)handle, (UINT)offset)");
					vertex_shader_runtime_globals.packed_shaders[packed_shader_index].loaded = TRUE;
				}
			}
		}
		else
		{
			handle = vertex_shader_table[vertex_shader_index].handle;
			match_vassert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_vertex_shaders_runtime.c",
				969,
				handle != 0,
				"### ERROR vertex shader was not valid");
			if (IDirect3DDevice8_SetVertexShader(global_d3d_device, handle) >= 0 && success)
				success = TRUE;
			else
			{
				success = FALSE;
				rasterizer_error(
					0,
					"IDirect3DDevice8_SetVertexShader(global_d3d_device, (DWORD)handle)");
			}

			for (packed_shader_index = 0; packed_shader_index < 3; packed_shader_index++)
				vertex_shader_runtime_globals.packed_shaders[packed_shader_index].loaded = FALSE;
		}

		if (rasterizer_debug_options.vertex_shader_statistics)
			rasterizer_frame_statistics.vertex_shader_instruction_count +=
				vertex_shader_table[vertex_shader_index].instruction_count;

	set_current_vertex_shader:
		vertex_shader_runtime_globals.current_vertex_shader_index = vertex_shader_index;
		if (!success)
			error(2, "### ERROR rasterizer_set_vertex_shader failed");
	}

	return;
}

/* ---------- private code */
