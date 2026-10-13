/*
PROGRESS_BAR_INTERNAL.H

Xbox render-state helpers owned by PROGRESS_BAR.C.
*/

#ifndef __PROGRESS_BAR_INTERNAL_H
#define __PROGRESS_BAR_INTERNAL_H
#pragma once

/* ---------- headers */

#include <xtl.h>

/* ---------- prototypes/PROGRESS_BAR.C */

void SetTextureStageStateSmart(
	unsigned long stage,
	D3DTEXTURESTAGESTATETYPE type,
	unsigned long value);

#endif /* __PROGRESS_BAR_INTERNAL_H */
