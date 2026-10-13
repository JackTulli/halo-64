/*
SOUND_SCENERY.C

symbols in this file:
001BF330 0030:
	_sound_scenery_new (0000)
001BF360 0010:
	_sound_scenery_delete (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "object_types.h"
#include "objects.h"
#include "sound/sound_scenery.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

boolean sound_scenery_new(
	long object_index)
{
	struct object_datum *object;

	object = sound_scenery_get(object_index);
	object->object.flags |= FLAG(_object_shadowless_bit);

	return TRUE;
}

void sound_scenery_delete(
	long object_index)
{
	return;
}

/* ---------- private code */
