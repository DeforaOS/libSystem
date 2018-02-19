/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
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



#include <stdio.h>
#include "System/error.h"
#include "System/parser.h"


/* parser */
static int _parser(char const * progname)
{
	int ret = 0;
	Parser * parser;
	char const * p;

	printf("%s: Testing parser_new()\n", progname);
	fflush(stdout);
	if((parser = parser_new(NULL)) != NULL)
		return -1;
	else
		error_print(progname);
	if((parser = parser_new("config-nonexistent.conf")) != NULL)
		return -1;
	else
		error_print(progname);
	if((parser = parser_new("config-empty.conf")) == NULL)
		return error_print(progname);
	if((p = parser_get_filename(parser)) == NULL)
		ret = error_print(progname);
	else if(string_compare(p, "config-empty.conf") != 0)
		ret = error_print(progname);
	if(parser_scan(parser) == 0)
		ret = error_set_print(progname, 2, "%s",
				"Should not be able to scan");
	parser_delete(parser);
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	(void) argc;

	return (_parser(argv[0]) == 0) ? 0 : 2;
}
