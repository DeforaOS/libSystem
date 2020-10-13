/* $Id$ */
/* Copyright (c) 2012-2020 Pierre Pronchery <khorben@defora.org> */
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



#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/variable.h"


/* Variable */
/* private */
/* types */
struct _Variable
{
	VariableType type;

	union
	{
		bool b;
		int8_t int8;
		uint8_t uint8;
		int16_t int16;
		uint16_t uint16;
		int32_t int32;
		uint32_t uint32;
		int64_t int64;
		uint64_t uint64;
		float f;
		double d;
		Buffer * buffer;
		String * string;
		struct {
			VariableType type;
			Array * array;
		} array;
		struct {
			String * name;
			Array * members;
		} compound;
		void * pointer;
	} u;
};


/* constants */
static const size_t _variable_sizes[VT_COUNT] = { 0, 1,
	sizeof(int8_t), sizeof(uint8_t), sizeof(int16_t), sizeof(uint16_t),
	sizeof(int32_t), sizeof(uint32_t), sizeof(int64_t), sizeof(uint64_t),
	sizeof(float), sizeof(double), sizeof(uint32_t), 0, 0, 0,
	sizeof(void *) };


/* prototypes */
static uint16_t _bswap16(uint16_t u);
static uint32_t _bswap32(uint32_t u);
static uint64_t _bswap64(uint64_t u);

static void _variable_destroy(Variable * variable);


/* public */
/* variable_new */
Variable * variable_new(VariableType type, ...)
{
	Variable * variable;
	va_list ap;

	va_start(ap, type);
	variable = variable_newv(type, ap);
	va_end(ap);
	return variable;
}


/* variable_newv */
Variable * variable_newv(VariableType type, va_list ap)
{
	Variable * variable;

	if((variable = (Variable *)object_new(sizeof(*variable))) == NULL)
		return NULL;
	variable->type = VT_NULL;
	if(variable_set_typev(variable, type, ap) != 0)
	{
		object_delete(variable);
		return NULL;
	}
	return variable;
}


/* variable_new_array */
Variable * variable_new_array(VariableType type, size_t size, ...)
{
	Variable * variable;
	va_list ap;

	va_start(ap, size);
	variable = variable_new_arrayv(type, size, ap);
	va_end(ap);
	return variable;
}


/* variable_new_arrayv */
Variable * variable_new_arrayv(VariableType type, size_t size, va_list ap)
{
	Variable * variable;
	size_t i;
	void * p;

	if((variable = variable_new(VT_ARRAY, type, size)) == NULL)
		return NULL;
	for(i = 0; i < size; i++)
	{
		p = va_arg(ap, void *);
		if(array_set(variable->u.array.array, i, p) != 0)
		{
			variable_delete(variable);
			return NULL;
		}
	}
	return variable;
}


/* variable_new_compound */
Variable * variable_new_compound(String const * name, size_t members, ...)
{
	Variable * variable;
	va_list ap;

	va_start(ap, members);
	variable = variable_new_compoundv(name, members, ap);
	va_end(ap);
	return variable;
}


/* variable_new_compoundv */
Variable * variable_new_compoundv(String const * name, size_t members,
		va_list ap)
{
	Variable * variable;
	size_t i;
	VariableType type;
	Variable * v;

	if((variable = variable_new(VT_COMPOUND, name, members)) == NULL)
		return NULL;
	for(i = 0; i < members; i++)
	{
		type = (VariableType)va_arg(ap, unsigned int);
		if((v = variable_newv(type, ap)) == NULL
				|| array_set(variable->u.compound.members, i,
					v) != 0)
		{
			variable_delete(variable);
			return NULL;
		}
	}
	return variable;
}


/* variable_new_compound_variables */
Variable * variable_new_compound_variables(String const * name, size_t members,
		Variable ** variables)
{
	Variable * variable;
	size_t i;
	Variable * v;

	if((variable = variable_new(VT_COMPOUND, name, members)) == NULL)
		return NULL;
	for(i = 0; i < members; i++)
		if((v = variable_new_copy(variables[i])) == NULL
				|| array_set(variable->u.compound.members, i,
					v) != 0)
		{
			variable_delete(variable);
			return NULL;
		}
	return variable;
}


/* variable_new_copy */
static Variable * _new_copy_array(Variable const * from);

Variable * variable_new_copy(Variable const * from)
{
	switch(from->type)
	{
		case VT_NULL:
			return variable_new(from->type);
		case VT_BOOL:
			return variable_new(from->type, from->u.b);
		case VT_INT8:
			return variable_new(from->type, from->u.int8);
		case VT_UINT8:
			return variable_new(from->type, from->u.uint8);
		case VT_INT16:
			return variable_new(from->type, from->u.int16);
		case VT_UINT16:
			return variable_new(from->type, from->u.uint16);
		case VT_INT32:
			return variable_new(from->type, from->u.int32);
		case VT_UINT32:
			return variable_new(from->type, from->u.uint32);
		case VT_INT64:
			return variable_new(from->type, from->u.int64);
		case VT_UINT64:
			return variable_new(from->type, from->u.uint64);
		case VT_FLOAT:
			return variable_new(from->type, from->u.f);
		case VT_DOUBLE:
			return variable_new(from->type, from->u.d);
		case VT_STRING:
			return variable_new(from->type, from->u.string);
		case VT_BUFFER:
			return variable_new(from->type, from->u.buffer);
		case VT_ARRAY:
			return _new_copy_array(from);
		case VT_POINTER:
			return variable_new(from->type, from->u.pointer);
	}
	error_set_code(1, "%s%u%s", "Unable to copy this type of variable (",
			from->type, ")");
	return NULL;
}

static Variable * _new_copy_array(Variable const * variable)
{
	Variable * ret;
	size_t size;

	size = array_get_size(variable->u.array.array);
	if((ret = variable_new_array(variable->u.array.type, size)) == NULL)
		return NULL;
	if(array_copy(ret->u.array.array, variable->u.array.array) != 0)
	{
		variable_delete(ret);
		return NULL;
	}
	return ret;
}


/* variable_new_deserialize */
Variable * variable_new_deserialize(size_t * size, char const * data)
{
	Variable * variable;
	uint8_t u8;
	VariableType type;
	size_t s;

	/* check the arguments */
	if(size == NULL || *size < sizeof(u8) || data == NULL)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		return NULL;
	}
	/* obtain the type from the data */
	u8 = data[0];
	type = (VariableType)u8;
	s = *size - sizeof(u8);
	/* deserialize according to the type */
	variable = variable_new_deserialize_type(type, &s, &data[sizeof(u8)]);
	*size = s + sizeof(u8);
	return variable;
}


/* variable_new_deserialize_buffer */
Variable * variable_new_deserialize_buffer(size_t * size, Buffer const * buffer)
{
	BufferData const * data;

	data = buffer_get_data(buffer);
	return variable_new_deserialize(size, data);
}


/* variable_new_deserialize_type */
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data)
{
	Variable * v;
	size_t s;
	uint32_t u32;
	uint64_t u64;
	float f;
	double d;
	Buffer * b;
	int res;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, %lu, %p)\n", __func__, type, *size,
			(void *)data);
#endif
	/* estimate the size required */
	s = (type < sizeof(_variable_sizes) / sizeof(*_variable_sizes))
		? _variable_sizes[type] : 0;
	switch(type)
	{
		case VT_NULL:
		case VT_BOOL:
		case VT_INT8:
		case VT_UINT8:
		case VT_INT16:
		case VT_UINT16:
		case VT_INT32:
		case VT_UINT32:
		case VT_INT64:
		case VT_UINT64:
		case VT_POINTER:
			break;
		case VT_BUFFER:
			if(*size < s)
				break;
			memcpy(&u32, data, s);
			u32 = _bswap32(u32);
			s += u32;
			break;
		case VT_FLOAT:
		case VT_DOUBLE:
		case VT_STRING:
			for(s = 0; s < *size && data[s] != '\0'; s++);
			break;
		default:
			error_set_code(1, "Unable to deserialize type %u",
					type);
			return NULL;
	}
	if(*size < s)
	{
		*size = s;
		error_set_code(1, "More data needed to deserialize type %u",
				type);
		return NULL;
	}
	*size = s;
	/* convert the data if necessary */
	switch(type)
	{
		case VT_NULL:
			v = variable_new(type);
			break;
		case VT_BOOL:
			if((data[0] & 0xfe) != 0x0)
			{
				error_set_code(1, "Invalid boolean value");
				return NULL;
			}
			v = variable_new(type, data[0]);
			break;
		case VT_INT8:
		case VT_UINT8:
			v = variable_new(type, data[0]);
			break;
		case VT_INT16:
		case VT_UINT16:
			v = variable_new(type, data[1] << 8 | data[0]);
			break;
		case VT_INT32:
		case VT_UINT32:
			v = variable_new(type, data[3] << 24 | data[2] << 16
					| data[1] << 8 | data[0]);
			break;
		case VT_FLOAT:
			res = sscanf(data, "%e", &f);
			if(res != 1)
			{
				error_set_code(1, "Invalid float value");
				return NULL;
			}
			v = variable_new(type, f);
			break;
		case VT_DOUBLE:
			res = sscanf(data, "%le", &d);
			if(res != 1)
			{
				error_set_code(1, "Invalid double value");
				return NULL;
			}
			v = variable_new(type, d);
			break;
		case VT_STRING:
			v = variable_new(type, data);
			break;
		case VT_INT64:
		case VT_UINT64:
			u64 = (uint64_t)data[7] << 56 | (uint64_t)data[6] << 48
				| (uint64_t)data[5] << 40
				| (uint64_t)data[4] << 32
				| data[3] << 24 | data[2] << 16
				| data[1] << 8 | data[0];
			v = variable_new(type, u64);
			break;
		case VT_BUFFER:
			/* XXX avoid copying the buffer */
			if((b = buffer_new(s - sizeof(u32), &data[sizeof(u32)]))
					== NULL)
				return NULL;
			v = variable_new(type, b);
			buffer_delete(b);
			break;
		default:
			error_set_code(1, "Unable to deserialize type %u",
					type);
			return NULL;
	}
	return v;
}


/* variable_delete */
void variable_delete(Variable * variable)
{
	_variable_destroy(variable);
	object_delete(variable);
}


/* variable_get_as */
static VariableError _get_as_convert(Variable * variable, VariableType type,
		void * result);
static VariableError _get_as_convert_string(Variable * variable,
		String ** result);

VariableError variable_get_as(Variable * variable, VariableType type,
		void * result)
{
	void * p;
	size_t size;
	Buffer ** b;
	String ** s;
	Array ** a;

	if(variable->type != type)
		return _get_as_convert(variable, type, result);
	p = variable_get_pointer(variable);
	size = (type < sizeof(_variable_sizes) / sizeof(*_variable_sizes))
		? _variable_sizes[type] : 0;
	switch(type)
	{
		case VT_NULL:
			return 0;
		case VT_BOOL:
		case VT_INT8:
		case VT_UINT8:
		case VT_INT16:
		case VT_UINT16:
		case VT_INT32:
		case VT_UINT32:
		case VT_INT64:
		case VT_UINT64:
		case VT_FLOAT:
		case VT_DOUBLE:
		case VT_POINTER:
			memcpy(result, p, size);
			return 0;
		case VT_BUFFER:
			b = (Buffer **)result;
			return (*b = buffer_new_copy(variable->u.buffer))
				!= NULL ? 0 : -1;
		case VT_STRING:
			s = (String **)result;
			return (*s = string_new(variable->u.string)) != NULL
				? 0 : -1;
		case VT_ARRAY:
			a = (Array **)result;
			return (*a = array_new_copy(variable->u.array.array))
				!= NULL ? 0 : -1;
	}
	/* FIXME implement the rest */
	return -1;
}

static VariableError _get_as_convert(Variable * variable, VariableType type,
		void * result)
{
	size_t size = 0;
	void * p = NULL;
	int8_t i8;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	float * fp;
	double * dp;

	switch(type)
	{
		case VT_NULL:
		case VT_POINTER:
			break;
		case VT_INT8:
			size = sizeof(i8);
			if(variable->type == VT_UINT8
					&& variable->u.uint8 <= 0x7f)
				i8 = variable->u.uint8;
			else if(variable->type == VT_INT16
					&& variable->u.int16 >= (-0x7f-1)
					&& variable->u.int16 <= 0x7f)
				i8 = variable->u.int16;
			else if(variable->type == VT_UINT16
					&& variable->u.uint16 <= 0x7f)
				i8 = variable->u.uint16;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= (-0x7f-1)
					&& variable->u.int32 <= 0x7f)
				i8 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32 <= 0x7f)
				i8 = variable->u.uint32;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= (-0x7f-1)
					&& variable->u.int64 <= 0x7f)
				i8 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0x7f)
				i8 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			p = &i8;
			break;
		case VT_BOOL:
		case VT_UINT8:
			size = sizeof(u8);
			if(variable->type == VT_UINT8)
			{
				p = &variable->u.uint8;
				break;
			}
			if(variable->type == VT_INT8
					&& variable->u.int8 >= 0)
				u8 = variable->u.int8;
			else if(variable->type == VT_INT16
					&& variable->u.int16 >= 0
					&& variable->u.int16 <= 0xff)
				u8 = variable->u.int16;
			else if(variable->type == VT_UINT16
					&& variable->u.uint16 <= 0xff)
				u8 = variable->u.uint16;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= 0
					&& variable->u.int32 <= 0xff)
				u8 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32 <= 0xff)
				u8 = variable->u.uint32;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= 0
					&& variable->u.int64 <= 0xff)
				u8 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0xff)
				u8 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			p = &u8;
			break;
		case VT_INT16:
			size = sizeof(i16);
			if(variable->type == VT_UINT16
					&& variable->u.uint16 <= 0x7fff)
				i16 = variable->u.uint16;
			else if(variable->type == VT_INT8)
				i16 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				i16 = variable->u.uint8;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= (-0x7fff-1)
					&& variable->u.int32 <= 0x7fff)
				i16 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32 <= 0x7fff)
				i16 = variable->u.uint32;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= (-0x7fff-1)
					&& variable->u.int64 <= 0x7fff)
				i16 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0x7fff)
				i16 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			p = &i16;
			break;
		case VT_UINT16:
			size = sizeof(u16);
			if(variable->type == VT_INT16 && variable->u.int16 >= 0)
				u16 = variable->u.int16;
			else if(variable->type == VT_INT8
					&& variable->u.int8 >= 0)
				u16 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				u16 = variable->u.uint8;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= 0
					&& variable->u.int32 <= 0xffff)
				u16 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32 <= 0xffff)
				u16 = variable->u.uint32;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= 0
					&& variable->u.int64 <= 0xffff)
				u16 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0xffff)
				u16 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			p = &u16;
			break;
		case VT_INT32:
			size = sizeof(i32);
			if(variable->type == VT_UINT32
					&& variable->u.uint32 <= 0x7fffffff)
				i32 = variable->u.uint32;
			else if(variable->type == VT_INT8)
				i32 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				i32 = variable->u.uint8;
			else if(variable->type == VT_INT16)
				i32 = variable->u.int16;
			else if(variable->type == VT_UINT16)
				i32 = variable->u.uint16;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= (-0x7fffffff-1)
					&& variable->u.int64 <= 0x7fffffff)
				i32 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0x7fffffff)
				i32 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			p = &i32;
			break;
		case VT_UINT32:
			size = sizeof(u32);
			if(variable->type == VT_INT32
					&& variable->u.int32 >= 0)
				u32 = variable->u.int32;
			else if(variable->type == VT_INT8
					&& variable->u.int8 >= 0)
				u32 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				u32 = variable->u.uint8;
			else if(variable->type == VT_INT16
					&& variable->u.int16 >= 0)
				u32 = variable->u.int16;
			else if(variable->type == VT_UINT16)
				u32 = variable->u.uint16;
			else if(variable->type == VT_INT64
					&& variable->u.int64 >= 0
					&& variable->u.int64 <= 0xffffffff)
				u32 = variable->u.int64;
			else if(variable->type == VT_UINT64
					&& variable->u.uint64 <= 0xffffffff)
				u32 = variable->u.uint64;
			else
				/* TODO implement more conversions */
				break;
			size = sizeof(u32);
			p = &u32;
			break;
		case VT_INT64:
			size = sizeof(i64);
			if(variable->type == VT_UINT64
					&& variable->u.int64 <= 0x7fffffffffffffff)
				i64 = variable->u.int64;
			else
				/* TODO implement more conversions */
				break;
			p = &i64;
			break;
		case VT_UINT64:
			size = sizeof(u64);
			if(variable->type == VT_INT64
					&& variable->u.int64 >= 0)
				u64 = variable->u.int64;
			else
				/* TODO implement more conversions */
				break;
			p = &u64;
			break;
		case VT_FLOAT:
			fp = (float *)result;
			if(variable->type == VT_DOUBLE)
				*fp = variable->u.d;
			else
				/* TODO implement more conversions */
				break;
			return 0;
		case VT_DOUBLE:
			dp = (double *)result;
			if(variable->type == VT_FLOAT)
				*dp = variable->u.f;
			else
				/* TODO implement more conversions */
				break;
			return 0;
		case VT_BUFFER:
			/* TODO serialize the variable */
			break;
		case VT_STRING:
			return _get_as_convert_string(variable,
					(String **)result);
	}
	if(size != 0 && p != NULL)
	{
		memcpy(result, p, size);
		return 0;
	}
	return -error_set_code(1, "Unable to convert from type %u to %u",
			variable->type, type);
}

static VariableError _get_as_convert_string(Variable * variable,
		String ** result)
{
	Variable * v = variable;

	switch(v->type)
	{
		case VT_NULL:
			*result = NULL;
			return 0;
		case VT_BOOL:
			return ((*result = string_new(v->u.b
							? "true" : "false"))
					!= NULL) ? 0 : -1;
		case VT_INT8:
			return ((*result = string_new_format("%" PRId8,
							v->u.int8)) != NULL)
				? 0 : -1;
		case VT_UINT8:
			return ((*result = string_new_format("%" PRIu8,
							v->u.uint8)) != NULL)
				? 0 : -1;
		case VT_INT16:
			return ((*result = string_new_format("%" PRId16,
							v->u.int16)) != NULL)
				? 0 : -1;
		case VT_UINT16:
			return ((*result = string_new_format("%" PRIu16,
							v->u.uint16)) != NULL)
				? 0 : -1;
		case VT_INT32:
			return ((*result = string_new_format("%" PRId32,
							v->u.int32)) != NULL)
				? 0 : -1;
		case VT_UINT32:
			return ((*result = string_new_format("%" PRIu32,
							v->u.uint32)) != NULL)
				? 0 : -1;
		case VT_INT64:
			return ((*result = string_new_format("%" PRId64,
							v->u.int64)) != NULL)
				? 0 : -1;
		case VT_UINT64:
			return ((*result = string_new_format("%" PRIu64,
							v->u.uint64)) != NULL)
				? 0 : -1;
		case VT_FLOAT:
			return ((*result = string_new_format("%f", v->u.f))
					!= NULL) ? 0 : -1;
		case VT_DOUBLE:
			return ((*result = string_new_format("%f", v->u.d))
					!= NULL) ? 0 : -1;
		case VT_STRING:
			/* cannot happen */
		case VT_COMPOUND:
			/* no possible conversion */
			break;
#if 0
		case VT_ARRAY:
		case VT_BUFFER:
			/* FIXME implement */
			break;
#endif
		case VT_POINTER:
			return ((*result = string_new_format("%p",
							v->u.pointer)) != NULL)
				? 0 : -1;
	}
	return -error_set_code(1, "Unable to convert from type %u to %u",
			variable->type, VT_STRING);
}


/* variable_get_pointer */
void * variable_get_pointer(Variable * variable)
{
	switch(variable->type)
	{
		case VT_NULL:
			return NULL;
		case VT_BOOL:
			return &variable->u.b;
		case VT_INT8:
			return &variable->u.int8;
		case VT_UINT8:
			return &variable->u.uint8;
		case VT_INT16:
			return &variable->u.int16;
		case VT_UINT16:
			return &variable->u.uint16;
		case VT_INT32:
			return &variable->u.int32;
		case VT_UINT32:
			return &variable->u.uint32;
		case VT_INT64:
			return &variable->u.int64;
		case VT_UINT64:
			return &variable->u.uint64;
		case VT_FLOAT:
			return &variable->u.f;
		case VT_DOUBLE:
			return &variable->u.d;
		case VT_BUFFER:
			return variable->u.buffer;
		case VT_STRING:
			/* XXX because of String operations */
			return &variable->u.string;
		case VT_ARRAY:
			return variable->u.array.array;
		case VT_COMPOUND:
			return NULL;
		case VT_POINTER:
			return &variable->u.pointer;
	}
	return NULL;
}


/* variable_get_type */
VariableType variable_get_type(Variable * variable)
{
	return variable->type;
}


/* variable_set */
VariableError variable_set(Variable * variable, ...)
{
	VariableError ret;
	va_list ap;

	va_start(ap, variable);
	ret = variable_setv(variable, ap);
	va_end(ap);
	return ret;
}


/* variable_setv */
VariableError variable_setv(Variable * variable, va_list ap)
{
	return variable_set_typev(variable, variable->type, ap);
}


/* variable_set_type */
VariableError variable_set_type(Variable * variable, VariableType type, ...)
{
	VariableError ret;
	va_list ap;

	va_start(ap, type);
	ret = variable_set_typev(variable, type, ap);
	va_end(ap);
	return ret;
}


/* variable_set_typev */
VariableError variable_set_typev(Variable * variable, VariableType type,
		va_list ap)
{
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	double d;
	Buffer * b;
	String * s;

	switch(type)
	{
		case VT_NULL:
			_variable_destroy(variable);
			break;
		case VT_BOOL:
			_variable_destroy(variable);
			u32 = va_arg(ap, uint32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%s)\n", __func__,
					u32 ? "true" : "false");
#endif
			variable->u.b = u32 ? true : false;
			break;
		case VT_INT8:
			_variable_destroy(variable);
			i32 = va_arg(ap, int32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__, i32);
#endif
			variable->u.int8 = i32;
			break;
		case VT_UINT8:
			_variable_destroy(variable);
			u32 = va_arg(ap, uint32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__, u32);
#endif
			variable->u.uint8 = u32;
			break;
		case VT_INT16:
			_variable_destroy(variable);
			i32 = va_arg(ap, int32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__, i32);
#endif
			variable->u.int16 = i32;
			break;
		case VT_UINT16:
			_variable_destroy(variable);
			u32 = va_arg(ap, uint32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__, u32);
#endif
			variable->u.uint16 = u32;
			break;
		case VT_INT32:
			_variable_destroy(variable);
			i32 = va_arg(ap, int32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__, i32);
#endif
			variable->u.int32 = i32;
			break;
		case VT_UINT32:
			_variable_destroy(variable);
			u32 = va_arg(ap, uint32_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__, u32);
#endif
			variable->u.uint32 = u32;
			break;
		case VT_INT64:
			_variable_destroy(variable);
			i64 = va_arg(ap, int64_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%ld)\n", __func__, i64);
#endif
			variable->u.int64 = i64;
			break;
		case VT_UINT64:
			_variable_destroy(variable);
			u64 = va_arg(ap, uint64_t);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%lu)\n", __func__, u64);
#endif
			variable->u.uint64 = u64;
			break;
		case VT_FLOAT:
			_variable_destroy(variable);
			d = va_arg(ap, double);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%f)\n", __func__, d);
#endif
			variable->u.f = d;
			break;
		case VT_DOUBLE:
			_variable_destroy(variable);
			d = va_arg(ap, double);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%lf)\n", __func__, d);
#endif
			variable->u.d = d;
			break;
		case VT_BUFFER:
			b = va_arg(ap, Buffer *);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%p)\n", __func__, (void *)b);
#endif
			if((b = buffer_new_copy(b)) == NULL)
				return -1;
			_variable_destroy(variable);
			variable->u.buffer = b;
			break;
		case VT_STRING:
			s = va_arg(ap, String *);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, s);
#endif
			if((s = string_new(s)) == NULL)
				return -1;
			_variable_destroy(variable);
			variable->u.string = s;
			break;
		case VT_POINTER:
			_variable_destroy(variable);
			variable->u.pointer = va_arg(ap, void *);
			break;
		default:
			return error_set_code(-ENOSYS, "%s", strerror(ENOSYS));
	}
	variable->type = type;
	return 0;
}


/* useful */
/* variable_copy */
static VariableError _copy_compound(Variable * variable, Variable const * from);

VariableError variable_copy(Variable * variable, Variable const * from)
{
	Buffer * b;
	String * s;
	Array * a;

	switch(from->type)
	{
		case VT_NULL:
		case VT_BOOL:
		case VT_INT8:
		case VT_UINT8:
		case VT_INT16:
		case VT_UINT16:
		case VT_INT32:
		case VT_UINT32:
		case VT_INT64:
		case VT_UINT64:
		case VT_FLOAT:
		case VT_DOUBLE:
		case VT_POINTER:
			_variable_destroy(variable);
			memcpy(&variable->u, &from->u, sizeof(from->u));
			break;
		case VT_BUFFER:
			if((b = buffer_new_copy(from->u.buffer)) == NULL)
				return -1;
			_variable_destroy(variable);
			variable->u.buffer = b;
			break;
		case VT_STRING:
			if((s = string_new(from->u.string)) == NULL)
				return -1;
			_variable_destroy(variable);
			variable->u.string = s;
			break;
		case VT_ARRAY:
			if((a = array_new_copy(from->u.array.array)) == NULL)
				return -1;
			_variable_destroy(variable);
			variable->u.array.type = from->u.array.type;
			variable->u.array.array = a;
			break;
		case VT_COMPOUND:
			return _copy_compound(variable, from);
		default:
			/* TODO implement */
			return error_set_code(-ENOSYS, "%s", strerror(ENOSYS));
	}
	variable->type = from->type;
	return 0;
}

static VariableError _copy_compound(Variable * variable, Variable const * from)
{
	String * s;
	Array * members;
	size_t count;
	size_t i;
	Variable * v;

	if(from->u.compound.name == NULL)
		s = NULL;
	else if((s = string_new(from->u.compound.name)) == NULL)
		return -1;
	if((members = array_new(array_get_size(from->u.compound.members)))
			== NULL)
	{
		string_delete(s);
		return -1;
	}
	count = array_count(from->u.compound.members);
	for(i = 0; i < count; i++)
		if((v = (Variable *)array_get(from->u.compound.members, i))
				== NULL
				|| (v = variable_new_copy(v)) == NULL
				|| array_set(members, i, v) != 0)
			break;
	if(i != count)
	{
		for(count = i, i = 0; i < count; i++)
			if((v = (Variable *)array_get(members, i)) != NULL)
				variable_delete(v);
		array_delete(members);
		string_delete(s);
		return -1;
	}
	_variable_destroy(variable);
	variable->u.compound.name = s;
	variable->u.compound.members = members;
	return 0;
}


/* variable_serialize */
VariableError variable_serialize(Variable * variable, Buffer * buffer,
		bool prefix)
{
	size_t size = 0;
	size_t offset;
	void * p;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	int64_t i64;
	uint32_t u32;
	uint64_t u64;
	char buf[16];

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u)\n", __func__, variable->type);
#endif
	switch(variable->type)
	{
		case VT_NULL:
			p = NULL;
			break;
		case VT_BOOL:
		case VT_INT8:
		case VT_UINT8:
			size = sizeof(variable->u.int8);
			p = &variable->u.int8;
			break;
		case VT_INT16:
			size = sizeof(i16);
			i16 = _bswap16(variable->u.int16);
			p = &i16;
			break;
		case VT_UINT16:
			size = sizeof(u16);
			u16 = _bswap16(variable->u.uint16);
			p = &u16;
			break;
		case VT_INT32:
			size = sizeof(i32);
			i32 = _bswap32(variable->u.int32);
			p = &i32;
			break;
		case VT_UINT32:
			size = sizeof(u32);
			u32 = _bswap32(variable->u.uint32);
			p = &u32;
			break;
		case VT_INT64:
			size = sizeof(i64);
			i64 = _bswap64(variable->u.int64);
			p = &i64;
			break;
		case VT_UINT64:
			size = sizeof(u64);
			u64 = _bswap64(variable->u.uint64);
			p = &u64;
			break;
		case VT_FLOAT:
			size = snprintf(buf, sizeof(buf), "%.e", variable->u.f);
			p = buf;
			break;
		case VT_DOUBLE:
			size = snprintf(buf, sizeof(buf), "%.e", variable->u.d);
			p = buf;
			break;
		case VT_BUFFER:
			size = buffer_get_size(variable->u.buffer);
			u32 = buffer_get_size(variable->u.buffer);
			u32 = _bswap32(u32);
			p = (void *)buffer_get_data(variable->u.buffer);
			break;
		case VT_STRING:
			size = string_get_length(variable->u.string) + 1;
			p = variable->u.string;
			break;
		case VT_POINTER:
			size = sizeof(u64);
			u64 = (uint64_t)variable->u.pointer;
			u64 = _bswap64(u64);
			p = &u64;
			break;
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %lu\n", __func__, size);
#endif
	if(size == 0 && variable->type != VT_NULL)
		return -error_set_code(1, "Unable to serialize type %u",
				variable->type);
	if(prefix)
	{
		/* prefix with the type */
		u8 = variable->type;
		if(buffer_set(buffer, sizeof(u8), (char const *)&u8) != 0)
			return -1;
		offset = sizeof(u8);
		if(variable->type == VT_BUFFER)
		{
			if(buffer_set_data(buffer, offset, (char const *)&u32,
						sizeof(u32)) != 0)
				return -1;
			offset += sizeof(u32);
		}
		return buffer_set_data(buffer, offset, (char const *)p, size);
	}
	if(variable->type == VT_BUFFER)
	{
		if(buffer_set(buffer, sizeof(u32), (char const *)&u32) != 0)
			return -1;
		return buffer_set_data(buffer, sizeof(u32), (char const *)p,
				size);
	}
	return buffer_set(buffer, size, (char const *)p);
}


/* private */
/* bswap16 */
static uint16_t _bswap16(uint16_t u)
{
	return ((u & 0xff) << 8) | ((u & 0xff00) >> 8);
}


/* bswap32 */
static uint32_t _bswap32(uint32_t u)
{
	return ((u & 0xff) << 24) | ((u & 0xff00) << 8)
		| ((u & 0xff0000) >> 8) | ((u & 0xff000000) >> 24);
}


/* bswap64 */
static uint64_t _bswap64(uint64_t u)
{
	return ((u & 0xff) << 56) | ((u & 0xff00) << 40)
		| ((u & 0xff0000) << 24) | ((u & 0xff000000) << 8)
		| ((u & 0xff00000000) >> 8) | ((u & 0xff0000000000) >> 24)
		| ((u & 0xff000000000000) >> 40)
		| ((u & 0xff00000000000000) >> 56);
}


/* variable_destroy */
static void _destroy_compound(Variable * variable);

static void _variable_destroy(Variable * variable)
{
	switch(variable->type)
	{
		case VT_NULL:
		case VT_BOOL:
		case VT_INT8:
		case VT_UINT8:
		case VT_INT16:
		case VT_UINT16:
		case VT_INT32:
		case VT_UINT32:
		case VT_INT64:
		case VT_UINT64:
		case VT_FLOAT:
		case VT_DOUBLE:
		case VT_POINTER:
			break;
		case VT_BUFFER:
			buffer_delete(variable->u.buffer);
			break;
		case VT_STRING:
			string_delete(variable->u.string);
			break;
		case VT_ARRAY:
			array_delete(variable->u.array.array);
			break;
		case VT_COMPOUND:
			_destroy_compound(variable);
			break;
	}
}

static void _destroy_compound(Variable * variable)
{
	size_t count;
	size_t i;
	Variable * v;

	string_delete(variable->u.compound.name);
	count = array_count(variable->u.compound.members);
	for(i = 0; i < count; i++)
	{
		v = (Variable *)array_get(variable->u.compound.members, i);
		variable_delete(v);
	}
	array_delete(variable->u.compound.members);
}
