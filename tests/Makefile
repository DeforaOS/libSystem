TARGETS	= $(OBJDIR)config $(OBJDIR)includes $(OBJDIR)pylint.log $(OBJDIR)string $(OBJDIR)variable $(OBJDIR)tests.log
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W
CFLAGS	= -Wall -g -O2
LDFLAGSF= -L$(OBJDIR). -L../src -lSystem
LDFLAGS	= -L$(PREFIX)/lib -Wl,-rpath,"$(PWD)/../src"
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

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

$(OBJDIR)tests.log: $(OBJDIR)config config.conf config-noeol.conf $(OBJDIR)includes python.sh $(OBJDIR)string tests.sh $(OBJDIR)variable
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

$(OBJDIR)config.o: config.c ../src/config.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)includes.o: includes.c
	$(CC) $(includes_CFLAGS) -o $(OBJDIR)includes.o -c includes.c

$(OBJDIR)string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(config_OBJS) $(includes_OBJS) $(pylint.log_OBJS) $(string_OBJS) $(variable_OBJS) $(tests.log_OBJS)
	./pylint.sh -c -P "$(PREFIX)" -- "pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
