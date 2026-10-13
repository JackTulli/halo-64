/*
WIDGETS.C

symbols in this file:
00125770 0030:
	_tag_group_to_widget_type (0000)
001257A0 0040:
	_code_001257a0 (0000)
001257E0 00b0:
	_widgets_initialize (0000)
00125890 0060:
	_widgets_initialize_for_new_map (0000)
001258F0 0060:
	_widgets_dispose_from_old_map (0000)
00125950 0050:
	_widgets_dispose (0000)
001259A0 0180:
	_widgets_new (0000)
00125B20 0100:
	_widgets_delete (0000)
00125C20 0080:
	_widgets_need_lighting (0000)
00125CA0 00d0:
	_widgets_render (0000)
00125D70 0060:
	_widgets_update (0000)
00289314 0027:
	??_C@_0CH@NKAAGPFD@type?$DO?$DN0?5?$CG?$CG?5type?$DMNUMBER_OF_WIDGET@ (0000)
0028933C 002e:
	??_C@_0CO@EDLFCNMP@c?3?2halo?2source?2objects?2widgets?2w@ (0000)
0028936C 001b:
	??_C@_0BL@FJEANII@type_definition?9?$DOgroup_tag?$AA@ (0000)
00289388 000c:
	??_C@_0M@KCKNIKHF@widget_data?$AA@ (0000)
00289394 0029:
	??_C@_0CJ@MOJLBDJC@c?3?2halo?2SOURCE?2objects?2widgets?2w@ (0000)
002893C0 001d:
	??_C@_0BN@OIGBDNFF@type_definition?9?$DOdelete_proc?$AA@ (0000)
002893E0 002d:
	??_C@_0CN@EMAFGINP@?$CBtype_definition?9?$DOneeds_lighting@ (0000)
0030B2B0 00c8:
	_data_0030b2b0 (0000)
*/

/* ---------- headers */

#include "objects/widgets/widgets.h"

#include "objects/object_definitions.h"
#include "objects/objects.h"
#include "objects/widgets/antenna.h"
#include "objects/widgets/flags.h"
#include "objects/widgets/glow.h"
#include "objects/widgets/light_volumes.h"
#include "objects/widgets/lightning.h"
#include "objects/widgets/widget_types.h"
#include "saved games/game_state.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
#ifdef HALO_N64
	/* (none are made on the N64: widget_new, below) */
	MAXIMUM_WIDGETS_PER_MAP = 1
#else
	MAXIMUM_WIDGETS_PER_MAP = 64
#endif
};

/* ---------- macros */

#define widget_type_definitions data_0030b2b0

/* ---------- structures */

/* ---------- prototypes */

static struct widget_type_definition *code_001257a0(
	short type);

/* ---------- globals */

struct widget_type_definition data_0030b2b0[NUMBER_OF_WIDGET_TYPES] =
{
	{
		'flag',
		TRUE,
		{ 0, 0, 0 },
		flags_initialize,
		flags_initialize_for_new_map,
		flags_dispose_from_old_map,
		flags_dispose,
		flag_new,
		flag_delete,
		flags_update,
		flag_render
	},
	{
		'ant!',
		FALSE,
		{ 0, 0, 0 },
		antennas_initialize,
		antennas_initialize_for_new_map,
		antennas_dispose_from_old_map,
		antennas_dispose,
		antenna_new,
		antenna_delete,
		antennas_update,
		antenna_render
	},
	{
		'glw!',
		FALSE,
		{ 0, 0, 0 },
		glow_initialize,
		glow_initialize_for_new_map,
		glow_dispose_from_old_map,
		glow_dispose,
		glow_new,
		glow_delete,
		NULL,
		glow_submit
	},
	{
		'mgs2',
		FALSE,
		{ 0, 0, 0 },
		light_volumes_initialize,
		light_volumes_initialize_for_new_map,
		light_volumes_dispose_from_old_map,
		light_volumes_dispose,
		light_volume_new,
		light_volume_delete,
		NULL,
		light_volume_submit
	},
	{
		'elec',
		FALSE,
		{ 0, 0, 0 },
		lightnings_initialize,
		lightnings_initialize_for_new_map,
		lightnings_dispose_from_old_map,
		lightnings_dispose,
		lightning_new,
		lightning_delete,
		NULL,
		lightning_submit
	}
};

/* ---------- public code */

short tag_group_to_widget_type(
	unsigned long group_tag)
{
	short result = NONE;
	short type;

	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		if (widget_type_definitions[type].group_tag == group_tag)
		{
			result = type;
			break;
		}
	}

	return result;
}

void widgets_initialize(
	void)
{
	short type;

	widget_data = game_state_data_new(
		"widget",
		MAXIMUM_WIDGETS_PER_MAP,
		sizeof(struct widget_datum));
	match_assert(
		"c:\\halo\\SOURCE\\objects\\widgets\\widgets.c",
		46,
		widget_data);

	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		struct widget_type_definition *type_definition = code_001257a0(type);

		match_assert(
			"c:\\halo\\SOURCE\\objects\\widgets\\widgets.c",
			55,
			type_definition->group_tag);
		if (type_definition->initialize)
			type_definition->initialize();
	}

	return;
}

void widgets_initialize_for_new_map(
	void)
{
	short type;

	data_make_valid(widget_data);
	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		struct widget_type_definition *type_definition = code_001257a0(type);

		if (type_definition->initialize_for_new_map)
			type_definition->initialize_for_new_map();
	}

	return;
}

void widgets_dispose_from_old_map(
	void)
{
	short type;

	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		struct widget_type_definition *type_definition = code_001257a0(type);

		if (type_definition->dispose_from_old_map)
			type_definition->dispose_from_old_map();
	}
	data_make_invalid(widget_data);

	return;
}

void widgets_dispose(
	void)
{
	short type;

	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		struct widget_type_definition *type_definition = code_001257a0(type);

		if (type_definition->dispose)
			type_definition->dispose();
	}

	return;
}

void widgets_new(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	struct object_definition *object_definition = object_definition_get(object->definition_index);
	short widget_reference_index;

	object->object.first_widget_index = NONE;
#ifdef HALO_N64
	/* (the N64 draws no widgets (antennas, flags, glows): none are made, and none are simulated --
	their point physics alone cast ~1,500 rays a second in Firefight) */
	return;
#endif
	widget_reference_index = 0;
	if (object_definition->object.widgets.count > 0)
	{
		do
		{
			struct object_definition_widget *widget_reference = TAG_BLOCK_GET_ELEMENT(
				&object_definition->object.widgets,
				widget_reference_index,
				struct object_definition_widget);
			short type = tag_group_to_widget_type(widget_reference->type.group_tag);

			if (type != NONE && widget_reference->type.index != NONE)
			{
				struct widget_type_definition *type_definition = code_001257a0(type);
				long widget_index = datum_new(widget_data);

				if (widget_index != NONE)
				{
					struct widget_datum *widget = widget_get(widget_index);

					widget->type = type;
					if (type_definition->new_proc)
					{
						widget->type_datum_index = type_definition->new_proc(widget_reference->type.index);
						if (widget->type_datum_index != NONE)
						{
							widget->next_widget_index = object->object.first_widget_index;
							object->object.first_widget_index = widget_index;
						}
						else
							datum_delete(widget_data, widget_index);
					}
					else
					{
						widget->next_widget_index = object->object.first_widget_index;
						object->object.first_widget_index = widget_index;
						widget->type_datum_index = NONE;
					}
				}
			}

			widget_reference_index = (short)(widget_reference_index + 1);
		}
		while (widget_reference_index < object_definition->object.widgets.count);
	}

	return;
}

void widgets_delete(
	long object_index)
{
	struct object_datum *object = object_get(object_index);
	long widget_index = object->object.first_widget_index;

	while (widget_index != NONE)
	{
		struct widget_datum *widget = widget_get(widget_index);
		struct widget_type_definition *type_definition = code_001257a0(widget->type);
		long next_widget_index = widget->next_widget_index;

		if (widget->type_datum_index != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\objects\\widgets\\widgets.c",
				190,
				type_definition->delete_proc);
			type_definition->delete_proc(widget->type_datum_index);
		}
		datum_delete(widget_data, widget_index);
		widget_index = next_widget_index;
	}
	object->object.first_widget_index = NONE;

	return;
}

boolean widgets_need_lighting(
	long widget_index)
{
	boolean result = FALSE;

	while (widget_index != NONE)
	{
		struct widget_datum *widget = widget_get(widget_index);

		if (code_001257a0(widget->type)->needs_lighting)
		{
			result = TRUE;
			break;
		}
		widget_index = widget->next_widget_index;
	}

	return result;
}

void widgets_render(
	long object_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation)
{
	struct object_datum *object = object_get(object_index);
	long widget_index = object->object.first_widget_index;

	while (widget_index != NONE)
	{
		struct widget_datum *widget = widget_get(widget_index);
		struct widget_type_definition *type_definition = code_001257a0(widget->type);

		if (type_definition->render_proc)
		{
			match_assert(
				"c:\\halo\\SOURCE\\objects\\widgets\\widgets.c",
				241,
				!type_definition->needs_lighting || lighting);
			type_definition->render_proc(
				object_index,
				widget->type_datum_index,
				lighting,
				animation);
		}
		widget_index = widget->next_widget_index;
	}

	return;
}

void widgets_update(
	real delta)
{
	short type;

	for (type = 0; type < NUMBER_OF_WIDGET_TYPES; type++)
	{
		struct widget_type_definition *type_definition = code_001257a0(type);

		if (type_definition->update)
			type_definition->update(delta);
	}

	return;
}

/* ---------- private code */

static struct widget_type_definition *code_001257a0(
	short type)
{
	match_assert(
		"c:\\halo\\source\\objects\\widgets\\widget_types.h",
		150,
		type>=0 && type<NUMBER_OF_WIDGET_TYPES);

	return &widget_type_definitions[type];
}
