/*
SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR.C

symbols in this file:
0016B4E0 0010:
	_shader_map_verify (0000)
0016B4F0 01b0:
	_shader_transparent_chicago_create (0000)
0029CE08 00d0:
	_shader_transparent_chicago_combiner_table (0000)
0029CED8 0025:
	??_C@_0CF@ILPKPMME@?$CD?$CD?$CD?5ERROR?5chicago?5shader?5has?5no?5@ (0000)
0029CF00 0049:
	??_C@_0EJ@GDMOILEK@c?3?2halo?2SOURCE?2rasterizer?2xbox?2s@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "rasterizer/xbox/rasterizer_xbox_pixel_shader.h"
#include "rasterizer/xbox/shader_transparent_chicago_preprocessor.h"
#include "shaders/shader_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	_shader_type_transparent_chicago = 6
};

enum
{
	_shader_transparent_chicago_map_flag_alpha_replicate_bit = 1
};

enum
{
	NUMBER_OF_SHADER_FUNCTIONS = 13,
	NUMBER_OF_PIXEL_SHADER_STAGES = 8
};

/* ---------- macros */

/* ---------- structures */

struct shader_transparent_chicago_map
{
	word flags;
	byte reserved_before_functions[42];
	short color_function;
	short alpha_function;
	byte reserved_after_functions[172];
};

struct shader_transparent_chicago_definition
{
	struct shader shader;
	byte numeric_counter_limit;
	byte flags;
	short first_map_type;
	short framebuffer_blend_function;
	short framebuffer_fade_mode;
	short framebuffer_fade_source;
	short pad;
	real lens_flare_spacing;
	struct tag_reference lens_flare;
	struct tag_block extra_layers;
	struct tag_block maps;
};

struct shader_transparent_chicago_combiner_table
{
	unsigned long rgb_inputs[2][NUMBER_OF_SHADER_FUNCTIONS];
	unsigned long alpha_inputs[NUMBER_OF_SHADER_FUNCTIONS];
	unsigned long stage_increments[NUMBER_OF_SHADER_FUNCTIONS];
};

/* ---------- prototypes */

boolean shader_map_verify(
	struct shader_transparent_chicago_map *map,
	short map_index);

boolean shader_transparent_chicago_create(
	struct shader *shader,
	struct pixel_shader_definition *pixel_shader);

/* ---------- globals */

const struct shader_transparent_chicago_combiner_table shader_transparent_chicago_combiner_table =
{
	{
		{
			0x0c200000, 0x08200000, 0x080c0000, 0x080c080c, 0x08200c20,
			0x08204c20, 0x0c204820, 0x08200c40, 0x0c200840, 0x081c0c3c,
			0x0c1c083c, 0x08180c38, 0x0c180838
		},
		{
			0x0c200000, 0x18200000, 0x180c0000, 0x180c180c, 0x18200c20,
			0x18204c20, 0x0c205820, 0x18200c40, 0x0c201840, 0x181c0c3c,
			0x0c1c183c, 0x18180c38, 0x0c181838
		}
	},
	{
		0x1c200000, 0x18200000, 0x181c0000, 0x181c181c, 0x18201c20,
		0x18205c20, 0x1c205820, 0x18201c40, 0x1c201840, 0x181c1c3c,
		0x1c1c183c, 0x18181c38, 0x1c181838
	},
	{
		0x00000000, 0x01000000, 0x01000000, 0x01000100, 0x01000000,
		0x01000000, 0x00000100, 0x01000000, 0x00000100, 0x01000000,
		0x00000100, 0x01010001, 0x00010101
	}
};

/* ---------- public code */

boolean shader_map_verify(
	struct shader_transparent_chicago_map *map,
	short map_index)
{
	return TRUE;
}

boolean shader_transparent_chicago_create(
	struct shader *shader,
	struct pixel_shader_definition *pixel_shader)
{
	struct shader_transparent_chicago_definition *chicago;
	boolean result = TRUE;
	short map_index;

#line 100 "c:\\halo\\SOURCE\\rasterizer\\xbox\\shader_transparent_chicago_preprocessor.c"
	match_assert(__FILE__, __LINE__, shader);
	match_assert(__FILE__, __LINE__, pixel_shader);

	chicago = (struct shader_transparent_chicago_definition *)shader_get_and_verify_type(shader, _shader_type_transparent_chicago);

	csmemset(pixel_shader, 0, sizeof(*pixel_shader));

	pixel_shader->combiner_count = (chicago->maps.count + 1) | 0x11000;

	if (chicago->maps.count > 0)
	{
		pixel_shader->texture_modes =
			((((chicago->maps.count > 3) << 5 | (chicago->maps.count > 2)) << 5 |
			(chicago->maps.count > 1)) << 5) |
			(2 * (chicago->first_map_type != 0) + 1);

		for (map_index = 0; map_index < chicago->maps.count; map_index++)
		{
			struct shader_transparent_chicago_map *map = TAG_BLOCK_GET_ELEMENT(
				&chicago->maps,
				map_index,
				struct shader_transparent_chicago_map);

			if (map_index != chicago->maps.count - 1)
			{
				pixel_shader->alpha_inputs[map_index + 1] =
					shader_transparent_chicago_combiner_table.stage_increments[map->alpha_function] * (map_index + 1) +
					shader_transparent_chicago_combiner_table.alpha_inputs[map->alpha_function];
				pixel_shader->rgb_inputs[map_index + 1] =
					shader_transparent_chicago_combiner_table.stage_increments[map->color_function] * (map_index + 1) +
					shader_transparent_chicago_combiner_table.rgb_inputs[TEST_FLAG(map->flags, _shader_transparent_chicago_map_flag_alpha_replicate_bit)][map->color_function];
			}
			else
			{
				pixel_shader->alpha_inputs[0] = 0x18200000;
				pixel_shader->rgb_inputs[0] = 0x08200000;
			}

			pixel_shader->alpha_outputs[map_index] = 0x00000c00;
			pixel_shader->rgb_outputs[map_index] = 0x00000c00;

			if (result)
			{
				result = shader_map_verify(map, map_index);
			}
		}
	}
	else
	{
		error(_error_silent, "### ERROR chicago shader has no maps");
		result = FALSE;
	}

	pixel_shader->final_combiner_inputs_abcd = 12;
	pixel_shader->final_combiner_inputs_efg = 0x1c00;

	return result;
}

/* ---------- private code */
