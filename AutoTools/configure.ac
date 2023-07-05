#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gusto], [0.69], [tony.buckley0000@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/gusto.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Checks for libraries.
not_inst=""
AC_SEARCH_LIBS([pthread_create], [pthread], [l_pthread=yes],  \
		[not_inst="${not_inst} pthread"])
AC_SEARCH_LIBS([jpeg_set_defaults], [jpeg], [l_jpeg=yes],     \
		[not_inst="${not_inst} libjpeg-dev"])
AC_SEARCH_LIBS([png_create_write_struct], [png], [l_png=yes], \
		[not_inst="${not_inst} libpng-dev"])
# FIXME: Do we need gstreamer & gtk lib checks?
AC_SEARCH_LIBS([gtk_init], [gtk-3], [l_gtk=yes], [])
AC_SEARCH_LIBS([gst_init], [gstreamer-1.0], [l_gst=yes], []) 


# Checks for standard header files (unlikely not to be present).
AC_CHECK_HEADERS([fcntl.h inttypes.h malloc.h stdint.h stdio.h \
                  time.h time.h unistd.h ctype.h stat.h types.h \
                  libgen.h errno.h])

if test "x${l_pthread}" = xyes; then
  AC_CHECK_HEADERS([pthread.h], [], [h_pthreads=no; not_inst="${not_inst}  pthread"])
fi

if test "x${l_jpeg}" = xyes; then
  AC_CHECK_HEADERS([jpeglib.h], [], [h_jpeg=no; not_inst="${not_inst}  libjpeg-dev"])
fi

if test "x${l_png}" = xyes; then
  AC_CHECK_HEADERS([png.h], [], [h_png=no; not_inst="${not_inst}  libpng-dev"])
fi

# GTK & Gstreamer development needs to be available
if test "x${l_gtk}" = xyes; then
  AC_CHECK_HEADERS([gtk.h], [], [h_gtk=no])
fi

if test "x${l_gst}" = xyes; then
  AC_CHECK_HEADERS([gst.h], [], [h_gst=no])
fi

# Other GTK and GST headers
AC_CHECK_HEADERS([gdkkeysyms.h glib.h video-format.h videooverlay.h \
	   	  gtkfontchooser.h videodev2.h pbutils.h gtk.h gst.h])

if test "x${l_gtk}" != xyes || test "x{h_gtk]" = "xno"; then
  echo " "
  echo "------------------------------------------"
  echo " It appears that the GTK development libraries and/or header files  "
  echo " are not pressent on this system.  "
  echo " To install try:    [sudo] apt-get install libgtk-3-dev  "
  echo " Note that depending on what development tools are present  "
  echo " you may need to run:    [sudo] apt-get build-dep libgtk-3-dev "
  echo "------------------------------------------"
fi

if test "x${l_gst}" != xyes || test "x{h_gst]" = "xno"; then
  echo " "
  echo "------------------------------------------"
  echo " It appears that the GStreamer development libraries and/or header files  "
  echo " are not pressent on this system.  "
  echo " To install try:    "
  echo "     [sudo] add-apt-repository ppa:gstreamer-developers/ppa  "
  echo "     [sudo] apt-get update  "
  echo "     [sudo] apt-get install gstreamer1.0* "
  echo "     [sudo] apt-get install gstreamer-video-1.0 "
  echo "     [sudo] apt-get install gstreamer-pbutils-1.0 "
  echo "     [sudo] apt-get install gstreamer-tools "
  echo "     [sudo] apt-get install libgstreamer-plugins-base1.0-dev "
  echo "------------------------------------------"
fi

if test "x${not_inst}" != x; then
  echo "------------------------------------------"
  echo " Unable to find one or more libraries and/or header files on this system.  "
  echo " The following will need to be installed to build Gusto:  "
  echo ${not_inst}

  echo " "
  echo " For example on Ubuntu or Debian based platforms try the following:  "
  echo "[sudo] apt-get install ${not_inst}  "
  echo "------------------------------------------"
fi

# pkg-config module checks for cflags and linker flags
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([X], [gtk+-3.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-pbutils-1.0])


# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([clock_gettime getpagesize memset mkdir select strchr strerror strstr strtol])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile
                 data/icons/Makefile])
AC_OUTPUT

echo \
"-------------------------------------------------

 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}

 Prefix: '${prefix}'.
 Compiler: '${CC} ${CFLAGS} ${CPPFLAGS}'

 Now type 'make @<:@<target>@:>@'
   where the optional <target> is:
     all                - build all binaries
     install            - install everything

--------------------------------------------------"
