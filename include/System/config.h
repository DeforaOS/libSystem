/* $Id$ */
/* Copyright (c) 2006-2015 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_CONFIG_H
# define LIBSYSTEM_SYSTEM_CONFIG_H

# include "string.h"


/* Config */
/* types */
typedef struct _Hash Config;

typedef void (*ConfigForeachCallback)(String const * section, void * priv);
typedef void (*ConfigForeachSectionCallback)(String const * variable,
		String const * value, void * priv);


/* functions */
Config * config_new(void);
void config_delete(Config * config);

/* accessors */
String const * config_get(Config * config, String const * section,
		String const * variable);
int config_set(Config * config, String const * section, String const * variable,
		String const * value);

/* useful */
void config_foreach(Config * config, ConfigForeachCallback callback,
		void * priv);
void config_foreach_section(Config * config, String const * section,
		ConfigForeachSectionCallback callback, void * priv);

int config_load(Config * config, String const * filename);

int config_load_preferences(Config * config, String const * vendor,
		String const * package, String const * filename);
int config_load_preferences_system(Config * config, String const * vendor,
		String const * package, String const * filename);
int config_load_preferences_user(Config * config, String const * vendor,
		String const * package, String const * filename);

int config_reset(Config * config);

int config_save(Config * config, String const * filename);

#endif /* !LIBSYSTEM_SYSTEM_CONFIG_H */
