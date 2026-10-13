/*
STDIO.H

Host <stdio.h> plus MSVC-named functions.
*/

#ifndef __HALO_LINUX_STDIO_H
#define __HALO_LINUX_STDIO_H

#include <stdarg.h>
#include_next <stdio.h>

int snprintf(char *buffer, size_t count, const char *format, ...);
int vsnprintf(char *buffer, size_t count, const char *format, va_list arguments);

/* MSVC's _snprintf does not terminate a truncated string; always
terminating is strictly safer for callers written against it. */
#define _snprintf snprintf
#define _vsnprintf vsnprintf

FILE *_fdopen(int handle, const char *mode);
int _fileno(FILE *stream);
#define fdopen _fdopen
#define fileno _fileno

/* MSVC printf length modifiers (%I64d, %I32x) are translated for glibc;
see port/linux/src/msvc_crt.c */
#ifndef HALO_LINUX_PLATFORM_LAYER
int halo_linux_vsnprintf(char *buffer, size_t count, const char *format, va_list arguments);
int halo_linux_snprintf(char *buffer, size_t count, const char *format, ...);
int halo_linux_vsprintf(char *buffer, const char *format, va_list arguments);
int halo_linux_sprintf(char *buffer, const char *format, ...);
int halo_linux_vfprintf(FILE *stream, const char *format, va_list arguments);
int halo_linux_fprintf(FILE *stream, const char *format, ...);
int halo_linux_printf(const char *format, ...);
int halo_linux_vprintf(const char *format, va_list arguments);
#undef _snprintf
#undef _vsnprintf
#define snprintf halo_linux_snprintf
#define vsnprintf halo_linux_vsnprintf
#define _snprintf halo_linux_snprintf
#define _vsnprintf halo_linux_vsnprintf
#define sprintf halo_linux_sprintf
#define vsprintf halo_linux_vsprintf
#define fprintf halo_linux_fprintf
#define vfprintf halo_linux_vfprintf
#define printf halo_linux_printf
#define vprintf halo_linux_vprintf
#endif

/* The game opens files by Xbox path (d:\\debug.txt, z:\\saved\\...);
translate them the way CreateFile does (port/linux/src/xbox_files.c). */
#ifndef HALO_LINUX_PLATFORM_LAYER
FILE *halo_linux_fopen(const char *path, const char *mode);
FILE *halo_linux_freopen(const char *path, const char *mode, FILE *stream);
int halo_linux_remove(const char *path);
int halo_linux_rename(const char *old_path, const char *new_path);
#define fopen halo_linux_fopen
#define freopen halo_linux_freopen
#define remove halo_linux_remove
#define rename halo_linux_rename
#endif

#endif
