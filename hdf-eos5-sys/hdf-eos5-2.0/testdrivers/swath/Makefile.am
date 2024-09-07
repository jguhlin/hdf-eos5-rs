# testdrivers/swath Makefile.am

# Boilerplate definitions
include $(top_srcdir)/config/include.am

EXTRA_DIST = hdfeos5.inc hdfeos5_64.inc

# Link against HDF-EOS5 library
AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(LIBHDFEOS5) $(LIBGCTP)

# Build TestSwath from TestSwath.c
check_PROGRAMS = TestSwath

if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
check_PROGRAMS += testswath32
testswath32_SOURCES = testswath32.f
else
check_PROGRAMS += testswath64
testswath64_SOURCES = testswath64.f
endif
endif

# Run TestSwath when user types 'make check'
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
	./TestSwath
test_c_verbose: $(check_PROGRAMS)
	./TestSwath -v h
test_32f: $(check_PROGRAMS)
	./testswath32
test_64f: $(check_PROGRAMS)
	./testswath64

check-local: $(TEST_SEQUENCES)
verbose_check: $(VERBOSE_TEST_SEQUENCES)

.PHONY: $(TEST_SEQUENCES)

