TARGETS	= $(OBJDIR)config$(EXEEXT) $(OBJDIR)variable$(EXEEXT) test
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= afl-gcc
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


all: $(OBJDIR)config$(EXEEXT) $(OBJDIR)variable$(EXEEXT)

config_OBJS = $(OBJDIR)config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config$(EXEEXT): $(config_OBJS)
	$(CC) -o $(OBJDIR)config$(EXEEXT) $(config_OBJS) $(config_LDFLAGS)

variable_OBJS = $(OBJDIR)variable.o
variable_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
variable_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)variable$(EXEEXT): $(variable_OBJS)
	$(CC) -o $(OBJDIR)variable$(EXEEXT) $(variable_OBJS) $(variable_LDFLAGS)

test: $(OBJDIR)config config-input/config.conf
	afl-fuzz -i config-input -o config-output -- $(OBJDIR)config @@

$(OBJDIR)config.o: config.c ../../src/array.c ../../src/config.c ../../src/error.c ../../src/hash.c ../../src/mutator.c ../../src/object.c ../../src/string.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)variable.o: variable.c ../../src/array.c ../../src/buffer.c ../../src/error.c ../../src/object.c ../../src/string.c ../../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(config_OBJS) $(variable_OBJS)

distclean: clean
	$(RM) -- $(OBJDIR)config $(OBJDIR)variable

install: all

uninstall:

.PHONY: all clean distclean install uninstall test
