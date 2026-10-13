/*
UI_WIDGET_GROUP.C

symbols in this file:
000E4620 0420:
	_ui_load_tags_for_scenario (0000)
0027857C 0033:
	??_C@_0DD@HIJLBPHL@failed?5to?5load?5miscellaneous?5loc@ (0000)
002785B0 003a:
	??_C@_0DK@MPPHFEHK@failed?5to?5load?5the?5solo?5scenario@ (0000)
002785EC 000e:
	??_C@_0O@INBKELDJ@ui?2shell?2solo?$AA@ (0000)
00278600 0041:
	??_C@_0EB@BLFLKJID@failed?5to?5load?5the?5multiplayer?5s@ (0000)
00278644 0015:
	??_C@_0BF@FHGIICCE@ui?2shell?2multiplayer?$AA@ (0000)
0027865C 0025:
	??_C@_0CF@LCDPGNMG@failed?5to?5load?5main?5menu?5title?5m@ (0000)
00278684 0024:
	??_C@_0CE@BPCKAMEK@failed?5to?5load?5ui?5failure?5sound?5@ (0000)
002786A8 0021:
	??_C@_0CB@DFIKOPNL@failed?5to?5load?5ui?5back?5sound?5tag@ (0000)
002786CC 0024:
	??_C@_0CE@JMNPCHFG@failed?5to?5load?5ui?5forward?5sound?5@ (0000)
002786F0 0023:
	??_C@_0CD@PCFDGADJ@failed?5to?5load?5ui?5cursor?5sound?5t@ (0000)
00278718 0045:
	??_C@_0EF@KPOLMCHO@failed?5to?5load?5the?5player?5profil@ (0000)
00278760 0052:
	??_C@_0FC@PFKACJEP@failed?5to?5load?5the?5player?5profil@ (0000)
002787B8 0054:
	??_C@_0FE@JKNKGHDC@failed?5to?5load?5the?5default?5playe@ (0000)
00278810 0045:
	??_C@_0EF@IDLODINJ@failed?5to?5load?5the?5player?5profil@ (0000)
00278858 004f:
	??_C@_0EP@GBOCIPKK@failed?5to?5load?5the?5player?5profil@ (0000)
002788A8 0040:
	??_C@_0EA@DGNKALF@failed?5to?5load?5the?5default?5playe@ (0000)
002788E8 0038:
	??_C@_0DI@OBALKBC@failed?5to?5load?5the?5game?5difficul@ (0000)
00278920 003b:
	??_C@_0DL@CNLOFCOK@ui?2shell?2main_menu?2player_profil@ (0000)
00278960 0043:
	??_C@_0ED@GHAOMBDD@failed?5to?5load?5the?5multiplayer?5v@ (0000)
002789A8 0042:
	??_C@_0EC@HAFOJMAH@failed?5to?5load?5the?5default?5playl@ (0000)
002789EC 002a:
	??_C@_0CK@DLIFJMDN@ui?2default_multiplayer_game_sett@ (0000)
00278A18 003f:
	??_C@_0DP@CGKBNILG@failed?5to?5load?5the?5default?5saved@ (0000)
00278A58 001b:
	??_C@_0BL@GMMNCEMI@ui?2saved_game_file_strings?$AA@ (0000)
00278A74 0038:
	??_C@_0DI@IALAJLCD@failed?5to?5load?5the?5multiplayer?5s@ (0000)
00278AAC 0019:
	??_C@_0BJ@PMBLKDHA@ui?2multiplayer_scenarios?$AA@ (0000)
00278AC8 0033:
	??_C@_0DD@OHHIGLLE@failed?5to?5load?5random?5player?5nam@ (0000)
00278AFC 0017:
	??_C@_0BH@HPKMNAPF@ui?2random_player_names?$AA@ (0000)
00278B14 0032:
	??_C@_0DC@KHBOAMNN@failed?5to?5load?5the?5browser?8s?5vir@ (0000)
00278B48 000b:
	??_C@_0L@BFJFJOJM@ui?2english?$AA@ (0000)
00278B54 003f:
	??_C@_0DP@IJPGFCNJ@failed?5to?5load?5the?5main?5menu?5sce@ (0000)
00278B94 0013:
	??_C@_0BD@OAAJMPAE@ui?2shell?2main_menu?$AA@ (0000)
00278BA8 0016:
	??_C@_0BG@PFHAIEEK@unknown?5scenario?5type?$AA@ (0000)
00278BC0 002b:
	??_C@_0CL@GIFHHALH@c?3?2halo?2SOURCE?2interface?2ui_widg@ (0000)
00278BEC 001d:
	??_C@_0BN@PIOKPAAC@generic?5white?5texture?5bitmap?$AA@ (0000)
00278C0C 0017:
	??_C@_0BH@CENABDIJ@ui?2shell?2bitmaps?2white?$AA@ (0000)
00278C24 0039:
	??_C@_0DJ@FKCKCMDM@failed?5to?5load?5the?5multiplayer?5g@ (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"
#include "cache/cache_files.h"
#include "cseries/errors.h"
#include "scenario/scenario_definitions.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- macros */

#define LOAD_UI_TAG(group, path, message) \
	if (tag_load(group, path, 0) == NONE) \
		error(_error_silent, message)

#define SCENARIO_GET(tag_index) ((struct scenario *)tag_get('scnr', tag_index))

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void ui_load_tags_for_scenario(
	long scenario_tag_index)
{
	struct scenario *scenario = SCENARIO_GET(scenario_tag_index);

	LOAD_UI_TAG('ustr', "ui\\multiplayer_game_text", "failed to load the multiplayer game text string list tag");
	LOAD_UI_TAG('bitm', "ui\\shell\\bitmaps\\white", "generic white texture bitmap");

	switch (scenario->type)
	{
	case _scenario_type_solo:
		LOAD_UI_TAG('Soul', "ui\\shell\\solo", "failed to load the solo scenario ui_widget_collection tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\strings\\temp_strings", "failed to load miscellaneous localized strings tag");
		break;

	case _scenario_type_multiplayer:
		LOAD_UI_TAG('Soul', "ui\\shell\\multiplayer", "failed to load the multiplayer scenario ui_widget_collection tag");
		break;

	case _scenario_type_main_menu:
		LOAD_UI_TAG('Soul', "ui\\shell\\main_menu", "failed to load the main menu scenario ui_widget_collection_tag");
		LOAD_UI_TAG('vcky', "ui\\english", "failed to load the browser's virtual keyboard tag");
		LOAD_UI_TAG('ustr', "ui\\random_player_names", "failed to load random player names string list tag");
		LOAD_UI_TAG('mply', "ui\\multiplayer_scenarios", "failed to load the multiplayer scenario description tag");
		LOAD_UI_TAG('ustr', "ui\\saved_game_file_strings", "failed to load the default saved game filename string list tag");
		LOAD_UI_TAG('ustr', "ui\\default_multiplayer_game_setting_names", "failed to load the default playlist profile names string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\strings\\game_variant_descriptions", "failed to load the multiplayer variant description string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\difficulty_names", "failed to load the game difficulty name string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\strings\\default_player_profile_names", "failed to load the default player profile names string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\button_set_long_descriptions", "failed to load the player profile button set long descriptions string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\button_set_short_descriptions", "failed to load the player profile short descriptions string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\joystick_set_defaults_descriptions", "failed to load the default player profile joystick set descriptions string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\joystick_set_short_descriptions", "failed to load the player profile joystick set short descriptions string list tag");
		LOAD_UI_TAG('ustr', "ui\\shell\\main_menu\\player_profiles_select\\profile_description_labels", "failed to load the player profile description labels string list tag");
		LOAD_UI_TAG('snd!', "sound\\sfx\\ui\\cursor", "failed to load ui cursor sound tag");
		LOAD_UI_TAG('snd!', "sound\\sfx\\ui\\forward", "failed to load ui forward sound tag");
		LOAD_UI_TAG('snd!', "sound\\sfx\\ui\\back", "failed to load ui back sound tag");
		LOAD_UI_TAG('snd!', "sound\\sfx\\ui\\flag_failure", "failed to load ui failure sound tag");
		LOAD_UI_TAG('lsnd', "sound\\music\\title1\\title1", "failed to load main menu title music");
		break;

	default:
		display_assert("unknown scenario type", "c:\\halo\\SOURCE\\interface\\ui_widget_group.c", 506, TRUE);
		system_exit(-1);
		break;
	}

	return;
}

/* ---------- private code */
