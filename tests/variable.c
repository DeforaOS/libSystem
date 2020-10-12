/* $Id$ */
/* Copyright (c) 2012-2020 Pierre Pronchery <khorben@defora.org> */
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



#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/variable.h"


/* variable */
/* private */
/* prototypes */
static int _variable_deserialize(char const * progname);
static int _variable_deserialize_type(char const * progname);

static int _variable_perror(int ret, char const * progname,
		char const * message);
static int _variable_serror(int ret, char const * progname,
		char const * message);


/* functions */
/* variable_deserialize */
static int _variable_deserialize(char const * progname)
{
	int ret = 0;
	struct
	{
		char const * filename;
		VariableType type;
	} files[] = {
		{ "variable-samples/null.var",		VT_NULL		},
		{ "variable-samples/bool-false.var",	VT_BOOL		},
		{ "variable-samples/bool-true.var",	VT_BOOL		},
		{ "variable-samples/int8_t-min.var",	VT_INT8		},
		{ "variable-samples/uint8_t-max.var",	VT_UINT8	},
		{ "variable-samples/int16_t-min.var",	VT_INT16	},
		{ "variable-samples/uint16_t-max.var",	VT_UINT16	},
		{ "variable-samples/int32_t-min.var",	VT_INT32	},
		{ "variable-samples/uint32_t-max.var",	VT_UINT32	},
		{ "variable-samples/int64_t-min.var",	VT_INT64	},
		{ "variable-samples/uint64_t-max.var",	VT_UINT64	},
		{ "variable-samples/float-zero.var",	VT_FLOAT	},
		{ "variable-samples/double-zero.var",	VT_DOUBLE	},
		{ "variable-samples/buffer-empty.var",	VT_BUFFER	},
		{ "variable-samples/string-empty.var",	VT_STRING	}
	};
	size_t i;
	int fd;
	char buf[BUFSIZ];
	ssize_t ssize;
	size_t size;
	Variable * variable;

	for(i = 0; i < sizeof(files) / sizeof(*files); i++)
	{
		printf("%s: Testing variable_new_deserialize(): \"%s\"\n",
				progname, files[i].filename);
		if((fd = open(files[i].filename, O_RDONLY)) < 0)
		{
			ret |= _variable_perror(2, progname, files[i].filename);
			continue;
		}
		if((ssize = read(fd, buf, sizeof(buf))) < 0)
		{
			ret |= _variable_perror(4, progname, files[i].filename);
			close(fd);
			continue;
		}
		if(close(fd) != 0)
		{
			ret |= _variable_perror(8, progname, files[i].filename);
			continue;
		}
		size = ssize;
		if((variable = variable_new_deserialize(&size, buf)) == NULL)
		{
			ret |= _variable_serror(16, progname, files[i].filename);
			continue;
		}
		if(variable_get_type(variable) != files[i].type)
		{
			fprintf(stderr, "%s: Wrong type for \"%s\"\n", progname,
					files[i].filename);
			ret |= 32;
		}
		variable_delete(variable);
	}
	return ret;
}


/* variable_deserialize_type */
static int _variable_deserialize_type(char const * progname)
{
	int ret = 0;
	Variable * variable;
	const int samples[] = { 0, -1, 1, -127, -128, 126, 127 };
	const float fsamples[] = { 0.0, -1.0, 1.0 };
	size_t i;
	int32_t j;
	int8_t i8;
	size_t s;
	char * p;
	double d;

	/* variable_new */
	for(i = 0; i < sizeof(samples) / sizeof(*samples); i++)
	{
		printf("%s: Testing variable_new_deserialize_type(): %d\n",
				progname, samples[i]);
		i8 = samples[i];
		s = sizeof(i8);
		p = (char *)&i8;
		if((variable = variable_new_deserialize_type(VT_INT8, &s, p))
				== NULL)
		{
			error_print(progname);
			ret += 1;
			continue;
		}
		p = (char *)&j;
		if(variable_get_as(variable, VT_INT32, (void *)p) != 0
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


/* variable_perror */
static int _variable_perror(int ret, char const * progname,
		char const * message)
{
	fprintf(stderr, "%s: %s%s%s\n", progname,
			(message != NULL) ? message : "",
			(message != NULL) ? ": " : "",
			strerror(errno));
	return ret;
}


/* variable_serror */
static int _variable_serror(int ret, char const * progname,
		char const * message)
{
	fprintf(stderr, "%s: %s%s%s\n", progname,
			(message != NULL) ? message : "",
			(message != NULL) ? ": " : "",
			error_get(NULL));
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	int ret;
	(void) argc;

	ret = _variable_deserialize(argv[0]);
	ret |= _variable_deserialize_type(argv[0]);
	return ret;
}
