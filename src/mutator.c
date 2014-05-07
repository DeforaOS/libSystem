/* $Id$ */
/* Copyright (c) 2014 Pierre Pronchery <khorben@defora.org> */
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
	hash_delete(mutator);
}


/* accessors */
/* mutator_count */
size_t mutator_count(Mutator * mutator)
{
	return hash_count(mutator);
}


/* mutator_get */
void * mutator_get(Mutator * mutator, String const * key)
{
	return hash_get(mutator, key);
}


/* mutator_set */
int mutator_set(Mutator * mutator, String const * key, void * value)
{
	return hash_set(mutator, key, value);
}


/* useful */
void mutator_foreach(Mutator * mutator, MutatorForeach func, void * data)
{
	hash_foreach(mutator, (HashForeach)func, data);
}


/* mutator_reset */
int mutator_reset(Mutator * mutator)
{
	return hash_reset(mutator);
}
