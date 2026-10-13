/*
RASTERIZER_XBOX_HARDWARE_GEOMETRY.C

symbols in this file:
00158410 0020:
	_IDirect3DDevice8_CreateVertexBuffer@24 (0000)
00158430 0020:
	_IDirect3DDevice8_CreateIndexBuffer@24 (0000)
00158450 0010:
	_code_00158450@8 (0000)
00158460 0010:
	_code_00158460@8 (0000)
00158470 0010:
	_D3DVertexBuffer_Unlock@4 (0000)
00158480 0010:
	_IDirect3DVertexBuffer8_Release@4 (0000)
00158490 0020:
	_IDirect3DVertexBuffer8_Lock@20 (0000)
001584B0 0010:
	_IDirect3DVertexBuffer8_Unlock@4 (0000)
001584C0 0010:
	_D3DIndexBuffer_Lock@20 (0000)
001584D0 0010:
	_D3DIndexBuffer_Unlock@4 (0000)
001584E0 0010:
	_IDirect3DIndexBuffer8_Release@4 (0000)
001584F0 0020:
	_IDirect3DIndexBuffer8_Lock@20 (0000)
00158510 0010:
	_IDirect3DIndexBuffer8_Unlock@4 (0000)
00158520 0170:
	_rasterizer_vertex_buffer_new (0000)
00158690 0030:
	_rasterizer_vertex_buffer_delete (0000)
001586C0 0160:
	_rasterizer_triangle_buffer_new (0000)
00158820 0030:
	_rasterizer_triangle_buffer_delete (0000)
00290F84 0039:
	??_C@_0DJ@PABNNDOA@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5verte@ (0000)
00290FC0 009a:
	??_C@_0JK@INEJEDKE@IDirect3DDevice8_CreateVertexBuf@ (0000)
0029105C 002c:
	??_C@_0CM@JPLBMMAK@vertex_size?$CKcount?$DN?$DNbuffer_size?5?$HM@ (0000)
00291088 0043:
	??_C@_0ED@PHIGAHC@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
002910D0 00a5:
	??_C@_0KF@HKLHKIPE@IDirect3DDevice8_CreateIndexBuff@ (0000)
00291178 003b:
	??_C@_0DL@GGBLFLCG@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5trian@ (0000)
002911B4 000a:
	??_C@_09PNPFALML@triangles?$AA@ (0000)
002911C0 0010:
	??_C@_0BA@LCGOFHFK@triangle_buffer?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_geometry.h"
#include <xtl.h>
#include "rasterizer/xbox/rasterizer_xbox.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void __stdcall code_00158450(
	void *resource,
	void *data)
{
	return;
}

void __stdcall code_00158460(
	void *resource,
	void *data)
{
	return;
}

boolean rasterizer_vertex_buffer_new(
	struct vertex_buffer *vertex_buffer,
	long vertex_type,
	long count,
	void const *vertices,
	long buffer_size)
{
	/* BUG: The January XDK CreateVertexBuffer failure returns without writing
	 * the output pointer (0x5de713..0x5de719); the original caller then tests
	 * that uninitialized output (January function +0xb3). This inherited
	 * failure-path bug is preserved; the Unlock restoration does not add it.
	 */
	D3DVertexBuffer *d3d_vertex_buffer;
	byte *locked_vertices;
	boolean success;
	short vertex_size;
	long result;

	success = TRUE;
	vertex_size = rasterizer_geometry_get_vertex_size(vertex_type);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c", 24, vertex_buffer);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c", 25, vertex_size*count==buffer_size || !vertices);

	if (!count)
		success = FALSE;
	if (!global_d3d_device)
		success = FALSE;
	else if (success)
	{
		result = IDirect3DDevice8_CreateVertexBuffer(
			global_d3d_device,
			buffer_size,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&d3d_vertex_buffer);
		if (result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"IDirect3DDevice8_CreateVertexBuffer(global_d3d_device, buffer_size, RASTERIZER_STATIC_BUFFER_USAGE, 0, RASTERIZER_STATIC_BUFFER_POOL, &d3d_vertex_buffer)");
		}
		if (!d3d_vertex_buffer)
			success = FALSE;
		if (!d3d_vertex_buffer || !success)
			d3d_vertex_buffer = NULL;
	}

	if (vertices && success)
	{
		rasterizer_globals.current_lock_operation = _rasterizer_lock_vertexbuffer_new;
		IDirect3DVertexBuffer8_Lock(
			d3d_vertex_buffer,
			0,
			buffer_size,
			&locked_vertices,
			0);
		rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

		if (!locked_vertices)
			success = FALSE;
		if (!success)
		{
			locked_vertices = NULL;
		}
		else
		{
			csmemcpy(locked_vertices, vertices, buffer_size);
			result = IDirect3DVertexBuffer8_Unlock(d3d_vertex_buffer);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DVertexBuffer8_Unlock(d3d_vertex_buffer)");
			}

			vertex_buffer->count = count;
			vertex_buffer->offset = 0;
			vertex_buffer->base_address = (void *)vertices;
			vertex_buffer->type = (short)vertex_type;
			vertex_buffer->hardware_format = d3d_vertex_buffer;
		}
	}

	if (!success)
	{
		csmemset(vertex_buffer, 0, sizeof(*vertex_buffer));
		error(_error_silent, "### ERROR failed to create vertex buffer hardware format");
	}

	return success;
}

void rasterizer_vertex_buffer_delete(
	struct vertex_buffer *vertex_buffer)
{
	if (vertex_buffer && vertex_buffer->hardware_format)
	{
		IDirect3DVertexBuffer8_Release(
			(D3DVertexBuffer *)vertex_buffer->hardware_format);
		vertex_buffer->hardware_format = 0;
	}

	return;
}

boolean rasterizer_triangle_buffer_new(
	struct triangle_buffer *triangle_buffer,
	short triangle_type,
	long count,
	void const *triangles)
{
	D3DIndexBuffer *d3d_index_buffer;
	byte *locked_triangles;
	boolean success;
	long buffer_size;
	long result;

	success = TRUE;
	buffer_size = 0;
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c", 115, triangle_buffer);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c", 116, triangles);
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c", 117, count>0);

	switch (triangle_type)
	{
	case 0:
		buffer_size = 6 * count;
		break;
	case 1:
		buffer_size = 2 * count + 4;
		break;
	default:
		display_assert(
			"### ERROR unsupported triangle buffer type",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox_hardware_geometry.c",
			128,
			TRUE);
		system_exit(-1);
		break;
	}

	if (!global_d3d_device)
	{
		success = FALSE;
		csmemset(triangle_buffer, 0, sizeof(*triangle_buffer));
		error(_error_silent, "### ERROR failed to create triangle buffer hardware format");
	}
	else
	{
		if (success)
		{
			result = IDirect3DDevice8_CreateIndexBuffer(
				global_d3d_device,
				buffer_size,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&d3d_index_buffer);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_CreateIndexBuffer(global_d3d_device, buffer_size, RASTERIZER_STATIC_BUFFER_USAGE, D3DFMT_INDEX16, RASTERIZER_STATIC_BUFFER_POOL, &d3d_index_buffer)");
			}
			/* BUG (preserved for exact matching): as in rasterizer_vertex_buffer_new, the
			 * January XDK CreateIndexBuffer failure path (0x5de5d3..0x5de5d8) returns without
			 * writing d3d_index_buffer, so after a failed call this test reads it unassigned.
			 * success is already FALSE on that path and the pointer is cleared below, so the
			 * result does not depend on the value. Source-policy approval pending (2026-09-27
			 * audit). */
			if (!d3d_index_buffer)
				success = FALSE;
			if (!success)
				d3d_index_buffer = NULL;
		}

		if (success)
		{
			IDirect3DIndexBuffer8_Lock(
				d3d_index_buffer,
				0,
				buffer_size,
				&locked_triangles,
				0);
			if (!locked_triangles)
				success = FALSE;
			if (!success)
				locked_triangles = NULL;
		}

		if (success)
		{
			csmemcpy(locked_triangles, triangles, buffer_size);
			result = IDirect3DIndexBuffer8_Unlock(d3d_index_buffer);
			if (result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DIndexBuffer8_Unlock(d3d_index_buffer)");
			}

			triangle_buffer->type = triangle_type;
			triangle_buffer->count = count;
			triangle_buffer->base_address = (void *)triangles;
			triangle_buffer->hardware_format = d3d_index_buffer;
		}
		else
		{
			csmemset(triangle_buffer, 0, sizeof(*triangle_buffer));
			error(_error_silent, "### ERROR failed to create triangle buffer hardware format");
		}
	}

	return success;
}

void rasterizer_triangle_buffer_delete(
	struct triangle_buffer *triangle_buffer)
{
	if (triangle_buffer && triangle_buffer->hardware_format)
	{
		IDirect3DIndexBuffer8_Release(
			(D3DIndexBuffer *)triangle_buffer->hardware_format);
		triangle_buffer->hardware_format = 0;
	}

	return;
}

/* ---------- private code */
