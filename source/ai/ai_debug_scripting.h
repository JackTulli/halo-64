/*
AI_DEBUG_SCRIPTING.H

Scripting callbacks implemented by ai_debug.c.
*/

#ifndef __AI_DEBUG_SCRIPTING_H
#define __AI_DEBUG_SCRIPTING_H
#pragma once

/* ---------- prototypes/AI_DEBUG.C */

void ai_debug_communication_suppress(
	long name_count,
	char const **names);
void ai_debug_communication_ignore(
	long name_count,
	char const **names);
void ai_debug_communication_focus(
	long name_count,
	char const **names);

#endif // __AI_DEBUG_SCRIPTING_H
