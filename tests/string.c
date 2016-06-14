/* $Id$ */
/* Copyright (c) 2011-2016 Pierre Pronchery <khorben@defora.org> */
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



#include <stdlib.h>
#include <stdio.h>
#include "System/string.h"


/* test */
static int _test(char const * progname, String * s)
{
	size_t len;
	size_t i;

	/* string_get_length */
	printf("%s: Testing %s\n", progname, "string_get_length()");
	if((len = string_get_length(s)) == 0)
		return 2;
	printf("\"%s\" => %zu\n", s, len);
	/* string_clear */
	printf("%s: Testing %s\n", progname, "string_clear()");
	string_clear(s);
	for(i = 0; i <= len; i++)
		if(s[i] != '\0')
			return 4;
	return 0;
}


/* test2 */
static int _test2(char const * progname, String const * string, String * rtrim,
		int rcount)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_rtrim */
	printf("%s: Testing %s\n", progname, "string_rtrim()");
	if((res = string_rtrim(s, rtrim)) != rcount)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", progname,
				string, rtrim, res, rcount);
		ret = 8;
	}
	string_delete(s);
	return ret;
}


/* test3 */
static int _test3(char const * progname, String const * string, String * ltrim,
		int lcount)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_rtrim */
	printf("%s: Testing %s\n", progname, "string_ltrim()");
	if((res = string_ltrim(s, ltrim)) != lcount)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", progname,
				string, ltrim, res, lcount);
		ret = 16;
	}
	string_delete(s);
	return ret;
}


/* test4 */
static int _test4(char const * progname, String const * string, String * trim,
		int count)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_trim */
	printf("%s: Testing %s\n", progname, "string_trim()");
	if((res = string_trim(s, trim)) != count)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", progname,
				string, trim, res, count);
		ret = 32;
	}
	string_delete(s);
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	int ret = 0;
	String * s;
	(void) argc;

	/* test */
	if((s = string_new(argv[0])) == NULL)
		return 2;
	ret |= _test(argv[0], s);
	string_delete(s);
	if((s = string_new_format("%s", argv[0])) == NULL)
		return 2;
	ret |= _test(argv[0], s);
	string_delete(s);
	/* test2 */
	ret |= _test2(argv[0], "", NULL, 0);
	ret |= _test2(argv[0], " ", NULL, 1);
	ret |= _test2(argv[0], "", "", 0);
	ret |= _test2(argv[0], "Y", "Y", 1);
	ret |= _test2(argv[0], "YYYY", NULL, 0);
	ret |= _test2(argv[0], "YYYY", "", 0);
	ret |= _test2(argv[0], "YYYY", "Y", 4);
	ret |= _test2(argv[0], "ZYYYY", "Z", 0);
	ret |= _test2(argv[0], "YYYYZ", "Z", 1);
	ret |= _test2(argv[0], "YYYYZZ", "Z", 2);
	ret |= _test2(argv[0], "YYYYZZ", "YZ", 6);
	/* test3 */
	ret |= _test3(argv[0], "", NULL, 0);
	ret |= _test3(argv[0], " ", NULL, 1);
	ret |= _test3(argv[0], "", "", 0);
	ret |= _test3(argv[0], "Y", "Y", 1);
	ret |= _test3(argv[0], "YYYY", NULL, 0);
	ret |= _test3(argv[0], "YYYY", "", 0);
	ret |= _test3(argv[0], "YYYY", "Y", 4);
	ret |= _test3(argv[0], "YZZZZ", "Z", 0);
	ret |= _test3(argv[0], "YZZZZ", "Y", 1);
	ret |= _test3(argv[0], "YYZZZZ", "Y", 2);
	ret |= _test3(argv[0], "YYYYZZ", "YZ", 6);
	/* test4 */
	ret |= _test4(argv[0], "", NULL, 0);
	ret |= _test4(argv[0], " ", NULL, 1);
	ret |= _test4(argv[0], "", "", 0);
	ret |= _test4(argv[0], "Y", "Y", 1);
	ret |= _test4(argv[0], "YYYY", NULL, 0);
	ret |= _test4(argv[0], "YYYY", "", 0);
	ret |= _test4(argv[0], "YYYY", "Y", 4);
	ret |= _test4(argv[0], "YYYYZ", "Z", 1);
	ret |= _test4(argv[0], "YYYYZZ", "Z", 2);
	ret |= _test4(argv[0], "YYYYZZ", "YZ", 6);
	ret |= _test4(argv[0], "ZZYYYYZZ", "YZ", 8);
	ret |= _test4(argv[0], "ZZYYYYZZ", "Y", 0);
	return ret;
}
