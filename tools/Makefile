TARGETS	= config
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

config_OBJS = config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

config: $(config_OBJS)
	$(CC) -o config $(config_OBJS) $(config_LDFLAGS)

config.o: config.c
	$(CC) $(config_CFLAGS) -c config.c

clean:
	$(RM) -- $(config_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 config $(DESTDIR)$(BINDIR)/config

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/config

.PHONY: all clean distclean install uninstall