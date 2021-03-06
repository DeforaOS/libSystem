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



#ifndef LIBSYSTEM_SYSTEM_BUFFER_H
# define LIBSYSTEM_SYSTEM_BUFFER_H

# include <stddef.h>

# ifdef __cplusplus
extern "C" {
# endif


/* Buffer */
/* types */
typedef struct _Buffer Buffer;

typedef char BufferData;


/* functions */
Buffer * buffer_new(size_t size, BufferData const * data);
Buffer * buffer_new_copy(Buffer const * buffer);
void buffer_delete(Buffer * buffer);

/* accessors */
BufferData const * buffer_get_data(Buffer const * buffer);
int buffer_set(Buffer * buffer, size_t size, BufferData const * data);
int buffer_set_data(Buffer * buffer, size_t offset, BufferData const * data,
		size_t size);

size_t buffer_get_size(Buffer const * buffer);
int buffer_set_size(Buffer * buffer, size_t size);

# ifdef __cplusplus
}
# endif

#endif /* !LIBSYSTEM_SYSTEM_BUFFER_H */
