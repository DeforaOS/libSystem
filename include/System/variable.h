/* $Id$ */
/* Copyright (c) 2012-2018 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_VARIABLE_H
# define LIBSYSTEM_SYSTEM_VARIABLE_H

# include <stdarg.h>
# include <stdbool.h>
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

typedef int VariableError;


/* functions */
Variable * variable_new(VariableType type, ...);
Variable * variable_newv(VariableType type, va_list ap);
Variable * variable_new_array(VariableType type, size_t size, ...);
Variable * variable_new_arrayv(VariableType type, size_t size, va_list ap);
Variable * variable_new_compound(String const * name, size_t members, ...);
Variable * variable_new_compoundv(String const * name, size_t members,
		va_list ap);
Variable * variable_new_compound_variables(String const * name, size_t members,
		Variable ** variables);
Variable * variable_new_copy(Variable const * from);
Variable * variable_new_deserialize(size_t * size, char const * data);
Variable * variable_new_deserialize_buffer(size_t * size,
		Buffer const * buffer);
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data);
void variable_delete(Variable * variable);


/* accessors */
VariableError variable_get_as(Variable * variable, VariableType type,
		void * result);
VariableClass variable_get_class(Variable * variable);
VariableType variable_get_type(Variable * variable);

bool variable_is_array(Variable * variable);
bool variable_is_class(Variable * variable, VariableClass _class);
bool variable_is_compound(Variable * variable);
bool variable_is_instance(Variable * variable, String const * name);
bool variable_is_scalar(Variable * variable);
bool variable_is_type(Variable * variable, VariableType type);

VariableError variable_set(Variable * variable, ...);
VariableError variable_setv(Variable * variable, va_list ap);
VariableError variable_set_type(Variable * variable, VariableType type, ...);
VariableError variable_set_typev(Variable * variable, VariableType type,
		va_list ap);

/* useful */
VariableError variable_copy(Variable * variable, Variable const * from);
VariableError variable_serialize(Variable * variable, Buffer * buffer,
		bool prefix);

#endif /* !LIBSYSTEM_SYSTEM_VARIABLE_H */
