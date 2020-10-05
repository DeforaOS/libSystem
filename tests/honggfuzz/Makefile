TARGETS	= $(OBJDIR)config$(EXEEXT) test
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= hfuzz-cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	=
CFLAGS	=
LDFLAGSF= -static
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(OBJDIR)config$(EXEEXT)

config_OBJS = $(OBJDIR)config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config$(EXEEXT): $(config_OBJS)
	$(CC) -o $(OBJDIR)config$(EXEEXT) $(config_OBJS) $(config_LDFLAGS)

test: $(OBJDIR)config config-input/config.conf
	honggfuzz -i config-input -o config-output -- $(OBJDIR)config ___FILE___

$(OBJDIR)config.o: config.c ../../src/array.c ../../src/config.c ../../src/error.c ../../src/hash.c ../../src/mutator.c ../../src/object.c ../../src/string.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

clean:
	$(RM) -- $(config_OBJS)

distclean: clean
	$(RM) -- $(OBJDIR)config

install: all

uninstall:

.PHONY: all clean distclean install uninstall test
