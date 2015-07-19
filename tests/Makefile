TARGETS	= $(OBJDIR)array $(OBJDIR)config $(OBJDIR)includes $(OBJDIR)pylint.log $(OBJDIR)string $(OBJDIR)variable $(OBJDIR)tests.log
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2 -pedantic -fstack-protector -fPIE
LDFLAGSF= -L../src -L$(OBJDIR)../src -Wl,-rpath,$(OBJDIR)../src -lSystem
LDFLAGS	= -L$(PREFIX)/lib -Wl,-rpath,"$(PWD)/../src" -Wl,-pie -Wl,-z,relro
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

array_OBJS = $(OBJDIR)array.o
array_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
array_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)array: $(array_OBJS)
	$(CC) -o $(OBJDIR)array $(array_OBJS) $(array_LDFLAGS)

config_OBJS = $(OBJDIR)config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config: $(config_OBJS)
	$(CC) -o $(OBJDIR)config $(config_OBJS) $(config_LDFLAGS)

includes_OBJS = $(OBJDIR)includes.o
includes_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
includes_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)includes: $(includes_OBJS)
	$(CC) -o $(OBJDIR)includes $(includes_OBJS) $(includes_LDFLAGS)

$(OBJDIR)pylint.log: pylint.sh
	./pylint.sh -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"

string_OBJS = $(OBJDIR)string.o
string_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
string_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)string: $(string_OBJS)
	$(CC) -o $(OBJDIR)string $(string_OBJS) $(string_LDFLAGS)

variable_OBJS = $(OBJDIR)variable.o
variable_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
variable_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)variable: $(variable_OBJS)
	$(CC) -o $(OBJDIR)variable $(variable_OBJS) $(variable_LDFLAGS)

$(OBJDIR)tests.log: $(OBJDIR)array $(OBJDIR)config config.conf config-noeol.conf $(OBJDIR)includes python.sh $(OBJDIR)string tests.sh $(OBJDIR)variable $(OBJDIR)../src/libSystem.a
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

$(OBJDIR)array.o: array.c ../src/array.c
	$(CC) $(array_CFLAGS) -o $(OBJDIR)array.o -c array.c

$(OBJDIR)config.o: config.c ../src/config.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)includes.o: includes.c
	$(CC) $(includes_CFLAGS) -o $(OBJDIR)includes.o -c includes.c

$(OBJDIR)string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(array_OBJS) $(config_OBJS) $(includes_OBJS) $(pylint.log_OBJS) $(string_OBJS) $(variable_OBJS) $(tests.log_OBJS)
	./pylint.sh -c -P "$(PREFIX)" -- "pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
