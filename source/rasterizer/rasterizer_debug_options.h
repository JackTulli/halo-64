/*
RASTERIZER_DEBUG_OPTIONS.H

January rasterizer debug settings. Named members are corroborated by the
scripting globals and rasterizer use sites. Unknown bytes remain reserved;
later builds have a different tail and are not the layout authority.
*/

#ifndef __RASTERIZER_DEBUG_OPTIONS_H
#define __RASTERIZER_DEBUG_OPTIONS_H
#pragma once

#include "cseries.h"

struct rasterizer_debug_options_definition
{
	boolean fps_accumulation; /* 0x00 */
	byte pad01; /* 0x01 */
	short stats; /* 0x02 */
	short mode; /* 0x04 */
	boolean wireframe; /* 0x06 */
	boolean debug_model_vertices; /* 0x07 */
	short debug_model_lod; /* 0x08 */
	boolean debug_transparents; /* 0x0A */
	boolean debug_meter_shader; /* 0x0B */
	boolean models; /* 0x0C */
	boolean model_transparents; /* 0x0D */
	boolean draw_first_person_weapon_first; /* 0x0E */
	boolean stencil_mask; /* 0x0F */
	byte environment; /* 0x10 */
	boolean environment_lightmaps; /* 0x11 */
	boolean environment_shadows; /* 0x12 */
	boolean environment_diffuse_lights; /* 0x13 */
	boolean environment_diffuse_textures; /* 0x14 */
	boolean environment_decals; /* 0x15 */
	boolean environment_specular_lights; /* 0x16 */
	boolean environment_specular_lightmaps; /* 0x17 */
	boolean environment_reflection_lightmap_mask; /* 0x18 */
	boolean environment_reflection_mirrors; /* 0x19 */
	boolean environment_reflections; /* 0x1A */
	boolean environment_transparents; /* 0x1B */
	boolean environment_fog; /* 0x1C */
	boolean environment_fog_screen; /* 0x1D */
	boolean water; /* 0x1E */
	boolean lens_flares; /* 0x1F */
	boolean dynamic_unlit_geometry; /* 0x20 */
	boolean dynamic_lit_geometry; /* 0x21 */
	boolean dynamic_screen_geometry; /* 0x22 */
	boolean hud_motion_sensor; /* 0x23 */
	boolean detail_objects; /* 0x24 */
	boolean debug_geometry; /* 0x25 */
	boolean debug_geometry_multipass; /* 0x26 */
	boolean fog_atmosphere; /* 0x27 */
	boolean fog_plane; /* 0x28 */
	boolean bump_mapping; /* 0x29 */
	byte pad2A[2]; /* 0x2A */
	real lightmap_ambient; /* 0x2C */
	byte reserved30[2]; /* 0x30 */
	short pad3; /* 0x32 */
	boolean lightmaps_incident_radiosity; /* 0x34 */
	boolean lightmaps_filtering; /* 0x35 */
	byte pad36[2]; /* 0x36 */
	real model_lighting_ambient; /* 0x38 */
	boolean environment_alpha_testing; /* 0x3C */
	boolean environment_specular_mask; /* 0x3D */
	boolean shadows_convolution; /* 0x3E */
	boolean shadows_debug; /* 0x3F */
	boolean water_mipmapping; /* 0x40 */
	boolean active_camouflage; /* 0x41 */
	boolean active_camouflage_multipass; /* 0x42 */
	boolean plasma_energy; /* 0x43 */
	boolean lens_flares_occlusion; /* 0x44 */
	boolean lens_flares_occlusion_debug; /* 0x45 */
	boolean ray_of_buddha; /* 0x46 */
	boolean screen_flashes; /* 0x47 */
	boolean screen_effects; /* 0x48 */
	boolean DXTC_noise; /* 0x49 */
	boolean soft_filter; /* 0x4A */
	boolean secondary_render_target_debug; /* 0x4B */
	boolean profile_log; /* 0x4C */
	byte pad4D[3]; /* 0x4D */
	real detail_objects_offset_multiplier; /* 0x50 */
	long zbias; /* 0x54 */
	real zoffset; /* 0x58 */
	boolean force_all_player_views_to_default_player; /* 0x5C */
	boolean safe_frame_bounds; /* 0x5D */
	short freeze_flying_camera; /* 0x5E */
	boolean zsprites; /* 0x60 */
	boolean filthy_decal_fog_hack; /* 0x61 */
	boolean smart; /* 0x62 */
	boolean splitscreen_VB_optimization; /* 0x63 */
	boolean profile_print_locks; /* 0x64 */
	byte pad65[3]; /* 0x65 */
	real profile_objectlock_time; /* 0x68 */
	real pad3_scale; /* 0x6C */
	real f[6]; /* 0x70 */
	boolean transparent_pixel_counter_active; /* 0x88 */
	boolean transparent_pixel_counter; /* 0x89 */
	byte pad8A[2]; /* 0x8A */
};

extern struct rasterizer_debug_options_definition rasterizer_debug_options;
extern boolean debug_render_freeze;

#endif /* __RASTERIZER_DEBUG_OPTIONS_H */
