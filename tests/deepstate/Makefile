PREFIX	= /home/khorben/pkg
BINDIR	= $(PREFIX)/bin
INCLUDEDIR=$(PREFIX)/include
LIBDIR	= $(PREFIX)/lib
CPPFLAGS=-isystem $(INCLUDEDIR) -I../../include
LDFLAGS=-L$(LIBDIR) -ldeepstate
CXX=	$(BINDIR)/clang++
DEEPSTATE=$(BINDIR)/deepstate
RM=	rm -f
OBJS=	array.o config.o config-deepstate.o error.o hash.o mutator.o object.o string.o

all: config-deepstate

config-deepstate: $(OBJS)
	$(CXX) $(LDFLAGS) -o config-deepstate $(OBJS)

array.o: array.cpp
	$(CXX) $(CPPFLAGS) -c array.cpp

config.o: config.cpp
	$(CXX) $(CPPFLAGS) -c config.cpp

config-deepstate.o: config-deepstate.cpp
	$(CXX) $(CPPFLAGS) -c config-deepstate.cpp

error.o: error.cpp
	$(CXX) $(CPPFLAGS) -c error.cpp

hash.o: hash.cpp
	$(CXX) $(CPPFLAGS) -c hash.cpp

mutator.o: mutator.cpp
	$(CXX) $(CPPFLAGS) -c mutator.cpp

object.o: object.cpp
	$(CXX) $(CPPFLAGS) -c object.cpp

string.o: string.cpp
	$(CXX) $(CPPFLAGS) -c string.cpp

clean:
	$(RM) -- $(OBJS)

distclean: clean

test: config-deepstate
	$(DEEPSTATE) ./config-deepstate

.PHONY: clean distclean test
