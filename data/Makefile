TARGETS	= $(OBJDIR)libSystem.pc
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)libSystem.pc: libSystem.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "$(OBJDIR)libSystem.pc"

clean:
	./pkgconfig.sh -c -P "$(PREFIX)" -- "$(OBJDIR)libSystem.pc"

distclean: clean
	$(RM) -- $(TARGETS)

install: all
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)libSystem.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "libSystem.pc"

.PHONY: all clean distclean install uninstall
