/*
HALO_LINKER_COMMON.C

Game definitions the Linux link needs that no reconstructed translation unit
provides yet. Nothing here is decompiled game code or earns matching credit;
every definition is weak, so the genuine one takes over automatically once
its owning unit is reconstructed.

Globals: in the January image these are tentative definitions that the
linker pooled into one COMMON block (csplit's source/linker_common; see
docs/linker_common_comparison_base_20260922.md). COMMON storage is zero
filled, so each is zeroed storage here. The sizes come from the spacing of
the symbols in config/symbols.json and so are upper bounds of the real
sizes; the declared type is noted beside each.

Functions: fast_ftol_C and main_crash are Halo functions that are still
missing from the reconstruction (docs/xbox_link_probe_20260924.md).
*/

#include "platform.h"

#include <math.h>
#include <stdlib.h>

/* ---------- pooled COMMON globals */

#define HALO_COMMON(name, size) \
	__attribute__((weak, aligned(16))) unsigned char name[size]

HALO_COMMON(ai_globals, 12); /* struct ai_globals_data *ai_globals */
HALO_COMMON(antenna_data, 12); /* struct data_array *antenna_data */
HALO_COMMON(assertion_count, 2); /* short assertion_count */
HALO_COMMON(avoidance_directions, 96); /* real_vector3d avoidance_directions[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS] */
HALO_COMMON(avoidance_rays, 448); /* struct vector_avoidance_ray avoidance_rays[VECTOR_AVOIDANCE_NUMBER_OF_DIRECTIONS][2] */
HALO_COMMON(blip_player_index, 2); /* short blip_player_index */
HALO_COMMON(blur_shader, 256); /* struct pixel_shader_definition blur_shader */
HALO_COMMON(build_sprite_globals, 40); /* struct build_sprite_globals_data build_sprite_globals */
HALO_COMMON(cached_player_profile, 156); /* struct cached_player_profile_entry cached_player_profile[3] */
HALO_COMMON(cached_variant_profile, 352); /* struct cached_variant_profile_entry cached_variant_profile[3] */
HALO_COMMON(center_point, 8); /* real_point2d center_point */
HALO_COMMON(cheat, 12); /* byte cheat[] */
HALO_COMMON(collision_debug, 1); /* byte collision_debug[] */
HALO_COMMON(collision_debug_features, 1); /* byte collision_debug_features[] */
HALO_COMMON(collision_debug_flag_back_facing_surfaces, 1); /* byte collision_debug_flag_back_facing_surfaces[] */
HALO_COMMON(collision_debug_flag_ignore_breakable_surfaces, 1); /* byte collision_debug_flag_ignore_breakable_surfaces[] */
HALO_COMMON(collision_debug_flag_ignore_two_sided_surfaces, 1); /* byte collision_debug_flag_ignore_two_sided_surfaces[] */
HALO_COMMON(collision_debug_flag_objects_bipeds, 1); /* byte collision_debug_flag_objects_bipeds[] */
HALO_COMMON(collision_debug_flag_objects_controls, 1); /* byte collision_debug_flag_objects_controls[] */
HALO_COMMON(collision_debug_flag_objects_equipment, 1); /* byte collision_debug_flag_objects_equipment[] */
HALO_COMMON(collision_debug_flag_objects_light_fixtures, 1); /* byte collision_debug_flag_objects_light_fixtures[] */
HALO_COMMON(collision_debug_flag_objects_machines, 1); /* byte collision_debug_flag_objects_machines[] */
HALO_COMMON(collision_debug_flag_objects_placeholders, 1); /* byte collision_debug_flag_objects_placeholders[] */
HALO_COMMON(collision_debug_flag_objects_projectiles, 1); /* byte collision_debug_flag_objects_projectiles[] */
HALO_COMMON(collision_debug_flag_objects_scenery, 1); /* byte collision_debug_flag_objects_scenery[] */
HALO_COMMON(collision_debug_flag_objects_vehicles, 1); /* byte collision_debug_flag_objects_vehicles[] */
HALO_COMMON(collision_debug_flag_objects_weapons, 1); /* byte collision_debug_flag_objects_weapons[] */
HALO_COMMON(collision_debug_flag_skip_passthrough_bipeds, 1); /* byte collision_debug_flag_skip_passthrough_bipeds[] */
HALO_COMMON(collision_debug_flag_try_to_keep_location_valid, 1); /* byte collision_debug_flag_try_to_keep_location_valid[] */
HALO_COMMON(collision_debug_flag_use_vehicle_physics, 1); /* byte collision_debug_flag_use_vehicle_physics[] */
HALO_COMMON(collision_debug_flags, 4); /* unsigned long collision_debug_flags */
HALO_COMMON(collision_debug_phantom_bsp_point, 12); /* real_point3d collision_debug_phantom_bsp_point */
HALO_COMMON(collision_debug_point, 12); /* real_point3d collision_debug_point */
HALO_COMMON(collision_debug_repeat, 1); /* boolean collision_debug_repeat */
HALO_COMMON(collision_debug_spray, 1); /* byte collision_debug_spray[] */
HALO_COMMON(collision_debug_vector, 16); /* real_vector3d collision_debug_vector */
HALO_COMMON(contrail_data, 4); /* struct data_array *contrail_data */
HALO_COMMON(contrail_point_data, 4); /* struct data_array *contrail_point_data */
HALO_COMMON(conversation_data, 20); /* struct data_array *conversation_data */
HALO_COMMON(current_time, 4); /* unsigned long current_time */
HALO_COMMON(debug_damage, 4); /* byte debug_damage[] */
HALO_COMMON(debug_decals, 4); /* byte debug_decals[] */
HALO_COMMON(debug_fog_planes, 1); /* boolean debug_fog_planes */
HALO_COMMON(debug_ignore_broken_surfaces, 1); /* boolean debug_ignore_broken_surfaces */
HALO_COMMON(debug_leaf_portals, 29); /* boolean debug_leaf_portals */
HALO_COMMON(debug_lights, 1); /* byte debug_lights[] */
HALO_COMMON(debug_looping_sound, 1); /* byte debug_looping_sound[] */
HALO_COMMON(debug_object_lights, 1); /* byte debug_object_lights[] */
HALO_COMMON(debug_objects, 4); /* byte debug_objects[] */
HALO_COMMON(debug_objects_biped_autoaim_pills, 1); /* byte debug_objects_biped_autoaim_pills[] */
HALO_COMMON(debug_objects_biped_physics_pills, 2); /* byte debug_objects_biped_physics_pills[] */
HALO_COMMON(debug_objects_devices, 4); /* boolean debug_objects_devices */
HALO_COMMON(debug_objects_vehicle_powered_mass_points, 1); /* byte debug_objects_vehicle_powered_mass_points[] */
HALO_COMMON(debug_obstacle_path_finishing, 3); /* boolean debug_obstacle_path_finishing */
HALO_COMMON(debug_obstacle_path_goal_point, 12); /* byte debug_obstacle_path_goal_point[] */
HALO_COMMON(debug_obstacle_path_goal_surface_index, 8); /* byte debug_obstacle_path_goal_surface_index[] */
HALO_COMMON(debug_obstacle_path_radius, 4); /* real debug_obstacle_path_radius */
HALO_COMMON(debug_obstacle_path_start_point, 12); /* byte debug_obstacle_path_start_point[] */
HALO_COMMON(debug_obstacle_path_start_surface_index, 4); /* byte debug_obstacle_path_start_surface_index[] */
HALO_COMMON(debug_point_physics, 32); /* byte debug_point_physics[] */
HALO_COMMON(debug_rasterizer_light_count, 6); /* short debug_rasterizer_light_count */
HALO_COMMON(debug_render_freeze, 11); /* byte debug_render_freeze[] */
HALO_COMMON(debug_scripting, 3); /* boolean debug_scripting */
HALO_COMMON(debug_sound, 1); /* byte debug_sound[] */
HALO_COMMON(debug_sound_cache, 1); /* byte debug_sound_cache[] */
HALO_COMMON(debug_sound_channels, 1); /* byte debug_sound_channels[] */
HALO_COMMON(debug_sound_environment, 4); /* boolean debug_sound_environment */
HALO_COMMON(debug_sound_reference_counts, 4); /* boolean debug_sound_reference_counts */
HALO_COMMON(debug_sprites, 24); /* byte debug_sprites[] */
HALO_COMMON(debug_trigger_volumes, 1); /* boolean debug_trigger_volumes */
HALO_COMMON(device_groups_data, 24); /* struct data_array *device_groups_data */
HALO_COMMON(director_camera_scripted, 4); /* struct director_scripting_globals *director_camera_scripted */
HALO_COMMON(dsound_globals, 30924); /* struct dsound_globals dsound_globals */
HALO_COMMON(effect_data, 4); /* struct data_array *effect_data */
HALO_COMMON(effect_location_data, 4); /* struct data_array *effect_location_data */
HALO_COMMON(error_globals, 2080); /* struct error_global_data error_globals */
HALO_COMMON(first_object_type_definition, 4); /* struct object_type_definition *first_object_type_definition */
HALO_COMMON(game_engine_globals, 36); /* struct game_engine_globals game_engine_globals */
HALO_COMMON(game_variant_global, 112); /* struct game_variant game_variant_global */
HALO_COMMON(global_address, 12); /* XNADDR global_address */
HALO_COMMON(global_ai_debug_string_position, 4); /* short global_ai_debug_string_position */
HALO_COMMON(global_communication_table_indices, 116); /* short global_communication_table_indices[NUMBER_OF_COMMUNICATION_TYPES] */
HALO_COMMON(global_d3d_caps, 224); /* D3DCAPS8 global_d3d_caps */
HALO_COMMON(global_decal_data, 4); /* struct data_array *global_decal_data */
HALO_COMMON(global_default_animation_colors, 48); /* real_rgb_color global_default_animation_colors[4] */
HALO_COMMON(global_default_animation_values, 16); /* real global_default_animation_values[4] */
HALO_COMMON(global_frame_parameters, 8); /* struct rasterizer_frame_begin_parameters global_frame_parameters */
HALO_COMMON(global_key, 16); /* XNKEY global_key */
HALO_COMMON(global_key_id, 8); /* XNKID global_key_id */
HALO_COMMON(global_nonce, 8); /* byte global_nonce[TRANSPORT_NONCE_LENGTH] */
HALO_COMMON(global_stage, 192); /* struct game_engine_stage global_stage */
HALO_COMMON(global_tag_instances, 4); /* struct cache_file_tag_instance *global_tag_instances */
HALO_COMMON(global_window_parameters, 600); /* struct rasterizer_window_begin_parameters global_window_parameters */
HALO_COMMON(glow_globals, 8); /* struct glow_globals glow_globals */
HALO_COMMON(hs_debug_data, 32); /* unsigned long hs_debug_data[] */
HALO_COMMON(hs_global_data, 4); /* struct data_array *hs_global_data */
HALO_COMMON(hs_thread_data, 4); /* struct data_array *hs_thread_data */
HALO_COMMON(hud_msg_def, 4); /* struct hud_messaging_parameters_definition *hud_msg_def */
HALO_COMMON(interrupt_result, 24); /* HRESULT interrupt_result */
HALO_COMMON(light_cluster_partition, 12); /* struct cluster_partition light_cluster_partition */
HALO_COMMON(light_data, 4); /* struct data_array *light_data */
HALO_COMMON(local_player_index_for_draw_string_and_hack_in_icons, 4); /* short local_player_index_for_draw_string_and_hack_in_icons */
HALO_COMMON(looping_sound_data, 4); /* struct data_array *looping_sound_data */
HALO_COMMON(loud_dialog_hack, 29); /* byte loud_dialog_hack[] */
HALO_COMMON(node_count, 4); /* long node_count */
HALO_COMMON(particle_data, 4); /* struct data_array *particle_data */
HALO_COMMON(particle_systems, 4); /* struct data_array *particle_systems */
HALO_COMMON(pixel_shader, 256); /* struct pixel_shader_definition pixel_shader */
HALO_COMMON(profile_display, 1); /* byte profile_display[] */
HALO_COMMON(profile_graph, 1); /* byte profile_graph[] */
HALO_COMMON(prop_data, 4); /* struct data_array *prop_data */
HALO_COMMON(rasterizer_frame_statistics, 368); /* struct rasterizer_frame_statistics_globals rasterizer_frame_statistics */
HALO_COMMON(rasterizer_lights, 7200); /* struct rasterizer_lights_globals_prefix rasterizer_lights */
HALO_COMMON(rasterizer_model_cortana_hack, 16); /* boolean rasterizer_model_cortana_hack */
HALO_COMMON(regular_shader, 240); /* struct pixel_shader_definition regular_shader */
HALO_COMMON(render_model_index_counts, 1); /* byte render_model_index_counts[] */
HALO_COMMON(render_model_markers, 1); /* byte render_model_markers[] */
HALO_COMMON(render_model_no_geometry, 1); /* byte render_model_no_geometry[] */
HALO_COMMON(render_model_nodes, 1); /* byte render_model_nodes[] */
HALO_COMMON(render_model_vertex_counts, 1); /* byte render_model_vertex_counts[] */
HALO_COMMON(renderstate_table, 576); /* unsigned long renderstate_table[D3DRS_MAX] */
HALO_COMMON(scenario_globals, 4); /* struct scenario_globals *scenario_globals */
HALO_COMMON(sense_rays, 256); /* struct vector_avoidance_ray sense_rays[9] */
HALO_COMMON(sound_channels, 6144); /* struct sound_channel_datum sound_channels[MAXIMUM_SOUND_CHANNELS] */
HALO_COMMON(sound_class_data, 20); /* struct sound_class_runtime *sound_class_data */
HALO_COMMON(sound_data, 4); /* struct data_array *sound_data */
HALO_COMMON(system_particles, 4); /* struct data_array *system_particles */
HALO_COMMON(temporary_hud, 4); /* byte temporary_hud[] */
HALO_COMMON(texture_table, 32); /* D3DBaseTexture *texture_table[D3DTSS_MAXSTAGES] */
HALO_COMMON(texturestagestate_table, 512); /* unsigned long texturestagestate_table[D3DTSS_MAXSTAGES][D3DTSS_MAX] */
HALO_COMMON(timeout_for_endgame_sound, 28); /* long timeout_for_endgame_sound */
HALO_COMMON(ui_plasma_effect_color, 16); /* real_argb_color ui_plasma_effect_color */
HALO_COMMON(weather_particle_data, 4); /* struct data_array *weather_particle_data */
HALO_COMMON(widget_data, 4); /* struct data_array *widget_data */
HALO_COMMON(wind_globals, 3340); /* struct wind_globals wind_globals */
HALO_COMMON(window_globals, 160); /* struct window_globals_prefix window_globals */

/* ---------- functions */

/* Round-to-integer as the x87 does in the current rounding mode, like the
inline fast_ftol in cseries.h (fld/fistp). */
__attribute__((weak)) long fast_ftol_C(float value)
{
#ifdef HALO_N64
	/* (the VR4300's conversion in the current mode: lrintf's result without
	the library call; cseries.h n64_round_to_long) */
	union { float f; long l; } word;

	__asm__("cvt.w.s %0, %1" : "=f"(word.f) : "f"(value));
	return word.l;
#else
	return lrintf(value);
#endif
}

/* The debug console's "crash" command, which deliberately brings the game
down to exercise crash handling. */
__attribute__((weak)) void main_crash(const char *reason)
{
	platform_log("main_crash: %s", reason ? reason : "");
	abort();
}
