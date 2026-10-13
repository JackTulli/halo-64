/*
XDK_CRT.C

The Microsoft C runtime names the game calls (port/linux/include's headers
route them here), over newlib.
*/

#include "platform.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _stricmp(const char *a, const char *b)
{
	for (;; a++, b++)
	{
		int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);

		if (x != y || !x)
			return x - y;
	}
}

int _strnicmp(const char *a, const char *b, size_t count)
{
	for (; count; count--, a++, b++)
	{
		int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);

		if (x != y || !x)
			return x - y;
	}
	return 0;
}

/* the x87 control word: no such thing on the VR4300 */
unsigned int _control87(unsigned int new_value, unsigned int mask)
{
	(void)new_value;
	(void)mask;
	return 0x0009001F;
}

/* rewrite %I64 as %ll and drop %I32 (MSVC's size prefixes) */
static const char *translate_format(const char *format, char *buffer, size_t size)
{
	const char *cursor;
	size_t length = 0;

	if (!strstr(format, "I64") && !strstr(format, "I32"))
		return format;
	for (cursor = format; *cursor && length + 3 < size; cursor++)
	{
		buffer[length++] = *cursor;
		if (*cursor != '%')
			continue;
		if (cursor[1] == '%')
		{
			buffer[length++] = *++cursor;
			continue;
		}
		while (cursor[1] && strchr("-+ #0123456789.*", cursor[1]) && length + 3 < size)
			buffer[length++] = *++cursor;
		if (cursor[1] == 'I' && cursor[2] == '6' && cursor[3] == '4')
		{
			buffer[length++] = 'l';
			buffer[length++] = 'l';
			cursor += 3;
		}
		else if (cursor[1] == 'I' && cursor[2] == '3' && cursor[3] == '2')
		{
			cursor += 3;
		}
	}
	buffer[length] = '\0';
	return buffer;
}

int halo_linux_vsnprintf(char *buffer, size_t count, const char *format, va_list arguments)
{
	char translated[512];

	return vsnprintf(buffer, count, translate_format(format, translated, sizeof(translated)), arguments);
}

int halo_linux_snprintf(char *buffer, size_t count, const char *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = halo_linux_vsnprintf(buffer, count, format, arguments);
	va_end(arguments);
	return result;
}

int halo_linux_vsprintf(char *buffer, const char *format, va_list arguments)
{
	/* the game sizes its buffers itself (MSVC's unbounded vsprintf) */
	return halo_linux_vsnprintf(buffer, 4096, format, arguments);
}

int halo_linux_sprintf(char *buffer, const char *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = halo_linux_vsprintf(buffer, format, arguments);
	va_end(arguments);
	return result;
}

/* the game's text files (its logs and debug dumps): nothing can be opened
on the N64; the game's log goes to the debug output instead
(cseries/errors.c) */
FILE *halo_linux_fopen(const char *path, const char *mode)
{
	(void)path;
	(void)mode;
	return NULL;
}

int halo_linux_vfprintf(FILE *stream, const char *format, va_list arguments)
{
	char line[512];
	int result = halo_linux_vsnprintf(line, sizeof(line), format, arguments);
	size_t length = strlen(line);

	(void)stream;
	while (length && (line[length - 1] == '\n' || line[length - 1] == '\r'))
		line[--length] = 0;
	if (length)
		n64_log(line);
	return result;
}

int halo_linux_fprintf(FILE *stream, const char *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = halo_linux_vfprintf(stream, format, arguments);
	va_end(arguments);
	return result;
}
