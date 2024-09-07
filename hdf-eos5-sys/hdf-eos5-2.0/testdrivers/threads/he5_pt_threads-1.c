/*
 *********************************************************
 *       FILE:     he5_pt_threads-1.c                    *                     
 *       PURPOSE:  To test thread-safe version of        *
 *                 HDF-EOS5 Point library                *              
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
	hid_t    ptid;
    char     pointName[81];
} ThreadInfo_t;


void *HE5_PointTest(void *arg);

int main()
{
  herr_t           ret       = SUCCEED;
  hid_t            fid       = FAIL;
  int              loopCount;
  int              status    = FAIL;
  ThreadInfo_t     threadInfo[NUM_THREADS];
  pthread_t        threadArray[NUM_THREADS];
  pthread_attr_t   threadAttr;
  
  /* Open HDF-EOS Point file */
  /* ----------------------- */
  fid = HE5_PTopen("point.h5",H5F_ACC_TRUNC); 
  if (fid == FAIL)
    {
	  printf( "*ERROR:  unable to open \"point.h5\"  file. \n");
	  return (FAIL);
    }
  
  /* Execute the loop to the NUM_THREADS times        */
  /* to assign one function to each thread afterwards */
  /* ------------------------------------------------ */
  for (loopCount = 0; loopCount < NUM_THREADS; loopCount++)
    {
	  threadInfo[loopCount].threadNum = loopCount+1;
	  threadInfo[loopCount].fid       = fid;
	  threadInfo[loopCount].ptid      = FAIL;
	  sprintf(threadInfo[loopCount].pointName,"POINT_%d",loopCount);
	  sleep(1);
	  pthread_attr_init(&threadAttr);
	  pthread_attr_setscope(&threadAttr,PTHREAD_SCOPE_SYSTEM);
	  status = pthread_create(&threadArray[loopCount],&threadAttr,HE5_PointTest,&threadInfo[loopCount]);
	  if (status != 0)
        {
		  printf("\nERROR:  problem starting thread %d, error val: %d\n",loopCount,status);
		  ret = FAIL;
        }	  
	  pthread_join(threadArray[loopCount],NULL); 
    }
 
   ret = HE5_PTclose(fid);
  if (ret == FAIL)
    {
	  printf( "*ERROR:  unable to close file. \n");
	  return (FAIL);
    }
  printf( "... successfully created %d points within \"point.h5\" file. \n",loopCount);
   
  return(ret);
}


void *HE5_PointTest(void *arg)
{
  herr_t      status = FAIL;
  FILE        *fp;
  int         i;
  int         n;
  hsize_t     count[1];
  size_t      datasize = 0;
  double      time;
  float       conc[4];
  char        spc[8];
 
  HE5_CmpDTSinfo    *dtsinfo;
  typedef struct
	{
	  double          time;
	  float           concentr[4];
	  char            spec[8];
	  
	} InputData1;

  typedef struct 
	{
	  double    Time;
	  float     Conc[4];
	  char      Spc[8];
	} CmpData_1;

  CmpData_1 datbuf_1[20];

  ThreadInfo_t *tag = (ThreadInfo_t *) arg;
  
  tag->ptid = HE5_PTcreate(tag->fid,tag->pointName); 
  if (tag->ptid == FAIL)
	{
	  printf( "*ERROR:  unable to create %s point. \n",tag->pointName);
	  return (void *)NULL;
	}

  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));
  
  dtsinfo->nfields   = 3;  
  dtsinfo->rank[0]   = 1;
  dtsinfo->rank[1]   = 1;
  dtsinfo->rank[2]   = 1;
  dtsinfo->offset[0] = HOFFSET(InputData1, time);
  dtsinfo->offset[1] = HOFFSET(InputData1, concentr);
  dtsinfo->offset[2] = HOFFSET(InputData1, spec);
  dtsinfo->dtype[0]  = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[1]  = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[2]  = H5T_NATIVE_CHAR;
  dtsinfo->array[0]  = 0;
  dtsinfo->array[1]  = 1;
  dtsinfo->array[2]  = 1;
  
  for (i = 0; i < 3; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));
  
  strcpy(dtsinfo->fieldname[0],"Time");
  strcpy(dtsinfo->fieldname[1],"Concentration");
  strcpy(dtsinfo->fieldname[2],"Species");
  dtsinfo->dims[0][0] = 1;
  dtsinfo->dims[1][0] = 4;
  dtsinfo->dims[2][0] = 8;
  
  dtsinfo->datasize = (size_t)sizeof(InputData1);
  
  status = HE5_PTdeflevel(tag->ptid, "Sensor", dtsinfo);
  
  for (i = 0; i < 3; i++)
	{
	  free(dtsinfo->fieldname[i]);
	  dtsinfo->fieldname[i] = NULL;
	}
  
  free(dtsinfo);
  
  fp = fopen("simple.txt", "r");
  
  n = 0;
  while(fscanf(fp, "%lf %f %f %f %f %s", &time, &conc[0], &conc[1], &conc[2], &conc[3], spc) != -1)
    {
	  datbuf_1[n].Time    = time;
	  datbuf_1[n].Conc[0] = conc[0];
	  datbuf_1[n].Conc[1] = conc[1];
	  datbuf_1[n].Conc[2] = conc[2];
	  datbuf_1[n].Conc[3] = conc[3];
	  memmove(datbuf_1[n].Spc, spc, sizeof(char)*strlen(spc));
	  datbuf_1[n].Spc[strlen(spc)] = 0;
	  n++;
    }
 
  fclose(fp);
  
  datasize = (size_t)sizeof(CmpData_1);
  count[0] = n;
  
  status = HE5_PTwritelevel(tag->ptid, 0, count, &datasize, datbuf_1);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to write data to level. \n");
	  return (void *)NULL;
	}  
  
  status = HE5_PTdetach(tag->ptid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s point. \n",tag->pointName);
	  return (void *)NULL;
	}
  
  tag->ptid = HE5_PTattach(tag->fid,tag->pointName);
  if (tag->ptid == FAIL)
	{
	  printf( "*ERROR:  unable to attach to %s point. \n",tag->pointName);
	  return (void *)NULL;
	}
  
  status = HE5_PTdetach(tag->ptid);
  if (status == FAIL)
	{
	  printf( "*ERROR:  unable to detach from %s point. \n",tag->pointName);
	  return (void *)NULL;
	}
  
  return 0;
}









