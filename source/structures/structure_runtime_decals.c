/*
STRUCTURE_RUNTIME_DECALS.C

symbols in this file:
00185AE0 0040:
	_structure_decals_initialize (0000)
00185B20 0030:
	_structure_decals_initialize_for_new_map (0000)
00185B50 0030:
	_structure_decals_reconnect_to_structure_bsp (0000)
00185B80 0070:
	_structure_decals_disconnect_from_structure_bsp (0000)
00185BF0 0010:
	_structure_decals_dispose_from_old_map (0000)
00185C00 0010:
	_structure_decals_dispose (0000)
00185C10 0210:
	_structure_decals_update (0000)
002A1A8C 0019:
	??_C@_0BJ@EHFGPMAL@structure_decals_globals?$AA@ (0000)
002A1AA8 0035:
	??_C@_0DF@EMFPFLCM@c?3?2halo?2SOURCE?2structures?2struct@ (0000)
002A1AE0 0011:
	??_C@_0BB@PDFGKABJ@structure?5decals?$AA@ (0000)
004C0CE8 0004:
	_structure_decals_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "effects/decal_definitions.h"
#include "effects/decals.h"
#include "math/real_math.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "structures.h"
#include "structure_bsp_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct structure_decals_globals
{
	boolean reconnect_to_structure_bsp;
	byte pad[3];
};

/* ---------- globals */

struct structure_decals_globals *structure_decals_globals = NULL;

/* ---------- public code */

void structure_decals_initialize(
	void)
{
	structure_decals_globals = game_state_malloc("structure decals", NULL, sizeof(*structure_decals_globals));
	match_assert("c:\\halo\\SOURCE\\structures\\structure_runtime_decals.c", 0x1c, structure_decals_globals);

	return;
}

void structure_decals_initialize_for_new_map(
	void)
{
	match_assert("c:\\halo\\SOURCE\\structures\\structure_runtime_decals.c", 0x24, structure_decals_globals);
	structure_decals_globals->reconnect_to_structure_bsp = FALSE;

	return;
}

void structure_decals_reconnect_to_structure_bsp(
	void)
{
	match_assert("c:\\halo\\SOURCE\\structures\\structure_runtime_decals.c", 0x2d, structure_decals_globals);
	structure_decals_globals->reconnect_to_structure_bsp = TRUE;

	return;
}

void structure_decals_disconnect_from_structure_bsp(
	void)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();

	if (structure_bsp->runtime_decals.count)
	{
		struct tag_block *clusters = &structure_bsp->clusters;
		short cluster_index = 0;
		short const cluster_count = (short)structure_bsp->clusters.count;

		if (cluster_index < cluster_count)
		{
			long element_index = 0;
			do
			{
				struct structure_cluster_runtime_decals *cluster = TAG_BLOCK_GET_ELEMENT(
					clusters,
					element_index,
					struct structure_cluster_runtime_decals);

				if (cluster->first_decal_index != NONE && cluster->decal_count)
				{
					decals_delete_permanent_from_cluster(cluster_index);
				}

				cluster_index++;
				element_index++;
			} while (cluster_index < cluster_count);
		}
	}

	return;
}

void structure_decals_dispose_from_old_map(
	void)
{
	return;
}

void structure_decals_dispose(
	void)
{
	return;
}

void structure_decals_update(
	unsigned long *old_combined_pvs,
	unsigned long *new_combined_pvs,
	short cluster_count)
{
	struct structure_bsp *structure_bsp = global_structure_bsp_get();
	struct tag_block *runtime_decals;
	short cluster_index;

	match_assert("c:\\halo\\SOURCE\\structures\\structure_runtime_decals.c", 0x62, structure_decals_globals);
	runtime_decals = &structure_bsp->runtime_decals;

	if (runtime_decals->count && cluster_count > (cluster_index = 0))
	{
		struct tag_block *clusters = &structure_bsp->clusters;
		long element_index = 0;

		do
		{
			struct structure_cluster_runtime_decals *cluster = TAG_BLOCK_GET_ELEMENT(
				clusters,
				element_index,
				struct structure_cluster_runtime_decals);
			boolean cluster_has_decals = cluster->first_decal_index != NONE && cluster->decal_count;
			boolean delete_decals = cluster_has_decals &&
				!structure_decals_globals->reconnect_to_structure_bsp &&
				BIT_VECTOR_TEST_FLAG(old_combined_pvs, element_index) &&
				!BIT_VECTOR_TEST_FLAG(new_combined_pvs, element_index);
			boolean create_decals = cluster_has_decals &&
				(!BIT_VECTOR_TEST_FLAG(old_combined_pvs, element_index) ||
					structure_decals_globals->reconnect_to_structure_bsp) &&
				BIT_VECTOR_TEST_FLAG(new_combined_pvs, element_index);

			if (delete_decals)
			{
				decals_delete_permanent_from_cluster(cluster_index);
			}
			else if (create_decals)
			{
				long decal_index = 0;
				if (decal_index < cluster->decal_count)
				{
					do
					{
						struct structure_runtime_decal *runtime_decal = TAG_BLOCK_GET_ELEMENT(
							runtime_decals,
							cluster->first_decal_index + decal_index,
							struct structure_runtime_decal);
						struct scenario_decal_palette_entry *palette_entry = TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->decal_palette,
							runtime_decal->palette_index,
							struct scenario_decal_palette_entry);
						long definition_index = palette_entry->reference.index;

						decal_definition_get(definition_index);
						{
							real_euler_angles2d angles =
							{
								runtime_decal->yaw * (_pi / 127.f),
								runtime_decal->pitch * (_pi / 254.f),
							};
							real_vector3d normal;

							vector3d_from_euler_angles2d(&normal, &angles);
							decal_new(definition_index, &runtime_decal->position, &normal, 1.0f, 1, NONE, FALSE);
						}
						decal_index++;
					} while (decal_index < cluster->decal_count);
				}
			}

			cluster_index++;
			element_index++;
		} while (cluster_index < cluster_count);
	}

	structure_decals_globals->reconnect_to_structure_bsp = FALSE;

	return;
}

/* ---------- private code */
