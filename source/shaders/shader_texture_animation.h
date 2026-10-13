/*
SHADER_TEXTURE_ANIMATION.H

header included in hcex build.
*/

#ifndef __SHADER_TEXTURE_ANIMATION_H
#define __SHADER_TEXTURE_ANIMATION_H
#pragma once

/* ---------- headers */

#include "math/real_math.h"

/* ---------- structures */

struct shader_texture_animation
{
	short u_source;
	short u_function;
	real u_period;
	real u_phase;
	real u_scale;
	short v_source;
	short v_function;
	real v_period;
	real v_phase;
	real v_scale;
	short r_source;
	short r_function;
	real r_period;
	real r_phase;
	real r_scale;
	real_point2d r_center;
};

typedef char verify_shader_texture_animation_size[
	sizeof(struct shader_texture_animation) == 0x38 ? 1 : -1];

/* ---------- public code */

#endif // __SHADER_TEXTURE_ANIMATION_H
