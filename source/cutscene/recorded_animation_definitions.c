/*
RECORDED_ANIMATION_DEFINITIONS.C

symbols in this file:
00081CD0 0050:
	_code_00081cd0 (0000)
00081D20 0060:
	_scenario_get_animation_by_name (0000)
00258A84 0021:
	??_C@_0CB@HNENIBHH@recorded?5animation?5event?5stream?$CK@ (0000)
00258AA8 001b:
	??_C@_0BL@GJLMOMCK@length?5of?5animation?$CK?3ticks?$AA@ (0000)
00258AC4 001b:
	??_C@_0BL@NBNEAFHE@unit?5control?5data?5version?$CK?$AA@ (0000)
00258AE0 0014:
	??_C@_0BE@LAHBNGDE@raw?5animation?5data?$CK?$AA@ (0000)
00258AF4 0009:
	??_C@_08COEAJJNH@version?$CK?$AA@ (0000)
00258B00 0019:
	??_C@_0BJ@DEDBCMOK@recorded_animation_block?$AA@ (0000)
00258B1C 0025:
	??_C@_0CF@DKFDGPGH@recorded_animation_event_stream_@ (0000)
002DCD58 00b4:
	_recorded_animation_event_stream_data (0000)
	_recorded_animation_block (0088)
*/

/* ---------- headers */

#include "cseries.h"
#include "cutscene/recorded_animation_definitions.h"
#include "cutscene/recorded_animation_playback.h"
#include "cutscene/recorded_animation_playback_v1.h"
#include "math/real_math.h"
#include "scenario/scenario_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void byte_swap_recording(
	struct recorded_animation_definition const *animation,
	void *stream,
	long stream_size);

/* ---------- globals */

struct recorded_animation_event_stream_definition
{
	struct tag_data_definition data;
	struct tag_field fields[10];
};

struct recorded_animation_event_stream_definition recorded_animation_event_stream_data =
{
	{
		"recorded_animation_event_stream_data",
		0,
		0x200000,
		byte_swap_recording,
	},
	{
		{ _tag_field_string, 0, "name^", NULL },
		{ _tag_field_char_integer, 0, "version*", NULL },
		{ _tag_field_char_integer, 0, "raw animation data*", NULL },
		{ _tag_field_char_integer, 0, "unit control data version*", NULL },
		{ _tag_field_pad, 0, NULL, (void *)1 },
		{ _tag_field_short_integer, 0, "length of animation*:ticks", NULL },
		{ _tag_field_pad, 0, NULL, (void *)2 },
		{ _tag_field_pad, 0, NULL, (void *)4 },
		{ _tag_field_data, 0, "recorded animation event stream*", &recorded_animation_event_stream_data.data },
		{ _tag_field_terminator, 0, NULL, NULL },
	},
};

struct tag_block_definition recorded_animation_block =
{
	"recorded_animation_block",
	0,
	1024,
	sizeof(struct recorded_animation_definition),
	NULL,
	recorded_animation_event_stream_data.fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

/* ---------- public code */

short scenario_get_animation_by_name(struct scenario const *scenario, char const *name)
{
	short animation_index;
	short result = NONE;

	for (animation_index = 0; animation_index < scenario->recorded_animations.count; animation_index++)
	{
		struct recorded_animation_definition const *animation = TAG_BLOCK_GET_ELEMENT(
			&scenario->recorded_animations,
			animation_index,
			struct recorded_animation_definition);

		if (!_stricmp(animation->name, name))
		{
			result = animation_index;
			break;
		}
	}

	return result;
}

/* ---------- private code */

static void byte_swap_recording(
	struct recorded_animation_definition const *animation,
	void *stream,
	long stream_size)
{
	if (animation->version > 0)
	{
		if (animation->version > 3)
		{
			if (animation->version == 4)
				byte_swap_recording_stream(stream, stream_size, animation->unit_control_data_version);
		}
		else
		{
			byte_swap_recording_stream_v1(stream, stream_size, animation->unit_control_data_version);
		}
	}
}
