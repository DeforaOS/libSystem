PACKAGE	= libSystem
VERSION	= 0.4.2
VENDOR	= DeforaOS
SUBDIRS	= data doc include src tests tools
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= tests
RM	= rm -f
LN	= ln -f
TAR	= tar
TGZEXT	= .tar.gz
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		([ -d "$(OBJDIR)$$i" ] || $(MKDIR) -- "$(OBJDIR)$$i") && \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/"; \
		else $(MAKE); fi) || exit; done

tests: all
	cd tests && (if [ -n "$(OBJDIR)" ]; then $(MAKE) OBJDIR="$(OBJDIR)tests/" "$(OBJDIR)tests/fixme.log"; else $(MAKE) fixme.log; fi)

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" clean; \
		else $(MAKE) clean; fi) || exit; done

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" distclean; \
		else $(MAKE) distclean; fi) || exit; done

dist:
	$(RM) -r -- $(OBJDIR)$(PACKAGE)-$(VERSION)
	$(LN) -s -- "$$PWD" $(OBJDIR)$(PACKAGE)-$(VERSION)
	@cd $(OBJDIR). && $(TAR) -czvf $(PACKAGE)-$(VERSION)$(TGZEXT) -- \
		$(PACKAGE)-$(VERSION)/data/Makefile \
		$(PACKAGE)-$(VERSION)/data/libSystem.pc.in \
		$(PACKAGE)-$(VERSION)/data/pkgconfig.sh \
		$(PACKAGE)-$(VERSION)/data/project.conf \
		$(PACKAGE)-$(VERSION)/doc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/configctl.css.xml \
		$(PACKAGE)-$(VERSION)/doc/configctl.xml.in \
		$(PACKAGE)-$(VERSION)/doc/docbook.sh \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc.sh \
		$(PACKAGE)-$(VERSION)/doc/manual.css.xml \
		$(PACKAGE)-$(VERSION)/doc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/libSystem-docs.xml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/libSystem-sections.txt \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/System.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/array.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/buffer.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/config.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/error.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/event.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/file.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/hash.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/libSystem-unused.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/object.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/parser.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/plugin.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/string.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/token.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/project.conf \
		$(PACKAGE)-$(VERSION)/include/System.h \
		$(PACKAGE)-$(VERSION)/include/Makefile \
		$(PACKAGE)-$(VERSION)/include/project.conf \
		$(PACKAGE)-$(VERSION)/include/System/array.h \
		$(PACKAGE)-$(VERSION)/include/System/buffer.h \
		$(PACKAGE)-$(VERSION)/include/System/config.h \
		$(PACKAGE)-$(VERSION)/include/System/error.h \
		$(PACKAGE)-$(VERSION)/include/System/event.h \
		$(PACKAGE)-$(VERSION)/include/System/file.h \
		$(PACKAGE)-$(VERSION)/include/System/hash.h \
		$(PACKAGE)-$(VERSION)/include/System/license.h \
		$(PACKAGE)-$(VERSION)/include/System/mutator.h \
		$(PACKAGE)-$(VERSION)/include/System/object.h \
		$(PACKAGE)-$(VERSION)/include/System/parser.h \
		$(PACKAGE)-$(VERSION)/include/System/plugin.h \
		$(PACKAGE)-$(VERSION)/include/System/string.h \
		$(PACKAGE)-$(VERSION)/include/System/token.h \
		$(PACKAGE)-$(VERSION)/include/System/variable.h \
		$(PACKAGE)-$(VERSION)/include/System/Makefile \
		$(PACKAGE)-$(VERSION)/include/System/project.conf \
		$(PACKAGE)-$(VERSION)/src/array.c \
		$(PACKAGE)-$(VERSION)/src/buffer.c \
		$(PACKAGE)-$(VERSION)/src/config.c \
		$(PACKAGE)-$(VERSION)/src/error.c \
		$(PACKAGE)-$(VERSION)/src/event.c \
		$(PACKAGE)-$(VERSION)/src/hash.c \
		$(PACKAGE)-$(VERSION)/src/mutator.c \
		$(PACKAGE)-$(VERSION)/src/object.c \
		$(PACKAGE)-$(VERSION)/src/parser.c \
		$(PACKAGE)-$(VERSION)/src/plugin.c \
		$(PACKAGE)-$(VERSION)/src/string.c \
		$(PACKAGE)-$(VERSION)/src/token.c \
		$(PACKAGE)-$(VERSION)/src/variable.c \
		$(PACKAGE)-$(VERSION)/src/Makefile \
		$(PACKAGE)-$(VERSION)/src/token.h \
		$(PACKAGE)-$(VERSION)/src/python/project.conf \
		$(PACKAGE)-$(VERSION)/src/python/Makefile \
		$(PACKAGE)-$(VERSION)/src/python/libSystem.c \
		$(PACKAGE)-$(VERSION)/src/python/libSystem.py \
		$(PACKAGE)-$(VERSION)/src/project.conf \
		$(PACKAGE)-$(VERSION)/tests/array.c \
		$(PACKAGE)-$(VERSION)/tests/buffer.c \
		$(PACKAGE)-$(VERSION)/tests/config.c \
		$(PACKAGE)-$(VERSION)/tests/error.c \
		$(PACKAGE)-$(VERSION)/tests/event.c \
		$(PACKAGE)-$(VERSION)/tests/includes.c \
		$(PACKAGE)-$(VERSION)/tests/parser.c \
		$(PACKAGE)-$(VERSION)/tests/string.c \
		$(PACKAGE)-$(VERSION)/tests/variable.c \
		$(PACKAGE)-$(VERSION)/tests/Makefile \
		$(PACKAGE)-$(VERSION)/tests/clint.sh \
		$(PACKAGE)-$(VERSION)/tests/config.conf \
		$(PACKAGE)-$(VERSION)/tests/config-empty.conf \
		$(PACKAGE)-$(VERSION)/tests/config-noeol.conf \
		$(PACKAGE)-$(VERSION)/tests/coverage.sh \
		$(PACKAGE)-$(VERSION)/tests/fixme.sh \
		$(PACKAGE)-$(VERSION)/tests/pkgconfig.sh \
		$(PACKAGE)-$(VERSION)/tests/pylint.sh \
		$(PACKAGE)-$(VERSION)/tests/python.sh \
		$(PACKAGE)-$(VERSION)/tests/tests.sh \
		$(PACKAGE)-$(VERSION)/tests/project.conf \
		$(PACKAGE)-$(VERSION)/tools/configctl.c \
		$(PACKAGE)-$(VERSION)/tools/Makefile \
		$(PACKAGE)-$(VERSION)/tools/platform.sh \
		$(PACKAGE)-$(VERSION)/tools/subst.sh \
		$(PACKAGE)-$(VERSION)/tools/project.conf \
		$(PACKAGE)-$(VERSION)/Makefile \
		$(PACKAGE)-$(VERSION)/COPYING \
		$(PACKAGE)-$(VERSION)/config.h \
		$(PACKAGE)-$(VERSION)/config.sh \
		$(PACKAGE)-$(VERSION)/README.md \
		$(PACKAGE)-$(VERSION)/project.conf
	$(RM) -- $(OBJDIR)$(PACKAGE)-$(VERSION)

distcheck: dist
	$(TAR) -xzvf $(OBJDIR)$(PACKAGE)-$(VERSION)$(TGZEXT)
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/objdir
	$(MKDIR) -- $(PACKAGE)-$(VERSION)/destdir
	cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/"
	cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" install
	cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" DESTDIR="$$PWD/destdir" uninstall
	cd "$(PACKAGE)-$(VERSION)" && $(MAKE) OBJDIR="$$PWD/objdir/" distclean
	cd "$(PACKAGE)-$(VERSION)" && $(MAKE) dist
	$(RM) -r -- $(PACKAGE)-$(VERSION)

install: all
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" install; \
		else $(MAKE) install; fi) || exit; done
	$(MKDIR) $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)
	$(INSTALL) -m 0644 COPYING $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)/COPYING
	$(MKDIR) $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)
	$(INSTALL) -m 0644 README.md $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)/README.md

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" uninstall; \
		else $(MAKE) uninstall; fi) || exit; done
	$(RM) -- $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)/COPYING
	$(RM) -- $(DESTDIR)$(PREFIX)/share/doc/$(PACKAGE)/README.md

.PHONY: all subdirs clean distclean dist distcheck install uninstall tests
