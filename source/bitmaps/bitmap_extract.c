/*
BITMAP_EXTRACT.C

symbols in this file:
00061EA0 0130:
	_extract_initialize (0000)
00061FD0 0120:
	_extract_find_sequence_bounds (0000)
000620F0 0080:
	_extract_warn_about_horizontal_border (0000)
00062170 0340:
	_extract_plateless_cube_map (0000)
000624B0 0190:
	_extract_find_bitmap_bounds (0000)
00062640 0240:
	_extract_get_bitmap_format (0000)
00062880 03b0:
	_extract_pixels_to_mipmap (0000)
00062C30 0210:
	_extract_pixels_from_mipmap (0000)
00062E40 0260:
	_extract_build_texture_pages_by_sequence (0000)
000630A0 0580:
	_extract_build_debug_plate (0000)
00063620 03d0:
	_extract_mipmaps_to_bitmap (0000)
000639F0 02b0:
	_extract_add_bitmap (0000)
00063CA0 01d0:
	_extract_3d_textures (0000)
00063E70 0220:
	_extract_cube_maps (0000)
00064090 0450:
	_extract_sprites (0000)
000644E0 0490:
	_extract_bitmap (0000)
00064970 0110:
	_extract_without_sequences (0000)
00064A80 02d0:
	_extract_sequence (0000)
00064D50 00b0:
	_extract_sequences (0000)
00064E00 02e0:
	_bitmaps_extract (0000)
000650E0 0130:
	_bitmaps_extract_from_plate (0000)
00251290 000e:
	??_C@_0O@IOJAHIHH@top_reference?$AA@ (0000)
002512A0 0028:
	??_C@_0CI@MDNPJHJC@c?3?2halo?2SOURCE?2bitmaps?2bitmap_ex@ (0000)
002512C8 0034:
	??_C@_0DE@MPBEDKDP@?$CD?$CD?$CD?5WARNING?5horizontal?5border?5br@ (0000)
00251300 0045:
	??_C@_0EF@HOBDLMPH@?$CD?$CD?$CD?5ERROR?5extract?3?5plateless?5cub@ (0000)
00251348 0042:
	??_C@_0EC@CNAGFICN@?$CD?$CD?$CD?5ERROR?5extract?3?5can?8t?5handle?5@ (0000)
0025138C 0037:
	??_C@_0DH@JLCLDNBC@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5all@ (0000)
002513C4 001c:
	??_C@_0BM@HDLJMFAA@bitmap_verify?$CIbitmap?0?5TRUE?$CJ?$AA@ (0000)
002513E0 001a:
	??_C@_0BK@OPAPKIME@adjusted_bounds_reference?$AA@ (0000)
002513FC 0007:
	??_C@_06CGBPGGIE@bounds?$AA@ (0000)
00251404 0033:
	??_C@_0DD@OAKGMMJC@?$CD?$CD?$CD?5ERROR?5extract?3?5unsupported?5b@ (0000)
00251438 0024:
	??_C@_0CE@MAGCMLBE@?$CD?$CD?$CD?5ERROR?5unsupported?5bitmap?5for@ (0000)
0025145C 003c:
	??_C@_0DM@JCMHMNJE@?$CBTEST_FLAG?$CIdestination_bitmap?9?$DOf@ (0000)
00251498 005a:
	??_C@_0FK@KAGBNKPE@destination_mipmap_index?$DO?$DN0?5?$CG?$CG?5d@ (0000)
002514F4 002e:
	??_C@_0CO@MKAFAHJI@destination_bitmap?9?$DOtype?$DN?$DNsource@ (0000)
00251524 0029:
	??_C@_0CJ@PMPPCDAH@bitmap_verify?$CIdestination_bitmap@ (0000)
00251550 0054:
	??_C@_0FE@NNBMCEJD@source_bitmap?9?$DOdepth?5?$DN?$DNMAX?$CI1?0?5de@ (0000)
002515A8 0054:
	??_C@_0FE@NIAAMOKD@source_bitmap?9?$DOheight?$DN?$DNMAX?$CI1?0?5de@ (0000)
00251600 0054:
	??_C@_0FE@OGKNFLAG@source_bitmap?9?$DOwidth?5?$DN?$DNMAX?$CI1?0?5de@ (0000)
00251654 0023:
	??_C@_0CD@ECDENEFB@bitmap_verify?$CIsource_bitmap?0?5TRU@ (0000)
00251678 004b:
	??_C@_0EL@CICFBLGI@source_mipmap_index?$DO?$DN0?5?$CG?$CG?5source@ (0000)
002516C4 002e:
	??_C@_0CO@LIKJMDNH@source_bitmap?9?$DOtype?$DN?$DNdestination@ (0000)
002516F4 0024:
	??_C@_0CE@HECEPIHL@bitmap_verify?$CIsource_bitmap?0?5FAL@ (0000)
00251718 004f:
	??_C@_0EP@JCFKDNGM@destination_bitmap?9?$DOdepth?5?$DN?$DNMAX?$CI@ (0000)
00251768 004f:
	??_C@_0EP@KOPMFNGM@destination_bitmap?9?$DOheight?$DN?$DNMAX?$CI@ (0000)
002517B8 004f:
	??_C@_0EP@OEMHGOAI@destination_bitmap?9?$DOwidth?5?$DN?$DNMAX?$CI@ (0000)
00251808 0028:
	??_C@_0CI@JBLFEOIJ@bitmap_verify?$CIdestination_bitmap@ (0000)
00251830 0024:
	??_C@_0CE@LCCMCEAJ@sequence?5spanned?5?$CFd?5texture?5page@ (0000)
00251854 0030:
	??_C@_0DA@BOIOHNMC@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5deb@ (0000)
00251884 0030:
	??_C@_0DA@NMPFGKNI@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5deb@ (0000)
002518B4 002a:
	??_C@_0CK@DMHGHFA@?$CD?$CD?$CD?5ERROR?5failed?5to?5allocate?5deb@ (0000)
002518E0 0022:
	??_C@_0CC@DECIGGNF@?$CD?$CD?$CD?5ERROR?5unsupported?5bitmap?5typ@ (0000)
00251904 0024:
	??_C@_0CE@BAFBIHF@alpha_bias?$DO?$DN?9255?5?$CG?$CG?5alpha_bias?$DM?$DN@ (0000)
00251928 0030:
	??_C@_0DA@KOPAIHGJ@destination_bitmap?9?$DOdepth?$DN?$DNsourc@ (0000)
00251958 0032:
	??_C@_0DC@IECBCEHB@destination_bitmap?9?$DOheight?$DN?$DNsour@ (0000)
0025198C 0030:
	??_C@_0DA@LLNHMAL@destination_bitmap?9?$DOwidth?$DN?$DNsourc@ (0000)
002519BC 0021:
	??_C@_0CB@CLPJEELI@destination_bitmap?9?$DObase_address@ (0000)
002519E0 0013:
	??_C@_0BD@JMEMKNJH@destination_bitmap?$AA@ (0000)
002519F4 0029:
	??_C@_0CJ@PEGGHKHE@bitmap?5created?3?5?$CD?$CFdx?$CD?$CFd?0?5?$CFs?0?5?$CFdK@ (0000)
00251A20 002d:
	??_C@_0CN@MGDDLHDM@bitmap?5created?3?5?$CD?$CFdx?$CD?$CFdx?$CD?$CFd?0?5?$CFs?0@ (0000)
00251A50 0028:
	??_C@_0CI@GPIJKLOM@?$CD?$CD?$CD?5ERROR?5unsupported?5bitmap?5gro@ (0000)
00251A78 0036:
	??_C@_0DG@OFJIKCDA@?$CD?$CD?$CD?5WARNING?5tried?5to?5smooth?5an?5i@ (0000)
00251AB0 002b:
	??_C@_0CL@CJDNFNLC@?$CD?$CD?$CD?5WARNING?5tried?5to?5smooth?5a?5sp@ (0000)
00251ADC 0003:
	??_C@_02PCIJFNDE@?$AN?6?$AA@ (0000)
00251AE0 0038:
	??_C@_0DI@GIJPOGBA@skipping?53D?5texture?5with?5non?5pow@ (0000)
00251B18 002f:
	??_C@_0CP@OCGAOEDI@skipping?53D?5texture?5with?5incompa@ (0000)
00251B48 0035:
	??_C@_0DF@CHABNBMJ@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5cre@ (0000)
00251B80 002b:
	??_C@_0CL@KIGCKIOC@skipping?5cube?5map?5which?5spanned?5@ (0000)
00251BAC 0031:
	??_C@_0DB@JFFHMOLJ@skipping?5cube?5map?5with?5incompati@ (0000)
00251BE0 002d:
	??_C@_0CN@GHMACEEI@skipping?5cube?5map?5with?5less?5than@ (0000)
00251C10 002a:
	??_C@_0CK@MAFGCHJA@skipping?5cube?5map?5with?5non?9squar@ (0000)
00251C3C 0012:
	??_C@_0BC@OIPPCJDD@?$CBtemporary_bitmap?$AA@ (0000)
00251C50 002b:
	??_C@_0CL@MNFLENMO@?$CD?$CD?$CD?5ERROR?5sprite?5budget?5exceeded@ (0000)
00251C7C 001e:
	??_C@_0BO@PEKHFDDE@sprite?5budget?5met?5?$CI?$CF3?40f?$CF?$CF?$CJ?$AN?6?$AA@ (0000)
00251C9C 0023:
	??_C@_0CD@PAPLMEDP@?$CD?$CD?$CD?5WARNING?5no?5sprite?5budget?5set@ (0000)
00251CC0 002e:
	??_C@_0CO@CKBCALDJ@texture?5page?5created?5?$CD?$CFdx?$CD?$CFd?5?$CI?$CF3@ (0000)
00251CF0 0041:
	??_C@_0EB@GJMFGLDP@?$CD?$CD?$CD?5ERROR?5extract_sprite?3?5failed@ (0000)
00251D38 0084:
	??_C@_0IE@MKAIONEP@?$CD?$CD?$CD?5ERROR?5hey?5?9?5don?8t?5even?5try?5i@ (0000)
00251DC0 0044:
	??_C@_0EE@DCBFAMBD@?$CD?$CD?$CD?5ERROR?5one?5or?5more?5sprites?5do@ (0000)
00251E04 0037:
	??_C@_0DH@NFCBCMJC@?$CD?$CD?$CD?5WARNING?5skipped?5a?5bitmap?5whi@ (0000)
00251E3C 0034:
	??_C@_0DE@MGADPMCF@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5add@ (0000)
00251E70 0097:
	??_C@_0JH@OBKFAPGC@?$CD?$CD?$CD?5ERROR?5extract?3?5tried?5to?5extr@ (0000)
00251F08 003b:
	??_C@_0DL@FLDEAMAO@?$CD?$CD?$CD?5ERROR?5interface?1linear?5bitma@ (0000)
00251F48 0054:
	??_C@_0FE@BCCMGDBK@?$DN?$DN?$DO?5?$CB?$CBWARNING?$CB?$CB?5bitmap?5with?5grea@ (0000)
00251FA0 0052:
	??_C@_0FC@GFFGPBON@?$DN?$DN?$DO?5?$CB?$CBWARNING?$CB?$CB?5usage?5set?5to?5alp@ (0000)
00251FF4 0036:
	??_C@_0DG@DKLFGIJN@?$CD?$CD?$CD?5ERROR?5can?8t?5extract?5sprites?5@ (0000)
0025202C 003a:
	??_C@_0DK@PBJHAOGB@?$CD?$CD?$CD?5ERROR?5can?8t?5extract?53D?5textu@ (0000)
00252068 0053:
	??_C@_0FD@OJBFAEHG@?$CD?$CD?$CD?5ERROR?5extract?3?5compressed?5co@ (0000)
002520BC 0023:
	??_C@_0CD@LJHCCFGP@bottom?$DM?$DNextract_data?4plate?9?$DOheig@ (0000)
002520E0 000c:
	??_C@_0M@NNAAFJDK@bottom?$DO?$DNtop?$AA@ (0000)
002520EC 0007:
	??_C@_06JJMFEGFL@top?$DO?$DN0?$AA@ (0000)
002520F4 002f:
	??_C@_0CP@LHGGIDHB@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5all@ (0000)
00252124 0032:
	??_C@_0DC@MGLFJBKA@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5all@ (0000)
00252158 0034:
	??_C@_0DE@BGBBNNDB@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5dec@ (0000)
00252190 0052:
	??_C@_0FC@OEKPABF@decompressed_plate_size?$DN?$DNsizeof?$CI@ (0000)
002521E4 003e:
	??_C@_0DO@EDMJDNFC@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5res@ (0000)
00252224 0033:
	??_C@_0DD@FAIEDAFJ@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5all@ (0000)
00252258 0040:
	??_C@_0EA@PFFINKKO@group?9?$DOusage?5?$DO?$DN0?5?$CG?$CG?5group?9?$DOusage@ (0000)
00252298 0041:
	??_C@_0EB@OKMLMBHL@group?9?$DOformat?$DO?$DN0?5?$CG?$CG?5group?9?$DOforma@ (0000)
002522DC 003d:
	??_C@_0DN@FIMFNAFO@group?9?$DOtype?5?$DO?$DN0?5?$CG?$CG?5group?9?$DOtype?5?$DM@ (0000)
0025231C 0006:
	??_C@_05MGJOOGAJ@group?$AA@ (0000)
00252324 0037:
	??_C@_0DH@BHCBHGEP@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5all@ (0000)
0025235C 0032:
	??_C@_0DC@MDFBMDEG@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5com@ (0000)
00252390 0031:
	??_C@_0DB@KLKAOHKM@?$CD?$CD?$CD?5ERROR?5extract?3?5failed?5to?5rea@ (0000)
002523C4 001b:
	??_C@_0BL@KAAMGFCF@bitmap_verify?$CIplate?0?5TRUE?$CJ?$AA@ (0000)
0031BED4 002c:
	_extract_data (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmap_group_internal.h"
#include "bitmaps/bitmap_drawing.h"
#include "bitmaps/bitmaps.h"
#include "bitmaps/bitmaps_internal.h"
#include "bitmaps/bitmaps_mipmap.h"
#include "bitmaps/bitmaps_quantitize_internal.h"
#include "bitmaps/bitmap_utilities.h"
#include "cache/cache_files.h"
#include "cseries/errors.h"
#include "math/integer_math.h"
#include "memory/data.h"
#include "memory/data_compress.h"
#include "memory/texture_page.h"

/* ---------- constants */

enum
{
	_bitmap_group_type_2d_textures,
	_bitmap_group_type_3d_textures,
	_bitmap_group_type_cube_maps,
	_bitmap_group_type_sprites,
	_bitmap_group_type_interface_bitmaps,
	NUMBER_OF_BITMAP_GROUP_TYPES
};

enum
{
	_bitmap_group_format_compressed_color_key_transparency,
	_bitmap_group_format_compressed_explicit_alpha,
	_bitmap_group_format_compressed_interpolated_alpha,
	_bitmap_group_format_16bit_color,
	_bitmap_group_format_32bit_color,
	_bitmap_group_format_monochrome,
	NUMBER_OF_BITMAP_GROUP_FORMATS
};

enum
{
	_bitmap_group_usage_alpha_blend,
	_bitmap_group_usage_default,
	_bitmap_group_usage_height_map,
	_bitmap_group_usage_detail_map,
	_bitmap_group_usage_light_map,
	_bitmap_group_usage_vector_map,
	NUMBER_OF_BITMAP_GROUP_USAGES
};

enum
{
	_bitmap_format_a8,
	_bitmap_format_y8,
	_bitmap_format_ay8,
	_bitmap_format_a8y8,
	_bitmap_format_unused1,
	_bitmap_format_unused2,
	_bitmap_format_r5g6b5,
	_bitmap_format_unused3,
	_bitmap_format_a1r5g5b5,
	_bitmap_format_a4r4g4b4,
	_bitmap_format_x8r8g8b8,
	_bitmap_format_a8r8g8b8,
	_bitmap_format_unused4,
	_bitmap_format_unused5,
	_bitmap_format_dxt1,
	_bitmap_format_dxt3,
	_bitmap_format_dxt5,
	_bitmap_format_p8_bump,
	NUMBER_OF_BITMAP_FORMATS
};

enum
{
	_bitmap_type_2d,
	_bitmap_type_3d,
	_bitmap_type_cube_map,
};

enum
{
	_bitmap_has_power_of_two_dimensions_bit,
	_bitmap_compressed_bit,
	_bitmap_palettized_bit,
	_bitmap_swizzled_bit,
	_bitmap_linear_bit,
};

enum
{
	_bitmap_group_diffusion_dither_bit,
	_bitmap_group_disable_vector_compression_bit,
	_bitmap_group_uniform_sprite_sequences_bit,
	_bitmap_group_extract_sprites_filthy_bug_fix_bit = 3,
};

enum
{
	_bitmap_group_sprite_budget_32,
	_bitmap_group_sprite_budget_64,
	_bitmap_group_sprite_budget_128,
	_bitmap_group_sprite_budget_256,
	_bitmap_group_sprite_budget_512,
	_bitmap_group_sprite_budget_1024,
};

enum
{
	_bitmap_group_sprite_usage_blend_add_sub_max,
	_bitmap_group_sprite_usage_multiply_min,
	_bitmap_group_sprite_usage_double_multiply,
};

/* ---------- macros */

/* ---------- structures */

struct bitmap_extract_data
{
	struct bitmap_extract_entry *bitmaps;
	short bitmap_count;
	pixel32 top_reference;
	pixel32 bottom_reference;
	pixel32 adjusted_bounds_reference;
	boolean extract_sequences;
	boolean single_sequence;
	struct bitmap_group *group;
	struct bitmap_data *plate;
	long build_debug_plate;
	struct bitmap_group_sequence *sequence;
	short sequence_index;
	short bitmap_index;
};

struct bitmap_extract_entry
{
	struct bitmap_data *bitmap;
	short sequence_index;
	short sprite_index;
	short page_index;
	word unused;
	long page_entry_index;
};

struct bitmap_extract_cube_map_face
{
	short source_x_block;
	short source_y_block;
	short source_x_edge;
	short source_y_edge;
	short source_x_column_delta;
	short source_y_column_delta;
	short source_x_row_delta;
	short source_y_row_delta;
};

typedef char bitmap_extract_entry_size_assert[
	sizeof(struct bitmap_extract_entry) == 0x10 ? 1 : -1];
typedef char bitmap_extract_data_size_assert[
	sizeof(struct bitmap_extract_data) == 0x2C ? 1 : -1];

/* ---------- prototypes */

static void extract_initialize(
	void);
static short extract_find_sequence_bounds(
	short *top_reference);
static void extract_warn_about_horizontal_border(
	short bottom);
static boolean extract_find_bitmap_bounds(
	rectangle2d const *bounds,
	rectangle2d *adjusted_bounds_reference);
static short extract_get_bitmap_format(
	struct bitmap_data *bitmap);
static void extract_pixels_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index);
struct bitmap_data *extract_build_debug_plate(
	struct bitmap_data *bitmap,
	boolean alpha_to_rgb,
	boolean include_mipmaps,
	boolean border);
static boolean extract_sequences(
	void);
static boolean extract_without_sequences(
	void);
static boolean extract_plateless_cube_map(
	struct bitmap_data *bitmap);
static void extract_build_texture_pages_by_sequence(
	struct texture_page **texture_pages,
	short *texture_page_count,
	short minimum_page_size,
	short spacing);
static short extract_add_bitmap(
	struct bitmap_data *bitmap);
static void extract_mipmaps_to_bitmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap);
static boolean extract_3d_textures(
	void);
static boolean extract_cube_maps(
	void);
static boolean extract_sprites(
	void);
static boolean extract_bitmap(
	rectangle2d const *bounds);
static boolean extract_sequence(
	short top,
	short bottom);

/* ---------- globals */

static struct bitmap_extract_data extract_data;

/* ---------- public code */

boolean bitmaps_extract(
	struct bitmap_group *group,
	long build_debug_plate)
{
	unsigned long decompressed_plate_size;
	boolean should_extract_sequences;
	boolean result = TRUE;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c", 0xB4, group);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0xB5,
		group->type >=0 && group->type <NUMBER_OF_BITMAP_GROUP_TYPES);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0xB6,
		group->format>=0 && group->format<NUMBER_OF_BITMAP_GROUP_FORMATS);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0xB7,
		group->usage >=0 && group->usage <NUMBER_OF_BITMAP_GROUP_USAGES);

	extract_data.bitmap_count = 0;
	extract_data.bitmaps = match_malloc(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0xBA,
		0x4000);
	if (!extract_data.bitmaps)
	{
		error(_error_silent, "### ERROR extract: failed to allocate bitmap array");
		result = FALSE;
	}

	if (!result ||
		!tag_block_resize(&group->bitmaps, 0) ||
		!tag_block_resize(&group->sequences, 0) ||
		!tag_data_resize(&group->pixel_data, 0))
	{
		error(_error_silent, "### ERROR extract: failed to resize bitmap group tags to zero");
		result = FALSE;
	}
	else
	{
		extract_data.sequence = NULL;
		extract_data.sequence_index = NONE;
		extract_data.group = group;
		extract_data.plate = bitmap_2d_new(
			group->import_width,
			group->import_height,
			0,
			_bitmap_format_a8r8g8b8);
		if (extract_data.plate)
		{
			decompressed_plate_size = data_decompressed_size(
				group->import_bitmap.address,
				group->import_bitmap.size);
			match_assert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x104,
				decompressed_plate_size==sizeof(pixel32)*group->import_width*group->import_height);
			if (data_decompress(
				group->import_bitmap.address,
				group->import_bitmap.size,
				bitmap_mipmap_address(extract_data.plate, 0),
				&decompressed_plate_size,
				decompressed_plate_size))
			{
				extract_initialize();
				should_extract_sequences = extract_data.extract_sequences;
				extract_data.build_debug_plate = build_debug_plate;
				if (should_extract_sequences)
					result = extract_sequences();
				else
					result = extract_without_sequences();

				bitmap_delete(extract_data.plate);
			}
			else
			{
				error(_error_silent, "### ERROR extract: failed to decompress color plate");
				result = FALSE;
			}
		}
		else
		{
			error(_error_silent, "### ERROR extract: failed to allocate color plate");
			result = FALSE;
		}
	}

	if (result)
	{
		switch (extract_data.group->type)
		{
		case _bitmap_group_type_2d_textures:
		case _bitmap_group_type_interface_bitmaps:
			break;
		case _bitmap_group_type_3d_textures:
			result = extract_3d_textures();
			break;
		case _bitmap_group_type_cube_maps:
			result = extract_cube_maps();
			break;
		case _bitmap_group_type_sprites:
			result = extract_sprites();
			break;
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x137,
				FALSE,
				"### ERROR unsupported bitmap group type");
		}
	}

	if (extract_data.bitmaps)
	{
		match_free(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
			0x13D,
			extract_data.bitmaps);
	}
	return result;
}

boolean bitmaps_extract_from_plate(
	struct bitmap_data *plate,
	struct bitmap_group *group,
	long build_debug_plate)
{
	long compressed_color_plate_size;
	void *compressed_color_plate;

	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c", 0x83, bitmap_verify(plate, TRUE));
	match_assert("c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c", 0x84, group);

	compressed_color_plate_size = bitmap_get_pixel_data_size(plate);
	group->import_width = plate->width;
	group->import_height = plate->height;
	group->import_bitmap.address = match_malloc(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x8A,
		compressed_color_plate_size);
	if (group->import_bitmap.address)
	{
		if (data_compress(
			bitmap_mipmap_address(plate, 0),
			compressed_color_plate_size,
			group->import_bitmap.address,
			&compressed_color_plate_size,
			compressed_color_plate_size))
		{
			compressed_color_plate = match_realloc(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x90,
				group->import_bitmap.address,
				compressed_color_plate_size);
			if (compressed_color_plate)
			{
				group->import_bitmap.address = compressed_color_plate;
				group->import_bitmap.size = compressed_color_plate_size;
				return bitmaps_extract(group, build_debug_plate);
			}

			error(_error_silent, "### ERROR extract: failed to realloc color plate");
			return FALSE;
		}

		error(_error_silent, "### ERROR extract: failed to compress color plate");
		return FALSE;
	}

	error(_error_silent, "### ERROR extract: failed to allocate temporary buffer");
	return FALSE;
}

/* ---------- private code */

static void extract_initialize(
	void)
{
	short x;

	extract_data.extract_sequences = TRUE;
	extract_data.single_sequence = FALSE;
	extract_data.top_reference =
		*(pixel32 *)bitmap_2d_address(extract_data.plate, 0, 0, 0) & 0xFFFFFF;
	extract_data.bottom_reference =
		*(pixel32 *)bitmap_2d_address(extract_data.plate, 1, 0, 0) & 0xFFFFFF;
	extract_data.adjusted_bounds_reference =
		*(pixel32 *)bitmap_2d_address(extract_data.plate, 2, 0, 0) & 0xFFFFFF;

	if (extract_data.adjusted_bounds_reference == extract_data.bottom_reference &&
		extract_data.bottom_reference != 0xFF)
	{
		extract_data.extract_sequences = FALSE;
	}

	if (extract_data.top_reference == extract_data.bottom_reference)
	{
		extract_data.adjusted_bounds_reference = 0xFFFF;
		extract_data.single_sequence = TRUE;
	}

	for (x = 3; x < extract_data.plate->width; x++)
	{
		pixel32 top = *(pixel32 *)bitmap_2d_address(extract_data.plate, x, 0, 0) & 0xFFFFFF;
		pixel32 bottom = *(pixel32 *)bitmap_2d_address(extract_data.plate, x, 1, 0) & 0xFFFFFF;

		if (top != extract_data.top_reference &&
			bottom != extract_data.bottom_reference)
		{
			extract_data.extract_sequences = FALSE;
		}
	}

	if (!extract_data.extract_sequences)
	{
		extract_data.adjusted_bounds_reference = 0xFF000000;
		extract_data.bottom_reference = 0xFF000000;
		extract_data.top_reference = 0xFF000000;
	}
	return;
}

static short extract_find_sequence_bounds(
	short *top_reference)
{
	short bottom;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x1D9,
		top_reference);

	if (extract_data.single_sequence)
	{
		boolean found_sequence = FALSE;

		for (bottom = *top_reference; bottom < extract_data.plate->height; bottom++)
		{
			short x;
			boolean found_sequence_pixel = FALSE;

			for (x = 0; x < extract_data.plate->width; x++)
			{
				pixel32 color = *(pixel32 *)bitmap_2d_address(
					extract_data.plate,
					x,
					bottom,
					0) & 0xFFFFFF;
				if (color != extract_data.top_reference)
					found_sequence_pixel = TRUE;
			}

			if (found_sequence_pixel)
			{
				found_sequence = TRUE;
			}
			else if (found_sequence)
			{
				break;
			}
			else
			{
				*top_reference = bottom + 1;
			}
		}
	}
	else
	{
		boolean found_top_reference = FALSE;

		for (bottom = *top_reference; bottom < extract_data.plate->height; bottom++)
		{
			pixel32 color = *(pixel32 *)bitmap_2d_address(
				extract_data.plate,
				0,
				bottom,
				0) & 0xFFFFFF;

			if (color == extract_data.top_reference)
			{
				found_top_reference = TRUE;
			}
			else if (color == extract_data.bottom_reference && found_top_reference)
			{
				break;
			}
			else
			{
				*top_reference = bottom + 1;
			}
		}
	}

	return bottom;
}

static void extract_warn_about_horizontal_border(
	short bottom)
{
	if (VALID_INDEX(bottom, extract_data.plate->height))
	{
		short x;

		for (x = 0; x < extract_data.plate->width; x++)
		{
			if ((*(pixel32 *)bitmap_2d_address(extract_data.plate, x, bottom, 0) & 0xFFFFFF) !=
				extract_data.bottom_reference)
			{
				fprintf(stdout, "### WARNING horizontal border broken at (#%d,#%d)\r\n", x, bottom);
				fflush(stdout);
				break;
			}
		}
	}
	return;
}

static boolean extract_plateless_cube_map(
	struct bitmap_data *bitmap)
{
	boolean result = TRUE;
	short face_size;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x2C2,
		bitmap_verify(bitmap, TRUE));

	if (!(bitmap->width % 4) &&
		bitmap->height >= 3 * (bitmap->width / 4) &&
		!(bitmap->width & (bitmap->width - 1)))
	{
		face_size = bitmap->width / 4;
		if (extract_data.bitmap_count + 6 <= 0x400)
		{
			struct bitmap_extract_cube_map_face faces[6] =
			{
				{ 0, 1, 1, 0,  0, 1, -1,  0 },
				{ 1, 1, 1, 1, -1, 0,  0, -1 },
				{ 2, 1, 0, 1,  0, -1, 1,  0 },
				{ 3, 1, 0, 0,  1, 0,  0,  1 },
				{ 0, 0, 1, 0,  0, 1, -1,  0 },
				{ 0, 2, 1, 0,  0, 1, -1,  0 },
			};
			short face_index;

			for (face_index = 0; face_index < NUMBEROF(faces); face_index++)
			{
				struct bitmap_extract_entry *entry =
					&extract_data.bitmaps[extract_data.bitmap_count++];

				entry->bitmap = bitmap_2d_new(
					face_size,
					face_size,
					0,
					_bitmap_format_a8r8g8b8);
				if (entry->bitmap)
				{
					short destination_y;

					for (destination_y = 0; destination_y < face_size; destination_y++)
					{
						short source_x =
							faces[face_index].source_x_block * face_size +
							faces[face_index].source_x_edge * (face_size - 1) +
							destination_y * faces[face_index].source_x_row_delta;
						short source_y =
							faces[face_index].source_y_block * face_size +
							faces[face_index].source_y_edge * (face_size - 1) +
							destination_y * faces[face_index].source_y_row_delta;
						short destination_x;

						for (destination_x = 0; destination_x < face_size; destination_x++)
						{
							*(pixel32 *)bitmap_2d_address(
								entry->bitmap,
								destination_x,
								destination_y,
								0) = *(pixel32 *)bitmap_2d_address(
									bitmap,
									source_x,
									source_y,
									0);
							source_x += faces[face_index].source_x_column_delta;
							source_y += faces[face_index].source_y_column_delta;
						}
					}

					entry->sequence_index = extract_data.sequence_index;
					entry->sprite_index = NONE;
					entry->page_index = NONE;
					entry->page_entry_index = NONE;
				}
				else
				{
					error(_error_silent, "### ERROR extract: failed to allocate temporary bitmap");
					result = FALSE;
				}
			}
		}
		else
		{
			error(
				_error_silent,
				"### ERROR extract: can't handle more than (#%d) temporary bitmaps",
				0x400);
			result = FALSE;
		}
	}
	else
	{
		error(
			_error_silent,
			"### ERROR extract: plateless cube map had invalid dimensions #%dx#%d",
			bitmap->width,
			bitmap->height);
		result = FALSE;
	}

	return result;
}

static boolean extract_find_bitmap_bounds(
	rectangle2d const *bounds,
	rectangle2d *adjusted_bounds_reference)
{
	boolean result = FALSE;
	short y;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x3F7,
		bounds);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x3F8,
		adjusted_bounds_reference);

	adjusted_bounds_reference->y0 = SHORT_MAX;
	adjusted_bounds_reference->x0 = SHORT_MAX;
	adjusted_bounds_reference->y1 = SHORT_MIN;
	adjusted_bounds_reference->x1 = SHORT_MIN;

	for (y = bounds->y0; y < bounds->y1; y++)
	{
		short x;

		for (x = bounds->x0; x < bounds->x1; x++)
		{
			if (VALID_INDEX(x, extract_data.plate->width) &&
				VALID_INDEX(y, extract_data.plate->height))
			{
				pixel32 color = *(pixel32 *)bitmap_2d_address(extract_data.plate, x, y, 0);
				pixel32 rgb = color & 0xFFFFFF;
				boolean contains_data = TRUE;

				if (extract_data.extract_sequences)
				{
					if (rgb == extract_data.top_reference ||
						rgb == extract_data.bottom_reference ||
						rgb == extract_data.adjusted_bounds_reference)
					{
						contains_data = FALSE;
					}
					else if (extract_data.group->usage == _bitmap_group_usage_alpha_blend &&
						!(color & 0xFF000000))
					{
						contains_data = FALSE;
					}
				}

				if (contains_data)
				{
					adjusted_bounds_reference->x0 = MIN(x, adjusted_bounds_reference->x0);
					adjusted_bounds_reference->y0 = MIN(y, adjusted_bounds_reference->y0);
					adjusted_bounds_reference->x1 = MAX(x, adjusted_bounds_reference->x1);
					adjusted_bounds_reference->y1 = MAX(y, adjusted_bounds_reference->y1);
					result = TRUE;
				}
			}
		}
	}

	adjusted_bounds_reference->x1++;
	adjusted_bounds_reference->y1++;
	return result;
}

static short extract_get_bitmap_format(
	struct bitmap_data *bitmap)
{
	short format = NONE;
	short alpha_bits = 0;
	short color_bits = 0;
	boolean channels_differ = FALSE;
	pixel32 *pixels;
	pixel32 first_pixel;
	long pixel_count;
	long pixel_index;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x429,
		bitmap_verify(bitmap, TRUE));

	pixels = bitmap_mipmap_address(bitmap, 0);
	first_pixel = *pixels;
	pixel_count = bitmap_get_pixel_count(bitmap);
	for (pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		pixel32 pixel = pixels[pixel_index];

		switch (pixel >> 24)
		{
		case 0:
			if ((first_pixel & 0xFF000000) == 0xFF000000)
				alpha_bits = MAX(alpha_bits, 1);
			break;
		case 0xFF:
			if (!(first_pixel & 0xFF000000))
				alpha_bits = MAX(alpha_bits, 1);
			break;
		default:
			alpha_bits = 8;
			break;
		}

		switch ((pixel >> 16) & 0xFF)
		{
		case 0:
			if ((first_pixel & 0x00FF0000) == 0x00FF0000)
				color_bits = MAX(color_bits, 1);
			break;
		case 0xFF:
			if (!(first_pixel & 0x00FF0000))
				color_bits = MAX(color_bits, 1);
			break;
		default:
			color_bits = 8;
			break;
		}

		if ((pixel >> 24) != ((pixel >> 16) & 0xFF))
			channels_differ = TRUE;
	}

	switch (extract_data.group->format)
	{
	case _bitmap_group_format_compressed_color_key_transparency:
		format = _bitmap_format_dxt1;
		break;
	case _bitmap_group_format_compressed_explicit_alpha:
		format = alpha_bits > 0 ? _bitmap_format_dxt3 : _bitmap_format_dxt1;
		break;
	case _bitmap_group_format_compressed_interpolated_alpha:
		format = alpha_bits > 0 ? _bitmap_format_dxt5 : _bitmap_format_dxt1;
		break;
	case _bitmap_group_format_16bit_color:
		if (alpha_bits == 0)
			format = _bitmap_format_r5g6b5;
		else if (alpha_bits == 1)
			format = _bitmap_format_a1r5g5b5;
		else
			format = _bitmap_format_a4r4g4b4;
		break;
	case _bitmap_group_format_32bit_color:
		format = alpha_bits == 0 ? _bitmap_format_x8r8g8b8 : _bitmap_format_a8r8g8b8;
		break;
	case _bitmap_group_format_monochrome:
		if (alpha_bits == 0)
			format = _bitmap_format_y8;
		else if (color_bits == 0)
			format = _bitmap_format_a8;
		else
			format = channels_differ ? _bitmap_format_a8y8 : _bitmap_format_ay8;
		break;
	default:
		match_vassert(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
			0x466,
			FALSE,
			"### ERROR extract: unsupported bitmap group format");
		break;
	}

	if (extract_data.group->type == _bitmap_group_type_interface_bitmaps)
	{
		switch (format)
		{
		case _bitmap_format_a1r5g5b5:
			format = _bitmap_format_a4r4g4b4;
			break;
		case _bitmap_format_x8r8g8b8:
			format = _bitmap_format_a8r8g8b8;
			break;
		}
	}

	if (extract_data.group->usage == _bitmap_group_usage_height_map ||
		extract_data.group->usage == _bitmap_group_usage_vector_map)
	{
		if (!TEST_FLAG(
			extract_data.group->flags,
			_bitmap_group_disable_vector_compression_bit))
		{
			format = _bitmap_format_p8_bump;
		}
	}

	return format;
}

static void extract_pixels_to_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short destination_mipmap_index)
{
	pixel32 *source_pixels;
	void *destination_pixels;
	long pixel_count;
	long pixel_index;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6A6,
		bitmap_verify(source_bitmap, TRUE));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6A7,
		source_bitmap->width ==MAX(1, destination_bitmap->width >>destination_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6A8,
		source_bitmap->height==MAX(1, destination_bitmap->height>>destination_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6A9,
		source_bitmap->depth ==MAX(1, destination_bitmap->depth >>destination_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6AB,
		bitmap_verify(destination_bitmap, FALSE));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6AC,
		destination_bitmap->type==source_bitmap->type);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6AD,
		destination_mipmap_index>=0 && destination_mipmap_index<=destination_bitmap->mipmap_count);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6AE,
		!TEST_FLAG(destination_bitmap->flags, _bitmap_swizzled_bit));

	if (TEST_FLAG(destination_bitmap->flags, _bitmap_compressed_bit))
	{
		bitmap_compress_to_mipmap(
			source_bitmap,
			destination_bitmap,
			destination_mipmap_index,
			extract_data.extract_sequences ? &extract_data.adjusted_bounds_reference : NULL);
		return;
	}

	source_pixels = bitmap_mipmap_address(source_bitmap, 0);
	destination_pixels = bitmap_mipmap_address(
		destination_bitmap,
		destination_mipmap_index);
	pixel_count = bitmap_get_pixel_count(source_bitmap);
	for (pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		pixel32 pixel = source_pixels[pixel_index];

		switch (destination_bitmap->format)
		{
		case _bitmap_format_r5g6b5:
			((word *)destination_pixels)[pixel_index] = (word)(
				((((pixel >> 16) & 0xFF) >> 3) << 11) |
				((((pixel >> 8) & 0xFF) >> 2) << 5) |
				((pixel & 0xFF) >> 3));
			break;
		case _bitmap_format_a1r5g5b5:
			((word *)destination_pixels)[pixel_index] = (word)(
				(((pixel >> 24) ? 0x80 : 0) << 8) |
				((((pixel >> 16) & 0xFF) >> 3) << 10) |
				((((pixel >> 8) & 0xFF) >> 3) << 5) |
				((pixel & 0xFF) >> 3));
			break;
		case _bitmap_format_a4r4g4b4:
			((word *)destination_pixels)[pixel_index] = (word)(
				(((pixel >> 24) >> 4) << 12) |
				((((pixel >> 16) & 0xFF) >> 4) << 8) |
				((((pixel >> 8) & 0xFF) >> 4) << 4) |
				((pixel & 0xFF) >> 4));
			break;
		case _bitmap_format_x8r8g8b8:
			((pixel32 *)destination_pixels)[pixel_index] = pixel | 0xFF000000;
			break;
		case _bitmap_format_a8r8g8b8:
			((pixel32 *)destination_pixels)[pixel_index] = pixel;
			break;
		case _bitmap_format_a8:
			((byte *)destination_pixels)[pixel_index] = (byte)(pixel >> 24);
			break;
		case _bitmap_format_y8:
		case _bitmap_format_ay8:
			((byte *)destination_pixels)[pixel_index] = (byte)(pixel >> 16);
			break;
		case _bitmap_format_a8y8:
			((word *)destination_pixels)[pixel_index] = (word)(pixel >> 16);
			break;
		case _bitmap_format_p8_bump:
			((byte *)destination_pixels)[pixel_index] =
				(byte)palette_find_closest_match(global_vector_palette, pixel);
			break;
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x6EC,
				FALSE,
				"### ERROR unsupported bitmap format");
			break;
		}
	}

	return;
}

static void extract_pixels_from_mipmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap,
	short source_mipmap_index)
{
	void *source_pixels;
	pixel32 *destination_pixels;
	long pixel_count;
	long pixel_index;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6F9,
		bitmap_verify(destination_bitmap, TRUE));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6FA,
		destination_bitmap->width ==MAX(1, source_bitmap->width >>source_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6FB,
		destination_bitmap->height==MAX(1, source_bitmap->height>>source_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6FC,
		destination_bitmap->depth ==MAX(1, source_bitmap->depth >>source_mipmap_index));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6FE,
		bitmap_verify(source_bitmap, FALSE));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x6FF,
		source_bitmap->type==destination_bitmap->type);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x700,
		source_mipmap_index>=0 && source_mipmap_index<=source_bitmap->mipmap_count);

	if (TEST_FLAG(source_bitmap->flags, _bitmap_compressed_bit))
	{
		bitmap_uncompress_from_mipmap(
			source_bitmap,
			destination_bitmap,
			source_mipmap_index);
		return;
	}

	source_pixels = bitmap_mipmap_address(source_bitmap, source_mipmap_index);
	destination_pixels = bitmap_mipmap_address(destination_bitmap, 0);
	pixel_count = bitmap_get_pixel_count(destination_bitmap);
	for (pixel_index = 0; pixel_index < pixel_count; pixel_index++)
	{
		destination_pixels[pixel_index] = bitmap_format_to_a8r8g8b8(
			source_bitmap->format,
			source_pixels,
			pixel_index);
	}

	return;
}

static void extract_build_texture_pages_by_sequence(
	struct texture_page **texture_pages,
	short *texture_page_count,
	short minimum_page_size,
	short spacing)
{
	short page_count = 0;
	short spanned_page_count = 1;
	short sequence_index;

	for (sequence_index = 0;
		sequence_index < extract_data.group->sequences.count;
		sequence_index++)
	{
		short first_bitmap_index = 0;
		short page_index = 0;
		boolean page_complete;

		do
		{
			struct texture_page *texture_page;
			boolean new_page;
			short bitmap_index;

			if (page_index < page_count)
			{
				texture_page = texture_pages[page_index];
				new_page = FALSE;
			}
			else if (page_count < 32)
			{
				texture_page = NULL;
				new_page = TRUE;
			}
			else
			{
				break;
			}

			if (texture_page)
				texture_page_textures_begin(texture_page);

			page_complete = TRUE;
			for (bitmap_index = first_bitmap_index;
				bitmap_index < extract_data.bitmap_count;
				bitmap_index++)
			{
				struct bitmap_extract_entry *entry = &extract_data.bitmaps[bitmap_index];

				if (entry->sequence_index == sequence_index)
				{
					long page_entry_index;

					if (!texture_page)
					{
						short page_width = MAX(minimum_page_size, entry->bitmap->width);
						short page_height = MAX(minimum_page_size, entry->bitmap->height);

						page_width = (short)MIN(512, ceiling_power2(page_width));
						page_height = (short)MIN(512, ceiling_power2(page_height));
						texture_page = texture_page_new(NULL, page_width, page_height, spacing);
						if (!texture_page)
							break;

						texture_page_textures_begin(texture_page);
						texture_pages[page_count++] = texture_page;
					}

					page_entry_index = texture_page_texture_new(
						texture_page,
						entry->bitmap->width,
						entry->bitmap->height,
						TRUE);
					if (page_entry_index != NONE)
					{
						entry->page_index = page_index;
						entry->page_entry_index = page_entry_index;
					}
					else
					{
						if (new_page)
						{
							spanned_page_count++;
							first_bitmap_index = bitmap_index;
							texture_page_textures_end(texture_page);
						}
						else
						{
							texture_page_textures_cancel(texture_page);
						}

						page_complete = FALSE;
						break;
					}
				}
			}

			if (texture_page && page_complete)
				texture_page_textures_end(texture_page);
			page_index++;
		}
		while (!page_complete);
	}

	for (sequence_index = 0; sequence_index < page_count; sequence_index++)
	{
		struct texture_page *texture_page = texture_pages[sequence_index];
		short width;
		short height;

		do
		{
			width = texture_page->width >> 1;
			height = texture_page->height >> 1;
		}
		while (width >= 32 &&
			height >= 32 &&
			texture_page_resize(texture_page, width, height));
	}

	fprintf(stdout, "sequence spanned %d texture pages\r\n", spanned_page_count);
	fflush(stdout);
	*texture_page_count = page_count;
	return;
}

struct bitmap_data *extract_build_debug_plate(
	struct bitmap_data *bitmap,
	boolean alpha_to_rgb,
	boolean include_mipmaps,
	boolean border)
{
	struct bitmap_data *converted_bitmap = NULL;
	struct bitmap_data *debug_bitmap = NULL;
	short mipmap_index;
	short slice_count;

	switch (bitmap->type)
	{
	case _bitmap_type_2d:
		converted_bitmap = bitmap_2d_new(
			bitmap->width,
			bitmap->height,
			bitmap->mipmap_count,
			_bitmap_format_a8r8g8b8);
		break;
	case _bitmap_type_3d:
		converted_bitmap = bitmap_3d_new(
			bitmap->width,
			bitmap->height,
			bitmap->depth,
			bitmap->mipmap_count,
			_bitmap_format_a8r8g8b8);
		break;
	case _bitmap_type_cube_map:
		converted_bitmap = bitmap_cube_map_new(
			bitmap->width,
			bitmap->mipmap_count,
			_bitmap_format_a8r8g8b8);
		break;
	default:
		match_vassert(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
			0x518,
			FALSE,
			"### ERROR unsupported bitmap type");
		break;
	}

	if (converted_bitmap && converted_bitmap->base_address)
	{
		short debug_width;
		short debug_height;

		for (mipmap_index = 0; mipmap_index <= bitmap->mipmap_count; mipmap_index++)
		{
			struct bitmap_data *mipmap_bitmap = NULL;
			short width = MAX(1, bitmap->width >> mipmap_index);
			short height = MAX(1, bitmap->height >> mipmap_index);
			short depth = MAX(1, bitmap->depth >> mipmap_index);

			switch (bitmap->type)
			{
			case _bitmap_type_2d:
				mipmap_bitmap = bitmap_2d_new(width, height, 0, _bitmap_format_a8r8g8b8);
				break;
			case _bitmap_type_3d:
				mipmap_bitmap = bitmap_3d_new(width, height, depth, 0, _bitmap_format_a8r8g8b8);
				break;
			case _bitmap_type_cube_map:
				mipmap_bitmap = bitmap_cube_map_new(width, 0, _bitmap_format_a8r8g8b8);
				break;
			default:
				match_vassert(
					"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
					0x542,
					FALSE,
					"### ERROR unsupported bitmap type");
				break;
			}

			if (mipmap_bitmap && mipmap_bitmap->base_address)
			{
				extract_pixels_from_mipmap(bitmap, mipmap_bitmap, mipmap_index);
				extract_pixels_to_mipmap(mipmap_bitmap, converted_bitmap, mipmap_index);
			}
			bitmap_delete(mipmap_bitmap);
		}

		switch (converted_bitmap->type)
		{
		case _bitmap_type_2d:
			slice_count = 1;
			break;
		case _bitmap_type_3d:
			slice_count = converted_bitmap->depth;
			break;
		case _bitmap_type_cube_map:
			slice_count = 6;
			break;
		/* slice_count is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x567,
				FALSE,
				"### ERROR unsupported bitmap type");
			break;
		}

		if (border)
		{
			debug_width = (converted_bitmap->width + 3) * slice_count + 3;
			debug_height = converted_bitmap->height + 8;
		}
		else
		{
			debug_width = converted_bitmap->width * slice_count;
			debug_height = converted_bitmap->height;
		}

		if (include_mipmaps)
		{
			for (mipmap_index = 1; mipmap_index <= converted_bitmap->mipmap_count; mipmap_index++)
			{
				debug_height += (converted_bitmap->height >> mipmap_index) + (border ? 4 : 0);
			}
		}

		debug_bitmap = bitmap_2d_new(
			debug_width,
			debug_height,
			0,
			_bitmap_format_a8r8g8b8);
		if (debug_bitmap && debug_bitmap->base_address)
		{
			short destination_y = border ? 4 : 0;
			pixel32 *pixels = bitmap_mipmap_address(debug_bitmap, 0);
			long pixel_count = bitmap_get_pixel_count(debug_bitmap);
			long pixel_index;

			for (pixel_index = 0; pixel_index < pixel_count; pixel_index++)
			{
				pixels[pixel_index] = 0x000000FF;
			}

			for (mipmap_index = 0;
				mipmap_index <= (include_mipmaps ? converted_bitmap->mipmap_count : 0);
				mipmap_index++)
			{
				short destination_x = border ? 3 : 0;
				struct bitmap_data *slice_bitmap = bitmap_2d_new(
					MAX(1, converted_bitmap->width >> mipmap_index),
					MAX(1, converted_bitmap->height >> mipmap_index),
					0,
					_bitmap_format_a8r8g8b8);

				if (slice_bitmap && slice_bitmap->base_address)
				{
					short mipmap_slice_count;
					short slice_index;

					switch (converted_bitmap->type)
					{
					case _bitmap_type_2d:
						mipmap_slice_count = 1;
						break;
					case _bitmap_type_3d:
						mipmap_slice_count = MAX(1, converted_bitmap->depth >> mipmap_index);
						break;
					case _bitmap_type_cube_map:
						mipmap_slice_count = 6;
						break;
					/* mipmap_slice_count is left unassigned only by this default arm. Not reached unassigned: the
					 * arm's assertion failure calls system_exit, which does not return in January
					 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
					 * Source-policy approval pending (2026-09-27 audit). */
					default:
						match_vassert(
							"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
							0x5AC,
							FALSE,
							"### ERROR unsupported bitmap type");
						break;
					}

					for (slice_index = 0; slice_index < mipmap_slice_count; slice_index++)
					{
						point2d destination_point;

						switch (converted_bitmap->type)
						{
						case _bitmap_type_2d:
							csmemcpy(
								bitmap_mipmap_address(slice_bitmap, 0),
								bitmap_mipmap_address(converted_bitmap, mipmap_index),
								bitmap_get_pixel_data_size(slice_bitmap));
							break;
						case _bitmap_type_3d:
							csmemcpy(
								bitmap_mipmap_address(slice_bitmap, 0),
								bitmap_3d_address(converted_bitmap, 0, 0, slice_index, mipmap_index),
								bitmap_get_pixel_data_size(slice_bitmap));
							break;
						case _bitmap_type_cube_map:
							csmemcpy(
								bitmap_mipmap_address(slice_bitmap, 0),
								bitmap_cube_map_address(converted_bitmap, 0, 0, slice_index, mipmap_index),
								bitmap_get_pixel_data_size(slice_bitmap));
							break;
						default:
							match_vassert(
								"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
								0x5C8,
								FALSE,
								"### ERROR unsupported bitmap type");
							break;
						}

						set_point2d(&destination_point, destination_x, destination_y);
						bitmap_copy(
							debug_bitmap,
							&destination_point,
							NULL,
							slice_bitmap,
							NULL,
							0xFFFFFFFF,
							0);
						destination_x += (converted_bitmap->width >> mipmap_index) + (border ? 3 : 0);
					}
				}
				else
				{
					error(_error_silent, "### ERROR failed to allocate debug slice bitmap");
				}

				bitmap_delete(slice_bitmap);
				destination_y += (converted_bitmap->height >> mipmap_index) + (border ? 4 : 0);
			}
		}
		else
		{
			error(_error_silent, "### ERROR failed to allocate debug plate bitmap");
		}
	}
	else
	{
		error(_error_silent, "### ERROR failed to allocate debug bitmap");
	}

	if (debug_bitmap && debug_bitmap->base_address && alpha_to_rgb)
	{
		bitmap_alpha_to_rgb(debug_bitmap);
	}

	bitmap_delete(converted_bitmap);
	return debug_bitmap;
}

static void extract_mipmaps_to_bitmap(
	struct bitmap_data *source_bitmap,
	struct bitmap_data *destination_bitmap)
{
	short mipmap_index;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5F9,
		bitmap_verify(source_bitmap, TRUE));
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FA,
		destination_bitmap);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FB,
		destination_bitmap->base_address);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FC,
		destination_bitmap->type==source_bitmap->type);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FD,
		destination_bitmap->width==source_bitmap->width);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FE,
		destination_bitmap->height==source_bitmap->height);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x5FF,
		destination_bitmap->depth==source_bitmap->depth);

	for (mipmap_index = 0;
		mipmap_index <= destination_bitmap->mipmap_count;
		mipmap_index++)
	{
		boolean ignore_transparent_pixels =
			extract_data.group->usage == _bitmap_group_usage_alpha_blend &&
			destination_bitmap->format != _bitmap_format_y8 &&
			destination_bitmap->format != _bitmap_format_r5g6b5 &&
			destination_bitmap->format != _bitmap_format_x8r8g8b8;
		real mipmap_fraction = (real)mipmap_index / destination_bitmap->mipmap_count;
		short alpha_bias = (short)floor(
			PIN(extract_data.group->alpha_bias, -1.0f, 1.0f) *
			mipmap_fraction * 255.0f + 0.5f);
		struct bitmap_data *mipmap_bitmap;

		match_assert(
			"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
			0x616,
			alpha_bias>=-255 && alpha_bias<=255);

		mipmap_bitmap = bitmap_shrink(
			source_bitmap,
			(short)(1 << mipmap_index),
			alpha_bias,
			ignore_transparent_pixels);
		if (mipmap_bitmap && mipmap_bitmap->base_address)
		{
			bitmap_sharpen(mipmap_bitmap, extract_data.group->sharpen_amount);

			if (extract_data.group->usage == _bitmap_group_usage_detail_map)
			{
				real mipmap_count = destination_bitmap->mipmap_count;
				real detail_fade = extract_data.group->detail_fade;
				real fade_amount = PIN(
					(real)mipmap_index / (mipmap_count * (1.0f - detail_fade) + detail_fade),
					0.0f,
					1.0f);

				bitmap_fade(mipmap_bitmap, 0xFF7F7F7F, fade_amount);
			}

			if (ignore_transparent_pixels)
				bitmap_alpha_bleed(mipmap_bitmap, 1);
			if (extract_data.group->usage == _bitmap_group_usage_height_map)
				bitmap_height_map(mipmap_bitmap, extract_data.group->bump_height);
			if (extract_data.group->usage == _bitmap_group_usage_vector_map)
				bitmap_vector_map(mipmap_bitmap);

			if (TEST_FLAG(
				extract_data.group->flags,
				_bitmap_group_diffusion_dither_bit) &&
				extract_data.group->usage != _bitmap_group_usage_height_map &&
				extract_data.group->usage != _bitmap_group_usage_light_map &&
				extract_data.group->usage != _bitmap_group_usage_vector_map)
			{
				short const *bits_per_channel;

				switch (destination_bitmap->format)
				{
				case _bitmap_format_r5g6b5:
					bits_per_channel = bits_per_channel_r5g6b5;
					break;
				case _bitmap_format_a1r5g5b5:
					bits_per_channel = bits_per_channel_a1r5g5b5;
					break;
				case _bitmap_format_a4r4g4b4:
					bits_per_channel = bits_per_channel_a4r4g4b4;
					break;
				default:
					bits_per_channel = NULL;
					break;
				}

				bitmap_quantitize(mipmap_bitmap, bits_per_channel);
			}

			extract_pixels_to_mipmap(
				mipmap_bitmap,
				destination_bitmap,
				mipmap_index);
			bitmap_delete(mipmap_bitmap);
		}
		else
		{
			error(_error_silent, "### ERROR extract: failed to allocate temporary bitmap");
		}
	}

	if (extract_data.build_debug_plate)
	{
		bitmap_delete(extract_build_debug_plate(destination_bitmap, FALSE, TRUE, TRUE));
	}

	return;
}

static short extract_add_bitmap(
	struct bitmap_data *bitmap)
{
	short format;
	short mipmap_count;
	short bitmap_index;
	struct bitmap_data *destination_bitmap;
	struct bitmap_data *working_bitmap;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x487,
		bitmap_verify(bitmap, TRUE));

	format = extract_get_bitmap_format(bitmap);
	if (extract_data.group->type == _bitmap_group_type_interface_bitmaps ||
		extract_data.group->usage == _bitmap_group_usage_light_map)
	{
		mipmap_count = 0;
	}
	else
	{
		mipmap_count = bitmap_get_max_mipmap_count(bitmap);
		if (extract_data.group->type == _bitmap_group_type_sprites && mipmap_count >= 2)
			mipmap_count = 2;

		if (extract_data.group->mipmap_count > 0)
		{
			mipmap_count = MIN(extract_data.group->mipmap_count - 1, mipmap_count);
		}
	}

	bitmap_index = bitmap_group_add_bitmap(
		extract_data.group,
		bitmap->width,
		bitmap->height,
		bitmap->depth,
		bitmap->type,
		format,
		mipmap_count);
	extract_data.bitmap_index = bitmap_index;
	if (bitmap_index != NONE)
	{
		working_bitmap = bitmap_clone(bitmap);
		destination_bitmap = TAG_BLOCK_GET_ELEMENT(
			&extract_data.group->bitmaps,
			bitmap_index,
			struct bitmap_data);

		if (TEST_FLAG(
			extract_data.group->flags,
			_bitmap_group_extract_sprites_filthy_bug_fix_bit))
		{
			destination_bitmap->registration_point.x =
				(bitmap->registration_point.x + 1) / 2;
			destination_bitmap->registration_point.y =
				(bitmap->registration_point.y + 1) / 2;
		}
		else
		{
			destination_bitmap->registration_point = bitmap->registration_point;
		}

		if (working_bitmap && working_bitmap->base_address)
		{
			if (extract_data.group->smoothing_filter_size > 0.0f)
			{
				switch (extract_data.group->type)
				{
				case _bitmap_group_type_2d_textures:
				case _bitmap_group_type_3d_textures:
				case _bitmap_group_type_cube_maps:
					bitmap_smooth(
						working_bitmap,
						extract_data.group->smoothing_filter_size);
					break;
				case _bitmap_group_type_sprites:
					fprintf(stdout, "### WARNING tried to smooth a sprite group", "\r\n");
					fflush(stdout);
					break;
				case _bitmap_group_type_interface_bitmaps:
					fprintf(stdout, "### WARNING tried to smooth an interface-bitmap group", "\r\n");
					fflush(stdout);
					break;
				default:
					match_vassert(
						"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
						0x4D0,
						FALSE,
						"### ERROR unsupported bitmap group type");
					break;
				}
			}

			extract_mipmaps_to_bitmap(working_bitmap, destination_bitmap);
			if (destination_bitmap->type == _bitmap_type_3d)
			{
				fprintf(
					stdout,
					"bitmap created: #%dx#%dx#%d, %s, %dK-bytes\r\n",
					destination_bitmap->width,
					destination_bitmap->height,
					destination_bitmap->depth,
					bitmap_format_get_string(destination_bitmap->format),
					bitmap_get_pixel_data_size(destination_bitmap) / 1024);
				fflush(stdout);
			}
			else
			{
				fprintf(
					stdout,
					"bitmap created: #%dx#%d, %s, %dK-bytes\r\n",
					destination_bitmap->width,
					destination_bitmap->height,
					bitmap_format_get_string(destination_bitmap->format),
					bitmap_get_pixel_data_size(destination_bitmap) / 1024);
				fflush(stdout);
			}
		}
	}

	return bitmap_index;
}

static boolean extract_bitmap(
	rectangle2d const *bounds)
{
	boolean result = TRUE;
	boolean warned_about_dxt1_alpha = FALSE;
	boolean warned_about_zero_alpha = FALSE;
	rectangle2d adjusted_bounds;
	struct bitmap_data *bitmap;
	short source_y;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x322,
		bounds);

	if (extract_find_bitmap_bounds(bounds, &adjusted_bounds))
	{
		bitmap = bitmap_2d_new(
			adjusted_bounds.x1 - adjusted_bounds.x0,
			adjusted_bounds.y1 - adjusted_bounds.y0,
			0,
			_bitmap_format_a8r8g8b8);
		if (bitmap)
		{
			if (TEST_FLAG(
				extract_data.group->flags,
				_bitmap_group_extract_sprites_filthy_bug_fix_bit))
			{
				bitmap->registration_point.x =
					bounds->x0 + bounds->x1 - 2 * adjusted_bounds.x0;
				bitmap->registration_point.y =
					bounds->y0 + bounds->y1 - 2 * adjusted_bounds.y0;
			}
			else
			{
				bitmap->registration_point.x =
					(bounds->x0 + bounds->x1) / 2 - adjusted_bounds.x0;
				bitmap->registration_point.y =
					(bounds->y0 + bounds->y1) / 2 - adjusted_bounds.y0;
			}

			for (source_y = adjusted_bounds.y0; source_y < adjusted_bounds.y1; source_y++)
			{
				pixel32 *destination = bitmap_2d_address(bitmap, 0, source_y - adjusted_bounds.y0, 0);
				short source_x;

				for (source_x = adjusted_bounds.x0; source_x < adjusted_bounds.x1; source_x++)
				{
					pixel32 color = *(pixel32 *)bitmap_2d_address(
						extract_data.plate,
						source_x,
						source_y,
						0);

					if (extract_data.extract_sequences)
					{
						pixel32 rgb = color & 0xFFFFFF;

						if (rgb == extract_data.top_reference ||
							rgb == extract_data.adjusted_bounds_reference ||
							rgb == extract_data.bottom_reference)
						{
							color = 0;
						}
					}

					if (extract_data.group->usage == _bitmap_group_usage_alpha_blend &&
						!(color & 0xFF000000) &&
						(color & 0xFFFFFF) &&
						!warned_about_zero_alpha)
					{
						fprintf(
							stdout,
							"==> !!WARNING!! usage set to alpha; non-zero color overlaps with zero-alpha <==\r\n");
						fflush(stdout);
						warned_about_zero_alpha = TRUE;
					}

					if (extract_data.group->format ==
						_bitmap_group_format_compressed_color_key_transparency)
					{
						if ((color >> 24) != 0 && (color >> 24) != 0xFF && !warned_about_dxt1_alpha)
						{
							fprintf(
								stdout,
								"==> !!WARNING!! bitmap with greater than 1-bit alpha being compressed as DXT1 <==\r\n");
							fflush(stdout);
							warned_about_dxt1_alpha = TRUE;
						}

						if (!(color >> 24) &&
							((color & 0xFFFFFF) == extract_data.adjusted_bounds_reference ||
							!extract_data.extract_sequences))
						{
							color = 0;
						}
						else
						{
							color |= 0xFF000000;
						}
					}

					if ((extract_data.group->format ==
							_bitmap_group_format_compressed_color_key_transparency ||
						extract_data.group->format ==
							_bitmap_group_format_compressed_explicit_alpha ||
						extract_data.group->format ==
							_bitmap_group_format_compressed_interpolated_alpha) &&
						extract_data.group->type == _bitmap_group_type_interface_bitmaps)
					{
						error(
							_error_immediate,
							"### ERROR interface/linear bitmap cannot be DXT-compressed");
					}

					destination[source_x - adjusted_bounds.x0] = color;
				}
			}

			if (extract_data.group->type == _bitmap_group_type_2d_textures ||
				extract_data.group->type == _bitmap_group_type_interface_bitmaps)
			{
				short bitmap_index = extract_add_bitmap(bitmap);

				if (bitmap_index != NONE)
				{
					if (extract_data.sequence->first_bitmap_index == NONE)
					{
						extract_data.sequence->first_bitmap_index = bitmap_index;
						extract_data.sequence->bitmap_count = 0;
					}

					extract_data.sequence->bitmap_count++;
				}

				bitmap_delete(bitmap);
			}
			else if (!extract_data.extract_sequences)
			{
				if (extract_data.group->type == _bitmap_group_type_cube_maps)
				{
					extract_plateless_cube_map(bitmap);
				}
				else
				{
					error(
						_error_silent,
						"### ERROR extract: tried to extract non-2d textures without a valid place but they weren't cube maps and/or EXTRACT_PLATELESS_CUBE_MAPS aren't allowed");
					result = FALSE;
				}

				bitmap_delete(bitmap);
			}
			else if (extract_data.bitmap_count < 0x400)
			{
				struct bitmap_extract_entry *entry =
					&extract_data.bitmaps[extract_data.bitmap_count++];

				entry->bitmap = bitmap;
				entry->sequence_index = extract_data.sequence_index;
				entry->sprite_index = NONE;
				entry->page_index = NONE;
				entry->page_entry_index = NONE;

				if (extract_data.group->type == _bitmap_group_type_sprites)
				{
					short sprite_index =
						(short)tag_block_add_element(&extract_data.sequence->sprites);

					if (sprite_index != NONE)
					{
						struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
							&extract_data.sequence->sprites,
							sprite_index,
							struct bitmap_group_sprite);

						sprite->bitmap_index = NONE;
						if (TEST_FLAG(
							extract_data.group->flags,
							_bitmap_group_extract_sprites_filthy_bug_fix_bit))
						{
							sprite->registration_point.x =
								(real)bitmap->registration_point.x * 0.5f;
							sprite->registration_point.y =
								(real)bitmap->registration_point.y * 0.5f;
						}
						else
						{
							sprite->registration_point.x = (real)bitmap->registration_point.x;
							sprite->registration_point.y = (real)bitmap->registration_point.y;
						}
						entry->sprite_index = sprite_index;
					}
					else
					{
						error(_error_silent, "### ERROR extract: failed to add sprite to sequence");
						result = FALSE;
					}
				}
			}
			else
			{
				error(
					_error_silent,
					"### ERROR extract: can't handle more than (#%d) temporary bitmaps",
					0x400);
				result = FALSE;
			}
		}
		else
		{
			error(_error_silent, "### ERROR extract: failed to allocate temporary bitmap");
			result = FALSE;
		}
	}
	else
	{
		fprintf(stdout, "### WARNING skipped a bitmap which contained no data\r\n");
		fflush(stdout);
	}

	return result;
}

static boolean extract_3d_textures(
	void)
{
	boolean result = TRUE;
	short first_bitmap_index = 0;

	while (result && first_bitmap_index < extract_data.bitmap_count)
	{
		struct bitmap_extract_entry *first_entry = &extract_data.bitmaps[first_bitmap_index];
		short sequence_index = first_entry->sequence_index;
		short width = first_entry->bitmap->width;
		short height = first_entry->bitmap->height;
		short bitmap_count = 0;
		boolean incompatible_dimensions = FALSE;

		// BUG (preserved): January does not bound the final run before reading its next sequence index.
		while (!incompatible_dimensions &&
			extract_data.bitmaps[first_bitmap_index + bitmap_count].sequence_index == sequence_index)
		{
			struct bitmap_data *bitmap =
				extract_data.bitmaps[first_bitmap_index + bitmap_count].bitmap;

			if (bitmap->width != width || bitmap->height != height)
				incompatible_dimensions = TRUE;

			bitmap_count++;
		}

		if (incompatible_dimensions)
		{
			fprintf(stdout, "skipping 3D texture with incompatible slices\r\n");
			fflush(stdout);
		}
		else if (bitmap_count & (bitmap_count - 1))
		{
			fprintf(stdout, "skipping 3D texture with non power-of-two slice count\r\n");
			fflush(stdout);
		}
		else
		{
			struct bitmap_data *bitmap =
				bitmap_3d_new(width, height, bitmap_count, 0, _bitmap_format_a8r8g8b8);

			if (bitmap && bitmap->base_address)
			{
				short slice_index;
				short bitmap_index;

				for (slice_index = 0; slice_index < bitmap_count; slice_index++)
				{
					bitmap_3d_slice_insert(
						extract_data.bitmaps[first_bitmap_index + slice_index].bitmap,
						bitmap,
						0,
						slice_index);
				}

				extract_data.sequence_index = sequence_index;
				bitmap_index = extract_add_bitmap(bitmap);
				if (bitmap_index != NONE)
				{
					struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
						&extract_data.group->sequences,
						sequence_index,
						struct bitmap_group_sequence);

					if (sequence->first_bitmap_index == NONE)
					{
						sequence->first_bitmap_index = bitmap_index;
						sequence->bitmap_count = 1;
					}
					else
					{
						sequence->bitmap_count++;
					}
				}
			}
			else
			{
				error(_error_silent, "### ERROR extract: failed to allocate temporary bitmap");
				result = FALSE;
			}

			bitmap_delete(bitmap);
		}

		first_bitmap_index += bitmap_count;
	}

	return result;
}

static boolean extract_cube_maps(
	void)
{
	boolean result = TRUE;
	struct bitmap_data *temporary_bitmap = NULL;
	short face_index = 0;
	short sequence_index;
	short bitmap_index;

	for (bitmap_index = 0; result && bitmap_index < extract_data.bitmap_count; bitmap_index++)
	{
		struct bitmap_extract_entry *entry = &extract_data.bitmaps[bitmap_index];
		boolean skip_cube_map = FALSE;

		if (face_index == 0)
		{
			match_assert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x798,
				!temporary_bitmap);

			if (entry->bitmap->width == entry->bitmap->height)
			{
				temporary_bitmap = bitmap_cube_map_new(
					entry->bitmap->width,
					0,
					_bitmap_format_a8r8g8b8);
				sequence_index = entry->sequence_index;
			}
			else
			{
				fprintf(stdout, "skipping cube map with non-square faces\r\n");
				fflush(stdout);
				skip_cube_map = TRUE;
			}
		}

		if (temporary_bitmap && temporary_bitmap->base_address)
		{
			if (entry->sequence_index == sequence_index)
			{
				if (entry->bitmap->width == temporary_bitmap->width &&
					entry->bitmap->height == temporary_bitmap->height)
				{
					bitmap_cube_map_face_insert(
						entry->bitmap,
						temporary_bitmap,
						0,
						face_index);
					face_index++;
				}
				else
				{
					fprintf(stdout, "skipping cube map with incompatible-size faces\r\n");
					fflush(stdout);
					skip_cube_map = TRUE;
				}
			}
			else
			{
				fprintf(stdout, "skipping cube map which spanned sequence\r\n");
				fflush(stdout);
				skip_cube_map = TRUE;
			}

			if (skip_cube_map)
			{
				/* BUG (original): January frees the partial cube map without clearing temporary_bitmap,
				so the !temporary_bitmap assert fires at the next face-0 entry. */
				bitmap_delete(temporary_bitmap);
				face_index = 0;
			}
		}
		else if (!skip_cube_map)
		{
			error(_error_silent, "### ERROR extract: failed to create temporary bitmap");
			result = FALSE;
		}

		if (result && face_index == 6)
		{
			short group_bitmap_index = extract_add_bitmap(temporary_bitmap);

			if (group_bitmap_index != NONE)
			{
				struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
					&extract_data.group->sequences,
					sequence_index,
					struct bitmap_group_sequence);

				if (sequence->first_bitmap_index == NONE)
				{
					sequence->first_bitmap_index = group_bitmap_index;
					sequence->bitmap_count = 0;
				}
				sequence->bitmap_count++;
			}

			bitmap_delete(temporary_bitmap);
			temporary_bitmap = NULL;
			face_index = 0;
		}

		bitmap_delete(entry->bitmap);
	}

	if (temporary_bitmap)
	{
		if (result)
		{
			fprintf(stdout, "skipping cube map with less than six faces\r\n");
			fflush(stdout);
		}
		bitmap_delete(temporary_bitmap);
	}

	return result;
}

static boolean extract_sprites(
	void)
{
	boolean result = TRUE;
	long total_page_pixel_count = 0;
	short sprite_page_count = extract_data.group->sprite_budget_count;
	short spacing = extract_data.group->mipmap_count == 1 ? 1 : 4;
	long page_size = 32 << (sprite_page_count
		? extract_data.group->sprite_budget_size
		: _bitmap_group_sprite_budget_512);
	long budget_pixel_count = sprite_page_count * page_size * page_size;
	pixel32 background_colors[3] =
	{
		0x00000000,
		0xFFFFFFFF,
		0x7F7F7F7F,
	};
	short maximum_bitmap_dimension = (short)(page_size - 2 * spacing);
	struct texture_page *texture_pages[32];
	short texture_page_count;
	short bitmap_index;
	short page_index;

	for (bitmap_index = 0; result && bitmap_index < extract_data.bitmap_count; bitmap_index++)
	{
		struct bitmap_extract_entry *entry = &extract_data.bitmaps[bitmap_index];

		if (entry->bitmap &&
			(entry->bitmap->width > maximum_bitmap_dimension ||
			entry->bitmap->height > maximum_bitmap_dimension))
		{
			error(
				_error_immediate,
				"### ERROR one or more sprites do not fit in the requested page size");
			result = FALSE;
		}
	}

	if (result)
	{
		if (TEST_FLAG(
			extract_data.group->flags,
			_bitmap_group_uniform_sprite_sequences_bit))
		{
			error(
				_error_immediate,
				"### ERROR hey - don't even try it! (uniform sprite sequences)\n"
				"don't fucking swim in that septic tank with your mouth open like that");
		}

		extract_build_texture_pages_by_sequence(
			texture_pages,
			&texture_page_count,
			(short)page_size,
			spacing);
		extract_data.group->sprite_spacing = spacing;
	}

	for (page_index = 0; result && page_index < texture_page_count; page_index++)
	{
		struct texture_page *texture_page = texture_pages[page_index];
		struct bitmap_data *page_bitmap = bitmap_2d_new(
			texture_page->width,
			texture_page->height,
			0,
			_bitmap_format_a8r8g8b8);

		if (page_bitmap && page_bitmap->base_address)
		{
			short entry_index;

			bitmap_fill(
				page_bitmap,
				background_colors[extract_data.group->sprite_usage]);
			for (entry_index = 0;
				result && entry_index < extract_data.bitmap_count;
				entry_index++)
			{
				struct bitmap_extract_entry *entry = &extract_data.bitmaps[entry_index];
				struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
					&extract_data.group->sequences,
					entry->sequence_index,
					struct bitmap_group_sequence);
				struct bitmap_group_sprite *sprite = TAG_BLOCK_GET_ELEMENT(
					&sequence->sprites,
					entry->sprite_index,
					struct bitmap_group_sprite);

				if (entry->page_index == page_index)
				{
					struct texture_page_texture *texture = texture_page_texture_get(
						texture_page,
						entry->page_entry_index);
					short sprite_spacing = spacing;
					point2d destination_point;

					if (!TEST_FLAG(
						extract_data.group->flags,
						_bitmap_group_extract_sprites_filthy_bug_fix_bit) &&
						texture_page->textures->actual_count == 1)
					{
						sprite_spacing = 0;
					}

					sprite->bitmap_index = page_index;
					sprite->registration_point.x =
						(sprite_spacing + sprite->registration_point.x) / texture_page->width;
					sprite->registration_point.y =
						(sprite_spacing + sprite->registration_point.y) / texture_page->height;
					sprite->bounds.x0 =
						(real)(texture->x - sprite_spacing) / texture_page->width;
					sprite->bounds.y0 =
						(real)(texture->y - sprite_spacing) / texture_page->height;
					sprite->bounds.x1 =
						(real)(texture->x + texture->width + sprite_spacing) / texture_page->width;
					sprite->bounds.y1 =
						(real)(texture->y + texture->height + sprite_spacing) / texture_page->height;

					if (sequence->first_bitmap_index == NONE)
					{
						sequence->first_bitmap_index = page_index;
						sequence->bitmap_count = 1;
					}
					else
					{
						sequence->bitmap_count = page_index - sequence->first_bitmap_index;
					}

					destination_point.x = texture->x;
					destination_point.y = texture->y;
					bitmap_copy(
						page_bitmap,
						&destination_point,
						NULL,
						entry->bitmap,
						NULL,
						0xFFFFFFFF,
						0);
					bitmap_delete(entry->bitmap);
				}
			}

			if (extract_add_bitmap(page_bitmap) != NONE)
			{
				total_page_pixel_count += texture_page->width * texture_page->height;
				bitmap_delete(page_bitmap);
			}
		}
		else
		{
			error(
				_error_silent,
				"### ERROR extract_sprite: failed to allocate texture page bitmap");
			result = FALSE;
		}

		fprintf(
			stdout,
			"texture page created #%dx#%d (%3.2f%% used)\r\n",
			texture_page->width,
			texture_page->height,
			texture_page_fraction_used(texture_page, TRUE) * 100.0f);
		fflush(stdout);
		texture_page_delete(texture_page);
	}

	if (result)
	{
		if ((real)budget_pixel_count == 0.0f)
		{
			fprintf(stdout, "### WARNING no sprite budget set\r\n");
			fflush(stdout);
		}
		else
		{
			real budget_fraction =
				(real)total_page_pixel_count / (real)budget_pixel_count;

			if (budget_fraction <= 1.0f)
			{
				fprintf(stdout, "sprite budget met (%3.0f%%)\r\n", budget_fraction * 100.0f);
				fflush(stdout);
			}
			else
			{
				error(
					_error_silent,
					"### ERROR sprite budget exceeded (%3.0f%%)",
					budget_fraction * 100.0f);
				result = FALSE;
			}
		}
	}

	return result;
}

static boolean extract_without_sequences(
	void)
{
	rectangle2d bounds;
	struct bitmap_group *group = extract_data.group;
	boolean result = TRUE;

	if (group->format == _bitmap_group_format_compressed_color_key_transparency &&
		extract_data.extract_sequences)
	{
		error(
			_error_silent,
			"### ERROR extract: compressed color-key transparency format must use a valid plate");
		result = FALSE;
	}
	else
	{
		switch (group->type)
		{
		case _bitmap_group_type_2d_textures:
		case _bitmap_group_type_cube_maps:
		case _bitmap_group_type_interface_bitmaps:
		{
			short plate_width;
			short plate_height;
			short sequence_index;

			bounds.y0 = 0;
			bounds.x0 = 0;
			plate_width = extract_data.plate->width;
			plate_height = extract_data.plate->height;
			bounds.x1 = plate_width;
			bounds.y1 = plate_height;
			sequence_index = (short)tag_block_add_element(&group->sequences);
			extract_data.sequence_index = sequence_index;
			extract_data.sequence = tag_block_get_element_with_size(
				&extract_data.group->sequences,
				sequence_index,
				sizeof(struct bitmap_group_sequence));
			extract_data.sequence->first_bitmap_index = NONE;
			extract_bitmap(&bounds);
			break;
		}

		case _bitmap_group_type_3d_textures:
			error(_error_silent, "### ERROR can't extract 3D textures without a valid plate");
			result = FALSE;
			break;

		case _bitmap_group_type_sprites:
			error(_error_silent, "### ERROR can't extract sprites without a valid plate");
			result = FALSE;
			break;

		default:
			match_vassert(
				"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
				0x1A6,
				FALSE,
				"### ERROR unsupported bitmap group type");
			break;
		}
	}

	return result;
}

static boolean extract_sequence(
	short top,
	short bottom)
{
	boolean result = TRUE;
	short x;

	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x234,
		top>=0);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x235,
		bottom>=top);
	match_assert(
		"c:\\halo\\SOURCE\\bitmaps\\bitmap_extract.c",
		0x236,
		bottom<=extract_data.plate->height);

	x = 0;
	while (result && x < extract_data.plate->width)
	{
		rectangle2d bounds;
		short extraction_state = 0;

		bounds.x0 = SHORT_MAX;
		bounds.y0 = top;
		bounds.x1 = SHORT_MIN;
		bounds.y1 = bottom;
		while (x < extract_data.plate->width && extraction_state != 2)
		{
			boolean found_bitmap = FALSE;
			boolean found_bottom_reference = FALSE;
			short y;

			for (y = top; y < bottom; y++)
			{
				pixel32 color = *(pixel32 *)bitmap_2d_address(
					extract_data.plate,
					x,
					y,
					0) & 0xFFFFFF;

				if (color == extract_data.bottom_reference)
				{
					found_bottom_reference = TRUE;
				}
				else if (color != extract_data.top_reference)
				{
					found_bitmap = TRUE;
					switch (extraction_state)
					{
					case 0:
						extraction_state = 1;
						bounds.x0 = x;
						/* fall through */
					case 1:
						bounds.y0 = MIN(y, bounds.y0);
						bounds.y1 = MAX(y, bounds.y1);
						bounds.x1 = x;
						break;
					}
				}
			}

			if ((found_bottom_reference || extract_data.single_sequence) && !found_bitmap)
			{
				if (extraction_state == 1)
					extraction_state = 2;
			}
			if (extraction_state == 1 && !found_bitmap)
			{
				extraction_state = 2;
			}
			x++;
		}

		if (extraction_state != 0)
		{
			rectangle2d adjusted_bounds;

			bounds.x1++;
			bounds.y1++;
			adjusted_bounds = bounds;
			if (TEST_FLAG(
				extract_data.group->flags,
				_bitmap_group_extract_sprites_filthy_bug_fix_bit))
			{
				short trim_y;

				for (trim_y = adjusted_bounds.y0; trim_y < adjusted_bounds.y1; trim_y++)
				{
					short trim_x;

					for (trim_x = adjusted_bounds.x0; trim_x < adjusted_bounds.x1; trim_x++)
					{
						pixel32 color = *(pixel32 *)bitmap_2d_address(
							extract_data.plate,
							trim_x,
							trim_y,
							0) & 0xFFFFFF;

						if (color != extract_data.top_reference)
							break;
					}

					if (trim_x < adjusted_bounds.x1)
						break;
				}
				adjusted_bounds.y0 = trim_y;

				for (trim_y = adjusted_bounds.y1 - 2; trim_y >= adjusted_bounds.y0; trim_y--)
				{
					short trim_x;

					for (trim_x = adjusted_bounds.x0; trim_x < adjusted_bounds.x1; trim_x++)
					{
						pixel32 color = *(pixel32 *)bitmap_2d_address(
							extract_data.plate,
							trim_x,
							trim_y,
							0) & 0xFFFFFF;

						if (color != extract_data.top_reference)
							break;
					}

					if (trim_x < adjusted_bounds.x1)
						break;
				}
				adjusted_bounds.y1 = trim_y + 1;
			}

			result = extract_bitmap(&adjusted_bounds);
		}
	}

	return result;
}

static boolean extract_sequences(
	void)
{
	boolean result = TRUE;
	short top = 1;

	while (result && top < extract_data.plate->height)
	{
		short bottom;
		short sequence_index;

		bottom = extract_find_sequence_bounds(&top);
		extract_warn_about_horizontal_border(bottom);
		sequence_index = (short)tag_block_add_element(&extract_data.group->sequences);
		if (sequence_index == NONE)
		{
			error(_error_silent, "### ERROR extract: failed to allocate sequence");
			return FALSE;
		}

		extract_data.sequence_index = sequence_index;
		extract_data.sequence = tag_block_get_element_with_size(
			&extract_data.group->sequences,
			sequence_index,
			sizeof(struct bitmap_group_sequence));
		extract_data.sequence->first_bitmap_index = NONE;
		extract_data.sequence->bitmap_count = 0;
		result = extract_sequence(top, bottom);
		top = bottom + 1;
	}

	return result;
}
