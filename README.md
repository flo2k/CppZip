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

[_Zip_](src/CppZip/Zip.h) compresses and zips files and folders.
 - Adding files from memory
 - Adding files from file system
 - Adding complete (and recursive) folders from file system
 - Delete files
 - Replace files
 - Supports Passwords

[_Unzip_](src/CppZip/Zip.h) uncompresses and unzips zip files.
 - Inspect files inside a zip (e.g. file list)
 - Extract to file system
 - Extract to memory
 - Supports Passwords

### Author:
_Florian Künzner_

### Version:
1.0.1.4 release state

### Date:
06.12.2017

### Compilation:
Compiles with cmake into the directory "build":

#### Compile on Linux
    #create build dir
    mkdir build

    #configure
    cmake -H. -Bbuild

    #build
    cmake --build build -- -j

    #build doc
    cmake --build build -- doc

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

    #build doc
    cmake --build build --config Release --target doc

### License:

see [LICENSE.txt](LICENSE.txt)
