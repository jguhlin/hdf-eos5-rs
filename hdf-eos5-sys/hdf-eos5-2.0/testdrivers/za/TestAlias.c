/*
 ******************************************************************
 * File:        TestAlias.c                                       *
 *                                                                *
 * Date:        Aug 2002                                          *
 * Purpose:     Testing ZA interfacing routines                   *        
 ******************************************************************
 */

#include        <HE5_HdfEosDef.h>

#define  FILENAME   "Alias.he5"
#define  RANK       2

int main()
{
  herr_t       status = FAIL;
  
  int          i, j;
  int          rank       = FAIL;
  int          length     = 0;
  int          attr[1]    = {2000};
  int          fieldgroup = FAIL;
  int          comp_level[5] = { 0,0,0,0,0 };
  int          comp_code;
  int          level[5] = {0,0,0,00}, code = 0;
  int          *fldrank = (int *)NULL;
  int          return_val = FAIL;

  H5T_class_t  dtype   = H5T_NO_CLASS; 
  hid_t        dtype1  = FAIL;
  H5T_order_t  order   = H5T_ORDER_ERROR;
  hid_t        ntype[3] = {FAIL, FAIL, FAIL}; 

  long         nattr      = FAIL;
  long         nfldalias     = FAIL;
  long         strbufsize = FAIL; 
  long         nflds = FAIL;
  long         nalias = FAIL;

  hid_t        ZAid = FAIL, zafid = FAIL;
  hid_t        datatype = FAIL;
  
  float        data[ 20 ][ 10 ];
  float        out_buffer[ 20 ][ 10 ];
  float        fillvalue = -777.777;
  float        out_fillvalue = -1.;

  size_t       size    = 0;

  hssize_t     start[ 2 ] = { 0, 0 };
  hsize_t      count[ 2 ] = { 5, 5};
  hsize_t      nelem = 0;
  hsize_t      dims[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  hsize_t      chunk_dims[ 2 ];
  
  char         dimlist[80];
  char         maxdimlist[80];
  char         *namebuf, attrlist[80];
  char         fldlist[80];
  char         fldalias[80];
  char         aliaslist[80];

  /* 
   *****************************************************************************
   *                 Open/Create the ZA "Test_1"                               *
   *****************************************************************************
   */
  
  zafid = HE5_ZAopen(FILENAME, H5F_ACC_TRUNC);
  printf("ZA file ID                      = %d\n", zafid);
  
  ZAid   = HE5_ZAcreate(zafid,"Test_1"); 
  printf("ZA ID                           = %d\n", ZAid);
  
  /* 
   *****************************************************************************
   *                     Define Dimensions                                     *
   *****************************************************************************
   */
  
  
  status = HE5_ZAdefdim(ZAid,"DataTrack", 20);
  printf("status returned by HE5_ZAdefdim        = %d\n", status);
  
  status = HE5_ZAdefdim(ZAid,"DataXtrack", 10);
  printf("status returned by HE5_ZAdefdim        = %d\n", status);

  chunk_dims[ 0 ] = 10;
  chunk_dims[ 1 ] = 5;
  comp_code       = 4;
  comp_level[0]   = 6;
  
  status = HE5_ZAdefcomchunk(ZAid, comp_code, comp_level, RANK, chunk_dims);
  printf("status returned by HE5_ZAdefcomchunk   = %d\n", status);

  status = HE5_ZAsetfillvalue(ZAid, "Temperature", H5T_NATIVE_FLOAT, &fillvalue);
  printf("status returned by HE5_ZAsetfillvalue  = %d\n", status);

  status = HE5_ZAdefine(ZAid,"Temperature", "DataTrack,DataXtrack", NULL, H5T_NATIVE_FLOAT);
  printf("status returned by HE5_ZAdefine  = %d\n", status);
  
  
  /* 
   *****************************************************************************
   *                  Write data to the field "Temperature"                    *
   *****************************************************************************
   */
  for ( i = 0; i < 20; i++ )
	for( j = 0; j < 10; j++ )
	  data[ i ][ j ] = 1.e0 + i + 5.e-2 * j ;
  
  status  = HE5_ZAwrite(ZAid,"Temperature", start, NULL, count, data );
  printf("status returned by HE5_ZAwrite    = %d\n", status);
  

  /* Set aliases for field "Temperature" */
  /* ----------------------------------- */
  status  = HE5_ZAsetalias(ZAid,"Temperature","T,Tmp,Temp,L2gpValue");
  printf("status returned by HE5_ZAsetalias      = %d\n", status);

  strcpy(fldalias,"");
  nfldalias = HE5_ZAinqfldalias(ZAid, fldalias, &strbufsize);
  printf("Number of fields & aliases is: %li\n", nfldalias);
  printf("Fields & aliases list:            %s \n", fldalias);
  printf("Length of fields & aliases list:  %li \n", strbufsize);

  strcpy(aliaslist,"");
  nalias = HE5_ZAgetaliaslist(ZAid, HE5_HDFE_DATAGROUP, aliaslist, &strbufsize);
  printf("Number of aliases is: %li\n", nalias);
  printf("Aliases list:            %s \n", aliaslist);
  printf("Length of aliases list:  %li \n", strbufsize);

  /* Get information about alias "Temp" */
  /* ---------------------------------- */
  status = HE5_ZAaliasinfo(ZAid,  HE5_HDFE_DATAGROUP, "Temp", &length, NULL);
  if (status == SUCCEED)
	printf("Name length: %d \n", length);
  
  namebuf = (char *)calloc(length, sizeof(char));

  /* Get the actual name of the field "Temp" */
  /* --------------------------------------- */
  status = HE5_ZAaliasinfo(ZAid,  HE5_HDFE_DATAGROUP, "Temp", &length, namebuf);
  if (status == SUCCEED)
	printf("Original field name: %s \n", namebuf);
  
  free(namebuf);
  
  /* Remove alias "Tmp" from the alias list */
  /* -------------------------------------- */
  status  = HE5_ZAdropalias(ZAid, HE5_HDFE_DATAGROUP, "Tmp");
  printf("status returned by HE5_ZAdropalias     = %d\n", status);

  /* Remove alias "Temp" from the alias list */
  /* --------------------------------------- */
  status  = HE5_ZAdropalias(ZAid, HE5_HDFE_DATAGROUP, "Temp");
  printf("status returned by HE5_ZAdropalias     = %d\n", status);

  /* Get information about alias  "L2gpValue" */
  /* ---------------------------------------- */  
  status = HE5_ZAaliasinfo(ZAid,  HE5_HDFE_DATAGROUP, "L2gpValue", &length, NULL);
  if (status == SUCCEED)
	printf("Name length: %d \n", length);
  

  namebuf = (char *)calloc(length, sizeof(char));

  /* Get the actual name of the field "L2gpValue" */
  /* -------------------------------------------- */  
  status = HE5_ZAaliasinfo(ZAid, HE5_HDFE_DATAGROUP, "L2gpValue", &length, namebuf);
  if (status == SUCCEED)
	printf("Original field name: %s \n", namebuf);
  
  free(namebuf);

  count[0] = 20;  count[1] = 10;
  
  /* Read the data from the field calling it by alias */
  /* ------------------------------------------------ */
  status  = HE5_ZAread(ZAid,"L2gpValue", start, NULL, count, out_buffer);
  printf("status returned by HE5_ZAread     = %d\n", status);

  printf("Data from \"L2gpValue\" field: \n");
  for ( i = 0; i < 20; i++ )
	for( j = 0; j < 10; j++ )
	  printf("%f \n", out_buffer[ i ][ j ]);

  /* Write local attribute to "L2gpValue" */
  /* ------------------------------------ */
  count[0] = 1;
  status = HE5_ZAwritelocattr(ZAid, "L2gpValue", "LocalAttribute", H5T_NATIVE_INT, count, attr);
  printf("status returned by HE5_ZAwritelocattr  = %d\n", status);


  status = HE5_ZAdetach(ZAid);
  printf("status returned by HE5_ZAdetach        = %d\n", status); 

  ZAid = HE5_ZAattach(zafid,"Test_1");
  printf("ZA ID returned by HE5_ZAattach      = %d\n", ZAid); 

  printf("Local Attributes:\n");
  nattr = HE5_ZAinqlocattrs(ZAid, "L2gpValue", NULL, &strbufsize);
  printf("Number of attributes:      %li \n", nattr);
  printf("Length of attribute list:  %li \n", strbufsize);
  strcpy(attrlist,"");
  nattr = HE5_ZAinqlocattrs(ZAid, "L2gpValue", attrlist, &strbufsize);
  printf("Attribute list:            %s \n", attrlist);

  printf("Local Attribute:\n");
  status = HE5_ZAlocattrinfo(ZAid,"T", "LocalAttribute",&dtype1, &nelem);
  printf("Data type:                 %d\n", dtype1);
  printf("Number of elements:        %lu \n", (unsigned long)nelem);    

  status = HE5_ZAinfo(ZAid, "L2gpValue", &rank, dims, &dtype1, dimlist, maxdimlist);
  if (status != FAIL)
	{
	  printf("Field rank:                %d \n", rank);
	  printf("Dimension Sizes: \n");
	  for ( i = 0; i < rank; i++)
		{
		  printf("                           %lu \n", (unsigned long)dims[i]);
		}
  
	  printf("Data type ID:              %d \n", dtype1);  
	  printf("DimList string:            %s \n", dimlist);
	  printf("MaxdimList string:         %s \n", maxdimlist);
	}

  fieldgroup = HE5_HDFE_DATAGROUP;
  status = HE5_ZAinqdatatype(ZAid, "T", NULL, fieldgroup, &datatype, &dtype, &order, &size);
  printf("status returned by HE5_ZAinqdatatype   = %d \n", status);
  if (status != FAIL)
	{
	  printf("Data type ID:              %d \n", datatype);
	  printf("Data type class ID:        %d \n", dtype);
	  printf("Byte order:                %d \n", order);
	  printf("Data size (bytes):         %d \n", (int)size);        
	}

  status  = HE5_ZAcompinfo(ZAid,"L2gpValue", &code, level);
  printf("status returned by HE5_ZAcompinfo      = %d\n", status);
  if (status != FAIL)
	{
	  printf("Compression Information: \n");  
	  printf("Compression code:          %d \n", code);
	  printf("Compression level:         %d \n", level[0]);
	}

  status = HE5_ZAgetfillvalue(ZAid,"L2gpValue",&out_fillvalue);
  printf("status returned by HE5_ZAgetfillvalue  = %d\n", status);
  printf("The fill value:            %f\n", out_fillvalue);

  status = HE5_ZAdetach(ZAid);
  printf("status returned by HE5_ZAdetach        = %d\n", status); 

  ZAid = HE5_ZAattach(zafid,"Test_1");
  printf("ZA ID returned by HE5_ZAattach      = %d\n", ZAid); 

  
  nflds = HE5_ZAnentries(ZAid,4,&strbufsize);
  printf("Value returned by HE5_ZAnentries       = %li\n", nflds); 
  if (nflds != FAIL)
	{
	  fldrank   = (int *)calloc(nflds, sizeof(int));
	 
	  nflds = FAIL;
	  nflds = HE5_ZAinquire(ZAid,fldlist,fldrank,ntype);
	  printf("Value returned by HE5_ZAinquire  = %li \n", nflds);
	  if (nflds != FAIL)
		{
		  printf("List of fields:            %s \n", fldlist); 
		  for (i = 0; i < nflds; i++)
			printf("Field rank, class ID:      %d, %d \n",fldrank[i],ntype[i]);
		}
	  
	  free(fldrank);
	  
	}

  status = HE5_ZAdetach(ZAid);
  printf("status returned by HE5_ZAdetach        = %d\n", status); 
  
  status = HE5_ZAclose(zafid);
  printf("status returned by HE5_ZAclose         = %d\n", status);

  return 0;
}

















