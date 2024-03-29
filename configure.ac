dnl **************************************************************************
dnl Versioning Macros
dnl **************************************************************************
m4_define([bayes_major_version], [0])
m4_define([bayes_minor_version], [1])
m4_define([bayes_micro_version], [0])
m4_define([bayes_version],
          [bayes_major_version.bayes_minor_version.bayes_micro_version])
m4_define([bayes_binary_age], [m4_eval(100 * bayes_minor_version +
           bayes_micro_version)])
m4_define([lt_current], [m4_eval(100 * bayes_minor_version +
           bayes_micro_version - bayes_interface_age)])
m4_define([lt_revision], [bayes_interface_age])
m4_define([lt_age], [m4_eval(bayes_binary_age - bayes_interface_age)])


dnl **************************************************************************
dnl Initialize Automake
dnl **************************************************************************
AC_PREREQ([2.64])
AC_INIT([bayes],[bayes_version])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wno-portability])
AM_MAINTAINER_MODE([enable])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])
AS_AM_REALLY_SILENT

# Check for programs
AC_PROG_CC
AM_PROG_CC_C_O
AM_PATH_GLIB_2_0
GLIB_GSETTINGS

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT


dnl **************************************************************************
dnl Versioning Definitions
dnl **************************************************************************
BAYES_MAJOR_VERSION=bayes_major_version
BAYES_MINOR_VERSION=bayes_minor_version
BAYES_MICRO_VERSION=bayes_micro_version
BAYES_API_VERSION=1.0
BAYES_VERSION=bayes_version
AC_SUBST(BAYES_MAJOR_VERSION)
AC_SUBST(BAYES_MINOR_VERSION)
AC_SUBST(BAYES_MICRO_VERSION)
AC_SUBST(BAYES_API_VERSION)
AC_SUBST(BAYES_VERSION)


dnl **************************************************************************
dnl Check for Required Modules
dnl **************************************************************************
PKG_CHECK_MODULES(GOBJECT, [gobject-2.0 >= 2.26])


dnl **************************************************************************
dnl Check for libunistring
dnl **************************************************************************
AC_CHECK_HEADERS([unistr.h])


dnl **************************************************************************
dnl Enable extra debugging options
dnl **************************************************************************
m4_define([debug_default],
	  [m4_if(m4_eval(bayes_minor_version % 2), [1],
	  [yes], [minimum])])
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
	      		      [turn on debugging @<:@default=debug_default@:>@])],
	      		      [],
	      		      [enable_debug=debug_default])
AS_CASE([$enable_debug],
	[yes], [
		CFLAGS="$CFLAGS -g"
		CFLAGS="$CFLAGS -Wall"
		CFLAGS="$CFLAGS -Werror"
		CFLAGS="$CFLAGS -Wold-style-definition"
		CFLAGS="$CFLAGS -Wdeclaration-after-statement"
		CFLAGS="$CFLAGS -Wredundant-decls"
		CFLAGS="$CFLAGS -Wmissing-noreturn"
		CFLAGS="$CFLAGS -Wcast-align"
		CFLAGS="$CFLAGS -Wwrite-strings"
		CFLAGS="$CFLAGS -Winline"
		CFLAGS="$CFLAGS -Wformat-nonliteral"
		CFLAGS="$CFLAGS -Wformat-security"
		CFLAGS="$CFLAGS -Wswitch-enum"
		CFLAGS="$CFLAGS -Wswitch-default"
		CFLAGS="$CFLAGS -Winit-self"
		CFLAGS="$CFLAGS -Wmissing-include-dirs"
		CFLAGS="$CFLAGS -Wundef"
		CFLAGS="$CFLAGS -Waggregate-return"
		CFLAGS="$CFLAGS -Wmissing-format-attribute"
		CFLAGS="$CFLAGS -Wnested-externs"
		CFLAGS="$CFLAGS -Wshadow"
	],
	[minimum], [
		CFLAGS="$CFLAGS -DG_DISABLE_CAST_CHECKS"
	],
	[no], [
		CFLAGS="$CFLAGS -DG_DISABLE_ASSERT"
		CFLAGS="$CFLAGS -DG_DISABLE_CHECKS"
		CFLAGS="$CFLAGS -DG_DISABLE_CAST_CHECKS"
	],
	[AC_MSG_ERROR([Unknown argument to --enable-debug])]
)
AC_SUBST(BAYES_DEBUG_CFLAGS)


dnl **************************************************************************
dnl Unit Tests
dnl **************************************************************************
AC_PATH_PROG([GTESTER], [gtester])
AC_PATH_PROG([GTESTER_REPORT], [gtester-report])
AM_CONDITIONAL(ENABLE_GLIB_TEST, test "x$enable_glibtest" = "xyes")


dnl **************************************************************************
dnl Miscellaneous
dnl **************************************************************************
GTK_DOC_CHECK([1.11])
GOBJECT_INTROSPECTION_CHECK([0.6.7])
SHAMROCK_EXPAND_LIBDIR
SHAMROCK_EXPAND_BINDIR
SHAMROCK_EXPAND_DATADIR


dnl **************************************************************************
dnl Output
dnl **************************************************************************
AC_CONFIG_FILES([
	Makefile
	bayes-glib-1.0.pc
	doc/Makefile
	doc/version.xml
])
AC_OUTPUT

echo ""
echo "Bayes-GLib $VERSION"
echo ""
echo "  Prefix.....................: ${prefix}"
echo "  Debug Level................: ${enable_debug}"
echo "  Compiler Flags.............: ${CFLAGS}"
echo "  Enable API Reference.......: ${enable_gtk_doc}"
echo "  Enable Test Suite..........: ${enable_glibtest}"
echo ""
