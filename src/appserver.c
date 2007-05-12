/* $Id$ */
/* Copyright (c) 2007 The DeforaOS Project */



#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include "System.h"
#include "appinterface.h"


/* AppServerClient */
/* private */
/* types */
typedef enum _AppServerClientState
{
	ASCS_NEW,
	ASCS_LOGGED
} AppServerClientState;

typedef struct _AppServerClient
{
	AppServerClientState state;
	int fd;
	uint32_t addr; /* FIXME uint8_t[4] instead? */
	uint16_t port;
#define ASC_BUFSIZE 65536 /* FIXME */
	char buf_read[ASC_BUFSIZE];
	size_t buf_read_cnt;
	char buf_write[ASC_BUFSIZE];
	size_t buf_write_cnt;
} AppServerClient;


/* functions */
/* _appserverclient_new */
static AppServerClient * _appserverclient_new(int fd, uint32_t addr,
		uint16_t port)
{
	AppServerClient * asc;

	if((asc = malloc(sizeof(AppServerClient))) == NULL)
		return NULL;
	asc->state = ASCS_NEW;
	asc->fd = fd;
	asc->addr = addr;
	asc->port = port;
	asc->buf_read_cnt = 0;
	asc->buf_write_cnt = 0;
	return asc;
}


/* _appserverclient_delete */
static void _appserverclient_delete(AppServerClient * appserverclient)
{
	/* FIXME find a way to properly report error */
#ifdef DEBUG
	if(close(appserverclient->fd) != 0)
		perror("close");
# else
	close(appserverclient->fd);
#endif
	free(appserverclient);
}


/* AppServer */
/* private */
/* types */
ARRAY(AppServerClient *, AppServerClient);
struct _AppServer
{
	AppInterface * interface;
	Event * event;
	int event_free;
	AppServerClientArray * clients;
};


/* functions */
static int _appserver_accept(int fd, AppServer * appserver);
static int _appserver_read(int fd, AppServer * appserver);
static int _appserver_write(int fd, AppServer * appserver);

/* _appserver_accept */
static int _appserver_accept(int fd, AppServer * appserver)
{
	struct sockaddr_in sa;
	socklen_t sa_size = sizeof(struct sockaddr_in);
	int newfd;
	AppServerClient * asc;

#ifdef DEBUG
	fprintf(stderr, "%s%d%s%p%s", "_appserver_accept(", fd, ", ", appserver,
			")\n");
#endif
	if((newfd = accept(fd, (struct sockaddr *)&sa, &sa_size)) == -1)
		return 1;
	if((asc = _appserverclient_new(newfd, sa.sin_addr.s_addr, sa.sin_port))
			== NULL)
	{
		/* FIXME report error */
		close(newfd);
		return 0;
	}
	array_append(appserver->clients, &asc);
	event_register_io_read(appserver->event, newfd,
			(EventIOFunc)_appserver_read, appserver);
	return 0;
}


/* _appserver_read */
static int _read_process(AppServer * appserver, AppServerClient * asc);
static int _appserver_read(int fd, AppServer * appserver)
{
	AppServerClient * asc = NULL;
	unsigned int i;
	ssize_t len;

	for(i = 0; i < array_count(appserver->clients); i++)
	{
		if(array_get_copy(appserver->clients, i, &asc))
			break;
		if(fd == asc->fd)
			break;
		asc = NULL;
	}
	if(asc == NULL)
		return 1;
	if((len = sizeof(asc->buf_read) - asc->buf_read_cnt) <= 0
			|| (len = read(fd, &asc->buf_read[asc->buf_read_cnt],
					len)) <= 0)
	{
		/* FIXME do all this in appserverclient_delete() or something
		 * like appserver_remove_client() */
		if(asc->buf_write_cnt > 0)
			event_unregister_io_write(appserver->event, fd);
		event_unregister_io_read(appserver->event, fd);
		_appserverclient_delete(asc);
		array_remove_pos(appserver->clients, i);
		return 1;
	}
	asc->buf_read_cnt+=len;
#ifdef DEBUG
	fprintf(stderr, "%s%d%s%d%s", "_appserver_read(", fd,
			", appserver): ", len, " characters read\n");
#endif
	return _read_process(appserver, asc);
}

static int _read_logged(AppServer * appserver, AppServerClient * asc);
static int _read_process(AppServer * appserver, AppServerClient * asc)
{
	switch(asc->state)
	{
		case ASCS_NEW:
			/* FIXME authenticate */
		case ASCS_LOGGED:
			return _read_logged(appserver, asc);
	}
	return 1;
}

static int _appserver_receive(AppServer * appserver, AppServerClient * asc);
static int _read_logged(AppServer * appserver, AppServerClient * asc)
{
	if(_appserver_receive(appserver, asc) != 0)
	{
		close(asc->fd);
		return 1;
	}
	return 0;
}

static int _appserver_receive(AppServer * appserver, AppServerClient * asc)
{
	int i;
	int32_t ret;

	if((i = appinterface_receive(appserver->interface, asc->buf_read,
			asc->buf_read_cnt, asc->buf_write,
			sizeof(asc->buf_write), &asc->buf_write_cnt, &ret))
			== -1)
		return -1;
	if(i <= 0 || i > asc->buf_read_cnt)
		return -1;
	memmove(asc->buf_read, &asc->buf_read[i], asc->buf_read_cnt-i);
	asc->buf_read_cnt-=i;
	/* FIXME should be done in AppInterface? */
	if(asc->buf_write_cnt + sizeof(int) > sizeof(asc->buf_write))
		return -1;
	ret = htonl(ret);
	memcpy(&(asc->buf_write[asc->buf_write_cnt]), &ret, sizeof(int));
	asc->buf_write_cnt += sizeof(int);
	event_register_io_write(appserver->event, asc->fd,
			(EventIOFunc)_appserver_write, appserver);
	return 0;
}


/* _appserver_write */
static int _appserver_write(int fd, AppServer * appserver)
{
	AppServerClient * asc;
	size_t len;
	unsigned int i;

	/* FIXME factorize this code */
	for(i = 0; i < array_count(appserver->clients); i++)
	{
		if(array_get_copy(appserver->clients, i, &asc))
			break;
		if(fd == asc->fd)
			break;
		asc = NULL;
	}
	if(asc == NULL)
		return 1;
#ifdef DEBUG
	fprintf(stderr, "sending result: %u long\n",
			(unsigned)asc->buf_write_cnt);
#endif
	if(asc->buf_write_cnt == 0 || (len = write(fd, asc->buf_write,
					asc->buf_write_cnt)) <= 0)
		return 1; /* FIXME what here?!? */
	memmove(asc->buf_write, &asc->buf_write[len], len);
	asc->buf_write_cnt-=len;
#ifdef DEBUG
	fprintf(stderr, "%s%d%s%u%s", "_appserver_write_int(", fd,
			", appserver): ", (unsigned)len,
			" characters written\n");
#endif
	return asc->buf_write_cnt == 0 ? 1 : 0;
}


/* public */
/* functions */
/* appserver_new */
AppServer * appserver_new(const char * app, int options)
{
	AppServer * appserver;
	Event * event;

	if((event = event_new()) == NULL)
		return NULL;
	if((appserver = appserver_new_event(app, options, event)) == NULL)
	{
		event_delete(event);
		return NULL;
	}
	appserver->event_free = 1;
	return appserver;
}


/* appserver_new_event */
static int _new_server(AppServer * appserver, int options);
AppServer * appserver_new_event(char const * app, int options, Event * event)
{
	AppServer * appserver;

	if((appserver = malloc(sizeof(AppServer))) == NULL)
		return NULL;
	appserver->interface = NULL;
	appserver->event = event;
	appserver->event_free = 0;
	appserver->clients = NULL;
	if((appserver->clients = AppServerClientarray_new()) == NULL
			|| (appserver->interface = appinterface_new_server(app))
			== NULL
			|| _new_server(appserver, options) != 0)
	{
		if(appserver->clients != NULL)
			array_delete(appserver->clients);
		if(appserver->interface != NULL)
			appinterface_delete(appserver->interface);
		free(appserver);
		return NULL;
	}
	return appserver;
}

static int _new_server(AppServer * appserver, int options)
{
	int fd;
	struct sockaddr_in sa;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 1;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(appinterface_get_port(appserver->interface));
	sa.sin_addr.s_addr = htonl(options & ASO_LOCAL ? INADDR_LOOPBACK
			: INADDR_ANY);
	if(bind(fd, (struct sockaddr *)&sa, sizeof(sa)) != 0
			|| listen(fd, 5) != 0)
	{
		/* FIXME report error */
#ifdef DEBUG
		if(close(fd) != 0)
			perror("close");
# else
		close(fd);
#endif
		return 1;
	}
	event_register_io_read(appserver->event, fd,
			(EventIOFunc)_appserver_accept, appserver);
	return 0;
}


/* appserver_delete */
void appserver_delete(AppServer * appserver)
{
	appinterface_delete(appserver->interface);
	if(appserver->event_free)
		event_delete(appserver->event);
	array_delete(appserver->clients);
	free(appserver);
}


/* useful */
int appserver_loop(AppServer * appserver)
{
	return event_loop(appserver->event);
}
