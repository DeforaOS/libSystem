/* $Id$ */
/* Copyright (c) 2006-2018 Pierre Pronchery <khorben@defora.org> */
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
