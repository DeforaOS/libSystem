/* $Id$ */
/* Copyright (c) 2006-2021 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_CONFIG_H
# define LIBSYSTEM_SYSTEM_CONFIG_H

# include "string.h"

# ifdef __cplusplus
extern "C" {
# endif


/* Config */
/* types */
typedef struct _Hash Config;

typedef void (*ConfigForeachCallback)(Config const * config,
		String const * section, void * priv);
typedef void (*ConfigForeachSectionCallback)(Config const * config,
		String const * section, String const * variable,
		String const * value, void * priv);


/* functions */
Config * config_new(void);
Config * config_new_copy(Config const * from);
Config * config_new_load(String const * filename);
void config_delete(Config * config);

/* accessors */
String const * config_get(Config const * config, String const * section,
		String const * variable);
int config_set(Config * config, String const * section, String const * variable,
		String const * value);

/* useful */
void config_foreach(Config const * config, ConfigForeachCallback callback,
		void * priv);
void config_foreach_section(Config const * config, String const * section,
		ConfigForeachSectionCallback callback, void * priv);

int config_load(Config * config, String const * filename);

int config_load_preferences(Config * config, String const * vendor,
		String const * package, String const * filename);
int config_load_preferences_system(Config * config, String const * vendor,
		String const * package, String const * filename);
int config_load_preferences_user(Config * config, String const * vendor,
		String const * package, String const * filename);

int config_reset(Config * config);

int config_save(Config const * config, String const * filename);
int config_save_preferences_user(Config const * config, String const * vendor,
		String const * package, String const * filename);

# ifdef __cplusplus
}
# endif
#endif /* !LIBSYSTEM_SYSTEM_CONFIG_H */
