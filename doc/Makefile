SUBDIRS	= gtkdoc
TARGETS	= $(OBJDIR)configctl.1 $(OBJDIR)configctl.html $(OBJDIR)configctl.xml $(OBJDIR)gtkdoc/html.stamp $(OBJDIR)gtkdoc/libSystem.types $(OBJDIR)gtkdoc/tmpl.stamp $(OBJDIR)gtkdoc/xml.stamp
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs $(TARGETS)

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		([ -d "$(OBJDIR)$$i" ] || $(MKDIR) -- "$(OBJDIR)$$i") && \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/"; \
		else $(MAKE); fi) || exit; done

$(OBJDIR)configctl.1: $(OBJDIR)configctl.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)configctl.1"

$(OBJDIR)configctl.html: configctl.css.xml $(OBJDIR)configctl.xml manual.css.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)configctl.html"

$(OBJDIR)configctl.xml: ../tools/subst.sh configctl.xml.in
	../tools/subst.sh -P "$(PREFIX)" -- "$(OBJDIR)configctl.xml"

$(OBJDIR)gtkdoc/html.stamp: gtkdoc.sh gtkdoc/libSystem-docs.xml $(OBJDIR)gtkdoc/xml.stamp gtkdoc/xml/gtkdocentities.ent ../config.ent ../config.sh
	./gtkdoc.sh -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/html.stamp"

$(OBJDIR)gtkdoc/libSystem.types: gtkdoc.sh ../config.sh
	./gtkdoc.sh -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/libSystem.types"

$(OBJDIR)gtkdoc/tmpl.stamp: gtkdoc.sh $(OBJDIR)gtkdoc/libSystem.types ../config.sh
	./gtkdoc.sh -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/tmpl.stamp"

$(OBJDIR)gtkdoc/xml.stamp: gtkdoc.sh $(OBJDIR)gtkdoc/tmpl.stamp
	./gtkdoc.sh -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/xml.stamp"

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" clean; \
		else $(MAKE) clean; fi) || exit; done
	./docbook.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.1"
	./docbook.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.html"
	../tools/subst.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.xml"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/html.stamp"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/libSystem.types"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/tmpl.stamp"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/xml.stamp"

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" distclean; \
		else $(MAKE) distclean; fi) || exit; done
	./docbook.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.1"
	./docbook.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.html"
	../tools/subst.sh -c -P "$(PREFIX)" -- "$(OBJDIR)configctl.xml"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/html.stamp"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/libSystem.types"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/tmpl.stamp"
	./gtkdoc.sh -c -P "$(PREFIX)" -- "$(OBJDIR)gtkdoc/xml.stamp"
	$(RM) -- $(TARGETS)

install: all
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" install; \
		else $(MAKE) install; fi) || exit; done
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)configctl.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)configctl.html"
	./gtkdoc.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)gtkdoc/html.stamp"

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && \
		if [ -n "$(OBJDIR)" ]; then \
		$(MAKE) OBJDIR="$(OBJDIR)$$i/" uninstall; \
		else $(MAKE) uninstall; fi) || exit; done
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "configctl.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "configctl.html"
	./gtkdoc.sh -P "$(DESTDIR)$(PREFIX)" -u -- "gtkdoc/html.stamp"

.PHONY: all subdirs clean distclean install uninstall
