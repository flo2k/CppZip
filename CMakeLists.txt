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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

MESSAGE(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
MESSAGE(STATUS "LIBRARY_OUTPUT_PATH: " ${LIBRARY_OUTPUT_PATH})
MESSAGE(STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH})

#debugging: enables this snippet to print all known variables...
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

#####################################
# External Dependencies for CppZip
#####################################

#zlib
find_package(ZLIB 1.2.8)

if(NOT ZLIB_FOUND)
    MESSAGE(STATUS "Use zlib provided by cppzip, because it is not found on the system.")

    add_subdirectory(src/external/zlib)
    set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/external/zlib)
    set(ZLIB_LIBRARY zlib)
    find_package(ZLIB 1.2.8 REQUIRED)
endif()

#minizip: currently, minizip sources are included in CppZip lib directly
#add_subdirectory(src/CppZip/minizip)

#Boost stuff
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system filesystem signals)

#CppZip
add_subdirectory(src/CppZip)

#Documentation with Doxygen
#add_subdirectory(doc)

#####################################
# External additionally dependencies for CppZipTest
#####################################

if(BUILD_TESTS)
	#Boost
	find_package(Boost 1.53.0 COMPONENTS system filesystem signals timer)

    MESSAGE(STATUS "CPPUNIT_INCLUDE_DIR: " ${CPPUNIT_INCLUDE_DIR})
    MESSAGE(STATUS "CPPUNIT_LIBRARY: " ${CPPUNIT_LIBRARY})

	#CppUnit
	find_package(CppUnit REQUIRED)

	enable_testing()
	option(BUILD_TESTS "Build unit tests" ON)

	#CppZipTest
	add_subdirectory(src/CppZipTest)
endif()
#####################################
# Installing files: output
#####################################
