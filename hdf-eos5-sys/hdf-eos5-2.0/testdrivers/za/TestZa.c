#include <HE5_HdfEosDef.h>
#include <tutils.h>

#define BUFSIZE  256
#define RANK1    1
#define RANK2    2

int main(int argc, char *argv[])
{

   FILE        *fp;

   herr_t      status          = FAIL;

   int	       i               = 0; 

   int         Verbosity       = 4;
   int         comp_level[ 5 ] = { 0, 0, 0, 0, 0 };
   int         level[ 5 ]      = { 0, 0, 0, 0, 0 };
   int         rank            = 1;
   int         AttrVal[ 4 ]    = {1,2,3,4};
   int         AttrValOut[ 4 ] = {0,0,0,0};
   int         errcnt          = 0;
   int         comp_code       = 0;
   int         code            = 0;
   int         *RankPtr        = (int *)NULL;
   int         inarray[10]     = {1,2,3,4,5,6,7,8,9,10};
   int         return_val      = FAIL;

   hid_t       ZAid_simple     = FAIL; 
   hid_t       ZAid_index      = FAIL;
   hid_t       zafidc_simple   = FAIL;

   hid_t       dtype           = FAIL;
   hid_t       ntype[20];
   
   long        number          = FAIL;
   long        strbufsize      = FAIL;
   long        *DimListPtr     = (long *)NULL;

   hssize_t    tstart[ 2 ]     = { 0, 0 };

   hsize_t     ndims           = 0;
   hsize_t     Dims[8]         = {0,0,0,0,0,0,0,0};
   hsize_t     dims[ 2 ]       = { 2, 2 };
   hsize_t     chunk_dims[ 2 ] = { 0, 0 };
   hsize_t     tedge[ 2 ]      = { 1, 1 };
   

   hsize_t     stride[ 2 ]     = { 1, 1 };
   hssize_t    inq_start[ 2 ]  = { 0, 0 };
   hsize_t     inq_stride[ 2 ] = { 1, 1 };
   hsize_t     inq_edge[ 2 ]   = { 1, 1 };
   hsize_t     *CountPtr       = (hsize_t *)NULL;
   hsize_t     CurrentSize[1]  = { 4 };
   
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
   
   double      TempDataOut[10][4];
   double      fillvalue2          = -1.11111;
   double      InqDoubleFillValue  = -1.1111;


   char        *FieldListPtr;
   char        *DimNamePtr; 
   char        *ObjNamesPtr; 
   char        DimList[200];


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
   
   
   /* --------------------------------- */
   
   
   MESSAGE(11,fp=fopen("TestZa_c.txtout","w"););
   MESSAGE(4,printf("Testing ZA C interface \n"););
   MESSAGE(4,printf("=========================\n\n"););
   MESSAGE(11,fprintf(fp,"Testing ZA C interface \n"););
   MESSAGE(11,fprintf(fp,"=========================\n\n"););
      
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAopen                             *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\tTesting HE5_ZAopen... \n"););
   MESSAGE(4,printf("\t===================== \n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAopen... \n"););
   MESSAGE(11,fprintf(fp,"\t===================== \n"););
   
   zafidc_simple = HE5_ZAopen("SimpleZa.he5",H5F_ACC_TRUNC);
   if (zafidc_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully opened the za file ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully opened the za file ****\n"););
       
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAcreate                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\tTesting HE5_ZAcreate... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAcreate... \n"););   
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(6,printf("\t\tCreating za SIMPLE \n"););   
   MESSAGE(11,fprintf(fp,"\t\tCreating za SIMPLE \n"););
   
   ZAid_simple = HE5_ZAcreate(zafidc_simple,"SIMPLE"); 
   if (ZAid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully created the za SIMPLE ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the za SIMPLE ****\n"););
       
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAcreate                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\tTesting HE5_ZAcreate... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAcreate... \n"););   
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(6,printf("\t\tCreating za INDEX \n"););   
   MESSAGE(11,fprintf(fp,"\t\tCreating za INDEX \n"););
   
   ZAid_index = HE5_ZAcreate(zafidc_simple,"INDEX"); 
   if (ZAid_index == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;  
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully created the za INDEX ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully created the za INDEX ****\n"););
       
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAdefdim                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdefdim... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdefdim... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(6,printf("\t\tSetting up dimensions for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting up dimensions for za SIMPLE \n"););   
   
   
   /*                 
   ------------------------------------- ZA   SIMPLE  ---------------------------------
	  
	  
	  
   --------------------- D e f i n i n g   d i m e n s i o n   DataTrack  ----------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension DataTrack in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension DataTrack in za SIMPLE \n"););
   
   status = HE5_ZAdefdim(ZAid_simple, "DataTrack", 10);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"DataTrack\", 10);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"DataTrack\", 10);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefdim %d\n", status););
	 }
   
   
   /*                 
   --------------------- D e f i n i n g   d i m e n s i o n   DataXtrack ----------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension DataXtrack in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension DataXtrack in za SIMPLE \n"););
   
   status = HE5_ZAdefdim(ZAid_simple, "DataXtrack", 4);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"DataXtrack\", 4);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"DataXtrack\", 4);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefdim %d\n", status););
	 }
   
   
   /*                 
   --------------------- D e f i n i n g   d i m e n s i o n  "Unlim" ----------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension \"Unlim\" in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension \"Unlim\" in za SIMPLE \n"););
   
   status = HE5_ZAdefdim(ZAid_simple, "Unlim", H5S_UNLIMITED);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"Unlim\", H5S_UNLIMITED);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdefdim(ZAid_simple, \"Unlim\", H5S_UNLIMITED);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefdim %d\n", status););
	 }
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tSetting up dimensions for za INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting up dimensions for za INDEX \n"););   
   
   
   /*
   ---------------------------------- ZA  INDEX  -------------------------------------
	  
	  
	  
   --------------------- D e f i n i n g   d i m e n s i o n   TrackDim  ----------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension TrackDim for za INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension TrackDim for za INDEX \n"););
   
   status = HE5_ZAdefdim(ZAid_index, "TrackDim", 8);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdefdim(ZAid_index, \"TrackDim\", 8);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdefdim(ZAid_index, \"TrackDim\", 8);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefdim %d\n", status););
	 }
   
   
   /*
   --------------------- D e f i n i n g   d i m e n s i o n  XtrackDim  ----------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining dimension XtrackDim for za INDEX \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining dimension TrackDim for za INDEX \n"););
   
   status = HE5_ZAdefdim(ZAid_index, "XtrackDim", 8);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdefdim(ZAid_index, \"XtrackDim\", 8);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefdim %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdefdim(ZAid_index, \"XtrackDim\", 8);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefdim %d\n", status););
	 }
   
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAsetfillvalue                     *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAsetfillvalue... \n"););
   MESSAGE(4,printf("\t=============================\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAsetfillvalue... \n"););
   MESSAGE(11,fprintf(fp,"\t=============================\n"););
   
   
   
   /*                 
   --------- S e t t i n g  F i l l  v a l u e  f o r  T e m p e r a t u r e  --------
   */
   
   fillvalue2 = -777.;
   
   MESSAGE(6,printf("\t\n");); 
   MESSAGE(6,printf("\t\tSetting fill value for field Temperature in za SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tSetting fill value for field Temperature in za SIMPLE\n"););
   status = HE5_ZAsetfillvalue(ZAid_simple, "Temperature", H5T_NATIVE_DOUBLE, &fillvalue2);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAsetfillvalue(ZAid_simple,\"Temperature\", &fillvalue2);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAsetfillvalue for field Temperature %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAsetfillvalue(ZAid_simple,\"Temperature\", &fillvalue2);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAsetfillvalue for field Temperature %d\n", status););
	 }
   
   
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAdefine                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdefine... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdefine... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data fields for za SIMPLE  \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data fields for za SIMPLE  \n"););
   
   
   /*
   ----------------------------  ZA  SIMPLE ---------------------------
	  
	  
	  
   ---------------  D e f i n i n g  T e m p e r a t u r e  --------------
   */
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining data field Temperature\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Temperature\n"););
   
   status = HE5_ZAdefine(ZAid_simple, "Temperature", "DataTrack,DataXtrack", NULL,H5T_NATIVE_DOUBLE); 
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
		 MESSAGE(10,printf("\t\tstatus=HE5_ZAdefine(ZAid_simple,\"Temperature\",NULL, H5T_NATIVE_DOUBLE ,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefine %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefine(ZAid_simple,\"Temperature\",NULL, H5T_NATIVE_DOUBLE ,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefine %d\n", status););
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
   MESSAGE(6,printf("\t\tDefining chunking for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining chunking for za SIMPLE \n"););  
   
   status = HE5_ZAdefchunk(ZAid_simple,RANK1,chunk_dims);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefchunk(ZAid_simple,RANK1,chunk_dims);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefchunk %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefchunk(ZAid_simple,RANK1,chunk_dims);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefchunk %d\n",status););
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
   
   
   comp_code = 4;
   
   /*                 
   ---------------  D e f i n i n g  c o m p r e s s i o n  l e v e l   --------------
   */
   
   comp_level[ 0 ] = 6;
   
   MESSAGE(6,printf("\t\n"););  
   MESSAGE(6,printf("\t\tDefining compression parameters for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tDefining compression parameters for za SIMPLE \n"););
   
   status = HE5_ZAdefcomp(ZAid_simple,comp_code,comp_level);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefcomp(ZAid_simple,comp_code, comp_level);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefcomp %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefcomp(ZAid_simple,comp_code, comp_level);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefcomp %d\n",status););
	 }
   
   
   /*                 
   ---------------   D e f i n i n g   C o u n t   --------------
   */

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining data field Count\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Count\n"););
   status = HE5_ZAdefine(ZAid_simple, "Count", "DataTrack","Unlim", H5T_NATIVE_INT);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefine(ZAid_simple, \"Count\",\"DataTrack\",\"Unlim\",H5T_NATIVE_INT,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefine %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefine(ZAid_simple,\"Count\",\"DataTrack\",\"Unlim\",H5T_NATIVE_INT,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefine %d\n", status););
	 }
   


   /*
   *********************************************************************************
   *                             Testing    HE5_ZAdefcomp                          *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\t\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdefcomp... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdefcomp... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   /*                 
   --------------------   D e f i n i n g   c h u n k i n g   ---------------------
   */
   
   
   chunk_dims[ 0 ] = 5 ;
   chunk_dims[ 1 ] = 2 ;
   
   
   MESSAGE(6,printf("\t\n"););
   MESSAGE(6,printf("\t\tDefining chunking for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining chunking for za SIMPLE \n"););  
   
   status = HE5_ZAdefchunk(ZAid_simple,RANK2,chunk_dims);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefchunk(ZAid_simple,RANK2,chunk_dims);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefchunk %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefchunk(ZAid_simple,RANK2,chunk_dims);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefchunk %d\n",status););
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
   
   
   comp_code = 4;
   
   /*                 
   ---------------  D e f i n i n g  c o m p r e s s i o n  l e v e l   --------------
   */
   
   comp_level[ 0 ] = 6;
   
   MESSAGE(6,printf("\t\n"););  
   MESSAGE(6,printf("\t\tDefining compression parameters for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tDefining compression parameters for za SIMPLE \n"););
   
   status = HE5_ZAdefcomp(ZAid_simple,comp_code,comp_level);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefcomp(ZAid_simple,comp_code, comp_level);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefcomp %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefcomp(ZAid_simple,comp_code, comp_level);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefcomp %d\n",status););
	 }
   
   
   /*                 
   ---------------   D e f i n i n g   C o n d u c t i o n   --------------
   */

   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDefining data field Conduction\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDefining data field Conduction\n"););
   status = HE5_ZAdefine(ZAid_simple, "Conduction", "DataTrack,DataXtrack", NULL, H5T_NATIVE_DOUBLE);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAdefine(ZAid_simple, \"Conduction\",\"DataTrack,DataXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAdefine %d\n", status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAdefine(ZAid_simple,\"Conduction\",\"DataTrack,DataXtrack\",NULL,H5T_NATIVE_DOUBLE,0);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAdefine %d\n", status););
	 }
   
   
   
   
   /*
   *********************************************************************************
   *                             Testing   HE5_ZAwrite                             *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAwrite... \n"););
   MESSAGE(4,printf("\t====================== \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAwrite... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););
   
   
   
   /*
   -------------------------------- ZA SIMPLE -------------------------
	  

   --------- W r i t i n g    T e m p e r a t u r e    i n    a    z a  --------
   */
   
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tWriting field Temperature in za SIMPLE \n"););   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Temperature in za SIMPLE \n"););
   
   tstart[ 0 ]   = 0;
   tstart[ 1 ]   = 0;
   tedge[ 0 ]    = 10;
   tedge[ 1 ]    = 4;
   
   status = HE5_ZAwrite(ZAid_simple, "Temperature", tstart, NULL, tedge, TempData);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAwrite(ZAid_simple,\"Temperature\", tstart,stride, tedge, TempData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAwrite %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAwrite(ZAid_simple,\"Temperature\", tstart, stride, tedge, TempData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAwrite %d\n",status););
	 }
   
   
   
   /*                 
   --------- W r i t i n g    C o n d u c t i o n    i n    a    z a  --------
   */
   
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting field Conduction in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Conduction in za SIMPLE \n"););
   
   status = HE5_ZAwrite(ZAid_simple, "Conduction", tstart, NULL, tedge, CondData);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAwrite(ZAid_simple, \"Conduction\",tstart, stride, tedge, CondData);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAwrite for field Conduction in za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAwrite(ZAid_simple,\"Conduction\", tstart, stride, tedge, CondData);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAwrite for field Conduction in za SIMPLE %d\n",status););
	 }
   
   
   
   /*                 
   --------- W r i t i n g    C o u n t    i n    a    z a  --------
   */

   tstart[0] = 0; tedge[0] = 40;
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting field Count in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting field Count in za SIMPLE \n"););
   
   status = HE5_ZAwrite(ZAid_simple, "Count", tstart, NULL, tedge, inarray);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAwrite(ZAid_simple, \"Count\",tstart, NULL, tedge, inarray);\n"););
       MESSAGE(8,printf("\t\tValue returned by HE5_ZAwrite for field Count in za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAwrite(ZAid_simple,\"Count\", tstart, stride, NULL, inarray);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by HE5_ZAwrite for field Conduction in za SIMPLE %d\n",status););
	 }
   


   /*
   *********************************************************************************
   *                             Testing   HE5_ZAwriteattr                         *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAwriteattr... \n"););
   MESSAGE(4,printf("\t==========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAwriteattr... \n"););
   MESSAGE(11,fprintf(fp,"\t==========================\n"););
   
   /*                 
   --------- W r i t i n g   " D r i f t "    i n    a    z a  --------
   */
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tWriting attribute Drift in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););   
   MESSAGE(11,fprintf(fp,"\t\tWriting attribute Drift in za SIMPLE \n"););   
   status = HE5_ZAwriteattr(ZAid_simple,"Drift",H5T_NATIVE_INT,CurrentSize,AttrVal);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAwriteattr(ZAid_simple, \"Drift\",H5T_NATIVE_INT, 4, AttrVal);\n"););
       MESSAGE(8,printf("\t\tValue returned by ZAwriteattr for attribute Drift in za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAwriteattr(ZAid_simple, \"Drift\",H5T_NATIVE_INT, 4, AttrVal);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by ZAwriteattr for attribute Drift in za SIMPLE %d\n",status););

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
   *                             Testing   HE5_ZAreadattr                          *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAreadattr... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAreadattr... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   
   /*                 
   --------- R e a d i n g   " D r i f t "    a t t r i b u t e  --------
   */
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading attribute Drift from za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading attribute Drift from za SIMPLE \n"););
   
   status = HE5_ZAreadattr(ZAid_simple,"Drift", AttrValOut);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAreadattr(ZAid_simple,\"Drift\", AttrValOut);\n"););
       MESSAGE(8,printf("\t\tValue returned by ZAreadattr for attribute Drift in za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAreadattr(ZAid_simple,\"Drift\", AttrValOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by ZAreadattr for attribute Drift in za SIMPLE %d\n",status););
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
   *                             Testing   HE5_ZAnentries                          *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAnentries... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAnentries... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   
   /*                 
   -----  R e t r i e v e   #  o f   d i m e n s i o n s   i n   a   z a  -----
   */
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve number of dimensions in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve number of dimensions in za SIMPLE \n"););
   
   number = HE5_ZAnentries(ZAid_simple,0,&strbufsize);
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAnentries(ZAid_simple,0,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of dimensions  %li\n",number););
       MESSAGE(8,printf("\t\tString size of dimension entries:        %li (bytes)\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAnentries(ZAid_simple,0,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of dimensions  %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tString size of dimension entries:        %li (bytes)\n",strbufsize););
	 }
   
   
   
   /*
   *********************************************************************************
   *                             Testing   HE5_ZAinqdims                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAinqdims... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAinqdims... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   DimNamePtr      = (char  *)calloc( 1, strbufsize+1);
   
   
   /*                 
   -----  R e t r i e v e   d i m e n s i o n   l i s t  f o r   a   z a  -----
   */   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve the dimension information for za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the dimension information for za SIMPLE \n"););
   
   
   number = HE5_ZAinqdims(ZAid_simple,DimNamePtr,Dims);   
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
       MESSAGE(8,printf("\t\tnumber=HE5_ZAinqdims(ZAid_simple,DimNamePtr,DimListPtr);\n"););
       MESSAGE(8,printf("\t\tDimension list: \n"););
       MESSAGE(8,printf("\t\t                %s\n",DimNamePtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAinqdims(ZAid_simple,DimNamePtr,DimListPtr);\n"););
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
   *                             Testing   HE5_ZAinquire                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAinquire... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAinquire... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about Data fields in za SIMPLE\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about Data fields in za SIMPLE\n"););
   
   number = HE5_ZAnentries(ZAid_simple,4,&strbufsize);   
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAnentries(ZAid_simple,4,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of data fields) returned by ZAnentries %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAnentries(ZAid_simple,4,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of data fields) returned by ZAnentries %li\n",number););
	 }
   
   RankPtr         = (int *)calloc(number, sizeof(int));
   FieldListPtr    = (char  *)calloc( BUFSIZE, sizeof(char));

   
   MESSAGE(6,printf("\t\tRetrieve the Data fields in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve the Data fields in za SIMPLE \n"););
	 
   number = HE5_ZAinquire(ZAid_simple,FieldListPtr,RankPtr,ntype);
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAinquire(ZAid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAinquire(ZAid_simple,FieldListPtr,RankPtr,ntype);\n"););
       MESSAGE(8,printf("\t\tList of Data fields:\n"););
       MESSAGE(8,printf("\t\t  %s\n",FieldListPtr););
       MESSAGE(11,fprintf(fp,"\t\tList of Data fields:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %s\n",FieldListPtr););
       MESSAGE(8,printf("\t\tRanks:       Data types:\n"););
       MESSAGE(11,fprintf(fp,"\t\tRanks:        Data types:\n"););

	   for (i = 0; i < number; i++)
		 {
           MESSAGE(8,printf("\t\t %d             %d\n", RankPtr[i], ntype[i]););
           MESSAGE(11,fprintf(fp,"\t\t %d             %d\n", RankPtr[i], ntype[i]););
		 }
	 }
   
   free(RankPtr);
   free(FieldListPtr);
   
   
   /*
   *********************************************************************************
   *                             Testing   HE5_ZAdiminfo...                        *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdiminfo... \n"););
   MESSAGE(4,printf("\t======================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdiminfo... \n"););
   MESSAGE(11,fprintf(fp,"\t======================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about DataXtrack dimension in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about DataXtrack dimension in za SIMPLE \n"););
   
   
   ndims = HE5_ZAdiminfo(ZAid_simple,"DataXtrack");   
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
       MESSAGE(10,printf("\t\tndims=HE5_ZAdiminfo(ZAid_simple,\"DataXtrack\");\n"););
       MESSAGE(8,printf("\t\tSize of dimension DataXtrack:    %lu \n",(unsigned long)ndims););
       MESSAGE(11,fprintf(fp,"\t\tndims=HE5_ZAdiminfo(ZAid_simple,\"DataXtrack\");\n"););
       MESSAGE(11,fprintf(fp,"\t\tSize of dimension DataXtrack:      %lu \n",(unsigned long)ndims););
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAread                             *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAread... \n"););
   MESSAGE(4,printf("\t===================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAread... \n"););
   MESSAGE(11,fprintf(fp,"\t===================== \n"););
   
   inq_start[0]  = 0;
   inq_start[1]  = 0;
   inq_stride[0] = 1;
   inq_stride[1] = 1;
   inq_edge[0]   = 10;
   inq_edge[1]   = 4;
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tReading data from field \"Temperature\" in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tReading data from field \"Temperature\" in za SIMPLE\n"););
   
   number = HE5_ZAread(ZAid_simple,"Temperature",inq_start,inq_stride,inq_edge,TempDataOut);   
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAread(ZAid_simple,\"Temperature\",inq_start,inq_stride,inq_edge,TempDataOut);\n"););
       MESSAGE(8,printf("\t\tstatus return from call to ZAread %li\n", number ););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAread(ZAid_simple,\"Temperature\",inq_start,inq_stride,inq_edge,TempDataOut);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus return from call to ZAread %li\n", number ););
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
   *                             Testing    HE5_ZAinfo                             *
   *********************************************************************************
   */
      
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAinfo... \n"););
   MESSAGE(4,printf("\t===================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););   
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t===================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about field \"Temperature\" in za SIMPLE e\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about field \"Temperature\" in za SIMPLE \n"););
   
   status = HE5_ZAinfo(ZAid_simple,"Temperature",&rank,dims,&dtype,DimList,NULL);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAinfo(ZAid_simple,\"Temperature\",&rank,dims,&dtype,DimList,NULL);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAfieldinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAinfo(ZAid_simple,\"Temperature\",&rank,dims,&dtype,DimList,NULL);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by ZAfieldinfo %d\n",status););
       MESSAGE(8,printf("\t\tRank:   %d      Data type:     %d\n",rank, dtype););
       MESSAGE(8,printf("\t\tDimension list:           %s\n",DimList););
       MESSAGE(11,fprintf(fp,"\t\tRank:    %d      Data type:    %d\n",rank, dtype););
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
   *                             Testing    HE5_ZAinqattrs                         *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAinqattrs... \n"););
   MESSAGE(4,printf("\t========================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAinqattrs... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about attributes defined in za INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attributes defined in za INDEX \n"););
   
   number = HE5_ZAinqattrs(ZAid_simple, NULL, &strbufsize);   
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAinqattrs(ZAid_simple, NULL, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAinqattrs(ZAid_simple, NULL, &strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue (number of attributes) returned by ZAinqattrs %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tValue (number of attributes) returned by ZAinqattrs %li\n",number););
       MESSAGE(8,printf("\t\tString length of attribute list:     %li\n",strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tString length of attribute list:    %li\n",strbufsize););
	 }
   
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve more information about attributes defined in za INDEX \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve more information about attributes defined in za INDEX \n"););
   
   ObjNamesPtr     = (char  *)calloc( BUFSIZE, sizeof(char));
   
   number = HE5_ZAinqattrs(ZAid_simple, ObjNamesPtr, &strbufsize);   
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
       MESSAGE(10,printf("\t\tnumber=HE5_ZAinqattrs(ZAid_simple, ObjNamesPtr, &strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tnumber=HE5_ZAinqattrs(ZAid_simple, ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of attributes returned by ZAinqattr    %li\n",number););
       MESSAGE(11,fprintf(fp,"\t\tNumber of attributes returned by ZAinqattr    %li\n",number););
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
   *                             Testing    HE5_ZAattrinfo                         *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAattrinfo... \n"););
   MESSAGE(4,printf("\t=========================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAattrinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t=========================\n"););
   
   CountPtr        = (hsize_t *)calloc(1, sizeof(hsize_t));
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about attribute \"Drift\" \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about attribute \"Drift\" \n"););
   
   status=HE5_ZAattrinfo(ZAid_simple,"Drift",&dtype,CountPtr);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAattrinfo(ZAid_simple,\"Drift\",&dtype,CountPtr);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAattrinfo %d\n",status););
       MESSAGE(6,printf("\t\tData type   Number of attribute elements:\n"););
       MESSAGE(6,printf("\t\t   %d            %d\n", dtype,(int)*CountPtr););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAattrinfo(ZAid_simple,\"Drift\",&dtype,CountPtr);\n"););
       MESSAGE(11,fprintf(fp,"\t\tStaus returned by ZAattrinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tData type   Number of attribute elements:\n"););
       MESSAGE(11,fprintf(fp,"\t\t   %d            %d\n", dtype,(int)*CountPtr););
	 }
   
   
   free(CountPtr);
      
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAcompinfo                         *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAcompinfo... \n"););
   MESSAGE(4,printf("\t========================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAcompinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t========================= \n"););
   MESSAGE(6,printf("\n");); 
   MESSAGE(6,printf("\t\tRetrieve Compression information for field \"Temperature\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve Compression information for field \"Temperature\"\n"););
   
   status = HE5_ZAcompinfo(ZAid_simple,"Temperature",&code,level);   
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
   
   
   code       = - 8;
   level[ 0 ] = - 8;
   
   MESSAGE(4,printf("\n"););   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(6,printf("\t\tRetrieve Compression information for field \"Conduction\"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieve Compression information for field \"Conduction\"\n"););
   
   status = HE5_ZAcompinfo(ZAid_simple,"Conduction",&code,level);
   
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
   *                             Testing    HE5_ZAgetfillvalue                     *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););    
   MESSAGE(4,printf("\tTesting HE5_ZAgetfillvalue... \n"););
   MESSAGE(4,printf("\t============================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAgetfillvalue... \n"););
   MESSAGE(11,fprintf(fp,"\t============================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tGet fill value for field \"Temperature\"\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tGet fill value for field \"Temperature\"\n"););
   status = HE5_ZAgetfillvalue(ZAid_simple,"Temperature",&InqDoubleFillValue);
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAgetfillvalue(ZAid_simple,\"Temperature\",&InqDoubleFillValue);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAgetfillvalue %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAgetfillvalue(ZAid_simple,\"Temperature\", &InqDoubleFillValue)\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by ZAgetfillvalue %d\n",status););
       MESSAGE(8,printf("\t\tFill value for field \"Temperature\":\n"););
       MESSAGE(8,printf("\n"););
       MESSAGE(8,printf("\t\t     \"%lf\" \n", InqDoubleFillValue););
       MESSAGE(11,fprintf(fp,"\t\tFill value for field \"Temperature\":\n"););
       MESSAGE(11,fprintf(fp,"\n"););
       MESSAGE(11,fprintf(fp,"\t\t      \"%lf\" \n",InqDoubleFillValue););
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAdetach                           *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from za SIMPLE \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from za SIMPLE \n"););
   

   status = HE5_ZAdetach(ZAid_simple);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdetach(ZAid_simple);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAdetach()   %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdetach(ZAid_simple);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by ZAdetach()    %d\n",status););
	 }
   
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAattach                           *
   *********************************************************************************
   */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAattach... \n"););
   MESSAGE(4,printf("\t=======================\n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAattach... \n"););
   MESSAGE(11,fprintf(fp,"\t=======================\n"););
  
   ZAid_simple = HE5_ZAattach(zafidc_simple,"SIMPLE");
   if (ZAid_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully attached to the za SIMPLE ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully attached to the za SIMPLE ****\n"););
	 }
   
   
   /*
   *********************************************************************************
   *                  Testing  Appending data to the field                         *
   *********************************************************************************
   */
   
   tstart[0] = 30; tedge[0] = 10;
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tWriting extended data to the field Count in za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tWriting extended data to the field Count in za SIMPLE \n"););
   
   status = HE5_ZAwrite(ZAid_simple, "Count", tstart, NULL, tedge, inarray);   
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAwrite(ZAid_simple, \"Count\",tstart, NULL, tedge, inarray);\n"););
       MESSAGE(8,printf("\t\tValue returned by ZAwrite for field Count in za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAwrite(ZAid_simple,\"Count\", tstart, stride, NULL, inarray);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by ZAwrite for field Conduction in za SIMPLE %d\n",status););
	 }
   
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAinfo                             *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAinfo... \n"););
   MESSAGE(4,printf("\t===================== \n"););
   
   MESSAGE(11,fprintf(fp,"\t\n"););   
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAinfo... \n"););
   MESSAGE(11,fprintf(fp,"\t===================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tRetrieve information about field \"Count\" in za SIMPLE e\n"););
   MESSAGE(11,fprintf(fp,"\t\n"););  
   MESSAGE(11,fprintf(fp,"\t\tRetrieve information about field \"Count\" in za SIMPLE \n"););
   
   status = HE5_ZAinfo(ZAid_simple,"Count",&rank,dims, &dtype,NULL,DimList);   
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
       MESSAGE(10,printf("\t\tstatus=HE5_ZAinfo(ZAid_simple,\"Count\",&rank,dims,&dtype,NULL,DimList);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAfieldinfo %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus=HE5_ZAinfo(ZAid_simple,\"Count\",&rank,dims, &dtype,NULL,DimList);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by ZAfieldinfo %d\n",status););
       MESSAGE(8,printf("\t\tRank:   %d      Data type:     %d\n",rank, dtype););
       MESSAGE(8,printf("\t\tMaximum Dimension list:           %s\n",DimList););
       MESSAGE(11,fprintf(fp,"\t\tRank:    %d      Data type:    %d\n",rank, dtype););
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
   *                             Testing    HE5_ZAdetach                           *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAdetach... \n"););
   MESSAGE(4,printf("\t======================= \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAdetach... \n"););
   MESSAGE(11,fprintf(fp,"\t======================= \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tDetaching from za SIMPLE \n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tDetaching from za SIMPLE \n"););

   status = HE5_ZAdetach(ZAid_simple);
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
       MESSAGE(10,printf("\t\tstatus = HE5_ZAdetach(ZAid_simple);\n"););
       MESSAGE(8,printf("\t\tstatus returned by ZAdetach for za SIMPLE %d\n",status););
       MESSAGE(11,fprintf(fp,"\t\tstatus = HE5_ZAdetach(ZAid_simple);\n"););
       MESSAGE(11,fprintf(fp,"\t\tstatus returned by ZAdetach for za SIMPLE%d\n",status););
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    HE5_ZAclose                            *
   *********************************************************************************
   */
   
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting HE5_ZAclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););
   MESSAGE(11,fprintf(fp,"\t\n"););
   MESSAGE(11,fprintf(fp,"\tTesting HE5_ZAclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file SimpleZa.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file SimpleZa.he5\n"););
   
   status = HE5_ZAclose(zafidc_simple);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\t*** Successfully clossed the SimpleZa.he5 file ****\n"););
	   MESSAGE(11,fprintf(fp,"\t\t*** Successfully clossed the SimpleZa.he5 file ****\n"););
	 }
   
   
   /*
   *********************************************************************************
   *                             Testing    ZAopen                                 *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting ZAopen... \n"););
   MESSAGE(4,printf("\t=================\n"););
   MESSAGE(11,fprintf(fp,"\tTesting ZAopen... \n"););
   MESSAGE(11,fprintf(fp,"\t=================\n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tOpening the file SimpleZa.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tOpening the file SimpleZa.he5\n"););
   
   zafidc_simple = HE5_ZAopen("SimpleZa.he5",H5F_ACC_RDWR);
   if (zafidc_simple == FAIL)
	 {
       MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
       MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););   
       errcnt++;
	 }
   else
	 {
       MESSAGE(4,printf("\t\t*** Successfully opened the za file ****\n"););
       MESSAGE(11,fprintf(fp,"\t\t*** Successfully opened the za file ****\n"););
       
	 }
   
   
   
   /*
   *********************************************************************************
   *                             Testing    ZAinqza                                *
   *********************************************************************************
   */
   
   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting ZAinqza... \n"););
   MESSAGE(4,printf("\t=================\n"););
   MESSAGE(11,fprintf(fp,"\tTesting ZAinqza... \n"););
   MESSAGE(11,fprintf(fp,"\t=================\n"););
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tRetrieving general information about objects within the file\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tRetrieving general information about objects within the file\n"););
   number = HE5_ZAinqza("SimpleZa.he5",NULL,&strbufsize);
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
       MESSAGE(10,printf("\t\tnumber = HE5_ZAinqza(\"SimpleZa.he5\",NULL,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tValue returned by ZAinqza %li\n", number););
       MESSAGE(8,printf("\t\tString length returned by ZAinqza %li\n", strbufsize););
       MESSAGE(11,fprintf(fp,"\t\tnumber = HE5_ZAinqza(\"SimpleZa.he5\",NULL,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tValue returned by ZAinqza %li\n", number););
       MESSAGE(11,fprintf(fp,"\t\tStringlength returned by ZAinqza %li\n", strbufsize););
	 }
   
   
   
   MESSAGE(6,printf("\n"););  
   MESSAGE(6,printf("\t\tContinue ...\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tContinue ...\n"););
   ObjNamesPtr = (char *)calloc(strbufsize+1, sizeof(char ) );
   number = HE5_ZAinqza("SimpleZa.he5",ObjNamesPtr,&strbufsize);
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
       MESSAGE(10,printf("\t\tnumber = HE5_ZAinqza(\"SimpleZa.he5\",ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(8,printf("\t\tNumber of za objects:        %li\n", number););
       MESSAGE(8,printf("\t\tList of za objects:      %s\n", ObjNamesPtr););
       MESSAGE(11,fprintf(fp,"\t\tnumber = HE5_ZAinqza(\"SimpleZa.he5\",ObjNamesPtr,&strbufsize);\n"););
       MESSAGE(11,fprintf(fp,"\t\tNumber of za objecys:       %li\n", number););
       MESSAGE(11,fprintf(fp,"\t\tList of za objects:      %s\n", ObjNamesPtr););

	 }
   
   
   free(ObjNamesPtr);
   
   
   
   
   /*
   *********************************************************************************
   *                             Testing    ZAclose                                *
   *********************************************************************************
   */

   MESSAGE(4,printf("\n"););
   MESSAGE(4,printf("\tTesting ZAclose... \n"););
   MESSAGE(4,printf("\t====================== \n"););
   
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\tTesting ZAclose... \n"););
   MESSAGE(11,fprintf(fp,"\t====================== \n"););
   
   MESSAGE(6,printf("\n"););
   MESSAGE(6,printf("\t\tClosing the file SimpleZa.he5\n"););
   MESSAGE(11,fprintf(fp,"\n"););
   MESSAGE(11,fprintf(fp,"\t\tClosing the file SimpleZa.he5\n"););
   
   status = HE5_ZAclose(zafidc_simple);
   if (status == FAIL)
	 {
	   MESSAGE(4,printf("\t\t********Error unexpected**********\n"););
	   MESSAGE(11,fprintf(fp,"\t\t********Error unexpected**********\n"););
	   errcnt++;
	 }
   else
	 {
	   MESSAGE(4,printf("\t\tSuccessfully clossed the SimpleZa.he5 file \n"););
	   MESSAGE(11,fprintf(fp,"\t\tSuccessfully clossed the SimpleZa.he5 file \n"););
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


