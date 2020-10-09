TARGETS	= $(OBJDIR)variable$(EXEEXT) test
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= clang
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	=
CFLAGS	=
LDFLAGSF= -static -fsanitize=fuzzer
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(OBJDIR)variable$(EXEEXT)

variable_OBJS = $(OBJDIR)variable.o
variable_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
variable_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)variable$(EXEEXT): $(variable_OBJS)
	$(CC) -o $(OBJDIR)variable$(EXEEXT) $(variable_OBJS) $(variable_LDFLAGS)

test: $(OBJDIR)variable
	$(OBJDIR)./variable

$(OBJDIR)variable.o: variable.c ../../src/array.c ../../src/buffer.c ../../src/error.c ../../src/object.c ../../src/string.c ../../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(variable_OBJS)

distclean: clean
	$(RM) -- $(OBJDIR)variable

install: all

uninstall:

.PHONY: all clean distclean install uninstall test
