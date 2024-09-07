# testdrivers/point Makefile.am

# Boilerplate definitions
include $(top_srcdir)/config/include.am

EXTRA_DIST = hdfeos5.inc hdfeos5_64.inc fixedBuoy0.txt fixedBuoy1.txt floatBuoy0.txt floatBuoy1.txt simple.txt 


# Link against HDF-EOS5 and libGCTP
AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(LIBHDFEOS5) $(LIBGCTP)

# Build TestPoint from TestPoint.c
check_PROGRAMS = TestPoint

if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
check_PROGRAMS += testpoint32
testpoint32_SOURCES = testpoint32.f
else
check_PROGRAMS += testpoint64
testpoint64_SOURCES = testpoint64.f
endif
endif

# Run TestPoint as a test when the user types 'make check'
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
	./TestPoint
test_c_verbose: $(check_PROGRAMS)
	./TestPoint -v h
test_32f: $(check_PROGRAMS)
	./testpoint32
test_64f: $(check_PROGRAMS)
	./testpoint64

check-local: $(TEST_SEQUENCES)
verbose_check: $(VERBOSE_TEST_SEQUENCES)

.PHONY: $(TEST_SEQUENCES)

