/*
 *********************************************************
 *       FILE:     he5_sw_threads-1.c                    *                     
 *       PURPOSE:  To test thread-safe version of        *
 *                 HDF-EOS5 Swath library                *              
 *       Author:   A. Muslimov, Emergent IT Inc.         * 
 *       Date:     Sep, 2001                             *  
 *       Updated:  Nov, 2001                             *              
 *********************************************************  
 */

#include <HE5_HdfEosDef.h>

#define NUM_THREADS   30

typedef struct ThreadInfo_tag
{
    int      threadNum;
	hid_t    fid;
	hid_t    swid;
    char     swathName[81];
} ThreadInfo_t;


void *HE5_SwathTest(void *arg);

int main()
{
  herr_t           ret       = SUCCEED;
  hid_t            fid       = FAIL;
  int              loopCount;
  int              status    = FAIL;
  ThreadInfo_t     threadInfo[NUM_THREADS];
  pthread_t        threadArray[NUM_THREADS];
  pthread_attr_t   threadAttr;

  /* Open HDF-EOS Swath file */
  /* ----------------------- */
  fid = HE5_SWopen("swath.h5",H5F_ACC_TRUNC); 
  if (fid == FAIL)
    {
	  printf( "*ERROR:  unable to open \"swath.h5\"  file. \n");
	  return (FAIL);
    }
  
  /* Execute the loop to the NUM_THREADS times        */
  /* to assign one function to each thread afterwards */
  /* ------------------------------------------------ */
  for (loopCount = 0; loopCount < NUM_THREADS; loopCount++)
    {
	  threadInfo[loopCount].threadNum = loopCount+1;
	  threadInfo[loopCount].fid = fid;
	  threadInfo[loopCount].swid  = FAIL;
	  sprintf(threadInfo[loopCount].swathName,"SWATH_%d",loopCount);
	  sleep(1);
	  pthread_attr_init(&threadAttr);
	  pthread_attr_setscope(&threadAttr,PTHREAD_SCOPE_SYSTEM);
	  status = pthread_create(&threadArray[loopCount],&threadAttr,HE5_SwathTest,&threadInfo[loopCount]);
	  if (status != 0)
        {
		  printf("\nERROR:  problem starting thread %d, error val: %d\n",loopCount,status);
		  ret = FAIL;
        }
	  pthread_join(threadArray[loopCount],NULL); 
    }
 
  ret = HE5_SWclose(fid);
  if (ret == FAIL)
    {
	  printf( "*ERROR:  unable to close file. \n");
	  return (FAIL);
    }

  printf( "... successfully created %d swaths within \"swath.h5\" file. \n",loopCount);
  
  return(ret);
}


void *HE5_SwathTest(void *arg)
{
  int         j;
  herr_t      status = FAIL;
  hssize_t    start[3];   
  hsize_t     count[3];
  double      *plane;
  double      *outbuf;

  ThreadInfo_t *tag = (ThreadInfo_t *) arg;

  tag->swid = HE5_SWcreate(tag->fid,tag->swathName); 
  if (tag->swid == FAIL)
	{
	  printf( "*ERROR:  unable to create %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}

  status = HE5_SWdefdim(tag->swid, "GeoTrack", 20);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define dimension for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
 
  status = HE5_SWdefdim(tag->swid, "GeoXtrack", 10);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define dimension for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}

  status = HE5_SWdefdim(tag->swid, "Res2tr", 40);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define dimension for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
  
  status = HE5_SWdefdim(tag->swid, "Res2xtr", 20);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define dimension for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}

  status = HE5_SWdefdim(tag->swid, "Bands", 15);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define dimension for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
  
  status = HE5_SWdefdatafield(tag->swid, "Spectra", "Bands,Res2tr,Res2xtr", NULL, H5T_NATIVE_DOUBLE, 0);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to define data field for %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}

  plane = (double *)calloc(12000,sizeof(double));
  if (plane == NULL)
	{
	  printf( "*ERROR:  unable to allocate memory. \n");
	  return (void *)NULL;
	}
  
  for (j = 0; j < 12000; j++)
	plane[j] = (double)j;

  start[0] = 0;      count[0] = 15;
  start[1] = 0;      count[1] = 40;
  start[2] = 0;      count[2] = 20;

  status = HE5_SWwritefield(tag->swid, "Spectra", start, NULL, count, plane);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to write data for \"Spectra\" field in  %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
 	  
  free(plane);

  status = HE5_SWdetach(tag->swid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
  

  tag->swid = HE5_SWattach(tag->fid,tag->swathName);
  if (tag->swid == FAIL)
	{
	  printf( "*ERROR:  unable to attach to %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
  
  start[0] = 0;      count[0] = 15;
  start[1] = 0;      count[1] = 40;
  start[2] = 0;      count[2] = 20;

  outbuf = (double *)calloc(12000, sizeof(double));
  if (outbuf == NULL)
	{
	  printf( "*ERROR:  unable to allocate memory. \n");
	  return (void *)NULL;
	}

  status = HE5_SWreadfield(tag->swid, "Spectra", start, NULL, count, outbuf);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to read data from \"Spectra\" field in  %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}

  free(outbuf);
  
  status = HE5_SWdetach(tag->swid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s swath. \n",tag->swathName);
	  return (void *)NULL;
	}
  
  return 0;
}









