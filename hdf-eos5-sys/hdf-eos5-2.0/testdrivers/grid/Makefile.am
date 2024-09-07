# testdrivers/grid Makefile.am

# Boilerplate definitions
include $(top_srcdir)/config/include.am

EXTRA_DIST = hdfeos5.inc hdfeos5_64.inc


# Link against HDF-EOS5 include files and both the HDF-EOS5 and libGCTP
# libraries
AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(LIBHDFEOS5) $(LIBGCTP)

# Build TestGrid.  Since no sources are listed, build it from TestGrid.c
check_PROGRAMS = TestGrid

if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
check_PROGRAMS += testgrid32
if SZIP_ENCODER_CONDITIONAL
testgrid32_SOURCES = testgrid_szip32.f
else
testgrid32_SOURCES = testgrid32.f
endif
else
check_PROGRAMS += testgrid64
if SZIP_ENCODER_CONDITIONAL
testgrid64_SOURCES = testgrid_szip64.f
else
testgrid64_SOURCES = testgrid64.f
endif
endif
endif

# Run TestGrid as a test when the user types 'make check'
TEST_SEQUENCES=test_c
VERBOSE_TEST_SEQUENCES=test_c_verbose
if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
TEST_SEQUENCES+=test_32f
VERBOSE_TEST_SEQUENCES+=test_32f
else
TEST_SEQUENCES+=test_64f
VERBOSE_TEST_SEQUENCES+=test_64f
endif
endif

test_c: $(check_PROGRAMS)
	./TestGrid
test_c_verbose: $(check_PROGRAMS)
	./TestGrid -v h
test_32f: $(check_PROGRAMS)
	./testgrid32
test_64f: $(check_PROGRAMS)
	./testgrid64

check-local: $(TEST_SEQUENCES)
verbose_check: $(VERBOSE_TEST_SEQUENCES)

.PHONY: $(TEST_SEQUENCES)

