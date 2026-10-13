/*
XDK_XBDM.H

Debug monitor declarations the game and the platform layer use.
(README.md)
*/

#ifndef HALO_XDK_XBDM_H
#define HALO_XDK_XBDM_H

#include "xdk_win32.h"

/* ---------- macros */

/* Debug monitor results: HRESULTs of facility 0x2db.
Source: cachebeta.exe, the start-up module walk (0x580ab0, shell_xbox.c)
compares every DmWalkLoadedModules and DmWalkModuleSections result with
0x82db0104, the end of a list. XBDM_NOERR is the facility's plain success
code (severity 0, code 0); nothing in the game tests it, only
port/linux/src/xbdm.c returns it. */
#define XBDM_NOERR ((HRESULT)0x02DB0000L)
#define XBDM_ENDOFLIST ((HRESULT)0x82DB0104L)

/* ---------- functions */

/* A loaded module, as DmWalkLoadedModules reports it.
Source: cachebeta.exe, the start-up module walk: the game passes the
record's start as the module name to DmWalkModuleSections, and gives the
record 0x118 bytes of its frame: a MAX_PATH name and five 32-bit fields.
Only Name is used; the other fields are named here for what a module
record describes. */
typedef struct _DMN_MODLOAD
{
	char Name[260];
	void *BaseAddress;
	unsigned long Size;
	unsigned long TimeStamp;
	unsigned long CheckSum;
	unsigned long Flags;
} DMN_MODLOAD, *PDMN_MODLOAD;

/* A section of a loaded module, as DmWalkModuleSections reports it.
Source: cachebeta.exe, the start-up module walk: it reads BaseAddress at
offset 0x104 and Size at 0x108, and the record has at most 0x110 bytes of
the frame. Index and Flags fill the rest; the game does not use them. */
typedef struct _DMN_SECTIONLOAD
{
	char Name[260];
	void *BaseAddress;
	unsigned long Size;
	unsigned short Index;
	unsigned short Flags;
} DMN_SECTIONLOAD, *PDMN_SECTIONLOAD;

/* the walks' opaque cursors: start from NULL */
typedef struct _DM_WALK_MODULES *PDM_WALK_MODULES;
typedef struct _DM_WALK_MODSECT *PDM_WALK_MODSECT;

/* Module and section walks.
Source: cachebeta.exe's imports from xbdm.dll (_DmWalkLoadedModules@8,
_DmWalkModuleSections@12, _DmCloseModuleSections@4: __stdcall) and the
game's calls; they match port/linux/src/xbdm.c. */
HRESULT __stdcall DmWalkLoadedModules(PDM_WALK_MODULES *walk, PDMN_MODLOAD module);
HRESULT __stdcall DmWalkModuleSections(PDM_WALK_MODSECT *walk, const char *module_name, PDMN_SECTIONLOAD section);
HRESULT __stdcall DmCloseModuleSections(PDM_WALK_MODSECT walk);

#endif
