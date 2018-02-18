/* $Id$ */
/* Copyright (c) 2008-2013 Pierre Pronchery <khorben@defora.org> */
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



#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include "System/error.h"
#include "System/object.h"
#include "System/string.h"
#include "System/token.h"
#include "token.h"


/* Token */
/* private */
/* types */
struct _Token
{
	TokenCode code;
	String * string;
	String * filename;
	unsigned int line;
	unsigned int col;
	void * data;
};


/* protected */
/* functions */
/* token_new */
Token * token_new(String const * filename, unsigned int line, unsigned int col)
{
	Token * token;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, %u)\n", __func__, line, col);
#endif
	if((token = object_new(sizeof(*token))) == NULL)
		return NULL;
	token->code = 0;
	token->string = NULL;
	token->filename = (filename != NULL) ? string_new(filename) : NULL;
	token->line = line;
	token->col = col;
	token->data = NULL;
	if(filename != NULL && token->filename == NULL)
	{
		error_set_code(1, "%s", strerror(errno));
		object_delete(token);
		return NULL;
	}
	return token;
}


/* public */
/* functions */
void token_delete(Token * token)
{
	string_delete(token->filename);
	string_delete(token->string);
	object_delete(token);
}


/* accessors */
/* token_get_code */
TokenCode token_get_code(Token * token)
{
	return token->code;
}


/* token_get_col */
unsigned int token_get_col(Token * token)
{
	return token->col;
}


/* token_get_data */
void * token_get_data(Token * token)
{
	return token->data;
}


/* token_get_filename */
String const * token_get_filename(Token * token)
{
	return token->filename;
}


/* token_get_line */
unsigned int token_get_line(Token * token)
{
	return token->line;
}


/* token_get_string */
String const * token_get_string(Token * token)
{
	return token->string;
}


/* token_set_code */
void token_set_code(Token * token, int code)
{
	token->code = code;
}


/* token_set_col */
void token_set_col(Token * token, unsigned int col)
{
	token->col = col;
}


/* token_set_data */
void token_set_data(Token * token, void * data)
{
	token->data = data;
}


/* token_set_filename */
int token_set_filename(Token * token, String const * filename)
{
	string_delete(token->filename);
	if((token->filename = string_new(filename)) == NULL)
		return 1;
	return 0;
}


/* token_set_line */
void token_set_line(Token * token, unsigned int line)
{
	token->line = line;
}


/* token_set_string */
int token_set_string(Token * token, String const * string)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p, \"%s\")\n", __func__, token, string);
#endif
	string_delete(token->string);
	if((token->string = string_new(string)) == NULL)
		return 1;
	return 0;
}


/* useful */
int token_in_set(Token * token, TokenSet set)
{
	TokenCode * code;

	for(code = set; *code != TC_NULL; code++)
		if(token->code == *code)
			return 1;
	return 0;
}
