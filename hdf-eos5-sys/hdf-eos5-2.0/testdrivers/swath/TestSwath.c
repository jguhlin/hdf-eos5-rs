/*
 ***********************************************************
 *       FILENAME: TestSwath.c                             *
 *                                                         *
 *       PURPOSE:  To test HDF-EOS v.3 Swath interface     *
 *                                                         *
 *       Author:                                           *
 *           A. Muslimov       /Emergent IT Inc.           *
 *           Adura Adekunjo    /L3 Comm. EER corp          *
 *                                                         *
 *       History:                                          *
 *        August 1999  AM   Initial version                *
 *        13-Oct-2000  AM   Updated                        *
 *        02-Dec-2004  AA   Updated with testdrivers for   *
 *                          some new functions (NCR 41197) *
 *                                                         *
 *                                                         *
 ***********************************************************
 */

#include <HE5_HdfEosDef.h>
#include <tutils.h>

#define BUFSIZE  256
#define RANK1    1
#define RANK2    2
int HE5_szip_can_encode(void );

int main(int argc, char *argv[])
{

   FILE        *fp;

   herr_t      status          = FAIL;

   int	       i               = 0; 
   int         j               = 0; 
   int         attr3[4]        = {100, 200, 300, 400}; 
   int         attr3Out[4]     = { 0, 0, 0, 0 };
   int         Verbosity       = 4;
   int         comp_level[ 5 ] = { 0, 0, 0, 0, 0 };
   int         level[ 5 ]      = { 0, 0, 0, 0, 0 };
   int         rank            = 1;
   int         AttrVal[ 4 ]    = {1,2,3,4};
   int         GattrVal[ 4 ]     = {4,3,2,1};
   int         AttrValOut[ 4 ] = {0,0,0,0};
   int         GattrValOut[ 4 ] = {0,0,0,0};
   int         errcnt          = 0;
   int         comp_code       = 0;
   int         code            = 0;
   int         chunk_rank;
   int         rank2           = 0; 
   int         rank3           = 0;
   int         *RankPtr        = (int *)NULL;
   int         inarray[10]     = {1,2,3,4,5,6,7,8,9,10};
   int         fieldgroup      = FAIL;
   int         return_val      = FAIL;

   hid_t       SWid_simple     = FAIL; 
   hid_t       SWid_index      = FAIL;
   hid_t       swfidc_simple   = FAIL;
   hid_t       PeriodID        = FAIL;
   hid_t       RegionID        = FAIL; 
   hid_t       OldRegionID     = FAIL;
   
   hid_t      datatype         = FAIL;
   hid_t       dtype           = FAIL;
   hid_t       dtype1          = FAIL;
   hid_t       ntype[20];
   H5T_class_t  classid    = H5T_NO_CLASS;
   H5T_order_t  order          = H5T_ORDER_ERROR;
   
   long        number          = FAIL;
   long        strbufsize      = FAIL;
   long        *DimListPtr     = (long *)NULL;
   long        *OffsetPtr      = (long *)NULL;
   long        *IncremPtr      = (long *)NULL;
   long        *IndexMap       = (long *)NULL;
   long        IdxMapSize      = FAIL;
   long        Indices[2]      = {0L, 0L};
   long        *UpdateIdx_1    = (long *)NULL;

   size_t      *size           = (size_t *)NULL;
   size_t      datasize        = 0;
   size_t      Size            = 0;
  
   hssize_t    start[ 1 ]      = { 0 };    
   hssize_t    tstart[ 2 ]     = { 0, 0 };

   hsize_t     ndims           = 0;
   hsize_t     nmaps           = 0;
   hsize_t     *SizePtr;
   hsize_t     Dims[8]         = {0,0,0,0,0,0,0,0};
   hsize_t     dims[ 2 ]       = { 2, 2 };
   hsize_t     chunk_dims[ 2 ] = { 0, 0 };
   hsize_t     chunk_dim[ 8 ] = { 0,0,0,0,0,0,0,0}; 
   hsize_t     tedge[ 2 ]      = { 1, 1 };
   hsize_t     edge[ 1 ]       = { 1 };
   

   hsize_t     stride[ 2 ]     = { 1, 1 };
   hssize_t    inq_start[ 2 ]  = { 0, 0 };
   hsize_t     inq_stride[ 2 ] = { 1, 1 };
   hsize_t     inq_edge[ 2 ]   = { 1, 1 };
   hsize_t     *CountPtr       = (hsize_t *)NULL;
   hsize_t     CurrentSize[1]  = { 4 };
   hsize_t     count[1];
   hsize_t     *indices[8];
   
   double     TempData[10][4]  = { 
	                               { 100., 101., 102., 103. },
				  { 110., 111., 112., 113. },
				  { 200., 201., 202., 203. }, 
				  { 210., 211., 212., 213. }, 
				  { 300., 301., 302., 303. }, 
				  { 310., 311., 312., 313. }, 
				  { 400., 401., 402., 403. }, 
				  { 410., 411., 412., 413. }, 
				  { 500., 501., 502., 503. }, 
				  { 510., 511., 512., 513. } 
                                 } ;

   double     LongData[5][4] =  { 
                                  { 100., 102., 104., 106. },
				  { 100., 102., 104., 106. },
				  { 100., 102., 104., 106. }, 
				  { 100., 102., 104., 106. }, 
				  { 100., 102., 104., 106. } 
                                 } ;

   double     LatData[5][4] =   { 
                                  { 24., 24., 24., 24. },
				  { 23., 23., 23., 23. },
				  { 22., 22., 22., 22. }, 
				  { 21., 21., 21., 21. }, 
				  { 20., 20., 20., 20. } 
                                 } ;


   double     CondData[10][4] = { 
                                  { 10., 20., 30., 40. },
                                  { 11., 21., 31., 41. },
                                  { 12., 22., 32., 42. },
                                  { 13., 23., 33., 43. },
                                  { 14., 24., 34., 44. },
                                  { 15., 25., 35., 45. },
                                  { 16., 26., 36., 46. },
                                  { 17., 27., 37., 47. },
                                  { 18., 28., 38., 48. },
                                  { 19., 29., 39., 49. }
                                 } ;
   
   double      Range[2];
   double      CornerLon[2]; 
   double      CornerLat[2]; 
   double      *BufferPtr;
   double      StartTime = 0.0;
   double      StopTime  = 0.0;
   double      *TimeData;
   double      *TimeDataOut;
   double      TempDataOut[10][4];
   double      fillvalue2          = -1.11111;
   double      InqDoubleFillValue  = -1.1111;

   float       IndexedData[8][8];
   float       FakeValue         = 100.;  
   float       IndexedLatData[6][6];
   float       FakeLat           = 60.;
   float       IndexedLonData[6][6];
   float       FakeLon           = 80.;
   float       *BufferFloatPtr;
   float       fillvalue         = -1.11111;
   float       InqFloatFillValue = -1.111111;
   float       GfattrVal = 3.14;
   float       GfattrValOut;
   
   hvl_t       buffer[4];    /* Buffer to write data to the profile */
   hvl_t       buffer_out[4];/* Buffer to read out data from profile */

   char        *FieldListPtr;
   char        *DimNamePtr; 
   char        *DimMapListPtr;
   char        *ObjNamesPtr; 
   char        DimList[200];
   char        *dimlist;


   /* 
	********************************************************************************
	*                 Get comand-line argument (Verbosity)                         *
	********************************************************************************
	*/
   
   
   for (i = 1; i < argc; i++)
	 {
	   if ((argc > i + 1) && ((HDstrcmp(argv[i], "-verbose") == 0) || (HDstrcmp(argv[i], "-v") == 0)))
		 {
		   if (argv[i + 1][0] == 'l')
			 Verbosity = 4;
		   else if (argv[i + 1][0] == 'm')
			 Verbosity = 6;
		   else if (argv[i + 1][0] == 'h')
			 Verbosity = 10;
		   else
			 Verbosity = atoi(argv[i + 1]);
		 } 
	 }
   
   
   /* -----  Generate Data Arrays ----- */
   
   i = 0;
   j = 0;
   
   
   while( i < 8 )
	 {
       while( j < 8 )
		 {
		   IndexedData[ i ][ j ] = FakeValue;
		   j++;
		   FakeValue+=0.1;
		 }
       i++;
       j = 0;
       
	 }
   
   
   /* -------------------------------- */
   
   i = 0;
   j = 0;
   
   
   while( i < 6 )
	 {
       while( j < 6 )
		 {
		   IndexedLatData[ i ][ j ] = FakeLat;
           IndexedLonData[ i ][ j ] = FakeLon;
		   FakeLon-=0.1;
		   j++;
		 }
       FakeLat-= 0.1;
       FakeLon = 80.;
       i++;
       j = 0;
       
	 }
   
   IndexMap = (long *)calloc(6, sizeof(long));
   
   IndexMap[0] = 1L;
   IndexMap[1] = 2L;
   IndexMap[2] = 3L;   
   IndexMap[3] = 5L;
   IndexMap[4] = 6L;
   IndexMap[5] = 7L;
   

   /* Allocate memory for and populate data buffer */
   datasize = 0;
   for (i = 0; i < 4; i++)
	 {
	   buffer[i].p = (hvl_t *)malloc( 25 *(i+1)* sizeof(unsigned int));
	   buffer[i].len  =  25 * (i+1);
	   /* calculate the data buffer size (bytes) */
	   datasize += buffer[i].len * sizeof(unsigned int);
	   for ( j = 0; j < 25 * (i+1); j++)
		 ((unsigned int *)buffer[i].p)[j] = (i+1)*1000 + j;
	 }

   
   /* --------------------------------- */
   
   
   MESSAGE(11,fp=fopen("TestSwath_c.txtout","w"););
   MESSAGE(4,printf("Testing Swath C interface \n"););
   MESSAGE(4,printf("=========================\n\n"););
   MESSAGE(11,fprintf(fp,"Testing Swath C interface \n"););
   MESSAGE(11,fprintf(fp,"=========================\n\n"););
      
   /*
	*********************************************************************************
	*                             Testing    HE5_SWopen                             *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\tTesting HE5_SWopen... \n"););
   MESSAGE(4,printf("\t===================== \n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWopen... \n"););
   MESSAGE(11,fprintf(fp,"\t===================== \n"););
   
   swfidc_simple = HE5_SWopen("SimpleSwath.he5",H5F_ACC_TRUNC);
   if (swfidc_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully opened the swath file ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully opened the swath file ****\n"););
       
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWcreate                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\tTesting HE5_SWcreate... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWcreate... \n"););   
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(6,printf("\t\tCreating swath SIMPLE \n"););   
   MESSAGE(11,fprintf(fp,"\t\tCreating swath SIMPLE \n"););


   /*
        Test allowed characters 
    */
   SWid_simple = HE5_SWcreate(swfidc_simple,"SIMPLE/error"); 
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t*** Successfully detected invalid character in swath name ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully detected invalid character in swath name ****\n"););
	 }
   else
	 {
       MESSAGE(4,printf("\t\t********Error expected but not given**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error expected but not given**********\n"););   
       errcnt++;  
	 }
   SWid_simple = HE5_SWcreate(swfidc_simple,"SIMPLE,error"); 
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t*** Successfully detected invalid character in swath name ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully detected invalid character in swath name ****\n"););
	 }
   else
	 {
       MESSAGE(4,printf("\t\t********Error expected but not given**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error expected but not given**********\n"););   
       errcnt++;  
	 }
   SWid_simple = HE5_SWcreate(swfidc_simple,"SIMPLE;error"); 
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t*** Successfully detected invalid character in swath name ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully detected invalid character in swath name ****\n"););
	 }
   else
	 {
       MESSAGE(4,printf("\t\t********Error expected but not given**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error expected but not given**********\n"););   
       errcnt++;  
	 }

   SWid_simple = HE5_SWcreate(swfidc_simple,"SIMPLE:noerror"); 
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error ':' not allowed in swath name'**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error ':' not allowed in swath name**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully tested ':' character in swath name ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully tested ':' character in swath name ****\n"););
	 }



   /* Create a valid swath for subsequent tests */

   SWid_simple = HE5_SWcreate(swfidc_simple,"SIMPLE"); 
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully created the swath SIMPLE ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the swath SIMPLE ****\n"););
       
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWcreate                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\tTesting HE5_SWcreate... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWcreate... \n"););   
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(6,printf("\t\tCreating swath INDEX \n"););   
   MESSAGE(11,fprintf(fp,"\t\tCreating swath INDEX \n"););
   
   SWid_index = HE5_SWcreate(swfidc_simple,"INDEX"); 
   if (SWid_index == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully created the swath INDEX ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the swath INDEX ****\n"););
       
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefdim                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefdim... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefdim... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(6,printf("\t\tSetting up dimensions for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting up dimensions for swath SIMPLE \n"););   
   
   
   /*                 
	  ------------------------------------- Swath   SIMPLE  ---------------------------------
	  
	  
	  
	  --------------------- D e f i n i n g   d i m e n s i o n   DataTrack  ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension DataTrack in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension DataTrack in swath SIMPLE \n"););
   
   status = HE5_SWdefdim(SWid_simple, "DataTrack", 10);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_simple, \"DataTrack\", 10);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_simple, \"DataTrack\", 10);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*                 
	  --------------------- D e f i n i n g   d i m e n s i o n   DataXtrack ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension DataXtrack in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension DataXtrack in swath SIMPLE \n"););
   
   status = HE5_SWdefdim(SWid_simple, "DataXtrack", 4);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_simple, \"DataXtrack\", 4);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_simple, \"DataXtrack\", 4);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   /*                 
	  --------------------- D e f i n i n g   d i m e n s i o n   Geotrack ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension GeoTrack for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension GeoTrack for swath SIMPLE \n"););
   status = HE5_SWdefdim(SWid_simple, "GeoTrack", 5);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_simple, \"GeoTrack\", 5);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_simple, \"GeoTrack\", 5);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   /*                 
	  --------------------- D e f i n i n g   d i m e n s i o n   GeoXtrack ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension GeoXtrack for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension GeoXtrack for swath SIMPLE \n"););
   status = HE5_SWdefdim(SWid_simple, "GeoXtrack", 4);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_simple, \"GeoXtrack\", 4);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_simple, \"GeoXtrack\", 4);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*                 
	  --------------------- D e f i n i n g   d i m e n s i o n  "Unlim" ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension \"Unlim\" in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension \"Unlim\" in swath SIMPLE \n"););
   
   status = HE5_SWdefdim(SWid_simple, "Unlim", H5S_UNLIMITED);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_simple, \"Unlim\", H5S_UNLIMITED);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_simple, \"Unlim\", H5S_UNLIMITED);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tSetting up dimensions for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting up dimensions for swath INDEX \n"););   
   
   
   /*
	  ---------------------------------- Swath  INDEX  -------------------------------------
	  
	  
	  
	  --------------------- D e f i n i n g   d i m e n s i o n   TrackDim  ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension TrackDim for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension TrackDim for swath INDEX \n"););
   
   status = HE5_SWdefdim(SWid_index, "TrackDim", 8);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_index, \"TrackDim\", 8);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_index, \"TrackDim\", 8);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*
	  --------------------- D e f i n i n g   d i m e n s i o n  XtrackDim  ----------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension XtrackDim for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension TrackDim for swath INDEX \n"););
   
   status = HE5_SWdefdim(SWid_index, "XtrackDim", 8);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_index, \"XtrackDim\", 8);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_index, \"XtrackDim\", 8);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*
	  --------------------- D e f i n i n g   d i m e n s i o n  GeoDim  -------------------
	*/
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension GeoDim for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension GeoDim for swath INDEX \n"););
   
   status = HE5_SWdefdim(SWid_index, "GeoDim", 6);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_index, \"GeoDim\", 6);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_index, \"GeoDim\", 6);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*
	  --------------------- D e f i n i n g   d i m e n s i o n  GeoXDim  -------------------
	*/
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension GeoXDim for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension GeoXDim for swath INDEX \n"););
   
   status = HE5_SWdefdim(SWid_index, "GeoXDim", 6);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdim(SWid_index, \"GeoXDim\", 6);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdim(SWid_index, \"GeoXDim\", 6);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdim %d\n", status););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefdimmap                        *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefdimmap... \n"););
   MESSAGE(4,printf("\t==========================\n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefdimmap... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension maps for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension maps for swath SIMPLE \n"););
   
   /*                 
	  ----------  D e f i n i n g  T r a c k   D i m.   M a p p i n g  ----------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining track dimension mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining track dimension mapping\n"););
   status = HE5_SWdefdimmap(SWid_simple, "GeoTrack", "DataTrack", 0, 2);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdimmap(SWid_simple, \"GeoTrack\",\"DataTrack\", 0, 2);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdimmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdimmap(SWid_simple,\"GeoTrack\", \"DataTrack\", 0, 2);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdimmap %d\n",status););
	 }
   
   
   /*                 
	  ----  D e f i n i n g  C r o s s  T r a c k   D i m.   M a p p i n g  ----
    */
   
   MESSAGE(6,printf("\t\n");); 
   MESSAGE(6,printf("\t\tDefining cross track dimension mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining cross track dimension mapping\n"););
   status = HE5_SWdefdimmap(SWid_simple, "GeoXtrack", "DataXtrack", 0, 1);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdimmap(SWid_simple, \"GeoXtrack\",\"DataXtrack\", 0, 1);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdimmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdimmap(SWid_simple,\"GeoXtrack\", \"DataXtrack\", 0, 1);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdimmap %d\n",status););
	 }
   
   
   /*                 
	  ----  D e f i n i n g  G e o  T r a c k / G e o  X  t r a c k  D i m.   M a p p i n g  ----
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining GeoTrack / GeoXtrack dimension mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining GeoTrack / GeoXtrack dimension mapping\n"););
   status = HE5_SWdefdimmap(SWid_simple, "GeoTrack", "GeoXtrack", 0, 1);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdimmap(SWid_simple, \"GeoTrack\",\"GeoXtrack\", 0, 1);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdimmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdimmap(SWid_simple,\"GeoTrack\", \"GeoXtrack\", 0, 1);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdimmap %d\n",status););
	 }
   
   
   
   
   
   /*                 
	  ---------  D e f i n i n g  G e o  T r a c k / G e o T r a c k  D i m.   M a p p i n g  -------
    */
   
   MESSAGE(6,printf("\t\n");); 
   MESSAGE(6,printf("\t\tDefining Geo track / Geo track dimension mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining Geo track / Geo track dimension mapping\n"););
   status = HE5_SWdefdimmap(SWid_simple, "GeoTrack", "GeoTrack", 0, 1);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefdimmap(SWid_simple, \"GeoTrack\",\"GeoTrack\", 0, 1);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdimmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefdimmap(SWid_simple,\"GeoTrack\", \"GeoTrack\", 0, 1);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdimmap %d\n",status););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefidxmap                        *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefidxmap... \n"););
   MESSAGE(4,printf("\t=================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefidxmap... \n"););
   MESSAGE(11,fprintf(fp,"\t=================\n"););
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining indexed mapping relationships for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining indexed mapping relationships for swath INDEX \n"););
   
   /*                 
	  -------  D e f i n i n g  G e o D i m  / T r a c k D i m  M a p p i n g  -------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining GeoDim/TrackDim mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining GeoDim/TrackDim mapping\n"););
   status = HE5_SWdefidxmap(SWid_index, "GeoDim", "TrackDim", IndexMap);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefidxmap(SWid_index, \"GeoDim\",\"TrackDim\", IndexMap);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefidxmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefidxmap(SWid_index,\"GeoDim\", \"TrackDim\", IndexMap);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefidxmap %d\n",status););
	 }
   
   /*                 
	  -------  D e f i n i n g  G e o X D i m  / X t r a c k D i m  M a p p i n g  -------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining GeoXDim/XtrackDim mapping\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining GeoXDim/XtrackDim mapping\n"););
   status = HE5_SWdefidxmap(SWid_index, "GeoXDim", "XtrackDim", IndexMap);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdefidxmap(SWid_index, \"GeoXDim\",\"XtrackDim\", IndexMap);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefidxmap %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdefidxmap(SWid_index,\"GeoXDim\", \"XtrackDim\", IndexMap);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefidxmap %d\n",status););
	 }
   
   free(IndexMap);
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWsetfillvalue                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWsetfillvalue... \n"););
   MESSAGE(4,printf("\t=============================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWsetfillvalue... \n"););
   MESSAGE(11,fprintf(fp,"\t=============================\n"););
   
   
   
   /*                 
	  --------- S e t t i n g  F i l l  v a l u e  f o r  T e m p e r a t u r e  --------
    */
   
   fillvalue2 = -777.;
   
   MESSAGE(6,printf("\t\n");); 
   MESSAGE(6,printf("\t\tSetting fill value for field Temperature in swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting fill value for field Temperature in swath SIMPLE\n"););
   status = HE5_SWsetfillvalue(SWid_simple, "Temperature", H5T_NATIVE_DOUBLE, &fillvalue2);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWsetfillvalue(SWid_simple,\"Temperature\", &fillvalue2);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWsetfillvalue for field Temperature %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWsetfillvalue(SWid_simple,\"Temperature\", &fillvalue2);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWsetfillvalue for field Temperature %d\n", status););
	 }
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefdatafield                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefdatafield... \n"););
   MESSAGE(4,printf("\t=============================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefdatafield... \n"););
   MESSAGE(11,fprintf(fp,"\t=============================\n"););
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data fields for swath SIMPLE  \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data fields for swath SIMPLE  \n"););
   
   
   /*
	  ----------------------------  Swath  SIMPLE ---------------------------
	  
	  
	  
	  ---------------  D e f i n i n g  T e m p e r a t u r e  --------------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data field Temperature\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Temperature\n"););
   
   status = HE5_SWdefdatafield(SWid_simple, "Temperature", "DataTrack,DataXtrack", NULL,H5T_NATIVE_DOUBLE , 0); 
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
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n");)
		 MESSAGE(10,printf("\t\tstatus=HE5_SWdefdatafield(SWid_simple,\"Temperature\",NULL, H5T_NATIVE_DOUBLE ,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdatafield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefdatafield(SWid_simple,\"Temperature\",NULL, H5T_NATIVE_DOUBLE ,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdatafield %d\n", status););
	 }
   
   
   
   /*
	  ---------------------------------- Defining Appendable field "Count" ---------------------
    */
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining appendable data field Count\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining appendable data field Count\n"););
   
   
   /*                 
	  --------------------   D e f i n i n g   c h u n k i n g   ---------------------
    */

   
   chunk_dims[ 0 ] = 10;
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining chunking for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining chunking for swath SIMPLE \n"););  
   
   status = HE5_SWdefchunk(SWid_simple,RANK1,chunk_dims);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefchunk(SWid_simple,RANK1,chunk_dims);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefchunk %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefchunk(SWid_simple,RANK1,chunk_dims);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefchunk %d\n",status););
	 }
   
   
   
   /*                 
	  ---------------  D e f i n i n g  c o m p r e s s i o n  s c h e m e  --------------
	  
	  ___________________________________________________________________
	  |                                                                 |
	  |    HE5_HDFE_COMP_NONE              0                            |
	  |    HE5_HDFE_COMP_RLE               1                            |
	  |    HE5_HDFE_COMP_NBIT              2                            |
	  |    HE5_HDFE_COMP_SKPHUFF           3                            |
	  |    HE5_HDFE_COMP_DEFLATE           4                            |
          |    HE5_HDFE_COMP_SZIP_CHIP         5                            |
          |    HE5_HDFE_COMP_SZIP_K13          6                            |
          |    HE5_HDFE_COMP_SZIP_EC           7                            |
          |    HE5_HDFE_COMP_SZIP_NN           8                            |
          |    HE5_HDFE_COMP_SZIP_K13orEC      9                            |
          |    HE5_HDFE_COMP_SZIP_K13orNN      10                           | 
          |    HE5_HDFE_COMP_SHUF_DEFLATE      11                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_CHIP    12                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13     13                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_EC      14                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_NN      15                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13orEC 16                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13orNN 17                           |
	  |_________________________________________________________________|
	                                                         
    */
   
   
   comp_code = 16;
   
   /*                 
	  ---------------  D e f i n i n g  c o m p r e s s i o n  l e v e l   --------------
    */
   
   comp_level[ 0 ] = 8;
   
   MESSAGE(6,printf("\t\n"););  
   MESSAGE(6,printf("\t\tDefining compression parameters for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tDefining compression parameters for swath SIMPLE \n"););

#ifdef H5_HAVE_FILTER_SZIP
   if(HE5_szip_can_encode() == 1) {
   status = HE5_SWdefcomp(SWid_simple,comp_code,comp_level);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefcomp(SWid_simple,comp_code, comp_level);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefcomp %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefcomp(SWid_simple,comp_code, comp_level);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefcomp %d\n",status););
	 }
      }
   else printf("No szip encoder provided, the szip compression test is be skipped.\n");
#else
   printf("No szip library available, the szip compression test is be skipped.\n");
#endif

   
   /*                 
	  ---------------   D e f i n i n g   C o u n t   --------------
    */
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining data field Count\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Count\n"););
   status = HE5_SWdefdatafield(SWid_simple, "Count", "DataTrack","Unlim", H5T_NATIVE_INT, 0);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefdatafield(SWid_simple, \"Count\",\"DataTrack\",\"Unlim\",H5T_NATIVE_INT,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdatafield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefdatafield(SWid_simple,\"Count\",\"DataTrack\",\"Unlim\",H5T_NATIVE_INT,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdatafield %d\n", status););
	 }
   


   /*
	*****************************************************************************
	*                             Testing    HE5_PRdefine                       *
	*****************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_PRdefine... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PRdefine... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining profile for swath SIMPLE  \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining profile for swath SIMPLE  \n"););
   
   status = HE5_PRdefine(SWid_simple,"Profile-2000", "DataXtrack", NULL, H5T_NATIVE_UINT); 
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
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n");)
	   MESSAGE(10,printf("\t\tstatus=HE5_PRdefine(SWid_simple,\"Profile-2000\", \"DataXtrack\", NULL, H5T_NATIVE_UINT);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_PRdefine %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_PRdefine(SWid_simple,\"Profile-2000\", \"DataXtrack\", NULL, H5T_NATIVE_UINT);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_PRdefine %d\n", status););
	 }

   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data fields for swath INDEX  \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data fields for swath INDEX  \n"););
   
   
   /*
	  -------------------------------  Swath  INDEX ---------------------------
    */
   
   
   
   /*                 
	  --------- S e t t i n g  F i l l  v a l u e  f o r  I n d e x e d  D a t a  --------
    */
   
   
   fillvalue = -555.;
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tSetting fill value for field Indexed_Data in swath INDEX\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting fill value for field Indexed_Data in swath INDEX\n"););
   status = HE5_SWsetfillvalue(SWid_index, "Indexed_Data", H5T_NATIVE_FLOAT, &fillvalue);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWsetfillvalue(SWid_index,\"Indexed_Data\", &fillvalue);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWsetfillvalue for field Indexed_Data%d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWsetfillvalue(SWid_index,\"Indexed_Data\", &fillvalue);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWsetfillvalue for field Indexed_Data %d\n", status););
	 }
   
   
   
   
   
   
   /*                 
	  ---------------  D e f i n i n g  I n d e x e d _ D a t a  -----------
    */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data field Indexed_Data\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Indexed_Data\n"););
   
   status = HE5_SWdefdatafield(SWid_index, "Indexed_Data", "TrackDim,XtrackDim", NULL,H5T_NATIVE_FLOAT, 0); 
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
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n");)
		 MESSAGE(10,printf("\t\tstatus=HE5_SWdefdatafield(SWid_index,\"Indexed_Data\",NULL, H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdatafield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefdatafield(SWid_index,\"Indexed_Data\",NULL, H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdatafield %d\n", status););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefcomp                          *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefcomp... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefcomp... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   /*                 
	  --------------------   D e f i n i n g   c h u n k i n g   ---------------------
    */
   
   
   chunk_dims[ 0 ] = 5 ;
   chunk_dims[ 1 ] = 2 ;
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining chunking for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining chunking for swath SIMPLE \n"););  
   
   status = HE5_SWdefchunk(SWid_simple,RANK2,chunk_dims);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefchunk(SWid_simple,RANK2,chunk_dims);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefchunk %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefchunk(SWid_simple,RANK2,chunk_dims);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefchunk %d\n",status););
	 }
   
   /*                 
	  ---------------  D e f i n i n g  c o m p r e s s i o n  s c h e m e  --------------
	  
	  
	  ___________________________________________________________________
          |                                                                 |
          |    HE5_HDFE_COMP_NONE              0                            |
          |    HE5_HDFE_COMP_RLE               1                            |
          |    HE5_HDFE_COMP_NBIT              2                            |
          |    HE5_HDFE_COMP_SKPHUFF           3                            |
          |    HE5_HDFE_COMP_DEFLATE           4                            |
          |    HE5_HDFE_COMP_SZIP_CHIP         5                            |
          |    HE5_HDFE_COMP_SZIP_K13          6                            |
          |    HE5_HDFE_COMP_SZIP_EC           7                            |
          |    HE5_HDFE_COMP_SZIP_NN           8                            |
          |    HE5_HDFE_COMP_SZIP_K13orEC      9                            |
          |    HE5_HDFE_COMP_SZIP_K13orNN      10                           |
          |    HE5_HDFE_COMP_SHUF_DEFLATE      11                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_CHIP    12                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13     13                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_EC      14                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_NN      15                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13orEC 16                           |
          |    HE5_HDFE_COMP_SHUF_SZIP_K13orNN 17                           |
          |_________________________________________________________________| 
    */
   
   
   comp_code = 16;
   
   /*                 
	  ---------------  D e f i n i n g  c o m p r e s s i o n  l e v e l   --------------
	*/
   
   comp_level[ 0 ] = 2;
   
   MESSAGE(6,printf("\t\n"););  
   MESSAGE(6,printf("\t\tDefining compression parameters for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tDefining compression parameters for swath SIMPLE \n"););

#ifdef H5_HAVE_FILTER_SZIP
  if(HE5_szip_can_encode() == 1) {
   status = HE5_SWdefcomp(SWid_simple,comp_code,comp_level);
   if (status == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););   
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
	 }
   else
	 {   
       MESSAGE(4,printf("\n"););   
       MESSAGE(4,printf("\t\t********Passed Test (SZIP) **********\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefcomp(SWid_simple,comp_code, comp_level);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefcomp %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefcomp(SWid_simple,comp_code, comp_level);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefcomp %d\n",status););
	 }
         }
   else printf("No szip encoder provided, the szip compression test is be skipped.\n");
#else
   printf("No szip library available, the szip compression test is be skipped.\n");
#endif

   
   /*                 
	  ---------------   D e f i n i n g   C o n d u c t i o n   --------------
	*/
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining data field Conduction\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Conduction\n"););
   status = HE5_SWdefdatafield(SWid_simple, "Conduction", "DataTrack,DataXtrack", NULL, H5T_NATIVE_DOUBLE, 0);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefdatafield(SWid_simple, \"Conduction\",\"DataTrack,DataXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefdatafield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefdatafield(SWid_simple,\"Conduction\",\"DataTrack,DataXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefdatafield %d\n", status););
	 }
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdefgeofield                      *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefgeofield... \n"););
   MESSAGE(4,printf("\t============================ \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefgeofield... \n"););
   MESSAGE(11,fprintf(fp,"\t============================ \n"););
   
   /*
	  --------------------------- Swath  SIMPLE -------------------------------------
    */
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining geolocation fields for swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation fields for swath SIMPLE\n"););
   
   
   
   /*
	  T I M E 
	*/
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining geolocation field Time\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation field Time\n"););
   status = HE5_SWdefgeofield(SWid_simple,"Time","GeoTrack",NULL,H5T_NATIVE_DOUBLE,0);
   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Time\",\"GeoTrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefgeofield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Time\",\"GeoTrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefgeofield %d\n",status););
	 }
   
   
   
   /*
	  L A T I T U D E
	*/
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining geolocation field Latitude\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation field Latitude\n"););
   
   status = HE5_SWdefgeofield(SWid_simple, "Latitude", "GeoTrack,GeoXtrack",NULL,H5T_NATIVE_DOUBLE,0);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Latitude\",\"GeoTrack,GeoXtrack\", NULL, H5T_NATIVE_DOUBLE, 0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefgeofield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Latitude\",\"GeoTrack,GeoXtrack\", NULL, H5T_NATIVE_DOUBLE, 0 );\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefgeofield %d\n",status););
	 }
   
   
   /* 
	  L O N G I T U D E         
	*/
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tDefining geolocation field Longitude\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation field Longitude\n"););
   status = HE5_SWdefgeofield(SWid_simple, "Longitude","GeoTrack,GeoXtrack",NULL,H5T_NATIVE_DOUBLE,0 );
   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Longitude\",\"GeoTrack,GeoXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefgeofield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefgeofield(SWid_simple,\"Longitude\",\"GeoTrack,GeoXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefgeofield %d\n",status););
	 }
   
   
   
   /*
	  -------------------------------- Swath  INDEX -------------------------------------
	*/
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining geolocation fields for swath INDEX\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation fields for swath INDEX\n"););
   
   /*
	  L A T I T U D E
	*/
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining geolocation field Latitude\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation field Latitude\n"););
   status = HE5_SWdefgeofield(SWid_index,"Latitude","GeoDim,GeoXDim",NULL,H5T_NATIVE_FLOAT,0);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefgeofield(SWid_index,\"Latitude\",\"GeoDim,GeoXDim\",NULL,H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefgeofield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefgeofield(SWid_index,\"Latitude\",\"GeoDim,GeoXDim\",NULL,H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefgeofield %d\n",status););
	 }
   
   
   /*
	  L O N G I T U D E
	*/
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining geolocation field Longitude\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining geolocation field Longitude\n"););
   status = HE5_SWdefgeofield(SWid_index,"Longitude","GeoDim,GeoXDim",NULL,H5T_NATIVE_FLOAT,0);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWdefgeofield(SWid_index,\"Longitude\",\"GeoDim,GeoXDim\",NULL,H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWdefgeofield %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWdefgeofield(SWid_index,\"Longitude\",\"GeoDim,GeoXDim\",NULL,H5T_NATIVE_FLOAT,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWdefgeofield %d\n",status););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWwritefield                        *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWwritefield... \n"););
   MESSAGE(4,printf("\t===========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWwritefield... \n"););
   MESSAGE(11,fprintf(fp,"\t===========================\n"););
   
   
   
   /*
	  -------------------------------- Swath SIMPLE -------------------------
	  
	  
	  --------- W r i t i n g    T i m e     i n    a    s w a t h   --------
	*/
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Time in swath SIMPLE \n");); 
   MESSAGE(11,fprintf(fp,"\n"););  
   MESSAGE(11,fprintf(fp,"\t\tWriting field Time in swath SIMPLE \n"););
   
   TimeData = (double *)calloc( 5, sizeof( double));
   
   for ( i = 0; i < 5; i++ ) 
	 {
	   TimeData[ i ] = 5.e7 + 5.e6 * i ;
	 }
   
   
   tstart[ 0 ]   = 0;
   tedge[ 0 ]    = 5;
   
   
   status = HE5_SWwritefield(SWid_simple,"Time", tstart, NULL, tedge, TimeData);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple,\"Time\", tstart,NULL, tedge, TimeData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Time\", tstart,NULL, tedge, TimeData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield %d\n",status););
	 }
   
   
   free( TimeData );
   TimeData = NULL;
   
   
   /*                 
	  --------- W r i t i n g    T e m p e r a t u r e    i n    a    s w a t h --------
	*/
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tWriting field Temperature in swath SIMPLE \n"););   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Temperature in swath SIMPLE \n"););
   
   tstart[ 0 ]   = 0;
   tstart[ 1 ]   = 0;
   tedge[ 0 ]    = 10;
   tedge[ 1 ]    = 4;
   
   status = HE5_SWwritefield(SWid_simple, "Temperature", tstart, NULL, tedge, TempData);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple,\"Temperature\", tstart,stride, tedge, TempData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Temperature\", tstart, stride, tedge, TempData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield %d\n",status););
	 }
   
   
   
   /*                 
	  --------- W r i t i n g    C o n d u c t i o n    i n    a    s w a t h --------
	*/
   
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting field Conduction in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Conduction in swath SIMPLE \n"););
   
   status = HE5_SWwritefield(SWid_simple, "Conduction", tstart, NULL, tedge, CondData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple, \"Conduction\",tstart, stride, tedge, CondData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Conduction in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Conduction\", tstart, stride, tedge, CondData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Conduction in swath SIMPLE %d\n",status););
	 }
   
   
   
   /*                 
	  --------- W r i t i n g    L o n g i t u d  e / L a t i t u d e    i n   a    s w a t h --------
	*/
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Longitude in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Longitude in swath SIMPLE \n"););
   
   tedge[0]  = 5;
   
   status = HE5_SWwritefield(SWid_simple, "Longitude", tstart, NULL, tedge, LongData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple, \"Longitude\",tstart, NULL, tedge, LongData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Longitude in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Longitude\", tstart, NULL, tedge, LongData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Conduction in swath SIMPLE %d\n",status););
	 }
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Latitude in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Latitude in swath SIMPLE \n"););
   
   status = HE5_SWwritefield(SWid_simple, "Latitude", tstart, NULL, tedge, LatData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple, \"Latitude\",tstart, NULL, tedge, LatData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Latitude in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Latitude\", tstart, NULL, tedge, LatData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field latitude in swath SIMPLE %d\n",status););
	 }   
   
   
   /*                 
	  --------- W r i t i n g    C o u n t    i n    a    s w a t h --------
	*/
   tstart[0] = 0; tedge[0] = 10;
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting field Count in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Count in swath SIMPLE \n"););
   
   status = HE5_SWwritefield(SWid_simple, "Count", tstart, NULL, tedge, inarray);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple, \"Count\",tstart, NULL, tedge, inarray);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Count in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Count\", tstart, stride, NULL, inarray);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Conduction in swath SIMPLE %d\n",status););
	 }
   


   /*
	**************************************************************************
	*                             Testing   HE5_PRwrite                      *
	**************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_PRwrite... \n"););
   MESSAGE(4,printf("\t======================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PRwrite... \n"););
   MESSAGE(11,fprintf(fp,"\t======================\n"););
   

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting profile \"Profile-2000\"  in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting profile \"Profile-2000\" in swath SIMPLE \n"););

 
   edge[ 0 ] = 4;
   status = HE5_PRwrite(SWid_simple, "Profile-2000", start, NULL, edge, datasize, buffer);
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
       MESSAGE(10,printf("\t\tstatus =  HE5_PRwrite(SWid_simple, \"Profile-2000\", start, NULL, edge, datasize, buffer);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_PRwrite in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_PRwrite(SWid_simple,\"Profile-2000\", start, NULL, edge, datasize, buffer);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_PRwrite in swath SIMPLE %d\n",status););
	 }   

   
   /*
	  -------------------------------- Swath INDEX -----------------------------------
	  
	  
	  ------- W r i t i n g   I n d e x e d _ D a t a   i n   a   s w a t h   --------
	*/
   
   
   tstart[ 0 ]   = 0;
   tstart[ 1 ]   = 0;
   tedge[ 0 ]    = 8;
   tedge[ 1 ]    = 4;
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Indexed_Data in swath INDEX \n");); 
   MESSAGE(11,fprintf(fp,"\n"););  
   MESSAGE(11,fprintf(fp,"\t\tWriting field Indexed_Data in swath INDEX \n"););
   
   status = HE5_SWwritefield(SWid_index, "Indexed_Data", tstart, NULL, tedge, IndexedData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_index, \"Indexed_Data\",tstart,NULL,tedge,IndexedData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Indexed_Data in swath INDEX %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Indexed_Data\",tstart,NULL,tedge,IndexedData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Indexed_Data in swath INDEX %d\n",status););
	 }
   
   
   /*
	  ------- W r i t i n g    L a t i t u d e / L o n g i t u d e    i n   a   s w a t h   --------
    */
   
   
   tstart[ 0 ]   = 0;
   tstart[ 1 ]   = 0;
   tedge[ 0 ]    = 6;
   tedge[ 1 ]    = 6;
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Latitude in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););  
   MESSAGE(11,fprintf(fp,"\t\tWriting field Latitude in swath INDEX \n"););
   
   status = HE5_SWwritefield(SWid_index, "Latitude", tstart, NULL, tedge, IndexedLatData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_index, \"Latitude\",tstart,NULL,tedge,IndexedLatData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Latitude in swath INDEX %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Latitude\",tstart,NULL,tedge,IndexedLatData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Latitude in swath INDEX %d\n",status););
	 }
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting field Longitude in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););   
   MESSAGE(11,fprintf(fp,"\t\tWriting field Longitude in swath INDEX \n"););
   
   status = HE5_SWwritefield(SWid_index, "Longitude", tstart, NULL, tedge, IndexedLonData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_index, \"Longitude\",tstart,NULL,tedge,IndexedLonData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_SWwritefield for field Longitude in swath INDEX %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Longitude\",tstart,NULL,tedge,IndexedLonData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_SWwritefield for field Longitude in swath INDEX %d\n",status););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWwriteattr                         *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWwriteattr... \n"););
   MESSAGE(4,printf("\t==========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWwriteattr... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   
   /*                 
	  --------- W r i t i n g   " D r i f t "    i n    a    s w a t h --------
    */
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting attribute Drift in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););   
   MESSAGE(11,fprintf(fp,"\t\tWriting attribute Drift in swath SIMPLE \n"););   
   status = HE5_SWwriteattr(SWid_simple,"Drift",H5T_NATIVE_INT,CurrentSize,AttrVal);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwriteattr(SWid_simple, \"Drift\",H5T_NATIVE_INT, 4, AttrVal);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWwriteattr for attribute Drift in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwriteattr(SWid_simple, \"Drift\",H5T_NATIVE_INT, 4, AttrVal);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWwriteattr for attribute Drift in swath SIMPLE %d\n",status););

       MESSAGE(11,fprintf(fp,"\t\t Attribute element   Value to write \n"););
       MESSAGE(11,printf("\t\t Attribute element   Value to write \n"););

	   for( i = 0; i < 4 ; i++ )  
		 {
		   MESSAGE(11,fprintf(fp,"\t\t    %d                   %d \n",i+1,AttrVal[ i ] ););
		   MESSAGE(11,printf("\t\t    %d                   %d \n", i+1,AttrVal[ i ] ););
		 }


	 }
      
         
 /*
        *********************************************************************************
        *                             Testing   HE5_SWwritegrpattr                         *
        *********************************************************************************
        */
 
 
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWwritegrpattr... \n"););
   MESSAGE(4,printf("\t==========================\n"););
 
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWwritegrpattr... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting attribute GroupAttribute in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting attribute GroupAttribute in swath SIMPLE \n");); 
   status = HE5_SWwritegrpattr(SWid_simple,"GroupAttribute",H5T_NATIVE_INT,CurrentSize,GattrVal);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritegrpattr(SWid_simple, \"GroupAttribute\",H5T_NATIVE_INT, 4,GattrVal);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWwritegrpattr for attribute GroupAttribute in swath SIMPLE %d \n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritegrpattr(SWid_simple, \"GroupAttribute\",H5T_NATIVE_INT, 4, GattrVal);\n"););
       
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWwritegrpattr for attribute GroupAttribute in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\t Attribute element   Value to write \n"););
       MESSAGE(11,printf("\t\t Attribute element   Value to write \n"););
       for( i = 0; i < 4 ; i++ )
                 {
                   MESSAGE(11,fprintf(fp,"\t\t    %d                   %d \n",i+1,GattrVal[ i ] ););
                   MESSAGE(11,printf("\t\t    %d                   %d \n", i+1,GattrVal[ i ] ););
                 }


         }

/*
        *********************************************************************************
        *                             Testing   HE5_SWwritegeogrpattr                         *
        *********************************************************************************
        */


   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWwritegeogrpattr... \n"););
   MESSAGE(4,printf("\t==========================\n"););

   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWwritegeogrpattr... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting attribute ScalarFloat in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting attribute ScalarFloat in swath SIMPLE \n"););

   count[0] = 1;
   status = HE5_SWwritegeogrpattr(SWid_simple,"ScalarFloat",H5T_NATIVE_FLOAT,count,&GfattrVal);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritegeogrpattr(SWid_simple, \"ScalarFloat\",H5T_NATIVE_FLOAT, 4,GfattrVal);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWwritegeogrpattr for attribute ScalarFloat in swath SIMPLE %d \n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritegeogrpattr(SWid_simple, \"ScalarFloat\",H5T_NATIVE_FLOAT, 4, GfattrVal);\n");)
;

       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWwritegeogrpattr for attribute ScalarFloat in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\t Attribute element   Value to write \n"););
       MESSAGE(11,printf("\t\t Attribute element   Value to write \n"););
       
   
   }


   /*
	*********************************************************************************
	*                             Testing   HE5_SWdefvrtregion                      *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefvrtregion... \n"););
   MESSAGE(4,printf("\t=============================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefvrtregion... \n"););
   MESSAGE(11,fprintf(fp,"\t=============================\n"););
   
   Range[ 0 ] = 5.5e7;
   Range[ 1 ] = 6.5e7;
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining vert region Time \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining vert region Time \n"););
   
   RegionID = HE5_SWdefvrtregion(SWid_simple,HE5_HDFE_NOPREVSUB,"Time",Range);
   if (RegionID == FAIL)
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
       MESSAGE(8,printf("\t\tRegion ID returned by SWdefvrtregion = %ld\n", (long) RegionID););
       MESSAGE(11,fprintf(fp,"\t\tRegion ID returned by SWdefvrtregion = %ld\n", (long) RegionID););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWreadfield                         *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadfield... \n"););
   MESSAGE(4,printf("\t==========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadfield... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   
   
   /*                 
	  --------- R e a d i n g    T i m e     f r o m    a    s w a t h   --------
    */
   
   
   tstart[ 0 ]   = 0;
   tedge[ 0 ]    = 5;
   
   TimeDataOut = (double *)calloc( 5, sizeof( double ));   
   
   MESSAGE(6,printf("\n"););   
   MESSAGE(6,printf("\t\tReading field Time from  swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););  
   MESSAGE(11,fprintf(fp,"\t\tReading field Time from swath SIMPLE \n"););
   
   status = HE5_SWreadfield(SWid_simple,"Time",tstart,NULL,tedge,TimeDataOut);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWreadfield(SWid_simple,\"Time\",tstart,NULL,tedge,TimeDataOut);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWreadfield %d\n", status ););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWreadfield(SWid_simple,\"Time\",tstart,NULL,tedge,TimeDataOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWreadfield %d\n", status ););
       MESSAGE(8,printf("\t\tFirst five values of \"Time\" field read\n"););
       MESSAGE(11,fprintf(fp,"\t\tFirst five values of \"Time\" field read\n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(11,fprintf(fp,"\n"););
	   for( i = 0; i < 5 ; i++)
		 {
		   MESSAGE(8,printf("\t\t      %lf\n", TimeDataOut[i]););
		   MESSAGE(11,fprintf(fp,"\t\t       %lf\n", TimeDataOut[i]););
		 }
	 }
   
   free(TimeDataOut);
   TimeDataOut = NULL;

   /*
	*********************************************************************************
	*                             Testing   HE5_PRread                              *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_PRread... \n"););
   MESSAGE(4,printf("\t=====================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_PRread... \n"););
   MESSAGE(11,fprintf(fp,"\t=====================\n"););

   start[0] = 0;
   edge[0]  = 4;
   status = HE5_PRread(SWid_simple, "Profile-2000", start, NULL, edge, buffer_out);
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
       MESSAGE(10,printf("\t\tstatus=HE5_PRread(SWid_simple, \"Profile-2000\", start, NULL, edge, buffer_out);\n"););
       MESSAGE(8,printf("\t\tstatus returned by HE5_PRread %d\n", status ););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_PRread(SWid_simple,\"Profile-2000\", start, NULL, edge, buffer_out);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by HE5_PRread %d\n", status ););
       MESSAGE(8,printf("\t\tThe first two elements of \"Profile-2000\" data read\n"););
       MESSAGE(11,fprintf(fp,"\t\tThe firdt two elements of \"Profile-2000\" data read\n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(11,fprintf(fp,"\n"););

	 }
   for (i = 0; i < 4; i++)
	 {                      
	   MESSAGE(8,printf("\t\tThe %d-th element length is %d \n", i, (unsigned)buffer_out[i].len););
	   MESSAGE(11,fprintf(fp,"\t\tThe %d-th element length is %d \n", i, (unsigned)buffer_out[i].len););
	   for (j = 0; j < 2; j++)
		 { 
		   MESSAGE(8,printf("\t\t            %d \n", ((unsigned int *)buffer_out[i].p)[j]););
		   MESSAGE(11,fprintf(fp,"\t\t            %d \n", ((unsigned int *)buffer_out[i].p)[j]););
		 }
	 }
   

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tReclaiming the space ... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tReclaiming the space ... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););

   status = HE5_PRreclaimspace(SWid_simple, "Profile-2000", buffer_out);
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
       MESSAGE(10,printf("\t\tstatus=HE5_PRreclaimspace(SWid_simple, \"Profile-2000\", buffer_out);\n"););
       MESSAGE(8,printf("\t\tstatus returned by HE5_PRreclaimspace %d\n", status ););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_PRreclaimspace(SWid_simple,\"Profile-2000\", buffer_out);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by HE5_PRreclaimspace %d\n", status ););
	 }


   /*
	*********************************************************************************
	*                             Testing   HE5_SWreadattr                          *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadattr... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadattr... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   
   /*                 
	  --------- R e a d i n g   " D r i f t "    a t t r i b u t e  --------
	*/
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading attribute Drift from swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading attribute Drift from swath SIMPLE \n"););
   
   status = HE5_SWreadattr(SWid_simple,"Drift", AttrValOut);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWreadattr(SWid_simple,\"Drift\", AttrValOut);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWreadattr for attribute Drift in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWreadattr(SWid_simple,\"Drift\", AttrValOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWreadattr for attribute Drift in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\t Attribute elements read \n"););
       MESSAGE(8,printf("\t\t Attribute elements read \n"););      
       MESSAGE(8,printf("\n"););
       MESSAGE(11,fprintf(fp,"\n"););
	   for( i = 0; i < 4 ; i++ )  
		 {
		   MESSAGE(11,fprintf(fp,"\t\t            %d \n", AttrValOut[ i ] ););
		   MESSAGE(8,printf("\t\t               %d \n", AttrValOut[ i ] ););       
		 }
	 }
  

    /*
        *********************************************************************************
        *                             Testing   HE5_SWreadgrpattr                       *
        *********************************************************************************
        */
  
  
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadgrpattr... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadgrpattr... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading attribute GroupAttribute from swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading attribute GroupAttribute from swath SIMPLE \n"););
  
   status = HE5_SWreadgrpattr(SWid_simple,"GroupAttribute", GattrValOut);
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
 /*    MESSAGE(4,printf("Group attribute values:\n"););
       for (i = 0; i < 4; i++)
       MESSAGE(4,printf("\t\t %d \n", GattrValOut[ i ] );); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus = HE5_SWreadgrpattr(SWid_simple,\"GroupAttribute\", GattrValOut);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWreadgrpattr for attribute GroupAttribute in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWreadgrpattr(SWid_simple,\"GroupAttribute\", GattrValOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWreadgrpattr for attribute GroupAttribute in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\t Attribute elements read \n"););
       MESSAGE(8,printf("\t\t Attribute elements read \n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(11,fprintf(fp,"\n"););
           for( i = 0; i < 4 ; i++ ) 
                 {
                   MESSAGE(11,fprintf(fp,"\t\t            %d \n", GattrValOut[ i ] ););
                   MESSAGE(8,printf("\t\t               %d \n", GattrValOut[ i ] ););
                 }
         }


   /*
     
      *********************************************************************************
      *                             Testing   HE5_SWreadgeogrpattr                    *
      *********************************************************************************
        */
 
 
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadgeogrpattr... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadgeogrpattr... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading attribute ScalarFloat from swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading attribute ScalarFloat from swath SIMPLE \n"););
 
   status = HE5_SWreadgeogrpattr(SWid_simple,"ScalarFloat", &GfattrValOut);
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
   /*  MESSAGE(4,printf("Geolocation fields group attribute values:\n"););
       MESSAGE(4,printf("\t\t %f \n", GfattrValOut );); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus = HE5_SWreadgeogrpattr(SWid_simple,\"GroupAttribute\", GattrValOut);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWreadgeogrpattr for attribute GroupAttribute in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWreadgeogrpattr(SWid_simple,\"GroupAttribute\", GattrValOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWreadgeogrpattr for attribute GroupAttribute in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\t Attribute elements read \n"););
       MESSAGE(8,printf("\t\t Attribute elements read \n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(8,printf("\t\t               %f \n", GfattrValOut););
         }


   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWnentries                          *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWnentries... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWnentries... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   
   /*                 
	  -----  R e t r i e v e   #  o f   d i m e n s i o n s   i n   a   s w a t h -----
	*/
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve number of dimensions in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve number of dimensions in swath SIMPLE \n"););
   
   number = HE5_SWnentries(SWid_simple,0,&strbufsize);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,0,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of dimensions  %li\n",number););
       MESSAGE(8,printf("\t\tString size of dimension entries:        %li (bytes)\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,0,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of dimensions  %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tString size of dimension entries:        %li (bytes)\n",strbufsize););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWinqdims                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqdims... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqdims... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   DimNamePtr      = (char  *)calloc( 1, strbufsize+1);
   
   
   /*                 
	  -----  R e t r i e v e   d i m e n s i o n   l i s t  f o r   a   s w a t h -----
    */   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve the dimension information for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the dimension information for swath SIMPLE \n"););
   
   
   number = HE5_SWinqdims(SWid_simple,DimNamePtr,Dims);   
   if (number == FAIL)
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
       MESSAGE(8,printf("\t\tnumber=HE5_SWinqdims(SWid_simple,DimNamePtr,DimListPtr);\n"););
       MESSAGE(8,printf("\t\tDimension list: \n"););
       MESSAGE(8,printf("\t\t                %s\n",DimNamePtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqdims(SWid_simple,DimNamePtr,DimListPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tDimension list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t                %s\n",DimNamePtr););
       MESSAGE(8,printf("\t\tDimension sizes: \n"););
       MESSAGE(11,fprintf(fp,"\t\tDimension sizes: \n"););
       for (i = 0; i < number; i++)
		 {
		   MESSAGE(8,printf("\t\t                %lu\n",(unsigned long)Dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t                %lu\n",(unsigned long)Dims[i]););
		 }
	 }
   
   
   free(DimNamePtr);
   DimNamePtr = NULL;
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWinqmaps                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqmaps... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqmaps... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   /*                 
	  -- R e t r i e v e   d i m e n s i o n   m a p p i n g s   i n   a   s w a t h --
    */ 
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tRetrieve dimension mappings for swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve dimension mappings for swath SIMPLE\n"););
   number = HE5_SWnentries(SWid_simple,1,&strbufsize);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,1,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of dimension mappings %li\n",number););
       MESSAGE(8,printf("\t\tSize of dimension mapping entries %li (bytes)\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,1,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of dimension mappings %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tSize of dimension mapping entries %li (bytes)\n",strbufsize););
	 }
   
   
   /*                 
	  -- R e t r i e v e   g e o l o c a t i o n   r e l a t i o n s  f o r  a  s w a t h --
	*/   
   
   
   OffsetPtr       = (long *)calloc(number, sizeof(long));   
   IncremPtr       = (long *)calloc(number, sizeof(long));
   DimMapListPtr   = (char *)calloc( BUFSIZE, sizeof(char));
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tRetrieve geolocation relations for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve geolocation relations for swath SIMPLE \n"););
   
   number = HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);\n"););
       MESSAGE(8,printf("\t\tNumber of maps %li\n", number););
       MESSAGE(6,printf("\t\tGeolocation relations: \n"););
       MESSAGE(6,printf("\t\t  %s\n",DimMapListPtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of maps %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tGeolocation relations: \n"););
       MESSAGE(11,fprintf(fp,"\t\t  %s\n",DimMapListPtr););

       MESSAGE(6,printf("\t\t  Offset     Increment \n"););
       MESSAGE(11,fprintf(fp,"\t\t  Offset     Increment \n"););
       for (i = 0; i < number; i++)
		 {
		   MESSAGE(6,printf("\t\t      %li          %li \n",OffsetPtr[i], IncremPtr[i]););
		   MESSAGE(11,fprintf(fp,"\t\t      %li          %li \n",OffsetPtr[i], IncremPtr[i]););
		 }
       
	 }
   
   
   free(OffsetPtr);
   free(IncremPtr);
   free(DimMapListPtr);
   OffsetPtr = NULL;
   IncremPtr = NULL;
   DimMapListPtr = NULL;
   
   
   
   /*                 
	  -- R e t r i e v e   t h e   I n d e x   d i m e n s i o n   m a p p i n g s   --
    */   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve the Index dimension mappings for swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\n"); );
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the Index dimension mappings for swath SIMPLE\n"); );
   
   number = HE5_SWnentries(SWid_simple,2,&strbufsize);
   if ( number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,2,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of Indexed mappings %li\n",number););
       MESSAGE(8,printf("\t\tSize of index mapping entries:  %li (bytes)\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,2,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of Indexed mappings %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tSize of index mapping entries:  %li (bytes)\n",strbufsize););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWinqgeofields                      *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqgeofields... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqgeofields... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about Geolocation fields in swath SIMPLE \n"););   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about Geolocation fields in swath SIMPLE \n"););
   
   number = HE5_SWnentries(SWid_simple,3,&strbufsize);
   if ( number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,3,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of geo fields) returned by SWnentries %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,3,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of geo fields) returned by SWnentries %li\n",number););
	 }
   
   
   /*                 
	  -- R e t r i e v e   g e o l o c a t i o n   f i e l d s   i n   a   s w a t h   --
    */   
   
   
   RankPtr         = (int *)calloc( number, sizeof(int));
   FieldListPtr    = (char  *)calloc( BUFSIZE, sizeof(char));
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tRetrieve more information about  Geolocation fields\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about Geolocation fields\n"););
   
   number = HE5_SWinqgeofields(SWid_simple,FieldListPtr,RankPtr,ntype);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqgeofields(SWid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(8,printf("\t\tList of Geolocation fields:\n"););
       MESSAGE(8,printf("\t\t %s\n",FieldListPtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqgeofields(SWid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(11,fprintf(fp,"\t\tList of Geolocation fields:\n"););
       MESSAGE(11,fprintf(fp,"\t\t %s\n",FieldListPtr);); 
       MESSAGE(8,printf("\t\tRanks:       Data types:\n"););
       MESSAGE(11,fprintf(fp,"\t\tRanks:        Data types:\n"););

	   for (i = 0; i < number; i++)
		 {
           MESSAGE(8,printf("\t\t %d             %ld\n", RankPtr[i], (long) ntype[i]););
           MESSAGE(11,fprintf(fp,"\t\t %d             %ld\n", RankPtr[i], (long) ntype[i]););
		 }
    
	 }
   
   free(RankPtr);
   RankPtr = NULL;
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWinqdatafields                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqdatafields... \n"););
   MESSAGE(4,printf("\t============================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqdatafields... \n"););
   MESSAGE(11,fprintf(fp,"\t============================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about Data fields in swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about Data fields in swath SIMPLE\n"););
   
   number = HE5_SWnentries(SWid_simple,4,&strbufsize);   
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,4,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of data fields) returned by SWnentries %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,4,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of data fields) returned by SWnentries %li\n",number););
	 }
   
   RankPtr         = (int *)calloc(number, sizeof(int));
   
   MESSAGE(6,printf("\t\tRetrieve the Data fields in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the Data fields in swath SIMPLE \n"););
	 
   number = HE5_SWinqdatafields(SWid_simple,FieldListPtr,RankPtr,ntype);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqdatafields(SWid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqdatafields(SWid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(8,printf("\t\tList of Data fields:\n"););
       MESSAGE(8,printf("\t\t  %s\n",FieldListPtr););
       MESSAGE(11,fprintf(fp,"\t\tList of Data fields:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %s\n",FieldListPtr););
       MESSAGE(8,printf("\t\tRanks:       Data types:\n"););
       MESSAGE(11,fprintf(fp,"\t\tRanks:        Data types:\n"););

	   for (i = 0; i < number; i++)
		 {
           MESSAGE(8,printf("\t\t %d             %ld\n", RankPtr[i], (long) ntype[i]););
           MESSAGE(11,fprintf(fp,"\t\t %d             %ld\n", RankPtr[i], (long) ntype[i]););
		 }
	 }
   
   free(RankPtr);
   free(FieldListPtr);


       /*
        *********************************************************************************
        *                             Testing   HE5_SWwritelocattr                      *
        *********************************************************************************
*/
       MESSAGE(4,printf("\tTesting HE5_SWwritelocattr... \n"););
       MESSAGE(4,printf("\t========================== \n"););

       MESSAGE(11,fprintf(fp,"\t\n"););
       MESSAGE(11,fprintf(fp,"\tTesting HE5_SWwritelocattr... \n"););
       MESSAGE(11,fprintf(fp,"\t========================== \n"););
   
       MESSAGE(6,printf("\n"););
       MESSAGE(6,printf("\t\tWriting attribute \"LocaAttribute\" to the field \"Count\"  \n"););
       MESSAGE(11,fprintf(fp,"\t\n"););
       MESSAGE(11,fprintf(fp,"\t\tWriting attribute \"LocalAttribute\" to the \"Count\" \n"););


      count[0] = 4;
      status = HE5_SWwritelocattr(SWid_simple, "Count", "LocalAttribute",H5T_NATIVE_INT, count, attr3);
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
        MESSAGE(10,printf("\t\tstatus =HE5_SWwritelocattr(SWid_simple, \"Count\", \"LocalAttribute\",H5T_NATIVE_INT, count, attr3); \n");
);
        MESSAGE(8,printf("\t\tStatus returned by HE5_GDwritelocattr()         %d\n",status););
        MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritelocattr(SWid_simple, \"Count\", \"LocalAttribute\",H5T_NATIVE_INT, count, attr3);\
n");)
        MESSAGE(11,fprintf(fp,"\t\tStatus returned by HE5_SWwritelocattr()         %d\n",status););

      }

      /*
        *********************************************************************************
        *                             Testing   HE5_SWreadlocattr                       *
        *********************************************************************************
        */


   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadlocattr... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadlocattr... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading attribute LocalAttribute from Count field \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading attribute LocalAttribute from Count field \n"););

   status = HE5_SWreadlocattr(SWid_simple, "Count", "LocalAttribute", attr3Out);
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
      MESSAGE(10,printf("\t\tstatus = HE5_SWreadlocattr(SWid_simple,\"Count\", \"LocalAttribute\", attr3Out);\n"););
      MESSAGE(8,printf("\t\tValue returned by SWreadlocattr for attribute LocalAttribute in field Count %d\n",status););
      MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWreadlocattr(SWid_simple,\"Count\", \"LocalAttribute\", attr3Out);\n"););
      MESSAGE(11,fprintf(fp,"\t\tValue returned by SWreadlocattr for attribute LocalAttribute in field Count %d\n",status););
      MESSAGE(11,fprintf(fp,"\t\t Attribute elements read \n"););
      MESSAGE(8,printf("\t\t Attribute elements read \n"););
      MESSAGE(8,printf("\n"););
      MESSAGE(11,fprintf(fp,"\n"););
           for( i = 0; i < 4 ; i++ )
                 {
                   MESSAGE(11,fprintf(fp,"\t\t            %d \n", attr3Out[ i ] ););
                    MESSAGE(8,printf("\t\t               %d \n", attr3Out[ i ] ););
                 }
         }


  
/*
           *********************************************************************************
           *                             Testing   HE5_SWinqdatatype                       *
           *********************************************************************************
        */
           MESSAGE(4,printf("\tTesting HE5_SWinqdatatype... \n"););
           MESSAGE(4,printf("\t======================== \n"););
           MESSAGE(11,fprintf(fp,"\t\n"););
           MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqdatatype... \n"););
           MESSAGE(11,fprintf(fp,"\t======================== \n"););
         
           MESSAGE(6,printf("\n"););
           MESSAGE(6,printf("\t\tRetrieving information about field data in a grid\n"););
           MESSAGE(11,fprintf(fp,"\t\n"););
           MESSAGE(11,fprintf(fp,"\t\t Retrieving information about field data in a grid\n"););
           
           fieldgroup = HE5_HDFE_ATTRGROUP;
           status = HE5_SWinqdatatype(SWid_simple, NULL, "Drift", fieldgroup, &datatype, &classid, &order, &Size);
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
       /*  MESSAGE(4,printf("\tdatatype:    %d \n", (int)datatype););
           MESSAGE(4,printf("\tclass ID:    %d \n", (int)classid););
           MESSAGE(4,printf("\torder:       %d \n", (int)order););
           MESSAGE(4,printf("\tsize:        %d \n", (int)Size);); */
           }

           
           fieldgroup = HE5_HDFE_GRPATTRGROUP;
           status = HE5_SWinqdatatype(SWid_simple, NULL, "GroupAttribute", fieldgroup, &datatype, &classid, &order, &Size);
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
         /*  MESSAGE(4,printf("\tdatatype:    %d \n", (int)datatype););
             MESSAGE(4,printf("\tclass ID:    %d \n", (int)classid););
             MESSAGE(4,printf("\torder:       %d \n", (int)order););
             MESSAGE(4,printf("\tsize:        %d \n", (int)Size);); */
             }

          fieldgroup = HE5_HDFE_LOCATTRGROUP;
          status = HE5_SWinqdatatype(SWid_simple, "Count", "LocalAttribute", fieldgroup, &datatype, &classid, &order, &Size);
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
       /*    MESSAGE(4,printf("\tdatatype:    %d \n", (int)datatype););
             MESSAGE(4,printf("\tclass ID:    %d \n", (int)classid););
             MESSAGE(4,printf("\torder:       %d \n", (int)order););
             MESSAGE(4,printf("\tsize:        %d \n", (int)Size);); */
             }

           
 

   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWdiminfo...                        *
	*********************************************************************************
	*/
   
   
           MESSAGE(4,printf("\n"););
           MESSAGE(4,printf("\tTesting HE5_SWdiminfo... \n"););
           MESSAGE(4,printf("\t======================== \n"););
   
           MESSAGE(11,fprintf(fp,"\t\n"););
           MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdiminfo... \n"););
           MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
           MESSAGE(6,printf("\n"););
           MESSAGE(6,printf("\t\tRetrieve information about DataXtrack dimension in swath SIMPLE \n"););
           MESSAGE(11,fprintf(fp,"\n"););
           MESSAGE(11,fprintf(fp,"\t\tRetrieve information about DataXtrack dimension in swath SIMPLE \n"););
   
   
          ndims = HE5_SWdiminfo(SWid_simple,"DataXtrack");   
          if ( ndims == 0)
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
            MESSAGE(10,printf("\t\tndims=HE5_SWdiminfo(SWid_simple,\"DataXtrack\");\n"););
            MESSAGE(8,printf("\t\tSize of dimension DataXtrack:    %lu \n",(unsigned long)ndims););
            MESSAGE(11,fprintf(fp,"\t\tndims=HE5_SWdiminfo(SWid_simple,\"DataXtrack\");\n"););
            MESSAGE(11,fprintf(fp,"\t\tSize of dimension DataXtrack:      %lu \n",(unsigned long)ndims););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWreadfield                        *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWreadfield... \n"););
   MESSAGE(4,printf("\t========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWreadfield... \n"););
   MESSAGE(11,fprintf(fp,"\t========================== \n"););
   
   inq_start[0]  = 0;
   inq_start[1]  = 0;
   inq_stride[0] = 1;
   inq_stride[1] = 1;
   inq_edge[0]   = 10;
   inq_edge[1]   = 4;
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading data from field \"Temperature\" in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading data from field \"Temperature\" in swath SIMPLE\n"););
   
   number = HE5_SWreadfield(SWid_simple,"Temperature",inq_start,inq_stride,inq_edge,TempDataOut);   
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWreadfield(SWid_simple,\"Temperature\",inq_start,inq_stride,inq_edge,TempDataOut);\n"););
       MESSAGE(8,printf("\t\tstatus return from call to SWreadfield %li\n", number ););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWreadfield(SWid_simple,\"Temperature\",inq_start,inq_stride,inq_edge,TempDataOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus return from call to SWreadfield %li\n", number ););
       MESSAGE(8,printf("\t\tThe values of data field \"Temperature\":\n"););
       MESSAGE(11,fprintf(fp,"\t\tThe values of data field \"Temperature\":\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(8,printf("\n"););
       for (i = 0; i < 10; i++)
		 {
		   MESSAGE(8,printf("\t\t    %lf  %lf  %lf  %lf \n",TempDataOut[i][0],TempDataOut[i][1],TempDataOut[i][2],TempDataOut[i][3] ););
		   MESSAGE(11,fprintf(fp,"\t\t    %lf  %lf  %lf  %lf \n",TempDataOut[i][0],TempDataOut[i][1],TempDataOut[i][2],TempDataOut[i][3] ););
		 }
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWfieldinfo                        *
	*********************************************************************************
	*/
      
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWfieldinfo... \n"););
   MESSAGE(4,printf("\t========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););   
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWfieldinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t========================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about field \"Temperature\" in swath SIMPLE e\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about field \"Temperature\" in swath SIMPLE \n"););
   
   status = HE5_SWfieldinfo(SWid_simple,"Temperature",&rank,dims,&dtype1,DimList,NULL);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWfieldinfo(SWid_simple,\"Temperature\",&rank,dims,&dtype1,DimList,NULL);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWfieldinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWfieldinfo(SWid_simple,\"Temperature\",&rank,dims,&dtype1,DimList,NULL);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWfieldinfo %d\n",status););
       MESSAGE(8,printf("\t\tRank:   %d      Data type:     %ld\n",rank, (long) dtype1););
       MESSAGE(8,printf("\t\tDimension list:           %s\n",DimList););
       MESSAGE(11,fprintf(fp,"\t\tRank:    %d      Data type:    %ld\n",rank, (long) dtype1););
       MESSAGE(11,fprintf(fp,"\t\tDimension list:          %s\n",DimList););
       MESSAGE(8,printf("\t\tDimension sizes:\n"););
       MESSAGE(11,fprintf(fp,"\t\tDimension sizes:\n"););
       for (i = 0; i < rank; i++)
		 {
		   MESSAGE(8,printf("\t\t  %lu\n",(unsigned long)dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t  %lu\n",(unsigned long)dims[i]););
		 }
       
	 }
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWinqmaps                          *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqmaps... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqmaps... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tRetrieve information about dimension mapping from swath SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about dimension mapping from swath SIMPLE\n"););
   
   number = HE5_SWnentries(SWid_simple,1,&strbufsize);   
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_simple,1,&strbufsize);\n"););       
       MESSAGE(8,printf("\t\tNumber of dimension mappings %li\n",number););
       MESSAGE(8,printf("\t\tSize of dimension mapping entries: %li (bytes)\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_simple,1,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of dimension mappings %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tSize of dimension mapping entries: %li (bytes)\n",strbufsize););
	 }
   
   OffsetPtr       = (long *)calloc(number, sizeof(long));   
   IncremPtr       = (long *)calloc(number, sizeof(long));
   DimMapListPtr   = (char *)calloc( BUFSIZE, sizeof(char));
   
   
   MESSAGE(6,printf("\t\tRetrieve the geolocation relations for swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the geolocation relations for swath SIMPLE \n"););
   
   number=HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);\n"););
       MESSAGE(8,printf("\t\tNumber of mappings in swath SIMPLE     %li\n",number););
       MESSAGE(6,printf("\t\tGeolocation relations: \n"););
       MESSAGE(6,printf("\t\t  %s\n",DimMapListPtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqmaps(SWid_simple,DimMapListPtr,OffsetPtr,IncremPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of mappings in swath SIMPLE      %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tGeolocation relations: \n"););
       MESSAGE(11,fprintf(fp,"\t\t  %s\n",DimMapListPtr););
       
       MESSAGE(6,printf("\t\t  Offset     Increment \n"););
       MESSAGE(11,fprintf(fp,"\t\t  Offset     Increment \n"););
       for (i = 0; i < number; i++)
		 {
		   MESSAGE(6,printf("\t\t      %li          %li \n",OffsetPtr[i], IncremPtr[i]););
		   MESSAGE(11,fprintf(fp,"\t\t      %li          %li \n",OffsetPtr[i], IncremPtr[i]););
		 }


	 }
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWmapinfo                          *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWmapinfo... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWmapinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n");); 
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve offset and increment for geolocation mapping\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve offset and increment for geolocation mapping\n"););
   
   status = HE5_SWmapinfo(SWid_simple,"GeoTrack","DataTrack",OffsetPtr,IncremPtr);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWmapinfo(SWid_simple,\"GeoTrack\",\"DataTrack\",OffsetPtr,IncremPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWmapinfo %d\n",status););
       MESSAGE(8,printf("\t\tOffset %li, Increment %li\n",*OffsetPtr,*IncremPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWmapinfo(SWid_simple,\"GeoTrack\",\"DataTrack\",OffsetPtr,IncremPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWmapinfo %d\n",status););   
       MESSAGE(11,fprintf(fp,"\t\tOffset %li, Increment %li\n",*OffsetPtr,*IncremPtr););       
	 }
   
   free(OffsetPtr);
   free(IncremPtr);
   free(DimMapListPtr);
   OffsetPtr = NULL;
   IncremPtr = NULL;
   DimMapListPtr = NULL;
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWgeomapinfo                       *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWgeomapinfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWgeomapinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n");); 
   
   
   /* Swath SIMPLE */
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving type of dimension mapping for dimension \"GeoTrack\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving type of dimension mapping for dimension \"GeoTrack\"\n"););
   MESSAGE(8,printf("\n"););
   MESSAGE(8,printf("\t\t(NOTE: 2 - indexed mapping, 1 - regular mapping)\n"););
   MESSAGE(11,fprintf(fp,"\n");); 
   MESSAGE(11,fprintf(fp,"\t\t(NOTE: 2 - indexed mapping, 1 - regular mapping)\n");); 
   
   
   status = HE5_SWgeomapinfo(SWid_simple,"GeoTrack");   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWgeomapinfo(SWid_simple,\"GeoTrack\",\"DataTrack\");\n"););
       MESSAGE(8,printf("\t\tValue returned by SWgeomapinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWgeomapinfo(SWid_simple,\"GeoTrack\",\"DataTrack\");\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWgeomapinfo %d\n",status););
	 }
   
   
   /* Swath INDEX */
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving type of dimension mapping for dimension \"GeoDim\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving type of dimension mapping for dimension \"GeoDim\"\n"););
   status = HE5_SWgeomapinfo(SWid_index,"GeoDim");   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWgeomapinfo(SWid_index,\"GeoDim\");\n"););
       MESSAGE(8,printf("\t\tValue returned by SWgeomapinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWmapinfo(SWid_index,\"GeoDim\");\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWgeomapinfo %d\n",status);); 
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWgetfillvalue                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););      
   MESSAGE(4,printf("\tTesting HE5_SWgetfillvalue... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWgetfillvalue... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tGet fill value for the field \"Indexed_Data\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tGet fill value for the field \"Indexed_Data\" \n"););
   status = HE5_SWgetfillvalue(SWid_index,"Indexed_Data",&InqFloatFillValue);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWgetfillvalue(SWid_index,\"Indexed_Data\",&InqFloatFillValue);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWgetfillvalue %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWgetfillvalue(SWid_index,\"Indexed_Data\", &InqFloatFillValue)\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWgetfillvalue %d\n",status););
       MESSAGE(8,printf("\t\tFill value for field \"Indexed_Data\":\n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(8,printf("\t\t     \"%f\" \n", InqFloatFillValue););
       MESSAGE(11,fprintf(fp,"\t\tFill value for field \"Indexed_Data\":\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t      \"%f\" \n",InqFloatFillValue););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdetach                           *
	*********************************************************************************
	*/

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from swath INDEX \n"););

   status = HE5_SWdetach(SWid_index);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdetach(SWid_index);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWdetach for swath INDEX %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdetach(SWid_index);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWdetach for swath INDEX%d\n",status););
	 }
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWinqattrs                         *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqattrs... \n"););
   MESSAGE(4,printf("\t========================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqattrs... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about attributes defined in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attributes defined in swath INDEX \n"););
   
   number = HE5_SWinqattrs(SWid_simple, NULL, &strbufsize);   
   if (number == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););   
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););          
       errcnt++;
	 }
   else
	 {   
       MESSAGE(4,printf("\n"););   
       MESSAGE(4,printf("\t\t********Passed Testst**********\n"););
  /*   MESSAGE(4,printf("\t\tNumber of attributes:             %li \n", number);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of attributes) returned by SWinqattrs %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of attributes) returned by SWinqattrs %li\n",number););
       MESSAGE(8,printf("\t\tString length of attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of attribute list:    %li\n",strbufsize););
	 }
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve more information about attributes defined in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about attributes defined in swath INDEX \n"););
   
   ObjNamesPtr     = (char  *)calloc( BUFSIZE, sizeof(char));
   
   number = HE5_SWinqattrs(SWid_simple, ObjNamesPtr, &strbufsize);   
   if (number == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
	 }
   else
	 {   
       MESSAGE(4,printf("\n"););   
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
   /*  MESSAGE(4,printf("\t\t Attribute list:    %s \n", ObjNamesPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqattrs(SWid_simple, ObjNamesPtr, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqattrs(SWid_simple, ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of attributes returned by SWinqattr    %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by SWinqattr    %li\n",number););
       MESSAGE(8,printf("\t\tAttribute list:\n"););
       MESSAGE(8,printf("\t\t    %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tAttribute list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t    %s\n", ObjNamesPtr););
       MESSAGE(8,printf("\t\tString length of attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of attribute list:      %li\n",strbufsize););
	 }
   
   free(ObjNamesPtr);
   ObjNamesPtr = NULL;

     /* 
        *********************************************************************************
        *                             Testing    HE5_SWinqgrpattrs                      *
        *********************************************************************************
        */
 
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqgrpattrs... \n"););
   MESSAGE(4,printf("\t========================= \n"););
 
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqgrpattrs... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););
 
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about group attributes defined in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about group attributes defined in swath INDEX \n");)
;
 
   number = HE5_SWinqgrpattrs(SWid_simple, NULL, &strbufsize);
   if (number == FAIL)
         {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
         }
   else
{
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Testst**********\n"););
  /*   MESSAGE(4,printf("\t\tNumber of group attributes:             %li \n", number);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqgrpattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqgrpattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of group attributes) returned by SWingrpqattrs %li\n",number);)
;
       MESSAGE(11,fprintf(fp,"\t\tValue (number of group attributes) returned by SWinqgrpattrs %li\n",number);)
       MESSAGE(8,printf("\t\tString length of group attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of group attribute list:    %li\n",strbufsize););
         }


   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve more information about group attributes defined in swath INDEX \n");)
;
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about group attributes defined in swath INDEX \
n"););

   ObjNamesPtr     = (char  *)calloc( BUFSIZE, sizeof(char));
number = HE5_SWinqgrpattrs(SWid_simple, ObjNamesPtr, &strbufsize);
   if (number == FAIL)
         {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
         }
   else
         {
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
/*     MESSAGE(4,printf("\t\t Group Attribute list:    %s \n", ObjNamesPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqgrpattrs(SWid_simple, ObjNamesPtr, &strbufsize);\n");)
;
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqgrpattrs(SWid_simple, ObjNamesPtr,&strbufsize);\n"
););
       MESSAGE(8,printf("\t\tNumber of attributes returned by SWinqgrpattr    %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by SWinqgrpattr    %li\n",number););
       MESSAGE(8,printf("\t\tGroup Attribute list:\n"););
       MESSAGE(8,printf("\t\t    %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tGroup Attribute list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t    %s\n", ObjNamesPtr););
       MESSAGE(8,printf("\t\tString length of Group attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of Group attribute list:      %li\n",strbufsize););
         }

   free(ObjNamesPtr);
   ObjNamesPtr = NULL;


         /*
        *********************************************************************************
        *                             Testing    HE5_SWinqgeogrpattrs                   *
        *********************************************************************************
        */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqgeogrpattrs... \n"););
   MESSAGE(4,printf("\t========================= \n"););

   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqgeogrpattrs... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about group attributes defined in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about group attributes defined in swath INDEX \n"););

   number = HE5_SWinqgeogrpattrs(SWid_simple, NULL, &strbufsize);
   if (number == FAIL)
         {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
         }
   else
{
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Testst**********\n"););
   /*  MESSAGE(4,printf("\t\tNumber of Geolocation fields group attributes:             %li \n", number);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqgeogrpattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqgeogrpattrs(SWid_simple, NULL, &strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of group attributes) returned by SWingeogrpqattrs %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of group attributes) returned by SWinqgeogrpattrs %li\n",number););
       MESSAGE(8,printf("\t\tString length of group attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of group attribute list:    %li\n",strbufsize););
         }


      MESSAGE(6,printf("\n"););
      MESSAGE(6,printf("\t\tRetrieve more information about group attributes defined in swath INDEX \n"););
      MESSAGE(11,fprintf(fp,"\n"););
      MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about group attributes defined in swath INDEX \n"););

      ObjNamesPtr     = (char  *)calloc( BUFSIZE, sizeof(char));
      number = HE5_SWinqgeogrpattrs(SWid_simple, ObjNamesPtr, &strbufsize);
      if (number == FAIL)
         {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
         }
      else
         {
       MESSAGE(4,printf("\n"););
       MESSAGE(4,printf("\t\t********Passed Test**********\n"););
  /*   MESSAGE(4,printf("\t\t Geolocation field group Attribute list:    %s \n", ObjNamesPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqgeogrpattrs(SWid_simple, ObjNamesPtr, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqgeogrpattrs(SWid_simple, ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of attributes returned by SWinqgeogrpattr    %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by SWinqgeogrpattr    %li\n",number););
       MESSAGE(8,printf("\t\tGroup Attribute list:\n"););
       MESSAGE(8,printf("\t\t    %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tGroup Attribute list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t    %s\n", ObjNamesPtr););
       MESSAGE(8,printf("\t\tString length of Group attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of Group attribute list:      %li\n",strbufsize););
         }

       free(ObjNamesPtr);
       ObjNamesPtr = NULL;


   /*
	*********************************************************************************
	*                             Testing    HE5_SWattrinfo                         *
	*********************************************************************************
	*/
   
      MESSAGE(4,printf("\n"););
      MESSAGE(4,printf("\tTesting HE5_SWattrinfo... \n"););
      MESSAGE(4,printf("\t=========================\n"););
   
      MESSAGE(11,fprintf(fp,"\n"););
      MESSAGE(11,fprintf(fp,"\tTesting HE5_SWattrinfo... \n"););
      MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
      CountPtr        = (hsize_t *)calloc(1, sizeof(hsize_t));
   
      MESSAGE(6,printf("\n"););
      MESSAGE(6,printf("\t\tRetrieve information about attribute \"Drift\" \n"););
      MESSAGE(11,fprintf(fp,"\n"););
      MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attribute \"Drift\" \n"););
   
      status=HE5_SWattrinfo(SWid_simple,"Drift",&dtype,CountPtr);   
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
   /*  MESSAGE(4,printf("\t\tData type:          %d\n", dtype););
       MESSAGE(4,printf("\t\t Number of elements:  %d \n", (int)*CountPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus=HE5_SWattrinfo(SWid_simple,\"Drift\",&dtype,CountPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWattrinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type   Number of attribute elements:\n"););
       MESSAGE(6,printf("\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWattrinfo(SWid_simple,\"Drift\",&dtype,CountPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tStaus returned by SWattrinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tData type   Number of attribute elements:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
	 }
   
   
   free(CountPtr);
      
   /*
	*********************************************************************************
	*                             Testing    HE5_SWcompinfo                         *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWcompinfo... \n"););
   MESSAGE(4,printf("\t========================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWcompinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tRetrieve Compression information for field \"Temperature\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve Compression information for field \"Temperature\"\n"););
   
   status = HE5_SWcompinfo(SWid_simple,"Temperature",&code,level);   
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
  /*   MESSAGE(4,printf("\t\t Compression scheme        Compression level\n"););
       MESSAGE(4,printf("\t\t %d,                      %d\n", code, level[0]);); */

       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(8,printf("\t\tCompression scheme       Compression level\n"););
       MESSAGE(8,printf("\t\t    %d,                      %d\n", code, level[0]););
       MESSAGE(11,fprintf(fp,"\t\tCompression scheme       Compression level\n"););
       MESSAGE(11,fprintf(fp,"\t\t    %d,                      %d\n", code, level[0]););
	 }
   
   
   code       = - 8;
   level[ 0 ] = - 8;
   
   MESSAGE(4,printf("\n"););   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(6,printf("\t\tRetrieve Compression information for field \"Conduction\"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve Compression information for field \"Conduction\"\n"););
   
   status = HE5_SWcompinfo(SWid_simple,"Conduction",&code,level);
   
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
       MESSAGE(8,printf("\t\tCompression scheme       Compression level\n"););
       MESSAGE(8,printf("\t\t    %d,                      %d\n", code, level[0]););
       MESSAGE(11,fprintf(fp,"\t\tCompression scheme       Compression level\n"););
       MESSAGE(11,fprintf(fp,"\t\t    %d,                      %d\n", code, level[0]););
	 }
/*
        *********************************************************************************
        *                           Testing   HE5_SWchunkinfo                           *
        *********************************************************************************
        */
        
        MESSAGE(4,printf("\n"););
        MESSAGE(4,printf("\tTesting HE5_SWchunkinfo.... \n"););
        MESSAGE(4,printf("\t============================= \n"););
        MESSAGE(11,fprintf(fp,"\t\n"););
        MESSAGE(11,fprintf(fp,"\tTesting HE5_SWchunkinfo... \n"););
        MESSAGE(11,fprintf(fp,"\t========================= \n"););
        MESSAGE(6,printf("\n"););
        MESSAGE(6,printf("\t\tRetrieve Chunking information for field \"Count\"\n"););
        MESSAGE(11,fprintf(fp,"\n"););
        MESSAGE(11,fprintf(fp,"\t\tRetrieve Compression information for field \"Count\"\n"););
 
        status = HE5_SWchunkinfo(SWid_simple,"Count",&chunk_rank,chunk_dim);
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
     /* MESSAGE(4,printf("\t\t Count chunk rank: %d\n", chunk_rank);); */

        MESSAGE(11,fprintf(fp,"\n"););
        MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
        MESSAGE(8,printf("\t\t Count chunk rank: %d\n", chunk_rank););
        MESSAGE(11,fprintf(fp,"\t\t Count chunk rank: %d\n", chunk_rank ););

        } 

        /*
        *********************************************************************************
        *                             Testing    HE5_SWgrpattrinfo                         *
        *********************************************************************************
        */
  
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWgrpattrinfo... \n"););
   MESSAGE(4,printf("\t=========================\n"););
  
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWgrpattrinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
  
   CountPtr        = (hsize_t *)calloc(1, sizeof(hsize_t));
  
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about attribute \"GroupAttribute\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attribute \"GroupAttribute\" \n"););
  
   status=HE5_SWgrpattrinfo(SWid_simple,"GroupAttribute",&dtype,CountPtr);
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
  /*   MESSAGE(4,printf("\t\tData type:          %d\n", dtype););
       MESSAGE(4,printf("\t\t Number of elements: %d \n", (int)*CountPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus=HE5_SWgrpattrinfo(SWid_simple,\"GroupAttribute\",&dtype,CountPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWgrpattrinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type   Number of attribute elements:\n"););
       MESSAGE(6,printf("\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWgrpattrinfo(SWid_simple,\"GroupAttribute\",&dtype,CountPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tStaus returned by SWgrpattrinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tData type   Number of attribute elements:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
         }
  

   free(CountPtr);
    

    /*
        ********************************************************************************
        *                             Testing    HE5_SWgeogrpattrinfo                  *
        ********************************************************************************
        */
 
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWgeogrpattrinfo... \n"););
   MESSAGE(4,printf("\t=========================\n"););
 
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWgeogrpattrinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
 
   CountPtr        = (hsize_t *)calloc(1, sizeof(hsize_t));
 
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about attribute \"ScalarFloat\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attribute \"ScalarFloat\" \n"););
 
   status=HE5_SWgeogrpattrinfo(SWid_simple,"ScalarFloat",&dtype,CountPtr);
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
  /*   MESSAGE(4,printf("\t\tData type:          %d\n", dtype););
       MESSAGE(4,printf("\t\t Number of elements: %d \n", (int)*CountPtr);); */
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus=HE5_SWgeogrpattrinfo(SWid_simple,\"ScalarFloat\",&dtype,CountPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWgeogrpattrinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type   Number of attribute elements:\n"););
       MESSAGE(6,printf("\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWgeogrpattrinfo(SWid_simple,\"ScalarFloat\",&dtype,CountPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tStaus returned by SWgeogrpattrinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tData type   Number of attribute elements:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
         }
 

   free(CountPtr);

   /*
	*********************************************************************************
	*                             Testing    HE5_SWgetfillvalue                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););    
   MESSAGE(4,printf("\tTesting HE5_SWgetfillvalue... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWgetfillvalue... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tGet fill value for field \"Temperature\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tGet fill value for field \"Temperature\"\n"););
   status = HE5_SWgetfillvalue(SWid_simple,"Temperature",&InqDoubleFillValue);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWgetfillvalue(SWid_simple,\"Temperature\",&InqDoubleFillValue);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWgetfillvalue %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWgetfillvalue(SWid_simple,\"Temperature\", &InqDoubleFillValue)\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWgetfillvalue %d\n",status););
       MESSAGE(8,printf("\t\tFill value for field \"Temperature\":\n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(8,printf("\t\t     \"%lf\" \n", InqDoubleFillValue););
       MESSAGE(11,fprintf(fp,"\t\tFill value for field \"Temperature\":\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t      \"%lf\" \n",InqDoubleFillValue););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdetach                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from swath SIMPLE \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from swath SIMPLE \n"););
   

   status = HE5_SWdetach(SWid_simple);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWdetach()   %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWdetach()    %d\n",status););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWattach                           *
	*********************************************************************************
	*/
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWattach... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWattach... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
  
   SWid_simple = HE5_SWattach(swfidc_simple,"SIMPLE");
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully attached to the swath SIMPLE ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully attached to the swath SIMPLE ****\n"););
	 }
   
   
   /*
	*********************************************************************************
	*                  Testing  Appending data to the field                         *
	*********************************************************************************
	*/
   
   tstart[0] = 30; tedge[0] = 10;
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting extended data to the field Count in swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting extended data to the field Count in swath SIMPLE \n"););
   
   status = HE5_SWwritefield(SWid_simple, "Count", tstart, NULL, tedge, inarray);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWwritefield(SWid_simple, \"Count\",tstart, NULL, tedge, inarray);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWwritefield for field Count in swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWwritefield(SWid_simple,\"Count\", tstart, stride, NULL, inarray);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWwritefield for field Conduction in swath SIMPLE %d\n",status););
	 }
   
  

 
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdeftimeperiod                    *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdeftimeperiod... \n"););
   MESSAGE(4,printf("\t============================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdeftimeperiod... \n"););
   MESSAGE(11,fprintf(fp,"\t============================== \n"););
   
   StartTime = 55000000.;  
   StopTime  = 65000000.;
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining time period for extraction\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining time period for extraction\n"););
   
   PeriodID = HE5_SWdeftimeperiod(SWid_simple,StartTime,StopTime,HE5_HDFE_MIDPOINT);   
   if (PeriodID == FAIL)
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
       MESSAGE(10,printf("\t\tPeriodID=HE5_SWdeftimeperiod(SWid_simple,StartTime,StopTime,HE5_HDFE_MIDPOINT);\n");); 
       MESSAGE(8,printf("\t\tPeriod ID returned by SWdeftimeperiod()    %ld\n", (long) PeriodID););
       MESSAGE(11,fprintf(fp,"\t\tPeriodID=HE5_SWdeftimeperiod(SWid_simple,StartTime,StopTime,HE5_HDFE_MIDPOINT);\n"););     
       MESSAGE(11,fprintf(fp,"\t\tPeriod ID returned by SWdeftimeperiod()     %ld\n", (long) PeriodID););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWperiodinfo                       *
	*********************************************************************************
	*/
   
   RankPtr         = (int *)calloc(1, sizeof(int));
   size            = (size_t *)calloc(1, sizeof(size_t));
      
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWperiodinfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWperiodinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about the subsetted period\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about the subsetted period\n"););
   
   status = HE5_SWperiodinfo(SWid_simple,PeriodID,"Time",&dtype1,RankPtr,dims,size);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWperiodinfo(SWid_simple,PeriodID,\"Time\",&dtype1,RankPtr,dims,size);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWperiodinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWperiodinfo(SWid_simple,PeriodID,\"Time\",&dtype1,RankPtr,dims,size);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWperiodinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type       Rank \n"););
       MESSAGE(6,printf("\t\t   %ld            %d \n", (long) dtype1,*RankPtr););
       MESSAGE(11,fprintf(fp,"\t\tData type     Rank \n"););
       MESSAGE(11,fprintf(fp,"\t\t   %ld          %d \n", (long) dtype1,*RankPtr););
       MESSAGE(6,printf("\t\tSize of subset period:      %lu (bytes)\n",(unsigned long)*size););
       MESSAGE(11,fprintf(fp,"\t\tSize of subset period:     %lu (bytes)\n",(unsigned long)*size););
       MESSAGE(6,printf("\t\tDimensions of subset period:\n"););
       MESSAGE(11,fprintf(fp,"\t\tDimensions of subset period:\n"););

	   for (i = 0; i < *RankPtr; i++)
		 {
		   MESSAGE(6,printf("\t\t     %d\n",(int)dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t    %d\n",(int)dims[i]););
		 }
	 }
      
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWextractperiod                    *
	*********************************************************************************
	*/
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWextractperiod... \n"););
   MESSAGE(4,printf("\t============================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWextractperiod... \n"););
   MESSAGE(11,fprintf(fp,"\t============================== \n"););
   
   
   BufferPtr = ( double *)malloc( *size );
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRead data from the subsetted time period\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRead data from the subsetted time period\n"););
   
   status = HE5_SWextractperiod(SWid_simple,PeriodID,"Time",HE5_HDFE_INTERNAL,BufferPtr);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWextractperiod(SWid_simple,PeriodID,\"Time\",HE5_HDFE_INTERNAL,BufferPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWextractperiod %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWextractperiod(SWid_simple,PeriodID,\"Time\",HE5_HDFE_INTERNAL,BufferPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWextractperiod %d\n", status););
       MESSAGE(8,printf("\t\n"););
       MESSAGE(8,printf("\t\tThe extracted values of \"Time\" field are: \n"););
       MESSAGE(11,fprintf(fp,"\t\n"););
       MESSAGE(11,fprintf(fp,"\t\tThe extracted values of \"Time\" field are: \n"););

       for( i = 0; i < ( *size / sizeof(double) ); i++ )
		 { 
           MESSAGE(8,printf("\t\t\t        %lf\n", BufferPtr[i]););
           MESSAGE(11,fprintf(fp,"\t\t\t      %lf\n", BufferPtr[i]););
		 }
	 }
   
   free(RankPtr);
   free(BufferPtr);
   RankPtr   = NULL;
   BufferPtr = NULL;
   free(size);
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWattach                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWattach... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWattach... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
  
   SWid_index = HE5_SWattach(swfidc_simple,"INDEX");
   if (SWid_index == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully attached to the swath INDEX ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully attached to the swath INDEX ****\n"););
	 }
   
   /*
        *********************************************************************************
        *                             Testing    HE5_SWlocattrinfo                      *
        *********************************************************************************
        */
        MESSAGE(4,printf("\n"););
        MESSAGE(4,printf("\tTesting HE5_SWlocattrinfo... \n"););
        MESSAGE(4,printf("\t=========================\n"););

        MESSAGE(11,fprintf(fp,"\n"););
        MESSAGE(11,fprintf(fp,"\tTesting HE5_SWlocattrinfo... \n"););
        MESSAGE(11,fprintf(fp,"\t=========================\n"););

        CountPtr        = (hsize_t *)calloc(1, sizeof(hsize_t));

        MESSAGE(6,printf("\n"););
        MESSAGE(6,printf("\t\tRetrieve information about attribute \"LocalAttribute\" \n"););
        MESSAGE(11,fprintf(fp,"\n"););
        MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attribute \"LocalAttribute\" \n"););

        status=HE5_SWlocattrinfo(SWid_simple,"Count", "LocalAttribute",&dtype,CountPtr);
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWlocattrinfo(SWid_Simple, \"Count\", \"LocalAttribute\", &dtype,CountPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by GDlocattrinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type   Number of attribute elements:\n"););
       MESSAGE(6,printf("\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWlocattrinfo(SWid_simple, \"Count\", \"LocalAttribute\",&dtype,CountPtr);\n"););

       MESSAGE(11,fprintf(fp,"\t\tStatus returned by SWlocattrinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tData type   Number of attribute elements:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %ld            %d\n", (long) dtype,(int)*CountPtr););
         }


   free(CountPtr);


 
   /*
	*********************************************************************************
	*                             Testing   HE5_SWnentries                          *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWnentries... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWnentries... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   /*                 
	  -- R e t r i e v e   t h e   I n d e x   d i m e n s i o n   m a p p i n g s   --
	*/   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about index dimension mapping in swath INDEX\n"););
   
   MESSAGE(11,fprintf(fp,"\n"); );
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about index dimension mapping in swath INDEX\n"); );
   
   number = HE5_SWnentries(SWid_index,2,&strbufsize);
   if ( number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWnentries(SWid_index,2,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWnentries(SWid_index,2,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of index mappings:   %li\n",number););
       MESSAGE(8,printf("\t\tNumber of index mappings:   %li\n",number););
       MESSAGE(8,printf("\t\tString length of mapping list:   %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of mapping list:   %li\n",strbufsize););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWinqidxmaps...                     *
	*********************************************************************************
	*/
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWinqidxmaps... \n"););
   MESSAGE(4,printf("\t===========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqidxmaps... \n"););
   MESSAGE(11,fprintf(fp,"\t===========================\n"););
   
   SizePtr         = (hsize_t *)calloc(number, sizeof(hsize_t));   
   DimMapListPtr   = (char *)calloc( BUFSIZE, sizeof(char));
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve indexed mapping relations for swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve indexed mapping relations for swath INDEX \n"););
   number = HE5_SWinqidxmaps(SWid_index,DimMapListPtr,SizePtr);   
   if (number  == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqidxmaps(SWid_index,DimMapListPtr,SizePtr);\n"););
       MESSAGE(8,printf("\t\tNumber of maps:  %li\n", number););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqidxmaps(SWid_index,DimMapListPtr,SizePtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of maps:   %li\n",number););
       MESSAGE(6,printf("\t\tIndexed mapping list: \n"););
       MESSAGE(6,printf("\t\t  %s\n",DimMapListPtr););
       MESSAGE(11,fprintf(fp,"\t\tIndexed mapping list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t   %s\n",DimMapListPtr););
	 }
   
   free(SizePtr);
   free(DimMapListPtr);

   /*
        *********************************************************************************
        *                             Testing    HE5_SWinqlocattrs                      *
        *********************************************************************************
        */
        MESSAGE(4,printf("\n"););
        MESSAGE(4,printf("\tTesting HE5_SWinqlocattrs... \n"););
        MESSAGE(4,printf("\t========================= \n"););

        MESSAGE(11,fprintf(fp,"\t\n"););
        MESSAGE(11,fprintf(fp,"\tTesting HE5_SWinqlocattrs... \n"););
        MESSAGE(11,fprintf(fp,"\t========================= \n"););

        MESSAGE(6,printf("\n"););
        MESSAGE(6,printf("\t\tRetrieve information about attributes defined in Count field \n"););
        MESSAGE(11,fprintf(fp,"\n"););
        MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attributes defined in Count \n"););

        number = HE5_SWinqlocattrs(SWid_simple, "Count", NULL, &strbufsize);
        if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqlocattrs(SWid_simple, \"Count\", NULL, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqlocattrs(SWid_simple, \"Count\", NULL, &strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of attributes) returned by SWinqlocattrs %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of attributes) returned by SWinqlocattrs %li\n",number););
       MESSAGE(8,printf("\t\tString length of attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of attribute list:    %li\n",strbufsize););
        }

       MESSAGE(6,printf("\n"););
       MESSAGE(6,printf("\t\tRetrieve more information about attributes defined in Count field \n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about attributes defined in Count field \n"););

       ObjNamesPtr     = (char  *)calloc( BUFSIZE, sizeof(char));
       number = HE5_SWinqlocattrs(SWid_simple, "Count", ObjNamesPtr, &strbufsize);
       if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber=HE5_SWinqlocattrs(SWid_simple, Count, ObjNamesPtr, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_SWinqlocattrs(SWid_simple, Count, ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of attributes returned by SWinqlocattrs   %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by SWinqlocattr   %li\n",number););
       MESSAGE(8,printf("\t\tAttribute list:\n"););
       MESSAGE(8,printf("\t\t    %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tAttribute list: \n"););
       MESSAGE(11,fprintf(fp,"\t\t    %s\n", ObjNamesPtr););
       MESSAGE(8,printf("\t\tString length of attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of attribute list:      %li\n",strbufsize););
         }
 
   free(ObjNamesPtr);
   ObjNamesPtr = NULL;

   
   /*
	*********************************************************************************
	*                             Testing   HE5_SWdiminfo...                        *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdiminfo... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdiminfo... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve size of dimension \"GeoDim\" in swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve size of dimension \"GeoDim\" in swath INDEX \n"););
   
   ndims = HE5_SWdiminfo(SWid_index,"GeoDim");   
   if ( ndims == 0)
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
       MESSAGE(10,printf("\t\tndims=HE5_SWdiminfo(SWid_index,\"GeoDim\");\n"););
       MESSAGE(8,printf("\t\tSize of dimension \"GeoDim\":     %lu \n",(unsigned long)ndims););
       MESSAGE(11,fprintf(fp,"\t\tndims=HE5_SWdiminfo(SWid_index,\"GeoDim\");\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of dimension \"GeoDim\":       %lu\n",(unsigned long)ndims););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWidxmapinfo                       *
	*********************************************************************************
	*/


   OffsetPtr = (long  *)calloc(100, sizeof(long));

   MESSAGE(4,printf("\n"););   
   MESSAGE(4,printf("\tTesting HE5_SWidxmapinfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWidxmapinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n");); 
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve indexed array of \"GeoDim/TrackDim\"  mapping\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve indexed array of \"GeoDim/TrackDim\"  mapping\n"););
   nmaps = HE5_SWidxmapinfo(SWid_index,"GeoDim","TrackDim",OffsetPtr);   
   if (nmaps == 0)
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
       MESSAGE(10,printf("\t\tnmaps=HE5_SWidxmapinfo(SWid_index,\"GeoDim\",\"TrackDim\",OffsetPtr);\n"););
       MESSAGE(8,printf("\t\tSize of indexed array:   %lu \n",(unsigned long)nmaps););
       MESSAGE(11,fprintf(fp,"\t\tnmaps=HE5_SWidxmapinfo(SWid_index,\"GeoDim\",\"TrackDim\",OffsetPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of indexed array:   %lu \n",(unsigned long)nmaps););
       MESSAGE(8,printf("\t\tEntry #          Mapping offset\n"););   
       MESSAGE(11,fprintf(fp,"\t\tEntry #          Mapping offset\n"););   
       for ( i = 0; i < nmaps; i++) 
		 {
		   MESSAGE(8,printf("\t\t  %d        %li \n", i+1, OffsetPtr[i]););   
		   MESSAGE(11,fprintf(fp,"\t\t  %d        %li \n", i+1, OffsetPtr[i]););   
		 }       
	 }
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve indexed array of \"GeoDim/XtrackDim\"  mapping\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve indexed array of \"GeoDim/XtrackDim\"  mapping\n"););
   
   nmaps = HE5_SWidxmapinfo(SWid_index,"GeoXDim","XtrackDim",OffsetPtr);   
   if (nmaps == 0)
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
       MESSAGE(10,printf("\t\tnmaps=HE5_SWidxmapinfo(SWid_index,\"GeoXDim\",\"XtrackDim\",OffsetPtr);\n"););
       MESSAGE(8,printf("\t\tSize of indexed array:    %lu \n",(unsigned long)nmaps););
       MESSAGE(11,fprintf(fp,"\t\tnmaps=HE5_SWidxmapinfo(SWid_index,\"GeoXDim\",\"XtrackDim\",OffsetPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of indexed array:   %lu\n",(unsigned long)nmaps););
       MESSAGE(8,printf("\t\tEntry #          Mapping offset\n"););   
       MESSAGE(11,fprintf(fp,"\t\tEntry #          Mapping offset\n"););   
       for ( i = 0; i < nmaps; i++) 
		 {
		   MESSAGE(8,printf("\t\t  %d                     %li \n", i+1, OffsetPtr[i]););   
		   MESSAGE(11,fprintf(fp,"\t\t  %d                     %li \n", i+1, OffsetPtr[i]););   
		 }
	 }
   
  

   /*
        *********************************************************************************
        *                             Testing    HE5_SWindexinfo                       *
        *********************************************************************************
        */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWindexinfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
  
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWindexinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n"););
  
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve indices information about \"Longitude\" field\n"););
  
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve indicesinformation about \"Longitude\"  field\n"););
        dimlist = (char  *)calloc( BUFSIZE, sizeof(char));
         for ( i = 0; i < 8; i++)
                  {
                      indices[i] = (hsize_t *) malloc(2*sizeof(hsize_t));
                  }

        status = HE5_SWindexinfo(RegionID, "Longitude", &rank3, dimlist, indices);
     if (status == FAIL)
         {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
       errcnt++;
         }
else
         { 
       MESSAGE(4,printf("\t\t********Passed Test**********\n");); 
       MESSAGE(4,printf("\n");); 
   /*    MESSAGE(4,printf("rank = %d\n", rank););
       MESSAGE(4,printf("dimlist = %s\n", dimlist););
       for ( i = 0; i < rank; i++)
                  {
                      for ( j = 0; j < 2; j++)
                      {
                        MESSAGE(4,printf("i = %d, j = %d, indices[i][j] = %lu\n", i, j, (unsigned long)indices[i][j]););
                      }
                  } */
        MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Passed Test**********\n"););
       MESSAGE(10,printf("\t\tstatus=HE5_SWindexinfo(RegionID, \"Longitude\", &rank3, dimlist, indices);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWindexinfo  %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWindexinfo(RegionID, \"Longitude\", &rank3, dimlist, indices);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWindexinfo  %d\n",status););
       MESSAGE(8,printf("rank = %d\n", rank););
       MESSAGE(8,printf("dimlist = %s\n", dimlist););
       MESSAGE(11,fprintf(fp,"\t\tRank: %d\n", rank););
       MESSAGE(11,fprintf(fp,"\t\tDimension list:          %s\n",dimlist););
       for ( i = 0; i < rank; i++)
                  {
                      for ( j = 0; j < 2; j++)
                      {
                        MESSAGE(8,printf("i = %d, j = %d, indices[i][j] = %lu\n", i, j, (unsigned long)indices[i][j]););
                        MESSAGE(11,fprintf(fp,"\t\ti = %d, j = %d, indices[i][j] = %lu\n", i, j, (unsigned long)indices[i][j]););
                      }
                 }
        }
        
        free(dimlist);
        dimlist = NULL;
        for (i = 0; i < 8; i++)
                 {
                   free(indices[i]);
                   indices[i] = NULL;
                 } 

 
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdetach                           *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from swath SIMPLE \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from swath SIMPLE \n"););

   status = HE5_SWdetach(SWid_simple);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWdetach for swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWdetach for swath SIMPLE %d\n",status););
	 }
   
   /*
	*********************************************************************************
	*                  Testing REGION SUBSETTING for swath INDEX                    *
	*********************************************************************************
	*/
   
   
   CornerLon[ 0 ] = 79.7e0;
   CornerLat[ 0 ] = 59.5e0;
   CornerLon[ 1 ] = 80.e0;
   CornerLat[ 1 ] = 60.e0;
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefboxregion... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefboxregion... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tDefining region to extract\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining region to extract\n"););
   
   
   RegionID = HE5_SWdefboxregion(SWid_index,CornerLon,CornerLat,HE5_HDFE_MIDPOINT);
   if (RegionID == FAIL)
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
	   MESSAGE(10,printf("\t\tRegionID=HE5_SWdefboxregion(SWid_index,CornerLon,CornerLat,mode);\n"););
	   MESSAGE(8,printf("\t\tRegion ID returned by SWdefboxregion %ld\n", (long) RegionID););
	   MESSAGE(11,fprintf(fp,"\t\tRegionID=HE5_SWdefboxregion(SWid_index,CornerLon,CornerLat,mode);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tRegion ID returned by SWdefboxregion %ld\n", (long) RegionID););
	 }
   
   
   /*
	*********************************************************************************
	*               Testing  HE5_SWregioninfo  for swath  INDEX                     *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWregioninfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWregioninfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n"););
   
   size     = (size_t *)calloc(1, sizeof(size_t));
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving information about defined region\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving information about defined region\n"););
   status = HE5_SWregioninfo(SWid_index,RegionID,"Indexed_Data",&dtype1,&rank2,dims,size);
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
	   MESSAGE(10,printf("\t\tstatus=HE5_SWregioninfo(SWid_index,RegionID,\"Indexed_Data\",&dtype1,RankPtr,dims,size);\n"););
	   MESSAGE(8,printf("\t\tstatus returned by SWregioninfo %d\n",status););
	   MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWregioninfo(SWid_index,RegionID,\"Indexed_Data\",&dtype1,RankPtr,dims,size);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWregioninfo %d\n",status););
	   MESSAGE(6,printf("\t\tData type:     Rank:\n"););
	   MESSAGE(6,printf("\t\t    %ld          %d\n", (long) dtype1, rank2););
	   MESSAGE(11,fprintf(fp,"\t\tData type:     Rank:\n"););
	   MESSAGE(11,fprintf(fp,"\t\t    %ld          %d\n", (long) dtype1, rank2););
	   MESSAGE(6,printf("\t\tSize of region:   %lu (bytes)\n",(unsigned long)*size););
	   MESSAGE(11,fprintf(fp,"\t\tSize of region:     %lu (bytes)\n",(unsigned long)*size););
	   MESSAGE(6,printf("\t\tDimensions of subset region:\n"););
	   MESSAGE(11,fprintf(fp,"\t\tDimensions of subset region:\n"););
	   for (i = 0; i < rank2; i++)
		 {
		   MESSAGE(6,printf("\t\t      %d\n",(int)dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t      %d\n",(int)dims[i]););
		 }
      
	 }
   
   /*
	*********************************************************************************
	*                    Testing  HE5_SWextractregion  for  swath INDEX             *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWextractregion... \n"););
   MESSAGE(4,printf("\t============================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWextractregion... \n"););
   MESSAGE(11,fprintf(fp,"\t============================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tExtracting data from subset region\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tExtracting data from subset region\n"););
 
   BufferFloatPtr = (float *)malloc( *size );
   
   status = HE5_SWextractregion(SWid_index,RegionID,"Indexed_Data",HE5_HDFE_INTERNAL,BufferFloatPtr);
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
	   MESSAGE(10,printf("\t\tstatus=HE5_SWextractregion(SWid_index,RegionID,\"Indexed_Data\",HE5_HDFE_INTERNAL,BufferFloatPtr);\n"););
	   MESSAGE(8,printf("\t\tstatus returned by SWextractregion %d\n",status););
	   MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWextractregion(SWid_simple,OldRegionID,\"Indexed_Data\",HE5_HDFE_INTERNAL,BufferFloatPtr);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWextractregion %d\n",status););

	   MESSAGE(10,printf("\t\t Extracted first 5 values:\n"););
	   MESSAGE(11,fprintf(fp,"\t\t Extracted first 5 values:\n"););
   
	   for (i = 0; i < 5; i++) 
		 {
		   MESSAGE(10,printf("\t\t\t          %f\n", BufferFloatPtr[i]););
		   MESSAGE(11,fprintf(fp,"\t\t\t         %f\n", BufferFloatPtr[i]););
		 }

	 }   
   
   free(BufferFloatPtr);
   free(size);
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWupdateidxmap                     *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););  
   MESSAGE(4,printf("\tTesting HE5_SWupdateidxmap... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWupdateidxmap... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n");); 
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving indexed array for a specified region\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving indexed array for a specified region\n"););
   
   IdxMapSize = HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,NULL,Indices);
   if (IdxMapSize == FAIL)
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
       MESSAGE(10,printf("\t\tIdxMapSize=HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,NULL,Indices);\n"););
       MESSAGE(8,printf("\t\tSize of updated indexed array:     %li\n",IdxMapSize););
       MESSAGE(11,fprintf(fp,"\t\tIdxMapSize=HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,NULL,Indices);\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of updated indexed array:      %li\n",IdxMapSize););
       
	 }
   
   UpdateIdx_1 = (long *)calloc(IdxMapSize, sizeof(long));
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tContinue ...\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tContinue ...\n"););

   IdxMapSize = HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,UpdateIdx_1,Indices);   
   if (IdxMapSize == FAIL)
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
       MESSAGE(10,printf("\t\tIdxMapSize=HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,UpdateIdx_1,Indices);\n"););
       MESSAGE(8,printf("\t\tSize of updated indexed array:     %li\n",IdxMapSize););
       MESSAGE(11,fprintf(fp,"\t\tIdxMapSize=HE5_SWupdateidxmap(SWid_index,RegionID,OffsetPtr,UpdateIdx_1,Indices);\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of updated indexed array:     %li\n",IdxMapSize););
       MESSAGE(8,printf("\t\tArray of indices of the data dimension to which each geolocation corresponds:\n"););
       MESSAGE(11,fprintf(fp,"\t\tArray of indices of the data dimension to which each geolocation corresponds:\n"););
	   for (i = 0; i < IdxMapSize; i++)
		 {
           MESSAGE(8,printf("\t\t    %li\n", OffsetPtr[i]););
           MESSAGE(11,fprintf(fp,"\t\t    %li\n", OffsetPtr[i]););
		 }

       MESSAGE(8,printf("\t\t Array of indices of the data dimension to which each geolocation corresponds: \n"););
       MESSAGE(11,fprintf(fp,"\t\t Array of indices of the data dimension to which each geolocation corresponds: \n"););
	   for (i = 0; i < IdxMapSize; i++)
		 {
           MESSAGE(8,printf("\t\t    %li \n", UpdateIdx_1[i]););
           MESSAGE(11,fprintf(fp,"\t\t    %li \n", UpdateIdx_1[i]););
		 }
	 }

   free(OffsetPtr);

   free(UpdateIdx_1);

   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdetach                           *
	*********************************************************************************
	*/

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from swath INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from swath INDEX \n"););

   status = HE5_SWdetach(SWid_index);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdetach(SWid_index);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWdetach for swath INDEX %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdetach(SWid_index);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWdetach for swath INDEX %d\n",status););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWattach                           *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWattach... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWattach... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   SWid_simple = HE5_SWattach(swfidc_simple,"SIMPLE");
   if (SWid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););  
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully attached to the swath SIMPLE ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully attached to the swath SIMPLE ****\n"););
       
	 }
   
   
   /*
	*********************************************************************************
	*                  Testing REGION SUBSETTING for swath SIMPLE                   *
	*********************************************************************************
	*/
   
   
   CornerLon[ 0 ] = 100.e0;
   CornerLat[ 0 ] = 20.e0;
   CornerLon[ 1 ] = 106.e0;
   CornerLat[ 1 ] = 24.e0;
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdefboxregion... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdefboxregion... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining region to extract\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining region to extract\n"););
   
   
   OldRegionID = HE5_SWdefboxregion(SWid_simple,CornerLon,CornerLat,HE5_HDFE_MIDPOINT);
   if (OldRegionID == FAIL)
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
	   MESSAGE(10,printf("\t\tOldRegionID=HE5_SWdefboxregion(SWid_simple,CornerLon,CornerLat,mode);\n"););
	   MESSAGE(8,printf("\t\tRegion ID returned by SWdefboxregion() %ld\n", (long) OldRegionID););
	   MESSAGE(11,fprintf(fp,"\t\tOldRegionID=HE5_SWdefboxregion(SWid_simple,CornerLon,CornerLat,mode);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tRegion ID returned by SWdefboxregion() %ld\n", (long) OldRegionID););
	 }
   
   
   /*
	*********************************************************************************
	*                Testing  HE5_SWregioninfo  for  swath  SIMPLE                  *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWregioninfo... \n"););
   MESSAGE(4,printf("\t=========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWregioninfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================== \n"););
   
   RankPtr   = (int *)calloc(1, sizeof(int));
   size      = (size_t *)calloc(1, sizeof(size_t));
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieving information about defined region\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving information about defined region\n"););
   status = HE5_SWregioninfo(SWid_simple,OldRegionID,"Temperature",&dtype1,RankPtr,dims,size);
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
	   MESSAGE(10,printf("\t\tstatus=HE5_SWregioninfo(SWid_simple,OldRegionID,\"Temperature\", &dtype1, RankPtr,dims,size);\n"););
	   MESSAGE(8,printf("\t\tstatus returned by SWregioninfo %d\n",status););
	   MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWregioninfo(SWid_simple,OldRegionID,\"Temperature\", &dtype1,RankPtr,dims,size);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWregioninfo %d\n",status););
	   MESSAGE(6,printf("\t\tData type      Rank\n"););
	   MESSAGE(6,printf("\t\t   %ld           %d \n", (long) dtype1, *RankPtr););
	   MESSAGE(6,printf("\t\tSize of region:   %lu (bytes)\n", (unsigned long)*size););
	   MESSAGE(11,fprintf(fp,"\t\tSize of region:     %lu (bytes)\n", (unsigned long)*size););
	   MESSAGE(6,printf("\t\tDimensions of subset region:\n"););
	   MESSAGE(11,fprintf(fp,"\t\tDimensions of subset region:\n"););
	   for (i = 0; i < *RankPtr; i++)
		 {
		   MESSAGE(6,printf("\t\t      %d\n",(int)dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t      %d\n",(int)dims[i]););
		 }
	 }
   
   
   /*
	*********************************************************************************
	*                    Testing  HE5_SWextractregion  for  swath SIMPLE            *
	*********************************************************************************
	*/
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWextractregion... \n"););
   MESSAGE(4,printf("\t============================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWextractregion... \n"););
   MESSAGE(11,fprintf(fp,"\t============================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tExtracting data from subset region\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tExtracting data from subset region\n"););
   
   BufferPtr = (double *)malloc( *size );
   
   status = HE5_SWextractregion(SWid_simple,OldRegionID,"Temperature",HE5_HDFE_INTERNAL,BufferPtr);
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
	   MESSAGE(10,printf("\t\tstatus=HE5_SWextractregion(SWid_simple,OldRegionID,\"Temperature\",HE5_HDFE_INTERNAL,BufferPtr);\n"););
	   MESSAGE(8,printf("\t\tstatus returned by SWextractregion() %d\n",status););
	   MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWextractregion(SWid_simple,OldRegionID,\"Temperature\",HE5_HDFE_INTERNAL,BufferPtr);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWextractregion() %d\n",status););
	
	   MESSAGE(10,printf("\t\t Extracted values:\n"););
	   MESSAGE(11,fprintf(fp,"\t\t Extracted values:\n"););
   
	   for (i = 0; i < *size/ sizeof(double); i++) 
		 {
		   MESSAGE(10,printf("\t\t\t          %lf\n", BufferPtr[i]););
		   MESSAGE(11,fprintf(fp,"\t\t\t         %lf\n", BufferPtr[i]););
		 }
  
	 }
   free(RankPtr);
   free(BufferPtr);
   free(size);
   
   
   RankPtr   = NULL;
   BufferPtr = NULL;
   
   /*
	*********************************************************************************
	*               Testing HE5_SWdupregion  for swath  SIMPLE                      *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdupregion... \n"););
   MESSAGE(4,printf("\t========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdupregion... \n"););
   MESSAGE(11,fprintf(fp,"\t========================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDuplicate region id %ld\n", (long) OldRegionID););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDuplicate region id %ld\n", (long) OldRegionID););
   RegionID = HE5_SWdupregion(OldRegionID);
   if (RegionID == FAIL)
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
	   MESSAGE(10,printf("\t\tRegionID = HE5_SWdupregion(OldRegionID);\n"););
	   MESSAGE(8,printf("\t\tRegion ID returned from call to SWdupregion %ld\n", (long) RegionID););
	   MESSAGE(11,fprintf(fp,"\t\tRegionID = HE5_SWdupregion(OldRegionID);\n"););
	   MESSAGE(11,fprintf(fp,"\t\tRegion ID returned from call to SWdupregion %ld\n", (long) RegionID););
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWfieldinfo                        *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWfieldinfo... \n"););
   MESSAGE(4,printf("\t========================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););   
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWfieldinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t========================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about field \"Count\" in swath SIMPLE e\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about field \"Count\" in swath SIMPLE \n"););
   
   status = HE5_SWfieldinfo(SWid_simple,"Count",&rank,dims, &dtype1,NULL,DimList);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_SWfieldinfo(SWid_simple,\"Count\",&rank,dims,&dtype1,NULL,DimList);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWfieldinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_SWfieldinfo(SWid_simple,\"Count\",&rank,dims, &dtype1,NULL,DimList);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWfieldinfo %d\n",status););
       MESSAGE(8,printf("\t\tRank:   %d      Data type:     %ld\n",rank, (long) dtype1););
       MESSAGE(8,printf("\t\tMaximum Dimension list:           %s\n",DimList););
       MESSAGE(11,fprintf(fp,"\t\tRank:    %d      Data type:    %ld\n",rank, (long) dtype1););
       MESSAGE(11,fprintf(fp,"\t\tMaximum Dimension list:          %s\n",DimList););
       MESSAGE(8,printf("\t\tDimension sizes:\n"););
       MESSAGE(11,fprintf(fp,"\t\tDimension sizes:\n"););
       for (i = 0; i < rank; i++)
		 {
		   MESSAGE(8,printf("\t\t  %lu\n",(unsigned long)dims[i]););
		   MESSAGE(11,fprintf(fp,"\t\t  %lu\n",(unsigned long)dims[i]););
		 }
       
	 }
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWdetach                           *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from swath SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from swath SIMPLE \n"););

   status = HE5_SWdetach(SWid_simple);
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
       MESSAGE(10,printf("\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(8,printf("\t\tstatus returned by SWdetach for swath SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_SWdetach(SWid_simple);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by SWdetach for swath SIMPLE%d\n",status););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    HE5_SWclose                            *
	*********************************************************************************
	*/
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_SWclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_SWclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file SimpleSwath.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file SimpleSwath.he5\n"););
   
   status = HE5_SWclose(swfidc_simple);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\t*** Successfully clossed the SimpleSwath.he5 file ****\n"););
	   MESSAGE(11,fprintf(fp,"\t\t*** Successfully clossed the SimpleSwath.he5 file ****\n"););
	 }
   
   
   /*
	*********************************************************************************
	*                             Testing    SWopen                                 *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting SWopen... \n"););
   MESSAGE(4,printf("\t=================\n"););
   MESSAGE(11,fprintf(fp,"\tTesting SWopen... \n"););
   MESSAGE(11,fprintf(fp,"\t=================\n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tOpening the file SimpleSwath.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tOpening the file SimpleSwath.he5\n"););
   
   swfidc_simple = HE5_SWopen("SimpleSwath.he5",H5F_ACC_RDWR);
   if (swfidc_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully opened the swath file ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully opened the swath file ****\n"););
       
	 }
   
   
   
   /*
	*********************************************************************************
	*                             Testing    SWinqswath                             *
	*********************************************************************************
	*/
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting SWinqswath... \n"););
   MESSAGE(4,printf("\t=================\n"););
   MESSAGE(11,fprintf(fp,"\tTesting SWinqswath... \n"););
   MESSAGE(11,fprintf(fp,"\t=================\n"););
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tRetrieving general information about objects within the file\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving general information about objects within the file\n"););
   number = HE5_SWinqswath("SimpleSwath.he5",NULL,&strbufsize);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber = HE5_SWinqswath(\"SimpleSwath.he5\",NULL,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue returned by SWinqswath %li\n", number););
       MESSAGE(8,printf("\t\tString length returned by SWinqswath %li\n", strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber = HE5_SWinqswath(\"SimpleSwath.he5\",NULL,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by SWinqswath %li\n", number););
       MESSAGE(11,fprintf(fp,"\t\tStringlength returned by SWinqswath %li\n", strbufsize););
	 }
   
   
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tContinue ...\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tContinue ...\n"););
   ObjNamesPtr = (char *)calloc(strbufsize+1, sizeof(char ) );
   number = HE5_SWinqswath("SimpleSwath.he5",ObjNamesPtr,&strbufsize);
   if (number == FAIL)
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
       MESSAGE(10,printf("\t\tnumber = HE5_SWinqswath(\"SimpleSwath.he5\",ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of swath objects:        %li\n", number););
       MESSAGE(8,printf("\t\tList of swath objects:      %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber = HE5_SWinqswath(\"SimpleSwath.he5\",ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of swath objecys:       %li\n", number););
       MESSAGE(11,fprintf(fp,"\t\tList of swath objects:      %s\n", ObjNamesPtr););

	 }
   
   
   free(ObjNamesPtr);
   
   
   
   
   /*
	*********************************************************************************
	*                             Testing    SWclose                                *
	*********************************************************************************
	*/
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting SWclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting SWclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file SimpleSwath.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file SimpleSwath.he5\n"););
   
   status = HE5_SWclose(swfidc_simple);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\tSuccessfully clossed the SimpleSwath.he5 file \n"););
	   MESSAGE(11,fprintf(fp,"\t\tSuccessfully clossed the SimpleSwath.he5 file \n"););
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