#####################################
# Main CMake file for CppZip
#####################################
cmake_minimum_required(VERSION 2.8)

project(CppZip)

#Version informations
set (CPPZIP_VERSION_MAJOR 0)
set (CPPZIP_VERSION_MINOR 3)
set (CPPZIP_VERSION_PATCH 1)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(LIB_DIR lib${LIB_SUFFIX})

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR "" CACHE FILEPATH "Documentation install directory")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -fPIC -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
    add_definitions(/DBOOST_ALL_DYN_LINK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif(MSVC)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_definitions(-DDEBUG)
#endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

#####################################
# External Dependencies for CppZip
#####################################

#zlib
#find_package(ZLIB 1.2.8)
find_package(ZLIB)

if(NOT ZLIB_FOUND)
    add_subdirectory(src/CppZip/zlib)
endif()

#minizip
#add_subdirectory(src/CppZip/minizip)

#Boost stuff
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system filesystem signals)

#CppZip
add_subdirectory(src/CppZip)

#Documentation with Doxygen
add_subdirectory(doc)

#####################################
# External additionally dependencies for CppZipTest
#####################################

#Boost
find_package(Boost 1.53.0 COMPONENTS system filesystem signals timer)

#CppUnit
find_package(CppUnit)

if(CPPUNIT_FOUND AND Boost_FOUND)
    enable_testing()
    option(BUILD_TESTS "Build unit tests" ON)

    if(BUILD_TESTS)
        #CppZipTest
        add_subdirectory(src/CppZipTest)
    endif()
endif()

#####################################
# Installing files: output
#####################################
