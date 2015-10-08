/* $Id$ */
/* Copyright (c) 2014-2015 Pierre Pronchery <khorben@defora.org> */
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
# define PROGNAME "configctl"
#endif


/* configctl */
/* private */
/* prototypes */
static int _configctl(int verbose, int write, char const * filename, int argc,
		char * argv[]);
static void _configctl_print(int verbose, char const * section,
		char const * variable, char const * value);

static int _error(char const * progname, int ret);
static int _usage(void);


/* functions */
/* configctl */
static int _configctl_do(int verbose, char const * filename,
		char const * section, char const * key, char const * value);

static int _configctl(int verbose, int write, char const * filename, int argc,
		char * argv[])
{
	int ret = 0;
	int i;
	char * section;
	char * key;
	char * value = NULL;

	for(i = 0; i < argc; i++)
	{
		section = argv[i];
		if((key = strchr(section, '.')) == NULL)
		{
			key = section;
			section = NULL;
		}
		else
			*(key++) = '\0';
		if(write && (value = strchr(key, '=')) != NULL)
			*(value++) = '\0';
		ret |= _configctl_do(verbose, filename, section, key, value);
	}
	return (ret == 0) ? 0 : 2;
}

static int _configctl_do(int verbose, char const * filename,
		char const * section, char const * key, char const * value)
{
	int ret = 0;
	Config * config;
	char const * p;

	if((config = config_new()) == NULL)
		ret = _error(PROGNAME, 1);
	if(config_load(config, filename) != 0)
		ret = _error(PROGNAME, 1);
	else if(value == NULL)
	{
		p = config_get(config, section, key);
		if(verbose < 0)
			ret = (p != NULL) ? 0 : 1;
		else if(p != NULL)
			_configctl_print(verbose, section, key, value);
	}
	else if(value != NULL)
	{
		_configctl_print(verbose, section, key, value);
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


/* configctl_print */
static void _configctl_print(int verbose, char const * section,
		char const * variable, char const * value)
{
	printf("%s%s%s%s%s\n", (verbose > 0 && section != NULL) ? section : "",
			(verbose > 0 && section != NULL) ? "." : "",
			(verbose > 0) ? variable : "",
			(verbose > 0) ? "=" : "",
			(value != NULL) ? value : "");
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
	fputs("Usage: " PROGNAME " -f filename [-qv] [section.]key...\n"
"       " PROGNAME " -w -f filename [-v] [section.]key[=value]...\n", stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	int verbose = 0;
	int write = 0;
	char const * filename = NULL;

	while((o = getopt(argc, argv, "f:qvw")) != -1)
		switch(o)
		{
			case 'f':
				filename = optarg;
				break;
			case 'q':
				verbose = -1;
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
	if(filename == NULL || optind == argc)
		return _usage();
	return (_configctl(verbose, write, filename, argc - optind,
				&argv[optind]) == 0) ? 0 : 2;
}
