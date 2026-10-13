/*
SHADER_DEFINITIONS.C

symbols in this file:
0017FF00 0060:
	_shader_get_and_verify_type (0000)
002A082C 001f:
	??_C@_0BP@FJMHFJCB@shader?9?$DObase?4type?$DN?$DNshader_type?$AA@ (0000)
002A084C 002c:
	??_C@_0CM@OMIDGMIG@c?3?2halo?2SOURCE?2shaders?2shader_de@ (0000)
0030E800 016c:
	_global_shader_effect_additive (0000)
	_global_shader_effect_alpha_blended (00b8)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"
#include "shader_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

struct shader_effect_definition global_shader_effect_additive =
{
	{
		{
			0,
			0,
			0.0f,
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			0,
			0,
			1,
			0
		}
	},
	0,
	3,
	0,
	0,
	{ 0 },
	{ 'bitm', "", 0, NONE },
	{ 0 }
};

struct shader_effect_definition global_shader_effect_alpha_blended =
{
	{
		{
			0,
			0,
			0.0f,
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			0,
			0,
			1,
			0
		}
	},
	0,
	0,
	0,
	0,
	{ 0 },
	{ 'bitm', "", 0, NONE },
	{ 0 }
};

/* ---------- public code */

struct shader *shader_get_and_verify_type(struct shader *shader, short shader_type)
{
	match_assert("c:\\halo\\SOURCE\\shaders\\shader_definitions.c", 2140, shader);
	match_assert("c:\\halo\\SOURCE\\shaders\\shader_definitions.c", 2141, shader->base.type==shader_type);
	return shader;
}

/* ---------- private code */
