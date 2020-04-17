/* $Id$ */
/* Copyright (c) 2012-2018 Pierre Pronchery <khorben@defora.org> */
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
#include "System/variable.h"


/* variable */
static int _variable(char const * progname)
{
	int ret = 0;
	Variable * variable;
	const int samples[] = { 0, -1, 1, -127, -128, 126, 127 };
	const float fsamples[] = { 0.0, -1.0, 1.0 };
	size_t i;
	int32_t j;
	int8_t i8;
	size_t s;
	void * p;
	double d;

	/* variable_new */
	for(i = 0; i < sizeof(samples) / sizeof(*samples); i++)
	{
		printf("%s: Testing variable_new_deserialize_type(): %d\n",
				progname, samples[i]);
		i8 = samples[i];
		s = sizeof(i8);
		p = &i8;
		if((variable = variable_new_deserialize_type(VT_INT8, &s, p))
				== NULL)
		{
			error_print(progname);
			ret += 1;
			continue;
		}
		p = &j;
		if(variable_get_as(variable, VT_INT32, p) != 0
				|| j != samples[i])
		{
			error_print(progname);
			ret += 1;
		}
		variable_delete(variable);
	}
	for(i = 0; i < sizeof(fsamples) / sizeof(*fsamples); i++)
	{
		if((variable = variable_new(VT_FLOAT, fsamples[i])) == NULL)
		{
			error_print(progname);
			ret += 1;
			continue;
		}
		d = 0.0 / 0.0;
		if(variable_get_as(variable, VT_DOUBLE, &d) != 0
				|| d != fsamples[i])
		{
			error_print(progname);
			ret += 1;
		}
		variable_delete(variable);
	}
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	(void) argc;

	return (_variable(argv[0]) == 0) ? 0 : 2;
}
