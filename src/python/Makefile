MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= $(OBJDIR)_libSystem.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags python-2.7 libSystem` -fPIC
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs python-2.7 libSystem`
LDFLAGS	= -L.. -L$(LIBDIR) -Wl,-rpath,$(LIBDIR)
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

_libSystem_OBJS = $(OBJDIR)libSystem.o
_libSystem_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
_libSystem_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)_libSystem.so: $(_libSystem_OBJS)
	$(CCSHARED) -o $(OBJDIR)_libSystem.so $(_libSystem_OBJS) $(_libSystem_LDFLAGS)

$(OBJDIR)libSystem.o: libSystem.c
	$(CC) $(_libSystem_CFLAGS) -o $(OBJDIR)libSystem.o -c libSystem.c

clean:
	$(RM) -- $(_libSystem_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0755 $(OBJDIR)_libSystem.so $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libSystem.so
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0644 libSystem.py $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libSystem.py

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libSystem.so
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libSystem.py

.PHONY: all clean distclean install uninstall
