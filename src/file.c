/* $Id$ */
/* Copyright (c) 2015-2018 Pierre Pronchery <khorben@defora.org> */
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
