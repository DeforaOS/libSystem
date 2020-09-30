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



#ifndef LIBSYSTEM_SYSTEM_HASH_H
# define LIBSYSTEM_SYSTEM_HASH_H

# include <stddef.h>


/* Hash */
/* types */
typedef struct _Hash Hash;

typedef unsigned int (*HashFunc)(void const * value);
typedef int (*HashCompare)(void const * value1, void const * value2);
typedef void (*HashForeach)(void const * key, void * value, void * data);


/* functions */
Hash * hash_new(HashFunc func, HashCompare compare);
void hash_delete(Hash * h);

/* helpers */
extern unsigned int hash_func_string(void const * value);
extern int hash_compare_string(void const * value1, void const * value2);

/* accessors */
void * hash_get(Hash const * h, void const * key);
void const * hash_get_key(Hash const * h, void const * key);
int hash_set(Hash * h, void const * key, void * value);
size_t hash_count(Hash const * hash);

/* useful */
void hash_foreach(Hash const * hash, HashForeach func, void * data);
int hash_reset(Hash * hash);

#endif /* !LIBSYSTEM_SYSTEM_HASH_H */
