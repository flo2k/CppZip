#!/bin/bash
[ -d build ] && rm -Rf build
mkdir build

cmake -H. -Bbuild
cmake --build build -- -j
ls -al build
