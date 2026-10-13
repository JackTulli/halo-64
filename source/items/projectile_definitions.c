/*
PROJECTILE_DEFINITIONS.C

symbols in this file:
00306A90 00a0:
	_default_projectile_material_response (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "projectile_definitions.h"

#include "effects/effect_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

struct projectile_material_response_definition default_projectile_material_response =
{
	0,
	_projectile_material_response_disappear,
	{ EFFECT_DEFINITION_TAG, "", 0, NONE },
	{ 0 },
	_projectile_material_response_disappear,
	0,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	{ EFFECT_DEFINITION_TAG, "", 0, NONE },
	{ 0 },
	_projectile_material_effect_scale_damage,
	0,
	0.0f,
	0.0f,
	{ EFFECT_DEFINITION_TAG, "", 0, NONE },
	{ 0 },
	0.0f,
	0.0f,
	0.0f,
	0.0f,
};

/* ---------- public code */

/* ---------- private code */
