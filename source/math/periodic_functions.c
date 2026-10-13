/*
PERIODIC_FUNCTIONS.C

symbols in this file:
000F9D90 0070:
	_periodic_functions_dispose (0000)
000F9E00 0130:
	_periodic_function_evaluate (0000)
000F9F30 0120:
	_transition_function_evaluate (0000)
000FA050 0100:
	@periodic_function_build_variable_period_x_table@4 (0000)
000FA150 0130:
	_transition_function_build_table (0000)
000FA280 02b0:
	_periodic_function_build_table (0000)
000FA530 00c0:
	_periodic_functions_initialize (0000)
0027AC7C 000a:
	??_C@_09PFNFPFNK@very?5late?$AA@ (0000)
0027AC88 0005:
	??_C@_04KNPHFNKG@late?$AA@ (0000)
0027AC90 000b:
	??_C@_0L@JFLLBJBL@very?5early?$AA@ (0000)
0027AC9C 0006:
	??_C@_05MMFAAGKE@early?$AA@ (0000)
0027ACA4 0006:
	??_C@_05GILPFPMI@spark?$AA@ (0000)
0027ACAC 0007:
	??_C@_06EJKHKJBM@wander?$AA@ (0000)
0027ACB4 0007:
	??_C@_06NJLEAOMD@jitter?$AA@ (0000)
0027ACBC 0018:
	??_C@_0BI@LODCFJBP@slide?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027ACD4 0006:
	??_C@_05IODDPPOI@slide?$AA@ (0000)
0027ACDC 0020:
	??_C@_0CA@POBCGHPD@diagonal?5wave?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027ACFC 000e:
	??_C@_0O@JLGDFEJA@diagonal?5wave?$AA@ (0000)
0027AD0C 0019:
	??_C@_0BJ@LIPGHGNF@cosine?5?$CIvariable?5period?$CJ?$AA@ (0000)
0027AD28 0007:
	??_C@_06IDPAJACI@cosine?$AA@ (0000)
0027AD30 0005:
	??_C@_04DDFHDMOL@zero?$AA@ (0000)
0027AD38 0004:
	??_C@_03OHDNLOHO@one?$AA@ (0000)
0027AD3C 0029:
	??_C@_0CJ@OKNGCK@c?3?2halo?2SOURCE?2math?2periodic_fun@ (0000)
0027AD68 0004:
	__real@41cccccd (0000)
0027AD6C 003f:
	??_C@_0DP@JEKDFHEF@function_type?$DO?$DN0?5?$CG?$CG?5function_typ@ (0000)
0027ADAC 0004:
	__real@447fc000 (0000)
0027ADB0 0041:
	??_C@_0EB@CENGDIOD@function_type?$DO?$DN0?5?$CG?$CG?5function_typ@ (0000)
0027ADF4 0004:
	__real@3cce16a7 (0000)
0027ADF8 0004:
	__real@3d002d4f (0000)
0027ADFC 0004:
	__real@3d377812 (0000)
0027AE00 0008:
	__real@4000000000000000 (0000)
0027AE08 0008:
	__real@3fd0000000000000 (0000)
0027AE10 0004:
	__real@3a802008 (0000)
0027AE14 0004:
	__real@422fede0 (0000)
0027AE18 0004:
	__real@41c90fdb (0000)
0027AE1C 0004:
	__real@3f65c8fa (0000)
0027AE20 0004:
	__real@41e00000 (0000)
0027AE24 0004:
	__real@3cdfffff (0000)
0027AE28 001d:
	??_C@_0BN@IGAJMLPO@?$CBfunction_tables_initialized?$AA@ (0000)
0030791C 0060:
	_global_periodic_functions_enum_strings (0000)
	_global_periodic_functions_enum (0030)
	_global_transition_functions_enum (0054)
004561BC 004c:
	_function_tables_initialized (0000)
	_transition_function_tables (0004)
	_periodic_function_tables (001c)
*/

/* ---------- headers */

#include "cseries.h"
#include "periodic_functions.h"
#include "tag_files/tag_groups.h"

#ifdef HALO_N64
#define N64_FRACTION(x) n64_fraction(x)
#else
#define N64_FRACTION(x) ((real)fmod((double)(x), 1.0))
#endif

/* ---------- constants */

enum
{
	_periodic_function_one = 0,
	_periodic_function_zero,
	_periodic_function_cosine,
	_periodic_function_cosine_variable_period,
	_periodic_function_diagonal_wave,
	_periodic_function_diagonal_wave_variable_period,
	_periodic_function_slide,
	_periodic_function_slide_variable_period,
	_periodic_function_noise,
	_periodic_function_jitter,
	_periodic_function_wander,
	_periodic_function_spark,
	NUMBER_OF_PERIODIC_FUNCTIONS,
};

enum
{
	PERIODIC_FUNCTION_TABLE_SIZE = 1024,
	PERIODIC_FUNCTION_TABLE_MASK = PERIODIC_FUNCTION_TABLE_SIZE-1,
};

enum
{
	SLIDE_PERIODIC_FUNCTION_FLAGS =
		FLAG(_periodic_function_slide) |
		FLAG(_periodic_function_slide_variable_period),
};

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static void __fastcall periodic_function_build_variable_period_x_table(
	real *x_table);
static void transition_function_build_table(
	short function_type,
	byte *table);
static void periodic_function_build_table(
	short function_type,
	byte *table);

/* ---------- globals */

static char *global_periodic_functions_enum_strings[NUMBER_OF_PERIODIC_FUNCTIONS] =
{
	"one",
	"zero",
	"cosine",
	"cosine (variable period)",
	"diagonal wave",
	"diagonal wave (variable period)",
	"slide",
	"slide (variable period)",
	"noise",
	"jitter",
	"wander",
	"spark",
};

struct tag_enum_definition global_periodic_functions_enum =
{
	NUMBER_OF_PERIODIC_FUNCTIONS,
	global_periodic_functions_enum_strings,
	NULL,
};

static char *global_transition_functions_enum_strings[NUMBER_OF_TRANSITION_FUNCTIONS] =
{
	"linear",
	"early",
	"very early",
	"late",
	"very late",
	"cosine",
};

struct tag_enum_definition global_transition_functions_enum =
{
	NUMBER_OF_TRANSITION_FUNCTIONS,
	global_transition_functions_enum_strings,
	NULL,
};

static boolean function_tables_initialized = FALSE;
static byte *transition_function_tables[NUMBER_OF_TRANSITION_FUNCTIONS] = { 0 };
static byte *periodic_function_tables[NUMBER_OF_PERIODIC_FUNCTIONS] = { 0 };

/* ---------- public code */

void periodic_functions_dispose(
	void)
{
	short function_index;

	if (function_tables_initialized)
	{
		for (function_index = 0; function_index < NUMBER_OF_PERIODIC_FUNCTIONS; function_index++)
		{
			match_free(
				"c:\\halo\\SOURCE\\math\\periodic_functions.c",
				122,
				periodic_function_tables[function_index]);
		}

		for (function_index = 0; function_index < NUMBER_OF_TRANSITION_FUNCTIONS; function_index++)
		{
			match_free(
				"c:\\halo\\SOURCE\\math\\periodic_functions.c",
				132,
				transition_function_tables[function_index]);
		}

		function_tables_initialized = FALSE;
	}

	return;
}

real periodic_function_evaluate(
	short function_type,
	real time)
{
	long index;
	byte *table;
	real fraction;
	real first_value;
	real second_value;
	real result;

	if (function_type == _periodic_function_one)
		return 1.0f;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		157,
		function_type>=0 && function_type<NUMBER_OF_PERIODIC_FUNCTIONS);

	if (function_tables_initialized)
	{
		time *= 25.6f;
		fraction = N64_FRACTION(time);
		index = fast_ftol(time-fraction);
		table = periodic_function_tables[function_type];
		index &= PERIODIC_FUNCTION_TABLE_MASK;
		first_value = table[index] * (1.0f/255.0f);
		index = (index+1)&PERIODIC_FUNCTION_TABLE_MASK;
		second_value = table[index] * (1.0f/255.0f);

		if (TEST_FLAG(SLIDE_PERIODIC_FUNCTION_FLAGS, function_type))
		{
			if (first_value > 0.75f && second_value < 0.25f)
				second_value += 1.0f;

			result = (1.0f-fraction)*first_value + second_value*fraction;
			if (result > 1.0f)
				return result-1.0f;

			return result;
		}

		return (1.0f-fraction)*first_value + second_value*fraction;
	}

	return 0.0f;
}

real transition_function_evaluate(
	short function_type,
	real value)
{
	long index;
	byte *table;
	real scaled;
	real fraction;
	real first_value;
	real second_value;

	if (value < 0.0f)
		value = 0.0f;
	else if (value > 1.0f)
		value = 1.0f;

	if (function_type == _transition_function_linear)
		return value;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		216,
		function_type>=0 && function_type<NUMBER_OF_TRANSITION_FUNCTIONS);

	if (function_tables_initialized)
	{
		table = transition_function_tables[function_type];
		scaled = value*(real)(PERIODIC_FUNCTION_TABLE_SIZE-1);
		fraction = N64_FRACTION(scaled);
		index = fast_ftol(scaled-0.5f);
		if ((short)index == PERIODIC_FUNCTION_TABLE_SIZE-1)
			return table[PERIODIC_FUNCTION_TABLE_SIZE-1] * (1.0f/255.0f);

		first_value = table[(short)index] * (1.0f/255.0f);
		second_value = table[(short)index+1] * (1.0f/255.0f);

		return first_value*(1.0f-fraction) + second_value*fraction;
	}

	return 0.0f;
}

/* ---------- private code */

static void __fastcall periodic_function_build_variable_period_x_table(
	real *x_table)
{
	real sum = 0.0f;
	short index;

	for (index = 0; index < PERIODIC_FUNCTION_TABLE_SIZE; index++)
	{
		x_table[index] = sum;
		sum += real_random()*0.25f + 0.25f +
			((real)cos(8.2f*_pi*index/PERIODIC_FUNCTION_TABLE_SIZE)+1.0f)*real_random() +
			((real)cos(10.2f*_pi*index/PERIODIC_FUNCTION_TABLE_SIZE)+1.0f)*real_random() +
			((real)cos(14.6f*_pi*index/PERIODIC_FUNCTION_TABLE_SIZE)+1.0f)*real_random();
	}

	for (index = 0; index < PERIODIC_FUNCTION_TABLE_SIZE; index++)
		x_table[index] = x_table[index]/sum;

	return;
}

static void transition_function_build_table(
	short function_type,
	byte *table)
{
	long transition_function = function_type;
	long index;
	long count;
	real value;
	real result;

	index = 0;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		value = index * (1.0f/(PERIODIC_FUNCTION_TABLE_SIZE-1));
		switch (transition_function)
		{
		case _transition_function_linear:
			result = value;
			break;
		case _transition_function_early:
			result = (real)pow((double)value, 0.5);
			break;
		case _transition_function_very_early:
			result = (real)pow((double)value, 0.25);
			break;
		case _transition_function_late:
			result = (real)pow((double)value, 2.0);
			break;
		case _transition_function_very_late:
			result = (real)pow((double)value, 4.0);
			break;
		case _transition_function_cosine:
			result = ((real)sin(value*3.1415927f-1.5707964f)+1.0f)*0.5f;
			break;
		/* result is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\math\\periodic_functions.c", 411, TRUE);
			system_exit(-1);
			break;
		}

		table[index] = (byte)PIN((long)(result*255.0f), 0, 255);
		index++;
	}

	return;
}

static void periodic_function_build_table(
	short function_type,
	byte *table)
{
	long index;
	long count;
	real random_values[PERIODIC_FUNCTION_TABLE_SIZE];
	real values[PERIODIC_FUNCTION_TABLE_SIZE];
	real minimum = 3.402823466e+38f;
	real maximum = -3.402823466e+38f;
	real x;
	real random_x;
	real result;
	real range;
	real *value;
	byte *destination;

	periodic_function_build_variable_period_x_table(random_values);
	index = 0;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		x = index*0.027343748f;
		random_x = random_values[index]*28.0f;
		switch (function_type)
		{
		case _periodic_function_one:
			result = 1.0f;
			break;
		case _periodic_function_zero:
			result = 0.0f;
			break;
		case _periodic_function_cosine:
			result = (real)cos(x*6.2831855f);
			break;
		case _periodic_function_cosine_variable_period:
			result = (real)cos(random_x*6.2831855f);
			break;
		case _periodic_function_slide:
			result = N64_FRACTION(x);
			break;
		case _periodic_function_slide_variable_period:
			result = N64_FRACTION(random_x);
			break;
		case _periodic_function_diagonal_wave:
			result = N64_FRACTION(x);
			if (result < 0.5f)
				result *= 2.0f;
			else
				result = 1.0f-(result-0.5f)*2.0f;
			break;
		case _periodic_function_diagonal_wave_variable_period:
			result = N64_FRACTION(random_x);
			if (result < 0.5f)
				result *= 2.0f;
			else
				result = 1.0f-(result-0.5f)*2.0f;
			break;
		case _periodic_function_noise:
			result = real_random();
			break;
		case _periodic_function_jitter:
		case _periodic_function_wander:
			{
				real cosine_fast = (real)cos(x*25.132742f);
				real cosine_slow = (real)cos(x*0.89759791f);

				result = (cosine_slow*cosine_fast +
					(real)cos(x*43.9823f)*(real)sin(x*1.5707964f))*0.5f +
					(real)sin(x*3.1415927f)*(real)cos(x*6.2831855f);
			}
			break;
		case _periodic_function_spark:
			result = N64_FRACTION(random_x);
			result *= result;
			break;
		/* result is left unassigned only by this default arm. Not reached unassigned: the
		 * arm's assertion failure calls system_exit, which does not return in January
		 * (0x47c960 jumps to halt_and_catch_fire 0x4f21c0, which loops or calls exit).
		 * Source-policy approval pending (2026-09-27 audit). */
		default:
			display_assert(NULL, "c:\\halo\\SOURCE\\math\\periodic_functions.c", 499, TRUE);
			system_exit(-1);
			break;
		}

		if (result > maximum)
			maximum = result;
		if (result < minimum)
			minimum = result;
		values[index] = result;
		index++;
	}

	range = TEST_FLAG(SLIDE_PERIODIC_FUNCTION_FLAGS, function_type)
		? 0.0f
		: maximum-minimum;
	destination = table;
	value = values;
	for (count = PERIODIC_FUNCTION_TABLE_SIZE; count; count--)
	{
		if (range != 0.0f)
			result = (*value-minimum)/range;
		else
			result = *value;
		*destination = (byte)PIN((long)(result*255.0f), 0, 255);
		value++;
		destination++;
	}

	return;
}

void periodic_functions_initialize(
	void)
{
	short function_index;

	match_assert(
		"c:\\halo\\SOURCE\\math\\periodic_functions.c",
		67,
		!function_tables_initialized);
	function_tables_initialized = TRUE;
	*get_global_random_seed_address() = 0x20F3F660;

	for (function_index = 0; function_index < NUMBER_OF_PERIODIC_FUNCTIONS; function_index++)
	{
		periodic_function_tables[function_index] = match_malloc(
			"c:\\halo\\SOURCE\\math\\periodic_functions.c",
			78,
			PERIODIC_FUNCTION_TABLE_SIZE);
		if (periodic_function_tables[function_index])
		{
			periodic_function_build_table(
				function_index,
				periodic_function_tables[function_index]);
		}
		else
		{
			function_tables_initialized = FALSE;
		}
	}

	for (function_index = 0; function_index < NUMBER_OF_TRANSITION_FUNCTIONS; function_index++)
	{
		transition_function_tables[function_index] = match_malloc(
			"c:\\halo\\SOURCE\\math\\periodic_functions.c",
			96,
			PERIODIC_FUNCTION_TABLE_SIZE);
		if (transition_function_tables[function_index])
		{
			transition_function_build_table(
				function_index,
				transition_function_tables[function_index]);
		}
		else
		{
			function_tables_initialized = FALSE;
		}
	}

	return;
}
