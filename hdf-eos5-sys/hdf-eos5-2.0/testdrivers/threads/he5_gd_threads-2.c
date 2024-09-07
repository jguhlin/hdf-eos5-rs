/*
 *********************************************************
 *       FILE:     he5_gd_threads-2.c                    *                     
 *       PURPOSE:  To test thread-safe version of        *
 *                 HDF-EOS5 Grid library                 *              
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
	hid_t    gdid;
    char     gridName[81];
} ThreadInfo_t;


void *HE5_GridTest(void *arg);

int main()
{
  herr_t           ret       = SUCCEED;
  hid_t            fid       = FAIL;
  int              loopCount;
  int              status    = FAIL;    
  ThreadInfo_t     threadInfo[NUM_THREADS];
  pthread_t        threadArray[NUM_THREADS];
  pthread_attr_t   threadAttr;


  /* Open HDF-EOS Grid file */
  /* ---------------------- */
  fid = HE5_GDopen("grid.h5",H5F_ACC_RDWR); 
  if (fid == FAIL)
    {
	  printf( "*ERROR:  unable to open \"grid.h5\"  file. \n");
	  return (FAIL);
    }
  
  /* Execute the loop to the NUM_THREADS times        */
  /* to assign one function to each thread afterwards */
  /* ------------------------------------------------ */
  for (loopCount = 0; loopCount < NUM_THREADS; loopCount++)
    {
	  threadInfo[loopCount].threadNum = loopCount+1;
	  threadInfo[loopCount].fid       = fid;
	  threadInfo[loopCount].gdid      = FAIL;
	  sprintf(threadInfo[loopCount].gridName,"UTMGrid_%d",loopCount);

	  sleep(1);
	  pthread_attr_init(&threadAttr);
	  pthread_attr_setscope(&threadAttr,PTHREAD_SCOPE_SYSTEM);
	  status = pthread_create(&threadArray[loopCount],&threadAttr,HE5_GridTest,&threadInfo[loopCount]);
	  if (status != 0)
        {
		  printf("\nERROR:  problem starting thread %d, error val: %d\n",loopCount,status);
		  ret = FAIL;
        }
	  pthread_join(threadArray[loopCount],NULL); 
    }
 
  ret = HE5_GDclose(fid);
  if (ret == FAIL)
    {
	  printf( "*ERROR:  unable to close file. \n");
	  return (FAIL);
    }
  
  return(ret);
}

void *HE5_GridTest(void *arg)
{
  herr_t      status = FAIL;
  int         i;
  float       *veg;
  float       *veg_out;
  hssize_t    start[3];  
  hsize_t     edge[3];

  ThreadInfo_t *tag = (ThreadInfo_t *) arg;

  veg = (float *)calloc(24000,sizeof(float));
  if (veg == NULL)
	{
	  printf( "*ERROR:  unable to allocate memory. \n");
	  return (void *)NULL;
	}

  for (i = 0; i < 24000; i++)
	veg[i] = (float)i;

  printf(" \n");
  printf("Starting thread  %d\n",tag->threadNum);
  printf(" \n");

  tag->gdid = HE5_GDattach(tag->fid,tag->gridName);
  if (tag->gdid == FAIL)
	{
	  printf( "*ERROR:  unable to attach to %s grid. \n",tag->gridName);
	  return (void *)NULL;
	}
	  
  start[0] = 0;     start[1] = 0;
  edge[0]  = 200;   edge[1]  = 120;
  status = HE5_GDwritefield(tag->gdid, "Vegetation", start, NULL, edge, veg);
  if (status == FAIL)
	{
	  printf( "*ERROR calling HE5_GDwritefield(\"Vegetation\"). \n");
	  return (void *)NULL;
	}

  free(veg);

  status = HE5_GDdetach(tag->gdid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s grid. \n",tag->gridName);
	  return (void *)NULL;
	}
  
  tag->gdid = HE5_GDattach(tag->fid,tag->gridName);
  if (tag->gdid == FAIL)
	{
	  printf( "*ERROR:  unable to attach to %s grid. \n",tag->gridName);
	  return (void *)NULL;
	}

  veg_out = (float *)calloc(24000,sizeof(float));
  if (veg_out == NULL)
	{
	  printf( "*ERROR:  unable to allocate memory. \n");
	  return (void *)NULL;
	}

  start[0] = 0;     start[1] = 0;
  edge[0]  = 200;   edge[1]  = 100;
  status = HE5_GDreadfield(tag->gdid,"Vegetation",start,NULL,edge,veg_out);
  if (status == FAIL)
	{
	  printf( "*ERROR calling HE5_GDreadfield(\"Vegetation\"). \n");
	  return (void *)NULL;
	}

  free(veg_out);

  status = HE5_GDdetach(tag->gdid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s grid. \n",tag->gridName);
	  return (void *)NULL;
	}
  
  return 0;
}









