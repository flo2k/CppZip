#####################################
# CMake file for CppZipTest
#####################################

#Includes
include_directories(
    ${CMAKE_SOURCE_DIR}/src/CppZip
    ${Boost_INCLUDE_DIRS}
    ${CPPUNIT_INCLUDE_DIR}
)

#Sources
set(SOURCES
    main.cpp
    PerformanceTests.cpp
    UnzipTest.cpp
    ZipTest.cpp
)

#CppZipTest Executable
add_executable(cppziptest ${SOURCES})

add_dependencies(cppziptest cppzip)

#addidtional dependencies
target_link_libraries(cppziptest
    ${Boost_LIBRARIES}
    ${CPPUNIT_LIBRARY}
    cppzip
)
