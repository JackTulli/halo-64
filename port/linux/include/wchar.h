/*
WCHAR.H

MSVC-compatible wide character interface for the Linux build.

The game is compiled with -fshort-wchar so that wchar_t is 16 bits as on the
Xbox (tag data and saved games store UTF-16 text). glibc's wide functions
assume a 32-bit wchar_t, so this header replaces <wchar.h> entirely and routes
every name to the 16-bit implementations in port/linux/src/msvc_wide.c. The
MSVC (pre-C99) signatures are kept: wcstok() takes two arguments and
swprintf()/vswprintf() take no buffer size.
*/

#ifndef __HALO_LINUX_WCHAR_H
#define __HALO_LINUX_WCHAR_H

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#if !defined(__WCHAR_MAX__) || __WCHAR_MAX__ != 0xffff
#error the Linux build requires -fshort-wchar
#endif

#if !defined(__wint_t_defined) && !defined(HALO_N64)
#define __wint_t_defined 1
#define _WINT_T 1
typedef unsigned short wint_t;
#endif

#ifndef WEOF
#define WEOF ((wint_t)0xFFFF)
#endif

typedef wchar_t wctype_t;

#define wcslen msvc_wcslen
#define wcsnlen msvc_wcsnlen
#define wcscpy msvc_wcscpy
#define wcsncpy msvc_wcsncpy
#define wcscat msvc_wcscat
#define wcsncat msvc_wcsncat
#define wcscmp msvc_wcscmp
#define wcsncmp msvc_wcsncmp
#define wcscoll msvc_wcscoll
#define wcsxfrm msvc_wcsxfrm
#define wcschr msvc_wcschr
#define wcsrchr msvc_wcsrchr
#define wcsstr msvc_wcsstr
#define wcsspn msvc_wcsspn
#define wcscspn msvc_wcscspn
#define wcspbrk msvc_wcspbrk
#define wcstok msvc_wcstok
#define wcstol msvc_wcstol
#define wcstoul msvc_wcstoul
#define wcstod msvc_wcstod
#define wcsdup msvc_wcsdup
#define _wcsdup msvc_wcsdup
#define _wcsicmp msvc_wcsicmp
#define _wcsnicmp msvc_wcsnicmp
#define wcsicmp msvc_wcsicmp
#define wcsnicmp msvc_wcsnicmp
#define _wcslwr msvc_wcslwr
#define _wcsupr msvc_wcsupr
#define wcslwr msvc_wcslwr
#define wcsupr msvc_wcsupr
#define _wtoi msvc_wtoi
#define _wtol msvc_wtol
#define wmemchr msvc_wmemchr
#define wmemcmp msvc_wmemcmp
#define wmemcpy msvc_wmemcpy
#define wmemmove msvc_wmemmove
#define wmemset msvc_wmemset
#define towlower msvc_towlower
#define towupper msvc_towupper
#define iswalpha msvc_iswalpha
#define iswupper msvc_iswupper
#define iswlower msvc_iswlower
#define iswdigit msvc_iswdigit
#define iswxdigit msvc_iswxdigit
#define iswspace msvc_iswspace
#define iswpunct msvc_iswpunct
#define iswalnum msvc_iswalnum
#define iswprint msvc_iswprint
#define iswgraph msvc_iswgraph
#define iswcntrl msvc_iswcntrl
#define iswascii msvc_iswascii
#define iswctype msvc_iswctype
#define swprintf msvc_swprintf
#define vswprintf msvc_vswprintf
#define _snwprintf msvc_snwprintf
#define _vsnwprintf msvc_vsnwprintf
#define wprintf msvc_wprintf
#define vwprintf msvc_vwprintf
#define fwprintf msvc_fwprintf
#define vfwprintf msvc_vfwprintf
#define fgetwc msvc_fgetwc
#define fputwc msvc_fputwc
#define getwc msvc_fgetwc
#define putwc msvc_fputwc
#define getwchar() msvc_fgetwc(stdin)
#define putwchar(c) msvc_fputwc((c), stdout)
#define ungetwc msvc_ungetwc
#define fgetws msvc_fgetws
#define fputws msvc_fputws
#define _getws msvc_getws
#define _putws msvc_putws
#define _wremove msvc_wremove
#define _wtmpnam msvc_wtmpnam
#define _wctime msvc_wctime
#define _wasctime msvc_wasctime
#define _wperror msvc_wperror
#define _wcserror msvc_wcserror
#define _wfopen msvc_wfopen

size_t msvc_wcslen(const wchar_t *string);
size_t msvc_wcsnlen(const wchar_t *string, size_t maximum_count);
wchar_t *msvc_wcscpy(wchar_t *destination, const wchar_t *source);
wchar_t *msvc_wcsncpy(wchar_t *destination, const wchar_t *source, size_t count);
wchar_t *msvc_wcscat(wchar_t *destination, const wchar_t *source);
wchar_t *msvc_wcsncat(wchar_t *destination, const wchar_t *source, size_t count);
int msvc_wcscmp(const wchar_t *string1, const wchar_t *string2);
int msvc_wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count);
int msvc_wcscoll(const wchar_t *string1, const wchar_t *string2);
size_t msvc_wcsxfrm(wchar_t *destination, const wchar_t *source, size_t count);
wchar_t *msvc_wcschr(const wchar_t *string, wchar_t character);
wchar_t *msvc_wcsrchr(const wchar_t *string, wchar_t character);
wchar_t *msvc_wcsstr(const wchar_t *string, const wchar_t *substring);
size_t msvc_wcsspn(const wchar_t *string, const wchar_t *characters);
size_t msvc_wcscspn(const wchar_t *string, const wchar_t *characters);
wchar_t *msvc_wcspbrk(const wchar_t *string, const wchar_t *characters);
wchar_t *msvc_wcstok(wchar_t *string, const wchar_t *delimiters);
long msvc_wcstol(const wchar_t *string, wchar_t **end, int base);
unsigned long msvc_wcstoul(const wchar_t *string, wchar_t **end, int base);
double msvc_wcstod(const wchar_t *string, wchar_t **end);
wchar_t *msvc_wcsdup(const wchar_t *string);
int msvc_wcsicmp(const wchar_t *string1, const wchar_t *string2);
int msvc_wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count);
wchar_t *msvc_wcslwr(wchar_t *string);
wchar_t *msvc_wcsupr(wchar_t *string);
int msvc_wtoi(const wchar_t *string);
long msvc_wtol(const wchar_t *string);
wchar_t *msvc_wmemchr(const wchar_t *buffer, wchar_t character, size_t count);
int msvc_wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
wchar_t *msvc_wmemcpy(wchar_t *destination, const wchar_t *source, size_t count);
wchar_t *msvc_wmemmove(wchar_t *destination, const wchar_t *source, size_t count);
wchar_t *msvc_wmemset(wchar_t *buffer, wchar_t character, size_t count);

wint_t msvc_towlower(wint_t character);
wint_t msvc_towupper(wint_t character);
int msvc_iswalpha(wint_t character);
int msvc_iswupper(wint_t character);
int msvc_iswlower(wint_t character);
int msvc_iswdigit(wint_t character);
int msvc_iswxdigit(wint_t character);
int msvc_iswspace(wint_t character);
int msvc_iswpunct(wint_t character);
int msvc_iswalnum(wint_t character);
int msvc_iswprint(wint_t character);
int msvc_iswgraph(wint_t character);
int msvc_iswcntrl(wint_t character);
int msvc_iswascii(wint_t character);
int msvc_iswctype(wint_t character, wctype_t type);

int msvc_swprintf(wchar_t *buffer, const wchar_t *format, ...);
int msvc_vswprintf(wchar_t *buffer, const wchar_t *format, va_list arguments);
int msvc_snwprintf(wchar_t *buffer, size_t count, const wchar_t *format, ...);
int msvc_vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list arguments);
int msvc_wprintf(const wchar_t *format, ...);
int msvc_vwprintf(const wchar_t *format, va_list arguments);
int msvc_fwprintf(FILE *stream, const wchar_t *format, ...);
int msvc_vfwprintf(FILE *stream, const wchar_t *format, va_list arguments);

wint_t msvc_fgetwc(FILE *stream);
wint_t msvc_fputwc(wchar_t character, FILE *stream);
wint_t msvc_ungetwc(wint_t character, FILE *stream);
wchar_t *msvc_fgetws(wchar_t *string, int count, FILE *stream);
int msvc_fputws(const wchar_t *string, FILE *stream);
wchar_t *msvc_getws(wchar_t *string);
int msvc_putws(const wchar_t *string);
int msvc_wremove(const wchar_t *path);
wchar_t *msvc_wtmpnam(wchar_t *string);
wchar_t *msvc_wctime(const time_t *timer);
wchar_t *msvc_wasctime(const struct tm *time);
void msvc_wperror(const wchar_t *string);
wchar_t *msvc_wcserror(int error_number);
FILE *msvc_wfopen(const wchar_t *path, const wchar_t *mode);

#endif
