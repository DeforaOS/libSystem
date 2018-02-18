/* $Id$ */
/* Copyright (c) 2014-2018 Pierre Pronchery <khorben@defora.org> */
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



#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/string.h"
#include "System/config.h"


/* test */
static int _test(char const * progname, String const * filename,
		String const * variable, String const * expected)
{
	int ret = 0;
	Config * config;
	String const * value;

	if((config = config_new()) == NULL)
		return -error_print(progname);
	/* config_load */
	printf("%s: Testing %s \"%s\"\n", progname, "config_load()", filename);
	fflush(stdout);
	if(config_load(config, filename) != 0)
	{
		config_delete(config);
		return -error_print(progname);
	}
	/* config_get */
	printf("%s: Testing %s\n", progname, "config_get()");
	fflush(stdout);
	if(config_get(config, NULL, "nonexistent") != NULL)
		ret = -error_set_print(progname, 1, "%s",
				"Unknown variable returned");
	if((value = config_get(config, NULL, variable)) == NULL)
		ret = -error_print(progname);
	else if(string_compare(expected, value) != 0)
		ret = -error_set_print(progname, 1, "%s: %s (\"%s\")", expected,
				"Invalid variable returned", value);
	/* config_set */
	printf("%s: Testing %s\n", progname, "config_set()");
	/* config_set: remove the variable */
	if(config_set(config, NULL, variable, NULL) != 0)
		ret = -error_print(progname);
	if((value = config_get(config, NULL, variable)) != NULL)
		ret = -error_set_print(progname, 1, "%s: %s (\"%s\")", "NULL",
				"Invalid variable returned", value);
	/* config_set: restore the variable */
	if(config_set(config, NULL, variable, expected) != 0)
		ret = -error_print(progname);
	if((value = config_get(config, NULL, variable)) == NULL)
		ret = -error_print(progname);
	else if(string_compare(expected, value) != 0)
		ret = -error_set_print(progname, 1, "%s: %s (\"%s\")", expected,
				"Invalid variable returned", value);
	/* config_set: empty the variable */
	if(config_set(config, NULL, variable, "") != 0)
		ret = -error_print(progname);
	if((value = config_get(config, NULL, variable)) == NULL
			|| string_get_length(value) != 0)
		ret = -error_set_print(progname, 1, "%s (\"%s\")",
				"Invalid variable returned", value);
	/* config_set: restore the variable */
	if(config_set(config, NULL, variable, expected) != 0)
		ret = -error_print(progname);
	if((value = config_get(config, NULL, variable)) == NULL)
		ret = -error_print(progname);
	else if(string_compare(expected, value) != 0)
		ret = -error_set_print(progname, 1, "%s: %s (\"%s\")", expected,
				"Invalid variable returned", value);
	config_delete(config);
	return ret;
}


/* test2 */
static int _test2(char const * progname, size_t size, ...)
{
	int ret = 0;
	va_list ap;
	char tmpname[] = P_tmpdir "/config-test2-XXXXXX";
	int fd;
	Config * config;
	String const * section;
	String const * variable;
	String const * value;
	struct stat st;

	/* config_save */
	printf("%s: Testing config_save()\n", progname);
	fflush(stdout);
	if((fd = mkstemp(tmpname)) < 0)
		return -error_set_print(progname, -errno, "%s: %s", "mktemp",
				strerror(errno));
	if((config = config_new()) == NULL)
	{
		unlink(tmpname);
		close(fd);
		return -error_print(progname);
	}
	va_start(ap, size);
	while(ret == 0 && (section = va_arg(ap, String const *)) != NULL)
	{
		variable = va_arg(ap, String const *);
		value = va_arg(ap, String const *);
		ret = config_set(config, section, variable, value);
	}
	va_end(ap);
	if(ret == 0)
		ret = config_save(config, tmpname);
	if(ret != 0)
		error_print(progname);
	config_delete(config);
	if(stat(tmpname, &st) != 0)
		ret = -error_set_print(progname, -errno, "%s: %s", tmpname,
				strerror(errno));
	else if(st.st_size < 0 || (size_t)st.st_size != size)
		ret = -error_set_print(progname, 2, "%s: %lu: Size mismatch"
				" (expected: %lu)", tmpname, st.st_size, size);
	if(unlink(tmpname) != 0)
		ret = -error_set_print(progname, -errno, "%s: %s", tmpname,
				strerror(errno));
	close(fd);
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	int ret = 0;
	String const variable[] = "variable";
	String const expected[] = "expected";
	(void) argc;

	ret |= _test(argv[0], "config.conf", variable, expected);
	ret |= _test(argv[0], "config-noeol.conf", variable, expected);
	ret |= _test2(argv[0], 0, NULL);
	ret |= _test2(argv[0], 0, "", "variable", NULL, NULL);
	ret |= _test2(argv[0], 15, "", "variable", "value", NULL);
	ret |= _test2(argv[0], 25, "section", "variable", "value", NULL);
	ret |= _test2(argv[0], 41, "", "variable", "value",
			"section", "variable", "value", NULL);
	ret |= _test2(argv[0], 53, "section1", "variable", "value",
			"section2", "variable", "value", NULL);
	ret |= _test2(argv[0], 80, "section1", "variable", "value",
			"section2", "variable", "value",
			"section3", "variable", "value", NULL);
	return (ret == 0) ? 0 : 2;
}
