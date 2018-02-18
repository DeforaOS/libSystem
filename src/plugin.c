/* $Id$ */
/* Copyright (c) 2008-2018 Pierre Pronchery <khorben@defora.org> */
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
