
#include    "H5public.h"

#include    "HE5_HdfEosDef.h"
#include    "HE5_config.h"

#include    <stdlib.h>


#ifndef HAVE_H5FREE_MEMORY
herr_t H5free_memory(void *buf) {
    free(buf);
    return 0;
}
#endif

#ifdef   _HDFEOS5_THREADSAFE
HE5_HDFE_MutexStruct   GlobalMutex;
#endif
