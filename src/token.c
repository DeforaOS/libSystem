/* $Id$ */
/* Copyright (c) 2008 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* libSystem is not free software; you can redistribute it and/or modify it
 * under the terms of the Creative Commons Attribution-NonCommercial-ShareAlike
 * 3.0 Unported as published by the Creative Commons organization.
 *
 * libSystem is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the Creative Commons Attribution-NonCommercial-
 * ShareAlike 3.0 Unported license for more details.
 *
 * You should have received a copy of the Creative Commons Attribution-
 * NonCommercial-ShareAlike 3.0 along with libSystem; if not, browse to
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ */



#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include "System.h"
#include "token.h"


/* Token */
/* private */
/* types */
struct _Token
{
	TokenCode code;
	char * string;
	unsigned int line;
	unsigned int col;
};


/* protected */
/* functions */
/* token_new */
Token * token_new(unsigned int line, unsigned int col)
{
	Token * token;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: token_new(%u, %u)\n", line, col);
#endif
	if((token = object_new(sizeof(*token))) == NULL)
		return NULL;
	token->code = 0;
	token->line = line;
	token->col = col;
	token->string = NULL;
	return token;
}


/* public */
/* functions */
void token_delete(Token * token)
{
	free(token->string);
	object_delete(token);
}


/* accessors */
/* token_get_code */
int token_get_code(Token * token)
{
	return token->code;
}


/* token_get_col */
unsigned int token_get_col(Token * token)
{
	return token->col;
}


/* token_get_line */
unsigned int token_get_line(Token * token)
{
	return token->line;
}


/* token_get_string */
char const * token_get_string(Token * token)
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


/* token_set_line */
void token_set_line(Token * token, unsigned int line)
{
	token->line = line;
}


/* token_set_string */
int token_set_string(Token * token, char const * string)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: token_set_string(%p, \"%s\")\n", token, string);
#endif
	free(token->string);
	if((token->string = strdup(string)) == NULL)
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
