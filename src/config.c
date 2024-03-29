/* $Id$ */
/* Copyright (c) 2005-2021 Pierre Pronchery <khorben@defora.org> */
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "System/error.h"
#include "System/mutator.h"
#include "System/config.h"
#include "../config.h"

/* constants */
#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef SYSCONFDIR
# define SYSCONFDIR	PREFIX "/etc"
#endif

#define CONFIG_COMMENT '#'


/* Config */
/* private */
/* types */
typedef struct _ConfigForeachData
{
	ConfigForeachCallback callback;
	void * priv;
} ConfigForeachData;

typedef struct _ConfigForeachSectionData
{
	ConfigForeachSectionCallback callback;
	String const * section;
	void * priv;
} ConfigForeachSectionData;

typedef struct _ConfigSave
{
	FILE * fp;
	String const * sep;
} ConfigSave;


/* public */
/* functions */
/* config_new */
Config * config_new(void)
{
	return mutator_new();
}


/* config_new_copy */
typedef struct _ConfigError
{
	Config * config;
	ErrorCode code;
} ConfigError;
static void _new_copy_foreach(Config const * from, String const * section,
		void * priv);
static void _new_copy_foreach_section(Config const * from,
		String const * section, String const * variable,
		String const * value, void * priv);

Config * config_new_copy(Config const * from)
{
	ConfigError ce;

	if((ce.config = mutator_new()) == NULL)
		return NULL;
	ce.code = 0;
	config_foreach(from, _new_copy_foreach, &ce);
	if(ce.code != 0)
	{
		config_delete(ce.config);
		return NULL;
	}
	return ce.config;
}

static void _new_copy_foreach(Config const * from, String const * section,
		void * priv)
{
	ConfigError * ce = (ConfigError *)priv;

	if(ce->code == 0)
		config_foreach_section(from, section, _new_copy_foreach_section,
				ce);
}

static void _new_copy_foreach_section(Config const * from,
		String const * section, String const * variable,
		String const * value, void * priv)
{
	ConfigError * ce = (ConfigError *)priv;
	(void) from;

	if(config_set(ce->config, section, variable, value) != 0)
		ce->code = error_get_code();
}


/* config_new_load */
Config * config_new_load(String const * filename)
{
	Config * config;

	if(filename == NULL)
		return config_new();
	if((config = config_new()) == NULL)
		return NULL;
	if(config_load(config, filename) != 0)
	{
		config_delete(config);
		return NULL;
	}
	return config;
}


/* config_delete */
void config_delete(Config * config)
{
	config_reset(config);
	mutator_delete(config);
}


/* accessors */
/* config_get */
String const * config_get(Config const * config, String const * section,
		String const * variable)
{
	Mutator * mutator;
	String const * value;

	if(section == NULL)
		section = "";
	if((mutator = (Mutator *)mutator_get(config, section)) == NULL)
	{
		/* the section does not exist */
		if(section[0] == '\0')
			error_set_code(1, "%s", "No default section");
		else
			error_set_code(1, "%s%s", section, ": No such section");
		return NULL;
	}
	if((value = (String const *)mutator_get(mutator, variable)) == NULL)
	{
		/* the variable is not defined */
		error_set_code(1, "%s%s%s%s%s", variable, ": Not defined in",
				(section[0] == '\0') ? " default" : "",
				" section ",
				(section[0] != '\0') ? section : "");
		return NULL;
	}
	return value;
}


/* config_set */
int config_set(Config * config, String const * section, String const * variable,
		String const * value)
{
	Mutator * mutator;
	String * p;
	String * newvalue = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\", \"%s\")\n", __func__,
			section, variable, value);
#endif
	if(section == NULL)
		section = "";
	if(variable == NULL || string_get_length(variable) == 0)
		return error_set_code(-EINVAL, "variable: %s",
				strerror(EINVAL));
	if((mutator = (Mutator *)mutator_get(config, section)) == NULL)
	{
		/* create a new section */
		if((mutator = mutator_new()) == NULL)
			return -1;
		if(mutator_set(config, section, mutator) != 0)
		{
			mutator_delete(mutator);
			return -1;
		}
		p = NULL;
	}
	else if((p = (String *)mutator_get(mutator, variable)) == NULL
			&& value == NULL)
		/* there is nothing to do */
		return 0;
	if(value != NULL && (newvalue = string_new(value)) == NULL)
		return -1;
	if(mutator_set(mutator, variable, newvalue) != 0)
	{
		string_delete(newvalue);
		return -1;
	}
	/* free the former value */
	string_delete(p);
	return 0;
}


/* useful */
/* config_foreach */
static void _foreach_callback(Config const * config, String const * key,
		void * value, void * data);

void config_foreach(Config const * config, ConfigForeachCallback callback,
		void * priv)
{
	ConfigForeachData data;

	data.callback = callback;
	data.priv = priv;
	mutator_foreach(config, _foreach_callback, &data);
}

static void _foreach_callback(Config const * config, String const * key,
		void * value, void * data)
{
	ConfigForeachData * priv = (ConfigForeachData *)data;
	(void) value;

	priv->callback(config, key, priv->priv);
}


/* config_foreach_section */
static void _foreach_section_callback(Mutator const * mutator,
		String const * key, void * value, void * data);

void config_foreach_section(Config const * config, String const * section,
		ConfigForeachSectionCallback callback, void * priv)
{
	Mutator * mutator;
	ConfigForeachSectionData data;

	if((mutator = (Mutator *)mutator_get(config, section)) == NULL)
		return; /* could not find section */
	data.callback = callback;
	data.section = section;
	data.priv = priv;
	mutator_foreach(mutator, _foreach_section_callback, &data);
}

static void _foreach_section_callback(Mutator const * mutator,
		String const * key, void * value, void * data)
{
	ConfigForeachSectionData * priv = (ConfigForeachSectionData *)data;

	priv->callback(mutator, priv->section, key, (String const *)value,
			priv->priv);
}


/* config_load */
static int _load_isprint(int c);
static String * _load_section(FILE * fp);
static String * _load_variable(FILE * fp, int c);
static String * _load_value(FILE * fp);

int config_load(Config * config, String const * filename)
{
	int ret = 0;
	size_t line;
	FILE * fp;
	String * section = NULL;
	String * variable = NULL;
	String * value = NULL;
	int c;
	String * str;

	if((fp = fopen(filename, "r")) == NULL)
		return error_set_code(-errno, "%s: %s", filename,
				strerror(errno));
	for(line = 0; (c = fgetc(fp)) != EOF; line++)
		if(c == CONFIG_COMMENT)
			/* skip the comment */
			while((c = fgetc(fp)) != EOF && c != '\n');
		else if(c == '[')
		{
			if((str = _load_section(fp)) == NULL)
				break;
			string_delete(section);
			section = str;
		}
		else if(_load_isprint(c))
		{
			if((str = _load_variable(fp, c)) == NULL)
				break;
			string_delete(variable);
			variable = str;
			if((str = _load_value(fp)) == NULL)
				break;
			string_delete(value);
			value = str;
			if(config_set(config, section, variable, value) != 0)
				break;
		}
		else if(c != '\n')
			break;
	string_delete(section);
	string_delete(variable);
	string_delete(value);
	if(c != EOF)
		ret = error_set_code(1, "%s: %s%ld", filename, "Syntax error"
				" at line ", line);
	if(fclose(fp) != 0)
		ret = error_set_code(-errno, "%s: %s", filename,
				strerror(errno));
	return ret;
}

static int _load_isprint(int c)
{
	if(c == EOF || c == '\n' || c == '\0')
		return 0;
	return 1;
}

static String * _load_section(FILE * fp)
{
	int c;
	String * str;
	String buf[2] = "\0";

	if((str = string_new("")) == NULL)
		return NULL;
	while((c = fgetc(fp)) != EOF && c != ']' && _load_isprint(c))
	{
		buf[0] = c;
		if(string_append(&str, buf) != 0)
			break;
	}
	if(c != ']')
	{
		string_delete(str);
		return NULL;
	}
	return str;
}

static String * _load_variable(FILE * fp, int c)
{
	String * str;
	String buf[2] = "\0";

	buf[0] = c;
	if((str = string_new(buf)) == NULL)
		return NULL;
	while((c = fgetc(fp)) != EOF && c != '=' && _load_isprint(c))
	{
		buf[0] = c;
		if(string_append(&str, buf) != 0)
			break;
	}
	if(c != '=')
	{
		string_delete(str);
		return NULL;
	}
	return str;
}

static String * _load_value(FILE * fp)
{
	int c;
	String * str = NULL;
	String buf[2] = "\0";

	while((c = fgetc(fp)) != EOF && _load_isprint(c))
	{
		buf[0] = c;
		if(string_append(&str, buf) != 0)
			break;
	}
	if(c != EOF && c != '\n')
	{
		string_delete(str);
		return NULL;
	}
	if(str == NULL)
		return string_new("");
	return str;
}


/* config_load_preferences */
int config_load_preferences(Config * config, String const * vendor,
		String const * package, String const * filename)
{
	int ret;

	if((ret = config_load_preferences_system(config, vendor, package,
					filename)) != 0
			&& ret != -ENOENT && ret != -EPERM)
		return ret;
	if((ret = config_load_preferences_user(config, vendor, package,
					filename)) != 0
			&& ret != -ENOENT)
		return ret;
	return 0;
}


/* config_load_preferences_system */
int config_load_preferences_system(Config * config, String const * vendor,
		String const * package, String const * filename)
{
	int ret;
	String * f;

	if(filename == NULL)
		return error_set_code(-EINVAL, "%s", strerror(EINVAL));
	if(vendor != NULL && (string_compare_length(vendor, "../", 3) == 0
				|| string_find(vendor, "/..") != NULL))
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if(package != NULL && (string_compare_length(package, "../", 3) == 0
				|| string_find(package, "/..") != NULL))
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if((f = string_new_append(SYSCONFDIR, "/",
					(vendor != NULL) ? vendor : "", "/",
					(package != NULL) ? package : "", "/",
					filename, NULL)) == NULL)
		return error_get_code();
	ret = config_load(config, f);
	string_delete(f);
	return ret;
}


/* config_load_preferences_user */
int config_load_preferences_user(Config * config, String const * vendor,
		String const * package, String const * filename)
{
	int ret;
	String const * homedir;
	String * f;

	if(filename == NULL)
		return error_set_code(-EINVAL, "%s", strerror(EINVAL));
	if(vendor != NULL && (string_compare_length(vendor, "../", 3) == 0
				|| string_find(vendor, "/..") != NULL))
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if(package != NULL && (string_compare_length(package, "../", 3) == 0
				|| string_find(package, "/..") != NULL))
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if(filename != NULL && string_find(filename, "/") != NULL)
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if((homedir = getenv("HOME")) == NULL)
		return error_set_code(-errno, "%s", strerror(errno));
	if((f = string_new_append(homedir, "/.config/",
					(vendor != NULL) ? vendor : "", "/",
					(package != NULL) ? package : "", "/",
					filename, NULL)) == NULL)
		return error_get_code();
	ret = config_load(config, f);
	string_delete(f);
	return ret;
}


/* config_reset */
static void _delete_foreach(Mutator const * mutator, String const * key,
		void * value, void * data);
static void _delete_foreach_section(Mutator const * mutator, String const * key,
		void * value, void * data);

int config_reset(Config * config)
{
	mutator_foreach(config, _delete_foreach, NULL);
	return mutator_reset(config);
}

static void _delete_foreach(Mutator const * mutator, String const * key,
		void * value, void * data)
{
	Mutator * m = (Mutator *)value;
	(void) mutator;
	(void) key;
	(void) data;

	/* free the values */
	mutator_foreach(m, _delete_foreach_section, NULL);
	mutator_delete(m);
}

static void _delete_foreach_section(Mutator const * mutator, String const * key,
		void * value, void * data)
{
	String * v = (String *)value;
	(void) mutator;
	(void) key;
	(void) data;

	string_delete(v);
}


/* config_save */
static void _save_foreach_default(Mutator const * mutator,
		String const * section, void * value, void * data);
static void _save_foreach(Mutator const * mutator, String const * section,
		void * value, void * data);
static void _save_foreach_section(Mutator const * mutator, String const * key,
		void * value, void * data);

int config_save(Config const * config, String const * filename)
{
	ConfigSave save;

	save.sep = "";
	if((save.fp = fopen(filename, "w")) == NULL)
		return error_set_code(-errno, "%s: %s", filename,
				strerror(errno));
	mutator_foreach(config, _save_foreach_default, &save);
	mutator_foreach(config, _save_foreach, &save);
	if(save.fp != NULL && save.sep[0] != '\0'
			&& fputs(save.sep, save.fp) == EOF)
	{
		fclose(save.fp);
		save.fp = NULL;
	}
	if(save.fp == NULL || fclose(save.fp) != 0)
		return error_set_code(-errno, "%s: %s", filename,
				strerror(errno));
	return 0;
}

static void _save_foreach_default(Mutator const * mutator,
		String const * section, void * value, void * data)
{
	ConfigSave * save = (ConfigSave *)data;
	Mutator * m = (Mutator *)value;
	(void) mutator;

	if(save->fp == NULL)
		return;
	if(section[0] != '\0')
		return;
	mutator_foreach(m, _save_foreach_section, save);
}

static void _save_foreach(Mutator const * mutator, String const * section,
		void * value, void * data)
{
	ConfigSave * save = (ConfigSave *)data;
	Mutator * m = (Mutator *)value;
	(void) mutator;

	if(save->fp == NULL)
		return;
	if(section[0] == '\0')
		return;
	if(fprintf(save->fp, "%s%s[%s]", save->sep, save->sep, section) < 0)
	{
		fclose(save->fp);
		save->fp = NULL;
		return;
	}
	save->sep = "\n";
	mutator_foreach(m, _save_foreach_section, save);
}

static void _save_foreach_section(Mutator const * mutator, String const * key,
		void * value, void * data)
{
	ConfigSave * save = (ConfigSave *)data;
	String const * val = (String const *)value;
	(void) mutator;

	if(save->fp == NULL)
		return;
	if(val == NULL)
		return;
	if(fprintf(save->fp, "%s%s=%s", save->sep, key, val) < 0)
	{
		fclose(save->fp);
		save->fp = NULL;
		return;
	}
	save->sep = "\n";
}


/* config_save_preferences_user */
static int _save_preferences_user_mkdir(String * dir);

int config_save_preferences_user(Config const * config, String const * vendor,
		String const * package, String const * filename)
{
	int ret;
	String const * homedir;
	String * f;

	if(filename == NULL)
		return error_set_code(-EINVAL, "%s", strerror(EINVAL));
	if((homedir = getenv("HOME")) == NULL)
		return error_set_code(-errno, "%s", strerror(errno));
	if((f = string_new_append(homedir, "/.config/",
					(vendor != NULL) ? vendor : "",
					(vendor != NULL) ? "/" : "",
					(package != NULL) ? package : "",
					(package != NULL) ? "/" : "",
					filename, NULL)) == NULL)
		return error_get_code();
	if((ret = _save_preferences_user_mkdir(f)) == 0)
		ret = config_save(config, f);
	string_delete(f);
	return ret;
}

static int _save_preferences_user_mkdir(String * dir)
{
	int ret = 0;
	size_t i;
	size_t j;
	size_t len;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, dir);
#endif
	len = string_get_length(dir);
	for(i = 0, j = 0; j < len; j++)
	{
		if(dir[j] != '/')
			continue;
		if(i == j)
		{
			i++;
			continue;
		}
		dir[j] = '\0';
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() \"%s\"\n", __func__, dir);
#endif
		if(dir[i] == '.')
			ret = error_set_code(-EPERM, "%s: %s", dir,
					strerror(EPERM));
		else if(mkdir(dir, 0777) != 0 && errno != EEXIST)
			ret = error_set_code(-errno, "%s: %s", dir,
					strerror(errno));
		dir[j] = '/';
		if(ret != 0)
			return ret;
	}
	return 0;
}
