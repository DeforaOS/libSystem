/* $Id$ */
/* Copyright (c) 2006-2018 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBSYSTEM_SYSTEM_EVENT_H
# define LIBSYSTEM_SYSTEM_EVENT_H

# include <sys/time.h>
# include <time.h>


/* Event */
/* types */
typedef struct _Event Event;

typedef int (*EventIOFunc)(int fd, void * data);
typedef int (*EventTimeoutFunc)(void * data);


/* functions */
Event * event_new(void);
void event_delete(Event * event);

/* useful */
int event_loop(Event * event);
void event_loop_quit(Event * event);
int event_register_idle(Event * event, EventTimeoutFunc func, void * userdata);
int event_register_io_read(Event * event, int fd, EventIOFunc func,
		void * userdata);
int event_register_io_write(Event * event, int fd, EventIOFunc func,
		void * userdata);
int event_register_timeout(Event * event, struct timeval * timeout,
		EventTimeoutFunc func, void * userdata);
int event_unregister_io_read(Event * event, int fd);
int event_unregister_io_write(Event * event, int fd);
int event_unregister_timeout(Event * event, EventTimeoutFunc func);

#endif /* !LIBSYSTEM_SYSTEM_EVENT_H */
