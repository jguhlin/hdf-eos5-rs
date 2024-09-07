
c----------------------------------------------------------------------------
c       FILE:     testpoint32.f                                               
c       PURPOSE:  To test HDF-EOS5 Point interface              
c----------------------------------------------------------------------------

      program       he5_pttestpoint32
 
      implicit      none
 
      include       'hdfeos5.inc'

      integer       status 
      integer       ptfid 
      integer       ptid1, ptid2, ptid3
      integer       ptid
      integer       rank_1(3)
      integer       rank_2(5)
      integer       rank_3(4)
      integer       rank_4(4)
      integer       rank_5(6)
      integer       dtype_1(3)
      integer       dtype_2(5)
      integer       dtype_3(4)
      integer       dtype_4(4)
      integer       dtype_5(6)
      integer       array_1(3)
      integer       array_2(5)
      integer       array_3(4)
      integer       array_4(4)
      integer       array_5(6)
      integer       i, zero
      integer       rank
      integer       datatype
      integer       attr
      integer       nflds
      integer       level
      integer       arr_rank
      integer       dtype(3)
      integer       nrecs
      integer       nlevels
      integer       attr1(5)
      integer       attr2(5)
      integer       ntype
      integer       classid
      integer       order
      integer       fieldgroup

      integer       he5_ptopen
      integer       he5_ptcreate
      integer       he5_ptdetach
      integer       he5_ptclose
      integer       he5_ptattach
      integer       he5_ptdeflevel 
      integer       he5_ptdeflinkage 
      integer       he5_ptwritelevel 
      integer       he5_ptfort2c
      integer       he5_ptwrbckptr
      integer       he5_ptwrfwdptr
      integer       he5_ptwriteattr 
      integer       he5_ptreadlevel
      integer       he5_ptlevelinfo 
      integer       he5_ptnrecs
      integer       he5_ptnlevels
      integer       he5_ptnfields
      integer       he5_ptc2fort
      integer       he5_ptinqattrs
      integer       he5_ptreadattr
      integer       he5_ptupdatelevel 
      integer       he5_ptwritegrpattr
      integer       he5_ptwritelocattr
      integer       he5_ptinqgrpattrs
      integer       he5_ptinqlocattrs
      integer       he5_ptattrinfo
      integer       he5_ptgrpattrinfo
      integer       he5_ptlocattrinfo
      integer       he5_ptreadgrpattr
      integer       he5_ptreadlocattr
      integer       he5_ptinqdatatype

      integer*4     dims_1(3,1)
      integer*4     dims_2(5,1)
      integer*4     dims_3(4,1)
      integer*4     dims_4(4,1)
      integer*4     dims_5(6,1)
      integer*4     n
      integer*4     count(1)
      integer*4     dimens(2)
      integer*4     fortcount(8)
      integer*4     date_tt
      integer*4     date(3)
      integer*4     date4_tt
      integer*4     date4(3)
      integer*4     wgt_tt
      integer*4     wgt(3)
      integer*4     datasize
      integer*4     rank_tt(3)
      integer*4     offset_tt(3)
      integer*4     dtype_tt(3)
      integer*4     dim_sizes_tt(3)
      integer*4     nattr
      integer*4     strbufsize
      integer*4     recs(32)
      integer*4     nrec
      integer*4     sz
      integer*4     size

      real*4        flt      
      real*4        concentration_tt(4)
      real*4        conc(15,4)
      real*4        outconc(4,15)
      real*4        rain_tt
      real*4        rain(20)
      real*4        temp_tt
      real*4        temp(20)
      real*4        rain5_tt
      real*4        rain5(25)
      real*4        temp5_tt
      real*4        temp5(25)
      real*4        con(4,15)
      real*4        outcon(15,4)
      real*4        conc_tt(4)

      real*8        time_tt
      real*8        time(15)
      real*8        lon_tt
      real*8        lon(3)
      real*8        lat_tt
      real*8        lat(3)
      real*8        time3_tt
      real*8        time3(20)
      real*8        time5_tt
      real*8        time5(25)
      real*8        lon5_tt
      real*8        lon5(25)
      real*8        lat5_tt
      real*8        lat5(25)
 
      character*240 fieldlist1
      character*240 fieldlist2
      character*240 fieldlist3
      character*240 fieldlist4
      character*240 fieldlist5
      character*80  levelname
      character*20  parent
      character*20  child
      character*20  linkfield
      character*80  fieldname
      character*80  attrname
      character*8   spc_tt
      character*8   spc(15)
      character*8   desc_tt
      character*8   desc(3)
      character*8   id_tt
      character*8   id(3)
      character*8   id3_tt
      character*8   id3(20)
      character*8   desc4_tt
      character*8   desc4(3)
      character*8   id4_tt
      character*8   id4(3)
      character*8   id5_tt
      character*8   id5(25)
      character*80  fieldname3(3)
      character*240 fieldlist
      character*240 attrlist
      character*8   spec(15)
      character*1   null_char_0

      open(unit=1, file="testpoint32_f.txtout", status = "UNKNOWN")

      write(*,*)' '
      write(1,*)' '
      write(*,*)'Testing he5_ptath FORTRAN 77 interface'
      write(*,*)'======================================'
      write(1,*)'Testing he5_ptath FORTRAN 77 interface'
      write(1,*)'======================================'
      write(*,*)' '
      write(1,*)' '

      zero=0

c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_TRUNC) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_TRUNC)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_TRUNC)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptcreate
c
      write(*,*)'Testing he5_ptcreate '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptcreate '
      write(1,*)'====================='
      ptid1 = he5_ptcreate(ptfid, "Simple Point")
      if (ptid1 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid1 = he5_ptcreate(ptfid, "Simple Point")'
         write(*,*)'	Value returned by he5_ptcreate ', ptid1
         write(1,*)'	ptid1 = he5_ptcreate(ptfid, "Simple Point")'
         write(1,*)'	Value returned by he5_ptcreate ', ptid1
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptcreate
c
      write(*,*)'Testing he5_ptcreate '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptcreate '
      write(1,*)'====================='
      ptid2 = he5_ptcreate(ptfid, "FixedBuoy Point")
      if (ptid2 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid2 = he5_ptcreate(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptcreate ', ptid2
         write(1,*)'	ptid2 = he5_ptcreate(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptcreate ', ptid2
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptcreate
c
      write(*,*)'Testing he5_ptcreate '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptcreate '
      write(1,*)'====================='
      ptid3 = he5_ptcreate(ptfid, "FloatBuoy Point")
      if (ptid3 .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid3 = he5_ptcreate(ptfid, "FloatBuoy Point")'
         write(*,*)'	Value returned by he5_ptcreate ', ptid3
         write(1,*)'	ptid3 = he5_ptcreate(ptfid, "FloatBuoy Point")'
         write(1,*)'	Value returned by he5_ptcreate ', ptid3
      endif
      write(*,*)' '
      write(1,*)' '



c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid1)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid1)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid2)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid2)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid3)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid3)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDWR) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "Simple Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '


c     Populate input information structure 
c     ------------------------------------ 

      levelname = 'Sensor'

      rank_1(1)  = 1
      rank_1(2)  = 1
      rank_1(3)  = 1

      fieldlist1 = 'Time,Concentration,Species'

      dtype_1(1)     =  HE5T_NATIVE_DOUBLE
      dtype_1(2)     =  HE5T_NATIVE_FLOAT
      dtype_1(3)     =  HE5T_NATIVE_CHAR

      array_1(1)     =  0
      array_1(2)     =  1
      array_1(3)     =  1

      dims_1(1,1)      =  1
      dims_1(2,1)      =  4
      dims_1(3,1)      =  8

c Test Case -- he5_ptdeflevel
c
      write(1,27)
      write(*,*)'Testing he5_ptdeflevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdeflevel '
      write(1,*)'====================='
      status = he5_ptdeflevel(ptid, levelname, rank_1, fieldlist1,
     1dims_1, dtype_1, array_1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflevel(ptid, levelname, rank_1'
         write(*,*)'             fieldlist1,dims_1, dtype_1, array_1)'
         write(*,*)'	Value returned by he5_ptdeflevel ', status
         write(1,*)'	status = he5_ptdeflevel(ptid, levelname, rank_1'
         write(1,*)'             fieldlist1,dims_1, dtype_1, array_1)'
         write(1,*)'	Value returned by he5_ptdeflevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '


c     Populate input information structure 
c     ------------------------------------ 

      levelname = 'Desc-Loc'

      rank_2(1)  = 1
      rank_2(2)  = 1
      rank_2(3)  = 1
      rank_2(4)  = 1
      rank_2(5)  = 1

      fieldlist2 = 'Label,Longitude,Latitude,DeployDate,ID'

      dtype_2(1)     =  HE5T_NATIVE_CHAR
      dtype_2(2)     =  HE5T_NATIVE_DOUBLE
      dtype_2(3)     =  HE5T_NATIVE_DOUBLE
      dtype_2(4)     =  HE5T_NATIVE_INT
      dtype_2(5)     =  HE5T_NATIVE_CHAR

      array_2(1)  = 1
      array_2(2)  = 0
      array_2(3)  = 0
      array_2(4)  = 0
      array_2(5)  = 1

      dims_2(1,1)      =  8
      dims_2(2,1)      =  1
      dims_2(3,1)      =  1
      dims_2(4,1)      =  1
      dims_2(5,1)      =  8


c Test Case -- he5_ptdeflevel
c
      write(1,27)
      write(*,*)'Testing he5_ptdeflevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdeflevel '
      write(1,*)'====================='
      status = he5_ptdeflevel(ptid, levelname, rank_2, fieldlist2,
     1dims_2, dtype_2, array_2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflevel(ptid, levelname, rank_2'
         write(*,*)'             fieldlist2,dims_2, dtype_2, array_2)'
         write(*,*)'	Value returned by he5_ptdeflevel ', status
         write(1,*)'	status = he5_ptdeflevel(ptid, levelname, rank_2'
         write(1,*)'             fieldlist2,dims_2, dtype_2, array_2)'
         write(1,*)'	Value returned by he5_ptdeflevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

c     Populate input information structure 
c     ------------------------------------ 

      levelname = 'Observations'

      rank_3(1)  = 1
      rank_3(2)  = 1
      rank_3(3)  = 1
      rank_3(4)  = 1

      fieldlist3 = 'Time,Rainfall,Temperature,ID'

      dtype_3(1)     =  HE5T_NATIVE_DOUBLE
      dtype_3(2)     =  HE5T_NATIVE_FLOAT
      dtype_3(3)     =  HE5T_NATIVE_FLOAT
      dtype_3(4)     =  HE5T_NATIVE_CHAR

      array_3(1)  = 0
      array_3(2)  = 0
      array_3(3)  = 0
      array_3(4)  = 1

      dims_3(1,1)      =  1
      dims_3(2,1)      =  1
      dims_3(3,1)      =  1
      dims_3(4,1)      =  8

c Test Case -- he5_ptdeflevel
c
      write(1,27)
      write(*,*)'Testing he5_ptdeflevel '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptdeflevel '
      write(1,*)'======================'
      status = he5_ptdeflevel(ptid, levelname, rank_3, fieldlist3,
     1dims_3, dtype_3, array_3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflevel(ptid, levelname, rank_3'
         write(*,*)'             fieldlist3,dims_3, dtype_3, array_3)'
         write(*,*)'	Value returned by he5_ptdeflevel ', status
         write(1,*)'	status = he5_ptdeflevel(ptid, levelname, rank_3'
         write(1,*)'             fieldlist3,dims_3, dtype_3, array_3)'
         write(1,*)'	Value returned by he5_ptdeflevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      parent    = 'Desc-Loc'
      child     = 'Observations'
      linkfield = 'ID'

c Test Case -- he5_ptdeflinkage
c
      write(*,*)'Testing he5_ptdeflinkage '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptdeflinkage '
      write(1,*)'========================'
      status = he5_ptdeflinkage(ptid, parent, child, linkfield)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflinkage(ptid, parent, child,'
         write(*,*)'	         linkfield)'
         write(*,*)'	Value returned by he5_ptdeflinkage ', status
         write(1,*)'	status = he5_ptdeflinkage(ptid, parent, child,'
         write(1,*)'	         linkfield)'
         write(1,*)'	Value returned by he5_ptdeflinkage ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FloatBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FloatBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FloatBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '


      levelname = 'Description'

      rank_4(1)  = 1
      rank_4(2)  = 1
      rank_4(3)  = 1
      rank_4(4)  = 1

      fieldlist4 = 'Label,DeployDate,Weight,ID'

      dtype_4(1)     =  HE5T_NATIVE_CHAR
      dtype_4(2)     =  HE5T_NATIVE_INT
      dtype_4(3)     =  HE5T_NATIVE_INT
      dtype_4(4)     =  HE5T_NATIVE_CHAR

      array_4(1)  = 1
      array_4(2)  = 0
      array_4(3)  = 0
      array_4(4)  = 1

      dims_4(1,1)      =  8
      dims_4(2,1)      =  1
      dims_4(3,1)      =  1
      dims_4(4,1)      =  8

c Test Case -- he5_ptdeflevel
c
      write(1,27)
      write(*,*)'Testing he5_ptdeflevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdeflevel '
      write(1,*)'====================='
      status = he5_ptdeflevel(ptid, levelname, rank_4, fieldlist4,
     1dims_4, dtype_4, array_4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflevel(ptid, levelname, rank_4'
         write(*,*)'             fieldlist4,dims_4, dtype_4, array_4)'
         write(*,*)'	Value returned by he5_ptdeflevel ', status
         write(1,*)'	status = he5_ptdeflevel(ptid, levelname, rank_4'
         write(1,*)'             fieldlist4,dims_4, dtype_4, array_4)'
         write(1,*)'	Value returned by he5_ptdeflevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      levelname = 'Measurements'

      rank_5(1)  = 1
      rank_5(2)  = 1
      rank_5(3)  = 1
      rank_5(4)  = 1
      rank_5(5)  = 1
      rank_5(6)  = 1

      fieldlist5 = 'Time,Longitude,Latitude,Rainfall,Temperature,ID'

      dtype_5(1)     =  HE5T_NATIVE_DOUBLE
      dtype_5(2)     =  HE5T_NATIVE_DOUBLE
      dtype_5(3)     =  HE5T_NATIVE_DOUBLE
      dtype_5(4)     =  HE5T_NATIVE_FLOAT
      dtype_5(5)     =  HE5T_NATIVE_FLOAT
      dtype_5(6)     =  HE5T_NATIVE_CHAR

      array_5(1)  = 0
      array_5(2)  = 0
      array_5(3)  = 0
      array_5(4)  = 0
      array_5(5)  = 0
      array_5(6)  = 1

      dims_5(1,1)      =  1
      dims_5(2,1)      =  1
      dims_5(3,1)      =  1
      dims_5(4,1)      =  1
      dims_5(5,1)      =  1
      dims_5(6,1)      =  8


c Test Case -- he5_ptdeflevel
c
      write(1,27)
      write(*,*)'Testing he5_ptdeflevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdeflevel '
      write(1,*)'====================='
      status = he5_ptdeflevel(ptid, levelname, rank_5, fieldlist5,
     1dims_5, dtype_5, array_5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflevel(ptid, levelname, rank_5'
         write(*,*)'             fieldlist5,dims_5, dtype_5, array_5)'
         write(*,*)'	Value returned by he5_ptdeflevel ', status
         write(1,*)'	status = he5_ptdeflevel(ptid, levelname, rank_5'
         write(1,*)'             fieldlist5,dims_5, dtype_5, array_5)'
         write(1,*)'	Value returned by he5_ptdeflevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      parent    = 'Description'
      child     = 'Measurements'
      linkfield = 'ID'

c Test Case -- he5_ptdeflinkage
c
      write(*,*)'Testing he5_ptdeflinkage '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdeflinkage '
      write(1,*)'====================='
      status = he5_ptdeflinkage(ptid, parent, child, linkfield)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdeflinkage(ptid, parent, child,'
         write(*,*)'	         linkfield)'
         write(*,*)'	Value returned by he5_ptdeflinkage ', status
         write(1,*)'	status = he5_ptdeflinkage(ptid, parent, child,'
         write(1,*)'	         linkfield)'
         write(1,*)'	Value returned by he5_ptdeflinkage ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDWR) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "Simple Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Read Simple Point 
c     ----------------- 
      open(unit=10, file='simple.txt', status='OLD')

      n = 0
      do 10 i=1,1000
         read(10, 110, end=100) time_tt, concentration_tt(1), 
     1                                  concentration_tt(2), 
     2                                  concentration_tt(3),
     3                                  concentration_tt(4), 
     4                                  spc_tt
         time(i)     = time_tt
         conc(i,1)   = concentration_tt(1)
         conc(i,2)   = concentration_tt(2)
         conc(i,3)   = concentration_tt(3)
         conc(i,4)   = concentration_tt(4)
         spc(i)      = spc_tt

         n = n + 1
   10 continue

  100 close(unit=10)
  110 format(F13.1,F6.2,F6.2,F6.2,F6.2,2X,A8)

      count(1) = n

      fieldname      = 'Time'
      datatype       = HE5T_NATIVE_DOUBLE


c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, time)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, time)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, time)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


   
c.....Convert array to 'C' order
      dimens(1) = 15
      dimens(2) = 4
      rank      = 2
      datatype  = HE5T_NATIVE_FLOAT

c Test Case -- he5_ptfort2c
c
      write(1,27)
      write(*,*)'Testing he5_ptfort2c '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptfort2c '
      write(1,*)'====================='
      status    = he5_ptfort2c(dimens, rank, datatype, conc, 
     1outconc)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptfort2c(dimens,rank,datatype,conc,'
         write(*,*)'	         outconc)'
         write(*,*)'	Value returned by he5_ptfort2c ', status
         write(1,*)'	status = he5_ptfort2c(dimens,rank,datatype,conc,'
         write(1,*)'	         outconc)'
         write(1,*)'	Value returned by he5_ptfort2c ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Concentration'

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, outconc)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, outconc)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, outconc)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Species'
      datatype       = HE5T_NATIVE_CHAR

      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, spc)
      write(*,*) 'Status returned by he5_ptwritelevel():  ',status

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Read FixedBuoy Point 
c     -------------------- 
      open(unit=11, file='fixedBuoy0.txt', status='OLD')

      n = 0
      do 20 i=1,1000
         read(11, 210, end=200) desc_tt, lon_tt, lat_tt, date_tt, 
     1id_tt
         desc(i)     = desc_tt
         lon(i)      = lon_tt
         lat(i)      = lat_tt
         date(i)     = date_tt
         id(i)       = id_tt

         n = n + 1
   20 continue

  200 close(unit=11)
  210 format(A8,F13.7,F13.7,I7,1X,A8)

      count(1) = n

      fieldname      = 'Label'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, desc)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, desc)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, desc)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Longitude'
      datatype       = HE5T_NATIVE_DOUBLE

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, lon)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, lon)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, lon)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Latitude'
      datatype       = HE5T_NATIVE_DOUBLE


c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, lat)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, lat)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, lat)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'DeployDate'
      datatype       = HE5T_NATIVE_INT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype,  date)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype,  date)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype,  date)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'ID'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, id)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, id)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, id)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


c     Read FixedBuoy Point - Level 1
c     ------------------------------ 
      open(unit=12, file='fixedBuoy1.txt', status='OLD')

      n = 0
      do 30 i=1,1000
         read(12, 310, end=300) time3_tt, rain_tt, temp_tt, id3_tt 
         time3(i)     = time3_tt
         rain(i)      = rain_tt
         temp(i)      = temp_tt
         id3(i)       = id3_tt

         n = n + 1
   30 continue

  300 close(unit=12)
  310 format(F13.2,F8.1,F8.2,3X,A8)

      count(1) = n

      fieldname      = 'Time'
      datatype       = HE5T_NATIVE_DOUBLE

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, time3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, time3)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, time3)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Rainfall'
      datatype       = HE5T_NATIVE_FLOAT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, rain)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, rain)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, rain)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Temperature'
      datatype       = HE5T_NATIVE_FLOAT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, temp)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, temp)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, temp)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldname      = 'ID'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, id3)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, id3)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, id3)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptwrbckptr
c
      write(1,27)
      write(*,*)'Testing he5_ptwrbckptr '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptwrbckptr '
      write(1,*)'====================='
      status = he5_ptwrbckptr(ptid,1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptwrbckptr(ptid,1)'
         write(*,*)'	Value returned by he5_ptwrbckptr ', status
         write(1,*)'	status = he5_ptwrbckptr(ptid,1)'
         write(1,*)'	Value returned by he5_ptwrbckptr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptwrfwdptr
c
      write(1,27)
      write(*,*)'Testing he5_ptwrfwdptr '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptwrfwdptr '
      write(1,*)'====================='
      status = he5_ptwrfwdptr(ptid,1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptwrfwdptr(ptid,1)'
         write(*,*)'	Value returned by he5_ptwrfwdptr ', status
         write(1,*)'	status = he5_ptwrfwdptr(ptid,1)'
         write(1,*)'	Value returned by he5_ptwrfwdptr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c     Write attributes to "Fixed Buoy Point"
c     --------------------------------------
      attrname       = 'GlobalAttribute_int'
      ntype          = HE5T_NATIVE_INT
      fortcount(1)   = 1
      attr           = 9999 

c Test Case -- he5_ptwriteattr
c
      write(1,27)
      write(*,*)'Testing he5_ptwriteattr '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptwriteattr '
      write(1,*)'====================='
      status   = he5_ptwriteattr(ptid,attrname,ntype,fortcount,
     1attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(*,*)'	           ntype,fortcount,attr)'
         write(*,*)'	Value returned by he5_ptwriteattr ', status
         write(1,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(1,*)'	           ntype,fortcount,attr)'
         write(1,*)'	Value returned by he5_ptwriteattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FloatBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FloatBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FloatBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Read FloatBuoy Point - Level 0
c     ------------------------------ 
      open(unit=14, file='floatBuoy0.txt', status='OLD')

      n = 0
      do 40 i=1,1000
         read(14, 410, end=400) desc4_tt, date4_tt, wgt_tt, id4_tt 
         desc4(i)     = desc4_tt
         date4(i)     = date4_tt
         wgt(i)       = wgt_tt
         id4(i)       = id4_tt

         n = n + 1
   40 continue

  400 close(unit=14)
  410 format(A8,I8,I7,2X,A8)

      count(1) = n

      fieldname      = 'Label'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, desc4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, desc4)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, desc4)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'DeployDate'
      datatype       = HE5T_NATIVE_INT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, date4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, date4)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, date4)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Weight'
      datatype       = HE5T_NATIVE_INT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, wgt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, wgt)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, wgt)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'ID'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 0, count, fieldname, 
     1datatype, id4)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(*,*)'	         fieldname,datatype, id4)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,0,count,'
         write(1,*)'	         fieldname,datatype, id4)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


c     Read FixedBuoy Point - Level 1
c     ------------------------------ 
      open(unit=15, file='floatBuoy1.txt', status='OLD')

      n = 0
      do 50 i=1,1000
         read(15, 510, end=500) time5_tt, lon5_tt, lat5_tt,  
     1rain5_tt,temp5_tt,id5_tt
         time5(i)     = time5_tt
         lon5(i)      = lon5_tt
         lat5(i)      = lat5_tt
         rain5(i)     = rain5_tt
         temp5(i)     = temp5_tt
         id5(i)       = id5_tt

         n = n + 1
   50 continue

  500 close(unit=15)
  510 format(F13.1,F13.6,F13.6,F8.1,F8.2,3X,A8)

      count(1) = n

      fieldname      = 'Time'
      datatype       = HE5T_NATIVE_DOUBLE

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, time5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, time5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, time5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Longitude'
      datatype       = HE5T_NATIVE_DOUBLE

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, lon5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, lon5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, lon5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Latitude'
      datatype       = HE5T_NATIVE_DOUBLE

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, lat5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, lat5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, lat5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Rainfall'
      datatype       = HE5T_NATIVE_FLOAT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, rain5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, rain5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'             fieldname,datatype, rain5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'Temperature'
      datatype       = HE5T_NATIVE_FLOAT

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, temp5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, temp5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, temp5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname      = 'ID'
      datatype       = HE5T_NATIVE_CHAR

c Test Case -- he5_ptwritelevel
c
      write(1,27)
      write(*,*)'Testing he5_ptwritelevel '
      write(*,*)'========================'
      write(1,*)'Testing he5_ptwritelevel '
      write(1,*)'========================'
      status = he5_ptwritelevel(ptid, 1, count, fieldname, 
     1datatype, id5)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(*,*)'	         fieldname,datatype, id5)'
         write(*,*)'	Value returned by he5_ptwritelevel ', status
         write(1,*)'    status = he5_ptwritelevel(ptid,1,count,'
         write(1,*)'	         fieldname,datatype, id5)'
         write(1,*)'	Value returned by he5_ptwritelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptwrbckptr
c
      write(*,*)'Testing he5_ptwrbckptr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptwrbckptr '
      write(1,*)'======================'
      status = he5_ptwrbckptr(ptid,1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptwrbckptr(ptid,1)'
         write(*,*)'	Value returned by he5_ptwrbckptr ', status
         write(1,*)'	status = he5_ptwrbckptr(ptid,1)'
         write(1,*)'	Value returned by he5_ptwrbckptr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptwrfwdptr
c
      write(*,*)'Testing he5_ptwrfwdptr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptwrfwdptr '
      write(1,*)'======================'
      status = he5_ptwrfwdptr(ptid,1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptwrfwdptr(ptid,1)'
         write(*,*)'	Value returned by he5_ptwrfwdptr ', status
         write(1,*)'	status = he5_ptwrfwdptr(ptid,1)'
         write(1,*)'	Value returned by he5_ptwrfwdptr ', status
      endif
      write(*,*)' '
      write(1,*)' '



c     Write attributes to "Float Buoy Point"
c     --------------------------------------
      attrname       = 'GlobalAttribute_float'
      ntype          = HE5T_NATIVE_FLOAT
      fortcount(1)   = 1
      flt            = -7.5

c Test Case -- he5_ptwriteattr
c
      write(*,*)'Testing he5_ptwriteattr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptwriteattr '
      write(1,*)'======================'
      status   = he5_ptwriteattr(ptid,attrname,
     1ntype,fortcount,flt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(*,*)'	           ntype,fortcount,flt)'
         write(*,*)'	Value returned by he5_ptwriteattr ', status
         write(1,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(1,*)'	           ntype,fortcount,flt)'
         write(1,*)'	Value returned by he5_ptwriteattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDONLY) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "Simple Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Get level information 
c     --------------------- 
      level = 0

c Test Case -- he5_ptlevelinfo
c
      write(*,*)'Testing he5_ptlevelinfo '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptlevelinfo '
      write(1,*)'====================='
      status = he5_ptlevelinfo(ptid, level, levelname, rank_tt,
     1fieldlist, dim_sizes_tt, datasize, offset_tt, dtype_tt)
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status=he5_ptlevelinfo(ptid,level,levelname,'
         write(*,*)'           rank_tt,fieldlist,dim_sizes_tt,'
         write(*,*)'           datasize,offset_tt,dtype_tt)'
         write(*,*)'	Value returned by he5_ptlevelinfo ', status
         write(1,*)'    status=he5_ptlevelinfo(ptid,level,levelname,'
         write(1,*)'           rank_tt,fieldlist,dim_sizes_tt,'
         write(1,*)'           datasize,offset_tt,dtype_tt)'
         write(1,*)'	Value returned by he5_ptlevelinfo ', status
      endif
      write(*,*)' '
      write(1,*)' '

c     Get the number of records in level
c     ---------------------------------- 
      level = 0

c Test Case -- he5_ptnrecs
c
      write(*,*)'Testing he5_ptnrecs '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptnrecs '
      write(1,*)'====================='
      nrecs = he5_ptnrecs(ptid, level)
      if (nrecs .le. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nrecs = he5_ptnrecs(ptid, level)'
         write(*,*)'	Value returned by he5_ptnrecs ', nrecs
         write(1,*)'	nrecs = he5_ptnrecs(ptid, level)'
         write(1,*)'	Value returned by he5_ptnrecs ', nrecs
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptnlevels
c
      write(*,*)'Testing he5_ptnlevels '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptnlevels '
      write(1,*)'====================='
      nlevels = he5_ptnlevels(ptid)
      if (nlevels .le. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nlevels = he5_ptnlevels(ptid)'
         write(*,*)'	Value returned by he5_ptnlevels ', nlevels
         write(1,*)'	nlevels = he5_ptnlevels(ptid)'
         write(1,*)'	Value returned by he5_ptnlevels ', nlevels
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptnfields
c
      write(*,*)'Testing he5_ptnfields '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptnfields '
      write(1,*)'====================='
      nflds = he5_ptnfields(ptid, level, fieldlist, strbufsize)
      if (nflds .le. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nflds = he5_ptnfields(ptid, level,'
         write(*,*)'	        fieldlist, strbufsize)'
         write(*,*)'	Value returned by he5_ptnfields ', nflds
         write(1,*)'	nflds = he5_ptnfields(ptid, level,'
         write(1,*)'	        fieldlist, strbufsize)'
         write(1,*)'	Value returned by he5_ptnfields ', nflds
      endif
      write(*,*)' '
      write(1,*)' '

c     Populate input information structure 
c     ------------------------------------ 
      dtype(1)     =  HE5T_NATIVE_DOUBLE
      dtype(2)     =  HE5T_NATIVE_FLOAT
      dtype(3)     =  HE5T_NATIVE_CHAR

c     Read the level data and print out
c     --------------------------------- 
      fieldname3(1)  = 'Time'

c Test Case -- he5_ptreadlevel
c
      write(*,*)'Testing he5_ptreadlevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptreadlevel '
      write(1,*)'====================='
      status = he5_ptreadlevel(ptid, 0, fieldname3(1), 
     1dtype(1), time)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(1),'
         write(*,*)'	         dtype(1), time)'
         write(*,*)'	Value returned by he5_ptreadlevel ', status
         write(1,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(1),'
         write(1,*)'	         dtype(1), time)'
         write(1,*)'	Value returned by he5_ptreadlevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname3(2)  = 'Concentration'

c Test Case -- he5_ptreadlevel
c
      write(*,*)'Testing he5_ptreadlevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptreadlevel '
      write(1,*)'====================='
      status = he5_ptreadlevel(ptid, 0, fieldname3(2), 
     1dtype(2), con)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(2),'
         write(*,*)'	         dtype(2), con)'
         write(*,*)'	Value returned by he5_ptreadlevel ', status
         write(1,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(2),'
         write(1,*)'	         dtype(2), con)'
         write(1,*)'	Value returned by he5_ptreadlevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

c     Convert 'C' array to Fortran order
c     ---------------------------------- 
      dimens(1) = 15
      dimens(2) = 4
      arr_rank  = 2
      datatype  = HE5T_NATIVE_FLOAT

ccc      status  = he5_ptc2fort(dimens, arr_rank, datatype, con, 
ccc     1outcon)
ccc      write(*,*) 'Status returned by he5_ptc2fort():  ',status

c Test Case -- he5_ptc2fort
c
      write(1,27)
      write(*,*)'Testing he5_ptc2fort '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptc2fort '
      write(1,*)'====================='
      status  = he5_ptc2fort(dimens, arr_rank, datatype, con, 
     1outcon)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'    status = he5_ptc2fort(dimens,arr_rank,'
         write(*,*)'	         datatype,con,outcon)'
         write(*,*)'	Value returned by he5_ptc2fort ', status
         write(1,*)'    status = he5_ptc2fort(dimens,arr_rank,'
         write(1,*)'	         datatype,con,outcon)'
         write(1,*)'	Value returned by he5_ptc2fort ', status
      endif
      write(*,*)' '
      write(1,*)' '


      fieldname3(3)  = 'Species'

c Test Case -- he5_ptreadlevel
c
      write(*,*)'Testing he5_ptreadlevel '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptreadlevel '
      write(1,*)'====================='
      status = he5_ptreadlevel(ptid, 0, fieldname3(3), 
     1dtype(3), spec)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(3),'
         write(*,*)'	         dtype(3), spec)'
         write(*,*)'	Value returned by he5_ptreadlevel ', status
         write(1,*)'	status = he5_ptreadlevel(ptid, 0, fieldname3(3),'
         write(1,*)'	         dtype(3), spec)'
         write(1,*)'	Value returned by he5_ptreadlevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GlobalAttribute_int'

      nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)

c Test Case -- he5_ptinqattrs
c
      write(*,*)'Testing he5_ptinqattrs '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptinqattrs '
      write(1,*)'======================'
      nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqattrs ', nattr
         write(1,*)'	nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptreadattr
c
      write(*,*)'Testing he5_ptreadattr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptreadattr '
      write(1,*)'======================'
      status   = he5_ptreadattr(ptid,attrname,attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptreadattr(ptid,attrname,attr)'
         write(*,*)'	Value returned by he5_ptreadattr ', nattr
         write(1,*)'	status   = he5_ptreadattr(ptid,attrname,attr)'
         write(1,*)'	Value returned by he5_ptreadattr ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDWR) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "Simple Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "Simple Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

      dtype(1)     =  HE5T_NATIVE_DOUBLE
      dtype(2)     =  HE5T_NATIVE_FLOAT
      dtype(3)     =  HE5T_NATIVE_CHAR

      nrec      = 1
      recs(1)   = 0

      level     = 0

      fieldname = 'Concentration'

      conc_tt(1)   = 1.11
      conc_tt(2)   = 2.22
      conc_tt(3)   = 3.33
      conc_tt(4)   = 4.44

c Test Case -- he5_ptupdatelevel
c
      write(*,*)'Testing he5_ptupdatelevel '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptupdatelevel '
      write(1,*)'========================='
      status = he5_ptupdatelevel(ptid, level, fieldname, nrec,
     1recs, dtype(2), conc_tt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(*,*)'	         fieldname, nrec, recs,'
         write(*,*)'	         dtype(2), conc_tt)'
         write(*,*)'	Value returned by he5_ptupdatelevel ', status
         write(1,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(1,*)'	         fieldname, nrec, recs,'
         write(1,*)'	         dtype(2), conc_tt)'
         write(1,*)'	Value returned by he5_ptupdatelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldname = 'Time'

      time_tt   = 13131313.0

c Test Case -- he5_ptupdatelevel
c
      write(*,*)'Testing he5_ptupdatelevel '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptupdatelevel '
      write(1,*)'========================='
      status = he5_ptupdatelevel(ptid, level, fieldname, nrec,
     1recs, dtype(1), time_tt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(*,*)'	         fieldname, nrec, recs,'
         write(*,*)'	         dtype(1), time_tt)'
         write(*,*)'	Value returned by he5_ptupdatelevel ', status
         write(1,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(1,*)'	         fieldname, nrec, recs,'
         write(1,*)'	         dtype(1), time_tt)'
         write(1,*)'	Value returned by he5_ptupdatelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldname = 'Species'

      spc_tt    = 'AM'

c Test Case -- he5_ptupdatelevel
c
      write(*,*)'Testing he5_ptupdatelevel '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptupdatelevel '
      write(1,*)'========================='
      status = he5_ptupdatelevel(ptid, level, fieldname, nrec,
     1recs, dtype(3), spc_tt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(*,*)'	         fieldname, nrec, recs,'
         write(*,*)'	         dtype(3), spc_tt)'
         write(*,*)'	Value returned by he5_ptupdatelevel ', status
         write(1,*)'	status = he5_ptupdatelevel(ptid, level,'
         write(1,*)'	         fieldname, nrec, recs,'
         write(1,*)'	         dtype(3), spc_tt)'
         write(1,*)'	Value returned by he5_ptupdatelevel ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDWR) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDWR)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Write attributes to "Fixed Buoy Point"
c     --------------------------------------
      attrname       = 'GlobalAttribute_int'
      ntype          = HE5T_NATIVE_INT
      fortcount(1)   = 1
      attr           = 9999
             

c Test Case -- he5_ptwriteattr
c
      write(*,*)'Testing he5_ptwriteattr '
      write(*,*)'======================='
      write(1,*)'Testing he5_ptwriteattr '
      write(1,*)'======================='
      status   = he5_ptwriteattr(ptid,attrname,ntype,fortcount,
     1attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(*,*)'	           ntype,fortcount,attr)'
         write(*,*)'	Value returned by he5_ptwriteattr ', status
         write(1,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(1,*)'	           ntype,fortcount,attr)'
         write(1,*)'	Value returned by he5_ptwriteattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GroupAttribute'
      ntype          = HE5T_NATIVE_INT
      fortcount(1)   = 5
      attr1(1)       = 1
      attr1(2)       = 2
      attr1(3)       = 3
      attr1(4)       = 4
      attr1(5)       = 5

c Test Case -- he5_ptwritegrpattr
c
      write(*,*)'Testing he5_ptwritegrpattr '
      write(*,*)'=========================='
      write(1,*)'Testing he5_ptwritegrpattr '
      write(1,*)'=========================='
      status   = he5_ptwritegrpattr(ptid,attrname,ntype,fortcount,
     1attr1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwritegrpattr(ptid,attrname,'
         write(*,*)'	           ntype,fortcount,attr1)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status   = he5_ptwritegrpattr(ptid,attrname,'
         write(1,*)'	           ntype,fortcount,attr1)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '

      levelname      = 'Observations'
      attrname       = 'LocalAttribute'
      ntype          = HE5T_NATIVE_INT
      fortcount(1)   = 5
      attr2(1)       = 10
      attr2(2)       = 20
      attr2(3)       = 30
      attr2(4)       = 40
      attr2(5)       = 50

c Test Case -- he5_ptwritelocattr
c
      write(*,*)'Testing he5_ptwritelocattr '
      write(*,*)'=========================='
      write(1,*)'Testing he5_ptwritelocattr '
      write(1,*)'=========================='
      status   = he5_ptwritelocattr(ptid,levelname,attrname,
     1ntype,fortcount,attr2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwritelocattr(ptid,'
         write(*,*)'	           levelname,attrname,ntype,'
         write(*,*)'	           fortcount,attr2)'
         write(*,*)'	Value returned by he5_ptwritelocattr ', status
         write(1,*)'	status   = he5_ptwritelocattr(ptid,'
         write(1,*)'	           levelname,attrname,ntype,'
         write(1,*)'	           fortcount,attr2)'
         write(1,*)'	Value returned by he5_ptwritelocattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '


c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GlobalAttribute_float'
      ntype          = HE5T_NATIVE_FLOAT
      fortcount(1)   = 1
      flt            = -7.5

c Test Case -- he5_ptwriteattr
c
      write(*,*)'Testing he5_ptwriteattr '
      write(*,*)'======================='
      write(1,*)'Testing he5_ptwriteattr '
      write(1,*)'======================='
      status   = he5_ptwriteattr(ptid,attrname,
     1ntype,fortcount,flt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(*,*)'	           ntype,fortcount,flt)'
         write(*,*)'	Value returned by he5_ptwriteattr ', status
         write(1,*)'	status   = he5_ptwriteattr(ptid,attrname,'
         write(1,*)'	           ntype,fortcount,flt)'
         write(1,*)'	Value returned by he5_ptwriteattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '


      null_char_0  = '0'

c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDONLY) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

c     Global Attributes
c     -----------------
      attrname       = 'GlobalAttribute_int'
             
      print *,'Global Attribute:   '

c Test Case -- he5_ptreadattr
c
      write(*,*)'Testing he5_ptreadattr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptreadattr '
      write(1,*)'======================'
      status   = he5_ptreadattr(ptid,attrname,attr)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptreadattr(ptid,attrname,attr)'
         write(*,*)'	Value returned by he5_ptreadattr ', status
         write(1,*)'	status   = he5_ptreadattr(ptid,attrname,attr)'
         write(1,*)'	Value returned by he5_ptreadattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqattrs
c
      write(*,*)'Testing he5_ptinqattrs '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptinqattrs '
      write(1,*)'======================'
      nattr   = he5_ptinqattrs(ptid,null_char_0,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr   = he5_ptinqattrs(ptid,'
         write(*,*)'	          null_char_0,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqattrs ', nattr
         write(1,*)'	nattr   = he5_ptinqattrs(ptid,'
         write(1,*)'	          null_char_0,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqattrs
c
      write(*,*)'Testing he5_ptinqattrs '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptinqattrs '
      write(1,*)'======================'
      nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqattrs ', nattr
         write(1,*)'	nattr   = he5_ptinqattrs(ptid,attrlist,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GlobalAttribute_int'

c Test Case -- he5_ptattrinfo
c
      write(*,*)'Testing he5_ptattrinfo '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptattrinfo '
      write(1,*)'======================'
      status   = he5_ptattrinfo(ptid,attrname,ntype,sz)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptattrinfo(ptid,attrname,ntype,sz)'
         write(*,*)'	Value returned by he5_ptattrinfo ', status
         write(1,*)'	status   = he5_ptattrinfo(ptid,attrname,ntype,sz)'
         write(1,*)'	Value returned by he5_ptattrinfo ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattrinfo
c
      write(*,*)'Testing he5_ptattrinfo '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptattrinfo '
      write(1,*)'======================'
      status   = he5_ptattrinfo(ptid,attrname,ntype,sz)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptattrinfo(ptid,attrname,ntype,sz)'
         write(*,*)'	Value returned by he5_ptattrinfo ', status
         write(1,*)'	status   = he5_ptattrinfo(ptid,attrname,ntype,sz)'
         write(1,*)'	Value returned by he5_ptattrinfo ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptreadattr
c
      write(*,*)'Testing he5_ptreadattr '
      write(*,*)'======================'
      write(1,*)'Testing he5_ptreadattr '
      write(1,*)'======================'
      status   = he5_ptreadattr(ptid,attrname,flt)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptreadattr(ptid,attrname,flt)'
         write(*,*)'	Value returned by he5_ptreadattr ', status
         write(1,*)'	status   = he5_ptreadattr(ptid,attrname,flt)'
         write(1,*)'	Value returned by he5_ptreadattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GroupAttribute'

c Test Case -- he5_ptreadgrpattr
c
      write(*,*)'Testing he5_ptreadgrpattr'
      write(*,*)'========================='
      write(1,*)'Testing he5_ptreadgrpattr'
      write(1,*)'========================='
      status = he5_ptreadgrpattr(ptid,attrname,attr1)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptreadgrpattr(ptid,attrname,attr1)'
         write(*,*)'	Value returned by he5_ptreadgrpattr ', status
         write(1,*)'	status = he5_ptreadgrpattr(ptid,attrname,attr1)'
         write(1,*)'	Value returned by he5_ptreadgrpattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqgrpattrs
c
      write(*,*)'Testing he5_ptinqgrpattrs '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqgrpattrs '
      write(1,*)'========================='
      nattr = he5_ptinqgrpattrs(ptid,null_char_0,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr = he5_ptinqgrpattrs(ptid,'
         write(*,*)'	        null_char_0,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqgrpattrs ', nattr
         write(1,*)'	nattr = he5_ptinqgrpattrs(ptid,'
         write(1,*)'	        null_char_0,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqgrpattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqgrpattrs
c
      write(*,*)'Testing he5_ptinqgrpattrs '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqgrpattrs '
      write(1,*)'========================='
      nattr = he5_ptinqgrpattrs(ptid,attrlist,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr = he5_ptinqgrpattrs(ptid,'
         write(*,*)'	        attrlist,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqgrpattrs ', status
         write(1,*)'	nattr = he5_ptinqgrpattrs(ptid,'
         write(1,*)'	        attrlist,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqgrpattrs ', status
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'GroupAttribute'

c Test Case -- he5_ptgrpattrinfo
c
      write(*,*)'Testing he5_ptgrpattrinfo '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptgrpattrinfo '
      write(1,*)'========================='
      status   = he5_ptgrpattrinfo(ptid,attrname,ntype,sz)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status   = he5_ptgrpattrinfo(ptid,'
         write(*,*)'	           attrname,ntype,sz)'
         write(*,*)'	Value returned by he5_ptgrpattrinfo ', status
         write(1,*)'	status   = he5_ptgrpattrinfo(ptid,'
         write(1,*)'	           attrname,ntype,sz)'
         write(1,*)'	Value returned by he5_ptgrpattrinfo ', status
      endif
      write(*,*)' '
      write(1,*)' '

      attrname       = 'LocalAttribute'
      levelname      = 'Observations'

c Test Case -- he5_ptreadlocattr
c
      write(*,*)'Testing he5_ptreadlocattr '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptreadlocattr '
      write(1,*)'========================='
      status = he5_ptreadlocattr(ptid,levelname,attrname,attr2)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptreadlocattr(ptid,'
         write(*,*)'	         levelname,attrname,attr2)'
         write(*,*)'	Value returned by he5_ptreadlocattr ', status
         write(1,*)'	status = he5_ptreadlocattr(ptid,'
         write(1,*)'	         levelname,attrname,attr2)'
         write(1,*)'	Value returned by he5_ptreadlocattr ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqlocattrs
c
      write(*,*)'Testing he5_ptinqlocattrs '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqlocattrs '
      write(1,*)'========================='
      nattr = he5_ptinqlocattrs(ptid,levelname,null_char_0,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr = he5_ptinqlocattrs(ptid,'
         write(*,*)'	        levelname,null_char_0,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqlocattrs ', nattr
         write(1,*)'	nattr = he5_ptinqlocattrs(ptid,'
         write(1,*)'	        levelname,null_char_0,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqlocattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptinqlocattrs
c
      write(*,*)'Testing he5_ptinqlocattrs '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptinqlocattrs '
      write(1,*)'====================='
      nattr = he5_ptinqlocattrs(ptid,levelname,attrlist,strbufsize)
      if (nattr .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	nattr = he5_ptinqlocattrs(ptid,'
         write(*,*)'	        levelname,attrlist,strbufsize)'
         write(*,*)'	Value returned by he5_ptinqlocattrs ', nattr
         write(1,*)'	nattr = he5_ptinqlocattrs(ptid,'
         write(1,*)'	        levelname,attrlist,strbufsize)'
         write(1,*)'	Value returned by he5_ptinqlocattrs ', nattr
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptlocattrinfo
c
      write(*,*)'Testing he5_ptlocattrinfo '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptlocattrinfo '
      write(1,*)'========================='
      status = he5_ptlocattrinfo(ptid,levelname,attrname,ntype,sz)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptlocattrinfo(ptid,'
         write(*,*)'	         levelname,attrname,ntype,sz)'
         write(*,*)'	Value returned by he5_ptlocattrinfo ', status
         write(1,*)'	status = he5_ptlocattrinfo(ptid,'
         write(1,*)'	         levelname,attrname,ntype,sz)'
         write(1,*)'	Value returned by he5_ptlocattrinfo ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '

      null_char_0  = '0'

c Test Case -- he5_ptopen
c
      write(*,*)'Testing he5_ptopen '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptopen '
      write(1,*)'====================='
      ptfid = he5_ptopen('point.he5',HE5F_ACC_RDONLY) 
      if (ptfid .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(*,*)'	Value returned by he5_ptopen ', ptfid
         write(1,*)'	ptfid= he5_ptopen("point.he5",HE5F_ACC_RDONLY)'
         write(1,*)'	Value returned by he5_ptopen ', ptfid
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptattach
c
      write(1,27)
      write(*,*)'Testing he5_ptattach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptattach '
      write(1,*)'====================='
      ptid = he5_ptattach(ptfid, "FixedBuoy Point")
      if (ptid .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(*,*)'	Value returned by he5_ptattach ', ptid
         write(1,*)'	ptid = he5_ptattach(ptfid, "FixedBuoy Point")'
         write(1,*)'	Value returned by he5_ptattach ', ptid
      endif
      write(*,*)' '
      write(1,*)' '

      fieldgroup = HE5_HDFE_DATAGROUP
      fieldname  = 'Observations'

c Test Case -- he5_ptinqdatatype
c
      write(*,*)'Testing he5_ptinqdatatype '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqdatatype '
      write(1,*)'========================='
      status = he5_ptinqdatatype(ptid,fieldname,null_char_0,fieldgroup,
     1datatype,classid,order,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptinqdatatype(ptid,fieldname,'
         write(*,*)'	         null_char_0,fieldgroup,datatype,classid,'
         write(*,*)'	         order,size)'
         write(*,*)'	Value returned by he5_ptinqdatatype ', status
         write(1,*)'	status = he5_ptinqdatatype(ptid,fieldname,'
         write(1,*)'	         null_char_0,fieldgroup,datatype,classid,'
         write(1,*)'	         order,size)'
         write(1,*)'	Value returned by he5_ptinqdatatype ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldgroup   = HE5_HDFE_ATTRGROUP
      attrname     = 'GlobalAttribute_int'

c Test Case -- he5_ptinqdatatype
c
      write(*,*)'Testing he5_ptinqdatatype '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqdatatype '
      write(1,*)'========================='
      status = he5_ptinqdatatype(ptid,null_char_0,attrname,fieldgroup,
     1datatype,classid,order,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptinqdatatype(ptid,null_char_0,'
         write(*,*)'	         attrname,fieldgroup,datatype,'
         write(*,*)'	         classid,order,size)'
         write(*,*)'	Value returned by he5_ptinqdatatype ', status
         write(1,*)'	status = he5_ptinqdatatype(ptid,null_char_0,'
         write(1,*)'	         attrname,fieldgroup,datatype,'
         write(1,*)'	         classid,order,size)'
         write(1,*)'	Value returned by he5_ptinqdatatype ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldgroup   = HE5_HDFE_GRPATTRGROUP
      attrname     = 'GroupAttribute'

c Test Case -- he5_ptinqdatatype
c
      write(*,*)'Testing he5_ptinqdatatype '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqdatatype '
      write(1,*)'========================='
      status = he5_ptinqdatatype(ptid,null_char_0,attrname,fieldgroup,
     1datatype,classid,order,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptinqdatatype(ptid,null_char_0,'
         write(*,*)'	         attrname,fieldgroup,datatype,'
         write(*,*)'	         classid,order,size)'
         write(*,*)'	Value returned by he5_ptinqdatatype ', status
         write(1,*)'	status = he5_ptinqdatatype(ptid,null_char_0,'
         write(1,*)'	         attrname,fieldgroup,datatype,'
         write(1,*)'	         classid,order,size)'
         write(1,*)'	Value returned by he5_ptinqdatatype ', status
      endif
      write(*,*)' '
      write(1,*)' '

      fieldname    = 'Observations'
      fieldgroup   = HE5_HDFE_LOCATTRGROUP
      attrname     = 'LocalAttribute'

c Test Case -- he5_ptinqdatatype
c
      write(*,*)'Testing he5_ptinqdatatype '
      write(*,*)'========================='
      write(1,*)'Testing he5_ptinqdatatype '
      write(1,*)'========================='
      status = he5_ptinqdatatype(ptid,fieldname,attrname,fieldgroup,
     1datatype,classid,order,size)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptinqdatatype(ptid,fieldname,'
         write(*,*)'	         attrname,fieldgroup,datatype,'
         write(*,*)'	         classid,order,size)'
         write(*,*)'	Value returned by he5_ptinqdatatype ', status
         write(1,*)'	status = he5_ptinqdatatype(ptid,fieldname,'
         write(1,*)'	         attrname,fieldgroup,datatype,'
         write(1,*)'	         classid,order,size)'
         write(1,*)'	Value returned by he5_ptinqdatatype ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptdetach
c
      write(1,27)
      write(*,*)'Testing he5_ptdetach '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptdetach '
      write(1,*)'====================='
      status = he5_ptdetach(ptid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
               else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptdetach(ptid)'
         write(*,*)'	Value returned by he5_ptdetach ', status
         write(1,*)'	status = he5_ptdetach(ptid)'
         write(1,*)'	Value returned by he5_ptdetach ', status
      endif
      write(*,*)' '
      write(1,*)' '

c Test Case -- he5_ptclose
c
      write(*,*)'Testing he5_ptclose '
      write(*,*)'====================='
      write(1,*)'Testing he5_ptclose '
      write(1,*)'====================='
      status = he5_ptclose(ptfid)
      if (status .lt. zero) then
         write(*,230)
         write(1,230)
      else
         write(*,*)'  '
         write(*,240)
         write(1,*)'  '
         write(1,240)
         write(*,*)'	status = he5_ptclose(ptfid)'
         write(*,*)'	Value returned by he5_ptclose ', status
         write(1,*)'	status = he5_ptclose(ptfid)'
         write(1,*)'	Value returned by he5_ptclose ', status
      endif
      write(*,*)' '
      write(1,*)' '

      close(unit=1)

   27 format(' ')
  230 format('********Error unexpected**********')
  240 format('***********Passed Test*********')

      stop
      end


