/*
SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR.H
*/

#ifndef __SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR_H
#define __SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR_H
#pragma once

#include "cseries/cseries.h"

struct pixel_shader_definition;
struct shader;

boolean shader_transparent_chicago_create(
	struct shader *shader,
	struct pixel_shader_definition *pixel_shader);

#endif /* __SHADER_TRANSPARENT_CHICAGO_PREPROCESSOR_H */
