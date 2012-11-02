/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_VARIABLE_H
# define LIBSYSTEM_VARIABLE_H

# include <sys/types.h>
# include "buffer.h"
# include "string.h"


/* Variable */
/* types */
typedef struct _Variable Variable;

typedef enum _VariableType
{
	VT_NULL = 0,
	VT_INT8,
	VT_UINT8,
	VT_INT16,
	VT_UINT16,
	VT_INT32,
	VT_UINT32,
	VT_INT64,
	VT_UINT64,
	VT_STRING
} VariableType;


/* functions */
Variable * variable_new(VariableType type);
void variable_delete(Variable * variable);


/* accessors */
int variable_get_as_integer(Variable * variable, int64_t integer);
int variable_get_as_uinteger(Variable * variable, uint64_t integer);
String * variable_get_as_string(Variable * variable);

int variable_set_from_integer(Variable * variable, int64_t integer);
int variable_set_from_uinteger(Variable * variable, uint64_t integer);

/* useful */
int variable_serialize(Variable * variable, Buffer * buffer);

#endif /* !LIBSYSTEM_VARIABLE_H */
