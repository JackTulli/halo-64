/*
PLATFORM.H

Shared internals of the Nintendo 64 platform layer's XDK-facing half
(port/n64/src/xdk_*.c, and the Linux units the N64 build compiles again):
the parts of the Xbox SDK and the MSVC runtime the game calls, over
libdragon. These files are compiled with the game's ABI and see the XDK
declarations (port/include/xdk), so every definition is type-checked against
the SDK prototype it replaces. libdragon is reached through n64.h, whose
implementations (port/n64/src/n64_*.c) are compiled against libdragon's
headers instead.

port/linux/src/platform.h defers to this header under HALO_N64.
*/

#ifndef __HALO_N64_PLATFORM_H
#define __HALO_N64_PLATFORM_H

#define DEBUG_KEYBOARD
#include "../../linux/include/halo_linux_winsock_names.h"
#include <xtl.h>
#include <xbdm.h>
#include <d3d8perf.h>
#define HALO_LINUX_WINSOCK_NAMES_UNDEFINE
#include "../../linux/include/halo_linux_winsock_names.h"
#undef HALO_LINUX_WINSOCK_NAMES_UNDEFINE

#include "n64.h"

/* ---------- logging */

/* prints a line to the debug output (ares' ISViewer / a flash cart's USB) */
void platform_log(const char *format, ...) __attribute__((format(printf, 1, 2)));

/* reports an SDK entry point the N64 build does not implement, once per
name; the caller then behaves as a failing or empty call */
void platform_unimplemented(const char *name);
#define PLATFORM_UNIMPLEMENTED() platform_unimplemented(__func__)

#endif /* __HALO_N64_PLATFORM_H */
