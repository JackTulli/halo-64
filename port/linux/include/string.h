/*
STRING.H

Host <string.h> plus MSVC-named functions.
*/

#ifndef __HALO_LINUX_STRING_H
#define __HALO_LINUX_STRING_H

#include_next <string.h>

int _stricmp(const char *string1, const char *string2);
int _strnicmp(const char *string1, const char *string2, size_t count);
char *_strdup(const char *string);
char *_strlwr(char *string);
char *_strupr(char *string);

#define stricmp _stricmp
#define strcmpi _stricmp
#define _strcmpi _stricmp
#define strnicmp _strnicmp
#define strdup _strdup

#endif
