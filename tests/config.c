/* $Id$ */
/* Copyright (c) 2014 Pierre Pronchery <khorben@defora.org> */
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



#include <stdio.h>
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
			|| string_length(value) != 0)
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


/* main */
int main(int argc, char * argv[])
{
	int ret = 0;
	String const variable[] = "variable";
	String const expected[] = "expected";

	ret |= _test(argv[0], "config.conf", variable, expected);
	ret |= _test(argv[0], "config-noeol.conf", variable, expected);
	return (ret == 0) ? 0 : 2;
}
