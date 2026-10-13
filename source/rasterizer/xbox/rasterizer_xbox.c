/*
RASTERIZER_XBOX.C

symbols in this file:
001448F0 0010:
	_Direct3D_Release@0 (0000)
00144900 0010:
	_IDirect3D8_Release@4 (0000)
00144910 0020:
	_IDirect3D8_CreateDevice@28 (0000)
00144930 0010:
	_D3DDevice_BeginScene@0 (0000)
00144940 0010:
	_D3DDevice_EndScene@0 (0000)
00144950 0010:
	_D3DDevice_GetRenderState (0000)
00144960 0020:
	_D3DDevice_GetTextureStageState (0000)
00144980 01b0:
	_D3DDevice_SetRenderState (0000)
00144B30 0050:
	_D3DDevice_SetTextureStageState (0000)
00144B80 0010:
	_IDirect3DDevice8_Release@4 (0000)
00144B90 0010:
	_IDirect3DDevice8_GetDeviceCaps@8 (0000)
00144BA0 0020:
	_IDirect3DDevice8_Present@20 (0000)
00144BC0 0010:
	_IDirect3DDevice8_GetBackBuffer@16 (0000)
00144BD0 0020:
	_code_00144bd0 (0000)
00144BF0 0030:
	_code_00144bf0 (0000)
00144C20 0020:
	_code_00144c20 (0000)
00144C40 0010:
	_code_00144c40 (0000)
00144C50 0010:
	_IDirect3DDevice8_SetRenderTarget@12 (0000)
00144C60 0120:
	_rasterizer_preinitialize__fill_you_up_with_the_devils_cock (0000)
00144D80 0010:
	_code_00144d80 (0000)
00144D90 0010:
	_IDirect3DDevice8_BeginScene@4 (0000)
00144DA0 0010:
	_IDirect3DDevice8_EndScene@4 (0000)
00144DB0 0020:
	_IDirect3DDevice8_Clear@28 (0000)
00144DD0 0010:
	_IDirect3DDevice8_SetViewport@8 (0000)
00144DE0 00a0:
	_rasterizer_get_bitmap_default_hardware_format (0000)
00144E80 0220:
	_IDirect3DDevice8_SetRenderState@12 (0000)
001450A0 0010:
	_IDirect3DDevice8_GetRenderState@12 (0000)
001450B0 0010:
	_code_001450b0 (0000)
001450C0 0010:
	_code_001450c0 (0000)
001450D0 0020:
	_IDirect3DDevice8_GetTextureStageState@16 (0000)
001450F0 0060:
	_IDirect3DDevice8_SetTextureStageState@16 (0000)
00145150 0010:
	_code_00145150 (0000)
00145160 0010:
	_code_00145160 (0000)
00145170 0010:
	_code_00145170 (0000)
00145180 0010:
	_code_00145180 (0000)
00145190 0010:
	_code_00145190 (0000)
001451A0 0010:
	_IDirect3DDevice8_SetPixelShaderProgram@8 (0000)
001451B0 0010:
	_IDirect3DDevice8_SetVerticalBlankCallback@8 (0000)
001451C0 0010:
	_IDirect3DDevice8_SetVertexData2s@16 (0000)
001451D0 0010:
	_IDirect3DDevice8_Begin@8 (0000)
001451E0 0010:
	_IDirect3DDevice8_End@4 (0000)
001451F0 0010:
	__rasterizer_reset_state (0000)
00145200 0010:
	_rasterizer_spin_begin (0000)
00145210 0010:
	_rasterizer_spin_end (0000)
00145220 0030:
	__rasterizer_windows_begin (0000)
00145250 0040:
	__rasterizer_window_get_fog (0000)
00145290 0010:
	_D3DTexture_UnlockRect@8 (0000)
001452A0 0030:
	__rasterizer_windows_end (0000)
001452D0 00d0:
	__rasterizer_frame_end (0000)
001453A0 0010:
	_code_001453a0 (0000)
001453B0 0020:
	_code_001453b0 (0000)
001453D0 0010:
	_IDirect3DTexture8_UnlockRect@8 (0000)
001453E0 0010:
	_D3DVolumeTexture_UnlockBox@8 (0000)
001453F0 0070:
	__rasterizer_dispose (0000)
00145460 0010:
	__rasterizer_set_vblank_callback (0000)
00145470 0020:
	_code_00145470 (0000)
00145490 0010:
	_IDirect3DVolumeTexture8_UnlockBox@8 (0000)
001454A0 00a0:
	_rasterizer_set_texture_bitmap_data (0000)
00145540 00b0:
	_rasterizer_set_texture_direct (0000)
001455F0 0010:
	_D3DCubeTexture_UnlockRect@12 (0000)
00145600 00d0:
	_rasterizer_set_texture_direct_non_blocking (0000)
001456D0 01f0:
	_rasterizer_set_texture (0000)
001458C0 0020:
	_code_001458c0 (0000)
001458E0 0010:
	_IDirect3DCubeTexture8_UnlockRect@12 (0000)
001458F0 01b0:
	_rasterizer_set_texture_non_blocking (0000)
00145AA0 01c0:
	_rasterizer_get_target (0000)
00145C60 0010:
	_D3DPalette_Unlock@4 (0000)
00145C70 0010:
	_code_00145c70 (0000)
00145C80 0010:
	_IDirect3DPalette8_Unlock@4 (0000)
00145C90 00d0:
	_rasterizer_set_vertex_shader (0000)
00145D60 0200:
	_rasterizer_set_pixel_shader (0000)
00145F60 0140:
	_rasterizer_set_model_skinning (0000)
001460A0 0010:
	_D3DSurface_UnlockRect@4 (0000)
001460B0 0010:
	_IDirect3DSurface8_Release@4 (0000)
001460C0 0180:
	_rasterizer_set_model_lighting_point_light (0000)
00146240 0010:
	_IDirect3DSurface8_GetDesc@8 (0000)
00146250 0020:
	_IDirect3DSurface8_LockRect@16 (0000)
00146270 0010:
	_IDirect3DSurface8_UnlockRect@4 (0000)
00146280 0090:
	_rasterizer_set_model_lighting_distant_light (0000)
00146310 0170:
	_rasterizer_set_model_lighting (0000)
00146480 0140:
	_rasterizer_set_frustum_z (0000)
001465C0 0010:
	_rasterizer_get_stencil_mode (0000)
001465D0 0080:
	_SetupSmartStates (0000)
00146650 0210:
	_rasterizer_filthy_bitmap_defaults_initialize (0000)
00146860 0930:
	__rasterizer_initialize (0000)
00147190 0090:
	__rasterizer_frame_begin (0000)
00147220 0470:
	__rasterizer_window_set_fog (0000)
00147690 0270:
	__rasterizer_present (0000)
00147900 0090:
	_rasterizer_set_framebuffer_blend_function (0000)
00147990 03b0:
	_rasterizer_set_target (0000)
00147D40 0310:
	_rasterizer_set_target_as_texture (0000)
00148050 02e0:
	_rasterizer_secondary_render_target_debug (0000)
00148330 0310:
	_rasterizer_set_stencil_mode (0000)
00148640 01a0:
	__rasterizer_window_begin (0000)
001487E0 00e0:
	__rasterizer_window_end (0000)
0028BFAC 006c:
	?srcblend_table@?1??rasterizer_set_framebuffer_blend_function@@9@9 (0000)
	?destblend_table@?1??rasterizer_set_framebuffer_blend_function@@9@9 (0024)
	?blendop_table@?1??rasterizer_set_framebuffer_blend_function@@9@9 (0048)
0028C018 0026:
	??_C@_0CG@LJAEJIHF@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5D3D?5d@ (0000)
0028C040 0096:
	??_C@_0JG@IMPKLFMH@IDirect3D8_CreateDevice?$CId3d?0?5D3D@ (0000)
0028C0D8 002a:
	??_C@_0CK@BAHPPAHK@?$CD?$CD?$CD?5ERROR?5rasterizer_preinitiali@ (0000)
0028C104 0026:
	??_C@_0CG@BKBJBKBM@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5D3D?5o@ (0000)
0028C12C 0010:
	??_C@_0BA@JHICOLCK@hardware_format?$AA@ (0000)
0028C13C 0031:
	??_C@_0DB@JJEKAJPN@c?3?2halo?2SOURCE?2rasterizer?2xbox?2r@ (0000)
0028C170 0012:
	??_C@_0BC@EFLMEHLA@global_d3d_device?$AA@ (0000)
0028C184 0004:
	??_C@_03KJMBPJEB@fog?$AA@ (0000)
0028C188 0026:
	??_C@_0CG@NPEGNFPM@?$CD?$CD?$CD?5ERROR?5rasterizer_frame_end?5f@ (0000)
0028C1B0 0038:
	??_C@_0DI@NKBECBMF@IDirect3DDevice8_SetIndices?$CIglob@ (0000)
0028C1E8 0044:
	??_C@_0EE@INAFMGG@IDirect3DDevice8_SetStreamSource@ (0000)
0028C22C 003c:
	??_C@_0DM@KKBOLCEG@IDirect3DDevice8_SetTexture?$CIglob@ (0000)
0028C268 002e:
	??_C@_0CO@PIHPEPNJ@?$CD?$CD?$CD?5ERROR?5direct?5texture?5not?5fou@ (0000)
0028C298 0039:
	??_C@_0DJ@JNBDDJC@?$CD?$CD?$CD?5YOU?5GOT?5FUCKED?5in?5rasterizer@ (0000)
0028C2D4 0034:
	??_C@_0DE@DLDFMINL@stage?$DO?$DN0?5?$CG?$CG?5stage?$DMRASTERIZER_MAX@ (0000)
0028C308 0034:
	??_C@_0DE@GGIKLJDM@?$CD?$CD?$CD?5YOU?5GOT?5FUCKED?5in?5rasterizer@ (0000)
0028C340 0041:
	??_C@_0EB@GOLEKMDL@?$CD?$CD?$CD?5YOU?5GOT?5FUCKED?5in?5rasterizer@ (0000)
0028C388 0042:
	??_C@_0EC@FMMEOKHO@?$CD?$CD?$CD?5ERROR?5default?5texture?5not?5fo@ (0000)
0028C3CC 002d:
	??_C@_0CN@IOHLPNGJ@?$CD?$CD?$CD?5YOU?5GOT?5FUCKED?5in?5rasterizer@ (0000)
0028C400 0040:
	??_C@_0EA@HGCGAPCF@?$CD?$CD?$CD?5ERROR?5incompatible?5bitmap?5ty@ (0000)
0028C440 002a:
	??_C@_0CK@MLIKKFDL@usage?$DO?$DN0?5?$CG?$CG?5usage?$DMNUMBER_OF_BITM@ (0000)
0028C46C 003a:
	??_C@_0DK@CFAMFBPP@?$CD?$CD?$CD?5YOU?5GOT?5FUCKED?5in?5rasterizer@ (0000)
0028C4A8 0028:
	??_C@_0CI@BKEJFOIK@?$CD?$CD?$CD?5ERROR?5unsupported?5rasterizer@ (0000)
0028C4D0 004a:
	??_C@_0EK@HBEOONOJ@mipmap_index?$DO?$DN0?5?$CG?$CG?5mipmap_index?$DM@ (0000)
0028C51C 0010:
	??_C@_0BA@JLAODHOH@mipmap_index?$DN?$DN0?$AA@ (0000)
0028C52C 002e:
	??_C@_0CO@GMABMPEH@?$CD?$CD?$CD?5ERROR?5rasterizer_set_vertex_@ (0000)
0028C55C 0028:
	??_C@_0CI@HGAJFNDH@?$CD?$CD?$CD?5ERROR?5vertex?5shader?5not?5vali@ (0000)
0028C584 002d:
	??_C@_0CN@PDDKNFOD@vertex_shader_index?$DMNUMBER_OF_VE@ (0000)
0028C5B4 0017:
	??_C@_0BH@OHPPDPJE@vertex_shader_index?$DO?$DN0?$AA@ (0000)
0028C5CC 000d:
	??_C@_0N@MANOBHKP@pixel_shader?$AA@ (0000)
0028C5E0 0060:
	??_C@_0GA@PLGEEOOC@skinning?9?$DOnode_matrix_count?$DO0?5?$CG?$CG@ (0000)
0028C640 0018:
	??_C@_0BI@KLHPEHGN@skinning?9?$DOnode_matrices?$AA@ (0000)
0028C658 0009:
	??_C@_08HJFAKFFP@skinning?$AA@ (0000)
0028C664 0013:
	??_C@_0BD@CDNJLMOL@light?9?$DOradius?$DO0?40f?$AA@ (0000)
0028C678 002e:
	??_C@_0CO@JMCGDDGD@?$CD?$CD?$CD?5ERROR?5invalid?5light?5index?5?$CD?$CF@ (0000)
0028C6A8 0013:
	??_C@_0BD@FMHEBCEM@lighting_constants?$AA@ (0000)
0028C6C0 0063:
	??_C@_0GD@MJAJPLGF@lighting?9?$DOdistant_light_count?$DO?$DN0@ (0000)
0028C728 005d:
	??_C@_0FN@EEGDNJMA@lighting?9?$DOpoint_light_count?$DO?$DN0?5?$CG@ (0000)
0028C788 003d:
	??_C@_0DN@INGBFNFC@?$CD?$CD?$CD?5ERROR?5rasterizer_filthy_bitm@ (0000)
0028C7C8 0064:
	??_C@_0GE@KICHIBFL@IDirect3DCubeTexture8_UnlockRect@ (0000)
0028C830 007d:
	??_C@_0HN@CBGCAOMO@IDirect3DCubeTexture8_LockRect?$CI?$CI@ (0000)
0028C8B0 0096:
	??_C@_0JG@KLNJKKPK@IDirect3DDevice8_CreateCubeTextu@ (0000)
0028C948 00a0:
	??_C@_0KA@OJCDNDGA@IDirect3DDevice8_CreateVolumeTex@ (0000)
0028C9E8 0091:
	??_C@_0JB@COIPALPH@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028CA7C 002a:
	??_C@_0CK@OCJFMEEI@?$CD?$CD?$CD?5ERROR?5failed?5to?5initialize?5r@ (0000)
0028CAA8 0030:
	??_C@_0DA@NKLNBECC@?$CD?$CD?$CD?5ERROR?5failed?5to?5create?5offsc@ (0000)
0028CAD8 0074:
	??_C@_0HE@PLFBFJHA@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028CB50 00d8:
	??_C@_0NI@ECMBGJIH@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028CC28 0070:
	??_C@_0HA@HPJNNEKC@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028CC98 00d6:
	??_C@_0NG@BODHHAEB@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028CD70 0070:
	??_C@_0HA@KNPKGEEK@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028CDE0 00e4:
	??_C@_0OE@NPLOEEFK@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028CEC8 006c:
	??_C@_0GM@HCLFIPDH@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028CF38 00de:
	??_C@_0NO@IJKJNOPC@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028D018 0073:
	??_C@_0HD@KCKBKMOK@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028D090 00ed:
	??_C@_0ON@JBHMAKFG@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028D180 0070:
	??_C@_0HA@PPPDGAKM@IDirect3DTexture8_GetSurfaceLeve@ (0000)
0028D1F0 00ed:
	??_C@_0ON@OCIPEHEH@IDirect3DDevice8_CreateTexture?$CIg@ (0000)
0028D2E0 0061:
	??_C@_0GB@BKAODOLK@IDirect3DDevice8_GetDepthStencil@ (0000)
0028D344 003f:
	??_C@_0DP@GDNFIFCJ@IDirect3DDevice8_SetPalette?$CIglob@ (0000)
0028D384 003f:
	??_C@_0DP@MNLNBELI@IDirect3DDevice8_SetPalette?$CIglob@ (0000)
0028D3C4 003f:
	??_C@_0DP@OEHFKAEK@IDirect3DDevice8_SetPalette?$CIglob@ (0000)
0028D404 003f:
	??_C@_0DP@EKBNDBNL@IDirect3DDevice8_SetPalette?$CIglob@ (0000)
0028D444 0026:
	??_C@_0CG@PPAJCKHN@IDirect3DPalette8_Unlock?$CId3d_pal@ (0000)
0028D46C 0036:
	??_C@_0DG@BMBIAMGI@IDirect3DPalette8_Lock?$CId3d_palet@ (0000)
0028D4A8 0050:
	??_C@_0FA@MCEBCHKA@IDirect3DDevice8_CreatePalette?$CIg@ (0000)
0028D4F8 0040:
	??_C@_0EA@CKHOOLOE@?$CD?$CD?$CD?5ERROR?5unsupported?5refresh?5ra@ (0000)
0028D538 0037:
	??_C@_0DH@HCIDBGAJ@global_window_parameters?4fog?4pla@ (0000)
0028D570 003a:
	??_C@_0DK@EDEGCCAG@global_window_parameters?4fog?4pla@ (0000)
0028D5B0 0075:
	??_C@_0HF@ELMEMOH@global_window_parameters?4fog?4atm@ (0000)
0028D628 003b:
	??_C@_0DL@PAKIILIC@global_window_parameters?4fog?4pla@ (0000)
0028D664 0024:
	??_C@_0CE@HPCABBCB@?$CD?$CD?$CD?5ERROR?5rasterizer_present?5fai@ (0000)
0028D688 0060:
	??_C@_0GA@CPCOJECG@IDirect3DDevice8_Present?$CIglobal_@ (0000)
0028D6E8 002d:
	??_C@_0CN@JAPEKLEM@?$CD?$CD?$CD?5ERROR?5rasterizer_present?3?5in@ (0000)
0028D718 003f:
	??_C@_0DP@EGFJOLCI@?$CD?$CD?$CD?5ERROR?5rasterizer_present?3?5fa@ (0000)
0028D758 0040:
	??_C@_0EA@KEPBNAO@?$CD?$CD?$CD?5ERROR?5rasterizer_present?3?5fa@ (0000)
0028D798 006a:
	??_C@_0GK@ICFOLFDN@d3d_locked_rect?4Pitch?$DN?$DNbitmap_fo@ (0000)
0028D808 0069:
	??_C@_0GJ@JMGDDDIM@framebuffer_blend_function?$DO?$DN0?5?$CG?$CG@ (0000)
0028D874 0018:
	??_C@_0BI@DJIILGGH@?$CBzbuffer?$HM?$HMd3d_surface_z?$AA@ (0000)
0028D88C 000c:
	??_C@_0M@HGIHFEHC@d3d_surface?$AA@ (0000)
0028D898 0032:
	??_C@_0DC@MKHBJDNL@?$CD?$CD?$CD?5ERROR?5rasterizer_set_target_@ (0000)
0028D8D0 005b:
	??_C@_0FL@EKIAGOAJ@IDirect3DDevice8_SetTexture?$CIglob@ (0000)
0028D930 0047:
	??_C@_0EH@LPKMMGNL@max_mipmap?$DO?$DN0?5?$CG?$CG?5max_mipmap?$DM?$DNRAS@ (0000)
0028D978 000c:
	??_C@_0M@GCLICPKO@d3d_texture?$AA@ (0000)
0028D984 002a:
	??_C@_0CK@FOODOHFI@IDirect3DSurface8_Release?$CId3d_ba@ (0000)
0028D9B0 000e:
	??_C@_0O@FGCAPMIP@max_mipmap?$DN?$DN0?$AA@ (0000)
0028D9C0 0023:
	??_C@_0CD@FEAAOJEM@?$CD?$CD?$CD?5ERROR?5unsupported?5stencil?5mo@ (0000)
0028D9E4 0020:
	??_C@_0CA@FGDHOGNI@parameters?9?$DOcamera?4z_near?$CB?$DN0?40f?$AA@ (0000)
0028DA04 003c:
	??_C@_0DM@KDAIMIHM@?$CD?$CD?$CD?5ERROR?5unsupported?5rasterizer@ (0000)
0028DA40 000b:
	??_C@_0L@HHPOFMCB@parameters?$AA@ (0000)
0030CEEC 0006:
	_rasterizer_state_cache (0000)
0045E028 08b6:
	_rasterizer_xbox_d3d_globals (0000)
	_global_d3d_device (08a8)
*/

/* ---------- headers */

#include "cseries.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmap_group_internal.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmap_strings_internal.h"
#include "cache/texture_cache.h"
#include "cseries/profile.h"
#include "effects/decals.h"
#include "game/game_globals.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/hud_draw.h"
#include "main/main_internal.h"
#include "rasterizer/common/rasterizer_common.h"
#include "cseries/errors.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/rasterizer_cinematics.h"
#include "rasterizer/rasterizer_debug.h"
#include "rasterizer/rasterizer_debug_options.h"
#include "rasterizer/rasterizer_frame_statistics.h"
#include "rasterizer/rasterizer_lights.h"
#include "rasterizer/rasterizer_text.h"
#include "rasterizer/rasterizer_transparent_geometry.h"
#include "rasterizer/xbox/rasterizer_xbox_draw_primitives.h"
#include "render/render.h"
#include "render/render_cameras.h"

/* The January object retains out-of-line copies of the D3D inline wrappers.
 * The stock XDK definition of D3DINLINE (static __forceinline) reproduces
 * them; do not replace them with handwritten Microsoft dispatchers or
 * override the XDK's inline policy: taking an address or weakening
 * __forceinline changes their emitted ABI and code shape. */
#include <xtl.h>

#include "rasterizer_xbox.h"
#include "rasterizer/xbox/rasterizer_xbox_pixel_shader.h"
#include "rasterizer_xbox_internal.h"
#include "rasterizer_xbox_vertex_shaders.h"

/* ---------- constants */

enum
{
	_shader_framebuffer_blend_function_alpha_blend = 0,
	_shader_framebuffer_blend_function_multiply,
	_shader_framebuffer_blend_function_double_multiply,
	_shader_framebuffer_blend_function_add,
	_shader_framebuffer_blend_function_subtract,
	_shader_framebuffer_blend_function_component_min,
	_shader_framebuffer_blend_function_component_max,
	_shader_framebuffer_blend_function_alpha_multiply_add,
	NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS
};

enum
{
	RASTERIZER_MAXIMUM_TEXTURE_STAGES = 4,
};

enum
{
	_rasterizer_target_render_primary = 0,
	_rasterizer_target_render_secondary,
	_rasterizer_target_shadow_primary,
	_rasterizer_target_shadow_secondary,
	_rasterizer_target_sun_glow_primary,
	_rasterizer_target_sun_glow_secondary,
	_rasterizer_target_water_bumpmap,
	/* the eighth target has no first-party name in this object; it is the
	 * second render-primary surface/texture pair */
	_rasterizer_target_render_primary_copy,

	NUMBER_OF_RASTERIZER_TARGETS
};

/* mode 1 makes the window clear to black instead of to the atmospheric fog
 * colour; the remaining values are not exercised by this object. */
enum
{
	_rasterizer_mode_default = 0,
	_rasterizer_mode_clear_to_black
};

enum
{
	RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS = 4
};

/* both names are first-party: they appear verbatim in this object's
 * IDirect3DDevice8_CreateTexture(... &global_d3d_texture_render_secondary)
 * error string. */
enum
{
	RASTERIZER_TARGET_RENDER_SECONDARY_WIDTH = 320,
	RASTERIZER_TARGET_RENDER_SECONDARY_HEIGHT = 240
};

/* the four bitmaps of a default-texture group, one per shader usage; only the
 * bump-map slot is named by this object's behaviour (it is skipped when bump
 * mapping is disabled). */
enum
{
	_bitmap_usage_bump_map = 3,

	NUMBER_OF_BITMAP_USAGES = 4
};

enum
{
	NUMBER_OF_BITMAP_TYPES = 3
};

/* render_fog.planar_mode and fog_definition.flags; both enumerations belong in
 * render/render_cameras.h, which this worker may not edit. */
enum
{
	_render_planar_fog_mode_off = 0,
	_render_planar_fog_mode_normal,
	_render_planar_fog_mode_fully_fogged,

	NUMBER_OF_RENDER_PLANAR_FOG_MODES
};

enum
{
	_fog_definition_is_water_bit = 0,
	_fog_definition_atmosphere_dominant_bit,
	_fog_definition_screen_effect_only_bit,

	NUMBER_OF_FOG_DEFINITION_FLAGS
};

/* the only two bitmap formats a screenshot capture accepts */
enum
{
	_bitmap_format_x8r8g8b8 = 10,
	_bitmap_format_a8r8g8b8 = 11
};

/* combiner_count register layout: the active combiner count in the low
 * nibble, "C0 unique per stage" at bit 12 and "C1 unique per stage" at bit 16.
 * Six or more combiners are uploaded as a whole program instead. */
enum
{
	PIXEL_SHADER_COMBINER_COUNT_MASK = 0xf,
	PIXEL_SHADER_UNIQUE_CONSTANT_0_SHIFT = 12,
	PIXEL_SHADER_UNIQUE_CONSTANT_1_SHIFT = 16,

	MAXIMUM_INLINE_PIXEL_SHADER_COMBINERS = 6
};

/* the model-lighting vertex shader constant block: three registers per point
 * light, two per distant light, one for the ambient colour */
enum
{
	MODEL_LIGHTING_DISTANT_LIGHT_CONSTANT = 6,
	MODEL_LIGHTING_AMBIENT_CONSTANT = 10,

	NUMBER_OF_MODEL_LIGHTING_CONSTANTS = 11
};


/* the device the preinitialization probe creates; the creation-flag name is
 * first-party (it appears verbatim in this object's IDirect3D8_CreateDevice
 * error string), the two dimension names are inferred. */
enum
{
	RASTERIZER_SCREEN_WIDTH = 640,
	RASTERIZER_SCREEN_HEIGHT = 480,

	RASTERIZER_DEVICE_CREATION_FLAGS = D3DCREATE_HARDWARE_VERTEXPROCESSING
};

enum
{
	INVALID_RASTERIZER_HARDWARE_STATE = -1,
};

enum
{
	RASTERIZER_STENCIL_MODE_NONE = 0,
	RASTERIZER_STENCIL_MODE_WRITE,
	RASTERIZER_STENCIL_MODE_REJECT,
	RASTERIZER_STENCIL_MODE_ACCEPT,
	RASTERIZER_STENCIL_MODE_WRITE_ALPHA_TESTED_DECAL,
	RASTERIZER_STENCIL_MODE_REJECT_ALPHA_TESTED_DECAL,
	NUMBER_OF_RASTERIZER_STENCIL_MODES
};


/* the Direct3D push-buffer sizes January defaults to when the corresponding
 * rasterizer_globals fields are still zero */
enum
{
	RASTERIZER_DEFAULT_PUSH_BUFFER_SIZE = 512,
	RASTERIZER_DEFAULT_KICK_OFF_SIZE = 32,
	RASTERIZER_PUSH_BUFFER_SIZE_UNIT = 1024
};

/* the 7.5% title-safe inset of the 640x480 screen */
enum
{
	RASTERIZER_FRAME_BOUNDS_X0 = 48,
	RASTERIZER_FRAME_BOUNDS_Y0 = 36,
	RASTERIZER_FRAME_BOUNDS_X1 = 592,
	RASTERIZER_FRAME_BOUNDS_Y1 = 444
};

/* the render-target dimensions; the four names are first-party (they appear
 * verbatim in this object's IDirect3DDevice8_CreateTexture error strings), the
 * values are read off January's own argument pushes. */
enum
{
	RASTERIZER_TARGET_WATER_SIZE = 128,
	RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE = 128,
	RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE = 128,
	RASTERIZER_TARGET_SUN_GLOW_SIZE = 64
};

enum
{
	RASTERIZER_FLICKER_FILTER_LEVEL = 5
};

/* the hand-built descriptor over the current back buffer: D3DCOMMON_TYPE_TEXTURE
 * with one reference, the Xbox D3DSIZE of a 640x480 2560-byte-pitch surface, and
 * the swizzled A8R8G8B8 format word. */
enum
{
	RASTERIZER_TARGET_RENDER_PRIMARY_COMMON = 0x00040001,
	RASTERIZER_TARGET_RENDER_PRIMARY_FORMAT = 0x00011229,
	RASTERIZER_TARGET_RENDER_PRIMARY_SIZE = 0x271df27f
};

/* bitmap_group.h does not name the bitmap types; these three spellings come
 * from source/bitmaps/bitmap_utilities.c's own assert strings
 * ("bitmap->type==_bitmap_type_2d" and friends). */
enum
{
	_bitmap_type_2d = 0,
	_bitmap_type_3d,
	_bitmap_type_cube_map
};

/* the default 4x4 A4R4G4B4 checkerboard every unbound texture stage falls back
 * to */
enum
{
	NUMBER_OF_CUBE_MAP_FACES = 6,
	DEFAULT_BITMAP_SIZE = 4,
	DEFAULT_BITMAP_PIXEL0 = 0x0f00,
	DEFAULT_BITMAP_PIXEL1 = 0xf0f0
};

enum
{
	NUMBER_OF_ENTRIES_IN_PALETTE = 256
};

/* ---------- macros */

#define rasterizer_push_buffer_size \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->push_buffer_size)
#define rasterizer_kick_off_size \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->kick_off_size)
#define rasterizer_floating_point_z_buffer \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->floating_point_z_buffer)
#define rasterizer_refresh_rate \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->refresh_rate)
#define rasterizer_default_2d_hardware_format \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->default_2d_hardware_format)
#define rasterizer_default_3d_hardware_format \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->default_3d_hardware_format)
#define rasterizer_default_cm_hardware_format \
	(((struct rasterizer_xbox_rasterizer_globals *)&rasterizer_globals)->default_cm_hardware_format)

/* ---------- structures */

struct rasterizer_hardware_state_cache
{
	short vertex_shader_index;
	short reserved2;
	short stencil_mode;
};

/* the model-lighting vertex shader constant block: 11 four-component
 * registers, three per point light, two per distant light, one ambient */
struct rasterizer_point_light_constants
{
	real_point3d position;
	real inverse_radius_squared;
	real_vector3d forward;
	real falloff_scale;
	real_rgb_color color;
	real falloff_offset;
};

struct rasterizer_distant_light_constants
{
	real_vector3d direction;
	real pad0C;
	real_rgb_color color;
	real pad1C;
};

struct rasterizer_model_lighting_constants
{
	struct rasterizer_point_light_constants
		point_lights[MAXIMUM_RENDERED_POINT_LIGHTS];
	struct rasterizer_distant_light_constants
		distant_lights[MAXIMUM_RENDERED_DISTANT_LIGHTS];
	real_rgb_color ambient_color;
	real pad;
};

struct point_light_geometry_parameters
{
	real radius;
	real radius_modifier_lower_bound;
	real radius_modifier_upper_bound;
	real falloff_angle;
	real cutoff_angle;
	real lens_flare_radius;
	real runtime_cosine_falloff_angle;
	real runtime_cosine_cutoff_angle;
	real specular_radius_multiplier;
	real runtime_sine_cutoff_angle;
	long unused[2];
};

struct point_light_definition
{
	unsigned long flags;
	struct point_light_geometry_parameters geometry;
};


/* the shell's window globals (HCEX `struct window_data`, 0x94 bytes, the size
 * of January's pooled window_globals record); this object only reads
 * hWndPresentTarget (+8), the name its own IDirect3DDevice8_Present() error
 * string spells. */
struct window_data
{
	HINSTANCE hInstance;
	HWND hWnd;
	HWND hWndPresentTarget;
	int nCmdShow;
	void *MainWndProc;
	char class_name[0x40];
	char window_title[0x40];
};

struct rasterizer_model_skinning_parameters
{
	real_matrix4x3 const *node_matrices;
	short node_matrix_count;
	word pad;
};

/* ---------- prototypes */

static void rasterizer_filthy_bitmap_defaults_initialize(
	void);

void SetupSmartStates(
	void);

/* ---------- globals */

/* January keeps this object's private Direct3D state in one contiguous owner.
 * The aggregate name follows the subsystem convention; member names come from
 * the object's own IDirect3D*() error strings and their relocation offsets. */
struct rasterizer_xbox_d3d_globals
{
	real node_matrix_constants
		[RASTERIZER_MAXIMUM_NODES_PER_MODEL][3][4];         /* +0    */
	point2d bitmap_dimensions_non_blocking;             /* +2112 */
	point2d bitmap_dimensions;                          /* +2116 */
	Direct3D *d3d;                                      /* +2120 */
	/* hand-built descriptors wrapping the two back buffers */
	D3DBaseTexture *global_d3d_texture_render_primary[2];/* +2124 */
	D3DSurface *global_d3d_surface_render_primary;      /* +2132 */
	D3DSurface *global_d3d_surface_render_primary_z;    /* +2136 */
	D3DTexture *global_d3d_texture_render_secondary;    /* +2140 */
	/* the render-secondary target has no depth buffer; both slots are
	 * explicitly cleared at initialization.  The texture name is inferred
	 * from the surface name immediately behind it. */
	D3DTexture *global_d3d_texture_render_secondary_z;   /* +2144 */
	D3DSurface *global_d3d_surface_render_secondary;    /* +2148 */
	D3DSurface *global_d3d_surface_render_secondary_z;   /* +2152 */
	D3DTexture *global_d3d_texture_shadow_primary;      /* +2156 */
	D3DSurface *global_d3d_surface_shadow_primary;      /* +2160 */
	D3DTexture *global_d3d_texture_shadow_secondary;    /* +2164 */
	D3DSurface *global_d3d_surface_shadow_secondary;    /* +2168 */
	D3DTexture *global_d3d_texture_sun_glow_primary;    /* +2172 */
	D3DSurface *global_d3d_surface_sun_glow_primary;    /* +2176 */
	D3DTexture *global_d3d_texture_sun_glow_secondary;  /* +2180 */
	D3DSurface *global_d3d_surface_sun_glow_secondary;  /* +2184 */
	D3DTexture *global_d3d_texture_water;               /* +2188 */
	D3DSurface *global_d3d_surface_water
		[RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS];    /* +2192 */
	D3DBaseTexture *global_d3d_texture_render_primary_copy; /* +2208 */
	D3DSurface *global_d3d_surface_render_primary_copy; /* +2212 */
};

struct rasterizer_xbox_d3d_globals rasterizer_xbox_d3d_globals = { 0 };

#define node_matrix_constants rasterizer_xbox_d3d_globals.node_matrix_constants
#define d3d rasterizer_xbox_d3d_globals.d3d
#define bitmap_dimensions_non_blocking rasterizer_xbox_d3d_globals.bitmap_dimensions_non_blocking
#define bitmap_dimensions rasterizer_xbox_d3d_globals.bitmap_dimensions
#define global_d3d_texture_render_primary rasterizer_xbox_d3d_globals.global_d3d_texture_render_primary
#define global_d3d_surface_render_primary rasterizer_xbox_d3d_globals.global_d3d_surface_render_primary
#define global_d3d_surface_render_primary_z rasterizer_xbox_d3d_globals.global_d3d_surface_render_primary_z
#define global_d3d_texture_render_secondary rasterizer_xbox_d3d_globals.global_d3d_texture_render_secondary
#define global_d3d_texture_render_secondary_z rasterizer_xbox_d3d_globals.global_d3d_texture_render_secondary_z
#define global_d3d_surface_render_secondary rasterizer_xbox_d3d_globals.global_d3d_surface_render_secondary
#define global_d3d_texture_shadow_primary rasterizer_xbox_d3d_globals.global_d3d_texture_shadow_primary
#define global_d3d_surface_shadow_primary rasterizer_xbox_d3d_globals.global_d3d_surface_shadow_primary
#define global_d3d_texture_shadow_secondary rasterizer_xbox_d3d_globals.global_d3d_texture_shadow_secondary
#define global_d3d_surface_shadow_secondary rasterizer_xbox_d3d_globals.global_d3d_surface_shadow_secondary
#define global_d3d_texture_sun_glow_primary rasterizer_xbox_d3d_globals.global_d3d_texture_sun_glow_primary
#define global_d3d_surface_sun_glow_primary rasterizer_xbox_d3d_globals.global_d3d_surface_sun_glow_primary
#define global_d3d_texture_sun_glow_secondary rasterizer_xbox_d3d_globals.global_d3d_texture_sun_glow_secondary
#define global_d3d_surface_sun_glow_secondary rasterizer_xbox_d3d_globals.global_d3d_surface_sun_glow_secondary
#define global_d3d_texture_water rasterizer_xbox_d3d_globals.global_d3d_texture_water
#define global_d3d_surface_water rasterizer_xbox_d3d_globals.global_d3d_surface_water
#define global_d3d_surface_render_secondary_z rasterizer_xbox_d3d_globals.global_d3d_surface_render_secondary_z
#define global_d3d_texture_render_primary_copy rasterizer_xbox_d3d_globals.global_d3d_texture_render_primary_copy
#define global_d3d_surface_render_primary_copy rasterizer_xbox_d3d_globals.global_d3d_surface_render_primary_copy

D3DDevice *global_d3d_device = NULL;

/* the shared 256-entry palette; January's own
 * IDirect3DDevice8_CreatePalette(global_d3d_device, D3DPALETTE_256,
 * &d3d_palette) error string names it. */
static D3DPalette *d3d_palette = NULL;

/* owned by another object; named by this object's own
 * D3DDevice_GetDeviceCaps() call site. */
extern D3DCAPS8 global_d3d_caps;

/* These two sit immediately behind global_d3d_device in January's .bss, which
 * is the only symbol csplit knows there, so the split still anchors their
 * relocations on `_global_d3d_device + 8` / `+ 12` (image 0x0045E8D8 and
 * 0x0045E8DC).  symbols.json names for those two addresses close the gap. */
static boolean suppress_window_begin_end = FALSE;
static short previous_window_index = 0;

static struct rasterizer_hardware_state_cache rasterizer_state_cache =
{
	INVALID_RASTERIZER_HARDWARE_STATE,
	0,
	INVALID_RASTERIZER_HARDWARE_STATE,
};


extern struct window_data window_globals;

extern struct rasterizer_window_begin_parameters global_window_parameters;
/* owned by source/bitmaps/bitmaps.c */
extern pixel32 global_vector_palette[NUMBER_OF_ENTRIES_IN_PALETTE];

/* the shadow tables of the SetRenderStateSmart family; that object owns them,
 * this one only primes them from the XDK's own shadows at initialization. */
extern unsigned long renderstate_table[D3DRS_MAX];
extern unsigned long texturestagestate_table[D3DTSS_MAXSTAGES][D3DTSS_MAX];
extern D3DBaseTexture *texture_table[D3DTSS_MAXSTAGES];

/* ---------- public code */

void _rasterizer_reset_state(
	void)
{
	return;
}

void rasterizer_spin_begin(
	short profile)
{
	return;
}

void rasterizer_spin_end(
	void)
{
	return;
}

void _rasterizer_set_vblank_callback(
	void (*callback)(unsigned long))
{
	IDirect3DDevice8_SetVerticalBlankCallback(
		global_d3d_device,
		callback);
	return;
}

void _rasterizer_windows_begin(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1331,
		global_d3d_device);
	IDirect3DDevice8_BeginScene(global_d3d_device);
	return;
}

void _rasterizer_window_get_fog(
	struct render_fog *fog)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1440,
		fog);
	*fog = global_window_parameters.fog;
	return;
}

void _rasterizer_window_set_fog(
	struct render_fog const *fog)
{
	real fog_constants[4][4];
	real inverse_atmospheric_range;
	real inverse_planar_distance;
	real inverse_planar_depth;
	real plane_distance;
	real view_distance;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1452,
		fog);
	global_window_parameters.fog = *fog;
	if (global_window_parameters.fog.atmospheric_maximum_density <= 0.0f)
		global_window_parameters.fog.atmospheric_maximum_density = 1.0f;
	if (global_window_parameters.fog.atmospheric_maximum_distance == 0.0f ||
		!rasterizer_debug_options.fog_atmosphere)
	{
		global_window_parameters.fog.atmospheric_maximum_density = 0.0f;
		global_window_parameters.fog.atmospheric_minimum_distance =
			global_window_parameters.camera.z_far;
		global_window_parameters.fog.atmospheric_maximum_distance =
			global_window_parameters.camera.z_far * 2.0f;
	}
	if (global_window_parameters.fog.planar_maximum_density <= 0.0f)
		global_window_parameters.fog.planar_maximum_density = 1.0f;
	if (global_window_parameters.fog.planar_mode &&
		!TEST_FLAG(fog->fog_definition_flags, _fog_definition_screen_effect_only_bit) &&
		rasterizer_debug_options.fog_plane)
	{
		if (global_window_parameters.fog.planar_mode ==
			_render_planar_fog_mode_fully_fogged)
		{
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
				1498,
				global_window_parameters.fog.planar_maximum_distance!=0.0f);
			global_window_parameters.fog.planar_maximum_depth = 1.0f;
			plane3d_from_point_and_normal(
				&global_window_parameters.fog.plane,
				&global_window_parameters.camera.position,
				&global_window_parameters.camera.forward);
			global_window_parameters.fog.plane.d +=
				global_window_parameters.camera.z_far;
		}
	}
	else
	{
		global_window_parameters.fog.planar_mode = _render_planar_fog_mode_off;
		global_window_parameters.fog.planar_maximum_density = 0.0f;
		global_window_parameters.fog.planar_maximum_distance = 1.0f;
		global_window_parameters.fog.planar_maximum_depth = 1.0f;
		global_window_parameters.fog.planar_color = *global_real_rgb_white;
		plane3d_from_point_and_normal(
			&global_window_parameters.fog.plane,
			&global_window_parameters.camera.position,
			&global_window_parameters.camera.forward);
	}
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1511,
		global_window_parameters.fog.atmospheric_maximum_distance>=global_window_parameters.fog.atmospheric_minimum_distance);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1512,
		global_window_parameters.fog.planar_maximum_distance>0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1513,
		global_window_parameters.fog.planar_maximum_depth>0.0f);
	inverse_atmospheric_range = 1.0f /
		(global_window_parameters.fog.atmospheric_maximum_distance -
			global_window_parameters.fog.atmospheric_minimum_distance);
	plane_distance = plane3d_distance_to_point(
		&global_window_parameters.fog.plane,
		&global_window_parameters.camera.position);
	view_distance = dot_product3d(
		(real_vector3d *)&global_window_parameters.camera.position,
		&global_window_parameters.camera.forward);
	inverse_planar_distance = 1.0f /
		global_window_parameters.fog.planar_maximum_distance;
	inverse_planar_depth = 1.0f /
		global_window_parameters.fog.planar_maximum_depth;
	fog_constants[0][0] = global_window_parameters.camera.forward.i *
		inverse_atmospheric_range;
	fog_constants[0][1] = global_window_parameters.camera.forward.j *
		inverse_atmospheric_range;
	fog_constants[0][2] = global_window_parameters.camera.forward.k *
		inverse_atmospheric_range;
	fog_constants[0][3] = -((global_window_parameters.fog.atmospheric_minimum_distance +
		view_distance) * inverse_atmospheric_range);
	fog_constants[1][0] = -(global_window_parameters.fog.plane.n.i *
		inverse_planar_depth);
	fog_constants[1][1] = -(global_window_parameters.fog.plane.n.j *
		inverse_planar_depth);
	fog_constants[1][2] = -(global_window_parameters.fog.plane.n.k *
		inverse_planar_depth);
	fog_constants[1][3] = global_window_parameters.fog.plane.d *
		inverse_planar_depth;
	fog_constants[2][0] = global_window_parameters.camera.forward.i *
		inverse_planar_distance;
	fog_constants[2][1] = global_window_parameters.camera.forward.j *
		inverse_planar_distance;
	fog_constants[2][2] = global_window_parameters.camera.forward.k *
		inverse_planar_distance;
	fog_constants[2][3] = -(inverse_planar_distance * view_distance);
	fog_constants[3][0] = PIN(global_window_parameters.fog.atmospheric_maximum_density,
		0.0f, 1.0f);
	fog_constants[3][1] = PIN(-(inverse_planar_depth * plane_distance), 0.0f, 1.0f);
	fog_constants[3][2] = PIN(global_window_parameters.fog.planar_maximum_density,
		0.0f, 1.0f);
	fog_constants[3][3] = 0.0f;
	D3DDevice_SetVertexShaderConstant(
		-88,
		fog_constants,
		4);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_FOGCOLOR,
		real_rgb_color_to_pixel32(&global_window_parameters.fog.atmospheric_color));
	return;
}

void _rasterizer_windows_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1629,
		global_d3d_device);
	IDirect3DDevice8_EndScene(global_d3d_device);
	return;
}

void rasterizer_set_pixel_shader(
	struct pixel_shader_definition const *pixel_shader)
{
	short combiner_count;
	short combiner_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2652,
		pixel_shader);
	combiner_count = (short)(pixel_shader->combiner_count &
		PIXEL_SHADER_COMBINER_COUNT_MASK);
	if (combiner_count < MAXIMUM_INLINE_PIXEL_SHADER_COMBINERS)
	{
		boolean unique_constant_0 = (boolean)((pixel_shader->combiner_count >>
			PIXEL_SHADER_UNIQUE_CONSTANT_0_SHIFT) & 1);
		boolean unique_constant_1 = (boolean)((pixel_shader->combiner_count >>
			PIXEL_SHADER_UNIQUE_CONSTANT_1_SHIFT) & 1);

		D3DDevice_SetRenderState(
			D3DRS_PSTEXTUREMODES,
			pixel_shader->texture_modes);
		D3DDevice_SetRenderState(
			D3DRS_PSCOMBINERCOUNT,
			pixel_shader->combiner_count);
		D3DDevice_SetRenderState(
			D3DRS_PSINPUTTEXTURE,
			pixel_shader->input_texture);
		D3DDevice_SetRenderState(
			D3DRS_PSDOTMAPPING,
			pixel_shader->dot_mapping);
		D3DDevice_SetRenderState(
			D3DRS_PSFINALCOMBINERINPUTSABCD,
			pixel_shader->final_combiner_inputs_abcd);
		D3DDevice_SetRenderState(
			D3DRS_PSFINALCOMBINERINPUTSEFG,
			pixel_shader->final_combiner_inputs_efg);
		D3DDevice_SetRenderState(
			D3DRS_PSFINALCOMBINERCONSTANT0,
			pixel_shader->final_combiner_constant_0);
		D3DDevice_SetRenderState(
			D3DRS_PSFINALCOMBINERCONSTANT1,
			pixel_shader->final_combiner_constant_1);
		for (combiner_index = 0; combiner_index < combiner_count; combiner_index++)
		{
			D3DDevice_SetRenderStateNotInline(
				D3DRS_PSALPHAINPUTS0 + combiner_index,
				pixel_shader->alpha_inputs[combiner_index]);
			D3DDevice_SetRenderStateNotInline(
				D3DRS_PSALPHAOUTPUTS0 + combiner_index,
				pixel_shader->alpha_outputs[combiner_index]);
			D3DDevice_SetRenderStateNotInline(
				D3DRS_PSRGBINPUTS0 + combiner_index,
				pixel_shader->rgb_inputs[combiner_index]);
			D3DDevice_SetRenderStateNotInline(
				D3DRS_PSRGBOUTPUTS0 + combiner_index,
				pixel_shader->rgb_outputs[combiner_index]);
			if (unique_constant_0 || combiner_index == 0)
				D3DDevice_SetRenderStateNotInline(
					D3DRS_PSCONSTANT0_0 + combiner_index,
					pixel_shader->constant_0[combiner_index]);
			if (unique_constant_1 || combiner_index == 0)
				D3DDevice_SetRenderStateNotInline(
					D3DRS_PSCONSTANT1_0 + combiner_index,
					pixel_shader->constant_1[combiner_index]);
		}
		if (rasterizer_debug_options.stats == 2)
			rasterizer_frame_statistics.pixel_shader_pushbuffer_bytes +=
				(4 + unique_constant_0 + unique_constant_1) * combiner_count *
					sizeof(unsigned long) + 32;
	}
	else
	{
		IDirect3DDevice8_SetPixelShaderProgram(
			global_d3d_device,
			(D3DPIXELSHADERDEF *)pixel_shader);
		if (rasterizer_debug_options.stats == 2)
			rasterizer_frame_statistics.pixel_shader_pushbuffer_bytes += 228;
	}
	return;
}

void rasterizer_set_model_lighting_point_light(
	long light_index,
	short constant_index,
	struct rasterizer_model_lighting_constants *lighting_constants)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2826,
		lighting_constants);
	if (light_index != NONE)
	{
		struct rasterizer_light_submit_parameters const *light;

		match_vassert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2835,
			light_index>=0 && light_index<rasterizer_lights.light_count,
			csprintf(
				temporary,
				"### ERROR invalid light index #%d (count=#%d)",
				light_index,
				rasterizer_lights.light_count));
		light = &rasterizer_lights.lights[light_index];
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2837,
			light->radius>0.0f);
		lighting_constants->point_lights[constant_index].position = light->position;
		lighting_constants->point_lights[constant_index].inverse_radius_squared =
			1.0f / (light->radius * light->radius);
		lighting_constants->point_lights[constant_index].forward = light->forward;
		lighting_constants->point_lights[constant_index].color = light->color;
		if (light->definition->geometry.runtime_cosine_falloff_angle != -1.0f)
		{
			real falloff_scale = 1.0f /
				(light->definition->geometry.runtime_cosine_falloff_angle -
					light->definition->geometry.runtime_cosine_cutoff_angle);

			lighting_constants->point_lights[constant_index].falloff_scale =
				falloff_scale;
			lighting_constants->point_lights[constant_index].falloff_offset =
				-(falloff_scale *
					light->definition->geometry.runtime_cosine_cutoff_angle);
		}
		else
		{
			lighting_constants->point_lights[constant_index].falloff_scale = 0.0f;
			lighting_constants->point_lights[constant_index].falloff_offset = 1.0f;
		}
	}
	else
	{
		csmemset(
			&lighting_constants->point_lights[constant_index],
			0,
			sizeof(lighting_constants->point_lights[constant_index]));
		lighting_constants->point_lights[constant_index].falloff_scale = 0.0f;
		lighting_constants->point_lights[constant_index].falloff_offset = 1.0f;
	}
	return;
}

void rasterizer_set_model_lighting_distant_light(
	struct render_distant_light const *light,
	short light_index,
	struct rasterizer_model_lighting_constants *lighting_constants)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2874,
		lighting_constants);
	if (light)
	{
		lighting_constants->distant_lights[light_index].direction =
			light->direction;
		lighting_constants->distant_lights[light_index].color = light->color;
	}
	else
	{
		csmemset(
			&lighting_constants->distant_lights[light_index],
			0,
			sizeof(lighting_constants->distant_lights[light_index]));
	}
	return;
}

void rasterizer_set_model_lighting(
	struct render_lighting const *lighting)
{
	struct rasterizer_model_lighting_constants lighting_constants;
	short light_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2896,
		lighting);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2897,
		lighting->point_light_count>=0 && lighting->point_light_count<=MAXIMUM_RENDERED_POINT_LIGHTS);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2898,
		lighting->distant_light_count>=0 && lighting->distant_light_count<=MAXIMUM_RENDERED_DISTANT_LIGHTS);
	if (rasterizer_debug_options.model_lighting_ambient > 0.0f)
	{
		csmemset(&lighting_constants, 0, sizeof(lighting_constants));
		lighting_constants.ambient_color.red =
			lighting_constants.ambient_color.green =
			lighting_constants.ambient_color.blue =
				rasterizer_debug_options.model_lighting_ambient;
	}
	else
	{
		for (light_index = 0; light_index < MAXIMUM_RENDERED_POINT_LIGHTS; light_index++)
			rasterizer_set_model_lighting_point_light(
				lighting->point_light_count > light_index ?
					lighting->point_light_indices[light_index] : NONE,
				light_index,
				&lighting_constants);
		for (light_index = 0; light_index < MAXIMUM_RENDERED_DISTANT_LIGHTS; light_index++)
			rasterizer_set_model_lighting_distant_light(
				lighting->distant_light_count > light_index ?
					&lighting->distant_lights[light_index] : NULL,
				light_index,
				&lighting_constants);
		lighting_constants.ambient_color = lighting->ambient_color;
	}
	D3DDevice_SetVertexShaderConstant(
		-79,
		&lighting_constants,
		NUMBER_OF_MODEL_LIGHTING_CONSTANTS);
	if (rasterizer_debug_options.stats)
		rasterizer_frame_statistics.vertex_shader_lighting_constant_bytes +=
			sizeof(lighting_constants);
	return;
}

void rasterizer_set_model_skinning(
	struct rasterizer_model_skinning_parameters const *skinning)
{
	short node_index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2749,
		skinning);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2750,
		skinning->node_matrices);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2751,
		skinning->node_matrix_count>0 && skinning->node_matrix_count<RASTERIZER_MAXIMUM_NODES_PER_MODEL);
	for (node_index = 0; node_index < skinning->node_matrix_count; node_index++)
	{
		real scale = skinning->node_matrices[node_index].scale;
		real_matrix4x3 const *matrix = &skinning->node_matrices[node_index];
		real (*constants)[4] = node_matrix_constants[node_index];

		constants[0][0] = scale * matrix->forward.i;
		constants[0][1] = scale * matrix->left.i;
		constants[0][2] = scale * matrix->up.i;
		constants[0][3] = matrix->position.x;
		constants[1][0] = scale * matrix->forward.j;
		constants[1][1] = scale * matrix->left.j;
		constants[1][2] = scale * matrix->up.j;
		constants[1][3] = matrix->position.y;
		constants[2][0] = scale * matrix->forward.k;
		constants[2][1] = scale * matrix->left.k;
		constants[2][2] = scale * matrix->up.k;
		constants[2][3] = matrix->position.z;
	}
	D3DDevice_SetVertexShaderConstant(
		-36,
		node_matrix_constants,
		skinning->node_matrix_count * 3);
	if (rasterizer_debug_options.stats)
		rasterizer_frame_statistics.vertex_shader_skinning_constant_bytes +=
			skinning->node_matrix_count * sizeof(node_matrix_constants[0]);
	return;
}

boolean rasterizer_set_texture_non_blocking(
	short stage,
	short type,
	short usage,
	long bitmap_definition_index,
	short bitmap_sequence_index)
{
	struct bitmap_data *bitmap = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2142,
		stage>=0 && stage<RASTERIZER_MAXIMUM_TEXTURE_STAGES);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2143,
		type>=0 && type<NUMBER_OF_BITMAP_TYPES);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2144,
		usage>=0 && usage<NUMBER_OF_BITMAP_USAGES);
	if ((rasterizer_debug_options.bump_mapping ||
		usage != _bitmap_usage_bump_map) &&
		bitmap_definition_index != NONE)
	{
		long bitmap_count = bitmap_group_get(bitmap_definition_index)->bitmaps.count;

		if (bitmap_count > 0)
		{
			bitmap = bitmap_group_try_and_get_bitmap(
				bitmap_definition_index,
				bitmap_sequence_index % bitmap_count);
			if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
			{
				if (bitmap->type != type)
				{
					error(
						_error_silent,
						"### ERROR incompatible bitmap type in shader got %s expected %s",
						bitmap_type_get_string(bitmap->type),
						bitmap_type_get_string(type));
					bitmap = NULL;
				}
			}
			else
			{
				return TRUE;
			}
		}
	}
	if (!bitmap)
	{
		long default_definition_index =
			global_rasterizer_data->default_textures[type].index;

		if (default_definition_index != NONE)
			bitmap = bitmap_group_try_and_get_bitmap(
				default_definition_index,
				usage);
	}
	if (bitmap)
	{
		rasterizer_set_texture_bitmap_data(stage, bitmap);
		bitmap_dimensions_non_blocking.x = bitmap->width;
		bitmap_dimensions_non_blocking.y = bitmap->height;
	}
	else
	{
		display_assert(
			"### YOU GOT FUCKED in rasterizer_set_texture_non_blocking",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2210,
			TRUE);
		error(
			_error_silent,
			"### ERROR default texture not found (stage=%d, type=%d, usage=%d)",
			stage,
			type,
			usage);
	}
	return FALSE;
}

void rasterizer_set_target_as_texture(
	short stage,
	short target,
	short max_mipmap)
{
	boolean success = TRUE;
	D3DTexture *d3d_texture = NULL;
	D3DBaseTexture *d3d_texture_water;

	switch (target)
	{
	case _rasterizer_target_render_primary:
		{
			D3DBaseTexture *render_primary = global_d3d_texture_render_primary[
				rasterizer_globals.fps_accumulation_frame_index & 1];

			match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2415, max_mipmap==0);
			if (!render_primary->Data)
			{
				D3DSurface *d3d_backbuffer;
				long result;

				IDirect3DDevice8_GetBackBuffer(
					global_d3d_device,
					0,
					0,
					&d3d_backbuffer);
				render_primary->Common = RASTERIZER_TARGET_RENDER_PRIMARY_COMMON;
				render_primary->Data = d3d_backbuffer->Data;
				render_primary->Lock = 0;
#ifdef HALO_LINUX
				/* the back buffer's width (rasterizer_screen_width_update) */
				render_primary->Size = d3d_backbuffer->Size;
#else
				render_primary->Size = RASTERIZER_TARGET_RENDER_PRIMARY_SIZE;
#endif
				render_primary->Format = RASTERIZER_TARGET_RENDER_PRIMARY_FORMAT;
				result = IDirect3DSurface8_Release(d3d_backbuffer);
				if (result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DSurface8_Release(d3d_backbuffer)");
				}
			}
			d3d_texture = (D3DTexture *)global_d3d_texture_render_primary[
				rasterizer_globals.fps_accumulation_frame_index & 1];
		}
		break;

	case _rasterizer_target_render_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2457, max_mipmap==0);
		d3d_texture = global_d3d_texture_render_secondary;
		break;

	case _rasterizer_target_shadow_primary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2461, max_mipmap==0);
		d3d_texture = global_d3d_texture_shadow_primary;
		break;

	case _rasterizer_target_shadow_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2465, max_mipmap==0);
		d3d_texture = global_d3d_texture_shadow_secondary;
		break;

	case _rasterizer_target_sun_glow_primary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2469, max_mipmap==0);
		d3d_texture = global_d3d_texture_sun_glow_primary;
		break;

	case _rasterizer_target_sun_glow_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2473, max_mipmap==0);
		d3d_texture = global_d3d_texture_sun_glow_secondary;
		break;

	case _rasterizer_target_water_bumpmap:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2477,
			max_mipmap>=0 && max_mipmap<=RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS);
		d3d_texture = global_d3d_texture_water;
		break;

	case _rasterizer_target_render_primary_copy:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2492, max_mipmap==0);
		d3d_texture = (D3DTexture *)global_d3d_texture_render_primary_copy;
		break;

	default:
		display_assert(
			"### ERROR unsupported rasterizer target",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2496,
			TRUE);
		system_exit(-1);
		break;
	}
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2499, d3d_texture);
	d3d_texture_water = (D3DBaseTexture *)global_d3d_texture_water;
	if (max_mipmap == 0)
		max_mipmap = RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS;
	else
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2508,
			max_mipmap>=0 && max_mipmap<=RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS);
	d3d_texture_water->Format = (d3d_texture_water->Format & 0xfff0ffff) |
		(max_mipmap << 16);
	IDirect3DDevice8_SetTexture(
		global_d3d_device,
		stage,
		(IDirect3DBaseTexture8 *)d3d_texture);
	if (!success)
	{
		rasterizer_error(
			0,
			"IDirect3DDevice8_SetTexture(global_d3d_device, stage, (IDirect3DBaseTexture8*)d3d_texture)");
		error(_error_silent, "### ERROR rasterizer_set_target_as_texture failed");
	}
	return;
}

void rasterizer_set_target(
	short target,
	short mipmap_index,
	pixel32 clear_color,
	boolean clear,
	boolean zbuffer)
{
	D3DVIEWPORT8 viewport;
	D3DSURFACE_DESC d3d_surface_desc;
	D3DSurface *d3d_surface = NULL;
	D3DSurface *d3d_surface_z = NULL;

	switch (target)
	{
	case _rasterizer_target_render_primary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2280, mipmap_index==0);
		d3d_surface = global_d3d_surface_render_primary;
		d3d_surface_z = global_d3d_surface_render_primary_z;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2284, d3d_surface);
		break;

	case _rasterizer_target_render_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2287, mipmap_index==0);
		d3d_surface = global_d3d_surface_render_secondary;
		d3d_surface_z = global_d3d_surface_render_secondary_z ?
			global_d3d_surface_render_secondary_z :
			global_d3d_surface_render_primary_z;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2290, d3d_surface);
		break;

	case _rasterizer_target_shadow_primary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2293, mipmap_index==0);
		d3d_surface = global_d3d_surface_shadow_primary;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2295, d3d_surface);
		break;

	case _rasterizer_target_shadow_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2298, mipmap_index==0);
		d3d_surface = global_d3d_surface_shadow_secondary;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2300, d3d_surface);
		break;

	case _rasterizer_target_sun_glow_primary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2303, mipmap_index==0);
		d3d_surface = global_d3d_surface_sun_glow_primary;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2305, d3d_surface);
		break;

	case _rasterizer_target_sun_glow_secondary:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2308, mipmap_index==0);
		d3d_surface = global_d3d_surface_sun_glow_secondary;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2310, d3d_surface);
		break;

	case _rasterizer_target_water_bumpmap:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2313,
			mipmap_index>=0 && mipmap_index<RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS);
		d3d_surface = global_d3d_surface_water[mipmap_index];
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2315, d3d_surface);
		break;

	case _rasterizer_target_render_primary_copy:
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2331, mipmap_index==0);
		d3d_surface = global_d3d_surface_render_primary_copy;
		match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2333, d3d_surface);
		break;

	default:
		display_assert(
			"### ERROR unsupported rasterizer target",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2336,
			TRUE);
		system_exit(-1);
		break;
	}
	match_assert("c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c", 2340, !zbuffer||d3d_surface_z);
	IDirect3DDevice8_SetRenderTarget(
		global_d3d_device,
		d3d_surface,
		zbuffer ? d3d_surface_z : NULL);
	if (target == _rasterizer_target_render_primary)
	{
		viewport.X = global_window_parameters.camera.viewport_bounds.x0;
		viewport.Y = global_window_parameters.camera.viewport_bounds.y0;
		viewport.Width = global_window_parameters.camera.viewport_bounds.x1 -
			global_window_parameters.camera.viewport_bounds.x0;
		viewport.Height = global_window_parameters.camera.viewport_bounds.y1 -
			global_window_parameters.camera.viewport_bounds.y0;
	}
	else
	{
		IDirect3DSurface8_GetDesc(d3d_surface, &d3d_surface_desc);
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = d3d_surface_desc.Width;
		viewport.Height = d3d_surface_desc.Height;
	}
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	IDirect3DDevice8_SetViewport(global_d3d_device, &viewport);
	if (clear)
	{
		unsigned long clear_flags = D3DCLEAR_TARGET;

		if (target == _rasterizer_target_render_primary ||
			target == _rasterizer_target_render_secondary)
			clear_flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
		IDirect3DDevice8_Clear(
			global_d3d_device,
			0,
			NULL,
			clear_flags,
			clear_color,
			1.0f,
			0);
	}
	return;
}

D3DSurface *rasterizer_get_target(
	short target,
	short mipmap_index)
{
	D3DSurface *d3d_surface = NULL;

	switch (target)
	{
	case _rasterizer_target_render_primary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2232,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_render_primary;
		break;

	case _rasterizer_target_render_secondary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2236,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_render_secondary;
		break;

	case _rasterizer_target_shadow_primary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2240,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_shadow_primary;
		break;

	case _rasterizer_target_shadow_secondary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2244,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_shadow_secondary;
		break;

	case _rasterizer_target_sun_glow_primary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2248,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_sun_glow_primary;
		break;

	case _rasterizer_target_sun_glow_secondary:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2252,
			mipmap_index==0);
		d3d_surface = global_d3d_surface_sun_glow_secondary;
		break;

	case _rasterizer_target_water_bumpmap:
		match_assert(
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2256,
			mipmap_index>=0 && mipmap_index<RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS);
		d3d_surface = global_d3d_surface_water[mipmap_index];
		break;

	default:
		display_assert(
			"### ERROR unsupported rasterizer target",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2260,
			TRUE);
		system_exit(-1);
		break;
	}
	return d3d_surface;
}

void _rasterizer_window_begin(
	struct rasterizer_window_begin_parameters const *parameters)
{
	pixel32 clear_color;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1351,
		parameters);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1352,
		global_d3d_device);
	global_window_parameters = *parameters;
	suppress_window_begin_end = (previous_window_index != NONE &&
		parameters->window_index == NONE);
	previous_window_index = parameters->window_index;
	if (!suppress_window_begin_end)
	{
		rasterizer_memory_pool_begin();
		rasterizer_dynamic_geometry_begin();
		rasterizer_transparent_geometry_begin();
		rasterizer_environment_fog_screen_window_begin();
		rasterizer_lights_begin();
	}
	rasterizer_water_set_visibility_for_window(FALSE);
	rasterizer_active_camouflage_set_visibility(FALSE);
	rasterizer_profile_window_begin();
	rasterizer_set_stencil_mode(RASTERIZER_STENCIL_MODE_NONE);
	rasterizer_window_set_fog(&parameters->fog);
	if (rasterizer_debug_options.mode == _rasterizer_mode_clear_to_black)
		clear_color = 0;
	else
		clear_color = real_rgb_color_to_pixel32(
			&global_window_parameters.fog.atmospheric_color);
	if (parameters->rasterizer_target == _rasterizer_target_render_primary ||
		parameters->rasterizer_target == _rasterizer_target_render_secondary)
	{
		rasterizer_profile_begin(_rasterizer_profile_clear);
		rasterizer_set_target(
			parameters->rasterizer_target,
			0,
			clear_color,
			!parameters->suppress_clear,
			TRUE);
		rasterizer_profile_end(_rasterizer_profile_clear);
		if (parameters->rasterizer_target == _rasterizer_target_render_primary)
			match_assert(
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
				1415,
				parameters->camera.z_near!=0.0f);
	}
	else
	{
		display_assert(
			"### ERROR unsupported rasterizer target for scene rendering",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			1420,
			TRUE);
		system_exit(-1);
	}
	rasterizer_set_frustum_z(-1.0f, -1.0f);
	D3DDevice_SetRenderState(
		D3DRS_FILLMODE,
		rasterizer_debug_options.wireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	return;
}

void _rasterizer_window_end(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1567,
		global_d3d_device);
	if (main_get_window_count() > 1 && !rasterizer_debug_options.pad3)
		IDirect3DDevice8_Clear(
			global_d3d_device,
			0,
			NULL,
			D3DCLEAR_TARGET_A,
			0,
			1.0f,
			0);
	if (global_window_parameters.has_mirror)
	{
		rectangle2d bounds;

		bounds.x0 = 512;
		bounds.x1 = 640;
		bounds.y0 = 0;
		bounds.y1 = 96;
		rasterizer_secondary_render_target_debug(&bounds);
	}
	if (global_window_parameters.window_index == NONE)
	{
		rasterizer_frame_statistics_get_fps(&rasterizer_frame_statistics);
		rasterizer_frame_statistics_draw();
	}
	if (!suppress_window_begin_end)
	{
		rasterizer_memory_pool_end();
		rasterizer_dynamic_geometry_end();
		rasterizer_transparent_geometry_end();
		rasterizer_environment_fog_screen_window_end();
		rasterizer_lights_end();
		rasterizer_debug_end();
		rasterizer_debug_begin();
	}
	rasterizer_profile_window_end();
	return;
}

boolean rasterizer_preinitialize__fill_you_up_with_the_devils_cock(
	void)
{
	boolean success;

	d3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (!d3d)
	{
		error(_error_silent, "### ERROR failed to create D3D object");
		success = FALSE;
	}
	else
	{
		D3DPRESENT_PARAMETERS d3d_present_parameters;
		HRESULT result;

		csmemset(&d3d_present_parameters, 0, sizeof(d3d_present_parameters));
		d3d_present_parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		d3d_present_parameters.Windowed = FALSE;
		d3d_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3d_present_parameters.EnableAutoDepthStencil = TRUE;
		d3d_present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3d_present_parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
#ifdef HALO_LINUX
		d3d_present_parameters.BackBufferWidth = halo_screen_width();
#else
		d3d_present_parameters.BackBufferWidth = RASTERIZER_SCREEN_WIDTH;
#endif
		d3d_present_parameters.BackBufferHeight = RASTERIZER_SCREEN_HEIGHT;
		d3d_present_parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		result = IDirect3D8_CreateDevice(
			d3d,
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			NULL,
			RASTERIZER_DEVICE_CREATION_FLAGS,
			&d3d_present_parameters,
			&global_d3d_device);
		if (result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, RASTERIZER_DEVICE_CREATION_FLAGS, &d3d_present_parameters, &global_d3d_device)");
		}
		if (!global_d3d_device)
			success = FALSE;
		if (!success)
		{
			global_d3d_device = NULL;
			error(_error_silent, "### ERROR failed to create D3D device");
		}
		else
		{
			IDirect3DDevice8_GetDeviceCaps(global_d3d_device, &global_d3d_caps);
			IDirect3DDevice8_Present(global_d3d_device, NULL, NULL, NULL, NULL);
			success = TRUE;

			if (global_d3d_device)
			{
				IDirect3DDevice8_Release(global_d3d_device);
				global_d3d_device = NULL;
			}
			if (d3d)
			{
				IDirect3D8_Release(d3d);
				d3d = NULL;
			}
		}
	}
	if (!success)
		error(_error_silent, "### ERROR rasterizer_preinitialize failed");
	return success;
}

void _rasterizer_dispose(
	void)
{
	rasterizer_memory_pool_dispose();
	rasterizer_dynamic_geometry_dispose();
	rasterizer_transparent_geometry_dispose();
	rasterizer_vertex_shaders_dispose();
	rasterizer_debug_dispose();
	rasterizer_frame_statistics_dispose();
	rasterizer_text_cache_dispose();
	rasterizer_detail_objects_dispose();
	rasterizer_profile_dispose();
	rasterizer_environment_fog_screen_dispose();
	rasterizer_screen_effects_dispose();
	texture_cache_delete();
	if (global_d3d_device)
	{
		IDirect3DDevice8_Release(global_d3d_device);
		global_d3d_device = NULL;
	}
	if (d3d)
	{
		IDirect3D8_Release(d3d);
		d3d = NULL;
	}
	return;
}

boolean rasterizer_set_texture_bitmap_data(
	short stage,
	struct bitmap_data const *bitmap)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1935,
		stage>=0 && stage<RASTERIZER_MAXIMUM_TEXTURE_STAGES);
	if (bitmap)
	{
		D3DBaseTexture *d3d_texture;

		profile_texture_start();
		d3d_texture = _texture_cache_bitmap_get_hardware_format(bitmap, TRUE, TRUE);
		profile_texture_end();
		IDirect3DDevice8_SetTexture(global_d3d_device, stage, d3d_texture);
	}
	else
	{
		display_assert(
			"### YOU GOT FUCKED in rasterizer_set_texture_bitmap_data",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			1943,
			TRUE);
		error(_error_silent, "### ERROR direct texture not found (stage=%d)", stage);
	}
	return TRUE;
}

boolean rasterizer_set_texture_direct(
	short stage,
	long bitmap_group_index,
	short bitmap_index)
{
	boolean success = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1958,
		stage>=0 && stage<RASTERIZER_MAXIMUM_TEXTURE_STAGES);
	if (bitmap_group_index != NONE)
	{
		long bitmap_count = bitmap_group_get(bitmap_group_index)->bitmaps.count;

		if (bitmap_count > 0)
		{
			struct bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(
				bitmap_group_index,
				bitmap_index % bitmap_count);

			if (bitmap)
			{
				rasterizer_set_texture_bitmap_data(stage, bitmap);
				success = TRUE;
			}
		}
	}
	if (!success)
	{
		display_assert(
			"### YOU GOT FUCKED in rasterizer_set_texture_direct",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			1982,
			TRUE);
		error(_error_silent, "### ERROR direct texture not found (stage=%d)", stage);
	}
	return success;
}

boolean rasterizer_set_texture_direct_non_blocking(
	short stage,
	long bitmap_group_index,
	short bitmap_index)
{
	boolean texture_pending = FALSE;
	struct bitmap_data *bitmap = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2003,
		stage>=0 && stage<RASTERIZER_MAXIMUM_TEXTURE_STAGES);
	if (bitmap_group_index != NONE)
	{
		long bitmap_count = bitmap_group_get(bitmap_group_index)->bitmaps.count;

		if (bitmap_count > 0)
			bitmap = bitmap_group_try_and_get_bitmap(
				bitmap_group_index,
				bitmap_index % bitmap_count);
	}
	if (bitmap)
	{
		if (_texture_cache_bitmap_get_hardware_format(bitmap, FALSE, TRUE))
			rasterizer_set_texture_bitmap_data(stage, bitmap);
		else
			texture_pending = TRUE;
	}
	else
	{
		display_assert(
			"### YOU GOT FUCKED in rasterizer_set_texture_direct_non_blocking",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			2035,
			TRUE);
		error(_error_silent, "### ERROR direct texture not found (stage=%d)", stage);
	}
	return texture_pending;
}

union point2d *rasterizer_set_texture(
	short stage,
	short type,
	short usage,
	long bitmap_definition_index,
	short bitmap_sequence_index)
{
	boolean success = FALSE;
	struct bitmap_data *bitmap = NULL;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2058,
		stage>=0 && stage<RASTERIZER_MAXIMUM_TEXTURE_STAGES);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2059,
		type>=0 && type<NUMBER_OF_BITMAP_TYPES);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2060,
		usage>=0 && usage<NUMBER_OF_BITMAP_USAGES);
	if ((rasterizer_debug_options.bump_mapping ||
		usage != _bitmap_usage_bump_map) &&
		bitmap_definition_index != NONE)
	{
		long bitmap_count = bitmap_group_get(bitmap_definition_index)->bitmaps.count;

		if (bitmap_count > 0)
		{
			bitmap = bitmap_group_try_and_get_bitmap(
				bitmap_definition_index,
				bitmap_sequence_index % bitmap_count);
			if (bitmap->type == type)
			{
				rasterizer_set_texture_bitmap_data(stage, bitmap);
				success = TRUE;
				bitmap_dimensions.x = bitmap->width;
				bitmap_dimensions.y = bitmap->height;
			}
			else
			{
				error(
					_error_silent,
					"### ERROR incompatible bitmap type in shader got %s expected %s",
					bitmap_type_get_string(bitmap->type),
					bitmap_type_get_string(type));
				bitmap = NULL;
			}
		}
	}
	if (!success)
	{
		long default_definition_index =
			global_rasterizer_data->default_textures[type].index;

		if (default_definition_index != NONE)
			bitmap = bitmap_group_try_and_get_bitmap(
				default_definition_index,
				usage);
		if (bitmap)
		{
			rasterizer_set_texture_bitmap_data(stage, bitmap);
			success = TRUE;
			bitmap_dimensions.x = bitmap->width;
			bitmap_dimensions.y = bitmap->height;
		}
		else
		{
			display_assert(
				"### YOU GOT FUCKED in rasterizer_set_texture",
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
				2117,
				TRUE);
			error(
				_error_silent,
				"### ERROR default texture not found (stage=%d, type=%d, usage=%d)",
				stage,
				type,
				usage);
		}
	}
	return success ? &bitmap_dimensions : NULL;
}

void rasterizer_set_framebuffer_blend_function(
	short framebuffer_blend_function)
{
	/* three separate NONE-terminated tables (January .rdata is 4-byte aligned, so
	 * no single 108-byte array); names, scope and type follow HCEX.pdb's static
	 * locals of this function (const unsigned long srcblend_table[9], ...). */
	static const unsigned long srcblend_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
	{
		D3DBLEND_SRCALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		NONE
	};

	static const unsigned long destblend_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
	{
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_ZERO,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_ONE,
		D3DBLEND_INVSRCALPHA,
		NONE
	};

	static const unsigned long blendop_table[NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS + 1] =
	{
		D3DBLENDOP_ADD,
		D3DBLENDOP_ADD,
		D3DBLENDOP_ADD,
		D3DBLENDOP_ADD,
		D3DBLENDOP_REVSUBTRACT,
		D3DBLENDOP_MIN,
		D3DBLENDOP_MAX,
		D3DBLENDOP_ADD,
		NONE
	};

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1907,
		framebuffer_blend_function>=0 && framebuffer_blend_function<NUMBER_OF_SHADER_FRAMEBUFFER_BLEND_FUNCTIONS);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_SRCBLEND,
		srcblend_table[framebuffer_blend_function]);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_DESTBLEND,
		destblend_table[framebuffer_blend_function]);
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_BLENDOP,
		blendop_table[framebuffer_blend_function]);
	return;
}

void _rasterizer_frame_begin(
	struct rasterizer_frame_begin_parameters const *parameters)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1256,
		global_d3d_device);
	rasterizer_globals.near_clip_distance = rasterizer_get_near_clip_distance();
	global_frame_parameters.game_time_sec = parameters->game_time_sec;
	rasterizer_profile_frame_begin();
	rasterizer_frame_statistics_begin();
	rasterizer_water_set_visibility_for_frame(FALSE);
	rasterizer_lights_begin_for_new_frame();
	texture_cache_idle();
	IDirect3DDevice8_SetRenderState(
		global_d3d_device,
		D3DRS_DXT1NOISEENABLE,
		rasterizer_debug_options.DXTC_noise);
	if (rasterizer_debug_options.environment_decals)
	{
		rasterizer_decal_vertices_begin_update();
		decals_update();
		rasterizer_decal_vertices_end_update();
	}
	return;
}

void rasterizer_secondary_render_target_debug(
	rectangle2d *bounds)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2532,
		bounds);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2533,
		global_d3d_device);
	if (rasterizer_debug_options.secondary_render_target_debug &&
		global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)
	{
		real screen_constants[5][4];
		short screen_width;
		short screen_height;

		rasterizer_set_target_as_texture(
			0,
			_rasterizer_target_render_secondary,
			0);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_ADDRESSU,
			D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_ADDRESSV,
			D3DTADDRESS_CLAMP);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_MAGFILTER,
			D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_MINFILTER,
			D3DTEXF_LINEAR);
		IDirect3DDevice8_SetTextureStageState(
			global_d3d_device,
			0,
			D3DTSS_MIPFILTER,
			D3DTEXF_LINEAR);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_CULLMODE,
			D3DCULL_CCW);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHABLENDENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ALPHATESTENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZENABLE,
			FALSE);
		IDirect3DDevice8_SetRenderState(
			global_d3d_device,
			D3DRS_ZBIAS,
			0);
		rasterizer_set_vertex_shader_permutation(4, 8, 0);
		screen_width = global_window_parameters.camera.viewport_bounds.x1 -
			global_window_parameters.camera.viewport_bounds.x0;
		screen_height = global_window_parameters.camera.viewport_bounds.y1 -
			global_window_parameters.camera.viewport_bounds.y0;
		screen_constants[0][0] = 2.0f * (1.0f / screen_width);
		screen_constants[0][1] = 0.0f;
		screen_constants[0][2] = 0.0f;
		screen_constants[0][3] = -1.0f - 1.0f / screen_width;
		screen_constants[1][0] = 0.0f;
		screen_constants[1][1] = -2.0f * (1.0f / screen_height);
		screen_constants[1][2] = 0.0f;
		screen_constants[1][3] = 1.0f + 1.0f / screen_height;
		screen_constants[2][0] = 0.0f;
		screen_constants[2][1] = 0.0f;
		screen_constants[2][2] = 0.0f;
		screen_constants[2][3] = 0.5f;
		screen_constants[3][0] = 0.0f;
		screen_constants[3][1] = 0.0f;
		screen_constants[3][2] = 0.0f;
		screen_constants[3][3] = 1.0f;
		screen_constants[4][0] = RASTERIZER_TARGET_RENDER_SECONDARY_WIDTH;
		screen_constants[4][1] = RASTERIZER_TARGET_RENDER_SECONDARY_HEIGHT;
		screen_constants[4][2] = 0.0f;
		screen_constants[4][3] = 1.0f;
		D3DDevice_SetVertexShaderConstant(
			-68,
			screen_constants,
			5);
		csmemset(&pixel_shader, 0, sizeof(pixel_shader));
		pixel_shader.texture_modes = 1;
		pixel_shader.combiner_count = 1;
		pixel_shader.final_combiner_inputs_abcd = 8;
		rasterizer_set_pixel_shader(&pixel_shader);
		IDirect3DDevice8_Begin(global_d3d_device, D3DPT_TRIANGLEFAN);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_SPECULAR, 0, 1);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_POSITION, bounds->x0, bounds->y0);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_SPECULAR, 1, 1);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_POSITION, bounds->x1, bounds->y0);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_SPECULAR, 1, 0);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_POSITION, bounds->x1, bounds->y1);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_SPECULAR, 0, 0);
		IDirect3DDevice8_SetVertexData2s(global_d3d_device, D3DVSDE_POSITION, bounds->x0, bounds->y1);
		IDirect3DDevice8_End(global_d3d_device);
	}
	return;
}

void _rasterizer_frame_end(
	void)
{
	boolean success = TRUE;
	short index;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1648,
		global_d3d_device);
	rasterizer_profile_frame_end();
	rasterizer_frame_statistics_end();

	for (index = 0; index < 4; index++)
	{
		if (IDirect3DDevice8_SetTexture(
			global_d3d_device,
			index,
			NULL) >= 0 && success)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				0,
				"IDirect3DDevice8_SetTexture(global_d3d_device, index, NULL)");
		}
	}
	for (index = 0; index < 16; index++)
	{
		if (IDirect3DDevice8_SetStreamSource(
			global_d3d_device,
			index,
			NULL,
			0) >= 0 && success)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				0,
				"IDirect3DDevice8_SetStreamSource(global_d3d_device, index, NULL, 0)");
		}
	}
	if (IDirect3DDevice8_SetIndices(
		global_d3d_device,
		NULL,
		0) >= 0 && success)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			0,
			"IDirect3DDevice8_SetIndices(global_d3d_device, NULL, 0)");
	}
	if (!success)
		error(_error_silent, "### ERROR rasterizer_frame_end failed");
	return;
}

#ifdef HALO_LINUX
/* The native ports can change the width of the screen between frames (F11
switches the desktop builds between fullscreen at the display's shape and a
640x480 window: halo_screen_commit, port/linux/src/d3d8_gl.c, which resizes
the back buffer). The screen and the title-safe frame follow, as set up in
rasterizer_initialize, and so do the textures the game made of the back
buffer and its copy. */
static void rasterizer_screen_width_update(
	void)
{
	long width = halo_screen_commit();

	if (width != rasterizer_globals.reserved04.screen_bounds.x1 - rasterizer_globals.reserved04.screen_bounds.x0)
	{
		short frame_index;

		rasterizer_globals.reserved04.screen_bounds.x0 = 0;
		rasterizer_globals.reserved04.screen_bounds.x1 = (short)width;
		rasterizer_globals.reserved04.frame_bounds.x0 =
			(short)(RASTERIZER_FRAME_BOUNDS_X0 * width / RASTERIZER_SCREEN_WIDTH);
		rasterizer_globals.reserved04.frame_bounds.x1 =
			(short)(width - rasterizer_globals.reserved04.frame_bounds.x0);
		if (global_d3d_surface_render_primary)
		{
			DWORD size = global_d3d_surface_render_primary->Size;

			for (frame_index = 0; frame_index < 2; frame_index++)
			{
				if (global_d3d_texture_render_primary[frame_index])
					global_d3d_texture_render_primary[frame_index]->Size = size;
			}
			if (global_d3d_texture_render_primary_copy)
				global_d3d_texture_render_primary_copy->Size = size;
			if (global_d3d_surface_render_primary_copy)
				global_d3d_surface_render_primary_copy->Size = size;
		}
	}

	return;
}

#endif
void _rasterizer_present(
	struct bitmap_data *screenshot_bitmap,
	point2d const *screenshot_index)
{
	boolean success = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		1689,
		global_d3d_device);
	if (screenshot_bitmap && screenshot_bitmap->base_address)
	{
		rectangle2d bounds = rasterizer_globals.reserved04.screen_bounds;

		if (screenshot_index)
		{
			short screen_width = rasterizer_globals.reserved04.screen_bounds.x1 -
				rasterizer_globals.reserved04.screen_bounds.x0;
			short screen_height = rasterizer_globals.reserved04.screen_bounds.y1 -
				rasterizer_globals.reserved04.screen_bounds.y0;

			bounds.x0 = screenshot_index->x * screen_width;
			bounds.y0 = screenshot_index->y * screen_height;
			bounds.x1 = bounds.x0 + screen_width;
			bounds.y1 = bounds.y0 + screen_height;
		}
		if ((screenshot_bitmap->format == _bitmap_format_a8r8g8b8 ||
			screenshot_bitmap->format == _bitmap_format_x8r8g8b8) &&
			screenshot_bitmap->mipmap_count == 0 &&
			bounds.x0 >= 0 &&
			bounds.y0 >= 0 &&
			bounds.x1 <= screenshot_bitmap->width &&
			bounds.y1 <= screenshot_bitmap->height)
		{
			D3DSurface *d3d_backbuffer = NULL;
			D3DSURFACE_DESC d3d_surface_desc;

			IDirect3DDevice8_GetBackBuffer(
				global_d3d_device,
				0,
				D3DBACKBUFFER_TYPE_MONO,
				&d3d_backbuffer);
			IDirect3DSurface8_GetDesc(d3d_backbuffer, &d3d_surface_desc);
			if (d3d_surface_desc.Size ==
				d3d_surface_desc.Width * d3d_surface_desc.Height * sizeof(pixel32))
			{
				D3DLOCKED_RECT d3d_locked_rect;

				IDirect3DSurface8_LockRect(
					d3d_backbuffer,
					&d3d_locked_rect,
					NULL,
					D3DLOCK_TILED | D3DLOCK_READONLY);
				if (d3d_locked_rect.pBits)
				{
					short screen_width = rasterizer_globals.reserved04.screen_bounds.x1 -
						rasterizer_globals.reserved04.screen_bounds.x0;
					short screen_height = rasterizer_globals.reserved04.screen_bounds.y1 -
						rasterizer_globals.reserved04.screen_bounds.y0;
					short y;

					match_assert(
						"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
						1732,
						d3d_locked_rect.Pitch==bitmap_format_get_bits_per_pixel(screenshot_bitmap->format)*screen_width/CHAR_BITS);
					for (y = 0; y < screen_height; y++)
					{
						void *bitmap_address = bitmap_2d_address(
							screenshot_bitmap,
							bounds.x0,
							bounds.y0 + y,
							0);

						csmemcpy(
							bitmap_address,
							(byte *)d3d_locked_rect.pBits + y * d3d_locked_rect.Pitch,
							d3d_locked_rect.Pitch);
					}
					IDirect3DSurface8_UnlockRect(d3d_backbuffer);
					success = TRUE;
				}
				else
				{
					error(_error_silent, "### ERROR rasterizer_present: failed to lock backbuffer surface");
					success = FALSE;
				}
			}
			else
			{
				error(_error_silent, "### ERROR rasterizer_present: failed to get backbuffer surface");
				success = FALSE;
			}
		}
		else
		{
			error(_error_silent, "### ERROR rasterizer_present: invalid bitmap");
			success = FALSE;
		}
	}
	if (IDirect3DDevice8_Present(
		global_d3d_device,
		NULL,
		NULL,
		window_globals.hWndPresentTarget,
		NULL) >= 0 && success)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			0,
			"IDirect3DDevice8_Present(global_d3d_device, NULL, NULL, window_globals.hWndPresentTarget, NULL)");
	}
	rasterizer_globals.fps_accumulation_frame_index++;
#ifdef HALO_LINUX
	rasterizer_screen_width_update();
#endif
	if (!success)
		error(_error_silent, "### ERROR rasterizer_present failed");
	return;
}

short rasterizer_get_stencil_mode(
	void)
{
	return rasterizer_state_cache.stencil_mode;
}

void rasterizer_set_stencil_mode(
	long stencil_mode)
{
	short mode;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		3034,
		global_d3d_device);
	if (!rasterizer_debug_options.stencil_mask)
		stencil_mode = RASTERIZER_STENCIL_MODE_NONE;
	mode = (short)stencil_mode;
	if (mode != rasterizer_state_cache.stencil_mode)
	{
		switch (mode)
		{
		case RASTERIZER_STENCIL_MODE_NONE:
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILENABLE,
				FALSE);
			break;

		case RASTERIZER_STENCIL_MODE_WRITE:
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILFAIL,
				D3DSTENCILOP_KEEP);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILZFAIL,
				D3DSTENCILOP_KEEP);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILPASS,
				D3DSTENCILOP_REPLACE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILFUNC,
				D3DCMP_ALWAYS);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILREF,
				1);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILMASK,
				1);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILWRITEMASK,
				1);
			break;

		case RASTERIZER_STENCIL_MODE_REJECT:
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILFAIL,
				D3DSTENCILOP_KEEP);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILZFAIL,
				D3DSTENCILOP_KEEP);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILPASS,
				D3DSTENCILOP_KEEP);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILFUNC,
				D3DCMP_EQUAL);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILREF,
				0);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILMASK,
				1);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILWRITEMASK,
				0);
			break;

		case RASTERIZER_STENCIL_MODE_ACCEPT:
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILENABLE,
				TRUE);
			IDirect3DDevice8_SetRenderState(
				global_d3d_device,
				D3DRS_STENCILFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILZFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILPASS,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILFUNC,
				D3DCMP_NOTEQUAL);
			SetRenderStateSmart(
				D3DRS_STENCILREF,
				0);
			SetRenderStateSmart(
				D3DRS_STENCILMASK,
				1);
			SetRenderStateSmart(
				D3DRS_STENCILWRITEMASK,
				0);
			break;

		case RASTERIZER_STENCIL_MODE_WRITE_ALPHA_TESTED_DECAL:
			SetRenderStateSmart(
				D3DRS_STENCILENABLE,
				TRUE);
			SetRenderStateSmart(
				D3DRS_STENCILFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILZFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILPASS,
				D3DSTENCILOP_REPLACE);
			SetRenderStateSmart(
				D3DRS_STENCILFUNC,
				D3DCMP_EQUAL);
			SetRenderStateSmart(
				D3DRS_STENCILREF,
				2);
			SetRenderStateSmart(
				D3DRS_STENCILMASK,
				1);
			SetRenderStateSmart(
				D3DRS_STENCILWRITEMASK,
				2);
			break;

		case RASTERIZER_STENCIL_MODE_REJECT_ALPHA_TESTED_DECAL:
			SetRenderStateSmart(
				D3DRS_STENCILENABLE,
				TRUE);
			SetRenderStateSmart(
				D3DRS_STENCILFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILZFAIL,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILPASS,
				D3DSTENCILOP_KEEP);
			SetRenderStateSmart(
				D3DRS_STENCILFUNC,
				D3DCMP_EQUAL);
			SetRenderStateSmart(
				D3DRS_STENCILREF,
				0);
			SetRenderStateSmart(
				D3DRS_STENCILMASK,
				3);
			SetRenderStateSmart(
				D3DRS_STENCILWRITEMASK,
				0);
			break;

		default:
			display_assert(
				"### ERROR unsupported stencil mode",
				"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
				3099,
				TRUE);
			system_exit(-1);
			break;
		}
		rasterizer_state_cache.stencil_mode = mode;
	}
	return;
}

void rasterizer_set_frustum_z(
	real z_near,
	real z_far)
{
	real vertex_constants[8][4];
	short column;
	short row;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2967,
		global_d3d_device);
	render_camera_hack_frustum_z(
		&global_window_parameters.frustum,
		z_near,
		z_far);
	for (column = 0; column < 4; column++)
	{
		for (row = 0; row < 4; row++)
		{
			const real *n = global_window_parameters.frustum.world_to_view.n[row];

			vertex_constants[column][row] =
				n[0] * global_window_parameters.frustum.projection_matrix[0][column] +
				n[1] * global_window_parameters.frustum.projection_matrix[1][column] +
				n[2] * global_window_parameters.frustum.projection_matrix[2][column];
		}
		vertex_constants[column][3] +=
			global_window_parameters.frustum.projection_matrix[3][column];
	}
	vertex_constants[4][0] = global_window_parameters.camera.position.x;
	vertex_constants[4][1] = global_window_parameters.camera.position.y;
	vertex_constants[4][2] = global_window_parameters.camera.position.z;
	vertex_constants[4][3] = 2.0f;
	vertex_constants[5][0] = global_window_parameters.camera.forward.i;
	vertex_constants[5][1] = global_window_parameters.camera.forward.j;
	vertex_constants[5][2] = global_window_parameters.camera.forward.k;
	vertex_constants[5][3] = 0.5f;
	vertex_constants[6][0] = global_window_parameters.frustum.view_to_world.forward.i;
	vertex_constants[6][1] = global_window_parameters.frustum.view_to_world.forward.j;
	vertex_constants[6][2] = global_window_parameters.frustum.view_to_world.forward.k;
	vertex_constants[6][3] = 1.0f;
	vertex_constants[7][0] = global_window_parameters.frustum.view_to_world.left.i;
	vertex_constants[7][1] = global_window_parameters.frustum.view_to_world.left.j;
	vertex_constants[7][2] = global_window_parameters.frustum.view_to_world.left.k;
	vertex_constants[7][3] = 255.9375f;
	IDirect3DDevice8_SetVertexShaderConstant(
		global_d3d_device,
		-96,
		vertex_constants,
		8);
	return;
}

void rasterizer_set_vertex_shader(
	short vertex_shader_index)
{
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2613,
		vertex_shader_index>=0);
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		2614,
		vertex_shader_index<NUMBER_OF_VERTEX_SHADERS);
	if (vertex_shader_index != rasterizer_state_cache.vertex_shader_index)
	{
		boolean success;

		if (vertex_shader_table[vertex_shader_index].handle != NONE)
		{
			IDirect3DDevice8_SetVertexShader(
				global_d3d_device,
				vertex_shader_table[vertex_shader_index].handle);
			success = TRUE;
			if (rasterizer_debug_options.stats)
				rasterizer_frame_statistics.vertex_shader_instruction_count +=
					vertex_shader_table[vertex_shader_index].instruction_count;
		}
		else
		{
			error(
				_error_silent,
				"### ERROR vertex shader not valid (#%d)",
				vertex_shader_index);
			success = FALSE;
		}
		rasterizer_state_cache.vertex_shader_index = vertex_shader_index;
		if (!success)
			error(_error_silent, "### ERROR rasterizer_set_vertex_shader failed");
	}
	return;
}



/* January's Xbox backend owns the fields beyond rasterizer.h's shared public
 * prefix.  This typed private view is backed by this object's option handling,
 * initialization constants, and texture-creation destinations. */
struct rasterizer_xbox_rasterizer_globals
{
	byte reserved00[0x38];
	short push_buffer_size;
	short kick_off_size;
	boolean floating_point_z_buffer;
	byte reserved3D[3];
	short refresh_rate;
	byte reserved42[0x12];
	void *default_2d_hardware_format;
	void *default_3d_hardware_format;
	void *default_cm_hardware_format;
	byte reserved60[8];
};

typedef char verify_rasterizer_xbox_rasterizer_globals_size[
	sizeof(struct rasterizer_xbox_rasterizer_globals) ==
		sizeof(struct rasterizer_globals_definition) ? 1 : -1];
typedef char verify_rasterizer_xbox_push_buffer_size_offset[
	offsetof(
		struct rasterizer_xbox_rasterizer_globals,
		push_buffer_size) == 0x38 ? 1 : -1];
typedef char verify_rasterizer_xbox_refresh_rate_offset[
	offsetof(
		struct rasterizer_xbox_rasterizer_globals,
		refresh_rate) == 0x40 ? 1 : -1];
typedef char verify_rasterizer_xbox_default_2d_hardware_format_offset[
	offsetof(
		struct rasterizer_xbox_rasterizer_globals,
		default_2d_hardware_format) == 0x54 ? 1 : -1];

boolean _rasterizer_initialize(
	void)
{
	boolean success;

	if (rasterizer_push_buffer_size == 0)
		rasterizer_push_buffer_size = RASTERIZER_DEFAULT_PUSH_BUFFER_SIZE;
	if (rasterizer_kick_off_size == 0)
		rasterizer_kick_off_size = RASTERIZER_DEFAULT_KICK_OFF_SIZE;
	Direct3D_SetPushBufferSize(
		rasterizer_push_buffer_size * RASTERIZER_PUSH_BUFFER_SIZE_UNIT,
		rasterizer_kick_off_size * RASTERIZER_PUSH_BUFFER_SIZE_UNIT);
	d3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (!d3d)
	{
		error(_error_silent, "### ERROR failed to create D3D object");
		success = FALSE;
	}
	else
	{
		D3DPRESENT_PARAMETERS d3d_present_parameters;
		HRESULT result;

		rasterizer_globals.reserved04.screen_bounds.y0 = 0;
		rasterizer_globals.reserved04.screen_bounds.x0 = 0;
#ifdef HALO_LINUX
		/* the device's aspect ratio, with the title-safe frame (the HUD)
		widened in proportion */
		rasterizer_globals.reserved04.screen_bounds.x1 = (short)halo_screen_width();
		rasterizer_globals.reserved04.screen_bounds.y1 = RASTERIZER_SCREEN_HEIGHT;
		rasterizer_globals.reserved04.frame_bounds.x0 =
			(short)(RASTERIZER_FRAME_BOUNDS_X0 * halo_screen_width() / RASTERIZER_SCREEN_WIDTH);
		rasterizer_globals.reserved04.frame_bounds.x1 =
			(short)(halo_screen_width() - rasterizer_globals.reserved04.frame_bounds.x0);
#else
		rasterizer_globals.reserved04.screen_bounds.x1 = RASTERIZER_SCREEN_WIDTH;
		rasterizer_globals.reserved04.screen_bounds.y1 = RASTERIZER_SCREEN_HEIGHT;
		rasterizer_globals.reserved04.frame_bounds.x0 = RASTERIZER_FRAME_BOUNDS_X0;
		rasterizer_globals.reserved04.frame_bounds.x1 = RASTERIZER_FRAME_BOUNDS_X1;
#endif
		rasterizer_globals.reserved04.frame_bounds.y0 = RASTERIZER_FRAME_BOUNDS_Y0;
		rasterizer_globals.reserved04.frame_bounds.y1 = RASTERIZER_FRAME_BOUNDS_Y1;
		rasterizer_globals.fps_accumulation_frame_index = 1;
		csmemset(&d3d_present_parameters, 0, sizeof(d3d_present_parameters));
		d3d_present_parameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		d3d_present_parameters.Windowed = FALSE;
		d3d_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3d_present_parameters.EnableAutoDepthStencil = TRUE;
		d3d_present_parameters.AutoDepthStencilFormat = rasterizer_floating_point_z_buffer ?
			D3DFMT_F24S8 : D3DFMT_D24S8;
		d3d_present_parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3d_present_parameters.BackBufferWidth = rasterizer_globals.reserved04.screen_bounds.x1 -
			rasterizer_globals.reserved04.screen_bounds.x0;
		d3d_present_parameters.BackBufferHeight = rasterizer_globals.reserved04.screen_bounds.y1 -
			rasterizer_globals.reserved04.screen_bounds.y0;
		switch (rasterizer_refresh_rate)
		{
		case NONE:
			d3d_present_parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			global_frame_rate_throttle = FALSE;
			break;

		case 30:
			d3d_present_parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_TWO;
			global_frame_rate_throttle = TRUE;
			break;

		case 60:
			d3d_present_parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			global_frame_rate_throttle = TRUE;
			break;

		default:
			if (rasterizer_refresh_rate != 0)
			{
				error(
					_error_silent,
					"### ERROR unsupported refresh rate (%dHz), switching to default",
					rasterizer_refresh_rate);
				rasterizer_refresh_rate = 0;
			}
			d3d_present_parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			global_frame_rate_throttle = TRUE;
			break;
		}
		result = IDirect3D8_CreateDevice(
			d3d,
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			NULL,
			RASTERIZER_DEVICE_CREATION_FLAGS,
			&d3d_present_parameters,
			&global_d3d_device);
		if (result >= 0)
		{
			success = TRUE;
		}
		else
		{
			success = FALSE;
			rasterizer_error(
				result,
				"IDirect3D8_CreateDevice(d3d, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, RASTERIZER_DEVICE_CREATION_FLAGS, &d3d_present_parameters, &global_d3d_device)");
		}
		if (!global_d3d_device)
			success = FALSE;
		if (!success)
		{
			global_d3d_device = NULL;
			error(_error_silent, "### ERROR failed to create D3D device");
		}
		else
		{
			D3DCOLOR *palette_data;

			IDirect3DDevice8_GetDeviceCaps(global_d3d_device, &global_d3d_caps);
			SetupSmartStates();
			palette_data = NULL;
			result = IDirect3DDevice8_CreatePalette(global_d3d_device, D3DPALETTE_256, &d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_CreatePalette(global_d3d_device, D3DPALETTE_256, &d3d_palette)");
			}
			result = IDirect3DPalette8_Lock(d3d_palette, &palette_data, 0);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DPalette8_Lock(d3d_palette, &palette_data, 0)");
			}
			csmemcpy(palette_data, global_vector_palette, sizeof(global_vector_palette));
			result = IDirect3DPalette8_Unlock(d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DPalette8_Unlock(d3d_palette)");
			}
			result = IDirect3DDevice8_SetPalette(global_d3d_device, 0, d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_SetPalette(global_d3d_device, 0, d3d_palette)");
			}
			result = IDirect3DDevice8_SetPalette(global_d3d_device, 1, d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_SetPalette(global_d3d_device, 1, d3d_palette)");
			}
			result = IDirect3DDevice8_SetPalette(global_d3d_device, 2, d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_SetPalette(global_d3d_device, 2, d3d_palette)");
			}
			result = IDirect3DDevice8_SetPalette(global_d3d_device, 3, d3d_palette);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DDevice8_SetPalette(global_d3d_device, 3, d3d_palette)");
			}
			if (success)
			{
				D3DBaseTexture *d3d_texture;
				D3DSurface *d3d_surface;
				short mipmap_index;
				short frame_index;

				IDirect3DDevice8_GetBackBuffer(
					global_d3d_device,
					0,
					D3DBACKBUFFER_TYPE_MONO,
					&global_d3d_surface_render_primary);
				result = IDirect3DDevice8_GetDepthStencilSurface(
					global_d3d_device,
					&global_d3d_surface_render_primary_z);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_GetDepthStencilSurface(global_d3d_device, &global_d3d_surface_render_primary_z)");
				}
				global_d3d_texture_render_primary[0] = match_malloc(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
					562,
					sizeof(D3DBaseTexture));
				global_d3d_texture_render_primary[1] = match_malloc(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
					563,
					sizeof(D3DBaseTexture));
				if (global_d3d_texture_render_primary[0] && global_d3d_texture_render_primary[1])
				{
					for (frame_index = 0; frame_index < 2; frame_index++)
					{
						d3d_texture = global_d3d_texture_render_primary[frame_index];

						d3d_texture->Common = RASTERIZER_TARGET_RENDER_PRIMARY_COMMON;
						d3d_texture->Data = (frame_index == 1) ? global_d3d_surface_render_primary->Data : 0;
						d3d_texture->Lock = 0;
#ifdef HALO_LINUX
						/* the back buffer's width (rasterizer_screen_width_update) */
						d3d_texture->Size = global_d3d_surface_render_primary->Size;
#else
						d3d_texture->Size = RASTERIZER_TARGET_RENDER_PRIMARY_SIZE;
#endif
						d3d_texture->Format = RASTERIZER_TARGET_RENDER_PRIMARY_FORMAT;
					}
				}
				else
				{
					success = FALSE;
				}
				d3d_surface = match_malloc(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
					610,
					sizeof(D3DSurface));
				global_d3d_surface_render_primary_copy = d3d_surface;
				if (d3d_surface)
				{
					*d3d_surface = *global_d3d_surface_render_primary;
					d3d_surface->Data = global_d3d_surface_render_primary_z->Data;
				}
				else
				{
					success = FALSE;
				}
				d3d_texture = match_malloc(
					"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
					623,
					sizeof(D3DBaseTexture));
				global_d3d_texture_render_primary_copy = d3d_texture;
				if (d3d_texture)
				{
					d3d_texture->Common = RASTERIZER_TARGET_RENDER_PRIMARY_COMMON;
					d3d_texture->Data = global_d3d_surface_render_primary_z->Data;
					d3d_texture->Lock = 0;
#ifdef HALO_LINUX
					d3d_texture->Size = global_d3d_surface_render_primary->Size;
#else
					d3d_texture->Size = RASTERIZER_TARGET_RENDER_PRIMARY_SIZE;
#endif
					d3d_texture->Format = RASTERIZER_TARGET_RENDER_PRIMARY_FORMAT;
				}
				else
				{
					success = FALSE;
				}
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_RENDER_SECONDARY_WIDTH,
					RASTERIZER_TARGET_RENDER_SECONDARY_HEIGHT,
					1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_LIN_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_render_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_RENDER_SECONDARY_WIDTH, RASTERIZER_TARGET_RENDER_SECONDARY_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_LIN_A8R8G8B8, D3DPOOL_DEFAULT, &global_d3d_texture_render_secondary)");
				}
				result = IDirect3DTexture8_GetSurfaceLevel(
					global_d3d_texture_render_secondary,
					0,
					&global_d3d_surface_render_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_render_secondary, 0, &global_d3d_surface_render_secondary)");
				}
				if (!global_d3d_texture_render_secondary || !global_d3d_surface_render_secondary)
					success = FALSE;
				global_d3d_texture_render_secondary_z = NULL;
				global_d3d_surface_render_secondary_z = NULL;
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_WATER_SIZE,
					RASTERIZER_TARGET_WATER_SIZE,
					RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_water);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_WATER_SIZE, RASTERIZER_TARGET_WATER_SIZE, RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &global_d3d_texture_water)");
				}
				if (!global_d3d_texture_water)
					success = FALSE;
				for (mipmap_index = 0; success && mipmap_index < RASTERIZER_TARGET_WATER_MAX_MIPMAP_LEVELS; mipmap_index++)
				{
					result = IDirect3DTexture8_GetSurfaceLevel(
						global_d3d_texture_water,
						mipmap_index,
						&global_d3d_surface_water[mipmap_index]);
					if (success && result >= 0)
					{
						success = TRUE;
					}
					else
					{
						success = FALSE;
						rasterizer_error(
							result,
							"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_water, mipmap_index, &global_d3d_surface_water[mipmap_index])");
					}
					if (!global_d3d_surface_water[mipmap_index])
						success = FALSE;
				}
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE,
					RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE,
					1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_R5G6B5,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_shadow_primary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE, RASTERIZER_TARGET_SHADOW_PRIMARY_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &global_d3d_texture_shadow_primary)");
				}
				result = IDirect3DTexture8_GetSurfaceLevel(
					global_d3d_texture_shadow_primary,
					0,
					&global_d3d_surface_shadow_primary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_shadow_primary, 0, &global_d3d_surface_shadow_primary)");
				}
				if (!global_d3d_texture_shadow_primary || !global_d3d_surface_shadow_primary)
					success = FALSE;
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE,
					RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE,
					1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_R5G6B5,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_shadow_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE, RASTERIZER_TARGET_SHADOW_SECONDARY_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &global_d3d_texture_shadow_secondary)");
				}
				result = IDirect3DTexture8_GetSurfaceLevel(
					global_d3d_texture_shadow_secondary,
					0,
					&global_d3d_surface_shadow_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_shadow_secondary, 0, &global_d3d_surface_shadow_secondary)");
				}
				if (!global_d3d_texture_shadow_secondary || !global_d3d_surface_shadow_secondary)
					success = FALSE;
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_SUN_GLOW_SIZE,
					RASTERIZER_TARGET_SUN_GLOW_SIZE,
					1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_sun_glow_primary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_SUN_GLOW_SIZE, RASTERIZER_TARGET_SUN_GLOW_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &global_d3d_texture_sun_glow_primary)");
				}
				result = IDirect3DTexture8_GetSurfaceLevel(
					global_d3d_texture_sun_glow_primary,
					0,
					&global_d3d_surface_sun_glow_primary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_sun_glow_primary, 0, &global_d3d_surface_sun_glow_primary)");
				}
				if (!global_d3d_texture_sun_glow_primary || !global_d3d_surface_sun_glow_primary)
					success = FALSE;
				result = IDirect3DDevice8_CreateTexture(
					global_d3d_device,
					RASTERIZER_TARGET_SUN_GLOW_SIZE,
					RASTERIZER_TARGET_SUN_GLOW_SIZE,
					1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&global_d3d_texture_sun_glow_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DDevice8_CreateTexture(global_d3d_device, RASTERIZER_TARGET_SUN_GLOW_SIZE, RASTERIZER_TARGET_SUN_GLOW_SIZE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &global_d3d_texture_sun_glow_secondary)");
				}
				result = IDirect3DTexture8_GetSurfaceLevel(
					global_d3d_texture_sun_glow_secondary,
					0,
					&global_d3d_surface_sun_glow_secondary);
				if (success && result >= 0)
				{
					success = TRUE;
				}
				else
				{
					success = FALSE;
					rasterizer_error(
						result,
						"IDirect3DTexture8_GetSurfaceLevel(global_d3d_texture_sun_glow_secondary, 0, &global_d3d_surface_sun_glow_secondary)");
				}
				if (!global_d3d_texture_sun_glow_secondary || !global_d3d_surface_sun_glow_secondary)
					success = FALSE;
				if (!success)
				{
					error(_error_silent, "### ERROR failed to create offscreen surface(s)");
				}
				else
				{
					IDirect3DDevice8_SetShaderConstantMode(global_d3d_device, D3DSCM_192CONSTANTS);
					success = TRUE;
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZENABLE, TRUE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZWRITEENABLE, TRUE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ZBIAS, 0);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_SOLIDOFFSETENABLE, TRUE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHATESTENABLE, FALSE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHAFUNC, D3DCMP_GREATER);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHAREF, 0);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_ALPHABLENDENABLE, FALSE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_SRCBLEND, D3DBLEND_ONE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_DESTBLEND, D3DBLEND_ZERO);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_BLENDOP, D3DBLENDOP_ADD);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_FOGENABLE, TRUE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_LIGHTING, FALSE);
					IDirect3DDevice8_SetRenderState(global_d3d_device, D3DRS_SPECULARENABLE, TRUE);
					IDirect3DDevice8_SetTextureStageState(global_d3d_device, 0, D3DTSS_TEXCOORDINDEX, 0);
					IDirect3DDevice8_SetTextureStageState(global_d3d_device, 1, D3DTSS_TEXCOORDINDEX, 1);
					IDirect3DDevice8_SetTextureStageState(global_d3d_device, 2, D3DTSS_TEXCOORDINDEX, 2);
					IDirect3DDevice8_SetTextureStageState(global_d3d_device, 3, D3DTSS_TEXCOORDINDEX, 3);
					D3DDevice_SetFlickerFilter(RASTERIZER_FLICKER_FILTER_LEVEL);
					D3DDevice_SetSoftDisplayFilter(FALSE);
				}
			}
		}
	}
	rasterizer_filthy_bitmap_defaults_initialize();
	success = success &&
		rasterizer_memory_pool_initialize() &&
		rasterizer_dynamic_geometry_initialize() &&
		rasterizer_transparent_geometry_initialize() &&
		rasterizer_vertex_shaders_initialize() &&
		rasterizer_debug_initialize() &&
		rasterizer_frame_statistics_initialize() &&
		rasterizer_text_cache_initialize() &&
		rasterizer_detail_objects_initialize() &&
		rasterizer_profile_initialize() &&
		rasterizer_environment_fog_screen_initialize();
	rasterizer_screen_effects_initialize();
	texture_cache_new();
	rasterizer_debug_begin();
	if (success)
		rasterizer_globals.initialized = TRUE;
	else
		error(_error_silent, "### ERROR failed to initialize rasterizer");
	return success;
}

/* ---------- private code */
void SetupSmartStates(
	void)
{
	long stage_index;
	long state_index;

	for (state_index = 0; state_index < D3DRS_MAX; state_index++)
		IDirect3DDevice8_GetRenderState(
			global_d3d_device,
			state_index,
			&renderstate_table[state_index]);
	for (state_index = 0; state_index < D3DTSS_MAX; state_index++)
	{
		IDirect3DDevice8_GetTextureStageState(
			global_d3d_device,
			0,
			state_index,
			&texturestagestate_table[0][state_index]);
		IDirect3DDevice8_GetTextureStageState(
			global_d3d_device,
			1,
			state_index,
			&texturestagestate_table[1][state_index]);
		IDirect3DDevice8_GetTextureStageState(
			global_d3d_device,
			2,
			state_index,
			&texturestagestate_table[2][state_index]);
		IDirect3DDevice8_GetTextureStageState(
			global_d3d_device,
			3,
			state_index,
			&texturestagestate_table[3][state_index]);
	}
	for (stage_index = 0; stage_index < D3DTSS_MAXSTAGES; stage_index++)
		texture_table[stage_index] = NULL;
	return;
}


void *rasterizer_get_bitmap_default_hardware_format(
	struct bitmap_data const *bitmap)
{
	void *hardware_format;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		209,
		bitmap);
	switch (bitmap->type)
	{
	case _bitmap_type_2d:
		hardware_format = rasterizer_default_2d_hardware_format;
		break;

	case _bitmap_type_3d:
		hardware_format = rasterizer_default_2d_hardware_format;
		break;

	case _bitmap_type_cube_map:
		hardware_format = rasterizer_default_cm_hardware_format;
		break;

	/* hardware_format is left unassigned only by this default arm. Not reached unassigned: the
	 * arm's assertion failure calls system_exit, which does not return in January
	 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
	 * Source-policy approval pending (2026-09-27 audit). */
	default:
		display_assert(
			"### ERROR unsupported bitmap type",
			"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
			223,
			TRUE);
		system_exit(-1);
		break;
	}
	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		226,
		hardware_format);
	return hardware_format;
}

static void rasterizer_filthy_bitmap_defaults_initialize(
	void)
{
	IDirect3DTexture8 *default_2d_hardware_format;
	IDirect3DVolumeTexture8 *default_3d_hardware_format;
	IDirect3DCubeTexture8 *default_cm_hardware_format;
	D3DLOCKED_RECT d3d_locked_rect;
	D3DLOCKED_BOX d3d_locked_box;
	word pixels[2];
	boolean success;
	HRESULT result;
	long face_index;
	long face_count;
	long pixel_index;
	long pixel_count;

	match_assert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		239,
		global_d3d_device);
	result = IDirect3DDevice8_CreateTexture(
		global_d3d_device,
		DEFAULT_BITMAP_SIZE,
		DEFAULT_BITMAP_SIZE,
		1,
		0,
		D3DFMT_A4R4G4B4,
		D3DPOOL_MANAGED,
		&default_2d_hardware_format);
	if (result >= 0)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			result,
			"IDirect3DDevice8_CreateTexture(global_d3d_device, 4, 4, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &(IDirect3DTexture8*)default_2d_hardware_format)");
	}
	result = IDirect3DDevice8_CreateVolumeTexture(
		global_d3d_device,
		DEFAULT_BITMAP_SIZE,
		DEFAULT_BITMAP_SIZE,
		DEFAULT_BITMAP_SIZE,
		1,
		0,
		D3DFMT_A4R4G4B4,
		D3DPOOL_MANAGED,
		&default_3d_hardware_format);
	if (success && result >= 0)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			result,
			"IDirect3DDevice8_CreateVolumeTexture(global_d3d_device, 4, 4, 4, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &(IDirect3DVolumeTexture8*)default_3d_hardware_format)");
	}
	result = IDirect3DDevice8_CreateCubeTexture(
		global_d3d_device,
		DEFAULT_BITMAP_SIZE,
		1,
		0,
		D3DFMT_A4R4G4B4,
		D3DPOOL_MANAGED,
		&default_cm_hardware_format);
	if (success && result >= 0)
	{
		success = TRUE;
	}
	else
	{
		success = FALSE;
		rasterizer_error(
			result,
			"IDirect3DDevice8_CreateCubeTexture(global_d3d_device, 4, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &(IDirect3DCubeTexture8*)default_cm_hardware_format)");
	}
	if (success &&
		default_2d_hardware_format &&
		default_3d_hardware_format &&
		default_cm_hardware_format)
	{
		pixels[0] = DEFAULT_BITMAP_PIXEL0;
		pixels[1] = DEFAULT_BITMAP_PIXEL1;
		IDirect3DTexture8_LockRect(
			default_2d_hardware_format,
			0,
			&d3d_locked_rect,
			NULL,
			0);
		for (pixel_index = 0, pixel_count = DEFAULT_BITMAP_SIZE * DEFAULT_BITMAP_SIZE; pixel_count; pixel_index++, pixel_count--)
			((word *)d3d_locked_rect.pBits)[pixel_index] = pixels[pixel_index & 1];
		IDirect3DTexture8_UnlockRect(
			default_2d_hardware_format,
			0);
		IDirect3DVolumeTexture8_LockBox(
			default_3d_hardware_format,
			0,
			&d3d_locked_box,
			NULL,
			0);
		for (pixel_index = 0, pixel_count = DEFAULT_BITMAP_SIZE * DEFAULT_BITMAP_SIZE * DEFAULT_BITMAP_SIZE; pixel_count; pixel_index++, pixel_count--)
			((word *)d3d_locked_box.pBits)[pixel_index] = pixels[pixel_index & 1];
		IDirect3DVolumeTexture8_UnlockBox(
			default_3d_hardware_format,
			0);
		success = TRUE;
		for (face_index = 0, face_count = NUMBER_OF_CUBE_MAP_FACES; face_count; face_index++, face_count--)
		{
			result = IDirect3DCubeTexture8_LockRect(
				default_cm_hardware_format,
				face_index,
				0,
				&d3d_locked_rect,
				NULL,
				0);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DCubeTexture8_LockRect((IDirect3DCubeTexture8*)default_cm_hardware_format, face_index, 0, &d3d_locked_rect, NULL, 0)");
			}
			for (pixel_index = 0, pixel_count = DEFAULT_BITMAP_SIZE * DEFAULT_BITMAP_SIZE; pixel_count; pixel_index++, pixel_count--)
				((word *)d3d_locked_rect.pBits)[pixel_index] = pixels[pixel_index & 1];
			result = IDirect3DCubeTexture8_UnlockRect(
				default_cm_hardware_format,
				face_index,
				0);
			if (success && result >= 0)
			{
				success = TRUE;
			}
			else
			{
				success = FALSE;
				rasterizer_error(
					result,
					"IDirect3DCubeTexture8_UnlockRect((IDirect3DCubeTexture8*)default_cm_hardware_format, face_index, 0)");
			}
		}
	}
	else
	{
		success = FALSE;
	}
	match_vassert(
		"c:\\halo\\SOURCE\\rasterizer\\xbox\\rasterizer_xbox.c",
		311,
		success,
		"### ERROR rasterizer_filthy_bitmap_default_initialize failed");
	rasterizer_default_2d_hardware_format = default_2d_hardware_format;
	rasterizer_default_3d_hardware_format = default_3d_hardware_format;
	rasterizer_default_cm_hardware_format = default_cm_hardware_format;
	return;
}
