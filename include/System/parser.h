/* $Id$ */
/* Copyright (c) 2008-2020 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_PARSER_H
# define LIBSYSTEM_SYSTEM_PARSER_H

# include "string.h"
# include "token.h"

# ifdef __cplusplus
extern "C" {
# endif


/* Parser */
/* types */
typedef struct _Parser Parser;
typedef int (*ParserFilter)(int * c, void * data);
typedef int (*ParserCallback)(Parser * parser, Token * token, int c,
		void * data);


/* functions */
Parser * parser_new(String const * pathname);
Parser * parser_new_string(String const * string, size_t length);
int parser_delete(Parser * parser);

/* accessors */
String const * parser_get_filename(Parser * parser);
int parser_get_token(Parser * parser, Token ** token);

/* useful */
int parser_add_callback(Parser * parser, ParserCallback callback,
		void * data);
int parser_remove_callback(Parser * parser, ParserCallback callback);

int parser_add_filter(Parser * parser, ParserFilter filter, void * data);
int parser_remove_filter(Parser * parser, ParserFilter filter);

int parser_scan(Parser * parser);
int parser_scan_filter(Parser * parser);

# ifdef __cplusplus
}
# endif

#endif /* !LIBSYSTEM_SYSTEM_PARSER_H */
