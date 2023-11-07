#!/bin/csh

# Filename: setup.csh
# Description: 
# Author: Xilin Liang < xilin.liang@email.ucr.edu >


starver SL20a
set called=($_)
if ("$called" != "") then
   set scriptdir=$called[2]
   set STARHOME=`dirname $scriptdir`
   set STARHOME=`c\d $STARHOME && pwd`    # ensure absolute path
else
    set scriptdir=$1
    set STARHOME=$scriptdir

endif

if(! $?SUMS_USER) then
    setenv STARHOME "$STARHOME"
else	
    setenv STARHOME "$PWD"  #For SUMS jobs
endif
    
setenv LD_LIBRARY_PATH "${STARHOME}/lib:${STARHOME}/.sl73_gcc485/lib:.sl73_gcc485/lib:lib:${LD_LIBRARY_PATH}"
setenv STARDEV "/afs/rhic.bnl.gov/star/packages/DEV/"
setenv FILLLIST "database/Run15FmsFillNumberDB.json" # run15 FMS data
setenv HOTCHLIST "database/Run15FmsHotChList.json" # run15 FMS data
echo $STARHOME
echo $LD_LIBRARY_PATH
echo $FILLLIST
echo $HOTCHLIST
