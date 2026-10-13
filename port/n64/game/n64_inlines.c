/*
N64_INLINES.C

Inline helpers whose only body lived in a unit the N64 build leaves out
(tools/n64_link_check.py finds them: a call to one would otherwise go to
address 0). The bodies are the game's own.
*/

#include "cseries.h"
#include "math/real_math.h"

/* ai/actor_looking.c */
boolean valid_real_normal2d(
	real_vector2d const *normal)
{
	return valid_realcmp(magnitude_squared2d(normal), 1.0f);
}
