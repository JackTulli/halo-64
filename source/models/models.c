/*
MODELS.C

symbols in this file:
00112DB0 0430:
	_render_model_parts (0000)
001131E0 0110:
	_model_interpolate_node_orientations (0000)
001132F0 0090:
	_model_get_node_orientations (0000)
00113380 0140:
	_model_get_node_matrices (0000)
001134C0 0110:
	_model_node_matrices_from_orientations (0000)
001135D0 00a0:
	_model_find_marker (0000)
00113670 0030:
	_model_get_default_inverse_matrix (0000)
001136A0 0070:
	_model_find_node (0000)
00113710 0010:
	_model_geometry_part_build_tangent_matrices (0000)
00113720 0860:
	_render_model (0000)
00113F80 01d0:
	_model_get_marker_by_name (0000)
00114150 0070:
	_model_build_tangent_matrices (0000)
0027F98C 000d:
	??_C@_0N@PBHIAIOK@render_model?$AA@ (0000)
0027F9A0 0061:
	??_C@_0GB@CAHPCM@part?9?$DOcentroid_secondary_node_in@ (0000)
0027FA08 005d:
	??_C@_0FN@MLJHGCND@part?9?$DOcentroid_primary_node_inde@ (0000)
0027FA68 002c:
	??_C@_0CM@MBIKHGBC@?$CBTEST_FLAG?$CIflags?0?5_render_model_@ (0000)
0027FA94 001f:
	??_C@_0BP@PJEILNMK@c?3?2halo?2SOURCE?2models?2models?4c?$AA@ (0000)
0027FAB4 001e:
	??_C@_0BO@FFDGBNA@node?9?$DOparent_node_index?$CB?$DNNONE?$AA@ (0000)
0027FAD8 0064:
	??_C@_0GE@EAOMEKOC@?$CIactual_detail_level_index?5?$DO?$DN?50?$CJ@ (0000)
0027FB3C 001e:
	??_C@_0BO@INPHBEBB@actual_detail_level_index?5?$DO?50?$AA@ (0000)
0027FB60 0060:
	??_C@_0GA@CEBIGMGH@geometry_detail_level_index?$DO?$DN0?5?$CG@ (0000)
0027FBC0 0009:
	??_C@_08OEDKDIOB@lighting?$AA@ (0000)
0027FBD0 0073:
	??_C@_0HD@NKPBNIND@object_marker?9?$DOnode_index?$DO?$DN0?5?$CG?$CG?5@ (0000)
0027FC44 0008:
	??_C@_07LGFKJAB@markers?$AA@ (0000)
0027FC4C 000e:
	??_C@_0O@BKFGBDDJ@node_matrices?$AA@ (0000)
0030A390 05f8:
	_render_model_section (0000)
00456650 0088:
	_default_function_values (0000)
	_default_render_model_change_colors (0000)
	_default_render_model_effect (0000)
	_default_render_model_region_permutation_indices (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/profile.h"
#include "models.h"

#include "model_definitions.h"

#include "game/game.h"
#include "math/real_math.h"
#include "objects/objects.h"
#include "rasterizer/rasterizer_geometry.h"
#include "render/render.h"
#include "render/render_debug.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"

/* ---------- constants */

enum
{
	NUMBER_OF_DETAIL_LEVELS_PER_MODEL = 5,
	CORTANA_MODEL_NODE_LIST_CHECKSUM = 124371095,
};

enum
{
	_scenario_cortana_hack_bit = 0,
	_scenario_demo_ui_bit,
	NUMBER_OF_SCENARIO_FLAGS
};

enum
{
	_rasterizer_geometry_no_sort_bit = 0,
	_rasterizer_geometry_no_queue_bit,
	_rasterizer_geometry_no_fog_bit,
	_rasterizer_geometry_no_zbuffer_bit,
	_rasterizer_geometry_sky_bit,
	_rasterizer_geometry_viewspace_bit,
	_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit,
	_rasterizer_geometry_first_person_bit,
	_rasterizer_geometry_parts_define_local_nodes_bit,
	NUMBER_OF_RASTERIZER_GEOMETRY_FLAGS
};

enum
{
	_render_model_immediate_bit = 0,
	_render_model_shadow_bit,
	_render_model_no_planar_fog_bit,
	_render_model_first_person_bit,
	NUMBER_OF_RENDER_MODEL_FLAGS
};

enum
{
	_render_model_pass_solid = 0,
	_render_model_pass_decal,
	_render_model_pass_transparent,
	NUMBER_OF_RENDER_MODEL_PASSES
};

enum
{
	_model_geometry_part_stripped_bit = 0,
	_model_geometry_part_local_nodes_bit,
	NUMBER_OF_MODEL_GEOMETRY_PART_FLAGS
};

enum
{
	_shader_type_screen = 0,
	_shader_type_effect,
	_shader_type_decal,
	_shader_type_environment,
	_shader_type_model,
	_shader_type_transparent_generic,
	_shader_type_transparent_chicago,
	_shader_type_transparent_water,
	_shader_type_transparent_glass,
	_shader_type_transparent_meter,
	_shader_type_transparent_plasma,
	NUMBER_OF_SHADER_TYPES
};

enum
{
	_shader_model_detail_after_reflection_bit = 0,
	_shader_model_two_sided_bit,
	_shader_model_not_alpha_tested_bit,
	_shader_model_alpha_blended_decal_bit,
	_shader_model_true_atmospheric_fog_bit,
	_shader_model_nocull_two_sided_bit,
	NUMBER_OF_SHADER_MODEL_FLAGS
};

/* ---------- macros */

/* ---------- structures */

struct model_shader_reference
{
	struct tag_reference shader;
	short permutation_index;
	word pad;
	long unused[3];
};

struct model_geometry
{
	byte reserved[0x24];
	struct tag_block parts;
};

struct model_geometry_part
{
	unsigned long flags;
	short shader_index;
	char previous_part_index;
	char next_part_index;
	short centroid_primary_node_index;
	short centroid_secondary_node_index;
	real centroid_primary_node_weight;
	real centroid_secondary_node_weight;
	real_point3d centroid;
	struct tag_block uncompressed_vertices;
	struct tag_block compressed_vertices;
	struct tag_block triangles;
	struct triangle_buffer triangle_buffer;
	struct vertex_buffer vertex_buffer;
};

typedef char verify_model_shader_reference_size[sizeof(struct model_shader_reference) == 0x20 ? 1 : -1];
typedef char verify_model_geometry_part_size[sizeof(struct model_geometry_part) == 0x68 ? 1 : -1];

struct shader_model_definition
{
	struct shader shader;
	word flags;
	short type;
	byte reserved_before_translucency[0xC];
	real translucency;
};

struct rasterizer_model_skinning
{
	real_matrix4x3 const *node_matrices;
	short node_matrix_count;
	word pad;
};

struct render_sort_filth
{
	short *previous_group_presorted_index_reference;
	short *next_group_presorted_index_reference;
	short group_index;
	short next_part_index;
	short part_index;
	word pad;
};

struct render_model_effect
{
	short type;
	word pad;
	real intensity;
	byte reserved[0x20];
};

struct rasterizer_model_begin_parameters
{
	unsigned long geometry_flags;
	long unique_identifier;
	struct rasterizer_model_skinning skinning;
	struct render_lighting lighting;
	struct render_animation animation;
	struct render_model_effect effect;
	real_point3d centroid;
	real radius;
	real_vector2d base_map_scale;
};

typedef char verify_render_model_effect_size[sizeof(struct render_model_effect) == 0x28 ? 1 : -1];
typedef char verify_rasterizer_model_begin_parameters_size[sizeof(struct rasterizer_model_begin_parameters) == 0xCC ? 1 : -1];

struct rasterizer_debug_options
{
	byte reserved[8];
	short debug_model_lod;
	byte trailing[0x5E];
};

typedef char verify_rasterizer_debug_options_size[sizeof(struct rasterizer_debug_options) == 0x68 ? 1 : -1];

/* ---------- prototypes */

#include "rasterizer/rasterizer_models.h"

static void render_model_parts(
	struct model const *model,
	char const *region_permutation_indices,
	struct rasterizer_model_skinning const *skinning,
	long object_index,
	short geometry_detail_level_index,
	short forced_shader_permutation_index,
	long flags);

/* ---------- globals */

extern struct rasterizer_debug_options rasterizer_debug_options;
extern boolean rasterizer_model_cortana_hack;

extern boolean render_model_nodes;
extern boolean render_model_markers;
extern boolean render_model_vertex_counts;
extern boolean render_model_index_counts;
extern boolean render_model_no_geometry;

static real default_function_values[MAXIMUM_FUNCTION_VALUES_PER_MODEL] = { 0 };
static real_rgb_color default_render_model_change_colors[MAXIMUM_CHANGE_COLORS_PER_MODEL] = { 0 };
static struct render_model_effect default_render_model_effect = { 0 };
static char default_render_model_region_permutation_indices[MAXIMUM_REGIONS_PER_MODEL] = { 0 };

struct profile_section render_model_section = { "render_model", NONE, TRUE };

/* ---------- private code */

static void render_model_parts(
	struct model const *model,
	char const *region_permutation_indices,
	struct rasterizer_model_skinning const *skinning,
	long object_index,
	short geometry_detail_level_index,
	short forced_shader_permutation_index,
	long flags)
{
	boolean immediate = TEST_FLAG(flags, _render_model_immediate_bit);
	short last_pass = TEST_FLAG(flags, _render_model_shadow_bit) ? _render_model_pass_solid : _render_model_pass_transparent;
	struct render_sort_filth sort_filth[MAXIMUM_PARTS_PER_MODEL_GEOMETRY];
	real_point3d centroid;
	short pass;

	for (pass = _render_model_pass_solid; pass<=last_pass; pass++)
	{
		short sort_filth_count = 0;
		short region_index;
		short i, j;

		for (region_index = 0; region_index<model->regions.count; region_index++)
		{
			struct model_region *region = TAG_BLOCK_GET_ELEMENT(&model->regions, region_index, struct model_region);
			char permutation_index = region_permutation_indices[region_index];

			if (permutation_index!=NONE)
			{
				struct model_region_permutation *permutation = TAG_BLOCK_GET_ELEMENT(
					&region->permutations,
					permutation_index,
					struct model_region_permutation);
				short geometry_index = permutation->geometry_indices[geometry_detail_level_index];

				if (!render_model_no_geometry && geometry_index!=NONE)
				{
					struct model_geometry *geometry = TAG_BLOCK_GET_ELEMENT(&model->geometries, geometry_index, struct model_geometry);
					short part_index;

					for (part_index = 0; part_index<geometry->parts.count; part_index++)
					{
						struct model_geometry_part *part = TAG_BLOCK_GET_ELEMENT(&geometry->parts, part_index, struct model_geometry_part);
						struct model_shader_reference *shader_reference = TAG_BLOCK_GET_ELEMENT(
							&model->shaders,
							part->shader_index,
							struct model_shader_reference);
						struct shader *shader = shader_definition_get(shader_reference->shader.index);

						if (shader_type_is_valid_for_model(shader->base.type) &&
							!TEST_FLAG(part->flags, _model_geometry_part_stripped_bit))
						{
							if (shader_type_is_transparent(shader->base.type))
							{
								if (pass==_render_model_pass_transparent)
								{
									match_assert("c:\\halo\\SOURCE\\models\\models.c", 442, !TEST_FLAG(flags, _render_model_shadow_bit));
									match_assert(
										"c:\\halo\\SOURCE\\models\\models.c",
										445,
										part->centroid_primary_node_index>=0 && part->centroid_primary_node_index<model->nodes.count);
									match_assert(
										"c:\\halo\\SOURCE\\models\\models.c",
										446,
										part->centroid_secondary_node_index>=0 && part->centroid_secondary_node_index<model->nodes.count);

									matrix4x3_transform_point(
										&skinning->node_matrices[part->centroid_primary_node_index],
										&part->centroid,
										&centroid);
									rasterizer_model_transparent_geometry_submit(
										shader,
										forced_shader_permutation_index ? forced_shader_permutation_index : shader_reference->permutation_index,
										&part->triangle_buffer,
										NONE,
										part->triangle_buffer.count,
										&part->vertex_buffer,
										NONE,
										&centroid,
										&sort_filth[sort_filth_count]);

									if (sort_filth_count<MAXIMUM_PARTS_PER_MODEL_GEOMETRY &&
										sort_filth[sort_filth_count].group_index!=NONE &&
										!immediate &&
										(part->next_part_index>0 || part->previous_part_index>0))
									{
										sort_filth[sort_filth_count].part_index = part_index;
										sort_filth[sort_filth_count].next_part_index = part->next_part_index;
										sort_filth_count++;
									}
								}
							}
							else if (shader->base.type==_shader_type_model &&
								TEST_FLAG(((struct shader_model_definition *)shader_get_and_verify_type(shader, _shader_type_model))->flags, _shader_model_alpha_blended_decal_bit))
							{
								if (pass==_render_model_pass_decal)
								{
									match_assert("c:\\halo\\SOURCE\\models\\models.c", 491, !TEST_FLAG(flags, _render_model_shadow_bit));

									rasterizer_model_draw(
										shader,
										forced_shader_permutation_index ? forced_shader_permutation_index : shader_reference->permutation_index,
										&part->triangle_buffer,
										NONE,
										part->triangle_buffer.count,
										&part->vertex_buffer,
										NONE);
								}
							}
							else if (pass==_render_model_pass_solid)
							{
								if (TEST_FLAG(flags, _render_model_shadow_bit))
								{
									rasterizer_environment_shadow_model_draw(
										shader,
										forced_shader_permutation_index ? forced_shader_permutation_index : shader_reference->permutation_index,
										&part->triangle_buffer,
										&part->vertex_buffer);
								}
								else
								{
									rasterizer_model_draw(
										shader,
										forced_shader_permutation_index ? forced_shader_permutation_index : shader_reference->permutation_index,
										&part->triangle_buffer,
										NONE,
										part->triangle_buffer.count,
										&part->vertex_buffer,
										NONE);
									rasterizer_debug_model_vertices(object_index, skinning, part);
								}
							}
						}
					}
				}
			}
		}

		for (i = 0; i<sort_filth_count; i++)
		{
			for (j = 0; j<sort_filth_count; j++)
			{
				if (sort_filth[i].next_part_index==sort_filth[j].part_index && sort_filth[i].next_part_index>0)
				{
					*sort_filth[i].next_group_presorted_index_reference = sort_filth[j].group_index;
					*sort_filth[j].previous_group_presorted_index_reference = sort_filth[i].group_index;
					break;
				}
			}
		}
	}

	return;
}

/* ---------- public code */

void model_interpolate_node_orientations(
	struct model const *model,
	struct real_orientation *original_node_orientations,
	struct real_orientation *target_node_orientations,
	short frame_index,
	short frame_count)
{
	real fraction = (real)(frame_index + 1) / (real)frame_count;
	real inverse_fraction = 1.f - fraction;
	short node_index;

	match_assert(
		"c:\\halo\\SOURCE\\models\\models.c",
		579,
		frame_count>0);
	match_assert(
		"c:\\halo\\SOURCE\\models\\models.c",
		580,
		frame_index<frame_count);

	for (node_index = 0; node_index < model->nodes.count; node_index++)
	{
		struct real_orientation *target = &target_node_orientations[node_index];
		struct real_orientation *original = &original_node_orientations[node_index];

		target->scale = original->scale * inverse_fraction + target->scale * fraction;
		quaternions_interpolate_and_normalize(
			&original->rotation,
			&target->rotation,
			fraction,
			&target->rotation);
		target->translation.x = original->translation.x * inverse_fraction + target->translation.x * fraction;
		target->translation.y = original->translation.y * inverse_fraction + target->translation.y * fraction;
		target->translation.z = original->translation.z * inverse_fraction + target->translation.z * fraction;
	}

	return;
}

void model_get_node_orientations(
	struct model const *model,
	real_orientation *node_orientations)
{
	short node_index;

	for (node_index = 0; node_index<model->nodes.count; node_index++)
	{
		struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);

		node_orientations[node_index].rotation = node->default_rotation;
		node_orientations[node_index].translation = node->default_translation;
		node_orientations[node_index].scale = 1.f;
	}

	return;
}

void model_get_node_matrices(
	struct model const *model,
	real_matrix4x3 *node_matrices,
	real_point3d const *origin,
	real_vector3d const *forward,
	real_vector3d const *up)
{
	short node_queue[MAXIMUM_NODES_PER_MODEL];
	short read_index, write_index;

	node_queue[0] = 0;
	read_index = 0;
	write_index = 1;

	while (read_index!=write_index)
	{
		short node_index = node_queue[read_index++];
		struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);
		real_matrix4x3 node_matrix;

		matrix4x3_from_point_and_quaternion(&node_matrix, &node->default_translation, &node->default_rotation);

		if (node_index == 0)
		{
			matrix4x3_from_point_and_vectors(
				&node_matrices[node_index],
				origin ? origin : global_origin3d,
				forward ? forward : global_forward3d,
				up ? up : global_up3d);
			matrix4x3_multiply(&node_matrices[node_index], &node_matrix, &node_matrices[node_index]);
		}
		else
		{
			match_assert("c:\\halo\\SOURCE\\models\\models.c", 650, node->parent_node_index!=NONE);
			matrix4x3_multiply(&node_matrices[node->parent_node_index], &node_matrix, &node_matrices[node_index]);
		}

		if (node->next_sibling_node_index!=NONE)
		{
			node_queue[write_index++] = node->next_sibling_node_index;
		}
		if (node->first_child_node_index!=NONE)
		{
			node_queue[write_index++] = node->first_child_node_index;
		}
	}

	return;
}

void model_node_matrices_from_orientations(
	struct model const *model,
	real_matrix4x3 *node_matrices,
	real_orientation const *node_orientations,
	real_point3d const *origin,
	real_vector3d const *forward,
	real_vector3d const *up)
{
	real_matrix4x3 root_matrix;

	matrix4x3_from_point_and_vectors(&root_matrix, origin, forward, up);

	if (model->nodes.count>0)
	{
		short node_queue[MAXIMUM_NODES_PER_MODEL];
		short read_index = 0;
		short write_index = 1;

		node_queue[0] = 0;

		do
		{
			short node_index = node_queue[read_index++];
			struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);
			real_matrix4x3 const *parent_matrix = node_index==0 ? &root_matrix : &node_matrices[node->parent_node_index];
			real_matrix4x3 node_matrix;

			matrix4x3_from_orientation(&node_matrix, &node_orientations[node_index]);
			matrix4x3_multiply(parent_matrix, &node_matrix, &node_matrices[node_index]);

			if (node->next_sibling_node_index!=NONE)
			{
				node_queue[write_index++] = node->next_sibling_node_index;
			}
			if (node->first_child_node_index!=NONE)
			{
				node_queue[write_index++] = node->first_child_node_index;
			}
		}
		while (read_index!=write_index);
	}

	return;
}

short model_find_marker(
	long model_index,
	char const *name)
{
	if (model_index != NONE && name && *name)
	{
		struct model *model = model_definition_get(model_index);
		short lower_bound = 0;
		short upper_bound = (short)model->markers.count - 1;

		while (lower_bound <= upper_bound)
		{
			short marker_index = (short)((lower_bound + upper_bound) / 2);
			struct model_marker *marker = TAG_BLOCK_GET_ELEMENT(
				&model->markers,
				marker_index,
				struct model_marker);
			long comparison = _stricmp(name, marker->name);

			if (comparison == 0)
			{
				return marker_index;
			}
			if (comparison < 0)
			{
				upper_bound = marker_index - 1;
			}
			else
			{
				lower_bound = marker_index + 1;
			}
		}
	}

	return NONE;
}

real_matrix4x3 *model_get_default_inverse_matrix(
	struct model *model,
	short node_index)
{
	struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);

	return &node->runtime_default_inverse_matrix;
}

short model_find_node(
	long model_index,
	char const *name)
{
	if (model_index!=NONE)
	{
		short node_index;
		struct model *model = model_definition_get(model_index);

		for (node_index = 0; node_index<model->nodes.count; node_index++)
		{
			struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);

			if (!csstrcmp(node->name, name))
			{
				return node_index;
			}
		}
	}

	return NONE;
}

short model_get_marker_by_name(
	long model_index,
	char const *name,
	byte const *region_permutations,
	short const *node_remapping_table,
	short node_count,
	real_matrix4x3 const *node_matrices,
	boolean mirrored_flag,
	struct object_marker *markers,
	short maximum_marker_count)
{
	short result = 0;
	short marker_index = model_find_marker(model_index, name);

	match_assert("c:\\halo\\SOURCE\\models\\models.c", 760, node_matrices);
	match_assert("c:\\halo\\SOURCE\\models\\models.c", 761, markers);

	if (marker_index!=NONE)
	{
		short i;

		struct model *model = model_definition_get(model_index);
		struct model_marker* marker = TAG_BLOCK_GET_ELEMENT(&model->markers, marker_index, struct model_marker);

		for (i =0; i<marker->instances.count; i++)
		{
			struct model_marker_instance* instance = TAG_BLOCK_GET_ELEMENT(&marker->instances, i, struct model_marker_instance);
			if (!region_permutations ||
				region_permutations[instance->region_index]==instance->permutation_index)
			{
				struct object_marker *object_marker;

				if (result>=maximum_marker_count)
				{
					break;
				}

				object_marker = &markers[result++];
				object_marker->node_index = node_remapping_table ? node_remapping_table[instance->node_index] : instance->node_index;
				matrix4x3_from_point_and_quaternion(&object_marker->node_matrix, &instance->translation, &instance->rotation);
				match_assert(
					"c:\\halo\\SOURCE\\models\\models.c",
					785,
					object_marker->node_index>=0 && object_marker->node_index<(node_remapping_table ? node_count : model->nodes.count));
				
				matrix4x3_multiply(&node_matrices[object_marker->node_index], &object_marker->node_matrix, &object_marker->matrix);
				if (mirrored_flag)
				{
					negate_vector3d(&object_marker->matrix.left, &object_marker->matrix.left);
				}
			}
		}
	}

	return result;
}

void model_build_tangent_matrices(
	struct model *model)
{
	short geometry_index;

	for (geometry_index = 0; geometry_index < model->geometries.count; geometry_index++)
	{
		struct model_geometry *geometry = TAG_BLOCK_GET_ELEMENT(
			&model->geometries,
			geometry_index,
			struct model_geometry);
		short part_index;

		for (part_index = 0; part_index < geometry->parts.count; part_index++)
		{
			struct model_geometry_part *part = TAG_BLOCK_GET_ELEMENT(
				&geometry->parts,
				part_index,
				struct model_geometry_part);
		}
	}

	return;
}

void model_geometry_part_build_tangent_matrices(
	struct model_geometry_part *part)
{
	return;
}

void render_model(
	long model_index,
	real level_of_detail_pixels,
	real_matrix4x3 const *node_matrices,
	char const *region_permutation_indices,
	real_rgb_color const *change_colors,
	real const *function_values,
	struct render_lighting const *lighting,
	real_point3d const *centroid,
	real radius,
	struct render_model_effect const *model_effect,
	long unique_identifier,
	short forced_shader_permutation_index,
	unsigned long flags)
{
	struct model *model = model_definition_get(model_index);

	profile_enter(render_model_section);

	match_assert("c:\\halo\\SOURCE\\models\\models.c", 82, lighting);

	if (model->node_list_checksum==CORTANA_MODEL_NODE_LIST_CHECKSUM &&
		TEST_FLAG(global_scenario_get()->flags, _scenario_cortana_hack_bit))
	{
		rasterizer_model_cortana_hack = TRUE;
	}
	else
	{
		rasterizer_model_cortana_hack = FALSE;
	}

	if (level_of_detail_pixels>=model->detail_cutoff_pixels[0] || TEST_FLAG(flags, _render_model_shadow_bit))
	{
		real_matrix4x3 relative_node_matrices[MAXIMUM_NODES_PER_MODEL];
		struct rasterizer_model_begin_parameters model_parameters;
		short geometry_detail_level_index;
		short node_index;

		if (!region_permutation_indices)
		{
			region_permutation_indices = default_render_model_region_permutation_indices;
		}
		if (!model_effect)
		{
			model_effect = &default_render_model_effect;
		}
		if (!change_colors)
		{
			change_colors = default_render_model_change_colors;
		}
		if (!function_values)
		{
			function_values = default_function_values;
		}
		if (!centroid)
		{
			centroid = &node_matrices->position;
		}

		if (node_matrices)
		{
			for (node_index = 0; node_index<model->nodes.count; node_index++)
			{
				struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);

				matrix4x3_multiply(
					&node_matrices[node_index],
					&node->runtime_default_inverse_matrix,
					&relative_node_matrices[node_index]);
			}
		}
		else
		{
			for (node_index = 0; node_index<model->nodes.count; node_index++)
			{
				relative_node_matrices[node_index] = render.frustum.world_to_view;
			}
		}

		geometry_detail_level_index = NUMBER_OF_DETAIL_LEVELS_PER_MODEL-1;
		while (geometry_detail_level_index>0 &&
			level_of_detail_pixels<model->detail_cutoff_pixels[geometry_detail_level_index])
		{
			geometry_detail_level_index--;
		}
		if (rasterizer_debug_options.debug_model_lod!=NONE)
		{
			geometry_detail_level_index = PIN(rasterizer_debug_options.debug_model_lod, 0, NUMBER_OF_DETAIL_LEVELS_PER_MODEL-1);
		}
		match_assert(
			"c:\\halo\\SOURCE\\models\\models.c",
			169,
			geometry_detail_level_index>=0 && geometry_detail_level_index<NUMBER_OF_DETAIL_LEVELS_PER_MODEL);

		if (!TEST_FLAG(flags, _render_model_shadow_bit))
		{
			if (render_model_nodes)
			{
				for (node_index = 0; node_index<model->nodes.count; node_index++)
				{
					struct model_node *node = TAG_BLOCK_GET_ELEMENT(&model->nodes, node_index, struct model_node);

					if (node->parent_node_index!=NONE)
					{
						render_debug_line(
							TRUE,
							&node_matrices[node_index].position,
							&node_matrices[node->parent_node_index].position,
							global_real_argb_white);
					}
					render_debug_matrix(TRUE, &node_matrices[node_index], 0.05f);
				}
			}

			if (render_model_markers)
			{
				short marker_index;

				for (marker_index = 0; marker_index<model->markers.count; marker_index++)
				{
					struct model_marker *marker = TAG_BLOCK_GET_ELEMENT(&model->markers, marker_index, struct model_marker);
					short instance_index;

					for (instance_index = 0; instance_index<marker->instances.count; instance_index++)
					{
						struct model_marker_instance *instance = TAG_BLOCK_GET_ELEMENT(
							&marker->instances,
							instance_index,
							struct model_marker_instance);

						if (region_permutation_indices[instance->region_index]==instance->permutation_index)
						{
							real_matrix4x3 marker_matrix;

							matrix4x3_from_point_and_quaternion(&marker_matrix, &instance->translation, &instance->rotation);
							matrix4x3_multiply(&node_matrices[instance->node_index], &marker_matrix, &marker_matrix);
							render_debug_matrix(FALSE, &marker_matrix, 0.05f);
							render_debug_string_at_point(FALSE, &marker_matrix.position, marker->name, global_real_argb_white);
						}
					}
				}
			}

			if (render_model_vertex_counts || render_model_index_counts)
			{
				short maximum_actual_detail_level_index = geometry_detail_level_index;
				boolean has_unstripped_parts = FALSE;
				short vertex_count = 0;
				short index_count = 0;
				short region_index;
				real distance;

				for (region_index = 0; region_index<model->regions.count; region_index++)
				{
					struct model_region *region = TAG_BLOCK_GET_ELEMENT(&model->regions, region_index, struct model_region);
					char permutation_index = region_permutation_indices[region_index];

					if (permutation_index!=NONE)
					{
						struct model_region_permutation *permutation = TAG_BLOCK_GET_ELEMENT(
							&region->permutations,
							permutation_index,
							struct model_region_permutation);
						short actual_detail_level_index;
						short geometry_index;

						for (actual_detail_level_index = geometry_detail_level_index+1;
							actual_detail_level_index<NUMBER_OF_DETAIL_LEVELS_PER_MODEL;
							actual_detail_level_index++)
						{
							if (permutation->geometry_indices[actual_detail_level_index]!=
								permutation->geometry_indices[geometry_detail_level_index])
							{
								break;
							}
						}
						match_assert("c:\\halo\\SOURCE\\models\\models.c", 247, actual_detail_level_index > 0);
						actual_detail_level_index--;
						match_assert(
							"c:\\halo\\SOURCE\\models\\models.c",
							249,
							(actual_detail_level_index >= 0) && (actual_detail_level_index < NUMBER_OF_DETAIL_LEVELS_PER_MODEL));
						maximum_actual_detail_level_index = MAX(maximum_actual_detail_level_index, actual_detail_level_index);

						geometry_index = permutation->geometry_indices[geometry_detail_level_index];
						if (geometry_index!=NONE)
						{
							struct model_geometry *geometry = TAG_BLOCK_GET_ELEMENT(&model->geometries, geometry_index, struct model_geometry);
							short part_index;

							for (part_index = 0; part_index<geometry->parts.count; part_index++)
							{
								struct model_geometry_part *part = TAG_BLOCK_GET_ELEMENT(&geometry->parts, part_index, struct model_geometry_part);

								vertex_count+= part->vertex_buffer.count;
								switch (part->triangle_buffer.type)
								{
								case _triangle_buffer_type_triangles:
									index_count+= 3*part->triangle_buffer.count;
									has_unstripped_parts = TRUE;
									break;
								case _triangle_buffer_type_precompiled_strip:
									index_count+= part->triangle_buffer.count+2;
									break;
								default:
									match_assert("c:\\halo\\SOURCE\\models\\models.c", 276, !"unreachable");
								}
							}
						}
					}
				}

				distance = fabs(
					render.frustum.world_to_view.forward.k*centroid->x +
					render.frustum.world_to_view.left.k*centroid->y +
					render.frustum.world_to_view.up.k*centroid->z +
					render.frustum.world_to_view.position.z);
				level_of_detail_pixels = distance/render.frustum.projection_world_to_screen.j*level_of_detail_pixels*0.5f;
				if (level_of_detail_pixels>0.0001f)
				{
					real_argb_color const *detail_level_colors[NUMBER_OF_DETAIL_LEVELS_PER_MODEL];
					real_argb_color const *color;
					char string[256];
					real_point3d point;

					detail_level_colors[0] = global_real_argb_blue;
					detail_level_colors[1] = global_real_argb_green;
					detail_level_colors[2] = global_real_argb_yellow;
					detail_level_colors[3] = global_real_argb_orange;
					detail_level_colors[4] = global_real_argb_red;
					color = detail_level_colors[maximum_actual_detail_level_index];
					if (has_unstripped_parts && (game_time_get()+model_index)%30<15)
					{
						color = global_real_argb_white;
					}

					csstrcpy(string, "");
					if (render_model_vertex_counts)
					{
						_snprintf(string+csstrlen(string), sizeof(string)-csstrlen(string), "%d", vertex_count);
					}
					if (render_model_vertex_counts && render_model_index_counts)
					{
						_snprintf(string+csstrlen(string), sizeof(string)-csstrlen(string), "/");
					}
					if (render_model_index_counts)
					{
						_snprintf(string+csstrlen(string), sizeof(string)-csstrlen(string), "%d", index_count);
					}

					set_real_point3d(&point, centroid->x, centroid->y, centroid->z+level_of_detail_pixels);
					render_debug_string_at_point(FALSE, &point, string, color);
				}
			}
		}

		model_parameters.unique_identifier = unique_identifier;
		model_parameters.lighting = *lighting;
		model_parameters.centroid = *centroid;
		model_parameters.radius = radius;
		model_parameters.effect = *model_effect;
		model_parameters.animation.colors = change_colors;
		model_parameters.animation.values = function_values;
		model_parameters.skinning.node_matrices = relative_node_matrices;
		model_parameters.skinning.node_matrix_count = model->nodes.count;
		model_parameters.geometry_flags = 0;
		model_parameters.base_map_scale = model->base_map_scale;

		if (TEST_FLAG(flags, _render_model_immediate_bit))
		{
			model_parameters.geometry_flags = FLAG(_rasterizer_geometry_no_sort_bit) |
				FLAG(_rasterizer_geometry_no_queue_bit) |
				FLAG(_rasterizer_geometry_no_fog_bit) |
				FLAG(_rasterizer_geometry_no_zbuffer_bit) |
				FLAG(_rasterizer_geometry_sky_bit);
		}
		SET_FLAG(model_parameters.geometry_flags, _rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit, TEST_FLAG(flags, _render_model_no_planar_fog_bit));
		SET_FLAG(model_parameters.geometry_flags, _rasterizer_geometry_first_person_bit, TEST_FLAG(flags, _render_model_first_person_bit));

		if (TEST_FLAG(flags, _render_model_shadow_bit))
		{
			rasterizer_environment_shadow_model_begin(&model_parameters);
		}
		else
		{
			rasterizer_model_begin(&model_parameters, FALSE);
		}
		render_model_parts(
			model,
			region_permutation_indices,
			&model_parameters.skinning,
			unique_identifier,
			geometry_detail_level_index,
			forced_shader_permutation_index,
			flags);
		if (TEST_FLAG(flags, _render_model_shadow_bit))
		{
			rasterizer_environment_shadow_model_end();
		}
		else
		{
			rasterizer_model_end();
		}
	}

	rasterizer_model_cortana_hack = FALSE;

	profile_exit(render_model_section);

	return;
}
