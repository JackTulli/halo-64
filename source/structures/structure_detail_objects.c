/*
STRUCTURE_DETAIL_OBJECTS.C

symbols in this file:
00183160 0020:
	_calculate_world_from_cell_index_and_offset (0000)
00183180 0040:
	_get_local_player_datum (0000)
001831C0 0050:
	_structure_detail_objects_initialize (0000)
00183210 0010:
	_structure_detail_objects_dispose_from_old_map (0000)
00183220 0010:
	_structure_detail_objects_dispose (0000)
00183230 0010:
	_structure_detail_objects_flush (0000)
00183240 0030:
	_detail_object_offset (0000)
00183270 0030:
	_key_compare_cells_lower_bound (0000)
001832A0 0030:
	_key_compare_cells_upper_bound (0000)
001832D0 0080:
	_get_lower_bound_cell (0000)
00183350 0080:
	_get_upper_bound_cell (0000)
001833D0 0030:
	_dot_product4d (0000)
00183400 0050:
	_structure_detail_objects_initialize_for_new_map (0000)
00183450 0470:
	_structure_render_detail_objects (0000)
001838C0 02f0:
	_render_debug_detail_objects (0000)
002A12BC 0016:
	??_C@_0BG@BEHEKGEA@local_player_index?$DN?$DN0?$AA@ (0000)
002A12D4 0035:
	??_C@_0DF@DGEAAFAE@c?3?2halo?2SOURCE?2structures?2struct@ (0000)
002A130C 0019:
	??_C@_0BJ@IOENOFIB@structure?5detail?5objects?$AA@ (0000)
002A1328 0022:
	??_C@_0CC@GPGHNCNI@detail_object_global_runtime_dat@ (0000)
002A134C 0023:
	??_C@_0CD@OMDCMBBI@lower_bound_cell?$DM?$DNupper_bound_ce@ (0000)
004C0CBC 0014:
	_debug_detail_objects (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#define dot_product4d dot_product4d_inline
#include "math/real_math.h"
#undef dot_product4d
#include "game/players.h"
#include "render/render.h"
#include "render/render_debug.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "structures/structure_bsp_definitions.h"
#include "tag_files/tag_groups.h"
#include "rasterizer/rasterizer.h"
#include "structures.h"

#undef memset
#pragma intrinsic(memset)
#pragma intrinsic(abs)

/* ---------- constants */

/* ---------- macros */

#define structure_detail_object_data_get(block, index) \
	TAG_BLOCK_GET_ELEMENT((block), (index), struct structure_detail_object_data)
#define detail_object_cell_definition_get(block, index) \
	TAG_BLOCK_GET_ELEMENT((block), (index), struct detail_object_cell_definition)
#define detail_object_count_get(block, index) \
	TAG_BLOCK_GET_ELEMENT((block), (index), word)
#define detail_object_z_reference_vector_get(block, index) \
	TAG_BLOCK_GET_ELEMENT((block), (index), real_vector4d)
#define detail_object_get(block, index) \
	TAG_BLOCK_GET_ELEMENT((block), (index), struct detail_object)

/* ---------- structures */

struct detail_object_cell_coordinate
{
	short x;
	short y;
	short z;
	boolean initialized;
	byte pad07;
};

struct detail_object_cell_definition
{
	short cell_x;
	short cell_y;
	short cell_z;
	short offset_z;
	unsigned long valid_layers;
	long start_index;
	long count_index;
	long unused14[3];
};

struct detail_object_cell_data
{
	long first_detail_object_index;
	long detail_object_count;
	short cell_x;
	short cell_y;
	real cell_z;
	long first_vertex_index;
	real_vector4d *z_reference_vector;
};

struct detail_object_layer_data
{
	struct detail_object_cell_data *cells;
	short cell_count;
	short collection_definition_index;
};

struct detail_object_view_data
{
	struct detail_object_layer_data *layers;
	short layer_count;
	word pad06;
};

struct detail_object_runtime_data
{
	struct detail_object_cell_data cells[32][27];
	struct detail_object_layer_data layers[32];
	struct detail_object_view_data view_data;
	struct detail_object_cell_coordinate cell_coordinate;
};

struct detail_object_global_runtime_data
{
	struct detail_object_runtime_data local_player_data[2];
	real_vector4d default_z_reference_vector;
};

struct structure_detail_object_data
{
	struct tag_block cells;
	struct tag_block detail_objects;
	struct tag_block counts;
	struct tag_block z_reference_vectors;
	byte valid;
	byte pad31[3];
	long unused34[3];
};

struct detail_object
{
	byte position[3];
	byte data;
	word color;
};

typedef char detail_object_cell_coordinate_size[
	sizeof(struct detail_object_cell_coordinate) == 0x8 ? 1 : -1];
typedef char detail_object_cell_definition_size[
	sizeof(struct detail_object_cell_definition) == 0x20 ? 1 : -1];
typedef char detail_object_cell_data_size[
	sizeof(struct detail_object_cell_data) == 0x18 ? 1 : -1];
typedef char detail_object_layer_data_size[
	sizeof(struct detail_object_layer_data) == 0x8 ? 1 : -1];
typedef char detail_object_view_data_size[
	sizeof(struct detail_object_view_data) == 0x8 ? 1 : -1];
typedef char detail_object_runtime_data_size[
	sizeof(struct detail_object_runtime_data) == 0x5210 ? 1 : -1];
typedef char detail_object_global_runtime_data_size[
	sizeof(struct detail_object_global_runtime_data) == 0xA430 ? 1 : -1];
typedef char structure_detail_object_data_size[
	sizeof(struct structure_detail_object_data) == 0x40 ? 1 : -1];
typedef char detail_object_size[
	sizeof(struct detail_object) == 0x6 ? 1 : -1];

/* ---------- prototypes */

static real calculate_world_from_cell_index_and_offset(
	real cell_index,
	real offset);
static struct detail_object_runtime_data *get_local_player_datum(
	short local_player_index);
static boolean key_compare_cells_lower_bound(
	struct detail_object_cell_coordinate const *key,
	struct detail_object_cell_definition const *cell);
static boolean key_compare_cells_upper_bound(
	struct detail_object_cell_coordinate const *key,
	struct detail_object_cell_definition const *cell);
static struct detail_object_cell_definition *get_lower_bound_cell(
	struct detail_object_cell_definition *begin,
	struct detail_object_cell_definition *end,
	struct detail_object_cell_coordinate const *key);
static struct detail_object_cell_definition *get_upper_bound_cell(
	struct detail_object_cell_definition *begin,
	struct detail_object_cell_definition *end,
	struct detail_object_cell_coordinate const *key);

/* ---------- globals */

boolean debug_detail_objects = FALSE;

static struct detail_object_global_runtime_data *detail_object_global_runtime_data = NULL;
static boolean fudge_vector = FALSE;
static real fudge_offset = 0.0f;
static real final_offset = 0.0f;

/* ---------- public code */

void structure_detail_objects_initialize(
	void)
{
	detail_object_global_runtime_data = game_state_malloc(
		"structure detail objects",
		NULL,
		sizeof(*detail_object_global_runtime_data));
	detail_object_global_runtime_data->default_z_reference_vector.i = 0.0f;
	detail_object_global_runtime_data->default_z_reference_vector.j = 0.0f;
	detail_object_global_runtime_data->default_z_reference_vector.k = 1.0f;
	detail_object_global_runtime_data->default_z_reference_vector.l = 0.0f;

	return;
}

void structure_detail_objects_dispose_from_old_map(
	void)
{
	return;
}

void structure_detail_objects_dispose(
	void)
{
	return;
}

void structure_detail_objects_flush(
	void)
{
	detail_object_global_runtime_data->local_player_data[0].cell_coordinate.initialized = FALSE;

	return;
}

void detail_object_offset(
	real offset)
{
	final_offset = offset;
	fudge_vector = TRUE;
	fudge_offset = offset - fudge_offset;

	return;
}

/* ---------- private code */

static real calculate_world_from_cell_index_and_offset(
	real cell_index,
	real offset)
{
	return (offset * (1.0f / 255.0f) + cell_index) * 8.0f;
}

static struct detail_object_runtime_data *get_local_player_datum(
	short local_player_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structure_detail_objects.c",
		0x56,
		local_player_index==0);

	return &detail_object_global_runtime_data->local_player_data[local_player_index];
}

static boolean key_compare_cells_lower_bound(
	struct detail_object_cell_coordinate const *key,
	struct detail_object_cell_definition const *cell)
{
	return cell->cell_x < key->x ||
		(cell->cell_x == key->x &&
			(cell->cell_y < key->y ||
				(cell->cell_y == key->y && cell->cell_z < key->z)));
}

static boolean key_compare_cells_upper_bound(
	struct detail_object_cell_coordinate const *key,
	struct detail_object_cell_definition const *cell)
{
	return cell->cell_x > key->x ||
		(cell->cell_x == key->x &&
			(cell->cell_y > key->y ||
				(cell->cell_y == key->y && cell->cell_z > key->z)));
}

static struct detail_object_cell_definition *get_lower_bound_cell(
	struct detail_object_cell_definition *begin,
	struct detail_object_cell_definition *end,
	struct detail_object_cell_coordinate const *key)
{
	long count = end - begin;

	while (count > 0)
	{
		long half = count / 2;
		struct detail_object_cell_definition *middle = begin + half;

		if (key_compare_cells_lower_bound(key, middle))
		{
			begin = middle + 1;
			count -= half + 1;
		}
		else
		{
			count = half;
		}
	}

	return begin;
}

static struct detail_object_cell_definition *get_upper_bound_cell(
	struct detail_object_cell_definition *begin,
	struct detail_object_cell_definition *end,
	struct detail_object_cell_coordinate const *key)
{
	long count = end - begin;

	while (count > 0)
	{
		long half = count / 2;
		struct detail_object_cell_definition *middle = begin + half;

		if (!key_compare_cells_upper_bound(key, middle))
		{
			begin = middle + 1;
			count -= half + 1;
		}
		else
		{
			count = half;
		}
	}

	return begin;
}

real dot_product4d(
	real_vector4d const *a,
	real_vector4d const *b)
{
	return a->i*b->i + a->j*b->j + a->k*b->k + a->l*b->l;
}

void structure_detail_objects_initialize_for_new_map(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\structures\\structure_detail_objects.c",
		0x6D,
		detail_object_global_runtime_data);
	csmemset(
		detail_object_global_runtime_data,
		0,
		sizeof(*detail_object_global_runtime_data));
	detail_object_global_runtime_data->local_player_data[0].cell_coordinate.initialized = FALSE;

	return;
}

void structure_render_detail_objects(
	void)
{
	if (local_player_count() == 1 && render.local_player_index != NONE)
	{
		struct structure_detail_object_data *detail_object_data =
			global_structure_bsp_get()->detail_object_data.count ?
				structure_detail_object_data_get(&global_structure_bsp_get()->detail_object_data, 0) :
				NULL;
		struct detail_object_runtime_data *local_player_data = get_local_player_datum(0);
		struct detail_object_cell_coordinate camera_cell = {
			(short)fast_ftol(render.camera.position.x * 0.125f - 0.5f),
			(short)fast_ftol(render.camera.position.y * 0.125f - 0.5f),
			(short)fast_ftol(render.camera.position.z * 0.125f - 0.5f) };

		if (detail_object_data->valid)
		{
			rasterizer_detail_objects_begin();

			if (camera_cell.x != local_player_data->cell_coordinate.x ||
				camera_cell.y != local_player_data->cell_coordinate.y ||
				camera_cell.z != local_player_data->cell_coordinate.z ||
				!local_player_data->cell_coordinate.initialized ||
				TEST_FLAG(detail_object_data->valid, 1))
			{
				unsigned long visible_layer_flags = 0;
				short layer_cell_counts[32] = { 0 };
				short x_delta;
				short y_delta;

				detail_object_data->valid = TRUE;
				local_player_data->cell_coordinate = camera_cell;
				local_player_data->cell_coordinate.initialized = TRUE;

				for (x_delta = -1; x_delta <= 1; x_delta++)
				{
					for (y_delta = -1; y_delta <= 1; y_delta++)
					{
						struct detail_object_cell_definition *begin =
							detail_object_cell_definition_get(&detail_object_data->cells, 0);
						struct detail_object_cell_definition *end = detail_object_cell_definition_get(
							&detail_object_data->cells,
							detail_object_data->cells.count - 1) + 1;
						struct detail_object_cell_coordinate key = {
							(short)(camera_cell.x - x_delta),
							(short)(camera_cell.y - y_delta),
							camera_cell.z };
						struct detail_object_cell_definition *lower_bound_cell;
						struct detail_object_cell_definition *upper_bound_cell;

						key.z--;
						lower_bound_cell = get_lower_bound_cell(begin, end, &key);
						key.z += 3;
						upper_bound_cell = get_upper_bound_cell(begin, end, &key) - 1;
						key.z = camera_cell.z;

						if (lower_bound_cell->cell_x == key.x &&
							lower_bound_cell->cell_y == key.y &&
							upper_bound_cell->cell_x == key.x &&
							upper_bound_cell->cell_y == key.y)
						{
							struct detail_object_cell_definition *cell;

							match_assert(
								"c:\\halo\\SOURCE\\structures\\structure_detail_objects.c",
								0xC6,
								lower_bound_cell<=upper_bound_cell);

							upper_bound_cell++;
							for (cell = lower_bound_cell; cell < upper_bound_cell; cell++)
							{
								if (abs(camera_cell.z - cell->cell_z) <= 1)
								{
									long first_detail_object_index = 0;
									short count_index = 0;
									short layer_index;

									visible_layer_flags |= cell->valid_layers;
									for (layer_index = 0; layer_index < 32; layer_index++)
									{
										if (TEST_FLAG(cell->valid_layers, layer_index))
										{
											struct detail_object_cell_data *cell_data =
												&local_player_data->cells[layer_index][layer_cell_counts[layer_index]++];

											cell_data->cell_x = cell->cell_x;
											cell_data->cell_y = cell->cell_y;
											cell_data->cell_z =
												(real)cell->offset_z * (1.0f / 255.0f) + (real)cell->cell_z;
											cell_data->first_detail_object_index =
												cell->start_index + first_detail_object_index;
											cell_data->detail_object_count = *detail_object_count_get(
												&detail_object_data->counts,
												cell->count_index + count_index);
											cell_data->z_reference_vector = detail_object_data->z_reference_vectors.count ?
												detail_object_z_reference_vector_get(
													&detail_object_data->z_reference_vectors,
													cell->count_index + count_index) :
												&detail_object_global_runtime_data->default_z_reference_vector;

											first_detail_object_index += cell_data->detail_object_count;
											count_index++;
										}
									}
								}
							}
						}
					}
				}

				{
					short render_layer_index = 0;
					short layer_index;

					local_player_data->view_data.layers = local_player_data->layers;
					local_player_data->view_data.layer_count = 0;
					for (layer_index = 0; layer_index < 32; layer_index++)
					{
						if (TEST_FLAG(visible_layer_flags, layer_index) && layer_cell_counts[layer_index])
						{
							struct detail_object_layer_data *layer =
								&local_player_data->layers[render_layer_index++];

							layer->cells = local_player_data->cells[layer_index];
							layer->cell_count = layer_cell_counts[layer_index];
							layer->collection_definition_index = layer_index;
							local_player_data->view_data.layer_count++;
						}
					}

					rasterizer_detail_objects_rebuild_vertices(
						&local_player_data->view_data);
				}
			}

			rasterizer_detail_objects_draw(&local_player_data->view_data);
			rasterizer_detail_objects_end();
		}
	}

	return;
}

void render_debug_detail_objects(
	void)
{
	if (local_player_count() == 1 &&
		render.local_player_index != NONE &&
		debug_detail_objects)
	{
		struct structure_detail_object_data *detail_object_data;
		struct detail_object_runtime_data *local_player_data;

		if (global_structure_bsp_get()->detail_object_data.count)
		{
			detail_object_data = structure_detail_object_data_get(
				&global_structure_bsp_get()->detail_object_data,
				0);
		}
		else
		{
			detail_object_data = NULL;
		}

		local_player_data = get_local_player_datum(0);
		if (debug_detail_objects)
		{
			struct detail_object_view_data *view_data = &get_local_player_datum(0)->view_data;
			short layer_index;

			for (layer_index = 0; layer_index < view_data->layer_count; layer_index++)
			{
				struct detail_object_layer_data *layer = &local_player_data->layers[layer_index];
				short cell_index;

				for (cell_index = 0; cell_index < layer->cell_count; cell_index++)
				{
					struct detail_object_cell_data *cell = &layer->cells[cell_index];
					boolean clipped = FALSE;
					long detail_object_index;

					if (fudge_vector)
					{
						cell->z_reference_vector->l +=
							fudge_offset * 0.125f;
					}
					for (detail_object_index = 0;
						detail_object_index < cell->detail_object_count;
						detail_object_index++)
					{
						struct detail_object *detail_object = detail_object_get(
							&detail_object_data->detail_objects,
							cell->first_detail_object_index + detail_object_index);
						real_vector3d detail_object_position;
						real_point3d position;

						detail_object_position.i = (real)detail_object->position[0];
						detail_object_position.j = (real)detail_object->position[1];
						detail_object_position.k = (real)detail_object->position[2];

						position.x = calculate_world_from_cell_index_and_offset(
							(real)cell->cell_x,
							(real)detail_object->position[0]);
						position.y = calculate_world_from_cell_index_and_offset(
							(real)cell->cell_y,
							(real)detail_object->position[1]);
						position.z =
							((detail_object_position.i * cell->z_reference_vector->i +
								detail_object_position.j * cell->z_reference_vector->j +
								detail_object_position.k * cell->z_reference_vector->k) *
								(1.0f / 255.0f) + cell->z_reference_vector->l +
								cell->cell_z) * 8.0f;

						if (position.z > (cell->cell_z + 1.0f) * 8.0f ||
							position.z < cell->cell_z * 8.0f)
						{
							clipped = TRUE;
						}

						render_debug_point(
							TRUE,
							&position,
							0.1f,
							global_real_argb_red);
					}

					{
						real_rectangle3d bounds = {
							(real)(cell->cell_x * 8),
							(real)(cell->cell_x * 8 + 8),
							(real)(cell->cell_y * 8),
							(real)(cell->cell_y * 8 + 8),
							cell->cell_z * 8.0f,
							(cell->cell_z + 1.0f) * 8.0f };

						render_debug_box_outline(TRUE, &bounds, global_real_argb_blue);

						if (clipped)
						{
							real_argb_color clipped_color = *global_real_argb_grey;

							clipped_color.alpha = 0.3f;
							render_debug_box_outline(TRUE, &bounds, &clipped_color);
						}
					}
				}
			}
		}

		fudge_vector = FALSE;
		fudge_offset = final_offset;
	}

	return;
}
