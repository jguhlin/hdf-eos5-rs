c----------------------------------------------------------------------------
c     Copyright (C) 2000   Emergent IT Inc. and Raytheon Systems Company    |
c----------------------------------------------------------------------------
c testgrid64.f                                                              |
c In this program we create, define and write a simple grid hdfeos file     |
c using the grid interface                                                  |
c----------------------------------------------------------------------------

      program       testgrid64

      implicit      none
 
      include       'hdfeos5.inc'

      integer   	status, i, ii
      integer		compparm(5)
      integer 	    gdfid, gdfid2, gdfid3, gdid_utm, gdid2, gdid3
      integer 	    zonecode, pixregcode, orgcode
      integer	    attr(4), iattr(4), gattr(4), lattr(4)
      integer 	    spherecode, zero
      integer		regionid, regionid2, regionid3, regionid4
      integer	    dtype, classid, order
      integer	    he5_gdopen, he5_gdcreate
      integer       he5_gdattach, he5_gddetach
      integer	    he5_gdclose, he5_gddefproj
      integer       he5_gddefdim, he5_gdinqdatatype
      integer	    he5_gddeftle, he5_gddeffld
      integer       he5_gdsetfill, he5_gdwrfld
      integer	    he5_gdwrattr, he5_gdwrgattr
      integer       he5_gdwrlattr, he5_gdorginfo
      integer	    he5_gddefcomp, he5_gdinqflds
      integer       he5_gdprojinfo
      integer	    he5_gdrdfld, he5_gdattrinfo
      integer       he5_gdgatinfo
      integer       he5_gdlatinfo, he5_gdrdattr
      integer       he5_gdrdgattr, he5_gdrdlattr
      integer	    he5_gdreginfo, he5_gddeftmeper
      integer       he5_gddefvrtreg
      integer	    he5_gddeforigin, he5_gddefpreg
      integer       he5_gdinqdims, he5_gdgetfill
      integer	    he5_gdgridinfo, he5_gdcompinfo
      integer       he5_gdfldinfo, he5_gddefboxreg
      integer	    he5_gdgetpix, he5_gddupreg
      integer       he5_gdpreginfo, he5_gdextreg
      integer		rnk(8), numtype(8)
      integer		zcode, scode, nflds

      integer*4	    ntype
      integer*4		nt(4), rk(4)
      integer*4		periodid
      integer*4		lstatus, n, one      
      integer*4		index1
      integer*4		index2
      integer*4		dimsizes(2)
      integer*4		lat(40,50)
      integer*4		latcnt
      integer*4		lon(40,50)
      integer*4		loncnt
      integer*4	    he5_gdinqgrid, he5_gddiminfo
      integer*4     he5_gdnentries
      integer*4	    he5_gdinqattrs, he5_gdinqgattrs
      integer*4     he5_gdinqlattrs
      integer*4	    pcode

      integer*8	    xdim, ydim
      integer*8	    testdim1, testdim2, hghtdim, conduction
      integer*8	    timedim, convection, setcount(1)
      integer*8 	start(2)
      integer*8 	stride(2)
      integer*8 	edge(2)
      integer*8 	start1d, stride1d, edge1d
      integer*8		tiledims(2)
      integer*8	    count, nlonlat
      integer*8	    size
      integer*8		rowval(5), colval(5)
      integer*8	    xdimsize, ydimsize
      integer*8	    he5_gdgetpixval, he5_gdinterpolate
      integer*8	    strbufsize
      integer*8		dims(8)

      real		    fillvalue, ifillv

      real*4		ray3(250)
      real*4		utmray(60,80), georay(60,80)
      real*4		utmhght(80)
      real*4		hghtinit
      real*4		utmcnt, geocnt
      real*4		raycnt, ray3cnt
      real*4		tempray(60,80)
      real*4		datbuf(4800)
      real*4		fldval(5)
      
      real*8		starttime, stoptime
      real*8		range(2)
      real*8		projparm(16), pparm(16)
      real*8		uplft(2), lowrgt(2)
      real*8		iuplft(2), ilrgt(2)
      real*8		interval(5,2)
      real*8		lonval(5), latval(5)
      real*8		utmtmeray(4800)
      real*8		tmeinit
      real*8		upleftpt(2), lowrtpt(2),corlon(2),corlat(2)

      character*80	grdnames, dimname, fldlist, attrlist
      character*80	maxdimname
      integer*4	    	compcodestring
      

      nt(1)= 0      
      nt(2)= 0      
      nt(3)= 0      
      nt(4)= 0
      rk(1)= 0
      rk(2)= 0
      rk(3)= 0
      rk(4)= 0

      index1 = 0
      index2 = 0
      dimsizes(1) = 40
      dimsizes(2) = 100
      compparm(1) = 6
      compparm(2) = 0
      compparm(3) = 0
      compparm(4) = 0
      compparm(5) = 0
      fillvalue = 1996.0
      raycnt = -799.0
      ray3cnt = -19.5
      attr(1) = 11
      attr(2) = 33
      attr(3) = 55
      attr(4) = 77
      utmcnt=-799.0
      geocnt=2001.0
      hghtinit = 323.0
      tmeinit = 35232487.2d0
      starttime = 35233000.8d0
      stoptime = 35435050.9d0
      range(1) = 352.8d0
      range(2) = 400.9d0
      lonval(1) = -122.0d0
      latval(1) = -68.0d0
      lonval(2) = -122.5d0
      latval(2) = -68.0d0
      lonval(3) = -123.0d0
      latval(3) = -68.0d0
      lonval(4) = -123.5d0
      latval(4) = -68.0d0
      lonval(5) = -124.0d0
      latval(5) = -68.0d0
      tiledims(1) = 16
      tiledims(2) = 15

      latcnt = 38.0
      loncnt = 78.0
      zero = 0
      one = 1
      
c
c This section of the program just fills some arrays with data that we will
c use later in the program
c
      do 70 index1=1,60
         do 80 index2=1,80
            utmray(index2,index1) = utmcnt
            georay(index2,index1) = geocnt
            utmcnt = utmcnt + .4
            geocnt = geocnt + .2
   80   continue
   70 continue
  
      
      do 30 index1=1,250
            ray3(index1) = ray3cnt
            ray3cnt = ray3cnt + .1
   30 continue
      
      do 32 index1=1,80
            utmhght(index1) = hghtinit
            hghtinit = hghtinit + 2.75
   32 continue

      do 34 index1=1,4800
            utmtmeray(index1) = tmeinit
            tmeinit = tmeinit + 70.7
   34 continue

      do 50 index1=1,50
         do 60 index2=1,40      
            lat(index2,index1) = latcnt
            lon(index2,index1) = loncnt
            loncnt = loncnt - 0.1
   60    continue
         latcnt = latcnt + 0.1
         loncnt = 78.0
   50 continue

  230 format('********Error unexpected**********')
  231 format('********Error Expected************')
  240 format('***********Passed Test*********')


      open(unit=10,file='testgridszip64_f.txtout', status = "UNKNOWN")

      write(*,*)'Testing Grid FORTRAN 77 interface '
      write(*,*)'=================================='
      write(10,*)'Testing Grid FORTRAN 77 interface '
      write(10,*)'=================================='
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdopen
c
      write(10,*)'Testing he5_gdopen... '
      write(10,*)'======================'
      write(*,*)'Testing he5_gdopen... '
      write(*,*)'======================'
      gdfid2 = he5_gdopen("grid2szip64.he5",HE5F_ACC_TRUNC)
      if (gdfid2 .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdfid2=he5_gdopen(
     1"grid2szip64.he5",HE5F_ACC_TRUNC)'
         write(10,*)'Value returned by gdopen ', gdfid2
         write(*,*)'gdfid2 = he5_gdopen(
     1"grid2szip64.he5",HE5F_ACC_TRUNC)'
         write(*,*)'Value returned by gdopen ', gdfid2
      endif
      write(*,*)' '
      write(10,*)' '

      gdfid3 = he5_gdopen("grid3szip64.he5",HE5F_ACC_TRUNC)
      if (gdfid3 .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdfid3=he5_gdopen(
     1"grid3szip64.he5",HE5F_ACC_TRUNC)'
         write(10,*) 'Value returned by gdopen ', gdfid3
         write(*,*)'gdfid3 = he5_gdopen(
     1"grid3szip64.he5",HE5F_ACC_TRUNC)'
         write(*,*)'Value returned by gdopen ', gdfid3
      endif
      write(*,*)' '
      write(10,*)' '

      gdfid = he5_gdopen('gridszip64.he5',HE5F_ACC_TRUNC)
      if (gdfid .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'Opening file gridszip64.he5'
         write(10,*)'Opening file gridszip64.he5'
         write(10,*) 'gdfid=he5_gdopen("gridszip64.he5",HE5F_ACC_TRUNC)'
         write(10,*) 'File id returned from gdopen ', gdfid
         write(*,*) 'gdfid=he5_gdopen("gridszip64.he5",HE5F_ACC_TRUNC)'
         write(*,*) 'File id returned from gdopen ', gdfid
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdcreate
c
      write(10,27)
      write(10,*)'Testing he5_gdcreate... '
      write(10,*)'========================'
      write(*,*)'Testing he5_gdcreate... '
      write(*,*)'========================'

      xdim      = 60
      ydim      = 80
      uplft(1)  = 353299.39125d0
      uplft(2)  = -7100284.64348d0
      lowrgt(1) = 603434.52148d0
      lowrgt(2) = -7991341.17870d0


      gdid2 = he5_gdcreate(gdfid2, 'UTMGrid2', 
     1xdim, ydim, uplft, lowrgt)
      if (gdid2 .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*) 'Create UTMGrid2'
         write(*,*)'gdid2 = he5_gdcreate(gdfid2, "UTMGrid2", 
     1xdim, ydim, uplft, lowrgt)'
         write(10,*) 'Value returned by gdcreate ', gdid2
         write(*,*) 'Create UTMGrid2'
         write(10,*)'gdid2 = he5_gdcreate(gdfid2, "UTMGrid2",
     1xdim, ydim, uplft, lowrgt)'
         write(*,*) 'Value returned by gdcreate ', gdid2
      endif

      gdid3 = he5_gdcreate(gdfid3, 'UTMGrid3', 
     1xdim, ydim, uplft, lowrgt)
      if (gdid3 .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*) 'Create UTMGrid3'
         write(*,*)'gdid3 = he5_gdcreate(gdfid3, "UTMGrid3", 
     1xdim, ydim, uplft, lowrgt)'
         write(10,*) 'Value returned by gdcreate ', gdid3
         write(*,*) 'Create UTMGrid3'
         write(10,*)'gdid3 = he5_gdcreate(gdfid3, "UTMGrid3",
     1xdim, ydim, uplft, lowrgt)'
         write(*,*) 'Value returned by gdcreate ', gdid3
      endif
      
      
    
      gdid_utm = he5_gdcreate(gdfid, 'UTMGrid', xdim, ydim, 
     1uplft, lowrgt)
      if (gdid_utm .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Creating UTMGrid'
         write(*,*)'Creating UTMGrid'
         write(*,*)'gdid_utm= he5_gdcreate(gdfid,"UTMGrid",xdim,
     1ydim,uplft,lowrgt)'
         write(10,*)'gdid_utm= he5_gdcreate(gdfid,"UTMGrid",xdim,
     1ydim,uplft,lowrgt)'
         write(10,*)'UTM grid id returned from gdcreate ',gdid_utm
         write(*,*)'UTM grid id returned from gdcreate ',gdid_utm
      endif 
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddefproj
c
      write(10,27)
      write(10,*)'Testing he5_gddefproj... '
      write(10,*)'========================'
      write(*,*)'Testing he5_gddefproj... '
      write(*,*)'========================'

      zonecode = 10
      spherecode = 0

      do 120 index1=1,16
         projparm(index1)=0
 120  continue
  
      status = he5_gddefproj(gdid_utm,HE5_GCTP_UTM,zonecode,
     1spherecode,projparm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Defining UTM projection'
         write(*,*)'Defining UTM projection'
         write(*,*)'status= he5_gddefproj(gdid_utm,HE5_GCTP_UTM,
     1zonecode,spherecode, projparm)'
         write(10,*)'status= he5_gddefproj(gdid_utm,HE5_GCTP_UTM,
     1zonecode,spherecode, projparm)'
         write(10,*)'Returned value from gddefproj ', status
         write(*,*)'Returned value from gddefproj ', status
      endif 
      write(10,*)' '
      write(*,*)' '

      status = he5_gddefproj(gdid3,HE5_GCTP_UTM,zonecode,
     1spherecode,projparm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Defining UTM projection'
         write(*,*)'Defining UTM projection'
         write(*,*)'status= he5_gddefproj(gdid3,HE5_GCTP_UTM,
     1zonecode,spherecode, projparm)'
         write(10,*)'status= he5_gddefproj(gdid3,HE5_GCTP_UTM,
     1zonecode,spherecode, projparm)'
         write(10,*)'Returned value from gddefproj ', status
         write(*,*)'Returned value from gddefproj ', status
      endif 
      write(10,*)' '
      write(*,*)' '

      status = he5_gddefproj(gdid2,HE5_GCTP_UTM,zonecode,
     1spherecode,projparm)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Defining UTM'
         write(10,*)'status = he5_gddefproj(gdid2,HE5_GCTP_UTM,
     1zonecode,spherecode,projparm)'
         write(10,*)'Value returned by gddefproj ', status
         write(*,*)'Defining UTM'
         write(*,*)'status = he5_gddefproj(gdid2,HE5_GCTP_UTM,
     1zonecode,spherecode,projparm)'
         write(*,*)'Value returned by gddefproj ', status
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gddefpreg
c
      write(10,27)
      write(10,*)'Testing he5_gddefpreg... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddefpreg... '
      write(*,*)'=========================='
      status = he5_gddefpreg(gdid_utm, HE5_HDFE_CORNER)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddefpreg(gdid_utm, 
     1HE5_HDFE_CORNER)'
         write(10,*)'status = he5_gddefpreg(gdid_utm, 
     1HE5_HDFE_CORNER)'
         write(10,*)'Returned from gddefpreg ', status
         write(*,*)'Returned from gddefpreg ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddefpreg(gdid3, HE5_HDFE_CORNER)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddefpreg(gdid3, 
     1HE5_HDFE_CORNER)'
         write(10,*)'status = he5_gddefpreg(gdid3, 
     1HE5_HDFE_CORNER)'
         write(10,*)'Returned from gddefpreg ', status
         write(*,*)'Returned from gddefpreg ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddeforigin
c
      write(10,27)
      write(10,*)'Testing he5_gddeforigin... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddeforigin... '
      write(*,*)'=========================='
      status = he5_gddeforigin(gdid_utm,HE5_HDFE_GD_UL)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddeforigin(gdid_utm,
     1HE5_HDFE_GD_UL)'
         write(*,*)'Value of status from gddeforigin ',status
         write(10,*)'status = he5_gddeforigin(gdid_utm,
     1HE5_HDFE_GD_UL)'
         write(10,*)'Value of status from gddeforigin ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddeforigin(gdid3,HE5_HDFE_GD_UL)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddeforigin(gdid3,
     1HE5_HDFE_GD_UL)'
         write(*,*)'Value of status from gddeforigin ',status
         write(10,*)'status = he5_gddeforigin(gdid3,
     1HE5_HDFE_GD_UL)'
         write(10,*)'Value of status from gddeforigin ',status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddefdim
c
      write(10,27)
      write(10,*)'Testing he5_gddefdim... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddefdim... '
      write(*,*)'=========================='
      testdim1 = 40
      status = he5_gddefdim(gdid2,'TestDim1',testdim1)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid2,"TestDim1",40)'
         write(10,*)'Value returned by gddefdim ', status
         write(*,*)'status = he5_gddefdim(gdid2,"TestDim1",40)'
         write(*,*)'Value returned by gddefdim ', status
      endif
      write(10,*)' '
      write(*,*)' '

      testdim2 = 25
      status = he5_gddefdim(gdid2, 'TestDim2', testdim2)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid2,"TestDim2",25)'
         write(10,*)'Value returned by gddefdim ', status
         write(*,*)'status = he5_gddefdim(gdid2,"TestDim2",25)'
         write(*,*)'Value returned by gddefdim ', status
      endif
      write(10,*)' '
      write(*,*)' '

      conduction = 250
      status = he5_gddefdim(gdid_utm, 'Conduction', conduction)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid_utm,
     1"Conduction",250)'
         write(10,*)'Returned value from gddefdim ', status
         write(*,*)'status = he5_gddefdim(gdid_utm, 
     1"Conduction", 250)'
         write(*,*)'Returned value from gddefdim ', status
      endif
      write(10,*)' '
      write(*,*)' '

      timedim = 4800
      status = he5_gddefdim(gdid_utm,'Timedim',timedim)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid_utm,
     1"Timedim",4800)'
         write(10,*)'Status returned by call to gddefdim ',status
         write(*,*)'status = he5_gddefdim(gdid_utm,
     1"Timedim",4800)'
         write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

      hghtdim = 80
      status = he5_gddefdim(gdid_utm,'Hghtdim',hghtdim)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gddefdim(gdid_utm,"Hghtdim",80)'
        write(10,*)'Status returned by call to gddefdim ',status
        write(*,*)'status = he5_gddefdim(gdid_utm,"Hghtdim",80)'
        write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

      convection = 60
      status = he5_gddefdim(gdid_utm,'Convection',convection)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define dimension Convection '
        write(10,*)'status = he5_gddefdim(gdid_utm,
     1"Convection",60)'
        write(10,*)'Status returned by call to gddefdim ',status
        write(*,*)'Define dimension Convection '
        write(*,*)'status = he5_gddefdim(gdid_utm,
     1"Convection",60)'
        write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

      conduction = 250
      status = he5_gddefdim(gdid3, 'Conduction', conduction)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid3,
     1"Conduction",250)'
         write(10,*)'Returned value from gddefdim ', status
         write(*,*)'status = he5_gddefdim(gdid3, 
     1"Conduction", 250)'
         write(*,*)'Returned value from gddefdim ', status
      endif
      write(10,*)' '
      write(*,*)' '

      timedim = 4800
      status = he5_gddefdim(gdid3,'Timedim',timedim)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddefdim(gdid3,
     1"Timedim",4800)'
         write(10,*)'Status returned by call to gddefdim ',status
         write(*,*)'status = he5_gddefdim(gdid3,
     1"Timedim",4800)'
         write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

      hghtdim = 80
      status = he5_gddefdim(gdid3,'Hghtdim',hghtdim)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gddefdim(gdid3,"Hghtdim",80)'
        write(10,*)'Status returned by call to gddefdim ',status
        write(*,*)'status = he5_gddefdim(gdid3,"Hghtdim",80)'
        write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

      convection = 60
      status = he5_gddefdim(gdid3,'Convection',convection)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define dimension Convection '
        write(10,*)'status = he5_gddefdim(gdid3,
     1"Convection",60)'
        write(10,*)'Status returned by call to gddefdim ',status
        write(*,*)'Define dimension Convection '
        write(*,*)'status = he5_gddefdim(gdid3,
     1"Convection",60)'
        write(*,*)'Status returned by call to gddefdim ',status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddetach
c
      write(10,27)
      write(10,*)'Testing he5_gddetach... '
      write(10,*)'======================='
      write(*,*)'Testing he5_gddetach... '
      write(*,*)'======================='
      status = he5_gddetach(gdid2)
      if(status .lt. zero) then
         write(*,231)
         write(10,231)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid2)'
         write(10,*)'Value returned by gddetach ', status
         write(*,*)'status = he5_gddetach(gdid2)'
         write(*,*)'Value returned by gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid_utm)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '
 
      status = he5_gddetach(gdid3)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid3)'
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '
 
c Test Case -- he5_gdattach
c
      write(10,27)
      write(10,*)'Testing he5_gdattach... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdattach... '
      write(*,*)'=========================='
      gdid_utm = he5_gdattach(gdfid, 'UTMGrid')
      if(gdid_utm .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid_utm=he5_gdattach(gdfid,"UTMGrid")'
         write(10,*)'Handle returned from gdattach ',gdid_utm
         write(*,*)'gdid_utm=he5_gdattach(gdfid,"UTMGrid")'
         write(*,*)'Handle returned from gdattach ',gdid_utm
      endif
      write(10,*)' '
      write(*,*)' '

      gdid2 = he5_gdattach(gdfid2, 'UTMGrid2')
      if(gdid2 .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid2=he5_gdattach(gdfid2, "UTMGrid2")'
         write(10,*)'Value returned by gdattach ', gdid2
         write(*,*)'gdid2=he5_gdattach(gdfid2, "UTMGrid2")'
         write(*,*)'Value returned by gdattach ', gdid2
      endif
      write(10,*)' '
      write(*,*)' '

      gdid3 = he5_gdattach(gdfid3, 'UTMGrid3')
      if(gdid3 .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid3=he5_gdattach(gdfid3, "UTMGrid3")'
         write(10,*)'Value returned by gdattach ', gdid3
         write(*,*)'gdid3=he5_gdattach(gdfid3, "UTMGrid3")'
         write(*,*)'Value returned by gdattach ', gdid3
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddeffld
c
      write(10,27)
      write(10,*)'Testing he5_gddeffld... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddeffld... '
      write(*,*)'=========================='

      status= he5_gddeffld(gdid2,"tempfield","TestDim2,TestDim1",
     1" ",HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)
      if(status .lt. zero) then
         write(*,231)
         write(10,231)
        
      else 
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define: "tempfield"'
        write(10,*)'status= he5_gddeffld(gdid2,"tempfield",
     1"TestDim2,TestDim1"," ",HE5T_NATIVE_FLOAT,HE5_HDFE_NOMERGE)'
        write(10,*)'Value returned by gddeffld ', status
        write(*,*)'Define: "tempfield"'
        write(*,*)'status= he5_gddeffld(gdid2,"tempfield",
     1"TestDim2,TestDim1","",HE5T_NATIVE_FLOAT,HE5_HDFE_NOMERGE)'
        write(*,*)'Value returned by gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,'Time','Timedim'," ",
     1HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
        write(10,*)' Define field Time'
        write(10,*)'status= he5_gddeffld(gdid_utm,"Time","Timedim",
     1"",HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)' Define field Time'
        write(*,*)'status= he5_gddeffld(gdid_utm,"Time","Timedim",
     1"",HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,'Height','Hghtdim'," ", 
     1HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
        write(10,*)' Define field Height'
        write(10,*)'status= he5_gddeffld(gdid_utm,"Height",
     1"Hghtdim","",HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)' Define field Height'
        write(*,*)'status= he5_gddeffld(gdid_utm,"Height","Hghtdim",
     1"",HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,'Time','Timedim'," ",
     1HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
        write(10,*)' Define field Time'
        write(10,*)'status= he5_gddeffld(gdid3,"Time","Timedim",
     1"",HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)' Define field Time'
        write(*,*)'status= he5_gddeffld(gdid3,"Time","Timedim",
     1"",HE5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,'Height','Hghtdim'," ", 
     1HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
        write(10,*)' Define field Height'
        write(10,*)'status= he5_gddeffld(gdid3,"Height",
     1"Hghtdim","",HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)' Define field Height'
        write(*,*)'status= he5_gddeffld(gdid3,"Height","Hghtdim",
     1"",HE5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gddeftle
c
      write(10,27)
      write(10,*)'Testing he5_gddeftle... '
      write(10,*)'======================='
      write(*,*)'Testing he5_gddeftle... '
      write(*,*)'======================='

      status = he5_gddeftle(gdid_utm,HE5_HDFE_TILE,2,tiledims)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define tiling dimensions '
        write(10,*)'status = he5_gddeftle(gdid_utm,
     1HE5_HDFE_TILE,2,tiledims)'
        write(10,*)'Status returned by call to gddeftle ',status
        write(*,*)'Status returned by call to gddeftle ',status
        write(*,*)'status = he5_gddeftle(gdid_utm,
     1HE5_HDFE_TILE,2,tiledims)'
        write(*,*)'Define tiling dimensions '
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,'SideB','YDim,Convection',
     1" ",HE5T_NATIVE_INT, HE5_HDFE_NOMERGE)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define field that will be tiled '
        write(10,*)'status= he5_gddeffld(gdid_utm,"SideB",
     1"YDim,Convection"," ",HE5T_NATIVE_INT,HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)'Define field that will be tiled '
        write(*,*)'status= he5_gddeffld(gdid_utm,"SideB",
     1"YDim,Convection","",HE5T_NATIVE_INT,HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid_utm)
      if (status .lt. zero) then 
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid_utm = he5_gdattach(gdfid, 'UTMGrid')
      if (gdid_utm .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gdattach(gdfid,"UTMGrid")'
         write(10,*)'Returned value from gdattach ', status
         write(*,*)'status = he5_gddetach(gdfid,"UTMGrid")'
         write(*,*)'Returned value from gdattach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,'Grounding','Conduction',
     1" ",HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Define: "Grounding"'                  
         write(10,*)'status= he5_gddeffld(gdid_utm,"Grounding",
     1"Conduction","",HE5T_NATIVE_FLOAT,HE5_HDF_NOMERGE)'
         write(10,*)'Value returned by gddeffld ', status
         write(*,*)'Define: "Grounding"'                  
         write(*,*)'status= he5_gddeffld(gdid_utm,"Grounding",
     1"Conduction","",HE5T_NATIVE_FLOAT,HE5_HDF_NOMERGE)'
         write(*,*)'Value returned by gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddeftle(gdid3,HE5_HDFE_TILE,2,tiledims)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define tiling dimensions '
        write(10,*)'status = he5_gddeftle(gdid3,
     1HE5_HDFE_TILE,2,tiledims)'
        write(10,*)'Status returned by call to gddeftle ',status
        write(*,*)'Status returned by call to gddeftle ',status
        write(*,*)'status = he5_gddeftle(gdid3,
     1HE5_HDFE_TILE,2,tiledims)'
        write(*,*)'Define tiling dimensions '
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,'SideB','YDim,Convection',
     1" ",HE5T_NATIVE_INT, HE5_HDFE_NOMERGE)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define field that will be tiled '
        write(10,*)'status= he5_gddeffld(gdid3,"SideB",
     1"YDim,Convection"," ",HE5T_NATIVE_INT,HE5_HDFE_NOMERGE)'
        write(10,*)'Status returned by call to gddeffld ',status
        write(*,*)'Define field that will be tiled '
        write(*,*)'status= he5_gddeffld(gdid3,"SideB",
     1"YDim,Convection","",HE5T_NATIVE_INT,HE5_HDFE_NOMERGE)'
        write(*,*)'Status returned by call to gddeffld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid3)
      if (status .lt. zero) then 
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid3)'
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid3 = he5_gdattach(gdfid3, 'UTMGrid3')
      if (gdid3 .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gdattach(gdfid3,"UTMGrid3")'
         write(10,*)'Returned value from gdattach ', status
         write(*,*)'status = he5_gddetach(gdfid3,"UTMGrid3")'
         write(*,*)'Returned value from gdattach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,'Grounding','Conduction',
     1" ",HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Define: "Grounding"'                  
         write(10,*)'status= he5_gddeffld(gdid3,"Grounding",
     1"Conduction","",HE5T_NATIVE_FLOAT,HE5_HDF_NOMERGE)'
         write(10,*)'Value returned by gddeffld ', status
         write(*,*)'Define: "Grounding"'                  
         write(*,*)'status= he5_gddeffld(gdid3,"Grounding",
     1"Conduction","",HE5T_NATIVE_FLOAT,HE5_HDF_NOMERGE)'
         write(*,*)'Value returned by gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddefcomp
c
      write(10,27)
      write(10,*)'Testing he5_gddefcomp... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddefcomp... '
      write(*,*)'=========================='

      tiledims(1) = 16
      tiledims(2) = 15
      status = he5_gddeftle(gdid_utm,HE5_HDFE_TILE,2,tiledims)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'Setup Tiling dimensions '
         write(*,*)'Required when using compression'
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddefcomp(gdid_utm,HE5_HDFE_COMP_DEFLATE, compparm)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Setup compression mode'
        write(10,*)'status = he5_gddefcomp(gdid_utm,
     1HE5_HDFE_COMP_DEFLATE, compparm)'
        write(10,*)'Status returned by call to gddefcomp ',status
        write(*,*)'Setup compression mode'
        write(*,*)'status = he5_gddefcomp(gdid_utm,
     1HE5_HDFE_COMP_DEFLATE, compparm)'
        write(*,*)'Status returned by call to gddefcomp ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,"Voltage","YDim,XDim"," ",
     1HE5T_NATIVE_FLOAT,0)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define: "Voltage"'
        write(10,*)'status = he5_gddeffld(gdid_utm,"Voltage",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(10,*)'Value returned from gddeffld ', status
        write(*,*)'Define: "Voltage"'
        write(*,*)'status = he5_gddeffld(gdid_utm,"Voltage",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddeftle(gdid3,HE5_HDFE_TILE,2,tiledims)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'Setup Tiling dimensions '
         write(*,*)'Required when using compression'
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddefcomp(gdid3,
     1HE5_HDFE_COMP_SHUF_SZIP_K13orEC, compparm)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Setup compression mode'
        write(10,*)'status = he5_gddefcomp(gdid3,
     1HE5_HDFE_COMP_SHUF_SZIP_K13orEC,compparm)'
        write(10,*)'Status returned by call to gddefcomp ',status
        write(*,*)'Setup compression mode'
        write(*,*)'status = he5_gddefcomp(gdid3,
     1HE5_HDFE_COMP_SHUF_SZIP_K13orEC,compparm)'
        write(*,*)'Status returned by call to gddefcomp ',status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,"Voltage","YDim,XDim"," ",
     1HE5T_NATIVE_FLOAT,0)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define: "Voltage"'
        write(10,*)'status = he5_gddeffld(gdid3,"Voltage",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(10,*)'Value returned from gddeffld ', status
        write(*,*)'Define: "Voltage"'
        write(*,*)'status = he5_gddeffld(gdid3,"Voltage",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdsetfill
c
      write(10,27)
      write(10,*)'Testing he5_gdsetfill... '
      write(10,*)'========================'
      write(*,*)'Testing he5_gdsetfill... '
      write(*,*)'========================='

      status = he5_gdsetfill(gdid_utm, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Testing he5_gdsetfill'
         write(10,*)'status = he5_gdsetfill(gdid_utm, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(10,*)'Returned from gdsetfill ', status
         write(*,*)'Testing he5_gdsetfill'
         write(*,*)'status = he5_gdsetfill(gdid_utm, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(*,*)'Returned from gdsetfill ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,"Drift","YDim,XDim"," ",
     1HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define: "Drift"'      
        write(10,*)'status= he5_gddeffld(gdid_utm,"Drift","YDim,XDim",
     1"",HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)'
        write(10,*)'Value returned from gddeffld ', status
        write(*,*)'Define: "Drift"'      
        write(*,*)'status= he5_gddeffld(gdid_utm,"Drift","YDim,XDim",
     1"",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid_utm,'Impedance','YDim,XDim'," ",
     1HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Define: "Impedance"'            
         write(10,*)'status= he5_gddeffld(gdid_utm,"Impedance",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
         write(10,*)'Value returned from gddeffld ', status
         write(*,*)'Define: "Impedance"'            
         write(*,*)'status= he5_gddeffld(gdid_utm,"Impedance",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
         write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '


      status = he5_gddetach(gdid_utm)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid_utm = he5_gdattach(gdfid, 'UTMGrid')
      if(gdid_utm .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(10,*)'Handle returned from gdattach ', gdid_utm
         write(*,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(*,*)'Handle returned from gdattach ',gdid_utm
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdsetfill(gdid3, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Testing he5_gdsetfill'
         write(10,*)'status = he5_gdsetfill(gdid3, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(10,*)'Returned from gdsetfill ', status
         write(*,*)'Testing he5_gdsetfill'
         write(*,*)'status = he5_gdsetfill(gdid3, "Drift", 
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(*,*)'Returned from gdsetfill ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,"Drift","YDim,XDim"," ",
     1HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Define: "Drift"'      
        write(10,*)'status= he5_gddeffld(gdid3,"Drift","YDim,XDim",
     1"",HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)'
        write(10,*)'Value returned from gddeffld ', status
        write(*,*)'Define: "Drift"'      
        write(*,*)'status= he5_gddeffld(gdid3,"Drift","YDim,XDim",
     1"",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
        write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status= he5_gddeffld(gdid3,'Impedance','YDim,XDim'," ",
     1HE5T_NATIVE_FLOAT, HE5_HDFE_AUTOMERGE)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'Define: "Impedance"'            
         write(10,*)'status= he5_gddeffld(gdid3,"Impedance",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
         write(10,*)'Value returned from gddeffld ', status
         write(*,*)'Define: "Impedance"'            
         write(*,*)'status= he5_gddeffld(gdid3,"Impedance",
     1"YDim,XDim","",HE5T_NATIVE_FLOAT,HE5_HDFE_AUTOMERGE)'
         write(*,*)'Value returned from gddeffld ', status
      endif
      write(10,*)' '
      write(*,*)' '


      status = he5_gddetach(gdid3)
      if(status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid3)'
         write(*,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Returned value from gddetach ', status
         write(*,*)'Returned value from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid3 = he5_gdattach(gdfid3, 'UTMGrid3')
      if(gdid3 .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(10,*)'Handle returned from gdattach ', gdid3
         write(*,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(*,*)'Handle returned from gdattach ',gdid3
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdwrfld
c
      write(10,27)
      write(10,*)'Testing he5_gdwrfld... '
      write(10,*)'======================'
      write(*,*)'Testing he5_gdwrfld... '
      write(*,*)'======================'

      start(1) = 0
      start(2) = 0
      stride(1) = 1
      stride(2) = 1
      edge(1) = 80
      edge(2) = 60
      
      status = he5_gdwrfld(gdid_utm,"Voltage",start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Voltage '
        write(10,*)'status= he5_gdwrfld(gdid_utm,"Voltage",start,
     1stride,edge,utmray)'
        write(10,*)'Status returned by gdwrfield ', status
        write(*,*)'Write to field Voltage '
        write(*,*)'status= he5_gdwrfld(gdid_utm,"Voltage",start,
     1stride,edge,utmray)'
        write(*,*)'Status returned by gdwrfield ', status
      endif
      write(10,*)' '
      write(*,*)' '


      start(1) = 10
      start(2) = 10
      edge(1) = 10
      edge(2) = 10
      status=he5_gdwrfld(gdid_utm,'Drift',start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Drift '
        write(10,*)'status=he5_gdwrfld(gdid_utm,"Drift",start,
     1stride,edge,utmray)'
        write(10,*)'Status returned from gdwrfld ', status
        write(*,*)'Write to field Drift '
        write(*,*)'status = he5_gdwrfld(gdid_utm,"Drift",start,
     1stride,edge,utmray)'
        write(*,*)'Status returned from gdwrfld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      start(1) = 0
      start(2) = 0
      edge(1) = 80
      edge(2) = 60
      status = he5_gdwrfld(gdid_utm, 'Impedance', start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Impedance '
        write(10,*)'status= he5_gdwrfld(gdid_utm,"Impedance",start,
     1stride,edge,utmray)'
        write(10,*)'Returned from gdwrfld ', status
        write(*,*)'Write to field Impedance '
        write(*,*)'status= he5_gdwrfld(gdid_utm,"Impedance",start,
     1stride,edge,utmray)'
        write(*,*)'Returned from gdwrfld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=4800
      status= he5_gdwrfld(gdid_utm,'Time',start1d,stride1d,
     1edge1d,utmtmeray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Time '
        write(10,*)'status= he5_gdwrfld(gdid_utm,"Time",start1d,
     1stride1d,edge1d,utmtmeray)'
        write(10,*)'Status returned by call to gdwrfld ',status
        write(*,*)'Write to field Time '
        write(*,*)'status= he5_gdwrfld(gdid_utm,"Time",start1d,stride1d,
     1edge1d,utmtmeray)'
        write(*,*)'Status returned by call to gdwrfld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=80
      status= he5_gdwrfld(gdid_utm,'Height',start1d,stride1d,
     1edge1d,utmhght)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Height '
        write(10,*)'status= he5_gdwrfld(gdid_utm,"Height",start1d,
     1stride1d,edge1d,utmhght)'
        write(10,*)'Status returned by call to gdwrfld ',status
        write(*,*)'Write to field Height '
        write(*,*)'status= he5_gdwrfld(gdid_utm,"Height",start1d,
     1stride1d,edge1d,utmhght)'
        write(*,*)'Status returned by call to gdwrfld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=250
      status= he5_gdwrfld(gdid_utm,'Grounding',start1d,
     1stride1d,edge1d,ray3)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Grounding '
        write(10,*)'status= he5_gdwrfld(gdid_utm,"Grounding",start1d,
     1stride1d,edge1d,ray3)'
        write(10,*)'Value returned by gdwrfld ', status
        write(*,*)'Write to field Grounding '
        write(*,*)'status= he5_gdwrfld(gdid_utm,"Grounding",start1d,
     1stride1d,edge1d,ray3)'
        write(*,*)'Value returned by gdwrfld ', status
      endif
        write(10,*)' '
        write(*,*)' '

      status = he5_gddetach(gdid_utm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Value of status returned by gddetach ',status
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'Value of status returned by gddetach ',status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid_utm = he5_gdattach(gdfid, 'UTMGrid')
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(10,*)'Grid id returned by gdattach ',gdid_utm
         write(*,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(*,*)'Grid id returned by gdattach ',gdid_utm
      endif
      write(10,*)' '
      write(*,*)' '

      start(1) = 0
      start(2) = 0
      stride(1) = 1
      stride(2) = 1
      edge(1) = 80
      edge(2) = 60
      
      status = he5_gdwrfld(gdid3,"Voltage",start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Voltage '
        write(10,*)'status= he5_gdwrfld(gdid3,"Voltage",start,
     1stride,edge,utmray)'
        write(10,*)'Status returned by gdwrfield ', status
        write(*,*)'Write to field Voltage '
        write(*,*)'status= he5_gdwrfld(gdid3,"Voltage",start,
     1stride,edge,utmray)'
        write(*,*)'Status returned by gdwrfield ', status
      endif
      write(10,*)' '
      write(*,*)' '


      start(1) = 10
      start(2) = 10
      edge(1) = 10
      edge(2) = 10
      status=he5_gdwrfld(gdid3,'Drift',start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Drift '
        write(10,*)'status=he5_gdwrfld(gdid3,"Drift",start,
     1stride,edge,utmray)'
        write(10,*)'Status returned from gdwrfld ', status
        write(*,*)'Write to field Drift '
        write(*,*)'status = he5_gdwrfld(gdid3,"Drift",start,
     1stride,edge,utmray)'
        write(*,*)'Status returned from gdwrfld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      start(1) = 0
      start(2) = 0
      edge(1) = 80
      edge(2) = 60
      status = he5_gdwrfld(gdid3, 'Impedance', start,stride,
     1edge,utmray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Impedance '
        write(10,*)'status= he5_gdwrfld(gdid3,"Impedance",start,
     1stride,edge,utmray)'
        write(10,*)'Returned from gdwrfld ', status
        write(*,*)'Write to field Impedance '
        write(*,*)'status= he5_gdwrfld(gdid3,"Impedance",start,
     1stride,edge,utmray)'
        write(*,*)'Returned from gdwrfld ', status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=4800
      status= he5_gdwrfld(gdid3,'Time',start1d,stride1d,
     1edge1d,utmtmeray)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Time '
        write(10,*)'status= he5_gdwrfld(gdid3,"Time",start1d,
     1stride1d,edge1d,utmtmeray)'
        write(10,*)'Status returned by call to gdwrfld ',status
        write(*,*)'Write to field Time '
        write(*,*)'status= he5_gdwrfld(gdid3,"Time",start1d,stride1d,
     1edge1d,utmtmeray)'
        write(*,*)'Status returned by call to gdwrfld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=80
      status= he5_gdwrfld(gdid3,'Height',start1d,stride1d,
     1edge1d,utmhght)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Height '
        write(10,*)'status= he5_gdwrfld(gdid3,"Height",start1d,
     1stride1d,edge1d,utmhght)'
        write(10,*)'Status returned by call to gdwrfld ',status
        write(*,*)'Write to field Height '
        write(*,*)'status= he5_gdwrfld(gdid3,"Height",start1d,
     1stride1d,edge1d,utmhght)'
        write(*,*)'Status returned by call to gdwrfld ',status
      endif
      write(10,*)' '
      write(*,*)' '

      start1d=0
      stride1d=1
      edge1d=250
      status= he5_gdwrfld(gdid3,'Grounding',start1d,
     1stride1d,edge1d,ray3)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'Write to field Grounding '
        write(10,*)'status= he5_gdwrfld(gdid3,"Grounding",start1d,
     1stride1d,edge1d,ray3)'
        write(10,*)'Value returned by gdwrfld ', status
        write(*,*)'Write to field Grounding '
        write(*,*)'status= he5_gdwrfld(gdid3,"Grounding",start1d,
     1stride1d,edge1d,ray3)'
        write(*,*)'Value returned by gdwrfld ', status
      endif
        write(10,*)' '
        write(*,*)' '

      status = he5_gddetach(gdid3)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Value of status returned by gddetach ',status
         write(*,*)'status = he5_gddetach(gdid3)'
         write(*,*)'Value of status returned by gddetach ',status
      endif
      write(10,*)' '
      write(*,*)' '

      gdid3 = he5_gdattach(gdfid3, 'UTMGrid3')
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(10,*)'Grid id returned by gdattach ',gdid3
         write(*,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(*,*)'Grid id returned by gdattach ',gdid3
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdwrattr
c
      write(10,27)
      write(10,*)'Testing he5_gdwrattr... Global attribute '
      write(10,*)'==================================== '
      write(*,*)'Testing he5_gdwrattr... Global attribute '
      write(*,*)'==================================== '

      setcount(1) = 4
      status = he5_gdwrattr(gdid_utm, 'Resistance', HE5T_NATIVE_INT, 
     1setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrattr(gdid_utm,"Resistance",
     1HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrattr ', status
         write(*,*)'status= he5_gdwrattr(gdid_utm,"Resistance",
     1HE5T_NATIVE_FLOAT, 4, attr)'
         write(*,*)'Returned from gdwrattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

      setcount(1) = 4
      status = he5_gdwrattr(gdid3, 'Resistance', HE5T_NATIVE_INT, 
     1setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrattr(gdid3,"Resistance",
     1HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrattr ', status
         write(*,*)'status= he5_gdwrattr(gdid3,"Resistance",
     1HE5T_NATIVE_FLOAT, 4, attr)'
         write(*,*)'Returned from gdwrattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdwrgattr
c
      write(10,27)
      write(10,*)'Testing he5_gdwrgattr... Group attribute '
      write(10,*)'==================================== '
      write(*,*)'Testing he5_gdwrgattr... Group attribute '
      write(*,*)'==================================== '

      attr(1) = 22
      attr(2) = 44
      attr(3) = 66
      attr(4) = 88
      status = he5_gdwrgattr(gdid_utm, 'group', HE5T_NATIVE_INT, 
     1setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrgattr(gdid_utm,"group",
     1HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrgattr ', status
         write(*,*)'status= he5_gdwrgattr(gdid_utm,"group",
     1HE5T_NATIVE_INT, 4, attr)'
         write(*,*)'Returned from gdwrgattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdwrgattr(gdid3, 'group', HE5T_NATIVE_INT, 
     1setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrgattr(gdid3,"group",
     1HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrgattr ', status
         write(*,*)'status= he5_gdwrgattr(gdid3,"group",
     1HE5T_NATIVE_INT, 4, attr)'
         write(*,*)'Returned from gdwrgattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdwrlattr
c
      write(10,27)
      write(10,*)'Testing he5_gdwrlattr... Local attribute '
      write(10,*)'======================================== '
      write(*,*)'Testing he5_gdwrlattr... Local attribute '
      write(*,*)'======================================== '

      attr(1) = 12
      attr(2) = 34
      attr(3) = 56
      attr(4) = 78
      status = he5_gdwrlattr(gdid_utm,"Grounding", "local", 
     1HE5T_NATIVE_INT,setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrlattr(gdid_utm,"Grounding",
     1"local",HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrlattr ', status
         write(*,*)'status= he5_gdwrlattr(gdid_utm,"Grounding",
     1"local",HE5T_NATIVE_INT, 4, attr)'
         write(*,*)'Returned from gdwrlattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid_utm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Returned from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'Returned from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdwrlattr(gdid3,"Grounding", "local", 
     1HE5T_NATIVE_INT,setcount, attr)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status= he5_gdwrlattr(gdid3,"Grounding",
     1"local",HE5T_NATIVE_INT, 4, attr)'
         write(10,*)'Returned from gdwrlattr ', status
         write(*,*)'status= he5_gdwrlattr(gdid3,"Grounding",
     1"local",HE5T_NATIVE_INT, 4, attr)'
         write(*,*)'Returned from gdwrlattr ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid3)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Returned from gddetach ', status
         write(*,*)'status = he5_gddetach(gdid3)'
         write(*,*)'Returned from gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '


      status = he5_gddetach(gdid2)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gddetach(gdid2)'
         write(10,*)'Value returned by gddetach ', status
         write(*,*)'status = he5_gddetach(gdid2)'
         write(*,*)'Value returned by gddetach ', status
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdclose
c
      write(10,27)
      write(10,*)'Testing he5_gdclose... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdclose... '
      write(*,*)'=========================='

      status = he5_gdclose(gdfid2)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gdclose(gdfid2)'
         write(10,*)'Value returned by gdclose ', status
         write(*,*)'status = he5_gdclose(gdfid2)'
         write(*,*)'Value returned by gdclose ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdclose(gdfid3)
      if (status .lt. zero) then
         write(*,231)
         write(10,231)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gdclose(gdfid3)'
         write(10,*)'Value returned by gdclose ', status
         write(*,*)'status = he5_gdclose(gdfid3)'
         write(*,*)'Value returned by gdclose ', status
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdclose(gdfid)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(10,*)'status = he5_gdclose(gdfid)'
         write(10,*)'Returned from gdclose ', status
         write(*,*)'status = he5_gdclose(gdfid)'
         write(*,*)'Returned from gdclose ', status
      endif
      write(10,*)' '
      write(*,*)' '

      
      gdfid = he5_gdopen('gridszip64.he5',HE5F_ACC_RDWR)
      if (gdfid .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'gdfid = he5_gdopen("gridszip64.he5",HE5F_ACC_RDWR)'
         write(10,*)'gdfid = he5_gdopen("gridszip64.he5",HE5F_ACC_RDWR)'
         write(*,*)'Grid file id returned by gdopen ',gdfid
         write(10,*)'Grid file id returned by gdopen ',gdfid
      endif
      write(10,*)' '
      write(*,*)' '

      gdid_utm = he5_gdattach(gdfid, 'UTMGrid')
      if (gdid_utm .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(10,*)'gdid_utm = he5_gdattach(gdfid, "UTMGrid")'
         write(*,*)'Grid id returned by gdattach ',gdid_utm
         write(10,*)'Grid id returned by gdattach ',gdid_utm
      endif
      write(10,*)' '
      write(*,*)' '

      gdfid3 = he5_gdopen('grid3szip64.he5',HE5F_ACC_RDWR)
      if (gdfid3 .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'gdfid3 = he5_gdopen(
     1"grid3szip64.he5",HE5F_ACC_RDWR)'
         write(10,*)'gdfid3 = he5_gdopen(
     1"grid3szip64.he5",HE5F_ACC_RDWR)'
         write(*,*)'Grid file id returned by gdopen ',gdfid3
         write(10,*)'Grid file id returned by gdopen ',gdfid3
      endif
      write(10,*)' '
      write(*,*)' '


      gdid3 = he5_gdattach(gdfid3, 'UTMGrid3')
      if (gdid3 .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(10,*)'gdid3 = he5_gdattach(gdfid3, "UTMGrid3")'
         write(*,*)'Grid id returned by gdattach ',gdid3
         write(10,*)'Grid id returned by gdattach ',gdid3
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdinqgrid
c
      write(10,27)
      write(10,*)'Testing he5_gdinqgrid... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdinqgrid... '
      write(*,*)'=========================='

      lstatus = he5_gdinqgrid('gridszip64.he5',grdnames,strbufsize)
      if (lstatus .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus = he5_gdinqgrid("gridszip64.he5",grdnames,
     1strbufsize)'
         write(*,*)'Names of grids ',grdnames
         write(*,*)'Number of grids ',lstatus
         write(*,*)'Size of names of grids ',strbufsize
         write(10,*)'lstatus = he5_gdinqgrid("gridszip64.he5",grdnames,
     1strbufsize)'
         write(10,*)'Names of grids ',grdnames
         write(10,*)'Number of grids ',lstatus
         write(10,*)'Size of names of grids ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

      lstatus = he5_gdinqgrid('grid3szip64.he5',grdnames,strbufsize)
      if (lstatus .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus = he5_gdinqgrid("grid3szip64.he5",grdnames,
     1strbufsize)'
         write(*,*)'Names of grids ',grdnames
         write(*,*)'Number of grids ',lstatus
         write(*,*)'Size of names of grids ',strbufsize
         write(10,*)'lstatus = he5_gdinqgrid("grid3szip64.he5",grdnames,
     1strbufsize)'
         write(10,*)'Names of grids ',grdnames
         write(10,*)'Number of grids ',lstatus
         write(10,*)'Size of names of grids ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdorginfo
c
      write(10,27)
      write(10,*)'Testing he5_gdorginfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdorginfo... '
      write(*,*)'=========================='

      status = he5_gdorginfo(gdid_utm, orgcode)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus = he5_gdorginfo(gdid_utm, orgcode)'
         write(10,*)'lstatus = he5_gdorginfo(gdid_utm, orgcode)'
         write(*,*)'Value returned by gdorginfo ',status
         write(10,*)'Value returned by gdorginfo ',status
         write(*,*)'Origin code value ',orgcode
         write(10,*)'Origin code value ',orgcode
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdorginfo(gdid3, orgcode)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus = he5_gdorginfo(gdid3, orgcode)'
         write(10,*)'lstatus = he5_gdorginfo(gdid3, orgcode)'
         write(*,*)'Value returned by gdorginfo ',status
         write(10,*)'Value returned by gdorginfo ',status
         write(*,*)'Origin code value ',orgcode
         write(10,*)'Origin code value ',orgcode
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdpreginfo
c
      write(10,27)
      write(10,*)'Testing he5_gdpreginfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdpreginfo... '
      write(*,*)'=========================='

      status = he5_gdpreginfo(gdid_utm, pixregcode)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdpreginfo(gdid_utm, pixregcode)'
         write(10,*)'status = he5_gdpreginfo(gdid_utm, pixregcode)'
         write(*,*)'Value returned by gdpreginfo ',status
         write(10,*)'Value returned by gdpreginfo ',status
         write(*,*)'Pixel reg code ',pixregcode
         write(10,*)'Pixel reg code ',pixregcode
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdpreginfo(gdid3, pixregcode)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdpreginfo(gdid3, pixregcode)'
         write(10,*)'status = he5_gdpreginfo(gdid3, pixregcode)'
         write(*,*)'Value returned by gdpreginfo ',status
         write(10,*)'Value returned by gdpreginfo ',status
         write(*,*)'Pixel reg code ',pixregcode
         write(10,*)'Pixel reg code ',pixregcode
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdinqdims
c
      write(10,27)
      write(10,*)'Testing he5_gdinqdims... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdinqdims... '
      write(*,*)'=========================='

      status = he5_gdinqdims(gdid_utm, dimname, dims)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdinqdims(gdid_utm, dimname, dims)'
         write(*,*)'Number of dimensions ',status
         write(*,*)'Dimensions in grid UTM ',dimname
         write(*,*)'Size of dimensions '
         write(10,*)'status = he5_gdinqdims(gdid_utm, dimname, dims)'
         write(10,*)'Number of dimensions ',status
         write(10,*)'Dimensions in grid UTM ',dimname
         write(10,*)'Size of dimensions '
         do 130 i=1,status
            write(*,*)' ',dims(i)
            write(10,*)' ',dims(i)
 130     continue
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdinqdims(gdid3, dimname, dims)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdinqdims(gdid3, dimname, dims)'
         write(*,*)'Number of dimensions ',status
         write(*,*)'Dimensions in grid UTM ',dimname
         write(*,*)'Size of dimensions '
         write(10,*)'status = he5_gdinqdims(gdid3, dimname, dims)'
         write(10,*)'Number of dimensions ',status
         write(10,*)'Dimensions in grid UTM ',dimname
         write(10,*)'Size of dimensions '
         do 131 i=1,status
            write(*,*)' ',dims(i)
            write(10,*)' ',dims(i)
 131     continue
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddiminfo
c
      write(10,27)
      write(10,*)'Testing he5_gddiminfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddiminfo... '
      write(*,*)'=========================='

      lstatus = he5_gddiminfo(gdid_utm, 'Conduction')
      if (lstatus .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus=he5_gddiminfo(gdid_utm,"Conduction")'
         write(*,*)'Size of dimension Conduction ',lstatus
         write(10,*)'lstatus=he5_gddiminfo(gdid_utm,"Conduction")'
         write(10,*)'Size of dimension Conduction ',lstatus
      endif
      write(10,*)' '
      write(*,*)' '

      lstatus = he5_gddiminfo(gdid3, 'Conduction')
      if (lstatus .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'lstatus=he5_gddiminfo(gdid3,"Conduction")'
         write(*,*)'Size of dimension Conduction ',lstatus
         write(10,*)'lstatus=he5_gddiminfo(gdid3,"Conduction")'
         write(10,*)'Size of dimension Conduction ',lstatus
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdgridinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdgridinfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdgridinfo... '
      write(*,*)'=========================='

      status= he5_gdgridinfo(gdid_utm,xdimsize,ydimsize,
     1upleftpt,lowrtpt)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status= he5_gdgridinfo(gdid_utm,xdimsize,
     1ydimsize,upleftpt,lowrtpt)'
         write(*,*)'Status returned by gdgridinfo ',status
         write(*,11)'X dim size ',xdimsize, ' Y dim size ',ydimsize
         write(*,12)'Upper left pt ',upleftpt(1),' ',upleftpt(2)
         write(*,12)'Lower right pt ',lowrtpt(1),' ',lowrtpt(2)
         write(10,*)'status= he5_gdgridinfo(gdid_utm,xdimsize,
     1ydimsize,upleftpt,lowrtpt)'
         write(10,*)'Value returned by gdgridinfo ',status
         write(10,*)'X dim size ',xdimsize, ' Y dim size ',ydimsize
         write(10,*)'Upper left pt ',upleftpt(1),upleftpt(2)
         write(10,*)'Lower right pt ',lowrtpt(1),lowrtpt(2)
      endif
      write(10,*)' '
      write(*,*)' '
   11 format(a,i2,a,i2)
   12 format(a,f14.5,a,f14.5)

      status= he5_gdgridinfo(gdid3,xdimsize,ydimsize,
     1upleftpt,lowrtpt)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status= he5_gdgridinfo(gdid3,xdimsize,
     1ydimsize,upleftpt,lowrtpt)'
         write(*,*)'Status returned by gdgridinfo ',status
         write(*,11)'X dim size ',xdimsize, ' Y dim size ',ydimsize
         write(*,12)'Upper left pt ',upleftpt(1),' ',upleftpt(2)
         write(*,12)'Lower right pt ',lowrtpt(1),' ',lowrtpt(2)
         write(10,*)'status= he5_gdgridinfo(gdid3,xdimsize,
     1ydimsize,upleftpt,lowrtpt)'
         write(10,*)'Value returned by gdgridinfo ',status
         write(10,*)'X dim size ',xdimsize, ' Y dim size ',ydimsize
         write(10,*)'Upper left pt ',upleftpt(1),upleftpt(2)
         write(10,*)'Lower right pt ',lowrtpt(1),lowrtpt(2)
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdprojinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdprojinfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdprojinfo... '
      write(*,*)'=========================='

      status = he5_gdprojinfo(gdid_utm,pcode,zcode,scode,pparm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdprojinfo(gdid_utm,pcode,
     1zcode,scode,pparm)'
         write(*,*)'Status returned by gdprojinfo ',status
         write(*,*)'Projection code ',pcode, ' Zone code ',zcode
         write(*,*)'Sphere code ',scode
         write(10,*)'status = he5_gdprojinfo(gdid_utm,pcode,
     1zcode,scode,pparm)'
         write(10,*)'Value returned by gdprojinfo ',status
         write(10,*)'Projection code ',pcode, ' Zone code ',zcode
         write(10,*)'Sphere code ',scode
         do 140 i=1,16
            write(*,55)' ',pparm(i)
            write(10,55)' ',pparm(i)
 140     continue
      endif
   55 format(a,f5.2)
      write(10,*)' '
      write(*,*)' '

      status = he5_gdprojinfo(gdid3,pcode,zcode,scode,pparm)
      if (status .lt. zero) then
         write(*,230)
         write(10,230)
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdprojinfo(gdid3,pcode,
     1zcode,scode,pparm)'
         write(*,*)'Status returned by gdprojinfo ',status
         write(*,*)'Projection code ',pcode, ' Zone code ',zcode
         write(*,*)'Sphere code ',scode
         write(10,*)'status = he5_gdprojinfo(gdid3,pcode,
     1zcode,scode,pparm)'
         write(10,*)'Value returned by gdprojinfo ',status
         write(10,*)'Projection code ',pcode, ' Zone code ',zcode
         write(10,*)'Sphere code ',scode
         do 141 i=1,16
            write(*,55)' ',pparm(i)
            write(10,55)' ',pparm(i)
 141     continue
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdcompinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdcompinfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdcompinfo... '
      write(*,*)'=========================='

      status = he5_gdcompinfo(gdid_utm,'Voltage',
     1compcodestring,compparm)
      if (status .lt. zero) then
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gdcompinfo(gdid_utm,"Voltage",
     1compcode,compparm)'
        write(10,*)'Compression info for field Voltage'
        write(10,*)'Comp code ',compcodestring
        write(10,*)'Compression parameters ',compparm
        write(*,*)'status = he5_gdcompinfo(gdid_utm,"Voltage",
     1compcode,compparm)'
        write(*,*)'Compression info for field Voltage'
        write(*,*)'Comp code ',compcodestring
        write(*,*)'Compression parameters ',compparm
      endif

      status = he5_gdcompinfo(gdid3,'Voltage',
     1compcodestring,compparm)
      if (status .lt. zero) then
         
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gdcompinfo(gdid3,"Voltage",
     1compcode,compparm)'
        write(10,*)'Compression info for field Voltage'
        write(10,*)'Comp code ',compcodestring
        write(10,*)'Compression parameters ',compparm
        write(*,*)'status = he5_gdcompinfo(gdid3,"Voltage",
     1compcode,compparm)'
        write(*,*)'Compression info for field Voltage'
        write(*,*)'Comp code ',compcodestring
        write(*,*)'Compression parameters ',compparm
      endif

c Test Case -- he5_gdinqflds
c
      write(10,27)
      write(10,*)'Testing he5_gdinqflds... '
      write(10,*)'========================'
      write(*,*)'Testing he5_gdinqflds... '
      write(*,*)'========================'

      nflds = he5_gdinqflds(gdid_utm,fldlist,rnk,numtype)
      if (nflds .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'nflds = he5_gdinqflds(gdid_utm,
     1fldlist,rnk,numtype)'
         write(*,*)'Number of fields ',nflds
         write(10,*)'nflds = he5_gdinqflds(gdid_utm,
     1fldlist,rnk,numtype)'
         write(10,*)'Number of fields ',nflds
         if (nflds .ne. 0) then
            write(*,*)'Fields ',fldlist
            write(10,*)'Fields ',fldlist
            do 150 i=1,nflds
               write(*,*)'Rank and type ',rnk(i), ' ',numtype(i)
               write(10,*)'Rank and type ',rnk(i), ' ',numtype(i)
  150       continue
         endif
      endif
      write(10,*)' '
      write(*,*)' '

      nflds = he5_gdinqflds(gdid3,fldlist,rnk,numtype)
      if (nflds .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'nflds = he5_gdinqflds(gdid3,
     1fldlist,rnk,numtype)'
         write(*,*)'Number of fields ',nflds
         write(10,*)'nflds = he5_gdinqflds(gdid3,
     1fldlist,rnk,numtype)'
         write(10,*)'Number of fields ',nflds
         if (nflds .ne. 0) then
            write(*,*)'Fields ',fldlist
            write(10,*)'Fields ',fldlist
            do 151 i=1,nflds
               write(*,*)'Rank and type ',rnk(i), ' ',numtype(i)
               write(10,*)'Rank and type ',rnk(i), ' ',numtype(i)
  151       continue
         endif
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdfldinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdfldinfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdfldinfo... '
      write(*,*)'=========================='

      status = he5_gdfldinfo(gdid_utm, 'Drift', rnk, dims, 
     1numtype, dimname, maxdimname)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdfldinfo(gdid_utm, "Drift", 
     1rnk, dims, numtype, dimname, maxdimname)'
         write(*,*)'Value returned by gdfldinfo for field Drift ',
     1status
         write(*,*)'Drift rank ',rnk(1)
         write(*,*)'Dimensions  ',dims(1),' ',dims(2)
         write(*,*)'Number type of field ',numtype(1)
         write(*,*)'Dimension names ',dimname
         write(10,*)'status = he5_gdfldinfo(gdid_utm, "Drift",
     1rnk, dims, numtype, dimname, maxdimname)'
         write(10,*)'Value returned by gdfldinfo for field Drift ',
     1status
         write(10,*)'Drift rank ',rnk(1)
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdfldinfo(gdid3, 'Drift', rnk, dims, 
     1numtype, dimname, maxdimname)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdfldinfo(gdid3, "Drift", 
     1rnk, dims, numtype, dimname, maxdimname)'
         write(*,*)'Value returned by gdfldinfo for field Drift ',
     1status
         write(*,*)'Drift rank ',rnk(1)
         write(*,*)'Dimensions  ',dims(1),' ',dims(2)
         write(*,*)'Number type of field ',numtype(1)
         write(*,*)'Dimension names ',dimname
         write(10,*)'status = he5_gdfldinfo(gdid3, "Drift",
     1rnk, dims, numtype, dimname, maxdimname)'
         write(10,*)'Value returned by gdfldinfo for field Drift ',
     1status
         write(10,*)'Drift rank ',rnk(1)
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdrdfld
c
      write(10,27)
      write(10,*)'Testing he5_gdrdfld... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdrdfld... '
      write(*,*)'=========================='

      start(1) = 0
      start(2) = 0
      stride(1) = 1
      stride(2) = 1
      edge(1) = 80
      edge(2) = 60
      status = he5_gdrdfld(gdid_utm,'Drift',start,stride,edge,tempray)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdrdfld(gdid_utm,"Drift",
     1start,stride,edge,tempray)'
         write(*,*)'Value returned by gdrdfld ',status
         write(10,*)'status = he5_gdrdfld(gdid_utm,"Drift",
     1start,stride,edge,tempray)'
         write(10,*)'Value returned by gdrdfld ',status
         do 65 ii=1,6
           do 66 i=1,5
              write(10,*)'Field value at ',ii,' ',i,'  ',
     1tempray(ii,i)
              write(*,*)'Field value at ',ii,' ',i,'  ',
     1tempray(ii,i)
   66      continue
   65   continue
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdrdfld(gdid3,'Drift',start,stride,edge,tempray)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdrdfld(gdid3,"Drift",
     1start,stride,edge,tempray)'
         write(*,*)'Value returned by gdrdfld ',status
         write(10,*)'status = he5_gdrdfld(gdid3,"Drift",
     1start,stride,edge,tempray)'
         write(10,*)'Value returned by gdrdfld ',status
         do 75 ii=1,6
           do 76 i=1,5
              write(10,*)'Field value at ',ii,' ',i,'  ',
     1tempray(ii,i)
              write(*,*)'Field value at ',ii,' ',i,'  ',
     1tempray(ii,i)
   76      continue
   75   continue
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdnentries
c
      write(10,27)
      write(10,*)'Testing he5_gdnentries... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdnentries... '
      write(*,*)'=========================='

      n=he5_gdnentries(gdid_utm,HE5_HDFE_NENTDIM,strbufsize)
      if (n .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'n = he5_gdnentries(gdid_utm, 
     1HE5_HDFE_NENTDIM, strbufsize)'
         write(*,*)'Number of dimensions ', n
         write(*,*)'Size of Dimension string ',strbufsize
         write(10,*)'n = he5_gdnentries(gdid_utm, 
     1HE5_HDFE_NENTDIM, strbufsize)'
         write(10,*)'Number of dimensions ', n
         write(10,*)'Size of Dimension string ',strbufsize
      endif

      n = he5_gdnentries(gdid_utm,HE5_HDFE_NENTDFLD,strbufsize)
      if (n .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'n = he5_gdnentries(gdid_utm,
     1HE5_HDFE_NENTDFLD,strbufsize)'
         write(*,*)'Number of data fields ',n
         write(*,*)'Size of Field string ',strbufsize
         write(10,*)'n = he5_gdnentries(gdid_utm,
     1HDFE_NENTDFLD,strbufsize)'
         write(10,*)'Number of data fields ',n
         write(10,*)'Size of Field string ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

      n=he5_gdnentries(gdid3,HE5_HDFE_NENTDIM,strbufsize)
      if (n .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'n = he5_gdnentries(gdid3, 
     1HE5_HDFE_NENTDIM, strbufsize)'
         write(*,*)'Number of dimensions ', n
         write(*,*)'Size of Dimension string ',strbufsize
         write(10,*)'n = he5_gdnentries(gdid3, 
     1HE5_HDFE_NENTDIM, strbufsize)'
         write(10,*)'Number of dimensions ', n
         write(10,*)'Size of Dimension string ',strbufsize
      endif

      n = he5_gdnentries(gdid3,HE5_HDFE_NENTDFLD,strbufsize)
      if (n .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'n = he5_gdnentries(gdid3,
     1HE5_HDFE_NENTDFLD,strbufsize)'
         write(*,*)'Number of data fields ',n
         write(*,*)'Size of Field string ',strbufsize
         write(10,*)'n = he5_gdnentries(gdid3,
     1HDFE_NENTDFLD,strbufsize)'
         write(10,*)'Number of data fields ',n
         write(10,*)'Size of Field string ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdattrinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdattrinfo, Global attribute'
      write(10,*)'============================================'
      write(*,*)'Testing he5_gdattrinfo, Global attribute'
      write(*,*)'============================================'

      status = he5_gdattrinfo(gdid_utm,'Resistance',ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdattrinfo(gdid_utm,"Resistance",
     1ntype,count)'
         write(*,*)'Value returned by gdattrinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdattrinfo(gdid_utm,"Resistance",
     1ntype,count)'
         write(10,*)'Value returned by gdattrinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdattrinfo(gdid3,'Resistance',ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdattrinfo(gdid3,"Resistance",
     1ntype,count)'
         write(*,*)'Value returned by gdattrinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdattrinfo(gdid3,"Resistance",
     1ntype,count)'
         write(10,*)'Value returned by gdattrinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdgatinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdgatinfo, Group attribute'
      write(10,*)'============================================'
      write(*,*)'Testing he5_gdgatinfo, Group attribute'
      write(*,*)'============================================'

      status = he5_gdgatinfo(gdid_utm,'group',ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdgatinfo(gdid_utm,"group",
     1ntype,count)'
         write(*,*)'Value returned by gdgatinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdgatinfo(gdid_utm,"group",
     1ntype,count)'
         write(10,*)'Value returned by gdgatinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdgatinfo(gdid3,'group',ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdgatinfo(gdid3,"group",
     1ntype,count)'
         write(*,*)'Value returned by gdgatinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdgatinfo(gdid3,"group",
     1ntype,count)'
         write(10,*)'Value returned by gdgatinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdlatinfo
c
      write(10,27)
      write(10,*)'Testing he5_gdlatinfo, Local attribute'
      write(10,*)'======================================'
      write(*,*)'Testing he5_gdlatinfo, Local attribute'
      write(*,*)'======================================'

      status = he5_gdlatinfo(gdid_utm,"Grounding","local",
     1ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdlatinfo(gdid_utm,"Grounding",
     1"local",ntype,count)'
         write(*,*)'Value returned by gdlatinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdlatinfo(gdid_utm,"Grounding",
     1"local",ntype,count)'
         write(10,*)'Value returned by gdgatinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdlatinfo(gdid3,"Grounding","local",
     1ntype,count)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdlatinfo(gdid3,"Grounding",
     1"local",ntype,count)'
         write(*,*)'Value returned by gdlatinfo ',status
         write(*,*)'Number type of attribute ',ntype
         write(*,*)'Number of total bytes in attribute ',count
         write(10,*)'status = he5_gdlatinfo(gdid3,"Grounding",
     1"local",ntype,count)'
         write(10,*)'Value returned by gdgatinfo ',status
         write(10,*)'Number type of attribute ',ntype
         write(10,*)'Number of total bytes in attribute ',count
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdrdattr
c
      write(10,27)
      write(10,*)'Testing he5_gdrdattr, Global attribute'
      write(10,*)'======================================'
      write(*,*)'Testing he5_gdrdattr, Global attribute'
      write(*,*)'======================================'

      status = he5_gdrdattr(gdid_utm,'Resistance',iattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdattr(gdid_utm,"Resistance",
     1iattr)'
        write(*,*)'Value returned by gdrdattr ',status
        write(10,*)'status = he5_gdrdattr(gdid_utm,"Resistance",
     1iattr)'
        write(10,*)'Value returned by gdrdattr ',status
        do 67 i=1,count
           write(10,*)'Attribute value  ',iattr(i)
           write(*,*)'Attribute value   ',iattr(i)
   67   continue
        
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdrdattr(gdid3,'Resistance',iattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdattr(gdid3,"Resistance",
     1iattr)'
        write(*,*)'Value returned by gdrdattr ',status
        write(10,*)'status = he5_gdrdattr(gdid3,"Resistance",
     1iattr)'
        write(10,*)'Value returned by gdrdattr ',status
        do 77 i=1,count
           write(10,*)'Attribute value  ',iattr(i)
           write(*,*)'Attribute value   ',iattr(i)
   77   continue
        
      endif
      write(10,*)' '
      write(*,*)' '
  
c Test Case -- he5_gdrdgattr
c
      write(10,27)
      write(10,*)'Testing he5_gdrdgattr, Group attribute '
      write(10,*)'======================================'
      write(*,*)'Testing he5_gdrdgattr, Group attribute'
      write(*,*)'======================================'

      status = he5_gdrdgattr(gdid_utm,'group',gattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdgattr(gdid_utm,"group",
     1gattr)'
        write(*,*)'Value returned by gdrdgattr ',status
        write(10,*)'status = he5_gdrdgattr(gdid_utm,"group",
     1gattr)'
        write(10,*)'Value returned by gdrdgattr ',status
        do 68 i=1,count
           write(10,*)'Attribute value  ',gattr(i)
           write(*,*)'Attribute value   ',gattr(i)
   68   continue
        
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdrdgattr(gdid3,'group',gattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdgattr(gdid3,"group",
     1gattr)'
        write(*,*)'Value returned by gdrdgattr ',status
        write(10,*)'status = he5_gdrdgattr(gdid3,"group",
     1gattr)'
        write(10,*)'Value returned by gdrdgattr ',status
        do 78 i=1,count
           write(10,*)'Attribute value  ',gattr(i)
           write(*,*)'Attribute value   ',gattr(i)
   78   continue
        
      endif
      write(10,*)' '
      write(*,*)' '
  
c Test Case -- he5_gdinqdatatype
c
      write(10,27)
      write(10,*)'Testing he5_gdinqdatatype '
      write(10,*)'============================='
      write(*,*)'Testing he5_gdinqdatatype'
      write(*,*)'============================='

      status = he5_gdinqdatatype(gdid_utm,"Grounding","local",
     1HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
         write(*,*)'status = he5_gdinqdatatype(gdid_utm,"Grounding",
     1"local",HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)'
         write(*,*)'Data type of attribute ',dtype
         write(*,*)'Class id of attribute ',classid
         write(*,*)'Order of attribute ',order
         write(*,*)'Size of attribute ',size
         write(10,*)'status = he5_gdinqdatatype(gdid_utm,"Grounding",
     1"local", HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)'
         write(10,*)'Data type of attribute ',dtype
         write(10,*)'Class id of attribute ',classid
         write(10,*)'Order of attribute ',order
         write(10,*)'Size of attribute ',size
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdinqdatatype(gdid3,"Grounding","local",
     1HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
        write(*,*)' '
        write(*,240)
        write(10,*)' '
        write(10,240)
         write(*,*)'status = he5_gdinqdatatype(gdid3,"Grounding",
     1"local",HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)'
         write(*,*)'Data type of attribute ',dtype
         write(*,*)'Class id of attribute ',classid
         write(*,*)'Order of attribute ',order
         write(*,*)'Size of attribute ',size
         write(10,*)'status = he5_gdinqdatatype(gdid3,"Grounding",
     1"local", HE5_HDFE_LOCATTRGROUP,dtype,classid,order,size)'
         write(10,*)'Data type of attribute ',dtype
         write(10,*)'Class id of attribute ',classid
         write(10,*)'Order of attribute ',order
         write(10,*)'Size of attribute ',size
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdrdlattr
c
      write(10,27)
      write(10,*)'Testing he5_gdrdlattr, Local attribute '
      write(10,*)'======================================'
      write(*,*)'Testing he5_gdrdlattr, Local attribute'
      write(*,*)'======================================'

      status = he5_gdrdlattr(gdid_utm,'Grounding',
     1'local',lattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdlattr(gdid_utm,"Grounding",
     1"local",lattr)'
        write(*,*)'Value returned by gdrdlattr ',status
        write(10,*)'status = he5_gdrdlattr(gdid_utm,"Grounding",
     1"local",lattr)'
        write(10,*)'Value returned by gdrdlattr ',status
        do 69 i=1,count
           write(10,*)'Attribute value  ',lattr(i)
           write(*,*)'Attribute value   ',lattr(i)
   69   continue
        
      endif
      write(10,*)' '
      write(*,*)' '
  
      status = he5_gdrdlattr(gdid3,'Grounding',
     1'local',lattr)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'status = he5_gdrdlattr(gdid3,"Grounding",
     1"local",lattr)'
        write(*,*)'Value returned by gdrdlattr ',status
        write(10,*)'status = he5_gdrdlattr(gdid3,"Grounding",
     1"local",lattr)'
        write(10,*)'Value returned by gdrdlattr ',status
        do 79 i=1,count
           write(10,*)'Attribute value  ',lattr(i)
           write(*,*)'Attribute value   ',lattr(i)
   79   continue
        
      endif
      write(10,*)' '
      write(*,*)' '
  
c Test Case -- he5_gdinqattrs
c
      write(10,27)
      write(10,*)'Testing he5_gdinqattrs, Global attribute'
      write(10,*)'========================================'
      write(*,*)'Testing he5_gdinqattrs, Global attribute'
      write(*,*)'========================================'

      lstatus = he5_gdinqattrs(gdid_utm, attrlist,strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqattrs(gdid_utm, 
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqattrs(gdid_utm, 
     1attrlist,strbufsize)'
        write(10,*)'Value returned by gdinqattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

      lstatus = he5_gdinqattrs(gdid3, attrlist,strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqattrs(gdid3, 
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqattrs(gdid3, 
     1attrlist,strbufsize)'
        write(10,*)'Value returned by gdinqattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdinqgattrs
c
      write(10,27)
      write(10,*)'Testing he5_gdinqgattrs, Group attribute'
      write(10,*)'============================================'
      write(*,*)'Testing he5_gdinqgattrs, Group attribute'
      write(*,*)'============================================'

      lstatus = he5_gdinqgattrs(gdid_utm,attrlist,strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqgattrs(gdid_utm, 
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqgattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqgattrs(gdid_utm,
     1attrlist,strbufsize)'
        write(10,*)'Value returned by gdinqgattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

      lstatus = he5_gdinqgattrs(gdid3,attrlist,strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqgattrs(gdid3, 
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqgattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqgattrs(gdid3,
     1attrlist,strbufsize)'
        write(10,*)'Value returned by gdinqgattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdinqlattrs
c
      write(10,27)
      write(10,*)'Testing he5_gdinqlattrs, Local attribute'
      write(10,*)'========================================'
      write(*,*)'Testing he5_gdinqlattrs, Local attribute'
      write(*,*)'========================================'

      lstatus = he5_gdinqlattrs(gdid_utm,"Grounding",attrlist,
     1strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqlattrs(gdid_utm,"Grounding",
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqlattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqlattrs(gdid_utm,"Grounding",
     1attrlist, strbufsize)'
        write(10,*)'Value returned by gdinqlattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

      lstatus = he5_gdinqlattrs(gdid3,"Grounding",attrlist,
     1strbufsize)
      if (lstatus .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(*,*)'lstatus = he5_gdinqlattrs(gdid3,"Grounding",
     1attrlist,strbufsize)'
        write(*,*)'Value returned by gdinqlattrs ',lstatus
        write(*,*)'List of attributes ',attrlist
        write(*,*)'Length of attrlist in bytes ',strbufsize
        write(10,*)'lstatus = he5_gdinqlattrs(gdid3,"Grounding",
     1attrlist, strbufsize)'
        write(10,*)'Value returned by gdinqlattrs ',lstatus
        write(10,*)'List of attributes ',attrlist
        write(10,*)'Length of attrlist in bytes ',strbufsize
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdgetfill
c
      write(10,27)
      write(10,*)'Testing he5_gdgetfill... '
      write(10,*)'======================='
      write(*,*)'Testing he5_gdgetfill... '
      write(*,*)'========================'

      status = he5_gdgetfill(gdid_utm,'Drift',ifillv)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdgetfill(gdid_utm,"Drift",
     1ifillv)'
         write(*,*)'Value returned by gdgetfill ',status
         write(*,*)'Fill value ',ifillv
         write(10,*)'status = he5_gdgetfill(gdid_utm,"Drift",
     1ifillv)'
         write(10,*)'Value returned by gdgetfill ',status
         write(10,*)'Fill value ',ifillv
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdgetfill(gdid3,'Drift',ifillv)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdgetfill(gdid3,"Drift",
     1ifillv)'
         write(*,*)'Value returned by gdgetfill ',status
         write(*,*)'Fill value ',ifillv
         write(10,*)'status = he5_gdgetfill(gdid3,"Drift",
     1ifillv)'
         write(10,*)'Value returned by gdgetfill ',status
         write(10,*)'Fill value ',ifillv
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddefboxreg
c
      write(10,27)
      write(10,*)'Testing he5_gddefboxreg... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddefboxreg... '
      write(*,*)'=========================='

      corlon(1)=-126.0
      corlon(2)=-121.5
      corlat(1)=-70.0
      corlat(2)=-65.0

      regionid = he5_gddefboxreg(gdid_utm,corlon,corlat)
      if (regionid .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'regionid = he5_gddefboxreg(gdid_utm,
     1corlon,corlat)'
         write(*,*)'Region id returned by gddefboxreg ',regionid
         write(10,*)'regionid = he5_gddefboxreg(gdid_utm,
     1corlon,corlat)'
         write(10,*)'Region id returned by gddefboxreg ',regionid
      endif
      write(10,*)' '
      write(*,*)' '

      regionid3 = he5_gddefboxreg(gdid3,corlon,corlat)
      if (regionid3 .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'regionid3 = he5_gddefboxreg(gdid3,
     1corlon,corlat)'
         write(*,*)'Region id returned by gddefboxreg ',regionid3
         write(10,*)'regionid3 = he5_gddefboxreg(gdid3,
     1corlon,corlat)'
         write(10,*)'Region id returned by gddefboxreg ',regionid3
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdreginfo
c
      write(10,27)
      write(10,*)'Testing he5_gdreginfo... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdreginfo... '
      write(*,*)'=========================='

      status = he5_gdreginfo(gdid_utm,regionid,'Impedance',nt,rk,
     1dims,size,iuplft,ilrgt)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdreginfo(gdid_utm,regionid,
     1"Impedance",nt, rk,dims,size,iuplft,ilrgt)'
         write(*,*)'Value returned by gdreginfo ',status
         write(*,*)'Number type of region ',nt
         write(*,*)'Rank of region ',rk
         write(*,*)'Size of region ',size
         write(*,11)'Dimensions of region ',dims(1),' ',dims(2)
         write(*,12)'Upper left point of region ',iuplft(1),' ',
     1iuplft(2)
         write(*,12)'Lower right point of region ',ilrgt(1),' ',
     1ilrgt(2)
         write(10,*)'status = he5_gdreginfo(gdid_utm,regionid,
     1"Impedance",nt,rk,dims,size,iuplft,ilrgt)'
         write(10,*)'Value returned by gdreginfo ',status
         write(10,*)'Number type of region ',nt
         write(10,*)'Rank of region ',rk
         write(10,*)'Size of region ',size
         write(10,11)'Dimensions of region ',dims(1),' ',dims(2)
         write(10,12)'Upper left point of region ',iuplft(1),' ',
     1iuplft(2)
         write(10,12)'Lower right point of region ',ilrgt(1),' ',
     1ilrgt(2)
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdreginfo(gdid3,regionid3,'Impedance',nt,rk,
     1dims,size,iuplft,ilrgt)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdreginfo(gdid3,regionid3,
     1"Impedance",nt, rk,dims,size,iuplft,ilrgt)'
         write(*,*)'Value returned by gdreginfo ',status
         write(*,*)'Number type of region ',nt
         write(*,*)'Rank of region ',rk
         write(*,*)'Size of region ',size
         write(*,11)'Dimensions of region ',dims(1),' ',dims(2)
         write(*,12)'Upper left point of region ',iuplft(1),' ',
     1iuplft(2)
         write(*,12)'Lower right point of region ',ilrgt(1),' ',
     1ilrgt(2)
         write(10,*)'status = he5_gdreginfo(gdid3,regionid3,
     1"Impedance",nt,rk,dims,size,iuplft,ilrgt)'
         write(10,*)'Value returned by gdreginfo ',status
         write(10,*)'Number type of region ',nt
         write(10,*)'Rank of region ',rk
         write(10,*)'Size of region ',size
         write(10,11)'Dimensions of region ',dims(1),' ',dims(2)
         write(10,12)'Upper left point of region ',iuplft(1),' ',
     1iuplft(2)
         write(10,12)'Lower right point of region ',ilrgt(1),' ',
     1ilrgt(2)
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdextreg
c
      write(10,27)
      write(10,*)'Testing he5_gdextreg... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdextreg... '
      write(*,*)'=========================='

      status = he5_gdextreg(gdid_utm,regionid,'Impedance',datbuf)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdextreg(gdid_utm,regionid,
     1"Impedance",datbuf)'
         write(*,*)'Value returned by gdextreg ',status
         write(10,*)'status = he5_gdextreg(gdid_utm,regionid,
     1"Impedance",datbuf)'
         write(10,*)'Value returned by gdextreg ',status
         do 175 i=100,150
            write(*,*)'Value of region ',datbuf(i)
            write(10,*)'Value of region ',datbuf(i)
  175    continue
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gdextreg(gdid3,regionid3,'Impedance',datbuf)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdextreg(gdid3,regionid3,
     1"Impedance",datbuf)'
         write(*,*)'Value returned by gdextreg ',status
         write(10,*)'status = he5_gdextreg(gdid3,regionid3,
     1"Impedance",datbuf)'
         write(10,*)'Value returned by gdextreg ',status
         do 176 i=100,150
            write(*,*)'Value of region ',datbuf(i)
            write(10,*)'Value of region ',datbuf(i)
  176    continue
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddeftmeper
c
      write(10,27)
      write(10,*)'Testing he5_gddeftmeper... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddeftmeper... '
      write(*,*)'=========================='

      periodid = he5_gddeftmeper(gdid_utm,HE5_HDFE_NOPREVSUB,
     1starttime,stoptime)
      if (periodid .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'periodid = he5_gddeftmeper(gdid_utm,
     1HE5_HDFE_NOPREVSUB,starttime,stoptime)'
        write(10,*)'Define a time period '
        write(10,*)'Period id returned by gddeftmeper ',periodid
        write(*,*)'periodid = he5_gddeftmeper(gdid_utm,
     1HE5_HDFE_NOPREVSUB,starttime,stoptime)'
        write(*,*)'Define a time period '
        write(*,*)'Period id returned by gddeftmeper ',periodid
      endif
      write(10,*)' '
      write(*,*)' '

      periodid = he5_gddeftmeper(gdid3,HE5_HDFE_NOPREVSUB,
     1starttime,stoptime)
      if (periodid .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'periodid = he5_gddeftmeper(gdid3,
     1HE5_HDFE_NOPREVSUB,starttime,stoptime)'
        write(10,*)'Define a time period '
        write(10,*)'Period id returned by gddeftmeper ',periodid
        write(*,*)'periodid = he5_gddeftmeper(gdid3,
     1HE5_HDFE_NOPREVSUB,starttime,stoptime)'
        write(*,*)'Define a time period '
        write(*,*)'Period id returned by gddeftmeper ',periodid
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddefvrtreg
c
      write(10,27)
      write(10,*)'Testing he5_gddefvrtreg... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gddefvrtreg... '
      write(*,*)'=========================='

      regionid = he5_gddefvrtreg(gdid_utm,HE5_HDFE_NOPREVSUB,
     1'Height',range)
      if (regionid .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'regionid = he5_gddefvrtreg(gdid_utm,
     1HE5_HDFE_NOPREVSUB,"Height",range)'
        write(10,*)'Define a vertical region '
        write(10,*)'Region id returned by gddefvrtreg ',regionid
        write(*,*)'regionid = he5_gddefvrtreg(gdid_utm,
     1HE5_HDFE_NOPREVSUB,"Height",range)'
        write(*,*)'Define a vertical region '
        write(*,*)'Region id returned by gddefvrtreg ',regionid
      endif
      write(10,*)' '
      write(*,*)' '

      regionid3 = he5_gddefvrtreg(gdid3,HE5_HDFE_NOPREVSUB,
     1'Height',range)
      if (regionid3 .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'regionid3 = he5_gddefvrtreg(gdid3,
     1HE5_HDFE_NOPREVSUB,"Height",range)'
        write(10,*)'Define a vertical region '
        write(10,*)'Region id returned by gddefvrtreg ',regionid3
        write(*,*)'regionid3 = he5_gddefvrtreg(gdid3,
     1HE5_HDFE_NOPREVSUB,"Height",range)'
        write(*,*)'Define a vertical region '
        write(*,*)'Region id returned by gddefvrtreg ',regionid3
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gddupreg
c
      write(10,27)
      write(10,*)'Testing he5_gddupreg... '
      write(10,*)'======================='
      write(*,*)'Testing he5_gddupreg... '
      write(*,*)'======================='

      regionid2 = he5_gddupreg(regionid)
      if (regionid2 .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'regionid2 = he5_gddupreg(regionid)'
        write(10,*)'Generate a new region id '
        write(10,*)'New region id ',regionid2
        write(*,*)'regionid2 = he5_gddupreg(regionid)'
        write(*,*)'Generate a new region id '
        write(*,*)'New region id ',regionid2
      endif
      write(10,*)' '
      write(*,*)' '

      regionid4 = he5_gddupreg(regionid3)
      if (regionid4 .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'regionid4 = he5_gddupreg(regionid3)'
        write(10,*)'Generate a new region id '
        write(10,*)'New region id ',regionid4
        write(*,*)'regionid4 = he5_gddupreg(regionid3)'
        write(*,*)'Generate a new region id '
        write(*,*)'New region id ',regionid4
      endif
      write(10,*)' '
      write(*,*)' '


c Test Case -- he5_gdgetpix
c
      write(10,27)
      write(10,*)'Testing he5_gdgetpix... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdgetpix... '
      write(*,*)'=========================='

      nlonlat = 5
      status= he5_gdgetpix(gdid_utm,nlonlat,lonval,
     1latval,rowval,colval)             
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gdgetpix(gdid_utm,5,lonval,
     1latval,rowval,colval)'
        write(10,*)'Retrieve pixel row. column values '
        write(*,*)'status = he5_gdgetpix(gdid_utm,5,lonval,latval,
     1rowval,colval)'
        write(*,*)'Retrieve pixel row. column values '
        do 37 i=1,5
           write(10,5)'Pixel position lon lat ',lonval(i),' ',
     1latval(i),' ','Row Column ',rowval(i),' ',colval(i) 
           write(*,5)'Pixel position lon lat ',lonval(i),' ',
     1latval(i),' ','Row Column ',rowval(i),' ',colval(i) 
   37   continue
      endif
    5 format(a,f6.1,a,f6.1,a,a,i2,a,i2)
      write(10,*)' '
      write(*,*)' '

      status= he5_gdgetpix(gdid3,nlonlat,lonval,
     1latval,rowval,colval)             
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'status = he5_gdgetpix(gdid3,5,lonval,
     1latval,rowval,colval)'
        write(10,*)'Retrieve pixel row. column values '
        write(*,*)'status = he5_gdgetpix(gdid3,5,lonval,latval,
     1rowval,colval)'
        write(*,*)'Retrieve pixel row. column values '
        do 47 i=1,5
           write(10,5)'Pixel position lon lat ',lonval(i),' ',
     1latval(i),' ','Row Column ',rowval(i),' ',colval(i) 
           write(*,5)'Pixel position lon lat ',lonval(i),' ',
     1latval(i),' ','Row Column ',rowval(i),' ',colval(i) 
   47   continue
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdgetpixval
c
      write(10,27)
      write(10,*)'Testing he5_gdgetpixval... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdgetpixval... '
      write(*,*)'=========================='

      nlonlat = 5
      size= he5_gdgetpixval(gdid_utm,nlonlat,rowval,colval,
     1"Voltage",fldval)
      if (size .le. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'lstatus = he5_gdgetpixval(gdid_utm,5,rowval,
     1colval,"Voltage",fldval)'
        write(10,*)'Retrieve pixel values for specific locations '
        write(*,*)'lstatus = he5_gdgetpixval(gdid_utm,5,rowval,
     1colval,"Voltage",fldval)'
        write(*,*)'Retrieve pixel values for specific locations '
        do i=1,5
           write(10,17)'Row Col ',rowval(i),' ',colval(i),' ',
     1'Pixel Value ',fldval(i)
           write(*,17)'Row Col ',rowval(i),' ',colval(i),' ',
     1'Pixel Value ',fldval(i)
        enddo
      endif
   17 format(a8,i2,a1,i2,a1,a12,f9.3)
      write(10,*)' '
      write(*,*)' '

      size= he5_gdgetpixval(gdid3,nlonlat,rowval,colval,
     1"Voltage",fldval)
      if (size .le. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'lstatus = he5_gdgetpixval(gdid3,5,rowval,
     1colval,"Voltage",fldval)'
        write(10,*)'Retrieve pixel values for specific locations '
        write(*,*)'lstatus = he5_gdgetpixval(gdid3,5,rowval,
     1colval,"Voltage",fldval)'
        write(*,*)'Retrieve pixel values for specific locations '
        do i=1,5
           write(10,17)'Row Col ',rowval(i),' ',colval(i),' ',
     1'Pixel Value ',fldval(i)
           write(*,17)'Row Col ',rowval(i),' ',colval(i),' ',
     1'Pixel Value ',fldval(i)
        enddo
      endif
      write(10,*)' '
      write(*,*)' '

c Test Case -- he5_gdinterpolate
c
      write(10,27)
      write(10,*)'Testing he5_gdinterpolate... '
      write(10,*)'=========================='
      write(*,*)'Testing he5_gdinterpolate... '
      write(*,*)'=========================='

      nlonlat = 5
      size= he5_gdinterpolate(gdid_utm,nlonlat,lonval,latval,
     1'Voltage',interval)
      if (size .le. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'lstatus= he5_gdinterpolate(gdid_utm,5,lonval,
     1latval,"Voltage",interval)'
        write(10,*)'Interpolate 5 values for field Voltage '
        write(*,*)'lstatus= he5_gdinterpolate(gdid_utm,5,lonval,
     1latval,"Voltage",interval)'
        write(*,*)'Interpolate 5 values for field Voltage '
        write(*,*)'Status returned by call to gdinterpolate ',lstatus
        do i=1,5
           write(*,4)'Interpolated value ',interval(i,1)
           write(10,4)'Interpolated value ',interval(i,1)
        enddo
      endif
    4 format(a,f9.3)
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid_utm)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddetach(gdid_utm)'
         write(*,*)'Value returned by gddetach ',status
         write(10,*)'status = he5_gddetach(gdid_utm)'
         write(10,*)'Value returned by gddetach ',status
      endif

      size= he5_gdinterpolate(gdid3,nlonlat,lonval,latval,
     1'Voltage',interval)
      if (size .le. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
        write(10,*)'lstatus= he5_gdinterpolate(gdid3,5,lonval,
     1latval,"Voltage",interval)'
        write(10,*)'Interpolate 5 values for field Voltage '
        write(*,*)'lstatus= he5_gdinterpolate(gdid3,5,lonval,
     1latval,"Voltage",interval)'
        write(*,*)'Interpolate 5 values for field Voltage '
        write(*,*)'Status returned by call to gdinterpolate ',lstatus
        do i=1,5
           write(*,4)'Interpolated value ',interval(i,1)
           write(10,4)'Interpolated value ',interval(i,1)
        enddo
      endif
      write(10,*)' '
      write(*,*)' '

      status = he5_gddetach(gdid3)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gddetach(gdid3)'
         write(*,*)'Value returned by gddetach ',status
         write(10,*)'status = he5_gddetach(gdid3)'
         write(10,*)'Value returned by gddetach ',status
      endif

      status = he5_gdclose(gdfid)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdclose(gdfid)'
         write(*,*)'Returned from gdclose ', status
         write(10,*)'status = he5_gdclose(gdfid)'
         write(10,*)'Returned from gdclose ', status
      endif

      status = he5_gdclose(gdfid3)
      if (status .lt. zero) then
        write(10,*)'**********Error unexpected***********'
        write(*,*)'**********Error unexpected***********'
        
      else
         write(*,*)' '
         write(*,240)
         write(10,*)' '
         write(10,240)
         write(*,*)'status = he5_gdclose(gdfid3)'
         write(*,*)'Returned from gdclose ', status
         write(10,*)'status = he5_gdclose(gdfid3)'
         write(10,*)'Returned from gdclose ', status
      endif

      close(unit=10)
 27   format(' ')
      stop
      end








