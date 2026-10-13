/*
AI_DEBUG.C
*/

/* ---------- headers */

#include "cseries.h"
#include "ai_debug.h"

#include "actions.h"
#include "actors.h"
#include "actor_definitions.h"
#include "actor_types.h"
#include "ai.h"
#include "ai_communication.h"
#include "ai_profile.h"
#include "ai_scenario_definitions.h"
#include "encounters.h"
#include "props.h"

#include "camera/director.h"
#include "camera/observer.h"
#include "editor/editor_stubs.h"
#include "game/game.h"
#include "game/player_control.h"
#include "game/players.h"
#include "memory/data.h"
#include "objects/damage.h"
#include "physics/collision_bsp_definitions.h"
#include "physics/collisions.h"
#include "rasterizer/rasterizer.h"
#include "render/render_debug.h"
#include "render/render_debug_geometry.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "structures/structure_bsp_definitions.h"
#include "text/draw_string.h"
#include "units/bipeds.h"
#include "units/dialogue_definitions.h"
#include "units/biped_definitions.h"
#include "units/units.h"
#include "units/unit_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* INFERRED FROM JANUARY'S BYTES - not attested in any surviving source.
 * The label for an attractor node is drawn 0.15 world units above the node's
 * point.  January's bytes require the offset to be a parenthesised group at
 * the call, which a bare `point_height+0.15f` argument does not produce; a
 * whole-argument parenthesised SUM occurs nowhere else in the tree, so the
 * group is expressed here as a named macro rather than as loose parentheses.
 * Measured on _ai_debug_render_path_node with everything else held constant:
 *
 *     point_height+0.15f                    residual [sha]
 *     (point_height+0.15f)                  EXACT
 *     ((point_height)+(0.15f))              EXACT
 *     ai_debug_attractor_label_height(...)  EXACT   <- this spelling
 *
 * The macro also names what the 0.15f is, which the bare sum did not.
 */
#define ai_debug_attractor_label_height(height)		\
	((height)+0.15f)

#define actor_debug_print_threat(actor, threat_type, string, color)		\
if (actor->situation.specific_threats[threat_type])						\
{																		\
	render_debug_string_at_point(										\
		TRUE,															\
		ai_debug_drawstack(),											\
		csprintf(														\
			temporary,													\
			string,														\
			(actor)->situation.specific_threats[threat_type],			\
			(actor)->situation.cumulative_threats[threat_type]),		\
		color);															\
}

/* ---------- structures */

struct ai_debug_speech_list
{
	char const *name;
	short vocalization_type;
	boolean all;
};

struct ai_debug_spatial_effect
{
	short type;
	short field_02;
	real_point3d point;
	long time;
};

struct ai_debug_enterable_vehicle
{
	long object_index;
	real distance;
	short team_flags;
	short actor_type_flags;
	short actor_count;
	word pad;
	long actor_index[6];
};

struct ai_debug_globals_view
{
	boolean ai_active;
	boolean initialized_for_new_map;
	char __unknown02[0x12E];
	short spatial_effect_head;
	short spatial_effect_tail;
	struct ai_debug_spatial_effect spatial_effect[32];
	boolean field_3B4;
	char __unknown3B5[1];
	short enterable_vehicle_count;
	struct ai_debug_enterable_vehicle enterable_vehicle[32];
	short field_8B8;
	char __unknown8BA[0x22];
};
struct encounter_actor_iterator
{
	long encounter_index;
	long actor_index;
	long next_actor_index;
};

struct actor_iterator
{
	struct data_iterator encounter_iterator;
	boolean encounterless_actors_done;
	boolean active_only;
	word pad;
	long actor_index;
	long next_actor_index;
};

typedef char ai_debug_enterable_vehicle_size_assert[
	sizeof(struct ai_debug_enterable_vehicle) == 0x28 ? 1 : -1];
typedef char ai_debug_globals_spatial_effect_offset_assert[
	offsetof(struct ai_debug_globals_view, spatial_effect) == 0x134 ? 1 : -1];
typedef char ai_debug_globals_enterable_vehicle_offset_assert[
	offsetof(struct ai_debug_globals_view, enterable_vehicle) == 0x3B8 ? 1 : -1];
typedef char ai_debug_globals_size_assert[
	sizeof(struct ai_debug_globals_view) == 0x8DC ? 1 : -1];
typedef char ai_debug_actor_iterator_size_assert[
	sizeof(struct actor_iterator) == 0x1C ? 1 : -1];


/* ---------- prototypes */

void ai_debug_drawstack_setup(
	union real_point3d const *drawstack_base);
static real_point3d *ai_debug_drawstack(
	void);

static void ai_debug_highlight_unit(
	long unit_index,
	boolean render_exclusive,
	union real_argb_color const *color);

static void ai_debug_render_surface(
	struct structure_bsp const *structure_bsp,
	long surface_index,
	real offset,
	union real_argb_color const *color);

static void ai_debug_render_actor(
	long actor_index,
	boolean render_exclusive,
	long *history_start_time);
static void ai_debug_render_path_storage(
	struct path_debug_storage *path);
static void ai_debug_render_path_line(
	real_point3d const *start,
	real_argb_color const *color,
	short step_count,
	struct path_step *steps);

static void ai_debug_render_lineoffire(
	void);
static void ai_debug_render_ballistic_lineoffire(
	void);
static void ai_debug_render_lineofsight(
	void);
static void ai_debug_render_aiming_validity(
	void);
static void ai_debug_render_idle_look(
	void);
static void ai_debug_render_spatial_effects(
	void);
static void ai_debug_select_this_actor(
	void);
static long ai_debug_get_this_actor(
	void);
static void ai_debug_render_speech(
	void);
static void ai_debug_render_vehicles_enterable(
	void);
static void ai_debug_render_path(
	void);
static void ai_debug_speech_update(
	void);
static void ai_debug_path_storage_update(
	void);
static void ai_debug_render_paths_failed(
	void);
static void ai_debug_render_all_actors(
	boolean render_inactive_actors);
static void ai_debug_render_encounter(
	long encounter_index);
static void ai_debug_render_path_node(
	struct path_node *node,
	real_point3d const *previous_point,
	struct structure_bsp const *structure_bsp,
	boolean render_surfaces,
	struct path_state *state,
	struct path_node const *previous_node,
	real_argb_color const *line_color,
	real_argb_color const *surface_color,
	real_argb_color const *distance_color,
	real_argb_color const *weight_color,
	real_argb_color const *cost_color,
	real_argb_color const *attractor_color);
static void ai_debug_render_path_nodes(
	struct path_state *state,
	boolean render_surfaces,
	boolean render_all_nodes,
	boolean render_polygons,
	boolean render_costs,
	boolean render_closest);
void actor_iterator_new(
	struct actor_iterator *iterator,
	boolean active_only);
struct actor_datum *actor_iterator_next(
	struct actor_iterator *iterator);
/* ---------- globals */

struct ai_debug_state ai_debug;

extern struct ai_debug_globals_view *ai_globals;

struct actor_debug_info *actor_debug_array = NULL;
struct path_debug_storage *actor_path_debug_array = NULL;
static short global_ai_debug_path_render_id = 0;

real_point3d global_ai_debug_drawstack_next_position;
real_point3d global_ai_debug_drawstack_last_position;
real global_ai_debug_drawstack_height;
real_argb_color global_temporary_render_color;
long global_ai_debug_firing_position_color_count = NONE;

static char const *postcombat_type_strings[NUMBER_OF_ACTOR_POSTCOMBAT_ACTIONS] =
{
	"none",
	"alone",
	"unscathed",
	"wounded",
	"massacre",
	"triumph",
	"run-to",
	"check-enemy",
	"check-friend",
	"shoot-corpse",
	"celebrate"
};

static long global_ai_debug_selected_squad_index = NONE;
static long global_ai_debug_selected_squad_time = NONE;
static unsigned long global_ai_debug_activation_cluster_bit_vector[16];

const real_argb_color global_ai_debug_firing_position_colors[] =
{
	{ { 1.f, 1.f, 0.f, 1.f } },
	{ { 1.f, 0.f, 1.f, 1.f } },
	{ { 1.f, 1.f, 0.5f, 0.f } },
	{ { 1.f, 0.f, 1.f, 0.5f } },
	{ { 1.f, 0.5f, 0.f, 1.f } },
	{ { 1.f, 1.f, 0.f, 0.5f } },
	{ { 1.f, 0.5f, 1.f, 0.f } },
	{ { 1.f, 0.f, 0.5f, 1.f } },
	{ { 1.f, 0.5f, 0.f, 0.f } },
	{ { 1.f, 0.f, 0.5f, 0.f } },
	{ { 1.f, 0.f, 0.f, 0.5f } },
	{ { 1.f, 1.f, 1.f, 0.5f } },
	{ { 1.f, 1.f, 0.5f, 1.f } },
	{ { 1.f, 0.5f, 1.f, 1.f } },
	{ { 1.f, 0.5f, 0.5f, 0.f } },
	{ { 1.f, 0.f, 0.5f, 0.5f } },
	{ { 1.f, 0.5f, 0.f, 0.5f } },
	{ { REAL_MAX, REAL_MAX, REAL_MAX, REAL_MAX } }
};

/* ---------- public code */

void ai_debug_initialize(
	void)
{
	memset(&ai_debug, 0, sizeof(ai_debug));
	
	ai_debug.selected_actor_index = NONE;
	ai_debug.selected_squad_index = NONE;
	ai_debug.last_render_id = 1;
	ai_debug.render = TRUE;

	actor_debug_array = actor_debug_array==NULL ? (struct actor_debug_info *)debug_malloc(sizeof(*actor_debug_array) * AI_DEBUG_ACTOR_COUNT, FALSE, "c:\\halo\\SOURCE\\ai\\ai_debug.c", 147) : actor_debug_array;
	actor_path_debug_array = actor_path_debug_array==NULL ? (struct path_debug_storage *)debug_malloc(sizeof(*actor_path_debug_array) * AI_DEBUG_PATH_COUNT, FALSE, "c:\\halo\\SOURCE\\ai\\ai_debug.c", 148) : actor_path_debug_array;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 150, actor_debug_array && actor_path_debug_array);

	return;
}

void ai_debug_dispose(
	void)
{
	if (actor_debug_array)
	{
		debug_free(actor_debug_array, "c:\\halo\\SOURCE\\ai\\ai_debug.c", 160);
		actor_debug_array = NULL;
	}

	if (actor_path_debug_array)
	{
		debug_free(actor_path_debug_array, "c:\\halo\\SOURCE\\ai\\ai_debug.c", 166);
		actor_path_debug_array = NULL;
	}

	return;
}

void ai_debug_dispose_from_old_map(
	void)
{
	struct scenario *scenario = global_scenario_try_and_get();

	if (scenario && ai_debug.selected_squad_index!=NONE)
	{
		struct encounter_definition* encounter = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters,
			DATUM_INDEX_TO_ABSOLUTE_INDEX(ai_debug.selected_squad_index),
			struct encounter_definition);
		
		strncpy(ai_debug.selected_squad_name, encounter->name, NUMBEROF(ai_debug.selected_squad_name));
		ai_debug.selected_squad_name[NUMBEROF(ai_debug.selected_squad_name)-1] = '\0';
	}
	else
	{
		strcpy(ai_debug.selected_squad_name, "");
	}

	return;
}

void ai_debug_clear_storage(
	void)
{
	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 208, actor_debug_array);

	memset(actor_debug_array, 0, sizeof(*actor_debug_array) * AI_DEBUG_ACTOR_COUNT);
	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 211, actor_path_debug_array)

	memset(actor_path_debug_array, 0, sizeof(*actor_path_debug_array) * AI_DEBUG_PATH_COUNT);

	return;
}

void ai_debug_actor_deleted(
	long actor_index)
{
	short path_index;

	for (path_index = 0; path_index<MAXIMUM_NUMBER_OF_ACTOR_PATHS; ++path_index)
	{
		struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(path_index)];

		if (path->valid)
		{
			if (path->actor_index==actor_index)
			{
				path->valid = FALSE;
			}
		}
	}

	return;
}

struct path_debug_storage *ai_debug_get_last_path(
	long actor_index)
{
	short path_index;

	short found_path_index = NONE;
	long found_path_time = NONE;

	for (path_index = 0; path_index<MAXIMUM_NUMBER_OF_ACTOR_PATHS; ++path_index)
	{
		struct path_debug_storage const *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(path_index)];
	
		if (path->valid && path->actor_index==actor_index && path->path_time>found_path_time)
		{
			found_path_index = path_index;
			found_path_time = path->path_time;
		}
	}

	return found_path_index==NONE ? NULL : &actor_path_debug_array[AI_DEBUG_PATH_SLOT(found_path_index)];
}

boolean ai_debug_highlight_cluster(
	short index,
	real_argb_color const **highlight_color)
{
	boolean result = FALSE;

	if (ai_debug.render_encounter_activeregion && ai_debug.selected_squad_index!=NONE)
	{
		if (global_ai_debug_selected_squad_time != game_time_get() ||
			global_ai_debug_selected_squad_index != ai_debug.selected_squad_index)
		{
			encounter_compute_activation_cluster_bit_vector(
				ai_debug.selected_squad_index,
				FALSE,
				SIZEOF_BITS(global_ai_debug_activation_cluster_bit_vector),
				0,
				global_ai_debug_activation_cluster_bit_vector);
			global_ai_debug_selected_squad_time = game_time_get();
			global_ai_debug_selected_squad_index = ai_debug.selected_squad_index;
		}

		match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4133, highlight_color);
	
		if (BIT_VECTOR_TEST_FLAG(global_ai_debug_activation_cluster_bit_vector, index))
		{
			if (encounter_get(ai_debug.selected_squad_index)->active)
			{
				*highlight_color = global_real_argb_yellow;
			}
			else
			{
				*highlight_color = global_real_argb_blue;
			}
		}
		else
		{
			*highlight_color = global_real_argb_grey;
		}

		result = TRUE;
	}

	return result;
}

void ai_debug_render(
	void)
{
	if (ai_globals->initialized_for_new_map)
	{
		global_ai_debug_string_position = rasterizer_globals.reserved04.frame_bounds.y1 - 20;

		ai_debug.last_render_id = (ai_debug.last_render_id + 1)%1000;

		if (ai_debug.selected_actor_index!=NONE)
		{
			ai_debug.selected_squad_index = actor_get(ai_debug.selected_actor_index)->meta.encounter_index;
		}

		if (ai_debug.select_this_actor)
		{
			ai_debug_select_this_actor();
		}

		if (ai_debug.render)
		{
			if (ai_debug.render_lineoffire)
			{
				ai_debug_render_lineoffire();
			}

			if (ai_debug.render_lineofsight)
			{
				ai_debug_render_lineofsight();
			}

			if (ai_debug.render_ballistic_lineoffire)
			{
				ai_debug_render_ballistic_lineoffire();
			}

			if (ai_debug.selected_squad_index!=NONE)
			{
				ai_debug_render_encounter(ai_debug.selected_squad_index);
			}

			if (ai_debug.selected_actor_index!=NONE)
			{
				ai_debug_render_actor(ai_debug.selected_actor_index, TRUE, NULL);
			}

			if (ai_debug.path)
			{
				ai_debug_render_path();
			}

			if (ai_debug.render_paths_failed)
			{
				ai_debug_render_paths_failed();
			}

			if (ai_debug.render_aiming_validity)
			{
				ai_debug_render_aiming_validity();
			}

			if (ai_debug.render_all_actors)
			{
				ai_debug_render_all_actors(ai_debug.render_inactive_actors);
			}

			if (ai_debug.render_speech || ai_debug.print_speech || ai_debug.render_dialogue_variants)
			{
				ai_debug_render_speech();
			}

			if (ai_debug.render_idle_look)
			{
				ai_debug_render_idle_look();
			}

			if (ai_debug.render_spatial_effects)
			{
				ai_debug_render_spatial_effects();
			}

			if (ai_debug.render_vehicles_enterable)
			{
				ai_debug_render_vehicles_enterable();
			}
		}
	}

	return;
}

/* ---------- private code */

static void ai_debug_drawstack_setup(
	real_point3d const *drawstack_base)
{
	real_vector3d vector_to_stack;

	struct observer_result const *camera = observer_get_camera(0);

	global_ai_debug_drawstack_last_position = *drawstack_base;
	global_ai_debug_drawstack_next_position = global_ai_debug_drawstack_last_position;

	if (camera)
	{
		vector_from_points3d(&global_ai_debug_drawstack_last_position, &camera->position, &vector_to_stack);
		global_ai_debug_drawstack_height = magnitude3d(&vector_to_stack) / 40.f;
	}
	else
	{
		global_ai_debug_drawstack_height = 0.05f;
	}
	return;
}

static real_point3d *ai_debug_drawstack(
	void)
{
	global_ai_debug_drawstack_last_position = global_ai_debug_drawstack_next_position;
	global_ai_debug_drawstack_next_position.x = global_up3d->i*global_ai_debug_drawstack_height + global_ai_debug_drawstack_last_position.x;
	global_ai_debug_drawstack_next_position.y = global_up3d->j*global_ai_debug_drawstack_height + global_ai_debug_drawstack_last_position.y;
	global_ai_debug_drawstack_next_position.z = global_up3d->k*global_ai_debug_drawstack_height + global_ai_debug_drawstack_last_position.z;
	return &global_ai_debug_drawstack_last_position;
}

static void ai_debug_render_path_node(
	struct path_node *node,
	real_point3d const *previous_point,
	struct structure_bsp const *structure_bsp,
	boolean render_surfaces,
	struct path_state *state,
	struct path_node const *previous_node,
	real_argb_color const *line_color,
	real_argb_color const *surface_color,
	real_argb_color const *distance_color,
	real_argb_color const *weight_color,
	real_argb_color const *cost_color,
	real_argb_color const *attractor_color)
{
	real_point3d position;
	real height;
	real_point3d const *entry_point;

	struct observer_result const *camera = observer_get_camera(0);

	position.x = (previous_point->x + node->entry_point.x)*0.5f;
	entry_point = &node->entry_point;
	position.y = (previous_point->y + entry_point->y)*0.5f;
	position.z = (previous_point->z + entry_point->z)*0.5f;

	position.x = global_up3d->i*0.1f + position.x;
	position.y = global_up3d->j*0.1f + position.y;
	position.z = global_up3d->k*0.1f + position.z;

	if (camera)
	{
		real_vector3d v;

		height = magnitude3d(vector_from_points3d(&position, &camera->position, &v))*0.025f;
	}
	else
	{
		height = 0.05f;
	}

	if (line_color)
	{
		render_debug_line_offset(TRUE, entry_point, previous_point, line_color, 0.03f);
		render_debug_tick(TRUE, entry_point, global_up3d, 0.02f, line_color);

		if (!previous_node)
		{
			render_debug_tick(TRUE, previous_point, global_up3d, 0.02f, line_color);
		}
	}

	if (surface_color && render_surfaces)
	{
		ai_debug_render_surface(structure_bsp, node->surface_index, 0.f, surface_color);
	}

	if (distance_color)
	{
		render_debug_string_at_point(TRUE, &position,
			csprintf(temporary, "%.1f", node->linear_distance_to_entry_point),
			distance_color);

		position.x = global_up3d->i*height + position.x;
		position.y = global_up3d->j*height + position.y;
		position.z = global_up3d->k*height + position.z;
	}

	if (attractor_color && node->closest_distance_to_attractor<REAL_MAX)
	{
		real_point3d point = node->closest_point_to_attractor;
		real point_height;

		if (camera)
		{
			real_vector3d v;

			point_height = magnitude3d(vector_from_points3d(&point, &camera->position, &v))*0.025f;
		}
		else
		{
			point_height = 0.05f;
		}

		render_debug_point(TRUE, &node->closest_point_to_attractor, 0.15f, attractor_color);

		point_from_line3d(
			&point,
			global_up3d,
			ai_debug_attractor_label_height(point_height),
			&point);

		render_debug_string_at_point(TRUE, &point,
			csprintf(temporary, "%.1f", node->closest_distance_to_attractor),
			attractor_color);
	}

	if (state && state->input.attractor_valid)
	{
		real closest_distance = REAL_MAX;
		real weight = path_attractor_weight(state, entry_point, previous_point,
			&closest_distance);

		if (weight_color)
		{
			render_debug_string_at_point(TRUE, &position,
				csprintf(temporary, "%.1f", closest_distance), global_real_argb_yellow);

			position.x = global_up3d->i*height + position.x;
			position.y = global_up3d->j*height + position.y;
			position.z = global_up3d->k*height + position.z;
		}

		if (cost_color && weight>0.f)
		{
			render_debug_string_at_point(TRUE, &position,
				csprintf(temporary, "%.1f", state->input.attractor_weight),
				global_real_argb_red);
		}
	}

	return;
}

static void ai_debug_highlight_unit(
	long unit_index,
	boolean render_exclusive,
	union real_argb_color const *color)
{
	struct biped_datum *biped = biped_try_and_get(unit_index);

	if (biped)
	{
		real_point3d base;
		real width;
		real pill_height;

		struct unit_datum *unit = unit_try_and_get(biped->object.parent_object_index);

		if (unit && unit->unit.driver_object_index==unit_index)
		{
			object_get_bounding_sphere(biped->object.parent_object_index, &base, &width);
			pill_height = 0.f;
		}
		else
		{
			biped_get_physics_pill(unit_index, &base, &pill_height, &width);
		}

		if (render_exclusive && pill_height>0.f)
		{
			real_vector3d height;
			set_real_vector3d(&height, 0.f, 0.f, pill_height);
			render_debug_pill(TRUE, &base, &height, width, color);
		}
		else
		{
			render_debug_sphere(TRUE, &base, 0.75f * width, color);
		}

		if (render_exclusive)
		{
			render_debug_point(TRUE, &base, 1.8f * width, color);
		}
	}

	return;
}

static void ai_debug_render_surface(
	struct structure_bsp const *structure_bsp,
	long surface_index,
	real offset,
	union real_argb_color const *color)
{
	struct collision_bsp const *collision_bsp = TAG_BLOCK_GET_ELEMENT(&structure_bsp->collision_bsp, 0, struct collision_bsp);
	struct collision_surface const *collision_surface = TAG_BLOCK_GET_ELEMENT(&collision_bsp->surfaces, surface_index, struct collision_surface);
	long edge_index = collision_surface->first_edge_index;

	do
	{
		struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(&collision_bsp->edges, edge_index, struct collision_edge);
		const boolean next_index_belongs_to_surface = edge->surface_indices[1] == surface_index;
		struct collision_vertex const *point0 = TAG_BLOCK_GET_ELEMENT(&collision_bsp->vertices, edge->vertex_indices[0], struct collision_vertex);
		struct collision_vertex const *point1 = TAG_BLOCK_GET_ELEMENT(&collision_bsp->vertices, edge->vertex_indices[1], struct collision_vertex);

		render_debug_line_offset(TRUE, &point0->point, &point1->point, color, offset + 0.015f);
		edge_index = edge->edge_indices[next_index_belongs_to_surface];
	}
	while (edge_index!=collision_surface->first_edge_index);

	return;
}

static void ai_debug_render_actor(
	long actor_index,
	boolean render_exclusive,
	long *history_start_time)
{
	struct actor_datum* actor = actor_get(actor_index);
	struct actor_debug_info *actor_debug_info = &actor_debug_array[AI_DEBUG_ACTOR_SLOT(DATUM_INDEX_TO_ABSOLUTE_INDEX(actor_index))];

	if (actor_debug_info->last_render_id!=ai_debug.last_render_id)
	{
		struct actor_definition *actor_definition = actor_definition_get(actor->meta.definition_index);
		struct actor_variant_definition *actor_variant_definition = actor_variant_definition_get(actor->meta.variant_definition_index);
		struct path_debug_storage *path = ai_debug_get_last_path(actor_index);
		struct unit_datum *unit = NULL;
		struct unit_definition *unit_definition = NULL;

		/* Get unit info if the actor has an associated unit */

		if (actor->meta.unit_index!=NONE)
		{
			unit = unit_get(actor->meta.unit_index);
			unit_definition = unit_definition_get(unit->definition_index);
		}

		actor_debug_info->last_render_id = ai_debug.last_render_id;

		/* Set history start time */

		if (history_start_time)
		{
			if (actor->state.combat_status>=2 && actor->target.target_prop_index!=NONE)
			{
				struct prop_datum *prop = prop_get(actor->target.target_prop_index);

				if (*history_start_time==NONE || prop->last_perceived_time<*history_start_time)
				{
					*history_start_time = prop->last_perceived_time;
				}
			}
		}

		/* Stack setup */

		{
			real_point3d stack_base;

			point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &stack_base);
			ai_debug_drawstack_setup(&stack_base);
		}

		/* Unit highlighting */

		if (actor->meta.swarm)
		{
			if (actor->meta.swarm_cache_index!=NONE)
			{
				short unit_num;

				struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);

				for (unit_num = 0; unit_num<swarm->unit_count; ++unit_num)
				{
					ai_debug_highlight_unit(swarm->unit_indices[unit_num], render_exclusive, actor_action_debug_color(actor_index));
				}
			}
		}
		else
		{
			ai_debug_highlight_unit(actor->meta.unit_index, render_exclusive, actor_action_debug_color(actor_index));
		}

		/* Line of fire crouching status */

		if (ai_debug.render_lineoffire_crouching &&
			(TEST_FLAG(actor_definition->flags, _actor_definition_crouch_in_line_of_fire_bit) ||
			TEST_FLAG(actor_definition->flags, _actor_definition_avoid_friend_line_of_fire_bit)))
		{
			/* Blocking messages */

			if (actor->emotions.crouch_blocking_line_of_fire ||
				actor->emotions.crouch_friends_in_line_of_fire ||
				actor->emotions.crouch_blocking_player_line_of_fire)
			{
				char const *blocking_type;

				char const *blocking_mode = actor->emotions.crouch_friends_in_line_of_fire ? "friends-blocking" : "";

				if (actor->emotions.crouch_blocking_player_line_of_fire)
				{
					blocking_type = "blocking-player ";
				}
				else
				{
					blocking_type = actor->emotions.crouch_blocking_line_of_fire ? "blocking " : "";
				}

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "%s%s", blocking_type, blocking_mode),
					global_real_argb_orange);
			}

			/* Moving into fire */

			if (actor->emotions.moving_into_player_line_of_fire ||
				actor->emotions.moving_into_fire_timer > 0)
			{
				char const *string = actor->emotions.moving_into_player_line_of_fire ? "moving-into-fire" : "";

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "%sforce-stop %d", string, actor->emotions.moving_into_fire_timer),
					global_real_argb_pink);
			}
		}

		/* Player aiming blocked */

		if (ai_debug.render_player_aiming_blocked)
		{
			struct prop_datum *prop;
			struct prop_iterator iterator;
			real_vector3d aiming_vector;

			short blockage_type = 0;
			boolean player_prop_found = FALSE;

			prop_iterator_new(&iterator, actor_index);

			while (prop = prop_iterator_next(&iterator))
			{
				if (prop->state >= _prop_state_becoming_unacknowledged &&
					prop->state <= _prop_state_acknowledged &&
					!prop->enemy)
				{
					if (prop->player)
					{
						short v0;

						player_prop_found = TRUE;
						unit_get_aiming_vector(prop->unit_index, &aiming_vector);
						v0 = actor_perception_aiming_vector_test_blockage(
							&prop->body_position,
							&aiming_vector,
							&actor->input.position.body_position,
							NULL);

						if (blockage_type <= v0)
						{
							blockage_type = v0;
						}
					}
				}
			}

			if (player_prop_found)
			{
				switch (blockage_type)
				{
				case 0:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "not-occluding-player", global_real_argb_green);
					break;
				case 1:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "occluding-player", global_real_argb_blue);
					break;
				case 2:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "blocking-player", global_real_argb_red);
					break;
				default:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "aiming occlusion error", global_real_argb_pink);
					break;
				}

			}
		}

		/* Vector avoidance */

		if (ai_debug.render_vector_avoidance &&
			actor_debug_info->field_19C != NONE &&
			actor_debug_info->field_19C + TICKS_PER_SECOND>game_time_get())
		{
			short i;
			short j;

			/* Avoidance rays */

			if (ai_debug.render_vector_avoidance_rays)
			{
				short ray_index;
				for (ray_index = 0; ray_index<ACTOR_MAXIMUM_AVOIDANCE_RAYS; ++ray_index)
				{
					real_argb_color const *color = global_real_argb_white;

					switch (actor_debug_info->avoidance_type[ray_index])
					{
					case _actor_vector_avoidance_obstructed_object:
						color = global_real_argb_magenta;
						break;
					case _actor_vector_avoidance_obstructed_structure:
						color = global_real_argb_red;
						break;
					}

					if (actor_debug_info->avoidance_type[ray_index]>0)
					{
						real_point3d point0;
						real_point3d point1;

						point_from_line3d(
							&actor_debug_info->ray_origin[ray_index],
							&actor_debug_info->ray_direction[ray_index],
							actor_debug_info->collision_t[ray_index],
							&point0);
						point_from_line3d(
							&actor_debug_info->ray_origin[ray_index],
							&actor_debug_info->ray_direction[ray_index],
							1.f,
							&point1);
						render_debug_line(TRUE, &actor_debug_info->ray_origin[ray_index], &point0, global_real_argb_white);
						render_debug_point(TRUE, &point0, 0.2f, color);
						render_debug_line(TRUE, &point0, &point1, color);

						if (ai_debug.render_vector_avoidance_sense_t)
						{
							point_from_line3d(&point0, global_up3d, 0.25f, &point0);
							render_debug_string_at_point(
								TRUE,
								&point0,
								csprintf(temporary, "%.3f", actor_debug_info->collision_t[ray_index]),
								color);
						}
					}
					else
					{
						render_debug_vector(
							TRUE,
							&actor_debug_info->ray_origin[ray_index],
							&actor_debug_info->ray_direction[ray_index],
							1.f,
							global_real_argb_white);
					}
				}
			}

			/* Avoidance rays */

			for (i = 0; i < 8; ++i)
			{
				for (j = 0; j < 2; ++j)
				{
					if (ai_debug.render_vector_avoidance_rays)
					{
						real_argb_color const *color = global_real_argb_white;

						switch (actor_debug_info->field_62F8[i][j])
						{
						case 1:
							color = global_real_argb_aqua;
							break;
						case 2:
							color = global_real_argb_yellow;
							break;
						}

						if (actor_debug_info->field_62F8[i][j]>0)
						{
							real_point3d point0;
							real_point3d point1;

							point_from_line3d(
								&actor_debug_info->field_6358[i][j],
								&actor_debug_info->field_6418[i][j],
								actor_debug_info->avoid_t[i][j],
								&point0);
							point_from_line3d(
								&actor_debug_info->field_6358[i][j],
								&actor_debug_info->field_6418[i][j],
								1.f,
								&point1);
							render_debug_line(TRUE, &actor_debug_info->field_6358[i][j], &point0, global_real_argb_blue);
							render_debug_point(TRUE, &point0, 0.2f, color);
							render_debug_line(TRUE, &point0, &point1, color);

							if (ai_debug.render_vector_avoidance_avoid_t)
							{
								point_from_line3d(&point0, global_up3d, 0.25f, &point0);
								render_debug_string_at_point(
									TRUE,
									&point0,
									csprintf(
										temporary,
										"%.3f",
										actor_debug_info->avoid_t[i][j]),
									color);
							}
						}
						else
						{
							render_debug_vector(
								TRUE,
								&actor_debug_info->field_6358[i][j],
								&actor_debug_info->field_6418[i][j],
								1.f,
								global_real_argb_blue);

							if (ai_debug.render_vector_avoidance_clear_time)
							{
								real_point3d point;

								point_from_line3d(&actor_debug_info->field_6358[i][j], &actor_debug_info->field_6418[i][j], 0.1f, &point);
								point_from_line3d(&point, global_up3d, 0.15f, &point);
								render_debug_string_at_point(
									TRUE,
									&point,
									csprintf(
										temporary,
										"c%d",
										actor->control.vector_avoidance_clear_times[i][j]),
									global_real_argb_white);
							}
						}
					}

					/* Avoidance weights */

					if (ai_debug.render_vector_avoidance_weights && j==1)
					{
						real_point3d point;

						point_from_line3d(
							&actor_debug_info->field_6358[i][j],
							&actor_debug_info->field_6418[i][j],
							0.25f,
							&point);
						point_from_line3d(&point, global_up3d, 0.15f, &point);
						render_debug_string_at_point(
							TRUE,
							&point,
							csprintf(
							temporary,
							"%d: w%.2f",
							i,
							actor_debug_info->field_64D8[i]),
							global_real_argb_green);
					}
				}
			}

			/* Avoidance intermediate */

			if (ai_debug.render_vector_avoidance_intermediate)
			{
				real_vector3d direction;

				real_argb_color const *color = actor_debug_info->field_6550 ? global_real_argb_magenta : global_real_argb_cyan;

				render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &actor_debug_info->field_6524, 5.f, global_real_argb_yellow);
				render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &actor_debug_info->field_6530, 1.f, global_real_argb_purple);
				actor_move_get_avoidance_direction(&actor_debug_info->avoidance_data, actor_debug_info->field_6500, &direction);

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "best %.2f at %d", actor_debug_info->field_64FC, actor_debug_info->field_6500),
					color);
				render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &direction, 2.f, color);

				actor_move_get_avoidance_direction(&actor_debug_info->avoidance_data, actor_debug_info->field_6504, &direction);
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "curr ~%.2f at %.2f", actor_debug_info->field_6508, actor_debug_info->field_6504),
					global_real_argb_green);

				render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &direction, 2.0, global_real_argb_green);
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "danger %.2f turncos %.2f", actor_debug_info->sign_no_danger, actor_debug_info->field_6510),
					global_real_argb_yellow);

				if (actor_debug_info->field_6551)
				{
					actor_move_transform_avoidance_vector(&actor_debug_info->avoidance_data, &actor_debug_info->avoidance_vector, &direction);
					render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &direction, 2.0, global_real_argb_darkgreen);
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(
						temporary,
						"turn angvel %.2f danger %.2f bonus %.2f",
						actor_debug_info->field_6558,
						actor_debug_info->field_6568,
						actor_debug_info->field_6554),
						global_real_argb_darkgreen);
				}
			}

			/* Avoidance objects */

			if (ai_debug.render_vector_avoidance_objects)
			{
				short avoidance_object_index;

				for (avoidance_object_index = 0; avoidance_object_index<actor_debug_info->avoidance_data.avoidance_object_count; ++avoidance_object_index)
				{
					struct vehicle_avoidance_cylinder *cylinder = &actor_debug_info->avoidance_data.avoidance_objects[avoidance_object_index];
					real_vector3d height;
					set_real_vector3d(&height, 0.f, 0.f, cylinder->height);
					render_debug_pill(
						TRUE,
						&cylinder->base,
						&height,
						cylinder->width,
						global_real_argb_aqua);
				}
			}

			{
				boolean found_player = FALSE;

				switch (actor_debug_info->field_653C)
				{
				case 0:
					sprintf(temporary, "clear");
					break;
				case 1:
					sprintf(temporary, "sensed at %f, turn %f", actor_debug_info->field_651C, actor_debug_info->field_6520);
					found_player = TRUE;
					break;
				case 2:
					sprintf(temporary, "sign no-danger %f < %f", actor_debug_info->sign_no_danger, 1.3f);
					break;
				case 3:
					sprintf(temporary, "sign too-far cosangle %f < %f", actor_debug_info->sign_too_far_cosangle, 0.5f);
					break;
				case 4:
					sprintf(temporary, "sign rotated %f", actor_debug_info->sign_rotated);
					found_player = TRUE;
					break;
				case 5:
					sprintf(temporary, "sharp new-turn");
					found_player = TRUE;
					break;
				case 6:
					sprintf(temporary, "sharp change-dir");
					found_player = TRUE;
					break;
				case 7:
					sprintf(temporary, "sharp continue");
					found_player = TRUE;
					break;
				default:
					sprintf(temporary, "<error>");
					break;
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_orange);

				if (found_player)
				{
					render_debug_vector(TRUE, &actor_debug_info->avoidance_data.origin, &actor_debug_info->field_6540, 1.f, global_real_argb_pink);
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "move emergency %.2f", actor_debug_info->field_654C), global_real_argb_pink);
				}
			}
		}

		/* Activation */

		if (ai_debug.render_activation)
		{
			{
				char encounterbuf[256];
				real_argb_color const *encounter_color;

				if (actor->meta.encounter_index!=NONE)
				{
					struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->ai_encounters,
						DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
						struct encounter_definition);
					struct encounter_datum *encounter = encounter_get(actor->meta.encounter_index);
					boolean outside_current_bsp = FALSE;


					if (encounter_definition->runtime_structure_bsp_reference_index==NONE)
					{
						encounter_color = global_real_argb_green;
						sprintf(encounterbuf, "%s (no-bsp)", encounter_definition->name);
					}
					else
					{
						outside_current_bsp = encounter_definition->runtime_structure_bsp_reference_index!=global_structure_bsp_index;
						sprintf(encounterbuf, "%s (bsp %d)", encounter_definition->name, encounter_definition->runtime_structure_bsp_reference_index);
					}

					if (encounter->active)
					{
						encounter_color = global_real_argb_green;
					}
					else if (outside_current_bsp)
					{
						encounter_color = global_real_argb_red;
					}
					else
					{
						encounter_color = global_real_argb_purple;
					}
				}
				else
				{
					encounter_color = global_real_argb_blue;
					sprintf(encounterbuf, "encounterless");
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), encounterbuf, encounter_color);
			}

			{
				real_argb_color const *actor_color = actor_activation_debug_color(actor_index);
				long unit_index = actor->meta.unit_index;
				short unit_count = 0;
				struct swarm_datum *swarm = NULL;
				struct observer_result const *camera = observer_get_camera(0);
				unsigned long const *pvs = players_get_combined_pvs();

				if (actor->meta.swarm && actor->meta.swarm_cache_index!=NONE)
				{
					swarm = swarm_get(actor->meta.swarm_cache_index);
				}

				while (TRUE)
				{
					if (unit_index!=NONE)
					{
						real_vector3d offset_vector;
						real_point3d base_point;
						short cluster_index;

						unit_get_head_position(unit_index, &base_point);
						point_from_line3d(&base_point, global_up3d, 0.2f, &base_point);

						if (camera==NULL)
						{
							offset_vector = *global_forward3d;
						}
						else
						{
							cross_product3d(&camera->forward, global_up3d, &offset_vector);
							if (normalize3d(&offset_vector)==0.f)
							{
								offset_vector = *global_forward3d;
							}
						}

						{
							struct object_datum const *ultimate_parent = object_get(object_get_ultimate_parent(unit_index));
							cluster_index = ultimate_parent->object.location.cluster_index;
						}

						if (cluster_index==NONE)
						{
							real_point3d p0;
							real_point3d p1;
							real_point3d p2;
							real_point3d p3;

							p0.x = base_point.x+offset_vector.i*0.1f;
							p0.y = base_point.y+offset_vector.j*0.1f;
							p0.z = base_point.z+offset_vector.k*0.1f;
							p1.x = base_point.x-offset_vector.i*0.1f;
							p1.y = base_point.y-offset_vector.j*0.1f;
							p1.z = base_point.z-offset_vector.k*0.1f;
							point_from_line3d(&p0, global_up3d, 0.2f, &p2);
							point_from_line3d(&p1, global_up3d, 0.2f, &p3);

							render_debug_line(TRUE, &p0, &p2, actor_color);
							render_debug_line(TRUE, &p2, &p3, actor_color);
							render_debug_line(TRUE, &p3, &p1, actor_color);
							render_debug_line(TRUE, &p1, &p0, actor_color);
						}
						else if (BIT_VECTOR_TEST_FLAG(pvs, cluster_index))
						{
							real_point3d mid_point;
							real_point3d p0;
							real_point3d p1;
							real_point3d p2;
							real_point3d p3;

							p0 = base_point;
							point_from_line3d(&base_point, global_up3d, 0.2f, &p2);
							mid_point.x = base_point.x+global_up3d->i*0.1f;
							mid_point.y = base_point.y+global_up3d->j*0.1f;
							mid_point.z = base_point.z+global_up3d->k*0.1f;
							p1.x = mid_point.x+offset_vector.i*0.1f;
							p1.y = mid_point.y+offset_vector.j*0.1f;
							p1.z = mid_point.z+offset_vector.k*0.1f;
							p3.x = mid_point.x-offset_vector.i*0.1f;
							p3.y = mid_point.y-offset_vector.j*0.1f;
							p3.z = mid_point.z-offset_vector.k*0.1f;

							render_debug_line(TRUE, &p0, &p1, actor_color);
							render_debug_line(TRUE, &p1, &p2, actor_color);
							render_debug_line(TRUE, &p2, &p3, actor_color);
							render_debug_line(TRUE, &p3, &p0, actor_color);
						}
						else
						{
							real_point3d p0;
							real_point3d p1;
							real_point3d p2;
							real_point3d p3;

							p0.x = base_point.x+offset_vector.i*0.1f;
							p0.y = base_point.y+offset_vector.j*0.1f;
							p0.z = base_point.z+offset_vector.k*0.1f;
							p1.x = base_point.x-offset_vector.i*0.1f;
							p1.y = base_point.y-offset_vector.j*0.1f;
							p1.z = base_point.z-offset_vector.k*0.1f;
							point_from_line3d(&p0, global_up3d, 0.2f, &p2);
							point_from_line3d(&p1, global_up3d, 0.2f, &p3);

							render_debug_line(TRUE, &p0, &p3, actor_color);
							render_debug_line(TRUE, &p2, &p1, actor_color);
						}
					}

					if (!swarm || unit_count >= swarm->unit_count)
					{
						break;
					}

					unit_index = swarm->unit_indices[unit_count++];
				}
			}
		}

		/* Support surfaces */

		if (ai_debug.render_support_surfaces)
		{
			struct swarm_datum *swarm = NULL;
			long unit_index = NONE;
			short unit_num = 0;

			if (!actor->meta.swarm || actor->meta.swarm_cache_index==NONE)
			{
				unit_index = actor->meta.unit_index;
			}
			else
			{
				swarm = swarm_get(actor->meta.swarm_cache_index);

				if (swarm->unit_count>0)
				{
					unit_index = swarm->unit_indices[0];
				}
			}

			while (unit_index!=NONE)
			{
				struct biped_datum *biped = biped_try_and_get(unit_index);

				if (!biped || biped->biped.support_surface_index==NONE)
				{
					real_point3d origin;

					object_get_origin(unit_index, &origin);
					render_debug_sphere(TRUE, &origin, 0.3f, global_real_argb_pink);

					if (!actor->meta.swarm)
					{
						if (actor->input.pathfinding_surface_index==NONE)
						{
							render_debug_sphere(TRUE, &actor->input.position.body_position, 0.4f, global_real_argb_red);
						}
						else
						{
							render_debug_sphere(TRUE, &actor->input.pathfinding_point, 0.4f, global_real_argb_orange);
							ai_debug_render_surface(global_structure_bsp_get(), actor->input.pathfinding_surface_index, 0.f, global_real_argb_orange);
						}
					}
				}
				else
				{
					ai_debug_render_surface(global_structure_bsp_get(), biped->biped.support_surface_index, 0.f, global_real_argb_pink);
				}

				++unit_num;
				unit_index = NONE;

				if (swarm && unit_num<swarm->unit_count)
				{
					unit_index = swarm->unit_indices[unit_num];
				}
			}
		}

		/* Vitality */

		if (ai_debug.render_vitality)
		{
			if (actor->input.body_vitality>0.f)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "body %3.2f", actor->input.body_vitality), global_real_argb_red);
			}

			if (actor->input.shield_vitality>0.f)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "shld %3.2f", actor->input.shield_vitality), global_real_argb_blue);
			}
		}

		/* Damage */

		if (ai_debug.render_recent_damage)
		{
			if (actor->input.recent_body_damage>0.f)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "b/dmg %3.2f", actor->input.recent_body_damage), global_real_argb_yellow);
			}

			if (actor->input.recent_shield_damage>0.f)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "s/dmg %3.2f", actor->input.recent_shield_damage), global_real_argb_green);
			}
		}

		/* Cover seeking */

		if (ai_debug.render_active_cover_seeking && actor_debug_info->field_B8)
		{
			const char *strings[8] =
			{
				"wrongaction",
				"visibletarget",
				"repeattimer",
				"visibletimer",
				"shielded",
				"unavailable",
				"success",
				"panic"
			};

			render_debug_string_at_point(
				TRUE,
				ai_debug_drawstack(),
				csprintf(temporary, "%s %d %.2f", strings[actor_debug_info->field_BA], actor_debug_info->field_BC, actor_debug_info->field_C0),
				global_real_argb_yellow);
		}

		/* Threats */

		if (ai_debug.render_threats && actor->situation.known_enemies)
		{
			render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "enemy %d", actor->situation.known_enemies), global_real_argb_red);

			actor_debug_print_threat(actor, _actor_threat_visible, "vis %d/%d", global_real_argb_white);
			actor_debug_print_threat(actor, _actor_threat_visible_facing_me, "facing %d/%d", global_real_argb_lightblue);
			actor_debug_print_threat(actor, _actor_threat_visible_aiming_at_me, "aim at %d/%d", global_real_argb_blue);
			actor_debug_print_threat(actor, _actor_threat_shooting, "shoot %d/%d", global_real_argb_yellow);
			actor_debug_print_threat(actor, _actor_threat_shooting_near_me, "s/near %d/%d", global_real_argb_orange);
			actor_debug_print_threat(actor, _actor_threat_shooting_at_me, "s/at me %d/%d", global_real_argb_red);
			actor_debug_print_threat(actor, _actor_threat_extremely_close_to_me, "ex.close %d/%d", global_real_argb_magenta);
			actor_debug_print_threat(actor, _actor_threat_damaging_me, "dmging %d/%d", global_real_argb_magenta);
		}

		/* Emotions */

		if (ai_debug.render_emotions)
		{
			render_debug_string_at_point(
				TRUE,
				ai_debug_drawstack(),
				csprintf(temporary, "dngr %3.2f/%3.2f", actor->emotions.instantaneous_danger, actor->emotions.perceived_danger),
				global_real_argb_yellow);

			if (actor->emotions.unopposable_retreat_timer>0)
			{
				real_point3d position;
				struct prop_datum *prop = prop_get(actor->emotions.unopposable_retreat_prop_index);

				point_from_line3d(&actor->input.position.head_position, global_up3d, 0.05f, &position);
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "retreat t%d", actor->emotions.unopposable_retreat_timer), global_real_argb_red);
				render_debug_line(TRUE, &position, &prop->head_position, global_real_argb_red);
			}

			if (TEST_FLAG(actor_definition->flags, _actor_definition_fixed_crouch_facing_bit))
			{
				if (actor->control.desire_stationary_facing)
				{
					if (actor->control.fixed_stationary_facing)
					{
						render_debug_string_at_point(TRUE, ai_debug_drawstack(), "fixed-facing", global_real_argb_orange);
						render_debug_vector(TRUE, &actor->input.position.head_position, &actor->control.fixed_stationary_facing_vector, 1.5f, global_real_argb_orange);
					}
					else
					{
						render_debug_string_at_point(TRUE, ai_debug_drawstack(), "desire-fixed-facing", global_real_argb_orange);
					}
				}
				else
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "nostationary%s", actor->control.moving ? " (moving)" : ""), global_real_argb_orange);
				}
			}
		}

		/* Teams */

		if (ai_debug.render_teams)
		{
			char const *teams[NUMBER_OF_SOLO_CAMPAIGN_TEAMS] =
			{
				"default",
				"player",
				"human",
				"covenant",
				"flood",
				"sentinel",
				"unused6",
				"unused7",
				"unused8",
				"unused9"
			};

			render_debug_string_at_point(TRUE, ai_debug_drawstack(), actor->meta.team_index==NONE ? "none" : teams[actor->meta.team_index], global_real_argb_green);
		}

		/* Player ratings */

		if (ai_debug.render_player_ratings && actor->meta.unit_index!=NONE)
		{
			real player_rating = ai_communication_get_player_rating(actor->meta.unit_index, TRUE, NULL, NULL);
			render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "%.1f", player_rating), player_rating==0.f ? global_real_argb_blue : global_real_argb_white);
		}

		/* Audibility */

		if (ai_debug.render_audibility && actor_debug_info->field_A4)
		{
			real_argb_color const *color;
			char const *aud_type;
			char textstring[512];

			if (actor_debug_info->field_A6==0)
			{
				color = global_real_argb_red;
				aud_type = "none";
				
			}
			else if(actor_debug_info->field_A6==1)
			{
				color = global_real_argb_blue;
				aud_type = "part";
			}
			else
			{
				color = global_real_argb_green;
				aud_type = "full";
			}

			sprintf(textstring, "aud/%s %.1fp/%.1fd", aud_type, actor_debug_info->field_A8, actor_debug_info->field_AC);

			if (actor_debug_info->field_B0!=-1.f)
			{
				strcat(textstring, csprintf(temporary, "/%.1fs/%.1ff", actor_debug_info->field_B0, actor_debug_info->field_B4));
			}

			render_debug_string_at_point(TRUE, ai_debug_drawstack(), textstring, color);
		}

		/* Props */

		if (ai_debug.render_props || ai_debug.render_props_web)
		{
			struct prop_datum *prop;
			struct prop_iterator iterator;

			real_point3d prop_start_point;

			short dead_count = 0;
			short friend_count = 0;
			short enemy_count = 0;
			short orphan_count = 0;
			short total_count = 0;

			point_from_line3d(&actor->input.position.head_position, global_up3d, 0.2f, &prop_start_point);

			prop_iterator_new(&iterator, actor_index);

			while (prop = prop_iterator_next(&iterator))
			{
				++total_count;

				if (prop->dead)
				{
					++dead_count;
				}
				else if (
					prop->state>=_prop_state_uninspected_orphan &&
					prop->state<=_prop_state_inspected_orphan)
				{
					match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 2230, prop->enemy);
					++orphan_count;
				}
				else
				{
					if (prop->enemy)
					{
						++enemy_count;
					}
					else
					{
						++friend_count;
					}
				}

				if ((render_exclusive || ai_debug.render_props_web) &&
					(prop->enemy || !ai_debug.render_props_no_friends))
				{
					real_argb_color const *color;
					real_point3d origin;
					real_point3d string_point;

					point_from_line3d(&prop->head_position, global_up3d, 0.2, &string_point);

					switch (prop->state)
					{
					case 2:
					case 3:
						render_debug_line(TRUE, &prop_start_point, &prop->head_position, global_real_argb_yellow);
						break;
					case 1:
						set_real_point3d(
							&origin,
							((1.f-prop->awareness)*prop_start_point.x) + (prop->awareness*prop->head_position.x),
							((1.f-prop->awareness)*prop_start_point.y) + (prop->awareness*prop->head_position.y),
							((1.f-prop->awareness)*prop_start_point.z) + (prop->awareness*prop->head_position.z)
						);
						render_debug_line(TRUE, &prop_start_point, &origin, global_real_argb_yellow);
						render_debug_line(TRUE, &origin, &prop->head_position, global_real_argb_black);
						break;
					case 0:
						render_debug_line(TRUE, &prop_start_point, &prop->head_position, global_real_argb_black);
						break;
					case 4:
					case 5:
						color = iterator.index==actor->meta.interesting_orphan_index ? global_real_argb_purple : global_real_argb_blue;
						render_debug_line(TRUE, &prop_start_point, &prop->head_position, color);
						
						if (!prop->definitely_located)
						{
							real_vector3d hint_vector;

							real_argb_color const *alt_color = prop->state==_prop_state_uninspected_orphan ? global_real_argb_yellow : global_real_argb_blue;

							set_real_vector3d(&hint_vector, prop->orphan_hint_vector.i, prop->orphan_hint_vector.j, 0.f);
							render_debug_sphere(TRUE, &prop->head_position, 0.2f, alt_color);
							render_debug_vector(TRUE, &prop->head_position, &hint_vector, 1.f, alt_color);
						}
						break;
					default:
						break;
					}

					point_from_line3d(&prop_start_point, global_up3d, 0.03f, &prop_start_point);

					if (prop->required_ticks>0 ||
						prop->state>=_prop_state_uninspected_orphan && prop->state<=_prop_state_inspected_orphan)
					{
						if (prop->required_ticks>0)
						{
							sprintf(temporary, "r%d ", prop->required_ticks);
						}
						else
						{
							strcpy(temporary, "");
						}

						if (prop->state>=_prop_state_uninspected_orphan && prop->state<=_prop_state_inspected_orphan)
						{
							char temp[256];
							strcpy(temp, temporary);
							sprintf(temporary, "%so%d ", temp, prop->orphan_lifespan_ticks);
						}
						
						if (prop->state==_prop_state_uninspected_orphan)
						{
							char temp[256];
							strcpy(temp, temporary);
							sprintf(temporary, "%si%d ", temp, prop->orphan_inspection_ticks);
						}

						render_debug_string_at_point(TRUE, &string_point, temporary, global_real_argb_pink);
						point_from_line3d(&string_point, global_up3d, 0.05f, &string_point);
					}

					if (ai_debug.render_props_unreachable)
					{
						real_argb_color const *color;

						if (prop->ignore)
						{
							color = global_real_argb_blue;
						}
						else
						{
							color = prop->preferred_target ? global_real_argb_pink : global_real_argb_red;
						}

						render_debug_string_at_point(TRUE, &string_point, csprintf(temporary, "%.2f", prop->target_weight), color);
						point_from_line3d(&string_point, global_up3d, 0.05f, &string_point);
						
						if (iterator.index==actor->target.target_prop_index)
						{
							render_debug_string_at_point(TRUE, &string_point, "target", global_real_argb_white);
							point_from_line3d(&string_point, global_up3d, 0.05f, &string_point);
						}
					}

					if (ai_debug.render_props_unopposable && prop->unreachable_ticks>0)
					{
						long time = prop->last_unreachable_time!=NONE ? game_time_get()-prop->last_unreachable_time : NONE;

						render_debug_string_at_point(
							TRUE,
							&string_point,
							csprintf(temporary, "unr %d %d", prop->unreachable_ticks, time),
							global_real_argb_darkgreen);
						point_from_line3d(&string_point, global_up3d, 0.05f, &string_point);
					}

					if (ai_debug.render_props_target_weight && prop->unopposable_enemy)
					{
						sprintf(
							temporary,
							"unopp c%d(%d) t%d",
							prop->unopposable_casualties_inflicted,
							prop->unopposable_casualty_decay_timer,
							prop->unopposable_trigger_timer);
						if (prop->unopposable_trigger_timer>0)
						{
							char string[256];

							sprintf(string, "/%d h%d", prop->unopposable_trigger_threshold, prop->unopposable_trigger_hysteresis);
							strcat(temporary, string);
						}

						render_debug_string_at_point(TRUE, &string_point, temporary, global_real_argb_pink);
						point_from_line3d(&string_point, global_up3d, 0.05f, &string_point);
						render_debug_line(TRUE, &prop_start_point, &prop->head_position, global_real_argb_pink);
						point_from_line3d(&prop_start_point, global_up3d, 0.03f, &prop_start_point);
					}
				}
			}

			if (ai_debug.render_props_target_weight && actor->emotions.unopposable_retreat_timer>0)
			{
				real_point3d p0;
				struct prop_datum const *retreating_prop = prop_get(actor->emotions.unopposable_retreat_prop_index);
				
				point_from_line3d(&actor->input.position.head_position, global_up3d, 0.03f, &p0);
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "retreat t%d", actor->emotions.unopposable_retreat_timer),
					global_real_argb_red);
				render_debug_line(TRUE, &p0, &retreating_prop->head_position, global_real_argb_red);
			}

			if (render_exclusive || ai_debug.render_props_web)
			{
				sprintf(temporary, "d%d o%d e%d f%d", dead_count, orphan_count, enemy_count, friend_count);
			}
			else
			{
				sprintf(temporary, "%d", total_count);
			}
			
			render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_green);
		}

		if (ai_debug.render_secondary_looking && actor->control.secondary_look_type>0)
		{
			char const *direction_specification_type_strings[NUMBER_OF_DIRECTION_SPECIFICATION_TYPES] =
			{
				"move",
				"prop",
				"targ",
				"point",
				"vector",
				"danger",
				NULL
			};

			char const *secondary_look_type_strings[NUMBER_OF_SECONDARY_LOOK_TYPES] =
			{
				"none",
				"noise",
				"moving",
				"impact",
				"ack",
				"bumped",
				"deton",
				"shoot",
				"comm",
				"comm/d",
				"combat",
				"damage",
				"danger",
				"script"
			};

			char const *secondary_look_priotity_strings[NUMBER_OF_SECONDARY_LOOK_PRIORITIES] =
			{
				"none",
				"def",
				"i/look",
				"i/aim",
				"aim",
				"turn/a",
				"stop/a",
				"over",
				"over/f"
			};

			render_debug_string_at_point(
				TRUE,
				ai_debug_drawstack(),
				csprintf(
					temporary,
					"%s %s %s %d",
					direction_specification_type_strings[actor->control.secondary_look_direction.type],
					secondary_look_type_strings[actor->control.secondary_look_type],
					secondary_look_priotity_strings[actor->control.secondary_look_priority],
					actor->control.secondary_look_timer),
				global_real_argb_magenta);

			switch (actor->control.secondary_look_direction.type)
			{
			case _direction_specification_prop:
			{
				struct prop_datum *prop = prop_get(actor->control.secondary_look_direction.prop_index);

				render_debug_line(
					TRUE,
					&actor->input.position.head_position,
					&prop->head_position,
					global_real_argb_magenta);
				break;
			}
			case _direction_specification_target:
				if (actor->target.target_prop_index!=NONE)
				{
					struct prop_datum *prop = prop_get(actor->target.target_prop_index);

					render_debug_line(
						TRUE,
						&actor->input.position.head_position,
						&prop->head_position,
						global_real_argb_magenta);
				}
				break;
			case _direction_specification_point:
				render_debug_line(
					TRUE, 
					&actor->input.position.head_position,
					&actor->control.secondary_look_direction.point,
					global_real_argb_magenta);
				break;
			case _direction_specification_vector:
			{
				real_point3d aim_pos;
				
				point_from_line3d(
					&actor->input.position.head_position,
					&actor->control.secondary_look_direction.vector,
					1.f,
					&aim_pos);
				render_debug_line(
					TRUE,
					&actor->input.position.head_position,
					&aim_pos,
					global_real_argb_magenta);
				break;
			}
			default:
				break;
			}
		}

		/* Pursuit */

		if (ai_debug.render_pursuit)
		{
			struct pursuit_location const *location = actor_get_pursuit_location(actor_index);

			if (location)
			{
				if (location->type==0 && actor->target.target_prop_index!=NONE)
				{
					struct prop_datum *prop = prop_get(actor->target.target_prop_index);
					render_debug_line(TRUE, &actor->input.position.head_position, &prop->body_position, actor_action_debug_color(actor_index));
				}
				else
				{
					if (location->type==1)
					{
						render_debug_line(TRUE, &actor->input.position.head_position, &location->position, actor_action_debug_color(actor_index));
					}
				}
			}
			
		}

		/* Aiming vectors */

		if (render_exclusive && ai_debug.render_aiming_vectors)
		{
			if (actor->meta.unit_index!=NONE)
			{
				real_point3d *head_position = &actor->input.position.head_position;
				real_point3d p0;
				real_point3d p1;
				real_vector3d forward;

				point_from_line3d(&actor->input.position.head_position, global_up3d, 0.01f, &p0);
				point_from_line3d(&actor->input.position.head_position, &actor->control.desired_facing_vector, 1.5f, &p1);
				point_from_line3d(&p1, global_up3d, 0.01f, &p1);
				render_debug_line(TRUE, &p0, &p1, global_real_argb_orange);
				
				point_from_line3d(&actor->input.position.head_position, &actor->control.desired_aiming_vector, 1.4f, &p1);
				point_from_line3d(&p1, global_up3d, 0.02f, &p1);
				render_debug_line(TRUE, &p0, &p1, global_real_argb_green);
				
				point_from_line3d(&actor->input.position.head_position, &actor->control.desired_looking_vector, 1.3f, &p1);
				point_from_line3d(&p1, global_up3d, 0.03f, &p1);
				render_debug_line(TRUE, &p0, &p1, global_real_argb_cyan);
				
				p0.x = head_position->x-global_up3d->i*0.04f;
				p0.y = head_position->y-global_up3d->j*0.04f;
				p0.z = head_position->z-global_up3d->k*0.04f;
				unit_get_facing_vector(actor->meta.unit_index, &forward);
				
				render_debug_vector(TRUE, &p0, &forward, 1.f, global_real_argb_red);
				render_debug_vector(TRUE, &p0, &unit->unit.aiming_vector, 1.f, global_real_argb_darkgreen);
				render_debug_vector(TRUE, &p0, &unit->unit.looking_vector, 1.f, global_real_argb_blue);


				if (unit->object.type==_object_type_biped && unit->object.parent_object_index==NONE)
				{
					real_vector3d throttle_vector;

					struct biped_definition *biped_definition = biped_definition_get(unit->definition_index);
					
					unit_get_facing_vector(actor->meta.unit_index, &forward);

					if (TEST_FLAG(biped_definition->biped.flags, _biped_flying_bit))
					{
						real_vector3d left;
						real_vector3d up;

						biped_build_flying_axes(&forward, &left, &up);
						scale_vector3d(&forward, actor->output.throttle.i, &throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &left, actor->output.throttle.j, (real_point3d *)&throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &up, actor->output.throttle.k, (real_point3d *)&throttle_vector);
					}
					else
					{
						real_vector3d v;

						set_real_vector3d(&v, -forward.j, forward.i, 0.f);
						scale_vector3d(&forward, actor->output.throttle.i, &throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &v, actor->output.throttle.j, (real_point3d *)&throttle_vector);
					}

					point_from_line3d(&actor->input.position.body_position, global_up3d, 0.1f, &p0);
					render_debug_vector(TRUE, &p0, &throttle_vector, 1.6f, global_real_argb_pink);
				}
			}
		}

		/* Gun positions */
		
		if (ai_debug.render_gun_positions && actor->meta.unit_index!=NONE)
		{
			real_point3d estimated_position;

			real_vector3d *gun_offset = NULL;
			real_argb_color const *color = global_real_argb_red;
			real_vector3d desired_facing = actor->input.aiming_vector;

			if (normalize2d((real_vector2d *)&desired_facing)>0.f)
			{
				desired_facing.k = 0.f;
			}
			else
			{
				desired_facing = actor->input.facing_vector;
			}

			if (actor->control.crouching)
			{
				if (magnitude_squared3d(&actor_variant_definition->ranged_combat.gun_offset_crouch)>_real_epsilon)
				{
					gun_offset = &actor_variant_definition->ranged_combat.gun_offset_crouch;
					color = global_real_argb_magenta;
				}
				else
				{
					if (magnitude_squared3d(&actor_definition->perception.gun_offset_crouch)>_real_epsilon)
					{
						gun_offset = &actor_definition->perception.gun_offset_crouch;
						color = global_real_argb_pink;
					}
				}
			}
			else
			{
				if (magnitude_squared3d(&actor_variant_definition->ranged_combat.gun_offset_stand)>_real_epsilon)
				{
					gun_offset = &actor_variant_definition->ranged_combat.gun_offset_stand;
					color = global_real_argb_magenta;
				}
				else
				{
					if (magnitude_squared3d(&actor_definition->perception.gun_offset_stand)>_real_epsilon)
					{
						gun_offset = &actor_definition->perception.gun_offset_stand;
						color = global_real_argb_pink;
					}
				}
			}

			if (gun_offset==NULL)
			{
				estimated_position = actor->input.position.head_position;
			}
			else
			{
				unit_estimate_position(
					actor->meta.unit_index,
					_unit_estimate_gun_position,
					&actor->input.position.body_position,
					&desired_facing,
					gun_offset,
					&estimated_position);
			}

			render_debug_vector(TRUE, &estimated_position, &actor->input.aiming_vector, 1.f, color);
		}

		/* Targets */

		if ((ai_debug.render_targets || ai_debug.render_targets_last_visible) &&
			actor->target.target_type!=0 &&
			actor->target.target_prop_index!=NONE)
		{
			real_point3d actor_target_position;
			real_point3d prop_target_position;

			struct prop_datum *prop = prop_get(actor->target.target_prop_index);
			real_argb_color const *target_color = global_real_argb_white;

			switch (actor->target.target_type)
			{
			case _actor_target_partial_enemy:
				target_color = global_real_argb_grey;
				break;
			case _actor_target_dead_enemy:
				target_color = global_real_argb_green;
				break;
			case _actor_target_disregarded_orphan:
				target_color = global_real_argb_salmon;
				break;
			case _actor_target_inspected_orphan:
				target_color = global_real_argb_blue;
				break;
			case _actor_target_uninspected_orphan:
				target_color = global_real_argb_lightblue;
				break;
			case _actor_target_definite_orphan:
				target_color = global_real_argb_cyan;
				break;
			case _actor_target_acknowledged_enemy:
				target_color = global_real_argb_purple;
				break;
			case _actor_target_clear_line_of_sight_enemy:
				target_color = global_real_argb_yellow;
				break;
			case _actor_target_potentially_dangerous_enemy:
				target_color = global_real_argb_orange;
				break;
			case _actor_target_visible_enemy:
				target_color = global_real_argb_red;
				break;
			case _actor_target_damaging_enemy:
				target_color = global_real_argb_magenta;
				break;
			default:
				match_vassert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 2653, FALSE, NULL);
			}

			point_from_line3d(&actor->input.position.head_position, global_down3d, 0.01f, &actor_target_position);
			point_from_line3d(&prop->head_position, global_down3d, 0.01f, &prop_target_position);
			render_debug_line(TRUE, &actor_target_position, &prop_target_position, target_color);

			if (ai_debug.render_targets_last_visible && prop->last_visible_time!=NONE)
			{
				render_debug_sphere(TRUE, &prop->last_visible_head_position, 0.2f, target_color);
			}
			
			if (prop->unreachable_ticks>0 && (!ai_debug.render_props || !ai_debug.render_props_unopposable))
			{
				long time = prop->last_unreachable_time!=NONE ? game_time_get()-prop->last_unreachable_time : NONE;

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "unr %d %d", prop->unreachable_ticks, time),
					global_real_argb_darkgreen);
			}
		}

		/* States */

		if (ai_debug.render_states)
		{
			struct observer_result const *camera = observer_get_camera(0);

			if (camera)
			{
				real_point3d position;

				point_from_line3d(&camera->position, &camera->forward, 0.05f, &position);
				render_debug_line(TRUE, &position, &actor->input.position.head_position, actor_action_debug_color(actor_index));
			}
		}

		/* Current state */

		if (ai_debug.render_current_state)
		{
			if (actor->meta.encounter_index!=NONE)
			{
				struct encounter_datum *encounter = encounter_get(actor->meta.encounter_index);
				struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
					struct encounter_definition);
				struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					actor->meta.squad_index,
					struct squad_definition);
				struct squad_datum *squad = encounter_get_squad(encounter, actor->meta.squad_index);

				if (squad->delay_timer>0)
				{
					if (squad->delay_timer_started)
					{
						render_debug_string_at_point(
							TRUE,
							ai_debug_drawstack(),
							csprintf(temporary, "delaying %d", squad->delay_timer),
							global_real_argb_green);
					}
					else
					{
						if (TEST_FLAG(squad_definition->flags, _squad_delay_forever_bit))
						{
							render_debug_string_at_point(TRUE, ai_debug_drawstack(), "delay forever", global_real_argb_green);
						}
						else
						{
							render_debug_string_at_point(TRUE, ai_debug_drawstack(), "delay not triggered", global_real_argb_green);
						}
					}
				}
			}

			switch (actor->state.action)
			{
			case _actor_action_flee:
				if (actor->state.action_data.flee.has_approach_point)
				{
					render_debug_sphere(TRUE, &actor->state.action_data.flee.approach_point, 0.25f, actor_action_debug_color(actor_index));
				}
				break;	
			case _actor_action_fight:
				if (actor->state.action_data.alert.move_position_order>0)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "change %d", actor->state.action_data.alert.move_position_order),
						actor_action_debug_color(actor_index));
				}
				break;
			case _actor_action_guard:
				if (actor->state.action_data.guard.wait_ticks>0)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "wait %d", actor->state.action_data.guard.wait_ticks),
						actor_action_debug_color(actor_index));
				}

				if (actor->state.action_data.guard.look_ticks>0)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "look %d", actor->state.action_data.guard.look_ticks),
						actor_action_debug_color(actor_index));
				}

				if (actor->state.action_data.guard.cower)
				{
					if (actor->state.action_data.guard.cower_from_retreat)
					{
						render_debug_string_at_point(
							TRUE,
							ai_debug_drawstack(),
							csprintf(temporary, "retreat %d", actor->emotions.unopposable_retreat_timer),
							actor_action_debug_color(actor_index));
					}
					else
					{
						char const *string = actor->state.action_data.guard.cower_panicked ? "panic" : "hide";
						render_debug_string_at_point(
							TRUE,
							ai_debug_drawstack(),
							csprintf(temporary, "%s %d", string, actor->state.action_data.guard.cower_ticks),
							actor_action_debug_color(actor_index));
					}
				}

				if (actor->state.action_data.guard.has_guard_direction)
				{
					render_debug_vector(
						TRUE,
						&actor->input.position.head_position,
						&actor->state.action_data.guard.guard_direction,
						2.5f,
						actor_action_debug_color(actor_index));
				}
				break;
			case _actor_action_uncover:
			case _actor_action_search:
			{
				struct pursuit_location *pursuit_location = actor_get_pursuit_location(actor_index);
				long delay = 0;

				match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 2773, pursuit_location != NULL);

				if (pursuit_location->type==_pursuit_location_target)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "targ(%d)", actor->firing_positions.pursuit_positions_count),
						actor_action_debug_color(actor_index));
				}
				else if (pursuit_location->type==_pursuit_location_position)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "pt(%d)", actor->firing_positions.pursuit_positions_count),
						actor_action_debug_color(actor_index));
				}
				else
				{
					render_debug_string_at_point(1, ai_debug_drawstack(), "undirected", actor_action_debug_color(actor_index));
				}

				if (actor->state.action==_actor_action_uncover)
				{
					delay = actor->state.action_data.uncover.uncover_remaining_time;
				}
				else if (actor->state.action==_actor_action_search)
				{
					if (120-actor->state.action_data.search.search_failure_timer<=actor->state.action_data.search.search_remaining_time)
					{
						delay = 120-actor->state.action_data.search.search_failure_timer;
					}
					else
					{
						delay = actor->state.action_data.search.search_remaining_time;
					}
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), csprintf(temporary, "delay %d", delay), actor_action_debug_color(actor_index));
				break;
			}
			case _actor_action_vehicle:
				if (actor->state.action_data.vehicle.started_entry)
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "entering", global_real_argb_darkgreen);
				}
				else
				{
					char textbuffer[1024];

					strcpy(textbuffer, "");

					if (actor->state.action_data.vehicle.currently_correct_facing)
					{
						strcat(textbuffer, "facing-ok ");
					}
					
					if (actor->state.action_data.vehicle.currently_within_range)
					{
						strcat(textbuffer, "range-ok ");
					}

					if (actor->state.action_data.vehicle.fake_entry_potential_timer>0)
					{
						strcat(textbuffer, csprintf(temporary, "fake-entry %d ", actor->state.action_data.vehicle.fake_entry_potential_timer));
					}

					if (actor_path_has_path(actor_index))
					{
						if (actor_path_at_destination(actor_index))
						{
							strcat(textbuffer, "destination-");
						}

						strcat(textbuffer, "moving ");
					}

					if (actor->state.action_data.vehicle.lock_facing)
					{
						strcat(textbuffer, "locked ");
					}

					render_debug_string_at_point(TRUE, ai_debug_drawstack(), textbuffer, global_real_argb_darkgreen);
				}

				render_debug_line(
					TRUE,
					&actor->input.position.body_position,
					&actor->state.action_data.vehicle.destination_point,
					global_real_argb_darkgreen);
				render_debug_vector(
					TRUE,
					&actor->state.action_data.vehicle.destination_point,
					&actor->state.action_data.vehicle.destination_facing,
					1.f,
					global_real_argb_yellow);
				break;
			case _actor_action_charge:
				switch (actor->state.action_data.charge.goal)
				{
				case _charge_goal_close_range:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "charge", global_real_argb_red);
					break;
				case _charge_goal_stalking:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(
							temporary,
							"stalk%s %s disc%d",
							actor->state.action_data.charge.stalking_catch_target ? " catchtarget" : "",
							actor->state.action_data.charge.stalking_currently_exposed ? " exposed" : "",
							actor->state.action_data.charge.stalking_discovery_timer),
						global_real_argb_blue);
					break;
				case _charge_goal_melee:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "melee", global_real_argb_red);
					break;
				case _charge_goal_melee_leaping:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "melee-leap", global_real_argb_red);
					break;
				case _charge_goal_vehicle_strafing:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "strafe", global_real_argb_red);
					break;
				case _charge_goal_vehicle_ramming:
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "ramming", global_real_argb_red);
					break;
				default:
					break;
				}

				if (actor->emotions.berserk)
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "berserk", global_real_argb_red);
				}
				break;
			case _actor_action_obey:
			{
				struct ai_command_definition *command = NULL;

				if (actor->state.action_data.obey.command_list_index!=NONE)
				{
					struct ai_command_list_definition const* command_list = TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->ai_command_lists,
						actor->state.action_data.obey.command_list_index,
						struct ai_command_list_definition);
					
					if (!actor->meta.swarm)
					{
						render_debug_string_at_point(
							TRUE,
							ai_debug_drawstack(),
							csprintf(
								temporary,
								"command-list %s: #%d of #%d",
								command_list->name,
								actor->state.action_data.obey.simple_control.current_command_index+1,
								command_list->commands.count),
							global_real_argb_purple);
						
						if (actor->state.action_data.obey.simple_control.current_command_index<command_list->commands.count)
						{
							command = TAG_BLOCK_GET_ELEMENT(
								&command_list->commands,
								actor->state.action_data.obey.simple_control.current_command_index,
								struct ai_command_definition);
						}

						if (TEST_FLAG(actor->state.action_data.obey.simple_control.metadata_flags, _obey_metadata_commands_finished_bit))
						{
							render_debug_string_at_point(TRUE, ai_debug_drawstack(), "finished", global_real_argb_purple);
						}

						if (command)
						{
							boolean v427 = FALSE;
							boolean v426 = FALSE;
							boolean v425 = FALSE;
							boolean v424 = FALSE;

							switch (command->atom_type)
							{
							case _ai_atom_go_to:
								v427 = TRUE;
								v426 = TRUE;
								break;
							case _ai_atom_go_to_and_face:
								v427 = TRUE;
								v426 = TRUE;
								v425 = TRUE;
								break;
							case _ai_atom_move_direction:
								v426 = TRUE;
								v424 = TRUE;
								break;
							case _ai_atom_look:
							case _ai_atom_shoot:
							case _ai_atom_grenade:
								v426 = TRUE;
								break;
							default:
								break;
							}

							if (v427)
							{
								if (actor->state.action_data.charge.melee_suicide)
								{
									render_debug_sphere(
										TRUE,
										&actor->state.action_data.obey.complex_control.destination_point,
										actor->state.action_data.obey.complex_control.destination_radius_valid ? 
										actor->state.action_data.obey.complex_control.destination_radius :
										0.5f,
										global_real_argb_purple);
								}
							}

							if (v426)
							{
								if (command->point1_index>=0 &&
									command->point1_index<command_list->points.count)
								{
									real_point3d position;

									struct ai_command_point_definition const *point = TAG_BLOCK_GET_ELEMENT(
										&command_list->points,
										command->point1_index,
										struct ai_command_point_definition);

									render_debug_line(
										TRUE,
										&actor->input.position.head_position,
										&point->position,
										global_real_argb_purple);
									point_from_line3d(&point->position, global_up3d, 0.1f, &position);
									render_debug_string_at_point(
										TRUE,
										&position,
										csprintf(temporary, "%d", command->point1_index),
										global_real_argb_purple);
								}
								else
								{
									if (v424 && command->parameter2>=0.f && command->parameter2<360.f)
									{
										real_vector3d v;

										vector3d_from_angle(&v, command->parameter2);
										render_debug_vector(TRUE, &actor->input.position.head_position, &v, 1.5f, global_real_argb_purple);
									}
									else
									{
										render_debug_string_at_point(
											TRUE,
											ai_debug_drawstack(),
											csprintf(temporary, "error: invalid point 1 specified (%d)", command->point1_index),
											global_real_argb_purple);
									}
								}
							}

							if (v425)
							{
								if (command->point2_index>=0 &&
									command->point2_index<command_list->points.count)
								{
									real_point3d position;

									struct ai_command_point_definition const *point = TAG_BLOCK_GET_ELEMENT(
										&command_list->points,
										command->point2_index,
										struct ai_command_point_definition);

									render_debug_line(
										TRUE,
										&actor->input.position.head_position,
										&point->position,
										global_real_argb_purple);
									point_from_line3d(&point->position, global_up3d, 0.1f, &position);
									render_debug_string_at_point(1, &position, csprintf(temporary, "%d", command->point2_index), global_real_argb_purple);

								}
								else
								{
									render_debug_string_at_point(
										TRUE,
										ai_debug_drawstack(),
										csprintf(temporary, "error: invalid point 2 specified (%d)", command->point2_index),
										global_real_argb_purple);
								}
							}
						}
					}

					if (command)
					{
						action_obey_describe_command(global_scenario_get(), command, temporary, NUMBEROF(temporary));
						render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_purple);
					}
				}
				break;
			}
			default:
				break;
			}
		}

		/* Shooting */

		if (ai_debug.render_shooting)
		{
			if (actor_debug_info->firing_decision!=_firing_no_target)
			{
				char const *string = actor_move_animation_busy(actor_index) ? "busy " : "";
				
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(), 
					csprintf(
						temporary,
						"%srof %.1f err %.1f dmg %.1f blk %d",
						string,
						actor_debug_info->shooting_rof,
						actor->control.burst_error,
						actor->control.burst_damage_modifier,
						actor->control.blocked_communication_timer),
					global_real_argb_white);


				if (actor->emotions.berserk)
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "berserk", global_real_argb_yellow);
				}
				else if (actor->control.firing_at_new_target)
				{
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(
							temporary,
							"newtarget %d",
							actor_variant_definition->ranged_combat.new_target_pattern_time*TICKS_PER_SECOND-
								actor->control.current_fire_target_timer
						),
						global_real_argb_blue);
				}
				else if (actor->control.firing_while_moving)
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "moving", global_real_argb_green);
				}

				switch (actor->control.fire_state)
				{
				case _actor_fire_state_none:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						"none",
						global_real_argb_red);
					break;
				case _actor_fire_state_holding:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "delay %d", actor->control.fire_state_timer),
						global_real_argb_red);
					break;
				case _actor_fire_state_bursting:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "burst %d", actor->control.fire_state_timer),
						global_real_argb_red);
					break;
				case _actor_fire_state_pausing:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "pause %d", actor->control.fire_state_timer),
						global_real_argb_red);
					break;
				case _actor_fire_state_wild:
					render_debug_string_at_point(
						TRUE,
						ai_debug_drawstack(),
						csprintf(temporary, "wildfire %d", actor->control.fire_state_timer), 
						global_real_argb_red);
					break;
				default:
					match_vassert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 3081, FALSE, NULL);
					break;
				}
			}

			if (VALID_INDEX(actor_debug_info->firing_decision, NUMBER_OF_ACTOR_DEBUG_FIRING_DECISIONS))
			{
				char const *firing_decision_names[NUMBER_OF_ACTOR_DEBUG_FIRING_DECISIONS] =
				{
					"firing disabled",
					"animation busy",
					"wrong target",
					"no target",
					"outside active region",
					"not visible",
					"outside range",
					"blocked",
					"first burst align",
					"first burst delay",
					"burst pause align",
					"burst pause",
					"firing wildly",
					"bursting",
					"in midair",
					"not crouching",
					"not standing",
					"not stationary",
					"underwater",
					"minimum range"
				};

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), firing_decision_names[actor_debug_info->firing_decision], global_real_argb_blue);
			}
		}

		/* Grenade decisions */

		if (ai_debug.render_grenade_decisions)
		{
			if (actor_debug_info->grenade_eval_time!=NONE &&
				actor_debug_info->grenade_eval_time+7 >= game_time_get())
			{
				switch (actor_debug_info->grenade_decision)
				{
				case _grenade_vehicle:
					csstrcpy(temporary, "in vehicle");
					break;
				case _grenade_unit_busy:
					csstrcpy(temporary, "unit busy");
					break;
				case _grenade_being_hurt:
					sprintf(temporary, "dmg %.2f", actor_debug_info->grenade_current_damage);
					break;
				case _grenade_no_grenades:
					csstrcpy(temporary, "no grenades");
					break;
				case _grenade_random_failed:
					sprintf(
						temporary,
						"random %.2f > %.2f",
						actor_debug_info->grenade_random_value,
						actor_debug_info->grenade_random_chance);
					break;
				case _grenade_encounter_timeout:
					sprintf(temporary, "encounter time %d", actor_debug_info->grenade_encounter_timeout_ticks);
					break;
				case _grenade_target_failed:
					csstrcpy(temporary, "no target");
					break;
				case _grenade_not_enough_enemies:
					sprintf(
						temporary,
						"not enough enemy %d < %d",
						actor_debug_info->grenade_enemy_count,
						actor_debug_info->grenade_required_enemy_count);
					break;
				case _grenade_collateral_damage:
					csstrcpy(temporary, "collateral dmg");
					break;
				case _grenade_trajectory_failed:
					csstrcpy(temporary, "no trajectory");
					break;
				case _grenade_success:
					csstrcpy(temporary, "success");
					break;
				default:
					csstrcpy(temporary, "<unknown>");
					break;
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_pink);
			}

			if (actor->control.grenade_trying_to_throw)
			{
				real damage = unit==NULL ? 0.f : unit->object.current_body_damage;
				char const *string = actor->meta.unit_index==NONE || !unit_is_busy(actor->meta.unit_index) ? "not-" : "";

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "trying: %sbusy dmg %.1f", string, damage),
					global_real_argb_pink);
			}
		}

		/* Danger zones */

		if (ai_debug.render_danger_zones && actor->danger_zone.danger_type>0)
		{
			real_argb_color const *color;

			if (actor->danger_zone.currently_perceived)
			{
				if (actor->danger_zone.acknowledgement_timer>0)
				{
					color = global_real_argb_white;
				}
				else
				{
					if (actor->danger_zone.noticed_danger)
					{
						color = global_real_argb_yellow;
					}
					else
					{
						color = global_real_argb_blue;
					}
				}
			}
			else
			{
				color = global_real_argb_darkgreen;
			}

			render_debug_line(TRUE, &actor->input.position.head_position, &actor->danger_zone.position, color);
			render_debug_sphere(TRUE, &actor->danger_zone.position, actor->danger_zone.danger_radius, global_real_argb_red);
			render_debug_vector(TRUE, &actor->danger_zone.position, &actor->danger_zone.velocity, 45.f, global_real_argb_red);
			render_debug_sphere(
				TRUE,
				&actor->danger_zone.bounding_sphere_center,
				actor->danger_zone.bounding_sphere_radius,
				global_real_argb_orange);

			if (actor->danger_zone.danger_type==_actor_unopposable_danger_shooting)
			{
				char const *string = actor->danger_zone.projectile.time_until_explosion==NONE ?
					"NONE" :
					csprintf(temporary, "%d", actor->danger_zone.projectile.time_until_explosion);

				render_debug_string_at_point(
					TRUE,
					&actor->danger_zone.position,
					string,
					color);
			}

			if (actor->danger_zone.danger_type==_actor_unopposable_danger_visible)
			{
				char const *string = actor->danger_zone.projectile.time_until_explosion==NONE ?
					"NONE" :
					csprintf(temporary, "%d", actor->danger_zone.projectile.time_until_explosion);

				render_debug_string_at_point(
					TRUE,
					&actor->danger_zone.position,
					string,
					color);
			}

			if (actor_debug_info->danger_avoidance_time!=NONE)
			{
				if (actor_debug_info->danger_avoidance_time+15 >= game_time_get())
				{
					boolean avoiding;

					if (actor_debug_info->danger_abandoned_path)
					{
						render_debug_string_at_point(TRUE, ai_debug_drawstack(), "discarded fp", global_real_argb_magenta);
					}

					avoiding = TRUE;

					switch (actor_debug_info->danger_decision)
					{
					case _danger_avoidance_none:
						avoiding = FALSE;
						break;
					case _danger_avoidance_unnoticed:
						strcpy(temporary, "unnoticed");
						break;
					case _danger_avoidance_animation_busy:
						strcpy(temporary, "animation busy");
						break;
					case _danger_avoidance_vehicle:
						strcpy(temporary, "in vehicle");
						break;
					case _danger_avoidance_far_away:
						sprintf(
							temporary,
							"far away (%.1f > %.1f)",
							actor_debug_info->danger_far_dist,
							actor_debug_info->danger_far_radius);
						break;
					case _danger_avoidance_outside_zone:
						sprintf(
							temporary,
							"outside (%.1f > %.1f)",
							actor_debug_info->danger_zone_dist,
							actor_debug_info->danger_zone_radius);
						break;
					case _danger_avoidance_evasion_disallowed:
						strcpy(temporary, "evasion not allowed");
						break;
					case _danger_avoidance_no_safe_direction:
						strcpy(temporary, "no safe direction");
						break;
					case _danger_avoidance_no_desire:
						if (actor_debug_info->danger_intersect_time==REAL_MAX)
						{
							strcpy(temporary, "no desire (no int'n)");
						}
						else
						{
							sprintf(temporary, "no desire (int'n %.1f)", actor_debug_info->danger_intersect_time);
						}
						break;
					case _danger_avoidance_can_avoid:
						strcpy(temporary, "can avoid");
						break;
					case _danger_avoidance_imminent_explosion:
						strcpy(temporary, "imminent explosion");
						break;
					case _danger_avoidance_imminent_impact:
						strcpy(temporary, "imminent impact");
						break;
					case _danger_avoidance_no_animation:
						strcpy(temporary, "no animation");
						break;
					case _danger_avoidance_attached_to_us:
						strcpy(temporary, "attached to us");
						break;
					default:
						strcpy(temporary, "<error>");
						break;
					}

					if (avoiding)
					{
						render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_magenta);
					}
				}
			}

			if (actor_debug_info->dive_decision_time!=NONE &&
				actor_debug_info->dive_decision_time+15 >= game_time_get())
			{
				switch (actor_debug_info->dive_decision)
				{
				case _dive_not_attempted:
					csstrcpy(temporary, "not attempted");
					break;
				case _dive_cannot_move:
					csstrcpy(temporary, "cannot move");
					break;
				case _dive_no_animation:
					csstrcpy(temporary, "animation unavailable");
					break;
				case _dive_animation_failure:
					csstrcpy(temporary, "animation failed");
					break;
				case _dive_success:
					csstrcpy(temporary, "success");
					break;
				default:
					csstrcpy(temporary, "<error>");
					break;
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_purple);
			}
		}

		/* Trigger */

		if (ai_debug.render_trigger && TEST_FLAG(actor->output.control_flags, _unit_control_weapon_primary_trigger_bit))
		{
			render_debug_string_at_point(
				TRUE,
				ai_debug_drawstack(),
				csprintf(temporary, "trigger %.1f", actor->output.analog_primary_trigger),
				global_real_argb_yellow);
		}

		/* Control */

		if (ai_debug.render_control && actor->meta.unit_index!=NONE)
		{
			short control_flag_bit;

			const char *control_flag_names[] =
			{
				"crouch",
				"jump",
				"user1",
				"user2",
				"light",
				"exactfacing",
				"action",
				"equipment",
				"lookdontturn",
				"forcealert",
				"reload",
				"trigger",
				"trigger2",
				"grenade"
			};

			short flag_count = NUMBEROF(control_flag_names);
			short count = 0;

			strcpy(temporary, "");

			for (control_flag_bit = 0; control_flag_bit<NUMBER_OF_UNIT_CONTROL_FLAGS; ++control_flag_bit)
			{
				if (TEST_FLAG(actor->output.control_flags, control_flag_bit))
				{
					if (count > 0)
					{
						strcat(temporary, " ");
					}

					if (control_flag_bit<flag_count)
					{
						strcat(temporary, control_flag_names[control_flag_bit]);
					}
					else
					{
						char tempbuf[80];

						sprintf(tempbuf, "<unknown %d>", control_flag_bit);
						strcat(temporary, tempbuf);
					}

					++count;
				}
			}
			
			if (count>0)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_yellow);
			}

			count = 0;

			strcpy(temporary, "");

			for (control_flag_bit = 0; control_flag_bit<NUMBER_OF_UNIT_CONTROL_FLAGS; ++control_flag_bit)
			{
				if (TEST_FLAG(actor->output.persistent_control_flags, control_flag_bit))
				{
					if (count > 0)
					{
						csstrcat(temporary, " ");
					}

					if (control_flag_bit<flag_count)
					{
						strcat(temporary, control_flag_names[control_flag_bit]);
					}
					else
					{
						char tempbuf[80];

						sprintf(tempbuf, "<unknown %d>", control_flag_bit);
						strcat(temporary, tempbuf);
					}

					++count;
				}
			}

			if (count>0)
			{
				char tempbuf[80];

				sprintf(tempbuf, ": persistent %d", actor->output.persistent_control_ticks);
				strcat(temporary, tempbuf);
	
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_orange);
			}

			{
				const char *movement_type_strings[NUMBER_OF_ACTOR_MOVEMENT_TYPES] =
				{
					"noncombat",
					"asleep",
					"combat",
					"flee",
					NULL
				};

				const char *aiming_speed_names[NUMBER_OF_UNIT_AIMING_SPEEDS] =
				{
					"alert",
					"casual"
				};
				
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(), csprintf(
						temporary,
						"m-%s a-%s",
						movement_type_strings[actor->output.movement_type],
						aiming_speed_names[actor->output.aiming_speed]),
					global_real_argb_magenta);
			}

			if (TEST_FLAG(actor->output.control_flags, _unit_control_weapon_primary_trigger_bit))
			{
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "analog %.2f", actor->output.analog_primary_trigger), 
					global_real_argb_cyan);
			}

			if (actor->output.animation.impulse!=NONE)
			{
				real_point3d base_point;
				real_vector3d alignment_vector_3d;

				point_from_line3d(&actor->input.position.body_position, global_up3d, 0.2f, &base_point);
				
				alignment_vector_3d.i = actor->output.animation.alignment_vector.i;
				alignment_vector_3d.j = actor->output.animation.alignment_vector.j;
				alignment_vector_3d.k = 0.f;

				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(), 
					csprintf(temporary, "animation %d", actor->output.animation.impulse),
					global_real_argb_pink);
				render_debug_vector(
					TRUE,
					&base_point,
					&alignment_vector_3d,
					1.f,
					global_real_argb_pink);
			}

			{
				real_point3d p0;
				real_point3d p1;

				point_from_line3d(&actor->input.position.head_position, global_up3d, 0.01f, &p0);
				point_from_line3d(&actor->input.position.head_position, &actor->output.facing_vector, 1.3f, &p1);
				point_from_line3d(&p1, global_up3d, 0.01f, &p1);
				
				render_debug_line(TRUE, &p0, &p1, global_real_argb_red);
				
				point_from_line3d(&actor->input.position.head_position, &actor->output.aiming_vector, 1.2f, &p1);
				point_from_line3d(&p1, global_up3d, 0.02f, &p1);
				
				render_debug_line(TRUE, &p0, &p1, global_real_argb_green);
				
				point_from_line3d(&actor->input.position.head_position, &actor->output.looking_vector, 1.1f, &p1);
				point_from_line3d(&p1, global_up3d, 0.03f, &p1);

				render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);

				if (unit->object.type==_object_type_biped && unit->object.parent_object_index==NONE)
				{
					real_vector3d throttle_vector;
					real_vector3d facing_vector;
					real_vector3d right_facing_vector;

					struct biped_definition const *biped_definition = biped_definition_get(unit->definition_index);

					unit_get_facing_vector(actor->meta.unit_index, &facing_vector);

					if (TEST_FLAG(biped_definition->biped.flags, _biped_flying_bit))
					{
						real_vector3d left_vector;
						real_vector3d up_vector;

						biped_build_flying_axes(&facing_vector, &left_vector, &up_vector);
						scale_vector3d(&facing_vector, actor->output.throttle.i, &throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &left_vector, actor->output.throttle.j, (real_point3d *)&throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &up_vector, actor->output.throttle.k, (real_point3d *)&throttle_vector);
					}
					else
					{
						set_real_vector3d(&right_facing_vector, -facing_vector.j, facing_vector.i, 0.f);
						scale_vector3d(&facing_vector, actor->output.throttle.i, &throttle_vector);
						point_from_line3d((real_point3d *)&throttle_vector, &right_facing_vector, actor->output.throttle.j, (real_point3d *)&throttle_vector);
					}

					point_from_line3d(&actor->input.position.body_position, global_up3d, 0.1f, &p0);
					render_debug_vector(TRUE, &p0, &throttle_vector, 1.f, global_real_argb_purple);
				}
			}
		}
		
		/* Charge decisions */

		if (ai_debug.render_charge_decisions &&
			actor_debug_info->charge_last_time!=NONE &&
			actor_debug_info->charge_last_time+TICKS_PER_SECOND >= game_time_get())
		{
			switch (actor_debug_info->charge_decision)
			{
			case _charge_vehicle_success:
				csstrcpy(temporary, "vehicle-success");
				break;
			case _charge_vehicle_not_driver:
				csstrcpy(temporary, "vehicle-notdriver");
				break;
			case _charge_melee_swarm_cant:
				csstrcpy(temporary, "melee-swarmcan't");
				break;
			case _charge_melee_inhibited:
				csstrcpy(temporary, "melee-inhibited");
				break;
			case _charge_melee_notarget:
				csstrcpy(temporary, "melee-notarget");
				break;
			case _charge_melee_no_animation:
				sprintf(temporary, "melee-noanimation (%sairborne)", actor_debug_info->field_198 ? "" : "not-");
				break;
			case _charge_melee_cannot_move:
				sprintf(temporary, "melee-cannotmove (%f)", actor_debug_info->field_194);
				break;
			case _charge_melee_success:
				sprintf(temporary, "melee-success (%sairborne)", actor_debug_info->field_198 ? "" : "not-");
				break;
			case _charge_stalking_success:
				csstrcpy(temporary, "stalking-success");
				break;
			case _charge_close_success:
				csstrcpy(temporary, "close-success");
				break;
			default:
				sprintf(temporary, "<unknown charge-setup decision %d>", actor_debug_info->charge_decision);
				break;
			}

			render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_red);
		}
		
		/* Melee check */

		if (ai_debug.render_melee_check &&
			actor_debug_info->last_melee_time!=NONE &&
			actor_debug_info->last_melee_time+TICKS_PER_SECOND >= game_time_get())
		{
			render_debug_vector(TRUE, &actor_debug_info->field_108, &actor_debug_info->field_114, 1.5f, global_real_argb_red);
			render_debug_point(TRUE, &actor_debug_info->field_120, 0.2f, global_real_argb_red);
		
			if (!actor_debug_info->field_139)
			{
				real_vector3d v;
				real_point3d p0;
				real_point3d p1;
				real_point3d p2;

				render_debug_line(TRUE, &actor_debug_info->field_120, &actor_debug_info->field_13C, global_real_argb_purple);
				render_debug_sphere(TRUE, &actor_debug_info->field_13C, 0.2f, global_real_argb_purple);
				
				point_from_line3d(&actor_debug_info->field_108, &actor_debug_info->field_12C, actor_debug_info->field_14C, &p0);
				render_debug_line(TRUE, &actor_debug_info->field_108, &p0, global_real_argb_green);
				
				point_from_line3d(&actor_debug_info->field_108, &actor_debug_info->field_12C, actor_debug_info->field_148, &p1);
				perpendicular3d(&actor_debug_info->field_12C, &v);
				
				p0.x = p1.x + v.i*0.3f;
				p0.y = p1.y + v.j*0.3f;
				p0.z = p1.z + v.k*0.3f;
				p2.x = p1.x - v.i*0.3f;
				p2.y = p1.y - v.j*0.3f;
				p2.z = p1.z - v.k*0.3f;
				render_debug_line(TRUE, &p0, &p2, global_real_argb_green);
			}

			render_debug_vector(
				TRUE,
				&actor_debug_info->field_108,
				&actor_debug_info->field_12C,
				2.f,
				actor_debug_info->field_138 ?
					(actor_debug_info->field_139 ? global_real_argb_yellow : global_real_argb_purple) :
					global_real_argb_white);
		}

		/* Vehicle avoidance */

		if (ai_debug.render_vehicle_avoidance &&
			actor_debug_info->last_vehicle_avoidance_time!=NONE &&
			actor_debug_info->last_vehicle_avoidance_time+TICKS_PER_SECOND >= game_time_get())
		{
			real_point3d p0;
			real_point3d p1;
			real_point3d p2;

			real_vector3d v0;

			render_debug_line(TRUE, &actor_debug_info->field_C8, &actor_debug_info->field_E4, global_real_argb_green);
			render_debug_sphere(TRUE, &actor_debug_info->field_D4, actor_debug_info->field_E0, global_real_argb_yellow);
		
			p1 = actor_debug_info->field_E4;
			p0 = p1;

			p0.x = p0.x - 0.2f;
			p1.x = p1.x + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_red);
			
			p0.x = p0.x + 0.2f;
			p1.x = p1.x - 0.2f;
			p0.y = p0.y - 0.2f;
			p1.y = p1.y + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_red);
			
			p0.y = p0.y + 0.2f;
			p1.y = p1.y - 0.2f;
			p0.z = p0.z - 0.2f;
			p1.z = p1.z + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_red);
			vector_from_points3d(&actor_debug_info->field_C8, &actor_debug_info->field_E4, &v0);
			point_from_line3d(&actor_debug_info->field_C8, &v0, actor_debug_info->field_F0, &p2);
			
			p1 = p2;
			p0 = p2;

			p0.x = p2.x - 0.2f;
			p1.x = p2.x + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			p0.x = p0.x + 0.2f;
			p1.x = p1.x - 0.2f;
			p0.y = p0.y - 0.2f;
			p1.y = p1.y + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			p0.y = p0.y + 0.2f;
			p1.y = p1.y - 0.2f;
			p0.z = p0.z - 0.2f;
			p1.z = p1.z + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);

			if (actor_debug_info->field_F4)
			{

				render_debug_line(TRUE, &actor_debug_info->field_D4, &actor_debug_info->field_F8, global_real_argb_yellow);
				render_debug_line(TRUE, &actor_debug_info->field_C8, &actor_debug_info->field_F8, global_real_argb_red);
				
				p1 = actor_debug_info->field_F8;
				p0 = p1;
				
				p0.x = p0.x - 0.2f;
				p1.x = p1.x + 0.2f;
				render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
				
				p0.x = p0.x + 0.2f;
				p1.x = p1.x - 0.2f;
				p0.y = p0.y - 0.2f;
				p1.y = p1.y + 0.2f;
				render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
				
				p0.y = p0.y + 0.2f;
				p1.y = p1.y - 0.2f;
				p0.z = p0.z - 0.2f;
				p1.z = p1.z + 0.2f;
				render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			}
		}

		/* Projectile aiming */

		if (ai_debug.render_projectile_aiming &&
			actor_debug_info->last_projectile_aiming_time!=NONE &&
			actor_debug_info->last_projectile_aiming_time+15 >= game_time_get())
		{
			real_point3d p0;
			real_point3d p1;

			p1 = actor_debug_info->field_64;
			p0 = p1;
			
			p0.x = p0.x - 0.2f;
			p1.x = p1.x + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			p0.x = p0.x + 0.2f;
			p1.x = p1.x - 0.2f;
			p0.y = p0.y - 0.2f;
			p1.y = p1.y + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			p0.y = p0.y + 0.2f;
			p1.y = p1.y - 0.2f;
			p0.z = p0.z - 0.2f;
			p1.z = p1.z + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			render_debug_vector(
				TRUE,
				&actor_debug_info->field_64,
				&actor_debug_info->field_70,
				2.f,
				actor_debug_info->field_60 ? global_real_argb_blue : global_real_argb_red);
			
			if (!actor_debug_info->field_60)
			{
				render_debug_sphere(TRUE, &actor_debug_info->field_7C, 0.2f, global_real_argb_red);
			}

			if (actor_debug_info->field_88)
			{
				render_debug_vector(
					TRUE,
					&actor->input.position.head_position,
					&actor_debug_info->field_98,
					1.f,
					global_real_argb_white);
				render_debug_vector(
					TRUE,
					&actor_debug_info->field_64,
					&actor_debug_info->field_8C,
					2.f,
					global_real_argb_purple);
			}
		}

		/* Burst Geometry */

		if (ai_debug.render_burst_geometry && actor->control.fire_state==_actor_fire_state_bursting)
		{
			real_point3d p0;
			real_point3d p1;

			render_debug_line(TRUE, &actor->control.burst_initial_position, &actor_debug_info->burst_last_known_position, global_real_argb_green);
			
			if (magnitude_squared3d(&actor_debug_info->burst_lead_vector)>_real_epsilon)
			{
				render_debug_vector(
					TRUE,
					&actor_debug_info->burst_tracked_position,
					&actor_debug_info->burst_lead_vector,
					1.f,
					global_real_argb_purple);
			}

			p1 = actor->control.burst_origin;
			p0 = p1;

			p0.x = p0.x - 0.2f;
			p1.x = p1.x + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			p0.x = p0.x + 0.2f;
			p1.x = p1.x - 0.2f;
			p0.y = p0.y - 0.2f;
			p1.y = p1.y + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);

			p0.y = p0.y + 0.2f;
			p1.y = p1.y - 0.2f;
			p0.z = p0.z - 0.2f;
			p1.z = p1.z + 0.2f;
			render_debug_line(TRUE, &p0, &p1, global_real_argb_blue);
			
			render_debug_sphere(TRUE, &actor->control.burst_target, 0.1f, global_real_argb_red);
			render_debug_vector(TRUE, &actor->control.burst_target, &actor->control.burst_adjustment, 5.f, global_real_argb_red);
		}


		/*  Vision cones */

		if (render_exclusive && ai_debug.render_vision_cones)
		{
			real max_distance;
			real perception_factor;
			real angle_itr;
			real_point3d last_points[2][2][2];

			real const angle_step = 0.08726646f;

			real_argb_color const *const *colors[2] =
			{
				&global_real_argb_red,
				&global_real_argb_blue
			};
			
			if (actor_debug_info->vision_last_time!=NONE &&
				actor_debug_info->vision_last_time+15 >= game_time_get())
			{
				max_distance = actor_debug_info->vision_last_maximum_distance;
				perception_factor = actor_debug_info->vision_last_perception_factor;
			}
			else
			{
				max_distance = actor_definition->perception.maximum_vision_distance;
				perception_factor = 1.f;
			}

			render_debug_vector(
				TRUE,
				&actor->input.position.head_position,
				&actor->input.looking_vector,
				max_distance*perception_factor,
				global_real_argb_yellow);

			for (angle_itr = 0.f; angle_itr<actor_definition->perception.peripheral_vision_angle+angle_step; angle_itr+=angle_step)
			{
				real_vector3d direction_vector[2][2];
				real_point3d current_points[2][2][2];
				real distance_references[2]; /* [0] full, [1] partial: both filled by actor_get_vision_distances */

				short side_index;
				short ring_index;
				short height_index;

				real cosine_vertical_angle[2];
				real sine_vertical_angle[2];

				real horizontal_angle = angle_itr>actor_definition->perception.peripheral_vision_angle ? actor_definition->perception.peripheral_vision_angle : angle_itr;
				real cosine_horizontal_angle = cosine(horizontal_angle);
				real sine_horizontal_angle = sine(horizontal_angle);
				
				cosine_vertical_angle[0] = cosine(DEGREES_TO_RADIANS(30));
				sine_vertical_angle[0] = sine(DEGREES_TO_RADIANS(30));
				cosine_vertical_angle[1] = cosine(DEGREES_TO_RADIANS(45));
				sine_vertical_angle[1] = -sine(DEGREES_TO_RADIANS(45));

				for (ring_index = 0; ring_index<2; ++ring_index)
				{
					for (height_index = 0; height_index<2; ++height_index)
					{
						real_vector3d headspace_vector;
						headspace_vector.i = cosine_horizontal_angle * cosine_vertical_angle[height_index];
						headspace_vector.j = sine_horizontal_angle * cosine_vertical_angle[height_index] * ((real)(ring_index==0 ? 1 : -1));
						headspace_vector.k = sine_vertical_angle[height_index];

						direction_vector[ring_index][height_index] = *global_zero_vector3d;
						point_from_line3d((real_point3d *)&direction_vector[ring_index][height_index], &actor->input.looking_vector, headspace_vector.i, (real_point3d *)&direction_vector[ring_index][height_index]);
						point_from_line3d((real_point3d *)&direction_vector[ring_index][height_index], &actor->input.looking_left_vector, headspace_vector.j, (real_point3d *)&direction_vector[ring_index][height_index]);
						point_from_line3d((real_point3d *)&direction_vector[ring_index][height_index], &actor->input.looking_up_vector, headspace_vector.k, (real_point3d *)&direction_vector[ring_index][height_index]);
					}
				}

				actor_get_vision_distances(actor_index, max_distance, perception_factor, horizontal_angle, &distance_references[0], &distance_references[1]);
			
				for (side_index = 0; side_index < 2; ++side_index)
				{
					for (ring_index = 0; ring_index < 2; ++ring_index)
					{
						for (height_index = 0; height_index < 2; ++height_index)
						{
							point_from_line3d(
								&actor->input.position.head_position,
								&direction_vector[ring_index][height_index],
								distance_references[side_index],
								&current_points[side_index][ring_index][height_index]);

							if (angle_itr>0.f || ring_index==0)
							{
								render_debug_line(
									TRUE,
									&current_points[side_index][ring_index][height_index],
									&actor->input.position.head_position,
									*colors[side_index]);
							}

							if (angle_itr>0.f)
							{
								render_debug_line(
									TRUE,
									&last_points[side_index][ring_index][height_index],
									&current_points[side_index][ring_index][height_index],
									*colors[side_index]);
							}
						}

						if (angle_itr>0.f || ring_index==0)
						{
							render_debug_line(TRUE, &current_points[side_index][ring_index][0], &current_points[side_index][ring_index][1], *colors[side_index]);
						}
					}
				}

				memcpy(last_points, current_points, sizeof(last_points));
			}
		}

		/* Detailed state */

		if (render_exclusive &&ai_debug.render_detailed_state)
		{
			struct prop_datum *prop;
			struct prop_iterator iterator;
			char buffer[1024];

			short tabs[7] =
			{
				100,
				175,
				250,
				325,
				400,
				475,
				550
			};

			sprintf(buffer, "|n|n|n|n");

			if (actor->meta.unit_index!=NONE)
			{
				sprintf(
					&buffer[strlen(buffer)],
					"body|t%3.2f|t[0.0,%3.2f]|nshield|t%3.2f|t[0.0,%3.2f]|n|n",
					object_get_actual_body_vitality(actor->meta.unit_index, FALSE),
					object_get_maximum_body_vitality(actor->meta.unit_index, FALSE),
					object_get_actual_shield_vitality(actor->meta.unit_index, FALSE),
					object_get_maximum_shield_vitality(actor->meta.unit_index, FALSE));
			}

			sprintf(&buffer[strlen(buffer)], "|ntype|tstate|tvis|taud|tlos|ttarget|tlook|n");

			prop_iterator_new(&iterator, actor_index);

			while (prop = prop_iterator_next(&iterator))
			{
				char const *states[NUMBER_OF_PROP_STATES] =
				{
					"-----",
					"->ack",
					"ack->",
					" ack ",
					"u/orph",
					"i/orph"
				};

				char const *los[NUMBER_OF_AI_LINE_OF_SIGHTS] =
				{
					"clear",
					"occl",
					"f/cvr",
					"to/cvr",
					"obstr"
				};

				char const *lighting_state_strings[NUMBER_OF_PROP_LIGHTING_STATES] =
				{
					" (dark)",
					" (dim)",
					""
				};

				char const *perceptions[NUMBER_OF_ACTOR_PERCEPTION_TYPES] =
				{
					"",
					"part",
					"full",
					NULL
				};

				sprintf(
					&buffer[strlen(buffer)],
					"%s|t%s|t%s%s|t%s|t%s|t%3.2f|t%3.2f|n",
					tag_get_name(object_get(prop->unit_index)->definition_index),
					states[prop->state],
					perceptions[prop->visibility],
					lighting_state_strings[prop->lighting],
					perceptions[prop->audibility],
					los[prop->line_of_sight],
					prop->target_weight,
					prop->look_interest);
			}

			draw_string_set_tab_stops(tabs, NUMBEROF(tabs));
			draw_string_set_color(global_real_argb_white);
			rasterizer_draw_string(NULL, NULL, NULL, 0, buffer);
			draw_string_set_tab_stops(NULL, 0);
		}

		/* Paths */

		if (ai_debug.render_paths && (!ai_debug.render_paths_selected_only || render_exclusive))
		{
			if (ai_debug.render_paths_current && actor_path_has_path(actor_index))
			{
				short step_index;
				short first_index;
				real_argb_color const *color;
				real_point3d offset_point;

				if (actor->control.path.at_destination)
				{
					color = global_real_argb_yellow;
				}
				else
				{
					color = actor->control.path.path.steps_finish_path ? global_real_argb_pink : global_real_argb_purple;
				}

				first_index = actor->control.path.path.step_index;

				render_debug_line_offset(
					TRUE,
					&actor->input.position.body_position,
					&actor->control.path.path.steps[first_index].point,
					color,
					0.1f);

				for (
					step_index = first_index;
					step_index<actor->control.path.path.step_count;
					++step_index)
				{
					if (step_index>first_index)
					{
						render_debug_line_offset(
							TRUE,
							&actor->control.path.path.steps[step_index-1].point,
							&actor->control.path.path.steps[step_index].point,
							color,
							0.1f);
					}
						
					point_from_line3d(&actor->control.path.path.steps[step_index].point, global_up3d, 0.1f, &offset_point);
					render_debug_tick(TRUE, &offset_point, global_up3d, 0.02, color);
				}

				point_from_line3d(&actor->control.path.path.endpoint.point, global_up3d, 0.1f, &offset_point);
				render_debug_sphere(TRUE, &offset_point, 0.15f, color);
			}

			if (actor_path_has_path(actor_index))
			{
				sprintf(
					temporary,
					"following path (%d/%d%s)",
					actor->control.path.path.step_index,
					actor->control.path.path.step_count,
					actor_path_at_destination(actor_index) ? " (at destination)" : "");
			}
			else
			{
				strcpy(temporary, "no current path");
			}

			if (actor->emotions.ignorant_of_broken_surfaces)
			{
				strcat(temporary, " [ignorant]");
			}

			render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary, global_real_argb_orange);
			
			if (actor_debug_info->last_path_refresh==NONE || actor_debug_info->last_path_refresh+150 < game_time_get())
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), "not refreshing path", global_real_argb_blue);
			}
			else
			{
				render_debug_string_at_point(
					TRUE,
					ai_debug_drawstack(),
					csprintf(temporary, "path refreshed (%d)", game_time_get()-actor_debug_info->last_path_refresh),
					global_real_argb_blue);
				
				if (path && path->valid)
				{
					ai_debug_render_path_storage(path);
				}
				else
				{
					render_debug_string_at_point(TRUE, ai_debug_drawstack(), "path debugging not available", global_real_argb_red);
				}
			}
		}

		/* Postcombat */

		if (ai_debug.render_postcombat && actor->external_orders.postcombat_type>0)
		{
			render_debug_string_at_point(TRUE, ai_debug_drawstack(), postcombat_type_strings[actor->external_orders.postcombat_type], global_real_argb_green);
			
			if (actor->external_orders.postcombat_prop_index!=NONE)
			{
				struct prop_datum *prop = prop_get(actor->external_orders.postcombat_prop_index);
				
				render_debug_line(TRUE, &actor->input.position.head_position, &prop->head_position, global_real_argb_green);
			}
		}
	}

	return;
}

/* Render the path-node overlays selected by the caller. */
static void ai_debug_render_path_nodes(
	struct path_state *state,
	boolean render_surfaces,
	boolean render_all_nodes,
	boolean render_polygons,
	boolean render_costs,
	boolean render_closest)
{
	struct path_node *node;
	struct path_node *previous_node;
	short node_index;

	real_argb_color const *polygon_color = render_polygons ? global_real_argb_purple : NULL;
	real_argb_color const *distance_color = render_costs ? global_real_argb_white : NULL;
	real_argb_color const *weight_color = render_costs ? global_real_argb_yellow : NULL;
	real_argb_color const *cost_color = render_costs ? global_real_argb_red : NULL;
	real_argb_color const *closest_color = render_closest ? global_real_argb_yellow : NULL;

	global_ai_debug_path_render_id++;

	if (state->destination_valid)
	{
		previous_node = NULL;
		node_index = path_node_from_hash_table(state, state->destination.surface_index);

		while (node_index!=NONE)
		{
			node = path_get_node(state, node_index);

			ai_debug_render_path_node(node,
				previous_node==NULL ? &state->destination.point : &previous_node->entry_point,
				state->structure, render_surfaces, state, previous_node,
				global_real_argb_red, polygon_color, distance_color, weight_color,
				cost_color, closest_color);

			node->last_render_id = global_ai_debug_path_render_id;
			node_index = node->parent_node_index;
			previous_node = node;
		}
	}

	if (render_all_nodes)
	{
		struct collision_bsp const *collision_bsp = TAG_BLOCK_GET_ELEMENT(
			&state->structure->collision_bsp, 0, struct collision_bsp);
		short index;

		for (index = state->node_count-1; index>=0; index--)
		{
			previous_node = NULL;
			node_index = index;

			while (node_index!=NONE)
			{
				node = path_get_node(state, node_index);

				if (node->last_render_id==global_ai_debug_path_render_id)
				{
					if (previous_node)
					{
						render_debug_line_offset(TRUE, &previous_node->entry_point,
							&node->entry_point, global_real_argb_blue, 0.1f);
					}

					break;
				}

				{
					real_point3d centre = *global_origin3d;
					real_point3d const *point;

					if (!previous_node)
					{
						if (render_surfaces)
						{
						struct collision_surface const *surface = TAG_BLOCK_GET_ELEMENT(
								&collision_bsp->surfaces, node->surface_index,
								struct collision_surface);
							long edge_index = surface->first_edge_index;
							long vertex_count = 0;
							real scale;

							do
							{
								struct collision_edge const *edge = TAG_BLOCK_GET_ELEMENT(
									&collision_bsp->edges, edge_index, struct collision_edge);
								const boolean next_index_belongs_to_surface =
									edge->surface_indices[1]==node->surface_index;
								struct collision_vertex const *vertex = TAG_BLOCK_GET_ELEMENT(
									&collision_bsp->vertices,
									edge->vertex_indices[next_index_belongs_to_surface],
									struct collision_vertex);

								add_vectors3d(
									(real_vector3d const *)&centre,
									(real_vector3d const *)&vertex->point,
									(real_vector3d *)&centre);
								vertex_count++;

								edge_index = edge->edge_indices[next_index_belongs_to_surface];
							}
							while (edge_index!=surface->first_edge_index);

							scale = 1.0f / vertex_count;
							centre.x *= scale;
							centre.y *= scale;
							centre.z *= scale;

							point = &centre;
						}
						else
						{
							centre = node->closest_point_to_attractor;
							point = &centre;
						}
					}
					else
					{
						point = &node->entry_point;
					}

					ai_debug_render_path_node(node, point,
						state->structure, render_surfaces, state, previous_node,
						global_real_argb_blue, polygon_color, distance_color, weight_color,
						cost_color, closest_color);
				}

				node->last_render_id = global_ai_debug_path_render_id;
				previous_node = node;
				node_index = node->parent_node_index;
			}
		}
	}

	return;
}

static void ai_debug_render_path_storage(
	struct path_debug_storage *path)
{
	if (path && path->valid && path->last_render_id != ai_debug.last_render_id)
	{
		char const *path_traverse_result_strings[NUMBER_OF_PATH_TRAVERSE_RESULTS] =
		{
			"none",
			"invalid start",
			"not close enough",
			"exhausted search",
			"overflowed nodes",
			"success"
		};

		char const *path_build_result_strings[NUMBER_OF_PATH_BUILD_RESULTS] =
		{
			"none",
			"no destination",
			"cached node missing",
			"not close enough",
			"obstacles blocked",
			"success"
		};

		boolean const matching_bsp = path->structure_bsp_index==global_structure_bsp_index_get();

		match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 3944, (path->path_traverse_result >= 0) && (path->path_traverse_result < NUMBER_OF_PATH_TRAVERSE_RESULTS));
		match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 3945, (path->path_build_result >= 0) && (path->path_build_result < NUMBER_OF_PATH_BUILD_RESULTS));

		render_debug_string_at_point(
			TRUE,
			ai_debug_drawstack(),
			csprintf(
				temporary,
				"%s / %s (%d)",
				path_traverse_result_strings[path->path_traverse_result],
				path_build_result_strings[path->path_build_result],
				game_time_get()-path->path_time),
			path->path_traverse_result!=_path_traverse_result_success || path->path_build_result!=_path_build_result_success ?
			global_real_argb_red :
			global_real_argb_green
		);

		if (ai_debug.render_paths_destination)
		{
			if (path->path_state.destination_valid != 0)
			{
				render_debug_line_offset(
					TRUE,
					&path->path_state.input.start_point,
					&path->path_state.destination.point,
					global_real_argb_pink,
					0.1f);
				render_debug_point(TRUE, &path->path_state.destination.point, 0.3f, global_real_argb_green);

				if (path->path_state.destination.target_radius > 0.f)
				{
					render_debug_sphere(
						TRUE,
						&path->path_state.destination.point,
						path->path_state.destination.target_radius,
						global_real_argb_green);
				}

				if (path->path_state.destination.surface_index!=NONE && matching_bsp)
				{
					ai_debug_render_surface(
						path->path_state.structure,
						path->path_state.destination.surface_index,
						0.05f,
						global_real_argb_green);
				}
			}
			else
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(), "undirected", global_real_argb_green);
			}
		}

		if (ai_debug.render_paths_raw)
		{
			ai_debug_render_path_line(&path->path_state.input.start_point, global_real_argb_red,
				path->raw_step_count, path->raw_steps);
		}

		if (ai_debug.render_paths_smoothed)
		{
			ai_debug_render_path_line(&path->path_state.input.start_point, global_real_argb_green,
				path->smoothed_step_count, path->smoothed_steps);
		}

		if (ai_debug.render_paths_avoided)
		{
			ai_debug_render_path_line(&path->path_state.input.start_point, global_real_argb_blue,
				path->avoided_step_count, path->avoided_steps);
		}

		if (ai_debug.render_paths_avoidance_segment>=0 &&
			ai_debug.render_paths_avoidance_segment<path->avoidance_path_count)
		{
			if (ai_debug.render_paths_avoidance_obstacles)
			{
				render_debug_obstacles(
					&path->avoidance_obstacles[ai_debug.render_paths_avoidance_segment],
					path->avoidance_paths[ai_debug.render_paths_avoidance_segment].radius);
			}

			if (ai_debug.render_paths_avoidance_search && matching_bsp)
			{
				render_debug_path(
					&path->avoidance_paths[ai_debug.render_paths_avoidance_segment]);
			}
		}

		if (ai_debug.render_paths_nodes)
		{
			ai_debug_render_path_nodes(&path->path_state, matching_bsp,
				ai_debug.render_paths_nodes_all, ai_debug.render_paths_nodes_polygons,
				ai_debug.render_paths_nodes_costs, ai_debug.render_paths_nodes_closest);
		}

		path->last_render_id = ai_debug.last_render_id;
	}

	return;
}

void ai_debug_sound_point_set(
	void)
{
	return;
}

void ai_debug_lineoffire_new(
	real_point3d const *start,
	real_vector3d const *vector)
{
	ai_debug.lineoffire_valid = TRUE;
	ai_debug.lineoffire_success = FALSE;

	ai_debug.lineoffire_start = *start;
	ai_debug.lineoffire_vector = *vector;

	ai_debug.lineoffire_pill_count = 0;

	return;
}

void ai_debug_lineoffire_success(
	boolean success)
{
	ai_debug.lineoffire_success = success;

	return;
}

void ai_debug_lineofsight_reset(
	void)
{
	ai_debug.lineofsight_point_count = 0;
	ai_debug.lineofsight_pair_count = 0;

	return;
}

void ai_debug_idle_look_clear(
	long unit_index)
{
	ai_debug.idle_look_valid = unit_index!=NONE;
	ai_debug.idle_look_unit_index = unit_index;
	ai_debug.idle_look_prop_count = 0;

	return;
}

void ai_debug_lineoffire_addpill(
	real_point3d const *start,
	real_vector3d const *vector,
	real radius,
	boolean hit)
{
	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4036, ai_debug.lineoffire_valid);

	if (ai_debug.lineoffire_pill_count<MAXIMUM_AI_DEBUG_LINEOFFIRE_PILLS)
	{
		ai_debug.lineoffire_pill_hit[ai_debug.lineoffire_pill_count] = hit;
		ai_debug.lineoffire_pill_start[ai_debug.lineoffire_pill_count] = *start;
		ai_debug.lineoffire_pill_vector[ai_debug.lineoffire_pill_count] = *vector;
		ai_debug.lineoffire_pill_radius[ai_debug.lineoffire_pill_count] = radius;
		ai_debug.lineoffire_pill_count++;
	}

	return;
}

void ai_debug_idle_look_addprop(
	long prop_index,
	real weight)
{
	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 5062, ai_debug.idle_look_valid);

	if (ai_debug.idle_look_prop_count<MAXIMUM_AI_DEBUG_IDLE_LOOK_PROPS)
	{
		ai_debug.idle_look_prop_index[ai_debug.idle_look_prop_count] = prop_index;
		ai_debug.idle_look_prop_weight[ai_debug.idle_look_prop_count] = weight;
		ai_debug.idle_look_prop_count++;
	}

	return;
}

static void ai_debug_communication_toggle_bits(
	long name_count,
	char const **names,
	unsigned long *vector,
	unsigned long vector_size,
	short (*lookup)(char const *))
{
	short set_count = 0;
	short clear_count = 0;
	unsigned long new_vector[BIT_VECTOR_SIZE_IN_LONGS(2048)];
	long name_index;
	long bit_index;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4968, lookup);
	match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4969, vector_size <= 2048);

	csmemset(new_vector, 0, BIT_VECTOR_SIZE_IN_BYTES(vector_size));

	for (name_index = 0; name_index<name_count; name_index++)
	{
		short comm_type = lookup(names[name_index]);

		if (comm_type!=NONE)
		{
			match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4978, (comm_type >= 0) && (comm_type < vector_size));
			BIT_VECTOR_SET_FLAG(new_vector, comm_type, TRUE);
		}
		else if (!csstrcmp(names[name_index], "all"))
		{
			csmemset(new_vector, NONE, BIT_VECTOR_SIZE_IN_BYTES(vector_size));
		}
	}

	for (bit_index = 0; bit_index<vector_size; bit_index++)
	{
		if (BIT_VECTOR_TEST_FLAG(new_vector, bit_index))
		{
			if (BIT_VECTOR_TEST_FLAG(vector, bit_index))
			{
				set_count++;
			}
			else
			{
				clear_count++;
			}
		}
	}

	if (clear_count)
	{
		bit_vector_or(vector_size, new_vector, vector, vector);
		console_printf(NULL, "set %d flags", clear_count);
	}
	else if (set_count)
	{
		bit_vector_not(vector_size, new_vector, new_vector);
		bit_vector_and(vector_size, new_vector, vector, vector);
		console_printf(NULL, "cleared %d flags", set_count);
	}

	return;
}

void ai_debug_communication_suppress(
	long name_count,
	char const **names)
{
	ai_debug_communication_toggle_bits(name_count, names, ai_debug.communication_suppress_vector,
		NUMBER_OF_AI_DEBUG_COMMUNICATION_TYPES, ai_communication_get_type_by_name);

	return;
}

void ai_debug_communication_ignore(
	long name_count,
	char const **names)
{
	ai_debug_communication_toggle_bits(name_count, names, ai_debug.communication_ignore_vector,
		NUMBER_OF_AI_DEBUG_COMMUNICATION_TYPES, ai_communication_get_type_by_name);

	return;
}

void ai_debug_communication_focus(
	long name_count,
	char const **names)
{
	ai_debug_communication_toggle_bits(name_count, names, ai_debug.communication_focus_vector,
		NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES, dialogue_get_vocalization_type_by_name);

	return;
}

static short ai_debug_lineofsight_findpoint(
	real_point3d const *point,
	short key)
{
	long index;

	for (index = 0; index<ai_debug.lineofsight_point_count; index++)
	{
		if (ai_debug.lineofsight_point_key[index]==key &&
			distance_squared3d(point, &ai_debug.lineofsight_point[index])<0.001f*0.001f)
		{
			break;
		}
	}

	if (index>=ai_debug.lineofsight_point_count)
	{
		if (ai_debug.lineofsight_point_count<MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS)
		{
			index = ai_debug.lineofsight_point_count++;

			ai_debug.lineofsight_point[index] = *point;
			ai_debug.lineofsight_point_reference_count[index] = 0;
			ai_debug.lineofsight_point_key[index] = key;
		}
		else
		{
			index = NONE;

			if (!ai_debug.lineofsight_overflowed)
			{
				error(2, "ai_debug_lineofsight: overflowed point buffer (%d) with %d rays and counting",
				MAXIMUM_AI_DEBUG_LINEOFSIGHT_POINTS,
					ai_debug.lineofsight_pair_count);
				ai_debug.lineofsight_overflowed = TRUE;
			}
		}
	}

	if (index!=NONE)
	{
		match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4223, index <= SHORT_MAX);
		ai_debug.lineofsight_point_reference_count[index]++;
	}

	return (short)index;
}

static long ai_debug_lineofsight_storeray(
	short start_index,
	short end_index)
{
	long index = NONE;

	if (start_index!=NONE && end_index!=NONE)
	{
		for (index = 0; index<ai_debug.lineofsight_pair_count; index++)
		{
			if (ai_debug.lineofsight_pair[index].start_index==start_index &&
				ai_debug.lineofsight_pair[index].end_index==end_index)
			{
				break;
			}
		}

		if (index>=ai_debug.lineofsight_pair_count)
		{
			if (ai_debug.lineofsight_pair_count<MAXIMUM_AI_DEBUG_LINEOFSIGHT_PAIRS)
			{
				index = ai_debug.lineofsight_pair_count++;

				ai_debug.lineofsight_pair[index].start_index = start_index;
				ai_debug.lineofsight_pair[index].end_index = end_index;
				ai_debug.lineofsight_pair[index].reference_count = 0;
			}
			else
			{
				index = NONE;

				if (!ai_debug.lineofsight_overflowed)
				{
					error(2, "ai_debug_lineofsight: overflowed ray buffer (%d) with %d points and counting",
					MAXIMUM_AI_DEBUG_LINEOFSIGHT_PAIRS,
						ai_debug.lineofsight_point_count);
					ai_debug.lineofsight_overflowed = TRUE;
				}
			}
		}

		if (index!=NONE)
		{
			ai_debug.lineofsight_pair[index].reference_count++;
		}
	}

	return index;
}

void ai_debug_lineofsight(
	real_point3d const *start,
	short start_key,
	real_point3d const *end,
	short end_key)
{
	ai_debug_lineofsight_storeray(ai_debug_lineofsight_findpoint(start, start_key), ai_debug_lineofsight_findpoint(end, end_key));

	return;
}

void ai_debug_vocalize(
	char const *priority_name,
	char const *vocalization_name)
{
	if (ai_debug.selected_actor_index!=NONE)
	{
		struct actor_datum *actor = actor_get(ai_debug.selected_actor_index);

		ai_debug.render_speech = TRUE;

		if (actor->meta.unit_index!=NONE)
		{
			short priority = unit_get_speech_priority_by_name(priority_name);
			short vocalization_type = dialogue_get_vocalization_type_by_name(vocalization_name);

			if (priority>0 && vocalization_type!=NONE)
			{
				long sound_definition_index = NONE;
				short speech_type = unit_test_speech(actor->meta.unit_index, priority, 1, 1, 0,
					&vocalization_type, &sound_definition_index);

				if (speech_type)
				{
					struct unit_speech_item item;

					csmemset(&item, 0, sizeof(item));

					item.priority = priority;
					item.vocalization_type = vocalization_type;
					item.sound_definition_index = sound_definition_index;

					ai_communication_packet_new(&item.ai);

					unit_speak(actor->meta.unit_index, speech_type, &item);
				}
			}
		}
	}

	return;
}

void ai_debug_speak_list(
	char const *list_name)
{
	if (ai_debug.selected_actor_index!=NONE)
	{
		struct ai_debug_speech_list list[] =
		{
			{ "all", 0, TRUE },
			{ "idle", 0, FALSE },
			{ "involuntary", 6, FALSE },
			{ "hurting people", 21, FALSE },
			{ "being hurt", 29, FALSE },
			{ "killing people", 49, FALSE },
			{ "player kill comments", 80, FALSE },
			{ "friends dying", 96, FALSE },
			{ "shouting", 108, FALSE },
			{ "group communication", 123, FALSE },
			{ "actions", 148, FALSE },
			{ "exclamations", 177, FALSE },
			{ "post-combat actions", 188, FALSE },
			{ "post-combat chatter", 197, FALSE },
			{ NULL, NONE, FALSE }
		};
		struct actor_datum *actor = actor_get(ai_debug.selected_actor_index);
		struct ai_debug_speech_list *entry;

		for (entry = list; entry->name; entry++)
		{
			if (!_stricmp(entry->name, list_name))
			{
				break;
			}
		}

		if (!entry->name)
		{
			console_printf(FALSE, "ai_speak_list: couldn't find the list '%s'... here are the known lists:", list_name);

			for (entry = list; entry->name; entry++)
			{
				console_printf(FALSE, "    %s", entry->name);
			}
		}
		else if (actor->meta.unit_index!=NONE && entry->vocalization_type!=NONE)
		{
			ai_debug.render_speech = TRUE;
			ai_debug.speak_valid = TRUE;
			ai_debug.field_85B28 = 0;
			ai_debug.field_85B21 = TRUE;
			ai_debug.field_85B22 = entry->all;
			ai_debug.speak_unit_index = actor->meta.unit_index;
			ai_debug.speak_vocalization_type = entry->vocalization_type;
		}
	}

	return;
}

void ai_debug_speak(
	char const *vocalization_name)
{
	if (ai_debug.selected_actor_index!=NONE)
	{
		struct actor_datum *actor = actor_get(ai_debug.selected_actor_index);
		short vocalization_type = dialogue_get_vocalization_type_by_name(vocalization_name);

		if (actor->meta.unit_index!=NONE && vocalization_type!=NONE)
		{
			ai_debug.render_speech = TRUE;
			ai_debug.speak_valid = TRUE;
			ai_debug.field_85B28 = 0;
			ai_debug.field_85B21 = FALSE;
			ai_debug.speak_unit_index = actor->meta.unit_index;
			ai_debug.speak_vocalization_type = vocalization_type;
		}
	}

	return;
}

char *ai_debug_describe_actor(
	long actor_index,
	long unit_index,
	boolean include_squad,
	char *buffer,
	long bufsize)
{
	char squad_string[256];
	char unit_string[256];
	char const *model_name;

	csstrcpy(squad_string, "");

	if (include_squad && actor_index!=NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);

		unit_index = actor->meta.unit_index;

		if (actor->meta.encounter_index==NONE)
		{
			csstrcpy(squad_string, "encounterless ");
		}
		else
		{
			struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters, actor->meta.encounter_index&0xffff,
				struct encounter_definition);
			struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
				&encounter->squads, actor->meta.squad_index, struct squad_definition);
			struct platoon_definition *platoon = NULL;

			if (actor->meta.platoon_index!=NONE)
			{
				platoon = TAG_BLOCK_GET_ELEMENT(&encounter->platoons, actor->meta.platoon_index,
					struct platoon_definition);
			}

			if (!platoon)
			{
				sprintf(squad_string, "%s/%s ", encounter->name, squad->name);
			}
			else
			{
				sprintf(squad_string, "%s/(%s) %s ", encounter->name, platoon->name, squad->name);
			}
		}
	}

	model_name = "";

	csstrcpy(unit_string, "");

	if (unit_index!=NONE)
	{
		struct unit_datum *unit = unit_get(unit_index);

		model_name = tag_name_strip_path(
			unit_definition_get(unit->definition_index)->object.model.name);

		if (unit->object.name_index!=NONE)
		{
			sprintf(unit_string, " (%s)", TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->object_names, unit->object.name_index,
				struct scenario_object_name)->name);
		}
	}

	_snprintf(buffer, bufsize, "%s%s%s", squad_string, model_name, unit_string);

	return buffer;
}

void ai_debug_select_encounter(
	long encounter_index)
{
	if (ai_debug.selected_squad_index!=encounter_index)
	{
		ai_debug.selected_squad_index = encounter_index;
		ai_debug.evaluation_context_valid = FALSE;

		csmemset(&ai_debug.evaluation_context, 0, sizeof(ai_debug.evaluation_context));
		csmemset(ai_debug.actor_record, 0, sizeof(ai_debug.actor_record));

		ai_debug_select_actor(encounter_index, NONE);
	}

	return;
}

void ai_debug_select_actor(
	long encounter_index,
	long actor_index)
{
	if (ai_debug.selected_squad_index!=encounter_index || ai_debug.selected_actor_index!=actor_index)
	{
		struct ai_debug_actor_record *record;
		long index;

		ai_debug_select_encounter(encounter_index);

		ai_debug.selected_actor_index = actor_index;
		ai_debug.evaluation_context_valid = FALSE;

		for (record = ai_debug.actor_record, index = NUMBER_OF_AI_DEBUG_ACTOR_RECORDS; index>0; index--, record++)
		{
			record->valid = FALSE;
		}

		ai_debug_idle_look_clear(actor_index);
	}

	return;
}

void ai_debug_initialize_for_new_map(
	void)
{
	long encounter_index = encounter_get_by_name(ai_debug.selected_squad_name);

	ai_debug_clear_storage();

	ai_debug_select_actor(encounter_index, NONE);

	return;
}

void ai_debug_update(
	void)
{
	if (ai_debug.render_lineofsight)
	{
		ai_debug.lineofsight_point_count = 0;
		ai_debug.lineofsight_pair_count = 0;
	}

	if (ai_debug.path)
	{
		if (!ai_debug.path_start_freeze)
		{
			long unit_index = player_control_get_unit_index(0);
			real_point3d point;

			if (unit_index!=NONE && biped_try_and_get(unit_index))
			{
				long surface_index = biped_find_pathfinding_surface_index(unit_index, &point);

				if (surface_index!=NONE)
				{
					ai_debug.path_start_surface_index = surface_index;
					ai_debug.path_start_point = point;
					ai_debug.path_start_unit_index = unit_index;
					ai_debug.path_start_valid = TRUE;
				}
			}
		}

		if (!ai_debug.path_end_freeze)
		{
			struct observer_result const *camera = observer_get_camera(0);

			if (camera)
			{
				struct collision_result collision;
				real_vector3d down_vector;

				ai_profile.meters[_ai_meter_collisions].accumulator++;

				down_vector.i = global_down3d->i*1000.f;
				down_vector.j = global_down3d->j*1000.f;
				down_vector.k = global_down3d->k*1000.f;

				if (collision_test_vector(0x21, &camera->position, &down_vector, NONE, &collision))
				{
					ai_debug.path_end_valid = TRUE;
					ai_debug.path_end_point = collision.point;
					ai_debug.path_end_surface_index = collision.surface_index;
					ai_debug.field_4C814 = 0.f;
				}
			}
		}

		if (ai_debug.path_start_valid)
		{
			struct path_input input;

			path_input_new(&input, 0.2f, FALSE, ai_debug.path_start_unit_index);
			path_input_set_start(&input, &ai_debug.path_start_point, ai_debug.path_start_surface_index);

			if (ai_debug.path_maximum_radius>0.f)
			{
				path_input_set_search_bounds(&input, ai_debug.path_maximum_radius);
			}

			if (ai_debug.path_attractor)
			{
				long unit_index = player_control_get_unit_index(0);

				if (unit_index!=NONE)
				{
					real_point3d point;
					real attractor_weight;
					real attractor_radius;

					unit_get(unit_index);
					object_get_origin(unit_index, &point);

					attractor_weight = ai_debug.path_attractor_weight==0.f ?
						20.f : ai_debug.path_attractor_weight;
					attractor_radius = ai_debug.path_attractor_radius==0.f ?
						8.f : ai_debug.path_attractor_radius;

					path_input_set_attractor(&input, &point, attractor_radius, NONE, attractor_weight);
				}
			}

			path_state_new(&input, &ai_debug.path_state, &ai_debug.path_storage);

			if (ai_debug.path_end_valid && !ai_debug.path_flood)
			{
				path_state_destination(&ai_debug.path_state, &ai_debug.path_end_point,
					ai_debug.path_end_surface_index, ai_debug.path_accept_radius);
			}

			path_state_find(&ai_debug.path_state);

			if (ai_debug.path_end_valid && ai_debug.path_flood)
			{
				path_state_destination(&ai_debug.path_state, &ai_debug.path_end_point,
					ai_debug.path_end_surface_index, ai_debug.path_accept_radius);
			}

			path_state_build_path(&ai_debug.path_state, &ai_debug.field_608A8);

			ai_debug.field_4C818 = TRUE;
			ai_debug.path_storage.valid = TRUE;
			ai_debug.path_storage.path_time = game_time_get();
			ai_debug.path_storage.actor_index = NONE;
		}
	}

	if (ai_debug.fix_defending_guard_firing_positions && game_in_editor())
	{
		struct scenario *scenario = global_scenario_get();
		long squad_count = 0;
		short encounter_index;
		short squad_index;

		for (encounter_index = 0; encounter_index<scenario->ai_encounters.count; encounter_index++)
		{
			struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_encounters, encounter_index, struct encounter_definition);

			for (squad_index = 0; squad_index<encounter->squads.count; squad_index++)
			{
				struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
					&encounter->squads, squad_index, struct squad_definition);
				short group_index;

				for (group_index = NUMBER_OF_FIRING_POSITION_GROUPS-1;
					group_index>_firing_position_group_attacking_guard;
					group_index--)
				{
					squad->firing_position_groups[group_index] =
						squad->firing_position_groups[group_index-1];
				}

				squad->firing_position_groups[_firing_position_group_attacking_guard] =
					squad->firing_position_groups[_firing_position_group_attacking];

				squad_count++;
			}
		}

		console_printf(FALSE, "updated all %d squads' guard positions. glory!", squad_count);

		ai_debug.fix_defending_guard_firing_positions = FALSE;
	}

	if (ai_debug.fix_actor_variants && game_in_editor())
	{
		struct scenario *scenario = global_scenario_get();
		long location_count = 0;
		short encounter_index;
		short squad_index;
		short location_index;

		for (encounter_index = 0; encounter_index<scenario->ai_encounters.count; encounter_index++)
		{
			struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_encounters, encounter_index, struct encounter_definition);

			for (squad_index = 0; squad_index<encounter->squads.count; squad_index++)
			{
				struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
					&encounter->squads, squad_index, struct squad_definition);

				for (location_index = 0; location_index<squad->starting_locations.count; location_index++)
				{
					struct actor_starting_location *location = TAG_BLOCK_GET_ELEMENT(
						&squad->starting_locations, location_index, struct actor_starting_location);

					location->actor_variant_index = NONE;

					location_count++;
				}
			}
		}

		console_printf(FALSE, "reset the actor variant in all %d starting locations. glory!", location_count);

		ai_debug.fix_actor_variants = FALSE;
	}

	ai_debug_speech_update();

	ai_debug_path_storage_update();

	return;
}

void ai_debug_change_selected_actor(
	boolean forward)
{
	struct encounter_datum *encounter = encounter_try_and_get(ai_debug.selected_squad_index);

	if (!encounter)
	{
		console_printf(FALSE, "no encounter selected (use F2/F3)");

		ai_debug_select_actor(NONE, NONE);
	}
	else
	{
		struct encounter_actor_iterator iterator;
		struct actor_datum *actor;
		short actor_number = 0;

		encounter_actor_iterator_new(&iterator, ai_debug.selected_squad_index);

		if (ai_debug.selected_actor_index!=NONE)
		{
			while (encounter_actor_iterator_next(&iterator))
			{
				if (iterator.actor_index==ai_debug.selected_actor_index)
					break;

				actor_number++;
			}
		}

		if (forward)
		{
			actor = encounter_actor_iterator_next(&iterator);
			actor_number++;
		}
		else
		{
			actor = encounter_actor_iterator_prev(&iterator);
			actor_number--;
		}

		if (actor)
		{
			ai_debug_describe_actor(iterator.actor_index, NONE, TRUE, temporary, 256);

			console_printf(FALSE, "actor %d/%d: %s", actor_number+1, encounter->current_count, temporary);

			ai_debug_select_actor(ai_debug.selected_squad_index, iterator.actor_index);
		}
		else
		{
			console_printf(FALSE, "no more actors");

			ai_debug_select_actor(ai_debug.selected_squad_index, NONE);
		}
	}

	return;
}

void ai_debug_change_selected_encounter(
	boolean forward)
{
	long encounter_index = forward ?
		data_next_index(encounter_data, ai_debug.selected_squad_index) :
		data_prev_index(encounter_data, ai_debug.selected_squad_index);
	struct encounter_datum *encounter = encounter_try_and_get(encounter_index);

	if (!encounter)
	{
		console_printf(FALSE, "no more encounters");

		ai_debug_select_encounter(NONE);
	}
	else
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters, encounter_index&0xffff,
			struct encounter_definition);
		char bsp_string[256];

		if (TEST_FLAG(encounter_definition->flags, _encounter_3d_firing_positions_bit))
		{
			csstrcpy(bsp_string, "3d-positions");
		}
		else
		{
			char index_string[256];

			if (encounter_definition->runtime_structure_bsp_reference_index==NONE)
			{
				csstrcpy(index_string, "NONE");
			}
			else
			{
				sprintf(index_string, "%d", encounter_definition->runtime_structure_bsp_reference_index);
			}

			sprintf(bsp_string, "%s-bsp %s",
				TEST_FLAG(encounter_definition->flags, _encounter_manual_structure_bsp_index_bit) ? "manual" : "auto",
				index_string);
		}

		console_printf(FALSE, "encounter %s [%s %s] (%d actors)",
			encounter_definition->name,
			encounter->active ? "active" : "inactive",
			bsp_string,
			encounter->current_count);

		ai_debug_select_encounter(encounter_index);
	}

	return;
}

void ai_debug_teleport_to(
	long encounter_index)
{
	if (encounter_index!=NONE)
	{
		struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters, encounter_index&0xffff,
			struct encounter_definition);

		if (encounter->player_starting_locations.count>0)
		{
			struct data_iterator iterator;
			struct player_datum *player;
			short location_index = 0;

			data_iterator_new(&iterator, player_data);

			while ((player = (struct player_datum *)data_iterator_next(&iterator))!=NULL)
			{
				if (player->unit_index!=NONE)
				{
					struct encounter_player_starting_location *location = TAG_BLOCK_GET_ELEMENT(
						&encounter->player_starting_locations,
						location_index%encounter->player_starting_locations.count,
						struct encounter_player_starting_location);
					real_vector3d facing;

					facing.i = cosine(location->facing);
					facing.j = sine(location->facing);
					facing.k = 0.f;

					object_set_position(player->unit_index, &location->position, &facing, NULL);

					location_index++;
				}
			}
		}
	}

	return;
}

struct path_debug_storage *ai_debug_get_path_storage(
	long actor_index)
{
	short storage_index = NONE;
	short index;

#ifdef HALO_XBOX
	/* the console: the storage (a 115 KB clear and an 82 KB copy for every
	path search) feeds only the AI debug displays; the path code does
	without it (every use tests for NULL) */
	if (!ai_debug.path && !ai_debug.render_paths && !ai_debug.render_paths_failed && !ai_debug.render_all_actors &&
		ai_debug.selected_actor_index == NONE)
	{
		return NULL;
	}
#endif

	for (index = 0; index<MAXIMUM_AI_DEBUG_PATH_STORAGE; index++)
	{
		struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(index)];

		if (path->actor_index==actor_index && !path->failure)
		{
			storage_index = index;
			break;
		}

		if (storage_index==NONE && !path->valid)
		{
			storage_index = index;
		}
	}

	if (storage_index==NONE)
	{
		short oldest_index = NONE;
		long oldest_time = LONG_MAX;

		for (index = 0; index<MAXIMUM_AI_DEBUG_PATH_STORAGE; index++)
		{
			struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(index)];

			match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 291, path->valid);

			if (path->path_time<oldest_time)
			{
				oldest_time = path->path_time;
				oldest_index = index;
			}
		}

		storage_index = oldest_index;
	}

	if (storage_index!=NONE)
	{
		struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(storage_index)];

		csmemset(path, 0, sizeof(struct path_debug_storage));

		path->valid = TRUE;
		path->actor_index = actor_index;
		path->path_time = game_time_get();

		return path;
	}

	return NULL;
}

static void ai_debug_render_lineoffire(
	void)
{
	if (ai_debug.lineoffire_valid)
	{
		real_point3d end_point;
		long index;

		point_from_line3d(
			&ai_debug.lineoffire_start,
			&ai_debug.lineoffire_vector,
			1.f,
			&end_point);

		render_debug_line(
			TRUE,
			&ai_debug.lineoffire_start,
			&end_point,
			ai_debug.lineoffire_success ? global_real_argb_green : global_real_argb_red);

		for (index = 0; index<ai_debug.lineoffire_pill_count; index++)
		{
			render_debug_pill(
				TRUE,
				&ai_debug.lineoffire_pill_start[index],
				&ai_debug.lineoffire_pill_vector[index],
				ai_debug.lineoffire_pill_radius[index],
				ai_debug.lineoffire_pill_hit[index] ? global_real_argb_red : global_real_argb_blue);
		}
	}

	return;
}

static void ai_debug_render_ballistic_lineoffire(
	void)
{
	if (ai_debug.ballistic_lineoffire_valid)
	{
		short index;

		render_debug_point(
			TRUE,
			&ai_debug.ballistic_lineoffire_start,
			0.1f,
			global_real_argb_yellow);

		render_debug_vector(
			TRUE,
			&ai_debug.ballistic_lineoffire_start,
			&ai_debug.ballistic_lineoffire_vector,
			1.f,
			global_real_argb_yellow);

		for (index = 0; index<ai_debug.ballistic_lineoffire_pill_count; index++)
		{
			render_debug_pill(
				TRUE,
				&ai_debug.ballistic_lineoffire_pill_start[index],
				&ai_debug.ballistic_lineoffire_pill_end[index],
				ai_debug.ballistic_lineoffire_pill_radius[index],
				global_real_argb_blue);
		}

		for (index = 0; index<ai_debug.ballistic_lineoffire_point_count-1; index++)
		{
			render_debug_line(
				TRUE,
				&ai_debug.ballistic_lineoffire_point[index],
				&ai_debug.ballistic_lineoffire_point[index+1],
				ai_debug.ballistic_lineoffire_success ? global_real_argb_green :
					(index==ai_debug.ballistic_lineoffire_point_count-2 ?
						global_real_argb_orange : global_real_argb_red));
		}
	}

	return;
}

static void ai_debug_render_lineofsight(
	void)
{
	long index;
	long pair_index;
	long reference_count;
	real_argb_color const **colors[13] =
	{
		&global_real_argb_black,
		&global_real_argb_blue,
		&global_real_argb_lightblue,
		&global_real_argb_cyan,
		&global_real_argb_green,
		&global_real_argb_purple,
		&global_real_argb_salmon,
		&global_real_argb_pink,
		&global_real_argb_magenta,
		&global_real_argb_red,
		&global_real_argb_orange,
		&global_real_argb_yellow,
		&global_real_argb_white
	};

	for (index = 0; index<ai_debug.lineofsight_point_count; index++)
	{
		sprintf(temporary, "%d", ai_debug.lineofsight_point_reference_count[index]);

		reference_count = ai_debug.lineofsight_point_reference_count[index];

		render_debug_string_at_point(
			TRUE,
			&ai_debug.lineofsight_point[index],
			temporary,
			*colors[MIN(reference_count, 12)]);
	}

	for (pair_index = 0; pair_index<ai_debug.lineofsight_pair_count; pair_index++)
	{
		struct ai_debug_lineofsight_pair *pair;

		reference_count = ai_debug.lineofsight_pair[pair_index].reference_count;
		pair = &ai_debug.lineofsight_pair[pair_index];

		render_debug_line(
			TRUE,
			&ai_debug.lineofsight_point[pair->start_index],
			&ai_debug.lineofsight_point[pair->end_index],
			*colors[MIN(reference_count, 12)]);
	}

	return;
}

static void ai_debug_render_aiming_validity(
	void)
{
	if (ai_debug.field_859F4!=ai_debug.selected_actor_index)
	{
		ai_debug.field_859F4 = ai_debug.selected_actor_index;
		ai_debug.field_859F9 = FALSE;
		ai_debug.field_859F8 = FALSE;
	}

	if (ai_debug.field_859F4!=NONE)
	{
		struct actor_datum *actor = actor_get(ai_debug.field_859F4);
		struct observer_result const *camera = observer_get_camera(0);

		if (camera)
		{
			real_point3d *head_position = &actor->input.position.head_position;
			real_vector3d vector;

			vector.i = camera->position.x - head_position->x;
			vector.j = camera->position.y - head_position->y;
			vector.k = camera->position.z - head_position->z;

			if (normalize3d(&vector)>0.f)
			{
				boolean aiming_valid;
				boolean looking_valid;

				actor_looking_test_validity(ai_debug.field_859F4, &vector,
					&aiming_valid, &looking_valid);

				if (aiming_valid)
				{
					ai_debug.field_859F9 = TRUE;
					ai_debug.field_85A08 = vector;
				}

				if (looking_valid)
				{
					ai_debug.field_859F8 = TRUE;
					ai_debug.field_859FC = vector;
				}

				render_debug_vector(TRUE, head_position, &vector, 1.f, global_real_argb_white);
				render_debug_vector(TRUE, head_position, &actor->input.facing_vector, 1.f,
					global_real_argb_red);
			}
		}

		if (ai_debug.field_859F9)
		{
			real_point3d point;

			point.x = global_up3d->i*0.05f + actor->input.position.head_position.x;
			point.y = global_up3d->j*0.05f + actor->input.position.head_position.y;
			point.z = global_up3d->k*0.05f + actor->input.position.head_position.z;

			render_debug_vector(TRUE, &point, &ai_debug.field_85A08, 1.f, global_real_argb_green);
		}

		if (ai_debug.field_859F8)
		{
			real_point3d point;

			point_from_line3d(&actor->input.position.head_position, global_up3d, 0.05f, &point);

			render_debug_vector(TRUE, &point, &ai_debug.field_859FC, 1.f, global_real_argb_blue);
		}
	}

	return;
}

static void ai_debug_render_idle_look(
	void)
{
	if (ai_debug.idle_look_valid)
	{
		struct actor_datum *actor = actor_try_and_get(ai_debug.idle_look_unit_index);

		if (actor)
		{
			short index;

			for (index = 0; index<ai_debug.idle_look_prop_count; index++)
			{
				struct prop_datum *prop = prop_try_and_get(ai_debug.idle_look_prop_index[index]);

				if (prop)
				{
					real_argb_color const *color;
					real_point3d point;

					point_from_line3d(&actor->input.position.head_position, global_up3d, 0.05f, &point);
					point_from_line3d(&point, &prop->actor_to_prop, 0.9f, &point);

					if (actor->control.idle_major_active &&
						actor->control.idle_major_direction.type==1 &&
						actor->control.idle_major_direction.prop_index==ai_debug.idle_look_prop_index[index])
					{
						color = global_real_argb_yellow;
					}
					else
					{
						color = global_real_argb_white;
					}

					render_debug_string_at_point(TRUE, &point,
						csprintf(temporary, "%.2f", ai_debug.idle_look_prop_weight[index]),
						color);
				}
			}
		}
	}

	return;
}

static void ai_debug_render_spatial_effects(
	void)
{
	long time = game_time_get();
	short index = ai_globals->spatial_effect_head;

	while (index!=ai_globals->spatial_effect_tail)
	{
		struct ai_debug_spatial_effect *effect = &ai_globals->spatial_effect[index];

		if (effect->type!=NONE)
		{
			real_argb_color const **colors[3] =
			{
				&global_real_argb_blue,
				&global_real_argb_yellow,
				&global_real_argb_red
			};
			real_argb_color const *color = global_real_argb_white;
			real_point3d *effect_point;
			real_point3d point;

			if (effect->type>=0 && effect->type<3)
			{
				color = *colors[effect->type];
			}

			effect_point = &effect->point;

			render_debug_sphere(TRUE, effect_point, 0.2f, color);

			point.x = global_up3d->i*0.3f + effect_point->x;
			point.y = global_up3d->j*0.3f + effect_point->y;
			point.z = global_up3d->k*0.3f + effect_point->z;

			render_debug_string_at_point(TRUE, &point,
				csprintf(temporary, "c%d t%d", effect->field_02, time - effect->time),
				color);
		}

		index = (index+1)&31;
	}

	return;
}

static void ai_debug_select_this_actor(
	void)
{
	long actor_index = ai_debug_get_this_actor();

	if (actor_index!=NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);

		ai_debug_describe_actor(actor_index, NONE, TRUE, temporary, 256);

		console_printf(FALSE, "selected %s", temporary);

		ai_debug_select_actor(actor->meta.encounter_index, actor_index);
	}
	else
	{
		ai_debug_select_actor(NONE, NONE);
	}

	ai_debug.select_this_actor = FALSE;

	return;
}

static void ai_debug_render_speech(
	void)
{
	struct object_iterator iterator;
	struct unit_datum *unit;

	object_iterator_new(&iterator, _object_mask_biped|_object_mask_vehicle, 0);

	while ((unit = object_iterator_next(&iterator))!=NULL)
	{
		real_point3d head_position;
		real_point3d point;

		unit_get_head_position(iterator.index, &head_position);

		point.x = global_up3d->i*0.1f + head_position.x;
		point.y = global_up3d->j*0.1f + head_position.y;
		point.z = global_up3d->k*0.1f + head_position.z;

		ai_debug_drawstack_setup(&point);

		if (ai_debug.render_dialogue_variants)
		{
			struct unit_definition *definition = unit_definition_get(unit->definition_index);

			if (definition->unit.dialogue_variants.count>0)
			{
				char const *dialogue_name = "<none>";
				short variant_number = NONE;
				short index;

				for (index = 0; index<definition->unit.dialogue_variants.count; index++)
				{
					struct unit_dialogue_variant *variant = TAG_BLOCK_GET_ELEMENT(
						&definition->unit.dialogue_variants, index, struct unit_dialogue_variant);

					if (variant->dialogue_index==unit->unit.dialogue_index)
					{
						variant_number = variant->variant_number;
						break;
					}
				}

				if (unit->unit.dialogue_index!=NONE)
				{
					dialogue_name = tag_name_strip_path(tag_get_name(unit->unit.dialogue_index));
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "variant %d dialogue %d %s",
						unit->object.variant_number, variant_number, dialogue_name),
					global_real_argb_pink);
			}
		}

		if (ai_debug.render_speech)
		{
			if (unit->unit.speech.current.priority>0)
			{
				char const *sound_name = unit->unit.speech.current.sound_definition_index==NONE ?
					"NONE" : tag_name_strip_path(
						tag_get_name(unit->unit.speech.current.sound_definition_index));
				char const *vocalization_name = unit->unit.speech.current.vocalization_type==NONE ?
					"NONE" : dialogue_get_vocalization_name(
						unit->unit.speech.current.vocalization_type, FALSE);

				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%s %s %s (%d %d)",
						unit_get_speech_priority_name(unit->unit.speech.current.priority),
						vocalization_name, sound_name,
						unit->unit.speech.sound_timer, unit->unit.speech.post_delay_timer),
					global_real_argb_white);
			}

			if (unit->unit.speech.queued.priority>0)
			{
				char const *sound_name = unit->unit.speech.queued.sound_definition_index==NONE ?
					"NONE" : tag_name_strip_path(
						tag_get_name(unit->unit.speech.queued.sound_definition_index));
				char const *vocalization_name = unit->unit.speech.queued.vocalization_type==NONE ?
					"NONE" : dialogue_get_vocalization_name(
						unit->unit.speech.queued.vocalization_type, FALSE);

				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%s %s %s",
						unit_get_speech_priority_name(unit->unit.speech.queued.priority),
						vocalization_name, sound_name),
					global_real_argb_yellow);
			}
		}

		if (ai_debug.print_speech && !ai_debug.render_speech)
		{
			short priority = unit->unit.speech.current.priority;

			if (priority>0)
			{
				char string[512];
				real_argb_color const *color;

				/* the priority names are not recovered; these are the values the
				target switches on */
				switch (priority)
				{
					case 2:
					case 7:
					case 10:
						color = global_real_argb_red;
						break;

					case 6:
						color = global_real_argb_blue;
						break;

					default:
						color = global_real_argb_white;
						break;
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					unit_describe_speech(iterator.index, FALSE, sizeof(string), string),
					color);
			}
		}
	}

	return;
}

static void ai_debug_render_vehicles_enterable(
	void)
{
	/* Every loop below shares this index, including the inner loops, so the
	vehicle loop resumes from the index left by the final inner loop. */
	short index;

	for (index = 0; index<ai_globals->enterable_vehicle_count; index++)
	{
		struct ai_debug_enterable_vehicle *vehicle = &ai_globals->enterable_vehicle[index];

		if (unit_try_and_get(vehicle->object_index))
		{
			real_point3d origin;

			object_get_origin(vehicle->object_index, &origin);

			point_from_line3d(&origin, global_up3d, 0.5f, &origin);

			ai_debug_drawstack_setup(&origin);

			render_debug_string_at_point(TRUE, ai_debug_drawstack(),
				csprintf(temporary, "enterable: dist %.1f", vehicle->distance),
				global_real_argb_pink);

			if (vehicle->team_flags)
			{
				char const *team_names[NUMBER_OF_SOLO_CAMPAIGN_TEAMS] =
				{
					"default", "player", "human", "covenant", "flood",
					"sentinel", "unused6", "unused7", "unused8", "unused9"
				};

				sprintf(temporary, "teams:");

				for (index = 0; index<NUMBER_OF_SOLO_CAMPAIGN_TEAMS; index++)
				{
					if (TEST_FLAG(vehicle->team_flags, index))
					{
						csstrcat(temporary, " ");
						csstrcat(temporary, team_names[index]);
					}
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary,
					global_real_argb_pink);
			}

			if (vehicle->actor_type_flags)
			{
				char const *actor_type_names[NUMBER_OF_ACTOR_TYPES] =
				{
					"elite", "jackal", "grunt", "hunter", "engineer", "assassin",
					"player", "marine", "crew", "combat form", "infection form",
					"carrier form", "monitor", "sentinel", "none", "mounted weapon"
				};

				sprintf(temporary, "actor types:");

				for (index = 0; index<NUMBER_OF_ACTOR_TYPES; index++)
				{
					if (TEST_FLAG(vehicle->actor_type_flags, index))
					{
						csstrcat(temporary, " ");
						csstrcat(temporary, actor_type_names[index]);
					}
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary,
					global_real_argb_pink);
			}

			if (vehicle->actor_count>0)
			{
				char string[256];

				sprintf(temporary, "actors:");

				for (index = 0; index<vehicle->actor_count; index++)
				{
					ai_index_to_string(vehicle->actor_index[index], global_scenario_get(),
						string, 256);

					csstrcat(temporary, " ");
					csstrcat(temporary, string);
				}

				render_debug_string_at_point(TRUE, ai_debug_drawstack(), temporary,
					global_real_argb_pink);
			}
		}
	}

	return;
}

static void ai_debug_render_path(
	void)
{
	if (ai_debug.path_start_valid && ai_debug.path_end_valid && !ai_debug.field_608A8)
	{
		real_argb_color const *color;

		if (!ai_debug.path_flood && !ai_debug.path_state.destination_valid)
		{
			color = global_real_argb_blue;
		}
		else if (ai_debug.path_state.node_count==0)
		{
			color = global_real_argb_green;
		}
		else if (ai_debug.path_state.node_count>=PATH_NODE_LIST_SIZE)
		{
			color = global_real_argb_yellow;
		}
		else
		{
			color = global_real_argb_pink;
		}

		render_debug_line(TRUE, &ai_debug.path_start_point, &ai_debug.path_end_point, color);
	}

	if (ai_debug.path_storage.valid)
	{
		ai_debug_drawstack_setup(&ai_debug.path_storage.path_state.input.start_point);

		ai_debug_render_path_storage(&ai_debug.path_storage);
	}

	return;
}

static void ai_debug_render_paths_failed(
	void)
{
	short index;

	for (index = 0; index<MAXIMUM_AI_DEBUG_PATH_STORAGE; index++)
	{
		struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(index)];

		if (path->valid && path->failure)
		{
			char string[256];
			real_point3d stack_base;

			point_from_line3d(&path->path_state.input.start_point, global_up3d, 1.f, &stack_base);

			ai_debug_drawstack_setup(&stack_base);

			ai_debug_describe_actor(path->actor_index, NONE, TRUE, string, 256);

			render_debug_string_at_point(TRUE, ai_debug_drawstack(), string, global_real_argb_red);

			ai_debug_render_path_storage(path);
		}
	}

	return;
}

static void ai_debug_render_all_actors(
	boolean render_inactive_actors)
{
	struct actor_iterator iterator;

	actor_iterator_new(&iterator, !render_inactive_actors);

	while (actor_iterator_next(&iterator))
	{
		ai_debug_render_actor(
			iterator.actor_index,
			iterator.actor_index==ai_debug.selected_actor_index,
			NULL);
	}

	return;
}

static void ai_debug_render_encounter(
	long encounter_index)
{
	long owner_actor_indices[NUMBER_OF_AI_DEBUG_ACTOR_RECORDS];
	struct encounter_definition *definition;
	long history_start_time;
	short index;

	encounter_get(encounter_index);

	definition = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index), struct encounter_definition);

	history_start_time = NONE;

	{
		struct encounter_actor_iterator iterator;

		encounter_actor_iterator_new(&iterator, encounter_index);

		while (encounter_actor_iterator_next(&iterator))
		{
			boolean selected = (ai_debug.selected_actor_index==iterator.actor_index);

			if (selected || ai_debug.selected_actor_index==NONE || ai_debug.render_all_actors)
			{
				ai_debug_render_actor(iterator.actor_index, selected, &history_start_time);
			}
		}
	}

	if (global_ai_debug_firing_position_color_count==NONE)
	{
		global_ai_debug_firing_position_color_count = 0;

		while (global_ai_debug_firing_position_colors[global_ai_debug_firing_position_color_count].alpha<=1.f &&
			global_ai_debug_firing_position_colors[global_ai_debug_firing_position_color_count].red<=1.f &&
			global_ai_debug_firing_position_colors[global_ai_debug_firing_position_color_count].green<=1.f &&
			global_ai_debug_firing_position_colors[global_ai_debug_firing_position_color_count].blue<=1.f)
		{
			global_ai_debug_firing_position_color_count++;
		}
	}

	encounter_build_firing_position_owner_actor_indices(encounter_index, owner_actor_indices);

	for (index = 0; index<definition->firing_positions.count; index++)
	{
		struct firing_position_definition *position = TAG_BLOCK_GET_ELEMENT(&definition->firing_positions, index,
			struct firing_position_definition);
		real_argb_color const *colors[MAXIMUM_NUMBER_OF_FIRING_POSITION_GROUPS];
		boolean owner_flags[MAXIMUM_NUMBER_OF_FIRING_POSITION_GROUPS];
		real_point3d corner[4];
		long num_firing_position_colors = 0;
		long *owner_actor_index;
		long color_index;
		real_point3d point;

		corner[0].z = corner[1].z = corner[2].z = corner[3].z = position->position.z+0.05f;
		corner[0].x = corner[3].x = position->position.x-0.25f;
		corner[1].x = corner[2].x = position->position.x+0.25f;
		corner[0].y = corner[1].y = position->position.y-0.25f;
		corner[2].y = corner[3].y = position->position.y+0.25f;

		csmemset(owner_flags, 0, sizeof(owner_flags));

		if (ai_debug.selected_actor_index==NONE)
		{
			goto default_firing_position_colors;
		}
		if (actor_get(ai_debug.selected_actor_index)->meta.encounter_index!=
			ai_debug.selected_squad_index)
		{
default_firing_position_colors:
			if (!game_in_editor())
			{
				num_firing_position_colors = 1;
				colors[0] = &global_ai_debug_firing_position_colors[
					position->group_index%global_ai_debug_firing_position_color_count];
			}
		}
		else
		{
			real_argb_color const *const *group_color[MAXIMUM_NUMBER_OF_FIRING_POSITION_GROUPS-1] =
			{
				&global_real_argb_red,
				&global_real_argb_orange,
				&global_real_argb_green,
				&global_real_argb_blue,
				&global_real_argb_lightblue,
				&global_real_argb_green,
				&global_real_argb_aqua
			};
			struct actor_datum *actor = actor_get(ai_debug.selected_actor_index);
			struct encounter_definition *actor_encounter = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
				struct encounter_definition);
			struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(&actor_encounter->squads,
				actor->meta.squad_index, struct squad_definition);
			short default_attacking_group;
			short default_defending_group;
			short attacking_group;
			short defending_group;
			short current_group = actor->emotions.currently_defending ? 5 : 2;

			if (TEST_FLAG(squad->firing_position_groups[current_group],
				position->group_index))
			{
				colors[0] = global_real_argb_green;
				num_firing_position_colors = 1;
			}

			default_attacking_group = actor->emotions.currently_defending ? 3 : 0;
			default_defending_group = actor->emotions.currently_defending ? 4 : 1;

			if (actor->state.searching)
			{
				attacking_group = default_defending_group;
				defending_group = default_attacking_group;
			}
			else
			{
				attacking_group = default_attacking_group;
				defending_group = default_defending_group;
			}

			if (TEST_FLAG(squad->firing_position_groups[attacking_group],
				position->group_index))
			{
				colors[num_firing_position_colors++] = *group_color[attacking_group];
			}
			else if (TEST_FLAG(squad->firing_position_groups[defending_group],
				position->group_index))
			{
				owner_flags[num_firing_position_colors] = TRUE;
				colors[num_firing_position_colors++] = *group_color[defending_group];
			}

			if (TEST_FLAG(squad->firing_position_groups[6], position->group_index))
			{
				colors[num_firing_position_colors++] = global_real_argb_aqua;
			}

			if (!num_firing_position_colors)
			{
				colors[num_firing_position_colors++] = global_real_argb_white;
			}
			else
			{
				match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 964,
					num_firing_position_colors < MAXIMUM_NUMBER_OF_FIRING_POSITION_GROUPS);
			}
		}
		owner_actor_index = &owner_actor_indices[index];

		if (*owner_actor_index!=NONE)
		{
			real_point3d polygon[4];
			actor_get(*owner_actor_index);

			polygon[0].z = polygon[1].z = polygon[2].z = polygon[3].z = position->position.z+0.05f;
			polygon[0].x = polygon[3].x = position->position.x-0.375f;
			polygon[1].x = polygon[2].x = position->position.x+0.375f;
			polygon[0].y = polygon[1].y = position->position.y-0.375f;
			polygon[2].y = polygon[3].y = position->position.y+0.375f;

			render_debug_polygon(polygon, 4, actor_action_debug_color(*owner_actor_index));
		}

		if (ai_debug.render_firing_positions)
		{
			real_argb_color const *color = position->surface_index==NONE ?
				global_real_argb_red : global_real_argb_white;
			real_point3d p0;
			real_point3d p1;

			p1 = position->position;
			p0 = position->position;

			p0.z = p0.z+0.5f;
			p1.z = p1.z-0.5f;
			render_debug_line(TRUE, &p0, &p1, color);

			p0.z = p0.z-0.5f;
			p1.z = p1.z+0.5f;
			p0.x = p0.x-0.1f;
			p1.x = p1.x+0.1f;
			render_debug_line(TRUE, &p0, &p1, color);

			p0.x = p0.x+0.1f;
			p1.x = p1.x-0.1f;
			p0.y = p0.y-0.1f;
			p1.y = p1.y+0.1f;
			render_debug_line(TRUE, &p0, &p1, color);
		}

		for (color_index = 0; color_index<num_firing_position_colors; color_index++)
		{
			corner[0].z = corner[1].z = corner[2].z = corner[3].z = corner[0].z+0.05f;

			if (owner_flags[color_index])
			{
				render_debug_line(TRUE, &corner[0], &corner[2], colors[color_index]);
				render_debug_line(TRUE, &corner[1], &corner[3], colors[color_index]);
			}
			else
			{
				render_debug_polygon_edges(corner, 4, colors[color_index]);
			}
		}

		point.x = global_up3d->i*0.2f + position->position.x;
		point.y = global_up3d->j*0.2f + position->position.y;
		point.z = global_up3d->k*0.2f + position->position.z;

		ai_debug_drawstack_setup(&point);

		if (ai_debug.render_pursuit && ai_debug.actor_record[index].pursuit)
		{
			boolean pursued = FALSE;
			boolean examined;
			short examined_count;

			if (ai_debug.selected_actor_index!=NONE)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%3.2f", ai_debug.actor_record[index].firing_position.evaluation),
					ai_debug.actor_record[index].firing_position.valid ?
						global_real_argb_white : global_real_argb_red);
			}

			if (ai_debug.selected_actor_index!=NONE)
			{
				struct pursuit_location const *location =
					actor_get_pursuit_location(ai_debug.selected_actor_index);

				pursued = (location && location->type==1 &&
					location->firing_position_index==index);
			}

			examined = encounter_pursuit_position_already_examined(encounter_index,
				ai_debug.selected_actor_index, index,
				history_start_time==NONE ? 0 : history_start_time, &examined_count, FALSE);
			if (ai_debug.selected_actor_index==NONE)
			{
				examined = FALSE;
			}

			render_debug_string_at_point(TRUE, ai_debug_drawstack(),
				csprintf(temporary, "%d", examined_count),
				pursued ? global_real_argb_yellow :
					examined ? global_real_argb_blue : global_real_argb_white);

			if (ai_debug.evaluation_context_valid &&
				ai_debug.evaluation_context.has_target && ai_debug.evaluation_context.find_path_direction_from_target &&
				ai_debug.actor_record[index].valid &&
				ai_debug.actor_record[index].firing_position.valid)
			{
				real_point3d origin;

				origin.x = ai_debug.evaluation_context.target_point.x+
					ai_debug.actor_record[index].firing_position.path_direction_from_target.i;
				origin.y = ai_debug.evaluation_context.target_point.y+
					ai_debug.actor_record[index].firing_position.path_direction_from_target.j;
				origin.z = ai_debug.evaluation_context.target_point.z+
					ai_debug.actor_record[index].firing_position.path_direction_from_target.k;

				render_debug_line(TRUE, &ai_debug.evaluation_context.target_point, &origin,
					global_real_argb_yellow);
				render_debug_line(TRUE, &origin, &position->position, global_real_argb_green);
			}
		}
		else if (ai_debug.render_evaluations && ai_debug.actor_record[index].valid &&
			!ai_debug.actor_record[index].pursuit &&
			ai_debug.selected_actor_index!=NONE)
		{
			real_argb_color const *color;
			real_argb_color const *string_color = NULL;

			if (!ai_debug.actor_record[index].firing_position.valid)
			{
				color = global_real_argb_red;
			}
			else if (ai_debug.actor_record[index].firing_position.pre_evaluation>0.f)
			{
				string_color = global_real_argb_white;
				color = *owner_actor_index==ai_debug.selected_actor_index ?
					global_real_argb_yellow : global_real_argb_blue;
			}
			else
			{
				color = global_real_argb_white;
			}

			if (string_color)
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%3.2f", ai_debug.actor_record[index].firing_position.pre_evaluation),
					string_color);

				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%3.2f", ai_debug.actor_record[index].firing_position.evaluation), color);
			}
			else
			{
				render_debug_string_at_point(TRUE, ai_debug_drawstack(),
					csprintf(temporary, "%3.2f", ai_debug.actor_record[index].firing_position.evaluation), color);
			}
		}
	}

	return;
}

static void ai_debug_speech_update(
	void)
{
	struct unit_datum *unit;

	if (!ai_debug.speak_valid)
	{
		return;
	}

	if (ai_debug.speak_unit_index==NONE)
	{
		return;
	}

	unit = unit_try_and_get(ai_debug.speak_unit_index);

	if (!unit || TEST_FLAG(unit->object.damage_flags, _object_dead_bit))
	{
		ai_debug.speak_valid = FALSE;
		return;
	}

	if (unit->unit.speech.current.priority!=0)
	{
		return;
	}

	if (ai_debug.field_85B28>0)
	{
		ai_debug.field_85B28--;
	}

	if (ai_debug.field_85B28!=0)
	{
		return;
	}

	if (ai_debug.speak_vocalization_type>=0 &&
		ai_debug.speak_vocalization_type<NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES)
	{
		char const *name;
		short vocalization_type = ai_debug.speak_vocalization_type;
		long sound_definition_index = NONE;
		short speech_type = unit_test_speech(ai_debug.speak_unit_index, 3, 0, 0, 0,
			&vocalization_type, &sound_definition_index);

		if (speech_type>=2)
		{
			struct unit_speech_item item;

			csmemset(&item, 0, sizeof(item));

			item.vocalization_type = vocalization_type;
			item.sound_definition_index = sound_definition_index;
			item.priority = 4;
			item.pause_time = 15;

			ai_communication_packet_new(&item.ai);

			unit_speak(ai_debug.speak_unit_index, speech_type, &item);

			if (sound_definition_index!=NONE)
			{
				char *tag_name = tag_get_name(sound_definition_index);
				char *conditional = strstr(tag_name, "conditional");

				name = tag_name;

				if (conditional)
				{
					char *separator = strchr(conditional, '\\');

					if (separator && ++separator)
					{
						name = separator;
					}
				}
			}
			else
			{
				name = "<none>";
			}
		}
		else
		{
			name = "<none>";
		}

		console_printf(FALSE, "%s: %s",
			dialogue_get_vocalization_name(ai_debug.speak_vocalization_type, FALSE), name);

		if (ai_debug.field_85B21)
		{
			ai_debug.field_85B28 = 15;

			do
			{
				ai_debug.speak_vocalization_type++;

				if (csstrcmp(dialogue_get_vocalization_name(ai_debug.speak_vocalization_type, FALSE), "unused")!=0)
				{
					break;
				}

				if (!ai_debug.field_85B22)
				{
					ai_debug.speak_vocalization_type = NONE;
					break;
				}
			}
			while (ai_debug.speak_vocalization_type<NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES);
		}
		else
		{
			ai_debug.speak_vocalization_type = NONE;
		}
	}

	if (ai_debug.speak_vocalization_type<0 ||
		ai_debug.speak_vocalization_type>=NUMBER_OF_AI_DEBUG_VOCALIZATION_TYPES)
	{
		console_printf(FALSE, "speech done");

		ai_debug.speak_valid = FALSE;
	}

	return;
}

static void ai_debug_path_storage_update(
	void)
{
	short index;
	short other_index;

	for (index = 0; index<MAXIMUM_AI_DEBUG_PATH_STORAGE; index++)
	{
		struct path_debug_storage *path = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(index)];

		if (path->valid && path->failure)
		{
			for (other_index = index+1; other_index<MAXIMUM_AI_DEBUG_PATH_STORAGE; other_index++)
			{
				struct path_debug_storage *other = &actor_path_debug_array[AI_DEBUG_PATH_SLOT(other_index)];

				if (other->valid && other->failure &&
					other->actor_index==path->actor_index &&
					distance_squared3d(&path->path_state.input.start_point, &other->path_state.input.start_point)<0.25f &&
					path->path_state.destination_valid==other->path_state.destination_valid &&
					(!path->path_state.destination_valid ||
						distance_squared3d(&path->path_state.destination.point, &other->path_state.destination.point)<0.25f))
				{
					if (path->path_time<other->path_time)
					{
						path->valid = FALSE;
						break;
					}

					other->valid = FALSE;
				}
			}
		}
	}

	return;
}

static void ai_debug_render_path_line(
	real_point3d const *start,
	real_argb_color const *color,
	short step_count,
	struct path_step *steps)
{
	short index;

	if (step_count>0)
	{
		render_debug_line_offset(TRUE, start, &steps[0].point, color, 0.1f);
	}

	for (index = 0; index<step_count; index++)
	{
		if (index>0)
		{
			render_debug_line_offset(TRUE, &steps[index-1].point, &steps[index].point, color, 0.1f);
		}

		render_debug_tick(TRUE, &steps[index].point, global_up3d, 0.02f, color);
	}

	return;
}

static long ai_debug_get_this_actor(
	void)
{
	long actor_index = NONE;
	short local_player_index = NONE;
	short index;

	for (index = 0; index<MAXIMUM_LOCAL_PLAYERS; index++)
	{
		if (local_player_exists(index))
		{
			local_player_index = index;
			break;
		}
	}

	if (local_player_index!=NONE)
	{
		struct observer_result const *camera = observer_get_camera(local_player_index);
		long ignore_object_index = NONE;
		real_vector3d vector;
		struct collision_result collision;

		match_assert("c:\\halo\\SOURCE\\ai\\ai_debug.c", 4489, camera != NULL);

		if (!director_get_perspective(local_player_index))
		{
			long player_index = local_player_get_player_index(local_player_index);

			if (player_index!=NONE)
			{
				ignore_object_index = player_get(player_index)->unit_index;
			}
		}

		vector.i = camera->forward.i*50.f;
		vector.j = camera->forward.j*50.f;
		vector.k = camera->forward.k*50.f;

		if (collision_test_vector(0x81, &camera->position, &vector, ignore_object_index, &collision) &&
			collision.type==3 && collision.object_index!=NONE)
		{
			struct unit_datum *unit = unit_try_and_get(collision.object_index);

			if (unit)
			{
				actor_index = unit->unit.swarm_actor_index==NONE ?
					unit->unit.actor_index : unit->unit.swarm_actor_index;

				if (actor_index==NONE && unit->unit.driver_object_index!=NONE)
				{
					struct unit_datum *driver = unit_get(unit->unit.driver_object_index);

					actor_index = driver->unit.swarm_actor_index!=NONE ?
						driver->unit.swarm_actor_index : driver->unit.actor_index;
				}
			}
		}
	}

	return actor_index;
}
