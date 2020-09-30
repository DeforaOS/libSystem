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

	if((file = (File *)object_new(sizeof(*file))) == NULL)
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
FileError file_read(File * file, void * buf, size_t size, size_t * count)
{
	size_t s;

	if((s = fread(buf, size, *count, file->fp)) < *count
			&& ferror(file->fp))
	{
		*count = s;
		return _file_error(file, errno);
	}
	return 0;
}


/* file_read_buffer */
FileError file_read_buffer(File * file, Buffer * buffer)
{
	int ret;
	size_t s;

	s = buffer_get_size(buffer);
	if((ret = file_read(file, buffer_get_data(buffer), sizeof(char),
					&s)) != 0)
		buffer_set_size(buffer, s);
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
FileError file_write(File * file, const void * buf, size_t size, size_t * count)
{
	size_t s;

	if((s = fwrite(buf, size, *count, file->fp)) < *count)
	{
		*count = s;
		return _file_error(file, errno);
	}
	return 0;
}


/* file_write_buffer */
FileError file_write_buffer(File * file, Buffer const * buffer)
{
	size_t size;

	size = buffer_get_size(buffer);
	return file_write(file, buffer_get_data(buffer), sizeof(char), &size);
}


/* private */
/* accessors */
static String const * _file_get_fmode(File * file, FileMode mode)
{
	struct
	{
		FileMode mode;
		String const * ret;
	} modes[] =
	{
		{ FILE_MODE_WRITE
			| FILE_MODE_APPEND
			| FILE_MODE_CREATE,	"a"	},
		{ FILE_MODE_WRITE
			| FILE_MODE_APPEND
			| FILE_MODE_CREATE
			| FILE_MODE_EXCLUSIVE,	"ax"	},
		{ FILE_MODE_READ_WRITE
			| FILE_MODE_APPEND
			| FILE_MODE_CREATE,	"a+"	},
		{ FILE_MODE_READ_WRITE
			| FILE_MODE_APPEND
			| FILE_MODE_CREATE
			| FILE_MODE_EXCLUSIVE,	"a+x"	},
		{ FILE_MODE_READ,		"r"	},
		{ FILE_MODE_READ_WRITE,		"r+"	},
		{ FILE_MODE_WRITE
			| FILE_MODE_CREATE
			| FILE_MODE_TRUNCATE,	"w"	},
		{ FILE_MODE_WRITE
			| FILE_MODE_CREATE
			| FILE_MODE_TRUNCATE
			| FILE_MODE_EXCLUSIVE,	"wx"	},
		{ FILE_MODE_READ_WRITE
			| FILE_MODE_CREATE
			| FILE_MODE_TRUNCATE,	"w+"	},
		{ FILE_MODE_READ_WRITE
			| FILE_MODE_CREATE
			| FILE_MODE_TRUNCATE
			| FILE_MODE_EXCLUSIVE,	"w+x"	},
	};
	size_t i;

	for(i = 0; i < sizeof(modes) / sizeof(*modes); i++)
		if(modes[i].mode == mode)
			return modes[i].ret;
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
