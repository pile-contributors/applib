AppLib
=======

Basic support code for an application's main library.

Some or most of the application code may be useful as a
shared library so that the application may be extended
using plug-ins that make use of the code in the library.

This pile provides the base class for the management 
class of such a library in AppLib, a class derived
from QObject to be able to use the Qt signaling 
mechanism. 

By using AppLib's changeState() the implementation
ensures that any interested parties are informed
about the change in the current status of the library.
This mechanism is used by the 
[plgin](https://github.com/pile-contributors/plgin) 
pile to implement plug-ins.

The pile also has a set of useful macros in
applib-util.h including support for cross-compiler
breakpoints, debug helpers and general code tricks.
