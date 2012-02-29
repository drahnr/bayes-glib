include Makefile.include
include Makefile.tests
include bayes-glib/Makefile.include
include tests/Makefile.include

SUBDIRS = . doc

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

DISTCHECK_CONFIGURE_FLAGS =
DISTCHECK_CONFIGURE_FLAGS += --enable-maintainer-flags
DISTCHECK_CONFIGURE_FLAGS += --enable-gtk-doc
DISTCHECK_CONFIGURE_FLAGS += --enable-introspection
DISTCHECK_CONFIGURE_FLAGS += --enable-silent-rules

bayesdocdir = ${prefix}/doc/bayes-glib-1.0
bayesdoc_DATA =
bayesdoc_DATA += README
bayesdoc_DATA += COPYING
bayesdoc_DATA += AUTHORS
bayesdoc_DATA += ChangeLog
bayesdoc_DATA += INSTALL
bayesdoc_DATA += NEWS

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += $(top_srcdir)/bayes-glib-1.0.pc

EXTRA_DIST += $(bayesdoc_DATA)

dist-hook:
	@if test -d "$(srcdir)/.git"; then                              \
          (cd "$(srcdir)" &&                                            \
           $(top_srcdir)/missing --run git log --stat ) > ChangeLog.tmp \
           && mv -f ChangeLog.tmp $(top_distdir)/ChangeLog              \
           || (rm -f ChangeLog.tmp;                                     \
               echo Failed to generate ChangeLog >&2);                  \
	else                                                            \
	  echo A git checkout is required to generate a ChangeLog >&2;  \
	fi

uninstall-local:
	-rm -r $(bayesdocdir)
