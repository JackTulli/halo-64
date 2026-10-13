/*
TEXT_GROUP.C

symbols in this file:
0018CC10 0060:
	_string_list_get_string (0000)
0018CC70 0060:
	_unicode_string_list_get_string (0000)
002A2A90 0011:
	??_C@_0BB@DGIKDNEK@?$DMmissing?5string?$DO?$AA@ (0000)
002A2AA4 0022:
	??_C@_1CC@IMCEGIAL@?$AA?$DM?$AAm?$AAi?$AAs?$AAs?$AAi?$AAn?$AAg?$AA?5?$AAs?$AAt?$AAr?$AAi?$AAn?$AAg?$AA?$DO?$AA?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "text/text_group.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

#ifdef HALO_LINUX
/* ---------- NTSC maps' missing multiplayer strings */

#include "tag_files/tag_files.h"

#define MULTIPLAYER_GAME_TEXT_TAG_NAME "ui\\multiplayer_game_text"
#define FIRST_FALLBACK_MULTIPLAYER_GAME_TEXT_STRING 36

/* ui\multiplayer_game_text holds 184 strings in the PAL release's maps
(01.01.14.2342, the build this code is), but only the first 36 in the NTSC
release's (01.10.12.2276), whose executable had the rest (the kill feed,
scores, game type messages) built in. This code asks the tag for all of
them, so with NTSC maps the native builds take the rest from here: the PAL
maps' English strings, at the same indices. */
static wchar_t *const fallback_multiplayer_game_text_strings[] =
{
	/* 36 */ L"1st",
	/* 37 */ L"2nd",
	/* 38 */ L"3rd",
	/* 39 */ L"4th",
	/* 40 */ L"5th",
	/* 41 */ L"6th",
	/* 42 */ L"7th",
	/* 43 */ L"8th",
	/* 44 */ L"9th",
	/* 45 */ L"10th",
	/* 46 */ L"11th",
	/* 47 */ L"12th",
	/* 48 */ L"13th",
	/* 49 */ L"14th",
	/* 50 */ L"15th",
	/* 51 */ L"16th",
	/* 52 */ L"(no lives)",
	/* 53 */ L"(1 life)",
	/* 54 */ L"(%d lives)",
	/* 55 */ L"Game ends in a draw",
	/* 56 */ L"Your team lost",
	/* 57 */ L"You lost",
	/* 58 */ L"Your team won",
	/* 59 */ L"You won",
	/* 60 */ L"Red leads Blue %s to %s %s",
	/* 61 */ L"Blue leads Red %s to %s %s",
	/* 62 */ L"Teams tied at %s %s",
	/* 63 */ L"Tied for %s place with %s %s",
	/* 64 */ L"In %s place with %s %s",
	/* 65 */ L"\tRed Team\t%s",
	/* 66 */ L"\tBlue Team\t%s",
	/* 67 */ L"Place",
	/* 68 */ L"Name",
	/* 69 */ L"Kills",
	/* 70 */ L"Assists",
	/* 71 */ L"Deaths",
	/* 72 */ L"\t%b-button =quit    %a-button =pick game",
	/* 73 */ L"\t%b-button =quit",
	/* 74 */ L"Welcome %s",
	/* 75 */ L"%s died",
	/* 76 */ L"%s was killed by the guardians",
	/* 77 */ L"%s was killed by a vehicle",
	/* 78 */ L"%s was killed by %s",
	/* 79 */ L"%s was betrayed by %s",
	/* 80 */ L"%s quit",
	/* 81 */ L"%s committed suicide",
	/* 82 */ L"You betrayed %s",
	/* 83 */ L"Killtacular!",
	/* 84 */ L"Triple Kill!",
	/* 85 */ L"Double Kill!",
	/* 86 */ L"Running Riot!",
	/* 87 */ L"You are on a killing spree!",
	/* 88 */ L"You killed %s",
	/* 89 */ L"Killtacular! (%d)",
	/* 90 */ L"Triple Kill! (%d)",
	/* 91 */ L"Double Kill! (%d)",
	/* 92 */ L"Running Riot! (%d)",
	/* 93 */ L"You are on a killing spree! (%d)",
	/* 94 */ L"You killed %s (%d)",
	/* 95 */ L"You are the odd man out",
	/* 96 */ L"You are out of lives",
	/* 97 */ L"Rejoin in %d",
	/* 98 */ L"Waiting for space to clear",
	/* 99 */ L"You quit out of the game",
	/* 100 */ L"Hold BACK for score",
	/* 101 */ L"Teleporter is blocked",
	/* 102 */ L"first place",
	/* 103 */ L"second place",
	/* 104 */ L"third place",
	/* 105 */ L"fourth place",
	/* 106 */ L"fifth place",
	/* 107 */ L"sixth place",
	/* 108 */ L"seventh place",
	/* 109 */ L"eighth place",
	/* 110 */ L"9th place",
	/* 111 */ L"10th place",
	/* 112 */ L"11th place",
	/* 113 */ L"12th place",
	/* 114 */ L"13th place",
	/* 115 */ L"14th place",
	/* 116 */ L"15th place",
	/* 117 */ L"16th place",
	/* 118 */ L"tied for first place",
	/* 119 */ L"tied for second place",
	/* 120 */ L"tied for third place",
	/* 121 */ L"tied for fourth place",
	/* 122 */ L"tied for fifth place",
	/* 123 */ L"tied for sixth place",
	/* 124 */ L"tied for seventh place",
	/* 125 */ L"tied for eighth place",
	/* 126 */ L"tied for 9th place",
	/* 127 */ L"tied for 10th place",
	/* 128 */ L"tied for 11th place",
	/* 129 */ L"tied for 12th place",
	/* 130 */ L"tied for 13th place",
	/* 131 */ L"tied for 14th place",
	/* 132 */ L"tied for 15th place",
	/* 133 */ L"tied for 16th place",
	/* 134 */ L"even",
	/* 135 */ L"winning",
	/* 136 */ L"losing",
	/* 137 */ L"tied",
	/* 138 */ L"Dead",
	/* 139 */ L"Quit",
	/* 140 */ L"Red Team %d Blue Team %d",
	/* 141 */ L"You scored %d to %d.",
	/* 142 */ L"Enemy scored %d to %d.",
	/* 143 */ L"Your ally scored %d to %d.",
	/* 144 */ L"You returned the flag.",
	/* 145 */ L"The enemy has your flag.",
	/* 146 */ L"The enemy returned the flag.",
	/* 147 */ L"Your ally has the flag.",
	/* 148 */ L"Your ally returned the flag.",
	/* 149 */ L"Your flag was returned.",
	/* 150 */ L"The enemy's flag was returned.",
	/* 151 */ L"Time expired.",
	/* 152 */ L"You are on offense.",
	/* 153 */ L"You are on defense.",
	/* 154 */ L"Score",
	/* 155 */ L"%s (%d seconds)",
	/* 156 */ L"Ally %s in on the hill (%d seconds)",
	/* 157 */ L"Enemy %s in on the hill (%d seconds)",
	/* 158 */ L"Time",
	/* 159 */ L"You are it!",
	/* 160 */ L"Ally is it!",
	/* 161 */ L"%s is it.",
	/* 162 */ L"You have the ball.",
	/* 163 */ L"An ally has the ball.",
	/* 164 */ L"%s has the ball.",
	/* 165 */ L"Ally %s has the ball (%d seconds)",
	/* 166 */ L"Enemy %s has the ball (%d seconds)",
	/* 167 */ L"You scored a flag!",
	/* 168 */ L"Ally %s scored a flag!",
	/* 169 */ L"Enemy %s scored a flag!",
	/* 170 */ L"You completed lap %d in %.2f seconds.",
	/* 171 */ L"Ally %s completed a lap %d.",
	/* 172 */ L"Enemy %s completed a lap.",
	/* 173 */ L"new best lap time %.2f.",
	/* 174 */ L"%s 1 flag",
	/* 175 */ L"%s %d flags",
	/* 176 */ L"%s all laps complete",
	/* 177 */ L"%s lap %d of %d",
	/* 178 */ L"Flags",
	/* 179 */ L"%d team %d",
	/* 180 */ L"New Target %s",
	/* 181 */ L"%s kills %d team %d of %d",
	/* 182 */ L"%s kills %d of %d",
	/* 183 */ L"You were telefragged",
};

/* indices 36 to 183 */
typedef char fallback_multiplayer_game_text_string_count_check[
	NUMBEROF(fallback_multiplayer_game_text_strings) == 184 - FIRST_FALLBACK_MULTIPLAYER_GAME_TEXT_STRING ? 1 : -1];

/* the built-in string for a string list too short to hold string_index,
or NULL */
static wchar_t *fallback_string(long tag_index, short string_index)
{
	short fallback_index = string_index - FIRST_FALLBACK_MULTIPLAYER_GAME_TEXT_STRING;

	if (fallback_index < 0 ||
		fallback_index >= (short)NUMBEROF(fallback_multiplayer_game_text_strings) ||
		csstrcasecmp(tag_get_name(tag_index), MULTIPLAYER_GAME_TEXT_TAG_NAME))
	{
		return NULL;
	}
	return fallback_multiplayer_game_text_strings[fallback_index];
}

#endif
/* ---------- public code */

char *string_list_get_string(long tag_index, short string_index)
{
	char *result = "<missing string>";

	if (tag_index != NONE)
	{
		struct string_list *list = string_list_definition_get(tag_index);

		if (string_index >= 0 && string_index < list->strings.count)
		{
			struct string_list_entry *entry = TAG_BLOCK_GET_ELEMENT(
				&list->strings,
				string_index,
				struct string_list_entry);

			if (entry->string.size > 0)
			{
				result = entry->string.address;
				result[entry->string.size - 1] = '\0';
			}
		}
	}

	return result;
}

wchar_t *unicode_string_list_get_string(long tag_index, short string_index)
{
	wchar_t *result = L"<missing string>";

	if (tag_index != NONE)
	{
		struct string_list *list = unicode_string_list_definition_get(tag_index);

		if (string_index >= 0 && string_index < list->strings.count)
		{
			struct string_list_entry *entry = TAG_BLOCK_GET_ELEMENT(
				&list->strings,
				string_index,
				struct string_list_entry);

			if (entry->string.size > 0)
			{
				result = entry->string.address;
				result[entry->string.size / sizeof(wchar_t) - 1] = L'\0';
			}
		}
#ifdef HALO_LINUX
		else if (fallback_string(tag_index, string_index))
		{
			result = fallback_string(tag_index, string_index);
		}
#endif
	}

	return result;
}

/* ---------- private code */
