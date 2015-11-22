/* $Id$ */
/* Copyright (c) 2007-2013 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stdarg.h>
#include <stdio.h>
#include "System/error.h"


/* Error */
/* private */
/* prototypes */
static String const * _error_do(int * codeptr, String const * format,
		va_list args);


/* public */
/* accessors */
/* error_get */
static String const * _get_do(String const * null, ...);

String const * error_get(void)
{
	/* XXX workaround for portability */
	return _get_do(NULL);
}

static String const * _get_do(String const * null, ...)
{
	String const * ret;
	va_list args;

	va_start(args, null);
	ret = _error_do(NULL, NULL, args);
	va_end(args);
	return ret;
}


/* error_get_code */
static String const * _get_code_do(int * code, ...);

String const * error_get_code(int * code)
{
	/* XXX workaround for portability */
	return _get_code_do(code);
}

static String const * _get_code_do(int * code, ...)
{
	String const * ret;
	va_list args;

	va_start(args, code);
	ret = _error_do(code, NULL, args);
	va_end(args);
	return ret;
}


/* error_set */
void error_set(String const * format, ...)
{
	va_list args;

	va_start(args, format);
	_error_do(NULL, format, args);
	va_end(args);
}


/* error_set_code */
int error_set_code(int code, String const * format, ...)
{
	va_list args;

	va_start(args, format);
	if(format == NULL)
		format = "";
	_error_do(&code, format, args);
	va_end(args);
	return code;
}


/* error_set_print */
int error_set_print(String const * program, int code, String const * format, ...)
{
	va_list args;

	va_start(args, format);
	_error_do(&code, format, args);
	va_end(args);
	return error_print(program);
}


/* useful */
/* error_print */
static int _print_do(String const * program, ...);

int error_print(String const * program)
{
	/* XXX workaround for portability */
	return _print_do(program);
}

static int _print_do(String const * program, ...)
{
	int ret = 0;
	va_list args;

	va_start(args, program);
	if(program != NULL)
	{
		fputs(program, stderr);
		fputs(": ", stderr);
	}
	fputs(_error_do(&ret, NULL, args), stderr);
	fputc('\n', stderr);
	va_end(args);
	return ret;
}


/* private */
/* functions */
/* error_do */
static String const * _error_do(int * codeptr, String const * format,
		va_list args)
{
	static String buf[256] = "";
	static int code = 0;

	if(format != NULL) /* setting the error */
	{
		vsnprintf(buf, sizeof(buf), format, args);
		if(codeptr != NULL)
			code = *codeptr;
		return buf;
	}
	if(codeptr != NULL)
		*codeptr = code;
	return buf;
}
