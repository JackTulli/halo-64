/*
GAME_ENGINE_KING.C

symbols in this file:
000A0030 0010:
	_king_engine_dispose (0000)
000A0040 0020:
	_point3d_to_point2d (0000)
000A0060 0350:
	_find_hill (0000)
000A03B0 0010:
	_king_engine_dispose_from_old_map (0000)
000A03C0 0020:
	_king_engine_player_added (0000)
000A03E0 0010:
	_king_engine_game_ending (0000)
000A03F0 0020:
	_king_engine_game_starting (0000)
000A0410 0010:
	_king_engine_statistics_append (0000)
000A0420 0010:
	_king_engine_handle_client_message (0000)
000A0430 0010:
	_king_engine_handle_server_message (0000)
000A0440 0010:
	_king_engine_pregame_post_rasterize (0000)
000A0450 0090:
	_player_inside_hill (0000)
000A04E0 0160:
	_king_engine_player_update (0000)
000A0640 01c0:
	_king_calculate_hill_state (0000)
000A0800 0010:
	_king_engine_player_damaged_player (0000)
000A0810 0010:
	_king_engine_player_killed_player (0000)
000A0820 0190:
	_king_engine_display_score (0000)
000A09B0 0010:
	_king_engine_prespawn_player_update (0000)
000A09C0 0040:
	_king_get_score (0000)
000A0A00 0090:
	_render_dynamic_quad_initialize (0000)
000A0A90 02b0:
	_render_dynamic_quad (0000)
000A0D40 0040:
	_king_get_score_string (0000)
000A0D80 0060:
	_king_get_score_header_string (0000)
000A0DE0 0030:
	_king_get_team_score_string (0000)
000A0E10 0020:
	_king_engine_goal_matches_player (0000)
000A0E30 0070:
	_find_next_hill (0000)
000A0EA0 0110:
	_king_engine_initialize_for_new_map (0000)
000A0FB0 03a0:
	_king_engine_post_rasterize (0000)
000A1350 0110:
	_king_engine_update (0000)
0025BDC0 000d:
	??_C@_0N@DGPCNCJC@NULL?5?$CB?$DN?5flag?$AA@ (0000)
0025BDD0 0027:
	??_C@_0CH@IHPMMFJJ@c?3?2halo?2SOURCE?2game?2game_engine_@ (0000)
0025BDF8 0023:
	??_C@_0CD@DHKICDEA@king_globals?4hill_point_count?5?$CB?$DN@ (0000)
0025BE1C 0014:
	??_C@_0BE@EEOONBNI@FAILED?5TO?5FIND?5HILL?$AA@ (0000)
0025BE30 000b:
	??_C@_0L@HEFOOJCG@crown_blue?$AA@ (0000)
0025BE3C 0038:
	??_C@_0DI@GPEOKAOO@failed?5to?5find?5hill?5?$CD?$CFd?5most?5lik@ (0000)
002DE488 0088:
	_king_engine (0000)
0043E948 0230:
	_king_globals (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cseries/errors.h"
#include "game/game_globals.h"
#include "game/game_engine_king.h"
#include "game/game_engine_place.h"
#include "game/players.h"
#include "main/console.h"
#include "math/geometry.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_geometry.h"
#include "rasterizer/rasterizer_models.h"
#include "render/render.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "shaders/shader_definitions.h"
#include "shaders/shaders.h"
#include "tag_files/tag_groups.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "units/units.h"

/* ---------- constants */

enum
{
#ifdef HALO_LINUX
	/* port: king_globals' score slots follow the session player limit */
	MAXIMUM_KING_SCORE_SLOTS = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
#else
	MAXIMUM_KING_SCORE_SLOTS = 16,
#endif
	MAXIMUM_HILL_POINTS = 12,
	MAXIMUM_HILLS = 64,
	NUMBER_OF_DEFAULT_ANIMATION_VALUES = 4,

	HILL_MOVE_TIME = TICKS_PER_MINUTE,
	HILL_CONTROL_TIME = 10*TICKS_PER_SECOND,
	HILL_SCORE_SOUND_INTERVAL = 5*TICKS_PER_SECOND,
	HILL_30_SECOND_WARNING = 30*TICKS_PER_SECOND,
	HILL_60_SECOND_WARNING = TICKS_PER_MINUTE,
};

enum
{
	_multiplayer_sound_60_seconds = 0x2,
	_multiplayer_sound_30_seconds,
	_multiplayer_sound_red_60_seconds,
	_multiplayer_sound_red_30_seconds,
	_multiplayer_sound_blue_60_seconds,
	_multiplayer_sound_blue_30_seconds,
	_multiplayer_sound_hill_move = 0x1E,
	_multiplayer_sound_team_king = 0x20,
	_multiplayer_sound_king = 0x24,
	_multiplayer_sound_hill_contested = 0x27,
	_multiplayer_sound_hill_controlled,
	_multiplayer_sound_countdown_timer_end = 0x2A,
};

enum
{
	_string_place_score_seconds = 0x9B,
	_string_ally_name_is_on_the_hill_score_seconds,
	_string_enemy_name_is_on_the_hill_score_seconds,
	_string_time,
};

/* ---------- macros */

/* ---------- structures */

/* Target-proven vertex layouts used by this translation unit's quad helper. */
struct model_vertex_uncompressed
{
	real_point3d position;
	real_vector3d normal;
	real_vector3d binormal;
	real_vector3d tangent;
	real_point2d texcoord;
	short nodes[2];
	real node_weights[2];
};

struct model_vertex_compressed
{
	real_point3d position;
	unsigned long normal;
	unsigned long binormal;
	unsigned long tangent;
	point2d texcoord;
	byte nodes[2];
	short node_weight;
};

typedef char verify_model_vertex_uncompressed_size[
	sizeof(struct model_vertex_uncompressed) == 0x44 ? 1 : -1];
typedef char verify_model_vertex_compressed_size[
	sizeof(struct model_vertex_compressed) == 0x20 ? 1 : -1];

/* January-local render packet layouts used by render_dynamic_quad. */
struct rasterizer_model_skinning
{
	real_matrix4x3 const *node_matrices;
	short node_matrix_count;
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

typedef char verify_rasterizer_model_begin_parameters_size[
	sizeof(struct rasterizer_model_begin_parameters) == 0xCC ? 1 : -1];

/* January scenario flag layout consumed by the King map scan. */
typedef char verify_scenario_netgame_flag_size[
	sizeof(struct scenario_netgame_flag) == 0x94 ? 1 : -1];
typedef char verify_scenario_netgame_flags_offset[
	offsetof(struct scenario, netgame_flags) == 0x378 ? 1 : -1];

/* ---------- prototypes */

static void point3d_to_point2d(
	real_point3d const *points,
	real_point2d *points2d,
	long point_count);
static void find_hill(
	void);
static long find_next_hill(
	long hill_id);
static boolean player_inside_hill(
	long player_index);
static void king_calculate_hill_state(
	void);

/* ---------- globals */

/* Shared rasterizer defaults, named by the January image. */
extern real_rgb_color global_default_animation_colors[4];
extern real global_default_animation_values[4];

struct king_globals king_globals = { 0 };
static short king_engine_hill_count = 0;
static short king_engine_hills[MAXIMUM_HILLS] = { 0 };

/* ---------- public code */

void king_engine_dispose(
	void)
{
	return;
}

void king_engine_dispose_from_old_map(
	void)
{
	return;
}

void king_engine_game_ending(
	void)
{
	return;
}

void king_engine_statistics_append(
	long statistic)
{
	return;
}

void king_engine_handle_client_message(
	void *message)
{
	return;
}

void king_engine_handle_server_message(
	void *message)
{
	return;
}

void king_engine_pregame_post_rasterize(
	void)
{
	return;
}

void king_engine_post_rasterize(
	void)
{
	struct game_globals *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	long hill_point_count;
	long shader_index;
	long point_index;
	real hill_perimeter;
	real accumulated_distance;
	real inverse_segment_count;
	real texels_per_unit;
	real previous_u;

	global_scenario_get();
	game_globals = scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);
	hill_perimeter = 0.0f;
	shader_index = multiplayer_information->hill_shader.index;
	hill_point_count = king_globals.hill_point_count;

	for (point_index = 0; point_index < king_globals.hill_point_count; point_index++)
	{
		hill_perimeter += distance3d(
			&king_globals.hill_points[point_index],
			&king_globals.hill_points[
				point_index + 1 == king_globals.hill_point_count ? 0 : point_index + 1]);
	}

	inverse_segment_count = (real)(1.0/floor(hill_perimeter + 0.5f));
	texels_per_unit = 1.0f/(inverse_segment_count*hill_perimeter);
	previous_u = 0.0f;
	accumulated_distance = 0.0f;

	for (point_index = 0; point_index < hill_point_count; point_index++)
	{
		struct model_vertex_uncompressed vertices[NUMBER_OF_VERTICES_PER_QUADRILATERAL];
		real_vector3d sides[2];
		real_vector3d normal;
		real edge_length = distance3d(
			&king_globals.hill_points[point_index],
			&king_globals.hill_points[
				point_index + 1 == hill_point_count ? 0 : point_index + 1]);
		real edge_end_u = (edge_length + accumulated_distance)*texels_per_unit;

		accumulated_distance = edge_length + accumulated_distance;
		csmemset(vertices, 0, sizeof(vertices));
		vertices[1].position = king_globals.hill_points[point_index];
		vertices[1].position.z += 0.8f;
		vertices[0].position = king_globals.hill_points[point_index];
		vertices[2].position = vertices[3].position = king_globals.hill_points[
			point_index + 1 == hill_point_count ? 0 : point_index + 1];
		vertices[2].position.z += 0.8f;

		vector_from_points3d(&vertices[0].position, &vertices[1].position, &sides[0]);
		vector_from_points3d(&vertices[1].position, &vertices[2].position, &sides[1]);
		cross_product3d(&sides[0], &sides[1], &normal);
		normalize3d(&normal);
		vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = normal;

		vertices[0].texcoord.x = previous_u*inverse_segment_count;
		vertices[0].texcoord.y = 1.0f;
		vertices[1].texcoord.x = previous_u*inverse_segment_count;
		vertices[1].texcoord.y = 0.2f;
		vertices[2].texcoord.x = edge_end_u*inverse_segment_count;
		vertices[2].texcoord.y = 0.2f;
		vertices[3].texcoord.x = edge_end_u*inverse_segment_count;
		vertices[3].texcoord.y = 1.0f;
		previous_u = edge_end_u;

		render_dynamic_quad(
			vertices,
			shader_index,
			NULL,
			NULL,
			1.0f/inverse_segment_count,
			1.0f);
	}

	return;
}

void king_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type)
{
	return;
}

void king_engine_player_killed_player(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire)
{
	return;
}

void king_engine_prespawn_player_update(
	long player_index)
{
	return;
}

void king_engine_player_added(
	long player_index)
{
	player_get(player_index);

	return;
}

void king_engine_game_starting(
	void)
{
	game_engine_play_multiplayer_sound(
		game_engine_has_teams() ?
			_multiplayer_sound_team_king :
			_multiplayer_sound_king);

	return;
}

wchar_t *king_get_score_string(
	long player_index,
	wchar_t *buffer)
{
	struct player_datum *player = player_get(player_index);

	ticks_to_unicode_time_string(
		player->statistics.multiplayer_statistics.king_statistics.time_on_hill,
		256,
		buffer);

	return buffer;
}

wchar_t *king_get_team_score_string(
	long team_index,
	wchar_t *buffer)
{
	ticks_to_unicode_time_string(
		king_globals.score[team_index],
		256,
		buffer);

	return buffer;
}

boolean king_engine_initialize_for_new_map(
	void)
{
	struct scenario *scenario = global_scenario_get();
	short flag_index;

	csmemset(&king_globals, 0, sizeof(king_globals));
	king_engine_hill_count = 0;
	for (flag_index = 0; flag_index < scenario->netgame_flags.count; flag_index++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag);

		if (flag->type == _netgame_flag_hill)
		{
			boolean found = FALSE;
			short hill_index;

			for (hill_index = 0; hill_index < king_engine_hill_count; hill_index++)
			{
				if (king_engine_hills[hill_index] == flag->team_index)
				{
					found = TRUE;
					break;
				}
			}
			if (!found)
				king_engine_hills[king_engine_hill_count++] = flag->team_index;
		}
	}

	king_globals.hill_id = 0;
	king_globals.hill_timer = HILL_MOVE_TIME;
	king_globals.hill_previous_controller = NONE;
	king_globals.hill_state = king_hill_uncontrolled;
	find_hill();
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine_king.c",
		0x154,
		king_globals.hill_point_count != 0);

	render_dynamic_quad_initialize();

	return TRUE;
}

void king_engine_player_update(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	game_engine_state_message(player_index, NONE, NONE);
	king_globals.on_the_hill[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)] = FALSE;
	if (player->unit_index != NONE)
	{
		struct unit_datum *unit = unit_get(player->unit_index);

		if (game_engine_can_score() && player_inside_hill(player_index))
		{
			king_globals.on_the_hill[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)] = TRUE;
			player->statistics.multiplayer_statistics.king_statistics.time_on_hill++;
			if (king_globals.score_tick[player->team_index] < game_time_get())
			{
				long score_to_win;
				long score;

				king_globals.score[player->team_index]++;
				king_globals.score_tick[player->team_index] = game_time_get();
				score_to_win = game_engine_get_variant()->universal_variant.score_to_win*TICKS_PER_MINUTE;
				score = king_globals.score[player->team_index];
				if (score_to_win - score == HILL_30_SECOND_WARNING)
				{
					if (game_engine_has_teams())
					{
						game_engine_play_multiplayer_sound(
							player->team_index ?
								_multiplayer_sound_blue_30_seconds :
								_multiplayer_sound_red_30_seconds);
					}
					else
					{
						game_engine_play_multiplayer_sound(_multiplayer_sound_30_seconds);
					}
				}
				if (score_to_win - score == HILL_60_SECOND_WARNING)
				{
					if (game_engine_has_teams())
					{
						game_engine_play_multiplayer_sound(
							player->team_index ?
								_multiplayer_sound_blue_60_seconds :
								_multiplayer_sound_red_60_seconds);
					}
					else
					{
						game_engine_play_multiplayer_sound(_multiplayer_sound_60_seconds);
					}
				}
				if (king_globals.score[player->team_index] > 0 &&
					king_globals.score[player->team_index] % HILL_SCORE_SOUND_INTERVAL == 0 &&
					king_globals.score[player->team_index] < score_to_win)
				{
					game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_timer_end);
				}
				if (score >= score_to_win)
					game_engine_end_game();
			}
			game_engine_state_message(player_index, king_message_you_are_on_the_hill, player_index);
		}
	}

	return;
}

boolean king_engine_display_score(
	long player_index,
	long message,
	long message_player_index,
	wchar_t *buffer,
	long buffer_size)
{
	struct player_datum *player = player_get(player_index);
	boolean result = TRUE;
	long string_list_index;
	wchar_t *string;

	switch (message)
	{
	case king_message_enemy_on_the_hill:
	case king_message_ally_on_the_hill:
	case king_message_you_are_on_the_hill:
		{
			struct player_datum *other_player = player_get(message_player_index);
			long score = king_globals.score[other_player->team_index]/TICKS_PER_SECOND;

			if (message == king_message_you_are_on_the_hill)
			{
				wchar_t *place_name = get_place_name(
					game_engine_get_place(player_index, _get_score_team));

				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_string_place_score_seconds);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, place_name, score);
			}
			else if (message == king_message_ally_on_the_hill)
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_string_ally_name_is_on_the_hill_score_seconds);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, other_player->name, score);
			}
			else if (message == king_message_enemy_on_the_hill)
			{
				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					string = unicode_string_list_get_string(
						string_list_index,
						_string_enemy_name_is_on_the_hill_score_seconds);
				}
				else
					string = L"";
				usnprintf(buffer, buffer_size, string, other_player->name, score);
			}
			else
			{
				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine_king.c",
					0x3A2,
					!"unreachable");
			}
		}
		break;

	default:
		result = FALSE;
		break;
	}

	return result;
}

long king_get_score(
	long player_index,
	enum get_score_type score_type)
{
	struct player_datum *player = player_get(player_index);

	if (score_type == _get_score_individual)
		return player->statistics.multiplayer_statistics.king_statistics.time_on_hill;

	return king_globals.score[player->team_index];
}

void render_dynamic_quad_initialize(
	void)
{
	short i;

	global_default_animation_colors[0] = *global_real_rgb_white;
	global_default_animation_colors[1] = *global_real_rgb_white;
	global_default_animation_colors[2] = *global_real_rgb_white;
	global_default_animation_colors[3] = *global_real_rgb_white;
	for (i = 0; i < NUMBER_OF_DEFAULT_ANIMATION_VALUES; i++)
		global_default_animation_values[i] = 0.0f;

	return;
}

void render_dynamic_quad(
	struct model_vertex_uncompressed *verts,
	long shader_index,
	struct render_lighting const *lighting,
	struct render_animation const *animation,
	real u_scale,
	real v_scale)
{
	long triangle_buffer_index;
	long vertex_buffer_index;

	rasterizer_globals.current_lock_operation = _rasterizer_lock_koth;
	triangle_buffer_index = rasterizer_dynamic_triangles_new(2);
	vertex_buffer_index = rasterizer_dynamic_vertices_new(_rasterizer_vertex_type_model_compressed, 4);
	if (triangle_buffer_index != NONE && vertex_buffer_index != NONE)
	{
		void *vertices = rasterizer_dynamic_vertices_lock(vertex_buffer_index);
		short *triangles = rasterizer_dynamic_triangles_lock(triangle_buffer_index);
		struct rasterizer_model_begin_parameters parameters;
		real_point3d centroid;
		struct shader *shader;

		rasterizer_geometry_compress_vertices(
			_rasterizer_vertex_type_model_uncompressed,
			4,
			vertices,
			4*sizeof(struct model_vertex_compressed),
			verts,
			4*sizeof(struct model_vertex_uncompressed));
		triangles[0] = 0;
		triangles[1] = 1;
		triangles[2] = 2;
		triangles[3] = 2;
		triangles[4] = 3;
		triangles[5] = 0;
		rasterizer_dynamic_triangles_unlock(triangle_buffer_index);
		rasterizer_dynamic_vertices_unlock(vertex_buffer_index);

		shader = shader_definition_get(shader_index);
		centroid.x = (verts[0].position.x + verts[1].position.x + verts[2].position.x + verts[3].position.x)*0.25f;
		centroid.y = (verts[0].position.y + verts[1].position.y + verts[2].position.y + verts[3].position.y)*0.25f;
		centroid.z = (verts[0].position.z + verts[1].position.z + verts[2].position.z + verts[3].position.z)*0.25f;

		csmemset(&parameters, 0, sizeof(parameters));
		parameters.unique_identifier = 1;
		parameters.skinning.node_matrices = global_identity4x3;
		parameters.skinning.node_matrix_count = 1;
		if (lighting)
		{
			parameters.lighting = *lighting;
		}
		else
		{
			parameters.lighting.ambient_color = *global_real_rgb_white;
			parameters.lighting.distant_light_count = 0;
			parameters.lighting.point_light_count = 0;
			parameters.lighting.reflection_tint_color = *global_real_argb_white;
			parameters.lighting.shadow_vector.i = 0.0f;
			parameters.lighting.shadow_vector.j = 1.0f;
			parameters.lighting.shadow_vector.k = 0.0f;
			parameters.lighting.shadow_color = *global_real_rgb_black;
		}
		if (animation)
		{
			parameters.animation = *animation;
		}
		else
		{
			parameters.animation.colors = global_default_animation_colors;
			parameters.animation.values = global_default_animation_values;
		}
		parameters.centroid = centroid;
		parameters.base_map_scale.i = u_scale;
		parameters.base_map_scale.j = v_scale;

		rasterizer_profile_enable(FALSE);
		rasterizer_models_begin(FALSE);
		rasterizer_model_begin(&parameters, TRUE);
		if (shader_type_is_transparent(shader->base.type))
		{
			rasterizer_model_transparent_geometry_submit(
				shader,
				0,
				NULL,
				triangle_buffer_index,
				2,
				NULL,
				vertex_buffer_index,
				&centroid,
				NULL);
		}
		else
		{
			rasterizer_model_draw(
				shader,
				0,
				NULL,
				triangle_buffer_index,
				2,
				NULL,
				vertex_buffer_index);
		}
		rasterizer_model_end();
		rasterizer_models_end();
		rasterizer_profile_enable(TRUE);
		rasterizer_dynamic_triangles_delete(triangle_buffer_index);
		rasterizer_dynamic_vertices_delete(vertex_buffer_index);
	}
	rasterizer_globals.current_lock_operation = _rasterizer_lock_none;

	return;
}

wchar_t *king_get_score_header_string(
	wchar_t *buffer)
{
	long string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	wchar_t const *string;

	if (string_list_index != NONE)
	{
		string = unicode_string_list_get_string(
			string_list_index,
			_string_time);
	}
	else
	{
		string = L"";
	}
	ustrcpy(buffer, string);

	return buffer;
}

boolean king_engine_goal_matches_player(
	long player_index,
	long goal_index)
{
	boolean matches = !king_globals.on_the_hill[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)];

	return matches;
}

void king_engine_update(
	void)
{
	if (game_engine_can_score() &&
		game_engine_get_variant()->game_engine_variant.king.moving_hill &&
		--king_globals.hill_timer == 0)
	{
		king_globals.hill_timer = HILL_MOVE_TIME;
		king_globals.hill_id = find_next_hill(king_globals.hill_id);
		find_hill();
		game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move);
		while (king_globals.hill_point_count == 0)
		{
			error(2, "failed to find hill #%d most likely bad point placement", king_globals.hill_id);
			if (king_globals.hill_id == 0)
				break;

			king_globals.hill_id = find_next_hill(king_globals.hill_id);
			find_hill();
			game_engine_play_multiplayer_sound(_multiplayer_sound_hill_move);
		}
	}

	if (king_globals.hill_point_count > 0)
	{
		real_point3d position = king_globals.hill_center;

		game_engine_set_goal_position(0, &position, 0.0f, "crown_blue", NONE, NONE, NONE);
	}
	else
	{
		console_printf(FALSE, "FAILED TO FIND HILL");
	}
	king_calculate_hill_state();

	return;
}


/* ---------- private code */

static void point3d_to_point2d(
	real_point3d const *points,
	real_point2d *points2d,
	long point_count)
{
	long i;

	for (i = 0; i < point_count; i++)
	{
		points2d[i].x = points[i].x;
		points2d[i].y = points[i].y;
	}

	return;
}

static void find_hill(
	void)
{
	struct scenario *scenario = global_scenario_get();
	long flag_indices[MAXIMUM_HILL_POINTS];
	real_point3d points[MAXIMUM_HILL_POINTS];
	real_point2d points2d[MAXIMUM_HILL_POINTS];
	short hull_indices[MAXIMUM_HILL_POINTS];
	real_point3d minimum, maximum;
	long point_count;
	short hull_point_count;
	long i;

	point_count = find_netgame_flags(
		NULL,
		0.0f,
		0.0f,
		_netgame_flag_hill,
		(short)king_globals.hill_id,
		MAXIMUM_HILL_POINTS,
		flag_indices);
	king_globals.hill_point_count = point_count;
	if (point_count)
	{
		for (i = 0; i < point_count; i++)
		{
			struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_flags,
				flag_indices[i],
				struct scenario_netgame_flag);

			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine_king.c",
				0xDE,
				NULL != flag);
			points[i] = flag->position;
		}
		if (point_count == 1)
		{
			points[1] = points[0];
			points[2] = points[0];
			points[3] = points[0];
			points[0].x -= 1.0f;
			points[0].y -= 1.0f;
			points[1].x += 1.0f;
			points[1].y -= 1.0f;
			points[2].x -= 1.0f;
			points[2].y += 1.0f;
			points[3].x += 1.0f;
			points[3].y += 1.0f;
			point_count = 4;
		}
		point3d_to_point2d(points, points2d, point_count);
		hull_point_count = convex_hull2d(point_count, points2d, hull_indices);
		for (i = 0; i < hull_point_count; i++)
		{
			king_globals.hill_points[i] = points[hull_indices[i]];
			king_globals.convex_hull[i] = points2d[hull_indices[i]];
		}
		king_globals.hill_point_count = hull_point_count;

		minimum = king_globals.hill_points[0];
		maximum = king_globals.hill_points[0];
		for (i = 0; i < hull_point_count; i++)
		{
			minimum.x = MIN(minimum.x, king_globals.hill_points[i].x);
			minimum.y = MIN(minimum.y, king_globals.hill_points[i].y);
			minimum.z = MIN(minimum.z, king_globals.hill_points[i].z);
			maximum.x = MAX(maximum.x, king_globals.hill_points[i].x);
			maximum.y = MAX(maximum.y, king_globals.hill_points[i].y);
			maximum.z = MAX(maximum.z, king_globals.hill_points[i].z);
		}
		king_globals.hill_bottom = minimum.z - 0.1f;
		king_globals.hill_top = maximum.z + 0.8f;
		king_globals.hill_center.x = (maximum.x + minimum.x)*0.5f;
		king_globals.hill_center.y = (maximum.y + minimum.y)*0.5f;
		king_globals.hill_center.z = (maximum.z + minimum.z)*0.5f;
	}

	return;
}

static boolean player_inside_hill(
	long player_index)
{
	boolean inside = FALSE;

	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);
			real_point3d const *position = &unit->object.bounding_sphere_center;

			if (position->z >= king_globals.hill_bottom && position->z <= king_globals.hill_top)
			{
				real_point2d point;

				point.x = position->x;
				point.y = position->y;
				inside = convex_hull2d_test_point(
					king_globals.hill_point_count,
					king_globals.convex_hull,
					&point,
					0.0f);
			}
		}
	}

	return inside;
}

static void king_calculate_hill_state(
	void)
{
	struct player_datum *player;

	if (game_engine_has_teams())
	{
		struct data_iterator player_iterator;
		long red_count = 0;
		long blue_count = 0;

		data_iterator_new(&player_iterator, player_data);
		player = (struct player_datum *)data_iterator_next(&player_iterator);
		while (player)
		{
			if (king_globals.on_the_hill[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_iterator.datum_index)])
			{
				if (player->team_index)
					blue_count++;
				else
					red_count++;
			}
			player = (struct player_datum *)data_iterator_next(&player_iterator);
		}

		if (blue_count)
		{
			if (red_count)
			{
				king_globals.hill_state = king_hill_contested;
				if (king_globals.hill_controlled_count > HILL_CONTROL_TIME)
					game_engine_play_multiplayer_sound(_multiplayer_sound_hill_contested);
				king_globals.hill_controlled_count = 0;
			}
			else
			{
				if (king_globals.hill_state == king_hill_controlled_blue)
					king_globals.hill_controlled_count++;
				else
					king_globals.hill_controlled_count = 0;
				king_globals.hill_state = king_hill_controlled_blue;
			}
		}
		else if (!red_count)
		{
			king_globals.hill_state = king_hill_uncontrolled;
			king_globals.hill_controlled_count = 0;
		}
		else
		{
			if (king_globals.hill_state == king_hill_controlled_red)
				king_globals.hill_controlled_count++;
			else
				king_globals.hill_controlled_count = 0;
			king_globals.hill_state = king_hill_controlled_red;
		}
	}
	else
	{
		struct data_iterator player_iterator;
		long player_count = 0;
		long controller;

		data_iterator_new(&player_iterator, player_data);
		player = (struct player_datum *)data_iterator_next(&player_iterator);
		while (player)
		{
			if (king_globals.on_the_hill[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_iterator.datum_index)])
			{
				controller = player_iterator.datum_index;
				player_count++;
			}
			player = (struct player_datum *)data_iterator_next(&player_iterator);
		}

		if (player_count > 1)
		{
			king_globals.hill_state = king_hill_contested;
			if (king_globals.hill_controlled_count > HILL_CONTROL_TIME)
				game_engine_play_multiplayer_sound(_multiplayer_sound_hill_contested);
			king_globals.hill_controlled_count = 0;
			king_globals.hill_previous_controller = NONE;
		}
		else if (player_count)
		{
			if (king_globals.hill_state == king_hill_controlled &&
				controller == king_globals.hill_previous_controller)
			{
				king_globals.hill_controlled_count++;
			}
			else
			{
				king_globals.hill_controlled_count = 0;
				king_globals.hill_previous_controller = controller;
			}
			king_globals.hill_state = king_hill_controlled;
		}
		else
		{
			king_globals.hill_previous_controller = NONE;
			king_globals.hill_state = king_hill_uncontrolled;
			king_globals.hill_controlled_count = 0;
		}
	}

	if (king_globals.hill_controlled_count == HILL_CONTROL_TIME)
		game_engine_play_multiplayer_sound(_multiplayer_sound_hill_controlled);

	return;
}

static long find_next_hill(
	long hill_id)
{
	long next_hill_id;
	short start_index = random_range(0, king_engine_hill_count);
	short i;

	for (i = 0; i < king_engine_hill_count; i++)
	{
		short hill_index = (start_index + i)%king_engine_hill_count;

		if (hill_id != king_engine_hills[hill_index])
			return king_engine_hills[hill_index];
	}

	/* January and the later Xbox build both leave the no-candidate result
	 * undefined. The caller expects maps to provide at least two hill ids. */
	return next_hill_id;
}

/* ---------- engine table */

struct game_engine king_engine =
{
	"king",
	game_engine_king,
	king_engine_dispose,
	king_engine_initialize_for_new_map,
	king_engine_dispose_from_old_map,
	king_engine_player_added,
	king_engine_game_ending,
	king_engine_game_starting,
	king_engine_statistics_append,
	king_engine_handle_client_message,
	king_engine_handle_server_message,
	king_engine_pregame_post_rasterize,
	king_engine_post_rasterize,
	king_engine_player_update,
	NULL,
	NULL,
	NULL,
	king_engine_update,
	king_get_score,
	king_get_score_string,
	king_get_score_header_string,
	king_get_team_score_string,
	NULL,
	king_engine_player_damaged_player,
	king_engine_player_killed_player,
	king_engine_display_score,
	NULL,
	king_engine_prespawn_player_update,
	NULL,
	NULL,
	king_engine_goal_matches_player,
	NULL,
	NULL,
	NULL,
};

#ifdef HALO_LINUX
/* the distributed netcode (port/linux/game/network_distributed.c): the game
type's state the host sends its clients, which take it as it is (the
scores and the hill, which moves) */
long game_engine_king_write_network_state(
	byte *buffer,
	long size)
{
	if (size < (long)sizeof(king_globals))
		return 0;
	csmemcpy(buffer, &king_globals, sizeof(king_globals));
	return sizeof(king_globals);
}

void game_engine_king_read_network_state(
	byte const *buffer,
	long size)
{
	if (size == (long)sizeof(king_globals))
		csmemcpy(&king_globals, buffer, sizeof(king_globals));
}
#endif
