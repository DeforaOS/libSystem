TARGETS	= libSystem.pc
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

libSystem.pc: libSystem.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "libSystem.pc"

clean:
	$(RM) -- $(libSystem.pc_OBJS)
	./pkgconfig.sh -c -P "$(PREFIX)" -- "libSystem.pc"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "libSystem.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "libSystem.pc"

.PHONY: all clean distclean install uninstall
