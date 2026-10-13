/*
MODEL_ANIMATION_DEFINITIONS.C

symbols in this file:
0010FD20 0090:
	_animation_get_frame_data (0000)
0010FDB0 0060:
	_animation_get_frame_info (0000)
0010FE10 0030:
	_animation_list_get_string (0000)
0027EC6C 0006:
	??_C@_05PLIADJJL@power?$AA@ (0000)
0027EC74 000b:
	??_C@_0L@BIFNFIGI@unoccupied?$AA@ (0000)
0027EC80 0009:
	??_C@_08NFIIDHFI@occupied?$AA@ (0000)
0027EC8C 000d:
	??_C@_0N@MBENMFOA@ground?9speed?$AA@ (0000)
0027EC9C 0008:
	??_C@_07HAHFHPDC@braking?$AA@ (0000)
0027ECA4 0009:
	??_C@_08IEKKGCGJ@throttle?$AA@ (0000)
0027ECB0 0005:
	??_C@_04FBAPEKPH@roll?$AA@ (0000)
0027ECB8 0009:
	??_C@_08NELMGGOM@steering?$AA@ (0000)
0027ECC4 000c:
	??_C@_0M@GHKMJFNH@o?9h?9s?9enter?$AA@ (0000)
0027ECD0 0009:
	??_C@_08IDIPKNK@o?9h?9exit?$AA@ (0000)
0027ECDC 000a:
	??_C@_09MOGHKEJM@exit?9full?$AA@ (0000)
0027ECE8 000b:
	??_C@_0L@NJHEMOHG@exit?9empty?$AA@ (0000)
0027ECF4 0012:
	??_C@_0BC@OMAJDCCJ@overheating?9again?$AA@ (0000)
0027ED08 000c:
	??_C@_0M@BKEBJHEB@overheating?$AA@ (0000)
0027ED14 0011:
	??_C@_0BB@HBGMJCAM@throw?9overheated?$AA@ (0000)
0027ED28 000a:
	??_C@_09HPOBKLNE@misfire?92?$AA@ (0000)
0027ED34 000a:
	??_C@_09FEMMPIBH@misfire?91?$AA@ (0000)
0027ED40 000b:
	??_C@_0L@GOFMCMLE@ammunition?$AA@ (0000)
0027ED4C 0013:
	??_C@_0BD@HEFEFIIA@overcharged?9jitter?$AA@ (0000)
0027ED60 000c:
	??_C@_0M@GOBLNEGG@overcharged?$AA@ (0000)
0027ED6C 000b:
	??_C@_0L@LLGANJAB@overheated?$AA@ (0000)
0027ED78 000c:
	??_C@_0M@DPEFKMJN@reload?9full?$AA@ (0000)
0027ED84 000d:
	??_C@_0N@KOICNMOI@reload?9empty?$AA@ (0000)
0027ED94 0009:
	??_C@_08CAEMCGPF@light?9on?$AA@ (0000)
0027EDA0 000a:
	??_C@_09PAEDNHCM@light?9off?$AA@ (0000)
0027EDAC 0009:
	??_C@_08OPBNIBHL@overlays?$AA@ (0000)
0027EDB8 0007:
	??_C@_06OHPAHEPJ@posing?$AA@ (0000)
0027EDC0 0009:
	??_C@_08INMOBBGO@hovering?$AA@ (0000)
0027EDCC 0008:
	??_C@_07OBKECFAM@closing?$AA@ (0000)
0027EDD4 0008:
	??_C@_07CPJOLGKM@opening?$AA@ (0000)
0027EDDC 000d:
	??_C@_0N@ECBDCFFF@flying?9right?$AA@ (0000)
0027EDEC 000c:
	??_C@_0M@IEGGCNDN@flying?9left?$AA@ (0000)
0027EDF8 000c:
	??_C@_0M@HPKFMIHO@flying?9back?$AA@ (0000)
0027EE04 000d:
	??_C@_0N@MFNJHBG@flying?9front?$AA@ (0000)
0027EE14 0006:
	??_C@_05POKJGOIE@user9?$AA@ (0000)
0027EE1C 0006:
	??_C@_05OHLCFPMF@user8?$AA@ (0000)
0027EE24 0006:
	??_C@_05GACKEDAK@user7?$AA@ (0000)
0027EE2C 0006:
	??_C@_05HJDBHCEL@user6?$AA@ (0000)
0027EE34 0006:
	??_C@_05FCBMCBII@user5?$AA@ (0000)
0027EE3C 0006:
	??_C@_05ELAHBAMJ@user4?$AA@ (0000)
0027EE44 0006:
	??_C@_05EEGIGAO@user3?$AA@ (0000)
0027EE4C 0006:
	??_C@_05CPGLNFMN@user0?$AA@ (0000)
0027EE54 0009:
	??_C@_08BIJNNHHN@emotions?$AA@ (0000)
0027EE60 0005:
	??_C@_04MDNIELIK@look?$AA@ (0000)
0027EE68 0005:
	??_C@_04MKNBDEPB@exit?$AA@ (0000)
0027EE70 0006:
	??_C@_05IHEOCMON@enter?$AA@ (0000)
0027EE78 0006:
	??_C@_05MOLGPPCG@twist?$AA@ (0000)
0027EE80 0005:
	??_C@_04GHHNIPCF@push?$AA@ (0000)
0027EE88 000c:
	??_C@_0M@KBFIGEFK@acc?9up?9down?$AA@ (0000)
0027EE94 000f:
	??_C@_0P@KLFFAAJC@acc?9left?9right?$AA@ (0000)
0027EEA4 000f:
	??_C@_0P@BFHBMAML@acc?9front?9back?$AA@ (0000)
0027EEB4 000d:
	??_C@_0N@OFNHNLHG@landing?9dead?$AA@ (0000)
0027EEC4 000e:
	??_C@_0O@HJDKCMG@airborne?9dead?$AA@ (0000)
0027EED4 0007:
	??_C@_06NPLJKCDM@r?9foot?$AA@ (0000)
0027EEDC 0006:
	??_C@_05CENGKFDD@r?9leg?$AA@ (0000)
0027EEE4 0007:
	??_C@_06MLFGBDAI@r?9hand?$AA@ (0000)
0027EEEC 0006:
	??_C@_05DFDKCAJB@r?9arm?$AA@ (0000)
0027EEF4 0007:
	??_C@_06OIFIEFKP@l?9foot?$AA@ (0000)
0027EEFC 0006:
	??_C@_05BNAKMGNI@l?9leg?$AA@ (0000)
0027EF04 0007:
	??_C@_06PMLHPEJL@l?9hand?$AA@ (0000)
0027EF0C 0006:
	??_C@_05MOGEDHK@l?9arm?$AA@ (0000)
0027EF14 0005:
	??_C@_04NEODDMOL@head?$AA@ (0000)
0027EF1C 0006:
	??_C@_05HIADHCOO@chest?$AA@ (0000)
0027EF24 0004:
	??_C@_03GBCDPJBA@gut?$AA@ (0000)
0027EF28 0005:
	??_C@_04JFCNHLLM@back?$AA@ (0000)
0027EF30 0007:
	??_C@_06EBLBNMNB@h?9kill?$AA@ (0000)
0027EF38 0007:
	??_C@_06CEGIBEOF@s?9kill?$AA@ (0000)
0027EF40 0007:
	??_C@_06LGPBHHOH@h?9ping?$AA@ (0000)
0027EF48 0007:
	??_C@_06NDCILPND@s?9ping?$AA@ (0000)
0027EF50 0008:
	??_C@_07FGKHKEFC@zapping?$AA@ (0000)
0027EF58 000b:
	??_C@_0L@HCIEODNI@leap?9melee?$AA@ (0000)
0027EF64 000e:
	??_C@_0O@FGCGLLOC@leap?9airborne?$AA@ (0000)
0027EF74 000b:
	??_C@_0L@KOACHADJ@leap?9start?$AA@ (0000)
0027EF80 0008:
	??_C@_07EPIHAGEP@feeding?$AA@ (0000)
0027EF88 0011:
	??_C@_0BB@PDEKBBDH@melee?9continuous?$AA@ (0000)
0027EF9C 000f:
	??_C@_0P@CGNINMGF@resurrect?9back?$AA@ (0000)
0027EFAC 0010:
	??_C@_0BA@IGGBCDOO@resurrect?9front?$AA@ (0000)
0027EFBC 0008:
	??_C@_07EMJOGDPL@flaming?$AA@ (0000)
0027EFC4 000f:
	??_C@_0P@BLBKFAHD@melee?9airborne?$AA@ (0000)
0027EFD4 000e:
	??_C@_0O@CGPDBLHN@stunned?9right?$AA@ (0000)
0027EFE4 000d:
	??_C@_0N@PPAMFCE@stunned?9left?$AA@ (0000)
0027EFF4 000d:
	??_C@_0N@PEDDCAGH@stunned?9back?$AA@ (0000)
0027F004 000e:
	??_C@_0O@GILNKJDO@stunned?9front?$AA@ (0000)
0027F014 000e:
	??_C@_0O@HHGHILCK@signal?9attack?$AA@ (0000)
0027F024 000c:
	??_C@_0M@PKIJMIOD@signal?9move?$AA@ (0000)
0027F030 0009:
	??_C@_08IGAICBEC@aim?9move?$AA@ (0000)
0027F03C 000a:
	??_C@_09HIFLLFAI@aim?9still?$AA@ (0000)
0027F048 0009:
	??_C@_08ELKCPANA@put?9away?$AA@ (0000)
0027F054 0006:
	??_C@_05GLLMDLIO@ready?$AA@ (0000)
0027F05C 0005:
	??_C@_04PIONEEJD@drop?$AA@ (0000)
0027F064 0007:
	??_C@_06IIFPLFEI@disarm?$AA@ (0000)
0027F06C 000e:
	??_C@_0O@OOCNGGMI@throw?9grenade?$AA@ (0000)
0027F07C 000a:
	??_C@_09OBCAJEHA@land?9hard?$AA@ (0000)
0027F088 000a:
	??_C@_09EHCCFMCN@land?9soft?$AA@ (0000)
0027F094 0009:
	??_C@_08CDBGFLDG@airborne?$AA@ (0000)
0027F0A0 000c:
	??_C@_0M@OOICDHNM@slide?9right?$AA@ (0000)
0027F0AC 000b:
	??_C@_0L@KJINOEBG@slide?9left?$AA@ (0000)
0027F0B8 000b:
	??_C@_0L@FCEOABFF@slide?9back?$AA@ (0000)
0027F0C4 000c:
	??_C@_0M@KAMMIFJP@slide?9front?$AA@ (0000)
0027F0D0 000b:
	??_C@_0L@ODKCNGCL@move?9right?$AA@ (0000)
0027F0DC 000a:
	??_C@_09OEAGCNJM@move?9left?$AA@ (0000)
0027F0E8 000a:
	??_C@_09BPMFMINP@move?9back?$AA@ (0000)
0027F0F4 000b:
	??_C@_0L@KNOMGEGI@move?9front?$AA@ (0000)
0027F100 000b:
	??_C@_0L@NLLPCKKK@dive?9front?$AA@ (0000)
0027F10C 000b:
	??_C@_0L@GBDHABO@turn?9right?$AA@ (0000)
0027F118 000a:
	??_C@_09IABOBFFB@turn?9left?$AA@ (0000)
0027F124 0008:
	??_C@_07LIHKAGE@gesture?$AA@ (0000)
0027F12C 0009:
	??_C@_08MIILNEOD@overheat?$AA@ (0000)
0027F138 000a:
	??_C@_09ILCKEMCI@charged?92?$AA@ (0000)
0027F144 000a:
	??_C@_09KAAHBPOL@charged?91?$AA@ (0000)
0027F150 0007:
	??_C@_06PHABDCJA@fire?92?$AA@ (0000)
0027F158 0007:
	??_C@_06NMCMGBFD@fire?91?$AA@ (0000)
0027F160 000a:
	??_C@_09DPADDJPP@chamber?92?$AA@ (0000)
0027F16C 000a:
	??_C@_09BECOGKDM@chamber?91?$AA@ (0000)
0027F178 0009:
	??_C@_08CHOPIKKI@reload?92?$AA@ (0000)
0027F184 0009:
	??_C@_08MMCNJGL@reload?91?$AA@ (0000)
0027F190 0035:
	??_C@_0DF@EOKADLMP@frame_index?$DO?$DN0?5?$CG?$CG?5frame_index?$DMan@ (0000)
0027F1C8 0034:
	??_C@_0DE@OOMCLPDD@c?3?2halo?2SOURCE?2models?2model_anim@ (0000)
0027F1FC 000b:
	??_C@_0L@JEIPCDKP@?$CD?$DMinvalid?$DO?$AA@ (0000)
00309E80 0508:
	_data_00309e80 (0000)
	_weapon_type_animation_list (0050)
	_weapon_class_animation_list (0210)
	_damage_type_strings (0218)
	_damage_direction_strings (0228)
	_damage_part_strings (0238)
	_unit_seat_animation_list (0358)
	_first_person_weapon_animation_list (0440)
	_weapon_animation_list (04a0)
	_vehicle_animation_list (04e8)
	_device_animation_list (0500)
*/

/* ---------- headers */

#include "cseries.h"
#include "model_animation_definitions.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

extern boolean hs_model_animation_compression_enabled;

typedef char verify_animation_size[
	sizeof(struct animation) == 0xB4 ? 1 : -1];
typedef char verify_animation_frame_count_offset[
	offsetof(struct animation, frame_count) == 0x22 ? 1 : -1];
typedef char verify_animation_frame_size_offset[
	offsetof(struct animation, frame_size) == 0x24 ? 1 : -1];
typedef char verify_animation_flags_offset[
	offsetof(struct animation, flags) == 0x3A ? 1 : -1];
typedef char verify_animation_frame_info_offset[
	offsetof(struct animation, frame_info) == 0x48 ? 1 : -1];
typedef char verify_animation_compressed_data_offset_offset[
	offsetof(struct animation, compressed_data_offset) == 0x88 ? 1 : -1];
typedef char verify_animation_data_offset[
	offsetof(struct animation, data) == 0xA0 ? 1 : -1];
typedef char verify_animation_list_entry_size[
	sizeof(struct animation_list_entry) == 0x08 ? 1 : -1];
typedef char verify_animation_list_size[
	sizeof(struct animation_list) == 0x08 ? 1 : -1];
typedef char verify_animation_list_animations_offset[
	offsetof(struct animation_list, animations) == 0x04 ? 1 : -1];

static struct animation_list_entry weapon_type_animation_list_entries[] =
{
	{ "reload-1", _animation_replacement },
	{ "reload-2", _animation_replacement },
	{ "chamber-1", _animation_overlay },
	{ "chamber-2", _animation_overlay },
	{ "fire-1", _animation_overlay },
	{ "fire-2", _animation_overlay },
	{ "charged-1", _animation_overlay },
	{ "charged-2", _animation_overlay },
	{ "melee", _animation_overlay },
	{ "overheat", _animation_overlay },
};

struct animation_list weapon_type_animation_list = { NUMBEROF(weapon_type_animation_list_entries), weapon_type_animation_list_entries };

static struct animation_list_entry weapon_class_animation_list_entries[] =
{
	{ "idle", _animation_base },
	{ "gesture", _animation_base },
	{ "turn-left", _animation_base },
	{ "turn-right", _animation_base },
	{ "dive-front", _animation_base },
	{ "dive-back", _animation_base },
	{ "dive-left", _animation_base },
	{ "dive-right", _animation_base },
	{ "move-front", _animation_base },
	{ "move-back", _animation_base },
	{ "move-left", _animation_base },
	{ "move-right", _animation_base },
	{ "slide-front", _animation_base },
	{ "slide-back", _animation_base },
	{ "slide-left", _animation_base },
	{ "slide-right", _animation_base },
	{ "airborne", _animation_base },
	{ "land-soft", _animation_base },
	{ "land-hard", _animation_base },
	{ "unused", _animation_overlay },
	{ "throw-grenade", _animation_base },
	{ "disarm", _animation_replacement },
	{ "drop", _animation_replacement },
	{ "ready", _animation_replacement },
	{ "put-away", _animation_replacement },
	{ "aim-still", _animation_overlay },
	{ "aim-move", _animation_overlay },
	{ "surprise-front", _animation_base },
	{ "surprise-back", _animation_base },
	{ "berserk", _animation_base },
	{ "evade-left", _animation_base },
	{ "evade-right", _animation_base },
	{ "signal-move", _animation_base },
	{ "signal-attack", _animation_base },
	{ "warn", _animation_base },
	{ "stunned-front", _animation_base },
	{ "stunned-back", _animation_base },
	{ "stunned-left", _animation_base },
	{ "stunned-right", _animation_base },
	{ "melee", _animation_base },
	{ "celebrate", _animation_base },
	{ "panic", _animation_base },
	{ "melee-airborne", _animation_base },
	{ "flaming", _animation_base },
	{ "resurrect-front", _animation_base },
	{ "resurrect-back", _animation_base },
	{ "melee-continuous", _animation_base },
	{ "feeding", _animation_base },
	{ "leap-start", _animation_base },
	{ "leap-airborne", _animation_base },
	{ "leap-melee", _animation_base },
	{ "zapping", _animation_base },
	{ "unused", _animation_base },
	{ "unused", _animation_base },
	{ "unused", _animation_base },
};

struct animation_list weapon_class_animation_list = { NUMBEROF(weapon_class_animation_list_entries), weapon_class_animation_list_entries };

char *damage_type_strings[] = { "s-ping", "h-ping", "s-kill", "h-kill" };
char *damage_direction_strings[] = { "front", "left", "right", "back" };
char *damage_part_strings[] =
{
	"gut", "chest", "head",
	"l-arm", "l-hand", "l-leg", "l-foot",
	"r-arm", "r-hand", "r-leg", "r-foot",
};

static struct animation_list_entry unit_seat_animation_list_entries[] =
{
	{ "airborne-dead", _animation_base },
	{ "landing-dead", _animation_base },
	{ "acc-front-back", _animation_overlay },
	{ "acc-left-right", _animation_overlay },
	{ "acc-up-down", _animation_overlay },
	{ "push", _animation_overlay },
	{ "twist", _animation_overlay },
	{ "enter", _animation_base },
	{ "exit", _animation_base },
	{ "look", _animation_overlay },
	{ "talk", _animation_overlay },
	{ "emotions", NONE },
	{ "unused", NONE },
	{ "user0", NONE },
	{ "user1", NONE },
	{ "user2", NONE },
	{ "user3", NONE },
	{ "user4", NONE },
	{ "user5", NONE },
	{ "user6", NONE },
	{ "user7", NONE },
	{ "user8", NONE },
	{ "user9", NONE },
	{ "flying-front", _animation_base },
	{ "flying-back", _animation_base },
	{ "flying-left", _animation_base },
	{ "flying-right", _animation_base },
	{ "opening", _animation_base },
	{ "closing", _animation_base },
	{ "hovering", _animation_base }
};

struct animation_list unit_seat_animation_list = { NUMBEROF(unit_seat_animation_list_entries), unit_seat_animation_list_entries };

static struct animation_list_entry first_person_weapon_animation_list_entries[] =
{
	{ "idle", _animation_base },
	{ "posing", _animation_base },
	{ "fire-1", _animation_base },
	{ "moving", _animation_overlay },
	{ "overlays", _animation_overlay },
	{ "light-off", _animation_base },
	{ "light-on", _animation_base },
	{ "reload-empty", _animation_base },
	{ "reload-full", _animation_base },
	{ "overheated", _animation_base },
	{ "ready", _animation_base },
	{ "put-away", _animation_base },
	{ "overcharged", _animation_base },
	{ "melee", _animation_base },
	{ "fire-2", _animation_base },
	{ "overcharged-jitter", _animation_overlay },
	{ "throw-grenade", _animation_base },
	{ "ammunition", _animation_overlay },
	{ "misfire-1", _animation_base },
	{ "misfire-2", _animation_base },
	{ "throw-overheated", _animation_base },
	{ "overheating", _animation_base },
	{ "overheating-again", _animation_base },
	{ "enter", _animation_base },
	{ "exit-empty", _animation_base },
	{ "exit-full", _animation_base },
	{ "o-h-exit", _animation_base },
	{ "o-h-s-enter", _animation_base },
};

struct animation_list first_person_weapon_animation_list = { NUMBEROF(first_person_weapon_animation_list_entries), first_person_weapon_animation_list_entries };

static struct animation_list_entry weapon_animation_list_entries[] =
{
	{ "idle", _animation_base },
	{ "ready", _animation_base },
	{ "put-away", _animation_base },
	{ "reload-1", _animation_base },
	{ "reload-2", _animation_base },
	{ "chamber-1", _animation_base },
	{ "chamber-2", _animation_base },
	{ "charged-1", _animation_base },
	{ "charged-2", _animation_base },
	{ "fire-1", _animation_base },
	{ "fire-2", _animation_base },
};

struct animation_list weapon_animation_list = { NUMBEROF(weapon_animation_list_entries), weapon_animation_list_entries };

static struct animation_list_entry vehicle_animation_list_entries[] =
{
	{ "steering", _animation_overlay },
	{ "roll", _animation_overlay },
	{ "throttle", _animation_overlay },
	{ "velocity", _animation_overlay },
	{ "braking", _animation_overlay },
	{ "ground-speed", _animation_overlay },
	{ "occupied", _animation_overlay },
	{ "unoccupied", _animation_overlay },
};

struct animation_list vehicle_animation_list = { NUMBEROF(vehicle_animation_list_entries), vehicle_animation_list_entries };

static struct animation_list_entry device_animation_list_entries[] =
{
	{ "position", _animation_overlay },
	{ "power", _animation_overlay },
};

struct animation_list device_animation_list = { NUMBEROF(device_animation_list_entries), device_animation_list_entries };

/* ---------- public code */

byte *animation_get_frame_data(struct animation const *animation, short frame_index)
{
	byte *frame_data;
	boolean compressed;

	compressed = TEST_FLAG(animation->flags, _animation_compressed_bit) && hs_model_animation_compression_enabled;
#ifdef HALO_N64
	/* frames stream from the cartridge (port/n64/src/n64_anim.c) */
	if (animation->data.pad & 1)
	{
		extern void *n64_animation_frame(long data_offset, long data_size, short frame_index, short frame_size);

		return n64_animation_frame(animation->data.file_offset, animation->data.size, frame_index, animation->frame_size);
	}
#endif
	frame_data = tag_data_get_pointer(&animation->data, 0, 0);

	match_assert("c:\\halo\\SOURCE\\models\\model_animation_definitions.c", 1147, frame_index>=0 && frame_index<animation->frame_count);

	if (compressed)
		frame_data += animation->compressed_data_offset;
	else
		frame_data += frame_index * animation->frame_size;

	return frame_data;
}

byte *animation_get_frame_info(struct animation const *animation, short frame_index, short frame_info_size)
{
	match_assert("c:\\halo\\SOURCE\\models\\model_animation_definitions.c", 1167, frame_index>=0 && frame_index<animation->frame_count);
#ifdef HALO_N64
	/* streamed from the cartridge like the frames (animation_get_frame_data) */
	if (animation->frame_info.pad & 1)
		return n64_animation_frame(animation->frame_info.file_offset, animation->frame_info.size, frame_index, frame_info_size);
#endif
	return tag_data_get_pointer(&animation->frame_info, frame_index * frame_info_size, frame_info_size);
}

char const *animation_list_get_string(struct animation_list const *animation_list, short animation_index)
{
	if (animation_index < animation_list->count && animation_list->animations[animation_index].name)
		return animation_list->animations[animation_index].name;

	return "#<invalid>";
}

/* ---------- private code */
