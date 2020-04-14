/* $Id$ */
/* Copyright (c) 2005-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/string.h"


/* String */
/* public */
/* string_new */
String * string_new(String const * string)
{
	String * ret = NULL;

	if(string == NULL)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		return NULL;
	}
	if(string_set(&ret, string) != 0)
		return NULL;
	return ret;
}


/* string_new_append */
String * string_new_append(String const * string, ...)
{
	String * ret;
	va_list ap;

	va_start(ap, string);
	ret = string_new_appendv(string, ap);
	va_end(ap);
	return ret;
}


/* string_new_appendv */
String * string_new_appendv(String const * string, va_list ap)
{
	String * ret = NULL;

	if(string == NULL)
		return string_new("");
	ret = string_new(string);
	for(string = va_arg(ap, String *); string != NULL;
			string = va_arg(ap, String *))
		if(string_append(&ret, string) != 0)
		{
			string_delete(ret);
			ret = NULL;
			break;
		}
	return ret;
}


/* string_new_format */
String * string_new_format(String const * format, ...)
{
	String * ret;
	va_list ap;

	va_start(ap, format);
	ret = string_new_formatv(format, ap);
	va_end(ap);
	return ret;
}


/* string_new_formatv */
String * string_new_formatv(String const * format, va_list ap)
{
	String * ret;
	va_list v;
	int len;
	size_t s;

	if(format == NULL)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		return NULL;
	}
	va_copy(v, ap);
	len = vsnprintf(NULL, 0, format, v);
	va_end(v);
	if(len < 0)
	{
		error_set_code(-errno, "%s", strerror(errno));
		return NULL;
	}
	s = (size_t)len + 1;
	if((ret = object_new(s)) == NULL)
		return NULL;
	if(vsnprintf(ret, s, format, ap) != len)
	{
		error_set_code(-errno, "%s", strerror(errno));
		object_delete(ret);
		ret = NULL;
	}
	return ret;
}


/* string_new_length */
String * string_new_length(String const * string, size_t length)
{
	String * ret;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %zu)\n", __func__, string, length);
#endif
	if(length + 1 == 0)
	{
		error_set_code(-ERANGE, "%s", strerror(ERANGE));
		return NULL;
	}
	if((ret = object_new(length + 1)) == NULL)
		return NULL;
	snprintf(ret, length + 1, "%s", (string != NULL) ? string : "");
	return ret;
}


/* string_new_replace */
String * string_new_replace(String const * string, String const * what,
		String const * by)
{
	String * ret;

	if((ret = string_new(string)) == NULL)
		return NULL;
	if(string_replace(&ret, what, by) != 0)
	{
		string_delete(ret);
		return NULL;
	}
	return ret;
}


/* string_delete */
void string_delete(String * string)
{
	object_delete(string);
}


/* accessors */
/* string_get */
# undef string_get
String const * string_get(String const * string)
{
	return string;
}
# define string_get(a) (a)


/* string_get_length */
size_t string_get_length(String const * string)
{
	size_t length;

	for(length = 0; string[length] != '\0'; length++);
	return length;
}


/* string_get_size */
size_t string_get_size(String const * string)
{
	return string_get_length(string) + 1;
}


/* string_set */
int string_set(String ** string, String const * string2)
{
	size_t len = string_get_length(string2);

	if(object_resize((Object **)string, len + 1) != 0)
		return 1;
	strcpy(*string, string2);
	return 0;
}


/* useful */
/* string_append */
int string_append(String ** string, String const * append)
{
	size_t slength = (*string != NULL) ? string_get_length(*string) : 0;
	size_t alength;

	if(append == NULL)
		return error_set_code(-EINVAL, "%s", strerror(EINVAL));
	if((alength = string_get_length(append)) == 0)
		return 0;
	if(object_resize((Object**)string, slength + alength + 1) != 0)
		return 1;
	strcpy(*string + slength, append);
	return 0;
}


/* string_clear */
void string_clear(String * string)
{
	String * s;

	for(s = string; *s != '\0'; s++)
		*s = '\0';
}


/* string_compare */
int string_compare(String const * string, String const * string2)
{
	int ret;
	unsigned char const * u1;
	unsigned char const * u2;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%s, %s)\n", __func__, string, string2);
#endif
	u1 = (unsigned char const *)string;
	u2 = (unsigned char const *)string2;
	while(*u1 && *u2 && *u1 == *u2)
	{
		u1++;
		u2++;
	}
	ret = *u1 - *u2;
	return ret;
}


/* string_compare_length */
int string_compare_length(String const * string, String const * string2,
		size_t length)
{
	int ret;
	unsigned char const * u1;
	unsigned char const * u2;

	if(length == 0)
		return 0;
	u1 = (unsigned char const *)string;
	u2 = (unsigned char const *)string2;
	while(--length && *u1 && *u2 && *u1 == *u2)
	{
		u1++;
		u2++;
	}
	ret = *u1 - *u2;
	return ret;
}


/* string_explode */
StringArray * string_explode(String const * string, String const * separator)
{
	StringArray * ret;
	String * p;			/* temporary pointer */
	size_t i;			/* current position */
	String const * s;		/* &string[i] */
	ssize_t j;			/* position of the next separator */
	ssize_t l;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\")\n", __func__, string,
			separator);
#endif
	if((ret = stringarray_new()) == NULL)
		return NULL;
	if(separator == NULL || (l = string_get_length(separator)) == 0)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		array_delete(ret);
		return NULL;
	}
	for(i = 0;; i += j + l)
	{
		s = &string[i];
		j = string_index(s, separator);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s(): i=%zu, j=%zd\n", __func__, i, j);
#endif
		if(j < 0)
		{
			if((p = string_new(s)) == NULL
					|| array_append(ret, p) != 0)
			{
				string_delete(p);
				break;
			}
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(): \"%s\"\n", __func__,
					ret[ret_cnt - 1]);
#endif
			return ret;
		}
		if((p = string_new_length(s, j)) == NULL
				|| array_append(ret, p) != 0)
		{
			string_delete(p);
			break;
		}
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s(): \"%s\"\n", __func__,
				ret[ret_cnt - 1]);
#endif
	}
	/* free everything */
	array_foreach(ret, (ArrayForeach)string_delete, NULL);
	array_delete(ret);
	return NULL;
}


/* string_find */
String * string_find(String const * string, String const * key)
{
	ssize_t i;

	if((i = string_index(string, key)) < 0)
		return NULL;
	return (String *)&string[i]; /* XXX */
}


/* string_index */
ssize_t string_index(String const * string, String const * key)
{
	size_t len;
	size_t keylen;
	size_t i;

	len = string_get_length(string);
	if((keylen = string_get_length(key)) == 0)
		return len;
	if(keylen > len)
		return -1;
	for(i = 0; i <= len - keylen; i++)
		if(string_compare_length(&string[i], key, keylen) == 0)
			return i;
	return -1;
}


/* string_ltrim */
size_t string_ltrim(String * string, String const * which)
{
	size_t i;
	size_t j;

	for(i = 0; string[i] != '\0'; i++)
		if(which == NULL)
		{
			if(!isspace((unsigned char)string[i]))
				break;
		}
		else
		{
			for(j = 0; which[j] != '\0' && string[i] != which[j];
					j++);
			if(which[j] == '\0')
				break;
		}
	for(j = i; string[j] != '\0'; j++)
		string[j - i] = string[j];
	string[j - i] = '\0';
	return i;
}


/* string_replace */
int string_replace(String ** string, String const * what, String const * by)
{
	String * ret = NULL;
	String const * p;
	size_t len = string_get_length(what);
	ssize_t index;
	String * q;

	for(p = *string; (index = string_index(p, what)) >= 0; p += index + len)
	{
		if((q = string_new_length(p, index)) == NULL
				|| string_append(&ret, q) != 0
				|| string_append(&ret, by) != 0)
		{
			string_delete(q);
			string_delete(ret);
			return -1;
		}
		string_delete(q);
	}
	if(ret != NULL)
	{
		if(string_append(&ret, p) != 0)
		{
			string_delete(ret);
			return -1;
		}
		string_delete(*string);
		*string = ret;
	}
	return 0;
}


/* string_rindex */
ssize_t string_rindex(String const * string, String const * key)
{
	size_t len;
	size_t keylen;
	ssize_t i;

	len = string_get_length(string);
	if((keylen = string_get_length(key)) == 0)
		return len;
	if(keylen > len)
		return -1;
	for(i = len - keylen; i >= 0; i--)
		if(string_compare_length(&string[i], key, keylen) == 0)
			return i;
	return -1;
}


/* string_rtrim */
size_t string_rtrim(String * string, String const * which)
{
	size_t ret = 0;
	size_t i;
	size_t j;

	for(i = string_get_length(string); i > 0; i--)
		if(which == NULL)
		{
			if(!isspace((unsigned char)string[i - 1]))
				return ret;
			string[i - 1] = '\0';
			ret++;
		}
		else
		{
			for(j = 0; which[j] != '\0'; j++)
				if(string[i - 1] == which[j])
				{
					string[i - 1] = '\0';
					ret++;
					break;
				}
			if(which[j] == '\0')
				break;
		}
	return ret;
}


/* string_tolower */
void string_tolower(String * string)
{
	size_t i;

	for(i = string_get_length(string); i > 0; i--)
		string[i - 1] = tolower((unsigned char)string[i - 1]);
}


/* string_toupper */
void string_toupper(String * string)
{
	size_t i;

	for(i = string_get_length(string); i > 0; i--)
		string[i - 1] = toupper((unsigned char)string[i - 1]);
}


/* string_trim */
size_t string_trim(String * string, String const * which)
{
	return string_ltrim(string, which) + string_rtrim(string, which);
}
