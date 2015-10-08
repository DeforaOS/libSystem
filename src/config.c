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
/* TODO:
 * - offer a way to load preferences (optionally from the system first) */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "System/error.h"
#include "System/mutator.h"
#include "System/config.h"

/* constants */
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
	char * oldvalue;
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
			return 1;
		if((p = string_new(section)) == NULL
				|| mutator_set(config, p, mutator) != 0)
		{
			string_delete(p);
			mutator_delete(mutator);
			return 1;
		}
		oldvalue = NULL;
	}
	else
		/* to free the current value if already set */
		oldvalue = mutator_get(mutator, variable);
	if((p = string_new(variable)) == NULL)
		return 1;
	if(value != NULL && (newvalue = string_new(value)) == NULL)
	{
		string_delete(p);
		return 1;
	}
	/* set the new value */
	if(mutator_set(mutator, p, newvalue) != 0)
	{
		string_delete(p);
		string_delete(newvalue);
		return 1;
	}
	string_delete(oldvalue);
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
	String * section;
	String * variable = NULL;
	int c;
	String * str;

	if((section = string_new("")) == NULL)
		return 1;
	if((fp = fopen(filename, "r")) == NULL)
	{
		string_delete(section);
		return error_set_code(1, "%s: %s", filename, strerror(errno));
	}
	/* FIXME unescape backslashes (eg allow multiple lines) */
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
			/* XXX optimize string alloc/free, and may fail */
			config_set(config, section, variable, str);
			string_delete(str);
		}
		else if(c != '\n')
			break;
	string_delete(section);
	string_delete(variable);
	if(!feof(fp))
		ret = error_set_code(1, "%s: %s%zd", filename, "Syntax error"
				" at line ", line);
	if(fclose(fp) != 0)
		ret = error_set_code(1, "%s: %s", filename, strerror(errno));
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


/* config_reset */
static void _delete_foreach(char const * key, void * value, void * data);
static void _delete_foreach_section(char const * key, void * value,
		void * data);

int config_reset(Config * config)
{
	mutator_foreach(config, _delete_foreach, NULL);
	return mutator_reset(config);
}

static void _delete_foreach(char const * key, void * value, void * data)
{
	char * str = (char *)key;
	Mutator * mutator = value;

	free(str);
	mutator_foreach(mutator, _delete_foreach_section, data);
	mutator_delete(mutator);
}

static void _delete_foreach_section(char const * key, void * value, void * data)
{
	char * k = (char *)key;
	char * v = value;

	free(k);
	free(v);
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
		return error_set_code(1, "%s: %s", filename, strerror(errno));
	mutator_foreach(config, _save_foreach_default, &save);
	mutator_foreach(config, _save_foreach, &save);
	if(save.fp != NULL && save.sep[0] != '\0'
			&& fputs(save.sep, save.fp) == EOF)
	{
		fclose(save.fp);
		save.fp = NULL;
	}
	if(save.fp == NULL || fclose(save.fp) != 0)
		return error_set_code(1, "%s: %s", filename, strerror(errno));
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
