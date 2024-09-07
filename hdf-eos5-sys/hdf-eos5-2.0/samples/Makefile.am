# samples Makefile.am
# Sample programs for HDF-EOS5

# Boilerplate definitions file
include $(top_srcdir)/config/include.am

EXTRA_DIST = hdfeos5.inc hdfeos5_64.inc fixedBuoy0.txt fixedBuoy1.txt floatBuoy0.txt floatBuoy1.txt simple.txt

# Link against GCTP library and HDF-EOS5
AM_CPPFLAGS=-I$(top_srcdir)/include -I$(top_srcdir)/gctp/include

LDADD=$(LIBHDFEOS5) $(LIBGCTP)

# The sample programs to be built when the user types 'make check'
check_PROGRAMS=he5_gd_datainfo he5_gd_defexternalfld he5_gd_definefields     \
      he5_gd_defunlimfld he5_gd_info he5_gd_rdexternaldata he5_gd_readdata   \
      he5_gd_writedata he5_gd_setup he5_gd_subset he5_gd_wrexternaldata	     \
      he5_gd_wrunlimfld     						     \
      he5_pt_datainfo he5_pt_definelevels he5_pt_readattrs he5_pt_readdata   \
      he5_pt_setup he5_pt_updatelevels he5_pt_writeattrs he5_pt_writedata    \
      he5_sw_datainfo he5_sw_defexternalfld he5_sw_definefields              \
      he5_sw_defunlimfld he5_sw_info he5_sw_rdexternaldata he5_sw_readdata   \
      he5_sw_setup he5_sw_subset he5_sw_wrextendrd he5_sw_wrexternaldata     \
      he5_sw_writedata he5_sw_wrunlimfld he5_za_datainfo                     \
      he5_za_defexternalfld he5_za_definefields he5_za_info                  \
      he5_za_rdexternaldata he5_za_readdata he5_za_setup                     \
      he5_za_wrexternaldata he5_za_writedata
if F2CFORTRAN_CONDITIONAL
	AM_LIBTOOLFLAGS=--tag=FC
if F2CFORTRAN_32PTR_CONDITIONAL
check_PROGRAMS += \
	he5_gd_definefieldsF_32 \
	he5_gd_infoF_32 \
	he5_gd_readdataF_32 \
	he5_gd_setupF_32 \
	he5_gd_subsetF_32 \
	he5_gd_writedataF_32 \
	he5_pt_datainfoF_32 \
	he5_pt_definelevelsF_32 \
	he5_pt_readattrsF_32 \
	he5_pt_readdataF_32 \
	he5_pt_setupF_32 \
	he5_pt_updatelevelsF_32 \
	he5_pt_writeattrsF_32 \
	he5_pt_writedataF_32 \
	he5_sw_defexternalfldF_32 \
	he5_sw_definefieldsF_32 \
	he5_sw_infoF_32 \
	he5_sw_rdexternaldataF_32 \
	he5_sw_readdataF_32 \
	he5_sw_setupF_32 \
	he5_sw_subsetF_32 \
	he5_sw_wrexternaldataF_32 \
	he5_sw_writedataF_32 \
	he5_za_defexternalfldF_32 \
	he5_za_definefieldsF_32 \
	he5_za_infoF_32 \
	he5_za_rdexternaldataF_32 \
	he5_za_readdataF_32 \
	he5_za_setupF_32 \
	he5_za_wrexternaldataF_32 \
	he5_za_writedataF_32
he5_gd_definefieldsF_32_SOURCES = he5_gd_definefieldsF_32.f
he5_gd_infoF_32_SOURCES = he5_gd_infoF_32.f
he5_gd_readdataF_32_SOURCES = he5_gd_readdataF_32.f
he5_gd_setupF_32_SOURCES = he5_gd_setupF_32.f
he5_gd_subsetF_32_SOURCES = he5_gd_subsetF_32.f
he5_gd_writedataF_32_SOURCES = he5_gd_writedataF_32.f
he5_pt_datainfoF_32_SOURCES = he5_pt_datainfoF_32.f
he5_pt_definelevelsF_32_SOURCES = he5_pt_definelevelsF_32.f
he5_pt_readattrsF_32_SOURCES = he5_pt_readattrsF_32.f
he5_pt_readdataF_32_SOURCES = he5_pt_readdataF_32.f
he5_pt_setupF_32_SOURCES = he5_pt_setupF_32.f
he5_pt_updatelevelsF_32_SOURCES = he5_pt_updatelevelsF_32.f
he5_pt_writeattrsF_32_SOURCES = he5_pt_writeattrsF_32.f
he5_pt_writedataF_32_SOURCES = he5_pt_writedataF_32.f
he5_sw_defexternalfldF_32_SOURCES = he5_sw_defexternalfldF_32.f
if SZIP_ENCODER_CONDITIONAL
he5_sw_definefieldsF_32_SOURCES = he5_sw_definefields_szipF_32.f
else
he5_sw_definefieldsF_32_SOURCES = he5_sw_definefieldsF_32.f
endif
he5_sw_infoF_32_SOURCES = he5_sw_infoF_32.f
he5_sw_rdexternaldataF_32_SOURCES = he5_sw_rdexternaldataF_32.f
he5_sw_readdataF_32_SOURCES = he5_sw_readdataF_32.f
he5_sw_setupF_32_SOURCES = he5_sw_setupF_32.f
he5_sw_subsetF_32_SOURCES = he5_sw_subsetF_32.f
he5_sw_wrexternaldataF_32_SOURCES = he5_sw_wrexternaldataF_32.f
he5_sw_writedataF_32_SOURCES = he5_sw_writedataF_32.f
he5_za_defexternalfldF_32_SOURCES = he5_za_defexternalfldF_32.f
he5_za_definefieldsF_32_SOURCES = he5_za_definefieldsF_32.f
he5_za_infoF_32_SOURCES = he5_za_infoF_32.f
he5_za_rdexternaldataF_32_SOURCES = he5_za_rdexternaldataF_32.f
he5_za_readdataF_32_SOURCES = he5_za_readdataF_32.f
he5_za_setupF_32_SOURCES = he5_za_setupF_32.f
he5_za_wrexternaldataF_32_SOURCES = he5_za_wrexternaldataF_32.f
he5_za_writedataF_32_SOURCES = he5_za_writedataF_32.f
else
check_PROGRAMS += \
	he5_gd_definefieldsF_64 \
	he5_gd_infoF_64 \
	he5_gd_readdataF_64 \
	he5_gd_setupF_64 \
	he5_gd_subsetF_64 \
	he5_gd_writedataF_64 \
	he5_pt_datainfoF_64 \
	he5_pt_definelevelsF_64 \
	he5_pt_readattrsF_64 \
	he5_pt_readdataF_64 \
	he5_pt_setupF_64 \
	he5_pt_updatelevelsF_64 \
	he5_pt_writeattrsF_64 \
	he5_pt_writedataF_64 \
	he5_sw_defexternalfldF_64 \
	he5_sw_definefieldsF_64 \
	he5_sw_infoF_64 \
	he5_sw_rdexternaldataF_64 \
	he5_sw_readdataF_64 \
	he5_sw_setupF_64 \
	he5_sw_subsetF_64 \
	he5_sw_wrexternaldataF_64 \
	he5_sw_writedataF_64 \
	he5_za_defexternalfldF_64 \
	he5_za_definefieldsF_64 \
	he5_za_infoF_64 \
	he5_za_rdexternaldataF_64 \
	he5_za_readdataF_64 \
	he5_za_setupF_64 \
	he5_za_wrexternaldataF_64 \
	he5_za_writedataF_64
he5_gd_definefieldsF_64_SOURCES = he5_gd_definefieldsF_64.f
he5_gd_infoF_64_SOURCES = he5_gd_infoF_64.f
he5_gd_readdataF_64_SOURCES = he5_gd_readdataF_64.f
he5_gd_setupF_64_SOURCES = he5_gd_setupF_64.f
he5_gd_subsetF_64_SOURCES = he5_gd_subsetF_64.f
he5_gd_writedataF_64_SOURCES = he5_gd_writedataF_64.f
he5_pt_datainfoF_64_SOURCES = he5_pt_datainfoF_64.f
he5_pt_definelevelsF_64_SOURCES = he5_pt_definelevelsF_64.f
he5_pt_readattrsF_64_SOURCES = he5_pt_readattrsF_64.f
he5_pt_readdataF_64_SOURCES = he5_pt_readdataF_64.f
he5_pt_setupF_64_SOURCES = he5_pt_setupF_64.f
he5_pt_updatelevelsF_64_SOURCES = he5_pt_updatelevelsF_64.f
he5_pt_writeattrsF_64_SOURCES = he5_pt_writeattrsF_64.f
he5_pt_writedataF_64_SOURCES = he5_pt_writedataF_64.f
he5_sw_defexternalfldF_64_SOURCES = he5_sw_defexternalfldF_64.f
if SZIP_ENCODER_CONDITIONAL
he5_sw_definefieldsF_64_SOURCES = he5_sw_definefields_szipF_64.f
else
he5_sw_definefieldsF_64_SOURCES = he5_sw_definefieldsF_64.f
endif
he5_sw_infoF_64_SOURCES = he5_sw_infoF_64.f
he5_sw_rdexternaldataF_64_SOURCES = he5_sw_rdexternaldataF_64.f
he5_sw_readdataF_64_SOURCES = he5_sw_readdataF_64.f
he5_sw_setupF_64_SOURCES = he5_sw_setupF_64.f
he5_sw_subsetF_64_SOURCES = he5_sw_subsetF_64.f
he5_sw_wrexternaldataF_64_SOURCES = he5_sw_wrexternaldataF_64.f
he5_sw_writedataF_64_SOURCES = he5_sw_writedataF_64.f
he5_za_defexternalfldF_64_SOURCES = he5_za_defexternalfldF_64.f
he5_za_definefieldsF_64_SOURCES = he5_za_definefieldsF_64.f
he5_za_infoF_64_SOURCES = he5_za_infoF_64.f
he5_za_rdexternaldataF_64_SOURCES = he5_za_rdexternaldataF_64.f
he5_za_readdataF_64_SOURCES = he5_za_readdataF_64.f
he5_za_setupF_64_SOURCES = he5_za_setupF_64.f
he5_za_wrexternaldataF_64_SOURCES = he5_za_wrexternaldataF_64.f
he5_za_writedataF_64_SOURCES = he5_za_writedataF_64.f
endif
endif

# Tests consist of several programs run in succession.  Each of these
# names is the name of such a "sequence" of tests.
TEST_SEQUENCES=gd_write_test gd_unlim_test pt_write_test sw_write_test \
       sw_unlim_test sw_extend_test za_write_test
if F2CFORTRAN_CONDITIONAL
if F2CFORTRAN_32PTR_CONDITIONAL
TEST_SEQUENCES+=gd_write_test_32f pt_write_test_32f sw_write_test_32f za_write_test_32f
else
TEST_SEQUENCES+=gd_write_test_64f pt_write_test_64f sw_write_test_64f za_write_test_64f
endif
endif

# 'make check' should run each of the test sequences above
check-local: $(TEST_SEQUENCES)

# Manually list the tests to be run in order for each test sequence.
gd_write_test: $(check_PROGRAMS)
	./he5_gd_setup
	./he5_gd_definefields
	./he5_gd_writedata
	./he5_gd_readdata
	./he5_gd_info
	./he5_gd_datainfo
	./he5_gd_subset
gd_write_test_32f: $(check_PROGRAMS)
	./he5_gd_setupF_32
	./he5_gd_definefieldsF_32
	./he5_gd_writedataF_32
	./he5_gd_readdataF_32
	./he5_gd_infoF_32
	./he5_gd_subsetF_32
gd_write_test_64f: $(check_PROGRAMS)
	./he5_gd_setupF_64
	./he5_gd_definefieldsF_64
	./he5_gd_writedataF_64
	./he5_gd_readdataF_64
	./he5_gd_infoF_64
	./he5_gd_subsetF_64

gd_unlim_test: $(check_PROGRAMS)
	./he5_gd_setup
	./he5_gd_defunlimfld
	./he5_gd_wrunlimfld

pt_write_test: $(check_PROGRAMS)
	./he5_pt_setup
	./he5_pt_definelevels
	srcdir=$(srcdir) ./he5_pt_writedata
	./he5_pt_writeattrs
	./he5_pt_readdata
	./he5_pt_readattrs
	./he5_pt_updatelevels
	./he5_pt_datainfo
pt_write_test_32f: $(check_PROGRAMS)
	./he5_pt_setupF_32
	./he5_pt_definelevelsF_32
	srcdir=$(srcdir) ./he5_pt_writedataF_32
	./he5_pt_writeattrsF_32
	./he5_pt_readdataF_32
	./he5_pt_readattrsF_32
	./he5_pt_updatelevelsF_32
	./he5_pt_datainfoF_32
pt_write_test_64f: $(check_PROGRAMS)
	./he5_pt_setupF_64
	./he5_pt_definelevelsF_64
	srcdir=$(srcdir) ./he5_pt_writedataF_64
	./he5_pt_writeattrsF_64
	./he5_pt_readdataF_64
	./he5_pt_readattrsF_64
	./he5_pt_updatelevelsF_64
	./he5_pt_datainfoF_64

sw_write_test: $(check_PROGRAMS)
	./he5_sw_setup
	./he5_sw_definefields
	./he5_sw_writedata
	./he5_sw_readdata
	./he5_sw_info
	./he5_sw_datainfo
	./he5_sw_subset
sw_write_test_32f: $(check_PROGRAMS)
	./he5_sw_setupF_32
	./he5_sw_definefieldsF_32
	./he5_sw_writedataF_32
	./he5_sw_readdataF_32
	./he5_sw_infoF_32
	./he5_sw_subsetF_32
sw_write_test_64f: $(check_PROGRAMS)
	./he5_sw_setupF_64
	./he5_sw_definefieldsF_64
	./he5_sw_writedataF_64
	./he5_sw_readdataF_64
	./he5_sw_infoF_64
	./he5_sw_subsetF_64

sw_unlim_test: $(check_PROGRAMS)
	./he5_sw_setup
	./he5_sw_defunlimfld
	./he5_sw_wrunlimfld
	./he5_sw_subset

sw_extend_test: $(check_PROGRAMS)
	./he5_sw_setup
	./he5_sw_definefields
	./he5_sw_wrextendrd

za_write_test: $(check_PROGRAMS)
	./he5_za_setup
	./he5_za_definefields
	./he5_za_writedata
	./he5_za_readdata
	./he5_za_info
	./he5_za_datainfo
za_write_test_32f: $(check_PROGRAMS)
	./he5_za_setupF_32
	./he5_za_definefieldsF_32
	./he5_za_writedataF_32
	./he5_za_readdataF_32
	./he5_za_infoF_32
za_write_test_64f: $(check_PROGRAMS)
	./he5_za_setupF_64
	./he5_za_definefieldsF_64
	./he5_za_writedataF_64
	./he5_za_readdataF_64
	./he5_za_infoF_64

# This avoids any confusion between the names of the sequences and
# files with the same names.
.PHONY: $(TEST_SEQUENCES)
