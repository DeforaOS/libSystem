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
		if((p = config_get(config, section, key)) == NULL)
			return -_configctl_error(PROGNAME, 1);
		_configctl_print(verbose, section, key, p);
	}
	else
	{
		if(config_set(config, section, key, value) != 0)
			return -_configctl_error(PROGNAME, 1);
		_configctl_print(verbose, section, key, value);
	}
	return 0;
}


/* configctl_list */
static void _list_foreach(Config const * config, String const * section,
		void * data);
static void _list_foreach_section(Config const * config, String const * section,
		String const * variable, String const * value, void * data);

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
	config_foreach(config, _list_foreach, NULL);
	config_delete(config);
	return 0;
}

static void _list_foreach(Config const * config, String const * section,
		void * data)
{
	(void) data;

	config_foreach_section(config, section, _list_foreach_section, NULL);
}

static void _list_foreach_section(Config const * config, String const * section,
		String const * variable, String const * value, void * data)
{
	(void) config;
	(void) data;

	_configctl_print(1, section, variable, value);
}


/* configctl_print */
static void _configctl_print(int verbose, char const * section,
		char const * variable, char const * value)
{
	if(verbose < 0)
		return;
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
"       " PROGNAME " -w -f filename [-qv] [section.]key[=value]...\n"
"  -a\tList every key of every section available\n"
"  -f\tFilename to parse or update\n"
"  -q\tQuiet mode\n"
"  -v\tVerbose mode\n"
"  -w\tSet and save values\n", stderr);
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
