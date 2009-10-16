# Copyright © 2009 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#
# Authors:
#    Eric Anholt <eric@anholt.net>

dnl Process this file with autoconf to create configure.

AC_PREREQ([2.59])
AC_INIT([mesa-demos], [7.7],
    [https://bugs.freedesktop.org/enter_bug.cgi?product=Mesa])
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([dist-bzip2])

dnl Check for progs
AC_PROG_CPP
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Enable quiet compiles on automake 1.11.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Make sure the pkg-config macros are defined
m4_ifndef([PKG_PROG_PKG_CONFIG],
    [m4_fatal([Could not locate the pkg-config autoconf macros.
  These are usually located in /usr/share/aclocal/pkg.m4. If your macros
  are in a different location, try setting the environment variable
  ACLOCAL="aclocal -I/other/macro/dir" before running autoreconf.])])
PKG_PROG_PKG_CONFIG()

dnl Get the pkg-config definitions for libGL.  Probably want a fallback for
dnl non-pkgconfig here.
PKG_CHECK_MODULES(DEMO, [gl])

dnl Check for GLUT
GLUT_CFLAGS=""
GLUT_LIB=-lglut
glut_enabled=yes
AC_ARG_WITH([glut],
	[AS_HELP_STRING([--with-glut=DIR],
			[glut install directory])],
	[GLUT_CFLAGS="-I$withval/include"
	 GLUT_LIB="-L$withval/lib -lglut"])
AC_CHECK_HEADER([GL/glut.h],
		[],
		[glut_enabled=no])
AC_CHECK_LIB([glut],
		[glutInit],
		[],
		[glut_enabled=no])

dnl Check for GLEW.
GLEW_CFLAGS=""
GLEW_LIB=-lGLEW
AC_ARG_WITH([glew],
	[AS_HELP_STRING([--with-glew=DIR],
			[glew install directory])],
	[GLEW_CFLAGS="-I$withval/include"
	 GLEW_LIB="-L$withval/lib -lGLEW"])
AC_CHECK_HEADER([GL/glew.h],
		[],
		AC_MSG_ERROR([GLEW required]))
AC_CHECK_LIB([GLEW],
		[glewInit],
		[],
		AC_MSG_ERROR([GLEW required]))
DEMO_CFLAGS="$DEMO_CFLAGS $GLEW_CFLAGS"
DEMO_LIBS="$DEMO_CFLAGS $GLEW_LIBS"

dnl Compiler macros
case "$host_os" in
linux*|*-gnu*|gnu*)
    DEMO_CFLAGS="$DEMO_CFLAGS -D_GNU_SOURCE -DPTHREADS"
    ;;
solaris*)
    DEMO_CFLAGS="$DEFINES -DPTHREADS -DSVR4"
    ;;
cygwin*)
    DEMO_CFLAGS="$DEFINES -DPTHREADS"
    ;;
esac

dnl These should be unnecessary, but let the user set them if they want
AC_ARG_VAR([OPT_FLAGS], [Additional optimization flags for the compiler.
    Default is to use CFLAGS.])
AC_ARG_VAR([ARCH_FLAGS], [Additional architecture specific flags for the
    compiler. Default is to use CFLAGS.])
AC_SUBST([OPT_FLAGS])
AC_SUBST([ARCH_FLAGS])

dnl Set up C warning flags.  Copy of XORG_CWARNFLAGS.
if  test "x$GCC" = xyes ; then
    CWARNFLAGS="-Wall -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes \
-Wmissing-declarations -Wnested-externs -fno-strict-aliasing \
-Wbad-function-cast"
    case `$CC -dumpversion` in
    3.4.* | 4.*)
	CWARNFLAGS="$CWARNFLAGS -Wold-style-definition -Wdeclaration-after-statement"
	;;
    esac
else
    AC_CHECK_DECL([__SUNPRO_C], [SUNCC="yes"], [SUNCC="no"])
    if test "x$SUNCC" = "xyes"; then
	CWARNFLAGS="-v"
    fi
fi
DEMO_CFLAGS="$DEMO_CFLAGS $CWARNFLAGS"

PKG_CHECK_MODULES(XLIB, [x11 xshape], [enable_x11=no], [enable_x11=yes])

mesa_source_enabled=no
AC_ARG_WITH([mesa-source],
	[AS_HELP_STRING([--with-mesa-source=DIR],
			[Mesa source directory for finding gl_API.xml])],
	[mesa_source_enabled="yes"
	 mesa_source_dir="$withval"],
	[mesa_source_enabled="no"])
MESA_GLAPI="$mesa_source_dir/src/mesa/glapi"

dnl
dnl Program library dependencies
dnl    Only libm is added here if necessary as the libraries should
dnl    be pulled in by the linker
dnl
if test "x$APP_LIB_DEPS" = x; then
    case "$host_os" in
    solaris*)
	APP_LIB_DEPS="-lX11 -lsocket -lnsl -lm"
	;;
    cygwin*)
	APP_LIB_DEPS="-lX11"
	;;
    *)
	APP_LIB_DEPS="-lm"
        ;;
    esac
fi
AC_SUBST([APP_LIB_DEPS])

AC_SUBST([DEMO_CFLAGS])
AC_SUBST([DEMO_LIBS])
AC_SUBST([GLUT_CFLAGS])
AC_SUBST([GLUT_LIBS])
AC_SUBST([X11_CFLAGS])
AC_SUBST([X11_LIBS])
AC_SUBST([MESA_GLAPI])

AM_CONDITIONAL(HAVE_GLUT, test "x$glut_enabled" = "xyes")
AM_CONDITIONAL(HAVE_X11, test "x$x11_enabled" = "xyes")
AM_CONDITIONAL(BUILD_GLTRACE, false)
AM_CONDITIONAL(HAVE_MESA_SOURCE, test "x$mesa_source_enabled" = "xyes")

AC_OUTPUT([
	Makefile
	src/Makefile
	src/demos/Makefile
	src/fp/Makefile
	src/glsl/Makefile
	src/tests/Makefile
	src/tools/Makefile
	src/tools/trace/Makefile
	src/trivial/Makefile
	src/util/Makefile
	src/vp/Makefile
	src/vpglsl/Makefile
	src/wgl/Makefile
	src/windml/Makefile
	src/xdemos/Makefile
])
