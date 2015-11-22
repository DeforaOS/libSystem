/* $Id$ */
/* Copyright (c) 2007-2015 Pierre Pronchery <khorben@defora.org> */
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
static int _error_do_code(int * codeptr);


/* public */
/* accessors */
/* error_get */
static String const * _get_do(int * code, ...);

String const * error_get(int * code)
{
	/* XXX workaround for portability */
	return _get_do(code);
}

static String const * _get_do(int * code, ...)
{
	String const * ret;
	va_list args;

	va_start(args, code);
	ret = _error_do(code, NULL, args);
	va_end(args);
	return ret;
}


/* error_get_code */
int error_get_code(void)
{
	return _error_do_code(NULL);
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

	if(format != NULL) /* setting the error */
	{
		vsnprintf(buf, sizeof(buf), format, args);
		if(codeptr != NULL)
			_error_do_code(codeptr);
	}
	else if(codeptr != NULL)
		*codeptr = _error_do_code(NULL);
	return buf;
}


/* error_do_code */
static int _error_do_code(int * codeptr)
{
	static int code = 0;

	if(codeptr != NULL)
		code = *codeptr;
	return code;
}
