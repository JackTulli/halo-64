/*
HALO_N64_PREFIX.H

Force-included ahead of every game unit in the Nintendo 64 build (gcc
-include). Like the Linux prefix it reproduces the MSVC/XDK environment the
game source assumes; the N64 is big-endian MIPS (VR4300) with 32-bit
pointers and longs (libdragon's o64 ABI), so the game's C structures keep
their Xbox layouts but every multi-byte value in memory is big-endian. Tag
data is byte-swapped ahead of time by tools/n64_map.py.
*/

#ifndef __HALO_N64_PREFIX_H
#define __HALO_N64_PREFIX_H

#if !defined(__mips__) || !defined(__MIPSEB__)
#error the N64 build targets big-endian MIPS
#endif

#define HALO_N64 1
/* the native (non-MSVC) build of the game: the game sources use this for
the places where other compilers and MSVC differ, as on Linux and the Xbox
port */
#define HALO_LINUX 1

/* ---------- XDK architecture selection (the SDK declarations need one) */

#define _X86_ 1
#define _STDCALL_SUPPORTED 1
#define _INTEGRAL_MAX_BITS 64
/* gcc's <stddef.h> leaves wchar_t out when _WCHAR_T_DEFINED is set (the
XDK headers test it), so it is declared here first (-fshort-wchar: 16 bits) */
typedef __WCHAR_TYPE__ wchar_t;
#define _WCHAR_T_DEFINED
#define _USE_MATH_DEFINES
#define __export

/* ---------- calling conventions and MSVC keywords that mean nothing here */

#define __cdecl
#define __stdcall
#define __fastcall
#define _cdecl
#define _stdcall
#define _fastcall
#define __declspec(x)
#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long
#define __unaligned
#define __w64
#define __ptr32

/* ---------- MSVC inline semantics

MSVC emits a C `__inline` function with external linkage as a pick-any
COMDAT. Under gcc's gnu89 inline rules a plain `__inline__` definition is an
external one in every unit; the generated semantics header marks each such
name `#pragma weak`, the ELF analogue of a COMDAT (tools/
linux_msvc_semantics.py --exclude-static). Functions the source declares
`static __inline` stay private. */

#define __inline __inline__
#define _inline __inline__
#define __forceinline __inline__ __attribute__((always_inline))
#define __NO_INLINE__ 1

#define DECLSPEC_SELECTANY __attribute__((weak))

#define _InterlockedCompareExchange halo_linux_InterlockedCompareExchange
#define _InterlockedDecrement halo_linux_InterlockedDecrement
#define _InterlockedExchange halo_linux_InterlockedExchange
#define _InterlockedExchangeAdd halo_linux_InterlockedExchangeAdd
#define _InterlockedIncrement halo_linux_InterlockedIncrement

/* ---------- names libdragon also defines (its text console) */

#define console_close halo_console_close

/* ---------- structured exception handling: compiled out */

#define __try if (1)
#define __except(filter) else if (0)
#define __finally
#define __leave

/* ---------- the N64's limits (4 local players, small pools) */

#include "halo_n64_limits.h"

#ifndef HALO_LINUX_PLATFORM_LAYER
#define FD_SETSIZE HALO_PORT_FD_SETSIZE
#include "../../linux/include/halo_linux_winsock_names.h"
#include "../../linux/include/halo_linux_source_fixups.h"
#endif

#include <stddef.h>

#endif /* __HALO_N64_PREFIX_H */
