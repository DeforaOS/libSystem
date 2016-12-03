/* $Id$ */
/* Copyright (c) 2013-2016 Pierre Pronchery <khorben@defora.org> */
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
static char const _libsystem_config_name[] = "libSystem::Config";
static char const _libsystem_event_name[] = "libSystem::Event";
static char const _libsystem_mutator_name[] = "libSystem::Mutator";
static char const _libsystem_plugin_name[] = "libSystem::Plugin";
static char const _libsystem_plugin_symbol_name[] = "libSystem::Plugin::symbol";
static char const _libsystem_string_name[] = "libSystem::String";


/* prototypes */
/* Config */
static PyObject * _libsystem_config_new(PyObject * self, PyObject * args);
static void _libsystem_config_delete(PyObject * self);

static PyObject * _libsystem_config_get(PyObject * self, PyObject * args);
static PyObject * _libsystem_config_set(PyObject * self, PyObject * args);

static PyObject * _libsystem_config_load(PyObject * self, PyObject * args);
static PyObject * _libsystem_config_reset(PyObject * self, PyObject * args);
static PyObject * _libsystem_config_save(PyObject * self, PyObject * args);

/* Error */
static PyObject * _libsystem_error_get(PyObject * self, PyObject * args);
static PyObject * _libsystem_error_get_code(PyObject * self, PyObject * args);

static PyObject * _libsystem_error_print(PyObject * self, PyObject * args);

/* Event */
static PyObject * _libsystem_event_new(PyObject * self, PyObject * args);
static void _libsystem_event_delete(PyObject * self);

static PyObject * _libsystem_event_loop(PyObject * self, PyObject * args);

/* Mutator */
static PyObject * _libsystem_mutator_new(PyObject * self, PyObject * args);
static void _libsystem_mutator_delete(PyObject * self);

static PyObject * _libsystem_mutator_get(PyObject * self, PyObject * args);

/* Plugin */
static PyObject * _libsystem_plugin_new(PyObject * self, PyObject * args);
static void _libsystem_plugin_delete(PyObject * self);

static PyObject * _libsystem_plugin_lookup(PyObject * self, PyObject * args);

/* String */
static PyObject * _libsystem_string_new(PyObject * self, PyObject * args);

static PyObject * _libsystem_string_get_length(PyObject * self,
		PyObject * args);

static PyObject * _libsystem_string_find(PyObject * self, PyObject * args);
static PyObject * _libsystem_string_index(PyObject * self, PyObject * args);
static PyObject * _libsystem_string_rindex(PyObject * self, PyObject * args);


/* variables */
static PyMethodDef _libsystem_methods[] =
{
	{ "config_new", _libsystem_config_new, METH_VARARGS,
		"Instantiates a Config object." },
	{ "config_get", _libsystem_config_get, METH_VARARGS,
		"Obtains a value from the Config object." },
	{ "config_set", _libsystem_config_set, METH_VARARGS,
		"Sets a value in the Config object." },
	{ "config_load", _libsystem_config_load, METH_VARARGS,
		"Load values in the Config object from a file." },
	{ "config_reset", _libsystem_config_reset, METH_VARARGS,
		"Reset the Config object." },
	{ "config_save", _libsystem_config_save, METH_VARARGS,
		"Save values in the Config object to a file." },
	{ "error_get", _libsystem_error_get, METH_VARARGS,
		"Get the last error." },
	{ "error_get_code", _libsystem_error_get_code, METH_VARARGS,
		"Get the code for the last error." },
	{ "error_print", _libsystem_error_print, METH_VARARGS,
		"Print the last error message." },
	{ "event_new", _libsystem_event_new, METH_VARARGS,
		"Instantiates an Event object." },
	{ "event_loop", _libsystem_event_loop, METH_VARARGS,
		"Loops an Event object." },
	{ "mutator_new", _libsystem_mutator_new, METH_VARARGS,
		"Instantiates a Mutator object." },
	{ "mutator_get", _libsystem_mutator_get, METH_VARARGS,
		"Get the corresponding value for the key." },
	{ "plugin_new", _libsystem_plugin_new, METH_VARARGS,
		"Opens a plug-in (or the current process)." },
	{ "plugin_lookup", _libsystem_plugin_lookup, METH_VARARGS,
		"Looks for a particular symbol with the Plugin object." },
	{ "string_new", _libsystem_string_new, METH_VARARGS,
		"Instantiates a String object." },
	{ "string_get_length", _libsystem_string_get_length, METH_VARARGS,
		"Returns the length of a string." },
	{ "string_find", _libsystem_string_find, METH_VARARGS,
		"Looks for the first occurence of a sub-string." },
	{ "string_index", _libsystem_string_index, METH_VARARGS,
		"Looks for the first occurence of a sub-string." },
	{ "string_rindex", _libsystem_string_rindex, METH_VARARGS,
		"Looks for the last occurence of a sub-string." },
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
/* Config */
/* libsystem_config_new */
static PyObject * _libsystem_config_new(PyObject * self, PyObject * args)
{
	Config * config;
	(void) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	if((config = config_new()) == NULL)
		return NULL;
	return PyCapsule_New(config, _libsystem_config_name,
			_libsystem_config_delete);
}


/* libsystem_config_delete */
static void _libsystem_config_delete(PyObject * self)
{
	Config * config;

	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return;
	config_delete(config);
}


/* libsystem_config_get */
static PyObject * _libsystem_config_get(PyObject * self, PyObject * args)
{
	Config * config;
	char const * ret;
	char const * section;
	char const * variable;

	if(!PyArg_ParseTuple(args, "Oss", &self, &section, &variable))
		return NULL;
	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return NULL;
	ret = config_get(config, section, variable);
	return Py_BuildValue("s", ret);
}


/* libsystem_config_set */
static PyObject * _libsystem_config_set(PyObject * self, PyObject * args)
{
	Config * config;
	int ret;
	char const * section;
	char const * variable;
	char const * value;

	if(!PyArg_ParseTuple(args, "Osss", &self, &section, &variable, &value))
		return NULL;
	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return NULL;
	ret = config_set(config, section, variable, value);
	return Py_BuildValue("i", ret);
}


/* libsystem_config_load */
static PyObject * _libsystem_config_load(PyObject * self, PyObject * args)
{
	Config * config;
	int ret;
	char const * filename;

	if(!PyArg_ParseTuple(args, "Os", &self, &filename))
		return NULL;
	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return NULL;
	ret = config_load(config, filename);
	return Py_BuildValue("i", ret);
}


/* libsystem_config_reset */
static PyObject * _libsystem_config_reset(PyObject * self, PyObject * args)
{
	Config * config;
	int ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return NULL;
	ret = config_reset(config);
	return Py_BuildValue("i", ret);
}


/* libsystem_config_save */
static PyObject * _libsystem_config_save(PyObject * self, PyObject * args)
{
	Config * config;
	int ret;
	char const * filename;

	if(!PyArg_ParseTuple(args, "Os", &self, &filename))
		return NULL;
	if((config = PyCapsule_GetPointer(self, _libsystem_config_name))
			== NULL)
		return NULL;
	ret = config_save(config, filename);
	return Py_BuildValue("i", ret);
}


/* Error */
/* libsystem_error_get */
static PyObject * _libsystem_error_get(PyObject * self, PyObject * args)
{
	char const * ret;
	(void) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	ret = error_get(NULL);
	return Py_BuildValue("s", ret);
}


/* libsystem_error_get_code */
static PyObject * _libsystem_error_get_code(PyObject * self, PyObject * args)
{
	(void) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	return Py_BuildValue("i", error_get_code());
}


/* libsystem_error_print */
static PyObject * _libsystem_error_print(PyObject * self, PyObject * args)
{
	int ret;
	char const * program;
	(void) self;

	if(!PyArg_ParseTuple(args, "s", &program))
		return NULL;
	ret = error_print(program);
	return Py_BuildValue("i", ret);
}


/* Event */
/* libsystem_event_new */
static PyObject * _libsystem_event_new(PyObject * self, PyObject * args)
{
	Event * event;
	(void) self;

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

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((event = PyCapsule_GetPointer(self, _libsystem_event_name)) == NULL)
		return NULL;
	ret = event_loop(event);
	return Py_BuildValue("i", ret);
}


/* Mutator */
/* libsystem_mutator_new */
static PyObject * _libsystem_mutator_new(PyObject * self, PyObject * args)
{
	Mutator * mutator;
	(void) self;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	if((mutator = mutator_new()) == NULL)
		return NULL;
	return PyCapsule_New(mutator, _libsystem_mutator_name,
			_libsystem_mutator_delete);
}


/* libsystem_mutator_delete */
static void _libsystem_mutator_delete(PyObject * self)
{
	Mutator * mutator;

	if((mutator = PyCapsule_GetPointer(self, _libsystem_mutator_name))
			== NULL)
		return;
	mutator_delete(mutator);
}


/* libsystem_mutator_get */
static PyObject * _libsystem_mutator_get(PyObject * self, PyObject * args)
{
	char const * ret;
	Mutator * mutator;
	char const * key;

	if((mutator = PyCapsule_GetPointer(self, _libsystem_mutator_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;
	ret = mutator_get(mutator, key);
	return Py_BuildValue("s", ret);
}


/* Plugin */
/* libsystem_plugin_new */
static PyObject * _libsystem_plugin_new(PyObject * self, PyObject * args)
{
	Plugin * plugin;
	char const * libdir;
	char const * package;
	char const * type;
	char const * name;
	(void) self;

	if(PyArg_ParseTuple(args, ""))
		plugin = plugin_new_self();
	else if(PyArg_ParseTuple(args, "ssss", &libdir, &package, &type, &name))
		plugin = plugin_new(libdir, package, type, name);
	else
		plugin = NULL;
	if(plugin == NULL)
		return NULL;
	return PyCapsule_New(plugin, _libsystem_plugin_name,
			_libsystem_plugin_delete);
}


/* libsystem_plugin_delete */
static void _libsystem_plugin_delete(PyObject * self)
{
	Plugin * plugin;

	if((plugin = PyCapsule_GetPointer(self, _libsystem_plugin_name))
			== NULL)
		return;
	plugin_delete(plugin);
}


/* libsystem_plugin_lookup */
static PyObject * _libsystem_plugin_lookup(PyObject * self, PyObject * args)
{
	Plugin * plugin;
	char const * symbol;
	void * ret;

	if(!PyArg_ParseTuple(args, "Os", &self, &symbol))
		return NULL;
	if((plugin = PyCapsule_GetPointer(self, _libsystem_plugin_name))
			== NULL)
		return NULL;
	if((ret = plugin_lookup(plugin, symbol)) == NULL)
		/* XXX differentiate from a real error */
		return NULL;
	return PyCapsule_New(ret, _libsystem_plugin_symbol_name, NULL);
}


/* libsystem_string_new */
static PyObject * _libsystem_string_new(PyObject * self, PyObject * args)
{
	String * string;
	char const * p;
	(void) self;

	if(PyArg_ParseTuple(args, ""))
	{
		if((string = string_new("")) == NULL)
			return NULL;
	}
	else if(!PyArg_ParseTuple(args, "s", &p))
		return NULL;
	else if((string = string_new(p)) == NULL)
		return NULL;
	return PyCapsule_New(string, _libsystem_string_name, NULL);
}


/* libsystem_string_get_length */
static PyObject * _libsystem_string_get_length(PyObject * self, PyObject * args)
{
	int ret;
	String * string;

	if(!PyArg_ParseTuple(args, ""))
		return NULL;
	if((string = PyCapsule_GetPointer(self, _libsystem_string_name))
			== NULL)
		return NULL;
	ret = string_get_length(string);
	return Py_BuildValue("i", ret);
}


/* libsystem_string_find */
static PyObject * _libsystem_string_find(PyObject * self, PyObject * args)
{
	char const * ret;
	String * string;
	char const * key;

	if((string = PyCapsule_GetPointer(self, _libsystem_string_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;
	ret = string_find(string, key);
	return Py_BuildValue("s", ret);
}


/* libsystem_string_index */
static PyObject * _libsystem_string_index(PyObject * self, PyObject * args)
{
	int ret;
	String * string;
	char const * key;

	if((string = PyCapsule_GetPointer(self, _libsystem_string_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;
	ret = string_index(string, key);
	return Py_BuildValue("i", ret);
}


/* libsystem_string_rindex */
static PyObject * _libsystem_string_rindex(PyObject * self, PyObject * args)
{
	int ret;
	String * string;
	char const * key;

	if((string = PyCapsule_GetPointer(self, _libsystem_string_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, "s", &key))
		return NULL;
	ret = string_rindex(string, key);
	return Py_BuildValue("i", ret);
}
