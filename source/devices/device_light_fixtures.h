/*
DEVICE_LIGHT_FIXTURES.H

header included in hcex build.
*/

#ifndef __DEVICE_LIGHT_FIXTURES_H
#define __DEVICE_LIGHT_FIXTURES_H
#pragma once

/* ---------- headers */

#include "devices/devices.h"
#include "objects/object_types.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

enum
{
	LIGHT_FIXTURE_DEFINITION_TAG = 'lifi',
};

/* ---------- macros */

#define light_fixture_definition_get(index) ((struct light_fixture_definition *)tag_get(LIGHT_FIXTURE_DEFINITION_TAG, (index)))
#define light_fixture_get(index) ((struct light_fixture_datum *)object_get_and_verify_type((index), _object_mask_light_fixture))
#define light_fixture_try_and_get(index) ((struct light_fixture_datum *)object_try_and_get_and_verify_type((index), _object_mask_light_fixture))

/* ---------- structures */

struct light_fixture_definition;

struct scenario_light_fixture_datum
{
	struct scenario_object_datum object;
	struct scenario_device_datum device;
	real_rgb_color color;
	real intensity;
	real falloff_angle;
	real cutoff_angle;
};

struct _light_fixture_datum
{
	real_rgb_color color;
	real intensity;
	real falloff_angle;
	real cutoff_angle;
};

struct light_fixture_datum
{
	long definition_index;
	struct _object_datum object;
	struct _device_datum device;
	struct _light_fixture_datum light_fixture;
};

/* ---------- prototypes/DEVICE_LIGHT_FIXTURES.C */

/* ---------- globals */

/* ---------- public code */

#endif // __DEVICE_LIGHT_FIXTURES_H
