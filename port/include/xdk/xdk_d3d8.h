/*
XDK_D3D8.H

Direct3D 8 (the Xbox's) and D3DX declarations the game and the platform
layer use.
(README.md)

The types, enumerations and library prototypes are in xdk_pdb.h. What is
here was worked out from the January build itself (cachebeta.exe and its
PDB: the D3D library's code and tables, and the game functions that use
each name, whose source byte-matches), from the NV2A's documented register
layouts (xboxdevwiki.net, nxdk) and from the public DirectX 8
documentation; each group names its source. The platform layer
(port/linux/src) implements the library functions the wrappers call.
*/

#ifndef HALO_XDK_D3D8_H
#define HALO_XDK_D3D8_H

#include "xdk_xbox.h"

/* ---------- macros */

/* the calling convention of the library's register-argument entry points
(the PDB's D3DDevice_SetRenderState_Simple and friends) */
#define D3DFASTCALL __fastcall

/* how the wrappers below are defined; a unit may choose otherwise before
including this header (rasterizer_xbox_vertex_shaders_initialize.c wants
plain static copies) */
#ifndef D3DINLINE
#define D3DINLINE static __forceinline
#endif

/* how D3DX's inline functions are defined (the D3DX math functions the
PDB lists are the game's own copies of them, with external linkage) */
#ifndef D3DXINLINE
#define D3DXINLINE __inline
#endif

/* __declspec(selectany) data: one copy of a table defined in every unit.
The Linux and Android prefix header maps it to a weak definition; an
MSVC-target compiler has the real thing. */
#ifndef DECLSPEC_SELECTANY
#define DECLSPEC_SELECTANY __declspec(selectany)
#endif

/* the interface names, which on the Xbox are the resource and device
structures themselves */
#define IDirect3D8 Direct3D
#define IDirect3DDevice8 D3DDevice
#define IDirect3DResource8 D3DResource
#define IDirect3DBaseTexture8 D3DBaseTexture
#define IDirect3DTexture8 D3DTexture
#define IDirect3DCubeTexture8 D3DCubeTexture
#define IDirect3DVolumeTexture8 D3DVolumeTexture
#define IDirect3DSurface8 D3DSurface
#define IDirect3DVertexBuffer8 D3DVertexBuffer
#define IDirect3DIndexBuffer8 D3DIndexBuffer
#define IDirect3DPalette8 D3DPalette

/* Direct3DCreate8's argument: the January build passes 0
(rasterizer_initialize and rasterizer_preinitialize in cachebeta.exe) */
#define D3D_SDK_VERSION 0

/* device creation, as in DirectX 8; the values are the ones the January
build passes to Direct3D_CreateDevice (rasterizer_initialize) */
#define D3DADAPTER_DEFAULT 0
#define D3DCREATE_HARDWARE_VERTEXPROCESSING 0x00000040L
#define D3DPRESENTFLAG_LOCKABLE_BACKBUFFER 0x00000001

/* presentation intervals: rasterizer_initialize's refresh rate switch
stores 0, 1, 2 and 0x80000000 (the DirectX 8 values) */
#define D3DPRESENT_INTERVAL_DEFAULT 0x00000000L
#define D3DPRESENT_INTERVAL_ONE 0x00000001L
#define D3DPRESENT_INTERVAL_TWO 0x00000002L
#define D3DPRESENT_INTERVAL_IMMEDIATE 0x80000000L

/* GetBackBuffer's buffer type: the only one, 0 (DirectX 8; progress_bar.c
passes a literal 0 for it) */
#define D3DBACKBUFFER_TYPE_MONO 0

/* a surface that is not multisampled, as D3DSurface_GetDesc reports it
(the library stores 0x11 in the description's MultiSampleType) */
#define D3DMULTISAMPLE_NONE 0x0011

/* errors: facility 0x876 as in DirectX 8. NOTFOUND is DirectX 8's code
2150 (also rasterizer_xbox_errors.c); TESTINCOMPLETE is what
D3DDevice_GetVisibilityTestResult returns while the test is pending, and
what rasterizer_transparent_geometry_groups_end waits on. */
#define D3DERR_NOTFOUND ((HRESULT)0x88760866L)
#define D3DERR_TESTINCOMPLETE ((HRESULT)0x88760828L)

/* shader version numbers in the device caps (DirectX 8's encoding) */
#define D3DPS_VERSION(major, minor) (0xFFFF0000 | ((major) << 8) | (minor))
#define D3DVS_VERSION(major, minor) (0xFFFE0000 | ((major) << 8) | (minor))

/* Clear's flags: the NV2A's clear surface bits (xboxdevwiki, nxdk
NV097_CLEAR_SURFACE: z, stencil, then one bit per colour channel from
0x10). The January build clears with 0xf0 and 0xf3 (rasterizer_set_target),
0x80 (_rasterizer_window_end) and 0x80 | 1
(_rasterizer_environment_fog_screen_begin). */
#define D3DCLEAR_ZBUFFER 0x00000001L
#define D3DCLEAR_STENCIL 0x00000002L
#define D3DCLEAR_TARGET_R 0x00000010L
#define D3DCLEAR_TARGET_G 0x00000020L
#define D3DCLEAR_TARGET_B 0x00000040L
#define D3DCLEAR_TARGET_A 0x00000080L
#define D3DCLEAR_TARGET (D3DCLEAR_TARGET_R | D3DCLEAR_TARGET_G | D3DCLEAR_TARGET_B | D3DCLEAR_TARGET_A)

/* D3DRS_COLORWRITEENABLE: one bit per byte, in D3DCOLOR's channel order
(the NV2A's colour mask, xboxdevwiki). The January build writes 0x1000000
for alpha (_rasterizer_environment_reflection_lightmap_masks_begin) and
0x10101 for red, green and blue (rasterizer_secondary_render_target_debug). */
#define D3DCOLORWRITEENABLE_BLUE (1L << 0)
#define D3DCOLORWRITEENABLE_GREEN (1L << 8)
#define D3DCOLORWRITEENABLE_RED (1L << 16)
#define D3DCOLORWRITEENABLE_ALPHA (1L << 24)
#define D3DCOLORWRITEENABLE_ALL (D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | \
	D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA)

/* resource usage and pool: the January build passes 1 and pool 0 for its
render target, pool 1 for its default textures (rasterizer_initialize) and
0x208 for dynamic write-only vertex buffers (rasterizer_xbox_detail_objects.c);
the DirectX 8 values */
#define D3DUSAGE_RENDERTARGET 0x00000001L
#define D3DUSAGE_WRITEONLY 0x00000008L
#define D3DUSAGE_DYNAMIC 0x00000200L
#define D3DPOOL_DEFAULT 0
#define D3DPOOL_MANAGED 1

/* lock flags, which differ from DirectX 8's: the January build locks
textures with 0x20 (no overwrite, rasterizer_xbox_hardware_bitmaps.c),
vertex buffers read-only with 0x80 (rasterizer_xbox_decals.c) and the back
buffer tiled and read-only with 0xc0 (rasterizer_xbox.c) */
#define D3DLOCK_NOOVERWRITE 0x00000020L
#define D3DLOCK_TILED 0x00000040L
#define D3DLOCK_READONLY 0x00000080L

/* SetShaderConstantMode: rasterizer_initialize selects 1 (192 constants);
the library's D3DDevice_SetShaderConstantMode takes bit 0x10 apart from the
count, as the switch that leaves the reserved constants to the game */
#define D3DSCM_192CONSTANTS 1
#define D3DSCM_NORESERVEDCONSTANTS 0x10

/* texture stages: D3D__TextureState, 32 entries per stage, spans 0x200
bytes up to D3D__RenderState in the January build, so four stages */
#define D3DTSS_MAXSTAGES 4

/* a resource's Common word (xboxdevwiki, XPR resource headers): reference
count, type, and whether Direct3D allocated it. The library's own
resources: vertex buffers 0x1000001, index buffers 0x1010001, palettes
0x1030001 | size << 30 (D3DDevice_Create*); D3DResource_Release masks the
count with 0xffff and the type with 0x70000, and treats 0x50000 as a
surface; the game's textures are 0x40001 (rasterizer_xbox.c). */
#define D3DCOMMON_REFCOUNT_MASK 0x0000FFFF
#define D3DCOMMON_TYPE_MASK 0x00070000
#define D3DCOMMON_TYPE_VERTEXBUFFER 0x00000000
#define D3DCOMMON_TYPE_INDEXBUFFER 0x00010000
#define D3DCOMMON_TYPE_PALETTE 0x00030000
#define D3DCOMMON_TYPE_TEXTURE 0x00040000
#define D3DCOMMON_TYPE_SURFACE 0x00050000
#define D3DCOMMON_D3DCREATED 0x01000000
#define D3DPALETTE_COMMON_PALETTESIZE_SHIFT 30

/* a texture's Format word: the NV2A's texture format register
(xboxdevwiki, nxdk NV097_SET_TEXTURE_FORMAT), as the January build's
texture_cache_initialize_hardware_format fills it (0x10029 | format << 8
for a linear texture) and the library's texture layout code reads it
(sizes at bits 20, 24 and 28, levels at 16) */
#define D3DFORMAT_DMACHANNEL_A 0x00000001
#define D3DFORMAT_CUBEMAP 0x00000004
#define D3DFORMAT_BORDERSOURCE_COLOR 0x00000008
#define D3DFORMAT_DIMENSION_MASK 0x000000F0
#define D3DFORMAT_DIMENSION_SHIFT 4
#define D3DFORMAT_FORMAT_MASK 0x0000FF00
#define D3DFORMAT_FORMAT_SHIFT 8
#define D3DFORMAT_MIPMAP_MASK 0x000F0000
#define D3DFORMAT_MIPMAP_SHIFT 16
#define D3DFORMAT_USIZE_MASK 0x00F00000
#define D3DFORMAT_USIZE_SHIFT 20
#define D3DFORMAT_VSIZE_MASK 0x0F000000
#define D3DFORMAT_VSIZE_SHIFT 24
#define D3DFORMAT_PSIZE_MASK 0xF0000000
#define D3DFORMAT_PSIZE_SHIFT 28

/* a linear texture's Size word: width - 1, height - 1, and the pitch in
64-byte units - 1 (xboxdevwiki XPR; the January build's back buffer
descriptor is 0x271df27f, 640x480 with a 2560-byte pitch) */
#define D3DSIZE_WIDTH_MASK 0x00000FFF
#define D3DSIZE_HEIGHT_MASK 0x00FFF000
#define D3DSIZE_HEIGHT_SHIFT 12
#define D3DSIZE_PITCH_MASK 0xFF000000
#define D3DSIZE_PITCH_SHIFT 24

/* texture memory: the library allocates it 128-byte aligned
(D3DDevice_CreateTexture's contiguous allocation) and rounds each cube map
face up to 128 bytes (its texture layout code); pitches are in 64-byte
units (above) */
#define D3DTEXTURE_ALIGNMENT 128
#define D3DTEXTURE_CUBEFACE_ALIGNMENT 128
#define D3DTEXTURE_PITCH_ALIGNMENT 64

/* vertex shader declaration tokens, as the library's declaration parser
(in the D3D section, near D3DDevice_Clear) reads them: the token type in
the top three bits; a stream token's stream in the low four; a data token's
register in the low five and its type in bits 16-23, or with bit 28 a skip
of (bits 16-19) dwords; a constant token's count of four-dword constants in
bits 25-28; the end token all ones. The extension count is DirectX 8's. */
#define D3DVSD_TOKENTYPESHIFT 29
#define D3DVSD_TOKENTYPEMASK (7UL << D3DVSD_TOKENTYPESHIFT)
#define D3DVSD_STREAMNUMBERMASK 0x0000000FUL
#define D3DVSD_DATALOADTYPEMASK (1UL << 28)
#define D3DVSD_VERTEXREGMASK 0x0000001FUL
#define D3DVSD_DATATYPESHIFT 16
#define D3DVSD_DATATYPEMASK (0xFFUL << D3DVSD_DATATYPESHIFT)
#define D3DVSD_SKIPCOUNTSHIFT 16
#define D3DVSD_SKIPCOUNTMASK (0xFUL << D3DVSD_SKIPCOUNTSHIFT)
#define D3DVSD_CONSTCOUNTSHIFT 25
#define D3DVSD_CONSTCOUNTMASK (0xFUL << D3DVSD_CONSTCOUNTSHIFT)
#define D3DVSD_EXTCOUNTSHIFT 24
#define D3DVSD_EXTCOUNTMASK (0x1FUL << D3DVSD_EXTCOUNTSHIFT)
#define D3DVSD_END() 0xFFFFFFFF

/* vertex data types: the NV2A's vertex attribute format, component count
<< 4 | type (xboxdevwiki, nxdk: 0 D3D-order bytes, 1 normalized shorts,
2 floats, 4 bytes, 5 shorts, 6 packed normal). The library's tables
D3D::g_UnitsOfElement and D3D::g_BytesPerUnit size them the same way,
count 7 meaning three; the January build's declarations
(rasterizer_xbox_vertex_shaders_initialize.c) use 0x32, 0x40, 0x21, 0x16, ...;
0x02 (floats, no components: nothing is fetched) is the format the
library's parser gives a register that a tessellator token fills. */
#define D3DVSDT_NONE 0x02
#define D3DVSDT_FLOAT1 0x12
#define D3DVSDT_FLOAT2 0x22
#define D3DVSDT_FLOAT3 0x32
#define D3DVSDT_FLOAT4 0x42
#define D3DVSDT_FLOAT2H 0x72
#define D3DVSDT_D3DCOLOR 0x40
#define D3DVSDT_NORMSHORT1 0x11
#define D3DVSDT_NORMSHORT2 0x21
#define D3DVSDT_NORMSHORT3 0x31
#define D3DVSDT_NORMSHORT4 0x41
#define D3DVSDT_NORMPACKED3 0x16
#define D3DVSDT_SHORT1 0x15
#define D3DVSDT_SHORT2 0x25
#define D3DVSDT_SHORT3 0x35
#define D3DVSDT_SHORT4 0x45
#define D3DVSDT_PBYTE1 0x14
#define D3DVSDT_PBYTE2 0x24
#define D3DVSDT_PBYTE3 0x34
#define D3DVSDT_PBYTE4 0x44

/* immediate-mode vertex registers (SetVertexData*): the January build uses
0 and 4 (rasterizer_secondary_render_target_debug), 3, 9 and 10 and -1 for
a whole vertex (progress_bar.c's draw_layer_int), 9 to 12
(do_convoluation_coords); the library's declaration parser treats 3 and 4
as the colours and 9 to 12 as texture coordinates */
#define D3DVSDE_POSITION 0
#define D3DVSDE_DIFFUSE 3
#define D3DVSDE_SPECULAR 4
#define D3DVSDE_TEXCOORD0 9
#define D3DVSDE_TEXCOORD1 10
#define D3DVSDE_TEXCOORD2 11
#define D3DVSDE_TEXCOORD3 12
#define D3DVSDE_VERTEX (-1)

/* pixel shader state words (D3DPIXELSHADERDEF), from the fields' enumerated
values (xdk_pdb.h) and the NV2A's register combiner registers (xboxdevwiki,
nxdk NV097_SET_COMBINER_*, NV097_SET_SHADER_*): inputs a, b, c, d one byte
each from the top; outputs cd, ab and sum four bits each from the bottom,
then the flags; the combiner count in the low byte with its flags above;
five bits of texture mode per stage; the dot product mapping of stages 1-3
four bits apart; the stage whose result stages 1, 2 and 3 read at bits 0,
16 and 20. Checked against the January build's constants:
rasterizer_active_camouflage_draw (0x2623, 0x11, 0x3420140c, 0xa0c0000) and
_rasterizer_environment_fog_screen_end (0x11004, 0x3089, 0x30ab). */
#define PS_COMBINERINPUTS(a, b, c, d) (((DWORD)(a) << 24) | ((DWORD)(b) << 16) | ((DWORD)(c) << 8) | (DWORD)(d))
#define PS_COMBINEROUTPUTS(ab, cd, mux_sum, flags) \
	(((DWORD)(flags) << 12) | ((DWORD)(mux_sum) << 8) | ((DWORD)(ab) << 4) | (DWORD)(cd))
#define PS_COMBINERCOUNT(count, flags) (((DWORD)(flags) << 8) | (DWORD)(count))
#define PS_TEXTUREMODES(t0, t1, t2, t3) \
	(((DWORD)(t3) << 15) | ((DWORD)(t2) << 10) | ((DWORD)(t1) << 5) | (DWORD)(t0))
#define PS_DOTMAPPING(t0, t1, t2, t3) (((DWORD)(t3) << 8) | ((DWORD)(t2) << 4) | (DWORD)(t1))
#define PS_INPUTTEXTURE(t0, t1, t2, t3) (((DWORD)(t3) << 20) | ((DWORD)(t2) << 16) | (DWORD)(t1))

/* ---------- functions */

/* the January build's constant tables below: shared by every unit that
includes this header (in C++, which gives a const object internal linkage
by default, they must be declared extern to be shared) */
#ifdef __cplusplus
#define HALO_XDK_TABLE extern const DECLSPEC_SELECTANY
#else
#define HALO_XDK_TABLE DECLSPEC_SELECTANY const
#endif

/* state the inline functions below read and write, which the platform
layer defines (port/linux/src/d3d8_gl.c); the library's own copies are
public symbols of the January build (D3D__RenderState, D3D__TextureState,
D3D__IndexData) */
extern DWORD D3D__RenderState[D3DRS_MAX];
extern DWORD D3D__TextureState[D3DTSS_MAXSTAGES][D3DTSS_MAX];
extern WORD *D3D__IndexData;

/* the push buffer method of each simple render state, which
D3DDevice_SetRenderState passes to D3DDevice_SetRenderState_Simple: the
January build's table (public symbol D3DSIMPLERENDERSTATEENCODE, 82 entries,
one per state below D3DRS_SIMPLE_MAX) */
HALO_XDK_TABLE DWORD D3DSIMPLERENDERSTATEENCODE[D3DRS_SIMPLE_MAX] =
{
	0x40260, 0x40264, 0x40268, 0x4026c, 0x40270, 0x40274, /* 0 */
	0x40278, 0x4027c, 0x40288, 0x4028c, 0x40a60, 0x40a64, /* 6 */
	0x40a68, 0x40a6c, 0x40a70, 0x40a74, 0x40a78, 0x40a7c, /* 12 */
	0x40a80, 0x40a84, 0x40a88, 0x40a8c, 0x40a90, 0x40a94, /* 18 */
	0x40a98, 0x40a9c, 0x40aa0, 0x40aa4, 0x40aa8, 0x40aac, /* 24 */
	0x40ab0, 0x40ab4, 0x40ab8, 0x40abc, 0x40ac0, 0x40ac4, /* 30 */
	0x40ac8, 0x40acc, 0x40ad0, 0x40ad4, 0x40ad8, 0x40adc, /* 36 */
	0x417f8, 0x41e20, 0x41e24, 0x41e40, 0x41e44, 0x41e48, /* 42 */
	0x41e4c, 0x41e50, 0x41e54, 0x41e58, 0x41e5c, 0x41e60, /* 48 */
	0x41d90, 0x41e74, 0x41e78, 0x40354, 0x4033c, 0x40304, /* 54 */
	0x40300, 0x40340, 0x40344, 0x40348, 0x4035c, 0x40310, /* 60 */
	0x4037c, 0x40358, 0x40374, 0x40378, 0x40364, 0x40368, /* 66 */
	0x4036c, 0x40360, 0x40350, 0x4034c, 0x409f8, 0x40384, /* 72 */
	0x40388, 0x40330, 0x40334, 0x40338, /* 78 */
};

/* vertices = [type][0] * primitives + [type][1], per D3DPRIMITIVETYPE: the
January build's table (public symbol D3DPRIMITIVETOVERTEXCOUNT), which the
DrawPrimitive and DrawIndexedPrimitive wrappers index */
HALO_XDK_TABLE UINT D3DPRIMITIVETOVERTEXCOUNT[D3DPT_MAX][2] =
{
	{ 0, 0 }, /* 0 */
	{ 1, 0 }, /* D3DPT_POINTLIST */
	{ 2, 0 }, /* D3DPT_LINELIST */
	{ 1, 1 }, /* D3DPT_LINELOOP */
	{ 1, 1 }, /* D3DPT_LINESTRIP */
	{ 3, 0 }, /* D3DPT_TRIANGLELIST */
	{ 1, 2 }, /* D3DPT_TRIANGLESTRIP */
	{ 1, 2 }, /* D3DPT_TRIANGLEFAN */
	{ 4, 0 }, /* D3DPT_QUADLIST */
	{ 2, 2 }, /* D3DPT_QUADSTRIP */
	{ 0, 0 }, /* D3DPT_POLYGON */
};

/* The library functions the SDK defined inline, as the January build's
copies of them do it (the D3DX section's local D3DDevice_SetRenderState,
D3DDevice_SetTextureStageState, D3DDevice_BeginScene and _EndScene; the
game's retained copies of D3DDevice_GetRenderState, _GetTextureStageState,
D3DIndexBuffer_Lock, the Unlock functions and Direct3D_Release, listed in
the sources' symbol comments):
- simple render states go to D3DDevice_SetRenderState_Simple with their
  method and are kept in D3D__RenderState, deferred ones go to
  D3DDevice_SetRenderState_Deferred, and every other state to its own
  function (anything past D3DRS_MAX is ignored);
- texture stage states below D3DTSS_DEFERRED_MAX are deferred, the rest go
  to their own functions (the bump environment ones share one);
- the getters read the state tables;
- scenes need nothing, and unlocking does nothing;
- an index buffer's data is ordinary memory at its Data address;
- the Direct3D object is never freed (Release returns 1). */

/* declared in xdk_pdb.h; __forceinline (not D3DINLINE) marks it for the
Linux build's list of pick-any inline functions */
__forceinline void __stdcall D3DDevice_SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
{
	if (state < D3DRS_SIMPLE_MAX)
	{
		D3DDevice_SetRenderState_Simple(D3DSIMPLERENDERSTATEENCODE[state], value);
		D3D__RenderState[state] = value;
	}
	else if (state < D3DRS_DEFERRED_MAX)
	{
		D3DDevice_SetRenderState_Deferred(state, value);
	}
	else
	{
		switch (state)
		{
		case D3DRS_PSTEXTUREMODES: D3DDevice_SetRenderState_PSTextureModes(value); break;
		case D3DRS_VERTEXBLEND: D3DDevice_SetRenderState_VertexBlend(value); break;
		case D3DRS_FOGCOLOR: D3DDevice_SetRenderState_FogColor(value); break;
		case D3DRS_FILLMODE: D3DDevice_SetRenderState_FillMode(value); break;
		case D3DRS_BACKFILLMODE: D3DDevice_SetRenderState_BackFillMode(value); break;
		case D3DRS_TWOSIDEDLIGHTING: D3DDevice_SetRenderState_TwoSidedLighting(value); break;
		case D3DRS_NORMALIZENORMALS: D3DDevice_SetRenderState_NormalizeNormals(value); break;
		case D3DRS_ZENABLE: D3DDevice_SetRenderState_ZEnable(value); break;
		case D3DRS_STENCILENABLE: D3DDevice_SetRenderState_StencilEnable(value); break;
		case D3DRS_STENCILFAIL: D3DDevice_SetRenderState_StencilFail(value); break;
		case D3DRS_FRONTFACE: D3DDevice_SetRenderState_FrontFace(value); break;
		case D3DRS_CULLMODE: D3DDevice_SetRenderState_CullMode(value); break;
		case D3DRS_TEXTUREFACTOR: D3DDevice_SetRenderState_TextureFactor(value); break;
		case D3DRS_ZBIAS: D3DDevice_SetRenderState_ZBias(value); break;
		case D3DRS_LOGICOP: D3DDevice_SetRenderState_LogicOp(value); break;
		case D3DRS_EDGEANTIALIAS: D3DDevice_SetRenderState_EdgeAntiAlias(value); break;
		case D3DRS_MULTISAMPLEANTIALIAS: D3DDevice_SetRenderState_MultiSampleAntiAlias(value); break;
		case D3DRS_MULTISAMPLEMASK: D3DDevice_SetRenderState_MultiSampleMask(value); break;
		case D3DRS_MULTISAMPLETYPE: D3DDevice_SetRenderState_MultiSampleType(value); break;
		case D3DRS_SHADOWFUNC: D3DDevice_SetRenderState_ShadowFunc(value); break;
		case D3DRS_LINEWIDTH: D3DDevice_SetRenderState_LineWidth(value); break;
		case D3DRS_DXT1NOISEENABLE: D3DDevice_SetRenderState_Dxt1NoiseEnable(value); break;
		case D3DRS_YUVENABLE: D3DDevice_SetRenderState_YuvEnable(value); break;
		case D3DRS_OCCLUSIONCULLENABLE: D3DDevice_SetRenderState_OcclusionCullEnable(value); break;
		case D3DRS_STENCILCULLENABLE: D3DDevice_SetRenderState_StencilCullEnable(value); break;
		case D3DRS_ROPZCMPALWAYSREAD: D3DDevice_SetRenderState_RopZCmpAlwaysRead(value); break;
		case D3DRS_ROPZREAD: D3DDevice_SetRenderState_RopZRead(value); break;
		case D3DRS_DONOTCULLUNCOMPRESSED: D3DDevice_SetRenderState_DoNotCullUncompressed(value); break;
		default: break;
		}
	}
}

D3DINLINE void __stdcall D3DDevice_SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
	if (type < D3DTSS_DEFERRED_MAX)
		D3DDevice_SetTextureState_Deferred(stage, type, value);
	else if (type == D3DTSS_TEXCOORDINDEX)
		D3DDevice_SetTextureState_TexCoordIndex(stage, value);
	else if (type == D3DTSS_BORDERCOLOR)
		D3DDevice_SetTextureState_BorderColor(stage, value);
	else if (type == D3DTSS_COLORKEYCOLOR)
		D3DDevice_SetTextureState_ColorKeyColor(stage, value);
	else if (type <= D3DTSS_BUMPENVLOFFSET)
		D3DDevice_SetTextureState_BumpEnv(stage, type, value);
}

D3DINLINE void __stdcall D3DDevice_GetRenderState(D3DRENDERSTATETYPE state, DWORD *value)
{
	*value = D3D__RenderState[state];
}

D3DINLINE void __stdcall D3DDevice_GetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD *value)
{
	*value = D3D__TextureState[stage][type];
}

D3DINLINE void __stdcall D3DDevice_BeginScene(void)
{
}

D3DINLINE void __stdcall D3DDevice_EndScene(void)
{
}

D3DINLINE void __stdcall D3DIndexBuffer_Lock(D3DIndexBuffer *buffer, UINT offset, UINT size, BYTE **data, DWORD flags)
{
	(void)size;
	(void)flags;
	*data = (BYTE *)buffer->Data + offset;
}

D3DINLINE void __stdcall D3DIndexBuffer_Unlock(D3DIndexBuffer *buffer)
{
	(void)buffer;
}

D3DINLINE void __stdcall D3DVertexBuffer_Unlock(D3DVertexBuffer *buffer)
{
	(void)buffer;
}


D3DINLINE void __stdcall D3DTexture_UnlockRect(D3DTexture *texture, UINT level)
{
}

D3DINLINE void __stdcall D3DCubeTexture_UnlockRect(D3DCubeTexture *texture, D3DCUBEMAP_FACES face, UINT level)
{
}

D3DINLINE void __stdcall D3DVolumeTexture_UnlockBox(D3DVolumeTexture *texture, UINT level)
{
}

D3DINLINE void __stdcall D3DSurface_UnlockRect(D3DSurface *surface)
{
	(void)surface;
}

D3DINLINE void __stdcall D3DPalette_Unlock(D3DPalette *palette)
{
	(void)palette;
}

D3DINLINE ULONG __stdcall Direct3D_Release(void)
{
	return 1;
}

/* The COM-style wrappers. On the Xbox there is one device and one Direct3D
object, so the interface pointer is ignored and each forwards to the C
function (xdk_pdb.h). Where the C function returns nothing, the wrapper
returns S_OK as the DirectX 8 method does; otherwise it passes the result
on. Established from the January build's retained copies of each wrapper
(listed in the sources' symbol comments, e.g. rasterizer_xbox.c's) and, for
those it kept none of, from the inline expansions at the call sites
(DrawIndexedPrimitive in rasterizer_draw_static_triangles_static_vertices,
SetIndices beside it). The Xbox-only methods BlockUntilVerticalBlank,
KickPushBuffer and SetVerticalBlankCallback return nothing. */

/* IDirect3D8 */

D3DINLINE ULONG __stdcall IDirect3D8_Release(Direct3D *direct3d)
{
	(void)direct3d;
	return Direct3D_Release();
}

D3DINLINE HRESULT __stdcall IDirect3D8_CreateDevice(Direct3D *direct3d, UINT adapter, D3DDEVTYPE device_type,
	void *focus_window, DWORD behavior_flags, D3DPRESENT_PARAMETERS *parameters, D3DDevice **device)
{
	(void)direct3d;
	return Direct3D_CreateDevice(adapter, device_type, focus_window, behavior_flags, parameters, device);
}

/* IDirect3DDevice8: the device */

D3DINLINE ULONG __stdcall IDirect3DDevice8_Release(D3DDevice *device)
{
	(void)device;
	return D3DDevice_Release();
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetDeviceCaps(D3DDevice *device, D3DCAPS8 *caps)
{
	(void)device;
	D3DDevice_GetDeviceCaps(caps);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_PersistDisplay(D3DDevice *device)
{
	(void)device;
	return D3DDevice_PersistDisplay();
}

D3DINLINE BOOL __stdcall IDirect3DDevice8_IsBusy(D3DDevice *device)
{
	(void)device;
	return D3DDevice_IsBusy();
}

D3DINLINE void __stdcall IDirect3DDevice8_KickPushBuffer(D3DDevice *device)
{
	(void)device;
	D3DDevice_KickPushBuffer();
}

D3DINLINE void __stdcall IDirect3DDevice8_BlockUntilVerticalBlank(D3DDevice *device)
{
	(void)device;
	D3DDevice_BlockUntilVerticalBlank();
}

D3DINLINE void __stdcall IDirect3DDevice8_SetVerticalBlankCallback(D3DDevice *device, D3DCALLBACK callback)
{
	(void)device;
	D3DDevice_SetVerticalBlankCallback(callback);
}

/* the device: frames and targets */

D3DINLINE HRESULT __stdcall IDirect3DDevice8_BeginScene(D3DDevice *device)
{
	(void)device;
	D3DDevice_BeginScene();
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_EndScene(D3DDevice *device)
{
	(void)device;
	D3DDevice_EndScene();
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_Clear(D3DDevice *device, DWORD count, const D3DRECT *rectangles,
	DWORD flags, D3DCOLOR color, float z, DWORD stencil)
{
	(void)device;
	D3DDevice_Clear(count, rectangles, flags, color, z, stencil);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_Present(D3DDevice *device, const RECT *source_rectangle,
	const RECT *destination_rectangle, void *destination_window, void *dirty_region)
{
	(void)device;
	D3DDevice_Present(source_rectangle, destination_rectangle, destination_window, dirty_region);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetBackBuffer(D3DDevice *device, INT back_buffer, D3DBACKBUFFER_TYPE type,
	D3DSurface **surface)
{
	(void)device;
	D3DDevice_GetBackBuffer(back_buffer, type, surface);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetDepthStencilSurface(D3DDevice *device, D3DSurface **surface)
{
	(void)device;
	return D3DDevice_GetDepthStencilSurface(surface);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetRenderTarget(D3DDevice *device, D3DSurface *render_target,
	D3DSurface *depth_stencil)
{
	(void)device;
	D3DDevice_SetRenderTarget(render_target, depth_stencil);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetViewport(D3DDevice *device, const D3DVIEWPORT8 *viewport)
{
	(void)device;
	D3DDevice_SetViewport(viewport);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetTransform(D3DDevice *device, D3DTRANSFORMSTATETYPE state,
	const D3DMATRIX *matrix)
{
	(void)device;
	D3DDevice_SetTransform(state, matrix);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetTransform(D3DDevice *device, D3DTRANSFORMSTATETYPE state,
	D3DMATRIX *matrix)
{
	(void)device;
	D3DDevice_GetTransform(state, matrix);
	return S_OK;
}

/* the device: render, texture stage and shader state */

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetRenderState(D3DDevice *device, D3DRENDERSTATETYPE state, DWORD value)
{
	(void)device;
	D3DDevice_SetRenderState(state, value);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetRenderState(D3DDevice *device, D3DRENDERSTATETYPE state, DWORD *value)
{
	(void)device;
	D3DDevice_GetRenderState(state, value);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetTextureStageState(D3DDevice *device, DWORD stage,
	D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
	(void)device;
	D3DDevice_SetTextureStageState(stage, type, value);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetTextureStageState(D3DDevice *device, DWORD stage,
	D3DTEXTURESTAGESTATETYPE type, DWORD *value)
{
	(void)device;
	D3DDevice_GetTextureStageState(stage, type, value);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetTexture(D3DDevice *device, DWORD stage, D3DBaseTexture *texture)
{
	(void)device;
	D3DDevice_SetTexture(stage, texture);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetPalette(D3DDevice *device, DWORD stage, D3DPalette *palette)
{
	(void)device;
	D3DDevice_SetPalette(stage, palette);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetPixelShaderProgram(D3DDevice *device, D3DPIXELSHADERDEF *definition)
{
	(void)device;
	D3DDevice_SetPixelShaderProgram(definition);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateVertexShader(D3DDevice *device, const DWORD *declaration,
	const DWORD *function, DWORD *handle, DWORD usage)
{
	(void)device;
	return D3DDevice_CreateVertexShader(declaration, function, handle, usage);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_DeleteVertexShader(D3DDevice *device, DWORD handle)
{
	(void)device;
	D3DDevice_DeleteVertexShader(handle);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexShader(D3DDevice *device, DWORD handle)
{
	(void)device;
	D3DDevice_SetVertexShader(handle);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_LoadVertexShader(D3DDevice *device, DWORD handle, DWORD address)
{
	(void)device;
	D3DDevice_LoadVertexShader(handle, address);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SelectVertexShader(D3DDevice *device, DWORD handle, DWORD address)
{
	(void)device;
	D3DDevice_SelectVertexShader(handle, address);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetVertexShaderSize(D3DDevice *device, DWORD handle, UINT *size)
{
	(void)device;
	D3DDevice_GetVertexShaderSize(handle, size);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexShaderConstant(D3DDevice *device, INT reg, const void *data,
	DWORD count)
{
	(void)device;
	D3DDevice_SetVertexShaderConstant(reg, data, count);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetShaderConstantMode(D3DDevice *device, D3DSHADERCONSTANTMODE mode)
{
	(void)device;
	D3DDevice_SetShaderConstantMode(mode);
	return S_OK;
}

/* the device: vertex data and drawing */

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetStreamSource(D3DDevice *device, UINT stream, D3DVertexBuffer *buffer,
	UINT stride)
{
	(void)device;
	D3DDevice_SetStreamSource(stream, buffer, stride);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetIndices(D3DDevice *device, D3DIndexBuffer *buffer, UINT base_vertex_index)
{
	(void)device;
	D3DDevice_SetIndices(buffer, base_vertex_index);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_DrawVertices(D3DDevice *device, D3DPRIMITIVETYPE type, UINT start_vertex,
	UINT vertex_count)
{
	(void)device;
	D3DDevice_DrawVertices(type, start_vertex, vertex_count);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_DrawPrimitive(D3DDevice *device, D3DPRIMITIVETYPE type, UINT start_vertex,
	UINT primitive_count)
{
	(void)device;
	D3DDevice_DrawVertices(type, start_vertex,
		D3DPRIMITIVETOVERTEXCOUNT[type][0] * primitive_count + D3DPRIMITIVETOVERTEXCOUNT[type][1]);
	return S_OK;
}

/* draws from the index buffer SetIndices selected; the vertex range is
not needed */
D3DINLINE HRESULT __stdcall IDirect3DDevice8_DrawIndexedPrimitive(D3DDevice *device, D3DPRIMITIVETYPE type,
	UINT minimum_index, UINT vertex_count, UINT start_index, UINT primitive_count)
{
	(void)device;
	(void)minimum_index;
	(void)vertex_count;
	D3DDevice_DrawIndexedVertices(type,
		D3DPRIMITIVETOVERTEXCOUNT[type][0] * primitive_count + D3DPRIMITIVETOVERTEXCOUNT[type][1],
		D3D__IndexData + start_index);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_Begin(D3DDevice *device, D3DPRIMITIVETYPE type)
{
	(void)device;
	D3DDevice_Begin(type);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_End(D3DDevice *device)
{
	(void)device;
	D3DDevice_End();
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexData2f(D3DDevice *device, INT reg, float a, float b)
{
	(void)device;
	D3DDevice_SetVertexData2f(reg, a, b);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexData4f(D3DDevice *device, INT reg, float a, float b, float c,
	float d)
{
	(void)device;
	D3DDevice_SetVertexData4f(reg, a, b, c, d);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexData2s(D3DDevice *device, INT reg, SHORT a, SHORT b)
{
	(void)device;
	D3DDevice_SetVertexData2s(reg, a, b);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexData4ub(D3DDevice *device, INT reg, BYTE a, BYTE b, BYTE c, BYTE d)
{
	(void)device;
	D3DDevice_SetVertexData4ub(reg, a, b, c, d);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_SetVertexDataColor(D3DDevice *device, INT reg, D3DCOLOR color)
{
	(void)device;
	D3DDevice_SetVertexDataColor(reg, color);
	return S_OK;
}

/* the device: visibility tests */

D3DINLINE HRESULT __stdcall IDirect3DDevice8_BeginVisibilityTest(D3DDevice *device)
{
	(void)device;
	D3DDevice_BeginVisibilityTest();
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_EndVisibilityTest(D3DDevice *device, DWORD index)
{
	(void)device;
	return D3DDevice_EndVisibilityTest(index);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_GetVisibilityTestResult(D3DDevice *device, DWORD index, UINT *result,
	ULONGLONG *time_stamp)
{
	(void)device;
	return D3DDevice_GetVisibilityTestResult(index, result, time_stamp);
}

/* the device: resource creation */

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateTexture(D3DDevice *device, UINT width, UINT height, UINT levels,
	DWORD usage, D3DFORMAT format, D3DPOOL pool, D3DTexture **texture)
{
	(void)device;
	return D3DDevice_CreateTexture(width, height, levels, usage, format, pool, texture);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateVolumeTexture(D3DDevice *device, UINT width, UINT height,
	UINT depth, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, D3DVolumeTexture **texture)
{
	(void)device;
	return D3DDevice_CreateVolumeTexture(width, height, depth, levels, usage, format, pool, texture);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateCubeTexture(D3DDevice *device, UINT edge_length, UINT levels,
	DWORD usage, D3DFORMAT format, D3DPOOL pool, D3DCubeTexture **texture)
{
	(void)device;
	return D3DDevice_CreateCubeTexture(edge_length, levels, usage, format, pool, texture);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateVertexBuffer(D3DDevice *device, UINT length, DWORD usage, DWORD fvf,
	D3DPOOL pool, D3DVertexBuffer **buffer)
{
	(void)device;
	return D3DDevice_CreateVertexBuffer(length, usage, fvf, pool, buffer);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreateIndexBuffer(D3DDevice *device, UINT length, DWORD usage,
	D3DFORMAT format, D3DPOOL pool, D3DIndexBuffer **buffer)
{
	(void)device;
	return D3DDevice_CreateIndexBuffer(length, usage, format, pool, buffer);
}

D3DINLINE HRESULT __stdcall IDirect3DDevice8_CreatePalette(D3DDevice *device, D3DPALETTESIZE size, D3DPalette **palette)
{
	(void)device;
	return D3DDevice_CreatePalette(size, palette);
}

/* IDirect3DBaseTexture8 (the resource functions) */

D3DINLINE ULONG __stdcall IDirect3DBaseTexture8_Release(D3DBaseTexture *texture)
{
	return D3DResource_Release((D3DResource *)texture);
}

D3DINLINE BOOL __stdcall IDirect3DBaseTexture8_IsBusy(D3DBaseTexture *texture)
{
	return D3DResource_IsBusy((D3DResource *)texture);
}

D3DINLINE void __stdcall IDirect3DBaseTexture8_Register(D3DBaseTexture *texture, void *base)
{
	D3DResource_Register((D3DResource *)texture, base);
}

/* IDirect3DTexture8 */

D3DINLINE ULONG __stdcall IDirect3DTexture8_Release(D3DTexture *texture)
{
	return D3DResource_Release((D3DResource *)texture);
}

D3DINLINE HRESULT __stdcall IDirect3DTexture8_GetLevelDesc(D3DTexture *texture, UINT level, D3DSURFACE_DESC *description)
{
	D3DTexture_GetLevelDesc(texture, level, description);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DTexture8_GetSurfaceLevel(D3DTexture *texture, UINT level, D3DSurface **surface)
{
	return D3DTexture_GetSurfaceLevel(texture, level, surface);
}

D3DINLINE HRESULT __stdcall IDirect3DTexture8_LockRect(D3DTexture *texture, UINT level, D3DLOCKED_RECT *locked,
	const RECT *rectangle, DWORD flags)
{
	D3DTexture_LockRect(texture, level, locked, rectangle, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DTexture8_UnlockRect(D3DTexture *texture, UINT level)
{
	D3DTexture_UnlockRect(texture, level);
	return S_OK;
}

/* IDirect3DCubeTexture8 */

D3DINLINE HRESULT __stdcall IDirect3DCubeTexture8_LockRect(D3DCubeTexture *texture, D3DCUBEMAP_FACES face, UINT level,
	D3DLOCKED_RECT *locked, const RECT *rectangle, DWORD flags)
{
	D3DCubeTexture_LockRect(texture, face, level, locked, rectangle, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DCubeTexture8_UnlockRect(D3DCubeTexture *texture, D3DCUBEMAP_FACES face, UINT level)
{
	D3DCubeTexture_UnlockRect(texture, face, level);
	return S_OK;
}

/* IDirect3DVolumeTexture8 */

D3DINLINE HRESULT __stdcall IDirect3DVolumeTexture8_LockBox(D3DVolumeTexture *texture, UINT level, D3DLOCKED_BOX *locked,
	const D3DBOX *box, DWORD flags)
{
	D3DVolumeTexture_LockBox(texture, level, locked, box, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DVolumeTexture8_UnlockBox(D3DVolumeTexture *texture, UINT level)
{
	D3DVolumeTexture_UnlockBox(texture, level);
	return S_OK;
}

/* IDirect3DSurface8 */

D3DINLINE ULONG __stdcall IDirect3DSurface8_Release(D3DSurface *surface)
{
	return D3DResource_Release((D3DResource *)surface);
}

D3DINLINE HRESULT __stdcall IDirect3DSurface8_GetDesc(D3DSurface *surface, D3DSURFACE_DESC *description)
{
	D3DSurface_GetDesc(surface, description);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DSurface8_LockRect(D3DSurface *surface, D3DLOCKED_RECT *locked,
	const RECT *rectangle, DWORD flags)
{
	D3DSurface_LockRect(surface, locked, rectangle, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DSurface8_UnlockRect(D3DSurface *surface)
{
	D3DSurface_UnlockRect(surface);
	return S_OK;
}

/* IDirect3DVertexBuffer8 */

D3DINLINE ULONG __stdcall IDirect3DVertexBuffer8_Release(D3DVertexBuffer *buffer)
{
	return D3DResource_Release((D3DResource *)buffer);
}

D3DINLINE BOOL __stdcall IDirect3DVertexBuffer8_IsBusy(D3DVertexBuffer *buffer)
{
	return D3DResource_IsBusy((D3DResource *)buffer);
}

D3DINLINE void __stdcall IDirect3DVertexBuffer8_BlockUntilNotBusy(D3DVertexBuffer *buffer)
{
	D3DResource_BlockUntilNotBusy((D3DResource *)buffer);
}

D3DINLINE void __stdcall IDirect3DVertexBuffer8_Register(D3DVertexBuffer *buffer, void *base)
{
	D3DResource_Register((D3DResource *)buffer, base);
}

D3DINLINE HRESULT __stdcall IDirect3DVertexBuffer8_Lock(D3DVertexBuffer *buffer, UINT offset, UINT size, BYTE **data,
	DWORD flags)
{
	D3DVertexBuffer_Lock(buffer, offset, size, data, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DVertexBuffer8_Unlock(D3DVertexBuffer *buffer)
{
	D3DVertexBuffer_Unlock(buffer);
	return S_OK;
}

/* IDirect3DIndexBuffer8 */

D3DINLINE ULONG __stdcall IDirect3DIndexBuffer8_Release(D3DIndexBuffer *buffer)
{
	return D3DResource_Release((D3DResource *)buffer);
}

D3DINLINE BOOL __stdcall IDirect3DIndexBuffer8_IsBusy(D3DIndexBuffer *buffer)
{
	return D3DResource_IsBusy((D3DResource *)buffer);
}

D3DINLINE void __stdcall IDirect3DIndexBuffer8_BlockUntilNotBusy(D3DIndexBuffer *buffer)
{
	D3DResource_BlockUntilNotBusy((D3DResource *)buffer);
}

D3DINLINE HRESULT __stdcall IDirect3DIndexBuffer8_Lock(D3DIndexBuffer *buffer, UINT offset, UINT size, BYTE **data,
	DWORD flags)
{
	D3DIndexBuffer_Lock(buffer, offset, size, data, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DIndexBuffer8_Unlock(D3DIndexBuffer *buffer)
{
	D3DIndexBuffer_Unlock(buffer);
	return S_OK;
}

/* IDirect3DPalette8 */

D3DINLINE HRESULT __stdcall IDirect3DPalette8_Lock(D3DPalette *palette, D3DCOLOR **colors, DWORD flags)
{
	D3DPalette_Lock(palette, colors, flags);
	return S_OK;
}

D3DINLINE HRESULT __stdcall IDirect3DPalette8_Unlock(D3DPalette *palette)
{
	D3DPalette_Unlock(palette);
	return S_OK;
}

/* D3DX: declared in xdk_pdb.h; the game's copy (in progress_bar.c's
object) clears the matrix, sets the diagonal to 1 and returns it */

D3DXINLINE D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *matrix)
{
	int row, column;

	for (row = 0; row < 4; row++)
	{
		for (column = 0; column < 4; column++)
			matrix->m[row][column] = row == column ? 1.0f : 0.0f;
	}
	return matrix;
}

#endif
