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
/* TODO:
 * - switch parser_scan() and parser_scan_filter() to avoid confusion */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/parser.h"
#include "token.h"


/* Parser */
/* private */
/* types */
typedef struct _ParserFilterData ParserFilterData;
typedef struct _ParserCallbackData ParserCallbackData;

struct _Parser
{
	/* parsing sources */
	String * filename;
	FILE * fp;
	String * string;
	size_t string_cnt;
	size_t string_pos;

	/* tracking the position */
	int error;
	unsigned int line;
	unsigned int col;
	int last;
	unsigned int lookahead;

	ParserFilter scanner;

	ParserFilterData * filters;
	size_t filters_cnt;
	ParserCallbackData * callbacks;
	size_t callbacks_cnt;
};

struct _ParserFilterData
{
	ParserFilter filter;
	void * data;
};

struct _ParserCallbackData
{
	ParserCallback callback;
	void * data;
};


/* prototypes */
static int _parser_scanner_file(int * c, void * data);
static int _parser_scanner_string(int * c, void * data);


/* functions */
/* parser_scan_filter */
int parser_scan_filter(Parser * parser)
{
	int c = parser->last;
	size_t i;
	ParserFilterData * pfd;
	int l;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p)\n", __func__, (void *)parser);
#endif
	if(parser->lookahead)
		parser->lookahead--;
	else if(parser->scanner(&c, parser) != 0)
	{
		parser->error = 1;
		return EOF;
	}
	for(i = 0; i < parser->filters_cnt; i++)
	{
		pfd = &parser->filters[i];
		if((l = pfd->filter(&c, pfd->data)) < 0)
		{
			parser->error = 1;
			return EOF;
		}
		parser->lookahead += l;
	}
	parser->last = c;
	return c;
}


/* parser_scanner_file */
static int _parser_scanner_file(int * c, void * data)
{
	Parser * parser = (Parser *)data;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(parser->last == '\n')
	{
		parser->line++;
		parser->col = 1;
	}
	else if(parser->last != EOF)
		parser->col++;
	if((*c = fgetc(parser->fp)) == EOF
			&& !feof(parser->fp))
		return -1;
	parser->last = *c;
	return 0;
}


/* parser_scanner_string */
static int _parser_scanner_string(int * c, void * data)
{
	Parser * parser = (Parser *)data;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if(parser->last == '\n')
	{
		parser->line++;
		parser->col = 1;
	}
	else if(parser->last != EOF)
		parser->col++;
	if(parser->string_pos == parser->string_cnt)
		*c = EOF;
	else
		*c = parser->string[parser->string_pos];
	parser->string_pos++;
	parser->last = *c;
	return 0;
}


/* public */
/* functions */
/* parser_new */
static Parser * _new_do(ParserFilter scanner);

Parser * parser_new(String const * pathname)
{
	Parser * parser;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, pathname);
#endif
	if((parser = _new_do(_parser_scanner_file)) == NULL)
		return NULL;
	if((parser->filename = string_new(pathname)) == NULL)
		error_set_code(-errno, "%s", strerror(errno));
	if((parser->fp = fopen(pathname, "r")) == NULL)
		error_set_code(-errno, "%s: %s", pathname, strerror(errno));
	if(parser->filename == NULL || parser->fp == NULL)
	{
		parser_delete(parser);
		return NULL;
	}
	return parser;
}

static Parser * _new_do(ParserFilter scanner)
{
	Parser * parser;

	if((parser = (Parser *)object_new(sizeof(*parser))) == NULL)
		return NULL;
	parser->filename = NULL;
	parser->fp = NULL;
	parser->string = NULL;
	parser->string_cnt = 0;
	parser->string_pos = 0;
	parser->error = 0;
	parser->line = 1;
	parser->col = 1;
	parser->last = EOF;
	parser->lookahead = 0;
	parser->scanner = scanner;
	parser->filters = NULL;
	parser->filters_cnt = 0;
	parser->callbacks = NULL;
	parser->callbacks_cnt = 0;
	return parser;
}


/* parser_new_string */
Parser * parser_new_string(String const * string, size_t length)
{
	Parser * parser;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", %lu)\n", __func__, string, length);
#endif
	if((parser = _new_do(_parser_scanner_string)) == NULL)
		return NULL;
	parser->string = (String *)malloc(length);
	parser->string_cnt = length;
	if(parser->string == NULL && length != 0)
	{
		error_set_code(-errno, "%s", strerror(errno));
		parser_delete(parser);
		return NULL;
	}
	memcpy(parser->string, string, length);
	return parser;
}


/* parser_delete */
int parser_delete(Parser * parser)
{
	int ret = 0;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p) \"%s\"\n", __func__, (void *)parser,
			parser->filename);
#endif
	if(parser->fp != NULL
			&& fclose(parser->fp) != 0)
		ret = error_set_code(-errno, "%s: %s", parser->filename,
				strerror(errno));
	string_delete(parser->filename);
	free(parser->string);
	free(parser->filters);
	free(parser->callbacks);
	object_delete(parser);
	return ret;
}


/* accessors */
/* parser_get_filename */
String const * parser_get_filename(Parser * parser)
{
	return parser->filename;
}


/* parser_get_token */
int parser_get_token(Parser * parser, Token ** token)
{
	int ret = 0; /* XXX not sure */
	size_t i;
	ParserCallbackData * pcd;

	if((*token = token_new(parser->filename, parser->line, parser->col))
			== NULL)
		return -1;
	if(parser->last == EOF
			&& parser_scan_filter(parser) == EOF
			&& parser->error != 0)
	{
		token_delete(*token);
		*token = NULL;
		return -1;
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %c\n", __func__, parser->last);
#endif
	for(i = 0; i < parser->callbacks_cnt; i++)
	{
		pcd = &parser->callbacks[i];
		if((ret = pcd->callback(parser, *token, parser->last,
						pcd->data)) <= 0)
			break;
	}
	if(ret == 0 && i != parser->callbacks_cnt)
		return 0; /* there is a token and no error */
	token_delete(*token);
	*token = NULL;
	return (ret >= 0 && parser->last == EOF) ? 0 : -1;
}


/* useful */
/* parser_add_callback */
int parser_add_callback(Parser * parser, ParserCallback callback, void * data)
{
	ParserCallbackData * p;

	if((p = (ParserCallbackData *)realloc(parser->callbacks,
					sizeof(*p) * (parser->callbacks_cnt
						+ 1))) == NULL)
		return 1;
	parser->callbacks = p;
	p = &parser->callbacks[parser->callbacks_cnt++];
	p->callback = callback;
	p->data = data;
	return 0;
}


/* parser_add_filter */
int parser_add_filter(Parser * parser, ParserFilter filter, void * data)
{
	ParserFilterData * p = parser->filters;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p, %p, %p)\n", __func__, (void *)parser,
			(void *)filter, (void *)data);
#endif
	if((p = (ParserFilterData *)realloc(p, sizeof(*p) * (parser->filters_cnt
						+ 1))) == NULL)
		return 1;
	parser->filters = p;
	p = &parser->filters[parser->filters_cnt++];
	p->filter = filter;
	p->data = data;
	return 0;
}


/* parser_remove_callback */
int parser_remove_callback(Parser * parser, ParserCallback callback)
	/* XXX untested */
{
	size_t i;
	ParserCallbackData * p;

	for(i = 0; i < parser->callbacks_cnt; i++)
		if(parser->callbacks[i].callback == callback)
			break;
	if(i != parser->callbacks_cnt)
		return 1;
	p = &parser->callbacks[i];
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(): memmove(%p, %p, %lu)\n", __func__,
			(void *)p, (void *)(p + 1),
			sizeof(*p) * (parser->callbacks_cnt - i));
#endif
	memmove(p, p + 1, sizeof(*p) * (parser->callbacks_cnt - i));
	parser->callbacks_cnt--;
	return 0;
}


/* parser_remove_filter */
int parser_remove_filter(Parser * parser, ParserFilter filter)
	/* XXX untested */
{
	size_t i;
	ParserFilterData * p;

	for(i = 0; i < parser->filters_cnt; i++)
		if(parser->filters[i].filter == filter)
			break;
	if(i != parser->filters_cnt)
		return 1;
	p = &parser->filters[i];
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(): memmove(%p, %p, %lu)\n", __func__,
			(void *)p, (void *)(p + 1),
			sizeof(*p) * (parser->callbacks_cnt - i));
#endif
	memmove(p, p + 1, sizeof(*p) * (parser->filters_cnt - i));
	parser->filters_cnt--;
	return 0;
}


/* parser_scan */
int parser_scan(Parser * parser)
{
	int c;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%p)\n", __func__, (void *)parser);
#endif
	if(parser->scanner(&c, parser) != 0)
		return EOF;
	return c;
}
