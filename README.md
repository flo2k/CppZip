CppZip
======

CppZip is an open source c++ zip library and is based on minizip and zlib. CppZip is implemented with
standard C++ (C++11) and some functions of the boost libraries.

It's implementation focus is
 - on easy to use - a clear and well documented API,
 - to run on multiple platforms (Windows, Linux, ...),
 - and well tested.


### Build status:

 - [![Build Status](https://travis-ci.org/flo2k/CppZip.svg?branch=test_dll)](https://travis-ci.org/flo2k/CppZip) TravisCI (linux)

 - [![Build Status](https://ci.appveyor.com/api/projects/status/qlctko1hvennty1h?svg=true)](https://ci.appveyor.com/project/flo2k/cppzip) AppVeyor (Windows)

### There are two classes in the library:

_Zip_ compresses and zips files and folders.
 - Adding files from memory
 - Adding files from file system
 - Adding complete (and recursive) folders from file system
 - Delete files
 - Replace files
 - Supports Passwords

_Unzip_ uncompresses and unzips zip files.
 - Inspect files inside a zip (e.g. file list)
 - Extract to file system
 - Extract to memory
 - Supports Passwords

###Author:
_Florian Künzner_

### Version:
1.0.0.0 release state

### Date:
22.01.2017

### Compilation:
Compiles with cmake into the directory "build":

#### Compile on Linux
    #create build dir
    mkdir build

    #configure
    cmake -H. -Bbuild

    #build
    cmake --build build -- -j

#### Compile on Windows
    #create build dir
    mkdir build

    #configure
    cmake -H. -Bbuild
        -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_LIBRARYDIR="%BOOST_LIBRARYDIR%"
        -DDOXYGEN_EXECUTABLE="%DOXYGEN_EXE%"
        -DZLIB_INCLUDE_DIR="%ZLIB_INCLUDE%" -DZLIB_LIBRARY="%ZLIB_LIBRARY%"
        -DCPPUNIT_INCLUDE_DIR="%CPPUNIT_INCLUDE_DIR%" -DCPPUNIT_LIBRARY_RELEASE="%CPPUNIT_LIBRARY%"

    #build
    cmake --build build --config Release --

### License:

CppZip -- IO on .zip files using minizip and zlib  
Version 1.0.0.2, 22.01.2017
CppZip project - (https://github.com/flo2k/CppZip)  

Copyright (C) 2011      Florian Künzner (CppZip)  
Copyright (C) 2012-2013 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)  
Copyright (C) 2014-2017 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)  
---------------------------------------------------------------------------

Condition of use and distribution are the same than minizip and zlib :

This software is provided 'as-is', without any express or implied  
warranty.  In no event will the authors be held liable for any damages  
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,  
including commercial applications, and to alter it and redistribute it  
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not  
   claim that you wrote the original software. If you use this software  
   in a product, an acknowledgment in the product documentation would be  
   appreciated but is not required.  
2. Altered source versions must be plainly marked as such, and must not be  
   misrepresented as being the original software.  
3. This notice may not be removed or altered from any source distribution.  

---------------------------------------------------------------------------
