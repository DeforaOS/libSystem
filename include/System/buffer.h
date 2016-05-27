/* $Id$ */
/* Copyright (c) 2006-2016 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_BUFFER_H
# define LIBSYSTEM_SYSTEM_BUFFER_H

# include <sys/types.h>


/* Buffer */
/* types */
typedef struct _Buffer Buffer;


/* functions */
Buffer * buffer_new(size_t size, char const * data);
Buffer * buffer_new_copy(Buffer const * buffer);
void buffer_delete(Buffer * buffer);

/* accessors */
char * buffer_get_data(Buffer const * buffer);
int buffer_set(Buffer * buffer, size_t size, char * data);
int buffer_set_data(Buffer * buffer, size_t offset, char * data, size_t size);

size_t buffer_get_size(Buffer const * buffer);
int buffer_set_size(Buffer * buffer, size_t size);

#endif /* !LIBSYSTEM_SYSTEM_BUFFER_H */
