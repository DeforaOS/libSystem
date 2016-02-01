/* $Id$ */
/* Copyright (c) 2015-2016 Pierre Pronchery <khorben@defora.org> */
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
#include "System/event.h"


/* event */
static int _event_on_idle(void * data);

static int _event(char const * progname)
{
	int ret;
	Event * event;

	printf("%s: Testing event_new()\n", progname);
	if((event = event_new()) == NULL)
		return -1;
	printf("%s: Testing event_loop()\n", progname);
	event_register_idle(event, _event_on_idle, event);
	ret = event_loop(event);
	event_delete(event);
	return ret;
}

static int _event_on_idle(void * data)
{
	Event * event = data;

	event_loop_quit(event);
	return 0;
}


/* main */
int main(int argc, char * argv[])
{
	(void) argc;

	return (_event(argv[0]) == 0) ? 0 : 2;
}
