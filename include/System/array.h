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



#ifndef LIBSYSTEM_SYSTEM_ARRAY_H
# define LIBSYSTEM_SYSTEM_ARRAY_H

# include <sys/types.h>
# include <stdbool.h>


/* Array */
/* macros */
/* XXX avoid using attributes */
# define ARRAY(type) \
	typedef struct _Array type ## Array; \
	static Array * type ## array_new(void) __attribute__ ((unused)); \
	static Array * type ## array_new(void) \
		{ return array_new(sizeof(type)); }
# define ARRAY2(type, name) \
	typedef struct _Array name ## Array; \
	static Array * name ## array_new(void) __attribute__ ((unused)); \
	static Array * name ## array_new(void) \
		{ return array_new(sizeof(type)); }
# define ARRAY3(type, name, prefix) \
	typedef struct _Array prefix ## Array; \
	static Array * name ## array_new(void) __attribute__ ((unused)); \
	static Array * name ## array_new(void) \
		{ return array_new(sizeof(type)); }


/* types */
typedef struct _Array Array;

/* XXX the Swap variants are useless here */
typedef bool (*ArrayFilter)(void * value, void * data);
typedef bool (*ArrayFilterSwap)(void * data, void * value);
typedef void (*ArrayForeach)(void * value, void * data);
typedef void (*ArrayForeachSwap)(void * data, void * value);


/* functions */
Array * array_new(size_t size);
# define array_new_type(object) array_new(sizeof(type))
void array_delete(Array * array);

/* accessors */
size_t array_count(Array * array);

void * array_get(Array * array, size_t pos);
int array_get_copy(Array * array, size_t pos, void * value);
int array_set(Array * array, size_t pos, void * value);

/* useful */
int array_append(Array * array, void * value);
int array_remove_pos(Array * array, size_t pos);

void array_filter(Array * array, ArrayFilter func, void * data);
void array_filter_swap(Array * array, ArrayFilter func, void * data);
void array_foreach(Array * array, ArrayForeachSwap func, void * data);
void array_foreach_swap(Array * array, ArrayForeachSwap func, void * data);

#endif /* !LIBSYSTEM_SYSTEM_ARRAY_H */
