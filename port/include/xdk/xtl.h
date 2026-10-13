/* xtl.h: the Xbox SDK's umbrella header (README.md). As in the SDK, NOD3D
and NODSOUND leave out Direct3D and DirectSound. */

#include "xdk_win32.h"
#include "xdk_xbox.h"
#ifndef NOD3D
#include "xdk_d3d8.h"
#endif
#ifndef NODSOUND
#include "xdk_dsound.h"
#endif
#include "xdk_winsock.h"
