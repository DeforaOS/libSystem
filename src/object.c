/* $Id$ */
/* Copyright (c) 2008-2017 Pierre Pronchery <khorben@defora.org> */
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
