# CppZip release notes

## Version 1.0.1.3, 02.12.2017

Service release

### Changes:
- Updated internal zlib version to 1.2.11 to use its newest minizip. With that newest minizip
  it is possible to compile and link the debug version of CppZip on Windows 7 with
  Visual Studio 2015. Nevertheless, it is possible to use cppzip together with zlib 1.2.8, because
  the minizip sources are at the moment directly compiled into CppZip and are still compatible
  with zlib 1.2.8.
- Some code cleanups to move the code to c++11 style.
- Some doc (doxygen) cleanups.
- AppVeyor build:
   - Build with Visual Studio 2015
   - Build with Visual Studio 2017
   - Uses vcpk to prepare zlib and cppunit

### Based on:
   - zlib version 1.2.8 (minimum required)
   - boost version 1.5.3 (minimum required)



## Version 1.0.0.2, 22.01.2017

First stable release with cmake as build tool.

### Based on:
   - zlib version 1.2.8 (minimum required)
   - boost version 1.5.3 (minimum required)
