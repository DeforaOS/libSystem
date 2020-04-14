/* $Id$ */
/* Copyright (c) 2015-2020 Pierre Pronchery <khorben@defora.org> */
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



#include <unistd.h>
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
	String * filename;
	FILE * fp;
	FileMode mode;
};


/* prototypes */
/* accessors */
static String const * _file_get_fmode(File * file, FileMode mode);

/* useful */
static FileError _file_error(File * file, int error);


/* public */
/* functions */
/* config_new */
File * file_new(String const * filename, FileMode mode)
{
	File * file;
	char const * fmode;

	if((file = object_new(sizeof(*file))) == NULL)
		return NULL;
	if((file->filename = string_new(filename)) == NULL
			|| (fmode = _file_get_fmode(file, mode)) == NULL)
	{
		object_delete(file);
		return NULL;
	}
	if((file->fp = fopen(filename, fmode)) == NULL)
	{
		file_delete(file);
		_file_error(file, errno);
		return NULL;
	}
	file->mode = mode;
	return file;
}


/* file_delete */
FileError file_delete(File * file)
{
	FileError ret = 0;

	if(file->fp != NULL
			&& (ret = fclose(file->fp)) != 0)
		ret = _file_error(file, errno);
	string_delete(file->filename);
	object_delete(file);
	return ret;
}


/* accessors */
/* file_get_filename */
String const * file_get_filename(File * file)
{
	return file->filename;
}


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
		return _file_error(file, errno);
	return 0;
}
#endif


/* useful */
/* file_read */
size_t file_read(File * file, void * buf, size_t size, size_t count)
{
	size_t ret;

	ret = fread(buf, size, count, file->fp);
	if(ferror(file->fp))
		_file_error(file, errno);
	return ret;
}


/* file_read_buffer */
size_t file_read_buffer(File * file, Buffer * buffer)
{
	size_t ret;

	ret = file_read(file, buffer_get_data(buffer), sizeof(char),
			buffer_get_size(buffer));
	buffer_set_size(buffer, ret);
	return ret;
}


/* file_seek */
FileError file_seek(File * file, FileSeekMode mode, FileOffset offset)
{
	switch(mode)
	{
		case FILE_SEEK_MODE_CURRENT:
			return fseek(file->fp, SEEK_CUR, offset);
		case FILE_SEEK_MODE_END:
			return fseek(file->fp, SEEK_END, offset);
		case FILE_SEEK_MODE_SET:
			return fseek(file->fp, SEEK_SET, offset);
	}
	return _file_error(file, EINVAL);
}


/* file_unlink */
FileError file_unlink(File * file)
{
	return (unlink(file->filename) != 0)
		? _file_error(file, errno) : 0;
}


/* file_write */
size_t file_write(File * file, void * buf, size_t size, size_t count)
{
	size_t ret;

	if((ret = fwrite(buf, size, count, file->fp)) < count)
		_file_error(file, errno);
	return ret;
}


/* file_write_buffer */
size_t file_write_buffer(File * file, Buffer * buffer)
{
	return file_write(file, buffer_get_data(buffer), sizeof(char),
			buffer_get_size(buffer));
}


/* private */
/* accessors */
static String const * _file_get_fmode(File * file, FileMode mode)
{
	switch(mode)
	{
		case FILE_MODE_READ:
			return "r";
		case FILE_MODE_WRITE:
			return "w";
	}
	_file_error(file, EINVAL);
	return NULL;
}


/* useful */
/* file_error */
static FileError _file_error(File * file, int error)
{
	return error_set_code(-error, "%s: %s", file->filename,
			strerror(error));
}
