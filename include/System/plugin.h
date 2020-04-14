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



#ifndef LIBSYSTEM_SYSTEM_PLUGIN_H
# define LIBSYSTEM_SYSTEM_PLUGIN_H

# include "license.h"
# include "string.h"


/* Plugin */
typedef struct _Plugin Plugin;
typedef void * PluginExport;

typedef struct _PluginHeader
{
	char const * name;
	char const * icon;
	char const * description;
	LicenseFlags license;
} PluginHeader;


/* functions */
Plugin * plugin_new(String const * libdir, String const * package,
		String const * type, String const * name);
Plugin * plugin_new_self(void);
void plugin_delete(Plugin * plugin);

/* accessors */
PluginExport plugin_get_export(Plugin * plugin, String const * name);

/* useful */
PluginExport plugin_lookup(Plugin * plugin, String const * symbol);

#endif /* !LIBSYSTEM_SYSTEM_PLUGIN_H */
