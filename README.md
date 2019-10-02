# UniGen
## United Generators version 2.0

## Setting up environment

Unpack the tar ball you have donwloaded. UNIGEN_TOP_DIR
is the top directory of the package, containing base, converters,
macros, examples, data, etc.

Enable ROOT
~~~bash
. /misc/cbmsoft/Debian3.1/tools/rootlogin
~~~

   (OPTIONALLY)
   In order to use Genesis converter and decayer, please
store the path to Genesis in GENESIS environment variable.
Note, that ${GENESIS}/lib should be added to the 
LD_LIBRARY_PATH.

## Compiling and installing the package

After setting up the environment, the unigen should be
compiled by typing
~~~bash
make
~~~

   To compile Genesis converter and decayer, please type
~~~bash
make genesis
~~~


   Optionally, you can install unigen run-time files outside
the source tree; note that for the time being the install
target does not support Genesis. Simply run
~~~bash
make DESTDIR=/my/target/directory install
~~~

   If you do not specify DESTDIR, make will try to use the
value of the environment variable UNIGEN. If that isn't set,
/usr/local/UniGen will be used.

## Run-time setup

This must be done before using UniGen, from a start-up 
script for instance.

   Enable ROOT
~~~
. /misc/cbmsoft/Debian3.1/tools/rootlogin
~~~

 a) for using unigen from the source directory

   Set the variable UNIGEN to point at the unigen source
tree, OR make sure it's unset and enter that directory
~~~
export UNIGEN=(UNIGEN_TOP_DIR)
~~~
or
~~~
unset UNIGEN
cd (UNIGEN_TOP_DIR)
~~~

   Set up UniGen variables and paths
~~~
. (UNIGEN_TOP_DIR)/config/unigenlogin.sh
~~~
 b) for using installed unigen

   Set the variable UNIGEN to point at the target directory
from the step 2, then if necessary add ${UNIGEN}/bin to PATH
and ${UNIGEN}/lib to LD_LIBRARY_PATH.

## Application

a) Using converters
   Converters are standalone executables and can be started
from command line. To get the instructions on which parameters
have to be specified, please execute desired converter without
any parameters:
    > urqmd2u
   
   By default, parameters are: inputFileName, outputFileName,
numberOfEvents:
    > urqmd2u inputFileName outputFileName numberOfEvents

   Example output files of all supported generators can be
found in data directory.

 b) Users's tasks
   Users's tasks are C++ classes, derived from base class UTask.
They should be compiled into the library and loaded in the macro
before instantiation. Access to the data event is maintained by
singleton UManager class: UManager::Instance()->GetEvent().
Before the event loop, method UTask::Init() is called for each
task by the Manager. In the event loop, method UTask::Exec() is
called. After processing all events, manager calls method
UTask::Finish().

___________________________________________________________
CREATED 2006-09-26 by UniGen development team
