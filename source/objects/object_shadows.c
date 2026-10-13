/*
OBJECT_SHADOWS.C

symbols in this file:
0012B870 0010:
	_code_0012b870 (0000)
0012B880 0050:
	_code_0012b880 (0000)
0012B8D0 0080:
	_object_build_shadow (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "objects/objects.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct object_shadow
{
	real object_bounding_radius;
	real_rectangle3d bounds;
	short count;
	short unknown1;
};

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

static void *object_shadow_get_object(
	long object_index)
{
	return object_get_and_verify_type(object_index, _object_mask_all);
}

void object_build_shadow_recursive(
	long object_index,
	void const *context,
	struct object_shadow *shadow)
{
	while (object_index != NONE)
	{
		struct object_datum *object = object_get(object_index);

		object_shadow_get_object(object_index);
		object_build_shadow_recursive(object->object.first_child_object_index, context, shadow);
		object_index = object->object.next_object_index;
	}
}

boolean object_build_shadow(
	long object_index,
	void const *context,
	struct object_shadow *shadow)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *definition = object_definition_get(object->definition_index);
	boolean result = FALSE;

	shadow->object_bounding_radius = definition->object.bounding_radius;
	shadow->bounds.x0 = REAL_MAX;
	shadow->bounds.x1 = -REAL_MAX;
	shadow->bounds.y0 = REAL_MAX;
	shadow->bounds.y1 = -REAL_MAX;
	shadow->bounds.z0 = REAL_MAX;
	shadow->bounds.z1 = -REAL_MAX;
	shadow->count = 0;
	shadow->unknown1 = 0;

	object_shadow_get_object(object_index);
	object_build_shadow_recursive(object->object.first_child_object_index, context, shadow);
	if (shadow->count > 0)
	{
		result = TRUE;
	}

	return result;
}

/* ---------- private code */
