/* $Id$ */
/* Copyright (c) 2014-2017 Pierre Pronchery <khorben@defora.org> */
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



#include <stddef.h>
#include "System/error.h"
#include "System/hash.h"
#include "System/mutator.h"
#include "System/object.h"


/* Mutator */
/* public */
/* functions */
/* mutator_new */
Mutator * mutator_new(void)
{
	return hash_new(hash_func_string, hash_compare_string);
}


/* mutator_delete */
void mutator_delete(Mutator * mutator)
{
	mutator_reset(mutator);
	hash_delete(mutator);
}


/* accessors */
/* mutator_count */
size_t mutator_count(Mutator const * mutator)
{
	return hash_count(mutator);
}


/* mutator_get */
void * mutator_get(Mutator const * mutator, String const * key)
{
	void * ret;

	if((ret = hash_get(mutator, key)) == NULL)
		error_set("%s: %s", key, "Key not found");
	return ret;
}


/* mutator_set */
int mutator_set(Mutator * mutator, String const * key, void * value)
{
	int ret;
	String * k;
	String * oldk;

	/* look for the former key */
	if((oldk = hash_get_key(mutator, key)) == NULL)
	{
		if(value == NULL)
			/* there is nothing to do */
			return 0;
		/* allocate the new key */
		if((k = string_new(key)) == NULL)
			return -1;
		key = k;
	}
	else
	{
		if(value != NULL)
			oldk = NULL;
		k = NULL;
	}
	if((ret = hash_set(mutator, key, value)) != 0)
	{
		error_set("%s: %s", key, "Could not set the value");
		string_delete(k);
	}
	else
		/* free the former key if removed */
		string_delete(oldk);
	return ret;
}


/* useful */
void mutator_foreach(Mutator const * mutator, MutatorForeach func, void * data)
{
	hash_foreach(mutator, (HashForeach)func, data);
}


/* mutator_reset */
static void _reset_foreach(String const * key, void * value, void * data);

int mutator_reset(Mutator * mutator)
{
	/* free the keys */
	mutator_foreach(mutator, _reset_foreach, NULL);
	return hash_reset(mutator);
}

static void _reset_foreach(String const * key, void * value, void * data)
{
	String * k = (String *)key;
	(void) value;
	(void) data;

	string_delete(k);
}
