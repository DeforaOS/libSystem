/* $Id$ */
/* Copyright (c) 2007-2020 Pierre Pronchery <khorben@defora.org> */
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
static String const * _error_do(ErrorCode * code, String const * format,
		va_list ap);
static ErrorCode _error_do_code(ErrorCode * code);


/* public */
/* accessors */
/* error_get */
static String const * _get_do(ErrorCode * code, ...);

String const * error_get(ErrorCode * code)
{
	/* XXX workaround for portability */
	return _get_do(code);
}

static String const * _get_do(ErrorCode * code, ...)
{
	String const * ret;
	va_list args;

	va_start(args, code);
	ret = _error_do(code, NULL, args);
	va_end(args);
	return ret;
}


/* error_get_code */
ErrorCode error_get_code(void)
{
	return _error_do_code(NULL);
}


/* error_set */
void error_set(String const * format, ...)
{
	va_list ap;

	va_start(ap, format);
	error_setv(format, ap);
	va_end(ap);
}


/* error_setv */
void error_setv(String const * format, va_list ap)
{
	_error_do(NULL, format, ap);
}


/* error_set_code */
ErrorCode error_set_code(ErrorCode code, String const * format, ...)
{
	ErrorCode ret;
	va_list ap;

	va_start(ap, format);
	ret = error_set_codev(code, format, ap);
	va_end(ap);
	return ret;
}


/* error_set_codev */
ErrorCode error_set_codev(ErrorCode code, String const * format, va_list ap)
{
	if(format == NULL)
		format = "";
	_error_do(&code, format, ap);
	return code;
}


/* error_set_print */
ErrorCode error_set_print(String const * program, ErrorCode code,
		String const * format, ...)
{
	ErrorCode ret;
	va_list ap;

	va_start(ap, format);
	ret = error_set_printv(program, code, format, ap);
	va_end(ap);
	return ret;
}


/* error_set_printv */
ErrorCode error_set_printv(String const * program, ErrorCode code,
		String const * format, va_list ap)
{
	_error_do(&code, format, ap);
	return error_print(program);
}


/* useful */
/* error_print */
static ErrorCode _print_do(String const * program, ...);

ErrorCode error_print(String const * program)
{
	/* XXX workaround for portability */
	return _print_do(program);
}

static ErrorCode _print_do(String const * program, ...)
{
	ErrorCode ret = 0;
	va_list args;
	String const * error;

	va_start(args, program);
	if(program != NULL && string_get_length(program) > 0)
	{
		fputs(program, stderr);
		fputs(": ", stderr);
	}
	if((error = _error_do(&ret, NULL, args)) == NULL
			|| string_get_length(error) == 0)
		error = "Unknown error";
	fputs(error, stderr);
	fputc('\n', stderr);
	va_end(args);
	return ret;
}


/* private */
/* functions */
/* error_do */
static String const * _error_do(ErrorCode * code, String const * format,
		va_list ap)
{
	static String buf[256] = "";

	if(format != NULL) /* setting the error */
	{
		vsnprintf(buf, sizeof(buf), format, ap);
		if(code != NULL)
			_error_do_code(code);
	}
	else if(code != NULL)
		*code = _error_do_code(NULL);
	return buf;
}


/* error_do_code */
static ErrorCode _error_do_code(ErrorCode * code)
{
	static int _code = 0;

	if(code != NULL)
		_code = *code;
	return _code;
}
