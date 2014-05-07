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



#ifndef LIBSYSTEM_MUTATOR_H
# define LIBSYSTEM_MUTATOR_H

# include "string.h"


/* Mutator */
/* types */
typedef struct _Hash Mutator;

typedef void (*MutatorForeach)(void const * key, void * value, void * data);


/* functions */
Mutator * mutator_new(void);
void mutator_delete(Mutator * mutator);

/* accessors */
void * mutator_get(Mutator * mutator, String const * key);
int mutator_set(Mutator * mutator, String const * key, void * value);
size_t mutator_count(Mutator * mutator);

/* useful */
void mutator_foreach(Mutator * mutator, MutatorForeach func, void * data);
int mutator_reset(Mutator * mutator);

#endif /* !LIBSYSTEM_MUTATOR_H */
