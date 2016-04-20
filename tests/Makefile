TARGETS	= $(OBJDIR)array$(EXEEXT) $(OBJDIR)config$(EXEEXT) $(OBJDIR)error$(EXEEXT) $(OBJDIR)event$(EXEEXT) $(OBJDIR)includes$(EXEEXT) $(OBJDIR)pylint.log $(OBJDIR)string$(EXEEXT) $(OBJDIR)variable$(EXEEXT) $(OBJDIR)tests.log
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	=
CFLAGS	= -W -Wall -g -O2 -pedantic -fPIE -D_FORTIFY_SOURCE=2 -fstack-protector-all
LDFLAGSF= -L$(OBJDIR)../src -Wl,-rpath,$(OBJDIR)../src -lSystem
LDFLAGS	= -L$(PREFIX)/lib -Wl,-rpath,"$(PWD)/../src" -pie -Wl,-z,relro -Wl,-z,now
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

array_OBJS = $(OBJDIR)array.o
array_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
array_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)array$(EXEEXT): $(array_OBJS)
	$(CC) -o $(OBJDIR)array$(EXEEXT) $(array_OBJS) $(array_LDFLAGS)

config_OBJS = $(OBJDIR)config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config$(EXEEXT): $(config_OBJS)
	$(CC) -o $(OBJDIR)config$(EXEEXT) $(config_OBJS) $(config_LDFLAGS)

error_OBJS = $(OBJDIR)error.o
error_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
error_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)error$(EXEEXT): $(error_OBJS)
	$(CC) -o $(OBJDIR)error$(EXEEXT) $(error_OBJS) $(error_LDFLAGS)

event_OBJS = $(OBJDIR)event.o
event_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
event_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)event$(EXEEXT): $(event_OBJS)
	$(CC) -o $(OBJDIR)event$(EXEEXT) $(event_OBJS) $(event_LDFLAGS)

includes_OBJS = $(OBJDIR)includes.o
includes_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
includes_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)includes$(EXEEXT): $(includes_OBJS)
	$(CC) -o $(OBJDIR)includes$(EXEEXT) $(includes_OBJS) $(includes_LDFLAGS)

$(OBJDIR)pylint.log: pylint.sh
	./pylint.sh -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"

string_OBJS = $(OBJDIR)string.o
string_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
string_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)string$(EXEEXT): $(string_OBJS)
	$(CC) -o $(OBJDIR)string$(EXEEXT) $(string_OBJS) $(string_LDFLAGS)

variable_OBJS = $(OBJDIR)variable.o
variable_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
variable_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)variable$(EXEEXT): $(variable_OBJS)
	$(CC) -o $(OBJDIR)variable$(EXEEXT) $(variable_OBJS) $(variable_LDFLAGS)

$(OBJDIR)tests.log: $(OBJDIR)array$(EXEEXT) $(OBJDIR)config$(EXEEXT) config.conf config-noeol.conf $(OBJDIR)error$(EXEEXT) $(OBJDIR)event$(EXEEXT) $(OBJDIR)includes$(EXEEXT) python.sh $(OBJDIR)string$(EXEEXT) tests.sh $(OBJDIR)variable$(EXEEXT) $(OBJDIR)../src/libSystem.a
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

$(OBJDIR)array.o: array.c ../src/array.c
	$(CC) $(array_CFLAGS) -o $(OBJDIR)array.o -c array.c

$(OBJDIR)config.o: config.c ../src/config.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)error.o: error.c ../src/error.c
	$(CC) $(error_CFLAGS) -o $(OBJDIR)error.o -c error.c

$(OBJDIR)event.o: event.c ../src/event.c
	$(CC) $(event_CFLAGS) -o $(OBJDIR)event.o -c event.c

$(OBJDIR)includes.o: includes.c
	$(CC) $(includes_CFLAGS) -o $(OBJDIR)includes.o -c includes.c

$(OBJDIR)string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	$(RM) -- $(array_OBJS) $(config_OBJS) $(error_OBJS) $(event_OBJS) $(includes_OBJS) $(pylint.log_OBJS) $(string_OBJS) $(variable_OBJS) $(tests.log_OBJS)
	./pylint.sh -c -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
