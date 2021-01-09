/* $Id$ */
/* Copyright (c) 2005-2021 Pierre Pronchery <khorben@defora.org> */
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



#include <stdlib.h>
#include <string.h>
#include "System/array.h"
#include "System/error.h"
#include "System/object.h"
#include "System/hash.h"


/* HashEntry */
/* private */
/* types */
typedef struct _HashEntry
{
	unsigned int hash;
	void const * key;
	void * value;
} HashEntry;
ARRAY2(HashEntry, _hashentry)
#define HashEntryArray _hashentryArray


/* prototypes */
static void _hashentry_init(HashEntry * he, HashFunc func, void const * key,
		void * value);

/* accessors */
static int _hashentry_set_value(HashEntry * he, void * value);


/* functions */
/* hashentry_init */
static void _hashentry_init(HashEntry * he, HashFunc func, void const * key,
		void * value)
{
	he->hash = (func != NULL) ? func(key) : 0;
	he->key = key;
	he->value = value;
}


/* accessors */
/* hashentry_set_value */
static int _hashentry_set_value(HashEntry * he, void * value)
{
	he->value = value;
	return 0;
}


/* Hash */
/* protected */
/* types */
struct _Hash
{
	HashFunc func;
	HashCompare compare;
	HashEntryArray * entries;
};


/* public */
/* functions */
/* hash_new */
Hash * hash_new(HashFunc func, HashCompare compare)
{
	Hash * hash;

	if(compare == NULL)
	{
		error_set_code(1, "%s", "Invalid comparison function");
		return NULL;
	}
	if((hash = (Hash *)object_new(sizeof(*hash))) == NULL)
		return NULL;
	if((hash->entries = _hashentryarray_new()) == NULL)
	{
		object_delete(hash);
		return NULL;
	}
	hash->func = func;
	hash->compare = compare;
	return hash;
}


/* hash_new_copy */
Hash * hash_new_copy(Hash const * from)
{
	Hash * hash;

	if((hash = (Hash *)object_new(sizeof(*from))) == NULL)
		return NULL;
	if((hash->entries = array_new_copy(from->entries)) == NULL)
	{
		object_delete(hash);
		return NULL;
	}
	hash->func = from->func;
	hash->compare = from->compare;
	return hash;
}


/* hash_delete */
void hash_delete(Hash * hash)
{
	array_delete(hash->entries);
	object_delete(hash);
}


/* helpers */
/* hash_func_string */
unsigned int hash_func_string(void const * key)
{
	String const * str = (String const *)key;
	size_t i;
	unsigned int hash = 0;

	for(i = 0; i < sizeof(hash) && str[i] != '\0'; i++)
		hash |= str[i] << (i << 3);
	return hash;
}


/* hash_compare_string */
int hash_compare_string(void const * value1, void const * value2)
{
	String const * str1 = (String const *)value1;
	String const * str2 = (String const *)value2;

	return string_compare(str1, str2);
}


/* accessors */
/* hash_count */
size_t hash_count(Hash const * hash)
{
	return array_count((Array const *)hash->entries);
}


/* hash_get */
void * hash_get(Hash const * hash, void const * key)
{
	Array const * entries = (Array const *)hash->entries;
	unsigned int h;
	size_t i;
	HashEntry * he;

	h = (hash->func != NULL) ? hash->func(key) : 0;
	for(i = array_count(entries); i > 0; i--)
	{
		if((he = (HashEntry *)array_get(entries, i - 1)) == NULL)
			return NULL;
		if(he->hash != h)
			continue;
		if(hash->compare(he->key, key) == 0)
			return he->value;
	}
	error_set_code(1, "%s", "Key not found");
	return NULL;
}


/* hash_get_key */
void const * hash_get_key(Hash const * hash, void const * key)
{
	Array const * entries = (Array const *)hash->entries;
	unsigned int h;
	size_t i;
	HashEntry const * he;

	h = (hash->func != NULL) ? hash->func(key) : 0;
	for(i = array_count(entries); i > 0; i--)
	{
		if((he = (HashEntry const *)array_get(entries, i - 1)) == NULL)
			return NULL;
		if(he->hash != h)
			continue;
		if(hash->compare(he->key, key) == 0)
			return he->key;
	}
	error_set_code(1, "%s", "Key not found");
	return NULL;
}


/* hash_set */
int hash_set(Hash * hash, void const * key, void * value)
{
	Array * entries = (Array *)hash->entries;
	unsigned int h;
	size_t i;
	size_t cnt;
	HashEntry he;
	HashEntry * p;

	h = (hash->func != NULL) ? hash->func(key) : 0;
	for(i = 0, cnt = array_count(entries); i < cnt; i++)
	{
		if((p = (HashEntry *)array_get(entries, i)) == NULL)
			return 1;
		if(p->hash != h)
			continue;
		if(hash->compare(p->key, key) != 0)
			continue;
		if(value == NULL)
			return (array_remove_pos(entries, i) == 0) ? 0 : 1;
		return _hashentry_set_value(p, value);
	}
	if(value == NULL)
		return 0;
	_hashentry_init(&he, hash->func, key, value);
	return (array_append(entries, &he) == 0) ? 0 : 1;
}


/* useful */
/* hash_foreach */
static void _hash_foreach(void * value, void * data);

struct funcdata
{
	HashForeach func;
	void * data;
};

void hash_foreach(Hash const * hash, HashForeach func, void * data)
{
	Array const * entries = (Array const *)hash->entries;
	struct funcdata fd;

	fd.func = func;
	fd.data = data;
	array_foreach(entries, _hash_foreach, &fd);
}

static void _hash_foreach(void * value, void * data)
{
	HashEntry * he = (HashEntry *)value;
	struct funcdata * fd = (struct funcdata *)data;

	fd->func(he->key, he->value, fd->data);
}


/* hash_reset */
int hash_reset(Hash * hash)
{
	Array * entries = (Array *)hash->entries;

	while(array_count(entries))
		if(array_remove_pos(entries, 0) != 0)
			return 1;
	return 0;
}
