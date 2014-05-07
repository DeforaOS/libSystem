/* $Id$ */
/* Copyright (c) 2008-2014 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_PLUGIN_H
# define LIBSYSTEM_PLUGIN_H

# include "string.h"


/* Plugin */
typedef void Plugin;


/* functions */
Plugin * plugin_new(String const * libdir, String const * package,
		String const * type, String const * name);
Plugin * plugin_new_self(void);
void plugin_delete(Plugin * plugin);


/* useful */
void * plugin_lookup(Plugin * plugin, String const * symbol);

#endif /* !LIBSYSTEM_PLUGIN_H */
