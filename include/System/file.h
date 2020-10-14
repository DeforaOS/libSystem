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



#ifndef LIBSYSTEM_SYSTEM_FILE_H
# define LIBSYSTEM_SYSTEM_FILE_H

# include <sys/types.h>
# include <stdbool.h>
# include "buffer.h"
# include "string.h"

# ifdef __cplusplus
extern "C" {
# endif


/* types */
typedef struct _File File;
typedef int FileError;
typedef unsigned int FileMode;
typedef off_t FileOffset;
typedef enum _FileSeekMode
{
	FILE_SEEK_MODE_CURRENT = 0,
	FILE_SEEK_MODE_END,
	FILE_SEEK_MODE_SET
} FileSeekMode;


/* constants */
#define FILE_MODE_READ		0x01
#define FILE_MODE_WRITE		0x02
#define FILE_MODE_READ_WRITE	0x03
#define FILE_MODE_APPEND	0x10
#define FILE_MODE_CREATE	0x20
#define FILE_MODE_EXCLUSIVE	0x40
#define FILE_MODE_TRUNCATE	0x80


/* functions */
File * file_new(String const * filename, FileMode mode);
FileError file_delete(File * file);

/* accessors */
String const * file_get_filename(File * file);
FileMode file_get_mode(File * file);

bool file_is_end(File * file);

/* useful */
FileError file_read(File * file, void * buf, size_t size, size_t * count);
FileError file_read_buffer(File * file, Buffer * buffer);
FileError file_write(File * file, const void * buf, size_t size,
		size_t * count);
FileError file_write_buffer(File * file, Buffer const * buffer);

FileError file_seek(File * file, FileSeekMode mode, FileOffset offset);

FileError file_unlink(File * file);

# ifdef __cplusplus
}
# endif

#endif /* !LIBSYSTEM_SYSTEM_FILE_H */
