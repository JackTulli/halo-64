/*
HUD_DEFINITIONS.C

symbols in this file:
0026FD34 000d:
	??_C@_0N@BBLMBDOF@bottom_right?$AA@ (0000)
0026FD44 000c:
	??_C@_0M@JIMDKNMP@bottom_left?$AA@ (0000)
0026FD50 000a:
	??_C@_09GAGOKEPG@top_right?$AA@ (0000)
0026FD5C 0009:
	??_C@_08PLPBDHIL@top_left?$AA@ (0000)
002E4C38 0014:
	_global_hud_anchor_names (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "hud_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

char const *global_hud_anchor_names[NUMBER_OF_HUD_ANCHORS]=
{
	"top_left",
	"top_right",
	"bottom_left",
	"bottom_right",
	"center"
};

/* ---------- public code */

/* ---------- private code */
