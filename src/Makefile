TARGETS	= $(OBJDIR)libSystem.la
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -fPIC
CFLAGS	= -W -Wall -g -O2 -D_FORTIFY_SOURCE=2 -fstack-protector
LDFLAGSF= 
LDFLAGS	= -Wl,-z,relro -Wl,-z,now
EXEEXT	=
AR	= ar
ARFLAGS	= -rc
RANLIB	= ranlib
CCSHARED= $(CC) -shared
SOEXT	= .so
LIBTOOL	= libtool
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libSystem_OBJS = $(OBJDIR)array.lo $(OBJDIR)buffer.lo $(OBJDIR)config.lo $(OBJDIR)error.lo $(OBJDIR)event.lo $(OBJDIR)file.lo $(OBJDIR)hash.lo $(OBJDIR)mutator.lo $(OBJDIR)object.lo $(OBJDIR)parser.lo $(OBJDIR)plugin.lo $(OBJDIR)string.lo $(OBJDIR)token.lo $(OBJDIR)variable.lo
libSystem_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
libSystem_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l dl` `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l m`

$(OBJDIR)libSystem.la: $(libSystem_OBJS)
	$(LIBTOOL) --mode=link --tag=CC $(CC) -o $(OBJDIR)libSystem.la $(libSystem_OBJS) `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l dl` `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l m` -rpath $(LIBDIR) $(libSystem_LDFLAGS)

$(OBJDIR)array.o $(OBJDIR)array.lo: array.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)array.o -c array.c

$(OBJDIR)buffer.o $(OBJDIR)buffer.lo: buffer.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)buffer.o -c buffer.c

$(OBJDIR)config.o $(OBJDIR)config.lo: config.c ../config.h
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)error.o $(OBJDIR)error.lo: error.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)error.o -c error.c

$(OBJDIR)event.o $(OBJDIR)event.lo: event.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)event.o -c event.c

$(OBJDIR)file.o $(OBJDIR)file.lo: file.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)file.o -c file.c

$(OBJDIR)hash.o $(OBJDIR)hash.lo: hash.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)hash.o -c hash.c

$(OBJDIR)mutator.o $(OBJDIR)mutator.lo: mutator.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)mutator.o -c mutator.c

$(OBJDIR)object.o $(OBJDIR)object.lo: object.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)object.o -c object.c

$(OBJDIR)parser.o $(OBJDIR)parser.lo: parser.c token.h
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)parser.o -c parser.c

$(OBJDIR)plugin.o $(OBJDIR)plugin.lo: plugin.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)plugin.o -c plugin.c

$(OBJDIR)string.o $(OBJDIR)string.lo: string.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)token.o $(OBJDIR)token.lo: token.c token.h
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)token.o -c token.c

$(OBJDIR)variable.o $(OBJDIR)variable.lo: variable.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(libSystem_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(libSystem_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: all
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(LIBTOOL) --mode=install $(INSTALL) -m 0755 $(OBJDIR)libSystem.la $(DESTDIR)$(LIBDIR)/libSystem.la
	$(LIBTOOL) --mode=finish $(DESTDIR)$(LIBDIR)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) -- $(DESTDIR)$(LIBDIR)/libSystem.la

.PHONY: all clean distclean install uninstall
