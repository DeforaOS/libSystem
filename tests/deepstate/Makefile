TARGETS	= $(OBJDIR)config-deepstate$(EXEEXT) test
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= $(PREFIX)/bin/clang++
CPPFLAGSF=
CPPFLAGS= -isystem $(PREFIX)/include -I../../include
CFLAGSF	=
CFLAGS	=
CXX	= $(CC)
LDFLAGS	= -L$(PREFIX)/lib -ldeepstate
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(OBJDIR)config-deepstate$(EXEEXT)

config-deepstate_OBJS = $(OBJDIR)array.o $(OBJDIR)config-deepstate.o $(OBJDIR)config.o $(OBJDIR)error.o $(OBJDIR)hash.o $(OBJDIR)mutator.o $(OBJDIR)object.o $(OBJDIR)string.o
config-deepstate_CXXFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CXXFLAGSF) $(CXXFLAGS)
config-deepstate_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)config-deepstate$(EXEEXT): $(config-deepstate_OBJS)
	$(CC) -o $(OBJDIR)config-deepstate$(EXEEXT) $(config-deepstate_OBJS) $(config-deepstate_LDFLAGS)

test: $(OBJDIR)config-deepstate
	$(PREFIX)/bin/deepstate $(OBJDIR)config-deepstate

$(OBJDIR)array.o: array.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)array.o -c array.cpp

$(OBJDIR)config-deepstate.o: config-deepstate.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)config-deepstate.o -c config-deepstate.cpp

$(OBJDIR)config.o: config.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)config.o -c config.cpp

$(OBJDIR)error.o: error.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)error.o -c error.cpp

$(OBJDIR)hash.o: hash.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)hash.o -c hash.cpp

$(OBJDIR)mutator.o: mutator.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)mutator.o -c mutator.cpp

$(OBJDIR)object.o: object.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)object.o -c object.cpp

$(OBJDIR)string.o: string.cpp
	$(CXX) $(config-deepstate_CXXFLAGS) -o $(OBJDIR)string.o -c string.cpp

clean:
	$(RM) -- $(config-deepstate_OBJS)

distclean: clean
	$(RM) -- $(OBJDIR)config-deepstate

install: all

uninstall:

.PHONY: all clean distclean install uninstall test
