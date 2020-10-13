/* $Id$ */
/* Copyright (c) 2006-2020 Pierre Pronchery <khorben@defora.org> */
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
/* TODO
 * - use a single allocation for each buffer */



#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/buffer.h"


/* Buffer */
/* private */
/* types */
struct _Buffer
{
	size_t size;
	BufferData * data;
};


/* public */
/* functions */
/* buffer_new */
Buffer * buffer_new(size_t size, BufferData const * data)
{
	Buffer * buffer;

	if((buffer = (Buffer *)object_new(sizeof(*buffer))) == NULL)
		return NULL;
	if((buffer->data = (BufferData *)object_new(size)) == NULL && size != 0)
	{
		object_delete(buffer);
		return NULL;
	}
	if(data == NULL)
		memset(buffer->data, 0, size);
	else
		memcpy(buffer->data, data, size);
	buffer->size = size;
	return buffer;
}


/* buffer_new_copy */
Buffer * buffer_new_copy(Buffer const * buffer)
{
	return buffer_new(buffer->size, buffer->data);
}


/* buffer_delete */
void buffer_delete(Buffer * buffer)
{
	object_delete(buffer->data);
	object_delete(buffer);
}


/* accessors */
/* buffer_get_data */
BufferData const * buffer_get_data(Buffer const * buffer)
{
	return buffer->data;
}


/* buffer_get_size */
size_t buffer_get_size(Buffer const * buffer)
{
	return buffer->size;
}


/* buffer_set */
int buffer_set(Buffer * buffer, size_t size, BufferData const * data)
{
	if(buffer_set_size(buffer, size) != 0)
		return -1;
	if(data == NULL)
		memset(buffer->data, 0, size);
	else
		memcpy(buffer->data, data, size);
	return 0;
}


/* buffer_set_data */
int buffer_set_data(Buffer * buffer, size_t offset, BufferData const * data,
		size_t size)
{
	size_t s = offset + size;

	if(s < offset && s < size)
		return error_set_code(-ERANGE, "%s", strerror(ERANGE));
	if(offset + size > buffer->size)
		if(buffer_set_size(buffer, offset + size) != 0)
			return -1;
	memcpy(&buffer->data[offset], data, size);
	return 0;
}


/* buffer_set_size */
int buffer_set_size(Buffer * buffer, size_t size)
{
	BufferData * p;

	if(size == buffer->size)
		return 0;
	if((p = (BufferData *)realloc(buffer->data, size)) == NULL && size != 0)
		return error_set_code(-errno, "%s", strerror(errno));
	buffer->data = p;
	if(size > buffer->size)
		memset(&buffer->data[buffer->size], 0, size - buffer->size);
	buffer->size = size;
	return 0;
}
