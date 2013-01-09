/* $Id$ */
/* Copyright (c) 2012-2013 Pierre Pronchery <khorben@defora.org> */
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
#ifdef DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "System.h"


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


/* public */
/* variable_new */
Variable * variable_new(VariableType type, void * value)
{
	Variable * variable;
	int8_t * i8;
	uint8_t * u8;
	int16_t * i16;
	uint16_t * u16;
	int32_t * i32;
	uint32_t * u32;
	int64_t * i64;
	uint64_t * u64;
	float * f;
	double * d;
	Buffer * b;
	char const * s;

	if((variable = object_new(sizeof(*variable))) == NULL)
		return NULL;
	memset(&variable->u, 0, sizeof(variable->u));
	if(value == NULL)
		variable->type = VT_NULL;
	else
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
				fprintf(stderr, "DEBUG: %s(%d)\n", __func__,
						*i16);
#endif
				break;
			case VT_UINT16:
				u16 = value;
				variable->u.uint16 = *u16;
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s(%u)\n", __func__,
						*u16);
#endif
				break;
			case VT_INT32:
				i32 = value;
				variable->u.int32 = *i32;
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s(%d)\n", __func__,
						*i32);
#endif
				break;
			case VT_UINT32:
				u32 = value;
				variable->u.uint32 = *u32;
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s(%u)\n", __func__,
						*u32);
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
				{
					object_delete(variable);
					return NULL;
				}
				variable->u.buffer = b;
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s(%p)\n", __func__,
						(void *)b);
#endif
				break;
			case VT_STRING:
				s = value;
				if((variable->u.string = string_new(s)) == NULL)
				{
					object_delete(variable);
					return NULL;
				}
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__,
						s);
#endif
				break;
		}
	return variable;
}


/* variable_new_copy */
Variable * variable_new_copy(Variable * variable)
{
	return variable_new(variable->type, &variable->u);
}


/* variable_new_deserialize */
Variable * variable_new_deserialize(size_t * size, char const * data)
{
	Variable * variable;
	unsigned char u;
	size_t s;

	/* obtain the type from the data */
	if(*size < sizeof(u))
	{
		*size = 1;
		return NULL;
	}
	u = data[0];
	s = *size - sizeof(u);
	/* deserialize according to the type */
	if((variable = variable_new_deserialize_type(u, &s, &data[sizeof(u)]))
			== NULL)
	{
		*size = s + sizeof(u);
		return NULL;
	}
	return variable;
}


/* variable_new_deserialize_type */
Variable * variable_new_deserialize_type(VariableType type, size_t * size,
		char const * data)
{
	size_t s;
	int16_t i16;
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	void * p = (char *)data;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%u, %lu, %p)\n", __func__, type, *size,
			(void *)data);
#endif
	/* estimate the size required */
	switch(type)
	{
		case VT_NULL:
			s = 0;
			break;
		case VT_INT8:
		case VT_UINT8:
			s = sizeof(int8_t);
			break;
		case VT_INT16:
		case VT_UINT16:
			s = sizeof(int16_t);
			p = (char *)&i16;
			break;
		case VT_INT32:
		case VT_UINT32:
			s = sizeof(int32_t);
			p = (char *)&i32;
			break;
		case VT_INT64:
		case VT_UINT64:
			s = sizeof(int64_t);
			p = (char *)&i64;
			break;
		case VT_BUFFER:
			s = sizeof(uint32_t);
			if(*size < s)
				break;
			memcpy(&u32, data, s);
			u32 = ntohl(u32);
			s += u32;
			break;
		case VT_STRING:
			for(s = 0; s < *size;)
				if(data[s++] != '\0')
					continue;
				else if((p = malloc(s)) == NULL)
				{
					error_set_code(1, "%s",
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
		case VT_INT8:
		case VT_UINT8:
			break;
		case VT_INT16:
		case VT_UINT16:
			i16 = ntohs(i16);
			break;
		case VT_INT32:
		case VT_UINT32:
			i32 = ntohl(i32);
			break;
		case VT_STRING:
			break;
		case VT_INT64:
		case VT_UINT64:
		case VT_BUFFER:
			if((p = buffer_new(s - sizeof(u32), &data[sizeof(u32)]))
					== NULL)
				return NULL;
			break;
		default:
			error_set_code(1, "Unable to deserialize type %u",
					type);
			return NULL;
	}
	return variable_new(type, p);
}


/* variable_delete */
void variable_delete(Variable * variable)
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


/* useful */
/* variable_serialize */
int variable_serialize(Variable * variable, Buffer * buffer, int type)
{
	size_t size = 0;
	size_t offset;
	void * p;
	unsigned char u;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;

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
			i16 = htons(variable->u.int16);
			p = &i16;
			break;
		case VT_UINT16:
			size = sizeof(u16);
			u16 = htons(variable->u.uint16);
			p = &u16;
			break;
		case VT_INT32:
			size = sizeof(i32);
			i32 = htonl(variable->u.int32);
			p = &i32;
			break;
		case VT_UINT32:
			size = sizeof(u32);
			u32 = htonl(variable->u.uint32);
			p = &u32;
			break;
		case VT_INT64:
		case VT_UINT64:
			/* FIXME convert to network endian */
			size = sizeof(variable->u.int64);
			p = &variable->u.int64;
			break;
		case VT_BUFFER:
			size = buffer_get_size(variable->u.buffer);
			u32 = buffer_get_size(variable->u.buffer);
			u32 = htonl(u32);
			p = buffer_get_data(variable->u.buffer);
			break;
		case VT_STRING:
			size = string_get_length(variable->u.string);
			p = variable->u.string;
			break;
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %lu\n", __func__, size);
#endif
	if(size == 0 && variable->type != VT_NULL)
		return -error_set_code(1, "Unable to serialize type %u", type);
	if(type)
	{
		/* prefix with the type */
		u = variable->type;
		if(buffer_set(buffer, sizeof(u), (char *)&u) != 0)
			return -1;
		offset = sizeof(u);
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
