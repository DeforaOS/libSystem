/* $Id$ */
/* Copyright (c) 2011-2018 Pierre Pronchery <khorben@defora.org> */
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
#include <stdio.h>
#include "System/string.h"

#ifndef PROGNAME
# define PROGNAME	"string"
#endif


/* prototypes */
static int _test(String * s);
static int _test2(String const * string, String const * rtrim, int rcount);
static int _test3(String const * string, String const * ltrim, int lcount);
static int _test4(String const * string, String const * trim, int count);
static int _test5(String const * string, String const * key, ssize_t expected);
static int _test6(String const * string, String const * key, ssize_t expected);
static int _test7(String const * string, size_t length,
		String const * expected);


/* functions */
/* test */
static int _test(String * s)
{
	size_t len;
	size_t i;

	/* string_get_length */
	printf("%s: Testing %s\n", PROGNAME, "string_get_length()");
	if((len = string_get_length(s)) == 0)
		return 2;
	printf("\"%s\" => %zu\n", s, len);
	/* string_clear */
	printf("%s: Testing %s\n", PROGNAME, "string_clear()");
	string_clear(s);
	for(i = 0; i <= len; i++)
		if(s[i] != '\0')
			return 4;
	return 0;
}


/* test2 */
static int _test2(String const * string, String const * rtrim, int rcount)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_rtrim */
	printf("%s: Testing %s\n", PROGNAME, "string_rtrim()");
	if((res = string_rtrim(s, rtrim)) != rcount)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", PROGNAME,
				string, rtrim, res, rcount);
		ret = 8;
	}
	string_delete(s);
	return ret;
}


/* test3 */
static int _test3(String const * string, String const * ltrim, int lcount)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_rtrim */
	printf("%s: Testing %s\n", PROGNAME, "string_ltrim()");
	if((res = string_ltrim(s, ltrim)) != lcount)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", PROGNAME,
				string, ltrim, res, lcount);
		ret = 16;
	}
	string_delete(s);
	return ret;
}


/* test4 */
static int _test4(String const * string, String const * trim, int count)
{
	int ret = 0;
	String * s;
	int res;

	if((s = string_new(string)) == NULL)
		return -1;
	/* string_trim */
	printf("%s: Testing %s\n", PROGNAME, "string_trim()");
	if((res = string_trim(s, trim)) != count)
	{
		printf("%s: %s, %s, %d: Test failed (expected: %d)\n", PROGNAME,
				string, trim, res, count);
		ret = 32;
	}
	string_delete(s);
	return ret;
}


/* test5 */
static int _test5(String const * string, String const * key, ssize_t expected)
{
	ssize_t res;

	/* string_index */
	printf("%s: Testing %s\n", PROGNAME, "string_index()");
	if((res = string_index(string, key)) != expected)
	{
		printf("%s: %s, %s, %zd: Test failed (expected: %zd)\n",
				PROGNAME, string, key, res, expected);
		return 2;
	}
	return 0;
}


/* test6 */
static int _test6(String const * string, String const * key, ssize_t expected)
{
	ssize_t res;

	/* string_rindex */
	printf("%s: Testing %s\n", PROGNAME, "string_rindex()");
	if((res = string_rindex(string, key)) != expected)
	{
		printf("%s: %s, %s, %zd: Test failed (expected: %zd)\n",
				PROGNAME, string, key, res, expected);
		return 2;
	}
	return 0;
}


/* test7 */
static int _test7(String const * string, size_t length,
		String const * expected)
{
	int ret = 0;
	String * s;

	printf("%s: Testing %s\n", PROGNAME, "string_new_length()");
	if((s = string_new_length(string, length)) == NULL)
		return 2;
	if(string_compare(s, expected) != 0)
	{
		printf("%s: %s, %zu, \"%s\": Test failed (expected: \"%s\")\n",
				PROGNAME, string, length, s, expected);
		ret = 2;
	}
	string_delete(s);
	return ret;
}


/* test8 */
static int _test8(String const * string, String const * expected)
{
	int ret = 0;
	String * s;

	printf("%s: Testing %s\n", PROGNAME, "string_tolower()");
	if((s = string_new(string)) == NULL)
		return 2;
	string_tolower(s);
	if(string_compare(s, expected) != 0)
	{
		printf("%s: %s, \"%s\": Test failed (expected: \"%s\")\n",
				PROGNAME, string, s, expected);
		ret = 2;
	}
	return ret;
}


/* test9 */
static int _test9(String const * string, String const * expected)
{
	int ret = 0;
	String * s;

	printf("%s: Testing %s\n", PROGNAME, "string_toupper()");
	if((s = string_new(string)) == NULL)
		return 2;
	string_toupper(s);
	if(string_compare(s, expected) != 0)
	{
		printf("%s: %s, \"%s\": Test failed (expected: \"%s\")\n",
				PROGNAME, string, s, expected);
		ret = 2;
	}
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
	ret |= _test(s);
	string_delete(s);
	if((s = string_new_format("%s", argv[0])) == NULL)
		return 2;
	ret |= _test(s);
	string_delete(s);
	/* test2 */
	ret |= _test2("", NULL, 0);
	ret |= _test2(" ", NULL, 1);
	ret |= _test2("", "", 0);
	ret |= _test2("Y", "Y", 1);
	ret |= _test2("YYYY", NULL, 0);
	ret |= _test2("YYYY", "", 0);
	ret |= _test2("YYYY", "Y", 4);
	ret |= _test2("ZYYYY", "Z", 0);
	ret |= _test2("YYYYZ", "Z", 1);
	ret |= _test2("YYYYZZ", "Z", 2);
	ret |= _test2("YYYYZZ", "YZ", 6);
	/* test3 */
	ret |= _test3("", NULL, 0);
	ret |= _test3(" ", NULL, 1);
	ret |= _test3("", "", 0);
	ret |= _test3("Y", "Y", 1);
	ret |= _test3("YYYY", NULL, 0);
	ret |= _test3("YYYY", "", 0);
	ret |= _test3("YYYY", "Y", 4);
	ret |= _test3("YZZZZ", "Z", 0);
	ret |= _test3("YZZZZ", "Y", 1);
	ret |= _test3("YYZZZZ", "Y", 2);
	ret |= _test3("YYYYZZ", "YZ", 6);
	/* test4 */
	ret |= _test4("", NULL, 0);
	ret |= _test4(" ", NULL, 1);
	ret |= _test4("", "", 0);
	ret |= _test4("Y", "Y", 1);
	ret |= _test4("YYYY", NULL, 0);
	ret |= _test4("YYYY", "", 0);
	ret |= _test4("YYYY", "Y", 4);
	ret |= _test4("YYYYZ", "Z", 1);
	ret |= _test4("YYYYZZ", "Z", 2);
	ret |= _test4("YYYYZZ", "YZ", 6);
	ret |= _test4("ZZYYYYZZ", "YZ", 8);
	ret |= _test4("ZZYYYYZZ", "Y", 0);
	/* test5 */
	ret |= _test5("test", "", 4);
	ret |= _test5("test", "test", 0);
	ret |= _test5("test", "test2", -1);
	ret |= _test5("test2", "test", 0);
	ret |= _test5("2test", "test", 1);
	ret |= _test5("2test2", "test", 1);
	ret |= _test5("2test2test2", "test", 1);
	/* test6 */
	ret |= _test6("test", "", 4);
	ret |= _test6("test", "test", 0);
	ret |= _test6("test", "test2", -1);
	ret |= _test6("test2", "test", 0);
	ret |= _test6("2test", "test", 1);
	ret |= _test6("2test2", "test", 1);
	ret |= _test6("2test2test2", "test", 6);
	/* test7 */
	ret |= _test7(NULL, 0, "");
	ret |= _test7(NULL, 7, "");
	ret |= _test7("test", 0, "");
	ret |= _test7("test", 1, "t");
	ret |= _test7("test", 4, "test");
	ret |= _test7("test", 7, "test");
	/* test8 */
	ret |= _test8("abcABC", "abcabc");
	ret |= _test8("abcABC123", "abcabc123");
	/* test9 */
	ret |= _test9("abcABC", "ABCABC");
	ret |= _test9("abcABC123", "ABCABC123");
	return ret;
}
