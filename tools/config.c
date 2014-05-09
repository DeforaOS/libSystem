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



#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "System/config.h"
#include "System/error.h"

#ifndef PROGNAME
# define PROGNAME "config"
#endif


/* config */
/* private */
/* prototypes */
static int _config(int verbose, char const * filename, char const * section,
		char const * key, char const * value);

static int _error(char const * progname, int ret);
static int _usage(void);


/* functions */
/* config */
static int _config(int verbose, char const * filename, char const * section,
		char const * key, char const * value)
{
	int ret = 0;
	Config * config;
	char const * p;

	if((config = config_new()) == NULL)
		ret = _error(PROGNAME, 1);
	if(config_load(config, filename) != 0)
		ret = _error(PROGNAME, 1);
	else if(value == NULL && (p = config_get(config, section, key)) != NULL)
		printf("%s%s%s%s%s\n",
				(verbose && section != NULL) ? section : "",
				(verbose && section != NULL) ? "." : "",
				(verbose) ? key : "", (verbose) ? "=" : "", p);
	else if(value != NULL)
	{
		printf("%s%s%s%s%s\n",
				(verbose && section != NULL) ? section : "",
				(verbose && section != NULL) ? "." : "",
				(verbose) ? key : "", (verbose) ? "=" : "",
				value);
		if(config_set(config, section, key, value) != 0)
			ret = _error(PROGNAME, 1);
		if(config_save(config, filename) != 0)
			ret = _error(PROGNAME, 1);
	}
	else
		ret = _error(PROGNAME, 1);
	config_delete(config);
	return ret;
}


/* error */
static int _error(char const * progname, int ret)
{
	error_print(progname);
	return ret;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: config [-v] -f filename [section.]key\n"
"       config [-v] -w -f filename [section.]key[=value]\n", stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	char const * filename = NULL;
	int verbose = 0;
	int write = 0;
	char * section;
	char * key;
	char * value = NULL;

	while((o = getopt(argc, argv, "f:vw")) != -1)
		switch(o)
		{
			case 'f':
				filename = optarg;
				break;
			case 'v':
				verbose = 1;
				break;
			case 'w':
				write = 1;
				break;
			default:
				return _usage();
		}
	if(filename == NULL || optind + 1 != argc)
		return _usage();
	section = argv[optind];
	if((key = strchr(section, '.')) == NULL)
	{
		key = section;
		section = NULL;
	}
	else
		*(key++) = '\0';
	if(write && (value = strchr(key, '=')) != NULL)
		*(value++) = '\0';
	return (_config(verbose, filename, section, key, value) == 0) ? 0 : 2;
}
