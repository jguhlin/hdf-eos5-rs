# testdrivers top-level Makefile.am
# The testdrivers directory should be placed in the HDF-EOS5 directory;
# this file should be located at hdfeos5/testdrivers/Makefile.am

# Include boilerplate definitions (from HDF-EOS5 configuration)
include $(top_srcdir)/config/include.am

# Only recurse into the threads directory if threads are being used.
if HDF5_THREADSAFE
  SUBDIRS=swath grid point za threads
else
  SUBDIRS=swath grid point za
endif
