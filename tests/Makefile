SUBDIRS	= afl honggfuzz
TARGETS	= $(OBJDIR)array$(EXEEXT) $(OBJDIR)buffer$(EXEEXT) $(OBJDIR)clint.log $(OBJDIR)config$(EXEEXT) $(OBJDIR)coverage.log $(OBJDIR)error$(EXEEXT) $(OBJDIR)event$(EXEEXT) $(OBJDIR)fixme.log $(OBJDIR)includes$(EXEEXT) $(OBJDIR)parser$(EXEEXT) $(OBJDIR)pylint.log $(OBJDIR)string$(EXEEXT) $(OBJDIR)variable$(EXEEXT) $(OBJDIR)tests.log
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	=
CFLAGS	= -W -Wall -g -O2 -fPIE -D_FORTIFY_SOURCE=2 -fstack-protector
LDFLAGSF= -L$(OBJDIR)../src -Wl,-rpath,$(OBJDIR)../src -lSystem `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l dl` `../tools/platform.sh -O DESTDIR="$(DESTDIR)" -l m`
LDFLAGS	= -pie -Wl,-z,relro -Wl,-z,now
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs $(OBJDIR)array$(EXEEXT) $(OBJDIR)buffer$(EXEEXT) $(OBJDIR)config$(EXEEXT) $(OBJDIR)error$(EXEEXT) $(OBJDIR)event$(EXEEXT) $(OBJDIR)includes$(EXEEXT) $(OBJDIR)parser$(EXEEXT) $(OBJDIR)string$(EXEEXT) $(OBJDIR)variable$(EXEEXT)

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		([ -d "$(OBJDIR)$$i" ] || $(MKDIR) -- "$(OBJDIR)$$i") && \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/"; \
		else $(MAKE); fi) || exit; done

array_OBJS = $(OBJDIR)array.o
array_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
array_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)array$(EXEEXT): $(array_OBJS)
	$(CC) -o $(OBJDIR)array$(EXEEXT) $(array_OBJS) $(array_LDFLAGS)

buffer_OBJS = $(OBJDIR)buffer.o
buffer_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
buffer_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)buffer$(EXEEXT): $(buffer_OBJS)
	$(CC) -o $(OBJDIR)buffer$(EXEEXT) $(buffer_OBJS) $(buffer_LDFLAGS)

$(OBJDIR)clint.log: $(OBJDIR)../src/libSystem.a clint.sh
	./clint.sh -P "$(PREFIX)" -O CPPFLAGS="-I$(DESTDIR)$(PREFIX)/include -I../include `pkg-config --cflags python-2.7`" -- "$(OBJDIR)clint.log"

config_OBJS = $(OBJDIR)config.o
config_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
config_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config$(EXEEXT): $(config_OBJS)
	$(CC) -o $(OBJDIR)config$(EXEEXT) $(config_OBJS) $(config_LDFLAGS)

$(OBJDIR)coverage.log: coverage.sh
	./coverage.sh -P "$(PREFIX)" -- "$(OBJDIR)coverage.log"

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

$(OBJDIR)fixme.log: $(OBJDIR)../src/libSystem.a fixme.sh
	./fixme.sh -P "$(PREFIX)" -- "$(OBJDIR)fixme.log"

includes_OBJS = $(OBJDIR)includes.o
includes_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
includes_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)includes$(EXEEXT): $(includes_OBJS)
	$(CC) -o $(OBJDIR)includes$(EXEEXT) $(includes_OBJS) $(includes_LDFLAGS)

parser_OBJS = $(OBJDIR)parser.o
parser_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
parser_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)parser$(EXEEXT): $(parser_OBJS)
	$(CC) -o $(OBJDIR)parser$(EXEEXT) $(parser_OBJS) $(parser_LDFLAGS)

$(OBJDIR)pylint.log: ../src/python/libSystem.py pylint.sh
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

$(OBJDIR)tests.log: $(OBJDIR)array$(EXEEXT) $(OBJDIR)buffer$(EXEEXT) $(OBJDIR)config$(EXEEXT) config.conf config-noeol.conf $(OBJDIR)error$(EXEEXT) $(OBJDIR)event$(EXEEXT) $(OBJDIR)includes$(EXEEXT) $(OBJDIR)parser$(EXEEXT) pkgconfig.sh python.sh $(OBJDIR)string$(EXEEXT) tests.sh $(OBJDIR)variable$(EXEEXT) $(OBJDIR)../src/libSystem.a ../src/python/libSystem.c
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

$(OBJDIR)array.o: array.c ../src/array.c
	$(CC) $(array_CFLAGS) -o $(OBJDIR)array.o -c array.c

$(OBJDIR)buffer.o: buffer.c ../src/buffer.c
	$(CC) $(buffer_CFLAGS) -o $(OBJDIR)buffer.o -c buffer.c

$(OBJDIR)config.o: config.c ../src/config.c
	$(CC) $(config_CFLAGS) -o $(OBJDIR)config.o -c config.c

$(OBJDIR)error.o: error.c ../src/error.c
	$(CC) $(error_CFLAGS) -o $(OBJDIR)error.o -c error.c

$(OBJDIR)event.o: event.c ../src/event.c
	$(CC) $(event_CFLAGS) -o $(OBJDIR)event.o -c event.c

$(OBJDIR)includes.o: includes.c
	$(CC) $(includes_CFLAGS) -o $(OBJDIR)includes.o -c includes.c

$(OBJDIR)parser.o: parser.c ../src/parser.c
	$(CC) $(parser_CFLAGS) -o $(OBJDIR)parser.o -c parser.c

$(OBJDIR)string.o: string.c ../src/string.c
	$(CC) $(string_CFLAGS) -o $(OBJDIR)string.o -c string.c

$(OBJDIR)variable.o: variable.c ../src/variable.c
	$(CC) $(variable_CFLAGS) -o $(OBJDIR)variable.o -c variable.c

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" clean; \
		else $(MAKE) clean; fi) || exit; done
	$(RM) -- $(array_OBJS) $(buffer_OBJS) $(config_OBJS) $(error_OBJS) $(event_OBJS) $(includes_OBJS) $(parser_OBJS) $(string_OBJS) $(variable_OBJS)
	./clint.sh -c -P "$(PREFIX)" -O CPPFLAGS="-I$(DESTDIR)$(PREFIX)/include -I../include `pkg-config --cflags python-2.7`" -- "$(OBJDIR)clint.log"
	./coverage.sh -c -P "$(PREFIX)" -- "$(OBJDIR)coverage.log"
	./fixme.sh -c -P "$(PREFIX)" -- "$(OBJDIR)fixme.log"
	./pylint.sh -c -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" distclean; \
		else $(MAKE) distclean; fi) || exit; done
	$(RM) -- $(array_OBJS) $(buffer_OBJS) $(config_OBJS) $(error_OBJS) $(event_OBJS) $(includes_OBJS) $(parser_OBJS) $(string_OBJS) $(variable_OBJS)
	./clint.sh -c -P "$(PREFIX)" -O CPPFLAGS="-I$(DESTDIR)$(PREFIX)/include -I../include `pkg-config --cflags python-2.7`" -- "$(OBJDIR)clint.log"
	./coverage.sh -c -P "$(PREFIX)" -- "$(OBJDIR)coverage.log"
	./fixme.sh -c -P "$(PREFIX)" -- "$(OBJDIR)fixme.log"
	./pylint.sh -c -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "$(OBJDIR)tests.log"
	$(RM) -- $(TARGETS)

install: all
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" install; \
		else $(MAKE) install; fi) || exit; done

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" uninstall; \
		else $(MAKE) uninstall; fi) || exit; done

.PHONY: all subdirs clean distclean install uninstall
