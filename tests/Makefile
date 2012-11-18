TARGETS	= string variable tests.log
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	?= cc
CPPFLAGSF= -I ../include
CPPFLAGS?=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2
LDFLAGSF= -L ../src -lSystem
LDFLAGS	= -L$(PREFIX)/lib -Wl,-rpath,"$(PWD)/../src"
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

string_OBJS = string.o
string_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
string_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

string: $(string_OBJS)
	$(CC) -o string $(string_OBJS) $(string_LDFLAGS)

variable_OBJS = variable.o
variable_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
variable_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

variable: $(variable_OBJS)
	$(CC) -o variable $(variable_OBJS) $(variable_LDFLAGS)

tests.log: string variable
	./tests.sh -P "$(PREFIX)" -- "tests.log"

string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -c string.c

variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -c variable.c

clean:
	$(RM) -- $(string_OBJS) $(variable_OBJS) $(tests.log_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
