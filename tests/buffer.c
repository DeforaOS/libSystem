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
#include <string.h>
#include "System/error.h"
#include "System/buffer.h"


/* buffer */
static int _buffer(String const * progname, size_t size, char const * data)
{
	int ret = 0;
	Buffer * buffer;

	printf("%s: Testing buffer_new()\n", progname);
	fflush(stdout);
	if((buffer = buffer_new(size, data)) == NULL)
	{
		error_print(progname);
		return 2;
	}
	if(buffer_get_size(buffer) != size)
		ret = 3;
	if(buffer_get_data(buffer) == NULL && size != 0)
		ret = 4;
	if(buffer_set(buffer, size, data) != 0
			|| buffer_set(buffer, size, NULL) != 0)
		ret = 5;
	if(buffer_set_size(buffer, size) != 0
			|| buffer_set_size(buffer, 0) != 0)
		ret = 6;
	buffer_delete(buffer);
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	int ret;
	char buf[65536];
	(void) argc;

	ret = _buffer(argv[0], 0, NULL);
	ret |= _buffer(argv[0], sizeof(buf), NULL);
	memset(buf, 0, sizeof(buf));
	ret |= _buffer(argv[0], sizeof(buf), buf);
	return ret;
}
