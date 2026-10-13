/*
RASTERIZER_XBOX_STATE.H

Narrow Xbox rasterizer state interface for translation units that do not use
the D3D declarations in rasterizer_xbox.h.
*/

#ifndef __RASTERIZER_XBOX_STATE_H
#define __RASTERIZER_XBOX_STATE_H
#pragma once

#include "cseries.h"

void rasterizer_set_stencil_mode(
	long stencil_mode);
void rasterizer_profile_begin(
	short profile);
void rasterizer_profile_end(
	short profile);

#endif /* __RASTERIZER_XBOX_STATE_H */
