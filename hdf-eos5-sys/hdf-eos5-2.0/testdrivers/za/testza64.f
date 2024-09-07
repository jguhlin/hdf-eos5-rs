!----------------------------------------------------------------------------
!     Copyright (C) 2002   Emergent IT Inc. and Raytheon Systems Company    |
!----------------------------------------------------------------------------
! testza64.f                                                                |
! In this program we create, define and write a simple za hdfeos file       |
! using the za interface                                                    |
!----------------------------------------------------------------------------

      program   testza64
 
      implicit  none
 
      include   'hdfeos5.inc'
 
      integer   status
      integer   index1
      integer   index2
      integer   zafid
      integer   zaid
      integer   zaid2
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
      integer   ntyped(4)
      integer   rnkd(4)

      integer   he5_zaopen
      integer   he5_zacreate
      integer   he5_zaattach
      integer   he5_zadefdim
      integer   he5_zadefine
      integer   he5_zadefchunk
      integer   he5_zadefcomp
      integer   he5_zasetfill
      integer   he5_zawrattr
      integer   he5_zawrlattr
      integer   he5_zawrite
      integer   he5_zaread
      integer   he5_zawrgattr
      integer   he5_zardattr
      integer   he5_zardgattr
      integer   he5_zardlattr
      integer   he5_zainfo 
      integer   he5_zacompinfo
      integer   he5_zaattrinfo
      integer   he5_zagattrinfo
      integer   he5_zalattrinfo
      integer   he5_zagetfill
      integer   he5_zaidtype
      integer   he5_zadetach
      integer   he5_zaclose 

      integer*8 he5_zainqza
      integer*8 he5_zanentries
      integer*8 he5_zainqdims
      integer*8 he5_zadiminfo
      integer*8 he5_zainqattrs
      integer*8 he5_zainqgattrs
      integer*8 he5_zainqlattrs
      integer*8 he5_zainquire
      integer*8 size
      integer*8 strbufsize
      integer*8 ndims
      integer*8 chunkdims(1)
      integer*8 start(2)
      integer*8 stride(2)
      integer*8 dataedge(2)
      integer*8 istart(2)
      integer*8 iedge(2)
      integer*8 dedge(2)
      integer*8 sd_start(2)
      integer*8 sd_stride(2)
      integer*8 sd_edge(2)
      integer*8 rd_start(2)
      integer*8 rd_stride(2)
      integer*8 rd_edge(2)
      integer*8 dimsize
      integer*8 nattr
      integer*8 nza
      integer*8 nmaps
      integer*8 strbufsz2
      integer*8 nflds
      integer*8 count(1)
      integer*8 zero
      integer*8 dims(2)
      integer*8 dims2(3)
      integer*8 sddims(2)
      integer*8 dim1
      integer*8 dim2
      integer*8 dim3
      integer*8 dim4

      real      grpattr(3)
      real      locattr(3)
      real      rdgnlattr(3)
      real      ray1(100,40)
      real      ray2(40,100)
      real      ray4(40,100)
      real      ray3(40,40)
      real      temp(40,100)
      real      raycnt
      real      fillvalue
      real      rdfilval
      real      tempbuf(4200)


      character*(256)  dimname
      character*(256)  fieldlist
      character*(256)  attrlist
      character*(256)  zalist
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
      istart(1)     = 0
      istart(2)     = 0
      iedge(1)      = 30
      iedge(2)      = 30
      dedge(1)      = 40
      dedge(2)      = 40
      rank          = 2
      fillvalue     = 16.0
      zero          = 0

!
! This section of the program just fills some arrays with data that will be
! used later in the program
!
      do 110 index1=1,100
         do 100 index2=1,40
            ray1(index1, index2)=raycnt
            ray2(index2, index1)=raycnt + 1.0
            ray4(index2,index1) = raycnt
            raycnt = raycnt +1.
  100    continue
  110 continue

      do 170 index1=1,40
         do 160 index2=1,40
            ray3(index1, index2)=raycnt
            raycnt = raycnt+1.25
  160    continue
  170 continue


  230 format('********Error unexpected**********')
  240 format('***********Passed Test*********')

      open(unit=1, file="testza64_f.txtout", status = "UNKNOWN")

      write(*,*)'Testing he5_za FORTRAN 77 interface'
      write(*,*)'======================================'
      write(1,*)'Testing he5_za FORTRAN 77 interface'
      write(1,*)'======================================'
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zaopen
!
      write(*,*)'Testing he5_zaopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_zaopen '
      write(1,*)'====================='
      zafid=he5_zaopen('za.he5',HE5F_ACC_TRUNC)
      if (zafid .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	zafid= he5_zaopen("za.he5",HE5F_ACC_TRUNC)'
         write(*,*)'	Value returned by he5_zaopen ', zafid
         write(1,*)'	zafid= he5_zaopen("za.he5",HE5F_ACC_TRUNC)'
         write(1,*)'	Value returned by he5_zaopen ', zafid
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zacreate
!
      write(1,27)
      write(*,*)'Testing he5_zacreate '
      write(*,*)'====================='
      write(1,*)'Testing he5_zacreate '
      write(1,*)'====================='
      zaid=he5_zacreate(zafid,'Simplef')
      if (zaid .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid=he5_zacreate(zafid, "Simplef")'
         write(*,*)'Value returned by he5_zacreate ', zaid
         write(1,*)'zaid= he5_zacreate(zafid, "Simplef")'
         write(1,*)'Value returned by he5_zacreate ', zaid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid2=he5_zacreate(zafid,'Indexedf')
      if (zaid2 .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid2= he5_zacreate(zafid, "Indexedf")'
         write(*,*)'Value returned by he5_zacreate ', zaid2
         write(1,*)'zaid2= he5_zacreate(zafid, "Indexedf")'
         write(1,*)'Value returned by he5_zacreate ', zaid2
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zadefdim
!
      write(1,27)
      write(*,*)'Testing he5_zadefdim '
      write(*,*)'====================='
      write(1,*)'Testing he5_zadefdim '
      write(1,*)'====================='
      status=he5_zadefdim(zaid,'DataTrack',dim4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_zadefdim(zaid, "DataTrack", 100)'
         write(*,*)'	Value returned by he5_zadefdim ',status
         write(1,*)'	status= he5_zadefdim(zaid, "DataTrack", 100)'
         write(1,*)'	Value returned by he5_zadefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefdim(zaid,'DataXtrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_zadefdim(zaid, "DataXtrack", 40)'
         write(*,*)'	Value returned by he5_zadefdim ',status
         write(1,*)'	status= he5_zadefdim(zaid, "DataXtrack", 40)'
         write(1,*)'	Value returned by he5_zadefdim ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefdim(zaid2,'DataTrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_zadefdim(zaid2, "DataTrack", 40)'
         write(*,*)'	Value returned by he5_zadefdim ', status
         write(1,*)'	status= he5_zadefdim(zaid2, "DataTrack", 40)'
         write(1,*)'	Value returned by he5_zadefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefdim(zaid2,'DataXtrack',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status= he5_zadefdim(zaid2, "DataXtrack", 40)'
         write(*,*)'	Value returned by he5_zadefdim ', status
         write(1,*)'	status= he5_zadefdim(zaid2, "DataXtrack", 40)'
         write(1,*)'	Value returned by he5_zadefdim ', status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefdim(zaid2,'Timedim',dim2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define dimension Timedim '
         write(*,*)'	Value returned by he5_zadefdim ', status
         write(1,*)'Define dimension Timedim '
         write(1,*)'	Value returned by he5_zadefdim ', status
      endif

      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zadefine
!
      write(1,27)
      write(*,*)'Testing he5_zadefine '
      write(*,*)'====================='
      write(1,*)'Testing he5_zadefine '
      write(1,*)'====================='

      status=he5_zadefine(zaid,'SideA','DataXtrack,DataTrack',
     1' ', HE5T_NATIVE_FLOAT)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
        
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field SideA '
         write(*,*)'Value returned by he5_zadefine ',status
         write(1,*)'Define data field SideA '
         write(1,*)'Value returned by he5_zadefine ',status
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case  -- he5_zasetfill
!
      write(1,27)
      write(*,*)'Testing he5_zasetfill '
      write(*,*)'====================='
      write(1,*)'Testing he5_zasetfill '
      write(1,*)'====================='
      status= he5_zasetfill(zaid,'Temperature',
     1HE5T_NATIVE_FLOAT, fillvalue)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zasetfill(zaid,"Temperature",
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(*,*)'Value returned by he5_zasetfill ',status
         write(1,*)'status=he5_zasetfill(zaid,"Temperature",
     1HE5T_NATIVE_FLOAT, fillvalue)'
         write(1,*)'Value returned by he5_zasetfill ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefine(zaid,'Temperature',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field Temperature '
         write(*,*)'Value returned by he5_zadefine ',status
         write(1,*)'Define data field Temperature '
         write(1,*)'Value returned by he5_zadefine ',status
         write(*,*)' '
         write(1,*)' '
      endif

      status= he5_zasetfill(zaid,'Conduction',
     1HE5T_NATIVE_FLOAT,fillvalue)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zasetfill(zaid,"Conduction",
     1HE5T_NATIVE_FLOAT,fillvalue)'
         write(*,*)'Value returned by he5_zasetfill ',status
         write(1,*)'status=he5_zasetfill(zaid,"Conduction",
     1HE5T_NATIVE_FLOAT,fillvalue)'
         write(1,*)'Value returned by he5_zasetfill ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefine(zaid,'Conduction',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadefine(zaid,"Conduction",
     1"DataXtrack,DataTrack","",HE5T_NATIVE_FLOAT)'
         write(*,*)'Value returned by he5_zadefine ',status
         write(1,*)'status= he5_zadefine(zaid,"Conduction",
     1"DataXtrack,DataTrack","",HE5T_NATIVE_FLOAT)'
         write(1,*)'Value returned by he5_zadefine ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadefine(zaid2,'Sensor1',
     1'DataXtrack,DataTrack',' ',HE5T_NATIVE_FLOAT)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zadefine(zaid2, "Sensor1",
     1"DataTrack,DataXtrack","",HE5T_NATIVE_FLOAT)'
         write(*,*)'Value returned by he5_zadefine ',status
         write(1,*)'status= he5_zadefine(zaid2, "Sensor1",
     1"DataTrack,DataXtrack","",HE5T_NATIVE_FLOAT)'
         write(1,*)'Value returned by he5_zadefine ',status
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zadefchunk
!
      write(1,27)
      write(*,*)'Testing he5_zadefchunk'
      write(*,*)'======================'
      write(1,*)'Testing he5_zadefchunk'
      write(1,*)'======================'
      status=he5_zadefchunk(zaid,cnkrank,chunkdims)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadefchunk(zaid, cnkrank, chunkdims)'
         write(*,*)'Value returned by he5_zadefchunk ',status
         write(1,*)'status= he5_zadefchunk(zaid, cnkrank, chunkdims)'
         write(1,*)'Value returned by he5_zadefchunk ',status
      endif
      write(*,*)' '
      write(1,*)' '
    
! Test Case -- he5_zadefcomp
!
      write(1,27)
      write(*,*)'Testing he5_zadefcomp'
      write(*,*)'====================='
      write(1,*)'Testing he5_zadefcomp'
      write(1,*)'====================='
      status=he5_zadefcomp(zaid,HE5_HDFE_COMP_DEFLATE,compparm)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadefcomp(zaid,HE5_HDFE_COMP_DEFLATE,
     1compparm)'
         write(*,*)'Value returned by he5_zadefcomp ',status
         write(1,*)'status= he5_zadefcomp(zaid,HE5_HDFE_COMP_DEFLATE,
     1compparm)'
         write(1,*)'Value returned by he5_zadefcomp ',status
      endif
      write(*,*)' '
      write(1,*)' '


      status=he5_zadefine(zaid,'CompChunk','DataTrack',
     1' ',HE5T_NATIVE_FLOAT)
      if (status .lt. zero) then
         write(*,230)
         write(1,230) 
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Define data field CompChunk '
         write(*,*)'Value returned by he5_zadefine ',status
         write(1,*)'Define data field CompChunk '
         write(1,*)'Value returned by he5_zadefine ',status
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case -- he5_zadetach
!
      write(1,27)
      write(*,*)'Testing he5_zadetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_zadetach '
      write(1,*)'====================='
      status=he5_zadetach(zaid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zadetach(zaid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid2)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid2)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      zaid=he5_zaattach(zafid,'Simplef')
      if (zaid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(*,*)'Value returned by he5_zaattach ',zaid
         write(1,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(1,*)'Value returned by he5_zaattach ',zaid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid2=he5_zaattach(zafid,'Indexedf')
      if (zaid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(*,*)'Value returned by he5_zaattach ',zaid2
         write(1,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(1,*)'Value returned by he5_zaattach ',zaid2
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zadetach
!
      write(1,27)
      write(*,*)'Testing he5_zadetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_zadetach '
      write(1,*)'====================='
      status= he5_zadetach(zaid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid2)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid2)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zaclose(zafid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)       
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zaclose(zafid)'
         write(*,*)'Value returned by he5_zaclose ',status
         write(1,*)'status= he5_zaclose(zafid)'
         write(1,*)'Value returned by he5_zaclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      zafid=he5_zaopen('za.he5',HE5F_ACC_RDWR)
      if (zafid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zafid=he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_zaopen ',zafid
         write(1,*)'zafid=he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_zaopen ',zafid
      endif
      write(*,*)' '
      write(1,*)' '
 
! Test Case -- he5_zaattach
!
      write(1,27)
      write(*,*)'Testing he5_zaattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_zaattach '
      write(1,*)'====================='

      zaid=he5_zaattach(zafid,'Simplef')
      if (zaid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(*,*)'Value returned by he5_zaattach ',zaid
         write(1,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(1,*)'Value returned by he5_zaattach ',zaid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid2=he5_zaattach(zafid,'Indexedf')
      if (zaid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(*,*)'Value returned by he5_zaattach ',zaid2
         write(1,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(1,*)'Value returned by he5_zaattach ',zaid2
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zawrite
!
      write(1,27)
      write(*,*)'Testing he5_zawrite '
      write(*,*)'==================='
      write(1,*)'Testing he5_zawrite '
      write(1,*)'==================='

      status=he5_zawrite(zaid,'SideA',sd_start,stride, 
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
         write(*,*)'Value returned by he5_zawrite ',status
         write(1,*)'Write data field SideA '
         write(1,*)'Value returned by he5_zawrite ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zawrite(zaid,'Temperature',start,stride, 
     1dataedge, ray4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zawrite(zaid,"Temperature",start, 
     1stride,dataedge, ray1)'
         write(*,*)'Value returned by he5_zawrite ',status
         write(1,*)'status=he5_zawrite(zaid,"Temperature",start, 
     1stride,dataedge, ray1)'
         write(1,*)'Value returned by he5_zawrite ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status=he5_zawrite(zaid,'Conduction',start,stride, 
     1dataedge, ray2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zawrite(zaid,"Conduction",start, 
     1stride,dataedge, ray2)'
         write(*,*)'Value returned by he5_zawrite ',status
         write(1,*)'status=he5_zawrite(zaid,"Conduction",start, 
     1stride,dataedge, ray2)'
         write(1,*)'Value returned by he5_zawrite ',status
      endif
      write(*,*)' '
      write(1,*)' '


      iedge(1)=40
      iedge(2)=40
      status=he5_zawrite(zaid2,'Sensor1',istart,stride, 
     1iedge,ray3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zawrite(zaid2, "Sensor1", 
     1istart, stride, iedge, ray3)'
         write(*,*)'Value returned by he5_zawrite ',status
         write(1,*)'status= he5_zawrite(zaid2, "Sensor1", 
     1istart, stride, iedge, ray3)'
         write(1,*)'Value returned by he5_zawrite ',status
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zawrlattr
!
      write(1,27)
      write(*,*)'Testing he5_zawrlattr, Local Attribute '
      write(*,*)'================================== '
      write(1,*)'Testing he5_zawrlattr, Local Attribute '
      write(1,*)'================================== '
      status=he5_zawrlattr(zaid2,'Sensor1','local',
     1HE5T_NATIVE_FLOAT,count,locattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zawrlattr(zaid2,"Sensor1","local",
     1HE5T_NATIVE_FLOAT,count,locattr)'
         write(*,*)'Value returned by he5_zawrlattr ',status
         write(1,*)'status=he5_zawrlattr(zaid2,"Sensor1", "local", 
     1HE5T_NATIVE_FLOAT,count,locattr)'
         write(1,*)'Value returned by he5_zawrlattr ',status
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zawrattr
!
      count(1) = 4

      write(1,27)
      write(*,*)'Testing he5_zawrattr, Global Attribute '
      write(*,*)'====================================== '
      write(1,*)'Testing he5_zawrattr, Global Attribute '
      write(1,*)'======================================= '
      status=he5_zawrattr(zaid,'Drift',HE5T_NATIVE_INT,count, 
     1global_attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zawrattr(zaid, "Drift", 
     1HE5T_NATIVE_INT, count, global_attr)'
         write(*,*)'Value returned by he5_zawrattr ',status
         write(1,*)'status=he5_zawrattr(zaid, "Drift", 
     1HE5T_NATIVE_INT, count, global_attr)'
         write(1,*)'Value returned by he5_zawrattr ',status
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zawrgattr
!
      count(1) = 3
     
      write(1,27)
      write(*,*)'Testing he5_zawrgattr, Group Attribute '
      write(*,*)'====================================== '
      write(1,*)'Testing he5_zawrgattr, Group Attribute '
      write(1,*)'====================================== '
      status=he5_zawrgattr(zaid,'group',HE5T_NATIVE_FLOAT, 
     1count,grpattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zawrgattr(zaid, "group", 
     1HE5T_NATIVE_FLOAT,count,grpgattr)'
         write(*,*)'Value returned by he5_zawrgattr ',status
         write(1,*)'status=he5_zawrgattr(zaid, "group", 
     1HE5T_NATIVE_FLOAT,count,grpattr)'
         write(1,*)'Value returned by he5_zawrgattr ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
      write(*,*)'status= he5_zadetach(zaid)'
      write(*,*)'Value returned by he5_zadetach ',status
      write(1,*)'status= he5_zadetach(zaid)'
      write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid2)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid2)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zaclose
!
      write(1,27)
      write(*,*)'Testing he5_zaclose '
      write(*,*)'==================='
      write(1,*)'Testing he5_zaclose '
      write(1,*)'==================='
      status= he5_zaclose(zafid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_zaclose ',status
         write(1,*)'status= he5_zaclose(zafid)'
         write(1,*)'Value returned by he5_zaclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      zafid= he5_zaopen('za.he5',HE5F_ACC_RDWR)
      if (zafid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_zaopen ', zafid
         write(1,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_zaopen ', zafid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid= he5_zaattach(zafid,'Simplef')
      if (zaid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(*,*)'Value returned by he5_zaattach ',zaid
         write(1,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(1,*)'Value returned by he5_zaattach ',zaid
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zaclose(zafid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zaclose(zafid)'
         write(*,*)'Value returned by he5_zaclose ',status
         write(1,*)'status= he5_zaclose(zafid)'
         write(1,*)'Value returned by he5_zaclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      zafid= he5_zaopen('za.he5',HE5F_ACC_RDWR)
      if (zafid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_zaopen ',zafid
         write(1,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_zaopen ',zafid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid=he5_zaattach(zafid,'Simplef')
      if (zaid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(*,*)'Value returned by he5_zaattach ',zaid
         write(1,*)'zaid= he5_zaattach(zafid, "Simplef")'
         write(1,*)'Value returned by he5_zaattach ',zaid
      endif
      write(*,*)' '
      write(1,*)' '

      zaid2=he5_zaattach(zafid,'Indexedf')
      if (zaid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(*,*)'Value returned by he5_zaattach ',zaid2
         write(1,*)'zaid2= he5_zaattach(zafid, "Indexedf")'
         write(1,*)'Value returned by he5_zaattach ',zaid2
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zanentries
!
      write(1,27)
      write(*,*)'Testing he5_zanentries '
      write(*,*)'====================='
      write(1,*)'Testing he5_zanentries '
      write(1,*)'====================='

      nmaps=he5_zanentries(zaid,1,strbufsize)
      if (nmaps .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nmaps= he5_zanentries(zaid, 1, strbufsize)'
         write(*,*)'Number of dimension mappings ',nmaps
         write(*,*)'Size of strbufsize ',strbufsize
         write(1,*)'nmaps= he5_zanentries(zaid, 1, strbufsize)'
         write(1,*)'Number of dimension mappings ',nmaps
         write(1,*)'Size of strbufsize ',strbufsize
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zainqdims
!
      write(1,27)
      write(*,*)'Testing he5_zainqdims '
      write(*,*)'====================='
      write(1,*)'Testing he5_zainqdims '
      write(1,*)'====================='
      ndims=he5_zainqdims(zaid,dimname,dims)
      if (ndims .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'ndims= he5_zainqdims(zaid, dimname, dims)'
         write(*,*)'Number of dimesions ',ndims
         write(*,*)'Names of dimensions ',dimname
         write(*,*)'Size of dimensions ',dims
         write(1,*)'ndims= he5_zainqdims(zaid, dimname, dims)'
         write(1,*)'Number of dimesions ',ndims
         write(1,*)'Names of dimensions ',dimname
         write(1,*)'Size of dimensions ',dims
      endif
      write(*,*)' '
      write(1,*)' '

      ndims= he5_zainqdims(zaid2, dimname, dims2)
      if (ndims .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'ndims=he5_zainqdims(zaid2,dimname,dims2)'
         write(*,*)'Number of dimesions ',ndims
         write(*,*)'Names of dimensions ',dimname
         write(*,*)'Size of dimensions ',dims2
         write(1,*)'ndims=he5_zainqdims(zaid2,dimname,dims2)'
         write(1,*)'Number of dimesions ',ndims
         write(1,*)'Names of dimensions ',dimname
         write(1,*)'Size of dimensions ',dims2
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zainquire
!
      write(1,27)
      write(*,*)'Testing he5_zainquire '
      write(*,*)'======================'
      write(1,*)'Testing he5_zainquire '
      write(1,*)'======================'
      nflds=he5_zainquire(zaid,fieldlist,rnkd,ntyped)
      if (nflds .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'nflds=he5_zainquire(zaid,fieldlist,rnkd,ntyped)'
         write(*,*)'Number of data fields in za ',nflds
         write(*,*)'Names of data fields ',fieldlist
         write(*,*)'Rank of fields ',rnkd
         write(*,*)'Number type of fields ',ntyped
         write(1,*)'nflds=he5_zainquire(zaid,fieldlist,rnkd,ntyped)'
         write(1,*)'Number of data fields in za ',nflds
         write(1,*)'Names of data fields ',fieldlist
         write(1,*)'Rank of fields ',rnkd
         write(1,*)'Number type of fields ',ntyped
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zadiminfo
!
      write(1,27)
      write(*,*)'Testing he5_zadiminfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_zadiminfo '
      write(1,*)'====================='
      dimsize= he5_zadiminfo(zaid,'DataTrack')
      if (dimsize .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'dimsize= he5_zadiminfo(zaid, "DataTrack")'
         write(*,*)'Size of dimension DataTrack ',dimsize
         write(1,*)'dimsize= he5_zadiminfo(zaid, "DataTrack")'
         write(1,*)'Size of dimension DataTrack ',dimsize
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case -- he5_zaread
!
      write(1,27)
      write(*,*)'Testing he5_zaread '
      write(*,*)'==================='
      write(1,*)'Testing he5_zaread '
      write(1,*)'==================='

      rd_start(1)=0
      rd_start(2)=0
      rd_stride(1)=1
      rd_stride(2)=1
      rd_edge(1)=40
      rd_edge(2)=100
      status= he5_zaread(zaid,'Conduction',rd_start,
     1rd_stride,rd_edge,temp)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Value returned by he5_zaread ',status
         write(*,*)'status= he5_zaread(zaid, "Conduction", 
     1rd_start,rd_stride, rd_edge, temp)'
         write(1,*)'status= he5_zaread(zaid, "Conduction", 
     1rd_start,rd_stride, rd_edge, temp)'
         write(1,*)'Value returned by he5_zaread ',status
         do 190 index1=10,15
            do 180 index2=10,15
               write(*,*)'value of conduction ',temp(index1,index2)
               write(1,*)'value of conduction ',temp(index1,index2)
  180       continue
  190    continue
      endif

      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zainfo
!
      write(1,27)
      write(*,*)'Testing he5_zainfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_zainfo '
      write(1,*)'====================='
      status=he5_zainfo(zaid,'SideA',rank,sddims,nt,
     1dimens,maxdims)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zainfo(zaid, "SideA", 
     1rank, sddims, nt, dimens, maxdims)'
         write(*,*)'Value returned by he5_zainfo ',status
         write(*,*)'Rank of field sideA ',rank
         write(*,*)'dimension list of field sideA ',dimens
         write(*,*)'Size of sideA dimensions ',sddims
         write(*,*)'number type of field sideA ',nt
         write(1,*)'status= he5_zainfo(zaid, "SideA", 
     1rank, sddims, nt, dimens, maxdims)'
         write(1,*)'Value returned by he5_zainfo ',status
         write(1,*)'Rank of field sideA ',rank
         write(1,*)'dimension list of field sideA ',dimens
         write(1,*)'Size of sideA dimensions ',sddims
         write(1,*)'number type of field sideA ',nt
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zainqattrs
!
      write(1,27)
      write(*,*)'Testing he5_zainqattrs Global attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_zainqattrs Global attributes '
      write(1,*)'==================================== '

      nattr = he5_zainqattrs(zaid,attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Number of global attributes in za ',nattr
         write(*,*)'Attribute list of za ',attrlist
         write(1,*)'Number of global attributes in za ',nattr
         write(1,*)'Attribute list of za ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case -- he5_zaattrinfo
!
      write(1,27)
      write(*,*)'Testing he5_zaattrinfo Global attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_zaattrinfo Global attribute'
      write(1,*)'======================================='
      status= he5_zaattrinfo(zaid, attrlist, n, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zaattrinfo(zaid,attrlist,n,size)'
         write(*,*)'Value returned by he5_zaattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'status=he5_zaattrinfo(zaid,attrlist,n,size)'
         write(1,*)'Value returned by he5_zaattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zardattr
!
      write(1,27)
      write(*,*)'Testing he5_zardattr, Global attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_zardattr, Global attribute '
      write(1,*)'======================================'

      status= he5_zardattr(zaid, attrlist, rdattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zardattr(zaid,attrlist,rdattr)'
         write(*,*)'Value returned by he5_zardattr ',status
         write(*,*)'attribute ',rdattr
         write(1,*)'status=he5_zardattr(zaid,attrlist,rdattr)'
         write(1,*)'Value returned by he5_zardattr ',status
         write(1,*)'attribute ',rdattr
      endif
      write(*,*)' '
      write(1,*)' '

!     Test Case -- he5_zainqgattrs
!
      write(1,27)
      write(*,*)'Testing he5_zainqgattrs Group attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_zainqgattrs Group attributes '
      write(1,*)'==================================== '

      nattr = he5_zainqgattrs(zaid,attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Number of group attributes in za ',nattr
         write(*,*)'Attribute list of za ',attrlist
         write(1,*)'Number of group attributes in za ',nattr
         write(1,*)'Attribute list of za ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case -- he5_zagattrinfo
!
      write(1,27)
      write(*,*)'Testing he5_zagattrinfo Group attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_zagattrinfo Group attribute'
      write(1,*)'======================================='
      status= he5_zagattrinfo(zaid, attrlist, n, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zagattrinfo(zaid,attrlist,n,size)'
         write(*,*)'Value returned by he5_zagattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'status=he5_zagattrinfo(zaid,attrlist,n,size)'
         write(1,*)'Value returned by he5_zagattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zardgattr
!
      write(1,27)
      write(*,*)'Testing he5_zardgattr, Group attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_zardgattr, Group attribute '
      write(1,*)'======================================'

       status= he5_zardgattr(zaid,'group',rdgnlattr)
       if (status .lt. zero) then
          write(*,230)
          write(1,230)
       else
          write(*,*)'  '
          write(*,240)
          write(1,*)'  '
          write(1,240)
          write(*,*)'status=he5_zardgattr(zaid,attrlist,rdgnlattr)'
          write(*,*)'Value returned by he5_zardgattr ',status
          write(*,*)'attribute ',rdgnlattr
          write(1,*)'status=he5_zardgattr(zaid,attrlist,rdgnlattr)'
          write(1,*)'Value returned by he5_zardgattr ',status
          write(1,*)'attribute ',rdgnlattr
       endif
       write(*,*)' '
       write(1,*)' '

! Test Case -- he5_zainqlattrs
!
      write(1,27)
      write(*,*)'Testing he5_zainqlattrs Local attributes '
      write(*,*)'==================================== '
      write(1,*)'Testing he5_zainqlattrs Local attributes '
      write(1,*)'==================================== '

      nattr=he5_zainqlattrs(zaid2,'Sensor1',attrlist,strbufsz2)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zainqlattrs(... "Sensor1" ...)'
         write(*,*)'Number of local attributes in field ',nattr
         write(*,*)'Attribute list of field ',attrlist
         write(1,*)'zainqlattrs(... "Sensor1" ...)'
         write(1,*)'Number of local attributes in field ',nattr
         write(1,*)'Attribute list of field ',attrlist
         write(*,*)' '
         write(1,*)' '
      endif

! Test Case -- he5_zalattrinfo
!
      write(1,27)
      write(*,*)'Testing he5_zalattrinfo Local attribute'
      write(*,*)'======================================='
      write(1,*)'Testing he5_zalattrinfo Local attribute'
      write(1,*)'======================================='
      status=he5_zalattrinfo(zaid2,'Sensor1','local',n,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zalattrinfo(... "Sensor1" ...)'
         write(*,*)'Value returned by he5_zalattrinfo ',status
         write(*,*)'Number type of attributes ',n,' Size ',size
         write(1,*)'zalattrinfo(... "Sensor1" ...)'
         write(1,*)'Value returned by he5_zalattrinfo ',status
         write(1,*)'Number type of attributes ',n,' Size ',size
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zaidtype
!
      write(1,27)
      write(*,*)'Testing he5_zaidtype, Inquire Datatype '
      write(*,*)'======================================='
      write(1,*)'Testing he5_zaidtype, Inquire Datatype '
      write(1,*)'======================================='

      status= he5_zaidtype(zaid2,'Sensor1','local', 
     1HE5_HDFE_LOCATTRGROUP,dtype,classid, order, size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zaidtype(zaid2,"Sensor1","local", 
     1HE5_HDFE_LOCATTRGROUP,dtype, classid, order, size)'
         write(*,*)'Value returned by he5_zaidtype ',status
         write(*,*)'Datatype of local attribute ',dtype
         write(*,*)'Class id and order ',classid, order
         write(*,*)'Size of attribute ',size
         write(1,*)'status= he5_zaidtype(zaid2,"Sensor1","local", 
     1HE5_HDFE_LOCATTRGROUP,dtype, classid, order, size)'
         write(1,*)'Value returned by he5_zaidtype ',status
         write(1,*)'Datatype of local attribute ',dtype
         write(1,*)'Class id and order ',classid, order
         write(1,*)'Size of attribute ',size
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zardlattr
!
      write(1,27)
      write(*,*)'Testing he5_zardlattr, Local attribute '
      write(*,*)'======================================'
      write(1,*)'Testing he5_zardlattr, Local attribute '
      write(1,*)'======================================'

      status=he5_zardlattr(zaid2,'Sensor1','local',rdgnlattr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zardlattr(zaid2,"Sensor1","local", 
     1rdgnlattr)'
         write(*,*)'Value returned by he5_zardlattr ',status
         write(*,*)'attribute ',rdgnlattr
         write(1,*)'status=he5_zardlattr(zaid2,"Sensor1","local", 
     1rdgnlattr)'
         write(1,*)'Value returned by he5_zardlattr ',status
         write(1,*)'attribute ',rdgnlattr
      endif
      write(*,*)' '
      write(1,*)' '

! Test Case -- he5_zagetfill
!
      write(1,27)
      write(*,*)'Testing he5_zagetfill '
      write(*,*)'====================='
      write(1,*)'Testing he5_zagetfill '
      write(1,*)'====================='
      status= he5_zagetfill(zaid,'Conduction', rdfilval)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zagetfill(zaid,"Conduction",rdfilval)'
         write(*,*)'Value returned by he5_zagetfill ',status
         write(*,*)'Value of fill ',rdfilval
         write(1,*)'status=he5_zagetfill(zaid,"Conduction",rdfilval)'
         write(1,*)'Value returned by he5_zagetfill ',status
         write(1,*)'Value of fill ',rdfilval
      endif
      write(*,*)' '
      write(1,*)' '



! Test Case -- he5_zacompinfo
!
      write(1,27)
      write(*,*)'Testing he5_zacompinfo '
      write(*,*)'======================='
      write(1,*)'Testing he5_zacompinfo '
      write(1,*)'======================='
      status=he5_zacompinfo(zaid,'CompChunk',compcode,rdcompparm)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status=he5_zacompinfo(zaid,"CompChunk",compcode, 
     1rdcompparm)'
         write(*,*)'Status returned by he5_zacompinfo ',status
         write(*,*)'Compression code ',compcode
         write(*,*)'Compression parameter ',rdcompparm(1)
         write(1,*)'status=he5_zacompinfo(zaid,"CompChunk",compcode, 
     1rdcompparm)'
         write(1,*)'Status returned by he5_zacompinfo ',status
         write(1,*)'Compression code ',compcode
         write(1,*)'Compression parameter ',rdcompparm(1)
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zadetach(zaid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)         
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zadetach(zaid2)'
         write(*,*)'Value returned by he5_zadetach ',status
         write(1,*)'status= he5_zadetach(zaid2)'
         write(1,*)'Value returned by he5_zadetach ',status
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zaclose(zafid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zaclose(zafid)'
         write(*,*)'Value returned by he5_zaclose ',status
         write(1,*)'status= he5_zaclose(zafid)'
         write(1,*)'Value returned by he5_zaclose ',status
      endif
      write(*,*)' '
      write(1,*)' '


      zafid= he5_zaopen('za.he5',HE5F_ACC_RDWR)
      if (zafid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(*,*)'Value returned by he5_zaopen ',zafid
         write(1,*)'zafid= he5_zaopen("za.he5",HE5F_ACC_RDWR)'
         write(1,*)'Value returned by he5_zaopen ',zafid
      endif
      write(*,*)' '
      write(1,*)' '


! Test Case -- he5_zainqza
!
      write(1,27)
      write(*,*)'Testing he5_zainqza '
      write(*,*)'======================'
      write(1,*)'Testing he5_zainqza '
      write(1,*)'======================'

      nza=he5_zainqza('za.he5',zalist,strbufsz2)
      if (nza .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'Retrieve information on zas in 
     1file za.he5'
         write(*,*)'Number of zas in file ', nza
         write(*,*)'List of zas ',zalist
         write(1,*)'Retrieve information on zas in 
     1file za.he5'
         write(1,*)'Number of zas in file ', nza
         write(1,*)'List of zas ',zalist
      endif
      write(*,*)' '
      write(1,*)' '

      status= he5_zaclose(zafid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'status= he5_zaclose(zafid)'
         write(*,*)'Value returned by he5_zaclose ',status
         write(1,*)'status= he5_zaclose(zafid)'
         write(1,*)'Value returned by he5_zaclose ',status
      endif
      write(*,*)' '
      write(1,*)' '

      close(unit=1)
   27 format(' ')
      stop
      end












