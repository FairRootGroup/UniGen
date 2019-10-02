#!/bin/tcsh

# this macro must be sourced to work

set called = ($_)
if ( "$called" == "" ) then 
  echo this script must be sourced to work, try \"source $0\"
  exit
endif

# assuming that this macro is sitting at its original location, 
# its path can be used to set the unigen enviroment  

set filnam = $called[2]              # filename of this (sourced) macro 
set absnam = `readlink -f $filnam`   # absolute filename 
set pather = `dirname $absnam`       # path to the config directory
set pather = `dirname $pather`       # path to the top unigen directory

setenv UNIGEN $pather

if ( !($?LD_LIBRARY_PATH) ) setenv LD_LIBRARY_PATH

setenv PATH ${UNIGEN}/lib':'${UNIGEN}/bin':'${PATH}

setenv LD_LIBRARY_PATH ${UNIGEN}/lib':'${LD_LIBRARY_PATH}

echo UNIGEN = $UNIGEN
