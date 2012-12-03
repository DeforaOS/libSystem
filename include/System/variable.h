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

# include <stdint.h>
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
	VT_BUFFER,
	VT_STRING
} VariableType;


/* functions */
Variable * variable_new(VariableType type, void * value);
Variable * variable_new_copy(Variable * variable);
Variable * variable_new_deserialize(size_t * size, char const * data);
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data);
void variable_delete(Variable * variable);


/* accessors */
int variable_get_as(Variable * variable, VariableType type, void * result);
int variable_get_as(Variable * variable, VariableType type, void * result);

int variable_set_from(Variable * variable, VariableType type, void * value);
int variable_set_from(Variable * variable, VariableType type, void * value);

/* useful */
int variable_serialize(Variable * variable, Buffer * buffer, int type);

#endif /* !LIBSYSTEM_VARIABLE_H */
