TARGETS	= $(OBJDIR)libSystem.a $(OBJDIR)libSystem.so.0.0 $(OBJDIR)libSystem.so.0 $(OBJDIR)libSystem.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	=
CFLAGS	= -W -Wall -g -O2 -pedantic -fPIC -D_FORTIFY_SOURCE=2 -fstack-protector-all
LDFLAGSF= 
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libSystem_OBJS = $(OBJDIR)array.o $(OBJDIR)buffer.o $(OBJDIR)config.o $(OBJDIR)error.o $(OBJDIR)event.o $(OBJDIR)hash.o $(OBJDIR)mutator.o $(OBJDIR)object.o $(OBJDIR)parser.o $(OBJDIR)plugin.o $(OBJDIR)string.o $(OBJDIR)token.o $(OBJDIR)variable.o
libSystem_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
libSystem_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)  

$(OBJDIR)libSystem.a: $(libSystem_OBJS)
	$(AR) -rc $(OBJDIR)libSystem.a $(libSystem_OBJS)
	$(RANLIB) $(OBJDIR)libSystem.a

$(OBJDIR)libSystem.so.0.0: $(libSystem_OBJS)
	$(CCSHARED) -o $(OBJDIR)libSystem.so.0.0 -Wl,-soname,libSystem.so.0 $(libSystem_OBJS) $(libSystem_LDFLAGS)

$(OBJDIR)libSystem.so.0: $(OBJDIR)libSystem.so.0.0
	$(LN) -s -- libSystem.so.0.0 $(OBJDIR)libSystem.so.0

$(OBJDIR)libSystem.so: $(OBJDIR)libSystem.so.0.0
	$(LN) -s -- libSystem.so.0.0 $(OBJDIR)libSystem.so

$(OBJDIR)array.o: array.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)array.o -c array.c

$(OBJDIR)buffer.o: buffer.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)buffer.o -c buffer.c

$(OBJDIR)config.o: config.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)error.o: error.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)error.o -c error.c

$(OBJDIR)event.o: event.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)event.o -c event.c

$(OBJDIR)hash.o: hash.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)hash.o -c hash.c

$(OBJDIR)mutator.o: mutator.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)mutator.o -c mutator.c

$(OBJDIR)object.o: object.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)object.o -c object.c

$(OBJDIR)parser.o: parser.c token.h
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)parser.o -c parser.c

$(OBJDIR)plugin.o: plugin.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)plugin.o -c plugin.c

$(OBJDIR)string.o: string.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)token.o: token.c token.h
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)token.o -c token.c

$(OBJDIR)variable.o: variable.c
	$(CC) $(libSystem_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(libSystem_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(INSTALL) -m 0644 $(OBJDIR)libSystem.a $(DESTDIR)$(LIBDIR)/libSystem.a
	$(INSTALL) -m 0755 $(OBJDIR)libSystem.so.0.0 $(DESTDIR)$(LIBDIR)/libSystem.so.0.0
	$(LN) -s -- libSystem.so.0.0 $(DESTDIR)$(LIBDIR)/libSystem.so.0
	$(LN) -s -- libSystem.so.0.0 $(DESTDIR)$(LIBDIR)/libSystem.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/libSystem.a
	$(RM) -- $(DESTDIR)$(LIBDIR)/libSystem.so.0.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libSystem.so.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libSystem.so

.PHONY: all clean distclean install uninstall
