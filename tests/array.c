/* $Id$ */
/* Copyright (c) 2015-2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stddef.h>
#include "System/array.h"
#include "System/error.h"

#ifndef PROGNAME
# define PROGNAME	"array"
#endif

ARRAY(int)
ARRAY2(unsigned int, UnsignedInt)


/* test */
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
	return 0;
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
