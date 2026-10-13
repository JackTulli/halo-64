/*
XDK_XBOX.H

Xbox system declarations the game and the platform layer use.
(README.md)
*/

#ifndef HALO_XDK_XBOX_H
#define HALO_XDK_XBOX_H

#include "xdk_win32.h"

/* ---------- macros */

/* Gamepad digital buttons: bits of XINPUT_GAMEPAD.wButtons.
Source: cachebeta.exe, input_xbox.c's gamepad_binary_button_masks table
(.rdata 0x66f578: 01 02 04 08 10 20 40 80, in the order the source lists
the names); agrees with the Xbox Dev Wiki's controller report layout. */
#define XINPUT_GAMEPAD_DPAD_UP 0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN 0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT 0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT 0x0008
#define XINPUT_GAMEPAD_START 0x0010
#define XINPUT_GAMEPAD_BACK 0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB 0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB 0x0080

/* Gamepad analog buttons: indices into XINPUT_GAMEPAD.bAnalogButtons.
Source: cachebeta.exe, input_xbox.c's gamepad_analog_button_indices table
(.rdata 0x66f570: 00 01 02 03 04 05 06 07, in source order). */
#define XINPUT_GAMEPAD_A 0
#define XINPUT_GAMEPAD_B 1
#define XINPUT_GAMEPAD_X 2
#define XINPUT_GAMEPAD_Y 3
#define XINPUT_GAMEPAD_BLACK 4
#define XINPUT_GAMEPAD_WHITE 5
#define XINPUT_GAMEPAD_LEFT_TRIGGER 6
#define XINPUT_GAMEPAD_RIGHT_TRIGGER 7

/* Controller ports (numbers passed to XInputOpen, and bit numbers in the
XGetDeviceChanges masks of port devices) and the slot argument for a
device without slots.
Source: cachebeta.exe, input_get_device_states (static, at 0x4bebd0): it
opens gamepads with XInputOpen(type, port index, 0, NULL) and tests the
change masks with 1 << port. */
#define XDEVICE_PORT0 0
#define XDEVICE_PORT1 1
#define XDEVICE_PORT2 2
#define XDEVICE_PORT3 3
#define XDEVICE_NO_SLOT 0

/* Masks of XGetDeviceChanges. A port device reports port n as bit n; a
memory unit reports the top slot of port n as bit n and the bottom slot as
bit n + 16.
Source: cachebeta.exe, input_get_device_states: the memory unit
masks are tested in source order as 0x1, 0x10000, 0x2, 0x20000, 0x4,
0x40000, 0x8, 0x80000; the gamepad masks as 1 << port. */
#define XDEVICE_PORT0_MASK 0x00000001
#define XDEVICE_PORT1_MASK 0x00000002
#define XDEVICE_PORT2_MASK 0x00000004
#define XDEVICE_PORT3_MASK 0x00000008
#define XDEVICE_PORT0_TOP_MASK 0x00000001
#define XDEVICE_PORT1_TOP_MASK 0x00000002
#define XDEVICE_PORT2_TOP_MASK 0x00000004
#define XDEVICE_PORT3_TOP_MASK 0x00000008
#define XDEVICE_PORT0_BOTTOM_MASK 0x00010000
#define XDEVICE_PORT1_BOTTOM_MASK 0x00020000
#define XDEVICE_PORT2_BOTTOM_MASK 0x00040000
#define XDEVICE_PORT3_BOTTOM_MASK 0x00080000

/* Device types are the addresses of the input library's per-type tables.
Source: cachebeta.exe: the game passes 0x63ae3c and 0x63adb8, the public
symbols _XDEVICE_TYPE_GAMEPAD_TABLE and _XDEVICE_TYPE_MEMORY_UNIT_TABLE, as
the type argument of XGetDeviceChanges, XInputOpen and XInitDevices. The
debug keyboard's type is in xdk_xkbd.h. */
#define XDEVICE_TYPE_GAMEPAD (&XDEVICE_TYPE_GAMEPAD_TABLE)
#define XDEVICE_TYPE_MEMORY_UNIT (&XDEVICE_TYPE_MEMORY_UNIT_TABLE)

/* XGetLanguage's results.
Source: cachebeta.exe, attract_mode_get_localized_movie_path (0x4cb4a0):
its jump table maps 1 to 6 onto the game's _english, _japanese, _german,
_french, _spanish and _italian; the Xbox Dev Wiki's EEPROM page lists the
same numbering for the language setting. */
#define XC_LANGUAGE_ENGLISH 1
#define XC_LANGUAGE_JAPANESE 2
#define XC_LANGUAGE_GERMAN 3
#define XC_LANGUAGE_FRENCH 4
#define XC_LANGUAGE_SPANISH 5
#define XC_LANGUAGE_ITALIAN 6

/* Launch data types (XGetLaunchInfo) and dashboard launch reasons
(LD_LAUNCH_DASHBOARD.dwReason).
Source: cachebeta.exe: shell_platform_initialize (0x580a00) tests the type
against 0 for LDT_TITLE; xbox_dashboard_launch (0x4cfaa0) stores 2 for
XLD_LAUNCH_DASHBOARD_MEMORY and 0 for XLD_LAUNCH_DASHBOARD_MAIN_MENU. The
Xbox Dev Wiki's Kernel/LaunchDataPage page gives type 2 as "switch from
dashboard", which XGetLaunchInfo (0x5c213c) also accepts without a title
check; it numbers the dashboard reasons the same way (0 main, 2 saved
data). */
#define LDT_TITLE 0
#define LDT_FROM_DASHBOARD 2
#define XLD_LAUNCH_DASHBOARD_MAIN_MENU 0
#define XLD_LAUNCH_DASHBOARD_MEMORY 2

/* Characters (terminator included) in a saved game's name and in a
nickname.
Source: cachebeta.pdb, XGAME_FIND_DATA.szSaveGameName is WCHAR[128];
cachebeta.exe, XSetNicknameW (0x5c2411) copies at most 32 characters into
its record, and the game passes 32 to XFindFirstNicknameW. */
#define MAX_GAMENAME 128
#define MAX_NICKNAME 32

/* ---------- functions */

/* The input library's device type tables (see XDEVICE_TYPE_*): data
symbols of cachebeta.exe; port/linux/src/xinput_sdl.c defines them for
the native ports. */
extern XPP_DEVICE_TYPE XDEVICE_TYPE_GAMEPAD_TABLE;
extern XPP_DEVICE_TYPE XDEVICE_TYPE_MEMORY_UNIT_TABLE;

/* the debug keyboard, for units that ask for it (DEBUG_KEYBOARD) */
#include "xdk_xkbd.h"

#endif
