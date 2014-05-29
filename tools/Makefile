TARGETS	= configctl
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2 -pedantic
LDFLAGSF= -L../src -Wl,-rpath,$(PREFIX)/lib -lSystem
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

configctl_OBJS = configctl.o
configctl_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
configctl_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

configctl: $(configctl_OBJS)
	$(CC) -o configctl $(configctl_OBJS) $(configctl_LDFLAGS)

configctl.o: configctl.c
	$(CC) $(configctl_CFLAGS) -c configctl.c

clean:
	$(RM) -- $(configctl_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 configctl $(DESTDIR)$(BINDIR)/configctl

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/configctl

.PHONY: all clean distclean install uninstall
