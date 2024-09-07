###################################
#       -*- Autoconf -*-
###################################



###################################
# Boilerplate autoconf setup
###################################
AC_PREREQ([2.59])
AC_INIT([hdf-eos5], [2.0], null@bogus.email)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

## Note we disable shared libraries otherwise gfortran fails linking against the resulting
## libX.la files. With shared libraries disables, libtool links gfortran against the static
## library libX.a. If fortran is disabled, this can be overridden using --enabled-shared
## with configure

AC_DISABLE_SHARED
AC_PROG_LIBTOOL



###################################
# Files that need to be configured
###################################

AC_CONFIG_HEADER([include/HE5_config.h])
AC_CONFIG_FILES([Makefile
                include/Makefile
                src/Makefile
                gctp/Makefile
                gctp/include/Makefile
                gctp/src/Makefile
                samples/Makefile
                testdrivers/Makefile
                testdrivers/grid/Makefile
                testdrivers/point/Makefile
                testdrivers/swath/Makefile
                testdrivers/za/Makefile
                testdrivers/threads/Makefile
                doc/Makefile])


###################################
#     Additional Macros
###################################
m4_include(m4/stdlib.m4)
m4_include(m4/stdinc.m4)



###################################
#       Start checks
###################################

AC_LANG(C)

# Check for the math library that HDF4 needs
#
AC_CHECK_LIB([m], [ceil])


######################################################################
#  Check size of some types
# We pass these through directly via the cpp flags rather than the config
# file to make it easier (given the current code state) to get it where
# it needs to go.
######################################################################
AC_CHECK_SIZEOF(long)
CPPFLAGS="${CPPFLAGS} -DSIZEOF_LONG=$ac_cv_sizeof_long"



###################################
#  Check endian-ness
###################################
AC_C_BIGENDIAN([
    AC_DEFINE([BIG_ENDIAN],[1],[Define if system is big-endian])
    CPPFLAGS="${CPPFLAGS} -DBIG_ENDIAN=1"
])


###################################
#  pthread library checks
# This will be needed if the HDF5
# library is threaded, otherwise it
# will be ignored.
###################################
AC_CHECK_LIB([pthread], [pthread_create])


###################################
#  ZLIB library checks
###################################

# HDFEOS does not need libz directly, but the HDF library might. This means
# we only need to find the libary, not the header files.

HDF_CHECK_LIB([zlib],[z],[compress2],[,/usr/local/lib,/opt/local/lib,/usr/lib64])
AS_IF(
    [test -z "$HAVE_LIBZ"],
    [AC_MSG_ERROR(unable to find libz)]
)



###################################
#  FORTRAN compiler check
###################################

# User may enable fortran components explicity with --enable-fortran
# or --enable-fortran=yes

AC_PROG_F77
AC_PROG_FC

AC_ARG_ENABLE(
    [fortran],
    AS_HELP_STRING([--enable-fortran], [Enable fortran components/tests]),,)

# Check to see if explicitly disabled

AS_IF(
    [test "x$enable_fortran" != "xyes"],
    [
         FC=""
         F77=""
    ])

AS_IF([test -n "$FC"],
    [
        AC_CHECK_SIZEOF([int*])
        AS_IF(
            [test "x$ac_cv_sizeof_intp" == "x4"],
            [
                F2CFORTRAN_32PTR="yes"
            ],[
                AC_MSG_NOTICE([Non-32 bit system; some fortran tests will be skipped])
            ])

        AC_MSG_CHECKING([fortran compiler])
        AC_LANG_PUSH([Fortran])
        AC_LINK_IFELSE(
            [
                AC_LANG_PROGRAM([], [
                    INTEGER :: I
                    REAL :: R
                    I=TRANSFER(R,I)
                ])
            ],[
                AC_MSG_RESULT([Fortran 90 code was successfully compiled; assume f90 or later])
                F2CFORTRAN_90="yes"
            ],[
                AC_MSG_RESULT([Fortran 90 code format was not compiled; assume f77])
            ])
        AC_LANG_POP([Fortran])

        F2CFORTRAN_MACRO="yes"
        CPPFLAGS="$CPPFLAGS -Df2cFortran=1"
    ],[
        AC_MSG_NOTICE([Fortran compiler not found])
        F2CFORTRAN_MACRO="no"
    ])



###################################
#  SZLIB checks
###################################

dnl --------------------------------------------------
dnl Check whether szlib is enabled.
dnl If HDF4 is built with szlib, user should use --with-szlib=DIR option.
dnl If this option is given, whether SZIP is decoder only or decoder plus encoder
dnl should be checked.

AC_ARG_WITH([szlib],
            [AC_HELP_STRING([--with-szlib=DIR],
                            [Use szlib library for external szlib I/O
                             filter.  Linker must be able to find szlib
                             if HDF4 was build with szip.])],,
            withval=default)

AS_IF(
    ## if withval==no szlib is disabled
    ##
    [test "x$withval" == "xno"],
    [
        HAVE_LIBSZ="no"
    ],


    ## Look for it, but do not abort if we can't find it
    ##
    [test "x$withval" == "xdefault"],
    [
        HDF_CHECK_INC_PATH([szlib.h],[,/usr/local/include,/opt/local/include,/opt/local/lib/libaec/include])
        HDF_CHECK_LIB_PATH([sz],[SZ_BufftoBuffCompress],[,/usr/local/lib,/opt/local/lib,/opt/local/lib/libaec/lib,/usr/lib64])
    ],


    ## if withval==yes We need to find szlib
    ##
    [test "x$withval" == "xyes"],
    [

        HDF_CHECK_INC_PATH([szlib.h],[,/usr/local/include,/opt/local/include,/opt/local/lib/libaec/include])
        AS_IF(
            [test "x$ac_cv_header_szlib_h" != "xyes"],
            [AC_MSG_ERROR([couldn't find szlib header])]
        )

        HDF_CHECK_LIB_PATH([sz],[SZ_BufftoBuffCompress],[,/usr/local/lib,/opt/local/lib,/opt/local/lib/libaec/lib,/usr/lib64])
        AS_IF(
            [test "x$HAVE_LIBSZ" != "xyes"],
            [AC_MSG_ERROR([couldn't find szlib library])]
        )
    ],

    ##  Else
    ##
    [
        szlib_inc="`echo $withval |awk -F, '{print $1}'`"
        szlib_lib="`echo $withval |awk -F, '{print $2}'`"
        AS_IF(
            [test -z "$szlib_inc"],
            [
                # No options provided at all
            ],
            [
                # A single option provided - use it for both inc and lib, but add suffix
                AS_IF(
                    [test -z "$szlib_lib"],
                    [
                        szlib_lib="$szlib_inc/lib"
                        szlib_inc="$szlib_inc/include"
                    ])
                CPPFLAGS="$CPPFLAGS -I$szlib_inc"
                LDFLAGS="$LDFLAGS -L$szlib_lib"
            ])

        AC_CHECK_HEADERS(
            [szlib.h],,
            [
                AC_MSG_ERROR([couldn't find szlib header file])
            ])
        AC_CHECK_LIB(
            [sz],
            [SZ_BufftoBuffCompress],,
            [
                AC_MSG_ERROR([couldn't find szlib library])
            ])

        HAVE_LIBSZ="yes"
    ])



# If szlib is available, additional checks are required to determine the
# available functionality. By default decoding is assumed. We are really just
# interested in whether or not encoding is also enabled.

AS_IF(
    [test "x$HAVE_LIBSZ" == "xyes"],
    [
        # Check for encoding functionality

        AC_MSG_CHECKING([if szlib is functional])
        AC_TRY_RUN(
            [
                #include <stddef.h>
                #include <szlib.h>

                int main(void)
                {
                    SZ_encoder_enabled();
                    exit(0);
                }
            ],[
                AC_MSG_RESULT([yes])
            ],[
                AC_MSG_ERROR([szlib is not functional])
            ])
        AC_DEFINE(HAVE_FILTER_SZIP, 1, [Define if support for szip filter is enabled])

        AC_MSG_CHECKING([szlib encoding])
        AC_TRY_RUN(
            [
                #include <stddef.h>
                #include <szlib.h>

                int main(void)
                {
                    /* SZ_encoder_enabled returns 1 if encoder is present */
                    if(SZ_encoder_enabled() == 1)
                        exit(0);
                    else
                        exit(1);
                }
            ],[
                AC_MSG_RESULT([yes])
                AC_DEFINE(HAVE_FILTER_SZIP_ENCODER, 1, [Define if support for szip encoder filter is enabled])
                USE_SZIP_ENCODER="yes"
            ],[
                AC_MSG_RESULT([no])
            ])
    ])



###################################
#  Check for HDF5 library
###################################

# Note that this is a required component - we cannot build HDF-EOS5 without it

AC_ARG_WITH(
    [hdf5],
    [AC_HELP_STRING(
        [--with-hdf5=DIR],
        [Specify path to HDF5 library to use while building
        [default=yes]]
    )],,
    withval=yes)


AS_IF(
    [test "x$withval" == "xno"],
    [
        AC_MSG_ERROR([HDF5 is a required dependency.])
    ],

    # No options, so we must try and find the HDF4 library

    [test "x$withval" == "xyes"],
    [
        # Test some library locations to see if we can find the HDF libraries

        HDF_CHECK_LIB_PATH([hdf5],[H5Fcreate],[,/usr/local/lib,/opt/local/lib,/usr/lib64,/usr/lib/x86_64-linux-gnu/hdf5/serial])

        # Check to see if we can find the HDF5 header file

        HDF_CHECK_INC_PATH([hdf5.h],[,/usr/local/include,/opt/local/include,/usr/include/hdf5/serial])
    ],


    # A user specified directory (one or two)

    [
        hdf5_inc="`echo $withval |awk -F, '{print $1}'`"
        hdf5_lib="`echo $withval |awk -F, '{print $2}'`"
        AS_IF(
            [test -z "$hdf5_lib"],
            [
                hdf5_lib="$hdf5_inc/lib"
                hdf5_inc="$hdf5_inc/include"
            ])

        HDF_CHECK_LIB_PATH([hdf5],[H5Fcreate],[,$hdf5_lib])
        HDF_CHECK_INC_PATH([hdf5.h],[,$hdf5_inc])
    ])

# Verify presence of all required headers and libraries

AS_IF(
    [test -z "$HAVE_LIBHDF5"],
    [AC_MSG_ERROR(unable to find HDF5 library)]
)
AS_IF(
    [test "x$ac_cv_header_hdf5_h" != "xyes"],
    [AC_MSG_ERROR([couldn't find HDF5 library header files])]
)


###################################
#  Check for High Level HDF5 lib
###################################
AC_CHECK_LIB([hdf5_hl],[H5DSset_label])



###################################
#  Check for thread safety in
#  HDF5 library
###################################

AC_MSG_CHECKING([if HDF5 threadsafe mode is enabled])
AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <hdf5.h>
        ],[
            #ifdef H5_HAVE_THREADSAFE
                return 0;
            #else
                return 1;
            #endif
        ])
    ],[
        AC_MSG_RESULT([yes])
        HDF5_THREADSAFE="yes"
        AS_IF(
            [test "x$ac_cv_lib_pthread_pthread_create" == "xno"],
            [
                AC_MSG_ERROR([HDF5 is multi-threaded, but no threading library found])
            ])
    ],
    [
        AC_MSG_RESULT([no])
        HDF5_THREADSAFE="no"
    ])

# Record threadsafe status in config.h and for Makefiles

if test "x$HDF5_THREADSAFE" = "xyes"; then
    AC_DEFINE([_HDFEOS5_THREADSAFE], [1], [Define if threadsafe HDF-EOS is enabled])
fi
AM_CONDITIONAL([HDF5_THREADSAFE], [test "x$HDF5_THREADSAFE" = "xyes"])


###################################
#  Check for H5free_memory in
#  HDF5 library
###################################
AC_CHECK_FUNCS([H5free_memory])

###################################
#  Check for SZIP in HDF5 library
###################################

# At this point, we know we can link against hdf5.

AC_MSG_CHECKING([for hdf5 szip decoding filter])
AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <H5Ipublic.h>
            #include <H5Zpublic.h>
        ],[
            herr_t ret;
            unsigned int flags = 0;
            int decoder = 0;

            ret = H5Zget_filter_info(H5Z_FILTER_SZIP, &flags);
            if (ret < 0) exit(1);
            decoder = flags & H5Z_FILTER_CONFIG_DECODE_ENABLED;
            if (decoder)
                exit(0);
            else
                exit(1);
        ])
    ],
    [
        AC_MSG_RESULT([yes])

        # HDF5 supports decoding. We have to make sure szlib is included. In the case
        # of static library linkage, this would already have been verified (the program
        # wouldn't run), but shared object libraries might use lazy resolution, so we
        # verify
        AS_IF(
            [test "x$HAVE_LIBSZ" != "xyes"],
            [
                AC_MSG_ERROR([HDF5 requires SZIP, please use --with-szlib])
            ])

        AC_DEFINE(HAVE_HDF5_SZIP_DECODER, 1, [Defined if HDF5 has szip decoder filter])
        AC_DEFINE(H5_HAVE_FILTER_SZIP, 1, [Define if HDF5 has szip any type of filter])
    ],[
        AC_MSG_RESULT([no])
    ])



AC_MSG_CHECKING([for hdf5 szip encoding support])
AC_RUN_IFELSE(
    [AC_LANG_PROGRAM(
        [
            #include <H5Ipublic.h>
            #include <H5Zpublic.h>
        ],[
            herr_t ret;
            unsigned int flags = 0;
            int encoder = 0;

            ret = H5Zget_filter_info(H5Z_FILTER_SZIP, &flags);
	        if (ret < 0) exit(1);
            encoder = flags & H5Z_FILTER_CONFIG_ENCODE_ENABLED;
            if (encoder)
                exit(0);
            else
                exit(1);
        ])
    ],[
        AC_MSG_RESULT([yes])
        AS_IF(
            [test "x$USE_SZIP_ENCODER" != "xyes"],
            [
                AC_MSG_ERROR([HDF5 requires SZIP with encoding support])
            ])
        AC_DEFINE(HAVE_HDF5_SZIP_ENCODER, 1, [Define if HDF5 has szip encoder filter])
    ],[
        AC_MSG_RESULT([no])
    ])



###################################
#  Enable/disable installation of
# GCTP headers (usually not needed)
###################################
AC_ARG_ENABLE(
    [gctp-include],
    AS_HELP_STRING([--enable-gctp-include], [Enable installation of GCTP library headers]),,)

AM_CONDITIONAL([GCTP_INCLUDE_CONDITIONAL], [test "x$enable_gctp_include" == "xyes"])





###################################
#  Miscellaneous checks
###################################

CFLAGS=$CFLAGS

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h malloc.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MEMCMP
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memchr memmove memset pow sqrt strchr strstr])



# Use version 1 of the hdf5 API

AC_DEFINE([H5_USE_16_API], [1], [Define if HDF-EOS5 should use version 1 api])


AM_CONDITIONAL([F2CFORTRAN_CONDITIONAL], [test "x$F2CFORTRAN_MACRO" = "xyes"])
AM_CONDITIONAL([F2CFORTRAN_90_CONDITIONAL], [test "x$F2CFORTRAN_90" = "xyes"])
AM_CONDITIONAL([F2CFORTRAN_32PTR_CONDITIONAL], [test "x$F2CFORTRAN_32PTR" = "xyes"])
AM_CONDITIONAL([SZIP_ENCODER_CONDITIONAL], [test "x$H5_HAVE_FILTER_SZIP" = "xyes"])


AC_OUTPUT


###################################
# SUMMARY
###################################

AC_MSG_NOTICE([------------------------------------------])
AC_MSG_NOTICE([        Configuration Summary])
AC_MSG_NOTICE([------------------------------------------])

AC_MSG_NOTICE([ ])
AC_MSG_NOTICE([CC:                       $CC])
AC_MSG_NOTICE([FC:                       $FC])
AC_MSG_NOTICE([F77:                      $F77])
AC_MSG_NOTICE([C flags:                  $CFLAGS])
AC_MSG_NOTICE([CPP flags:                $CPPFLAGS])
AC_MSG_NOTICE([LD flags:                 $LDFLAGS])
AC_MSG_NOTICE([LIB flags:                $LIBS])
