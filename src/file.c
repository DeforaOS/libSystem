/* $Id$ */
/* Copyright (c) 2015 Pierre Pronchery <khorben@defora.org> */
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



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "System/error.h"
#include "System/object.h"
#include "System/file.h"


/* File */
/* private */
/* types */
struct _File
{
	FILE * fp;
	FileMode mode;
};


/* prototypes */
static String const * _file_get_fmode(FileMode mode);


/* public */
/* functions */
/* config_new */
File * file_new(char const * filename, FileMode mode)
{
	File * file;
	char const * fmode;

	if((fmode = _file_get_fmode(mode)) == NULL)
		return NULL;
	if((file = object_new(sizeof(*file))) == NULL)
		return NULL;
	if((file->fp = fopen(filename, fmode)) == NULL)
	{
		file_delete(file);
		return NULL;
	}
	file->mode = mode;
	return file;
}


/* file_delete */
int file_delete(File * file)
{
	int ret = 0;

	if(file->fp != NULL)
		ret = fclose(file->fp);
	object_delete(file);
	return ret;
}


/* accessors */
/* file_get_mode */
FileMode file_get_mode(File * file)
{
	return file->mode;
}


#if 0
/* file_set_mode */
int file_set_mode(File * file, FileMode mode)
{
	char const * fmode;

	if((fmode = _file_get_fmode(mode)) == NULL)
		return -1;
	if(freopen(file->fp, fmode) != 0)
		return error_set_code(-errno, "%s", strerror(errno));
	return 0;
}
#endif


/* private */
/* accessors */
static String const * _file_get_fmode(FileMode mode)
{
	switch(mode)
	{
		case FILE_MODE_READ:
			return "r";
		case FILE_MODE_WRITE:
			return "w";
	}
	error_set_code(-ENOSYS, "%s", strerror(ENOSYS));
	return NULL;
}
