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
static int _configctl_list(char const * filename);

static void _configctl_print(int verbose, char const * section,
		char const * variable, char const * value);

static int _configctl_error(char const * progname, int ret);
static int _usage(void);


/* functions */
/* configctl */
static int _configctl_do(Config * config, int verbose, char const * section,
		char const * key, char const * value);

static int _configctl(int verbose, int write, char const * filename, int argc,
		char * argv[])
{
	int ret = 0;
	Config * config;
	int i;
	char * section;
	char * key;
	char * value = NULL;

	if((config = config_new()) == NULL)
		return -_configctl_error(PROGNAME, 1);
	if(config_load(config, filename) != 0)
	{
		config_delete(config);
		return -_configctl_error(PROGNAME, 1);
	}
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
		ret |= _configctl_do(config, verbose, section, key, value);
	}
	if(ret == 0 && write && config_save(config, filename) != 0)
		ret = -_configctl_error(PROGNAME, 1);
	config_delete(config);
	return ret;
}

static int _configctl_do(Config * config, int verbose, char const * section,
		char const * key, char const * value)
{
	char const * p;

	if(value == NULL)
	{
		p = config_get(config, section, key);
		if(verbose < 0)
			return (p != NULL) ? 0 : -1;
		else if(p != NULL)
			_configctl_print(verbose, section, key, p);
	}
	else if(value != NULL)
	{
		if(config_set(config, section, key, value) != 0)
			return -_configctl_error(PROGNAME, 1);
		_configctl_print(verbose, section, key, value);
	}
	else
		return -_configctl_error(PROGNAME, 1);
	return 0;
}


/* configctl_list */
static void _list_foreach(String const * section, void * data);
static void _list_foreach_section(String const * variable, String const * value,
		void * data);

static int _configctl_list(char const * filename)
{
	Config * config;

	if((config = config_new()) == NULL)
		return -_configctl_error(PROGNAME, 1);
	if(config_load(config, filename) != 0)
	{
		config_delete(config);
		return -_configctl_error(PROGNAME, 1);
	}
	config_foreach(config, _list_foreach, config);
	config_delete(config);
	return 0;
}

static void _list_foreach(String const * section, void * data)
{
	Config * config = data;

	config_foreach_section(config, section, _list_foreach_section,
			(void *)section);
}

static void _list_foreach_section(String const * variable, String const * value,
		void * data)
{
	String const * section = data;

	_configctl_print(1, section, variable, value);
}


/* configctl_print */
static void _configctl_print(int verbose, char const * section,
		char const * variable, char const * value)
{
	printf("%s%s%s%s%s\n", (verbose > 0 && section != NULL) ? section : "",
			(verbose > 0 && section != NULL && section[0] != '\0')
			? "." : "",
			(verbose > 0) ? variable : "",
			(verbose > 0) ? "=" : "",
			(value != NULL) ? value : "");
}


/* configctl_error */
static int _configctl_error(char const * progname, int ret)
{
	error_print(progname);
	return ret;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: " PROGNAME " -f filename -a\n"
"       " PROGNAME " -f filename [-qv] [section.]key...\n"
"       " PROGNAME " -w -f filename [-v] [section.]key[=value]...\n", stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	int list = 0;
	int verbose = 0;
	int write = 0;
	char const * filename = NULL;

	while((o = getopt(argc, argv, "af:qvw")) != -1)
		switch(o)
		{
			case 'a':
				list = 1;
				break;
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
	if(list)
	{
		if(verbose != 0 || write != 0 || filename == NULL
				|| optind != argc)
			return _usage();
		return _configctl_list(filename) ? 0 : 2;
	}
	if(filename == NULL || optind == argc)
		return _usage();
	return (_configctl(verbose, write, filename, argc - optind,
				&argv[optind]) == 0) ? 0 : 2;
}
