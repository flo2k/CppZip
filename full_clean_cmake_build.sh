#!/bin/bash

##prepare build dir
#deletes the build dir if exists
[ -d build ] && rm -Rf build
#creates the build dir
mkdir build

##prepare build
# for options details see: http://stackoverflow.com/questions/31090821/what-does-the-h-option-means-for-cmake
# -H set source dir
# -B set binary dir
cmake -H. -Bbuild

##start build
# --build set build dir
# -j parallel build
cmake --build build -- -j all build

#or
#make -C build -j all doc

#show files in build dir
ls -al build

#start tests
#cd build
#make test ARGS="-V"
