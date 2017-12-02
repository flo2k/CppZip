CppZip
======

CppZip is an open source c++ zip library and is based on minizip and zlib. CppZip is implemented with
standard C++ (C++11) and some functions of the boost libraries.

It's implementation focus is
 - on easy to use - a clear and well documented API,
 - to run on multiple platforms (Windows, Linux, ...),
 - and well tested.


### Build status:

 - [![Build Status](https://travis-ci.org/flo2k/CppZip.svg?branch=master)](https://travis-ci.org/flo2k/CppZip) TravisCI (linux)

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

### Author:
_Florian Künzner_

### Version:
1.0.1.3 release state

### Date:
02.12.2017

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

see [LICENSE.txt](LICENSE.txt)
