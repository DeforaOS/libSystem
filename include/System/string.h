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



#ifndef LIBSYSTEM_SYSTEM_STRING_H
# define LIBSYSTEM_SYSTEM_STRING_H

# include <sys/types.h>
# include <stdarg.h>
# include "array.h"


/* String */
/* types */
typedef char String;

ARRAY3(String *, string, String)


/* functions */
String * string_new(String const * string);
String * string_new_append(String const * string, ...);
String * string_new_format(String const * format, ...);
String * string_new_length(String const * string, size_t length);
String * string_new_replace(String const * string, String const * what,
		String const * by);
void string_delete(String * string);

/* accessors */
String const * string_get(String const * string);
# define string_get(a) (a)
size_t string_get_length(String const * string);
size_t string_get_size(String const * string);

int string_set(String ** string, String const * string2);

/* useful */
int string_append(String ** string, String const * append);
void string_clear(String * string);
void string_cut(String * string, size_t length);

int string_compare(String const * string, String const * string2);
int string_compare_length(String const * string, String const * string2,
		size_t length);

StringArray * string_explode(String const * string, String const * separator);

String * string_find(String const * string, String const * key);
ssize_t string_index(String const * string, String const * key);
ssize_t string_rindex(String const * string, String const * key);

int string_replace(String ** string, String const * what, String const * by);

size_t string_ltrim(String * string, String const * which);
size_t string_rtrim(String * string, String const * which);
size_t string_trim(String * string, String const * which);

void string_tolower(String * string);
void string_toupper(String * string);

#endif /* !LIBSYSTEM_SYSTEM_STRING_H */
