/*
DEVICE_LIGHT_FIXTURES.C

symbols in this file:
00083FE0 0010:
	_light_fixtures_initialize (0000)
00083FF0 0010:
	_light_fixtures_dispose (0000)
00084000 0010:
	_light_fixtures_initialize_for_new_map (0000)
00084010 0010:
	_light_fixtures_dispose_from_old_map (0000)
00084020 0070:
	_light_fixture_place (0000)
00084090 0030:
	_light_fixture_new (0000)
000840C0 0010:
	_light_fixture_delete (0000)
000840D0 0030:
	_light_fixture_update (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "devices/device_light_fixtures.h"
#include "devices/devices.h"
#include "objects/object_types.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void light_fixtures_initialize(
	void)
{
	return;
}

void light_fixtures_dispose(
	void)
{
	return;
}

void light_fixtures_initialize_for_new_map(
	void)
{
	return;
}

void light_fixtures_dispose_from_old_map(
	void)
{
	return;
}

void light_fixture_place(
	long object_index,
	struct scenario_light_fixture_datum *scenario_light_fixture)
{
	struct light_fixture_datum *light_fixture = light_fixture_get(object_index);

	light_fixture_definition_get(light_fixture->definition_index);
	device_add_scenario_information(object_index, &scenario_light_fixture->device);
	light_fixture->light_fixture.color = scenario_light_fixture->color;
	light_fixture->light_fixture.intensity = scenario_light_fixture->intensity;
	light_fixture->light_fixture.falloff_angle = scenario_light_fixture->falloff_angle;
	light_fixture->light_fixture.cutoff_angle = scenario_light_fixture->cutoff_angle;

	return;
}

boolean light_fixture_new(
	long object_index)
{
	struct light_fixture_datum *light_fixture = light_fixture_get(object_index);

	light_fixture_definition_get(light_fixture->definition_index);
	return TRUE;
}

void light_fixture_delete(
	long object_index)
{
	return;
}

boolean light_fixture_update(
	long object_index)
{
	struct light_fixture_datum *light_fixture = light_fixture_get(object_index);

	light_fixture_definition_get(light_fixture->definition_index);
	return TRUE;
}

/* ---------- private code */
