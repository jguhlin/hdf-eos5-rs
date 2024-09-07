# testdrivers/threads Makefile.am

# Boilerplate definitions
include $(top_srcdir)/config/include.am

# Link against HDF-EOS5, libGCTP, and the pthread library.
## This directory shouldn't be built unless configure detects that the
## pthread library is present.
AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(LIBHDFEOS5) $(LIBGCTP) pthread

# Programs to build when user types 'make check'.
# Each of these is build from a single *.c file.
check_PROGRAMS = he5_gd_threads-1 he5_gd_threads-2 he5_pt_threads-1 \
                 he5_pt_threads-2 he5_sw_threads-1 he5_sw_threads-2

# Run all programs above as tests when the user types 'make check'
TESTS = $(check_PROGRAMS)

