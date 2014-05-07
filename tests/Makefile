TARGETS	= includes string variable tests.log
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2
LDFLAGSF= -L ../src -lSystem
LDFLAGS	= -L$(PREFIX)/lib -Wl,-rpath,"$(PWD)/../src"
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

includes_OBJS = includes.o
includes_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
includes_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

includes: $(includes_OBJS)
	$(CC) -o includes $(includes_OBJS) $(includes_LDFLAGS)

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

tests.log: includes string tests.sh variable
	./tests.sh -P "$(PREFIX)" -- "tests.log"

includes.o: includes.c
	$(CC) $(includes_CFLAGS) -c includes.c

string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -c string.c

variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -c variable.c

clean:
	$(RM) -- $(includes_OBJS) $(string_OBJS) $(variable_OBJS) $(tests.log_OBJS)
	./tests.sh -c -P "$(PREFIX)" -- "tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
