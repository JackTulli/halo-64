/*
XDK_WIN32.H

Win32 declarations the game and the platform layer use.
(README.md)
*/

#ifndef HALO_XDK_WIN32_H
#define HALO_XDK_WIN32_H

/* C runtime headers the game uses without including them itself:
structured exception handling's filter results and helpers
(EXCEPTION_EXECUTE_HANDLER in source/cseries/cseries_windows.c), and the
integer limits (SCHAR_MAX in source/units/bipeds.c), and the character
classes (isalpha in source/tag_files/files_windows.c). Every game unit reads
cseries.h, whose own LONG_MAX and CHAR_MAX enumerators come first, before
these headers. */
#include <ctype.h>
#include <excpt.h>
#include <limits.h>

/* Winsock's fd_set capacity, unless the unit or its port sets its own
(learn.microsoft.com, select: 64 by default). It comes before xdk_pdb.h,
where struct fd_set is. */
#ifndef FD_SETSIZE
#define FD_SETSIZE 64
#endif

#include "xdk_pdb.h"

/* ---------- macros */

/* Calling conventions and spelling helpers. Every Win32 and COM entry point
on x86 is __stdcall (learn.microsoft.com, "Windows Data Types" and
"Argument Passing and Naming Conventions"); the PDB's prototypes in
xdk_pdb.h agree. */
#define WINAPI __stdcall
#define CALLBACK __stdcall
#define NTAPI __stdcall
#define PASCAL __stdcall
/* APIENTRY is WINAPI (learn.microsoft.com, "Windows Data Types"; mingw-w64
minwindef.h, public domain). The platform's OpenGL header (port/linux/src/gl.h)
relies on it: with APIENTRY defined, SDL's OpenGL header does not include
the host's windows.h on Windows. */
#define APIENTRY WINAPI
#define STDMETHODCALLTYPE __stdcall

#define VOID void
#define CONST const
/* as cseries.h spells them, so either may come first */
#define TRUE 1
#define FALSE 0

/* the longest path the file functions take (learn.microsoft.com, "Maximum
Path Length Limitation") */
#define MAX_PATH 260

/* a WORD from its low and high bytes (learn.microsoft.com, MAKEWORD) */
#define MAKEWORD(low, high) \
	((WORD)(((WORD)(BYTE)(low)) | (((WORD)(BYTE)(high)) << 8)))

/* The ANSI functions under their generic names; the game is built without
UNICODE (learn.microsoft.com, "Conventions for Function Prototypes"). */
#define CreateEvent CreateEventA
#define CreateFile CreateFileA
#define GetDiskFreeSpaceEx GetDiskFreeSpaceExA
#define OutputDebugString OutputDebugStringA

/* The current thread's pseudo handle (learn.microsoft.com,
GetCurrentThread). The platform layer treats -1 and -2 as the process and
thread pseudo handles (port/linux/src/xbox_kernel.c). */
#define GetCurrentThread() ((HANDLE)(LONG)-2)

/* Critical sections are the kernel's Rtl* routines (see the functions below
and port/linux/src/xbox_kernel.c). */
#define InitializeCriticalSection RtlInitializeCriticalSection
#define EnterCriticalSection RtlEnterCriticalSection
#define LeaveCriticalSection RtlLeaveCriticalSection
#define TryEnterCriticalSection RtlTryEnterCriticalSection

/* The Interlocked* functions under the names the port prefix headers route
to the platform layer (halo_linux_prefix.h, halo_windows_prefix.h). */
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedExchange _InterlockedExchange
#define InterlockedExchangeAdd _InterlockedExchangeAdd
#define InterlockedCompareExchange _InterlockedCompareExchange

/* System error codes (learn.microsoft.com, "System Error Codes (0-499)",
"(500-999)", "(1000-1299)", "(1300-1699)", "(1700-3999)"; the same values as
mingw-w64 winerror.h, public domain). The Xbox's Win32 subset reports the
same codes; the game's winsock_error_to_string switch in cachebeta.exe
tests 6, 8, 87 and 995-997. */
#define ERROR_SUCCESS 0L
#define ERROR_FILE_NOT_FOUND 2L
#define ERROR_PATH_NOT_FOUND 3L
#define ERROR_TOO_MANY_OPEN_FILES 4L
#define ERROR_ACCESS_DENIED 5L
#define ERROR_INVALID_HANDLE 6L
#define ERROR_NOT_ENOUGH_MEMORY 8L
#define ERROR_NO_MORE_FILES 18L
#define ERROR_GEN_FAILURE 31L
#define ERROR_HANDLE_EOF 38L
#define ERROR_INVALID_PARAMETER 87L
#define ERROR_DISK_FULL 112L
#define ERROR_INSUFFICIENT_BUFFER 122L
#define ERROR_DIR_NOT_EMPTY 145L
#define ERROR_BUSY 170L
#define ERROR_ALREADY_EXISTS 183L
#define ERROR_NOT_OWNER 288L
#define ERROR_OPERATION_ABORTED 995L
#define ERROR_IO_INCOMPLETE 996L
#define ERROR_IO_PENDING 997L
#define ERROR_DEVICE_NOT_CONNECTED 1167L
#define ERROR_NO_SYSTEM_RESOURCES 1450L
#define ERROR_INVALID_USER_BUFFER 1784L

/* COM results (learn.microsoft.com, "Common HRESULT Values" and
SUCCEEDED; the same values as mingw-w64 winerror.h, public domain) */
#define S_OK ((HRESULT)0L)
#define E_FAIL ((HRESULT)0x80004005L)
#define E_INVALIDARG ((HRESULT)0x80070057L)
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define SUCCEEDED(result) (((HRESULT)(result)) >= 0)

/* Exception codes, as GetExceptionCode and EXCEPTION_RECORD report them
(learn.microsoft.com, EXCEPTION_RECORD, and the matching NTSTATUS values
in "[MS-ERREF] NTSTATUS Values") */
#define EXCEPTION_DATATYPE_MISALIGNMENT ((DWORD)0x80000002L)
#define EXCEPTION_BREAKPOINT ((DWORD)0x80000003L)
#define EXCEPTION_SINGLE_STEP ((DWORD)0x80000004L)
#define EXCEPTION_ACCESS_VIOLATION ((DWORD)0xC0000005L)
#define EXCEPTION_NONCONTINUABLE_EXCEPTION ((DWORD)0xC0000025L)
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED ((DWORD)0xC000008CL)
#define EXCEPTION_FLT_DENORMAL_OPERAND ((DWORD)0xC000008DL)
#define EXCEPTION_FLT_DIVIDE_BY_ZERO ((DWORD)0xC000008EL)
#define EXCEPTION_FLT_INEXACT_RESULT ((DWORD)0xC000008FL)
#define EXCEPTION_FLT_INVALID_OPERATION ((DWORD)0xC0000090L)
#define EXCEPTION_FLT_OVERFLOW ((DWORD)0xC0000091L)
#define EXCEPTION_FLT_STACK_CHECK ((DWORD)0xC0000092L)
#define EXCEPTION_FLT_UNDERFLOW ((DWORD)0xC0000093L)
#define EXCEPTION_INT_DIVIDE_BY_ZERO ((DWORD)0xC0000094L)
#define EXCEPTION_INT_OVERFLOW ((DWORD)0xC0000095L)
#define EXCEPTION_PRIV_INSTRUCTION ((DWORD)0xC0000096L)

/* Waits (learn.microsoft.com, WaitForSingleObjectEx); cachebeta.exe's
take_mutex tests 0x80 and its winsock_error_to_string 0xC0 and 0x102 */
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 ((DWORD)0x00000000L)
#define WAIT_ABANDONED ((DWORD)0x00000080L)
#define WAIT_IO_COMPLETION ((DWORD)0x000000C0L)
#define WAIT_TIMEOUT ((DWORD)0x00000102L)
#define WAIT_FAILED ((DWORD)0xFFFFFFFF)

/* Threads (learn.microsoft.com, CreateThread, GetExitCodeThread and
SetThreadPriority); cachebeta.exe's create_thread passes 4 for
CREATE_SUSPENDED and 0, 1 and -1 as priorities, input_initialize 2 */
#define CREATE_SUSPENDED 0x00000004
#define STILL_ACTIVE ((DWORD)0x00000103L)
#define THREAD_PRIORITY_BELOW_NORMAL (-1)
#define THREAD_PRIORITY_NORMAL 0
#define THREAD_PRIORITY_ABOVE_NORMAL 1
#define THREAD_PRIORITY_HIGHEST 2

/* Files (learn.microsoft.com, CreateFileA, "File Attribute Constants",
SetFilePointer and GetFileSize); cachebeta.exe's
game_state_create_or_open_file passes 0xC0000000, 4 (OPEN_ALWAYS) and
0x28000000 (no buffering, sequential scan) */
#define GENERIC_READ 0x80000000UL
#define GENERIC_WRITE 0x40000000UL

#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5

#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_NORMAL 0x00000080

#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000
#define FILE_FLAG_NO_BUFFERING 0x20000000
#define FILE_FLAG_OVERLAPPED 0x40000000

#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG)-1)
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)

/* Global memory flags (learn.microsoft.com, GlobalAlloc and GlobalReAlloc);
cachebeta.exe's system_calloc passes 0x40, system_realloc 2 */
#define GMEM_MOVEABLE 0x0002
#define GMEM_ZEROINIT 0x0040

/* Page protection (learn.microsoft.com, "Memory Protection Constants").
cachebeta.exe's physical_memory_allocate passes 4 (PAGE_READWRITE) and
0x404 (PAGE_READWRITE | PAGE_WRITECOMBINE) to XPhysicalAlloc. */
#define PAGE_NOACCESS 0x01
#define PAGE_READONLY 0x02
#define PAGE_READWRITE 0x04
#define PAGE_EXECUTE 0x10
#define PAGE_EXECUTE_READ 0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_NOCACHE 0x200
#define PAGE_WRITECOMBINE 0x400

/* ---------- functions */

/* Critical sections: kernel routines whose definitions are in
port/linux/src/xbox_kernel.c (the structure is the PDB's, in xdk_pdb.h) */
VOID NTAPI RtlInitializeCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);
VOID NTAPI RtlEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);
VOID NTAPI RtlLeaveCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);
DWORD NTAPI RtlTryEnterCriticalSection(PRTL_CRITICAL_SECTION CriticalSection);

/* Interlocked operations (learn.microsoft.com, InterlockedIncrement and
the rest; argument order as documented), declared as the platform layer
defines them (port/linux/src/xbox_kernel.c). The PDB records the Xbox's
own as __fastcall; the game never calls them. */
LONG WINAPI _InterlockedIncrement(LPLONG Addend);
LONG WINAPI _InterlockedDecrement(LPLONG Addend);
LONG WINAPI _InterlockedExchange(LPLONG Target, LONG Value);
LONG WINAPI _InterlockedExchangeAdd(LPLONG Addend, LONG Value);
LONG WINAPI _InterlockedCompareExchange(LPLONG Destination, LONG Exchange, LONG Comparand);

#endif
