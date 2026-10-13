/*
RASTERIZER_XBOX_PIXEL_SHADER.H

Complete Xbox pixel-shader state shared by the backend and shader builders.
*/

#ifndef __RASTERIZER_XBOX_PIXEL_SHADER_H
#define __RASTERIZER_XBOX_PIXEL_SHADER_H
#pragma once

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

#endif /* __RASTERIZER_XBOX_PIXEL_SHADER_H */
