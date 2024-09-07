/*
 *********************************************************
 *       FILE:     TestPoint.c                           *
 *       PURPOSE:  To test HDF-EOS v.5.1 Point interface *
 *       Author:   A. Muslimov, Emergent IT Inc.         *
 *       Date:     June 2001                             *
 *********************************************************
 */


#include <HE5_HdfEosDef.h>
#include <tutils.h>

#define NAMEBUF_SIZE 100

/* Set up namebuf value in case the input file is in
 * a different directory and "srcdir" is set in the
 * environment.
 */
static void setup_namebuf(char *namebuf, char *filename)
{
  char * srcdir;

  srcdir = getenv("srcdir");
  if(srcdir != NULL && (strlen(srcdir) + strlen(filename) + 2) <= NAMEBUF_SIZE)
  {
    strcpy(namebuf, srcdir);
    strcat(namebuf, "/");
    strcat(namebuf, filename);
  }
  else
  {
    strcpy(namebuf, filename);
  }
}

int main(int argc, char *argv[])
{

  FILE            *fp;
  FILE            *fp_in;

  herr_t          status    = FAIL;

  hid_t           ptfid     = FAIL;
  hid_t           PTid1     = FAIL;
  hid_t           PTid2     = FAIL;
  hid_t           PTid3     = FAIL;
  hid_t           datatype  = FAIL;

  int	          CLLoop;
  int             Verbosity = 4;
  int             errcnt    = 0;
  int             i, j;
  int             n;
  int             date;
  int             wgt;
  int             IntAttr    = 9999;
  int             attr       = 9999;
  int             attr1[ 5 ] = { 1, 2, 3, 4, 5};
  int             attr2[ 5 ] = { 10, 20, 30, 40, 50};
  int             nflds      = FAIL;
  int             fieldgroup = FAIL;
  int             return_val = FAIL;

  long            nattr      = FAIL;
  long            strbufsize = FAIL;

  H5T_class_t     classid    = H5T_NO_CLASS;
  H5T_order_t     order      = H5T_ORDER_ERROR;

  typedef struct
	{
	  double      time;
	  float       concentr[4];
	  char        spec[8];

	} InputData1;

  typedef struct
	{
	  char        label[16];
	  double      lon;
	  double      lat;
	  int         date;
	  char        id[8];
	} InputData2;

  typedef struct
	{
	  double      time;
	  float       rain;
	  float       temp;
	  char        id[8];
	} InputData3;


  typedef struct
	{
	  char        label[10];
	  int         date;
	  int         weight;
	  char        id[8];
	} InputData4;


  typedef struct
	{
	  double      time;
	  double      lon;
	  double      lat;
	  float       rain;
	  float       temp;
	  char        id[8];
	} InputData5;

  typedef struct {
	double   time;
	float    con[4];
	char     spec[8];
  } Sensor;

  hsize_t         count[8];
  hsize_t         nrecs = 0;
  hsize_t         nrec  = 1;

  hssize_t        recs[32];

  size_t          datasize = 0;
  size_t          size     = 0;

  float           conc[4];
  float           rain;
  float           temp;
  float           flt = -7.5;

  double          time;
  double          lon;
  double          lat;

  char            spc[8];
  char            desc[16];
  char            id[ 2 ];

  HE5_CmpDTSinfo  level;
  HE5_CmpDTSinfo  inInfo;
  HE5_CmpDTSinfo  inInfo2;
  HE5_CmpDTSinfo  *dtsinfo;

  InputData1      datbuf_1[20];
  InputData2      datbuf_2[5];
  InputData3      datbuf_3[25];
  InputData4      datbuf_4[5];
  InputData5      datbuf_5[30];

  InputData1      buf_1;
  Sensor          s[50];
  double          s1[50];

  char            namebuf[NAMEBUF_SIZE];


  /*
   ********************************************************************************
   *                 Get comand-line argument (Verbosity)                         *
   ********************************************************************************
   */


  for (CLLoop = 1; CLLoop < argc; CLLoop++)
	{
	  if ((argc > CLLoop + 1) && ((HDstrcmp(argv[CLLoop], "-verbose") == 0) || (HDstrcmp(argv[CLLoop], "-v") == 0)))
		{
		  if (argv[CLLoop + 1][0] == 'l')
			Verbosity = 4;
		  else if (argv[CLLoop + 1][0] == 'm')
			Verbosity = 6;
		  else if (argv[CLLoop + 1][0] == 'h')
			Verbosity = 10;
		  else
			Verbosity = atoi(argv[CLLoop + 1]);
		}
	}


  setup_namebuf(namebuf, "TestPoint_c.txtout");
  MESSAGE(11,fp=fopen(namebuf,"w"););

  MESSAGE(4,printf("Testing Point C interface \n"););
  MESSAGE(4,printf("========================= \n\n"););
  MESSAGE(11,fprintf(fp,"Testing Point C interface \n"););
  MESSAGE(11,fprintf(fp,"========================= \n\n"););


  /*
   *********************************************************************************
   *                             Testing    PTopen                                 *
   *********************************************************************************
   */

  MESSAGE(4,printf("\n"););
  MESSAGE(4,printf("\tTesting HE5_PTopen... \n"););
  MESSAGE(4,printf("\t===================== \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTopen... \n"););
  MESSAGE(11,fprintf(fp,"\t===================== \n"););

  ptfid = HE5_PTopen("SimplePoint.he5", H5F_ACC_TRUNC);
  if (ptfid == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully opened the point file ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully opened the point file ****\n"););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTcreate                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\n"););
  MESSAGE(4,printf("\tTesting HE5_PTcreate... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTcreate... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tCreating point \"Simple Point\"  \n"););
  MESSAGE(11,fprintf(fp,"\t\tCreating point \"Simple Point\" \n"););

  PTid1 = HE5_PTcreate(ptfid, "Simple Point");
  if (PTid1 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully created the point \"Simple Point\" ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the point \"Simple Point\" ****\n"););
	}



  /*
   *********************************************************************************
   *                             Testing    HE5_PTcreate                           *
   *********************************************************************************
   */

  MESSAGE(4,printf("\n"););
  MESSAGE(4,printf("\tTesting HE5_PTcreate... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTcreate... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tCreating point \"FixedBuoy Point\"  \n"););
  MESSAGE(11,fprintf(fp,"\t\tCreating point \"FixedBuoy Point\" \n"););

  PTid2 = HE5_PTcreate(ptfid, "FixedBuoy Point");
  if (PTid2 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully created the point \"FixedBuoy Point\" ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the point \"FixedBuoy Point\" ****\n"););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTcreate                           *
   *********************************************************************************
   */

  MESSAGE(4,printf("\n"););
  MESSAGE(4,printf("\tTesting HE5_PTcreate... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTcreate... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tCreating point \"FloatBuoy Point\"  \n"););
  MESSAGE(11,fprintf(fp,"\t\tCreating point \"FloatBuoy Point\" \n"););

  PTid3 = HE5_PTcreate(ptfid, "FloatBuoy Point");
  if (PTid3 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully created the point \"FloatBuoy Point\" ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the point \"FloatBuoy Point\" ****\n"););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"Simple Point\" \n"););

  status = HE5_PTdetach(PTid1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTdetach(PTid2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}



  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FloatBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FloatBuoy Point\" \n"););

  status = HE5_PTdetach(PTid3);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"Simple Point\" \n"););
   MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"Simple Point\"  \n"););

  PTid1 = HE5_PTattach(ptfid, "Simple Point");
  if (PTid1 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1););
	  MESSAGE(11,fprintf(fp,"\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1) ;);
	}


  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));

  /* Set up level data structure */
  /* --------------------------- */
  dtsinfo->nfields = 3;

  dtsinfo->rank[0] = 1;
  dtsinfo->rank[1] = 1;
  dtsinfo->rank[2] = 1;

  dtsinfo->offset[0] = HOFFSET(InputData1, time);
  dtsinfo->offset[1] = HOFFSET(InputData1, concentr);
  dtsinfo->offset[2] = HOFFSET(InputData1, spec);

  dtsinfo->dtype[0] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[1] = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[2] = H5T_NATIVE_CHAR;

  dtsinfo->array[0] = 0;
  dtsinfo->array[1] = 1;
  dtsinfo->array[2] = 1;

  for (i = 0; i < 3; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));

  strcpy(dtsinfo->fieldname[0],"Time");
  strcpy(dtsinfo->fieldname[1],"Concentration");
  strcpy(dtsinfo->fieldname[2],"Species");

  dtsinfo->dims[0][0] = 1;
  dtsinfo->dims[1][0] = 4;
  dtsinfo->dims[2][0] = 8;

  dtsinfo->datasize = (size_t)sizeof(InputData1);


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdeflevel                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\t\tDefining level \"Sensor\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining level \"Sensor\" \n"););
  status = HE5_PTdeflevel(PTid1, "Sensor", dtsinfo);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflevel()          %d\n",status););
	}

  for (i = 0; i < 3; i++)
	{
	  free(dtsinfo->fieldname[i]);
	  dtsinfo->fieldname[i] = NULL;
	}
  free(dtsinfo);

  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"Simple Point\" \n"););

  status = HE5_PTdetach(PTid1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FixedBuoy Point\"  \n"););

  PTid2 = HE5_PTattach(ptfid, "FixedBuoy Point");
  if (PTid2 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2););
	  MESSAGE(11,fprintf(fp,"\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2) ;);
	}

  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));

  /* Set up level data struvcture */
  /* ---------------------------- */
  dtsinfo->nfields = 5;

  dtsinfo->rank[0] = 1;
  dtsinfo->rank[1] = 1;
  dtsinfo->rank[2] = 1;
  dtsinfo->rank[3] = 1;
  dtsinfo->rank[4] = 1;
  dtsinfo->array[0] = 1;
  dtsinfo->array[1] = 0;
  dtsinfo->array[2] = 0;
  dtsinfo->array[3] = 0;
  dtsinfo->array[4] = 1;

  dtsinfo->offset[0] = HOFFSET(InputData2, label);
  dtsinfo->offset[1] = HOFFSET(InputData2, lon);
  dtsinfo->offset[2] = HOFFSET(InputData2, lat);
  dtsinfo->offset[3] = HOFFSET(InputData2, date);
  dtsinfo->offset[4] = HOFFSET(InputData2, id);

  dtsinfo->dtype[0] = H5T_NATIVE_CHAR;
  dtsinfo->dtype[1] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[2] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[3] = H5T_NATIVE_INT;
  dtsinfo->dtype[4] = H5T_NATIVE_CHAR;

  for (i = 0; i < 5; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));

  strcpy(dtsinfo->fieldname[0],"Label");
  strcpy(dtsinfo->fieldname[1],"Longitude");
  strcpy(dtsinfo->fieldname[2],"Latitude");
  strcpy(dtsinfo->fieldname[3],"DeployDate");
  strcpy(dtsinfo->fieldname[4],"ID");

  dtsinfo->dims[0][0] = 8;
  dtsinfo->dims[1][0] = 1;
  dtsinfo->dims[2][0] = 1;
  dtsinfo->dims[3][0] = 1;
  dtsinfo->dims[4][0] = 8;

  dtsinfo->datasize = (size_t)sizeof(InputData2);

  /*
   *********************************************************************************
   *                         Testing    HE5_PTdeflevel                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\t\tDefining level \"Desc-Loc\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining level \"Desc-Loc\" \n"););
  status = HE5_PTdeflevel(PTid2, "Desc-Loc", dtsinfo);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflevel()          %d\n",status););
	}


  for (i = 0; i < 5; i++)
	free(dtsinfo->fieldname[i]);

  free(dtsinfo);

  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));

  /* Set up level data structure */
  /* --------------------------- */
  dtsinfo->nfields = 4;

  dtsinfo->rank[0] = 1;
  dtsinfo->rank[1] = 1;
  dtsinfo->rank[2] = 1;
  dtsinfo->rank[3] = 1;

  dtsinfo->array[0] = 0;
  dtsinfo->array[1] = 0;
  dtsinfo->array[2] = 0;
  dtsinfo->array[3] = 1;

  dtsinfo->offset[0] = HOFFSET(InputData3, time);
  dtsinfo->offset[1] = HOFFSET(InputData3, rain);
  dtsinfo->offset[2] = HOFFSET(InputData3, temp);
  dtsinfo->offset[3] = HOFFSET(InputData3, id);

  dtsinfo->dtype[0] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[1] = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[2] = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[3] = H5T_NATIVE_CHAR;

  for (i = 0; i < 4; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));

  strcpy(dtsinfo->fieldname[0],"Time");
  strcpy(dtsinfo->fieldname[1],"Rainfall");
  strcpy(dtsinfo->fieldname[2],"Temperature");
  strcpy(dtsinfo->fieldname[3],"ID");

  dtsinfo->dims[0][0] = 1;
  dtsinfo->dims[1][0] = 1;
  dtsinfo->dims[2][0] = 1;
  dtsinfo->dims[3][0] = 8;

  dtsinfo->datasize = (size_t)sizeof(InputData3);


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdeflevel                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\t\tDefining level \"Observations\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining level \"Observations\" \n"););
  status = HE5_PTdeflevel(PTid2, "Observations", dtsinfo);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflevel()          %d\n",status););
	}

  for (i = 0; i < 4; i++)
	free(dtsinfo->fieldname[i]);

  free(dtsinfo);


  /*
   *********************************************************************************
   *                           Testing  HE5_PTdeflinkage                           *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflinkage... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflinkage... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\t\tDefining linkage between levels \"Desc-Loc\" and \"Observations\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining linkage between levels \"Desc-Loc\" and \"Observations\" \n"););

  status = HE5_PTdeflinkage(PTid2, "Desc-Loc", "Observations", "ID");
  if (status == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus = HE5_PTdeflinkage();\n"););
       MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflinkage()           %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflinkage();\n"););
	   MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflinkage()          %d\n",status););
	 }


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTdetach(PTid2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FloatBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FloatBuoy Point\"  \n"););

  PTid3 = HE5_PTattach(ptfid, "FloatBuoy Point");
  if (PTid3 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid3 = HE5_PTattach(ptfid, \"FloatBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid3););
	  MESSAGE(11,fprintf(fp,"\t\tPTid3 = HE5_PTattach(ptfid, \"FloatBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid3) ;);
	}


  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));

  /* Set up level data structure */
  /* --------------------------- */
  dtsinfo->nfields = 4;

  dtsinfo->rank[0] = 1;
  dtsinfo->rank[1] = 1;
  dtsinfo->rank[2] = 1;
  dtsinfo->rank[3] = 1;

  dtsinfo->array[0] = 1;
  dtsinfo->array[1] = 0;
  dtsinfo->array[2] = 0;
  dtsinfo->array[3] = 1;

  dtsinfo->offset[0] = HOFFSET(InputData4, label);
  dtsinfo->offset[1] = HOFFSET(InputData4, date);
  dtsinfo->offset[2] = HOFFSET(InputData4, weight);
  dtsinfo->offset[3] = HOFFSET(InputData4, id);

  dtsinfo->dtype[0] = H5T_NATIVE_CHAR;
  dtsinfo->dtype[1] = H5T_NATIVE_INT;
  dtsinfo->dtype[2] = H5T_NATIVE_INT;
  dtsinfo->dtype[3] = H5T_NATIVE_CHAR;

  for (i = 0; i < 4; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));

  strcpy(dtsinfo->fieldname[0],"Label");
  strcpy(dtsinfo->fieldname[1],"DeployDate");
  strcpy(dtsinfo->fieldname[2],"Weight");
  strcpy(dtsinfo->fieldname[3],"ID");

  dtsinfo->dims[0][0] = 8;
  dtsinfo->dims[1][0] = 1;
  dtsinfo->dims[2][0] = 1;
  dtsinfo->dims[3][0] = 8;

  dtsinfo->datasize = (size_t)sizeof(InputData4);


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdeflevel                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\t\tDefining level \"Description\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining level \"Description\" \n"););
  status = HE5_PTdeflevel(PTid3, "Description", dtsinfo);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflevel()          %d\n",status););
	}

  for (i = 0; i < 4; i++)
	free(dtsinfo->fieldname[i]);

  free(dtsinfo);


  dtsinfo = (HE5_CmpDTSinfo *)calloc(1, sizeof(HE5_CmpDTSinfo));

  /* Set up level data structure */
  /* --------------------------- */
  dtsinfo->nfields = 6;

  dtsinfo->rank[0] = 1;
  dtsinfo->rank[1] = 1;
  dtsinfo->rank[2] = 1;
  dtsinfo->rank[3] = 1;
  dtsinfo->rank[4] = 1;
  dtsinfo->rank[5] = 1;

  dtsinfo->array[0] = 0;
  dtsinfo->array[1] = 0;
  dtsinfo->array[2] = 0;
  dtsinfo->array[3] = 0;
  dtsinfo->array[4] = 0;
  dtsinfo->array[5] = 1;

  dtsinfo->offset[0] = HOFFSET(InputData5, time);
  dtsinfo->offset[1] = HOFFSET(InputData5, lon);
  dtsinfo->offset[2] = HOFFSET(InputData5, lat);
  dtsinfo->offset[3] = HOFFSET(InputData5, rain);
  dtsinfo->offset[4] = HOFFSET(InputData5, temp);
  dtsinfo->offset[5] = HOFFSET(InputData5, id);

  dtsinfo->dtype[0] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[1] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[2] = H5T_NATIVE_DOUBLE;
  dtsinfo->dtype[3] = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[4] = H5T_NATIVE_FLOAT;
  dtsinfo->dtype[5] = H5T_NATIVE_CHAR;
  for (i = 0; i < 6; i++)
	dtsinfo->fieldname[i] = (char *)calloc(64, sizeof(char));

  strcpy(dtsinfo->fieldname[0],"Time");
  strcpy(dtsinfo->fieldname[1],"Longitude");
  strcpy(dtsinfo->fieldname[2],"Latitude");
  strcpy(dtsinfo->fieldname[3],"Rainfall");
  strcpy(dtsinfo->fieldname[4],"Temperature");
  strcpy(dtsinfo->fieldname[5],"ID");

  dtsinfo->dims[0][0] = 1;
  dtsinfo->dims[1][0] = 1;
  dtsinfo->dims[2][0] = 1;
  dtsinfo->dims[3][0] = 1;
  dtsinfo->dims[4][0] = 1;
  dtsinfo->dims[5][0] = 8;

  dtsinfo->datasize = (size_t)sizeof(InputData5);


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdeflevel                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\t\tDefining level \"Measurements\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining level \"Measurements\" \n"););
  status = HE5_PTdeflevel(PTid3, "Measurements", dtsinfo);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflevel()          %d\n",status););
	}

  for (i = 0; i < 6; i++)
	free(dtsinfo->fieldname[i]);

  free(dtsinfo);




  /*
   *********************************************************************************
   *                           Testing  HE5_PTdeflinkage                           *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdeflinkage... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdeflinkage... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\t\tDefining linkage between levels \"Description\" and \"Measurements\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDefining linkage between levels \"Description\" and \"Measurements\" \n"););

  status = HE5_PTdeflinkage(PTid3, "Description", "Measurements", "ID");
  if (status == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus = HE5_PTdeflinkage();\n"););
       MESSAGE(8,printf("\t\tStatus returned by HE5_PTdeflinkage()           %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdeflinkage();\n"););
	   MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdeflinkage()          %d\n",status););
	 }


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FloatBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FloatBuoy Point\" \n"););

  status = HE5_PTdetach(PTid3);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}



  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"Simple Point\" \n"););
   MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"Simple Point\"  \n"););

  PTid1 = HE5_PTattach(ptfid, "Simple Point");
  if (PTid1 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1););
	  MESSAGE(11,fprintf(fp,"\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1) ;);
	}


  setup_namebuf(namebuf, "simple.txt");
  fp_in = fopen(namebuf, "r");

  n = 0;
  while(fscanf(fp_in, "%lf %f %f %f %f %s", &time, &conc[0], &conc[1], &conc[2], &conc[3], spc) != -1)
    {
	  datbuf_1[n].time    = time;
	  datbuf_1[n].concentr[0] = conc[0];
	  datbuf_1[n].concentr[1] = conc[1];
	  datbuf_1[n].concentr[2] = conc[2];
	  datbuf_1[n].concentr[3] = conc[3];
	  memmove(datbuf_1[n].spec, spc, sizeof(char)*strlen(spc));
	  datbuf_1[n].spec[strlen(spc)] = 0;
	  n++;
    }

  fclose(fp_in);

  datasize = (size_t)sizeof(InputData1);
  count[0] = n;


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelevel                       *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting records to the  point \"Simple Point\" level 0 \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting records to the  point \"Simple Point\" level 0 \n"););

  status = HE5_PTwritelevel(PTid1, 0, count, &datasize, datbuf_1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelevel()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"Simple Point\" \n"););

  status = HE5_PTdetach(PTid1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}

  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FixedBuoy Point\"  \n"););

  PTid2 = HE5_PTattach(ptfid, "FixedBuoy Point");
  if (PTid2 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2););
	  MESSAGE(11,fprintf(fp,"\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2) ;);
	}

  setup_namebuf(namebuf, "fixedBuoy0.txt");
  fp_in = fopen(namebuf, "r");

  n = 0;
  while(fscanf(fp_in, "%s %lf %lf %d  %s", desc, &lon, &lat, &date, id) != -1)
    {
	  strcpy(datbuf_2[n].label, desc);
	  datbuf_2[n].lon     = lon;
	  datbuf_2[n].lat     = lat;
	  datbuf_2[n].date    = date;
	  memmove(datbuf_2[n].id, id, sizeof(char)*strlen(id));
	  datbuf_2[n].id[strlen(id)] = 0;
	  n++;
    }

  fclose(fp_in);

  datasize = (size_t)sizeof(InputData2);
  count[0] = n;


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelevel                       *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting records to the  point \"FixedBuoy Point\" level 0 \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting records to the  point \"FixedBuoy Point\" level 0 \n"););

  status = HE5_PTwritelevel(PTid2, 0, count, &datasize, datbuf_2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelevel()          %d\n",status););
	}


  setup_namebuf(namebuf, "fixedBuoy1.txt");
  fp_in = fopen(namebuf, "r");

  n = 0;
  while(fscanf(fp_in, "%lf %f %f %s", &time, &rain, &temp, id) != -1)
    {
	  datbuf_3[n].time = time;
	  datbuf_3[n].rain = rain;
	  datbuf_3[n].temp = temp;
	  memmove(datbuf_3[n].id, id, sizeof(char)*strlen(id));
	  datbuf_3[n].id[strlen(id)] = 0;
	  n++;
    }

  fclose(fp_in);

  datasize = (size_t)sizeof(InputData3);
  count[0] = n;

  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelevel                       *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting records to the  point \"FixedBuoy Point\" level 1 \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting records to the  point \"FixedBuoy Point\" level 1 \n"););

  status = HE5_PTwritelevel(PTid2, 1, count, &datasize, datbuf_3);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelevel()          %d\n",status););
	}



  /*
   *********************************************************************************
   *                             Testing    HE5_PTwriteattr                        *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting attribute to the  point \"FixedBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting attribute to the  point \"FixedBuoy Point\" \n"););

  count[0] = 1;
  status = HE5_PTwriteattr(PTid2, "GlobalAttr_Integer", H5T_NATIVE_INT, count, &IntAttr);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwriteattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwriteattr()          %d\n",status););
	}

  /*
   *********************************************************************************
   *                             Testing    HE5_PTwriteattr                        *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting attribute to the  point \"FixedBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting attribute to the  point \"FixedBuoy Point\" \n"););

  count[0] = 1;
  status = HE5_PTwriteattr(PTid2, "GlobalAttribute_int", H5T_NATIVE_INT, count, &attr);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwriteattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwriteattr()          %d\n",status););
	}



  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritegrpattr                     *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritegrpattr... \n"););
  MESSAGE(4,printf("\t============================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritegrpattr... \n"););
  MESSAGE(11,fprintf(fp,"\t============================= \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting group attribute to the  point \"FixedBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting group attribute to the  point \"FixedBuoy Point\" \n"););

  count[0] = 5;
  status = HE5_PTwritegrpattr(PTid2, "GroupAttribute", H5T_NATIVE_INT, count, attr1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritegrpattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritegrpattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritegrpattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritegrpattr()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelocattr                     *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelocattr... \n"););
  MESSAGE(4,printf("\t============================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelocattr... \n"););
  MESSAGE(11,fprintf(fp,"\t============================= \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting local attribute to the  point \"FixedBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting local attribute to the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTwritelocattr(PTid2, "Observations", "LocalAttribute", H5T_NATIVE_INT, count, attr2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelocattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelocattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelocattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelocattr()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTdetach(PTid2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}



  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FloatBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FloatBuoy Point\"  \n"););

  PTid3 = HE5_PTattach(ptfid, "FloatBuoy Point");
  if (PTid3 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid3 = HE5_PTattach(ptfid, \"FloatBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid3););
	  MESSAGE(11,fprintf(fp,"\t\tPTid3 = HE5_PTattach(ptfid, \"FloatBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid3) ;);
	}

  setup_namebuf(namebuf, "floatBuoy0.txt");
  fp_in = fopen(namebuf, "r");

  n = 0;
  while(fscanf(fp_in, "%s %d %d %s", desc, &date, &wgt, id) != -1)
    {
	  strcpy(datbuf_4[n].label, desc);
	  datbuf_4[n].date   = date;
	  datbuf_4[n].weight = wgt;
	  memmove(datbuf_4[n].id, id, sizeof(char)*strlen(id));
      datbuf_4[n].id[strlen(id)] = 0;
	  n++;
    }

  fclose(fp_in);

  datasize = (size_t)sizeof(InputData4);


  count[0] = n;


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelevel                       *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting records to the  point \"FloatBuoy Point\" level 0 \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting records to the  point \"FloatBuoy Point\" level 0 \n"););

  status = HE5_PTwritelevel(PTid3, 0, count, &datasize, datbuf_4);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelevel()          %d\n",status););
	}


  setup_namebuf(namebuf, "floatBuoy1.txt");
  fp_in = fopen(namebuf, "r");

  n = 0;
  while(fscanf(fp_in, "%lf %lf %lf %f %f %s", &time, &lon, &lat, &rain, &temp, id) != -1)
    {
	  datbuf_5[n].time = time;
	  datbuf_5[n].lon  = lon;
	  datbuf_5[n].lat  = lat;
	  datbuf_5[n].rain = rain;
	  datbuf_5[n].temp = temp;
	  memmove(datbuf_5[n].id, id,sizeof(char)*strlen(id));
      datbuf_5[n].id[strlen(id)] = 0;
	  n++;
    }

  fclose(fp_in);

  datasize = (size_t)sizeof(InputData5);
  count[0] = n;



  /*
   *********************************************************************************
   *                             Testing    HE5_PTwritelevel                       *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(4,printf("\t=========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwritelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t=========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting records to the  point \"FloatBuoy Point\" level 1 \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting records to the  point \"FloatBuoy Point\" level 1 \n"););

  status = HE5_PTwritelevel(PTid3, 1, count, &datasize, datbuf_5);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwritelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwritelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwritelevel()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwriteattr                        *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting attribute to the  point \"FloatBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting attribute to the  point \"FloatBuoy Point\" \n"););

  count[0] = 1;
  status = HE5_PTwriteattr(PTid3, "GlobalAttr", H5T_NATIVE_FLOAT, count, &flt);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwriteattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwriteattr()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTwriteattr                        *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTwriteattr... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tWriting attribute to the  point \"FloatBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tWriting attribute to the  point \"FloatBuoy Point\" \n"););

  count[0] = 1;
  status = HE5_PTwriteattr(PTid3, "GlobalAttribute_float", H5T_NATIVE_FLOAT, count, &flt);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTwriteattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTwriteattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTwriteattr()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FloatBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FloatBuoy Point\" \n"););

  status = HE5_PTdetach(PTid3);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTclose                            *
   *********************************************************************************
   */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_PTclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););

   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PTclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file \"SimplePoint.he5\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file \"SimplePoint.he5\" \n"););

   status = HE5_PTclose(ptfid);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\tSuccessfully clossed the \"SimplePoint.he5\" file \n"););
	   MESSAGE(11,fprintf(fp,"\t\tSuccessfully clossed the \"SimplePoint.he5\" file \n"););
	 }


  /*
   *********************************************************************************
   *                          Testing   HE5_PTopen                                 *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTopen... \n"););
  MESSAGE(4,printf("\t===================== \n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTopen... \n"););
  MESSAGE(11,fprintf(fp,"\t===================== \n"););
  ptfid = HE5_PTopen("SimplePoint.he5",H5F_ACC_RDWR);
  if (ptfid == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully re-opened the point file ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully re-opened the point file ****\n"););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"Simple Point\"  \n"););

  PTid1 = HE5_PTattach(ptfid, "Simple Point");
  if (PTid1 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1););
	  MESSAGE(11,fprintf(fp,"\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1) ;);
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTlevelinfo                            *
   *********************************************************************************
   */

   MESSAGE(4,printf("\tTesting HE5_PTlevelinfo... \n"););
   MESSAGE(4,printf("\t========================== \n"););

   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PTlevelinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t========================== \n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving level information  for point \"Simple Point\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving level information  for point \"Simple Point\" \n"););


   status = HE5_PTlevelinfo(PTid1, 0, &level);
   if (status == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
	 }
   else
	 {
	   nflds = level.nfields;

       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\t status = HE5_PTlevelinfo();\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_PTlevelinfo()           %d\n", status););
       MESSAGE(8,printf("\t\tNumber of fields in level:           %d\n", nflds););
	   for (i = 0; i < nflds; i++)
		 {
		   MESSAGE(8,printf("Field name:                  %s \n", level.fieldname[i]););
		   MESSAGE(8,printf("Field rank:                  %d \n", level.rank[i]););
		   MESSAGE(8,printf("Field type ID:               %d \n", level.dtype[i]););
		   for (j = 0; j < level.rank[i]; j++)
			 MESSAGE(8,printf("Field dims:                  %d \n", (int)level.dims[i][j]););
		   MESSAGE(8,printf("Field class:                 %d \n", level.dclass[i]););
		 }

	   MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTlevelinfo(); \n"););
	   MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_PTlevelinfo()            %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tNumber of fields in level:           %d\n", nflds););
	   for (i = 0; i < nflds; i++)
		 {
		   MESSAGE(11,fprintf(fp,"Field name:                  %s \n", level.fieldname[i]););
		   MESSAGE(11,fprintf(fp,"Field rank:                  %d \n", level.rank[i]););
		   MESSAGE(11,fprintf(fp,"Field type ID:               %d \n", level.dtype[i]););
		   for (j = 0; j < level.rank[i]; j++)
			 MESSAGE(11,fprintf(fp,"Field dims:                  %d \n", (int)level.dims[i][j]););
		   MESSAGE(11,fprintf(fp,"Field class:                 %d \n", level.dclass[i]););
		 }
	 }



  /*
   *********************************************************************************
   *                             Testing    HE5_PTnrecs                            *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTnrecs... \n"););
  MESSAGE(4,printf("\t====================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTnrecs... \n"););
  MESSAGE(11,fprintf(fp,"\t====================== \n"););
  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tRetrieving number of records in level 0 in point \"Simple Point\' \n"););
  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tRetrieving number of records in level 0 in  point \"Simple Point\" \n"););

  nrecs = HE5_PTnrecs(PTid1, 0);
  if (nrecs == 0)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tnrecs = HE5_PTnrecs();\n"););
	  MESSAGE(8,printf("\t\tNumber of records returned by HE5_PTnrecs():   %lu \n",(unsigned long)nrecs););
	  MESSAGE(11,fprintf(fp,"\t\tnrecs = HE5_PTnrecs();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tNumber of records returned by HE5_PTnrecs():   %lu \n", (unsigned long)nrecs););
	}

  datasize          = (size_t)sizeof(Sensor);
  inInfo.nfields    = nflds;
  inInfo.datasize   = (size_t)sizeof(Sensor);
  inInfo.rank[0]    = 1;
  inInfo.rank[1]    = 1;
  inInfo.rank[2]    = 1;
  inInfo.offset[0]  = HOFFSET(Sensor, time);
  inInfo.offset[1]  = HOFFSET(Sensor, con);
  inInfo.offset[2]  = HOFFSET(Sensor, spec);
  inInfo.dtype[0]   = H5T_NATIVE_DOUBLE;
  inInfo.dtype[1]   = H5T_NATIVE_FLOAT;
  inInfo.dtype[2]   = H5T_NATIVE_CHAR;
  inInfo.dclass[0]  = H5T_NO_CLASS;
  inInfo.dclass[1]  = H5T_NO_CLASS;
  inInfo.dclass[2]  = H5T_NO_CLASS;
  inInfo.dims[0][0] = 1;
  inInfo.dims[1][0] = 4;
  inInfo.dims[2][0] = 8;
  inInfo.array[0]   = 0;
  inInfo.array[1]   = 1;
  inInfo.array[2]   = 1;

  for( i = 0; i < nflds; i++)
	{
	  inInfo.fieldname[i] = (char *)calloc(64, sizeof(char));
	  strcpy(inInfo.fieldname[i], level.fieldname[i]);
	}



  /*
   *********************************************************************************
   *                             Testing  HE5_PTreadlevel                          *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTreadlevel... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTreadlevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););
  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tRetrieving field values for point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tRetrieving field values for point \"Simple Point\" \n"););

  status = HE5_PTreadlevel(PTid1, 0, &inInfo, &datasize, s);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTreadlevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTreadlevel()           %d\n", status););
	  MESSAGE(8,printf("\n"););
	  for (i = 0; i < nrecs; i++)
		{
		  MESSAGE(8,printf("%lf  %f  %f  %f  %f  %s\n", s[i].time, s[i].con[0], s[i].con[1],s[i].con[2], s[i].con[3], s[i].spec););
		  MESSAGE(11,fprintf(fp,"%lf  %f  %f  %f  %f  %s\n", s[i].time, s[i].con[0], s[i].con[1],s[i].con[2], s[i].con[3], s[i].spec););
		}
	}

  for (i = 0; i < nflds; i++)
	if (inInfo.fieldname[i] != NULL) free(inInfo.fieldname[i]);

  datasize           = (size_t)sizeof(double);
  inInfo2.nfields    = 1;
  inInfo2.datasize   = (size_t)sizeof(double);
  inInfo2.rank[0]    = 1;
  inInfo2.offset[0]  = 0;
  inInfo2.dtype[0]   = H5T_NATIVE_DOUBLE;
  inInfo2.dclass[0]  = H5T_FLOAT;
  inInfo2.dims[0][0] = 1;
  inInfo2.array[0]   = 0;

  inInfo2.fieldname[0] = (char *)calloc(64, sizeof(char));
  strcpy(inInfo2.fieldname[0], level.fieldname[0]);



  /*
   *********************************************************************************
   *                             Testing  HE5_PTreadlevel                          *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTreadlevel... \n"););
  MESSAGE(4,printf("\t========================== \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTreadlevel... \n"););
  MESSAGE(11,fprintf(fp,"\t========================== \n"););
  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tRetrieving field values for point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tRetrieving field values for point \"Simple Point\" \n"););

  status = HE5_PTreadlevel(PTid1, 0, &inInfo2, &datasize, s1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTreadlevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTreadlevel()           %d\n", status););
	  MESSAGE(8,printf("\n"););
	  for (i = 0; i < nrecs; i++)
		{
		  MESSAGE(8,printf("%lf \n", s1[i]););
		  MESSAGE(11,fprintf(fp,"%lf \n", s1[i]););
		}
	}

  if (inInfo2.fieldname[0] != NULL)
	free(inInfo2.fieldname[0]);

  for (i = 0; i < nflds; i++)
	if(level.fieldname[i] != NULL) H5free_memory(level.fieldname[i]);


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"Simple Point\" \n"););

  status = HE5_PTdetach(PTid1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FixedBuoy Point\"  \n"););

  PTid2 = HE5_PTattach(ptfid, "FixedBuoy Point");
  if (PTid2 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2););
	  MESSAGE(11,fprintf(fp,"\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2) ;);
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTinqattrs                             *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTinqattrs... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTinqattrs... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););
  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tRetrieving the number of attributes in the point \"FixedBuoy Point\".\n"););
  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tRetrieving the number of attributes in the  point \"FixedBuoy Point\".\n"););

  nattr  = HE5_PTinqattrs(PTid2, NULL, &strbufsize);
  if (nattr < 0)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(8,printf("\t\tnattr = HE5_PTinqattrs();\n"););
	  MESSAGE(8,printf("\t\tNumber of attributes returned by HE5_PTinqattrs():     %li\n", nattr););
	  MESSAGE(8,printf("\t\tString length of the attribute list:     %li\n", strbufsize););
	  MESSAGE(11,fprintf(fp,"\t\tnattr = HE5_PTinqattrs();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by HE5_PTinqattrs():    %li\n", nattr););
	  MESSAGE(11,fprintf(fp,"\t\tString length of the attribute list:     %li\n", strbufsize););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTreadattr                         *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTreadattr... \n"););
  MESSAGE(4,printf("\t========================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTreadattr... \n"););
  MESSAGE(11,fprintf(fp,"\t========================= \n"););

  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tReading value of the attribute \"GlobalAttr_Integer\" in the point \"FixedBuoy Point\" \n"););

  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tReading value of the attribute \"GlobalAttr_Integer\" in the point \"FixedBuoy Point\" \n"););

  status = HE5_PTreadattr(PTid2, "GlobalAttr_Integer", &IntAttr);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTreadattr();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTreadattr()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTreadattr();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTreadattr()        %d\n",status););
	  MESSAGE(8,printf("\n"););
	  MESSAGE(8,printf("\t\t   Attribute value reads\n"););
	  MESSAGE(8,printf("\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t  Attribute value reads\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(8,printf("\t\t        %d\n", IntAttr););
	  MESSAGE(11,fprintf(fp,"\t\t       %d\n", IntAttr););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTdetach(PTid2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                             Testing    HE5_PTclose                            *
   *********************************************************************************
   */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_PTclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););

   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PTclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file \"SimplePoint.he5\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file \"SimplePoint.he5\" \n"););

   status = HE5_PTclose(ptfid);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\tSuccessfully clossed the \"SimplePoint.he5\" file \n"););
	   MESSAGE(11,fprintf(fp,"\t\tSuccessfully clossed the \"SimplePoint.he5\" file \n"););
	 }


  /*
   *********************************************************************************
   *                          Testing   HE5_PTopen                                 *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTopen... \n"););
  MESSAGE(4,printf("\t===================== \n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTopen... \n"););
  MESSAGE(11,fprintf(fp,"\t===================== \n"););
  ptfid = HE5_PTopen("SimplePoint.he5",H5F_ACC_RDWR);
  if (ptfid == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\t\t*** Successfully re-opened the point file ****\n"););
	  MESSAGE(11,fprintf(fp,"\t\t*** Successfully re-opened the point file ****\n"););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"Simple Point\"  \n"););

  PTid1 = HE5_PTattach(ptfid, "Simple Point");
  if (PTid1 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1););
	  MESSAGE(11,fprintf(fp,"\t\tPTid1 = HE5_PTattach(ptfid, \"Simple Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid1) ;);
	}


  /*
   *********************************************************************************
   *                        Testing    HE5_PTupdatelevel                           *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTupdatelevel... \n"););
  MESSAGE(4,printf("\t============================ \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTupdatelevel... \n"););
  MESSAGE(11,fprintf(fp,"\t============================ \n"););
  MESSAGE(6,printf("\n"););
  MESSAGE(6,printf("\t\tUpdating level \"Concentration\" in  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\n"););
  MESSAGE(11,fprintf(fp,"\t\tUpdating level \"Concentration\" in  point \"Simple Point\" \n"););

  recs[0]           = 0;
  buf_1.time        = 13131313.0;
  buf_1.concentr[0] = 1.11;
  buf_1.concentr[1] = 2.22;
  buf_1.concentr[2] = 3.33;
  buf_1.concentr[3] = 4.44;
  strcpy(buf_1.spec,"AM");

  status = HE5_PTupdatelevel(PTid1, 0, "Concentration", nrec, recs, &buf_1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTupdatelevel();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTupdatelevel()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTupdatelevel();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTupdatelevel()          %d\n",status););
	}

  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"Simple Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"Simple Point\" \n"););

  status = HE5_PTdetach(PTid1);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTattach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTattach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTattach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tAttaching to the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tAttaching to the  point \"FixedBuoy Point\"  \n"););

  PTid2 = HE5_PTattach(ptfid, "FixedBuoy Point");
  if (PTid2 == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(8,printf("\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2););
	  MESSAGE(11,fprintf(fp,"\t\tPTid2 = HE5_PTattach(ptfid, \"FixedBuoy Point\");\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe point ID returned by HE5_PTattach()           %d\n",PTid2) ;);
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTinqdatatype                          *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTinqdatatype... \n"););
  MESSAGE(4,printf("\t============================ \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTinqdatatype... \n"););
  MESSAGE(11,fprintf(fp,"\t============================ \n"););

  MESSAGE(6,printf("\t\tInquiring datatype information for \"Observations\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tInquiring datatype information for \"Observations\"  \n"););

  fieldgroup = HE5_HDFE_DATAGROUP;

  status = HE5_PTinqdatatype(PTid2, "Observations", NULL, fieldgroup, &datatype, &classid, &order, &size);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(8,printf("\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status););
      MESSAGE(8,printf("\tdatatype:    %d \n", (int)datatype););
      MESSAGE(8,printf("\tclass ID:    %d \n", (int)classid););
      MESSAGE(8,printf("\tsize:        %d \n", (int)size););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status) ;);
      MESSAGE(11,fprintf(fp,"\tdatatype:    %d \n", (int)datatype););
      MESSAGE(11,fprintf(fp,"\tclass ID:    %d \n", (int)classid););
      MESSAGE(11,fprintf(fp,"\tsize:        %d \n", (int)size););
	}

  fieldgroup = HE5_HDFE_ATTRGROUP;
  status = HE5_PTinqdatatype(PTid2, NULL, "GlobalAttribute_int", fieldgroup, &datatype, &classid, &order, &size);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(8,printf("\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status););
      MESSAGE(8,printf("\tdatatype:    %d \n", (int)datatype););
      MESSAGE(8,printf("\tclass ID:    %d \n", (int)classid););
      MESSAGE(8,printf("\torder:       %d \n", (int)order););
      MESSAGE(8,printf("\tsize:        %d \n", (int)size););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status) ;);
      MESSAGE(11,fprintf(fp,"\tdatatype:    %d \n", (int)datatype););
      MESSAGE(11,fprintf(fp,"\tclass ID:    %d \n", (int)classid););
      MESSAGE(11,fprintf(fp,"\torder:       %d \n", (int)order););
      MESSAGE(11,fprintf(fp,"\tsize:        %d \n", (int)size););
	}

  fieldgroup = HE5_HDFE_GRPATTRGROUP;
  status = HE5_PTinqdatatype(PTid2, NULL, "GroupAttribute", fieldgroup, &datatype, &classid, &order, &size);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(8,printf("\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status););
      MESSAGE(8,printf("\tdatatype:    %d \n", (int)datatype););
      MESSAGE(8,printf("\tclass ID:    %d \n", (int)classid););
      MESSAGE(8,printf("\torder:       %d \n", (int)order););
      MESSAGE(8,printf("\tsize:        %d \n", (int)size););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status) ;);
      MESSAGE(11,fprintf(fp,"\tdatatype:    %d \n", (int)datatype););
      MESSAGE(11,fprintf(fp,"\tclass ID:    %d \n", (int)classid););
      MESSAGE(11,fprintf(fp,"\torder:       %d \n", (int)order););
      MESSAGE(11,fprintf(fp,"\tsize:        %d \n", (int)size););
	}


  fieldgroup = HE5_HDFE_LOCATTRGROUP;
  status = HE5_PTinqdatatype(PTid2, "Observations", "LocalAttribute", fieldgroup, &datatype, &classid, &order, &size);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(8,printf("\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status););
      MESSAGE(8,printf("\tdatatype:    %d \n", (int)datatype););
      MESSAGE(8,printf("\tclass ID:    %d \n", (int)classid););
      MESSAGE(8,printf("\torder:       %d \n", (int)order););
      MESSAGE(8,printf("\tsize:        %d \n", (int)size););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTinqdatatype();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tThe status returned by HE5_PTinqdatatype()           %d\n",status) ;);
      MESSAGE(11,fprintf(fp,"\tdatatype:    %d \n", (int)datatype););
      MESSAGE(11,fprintf(fp,"\tclass ID:    %d \n", (int)classid););
      MESSAGE(11,fprintf(fp,"\torder:       %d \n", (int)order););
      MESSAGE(11,fprintf(fp,"\tsize:        %d \n", (int)size););
	}


  /*
   *********************************************************************************
   *                         Testing    HE5_PTdetach                               *
   *********************************************************************************
   */

  MESSAGE(4,printf("\tTesting HE5_PTdetach... \n"););
  MESSAGE(4,printf("\t======================= \n"););

  MESSAGE(11,fprintf(fp,"\t\n"););
  MESSAGE(11,fprintf(fp,"\tTesting HE5_PTdetach... \n"););
  MESSAGE(11,fprintf(fp,"\t======================= \n"););

  MESSAGE(6,printf("\t\tDetaching from the  point \"FixedBuoy Point\" \n"););
  MESSAGE(11,fprintf(fp,"\t\tDetaching from the  point \"FixedBuoy Point\" \n"););

  status = HE5_PTdetach(PTid2);
  if (status == FAIL)
	{
	  MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	  errcnt++;
	}
  else
	{
	  MESSAGE(4,printf("\n"););
	  MESSAGE(4,printf("\t\t********Passed Test**********\n"););
	  MESSAGE(11,fprintf(fp,"\n"););
	  MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
	  MESSAGE(10,printf("\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(8,printf("\t\tStatus returned by HE5_PTdetach()           %d\n",status););
	  MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PTdetach();\n"););
	  MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_PTdetach()          %d\n",status););
	}




  /*
   *********************************************************************************
   *                             Testing    HE5_PTclose                            *
   *********************************************************************************
   */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_PTclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););

   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PTclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file \"SimplePoint.he5\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file \"SimplePoint.he5\" \n"););

   status = HE5_PTclose(ptfid);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\tSuccessfully clossed the SimplePoint.he5 file \n"););
	   MESSAGE(11,fprintf(fp,"\t\tSuccessfully clossed the SimplePoint.he5 file \n"););
	   MESSAGE(4,printf("\n"););
	   MESSAGE(4,printf("\t ........... End of Testing. \n"););
	   MESSAGE(11,fprintf(fp,"\n"););
	   MESSAGE(11,fprintf(fp,"\t ............. End of Testing. \n"););
	 }

   if (errcnt == 0)
	 {
	   MESSAGE(11,printf("\n"););
	   MESSAGE(11,printf("\n"););
	   MESSAGE(11,printf("\t\t A L L    T E S T S     P A S S E D \n"););
	   MESSAGE(11,fprintf(fp,"\n"););
	   MESSAGE(11,fprintf(fp,"\n"););
	   MESSAGE(11,fprintf(fp,"\t\t A L L    T E S T S     P A S S E D \n"););
           return_val = 0;
	 }
   else
	 {
	   MESSAGE(11,printf("\n"););
	   MESSAGE(11,printf("\n"););
	   MESSAGE(11,printf("\t\t%d    T E S T S     F A I L E D \n",errcnt););
	   MESSAGE(11,fprintf(fp,"\n"););
	   MESSAGE(11,fprintf(fp,"\n"););
	   MESSAGE(11,fprintf(fp,"\t\t%d    T E S T S     F A I L E D \n",errcnt););
           return_val = 1;
	 }

  MESSAGE(11,fclose(fp););
  return return_val;
}
