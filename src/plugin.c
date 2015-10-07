/* $Id$ */
/* Copyright (c) 2008-2015 Pierre Pronchery <khorben@defora.org> */
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



#include <stddef.h>
#ifdef __WIN32__
# include <windows.h>
#else
# include <dlfcn.h>
#endif
#include "System/error.h"
#include "System/plugin.h"


/* Plugin */
/* private */
/* constants */
#if defined(__APPLE__)
# define PLUGIN_EXTENSION	".dylib"
#elif defined(__WIN32__)
# define PLUGIN_EXTENSION	".dll"
#else
# define PLUGIN_EXTENSION	".so"
#endif


/* prototypes */
static Plugin * _plugin_open(String const * filename);


/* functions */
static Plugin * _plugin_open(String const * filename)
{
#ifdef __WIN32__
	if(filename == NULL)
		return GetModuleHandle(NULL);
	return LoadLibraryEx(filename, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
#else
	Plugin * plugin;

	if((plugin = dlopen(filename, RTLD_LAZY)) == NULL)
		error_set_code(1, "%s", dlerror());
	return plugin;
#endif
}


/* public */
/* functions */
/* plugin_new */
Plugin * plugin_new(String const * libdir, String const * package,
		String const * type, String const * name)
{
	Plugin * plugin;
	String * filename;
	char const ext[] = PLUGIN_EXTENSION;

	if((filename = string_new_append(libdir, "/", package, "/", type, "/",
					name, ext, NULL)) == NULL)
		return NULL;
	plugin = _plugin_open(filename);
	string_delete(filename);
	return plugin;
}


/* plugin_new_self */
Plugin * plugin_new_self(void)
{
	return _plugin_open(NULL);
}


/* plugin_delete */
void plugin_delete(Plugin * plugin)
{
#ifdef __WIN32__
	FreeLibrary(plugin);
#else
	dlclose(plugin);
#endif
}


/* useful */
/* plugin_lookup */
void * plugin_lookup(Plugin * plugin, String const * symbol)
{
#ifdef __WIN32__
	return GetProcAddress(plugin, symbol);
#else
	void * ret;

	if((ret = dlsym(plugin, symbol)) == NULL)
		error_set_code(1, "%s", dlerror());
	return ret;
#endif
}
