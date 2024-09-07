# testdrivers/za Makefile.am

# Boilerplate definitions
include $(top_srcdir)/config/include.am

EXTRA_DIST = hdfeos5.inc hdfeos5_64.inc

# Link against HDF-EOS5 and libGCTP libraries
AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(LIBHDFEOS5) $(LIBGCTP)

# Programs to build when user types 'make check'
check_PROGRAMS = TestZa TestAlias

if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
check_PROGRAMS += testza32
testza32_SOURCES = testza32.f
else
check_PROGRAMS += testza64
testza64_SOURCES = testza64.f
endif
endif

# When user types 'make check', run programs above as tests.
TEST_SEQUENCES=test_c
if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
TEST_SEQUENCES+=test_32f
else
TEST_SEQUENCES+=test_64f
endif
endif

test_c: $(check_PROGRAMS)
	./TestZa
	./TestAlias
test_32f: $(check_PROGRAMS)
	./testza32
test_64f: $(check_PROGRAMS)
	./testza64

check-local: $(TEST_SEQUENCES)

.PHONY: $(TEST_SEQUENCES)

