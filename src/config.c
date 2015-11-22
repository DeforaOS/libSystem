/* $Id$ */
/* Copyright (c) 2005-2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



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
	void * priv;
} ConfigForeachSectionData;

typedef struct _ConfigSave
{
	FILE * fp;
	char const * sep;
} ConfigSave;


/* public */
/* functions */
/* config_new */
Config * config_new(void)
{
	return mutator_new();
}


/* config_delete */
void config_delete(Config * config)
{
	config_reset(config);
	mutator_delete(config);
}


/* accessors */
/* config_get */
char const * config_get(Config * config, char const * section,
		char const * variable)
{
	Mutator * mutator;
	char const * value;

	if(section == NULL)
		section = "";
	if((mutator = mutator_get(config, section)) == NULL)
	{
		/* the section does not exist */
		if(section[0] == '\0')
			error_set_code(1, "%s", "No default section");
		else
			error_set_code(1, "%s%s", section, ": No such section");
		return NULL;
	}
	if((value = mutator_get(mutator, variable)) == NULL)
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
int config_set(Config * config, char const * section, char const * variable,
		char const * value)
{
	Mutator * mutator;
	char * p;
	char * newvalue = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\", \"%s\", \"%s\")\n", __func__,
			section, variable, value);
#endif
	if(section == NULL)
		section = "";
	if(variable == NULL || string_get_length(variable) == 0)
		return error_set_code(-EINVAL, "variable: %s",
				strerror(EINVAL));
	if((mutator = mutator_get(config, section)) == NULL)
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
	else if((p = mutator_get(mutator, variable)) == NULL && value == NULL)
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
static void _foreach_callback(char const * key, void * value, void * data);

void config_foreach(Config * config, ConfigForeachCallback callback,
		void * priv)
{
	ConfigForeachData data;

	data.callback = callback;
	data.priv = priv;
	mutator_foreach(config, _foreach_callback, &data);
}

static void _foreach_callback(char const * key, void * value, void * data)
{
	ConfigForeachData * priv = data;
	(void) value;

	priv->callback(key, priv->priv);
}


/* config_foreach_section */
static void _foreach_section_callback(char const * key, void * value,
		void * data);

void config_foreach_section(Config * config, char const * section,
		ConfigForeachSectionCallback callback, void * priv)
{
	Mutator * mutator;
	ConfigForeachSectionData data;

	if((mutator = mutator_get(config, section)) == NULL)
		return; /* could not find section */
	data.callback = callback;
	data.priv = priv;
	mutator_foreach(mutator, _foreach_section_callback, &data);
}

static void _foreach_section_callback(char const * key, void * value,
		void * data)
{
	ConfigForeachSectionData * priv = data;

	priv->callback(key, value, priv->priv);
}


/* config_load */
static int _load_isprint(int c);
static String * _load_section(FILE * fp);
static String * _load_variable(FILE * fp, int c);
static String * _load_value(FILE * fp);

int config_load(Config * config, char const * filename)
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
	char * str = NULL;
	size_t len = 0;
	char * p;

	while((c = fgetc(fp)) != EOF && c != ']' && _load_isprint(c))
	{
		if((p = realloc(str, sizeof(*str) * (len + 2))) == NULL)
		{
			free(str);
			return NULL;
		}
		str = p;
		str[len++] = c;
	}
	if(c != ']')
	{
		free(str);
		return NULL;
	}
	if(str == NULL)
		return string_new("");
	str[len] = '\0';
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
		{
			string_delete(str);
			return NULL;
		}
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
	size_t len = 0;
	char * p;

	while((c = fgetc(fp)) != EOF && _load_isprint(c))
	{
		if((p = realloc(str, sizeof(*str) * (len + 2))) == NULL)
		{
			free(str);
			return NULL;
		}
		str = p;
		str[len++] = c;
	}
	if(c != EOF && c != '\n')
	{
		free(str);
		return NULL;
	}
	if(str == NULL)
		return string_new("");
	str[len] = '\0';
	return str;
}


/* config_load_preferences */
int config_load_preferences(Config * config, String const * vendor,
		String const * package, String const * filename)
{
	int ret;

	if((ret = config_load_preferences_system(config, vendor, package,
					filename)) != 0
			&& ret != -ENOENT)
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
	if(vendor != NULL && string_find(vendor, "/") != NULL)
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if(package != NULL && string_find(package, "/") != NULL)
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
	if(vendor != NULL && string_find(vendor, "/") != NULL)
		return error_set_code(-EPERM, "%s", strerror(EPERM));
	if(package != NULL && string_find(package, "/") != NULL)
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
static void _delete_foreach(String const * key, void * value, void * data);
static void _delete_foreach_section(String const * key, void * value,
		void * data);

int config_reset(Config * config)
{
	mutator_foreach(config, _delete_foreach, NULL);
	return mutator_reset(config);
}

static void _delete_foreach(String const * key, void * value, void * data)
{
	Mutator * mutator = value;
	(void) key;
	(void) data;

	/* free the values */
	mutator_foreach(mutator, _delete_foreach_section, NULL);
	mutator_delete(mutator);
}

static void _delete_foreach_section(String const * key, void * value,
		void * data)
{
	String * v = value;
	(void) key;
	(void) data;

	string_delete(v);
}


/* config_save */
static void _save_foreach_default(char const * section, void * value,
		void * data);
static void _save_foreach(char const * section, void * value, void * data);
static void _save_foreach_section(char const * key, void * value, void * data);

int config_save(Config * config, char const * filename)
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

static void _save_foreach_default(char const * section, void * value,
		void * data)
{
	ConfigSave * save = data;
	Mutator * mutator = value;

	if(save->fp == NULL)
		return;
	if(section[0] != '\0')
		return;
	mutator_foreach(mutator, _save_foreach_section, save);
}

static void _save_foreach(char const * section, void * value, void * data)
{
	ConfigSave * save = data;
	Mutator * mutator = value;

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
	mutator_foreach(mutator, _save_foreach_section, save);
}

static void _save_foreach_section(char const * key, void * value, void * data)
{
	ConfigSave * save = data;
	char const * val = value;

	if(save->fp == NULL)
		return;
	/* FIXME escape lines with a backslash */
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
