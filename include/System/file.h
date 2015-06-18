/* $Id$ */
/* Copyright (c) 2006-2015 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_FILE_H
# define LIBSYSTEM_SYSTEM_FILE_H

# include <sys/types.h>


/* types */
typedef struct _File File;
typedef unsigned int FileMode;

typedef unsigned int FileSeekMode;

/* constants */
#define FILE_MODE_READ		0x01
#define FILE_MODE_WRITE		0x02
#define FILE_MODE_APPEND	0x10
#define FILE_MODE_CREATE	0x20
#define FILE_MODE_EXCLUSIVE	0x40
#define FILE_MODE_TRUNCATE	0x80

/* functions */
File * file_new(char const * path, FileMode mode);
int file_delete(File * file);

/* accessors */
FileMode file_get_mode(File * file);

/* useful */
ssize_t file_read(File * file, void * buf, size_t size, ssize_t count);
ssize_t file_write(File * file, void * buf, size_t size, ssize_t count);

int file_seek(File * file, FileSeekMode mode, off_t offset);

int file_unlink(File * file);

#endif /* !LIBSYSTEM_SYSTEM_FILE_H */
