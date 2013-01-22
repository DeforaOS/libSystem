/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
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



#include <Python.h>
#include "System.h"


/* libSystem */
/* private */
/* constants */
static char const _libsystem_event_name[] = "libSystem::Event";


/* prototypes */
/* Event */
static PyObject * _libsystem_event_new(PyObject * self, PyObject * args);
static void _libsystem_event_delete(PyObject * self);

/* useful */
static PyObject * _libsystem_event_loop(PyObject * self, PyObject * args);


/* variables */
static PyMethodDef _libsystem_methods[] =
{
	{ "event_new", _libsystem_event_new, METH_VARARGS,
		"Instantiates an Event object." },
	{ "event_loop", _libsystem_event_loop, METH_VARARGS,
		"Loops an Event object." },
	{ NULL, NULL, 0, NULL }
};


/* public */
/* prototypes */
PyMODINIT_FUNC init_libSystem(void);


/* functions */
/* initlibsystem */
PyMODINIT_FUNC init_libSystem(void)
{
	Py_InitModule("_libSystem", _libsystem_methods);
}


/* private */
/* functions */
/* Event */
/* libsystem_event_new */
static PyObject * _libsystem_event_new(PyObject * self, PyObject * args)
{
	Event * event;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	if((event = event_new()) == NULL)
		return NULL;
	return PyCapsule_New(event, _libsystem_event_name,
			_libsystem_event_delete);
}


/* libsystem_event_delete */
static void _libsystem_event_delete(PyObject * self)
{
	Event * event;

	if((event = PyCapsule_GetPointer(self, _libsystem_event_name)) == NULL)
		return;
	event_delete(event);
}


/* useful */
/* libsystem_event_loop */
static PyObject * _libsystem_event_loop(PyObject * self, PyObject * args)
{
	int ret;
	Event * event;

	if(!PyArg_ParseTuple(args, "O:event", &event))
		return NULL;
	ret = event_loop(event);
	return Py_BuildValue("i", ret);
}
