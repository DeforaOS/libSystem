/* $Id$ */
/* Copyright (c) 2012-2015 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_VARIABLE_H
# define LIBSYSTEM_SYSTEM_VARIABLE_H

# include <stdint.h>
# include "buffer.h"
# include "string.h"


/* Variable */
/* types */
typedef struct _Variable Variable;

typedef enum _VariableType
{
	VT_NULL = 0,
	VT_BOOL,
	VT_INT8,
	VT_UINT8,
	VT_INT16,
	VT_UINT16,
	VT_INT32,
	VT_UINT32,
	VT_INT64,
	VT_UINT64,
	VT_FLOAT,
	VT_DOUBLE,
	VT_BUFFER,
	VT_STRING,
	VT_ARRAY,
	VT_COMPOUND
} VariableType;

typedef unsigned int VariableClass;


/* functions */
Variable * variable_new(VariableType type, void const * value);
Variable * variable_new_array(VariableType type, size_t size, ...);
Variable * variable_new_arrayv(VariableType type, size_t size, void ** values);
Variable * variable_new_compound(String const * name, size_t members, ...);
Variable * variable_new_compoundv(String const * name, size_t members,
		VariableType * types, void ** values);
Variable * variable_new_copy(Variable * variable);
Variable * variable_new_deserialize(size_t * size, char const * data);
Variable * variable_new_deserialize_buffer(size_t * size,
		Buffer const * buffer);
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data);
void variable_delete(Variable * variable);


/* accessors */
int variable_get_as(Variable * variable, VariableType type, void * result);
VariableClass variable_get_class(Variable * variable);
VariableType variable_get_type(Variable * variable);

int variable_is_array(Variable * variable);
int variable_is_class(Variable * variable, VariableClass _class);
int variable_is_compound(Variable * variable);
int variable_is_instance(Variable * variable, String const * name);
int variable_is_scalar(Variable * variable);
int variable_is_type(Variable * variable, VariableType type);

int variable_set(Variable * variable, Variable * from);
int variable_set_from(Variable * variable, VariableType type,
		void const * value);

/* useful */
int variable_serialize(Variable * variable, Buffer * buffer, int type);

#endif /* !LIBSYSTEM_SYSTEM_VARIABLE_H */
