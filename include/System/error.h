/* $Id$ */
/* Copyright (c) 2007-2014 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_ERROR_H
# define LIBSYSTEM_SYSTEM_ERROR_H

# include "string.h"


/* functions */
/* accessors */
String const * error_get(int * code);
int error_get_code(void);

void error_set(String const * format, ...);
int error_set_code(int code, String const * format, ...);
int error_set_print(String const * program, int code,
		String const * format, ...);

/* useful */
int error_print(String const * program);

#endif /* !LIBSYSTEM_SYSTEM_ERROR_H */
