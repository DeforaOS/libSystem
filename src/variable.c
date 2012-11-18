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



#include <string.h>
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
			case VT_INT8:
				i8 = value;
				variable->u.int8 = *i8;
				break;
			case VT_UINT8:
				u8 = value;
				variable->u.uint8 = *u8;
				break;
			case VT_INT16:
				i16 = value;
				variable->u.int16 = *i16;
				break;
			case VT_UINT16:
				u16 = value;
				variable->u.uint16 = *u16;
				break;
			case VT_INT32:
				i32 = value;
				variable->u.int32 = *i32;
				break;
			case VT_UINT32:
				u32 = value;
				variable->u.uint32 = *u32;
				break;
			case VT_INT64:
				i64 = value;
				variable->u.int64 = *i64;
				break;
			case VT_UINT64:
				u64 = value;
				variable->u.uint64 = *u64;
				break;
			case VT_STRING:
				s = value;
				if((variable->u.string = string_new(s)) == NULL)
				{
					object_delete(variable);
					return NULL;
				}
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
	/* FIXME implement */
	return NULL;
}


/* variable_delete */
void variable_delete(Variable * variable)
{
	switch(variable->type)
	{
		case VT_NULL:
		case VT_INT8:
		case VT_UINT8:
		case VT_INT16:
		case VT_UINT16:
		case VT_INT32:
		case VT_UINT32:
		case VT_INT64:
		case VT_UINT64:
			break;
		case VT_STRING:
			string_delete(variable->u.string);
			break;
	}
	object_delete(variable);
}


/* useful */
/* variable_serialize */
int variable_serialize(Variable * variable, Buffer * buffer, int type)
{
	size_t size = 0;
	void * p;
	unsigned char u;

	/* FIXME set everything in network endian */
	switch(variable->type)
	{
		case VT_NULL:
			p = NULL;
			break;
		case VT_INT8:
		case VT_UINT8:
			size = sizeof(variable->u.int8);
			p = &variable->u.int8;
			break;
		case VT_INT16:
		case VT_UINT16:
			size = sizeof(variable->u.int16);
			p = &variable->u.int16;
			break;
		case VT_INT32:
		case VT_UINT32:
			size = sizeof(variable->u.int32);
			p = &variable->u.int32;
			break;
		case VT_INT64:
		case VT_UINT64:
			size = sizeof(variable->u.int64);
			p = &variable->u.int64;
			break;
		case VT_STRING:
			size = string_get_length(variable->u.string);
			p = variable->u.string;
			break;
	}
	if(size == 0 && type != VT_NULL)
		return -error_set_code(1, "Unable to serialize type %u", type);
	if(type)
	{
		u = variable->type;
		if(buffer_set(buffer, sizeof(u), (char *)&u) != 0)
			return -1;
		return buffer_set_data(buffer, sizeof(u), p, size);
	}
	else
		return buffer_set(buffer, size, p);
}
