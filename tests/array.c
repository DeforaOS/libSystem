/* $Id$ */
/* Copyright (c) 2015-2018 Pierre Pronchery <khorben@defora.org> */
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



#include <stddef.h>
#include "System/array.h"
#include "System/error.h"

#ifndef PROGNAME
# define PROGNAME	"array"
#endif

ARRAY(int)
ARRAY2(unsigned int, UnsignedInt)


/* test */
static bool _test_filter(void * value, void * data);
static bool _test_filter_swap(void * data, void * value);
static void _test_foreach(void * value, void * data);
static void _test_foreach_swap(void * data, void * value);

static int _test(intArray * array)
{
	int i;
	int j;
	int * p;

	if(array_get(array, 0) != NULL)
		return 2;
	i = 0;
	if(array_set(array, 0, &i) != 0)
		return 3;
	i = 0xffffffff;
	if(array_get_copy(array, 0, &i) != 0 || i != 0)
		return 4;
	for(i = 0; i < 1024; i++)
		if(array_set(array, i, &i) != 0)
			return 5;
	for(i = 0; i < 1024; i++)
		if(array_get_copy(array, i, &j) != 0)
			return 6;
		else if(j != i)
			return 7;
	if(array_count(array) != 1024)
		return 8;
	if((p = array_get(array, 512)) == NULL || *p != 512)
		return 9;
	j = 0;
	array_foreach(array, _test_foreach, &j);
	if(j != 523776)
		return 10;
	j = 0;
	array_foreach_swap(array, _test_foreach_swap, &j);
	if(j != 523776)
		return 11;
	if(array_remove_pos(array, 512) != 0)
		return 12;
	if((p = array_get(array, 512)) == NULL || *p != 513)
		return 13;
	array_filter(array, _test_filter, NULL);
	if(array_count(array) != 1022)
		return 14;
	array_filter_swap(array, _test_filter_swap, NULL);
	if(array_count(array) != 0)
		return 15;
	return 0;
}

static bool _test_filter(void * value, void * data)
{
	int * i = value;
	(void) data;

	return (*i != 0) ? true : false;
}

static bool _test_filter_swap(void * data, void * value)
{
	int * i = value;
	(void) data;

	return (*i != 0) ? false : true;
}

static void _test_foreach(void * value, void * data)
{
	int * i = value;
	int * j = data;

	*j += *i;
}

static void _test_foreach_swap(void * data, void * value)
{
	_test_foreach(value, data);
}


/* main */
int main(void)
{
	int ret;
	intArray * array;

	if((array = array_new(sizeof(int))) == NULL)
		return 2;
	if((ret = _test(array)) != 0)
		error_print(PROGNAME);
	array_delete(array);
	return ret;
}
