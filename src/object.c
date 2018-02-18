/* $Id$ */
/* Copyright (c) 2008-2018 Pierre Pronchery <khorben@defora.org> */
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



#include <stdlib.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"


/* Object */
/* public */
/* functions */
/* object_new */
Object * object_new(size_t size)
{
	void * object;

	if(size == 0)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		return NULL;
	}
	if((object = malloc(size)) == NULL)
	{
		error_set_code(-errno, "%s", strerror(errno));
		return NULL;
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() => %p\n", __func__, object);
#endif
	return object;
}


/* object_delete */
void object_delete(Object * object)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p)\n", __func__, object);
#endif
	free(object);
}


/* useful */
int object_resize(Object ** object, size_t size)
{
	void * p;

	if((p = realloc(*object, size)) == NULL)
		return error_set_code(-errno, "%s", strerror(errno));
	*object = p;
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(&%p, %zu) => %p\n", __func__, *object, size,
			p);
#endif
	return 0;
}
