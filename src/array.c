/* $Id$ */
/* Copyright (c) 2005-2020 Pierre Pronchery <khorben@defora.org> */
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



#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/array.h"


/* Array */
/* protected */
/* types */
struct _Array
{
	uint32_t count;
	uint32_t size;
	char * value;
};


/* public */
/* array_new */
Array * array_new(size_t size)
{
	Array * array;

	/* check for overflows */
	if(UINT32_MAX < SIZE_MAX && size > UINT32_MAX)
	{
		error_set_code(-ERANGE, "%s", strerror(ERANGE));
		return NULL;
	}
	if((array = (Array *)object_new(sizeof(*array))) == NULL)
		return NULL;
	array->count = 0;
	array->size = size;
	array->value = NULL;
	return array;
}


/* array_new_copy */
Array * array_new_copy(Array const * from)
{
	/* FIXME implement */
	return NULL;
}


/* array_new_filter */
Array * array_new_filter(Array const * from, ArrayFilter func, UserData * data)
{
	Array * array;
	size_t i;
	char * value;

	if((array = array_new(from->size)) == NULL)
		return NULL;
	for(i = 0; i < from->count; i++)
	{
		value = from->value + (from->size * i);
		if(func(value, data) == true)
			if(array_append(array, value) != 0)
			{
				array_delete(array);
				return NULL;
			}
	}
	return array;
}


/* array_new_filter_swap */
Array * array_new_filter_swap(Array const * from, ArrayFilterSwap func,
		UserData * data)
{
	Array * array;
	size_t i;
	char * value;

	if((array = array_new(from->size)) == NULL)
		return NULL;
	for(i = 0; i < from->count; i++)
	{
		value = from->value + (from->size * i);
		if(func(data, value) == true)
			if(array_append(array, value) != 0)
			{
				array_delete(array);
				return NULL;
			}
	}
	return array;
}


/* array_delete */
void array_delete(Array * array)
{
	free(array->value);
	object_delete(array);
}


/* accessors */
/* array_count */
size_t array_count(Array const * array)
{
	return array->count;
}


/* array_get */
void * array_get(Array const * array, size_t pos)
{
	uint64_t offset;

	if(pos >= array->count)
		return NULL;
	offset = pos * array->size;
	return &array->value[offset];
}


/* array_get_copy */
ArrayError array_get_copy(Array const * array, size_t pos, ArrayData * value)
{
	uint64_t offset;

	if(pos >= array->count)
		return error_set_code(-ERANGE, "%s", strerror(ERANGE));
	offset = pos * array->size;
	memcpy(value, &array->value[offset], array->size);
	return 0;
}


/* array_get_size */
size_t array_get_size(Array const * array)
{
	return array->size;
}


/* array_set */
ArrayError array_set(Array * array, size_t pos, ArrayData * value)
{
	uint32_t p = pos + 1;
	uint64_t offset;
	uint64_t curpos;
	void * q;

	/* check for overflows */
	if(pos >= UINT32_MAX)
		return error_set_code(-ERANGE, "%s", strerror(ERANGE));
	offset = pos * array->size;
	if(array->count < p)
	{
		/* grow the array */
		if(UINT64_MAX - offset < array->size
				|| offset + array->size > SIZE_MAX)
			return error_set_code(-ERANGE, "%s", strerror(ERANGE));
		if((q = realloc(array->value, offset + array->size)) == NULL)
			return error_set_code(-errno, "%s", strerror(errno));
		array->value = (char *)q;
		curpos = array->count * array->size;
		memset(&array->value[curpos], 0, offset - curpos);
		array->count = pos + 1;
	}
	/* set the value */
	memcpy(&array->value[offset], value, array->size);
	return 0;
}


/* useful */
/* array_append */
ArrayError array_append(Array * array, ArrayData * value)
{
	char * p;
	uint64_t offset = array->size * array->count;

	/* check for overflows */
	if(UINT64_MAX - offset < array->size
			|| offset + array->size > SIZE_MAX)
		return error_set_code(-ERANGE, "%s", strerror(ERANGE));
	if((p = (char *)realloc(array->value, offset + array->size)) == NULL)
		return error_set_code(-errno, "%s", strerror(errno));
	array->value = p;
	memcpy(&p[offset], value, array->size);
	array->count++;
	return 0;
}


/* array_copy */
ArrayError array_copy(Array * array, Array const * from)
{
	/* FIXME implement */
	return -1;
}


/* array_prepend */
ArrayError array_prepend(Array * array, ArrayData * value)
{
	/* FIXME implement */
	return -1;
}


/* array_remove_pos */
ArrayError array_remove_pos(Array * array, size_t pos)
{
	void * p;

	if(pos >= array->count)
		return error_set_code(-ERANGE, "%s", strerror(ERANGE));
	array->count--;
	memmove(&array->value[pos * array->size],
			&array->value[(pos + 1) * array->size],
			(array->count - pos) * array->size);
	if((p = realloc(array->value, array->size * array->count)) == NULL
			&& array->count != 0)
		/* XXX ignore the error */
		return 0;
	array->value = (char *)p;
	return 0;
}


/* array_filter */
void array_filter(Array * array, ArrayFilter func, UserData * data)
{
	uint32_t i;
	uint64_t offset;

	for(i = 0, offset = 0; i < array->count;)
		if(func(array->value + offset, data) == false)
			/* cannot fail */
			array_remove_pos(array, i);
		else
		{
			i++;
			offset += array->size;
		}
}


/* array_filter_swap */
void array_filter_swap(Array * array, ArrayFilterSwap func, UserData * data)
{
	uint32_t i;
	uint64_t offset;

	for(i = 0, offset = 0; i < array->count;)
		if(func(data, array->value + offset) == false)
			/* cannot fail */
			array_remove_pos(array, i);
		else
		{
			i++;
			offset += array->size;
		}
}


/* array_foreach */
void array_foreach(Array const * array, ArrayForeach func, UserData * data)
{
	uint32_t i;
	uint64_t offset;

	for(i = 0, offset = 0; i < array->count; i++, offset += array->size)
		func(array->value + offset, data);
}


/* array_foreach_swap */
void array_foreach_swap(Array const * array, ArrayForeachSwap func,
		UserData * data)
{
	uint32_t i;
	uint64_t offset;

	for(i = 0, offset = 0; i < array->count; i++, offset += array->size)
		func(data, array->value + offset);
}
