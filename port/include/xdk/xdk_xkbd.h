/*
XDK_XKBD.H

Debug keyboard declarations the game uses (README.md). As in the SDK, a
unit gets them by defining DEBUG_KEYBOARD before including xbox.h or
xkbd.h.
*/

#ifdef DEBUG_KEYBOARD
#ifndef HALO_XDK_XKBD_H
#define HALO_XDK_XKBD_H

#include "xdk_win32.h"

/* ---------- macros */

/* The debug keyboard's device type: the address of its table.
Source: cachebeta.exe: the game passes 0x63ae48, the public symbol
_XDEVICE_TYPE_DEBUG_KEYBOARD_TABLE, to XGetDeviceChanges, XInputOpen and
XInitDevices. */
#define XDEVICE_TYPE_DEBUG_KEYBOARD (&XDEVICE_TYPE_DEBUG_KEYBOARD_TABLE)

/* Which key events XInputDebugInitKeyboardQueue queues
(XINPUT_DEBUG_KEYQUEUE_PARAMETERS.dwFlags).
Source: cachebeta.exe, the input library's keyboard report handler
(0x642994): bit 0x1 queues newly pressed keys and bit 0x4 released ones
(which it marks KEYUP); with no parameters the library defaults to 0x3,
key downs and repeats, so repeats are 0x2. The game's own request
(input_initialize, 0x4bf560) is 0x7, all three. */
#define XINPUT_DEBUG_KEYQUEUE_FLAG_KEYDOWN 0x00000001
#define XINPUT_DEBUG_KEYQUEUE_FLAG_KEYREPEAT 0x00000002
#define XINPUT_DEBUG_KEYQUEUE_FLAG_KEYUP 0x00000004

/* XINPUT_DEBUG_KEYSTROKE.Flags.
Source: cachebeta.exe: the input library (0x642a79...) builds the flags
from the USB modifier byte as ctrl 0x01, shift 0x02, alt 0x04 and marks
key releases 0x40; input_update_keyboard_devices (static, at 0x4bf150)
reads the same bits. port/linux/src/sdl_platform.c produces these values. */
#define XINPUT_DEBUG_KEYSTROKE_FLAG_CTRL 0x01
#define XINPUT_DEBUG_KEYSTROKE_FLAG_SHIFT 0x02
#define XINPUT_DEBUG_KEYSTROKE_FLAG_ALT 0x04
#define XINPUT_DEBUG_KEYSTROKE_FLAG_KEYUP 0x40

/* ---------- functions */

/* the debug keyboard's device type table: a data symbol of
cachebeta.exe; port/linux/src/xinput_sdl.c defines it for the native
ports */
extern XPP_DEVICE_TYPE XDEVICE_TYPE_DEBUG_KEYBOARD_TABLE;

#endif
#endif
