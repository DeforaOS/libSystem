/* $Id$ */
/* Copyright (c) 2005-2016 Pierre Pronchery <khorben@defora.org> */
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
	if((array = object_new(sizeof(*array))) == NULL)
		return NULL;
	array->count = 0;
	array->size = size;
	array->value = NULL;
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
size_t array_count(Array * array)
{
	return array->count;
}


/* array_get */
void * array_get(Array * array, size_t pos)
{
	uint64_t offset;

	if(pos >= array->count)
		return NULL;
	offset = pos * array->size;
	return &array->value[offset];
}


/* array_get_copy */
int array_get_copy(Array * array, size_t pos, void * value)
{
	uint64_t offset;

	if(pos >= array->count)
		return 1;
	offset = pos * array->size;
	memcpy(value, &array->value[offset], array->size);
	return 0;
}


/* array_set */
int array_set(Array * array, size_t pos, void * value)
{
	uint32_t p = pos + 1;
	uint64_t offset;
	uint64_t curpos;
	void * q;

	/* check for overflows */
	if(pos >= UINT32_MAX)
		return -error_set_code(1, "%s", strerror(ERANGE));
	offset = pos * array->size;
	if(array->count < p)
	{
		/* grow the array */
		if(UINT64_MAX - offset < array->size
				|| offset + array->size > SIZE_MAX)
			return -error_set_code(-ERANGE, "%s", strerror(ERANGE));
		if((q = realloc(array->value, offset + array->size)) == NULL)
			return -error_set_code(-errno, "%s", strerror(errno));
		array->value = q;
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
int array_append(Array * array, void * value)
{
	char * p;
	uint64_t offset = array->size * array->count;

	/* check for overflows */
	if(UINT64_MAX - offset < array->size
			|| offset + array->size > SIZE_MAX)
		return -error_set_code(-ERANGE, "%s", strerror(ERANGE));
	if((p = realloc(array->value, offset + array->size)) == NULL)
		return error_set_code(-errno, "%s", strerror(errno));
	array->value = p;
	memcpy(&p[offset], value, array->size);
	array->count++;
	return 0;
}


/* array_remove_pos */
int array_remove_pos(Array * array, size_t pos)
{
	if(pos >= array->count)
		return -error_set_code(-ERANGE, "%s", strerror(ERANGE));
	array->count--; /* FIXME resize array? */
	memmove(&array->value[pos * array->size],
			&array->value[(pos + 1) * array->size],
			(array->count - pos) * array->size);
	return 0;
}


/* array_foreach */
void array_foreach(Array * array, ArrayForeach func, void * data)
{
	uint32_t i;
	uint64_t offset;

	for(i = 0, offset = 0; i < array->count; i++, offset += array->size)
		func(array->value + offset, data);
}
