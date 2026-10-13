/*
BITMAP_GROUP.C

symbols in this file:
00065210 0010:
	_code_00065210 (0000)
00065220 0020:
	_code_00065220 (0000)
00065240 0420:
	_code_00065240 (0000)
00065660 0050:
	_bitmap_group_try_and_get_bitmap (0000)
000656B0 00e0:
	_bitmap_group_get_bitmap_from_sequence (0000)
00065790 03f0:
	_bitmap_group_add_bitmap (0000)
002523E0 0009:
	??_C@_08PODLPFBP@bitmaps?$CK?$AA@ (0000)
002523EC 000b:
	??_C@_0L@MHNIOAFC@sequences?$CK?$AA@ (0000)
002523F8 0010:
	??_C@_0BA@IDLEJNDG@sprite?5spacing?$CK?$AA@ (0000)
00252408 000d:
	??_C@_0N@JBKPDNJN@sprite?5usage?$AA@ (0000)
00252418 003d:
	??_C@_0DN@LIABDKDH@Sprite?5usage?5controls?5the?5backgr@ (0000)
00252458 001a:
	??_C@_0BK@PEJHLPMF@?4?4?4more?5sprite?5processing?$AA@ (0000)
00252474 002d:
	??_C@_0CN@MNMFNJEI@mipmap?5count?3levels?$CD0?5defaults?5t@ (0000)
002524A4 0032:
	??_C@_0DC@IOGOKMGM@alpha?5bias?3?$FL?91?01?$FN?$CDaffects?5alpha?5@ (0000)
002524D8 004a:
	??_C@_0EK@LGHFBHCF@blur?5filter?5size?3?$FL0?010?$FN?5pixels?$CDb@ (0000)
00252524 000e:
	??_C@_0O@HHLKLECM@miscellaneous?$AA@ (0000)
00252534 0016:
	??_C@_0BG@DPOLDEKM@processed?5pixel?5data?$CK?$AA@ (0000)
0025254C 002e:
	??_C@_0CO@DGGLDMI@Pixel?5data?5after?5being?5processed@ (0000)
0025257C 0015:
	??_C@_0BF@IFIJLIHA@processed?5pixel?5data?$AA@ (0000)
00252594 001d:
	??_C@_0BN@KONKDNNH@compressed?5color?5plate?5data?$CK?$AA@ (0000)
002525B4 001b:
	??_C@_0BL@DFHBBBIO@color?5plate?5height?$CK?3pixels?$AA@ (0000)
002525D0 001a:
	??_C@_0BK@DBGDBACB@color?5plate?5width?$CK?3pixels?$AA@ (0000)
002525EC 0038:
	??_C@_0DI@PNMNMEDC@The?5original?5TIFF?5file?5used?5to?5i@ (0000)
00252624 000c:
	??_C@_0M@OOALLIOJ@color?5plate?$AA@ (0000)
00252630 0014:
	??_C@_0BE@LBDABFMB@sprite?5budget?5count?$AA@ (0000)
00252644 0013:
	??_C@_0BD@MBLHAGLG@sprite?5budget?5size?$AA@ (0000)
00252658 00c0:
	??_C@_0MA@NPOIOCI@When?5creating?5a?5sprite?5group?0?5sp@ (0000)
00252718 0012:
	??_C@_0BC@FOKLDGHM@sprite?5processing?$AA@ (0000)
00252730 00a7:
	??_C@_0KH@FAIFDBCJ@bump?5height?3repeats?$CDthe?5apparent@ (0000)
002527D8 0038:
	??_C@_0DI@CCFOOEFG@sharpen?5amount?3?$FL0?01?$FN?$CDsharpens?5mi@ (0000)
00252810 0063:
	??_C@_0GD@GEAGIEIP@detail?5fade?5factor?3?$FL0?01?$FN?$CD0?5means@ (0000)
00252874 0039:
	??_C@_0DJ@BEENCLGA@These?5properties?5control?5how?5mip@ (0000)
002528B0 0010:
	??_C@_0BA@KFPIBANN@post?9processing?$AA@ (0000)
002528C0 0006:
	??_C@_05GECEPKB@flags?$AA@ (0000)
002528C8 029e:
	??_C@_0CJO@FGMDBDDK@Usage?5controls?5how?5mipmaps?5are?5g@ (0000)
00252B68 0006:
	??_C@_05HNCIIKBA@usage?$AA@ (0000)
00252B70 0550:
	??_C@_0FFA@NKBLAKLE@Format?5controls?5how?5pixels?5will?5@ (0000)
002530C0 0007:
	??_C@_06DLEPGFEF@format?$AA@ (0000)
002530C8 0246:
	??_C@_0CEG@CAJOMHCD@Type?5controls?5bitmap?5?8geometry?8?4@ (0000)
00253310 0005:
	??_C@_04GPMDFGEJ@type?$AA@ (0000)
00253318 0007:
	??_C@_06PCHFJCOP@bitmap?$AA@ (0000)
00253320 0011:
	??_C@_0BB@FKJKINHO@color_plate_data?$AA@ (0000)
00253334 0012:
	??_C@_0BC@EJFOKKAM@bitmap_pixel_data?$AA@ (0000)
00253348 0010:
	??_C@_0BA@HOHDIMNA@double?5multiply?$AA@ (0000)
00253358 000d:
	??_C@_0N@KKNPCJNO@multiply?1min?$AA@ (0000)
00253368 0017:
	??_C@_0BH@PLGFAMGD@blend?1add?1subtract?1max?$AA@ (0000)
00253380 0008:
	??_C@_07NGNIANIE@512x512?$AA@ (0000)
00253388 0008:
	??_C@_07ENHBDHPJ@256x256?$AA@ (0000)
00253390 0008:
	??_C@_07LFCKPLAB@128x128?$AA@ (0000)
00253398 0006:
	??_C@_05PNPIEGAM@64x64?$AA@ (0000)
002533A0 0006:
	??_C@_05HCOEEHHC@32x32?$AA@ (0000)
002533A8 000b:
	??_C@_0L@FEJFGKNE@monochrome?$AA@ (0000)
002533B4 000d:
	??_C@_0N@IHCHFFCJ@32?9bit?5color?$AA@ (0000)
002533C4 000d:
	??_C@_0N@OEHNNLBN@16?9bit?5color?$AA@ (0000)
002533D4 0023:
	??_C@_0CD@OMBPPCAE@compressed?5with?5interpolated?5alp@ (0000)
002533F8 001f:
	??_C@_0BP@DINLCHDL@compressed?5with?5explicit?5alpha?$AA@ (0000)
00253418 0027:
	??_C@_0CH@NDOHNPCP@compressed?5with?5color?9key?5transp@ (0000)
00253440 000b:
	??_C@_0L@KNGGHDGB@vector?5map?$AA@ (0000)
0025344C 000a:
	??_C@_09LLANHIEE@light?5map?$AA@ (0000)
00253458 000b:
	??_C@_0L@PNAKEIEN@detail?5map?$AA@ (0000)
00253464 000b:
	??_C@_0L@CFACPNGJ@height?5map?$AA@ (0000)
00253470 000c:
	??_C@_0M@JIBEDIPA@alpha?9blend?$AA@ (0000)
0025347C 0012:
	??_C@_0BC@LGCCEBL@interface?5bitmaps?$AA@ (0000)
00253490 0008:
	??_C@_07CIPBMLAN@sprites?$AA@ (0000)
00253498 000a:
	??_C@_09GCKGAOCN@cube?5maps?$AA@ (0000)
002534A4 000c:
	??_C@_0M@OCLHNBC@3D?5textures?$AA@ (0000)
002534B0 000c:
	??_C@_0M@JFIODBHN@2D?5textures?$AA@ (0000)
002534BC 0016:
	??_C@_0BG@BOBOGCPF@filthy?5sprite?5bug?5fix?$AA@ (0000)
002534D4 0019:
	??_C@_0BJ@MGIDJLPC@uniform?5sprite?5sequences?$AA@ (0000)
002534F0 001f:
	??_C@_0BP@GKLFCFJP@disable?5height?5map?5compression?$AA@ (0000)
00253510 001b:
	??_C@_0BL@LJMPIHHF@enable?5diffusion?5dithering?$AA@ (0000)
0025352C 0009:
	??_C@_08BBPCEBFO@sprites?$CK?$AA@ (0000)
00253538 000e:
	??_C@_0O@NEMJLKKP@bitmap?5count?$CK?$AA@ (0000)
00253548 0014:
	??_C@_0BE@IAPINFEM@first?5bitmap?5index?$CK?$AA@ (0000)
0025355C 0006:
	??_C@_05HLPJDIEE@name?$FO?$AA@ (0000)
00253564 001c:
	??_C@_0BM@FLKAGKOE@bitmap_group_sequence_block?$AA@ (0000)
00253580 0008:
	??_C@_07KFCNMBIK@bottom?$CK?$AA@ (0000)
00253588 0005:
	??_C@_04GNFAANMA@top?$CK?$AA@ (0000)
00253590 0007:
	??_C@_06NPIMMHAE@right?$CK?$AA@ (0000)
00253598 0006:
	??_C@_05ECAHMNDL@left?$CK?$AA@ (0000)
002535A0 000e:
	??_C@_0O@DIHOPGBD@bitmap?5index?$CK?$AA@ (0000)
002535B0 001a:
	??_C@_0BK@HODJICEH@bitmap_group_sprite_block?$AA@ (0000)
002535CC 000f:
	??_C@_0P@CIKFOMOO@pixels?5offset?$CK?$AA@ (0000)
002535DC 000e:
	??_C@_0O@IBFLDFCG@mipmap?5count?$CK?$AA@ (0000)
002535EC 0014:
	??_C@_0BE@DBBAACBH@registration?5point?$CK?$AA@ (0000)
00253600 0007:
	??_C@_06MOLMBNBJ@flags?$CK?$AA@ (0000)
00253608 0039:
	??_C@_0DJ@DPDJDOCI@format?$CK?$CDdetermines?5how?5pixels?5ar@ (0000)
00253644 0023:
	??_C@_0CD@BFJPHNAO@type?$CK?$CDdetermines?5bitmap?5?8geometr@ (0000)
00253668 0037:
	??_C@_0DH@NPAEDGFL@depth?$CK?3pixels?$CDdepth?5is?51?5for?52D?5@ (0000)
002536A0 000f:
	??_C@_0P@CNDIOOOJ@height?$CK?3pixels?$AA@ (0000)
002536B0 000e:
	??_C@_0O@MPNLAAPG@width?$CK?3pixels?$AA@ (0000)
002536C0 000b:
	??_C@_0L@EPCIMBHC@signature?$CK?$AA@ (0000)
002536CC 0012:
	??_C@_0BC@DEJOGKGO@bitmap_data_block?$AA@ (0000)
002536E0 0007:
	??_C@_06PMDIBCMC@v16u16?$AA@ (0000)
002536E8 0007:
	??_C@_06HPJICMPM@linear?$AA@ (0000)
002536F0 0009:
	??_C@_08EPMFLMOE@swizzled?$AA@ (0000)
002536FC 000b:
	??_C@_0L@CBJKOGIC@palettized?$AA@ (0000)
00253708 000b:
	??_C@_0L@NHGFHLFK@compressed?$AA@ (0000)
00253714 0018:
	??_C@_0BI@DGLMAALD@power?5of?5two?5dimensions?$AA@ (0000)
0025372C 0008:
	??_C@_07MGBABJNN@p8?9bump?$AA@ (0000)
00253734 0005:
	??_C@_04KCNFDCG@dxt5?$AA@ (0000)
0025373C 0005:
	??_C@_04FMHHPEKA@dxt3?$AA@ (0000)
00253744 0005:
	??_C@_04GOEBJGCC@dxt1?$AA@ (0000)
0025374C 0008:
	??_C@_07KBGOCIMM@unused5?$AA@ (0000)
00253754 0008:
	??_C@_07LIHFBJIN@unused4?$AA@ (0000)
0025375C 0009:
	??_C@_08NFFJKKBK@a8r8g8b8?$AA@ (0000)
00253768 0009:
	??_C@_08NFDKFODA@x8r8g8b8?$AA@ (0000)
00253774 0009:
	??_C@_08BDFHIPJK@a4r4g4b4?$AA@ (0000)
00253780 0009:
	??_C@_08DBEMAEHP@a1r5g5b5?$AA@ (0000)
0025378C 0008:
	??_C@_07PHDEIPEK@unused3?$AA@ (0000)
00253794 0007:
	??_C@_06FKODKGEF@r5g6b5?$AA@ (0000)
0025379C 0008:
	??_C@_07OOCPLOAL@unused2?$AA@ (0000)
002537A4 0008:
	??_C@_07MFACONMI@unused1?$AA@ (0000)
002537AC 0005:
	??_C@_04OEKPGJHF@a8y8?$AA@ (0000)
002537B4 0004:
	??_C@_03BBJOFPKB@ay8?$AA@ (0000)
002537B8 0003:
	??_C@_02EMABMJMJ@y8?$AA@ (0000)
002537BC 0003:
	??_C@_02FODEDLAB@a8?$AA@ (0000)
002537C0 0009:
	??_C@_08LCBPNCNK@cube?5map?$AA@ (0000)
002537CC 000b:
	??_C@_0L@DJPJAAIO@3D?5texture?$AA@ (0000)
002537D8 000b:
	??_C@_0L@PIHHNPEO@2D?5texture?$AA@ (0000)
002537E8 0049:
	??_C@_0EJ@OLILMBBF@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5group?5?8?$CF@ (0000)
00253838 0051:
	??_C@_0FB@JLLAHNBC@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5group?5?8?$CF@ (0000)
0025388C 003f:
	??_C@_0DP@LJKAEFNF@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5group?5?8?$CF@ (0000)
002538D0 004e:
	??_C@_0EO@OAADPGCE@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5group?5?8?$CF@ (0000)
00253920 005e:
	??_C@_0FO@FOOFPLKF@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5group?5?8?$CF@ (0000)
00253980 0023:
	??_C@_0CD@BILOFJCH@bitmap?5group?5?8?$CFs?8?5has?5?$CFd?5sequenc@ (0000)
002539A4 0012:
	??_C@_0BC@BFAICBN@?$CB?$CBMUST?5BE?5FIXED?3?5?$AA@ (0000)
002539B8 0021:
	??_C@_0CB@DBDAHEOF@bitmap?5group?5?8?$CFs?8?5has?5?$CFd?5bitmaps@ (0000)
002539E0 0045:
	??_C@_0EF@JJMMADOM@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5?$CD?$CFd?5of?5g@ (0000)
00253A28 003a:
	??_C@_0DK@IPFAJPOP@?$CB?$CBMUST?5BE?5FIXED?3?5bitmap?5?$CD?$CFd?5of?5g@ (0000)
00253A64 0030:
	??_C@_0DA@OHKFJKNL@?$CD?$CD?$CD?5FATAL_ERROR?5failed?5to?5fix?5bi@ (0000)
00253A94 0024:
	??_C@_0CE@BCHDGFBI@sequence_index?$DO?$DN0?5?$CG?$CG?5frame_index@ (0000)
00253AB8 0026:
	??_C@_0CG@LLBBNHEP@c?3?2halo?2SOURCE?2bitmaps?2bitmap_gr@ (0000)
00253AE0 003c:
	??_C@_0DM@LBADMOEL@?$CD?$CD?$CD?5ERROR?5failed?5to?5add?5bitmap?5t@ (0000)
00253B1C 000b:
	??_C@_0L@CJDGMBAB@new_bitmap?$AA@ (0000)
00253B28 0030:
	??_C@_0DA@KJFOMJIA@?$CD?$CD?$CD?5WARNING?5bitmap?5group?5pixel?5d@ (0000)
00253B58 0011:
	??_C@_0BB@PIAKKBNN@space_between?$DO?$DN0?$AA@ (0000)
00253B70 007e:
	??_C@_0HO@CGGNING@?$CIbyte?$CK?$CJbitmap?9?$DObase_address?5?$CL?5bi@ (0000)
00253BF0 003e:
	??_C@_0DO@OLLHEHHP@?$CIbyte?$CK?$CJbitmap?9?$DObase_address?$DO?$DN?$CIby@ (0000)
00253C30 0019:
	??_C@_0BJ@ICGOMECA@?$CBbitmap?9?$DOhardware_format?$AA@ (0000)
00253C4C 003d:
	??_C@_0DN@DEAHHMKK@skipping?5bitmap?5with?5non?5power?9o@ (0000)
00253C90 0040:
	??_C@_0EA@IKDHKBOI@skipping?5bitmap?5with?5non?9power?9o@ (0000)
00253CD0 0034:
	??_C@_0DE@FKIMOKAO@skipping?5cube?5map?5with?5non?9squar@ (0000)
002DC0D0 0590:
	_global_bitmap_reference (0000)
	_global_bitmap_reference_optional (000c)
	_bitmap_pixel_data (035c)
	_color_plate_data (036c)
	_bitmap_group (0530)
*/

/* ---------- headers */

#include "cseries.h"

#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmaps_internal.h"
#include "cache/cache_files.h"
#include "cseries/errors.h"
#include "tag_files/tag_files.h"

/* ---------- constants */

enum
{
	_bitmap_group_type_cube_maps = 2,
	_bitmap_group_type_sprites = 3,
	_bitmap_group_type_interface_bitmaps = 4,
	_bitmap_format_a8y8 = 3,
	_bitmap_format_dxt1 = 14,
	_bitmap_format_dxt5 = 16,
	_bitmap_format_p8_bump = 17,
	_bitmap_has_power_of_two_dimensions_bit = 0,
	_bitmap_compressed_bit = 1,
	_bitmap_palettized_bit = 2,
	_bitmap_linear_bit = 4,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- declarations that belong in tag_files/tag_groups.h
   These are shared tag-system types, not bitmap_group's own: seven
   tag_field_type constants January's bitmap_fields uses, and struct
   tag_flags_definition and struct tag_group (member lists attested by
   HCEX.pdb).  They are held here because this lane may not commit shared
   cross-lane headers; moving them to tag_groups.h is an owner action and is
   data-inert (VC7 lays .data out by declaration order, not name count).
   ---------- */

enum
{
	_tag_field_tag = 5,
	_tag_field_point2d = 10,
	_tag_field_real = 14,
	_tag_field_real_fraction = 15,
	_tag_field_real_point2d = 16,
	_tag_field_explanation = 42,
	_tag_field_custom = 43,
};

typedef boolean (*postprocess_tag_proc)(
	long tag_index,
	boolean editing);

struct tag_flags_definition
{
	long count;
	char **names;
};

struct tag_group
{
	char *name;
	unsigned long flags;
	unsigned long group_tag;
	unsigned long parent_group_tag;
	short version;
	postprocess_tag_proc postprocess_tag;
	struct tag_block_definition *header_block_definition;
	unsigned long child_group_tags[16];
	short child_count;
};

typedef char tag_group_size_assert[sizeof(struct tag_group) == 0x60 ? 1 : -1];

/* ---------- END OWNER HEADER PREREQUISITE */

/* ---------- prototypes */

boolean postprocess_bitmap(
	struct bitmap_data *bitmap,
	boolean editing);
void delete_bitmap(
	struct tag_block *block,
	long element_index);
static boolean postprocess_bitmap_group(
	long bitmap_group_index,
	boolean editing);

/* ---------- globals */

extern boolean find_all_fucked_up_shit;

struct tag_reference_definition global_bitmap_reference =
{
	0,
	BITMAP_GROUP_TAG,
	NULL,
};

struct tag_reference_definition global_bitmap_reference_optional =
{
	0,
	BITMAP_GROUP_TAG,
	NULL,
};

static char *bitmap_types_strings[3] =
{
	"2D texture",
	"3D texture",
	"cube map",
};

static struct tag_enum_definition bitmap_types =
{
	3,
	bitmap_types_strings,
	NULL,
};

static char *bitmap_formats_strings[18] =
{
	"a8",
	"y8",
	"ay8",
	"a8y8",
	"unused1",
	"unused2",
	"r5g6b5",
	"unused3",
	"a1r5g5b5",
	"a4r4g4b4",
	"x8r8g8b8",
	"a8r8g8b8",
	"unused4",
	"unused5",
	"dxt1",
	"dxt3",
	"dxt5",
	"p8-bump",
};

static struct tag_enum_definition bitmap_formats =
{
	18,
	bitmap_formats_strings,
	NULL,
};

static char *bitmap_flags_strings[6] =
{
	"power of two dimensions",
	"compressed",
	"palettized",
	"swizzled",
	"linear",
	"v16u16",
};

static struct tag_flags_definition bitmap_flags =
{
	6,
	bitmap_flags_strings,
};

static struct tag_field bitmap_data_block_fields[16] =
{
	{ _tag_field_tag, 0, "signature*", NULL },
	{ _tag_field_short_integer, 0, "width*:pixels", NULL },
	{ _tag_field_short_integer, 0, "height*:pixels", NULL },
	{ _tag_field_short_integer, 0, "depth*:pixels#depth is 1 for 2D textures and cube maps", NULL },
	{ _tag_field_enum, 0, "type*#determines bitmap 'geometry'", &bitmap_types },
	{ _tag_field_enum, 0, "format*#determines how pixels are represented internally", &bitmap_formats },
	{ _tag_field_word_flags, 0, "flags*", &bitmap_flags },
	{ _tag_field_point2d, 0, "registration point*", NULL },
	{ _tag_field_short_integer, 0, "mipmap count*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)2 },
	{ _tag_field_long_integer, 0, "pixels offset*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)4 },
	{ _tag_field_pad, 0, NULL, (void *)4 },
	{ _tag_field_pad, 0, NULL, (void *)4 },
	{ _tag_field_pad, 0, NULL, (void *)8 },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition bitmap_data_block =
{
	"bitmap_data_block",
	0,
	2048,
	sizeof(struct bitmap_data),
	NULL,
	bitmap_data_block_fields,
	NULL,
	postprocess_bitmap,
	NULL,
	delete_bitmap,
	NULL,
};

static struct tag_field bitmap_group_sprite_block_fields[9] =
{
	{ _tag_field_short_integer, 0, "bitmap index*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)2 },
	{ _tag_field_pad, 0, NULL, (void *)4 },
	{ _tag_field_real, 0, "left*", NULL },
	{ _tag_field_real, 0, "right*", NULL },
	{ _tag_field_real, 0, "top*", NULL },
	{ _tag_field_real, 0, "bottom*", NULL },
	{ _tag_field_real_point2d, 0, "registration point*", NULL },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition bitmap_group_sprite_block =
{
	"bitmap_group_sprite_block",
	0,
	64,
	sizeof(struct bitmap_group_sprite),
	NULL,
	bitmap_group_sprite_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static struct tag_field bitmap_group_sequence_block_fields[6] =
{
	{ _tag_field_string, 0, "name^", NULL },
	{ _tag_field_short_integer, 0, "first bitmap index*", NULL },
	{ _tag_field_short_integer, 0, "bitmap count*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)16 },
	{ _tag_field_block, 0, "sprites*", &bitmap_group_sprite_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition bitmap_group_sequence_block =
{
	"bitmap_group_sequence_block",
	0,
	256,
	sizeof(struct bitmap_group_sequence),
	NULL,
	bitmap_group_sequence_block_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static char *bitmap_group_flags_strings[4] =
{
	"enable diffusion dithering",
	"disable height map compression",
	"uniform sprite sequences",
	"filthy sprite bug fix",
};

static struct tag_flags_definition bitmap_group_flags =
{
	4,
	bitmap_group_flags_strings,
};

static char *bitmap_group_types_strings[5] =
{
	"2D textures",
	"3D textures",
	"cube maps",
	"sprites",
	"interface bitmaps",
};

static struct tag_enum_definition bitmap_group_types =
{
	5,
	bitmap_group_types_strings,
	NULL,
};

static char *bitmap_group_usages_strings[6] =
{
	"alpha-blend",
	"default",
	"height map",
	"detail map",
	"light map",
	"vector map",
};

static struct tag_enum_definition bitmap_group_usages =
{
	6,
	bitmap_group_usages_strings,
	NULL,
};

static char *bitmap_group_formats_strings[6] =
{
	"compressed with color-key transparency",
	"compressed with explicit alpha",
	"compressed with interpolated alpha",
	"16-bit color",
	"32-bit color",
	"monochrome",
};

static struct tag_enum_definition bitmap_group_formats =
{
	6,
	bitmap_group_formats_strings,
	NULL,
};

static char *bitmap_group_sprite_budgets_strings[5] =
{
	"32x32",
	"64x64",
	"128x128",
	"256x256",
	"512x512",
};

static struct tag_enum_definition bitmap_group_sprite_budgets =
{
	5,
	bitmap_group_sprite_budgets_strings,
	NULL,
};

static char *bitmap_group_sprite_usages_strings[3] =
{
	"blend/add/subtract/max",
	"multiply/min",
	"double multiply",
};

static struct tag_enum_definition bitmap_group_sprite_usages =
{
	3,
	bitmap_group_sprite_usages_strings,
	NULL,
};

struct tag_data_definition bitmap_pixel_data =
{
	"bitmap_pixel_data",
	1,
	0x1000000,
	NULL,
};

struct tag_data_definition color_plate_data =
{
	"color_plate_data",
	1,
	0x1000000,
	NULL,
};

static struct tag_field bitmap_fields[32] =
{
	{ _tag_field_custom, 0, NULL, (void *)'bshw' },
	{ _tag_field_explanation, 0, "type", "Type controls bitmap 'geometry'. All dimensions must be a power of two except for SPRITES and INTERFACE BITMAPS:\n\n* 2D TEXTURES: Ordinary, 2D textures will be generated.\n* 3D TEXTURES: Volume textures will be generated from each sequence of 2D texture 'slices'.\n* CUBE MAPS: Cube maps will be generated from each consecutive set of six 2D textures in each sequence, all faces of a cube map must be square and the same size.\n* SPRITES: Sprite texture pages will be generated.\n* INTERFACE BITMAPS: Similar to 2D TEXTURES, but without mipmaps and without the power of two restriction." },
	{ _tag_field_enum, 0, "type", &bitmap_group_types },
	{ _tag_field_explanation, 0, "format", "Format controls how pixels will be stored internally:\n\n* COMPRESSED WITH COLOR-KEY TRANSPARENCY: DXT1 compression, uses 4 bits per pixel. 4x4 blocks of pixels are reduced to 2 colors and interpolated, alpha channel uses color-key transparency instead of alpha from the plate (all zero-alpha pixels also have zero-color).\n* COMPRESSED WITH EXPLICIT ALPHA: DXT2/3 compression, uses 8 bits per pixel. Same as DXT1 without the color key transparency, alpha channel uses alpha from plate quantized down to 4 bits per pixel.\n* COMPRESSED WITH INTERPOLATED ALPHA: DXT4/5 compression, uses 8 bits per pixel. Same as DXT2/3, except alpha is smoother. Better for smooth alpha gradients, worse for noisy alpha.\n* 16-BIT COLOR: Uses 16 bits per pixel. Depending on the alpha channel, bitmaps are quantized to either r5g6b5 (no alpha), a1r5g5b5 (1-bit alpha), or a4r4g4b4 (>1-bit alpha).\n* 32-BIT COLOR: Uses 32 bits per pixel. Very high quality, can have alpha at no added cost. This format takes up the most memory, however. Bitmap formats are x8r8g8b8 and a8r8g8b.\n* MONOCHROME: Uses either 8 or 16 bits per pixel. Bitmap formats are a8 (alpha), y8 (intensity), ay8 (combined alpha-intensity) and a8y8 (separate alpha-intensity).\n\nNote: Height maps (a.k.a. bump maps) should use 32-bit color; this is internally converted to a palettized format which takes less memory." },
	{ _tag_field_enum, 0, "format", &bitmap_group_formats },
	{ _tag_field_explanation, 0, "usage", "Usage controls how mipmaps are generated:\n\n* ALPHA BLEND: Pixels with zero alpha are ignored in mipmaps, to prevent bleeding the transparent color.\n* DEFAULT: Downsampling works normally, as in Photoshop.\n* HEIGHT MAP: The bitmap (normally grayscale) is a height map which gets converted to a bump map. Uses <bump height> below. Alpha is passed through unmodified.\n* DETAIL MAP: Mipmap color fades to gray, controlled by <detail fade factor> below. Alpha fades to white.\n* LIGHT MAP: Generates no mipmaps. Do not use!\n* VECTOR MAP: Used mostly for special effects; pixels are treated as XYZ vectors and normalized after downsampling. Alpha is passed through unmodified." },
	{ _tag_field_enum, 0, "usage", &bitmap_group_usages },
	{ _tag_field_word_flags, 0, "flags", &bitmap_group_flags },
	{ _tag_field_explanation, 0, "post-processing", "These properties control how mipmaps are post-processed." },
	{ _tag_field_real_fraction, 0, "detail fade factor:[0,1]#0 means fade to gray by last mipmap, 1 means fade to gray by first mipmap", NULL },
	{ _tag_field_real_fraction, 0, "sharpen amount:[0,1]#sharpens mipmap after downsampling", NULL },
	{ _tag_field_real_fraction, 0, "bump height:repeats#the apparent height of the bump map above the triangle it is textured onto, in texture repeats (i.e., 1.0 would be as high as the texture is wide)", NULL },
	{ _tag_field_explanation, 0, "sprite processing", "When creating a sprite group, specify the number and size of textures that the group is allowed to occupy. During importing, you'll receive feedback about how well the alloted space was used." },
	{ _tag_field_enum, 0, "sprite budget size", &bitmap_group_sprite_budgets },
	{ _tag_field_short_integer, 0, "sprite budget count", NULL },
	{ _tag_field_explanation, 0, "color plate", "The original TIFF file used to import the bitmap group." },
	{ _tag_field_short_integer, 0, "color plate width*:pixels", NULL },
	{ _tag_field_short_integer, 0, "color plate height*:pixels", NULL },
	{ _tag_field_data, 0, "compressed color plate data*", &color_plate_data },
	{ _tag_field_explanation, 0, "processed pixel data", "Pixel data after being processed by the tool." },
	{ _tag_field_data, 0, "processed pixel data*", &bitmap_pixel_data },
	{ _tag_field_explanation, 0, "miscellaneous", "" },
	{ _tag_field_real, 0, "blur filter size:[0,10] pixels#blurs the bitmap before generating mipmaps", NULL },
	{ _tag_field_real, 0, "alpha bias:[-1,1]#affects alpha mipmap generation", NULL },
	{ _tag_field_short_integer, 0, "mipmap count:levels#0 defaults to all levels", NULL },
	{ _tag_field_explanation, 0, "...more sprite processing", "Sprite usage controls the background color of sprite plates." },
	{ _tag_field_enum, 0, "sprite usage", &bitmap_group_sprite_usages },
	{ _tag_field_short_integer, 0, "sprite spacing*", NULL },
	{ _tag_field_pad, 0, NULL, (void *)2 },
	{ _tag_field_block, 0, "sequences*", &bitmap_group_sequence_block },
	{ _tag_field_block, 0, "bitmaps*", &bitmap_data_block },
	{ _tag_field_terminator, 0, NULL, NULL },
};

static struct tag_block_definition bitmap_block =
{
	"bitmap",
	0,
	1,
	sizeof(struct bitmap_group),
	NULL,
	bitmap_fields,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

struct tag_group bitmap_group =
{
	"bitmap",
	8,
	BITMAP_GROUP_TAG,
	NONE,
	7,
	postprocess_bitmap_group,
	&bitmap_block,
};

/* ---------- public code */

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index)
{
	struct bitmap_group *group = bitmap_group_get(bitmap_group_index);
	struct bitmap_data *result = NULL;

	if (group && bitmap_index >= 0 && bitmap_index < group->bitmaps.count)
	{
		result = TAG_BLOCK_GET_ELEMENT(
			&group->bitmaps,
			bitmap_index,
			struct bitmap_data);
	}

	return result;
}

struct bitmap_data *bitmap_group_get_bitmap_from_sequence(
	long bitmap_group_index,
	short sequence_index,
	short frame_index)
{
	struct bitmap_data *result = NULL;

	if (bitmap_group_index != NONE)
	{
		short bitmap_index = NONE;
		struct bitmap_group *group;

		match_assert(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
			0x2A6,
			sequence_index>=0 && frame_index>=0);

		group = bitmap_group_get(bitmap_group_index);
		if (group)
		{
			if (group->sequences.count > 0)
			{
				struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
					&group->sequences,
					sequence_index % group->sequences.count,
					struct bitmap_group_sequence);

				if (sequence->bitmap_count > 0)
				{
					bitmap_index = (short)(frame_index % sequence->bitmap_count +
						sequence->first_bitmap_index);
				}
				else if (sequence->sprites.count)
				{
					bitmap_index = TAG_BLOCK_GET_ELEMENT(
						&sequence->sprites,
						frame_index,
						struct bitmap_group_sprite)->bitmap_index;
				}
			}

			if (bitmap_index == NONE)
				bitmap_index = frame_index;

			if (bitmap_index >= 0 && bitmap_index < group->bitmaps.count)
			{
				result = TAG_BLOCK_GET_ELEMENT(
					&group->bitmaps,
					bitmap_index,
					struct bitmap_data);
			}
		}
	}

	return result;
}

short bitmap_group_add_bitmap(
	struct bitmap_group *group,
	short width,
	short height,
	short depth,
	short type,
	short format,
	short mipmap_count)
{
	struct bitmap_data new_bitmap_data;
	long pixels_end = 0;
	long previous_count;
	long pixel_data_size;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c", 0x2DB, group);

	new_bitmap_data.type = type;
	new_bitmap_data.flags = 0;
	new_bitmap_data.registration_point.y = 0;
	new_bitmap_data.registration_point.x = 0;
	new_bitmap_data.mipmap_count = mipmap_count;
	new_bitmap_data.pixels_offset = 0;
	new_bitmap_data.hardware_format = NULL;
	new_bitmap_data.base_address = NULL;
	new_bitmap_data.signature = BITMAP_GROUP_TAG;
	new_bitmap_data.width = width;
	new_bitmap_data.height = height;
	new_bitmap_data.depth = depth;
	new_bitmap_data.format = format;

	if (group->type == _bitmap_group_type_interface_bitmaps)
	{
		SET_FLAG(new_bitmap_data.flags, _bitmap_linear_bit, TRUE);
	}
	else if ((width & (width - 1)) ||
		(height & (height - 1)) ||
		(depth & (depth - 1)))
	{
		fprintf(
			stdout,
			"skipping bitmap with non-power-of-two dimensions (#%dx#%d#%d)\r\n",
			width,
			height,
			depth);
		fflush(stdout);
		return NONE;
	}
	else if (group->type == _bitmap_group_type_cube_maps && width != height)
	{
		fprintf(
			stdout,
			"skipping cube map with non-square faces (#%dx#%d)\r\n",
			width,
			height);
		fflush(stdout);
		return NONE;
	}
	else
	{
		SET_FLAG(
			new_bitmap_data.flags,
			_bitmap_has_power_of_two_dimensions_bit,
			TRUE);
	}

	if (format >= _bitmap_format_dxt1 && format <= _bitmap_format_dxt5)
	{
		SET_FLAG(new_bitmap_data.flags, _bitmap_compressed_bit, TRUE);
	}
	if (format == _bitmap_format_p8_bump)
	{
		SET_FLAG(new_bitmap_data.flags, _bitmap_palettized_bit, TRUE);
	}

	/* January repeats these validation guards after assigning format flags. */
	if (group->type == _bitmap_group_type_cube_maps && width != height)
	{
		fprintf(
			stdout,
			"skipping cube map with non-square faces (#%dx#%d)\r\n",
			width,
			height);
		fflush(stdout);
		return NONE;
	}
	if (!TEST_FLAG(
		new_bitmap_data.flags,
		_bitmap_has_power_of_two_dimensions_bit) &&
		group->type != _bitmap_group_type_interface_bitmaps)
	{
		fprintf(
			stdout,
			"skipping bitmap with non power-of-two dimensions (#%dx#%d)\r\n",
			width,
			height);
		fflush(stdout);
		return NONE;
	}

	previous_count = group->bitmaps.count;
	pixel_data_size = bitmap_get_pixel_data_size(&new_bitmap_data);
	if (tag_block_resize(&group->bitmaps, group->bitmaps.count + 1) &&
		tag_data_resize(&group->pixel_data, group->pixel_data.size + pixel_data_size))
	{
		struct bitmap_data *previous_bitmap = NULL;
		short bitmap_index = 0;

		for (;
			bitmap_index < group->bitmaps.count;
			bitmap_index = (short)(bitmap_index + 1))
		{
			struct bitmap_data *bitmap = TAG_BLOCK_GET_ELEMENT(
				&group->bitmaps,
				bitmap_index,
				struct bitmap_data);

			if (bitmap->base_address)
			{
				long space_between;

				match_assert(
					"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
					0x34D,
					!bitmap->hardware_format);
				bitmap->base_address =
					(byte *)group->pixel_data.address + bitmap->pixels_offset;
				match_assert(
					"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
					0x352,
					(byte*)bitmap->base_address>=(byte*)group->pixel_data.address);
				match_assert(
					"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
					0x354,
					(byte*)bitmap->base_address + bitmap_get_pixel_data_size(bitmap) <= (byte*)group->pixel_data.address + group->pixel_data.size);

				if (previous_bitmap)
				{
					space_between = bitmap->pixels_offset -
						previous_bitmap->pixels_offset -
						bitmap_get_pixel_data_size(previous_bitmap);
					match_assert(
						"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
						0x35B,
						space_between>=0);
					if (space_between != 0)
					{
						error(
							_error_silent,
							"### WARNING bitmap group pixel data isn't tight");
					}
				}

				previous_bitmap = bitmap;
				pixels_end = bitmap_get_pixel_data_size(bitmap) +
					bitmap->pixels_offset;
			}
		}

		{
			struct bitmap_data *new_bitmap = TAG_BLOCK_GET_ELEMENT(
				&group->bitmaps,
				previous_count,
				struct bitmap_data);

			match_assert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_group.c",
				0x371,
				new_bitmap);
			csmemcpy(new_bitmap, &new_bitmap_data, sizeof(new_bitmap_data));
			new_bitmap->pixels_offset = pixels_end;
			new_bitmap->base_address = (byte *)group->pixel_data.address + pixels_end;
			csmemset(new_bitmap->base_address, 0, pixel_data_size);
		}

		return (short)previous_count;
	}

	error(
		_error_silent,
		"### ERROR failed to add bitmap to group (tag resize failed)");
	tag_block_resize(&group->bitmaps, previous_count);
	return NONE;
}

/* ---------- private code */

boolean postprocess_bitmap(
	struct bitmap_data *bitmap,
	boolean editing)
{
	return TRUE;
}

void delete_bitmap(
	struct tag_block *block,
	long element_index)
{
	bitmap_delete(TAG_BLOCK_GET_ELEMENT(block, element_index, struct bitmap_data));
	return;
}

static boolean postprocess_bitmap_group(
	long bitmap_group_index,
	boolean editing)
{
	struct bitmap_group *group = bitmap_group_get(bitmap_group_index);
	boolean result = TRUE;
	short bitmap_index;
	short sequence_index;

	for (bitmap_index = 0;
		bitmap_index < group->bitmaps.count;
		bitmap_index = (short)(bitmap_index + 1))
	{
		struct bitmap_data *bitmap = TAG_BLOCK_GET_ELEMENT(
			&group->bitmaps,
			bitmap_index,
			struct bitmap_data);

		if (group->type == _bitmap_group_type_interface_bitmaps)
			SET_FLAG(bitmap->flags, _bitmap_linear_bit, TRUE);

		if (bitmap_verify(bitmap, FALSE))
			texture_cache_bitmap_new(bitmap_group_index, bitmap);
		else
			result = FALSE;
	}

	for (sequence_index = 0;
		sequence_index < group->sequences.count;
		sequence_index = (short)(sequence_index + 1))
	{
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&group->sequences,
			sequence_index,
			struct bitmap_group_sequence);

		if (sequence_index < group->sequences.count - 1)
		{
			(void)TAG_BLOCK_GET_ELEMENT(
				&group->sequences,
				sequence_index + 1,
				struct bitmap_group_sequence);
		}

		if (group->type == _bitmap_group_type_sprites &&
			(sequence->first_bitmap_index || sequence->bitmap_count))
		{
			TAG_BLOCK_GET_ELEMENT(
				&group->sequences,
				sequence_index,
				struct bitmap_group_sequence)->first_bitmap_index = 0;
			TAG_BLOCK_GET_ELEMENT(
				&group->sequences,
				sequence_index,
				struct bitmap_group_sequence)->bitmap_count = 0;
		}
	}

	if (group->sequences.count > 0)
	{
		struct bitmap_group_sequence *last_sequence = TAG_BLOCK_GET_ELEMENT(
			&group->sequences,
			group->sequences.count - 1,
			struct bitmap_group_sequence);

		if (!last_sequence->bitmap_count && !last_sequence->sprites.count &&
			!tag_block_resize(&group->sequences, group->sequences.count - 1))
		{
			error(
				_error_immediate,
				"### FATAL_ERROR failed to fix bitmap group '%s'",
				tag_get_name(bitmap_group_index));
			result = FALSE;
		}
	}

	if (find_all_fucked_up_shit)
	{
		for (bitmap_index = 0;
			bitmap_index < group->bitmaps.count;
			bitmap_index = (short)(bitmap_index + 1))
		{
			struct bitmap_data *bitmap = TAG_BLOCK_GET_ELEMENT(
				&group->bitmaps,
				bitmap_index,
				struct bitmap_data);

			if (bitmap->format == _bitmap_format_a8y8)
			{
				error(
					_error_silent,
					"!!MUST BE FIXED: bitmap #%d of group '%s' has a8y8 format",
					bitmap_index,
					tag_get_name(bitmap_group_index));
			}
			if (TEST_FLAG(bitmap->flags, _bitmap_linear_bit) &&
				!(bitmap->width & (bitmap->width - 1)) &&
				!(bitmap->height & (bitmap->height - 1)))
			{
				error(
					_error_silent,
					"!!MUST BE FIXED: bitmap #%d of group '%s' is linear and power-of-two",
					bitmap_index,
					tag_get_name(bitmap_group_index));
			}
		}

		if (group->bitmaps.count < 1)
		{
			error(
				_error_silent,
				"!!MUST BE FIXED: ",
				"bitmap group '%s' has %d bitmaps",
				tag_get_name(bitmap_group_index),
				group->bitmaps.count);
		}
		if (group->sequences.count < 1)
		{
			error(
				_error_silent,
				"!!MUST BE FIXED: ",
				"bitmap group '%s' has %d sequences",
				tag_get_name(bitmap_group_index),
				group->sequences.count);
		}

		for (sequence_index = 0;
			sequence_index < group->sequences.count;
			sequence_index = (short)(sequence_index + 1))
		{
			struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
				&group->sequences,
				sequence_index,
				struct bitmap_group_sequence);
			struct bitmap_group_sequence *next_sequence;
			short sprite_index;

			if (sequence_index < group->sequences.count - 1)
			{
				next_sequence = TAG_BLOCK_GET_ELEMENT(
					&group->sequences,
					sequence_index + 1,
					struct bitmap_group_sequence);
			}
			else
			{
				next_sequence = NULL;
			}

			if (group->type == _bitmap_group_type_sprites)
			{
				if (sequence->first_bitmap_index || sequence->bitmap_count)
				{
					error(
						_error_silent,
						"!!MUST BE FIXED: bitmap group '%s' (type=%d) sequence #%d doesn't know it's a sprite sequence",
						tag_get_name(bitmap_group_index),
						group->type,
						sequence_index);
				}
			}
			else if (sequence->first_bitmap_index < 0 ||
				sequence->first_bitmap_index >= group->bitmaps.count ||
				sequence->bitmap_count < 1 ||
				sequence->first_bitmap_index + sequence->bitmap_count > group->bitmaps.count ||
				(sequence_index == 0 && sequence->first_bitmap_index != 0) ||
				(next_sequence && next_sequence->first_bitmap_index !=
					sequence->first_bitmap_index + sequence->bitmap_count))
			{
				error(
					_error_silent,
					"!!MUST BE FIXED: bitmap group '%s' sequence #%d references bitmaps [#%d..#%d]",
					tag_get_name(bitmap_group_index),
					sequence_index,
					sequence->first_bitmap_index,
					sequence->first_bitmap_index + sequence->bitmap_count);
			}

			if (group->type == _bitmap_group_type_sprites)
			{
				if (sequence->sprites.count < 1)
				{
					error(
						_error_silent,
						"!!MUST BE FIXED: bitmap group '%s' sequence #%d has %d sprites",
						tag_get_name(bitmap_group_index),
						sequence_index,
						sequence->sprites.count);
				}
				else
				{
					sprite_index = 0;
					for (;
						sprite_index < sequence->sprites.count;
						sprite_index = (short)(sprite_index + 1))
					{
						short sprite_bitmap_index = TAG_BLOCK_GET_ELEMENT(
							&sequence->sprites,
							sprite_index,
							struct bitmap_group_sprite)->bitmap_index;

						if (sprite_bitmap_index < 0 ||
							sprite_bitmap_index >= group->bitmaps.count)
						{
							error(
								_error_silent,
								"!!MUST BE FIXED: bitmap group '%s' sequence #%d sprite #%d references bitmap #%d",
								tag_get_name(bitmap_group_index),
								sequence_index,
								sprite_index,
								sprite_bitmap_index);
						}
					}
				}
			}
			else if (sequence->sprites.count > 0)
			{
				error(
					_error_silent,
					"!!MUST BE FIXED: bitmap group '%s' (type=%d) sequence #%d has %d sprites",
					tag_get_name(bitmap_group_index),
					group->type,
					sequence_index,
					sequence->sprites.count);
			}
		}
	}

	return result;
}
