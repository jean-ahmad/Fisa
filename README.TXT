
FInite State Automata library
#############################

Library that allows the modeling and the simulation of automata or concurrent systems.
There is no limitation in the number of states and transitions that can be used to build a machine.
Machines created match closely the specifications of "State Machines" as described in SysML.
Provide consistency information such as multiple transitions firing from the same state.

Documentation
#############

The documentation in HTML is available in the directory "docs/html" from the file "index.html".

Installation
############

To compile and install the FISA library, the CMake (http://www.cmake.org) building system is required.
It is also necessary that the compiler provides c++11 smart pointers.
Currently, support of "TimeEvent" events is available only with a GCC compiler.

Under Linux
-----------
At the root of the project directory, type the following instructions in a command interpreter:

cmake -DCMAKE_INSTALL_PREFIX="destination path" .
make
make install

Don't forget the DOT at the end of the first line and replace "destination path" (with quotes) by an existing directory.
The examples are put in the subdirectory "bin" of the "destination path", the static library in "lib" and the headers in "include".
