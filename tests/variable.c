/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stdio.h>
#include "System/error.h"
#include "System/variable.h"


/* variable */
static int _variable(char const * progname)
{
	int ret = 0;
	Variable * variable;
	const int samples[] = { 0, -1, 1, -127, -128, 126, 127 };
	size_t i;
	int32_t j;
	int8_t i8;
	size_t s;
	void * p;

	/* variable_new */
	for(i = 0; i < sizeof(samples) / sizeof(*samples); i++)
	{
		printf("%s: Testing variable_new_deserialize_type(): %d\n",
				progname, samples[i]);
		i8 = samples[i];
		s = sizeof(i8);
		p = &i8;
		if((variable = variable_new_deserialize_type(VT_INT8, &s, p))
				== NULL)
		{
			error_print(progname);
			ret += 1;
			continue;
		}
		p = &j;
		if(variable_get_as(variable, VT_INT32, p) != 0
				|| j != samples[i])
		{
			error_print(progname);
			ret += 1;
		}
		variable_delete(variable);
	}
	return ret;
}


/* main */
int main(int argc, char * argv[])
{
	return (_variable(argv[0]) == 0) ? 0 : 2;
}
