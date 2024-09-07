c----------------------------------------------------------------------------
c     Copyright (C) 2000   Emergent IT Inc. and Raytheon Systems Company    |
c----------------------------------------------------------------------------
c testswath64.f                                                             |
c In this program we create, define and write a simple swath hdfeos file    |
c using the swath interface                                                 |
c----------------------------------------------------------------------------

      program   testswath64
 
      implicit  none
 
      include   'hdfeos5.inc'
 
      integer   status
      integer   index1
      integer   index2
      integer   swfid
      integer   swid
      integer   swid2
      integer   rank
      integer   n, nt
      integer   dtype
      integer   classid
      integer   order
      integer   rdattr(4)
      integer   global_attr(4)
      integer   compparm(5)
      integer   cnkrank
      integer   rdcompparm(5)
      integer   compcode
      integer   regionid
      integer   regionid2
      integer   regionid3
      integer   ntype(5)
      integer   rnk(5)
      integer   ntyped(5)
      integer   rnkd(5)

      integer   he5_swopen
      integer   he5_swcreate
      integer   he5_swattach
      integer   he5_swdefdim
      integer   he5_swdefgfld
      integer   he5_swdefdfld
      integer   he5_swdefmap
      integer   he5_swdefchunk
      integer   he5_swdefcomp
      integer   he5_swsetfill
      integer   he5_swwrattr
      integer   he5_swdefimap
      integer   he5_swwrlattr
      integer   he5_swwrfld
      integer   he5_swrdfld
      integer   he5_swwrgattr
      integer   he5_swrdattr
      integer   he5_swrdgattr
      integer   he5_swrdlattr
      integer   he5_swdefvrtreg
      integer   he5_swdefboxreg
      integer   he5_swdeftmeper
      integer   he5_swgmapinfo
      integer   he5_swfldinfo 
      integer   he5_swcompinfo
      integer   he5_swmapinfo
      integer   he5_swattrinfo
      integer   he5_swgattrinfo
      integer   he5_swreginfo
      integer   he5_swextreg
      integer   he5_swregidx
      integer   he5_swperinfo
      integer   he5_swextper
      integer   he5_swlattrinfo
      integer   he5_swgetfill
      integer   he5_swidtype
      integer   he5_swdupreg
      integer   he5_swdetach
      integer   he5_swclose 

      integer*8 he5_swupimap
      integer*8 he5_swinqswath
      integer*8 he5_swnentries
      integer*8 he5_swinqmaps
      integer*8 he5_swinqimaps
      integer*8 he5_swinqdims
      integer*8 he5_swdiminfo
      integer*8 he5_swimapinfo
      integer*8 he5_swinqattrs
      integer*8 he5_swinqgattrs
      integer*8 he5_swinqlattrs
      integer*8 he5_swinqgflds
      integer*8 he5_swinqdflds
      integer*8 size
      integer*8 strbufsize
      integer*8 ndims
      integer*8 chunkdims(1)
      integer*8 start(2)
      integer*8 stride(2)
      integer*8 dataedge(2)
      integer*8 geoedge(2)
      integer*8 geostart(2)
      integer*8 istart(2)
      integer*8 iedge(2)
      integer*8 dedge(2)
      integer*8 sd_start(2)
      integer*8 sd_stride(2)
      integer*8 sd_edge(2)
      integer*8 rd_start(2)
      integer*8 rd_stride(2)
      integer*8 rd_edge(2)
      integer*8 indexmap(30)
      integer*8 dimsize
      integer*8 nattr
      integer*8 nswath
      integer*8 idxsz
      integer*8 nmaps
      integer*8 tstart(1)
      integer*8 tstride(1)
      integer*8 tedge(1)
      integer*8 strbufsz2
      integer*8 nflds
      integer*8 idxrange(2)
      integer*8 count(1)
      integer*8 offset
      integer*8 inc
      integer*8 status4
      integer*8 zero
      integer*8 ndxmap(2)
      integer*8 dims(5)
      integer*8 latdims(2)
      integer*8 upidxmap(50)
      integer*8 idxmap(50)
      integer*8 dim1
      integer*8 dim2
      integer*8 dim3
      integer*8 dim4

      real      grpattr(3)
      real      locattr(3)
      real      rdgnlattr(3)
      real      ray1(100,40)
      real      ray2(40,100)
      real      lat(40,50)
      real      lon(40,50)
      real      ray4(40,100)
      real      ray3(40,40)
      real      temp(40,100)
      real      idxlat(30,30)
      real      idxlon(30,30)
      real      latcnt
      real      loncnt
      real      raycnt
      real      idxlatcnt
      real      idxloncnt	
      real      fillvalue
      real      rdfilval
      real      tempbuf(4200)

      real*8    cornerlon(2)
      real*8    cornerlat(2)
      real*8    twodtime(40,50)
      real*8    onedtime(40)
      real*8    range(2)
      real*8    tmcnt
      real*8    starttime
      real*8    endtime

      character*(256)  geodim
      character*(256)  idxdimmap
      character*(256)  dimmap
      character*(256)  dimname
      character*(256)  fieldlist
      character*(256)  attrlist
      character*(256)  swathlist
      character*(256)  dimens
      character*(256)  maxdims

      dim1          = 30
      dim2          = 40
      dim3          = 50
      dim4          = 100
      count(1)      = 3
      compparm(1)   = 6
      compparm(2)   = 0
      compparm(3)   = 0
      compparm(4)   = 0
      compparm(5)   = 0
      cnkrank       = 1
      chunkdims(1)  = 10
      rdcompparm(1) = 1
      rdcompparm(2) = 1
      rdcompparm(3) = 1
      rdcompparm(4) = 1
      rdcompparm(5) = 1
      compcode      = -1
      raycnt        = -799.0
      tmcnt         = 46353400.0d0
      starttime     = 46353450.0d0
      endtime       = 46500000.0d0
      range(1)      = 46353450.0d0
      range(2)      = 46500000.0d0
      latcnt        = 39.8
      loncnt        = 78.0
      global_attr(1)= 11
      global_attr(2)= 33
      global_attr(3)= 66
      global_attr(4)= 99
      grpattr(1)    = 21.7
      grpattr(2)    = 24.3
      grpattr(3)    = 26.3
      locattr(3)    = 21.7
      locattr(2)    = 24.3
      locattr(1)    = 26.3
      start(1)      = 10
      start(2)      = 10
      tstart(1)     = 0
      tstride(1)    = 1
      tedge(1)      = 40
      geostart(1)   = 0
      geostart(2)   = 0 
      stride(1)     = 1
      stride(2)     = 1
      sd_start(1)   = 0
      sd_start(2)   = 0
      sd_stride(1)  = 1
      sd_stride(2)  = 1
      sd_edge(1)    = 100
      sd_edge(2)    = 40
      rd_start(1)   = 0
      rd_start(2)   = 0
      rd_stride(1)  = 1
      rd_stride(2)  = 1
      rd_edge(1)    = 40
      rd_edge(2)    = 100
      dataedge(1)   = 20
      dataedge(2)   = 50
      geoedge(1)    = 40
      geoedge(2)    = 50
      idxlatcnt     = 39.8
      idxloncnt     = 78.0
      istart(1)     = 0
      istart(2)     = 0
      iedge(1)      = 30
      iedge(2)      = 30
      dedge(1)      = 40
      dedge(2)      = 40
      rank          = 2
      fillvalue     = 16.0
      cornerlon(1)  = 75.0
      cornerlon(2)  = 78.0
      cornerlat(1)  = 38.0
      cornerlat(2)  = 39.8
      indexmap(1)   = 1
      indexmap(2)   = 2
      indexmap(3)   = 3
      indexmap(4)   = 4
      indexmap(5)   = 6
      indexmap(6)   = 7
      indexmap(7)   = 8
      indexmap(8)   = 9
      indexmap(9)   = 11
      indexmap(10)  = 12
      indexmap(11)  = 13
      indexmap(12)  = 14
      indexmap(13)  = 16
      indexmap(14)  = 17
      indexmap(15)  = 18
      indexmap(16)  = 19
      indexmap(17)  = 21
      indexmap(18)  = 22
      indexmap(19)  = 23
      indexmap(20)  = 24
      indexmap(21)  = 26
      indexmap(22)  = 27
      indexmap(23)  = 28
      indexmap(24)  = 29
      indexmap(25)  = 31
      indexmap(26)  = 32
      indexmap(27)  = 33
      indexmap(28)  = 34
      indexmap(29)  = 36
      indexmap(30)  = 37
      zero          = 0

c
c This section of the program just fills some arrays with data that will be
c used later in the program
c
      do 110 index1=1,100
         do 100 index2=1,40
            ray1(index1, index2)=raycnt
            ray2(index2, index1)=raycnt + 1.0
            ray4(index2,index1) = raycnt
            raycnt = raycnt +1.
  100    continue
  110 continue

      do 130 index1=1,40
         do 120 index2=1,50
            lat(index1, index2)=latcnt
            lon(index1, index2)=loncnt
            twodtime(index1,index2)=tmcnt
            loncnt=loncnt-.1
            tmcnt=tmcnt+5.0
  120    continue
         onedtime(index1)=tmcnt
         latcnt=latcnt-0.1
         loncnt = 78.0
  130 continue

      do 150 index1=1,30
         do 140 index2=1,30
            idxlat(index1, index2)=idxlatcnt
            idxlon(index1, index2)=idxloncnt
            idxloncnt=idxloncnt-.1
  140    continue
         idxlatcnt=idxlatcnt-.1
         idxloncnt=78.0
  150 continue


      do 170 index1=1,40
         do 160 index2=1,40
            ray3(index1, index2)=raycnt
            raycnt = raycnt+1.25
  160    continue
  170 continue

      do 987 index1=1,50
         idxmap(index1)=zero
         upidxmap(index1)=zero
  987 continue

  230 format('********Error unexpected**********')
  240 format('***********Passed Test*********')

      open(unit=1, file="testswath64_f.txtout", status = "UNKNOWN")

      write(*,*)'Testing he5_swath FORTRAN 77 interface'
      write(*,*)'======================================'
      write(1,*)'Testing he5_swath FORTRAN 77 interface'
      write(1,*)'======================================'
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swopen
c
      write(*,*)'Testing he5_swopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_swopen '
      write(1,*)'====================='
      swfid=he5_swopen('swath.he5',HE5F_ACC_TRUNC)
      if (swfid .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	swfid= he5_swopen("swath.he5",HE5F_ACC_TRUNC)'
         write(*,*)'	Value returned by he5_swopen ', swfid
         write(1,*)'	swfid= he5_swopen("swath.he5",HE5F_ACC_TRUNC)'
         write(1,*)'	Value returned by he5_swopen ', swfid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swcreate
c
      write(1,27)
      write(*,*)'Testing he5_swcreate '
      write(*,*)'====================='
      write(1,*)'Testing he5_swcreate '
      write(1,*)'====================='
      swid=he5_swcreate(swfid,'Simplef')
      if (swid .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid=he5_swcreate(swfid, "Simplef")'
         write(*,*)'Value returned by he5_swcreate ', swid
         write(1,*)'swid= he5_swcreate(swfid, "Simplef")'
         write(1,*)'Value returned by he5_swcreate ', swid
      endif
      write(*,*)' '
      write(1,*)' '

      swid2=he5_swcreate(swfid,'Indexedf')
      if (swid2 .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid2= he5_swcreate(swfid, "Indexedf")'
         write(*,*)'Value returned by he5_swcreate ', swid2
         write(1,*)'swid2= he5_swcreate(swfid, "Indexedf")'
         write(1,*)'Value returned by he5_swcreate ', swid2
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swdefdim
c
      write(1,27)
      write(*,*)'Testing he5_swdefdim '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefdim '
      write(1,*)'====================='
      status=he5_swdefdim(swid,'DataTrack',dim4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid, "DataTrack", 100)'
         write(*,*)'	Value returned by he5_swdefdim ',status
         write(1,*)'	status= he5_swdefdim(swid, "DataTrack", 100)'
         write(1,*)'	Value returned by he5_swdefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid,'DataXtrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid, "DataXtrack", 40)'
         write(*,*)'	Value returned by he5_swdefdim ',status
         write(1,*)'	status= he5_swdefdim(swid, "DataXtrack", 40)'
         write(1,*)'	Value returned by he5_swdefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid,'GeoTrack',dim3) 
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid, "GeoTrack", 50)'
         write(*,*)'	Value returned by he5_swdefdim ',status
         write(1,*)'	status= he5_swdefdim(swid, "GeoTrack", 50)'
         write(1,*)'	Value returned by he5_swdefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid,'GeoXtrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid, "GeoXtrack", 40)'
         write(*,*)'	Value returned by he5_swdefdim ',status
         write(1,*)'	status= he5_swdefdim(swid, "GeoXtrack", 40)'
         write(1,*)'	Value returned by he5_swdefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid2,'DataTrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid2, "DataTrack", 40)'
         write(*,*)'	Value returned by he5_swdefdim ', status
         write(1,*)'	status= he5_swdefdim(swid2, "DataTrack", 40)'
         write(1,*)'	Value returned by he5_swdefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid2,'DataXtrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid2, "DataXtrack", 40)'
         write(*,*)'	Value returned by he5_swdefdim ', status
         write(1,*)'	status= he5_swdefdim(swid2, "DataXtrack", 40)'
         write(1,*)'	Value returned by he5_swdefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid2,'GeoTrack',dim1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid2, "GeoTrack", 30)'
         write(*,*)'	Value returned by he5_swdefdim ', status
         write(1,*)'	status= he5_swdefdim(swid2, "GeoTrack", 30)'
         write(1,*)'	Value returned by he5_swdefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid2,'GeoXtrack',dim1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_swdefdim(swid2, "GeoXtrack", 30)'
         write(*,*)'	Value returned by he5_swdefdim ', status
         write(1,*)'	status= he5_swdefdim(swid2, "GeoXtrack", 30)'
         write(1,*)'	Value returned by he5_swdefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdim(swid2,'Timedim',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define dimension Timedim '
         write(*,*)'	Value returned by he5_swdefdim ', status
         write(1,*)'Define dimension Timedim '
         write(1,*)'	Value returned by he5_swdefdim ', status
      endif

      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefgfld
c
      write(1,27)
      write(*,*)'Testing he5_swdefgfld '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefgfld '
      write(1,*)'====================='
      status=he5_swdefgfld(swid,'Latitude','GeoXtrack,GeoTrack',
     1' ',HE5T_NATIVE_FLOAT,0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefgfld(swid,"Latitude",
     1"GeoXtrack,GeoTrack"," ", HE5T_NATIVE_FLOAT, 0)'
         write(1,*)'status= he5_swdefgfld(swid,"Latitude",
     1"GeoXtrack,GeoTrack"," ", HE5T_NATIVE_FLOAT, 0)'
         write(1,*)'Value returned by he5_swdefgfld ',status
         write(*,*)'Value returned by he5_swdefgfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefgfld(swid,'Longitude','GeoXtrack,GeoTrack',
     1' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_swdefgfld ',status
         write(1,*)'Value returned by he5_swdefgfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefgfld(swid2,'Latitude','GeoXtrack,GeoTrack',
     1' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_swdefgfld ', status
         write(1,*)'Value returned by he5_swdefgfld ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefgfld(swid2,'Longitude','GeoXtrack,GeoTrack',
     1' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_swdefgfld ', status
         write(1,*)'Value returned by he5_swdefgfld ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefgfld(swid2,'Time','Timedim',' ',
     1HE5T_NATIVE_DOUBLE, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define field Time for swath Indexedf '
         write(*,*)'Status returned from call to he5_swdefgfld ',status
         write(1,*)'Define field Time for swath Indexedf '
         write(1,*)'Status returned from call to he5_swdefgfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefgfld(swid,'Time','GeoXtrack,GeoTrack',
     1' ', HE5T_NATIVE_DOUBLE, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefgfld(swid, "Time","GeoXtrack,GeoTrack",
     1"",HE5T_NATIVE_DOUBLE, 0)'
         write(*,*)'Value returned by he5_swdefgfld ',status
         write(1,*)'status= he5_swdefgfld(swid, "Time","GeoXtrack,GeoTrack",
     1"",HE5T_NATIVE_DOUBLE, 0)'
         write(1,*)'Value returned by he5_swdefgfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefdfld
c
      write(1,27)
      write(*,*)'Testing he5_swdefdfld '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefdfld '
      write(1,*)'====================='

      status=he5_swdefdfld(swid,'SideA','DataXtrack,DataTrack',
     1' ', HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
        
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field SideA '
         write(*,*)'Value returned by he5_swdefdfld ',status
         write(1,*)'Define data field SideA '
         write(1,*)'Value returned by he5_swdefdfld ',status
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case  -- he5_swsetfill
c
      write(1,27)
      write(*,*)'Testing he5_swsetfill '
      write(*,*)'====================='
      write(1,*)'Testing he5_swsetfill '
      write(1,*)'====================='
      status= he5_swsetfill(swid,'Temperature',
     1HE5T_NATIVE_FLOAT, fillvalue)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swsetfill(swid,"Temperature",
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(*,*)'Value returned by he5_swsetfill ',status
         write(1,*)'status=he5_swsetfill(swid,"Temperature",
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(1,*)'Value returned by he5_swsetfill ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdfld(swid,'Temperature',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field Temperature '
         write(*,*)'Value returned by he5_swdefdfld ',status
         write(1,*)'Define data field Temperature '
         write(1,*)'Value returned by he5_swdefdfld ',status
         write(*,*)' '
         write(1,*)' '
      endif

      status= he5_swsetfill(swid,'Conduction',
     1HE5T_NATIVE_FLOAT,fillvalue)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swsetfill(swid,"Conduction",
     1HE5T_NATIVE_FLOAT,fillvalue)'
         write(*,*)'Value returned by he5_swsetfill ',status
         write(1,*)'status=he5_swsetfill(swid,"Conduction",
     1HE5T_NATIVE_FLOAT,fillvalue)'
         write(1,*)'Value returned by he5_swsetfill ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdfld(swid,'Conduction',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT,0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefdfld(swid,"Conduction",
     1"DataXtrack,DataTrack","",HE5T_NATIVE_FLOAT, 0)'
         write(*,*)'Value returned by he5_swdefdfld ',status
         write(1,*)'status= he5_swdefdfld(swid,"Conduction",
     1"DataXtrack,DataTrack","",HE5T_NATIVE_FLOAT, 0)'
         write(1,*)'Value returned by he5_swdefdfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefdfld(swid2,'Sensor1',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swdefdfld(swid2, "Sensor1",
     1"DataTrack,DataXtrack","",HE5T_NATIVE_FLOAT, 0)'
         write(*,*)'Value returned by he5_swdefdfld ',status
         write(1,*)'status= he5_swdefdfld(swid2, "Sensor1",
     1"DataTrack,DataXtrack","",HE5T_NATIVE_FLOAT, 0)'
         write(1,*)'Value returned by he5_swdefdfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefchunk
c
      write(1,27)
      write(*,*)'Testing he5_swdefchunk'
      write(*,*)'======================'
      write(1,*)'Testing he5_swdefchunk'
      write(1,*)'======================'
      status=he5_swdefchunk(swid,cnkrank,chunkdims)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefchunk(swid, cnkrank, chunkdims)'
         write(*,*)'Value returned by he5_swdefchunk ',status
         write(1,*)'status= he5_swdefchunk(swid, cnkrank, chunkdims)'
         write(1,*)'Value returned by he5_swdefchunk ',status
      endif
      write(*,*)' '
      write(1,*)' '
    
c Test Case -- he5_swdefcomp
c
      write(1,27)
      write(*,*)'Testing he5_swdefcomp'
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefcomp'
      write(1,*)'====================='
      status=he5_swdefcomp(swid,HE5_HDFE_COMP_DEFLATE,compparm)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefcomp(swid,HE5_HDFE_COMP_DEFLATE,
     1compparm)'
         write(*,*)'Value returned by he5_swdefcomp ',status
         write(1,*)'status= he5_swdefcomp(swid,HE5_HDFE_COMP_DEFLATE,
     1compparm)'
         write(1,*)'Value returned by he5_swdefcomp ',status
      endif
      write(*,*)' '
      write(1,*)' '


      status=he5_swdefdfld(swid,'CompChunk','GeoTrack',
     1' ',HE5T_NATIVE_FLOAT, 0)
      if (status .lt. zero) then
         write(*,230)
         write(1,230) 
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field CompChunk '
         write(*,*)'Value returned by he5_swdefdfld ',status
         write(1,*)'Define data field CompChunk '
         write(1,*)'Value returned by he5_swdefdfld ',status
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swdetach
c
      write(1,27)
      write(*,*)'Testing he5_swdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdetach '
      write(1,*)'====================='
      status=he5_swdetach(swid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdetach(swid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid2)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid2)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      swid=he5_swattach(swfid,'Simplef')
      if (swid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid= he5_swattach(swfid, "Simplef")'
         write(*,*)'Value returned by he5_swattach ',swid
         write(1,*)'swid= he5_swattach(swfid, "Simplef")'
         write(1,*)'Value returned by he5_swattach ',swid
      endif
      write(*,*)' '
      write(1,*)' '

      swid2=he5_swattach(swfid,'Indexedf')
      if (swid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(*,*)'Value returned by he5_swattach ',swid2
         write(1,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(1,*)'Value returned by he5_swattach ',swid2
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefmap
c
      write(1,27)
      write(*,*)'Testing he5_swdefmap '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefmap '
      write(1,*)'====================='
      offset = 0
      inc    = 2
      status=he5_swdefmap(swid,'GeoTrack','DataTrack',
     1offset,inc)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefmap(swid,"GeoTrack",
     1"DataTrack", offset, inc)'
         write(*,*)'Value returned by he5_swdefmap ',status
         write(1,*)'status= he5_swdefmap(swid,"GeoTrack",
     1"DataTrack", offset, inc)'
         write(1,*)'Value returned by he5_swdefmap ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefimap
c
      write(1,27)
      write(*,*)'Testing he5_swdefimap '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdefimap '
      write(1,*)'====================='
      status=he5_swdefimap(swid2,'GeoTrack','DataTrack',indexmap)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdefimap(swid2, "GeoTrack", 
     1"DataTrack",indexmap)'
         write(*,*)'Value returned by he5_swdefimap ',status
         write(1,*)'status= he5_swdefimap(swid2, "GeoTrack",
     1"DataTrack",indexmap)'
         write(1,*)'Value returned by he5_swdefimap ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swdefimap(swid2,'GeoXtrack','DataXtrack',
     1indexmap)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swdefimap(swid2, "GeoXtrack",
     1"DataXtrack",indexmap)'
         write(*,*)'Value returned by he5_swdefimap ',status
         write(1,*)'status=he5_swdefimap(swid2, "GeoXtrack",
     1"DataXtrack",indexmap)'
         write(1,*)'Value returned by he5_swdefimap ',status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swdetach
c
      write(1,27)
      write(*,*)'Testing he5_swdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdetach '
      write(1,*)'====================='
      status= he5_swdetach(swid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid2)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid2)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swclose(swfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)       
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swclose(swfid)'
         write(*,*)'Value returned by he5_swclose ',status
         write(1,*)'status= he5_swclose(swfid)'
         write(1,*)'Value returned by he5_swclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      swfid=he5_swopen('swath.he5',HE5F_ACC_RDWR)
      if (swfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swfid=he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_swopen ',swfid
         write(1,*)'swfid=he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_swopen ',swfid
      endif
      write(*,*)' '
      write(1,*)' '
 
c Test Case -- he5_swattach
c
      write(1,27)
      write(*,*)'Testing he5_swattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_swattach '
      write(1,*)'====================='

      swid=he5_swattach(swfid,'Simplef')
      if (swid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid= he5_swattach(swfid, "Simplef")'
         write(*,*)'Value returned by he5_swattach ',swid
         write(1,*)'swid= he5_swattach(swfid, "Simplef")'
         write(1,*)'Value returned by he5_swattach ',swid
      endif
      write(*,*)' '
      write(1,*)' '

      swid2=he5_swattach(swfid,'Indexedf')
      if (swid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(*,*)'Value returned by he5_swattach ',swid2
         write(1,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(1,*)'Value returned by he5_swattach ',swid2
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swwrfld
c
      write(1,27)
      write(*,*)'Testing he5_swwrfld '
      write(*,*)'==================='
      write(1,*)'Testing he5_swwrfld '
      write(1,*)'==================='

      status=he5_swwrfld(swid,'SideA',sd_start,stride, 
     1rd_edge, ray4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Write data field SideA '
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'Write data field SideA '
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid,'Temperature',start,stride, 
     1dataedge, ray4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrfld(swid,"Temperature",start, 
     1stride,dataedge, ray1)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status=he5_swwrfld(swid,"Temperature",start, 
     1stride,dataedge, ray1)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid,'Conduction',start,stride, 
     1dataedge, ray2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrfld(swid,"Conduction",start, 
     1stride,dataedge, ray2)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status=he5_swwrfld(swid,"Conduction",start, 
     1stride,dataedge, ray2)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid,'Latitude',geostart,stride,
     1geoedge,lat)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swwrfld(swid, "Latitude", geostart, 
     1stride, geoedge, lat)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status= he5_swwrfld(swid, "Latitude", geostart, 
     1stride, geoedge, lat)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid,'Longitude',geostart,stride, 
     1geoedge,lon)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swwrfld(swid,"Longitude",geostart,
     1stride,geoedge, lon)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status= he5_swwrfld(swid,"Longitude",geostart,
     1stride,geoedge, lon)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid,'Time',geostart,stride,geoedge,
     1twodtime)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrfld(swid,"Time",geostart,stride, 
     1geoedge, twodtime)'
         write(*,*)'Value returned by he5_swwrfld for Time ',status
         write(1,*)'status= he5_swwrfld(swid, "Time", geostart, stride, 
     1geoedge, twodtime)'
         write(1,*)'Value returned by he5_swwrfld for Time ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid2,'Latitude',istart,stride,
     1iedge,idxlat)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)       
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swwrfld(swid2,"Latitude", 
     1istart,stride,iedge,idxlat)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status= he5_swwrfld(swid2,"Latitude",
     1istart,stride,iedge,lat)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid2,'Longitude',istart,stride, 
     1iedge,idxlon)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrfld(swid2,"Longitude", 
     1istart,stride,iedge,idxlon)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status= he5_swwrfld(swid2,"Longitude",
     1istart,stride,iedge,idxlon)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_swwrfld(swid2,'Time',tstart,tstride,
     1tedge,onedtime)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Write field Time '
         write(*,*)'Status returned by he5_swwrfld ',status
         write(1,*)'Write field Time '
         write(1,*)'Status returned by he5_swwrfld ',status
         write(*,*)' '
         write(1,*)' '
      endif

      iedge(1)=40
      iedge(2)=40
      status=he5_swwrfld(swid2,'Sensor1',istart,stride, 
     1iedge,ray3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swwrfld(swid2, "Sensor1", 
     1istart, stride, iedge, ray3)'
         write(*,*)'Value returned by he5_swwrfld ',status
         write(1,*)'status= he5_swwrfld(swid2, "Sensor1", 
     1istart, stride, iedge, ray3)'
         write(1,*)'Value returned by he5_swwrfld ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swwrlattr
c
      write(1,27)
      write(*,*)'Testing he5_swwrlattr, Local Attribute '
      write(*,*)'================================== '
      write(1,*)'Testing he5_swwrlattr, Local Attribute '
      write(1,*)'================================== '
      status=he5_swwrlattr(swid2,'Sensor1','local',
     1HE5T_NATIVE_FLOAT,count,locattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrlattr(swid2,"Sensor1","local",
     1HE5T_NATIVE_FLOAT,count,locattr)'
         write(*,*)'Value returned by he5_swwrlattr ',status
         write(1,*)'status=he5_swwrlattr(swid2,"Sensor1", "local", 
     1HE5T_NATIVE_FLOAT,count,locattr)'
         write(1,*)'Value returned by he5_swwrlattr ',status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swwrattr
c
      count(1) = 4

      write(1,27)
      write(*,*)'Testing he5_swwrattr, Global Attribute '
      write(*,*)'====================================== '
      write(1,*)'Testing he5_swwrattr, Global Attribute '
      write(1,*)'======================================= '
      status=he5_swwrattr(swid,'Drift',HE5T_NATIVE_INT,count, 
     1global_attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrattr(swid, "Drift", 
     1HE5T_NATIVE_INT, count, global_attr)'
         write(*,*)'Value returned by he5_swwrattr ',status
         write(1,*)'status=he5_swwrattr(swid, "Drift", 
     1HE5T_NATIVE_INT, count, global_attr)'
         write(1,*)'Value returned by he5_swwrattr ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swwrgattr
c
      count(1) = 3
     
      write(1,27)
      write(*,*)'Testing he5_swwrgattr, Group Attribute '
      write(*,*)'====================================== '
      write(1,*)'Testing he5_swwrgattr, Group Attribute '
      write(1,*)'====================================== '
      status=he5_swwrgattr(swid,'group',HE5T_NATIVE_FLOAT, 
     1count,grpattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swwrgattr(swid, "group", 
     1HE5T_NATIVE_FLOAT,count,grpgattr)'
         write(*,*)'Value returned by he5_swwrgattr ',status
         write(1,*)'status=he5_swwrgattr(swid, "group", 
     1HE5T_NATIVE_FLOAT,count,grpattr)'
         write(1,*)'Value returned by he5_swwrgattr ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
      write(*,*)'status= he5_swdetach(swid)'
      write(*,*)'Value returned by he5_swdetach ',status
      write(1,*)'status= he5_swdetach(swid)'
      write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid2)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid2)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swclose
c
      write(1,27)
      write(*,*)'Testing he5_swclose '
      write(*,*)'==================='
      write(1,*)'Testing he5_swclose '
      write(1,*)'==================='
      status= he5_swclose(swfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_swclose ',status
         write(1,*)'status= he5_swclose(swfid)'
         write(1,*)'Value returned by he5_swclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      swfid= he5_swopen('swath.he5',HE5F_ACC_RDWR)
      if (swfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_swopen ', swfid
         write(1,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_swopen ', swfid
      endif
      write(*,*)' '
      write(1,*)' '

      swid= he5_swattach(swfid,'Simplef')
      if (swid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid= he5_swattach(swfid, "Simplef")'
         write(*,*)'Value returned by he5_swattach ',swid
         write(1,*)'swid= he5_swattach(swfid, "Simplef")'
         write(1,*)'Value returned by he5_swattach ',swid
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swclose(swfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swclose(swfid)'
         write(*,*)'Value returned by he5_swclose ',status
         write(1,*)'status= he5_swclose(swfid)'
         write(1,*)'Value returned by he5_swclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      swfid= he5_swopen('swath.he5',HE5F_ACC_RDWR)
      if (swfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_swopen ',swfid
         write(1,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_swopen ',swfid
      endif
      write(*,*)' '
      write(1,*)' '

      swid=he5_swattach(swfid,'Simplef')
      if (swid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid= he5_swattach(swfid, "Simplef")'
         write(*,*)'Value returned by he5_swattach ',swid
         write(1,*)'swid= he5_swattach(swfid, "Simplef")'
         write(1,*)'Value returned by he5_swattach ',swid
      endif
      write(*,*)' '
      write(1,*)' '

      swid2=he5_swattach(swfid,'Indexedf')
      if (swid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(*,*)'Value returned by he5_swattach ',swid2
         write(1,*)'swid2= he5_swattach(swfid, "Indexedf")'
         write(1,*)'Value returned by he5_swattach ',swid2
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefvrtreg
c
      write(1,27)
      write(*,*)'Testing he5_swdefvrtreg'
      write(*,*)'======================='
      write(1,*)'Testing he5_swdefvrtreg'
      write(1,*)'======================='

      regionid=he5_swdefvrtreg(swid2,HE5_HDFE_NOPREVSUB,'Time',range)
      if (regionid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define region id for field Time '
         write(*,*)'Region id returned by he5_swdefvrtreg ',
     1regionid
         write(1,*)'Define region id for field Time '
         write(1,*)'Region id returned by he5_swdefvrtreg ',
     1regionid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swnentries
c
      write(1,27)
      write(*,*)'Testing he5_swnentries '
      write(*,*)'====================='
      write(1,*)'Testing he5_swnentries '
      write(1,*)'====================='

      nmaps=he5_swnentries(swid,1,strbufsize)
      if (nmaps .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nmaps= he5_swnentries(swid, 1, strbufsize)'
         write(*,*)'Number of dimension mappings ',nmaps
         write(*,*)'Size of strbufsize ',strbufsize
         write(1,*)'nmaps= he5_swnentries(swid, 1, strbufsize)'
         write(1,*)'Number of dimension mappings ',nmaps
         write(1,*)'Size of strbufsize ',strbufsize
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swinqmaps
c
      write(1,27)
      write(*,*)'Testing he5_swinqmaps '
      write(*,*)'====================='
      write(1,*)'Testing he5_swinqmaps '
      write(1,*)'====================='
      offset = -1
      inc    = -1
      nmaps=he5_swinqmaps(swid, dimmap, offset, inc)
      if (nmaps .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nmaps=he5_swinqmaps(swid,dimmap,offset,inc)'
         write(*,*)'Dimension mappings ',dimmap
         write(*,*)'offset ',offset
         write(*,*)'increment ',inc
         write(1,*)'nmaps=he5_swinqmaps(swid,dimmap,offset,inc)'
         write(1,*)'Dimension mappings ',dimmap
         write(1,*)'offset ',offset
         write(1,*)'increment ',inc
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swgmapinfo
c
      write(1,27)
      write(*,*)'Testing he5_swgmapinfo '
      write(*,*)'======================'
      write(1,*)'Testing he5_swgmapinfo '
      write(1,*)'======================'
      status = he5_swgmapinfo(swid2,'GeoTrack')
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status = he5_swgmapinfo(swid2, "GeoTrack")'
         write(1,*)'status = he5_swgmapinfo(swid2, "GeoTrack")'
         write(*,*)'Status returned by he5_swgmapinfo ',status
         write(1,*)'Status returned by he5_swgmapinfo ',status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swinqimaps
c
      write(1,27)
      write(*,*)'Testing he5_swinqimaps '
      write(*,*)'====================='
      write(1,*)'Testing he5_swinqimaps '
      write(1,*)'====================='
      nmaps=he5_swinqimaps(swid2, idxdimmap, ndxmap)
      if (nmaps .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nmaps=he5_swinqimaps(swid2,idxdimmap,ndxmap)'
         write(*,*)'Number of indexed mappings ',nmaps
         write(*,*)'Indexed Dimension mappings ',idxdimmap
         write(*,*)'Index size ',ndxmap
         write(1,*)'nmaps=he5_swinqimaps(swid2,idxdimmap,ndxmap)'
         write(1,*)'Number of indexed mappings ',nmaps
         write(1,*)'Indexed Dimension mappings ',idxdimmap
         write(1,*)'Index size ',ndxmap
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swinqdims
c
      write(1,27)
      write(*,*)'Testing he5_swinqdims '
      write(*,*)'====================='
      write(1,*)'Testing he5_swinqdims '
      write(1,*)'====================='
      ndims=he5_swinqdims(swid,dimname,dims)
      if (ndims .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'ndims= he5_swinqdims(swid, dimname, dims)'
         write(*,*)'Number of dimesions ',ndims
         write(*,*)'Names of dimensions ',dimname
         write(*,*)'Size of dimensions ',dims
         write(1,*)'ndims= he5_swinqdims(swid, dimname, dims)'
         write(1,*)'Number of dimesions ',ndims
         write(1,*)'Names of dimensions ',dimname
         write(1,*)'Size of dimensions ',dims
      endif
      write(*,*)' '
      write(1,*)' '

      ndims= he5_swinqdims(swid2, dimname, dims)
      if (ndims .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'ndims=he5_swinqdims(swid2,dimname,dims)'
         write(*,*)'Number of dimesions ',ndims
         write(*,*)'Names of dimensions ',dimname
         write(*,*)'Size of dimensions ',dims
         write(1,*)'ndims=he5_swinqdims(swid2,dimname,dims)'
         write(1,*)'Number of dimesions ',ndims
         write(1,*)'Names of dimensions ',dimname
         write(1,*)'Size of dimensions ',dims
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swinqgflds
c
      write(1,27)
      write(*,*)'Testing he5_swinqgflds '
      write(*,*)'======================'
      write(1,*)'Testing he5_swinqgflds '
      write(1,*)'======================'

      nflds= he5_swinqgflds(swid,fieldlist,rnk,ntype)
      if (nflds .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nflds=he5_swinqgflds(swid,fieldlist,rnk,ntype)'
         write(*,*)'Number of geolocation fields in swath ',nflds
         write(*,*)'Names of geolocation fields ',fieldlist
         write(*,*)'Rank of fields ',rnk
         write(*,*)'Number type of fields ',ntype
         write(1,*)'nflds=he5_swinqgflds(swid,fieldlist,rnk,ntype)'
         write(1,*)'Number of geolocation fields in swath ',nflds
         write(1,*)'Names of geolocation fields ',fieldlist
         write(1,*)'Rank of fields ',rnk
         write(1,*)'Number type of fields ',ntype
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swinqdflds
c
      write(1,27)
      write(*,*)'Testing he5_swinqdflds '
      write(*,*)'======================'
      write(1,*)'Testing he5_swinqdflds '
      write(1,*)'======================'
      nflds=he5_swinqdflds(swid,fieldlist,rnkd,ntyped)
      if (nflds .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nflds=he5_swinqdflds(swid,fieldlist,rnkd,ntyped)'
         write(*,*)'Number of data fields in swath ',nflds
         write(*,*)'Names of data fields ',fieldlist
         write(*,*)'Rank of fields ',rnkd
         write(*,*)'Number type of fields ',ntyped
         write(1,*)'nflds=he5_swinqdflds(swid,fieldlist,rnkd,ntyped)'
         write(1,*)'Number of data fields in swath ',nflds
         write(1,*)'Names of data fields ',fieldlist
         write(1,*)'Rank of fields ',rnkd
         write(1,*)'Number type of fields ',ntyped
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdiminfo
c
      write(1,27)
      write(*,*)'Testing he5_swdiminfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdiminfo '
      write(1,*)'====================='
      dimsize= he5_swdiminfo(swid,'DataTrack')
      if (dimsize .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'dimsize= he5_swdiminfo(swid, "DataTrack")'
         write(*,*)'Size of dimension DataTrack ',dimsize
         write(1,*)'dimsize= he5_swdiminfo(swid, "DataTrack")'
         write(1,*)'Size of dimension DataTrack ',dimsize
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swrdfld
c
      write(1,27)
      write(*,*)'Testing he5_swrdfld '
      write(*,*)'==================='
      write(1,*)'Testing he5_swrdfld '
      write(1,*)'==================='

      rd_start(1)=0
      rd_start(2)=0
      rd_stride(1)=1
      rd_stride(2)=1
      rd_edge(1)=40
      rd_edge(2)=100
      status= he5_swrdfld(swid,'Conduction',rd_start,
     1rd_stride,rd_edge,temp)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_swrdfld ',status
         write(*,*)'status= he5_swrdfld(swid, "Conduction", 
     1rd_start,rd_stride, rd_edge, temp)'
         write(1,*)'status= he5_swrdfld(swid, "Conduction", 
     1rd_start,rd_stride, rd_edge, temp)'
         write(1,*)'Value returned by he5_swrdfld ',status
         do 190 index1=10,15
            do 180 index2=10,15
               write(*,*)'value of conduction ',temp(index1,index2)
               write(1,*)'value of conduction ',temp(index1,index2)
  180       continue
  190    continue
      endif

      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swfldinfo
c
      write(1,27)
      write(*,*)'Testing he5_swfldinfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_swfldinfo '
      write(1,*)'====================='
      status=he5_swfldinfo(swid,'Latitude',rank,latdims,nt,
     1dimens,maxdims)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swfldinfo(swid, "Latitude", 
     1rank, latdims, nt, dimens, maxdims)'
         write(*,*)'Value returned by he5_swfldinfo ',status
         write(*,*)'Rank of field latitude ',rank
         write(*,*)'dimension list of field latitude ',dimens
         write(*,*)'Size of latitude dimensions ',latdims
         write(*,*)'number type of field latitude ',nt
         write(1,*)'status= he5_swfldinfo(swid, "Latitude", 
     1rank, latdims, nt, dimens, maxdims)'
         write(1,*)'Value returned by he5_swfldinfo ',status
         write(1,*)'Rank of field latitude ',rank
         write(1,*)'dimension list of field latitude ',dimens
         write(1,*)'Size of latitude dimensions ',latdims
         write(1,*)'number type of field latitude ',nt
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swmapinfo
c
      write(1,27)
      write(*,*)'Testing he5_swmapinfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_swmapinfo '
      write(1,*)'====================='
      status=he5_swmapinfo(swid,'GeoTrack','DataTrack',offset,inc)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swmapinfo(swid,"GeoTrack","DataTrack", 
     1offset,inc)'
         write(*,*)'Value returned by he5_swmapinfo ',status
         write(*,*)'Offset ',offset,' Increment ',inc
         write(1,*)'status=he5_swmapinfo(swid,"GeoTrack","DataTrack", 
     1offset,inc)'
         write(1,*)'Value returned by he5_swmapinfo ',status
         write(1,*)'Offset ',offset,' Increment ',inc
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swimapinfo
c
      write(1,27)
      write(*,*)'Testing he5_swimapinfo '
      write(*,*)'======================'
      write(1,*)'Testing he5_swimapinfo '
      write(1,*)'======================'
      status4=he5_swimapinfo(swid2,'GeoXtrack','DataXtrack',idxmap)
      if (status4 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status4= he5_swimapinfo(swid2,"GeoXtrack",
     1"DataXtrack", idxmap)'
         write(*,*)'Value returned by he5_swimapinfo ',status4
         do 980 index1=1,30
            write(*,*)' Value of index array ',idxmap(index1)
            write(1,*)' Value of index array ',idxmap(index1)
  980    continue
         write(1,*)'status4= he5_swimapinfo(swid2, "GeoXtrack", 
     1"DataXtrack",idxmap)'
         write(1,*)'Value returned by he5_swimapinfo ',status4
         write(1,*)'Indexed map values ',idxmap
      endif
      write(*,*)' '
      write(1,*)' '
 
c Test Case -- he5_swinqattrs
c
      write(1,27)
      write(*,*)'Testing he5_swinqattrs Global attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_swinqattrs Global attributes '
      write(1,*)'==================================== '

      nattr = he5_swinqattrs(swid,attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Number of global attributes in swath ',nattr
         write(*,*)'Attribute list of swath ',attrlist
         write(1,*)'Number of global attributes in swath ',nattr
         write(1,*)'Attribute list of swath ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swattrinfo
c
      write(1,27)
      write(*,*)'Testing he5_swattrinfo Global attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_swattrinfo Global attribute'
      write(1,*)'======================================='
      status= he5_swattrinfo(swid, attrlist, n, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swattrinfo(swid,attrlist,n,size)'
         write(*,*)'Value returned by he5_swattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'status=he5_swattrinfo(swid,attrlist,n,size)'
         write(1,*)'Value returned by he5_swattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swrdattr
c
      write(1,27)
      write(*,*)'Testing he5_swrdattr, Global attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_swrdattr, Global attribute '
      write(1,*)'======================================'

      status= he5_swrdattr(swid, attrlist, rdattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swrdattr(swid,attrlist,rdattr)'
         write(*,*)'Value returned by he5_swrdattr ',status
         write(*,*)'attribute ',rdattr
         write(1,*)'status=he5_swrdattr(swid,attrlist,rdattr)'
         write(1,*)'Value returned by he5_swrdattr ',status
         write(1,*)'attribute ',rdattr
      endif
      write(*,*)' '
      write(1,*)' '

c     Test Case -- he5_swinqgattrs
c
      write(1,27)
      write(*,*)'Testing he5_swinqgattrs Group attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_swinqgattrs Group attributes '
      write(1,*)'==================================== '

      nattr = he5_swinqgattrs(swid,attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Number of group attributes in swath ',nattr
         write(*,*)'Attribute list of swath ',attrlist
         write(1,*)'Number of group attributes in swath ',nattr
         write(1,*)'Attribute list of swath ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swgattrinfo
c
      write(1,27)
      write(*,*)'Testing he5_swgattrinfo Group attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_swgattrinfo Group attribute'
      write(1,*)'======================================='
      status= he5_swgattrinfo(swid, attrlist, n, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swgattrinfo(swid,attrlist,n,size)'
         write(*,*)'Value returned by he5_swgattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'status=he5_swgattrinfo(swid,attrlist,n,size)'
         write(1,*)'Value returned by he5_swgattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swrdgattr
c
      write(1,27)
      write(*,*)'Testing he5_swrdgattr, Group attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_swrdgattr, Group attribute '
      write(1,*)'======================================'

       status= he5_swrdgattr(swid,'group',rdgnlattr)
       if (status .lt. zero) then
          write(*,230)
          write(1,230)
       else
          write(*,*)'  '
          write(*,240)
          write(1,*)'  '
          write(1,240)
          write(*,*)'status=he5_swrdgattr(swid,attrlist,rdgnlattr)'
          write(*,*)'Value returned by he5_swrdgattr ',status
          write(*,*)'attribute ',rdgnlattr
          write(1,*)'status=he5_swrdgattr(swid,attrlist,rdgnlattr)'
          write(1,*)'Value returned by he5_swrdgattr ',status
          write(1,*)'attribute ',rdgnlattr
       endif
       write(*,*)' '
       write(1,*)' '

c Test Case -- he5_swinqlattrs
c
      write(1,27)
      write(*,*)'Testing he5_swinqlattrs Local attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_swinqlattrs Local attributes '
      write(1,*)'==================================== '

      nattr=he5_swinqlattrs(swid2,'Sensor1',attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swinqlattrs(... "Sensor1" ...)'
         write(*,*)'Number of local attributes in field',nattr
         write(*,*)'Attribute list of field ',attrlist
         write(1,*)'swinqlattrs(... "Sensor1" ...)'
         write(1,*)'Number of local attributes in field',nattr
         write(1,*)'Attribute list of field ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swlattrinfo
c
      write(1,27)
      write(*,*)'Testing he5_swlattrinfo Local attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_swlattrinfo Local attribute'
      write(1,*)'======================================='
      status=he5_swlattrinfo(swid2,'Sensor1','local',n,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swlattrinfo(... "Sensor1" ...)'
         write(*,*)'Value returned by he5_swlattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'swlattrinfo(... "Sensor1" ...)'
         write(1,*)'Value returned by he5_swlattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swidtype
c
      write(1,27)
      write(*,*)'Testing he5_swidtype, Inquire Datatype '
      write(*,*)'======================================='
      write(1,*)'Testing he5_swidtype, Inquire Datatype '
      write(1,*)'======================================='

      status= he5_swidtype(swid2,'Sensor1','local', 
     1HE5_HDFE_LOCATTRGROUP,dtype,classid, order, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swidtype(swid2,"Sensor1","local", 
     1HE5_HDFE_LOCATTRGROUP,dtype, classid, order, size)'
         write(*,*)'Value returned by he5_swidtype ',status
         write(*,*)'Datatype of local attribute ',dtype
         write(*,*)'Class id and order ',classid, order
         write(*,*)'Size of attribute ',size
         write(1,*)'status= he5_swidtype(swid2,"Sensor1","local", 
     1HE5_HDFE_LOCATTRGROUP,dtype, classid, order, size)'
         write(1,*)'Value returned by he5_swidtype ',status
         write(1,*)'Datatype of local attribute ',dtype
         write(1,*)'Class id and order ',classid, order
         write(1,*)'Size of attribute ',size
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swrdlattr
c
      write(1,27)
      write(*,*)'Testing he5_swrdlattr, Local attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_swrdlattr, Local attribute '
      write(1,*)'======================================'

      status=he5_swrdlattr(swid2,'Sensor1','local',rdgnlattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swrdlattr(swid2,"Sensor1","local", 
     1rdgnlattr)'
         write(*,*)'Value returned by he5_swrdlattr ',status
         write(*,*)'attribute ',rdgnlattr
         write(1,*)'status=he5_swrdlattr(swid2,"Sensor1","local", 
     1rdgnlattr)'
         write(1,*)'Value returned by he5_swrdlattr ',status
         write(1,*)'attribute ',rdgnlattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swgetfill
c
      write(1,27)
      write(*,*)'Testing he5_swgetfill '
      write(*,*)'====================='
      write(1,*)'Testing he5_swgetfill '
      write(1,*)'====================='
      status= he5_swgetfill(swid,'Conduction', rdfilval)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swgetfill(swid,"Conduction",rdfilval)'
         write(*,*)'Value returned by he5_swgetfill ',status
         write(*,*)'Value of fill ',rdfilval
         write(1,*)'status=he5_swgetfill(swid,"Conduction",rdfilval)'
         write(1,*)'Value returned by he5_swgetfill ',status
         write(1,*)'Value of fill ',rdfilval
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdefboxreg
c
      write(1,27)
      write(*,*)'Testing he5_swdefboxreg '
      write(*,*)'======================='
      write(1,*)'Testing he5_swdefboxreg '
      write(1,*)'======================='

      regionid= he5_swdefboxreg(swid, cornerlon, cornerlat, 
     1HE5_HDFE_ENDPOINT)
      if (regionid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'regionid= he5_swdefboxreg(swid,cornerlon,
     1cornerlat, HE5_HDFE_ENDPOINT)'
         write(*,*)'Value returned by he5_swdefboxreg ',regionid
         write(1,*)'regionid= he5_swdefboxreg(swid, cornerlon, 
     1cornerlat, HE5_HDFE_ENDPOINT)'
         write(1,*)'Value returned by he5_swdefboxreg ',regionid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swregidx
c
      write(1,27)
      write(*,*)'Testing he5_swregidx '
      write(*,*)'===================='
      write(1,*)'Testing he5_swregidx '
      write(1,*)'===================='

      regionid3=he5_swregidx(swid, cornerlon, cornerlat, 
     1HE5_HDFE_ENDPOINT, geodim, idxrange)
      if (regionid3 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'regionid3= he5_swregidx(swid, cornerlon, 
     1cornerlat, HE5_HDFE_ENDPOINT, geodim, idxrange)'
         write(*,*)'Region ID returned by he5_swregdex ',regionid3
         write(*,*)'Geodim of region ',geodim
         write(*,*)'indicies of region ',idxrange(1),idxrange(2)
         write(1,*)'regionid3= he5_swregidx(swid, cornerlon, 
     1cornerlat, HE5_HDFE_ENDPOINT, geodim, idxrange)'
         write(1,*)'Region ID returned by he5_swregdex ',regionid3
         write(1,*)'Geodim of region ',geodim
         write(1,*)'indicies of region ',idxrange(1),idxrange(2)
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swdupreg
c
      write(1,27)
      write(*,*)'Testing he5_swdupreg '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdupreg '
      write(1,*)'====================='

      regionid2= he5_swdupreg(regionid)
      if (regionid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Duplicate region id information '
         write(*,*)'Region ID returned by he5_swdupreg ',regionid2
         write(1,*)'Duplicate region id information '
         write(1,*)'Region ID returned by he5_swdupreg ',regionid2
         write(*,*)' '
         write(1,*)' '
      endif

c Test Case -- he5_swreginfo
c
      write(1,27)
      write(*,*)'Testing he5_swreginfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_swreginfo '
      write(1,*)'====================='
      status=he5_swreginfo(swid,regionid,'Temperature',nt,rank, 
     1dims,size)      
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swreginfo(swid,regionid,"Temperature", 
     1nt, rank, dims, size)'
         write(*,*)'Value returned by he5_swreginfo ',status
         write(*,*)'Number type of region ',nt,' Rank of region ',rank
         write(*,*)'Dimensions of region ',dims(1),' ',dims(2)
         write(*,*)'Value of size ',size
         write(1,*)'status=he5_swreginfo(swid,regionid,"Temperature", 
     1nt, rank, dims, size)'
         write(1,*)'Value returned by he5_swreginfo ',status
         write(1,*)'Number type of region ',nt,' Rank of region ',rank
         write(1,*)'Value of size ',size
         write(1,*)'Dimensions of region ',dims(1),' ',dims(2)
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swextreg
c
      write(1,27)
      write(*,*)'Testing he5_swextreg '
      write(*,*)'====================='
      write(1,*)'Testing he5_swextreg '
      write(1,*)'====================='
      status=he5_swextreg(swid,regionid,'Temperature', 
     1HE5_HDFE_INTERNAL, tempbuf) 
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status = he5_swextreg(swid, regionid, 
     1"Temperature", HE5_HDFE_INTERNAL, tempbuf)'
         write(*,*)'Value returned by he5_swextreg ', status
         write(1,*)'status = he5_swextreg(swid, regionid, 
     1"Temperature", HE5_HDFE_INTERNAL,tempbuf)'
         write(1,*)'Value returned by he5_swextreg ', status
         do 210 index1=100,115
            write(*,*)'region value at position ',tempbuf(index1)
            write(1,*)'region value at position ',tempbuf(index1)
  210    continue
      endif

      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swdeftmeper
c
      write(1,27)
      write(*,*)'Testing he5_swdeftmeper '
      write(*,*)'====================='
      write(1,*)'Testing he5_swdeftmeper '
      write(1,*)'====================='
      regionid = he5_swdeftmeper(swid, starttime, endtime, 
     1HE5_HDFE_ENDPOINT)
      if (regionid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'regionid= he5_swdeftmeper(swid, starttime, 
     1endtime, HE5_HDFE_ENDPOINT)'
         write(*,*)'Value returned by he5_swdeftmeper ',regionid
         write(1,*)'regionid= he5_swdeftmeper(swid, starttime, 
     1endtime, HE5_HDFE_ENDPOINT)'
         write(1,*)'Value returned by he5_swdeftmeper ',regionid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swperinfo
c
      write(1,27)
      write(*,*)'Testing he5_swperinfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_swperinfo '
      write(1,*)'====================='
      status = he5_swperinfo(swid,regionid,'Temperature',nt,rank,
     1dims,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status = he5_swperinfo(swid, regionid,
     1"Temperature",nt, rank, dims, size)'
         write(*,*)'Value returned by he5_swperinfo ',status
         write(*,*)'Rank of field ',rank,' Number type of region ',nt
         write(*,*)'Dimensions of region ',dims(1),' ',dims(2)
         write(*,*)'Value of size ',size
         write(1,*)'status = he5_swperinfo(swid, regionid,
     1"Temperature",nt, rank, dims, size)'
         write(1,*)'Value returned by he5_swperinfo ',status
         write(1,*)'Rank of field ',rank,' Number type of region ',nt
         write(1,*)'Dimensions of region ',dims(1),' ',dims(2)
         write(1,*)'Value of size ',size
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swextper
c
      write(1,27)
      write(*,*)'Testing he5_swextper '
      write(*,*)'===================='
      write(1,*)'Testing he5_swextper '
      write(1,*)'===================='
      status= he5_swextper(swid,regionid,'Temperature', 
     1HE5_HDFE_INTERNAL,tempbuf)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swextper(swid, regionid, 
     1"Temperature", HE5_HDFE_INTERNAL, tempbuf)'
         write(*,*)'Value returned by he5_swextper swath ',status
         write(1,*)'status= he5_swextper(swid, regionid, 
     1"Temperature", HE5_HDFE_INTERNAL, tempbuf) '
         write(1,*)'Value returned by he5_swextper swath ',status
         do 220 index1=1000,1015
            write(*,*)'region value at position ',tempbuf(index1)
            write(1,*)'region value at position ',tempbuf(index1)
  220    continue
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_swupimap
c
      write(1,27)
      write(*,*)'Testing he5_swupimap '
      write(*,*)'====================='
      write(1,*)'Testing he5_swupimap '
      write(1,*)'====================='
      idxsz=he5_swupimap(swid2,regionid,idxmap,upidxmap,ndxmap)
      if (idxsz .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Size of index array in region ',idxsz
         write(1,*)'Size of index array in region ',idxsz
         write(*,*)'Indicies of updated index ',ndxmap(1),ndxmap(2)
         write(1,*)'Indicies of updated index ',ndxmap(1),ndxmap(2)
         do 380 index1=1,idxsz
            write(*,*)' Value of index array ',upidxmap(index1)
            write(1,*)' Value of index array ',upidxmap(index1)
  380    continue
      endif



c Test Case -- he5_swcompinfo
c
      write(1,27)
      write(*,*)'Testing he5_swcompinfo '
      write(*,*)'======================='
      write(1,*)'Testing he5_swcompinfo '
      write(1,*)'======================='
      status=he5_swcompinfo(swid,'CompChunk',compcode,rdcompparm)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_swcompinfo(swid,"CompChunk",compcode, 
     1rdcompparm)'
         write(*,*)'Status returned by he5_swcompinfo ',status
         write(*,*)'Compression code ',compcode
         write(*,*)'Compression parameter ',rdcompparm(1)
         write(1,*)'status=he5_swcompinfo(swid,"CompChunk",compcode, 
     1rdcompparm)'
         write(1,*)'Status returned by he5_swcompinfo ',status
         write(1,*)'Compression code ',compcode
         write(1,*)'Compression parameter ',rdcompparm(1)
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swdetach(swid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swdetach(swid2)'
         write(*,*)'Value returned by he5_swdetach ',status
         write(1,*)'status= he5_swdetach(swid2)'
         write(1,*)'Value returned by he5_swdetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swclose(swfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swclose(swfid)'
         write(*,*)'Value returned by he5_swclose ',status
         write(1,*)'status= he5_swclose(swfid)'
         write(1,*)'Value returned by he5_swclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      swfid= he5_swopen('swath.he5',HE5F_ACC_RDWR)
      if (swfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_swopen ',swfid
         write(1,*)'swfid= he5_swopen("swath.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_swopen ',swfid
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_swinqswath
c
      write(1,27)
      write(*,*)'Testing he5_swinqswath '
      write(*,*)'======================'
      write(1,*)'Testing he5_swinqswath '
      write(1,*)'======================'

      nswath=he5_swinqswath('swath.he5',swathlist,strbufsz2)
      if (nswath .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Retrieve information on swaths in 
     1file swath.he5'
         write(*,*)'Number of swaths in file ', nswath
         write(*,*)'List of swaths ',swathlist
         write(1,*)'Retrieve information on swaths in 
     1file swath.he5'
         write(1,*)'Number of swaths in file ', nswath
         write(1,*)'List of swaths ',swathlist
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_swclose(swfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_swclose(swfid)'
         write(*,*)'Value returned by he5_swclose ',status
         write(1,*)'status= he5_swclose(swfid)'
         write(1,*)'Value returned by he5_swclose ',status
      endif
      write(*,*)' '
      write(1,*)' '

      close(unit=1)
   27 format(' ')
      stop
      end












