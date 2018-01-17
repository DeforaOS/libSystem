/* $Id$ */
/* Copyright (c) 2012-2018 Pierre Pronchery <khorben@defora.org> */
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
	} u;
};


/* constants */
#define VT_LAST VT_STRING
#define VT_COUNT (VT_LAST + 1)
static const size_t _variable_sizes[VT_COUNT] = { 0, 1,
	sizeof(int8_t), sizeof(uint8_t), sizeof(int16_t), sizeof(uint16_t),
	sizeof(int32_t), sizeof(uint32_t), sizeof(int64_t), sizeof(uint64_t),
	0, 0, sizeof(uint32_t), 0 };


/* prototypes */
static uint16_t _bswap16(uint16_t u);
static uint32_t _bswap32(uint32_t u);

static void _variable_destroy(Variable * variable);


/* public */
/* variable_new */
Variable * variable_new(VariableType type, void const * value)
{
	Variable * variable;

	if((variable = object_new(sizeof(*variable))) == NULL)
		return NULL;
	variable->type = VT_NULL;
	if(variable_set_from(variable, type, value) != 0)
	{
		object_delete(variable);
		return NULL;
	}
	return variable;
}


/* variable_new_array */
Variable * variable_new_array(VariableType type, size_t size, ...)
{
	/* FIXME implement */
	return NULL;
}


/* variable_new_arrayv */
Variable * variable_new_arrayv(VariableType type, size_t size, void ** values)
{
	/* FIXME implement */
	return NULL;
}


/* variable_new_compound */
Variable * variable_new_compound(String const * name, size_t members, ...)
{
	/* FIXME implement */
	return NULL;
}


/* variable_new_compoundv */
Variable * variable_new_compoundv(String const * name, size_t members,
		VariableType * types, void ** values)
{
	/* FIXME implement */
	return NULL;
}


/* variable_new_copy */
Variable * variable_new_copy(Variable * variable)
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
			return variable_new(variable->type, &variable->u);
		case VT_STRING:
			return variable_new(variable->type, variable->u.string);
		case VT_BUFFER:
			return variable_new(variable->type, variable->u.buffer);
	}
	error_set_code(1, "%s", "Unable to copy this type of variable");
	return NULL;
}


/* variable_new_deserialize */
Variable * variable_new_deserialize(size_t * size, char const * data)
{
	Variable * variable;
	uint8_t u8;
	size_t s;

	/* check the arguments */
	if(size == NULL || *size < sizeof(u8) || data == NULL)
	{
		error_set_code(-EINVAL, "%s", strerror(EINVAL));
		return NULL;
	}
	/* obtain the type from the data */
	u8 = data[0];
	s = *size - sizeof(u8);
	/* deserialize according to the type */
	variable = variable_new_deserialize_type(u8, &s, &data[sizeof(u8)]);
	*size = s + sizeof(u8);
	return variable;
}


/* variable_new_deserialize_buffer */
Variable * variable_new_deserialize_buffer(size_t * size, Buffer const * buffer)
{
	char const * data;

	data = buffer_get_data(buffer);
	return variable_new_deserialize(size, data);
}


/* variable_new_deserialize_type */
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data)
{
	Variable * v;
	size_t s;
	uint8_t u8;
	int16_t i16;
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	float f;
	double d;
	Buffer * b = NULL;
	void * p = (char *)data;

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
			break;
		case VT_BOOL:
			p = (char *)&u8;
			break;
		case VT_INT8:
		case VT_UINT8:
			break;
		case VT_INT16:
		case VT_UINT16:
			p = (char *)&i16;
			break;
		case VT_INT32:
		case VT_UINT32:
			p = (char *)&i32;
			break;
		case VT_INT64:
		case VT_UINT64:
			p = (char *)&i64;
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
			for(s = 0; s < *size; s++)
				if(data[s] != '\0')
					continue;
				else if((p = malloc(++s)) == NULL)
				{
					error_set_code(-errno, "%s",
							strerror(errno));
					return NULL;
				}
				else
					break;
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
	if(p != data)
		memcpy(p, data, s);
	/* convert the data if necessary */
	switch(type)
	{
		case VT_NULL:
			break;
		case VT_BOOL:
			if((u8 & 0xfe) != 0x0)
			{
				error_set_code(1, "Invalid boolean value");
				return NULL;
			}
			break;
		case VT_INT8:
		case VT_UINT8:
			break;
		case VT_INT16:
		case VT_UINT16:
			i16 = _bswap16(i16);
			break;
		case VT_INT32:
		case VT_UINT32:
			i32 = _bswap32(i32);
			break;
		case VT_FLOAT:
			i32 = sscanf(p, "%e", &f);
			free(p);
			if(i32 != 1)
			{
				error_set_code(1, "Invalid float value");
				return NULL;
			}
			p = &f;
			break;
		case VT_DOUBLE:
			i32 = sscanf(p, "%le", &d);
			free(p);
			if(i32 != 1)
			{
				error_set_code(1, "Invalid double value");
				return NULL;
			}
			p = &d;
			break;
		case VT_STRING:
			break;
		case VT_INT64:
		case VT_UINT64:
			/* FIXME need to be converted */
			break;
		case VT_BUFFER:
			if((b = buffer_new(s - sizeof(u32), &data[sizeof(u32)]))
					== NULL)
				return NULL;
			p = b;
			break;
		default:
			error_set_code(1, "Unable to deserialize type %u",
					type);
			return NULL;
	}
	v = variable_new(type, p);
	if(b != NULL)
		buffer_delete(b);
	return v;
}


/* variable_delete */
void variable_delete(Variable * variable)
{
	_variable_destroy(variable);
	object_delete(variable);
}


/* variable_get_as */
int variable_get_as(Variable * variable, VariableType type, void * result)
{
	size_t size = 0;
	void * p = NULL;
	int8_t i8;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	Buffer ** b;
	String ** s;

	switch(type)
	{
		case VT_NULL:
			if(variable->type == VT_NULL)
				return 0;
			break;
		case VT_INT8:
			size = sizeof(i8);
			if(variable->type == VT_INT8)
			{
				p = &variable->u.int8;
				break;
			}
			if(variable->type == VT_UINT8
					&& variable->u.uint8 < (1 << 7))
				i8 = variable->u.uint8;
			else if(variable->type == VT_INT16
					&& variable->u.int16 >= -128
					&& variable->u.int16 < (1 << 7))
				i8 = variable->u.int16;
			else if(variable->type == VT_UINT16
					&& variable->u.uint16 < (1 << 7))
				i8 = variable->u.uint16;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= -128
					&& variable->u.int32 < (1 << 7))
				i8 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32 < (1 << 7))
				i8 = variable->u.uint32;
			else
				/* FIXME implement more conversions */
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
					&& variable->u.int16 < (1 << 8))
				u8 = variable->u.int16;
			else if(variable->type == VT_UINT16
					&& variable->u.uint16 < (1 << 8))
				u8 = variable->u.uint16;
			else if(variable->type == VT_INT32
					&& variable->u.int32 >= 0
					&& variable->u.int32 < (1 << 8))
				u8 = variable->u.int32;
			else if(variable->type == VT_UINT32
					&& variable->u.uint32
					< (1 << 8))
				u8 = variable->u.uint32;
			else
				/* FIXME implement more conversions */
				break;
			p = &u8;
			break;
		case VT_INT16:
			size = sizeof(i16);
			if(variable->type == VT_INT16)
			{
				p = &variable->u.int16;
				break;
			}
			if(variable->type == VT_UINT16
					&& variable->u.uint16 < (1 << 15))
				i16 = variable->u.uint16;
			else if(variable->type == VT_INT8)
				i16 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				i16 = variable->u.uint8;
			else
				/* FIXME implement more conversions */
				break;
			p = &i16;
			break;
		case VT_UINT16:
			size = sizeof(u16);
			if(variable->type == VT_UINT16)
			{
				p = &variable->u.uint16;
				break;
			}
			if(variable->type == VT_INT16 && variable->u.int16 >= 0)
				u16 = variable->u.int16;
			else if(variable->type == VT_INT8
					&& variable->u.int8 >= 0)
				u16 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				u16 = variable->u.uint8;
			else
				/* FIXME implement more conversions */
				break;
			p = &u16;
			break;
		case VT_INT32:
			size = sizeof(i32);
			if(variable->type == VT_INT32)
			{
				p = &variable->u.int32;
				break;
			}
			if(variable->type == VT_UINT32
					&& variable->u.uint32
					< (uint32_t)1 << 31)
				i32 = variable->u.uint32;
			else if(variable->type == VT_INT8)
				i32 = variable->u.int8;
			else if(variable->type == VT_UINT8)
				i32 = variable->u.uint8;
			else if(variable->type == VT_INT16)
				i32 = variable->u.int16;
			else if(variable->type == VT_UINT16)
				i32 = variable->u.uint16;
			else
				/* FIXME implement more conversions */
				break;
			p = &i32;
			break;
		case VT_UINT32:
			size = sizeof(u32);
			if(variable->type == VT_UINT32)
			{
				p = &variable->u.uint32;
				break;
			}
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
			else
				/* FIXME implement more conversions */
				break;
			size = sizeof(u32);
			p = &u32;
			break;
		case VT_INT64:
		case VT_UINT64:
			/* FIXME consider signedness */
			if(variable->type == VT_INT64
					|| variable->type == VT_UINT64)
			{
				size = sizeof(variable->u.uint64);
				p = &variable->u.uint64;
				break;
			}
			break;
		case VT_FLOAT:
			if(variable->type == VT_FLOAT)
			{
				size = sizeof(variable->u.f);
				p = &variable->u.f;
				break;
			}
			break;
		case VT_DOUBLE:
			if(variable->type == VT_DOUBLE)
			{
				size = sizeof(variable->u.d);
				p = &variable->u.d;
				break;
			}
			break;
		case VT_BUFFER:
			if(variable->type == VT_BUFFER)
			{
				b = result;
				*b = buffer_new_copy(variable->u.buffer);
				return 0;
			}
			break;
		case VT_STRING:
			if(variable->type == VT_STRING)
			{
				s = result;
				*s = string_new(variable->u.string);
				return 0;
			}
			break;
	}
	if(size != 0 && p != NULL)
	{
		memcpy(result, p, size);
		return 0;
	}
	return -error_set_code(1, "Unable to convert from type %u to %u",
			variable->type, type);
}


/* variable_get_type */
VariableType variable_get_type(Variable * variable)
{
	return variable->type;
}


/* variable_set */
int variable_set(Variable * variable, Variable * from)
{
	Buffer * b;
	String * s;

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
		default:
			/* FIXME implement */
			return -error_set_code(-ENOSYS, "%s", strerror(ENOSYS));
	}
	variable->type = from->type;
	return 0;
}


/* variable_set_from */
int variable_set_from(Variable * variable, VariableType type,
		void const * value)
{
	int8_t const * i8;
	uint8_t const * u8;
	int16_t const * i16;
	uint16_t const * u16;
	int32_t const * i32;
	uint32_t const * u32;
	int64_t const * i64;
	uint64_t const * u64;
	float const * f;
	double const * d;
	Buffer * b;
	char const * s;

	/* XXX keep the previous contents in case of error? */
	_variable_destroy(variable);
	memset(&variable->u, 0, sizeof(variable->u));
	if(value == NULL)
		type = VT_NULL;
	switch((variable->type = type))
	{
		case VT_NULL:
			break;
		case VT_BOOL:
			u8 = value;
			variable->u.uint8 = (*u8 != 0) ? 1 : 0;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__,
					variable->u.uint8);
#endif
			break;
		case VT_INT8:
			i8 = value;
			variable->u.int8 = *i8;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__,
					variable->u.int8);
#endif
			break;
		case VT_UINT8:
			u8 = value;
			variable->u.uint8 = *u8;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__,
					variable->u.uint8);
#endif
			break;
		case VT_INT16:
			i16 = value;
			variable->u.int16 = *i16;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__, *i16);
#endif
			break;
		case VT_UINT16:
			u16 = value;
			variable->u.uint16 = *u16;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__, *u16);
#endif
			break;
		case VT_INT32:
			i32 = value;
			variable->u.int32 = *i32;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%d)\n", __func__, *i32);
#endif
			break;
		case VT_UINT32:
			u32 = value;
			variable->u.uint32 = *u32;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%u)\n", __func__, *u32);
#endif
			break;
		case VT_INT64:
			i64 = value;
			variable->u.int64 = *i64;
			break;
		case VT_UINT64:
			u64 = value;
			variable->u.uint64 = *u64;
			break;
		case VT_FLOAT:
			f = value;
			variable->u.f = *f;
			break;
		case VT_DOUBLE:
			d = value;
			variable->u.d = *d;
			break;
		case VT_BUFFER:
			if((b = buffer_new_copy(value)) == NULL)
				return -1;
			variable->u.buffer = b;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(%p)\n", __func__, (void *)b);
#endif
			break;
		case VT_STRING:
			s = value;
			if((variable->u.string = string_new(s)) == NULL)
				return -1;
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, s);
#endif
			break;
		default:
			return -1;
	}
	return 0;
}


/* useful */
/* variable_serialize */
int variable_serialize(Variable * variable, Buffer * buffer, bool prefix)
{
	size_t size = 0;
	size_t offset;
	void * p;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
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
		case VT_UINT64:
			/* FIXME convert to network endian */
			size = sizeof(variable->u.int64);
			p = &variable->u.int64;
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
			p = buffer_get_data(variable->u.buffer);
			break;
		case VT_STRING:
			size = string_get_length(variable->u.string) + 1;
			p = variable->u.string;
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
		if(buffer_set(buffer, sizeof(u8), (char *)&u8) != 0)
			return -1;
		offset = sizeof(u8);
		if(variable->type == VT_BUFFER)
		{
			if(buffer_set_data(buffer, offset, (char *)&u32,
						sizeof(u32)) != 0)
				return -1;
			offset += sizeof(u32);
		}
		return buffer_set_data(buffer, offset, p, size);
	}
	if(variable->type == VT_BUFFER)
	{
		if(buffer_set(buffer, sizeof(u32), (char *)&u32) != 0)
			return -1;
		return buffer_set_data(buffer, sizeof(u32), p, size);
	}
	return buffer_set(buffer, size, p);
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


/* variable_destroy */
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
			break;
		case VT_BUFFER:
			buffer_delete(variable->u.buffer);
			break;
		case VT_STRING:
			string_delete(variable->u.string);
			break;
	}
}
