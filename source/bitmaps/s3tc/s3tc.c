/*
S3TC.C

symbols in this file:
0005EB30 0060:
	_ColorToFcolor (0000)
0005EB90 0050:
	_FcolorToColor (0000)
0005EBE0 0040:
	_ColorToRGB (0000)
0005EC20 0060:
	_RGBToColor (0000)
0005EC80 00a0:
	_Square3x3 (0000)
0005ED20 0210:
	_Quantize (0000)
0005EF30 0140:
	_ClipExtrema (0000)
0005F070 0170:
	_AllSame (0000)
0005F1E0 0890:
	_EncodeBlockRGBColorKey (0000)
0005FA70 01c0:
	_DecodeBlockRGB (0000)
0005FC30 01f0:
	_DecodeBlockRGB__single_pixel (0000)
0005FE20 0090:
	_DecodeBlockAlpha4 (0000)
0005FEB0 0050:
	_DecodeBlockAlpha4__single_pixel (0000)
0005FF00 0260:
	_DecodeBlockAlpha3 (0000)
00060160 01b0:
	_DecodeBlockAlpha3__single_pixel (0000)
00060310 0020:
	_EncodeBlockRGB (0000)
00060330 0070:
	_EncodeBlockAlpha4 (0000)
000603A0 0270:
	_EncodeBlockAlpha3 (0000)
002510C8 0004:
	__real@3b808081 (0000)
002510CC 0004:
	__real@c7c34f80 (0000)
002510D0 000d:
	??_C@_0N@NBFAEMLN@v?$DO?$DN0?5?$CG?$CG?5v?$DM?$DN4?$AA@ (0000)
002510E0 000d:
	??_C@_0N@CLDKFKKD@u?$DO?$DN0?5?$CG?$CG?5u?$DM?$DN4?$AA@ (0000)
002510F0 0023:
	??_C@_0CD@CADFKEEP@c?3?2halo?2SOURCE?2bitmaps?2s3tc?2s3tc@ (0000)
002DB64C 002c:
	_wtPrimary (0000)
	_mapRGB4 (000c)
	_mapRGB3 (001c)
*/

/* ---------- headers */

#include "bitmaps/s3tc/s3tc.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct s3tc_fcolor
{
	real rgba[4];
};

/* ---------- prototypes */

static void ColorToFcolor(
	struct s3tc_color *pcolor,
	struct s3tc_fcolor *pfcolor);
static void FcolorToColor(
	struct s3tc_fcolor *pfcolor,
	struct s3tc_color *pcolor);
static void ColorToRGB(
	struct s3tc_color *pcolor,
	word *prgb);
static void RGBToColor(
	word const *prgb,
	struct s3tc_color *pcolor);
static void Square3x3(
	real m[3][3],
	real m2[3][3]);
static void Quantize(
	struct s3tc_fcolor *pfcolor0,
	struct s3tc_fcolor *pfcolor1,
	struct s3tc_block_rgb *pblock,
	long cOpaque);
static void ClipExtrema(
	struct s3tc_fcolor *plower,
	struct s3tc_fcolor *pupper);
static void AllSame(
	struct s3tc_color *pcolor,
	struct s3tc_block_rgb *pblock,
	word wAlpha);

/* ---------- globals */

/* luminance weights, blue/green/red channel order */
static real wtPrimary[3] = { 0.082f, 0.6094f, 0.3086f };

/* line-position index to block color index, four- and three-color blocks */
static long mapRGB4[4] = { 0, 2, 3, 1 };
static long mapRGB3[4] = { 0, 2, 1, 3 };

/* ---------- private code */

static void ColorToFcolor(
	struct s3tc_color *pcolor,
	struct s3tc_fcolor *pfcolor)
{
	real value;
	long channel;

	for (channel = 0; channel < 3; ++channel)
	{
		value = pcolor->rgba[channel];
		pfcolor->rgba[channel] = value * wtPrimary[channel] * (1.0f / 255.0f);
	}

	return;
}

static void FcolorToColor(
	struct s3tc_fcolor *pfcolor,
	struct s3tc_color *pcolor)
{
	pcolor->rgba[0] = (byte)(pfcolor->rgba[0] / wtPrimary[0] * 255.0f);
	pcolor->rgba[1] = (byte)(pfcolor->rgba[1] / wtPrimary[1] * 255.0f);
	pcolor->rgba[2] = (byte)(pfcolor->rgba[2] / wtPrimary[2] * 255.0f);

	return;
}

static void ColorToRGB(
	struct s3tc_color *pcolor,
	word *prgb)
{
	word rgb = pcolor->rgba[2] >> 3;
	rgb <<= 6;
	rgb |= pcolor->rgba[1] >> 2;
	rgb <<= 5;
	rgb |= pcolor->rgba[0] >> 3;
	*prgb = rgb;

	return;
}

static void RGBToColor(
	word const *prgb,
	struct s3tc_color *pcolor)
{
	word rgb = *prgb;
	/* color.rgba[S3TC_ALPHA] is deliberately left unwritten, as January leaves
	   it: January stores only three bytes of this local and copies all four out.
	   Both callers, DecodeBlockRGB and DecodeBlockRGB__single_pixel, assign the
	   alpha of every colour on the line immediately after calling this, so the
	   indeterminate byte is never read. Initialising it - either with = { 0 } or
	   with an explicit store before or after the channels - returns both this
	   function and DecodeBlockRGB to residual. */
	struct s3tc_color color;

	color.rgba[0] = (byte)rgb;
	rgb >>= 5;
	color.rgba[1] = (byte)rgb;
	rgb >>= 6;
	color.rgba[2] = (byte)rgb;
	color.rgba[0] <<= 3;
	color.rgba[1] <<= 2;
	color.rgba[2] <<= 3;
	color.rgba[0] |= color.rgba[0] >> 5;
	color.rgba[1] |= color.rgba[1] >> 6;
	color.rgba[2] |= color.rgba[2] >> 5;
	*pcolor = color;

	return;
}

static void Square3x3(
	real m[3][3],
	real m2[3][3])
{
	m2[0][0] = m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2];
	m2[0][1] = (m[1][1] + m[0][0]) * m[0][1] + m[1][2] * m[0][2];
	m2[0][2] = (m[2][2] + m[0][0]) * m[0][2] + m[1][2] * m[0][1];
	m2[1][1] = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[1][2] * m[1][2];
	m2[1][2] = (m[2][2] + m[1][1]) * m[1][2] + m[0][2] * m[0][1];
	m2[2][2] = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];

	return;
}

static void Quantize(
	struct s3tc_fcolor *pfcolor0,
	struct s3tc_fcolor *pfcolor1,
	struct s3tc_block_rgb *pblock,
	long cOpaque)
{
	struct s3tc_color color;
	word rgb;

	FcolorToColor(pfcolor0, &color);
	ColorToRGB(&color, &pblock->color0);
	FcolorToColor(pfcolor1, &color);
	ColorToRGB(&color, &pblock->color1);

	if ((cOpaque == 16) ^ (pblock->color1 < pblock->color0))
	{
		rgb = pblock->color0;
		pblock->color0 = pblock->color1;
		pblock->color1 = rgb;
	}

	RGBToColor(&pblock->color0, &color);
	ColorToFcolor(&color, pfcolor0);
	RGBToColor(&pblock->color1, &color);
	ColorToFcolor(&color, pfcolor1);

	return;
}

static void ClipExtrema(
	struct s3tc_fcolor *plower,
	struct s3tc_fcolor *pupper)
{
	struct s3tc_fcolor *pmove;
	real t;
	long i;

	for (i = 0; i < 3; ++i)
	{
		if ((plower->rgba[i] < 0.0f) ^ (pupper->rgba[i] < 0.0f))
		{
			t = -(plower->rgba[i] / (pupper->rgba[i] - plower->rgba[i]));
			if (plower->rgba[i] < 0.0f)
			{
				pmove = plower;
			}
			else
			{
				t -= 1.0f;
				pmove = pupper;
			}
			pmove->rgba[2] += t * (pupper->rgba[2] - plower->rgba[2]);
			pmove->rgba[1] += t * (pupper->rgba[1] - plower->rgba[1]);
			pmove->rgba[0] += t * (pupper->rgba[0] - plower->rgba[0]);
		}

		if ((plower->rgba[i] > wtPrimary[i]) ^ (pupper->rgba[i] > wtPrimary[i]))
		{
			t = (wtPrimary[i] - plower->rgba[i]) / (pupper->rgba[i] - plower->rgba[i]);
			if (plower->rgba[i] > wtPrimary[i])
			{
				pmove = plower;
			}
			else
			{
				t -= 1.0f;
				pmove = pupper;
			}
			pmove->rgba[2] += t * (pupper->rgba[2] - plower->rgba[2]);
			pmove->rgba[1] += t * (pupper->rgba[1] - plower->rgba[1]);
			pmove->rgba[0] += t * (pupper->rgba[0] - plower->rgba[0]);
		}
	}

	return;
}

static void AllSame(
	struct s3tc_color *pcolor,
	struct s3tc_block_rgb *pblock,
	word wAlpha)
{
	struct s3tc_color color = *pcolor;
	word bit;
	unsigned long mask;
	long i;

	ColorToRGB(pcolor, &pblock->color0);
	pblock->color1 = pblock->color0;
	pblock->bitmap = 0;

	if (wAlpha != UNSIGNED_SHORT_MAX)
	{
		bit = 1;
		mask = 3;
		for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
		{
			if ((wAlpha & bit) == 0)
			{
				pblock->bitmap |= mask;
			}
			else
			{
				color = pcolor[i];
			}
			bit <<= 1;
			mask <<= 2;
		}

		ColorToRGB(&color, &pblock->color0);
		pblock->color1 = pblock->color0;
	}

	return;
}

/* ---------- public code */

void EncodeBlockRGBColorKey(
	struct s3tc_color colors[S3TC_BLOCK_PIXELS],
	struct s3tc_block_rgb *block,
	byte alpha_key)
{
	struct s3tc_fcolor fcolors[S3TC_BLOCK_PIXELS];
	struct s3tc_fcolor mean;
	struct s3tc_fcolor lower;
	struct s3tc_fcolor upper;
	real m[3][3];
	real m2[3][3];
	real scale;
	real trace;
	real best;
	real inv;
	real x;
	real y;
	real z;
	real len2;
	real min;
	real max;
	real t;
	real dr;
	real dg;
	real db;
	word wAlpha = 0;
	word bit;
	long cOpaque = 0;
	long same;
	long axis = 0;
	long iteration;
	long channel;
	long row;
	long column;
	long i;

	if (block == NULL)
	{
		return;
	}

	for (i = S3TC_BLOCK_PIXELS - 1; i >= 0; --i)
	{
		wAlpha <<= 1;
		if (colors[i].rgba[S3TC_ALPHA] >= alpha_key)
		{
			wAlpha |= 1;
			cOpaque++;
		}
		else
		{
			wAlpha &= ~1;
		}
	}

	if (cOpaque == 0)
	{
		block->color0 = 0;
		block->color1 = UNSIGNED_SHORT_MAX;
		block->bitmap = UNSIGNED_LONG_MAX;
		return;
	}

	same = TRUE;
	for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
	{
		if (same && i > 0 &&
			(colors[i].rgba[2] != colors[i - 1].rgba[2] ||
				colors[i].rgba[1] != colors[i - 1].rgba[1] ||
				colors[i].rgba[0] != colors[i - 1].rgba[0]))
		{
			same = FALSE;
		}
	}
	if (same)
	{
		AllSame(colors, block, wAlpha);
		return;
	}

	for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
	{
		ColorToFcolor(&colors[i], &fcolors[i]);
	}

	scale = 1.0f / cOpaque;
	for (channel = 0; channel < 3; ++channel)
	{
		mean.rgba[channel] = 0.0f;
		bit = 1;
		for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
		{
			if (wAlpha & bit)
			{
				mean.rgba[channel] += fcolors[i].rgba[channel];
			}
			bit <<= 1;
		}
		mean.rgba[channel] *= scale;
	}

	for (channel = 0; channel < 3; ++channel)
	{
		for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
		{
			fcolors[i].rgba[channel] -= mean.rgba[channel];
		}
	}

	for (row = 0; row < 3; ++row)
	{
		for (column = row; column < 3; ++column)
		{
			m[row][column] = 0.0f;
			bit = 1;
			for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
			{
				if (wAlpha & bit)
				{
					m[row][column] += fcolors[i].rgba[column] * fcolors[i].rgba[row];
				}
				bit <<= 1;
			}
		}
	}

	for (iteration = 0; iteration < 9; ++iteration)
	{
		Square3x3(m, m2);
		Square3x3(m2, m);
		trace = m[2][2] + m[1][1] + m[0][0];
		if (trace == 0.0f)
		{
			AllSame(colors, block, wAlpha);
			return;
		}
		scale = 3.0f / trace;
		for (row = 0; row < 3; ++row)
		{
			for (column = row; column < 3; ++column)
			{
				m[row][column] *= scale;
			}
		}
	}

	m[1][0] = m[0][1];
	m[2][0] = m[0][2];
	m[2][1] = m[1][2];

	best = 0.0f;
	if (m[0][0] > best)
	{
		best = m[0][0];
		axis = 0;
	}
	if (m[1][1] > best)
	{
		best = m[1][1];
		axis = 1;
	}
	if (m[2][2] > best)
	{
		best = m[2][2];
		axis = 2;
	}

	inv = 1.0f / (real)sqrt(best);
	x = m[0][axis] * inv;
	y = m[1][axis] * inv;
	z = m[2][axis] * inv;
	len2 = x * x + y * y + z * z;
	if (len2 == 0.0f)
	{
		AllSame(colors, block, wAlpha);
		return;
	}

	min = 99999.0f;
	max = -99999.0f;
	bit = 1;
	for (i = 0; i < S3TC_BLOCK_PIXELS; ++i)
	{
		if (wAlpha & bit)
		{
			t = (x * fcolors[i].rgba[0] + y * fcolors[i].rgba[1] + z * fcolors[i].rgba[2]) / len2;
			if (t < min)
			{
				min = t;
			}
			if (t > max)
			{
				max = t;
			}
		}
		bit <<= 1;
	}

	lower.rgba[0] = x * min + mean.rgba[0];
	upper.rgba[0] = x * max + mean.rgba[0];
	lower.rgba[1] = y * min + mean.rgba[1];
	upper.rgba[1] = y * max + mean.rgba[1];
	lower.rgba[2] = z * min + mean.rgba[2];
	upper.rgba[2] = z * max + mean.rgba[2];

	ClipExtrema(&lower, &upper);
	Quantize(&lower, &upper, block, cOpaque);

	dr = upper.rgba[0] - lower.rgba[0];
	dg = upper.rgba[1] - lower.rgba[1];
	db = upper.rgba[2] - lower.rgba[2];
	len2 = dr * dr + dg * dg + db * db;
	if (len2 == 0.0f)
	{
		AllSame(colors, block, wAlpha);
		return;
	}

	block->bitmap = 0;
	bit = 0x8000;
	for (i = S3TC_BLOCK_PIXELS - 1; i >= 0; --i)
	{
		if (wAlpha & bit)
		{
			fcolors[i].rgba[0] += mean.rgba[0];
			fcolors[i].rgba[1] += mean.rgba[1];
			fcolors[i].rgba[2] += mean.rgba[2];
			t = ((fcolors[i].rgba[0] - lower.rgba[0]) * dr +
				(fcolors[i].rgba[1] - lower.rgba[1]) * dg +
				(fcolors[i].rgba[2] - lower.rgba[2]) * db) / len2;
			if (cOpaque == 16)
			{
				t *= 4.0f;
				if (t < 0.0f)
				{
					t = 0.0f;
				}
				else if (t >= 4.0f)
				{
					t = 3.0f;
				}
				block->bitmap <<= 2;
				block->bitmap |= mapRGB4[(long)t];
			}
			else
			{
				t *= 3.0f;
				if (t < 0.0f)
				{
					t = 0.0f;
				}
				else if (t >= 3.0f)
				{
					t = 2.0f;
				}
				block->bitmap <<= 2;
				block->bitmap |= mapRGB3[(long)t];
			}
		}
		else
		{
			block->bitmap <<= 2;
			block->bitmap |= 3;
		}
		bit >>= 1;
	}

	return;
}

void DecodeBlockRGB(
	struct s3tc_block_rgb *source,
	struct s3tc_color colors[S3TC_BLOCK_PIXELS])
{
	struct s3tc_color color[4];
	unsigned long bitmap;
	long pixel;
	long channel;

	if (source == NULL)
	{
		memset(colors, 0, S3TC_BLOCK_PIXELS * sizeof(struct s3tc_color));
		return;
	}

	RGBToColor(&source->color0, &color[0]);
	RGBToColor(&source->color1, &color[1]);
	color[0].rgba[S3TC_ALPHA] = color[1].rgba[S3TC_ALPHA] = color[2].rgba[S3TC_ALPHA] = 0xFF;

	if (source->color0 > source->color1)
	{
		word c0;
		word c1;

		c0 = color[0].rgba[0];
		c1 = color[1].rgba[0];
		color[2].rgba[0] = (byte)((2 * c0 + c1 + 1) / 3);
		color[3].rgba[0] = (byte)((2 * c1 + c0 + 1) / 3);
		c0 = color[0].rgba[1];
		c1 = color[1].rgba[1];
		color[2].rgba[1] = (byte)((2 * c0 + c1 + 1) / 3);
		color[3].rgba[1] = (byte)((2 * c1 + c0 + 1) / 3);
		c0 = color[0].rgba[2];
		c1 = color[1].rgba[2];
		color[2].rgba[2] = (byte)((2 * c0 + c1 + 1) / 3);
		color[3].rgba[2] = (byte)((2 * c1 + c0 + 1) / 3);
		color[3].rgba[S3TC_ALPHA] = 0xFF;
	}
	else
	{
		for (channel = 0; channel < 3; ++channel)
		{
			color[2].rgba[channel] = (byte)((color[0].rgba[channel] + color[1].rgba[channel]) / 2);
			color[3].rgba[channel] = 0;
		}
		color[3].rgba[S3TC_ALPHA] = 0;
	}

	bitmap = source->bitmap;
	for (pixel = 0; pixel < S3TC_BLOCK_PIXELS; ++pixel)
	{
		for (channel = 0; channel < 4; ++channel)
		{
			colors[pixel].rgba[channel] = color[bitmap & 3].rgba[channel];
		}
		bitmap >>= 2;
	}

	return;
}

void DecodeBlockRGB__single_pixel(
	struct s3tc_block_rgb const *source,
	struct s3tc_color *color,
	short u,
	short v)
{
	struct s3tc_color colors[4];
	long channel;

	if (source == NULL)
	{
		memset(color, 0, sizeof(*color));
		return;
	}

	RGBToColor(&source->color0, &colors[0]);
	RGBToColor(&source->color1, &colors[1]);
	colors[0].rgba[S3TC_ALPHA] = colors[1].rgba[S3TC_ALPHA] = colors[2].rgba[S3TC_ALPHA] = 0xFF;

	if (source->color0 > source->color1)
	{
		word c0;
		word c1;

		c0 = colors[0].rgba[0];
		c1 = colors[1].rgba[0];
		colors[2].rgba[0] = (byte)((2 * c0 + c1 + 1) / 3);
		colors[3].rgba[0] = (byte)((2 * c1 + c0 + 1) / 3);
		c0 = colors[0].rgba[1];
		c1 = colors[1].rgba[1];
		colors[2].rgba[1] = (byte)((2 * c0 + c1 + 1) / 3);
		colors[3].rgba[1] = (byte)((2 * c1 + c0 + 1) / 3);
		c0 = colors[0].rgba[2];
		c1 = colors[1].rgba[2];
		colors[2].rgba[2] = (byte)((2 * c0 + c1 + 1) / 3);
		colors[3].rgba[2] = (byte)((2 * c1 + c0 + 1) / 3);
		colors[3].rgba[S3TC_ALPHA] = 0xFF;
	}
	else
	{
		for (channel = 0; channel < 3; ++channel)
		{
			colors[2].rgba[channel] = (byte)((colors[0].rgba[channel] + colors[1].rgba[channel]) / 2);
			colors[3].rgba[channel] = 0;
		}
		colors[3].rgba[S3TC_ALPHA] = 0;
	}

	match_assert("c:\\halo\\SOURCE\\bitmaps\\s3tc\\s3tc.c", 773, u >= 0 && u < 4);
	match_assert("c:\\halo\\SOURCE\\bitmaps\\s3tc\\s3tc.c", 774, v >= 0 && v < 4);

	*color = colors[(source->bitmap >> (2 * (4 * v + u))) & 3];

	return;
}

void DecodeBlockAlpha4(
	struct s3tc_block_alpha4 *source,
	struct s3tc_color colors[S3TC_BLOCK_PIXELS])
{
	long row;
	long column;
	word alpha;

	DecodeBlockRGB(&source->rgb, colors);

	for (row = 0; row < 4; ++row)
	{
		alpha = source->alpha_bitmap[row];

		for (column = 0; column < 4; ++column)
		{
			colors[4 * row + column].rgba[S3TC_ALPHA] =
				((alpha & 0xF) << 4) | (alpha & 0xF);
			alpha >>= 4;
		}
	}

	return;
}

void DecodeBlockAlpha4__single_pixel(
	struct s3tc_block_alpha4 const *source,
	struct s3tc_color *color,
	short u,
	short v)
{
	word alpha;

	DecodeBlockRGB__single_pixel(&source->rgb, color, u, v);

	alpha = (source->alpha_bitmap[v] >> (4 * u)) & 0xF;
	color->rgba[S3TC_ALPHA] = (alpha << 4) | alpha;

	return;
}

void DecodeBlockAlpha3(
	struct s3tc_block_alpha3 *source,
	struct s3tc_color colors[S3TC_BLOCK_PIXELS])
{
	long pixel;
	long alpha[8];
	unsigned long bitmap = 0;

	DecodeBlockRGB(&source->rgb, colors);

	alpha[0] = source->alpha0;
	alpha[1] = source->alpha1;

	if (alpha[0] > alpha[1])
	{
		alpha[2] = (6 * alpha[0] + 1 * alpha[1]) / 7;
		alpha[3] = (5 * alpha[0] + 2 * alpha[1]) / 7;
		alpha[4] = (4 * alpha[0] + 3 * alpha[1]) / 7;
		alpha[5] = (3 * alpha[0] + 4 * alpha[1]) / 7;
		alpha[6] = (2 * alpha[0] + 5 * alpha[1]) / 7;
		alpha[7] = (1 * alpha[0] + 6 * alpha[1]) / 7;
	}
	else
	{
		alpha[2] = (4 * alpha[0] + 1 * alpha[1]) / 5;
		alpha[3] = (3 * alpha[0] + 2 * alpha[1]) / 5;
		alpha[4] = (2 * alpha[0] + 3 * alpha[1]) / 5;
		alpha[5] = (1 * alpha[0] + 4 * alpha[1]) / 5;
		alpha[6] = 0;
		alpha[7] = 255;
	}

	for (pixel = 0; pixel < S3TC_BLOCK_PIXELS; ++pixel)
	{
		if ((pixel & 7) == 0)
		{
			if (pixel == 0)
			{
				bitmap = source->alpha_bitmap[2];
				bitmap <<= 8;
				bitmap |= source->alpha_bitmap[1];
				bitmap <<= 8;
				bitmap |= source->alpha_bitmap[0];
			}
			else
			{
				bitmap = source->alpha_bitmap[5];
				bitmap <<= 8;
				bitmap |= source->alpha_bitmap[4];
				bitmap <<= 8;
				bitmap |= source->alpha_bitmap[3];
			}
		}

		colors[pixel].rgba[S3TC_ALPHA] = (byte)alpha[bitmap & 7];
		bitmap >>= 3;
	}

	return;
}

void DecodeBlockAlpha3__single_pixel(
	struct s3tc_block_alpha3 const *source,
	struct s3tc_color *color,
	short u,
	short v)
{
	word alpha[8];
	unsigned long bitmap;

	DecodeBlockRGB__single_pixel(&source->rgb, color, u, v);

	alpha[0] = source->alpha0;
	alpha[1] = source->alpha1;

	if (alpha[0] > alpha[1])
	{
		alpha[2] = (6 * alpha[0] + alpha[1]) / 7;
		alpha[3] = (5 * alpha[0] + 2 * alpha[1]) / 7;
		alpha[4] = (4 * alpha[0] + 3 * alpha[1]) / 7;
		alpha[5] = (3 * alpha[0] + 4 * alpha[1]) / 7;
		alpha[6] = (2 * alpha[0] + 5 * alpha[1]) / 7;
		alpha[7] = (alpha[0] + 6 * alpha[1]) / 7;
	}
	else
	{
		alpha[2] = (4 * alpha[0] + alpha[1]) / 5;
		alpha[3] = (3 * alpha[0] + 2 * alpha[1]) / 5;
		alpha[4] = (2 * alpha[0] + 3 * alpha[1]) / 5;
		alpha[5] = (alpha[0] + 4 * alpha[1]) / 5;
		alpha[6] = 0;
		alpha[7] = 255;
	}

	if (v < 2)
	{
		bitmap = source->alpha_bitmap[2];
		bitmap <<= 8;
		bitmap |= source->alpha_bitmap[1];
		bitmap <<= 8;
		bitmap |= source->alpha_bitmap[0];
		bitmap >>= 3 * (4 * v + u);
		color->rgba[S3TC_ALPHA] = (byte)alpha[bitmap & 7];
	}
	else
	{
		bitmap = source->alpha_bitmap[5];
		bitmap <<= 8;
		bitmap |= source->alpha_bitmap[4];
		bitmap <<= 8;
		bitmap |= source->alpha_bitmap[3];
		bitmap >>= 3 * (4 * (v - 2) + u);
		color->rgba[S3TC_ALPHA] = (byte)alpha[bitmap & 7];
	}

	return;
}

void EncodeBlockRGB(
	struct s3tc_color colors[S3TC_BLOCK_PIXELS],
	struct s3tc_block_rgb *block)
{
	EncodeBlockRGBColorKey(colors, block, 0);

	return;
}

void EncodeBlockAlpha4(
	struct s3tc_color colors[S3TC_BLOCK_PIXELS],
	struct s3tc_block_alpha4 *block)
{
	long row;
	long column;

	for (row = 0; row < 4; ++row)
	{
		for (column = 3; column >= 0; --column)
		{
			block->alpha_bitmap[row] <<= 4;
			block->alpha_bitmap[row] |= colors[4 * row + column].rgba[S3TC_ALPHA] >> 4;
		}
	}

	EncodeBlockRGBColorKey(colors, &block->rgb, 0);

	return;
}

void EncodeBlockAlpha3(
	struct s3tc_color colors[S3TC_BLOCK_PIXELS],
	struct s3tc_block_alpha3 *block)
{
	byte alpha0;
	byte alpha1;
	byte alpha;
	long six_alpha;
	long range;
	long bias;
	long steps;
	long index;
	long pixel;
	unsigned long bitmap = 0;

	alpha0 = alpha1 = colors[0].rgba[S3TC_ALPHA];
	for (pixel = 1; pixel < S3TC_BLOCK_PIXELS; ++pixel)
	{
		alpha = colors[pixel].rgba[S3TC_ALPHA];
		if (alpha > alpha0)
		{
			alpha0 = alpha;
		}
		if (alpha < alpha1)
		{
			alpha1 = alpha;
		}
	}

	if (alpha0 == 255 && alpha1 == 0)
	{
		for (pixel = 0; pixel < S3TC_BLOCK_PIXELS; ++pixel)
		{
			alpha = colors[pixel].rgba[S3TC_ALPHA];
			if (alpha < alpha0 && alpha != 0)
			{
				alpha0 = alpha;
			}
			if (alpha > alpha1 && alpha != 255)
			{
				alpha1 = alpha;
			}
		}

		if (alpha0 < alpha1)
		{
			six_alpha = TRUE;
		}
		else
		{
			alpha0 = 255;
			alpha1 = 0;
			six_alpha = FALSE;
		}
	}
	else
	{
		six_alpha = FALSE;
	}

	block->alpha0 = alpha0;
	block->alpha1 = alpha1;

	if (alpha0 != alpha1)
	{
		range = alpha0 - alpha1;
		bias = range >> 1;
		steps = six_alpha ? 5 : 7;

		for (pixel = S3TC_BLOCK_PIXELS - 1; pixel >= 0; --pixel)
		{
			bitmap <<= 3;
			if (six_alpha && colors[pixel].rgba[S3TC_ALPHA] == 0)
			{
				bitmap |= 6;
			}
			else if (six_alpha && colors[pixel].rgba[S3TC_ALPHA] == 255)
			{
				bitmap |= 7;
			}
			else
			{
				index = ((alpha0 - colors[pixel].rgba[S3TC_ALPHA]) * steps + bias) / range;
				if (index >= steps)
				{
					bitmap |= 1;
				}
				else if (index > 0)
				{
					bitmap |= index + 1;
				}
			}

			if ((pixel & 7) == 0)
			{
				if (pixel == 8)
				{
					block->alpha_bitmap[3] = (byte)bitmap;
					bitmap >>= 8;
					block->alpha_bitmap[4] = (byte)bitmap;
					bitmap >>= 8;
					block->alpha_bitmap[5] = (byte)bitmap;
				}
				else
				{
					block->alpha_bitmap[0] = (byte)bitmap;
					bitmap >>= 8;
					block->alpha_bitmap[1] = (byte)bitmap;
					bitmap >>= 8;
					block->alpha_bitmap[2] = (byte)bitmap;
				}
			}
		}
	}
	else
	{
		block->alpha_bitmap[0] = block->alpha_bitmap[1] = block->alpha_bitmap[2] =
			block->alpha_bitmap[3] = block->alpha_bitmap[4] = block->alpha_bitmap[5] = 0;
	}

	EncodeBlockRGBColorKey(colors, &block->rgb, 0);

	return;
}
