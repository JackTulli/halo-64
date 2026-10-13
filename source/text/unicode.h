/*
UNICODE.H

header included in hcex build.
*/

#ifndef __UNICODE_H
#define __UNICODE_H
#pragma once

/* ---------- headers */

#include <wchar.h>
#include <time.h>
#include <stdarg.h>

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes/UNICODE.C */

/* ---------- globals */

/* ---------- public code */

int uisalpha(
	wchar_t character);
int uisupper(
	wchar_t character);
int uislower(
	wchar_t character);
int uisdigit(
	wchar_t character);
int uisxdigit(
	wchar_t character);
int uisspace(
	wchar_t character);
int uispunct(
	wchar_t character);
int uisalnum(
	wchar_t character);
int uisprint(
	wchar_t character);
int uisgraph(
	wchar_t character);
int uiscntrl(
	wchar_t character);
int utoupper(
	wchar_t character);
int utolower(
	wchar_t character);
void *umemchr(
	void const *buffer,
	int value,
	unsigned long count);
long umemcmp(
	void const *buffer1,
	void const *buffer2,
	unsigned long count);
void *umemmove(
	void *dest,
	void const *src,
	unsigned long count);
wint_t ufgetc(
	FILE *stream);
wint_t ufputc(
	wchar_t character,
	FILE *stream);
wint_t uungetc(
	wchar_t character,
	FILE *stream);
int ufclose(
	FILE *stream);
wchar_t *uctime(
	time_t const *timer);
wchar_t *uasctime(
	struct tm const *timeptr);
wchar_t *utmpnam(
	wchar_t *string);
unsigned long ustrlen(
	wchar_t const *string);
wchar_t *ustrchr(
	wchar_t const *string,
	wchar_t character);
wchar_t *ustrrchr(
	wchar_t const *string,
	wchar_t character);
wchar_t *ustrlwr(
	wchar_t *string);
wchar_t *ustrupr(
	wchar_t *string);
wchar_t *ugets(
	wchar_t *string);
int uputs(
	wchar_t const *string);
int uremove(
	wchar_t const *path);
long ustrtol(
	wchar_t const *nptr,
	wchar_t **endptr,
	int base);
int uatoi(
	wchar_t const *string);
void *umemset(
	void *buffer,
	int value,
	unsigned long count);
long ustrncmp(
	wchar_t const *string1,
	wchar_t const *string2,
	unsigned long count);
wchar_t *ustrncpy(
	wchar_t *dest,
	wchar_t const *src,
	unsigned long count);
wchar_t *ustrtok(
	wchar_t *string,
	wchar_t const *delimiters);
int uprintf(
	wchar_t const *format,
	...);
int uvprintf(
	wchar_t const *format,
	va_list arglist);
void uperror(
	wchar_t const *string);
unsigned long ustrtoul(
	wchar_t const *nptr,
	wchar_t **endptr,
	int base);
double ustrtod(
	wchar_t const *nptr,
	wchar_t **endptr);
unsigned long ustrnlen(
	wchar_t const *string,
	unsigned long maximum_length);
wchar_t *ustrcpy(
	wchar_t *dest,
	wchar_t const *src);
int uvfprintf(
	FILE *stream,
	wchar_t const *format,
	va_list arglist);
void *umemcpy(
	void *dest,
	void const *src,
	unsigned long count);
wchar_t *ustrncat(
	wchar_t *dest,
	wchar_t const *src,
	unsigned long count);
wchar_t *ufgets(
	wchar_t *string,
	int size,
	FILE *stream);
int ufputs(
	wchar_t const *string,
	FILE *stream);
int ufprintf(
	FILE *stream,
	wchar_t const *format,
	...);
FILE *ufdopen(
	int fd,
	wchar_t const *path);
FILE *ufopen(
	wchar_t const *path,
	wchar_t const *mode);
FILE *upopen(
	wchar_t const *command,
	wchar_t const *mode);
wchar_t *ascii_to_wide(
	char const *ascii,
	wchar_t *unicode,
	unsigned long size);
wchar_t *ustrcat(
	wchar_t *dest,
	wchar_t const *src);
long ustrcmp(
	wchar_t const *string1,
	wchar_t const *string2);
long ustrcoll(
	wchar_t const *string1,
	wchar_t const *string2);
unsigned long ustrcspn(
	wchar_t const *string,
	wchar_t const *character_set);
wchar_t *ustrpbrk(
	wchar_t const *string,
	wchar_t const *character_set);
unsigned long ustrspn(
	wchar_t const *string,
	wchar_t const *character_set);
wchar_t *ustrstr(
	wchar_t const *string,
	wchar_t const *character_set);
wchar_t *ustrnlwr(
	wchar_t *string,
	unsigned long count);
wchar_t *ustrnupr(
	wchar_t *string,
	unsigned long count);
unsigned long ustrxfrm(
	wchar_t *dest,
	wchar_t const *src,
	unsigned long count);
long ustrcasecmp(
	wchar_t const *string1,
	wchar_t const *string2);
long ustrncasecmp(
	wchar_t const *string1,
	wchar_t const *string2,
	unsigned long count);
int usnprintf(
	wchar_t *string,
	unsigned long size,
	wchar_t const *format,
	...);
int usprintf(
	wchar_t *string,
	wchar_t const *format,
	...);
int uvsnprintf(
	wchar_t *string,
	unsigned long size,
	wchar_t const *format,
	va_list arglist);
int uvsprintf(
	wchar_t *string,
	wchar_t const *format,
	va_list arglist);
FILE *ufreopen(
	wchar_t const *path,
	wchar_t const *mode,
	FILE *stream);
char *wide_to_ascii(
	wchar_t const *unicode,
	char *ascii,
	unsigned long size);
wchar_t *ustrerror(
	int error_number);

#endif // __UNICODE_H
