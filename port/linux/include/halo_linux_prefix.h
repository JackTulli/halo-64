/*
HALO_LINUX_PREFIX.H

Force-included ahead of every translation unit in the native Linux build
(clang -include). It reproduces the handful of MSVC/XDK environment
assumptions that the game source relies on, so the source itself can stay
byte-for-byte identical to what the matching MSVC build compiles.
*/

#ifndef __HALO_LINUX_PREFIX_H
#define __HALO_LINUX_PREFIX_H

#if !defined(__i386__) && !defined(HALO_ANDROID)
#error the Linux port targets 32-bit x86: game data structures assume 32-bit pointers
#endif

#define HALO_LINUX 1

/* ---------- XDK architecture selection (MSVC predefines these) */

#define _X86_ 1
#define _M_IX86 600
#define _STDCALL_SUPPORTED 1
#define _INTEGRAL_MAX_BITS 64
#define _WCHAR_T_DEFINED
#define _USE_MATH_DEFINES
/* the XDK's COM headers decorate methods with __export when _WIN32 is unset */
#define __export

/* ---------- MSVC inline semantics

MSVC gives C `__inline` functions COMDAT (pick-any) linkage. ELF C has no
equivalent, so every translation unit gets its own private copy instead.
Clang only warns about the resulting `static static`. */

#define __inline static __inline__
#define _inline static __inline__
#define __forceinline static __inline__ __attribute__((always_inline))

/* An inline function that also has an ordinary prototype keeps external
linkage; the generated halo_msvc_semantics.h marks every inline function
name `#pragma weak`, making those definitions pick-any like a COMDAT. */

/* glibc spells its own extern-inline helpers with __inline; keep it from
emitting them so the redefinition above cannot reach them. */
#define __NO_INLINE__ 1

/* ---------- __declspec(selectany) data (XDK D3DCONST tables) */

#define DECLSPEC_SELECTANY __attribute__((weak))

/* ---------- MSVC intrinsics

Clang predeclares the MSVC _Interlocked* builtins with prototypes that
conflict with the XDK's WINAPI declarations; route the XDK names to the
platform layer instead. */

#define _InterlockedCompareExchange halo_linux_InterlockedCompareExchange
#define _InterlockedDecrement halo_linux_InterlockedDecrement
#define _InterlockedExchange halo_linux_InterlockedExchange
#define _InterlockedExchangeAdd halo_linux_InterlockedExchangeAdd
#define _InterlockedIncrement halo_linux_InterlockedIncrement

/* ---------- structured exception handling

Only the top-level crash handler in main() uses SEH. POSIX has no equivalent
(the platform layer installs signal handlers instead), so the guarded block
always runs and the handler is compiled out. */

#define __try if (1)
#define __except(filter) else if (0)
#define __finally
#define __leave

/* ---------- multiplayer session limits of the native builds */

#include "halo_port_limits.h"

/* the Xbox Winsock headers' fd_set in game units (platform units see glibc's,
which is larger) */
#ifndef HALO_LINUX_PLATFORM_LAYER
#define FD_SETSIZE HALO_PORT_FD_SETSIZE
#endif

/* ---------- Winsock

Game code sees the XDK's Winsock under private names (see the header). The
platform layer includes the XDK headers itself, via platform.h. */

#ifndef HALO_LINUX_PLATFORM_LAYER
#include "halo_linux_winsock_names.h"
#include "halo_linux_source_fixups.h"
#endif

/* ---------- MSVC built-in types */

#include <stddef.h>

#endif /* __HALO_LINUX_PREFIX_H */
