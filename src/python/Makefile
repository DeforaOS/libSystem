OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= $(OBJDIR)_libSystem$(SOEXT)
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I../../include
CPPFLAGS=
CFLAGSF	= -fPIC `pkg-config --cflags python-2.7`
CFLAGS	= -W -Wall -g -O2 -D_FORTIFY_SOURCE=2 -fstack-protector
LDFLAGSF= `pkg-config --libs python-2.7` -L$(OBJDIR).. -L$(OBJDIR)../.libs -lSystem
LDFLAGS	= -Wl,-rpath,$(LIBDIR) -Wl,-z,relro -Wl,-z,now
EXEEXT	=
AR	= ar
ARFLAGS	= -rc
RANLIB	= ranlib
CCSHARED= $(CC) -shared
SOEXT	= .so
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

_libSystem_OBJS = $(OBJDIR)libSystem.o
_libSystem_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
_libSystem_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)_libSystem$(SOEXT): $(_libSystem_OBJS)
	$(CCSHARED) -o $(OBJDIR)_libSystem$(SOEXT) $(_libSystem_OBJS) $(_libSystem_LDFLAGS)

$(OBJDIR)libSystem.o: libSystem.c
	$(CC) $(_libSystem_CFLAGS) -o $(OBJDIR)libSystem.o -c libSystem.c

clean:
	$(RM) -- $(_libSystem_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: all
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0755 $(OBJDIR)_libSystem$(SOEXT) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libSystem$(SOEXT)
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0644 libSystem.py $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libSystem.py

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libSystem$(SOEXT)
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libSystem.py

.PHONY: all clean distclean install uninstall
