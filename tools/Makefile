TARGETS	= $(OBJDIR)configctl$(EXEEXT)
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
LDFLAGSF= -L$(OBJDIR)../src -Wl,-rpath,$(PREFIX)/lib -lSystem
LDFLAGS	= -pie -Wl,-z,relro -Wl,-z,now
EXEEXT	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

configctl_OBJS = $(OBJDIR)configctl.o
configctl_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
configctl_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)configctl$(EXEEXT): $(configctl_OBJS)
	$(CC) -o $(OBJDIR)configctl$(EXEEXT) $(configctl_OBJS) $(configctl_LDFLAGS)

$(OBJDIR)configctl.o: configctl.c
	$(CC) $(configctl_CFLAGS) -o $(OBJDIR)configctl.o -c configctl.c

clean:
	$(RM) -- $(configctl_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)configctl$(EXEEXT) $(DESTDIR)$(BINDIR)/configctl$(EXEEXT)

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/configctl$(EXEEXT)

.PHONY: all clean distclean install uninstall
