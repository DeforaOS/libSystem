/* $Id$ */
/* Copyright (c) 2008-2014 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_TOKEN_H
# define LIBSYSTEM_SYSTEM_TOKEN_H

# include "string.h"


/* Token */
/* types */
typedef struct _Token Token;
typedef unsigned int TokenCode;
# define TC_NULL 0
typedef TokenCode * TokenSet;


/* functions */
void token_delete(Token * token);


/* accessors */
String const * token_get_filename(Token * token);
int token_set_filename(Token * token, String const * filename);

unsigned int token_get_col(Token * token);
void token_set_col(Token * token, unsigned int col);

unsigned int token_get_line(Token * token);
void token_set_line(Token * token, unsigned int line);

TokenCode token_get_code(Token * token);
void token_set_code(Token * token, int code);

String const * token_get_string(Token * token);
int token_set_string(Token * token, String const * string);

void * token_get_data(Token * token);
void token_set_data(Token * token, void * data);


/* useful */
int token_in_set(Token * token, TokenSet set);

#endif /* !LIBSYSTEM_SYSTEM_TOKEN_H */
