/* $Id$ */
/* Copyright (c) 2007-2015 Pierre Pronchery <khorben@defora.org> */
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
	String const * error;

	va_start(args, program);
	if(program != NULL && string_length(program) > 0)
	{
		fputs(program, stderr);
		fputs(": ", stderr);
	}
	if((error = _error_do(&ret, NULL, args)) == NULL
			|| string_length(error) == 0)
		error = "Unknown error";
	fputs(error, stderr);
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
