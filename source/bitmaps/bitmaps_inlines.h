/*
BITMAPS_INLINES.H

file has inline function assertions.
*/

#ifndef __BITMAPS_INLINES_H
#define __BITMAPS_INLINES_H

/* (long)rint: on the N64 one conversion instruction (cseries.h), not a
library call per channel */
#ifdef HALO_N64
#define BITMAPS_RINT_LONG(x) n64_round_double_to_long(x)
#else
#define BITMAPS_RINT_LONG(x) ((long)__builtin_rint(x))
#endif
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "bitmaps/bitmaps.h"
#include "math/integer_math.h"
#include "math/real_math.h"
#include "effects/particles.h"

/* ---------- constants */

/* ---------- macros */

#define match_assert_valid_real_rgb_color(file, line, rgb) \
	match_vassert( \
		file, \
		line, \
		valid_real_rgb_color(rgb), \
		csprintf( \
			temporary, \
			"%s: assert_valid_real_rgb_color(%f, %f, %f)", \
			#rgb, \
			(*rgb).red, \
			(*rgb).green, \
			(*rgb).blue))

/* ---------- structures */

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

__inline pixel32 real_argb_color_to_pixel32(
	real_argb_color const *color)
{
	real scale = 255.0f;
	pixel32 result;

	match_vassert(
		"..\\bitmaps\\bitmaps_inlines.h",
		89,
		valid_real_argb_color(color),
		csprintf(
			temporary,
			"%s: assert_valid_real_argb_color(%f, %f, %f, %f)",
			"color",
			color->alpha,
			color->red,
			color->green,
			color->blue));

	{
		long alpha;
		long red;
		long green;
		long blue;

#ifdef HALO_LINUX
		result = (pixel32)(
			BITMAPS_RINT_LONG((double)color->blue * scale) |
			(BITMAPS_RINT_LONG((double)color->green * scale) << 8) |
			(BITMAPS_RINT_LONG((double)color->red * scale) << 16) |
			(BITMAPS_RINT_LONG((double)color->alpha * scale) << 24));
#else
		__asm
		{
			mov		edx, color
			fld		dword ptr [edx]
			fld		dword ptr [edx+4]
			fld		dword ptr [edx+8]
			fld		dword ptr [edx+12]
			fld		scale
			fmul	st(4), st
			fmul	st(3), st
			fmul	st(2), st
			fmulp	st(1), st
			fistp	blue
			fistp	green
			fistp	red
			fistp	alpha
			mov		edx, blue
			mov		ebx, green
			mov		ecx, red
			mov		eax, alpha
			shl		ebx, 8
			shl		ecx, 16
			shl		eax, 24
			or		edx, ebx
			or		edx, ecx
			or		edx, eax
			mov		result, edx
		}
#endif
	}

	{
		pixel32 verify;

#ifdef HALO_LINUX
		verify = (pixel32)(
			(BITMAPS_RINT_LONG((double)color->blue * scale) & 0xff) |
			((BITMAPS_RINT_LONG((double)color->green * scale) & 0xff) << 8) |
			((BITMAPS_RINT_LONG((double)color->red * scale) & 0xff) << 16) |
			(BITMAPS_RINT_LONG((double)color->alpha * scale) << 24));
#else
		__asm
		{
			mov		edx, color
			fld		dword ptr [edx]
			fld		dword ptr [edx+4]
			fld		dword ptr [edx+8]
			fld		dword ptr [edx+12]
			fld		scale
			fmul	st(4), st
			fmul	st(3), st
			fmul	st(2), st
			fmulp	st(1), st
			fistp	verify
			and		verify, 0FFh
			mov		edx, verify
			fistp	verify
			and		verify, 0FFh
			shl		verify, 8
			or		edx, verify
			fistp	verify
			and		verify, 0FFh
			shl		verify, 16
			or		edx, verify
			fistp	verify
			shl		verify, 24
			or		edx, verify
			mov		verify, edx
		}
#endif

		match_assert(
			"..\\bitmaps\\bitmaps_inlines.h",
			188,
			verify == result);
	}

	return result;
}

__inline pixel32 real_rgb_color_to_pixel32(
	real_rgb_color const *color)
{
	pixel32 result;
	real scale = (real)UNSIGNED_CHAR_MAX;

	match_assert_valid_real_rgb_color("..\\bitmaps\\bitmaps_inlines.h", 0xC9, color);

#ifdef HALO_LINUX
	result = (pixel32)(
		(BITMAPS_RINT_LONG((double)color->blue * scale) & 0xff) |
		((BITMAPS_RINT_LONG((double)color->green * scale) & 0xff) << 8) |
		((BITMAPS_RINT_LONG((double)color->red * scale) & 0xff) << 16));
#else
	__asm
	{
		mov edx, color
		fld dword ptr [edx]
		fld dword ptr [edx+4]
		fld dword ptr [edx+8]
		fld scale
		fmul st(3), st(0)
		fmul st(2), st(0)
		fmulp st(1), st(0)
		fistp result
		and result, 0ffh
		mov edx, result
		fistp result
		and result, 0ffh
		shl result, 8
		or edx, result
		fistp result
		and result, 0ffh
		shl result, 16
		or edx, result
		mov result, edx
	}
#endif

	return result;
}

/* January retains an out-of-line copy of this inline in
 * rasterizer_xbox_active_camouflage.obj. Its assertion records this header
 * and line 291. The stack-local scale, 32-bit FISTP, and in-memory shift are
 * the characteristic packet of the original small x87 helper. */
__inline pixel32 real_alpha_to_pixel32(
	real alpha)
{
	real scale = 255.0f;
	pixel32 result;

	match_assert(
		"..\\bitmaps\\bitmaps_inlines.h",
		291,
		alpha>=0.0f && alpha<=1.0f);

#ifdef HALO_LINUX
	result = (pixel32)(BITMAPS_RINT_LONG((double)alpha * scale) << 24);
#else
	__asm
	{
		fld		alpha
		fld		scale
		fmulp	st(1), st
		fistp	result
		shl		result, 24
	}
#endif

	return result;
}
#endif // __BITMAPS_INLINES_H
