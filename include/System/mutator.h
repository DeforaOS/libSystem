/* $Id$ */
/* Copyright (c) 2014-2020 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_MUTATOR_H
# define LIBSYSTEM_SYSTEM_MUTATOR_H

# include "string.h"

# ifdef __cplusplus
extern "C" {
# endif


/* Mutator */
/* types */
typedef struct _Hash Mutator;

typedef void (*MutatorForeach)(String const * key, void * value, void * data);


/* functions */
Mutator * mutator_new(void);
Mutator * mutator_new_copy(Mutator const * from);
void mutator_delete(Mutator * mutator);

/* accessors */
void * mutator_get(Mutator const * mutator, String const * key);
int mutator_set(Mutator * mutator, String const * key, void * value);
size_t mutator_count(Mutator const * mutator);

/* useful */
void mutator_foreach(Mutator const * mutator, MutatorForeach func, void * data);
int mutator_reset(Mutator * mutator);

# ifdef __cplusplus
}
# endif

#endif /* !LIBSYSTEM_SYSTEM_MUTATOR_H */
