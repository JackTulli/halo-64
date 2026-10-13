/*
MSVC_WIDE.C

The Microsoft C runtime's wide character functions for 16-bit wchar_t
(UTF-16, as the Xbox uses), declared in port/linux/include/wchar.h.

MSVC semantics are kept where they differ from ISO C: wcstok takes two
arguments, swprintf/vswprintf take no size, and in the wide printf family
%s/%c take wide arguments while %S/%C (and %hs/%hc) take narrow ones.
Character classification covers ASCII and Latin-1, which is what the
game's text uses outside of its own font tables.
*/

#include "platform.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

/* ---------- strings */

size_t msvc_wcslen(const wchar_t *string)
{
	const wchar_t *cursor = string;

	while (*cursor)
		cursor++;
	return (size_t)(cursor - string);
}

size_t msvc_wcsnlen(const wchar_t *string, size_t maximum_count)
{
	size_t count = 0;

	while (count < maximum_count && string[count])
		count++;
	return count;
}

wchar_t *msvc_wcscpy(wchar_t *destination, const wchar_t *source)
{
	wchar_t *cursor = destination;

	while ((*cursor++ = *source++) != 0)
		;
	return destination;
}

wchar_t *msvc_wcsncpy(wchar_t *destination, const wchar_t *source, size_t count)
{
	size_t index = 0;

	for (; index < count && source[index]; index++)
		destination[index] = source[index];
	for (; index < count; index++)
		destination[index] = 0;
	return destination;
}

wchar_t *msvc_wcscat(wchar_t *destination, const wchar_t *source)
{
	msvc_wcscpy(destination + msvc_wcslen(destination), source);
	return destination;
}

wchar_t *msvc_wcsncat(wchar_t *destination, const wchar_t *source, size_t count)
{
	wchar_t *cursor = destination + msvc_wcslen(destination);

	while (count-- && *source)
		*cursor++ = *source++;
	*cursor = 0;
	return destination;
}

int msvc_wcscmp(const wchar_t *string1, const wchar_t *string2)
{
	while (*string1 && *string1 == *string2)
	{
		string1++;
		string2++;
	}
	return (int)*string1 - (int)*string2;
}

int msvc_wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	for (; count; count--, string1++, string2++)
	{
		if (*string1 != *string2 || !*string1)
			return (int)*string1 - (int)*string2;
	}
	return 0;
}

int msvc_wcscoll(const wchar_t *string1, const wchar_t *string2)
{
	/* the C locale collates by code unit */
	return msvc_wcscmp(string1, string2);
}

size_t msvc_wcsxfrm(wchar_t *destination, const wchar_t *source, size_t count)
{
	size_t length = msvc_wcslen(source);

	if (destination && count)
		msvc_wcsncpy(destination, source, count);
	return length;
}

wchar_t *msvc_wcschr(const wchar_t *string, wchar_t character)
{
	for (;; string++)
	{
		if (*string == character)
			return (wchar_t *)string;
		if (!*string)
			return NULL;
	}
}

wchar_t *msvc_wcsrchr(const wchar_t *string, wchar_t character)
{
	const wchar_t *result = NULL;

	for (;; string++)
	{
		if (*string == character)
			result = string;
		if (!*string)
			return (wchar_t *)result;
	}
}

wchar_t *msvc_wcsstr(const wchar_t *string, const wchar_t *substring)
{
	size_t length = msvc_wcslen(substring);

	if (!length)
		return (wchar_t *)string;
	for (; *string; string++)
	{
		if (*string == *substring && !msvc_wcsncmp(string, substring, length))
			return (wchar_t *)string;
	}
	return NULL;
}

size_t msvc_wcsspn(const wchar_t *string, const wchar_t *characters)
{
	size_t count = 0;

	while (string[count] && msvc_wcschr(characters, string[count]))
		count++;
	return count;
}

size_t msvc_wcscspn(const wchar_t *string, const wchar_t *characters)
{
	size_t count = 0;

	while (string[count] && !msvc_wcschr(characters, string[count]))
		count++;
	return count;
}

wchar_t *msvc_wcspbrk(const wchar_t *string, const wchar_t *characters)
{
	for (; *string; string++)
	{
		if (msvc_wcschr(characters, *string))
			return (wchar_t *)string;
	}
	return NULL;
}

wchar_t *msvc_wcstok(wchar_t *string, const wchar_t *delimiters)
{
	/* MSVC keeps the tokenizer state per thread */
	static __thread wchar_t *next;
	wchar_t *token;

	if (string)
		next = string;
	if (!next)
		return NULL;
	next += msvc_wcsspn(next, delimiters);
	if (!*next)
	{
		next = NULL;
		return NULL;
	}
	token = next;
	next += msvc_wcscspn(next, delimiters);
	if (*next)
		*next++ = 0;
	else
		next = NULL;
	return token;
}

wchar_t *msvc_wcsdup(const wchar_t *string)
{
	size_t size = (msvc_wcslen(string) + 1) * sizeof(wchar_t);
	wchar_t *copy = malloc(size);

	if (copy)
		memcpy(copy, string, size);
	return copy;
}

/* ---------- character classes (ASCII and Latin-1) */

static int is_latin1_upper(wint_t character)
{
	return (character >= 'A' && character <= 'Z') ||
		(character >= 0xc0 && character <= 0xde && character != 0xd7);
}

static int is_latin1_lower(wint_t character)
{
	return (character >= 'a' && character <= 'z') ||
		(character >= 0xdf && character <= 0xff && character != 0xf7);
}

wint_t msvc_towlower(wint_t character)
{
	return is_latin1_upper(character) ? (wint_t)(character + 0x20) : character;
}

wint_t msvc_towupper(wint_t character)
{
	/* 0xdf (sharp s) and 0xff have no single upper case letter in Latin-1 */
	return (is_latin1_lower(character) && character != 0xdf && character != 0xff) ?
		(wint_t)(character - 0x20) : character;
}

int msvc_iswupper(wint_t character) { return is_latin1_upper(character); }
int msvc_iswlower(wint_t character) { return is_latin1_lower(character); }
int msvc_iswalpha(wint_t character) { return is_latin1_upper(character) || is_latin1_lower(character); }
int msvc_iswdigit(wint_t character) { return character >= '0' && character <= '9'; }
int msvc_iswalnum(wint_t character) { return msvc_iswalpha(character) || msvc_iswdigit(character); }
int msvc_iswascii(wint_t character) { return character < 0x80; }
int msvc_iswcntrl(wint_t character) { return character < 0x20 || (character >= 0x7f && character < 0xa0); }
int msvc_iswprint(wint_t character) { return !msvc_iswcntrl(character) && character != WEOF; }
int msvc_iswgraph(wint_t character) { return msvc_iswprint(character) && character != ' ' && character != 0xa0; }

int msvc_iswxdigit(wint_t character)
{
	return msvc_iswdigit(character) ||
		(character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F');
}

int msvc_iswspace(wint_t character)
{
	return character == ' ' || (character >= '\t' && character <= '\r') || character == 0xa0;
}

int msvc_iswpunct(wint_t character)
{
	return msvc_iswgraph(character) && !msvc_iswalnum(character);
}

int msvc_iswctype(wint_t character, wctype_t type)
{
	/* the MSVC _UPPER, _LOWER, _DIGIT, _SPACE, _PUNCT, _CONTROL, _BLANK,
	_HEX, _ALPHA classification bits */
	int result = 0;

	if ((type & 0x001) && msvc_iswupper(character)) result |= 0x001;
	if ((type & 0x002) && msvc_iswlower(character)) result |= 0x002;
	if ((type & 0x004) && msvc_iswdigit(character)) result |= 0x004;
	if ((type & 0x008) && msvc_iswspace(character)) result |= 0x008;
	if ((type & 0x010) && msvc_iswpunct(character)) result |= 0x010;
	if ((type & 0x020) && msvc_iswcntrl(character)) result |= 0x020;
	if ((type & 0x040) && (character == ' ' || character == '\t')) result |= 0x040;
	if ((type & 0x080) && msvc_iswxdigit(character)) result |= 0x080;
	if ((type & 0x100) && msvc_iswalpha(character)) result |= 0x100;
	return result;
}

int msvc_wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	for (;; string1++, string2++)
	{
		wint_t c1 = msvc_towlower(*string1);
		wint_t c2 = msvc_towlower(*string2);

		if (c1 != c2 || !c1)
			return (int)c1 - (int)c2;
	}
}

int msvc_wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	for (; count; count--, string1++, string2++)
	{
		wint_t c1 = msvc_towlower(*string1);
		wint_t c2 = msvc_towlower(*string2);

		if (c1 != c2 || !c1)
			return (int)c1 - (int)c2;
	}
	return 0;
}

wchar_t *msvc_wcslwr(wchar_t *string)
{
	wchar_t *cursor;

	for (cursor = string; *cursor; cursor++)
		*cursor = msvc_towlower(*cursor);
	return string;
}

wchar_t *msvc_wcsupr(wchar_t *string)
{
	wchar_t *cursor;

	for (cursor = string; *cursor; cursor++)
		*cursor = msvc_towupper(*cursor);
	return string;
}

/* ---------- memory */

wchar_t *msvc_wmemchr(const wchar_t *buffer, wchar_t character, size_t count)
{
	for (; count; count--, buffer++)
	{
		if (*buffer == character)
			return (wchar_t *)buffer;
	}
	return NULL;
}

int msvc_wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	for (; count; count--, buffer1++, buffer2++)
	{
		if (*buffer1 != *buffer2)
			return (int)*buffer1 - (int)*buffer2;
	}
	return 0;
}

wchar_t *msvc_wmemcpy(wchar_t *destination, const wchar_t *source, size_t count)
{
	return memcpy(destination, source, count * sizeof(wchar_t));
}

wchar_t *msvc_wmemmove(wchar_t *destination, const wchar_t *source, size_t count)
{
	return memmove(destination, source, count * sizeof(wchar_t));
}

wchar_t *msvc_wmemset(wchar_t *buffer, wchar_t character, size_t count)
{
	size_t index;

	for (index = 0; index < count; index++)
		buffer[index] = character;
	return buffer;
}

/* ---------- conversion to and from UTF-8 */

/* narrow copy of a wide string for the host C library; returns the number
of bytes written excluding the terminator */
static size_t wide_to_utf8(const wchar_t *source, size_t source_count, char *destination, size_t size)
{
	size_t written = 0;
	size_t index;

	for (index = 0; index < source_count && source[index]; index++)
	{
		unsigned long code = source[index];
		char encoded[4];
		size_t length;

		if (code >= 0xd800 && code < 0xdc00 && index + 1 < source_count &&
			source[index + 1] >= 0xdc00 && source[index + 1] < 0xe000)
		{
			code = 0x10000 + ((code - 0xd800) << 10) + (source[index + 1] - 0xdc00);
			index++;
		}
		if (code < 0x80)
		{
			encoded[0] = (char)code;
			length = 1;
		}
		else if (code < 0x800)
		{
			encoded[0] = (char)(0xc0 | (code >> 6));
			encoded[1] = (char)(0x80 | (code & 0x3f));
			length = 2;
		}
		else if (code < 0x10000)
		{
			encoded[0] = (char)(0xe0 | (code >> 12));
			encoded[1] = (char)(0x80 | ((code >> 6) & 0x3f));
			encoded[2] = (char)(0x80 | (code & 0x3f));
			length = 3;
		}
		else
		{
			encoded[0] = (char)(0xf0 | (code >> 18));
			encoded[1] = (char)(0x80 | ((code >> 12) & 0x3f));
			encoded[2] = (char)(0x80 | ((code >> 6) & 0x3f));
			encoded[3] = (char)(0x80 | (code & 0x3f));
			length = 4;
		}
		if (written + length + 1 > size)
			break;
		memcpy(destination + written, encoded, length);
		written += length;
	}
	if (size)
		destination[written] = '\0';
	return written;
}

/* wide copy of a narrow string; bytes are taken as Latin-1 unless they
form valid UTF-8 */
static size_t narrow_to_wide(const char *source, size_t source_count, wchar_t *destination, size_t count)
{
	const unsigned char *cursor = (const unsigned char *)source;
	const unsigned char *end = cursor + source_count;
	size_t written = 0;

	while (cursor < end && *cursor && written + 1 < count)
	{
		unsigned long code = *cursor;
		size_t length = 1;

		if (code >= 0xc2 && code < 0xe0 && cursor + 1 < end && (cursor[1] & 0xc0) == 0x80)
		{
			code = ((code & 0x1f) << 6) | (cursor[1] & 0x3f);
			length = 2;
		}
		else if (code >= 0xe0 && code < 0xf0 && cursor + 2 < end &&
			(cursor[1] & 0xc0) == 0x80 && (cursor[2] & 0xc0) == 0x80)
		{
			code = ((code & 0x0f) << 12) | ((unsigned long)(cursor[1] & 0x3f) << 6) | (cursor[2] & 0x3f);
			length = 3;
		}
		destination[written++] = (wchar_t)code;
		cursor += length;
	}
	if (count)
		destination[written] = 0;
	return written;
}

/* ---------- numbers */

static size_t ascii_prefix(const wchar_t *string, char *buffer, size_t size)
{
	size_t index;

	for (index = 0; index + 1 < size && string[index] && string[index] < 0x80; index++)
		buffer[index] = (char)string[index];
	buffer[index] = '\0';
	return index;
}

long msvc_wcstol(const wchar_t *string, wchar_t **end, int base)
{
	char buffer[128];
	char *narrow_end;
	long result;

	ascii_prefix(string, buffer, sizeof(buffer));
	result = strtol(buffer, &narrow_end, base);
	if (end)
		*end = (wchar_t *)string + (narrow_end - buffer);
	return result;
}

unsigned long msvc_wcstoul(const wchar_t *string, wchar_t **end, int base)
{
	char buffer[128];
	char *narrow_end;
	unsigned long result;

	ascii_prefix(string, buffer, sizeof(buffer));
	result = strtoul(buffer, &narrow_end, base);
	if (end)
		*end = (wchar_t *)string + (narrow_end - buffer);
	return result;
}

double msvc_wcstod(const wchar_t *string, wchar_t **end)
{
	char buffer[128];
	char *narrow_end;
	double result;

	ascii_prefix(string, buffer, sizeof(buffer));
	result = strtod(buffer, &narrow_end);
	if (end)
		*end = (wchar_t *)string + (narrow_end - buffer);
	return result;
}

int msvc_wtoi(const wchar_t *string)
{
	return (int)msvc_wcstol(string, NULL, 10);
}

long msvc_wtol(const wchar_t *string)
{
	return msvc_wcstol(string, NULL, 10);
}

/* ---------- formatted output */

struct wide_output
{
	wchar_t *buffer;
	size_t capacity; /* in characters, including the terminator */
	size_t length;
};

static void output_character(struct wide_output *output, wchar_t character)
{
	if (output->length + 1 < output->capacity)
		output->buffer[output->length] = character;
	output->length++;
}

static void output_padding(struct wide_output *output, long count)
{
	while (count-- > 0)
		output_character(output, ' ');
}

static void output_wide(struct wide_output *output, const wchar_t *string, long precision, long width, BOOL left)
{
	long length = 0;
	long index;

	if (!string)
	{
		static const wchar_t null_string[] = { '(', 'n', 'u', 'l', 'l', ')', 0 };

		string = null_string;
	}
	while (string[length] && (precision < 0 || length < precision))
		length++;
	if (!left)
		output_padding(output, width - length);
	for (index = 0; index < length; index++)
		output_character(output, string[index]);
	if (left)
		output_padding(output, width - length);
}

static void output_narrow(struct wide_output *output, const char *string, long precision, long width, BOOL left)
{
	wchar_t converted[1024];
	size_t length;

	if (!string)
		string = "(null)";
	length = strlen(string);
	if (precision >= 0 && (size_t)precision < length)
		length = (size_t)precision;
	narrow_to_wide(string, length, converted, sizeof(converted) / sizeof(converted[0]));
	output_wide(output, converted, -1, width, left);
}

static int wide_format(struct wide_output *output, const wchar_t *format, va_list arguments)
{
	while (*format)
	{
		char specification[64];
		size_t specification_length = 0;
		long width = 0;
		long precision = -1;
		BOOL left = FALSE;
		int size = 0; /* 'h' = 1, 'l'/'w' = 2, 'll'/'I64' = 3, 'L' = 4 */
		wchar_t conversion;

		if (*format != '%')
		{
			output_character(output, *format++);
			continue;
		}
		format++;
		if (*format == '%')
		{
			output_character(output, *format++);
			continue;
		}

		specification[specification_length++] = '%';
		/* flags */
		while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' || *format == '0')
		{
			if (*format == '-')
				left = TRUE;
			specification[specification_length++] = (char)*format++;
		}
		/* width */
		if (*format == '*')
		{
			width = va_arg(arguments, int);
			if (width < 0)
			{
				left = TRUE;
				width = -width;
				specification[specification_length++] = '-';
			}
			specification_length += (size_t)snprintf(specification + specification_length,
				sizeof(specification) - specification_length, "%ld", width);
			format++;
		}
		else
		{
			while (*format >= '0' && *format <= '9')
			{
				width = width * 10 + (*format - '0');
				specification[specification_length++] = (char)*format++;
			}
		}
		/* precision */
		if (*format == '.')
		{
			precision = 0;
			specification[specification_length++] = '.';
			format++;
			if (*format == '*')
			{
				precision = va_arg(arguments, int);
				if (precision < 0)
				{
					precision = -1;
					specification_length--;
				}
				else
				{
					specification_length += (size_t)snprintf(specification + specification_length,
						sizeof(specification) - specification_length, "%ld", precision);
				}
				format++;
			}
			else
			{
				while (*format >= '0' && *format <= '9')
				{
					precision = precision * 10 + (*format - '0');
					specification[specification_length++] = (char)*format++;
				}
			}
		}
		/* size */
		if (*format == 'h')
		{
			size = 1;
			format++;
		}
		else if (*format == 'l' || *format == 'w')
		{
			size = 2;
			format++;
			if (*format == 'l')
			{
				size = 3;
				format++;
			}
		}
		else if (*format == 'I' && format[1] == '6' && format[2] == '4')
		{
			size = 3;
			format += 3;
		}
		else if (*format == 'L')
		{
			size = 4;
			format++;
		}

		conversion = *format;
		if (!conversion)
			break;
		format++;

		switch (conversion)
		{
		case 's':
			if (size == 1)
				output_narrow(output, va_arg(arguments, const char *), precision, width, left);
			else
				output_wide(output, va_arg(arguments, const wchar_t *), precision, width, left);
			break;
		case 'S':
			if (size == 2)
				output_wide(output, va_arg(arguments, const wchar_t *), precision, width, left);
			else
				output_narrow(output, va_arg(arguments, const char *), precision, width, left);
			break;
		case 'c':
		case 'C':
		{
			wchar_t character[2] = { (wchar_t)va_arg(arguments, int), 0 };

			if ((conversion == 'c' && size == 1) || (conversion == 'C' && size != 2))
				character[0] = (wchar_t)(unsigned char)character[0];
			output_wide(output, character, -1, width, left);
			break;
		}
		case 'n':
		{
			int *count = va_arg(arguments, int *);

			if (count)
				*count = (int)output->length;
			break;
		}
		case 'd': case 'i': case 'u': case 'x': case 'X': case 'o':
		case 'e': case 'E': case 'f': case 'g': case 'G': case 'p':
		{
			char narrow[512];
			wchar_t wide[512];

			if (conversion == 'p')
			{
				snprintf(narrow, sizeof(narrow), "%08lX", (unsigned long)va_arg(arguments, void *));
			}
			else if (conversion == 'e' || conversion == 'E' || conversion == 'f' ||
				conversion == 'g' || conversion == 'G')
			{
				specification[specification_length++] = (char)conversion;
				specification[specification_length] = '\0';
				snprintf(narrow, sizeof(narrow), specification, va_arg(arguments, double));
			}
			else if (size == 3)
			{
				specification[specification_length++] = 'l';
				specification[specification_length++] = 'l';
				specification[specification_length++] = (char)conversion;
				specification[specification_length] = '\0';
				snprintf(narrow, sizeof(narrow), specification, va_arg(arguments, long long));
			}
			else
			{
				int value = va_arg(arguments, int);

				if (size == 1)
					value = (conversion == 'd' || conversion == 'i') ? (short)value : (unsigned short)value;
				specification[specification_length++] = (char)conversion;
				specification[specification_length] = '\0';
				snprintf(narrow, sizeof(narrow), specification, value);
			}
			narrow_to_wide(narrow, strlen(narrow), wide, sizeof(wide) / sizeof(wide[0]));
			output_wide(output, wide, -1, 0, FALSE);
			break;
		}
		default:
			/* unknown conversion: print it literally, as MSVC does */
			output_character(output, conversion);
			break;
		}
	}
	if (output->capacity)
		output->buffer[output->length < output->capacity ? output->length : output->capacity - 1] = 0;
	return (int)output->length;
}

int msvc_vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list arguments)
{
	struct wide_output output = { buffer, count, 0 };
	int length = wide_format(&output, format, arguments);

	/* MSVC returns -1 when the output (and its terminator) did not fit */
	return (size_t)length < count ? length : -1;
}

int msvc_snwprintf(wchar_t *buffer, size_t count, const wchar_t *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = msvc_vsnwprintf(buffer, count, format, arguments);
	va_end(arguments);
	return result;
}

int msvc_vswprintf(wchar_t *buffer, const wchar_t *format, va_list arguments)
{
	/* the pre-C99 MSVC form writes without a bound */
	return msvc_vsnwprintf(buffer, (size_t)INT_MAX, format, arguments);
}

int msvc_swprintf(wchar_t *buffer, const wchar_t *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = msvc_vswprintf(buffer, format, arguments);
	va_end(arguments);
	return result;
}

int msvc_vfwprintf(FILE *stream, const wchar_t *format, va_list arguments)
{
	wchar_t wide[4096];
	char narrow[4096 * 3];
	struct wide_output output = { wide, sizeof(wide) / sizeof(wide[0]), 0 };
	int length = wide_format(&output, format, arguments);

	wide_to_utf8(wide, sizeof(wide) / sizeof(wide[0]), narrow, sizeof(narrow));
	fputs(narrow, stream);
	return length;
}

int msvc_fwprintf(FILE *stream, const wchar_t *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = msvc_vfwprintf(stream, format, arguments);
	va_end(arguments);
	return result;
}

int msvc_vwprintf(const wchar_t *format, va_list arguments)
{
	return msvc_vfwprintf(stdout, format, arguments);
}

int msvc_wprintf(const wchar_t *format, ...)
{
	va_list arguments;
	int result;

	va_start(arguments, format);
	result = msvc_vwprintf(format, arguments);
	va_end(arguments);
	return result;
}

/* ---------- streams (UTF-16 little endian, as the game's text files are) */

wint_t msvc_fgetwc(FILE *stream)
{
	int low = fgetc(stream);
	int high;

	if (low == EOF)
		return WEOF;
	high = fgetc(stream);
	if (high == EOF)
		return WEOF;
	return (wint_t)(low | (high << 8));
}

wint_t msvc_fputwc(wchar_t character, FILE *stream)
{
	if (fputc(character & 0xff, stream) == EOF || fputc(character >> 8, stream) == EOF)
		return WEOF;
	return (wint_t)character;
}

wint_t msvc_ungetwc(wint_t character, FILE *stream)
{
	/* stdio guarantees only one byte of push-back, so step back instead */
	if (character == WEOF || fseek(stream, -2, SEEK_CUR) != 0)
		return WEOF;
	return character;
}

wchar_t *msvc_fgetws(wchar_t *string, int count, FILE *stream)
{
	int index = 0;

	if (count <= 0)
		return NULL;
	while (index + 1 < count)
	{
		wint_t character = msvc_fgetwc(stream);

		if (character == WEOF)
			break;
		string[index++] = (wchar_t)character;
		if (character == '\n')
			break;
	}
	if (!index)
		return NULL;
	string[index] = 0;
	return string;
}

int msvc_fputws(const wchar_t *string, FILE *stream)
{
	for (; *string; string++)
	{
		if (msvc_fputwc(*string, stream) == WEOF)
			return EOF;
	}
	return 0;
}

wchar_t *msvc_getws(wchar_t *string)
{
	/* console input is narrow; read a line and widen it */
	char line[1024];
	size_t length;

	if (!fgets(line, sizeof(line), stdin))
		return NULL;
	length = strlen(line);
	if (length && line[length - 1] == '\n')
		line[--length] = '\0';
	narrow_to_wide(line, length, string, length + 1);
	return string;
}

int msvc_putws(const wchar_t *string)
{
	char narrow[4096];

	wide_to_utf8(string, (size_t)-1, narrow, sizeof(narrow));
	return puts(narrow);
}

/* ---------- files and time by wide name */

FILE *halo_linux_fopen(const char *path, const char *mode);
FILE *halo_linux_freopen(const char *path, const char *mode, FILE *stream);
int halo_linux_remove(const char *path);

FILE *msvc_wfopen(const wchar_t *path, const wchar_t *mode)
{
	char narrow_path[1024];
	char narrow_mode[16];

	wide_to_utf8(path, (size_t)-1, narrow_path, sizeof(narrow_path));
	wide_to_utf8(mode, (size_t)-1, narrow_mode, sizeof(narrow_mode));
	return halo_linux_fopen(narrow_path, narrow_mode);
}

FILE *_wfreopen(const wchar_t *path, const wchar_t *mode, FILE *stream)
{
	char narrow_path[1024];
	char narrow_mode[16];

	wide_to_utf8(mode, (size_t)-1, narrow_mode, sizeof(narrow_mode));
	if (!path)
		return halo_linux_freopen(NULL, narrow_mode, stream);
	wide_to_utf8(path, (size_t)-1, narrow_path, sizeof(narrow_path));
	return halo_linux_freopen(narrow_path, narrow_mode, stream);
}

FILE *_wfdopen(int handle, const wchar_t *mode)
{
	char narrow_mode[16];

	wide_to_utf8(mode, (size_t)-1, narrow_mode, sizeof(narrow_mode));
	return fdopen(handle, narrow_mode);
}

int msvc_wremove(const wchar_t *path)
{
	char narrow_path[1024];

	wide_to_utf8(path, (size_t)-1, narrow_path, sizeof(narrow_path));
	return halo_linux_remove(narrow_path);
}

wchar_t *msvc_wtmpnam(wchar_t *string)
{
	static wchar_t buffer[L_tmpnam];
	static unsigned long counter = 0;
	char narrow[L_tmpnam];

	snprintf(narrow, sizeof(narrow), "z:\\tmp%lu", ++counter);
	if (!string)
		string = buffer;
	narrow_to_wide(narrow, strlen(narrow), string, L_tmpnam);
	return string;
}

wchar_t *msvc_wasctime(const struct tm *time)
{
	static wchar_t buffer[64];
	char narrow[64];

	if (!time || !strftime(narrow, sizeof(narrow), "%a %b %d %H:%M:%S %Y\n", time))
		return NULL;
	narrow_to_wide(narrow, strlen(narrow), buffer, sizeof(buffer) / sizeof(buffer[0]));
	return buffer;
}

wchar_t *msvc_wctime(const time_t *timer)
{
	struct tm local;

	if (!timer || !localtime_r(timer, &local))
		return NULL;
	return msvc_wasctime(&local);
}

wchar_t *msvc_wcserror(int error_number)
{
	static wchar_t buffer[256];
	const char *text = strerror(error_number);

	narrow_to_wide(text, strlen(text), buffer, sizeof(buffer) / sizeof(buffer[0]));
	return buffer;
}

void msvc_wperror(const wchar_t *string)
{
	char narrow[1024];

	if (string && *string)
	{
		wide_to_utf8(string, (size_t)-1, narrow, sizeof(narrow));
		fprintf(stderr, "%s: %s\n", narrow, strerror(errno));
	}
	else
	{
		fprintf(stderr, "%s\n", strerror(errno));
	}
}
