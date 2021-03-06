#####################################
# CMake file for CppZipTest
#####################################

set(CPPZIP_TEST_WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME cppziptest COMMAND cppziptest
        WORKING_DIRECTORY ${CPPZIP_TEST_WORKING_DIR}
)
#to execute tests: make test
#to execute tests with output from cppziptest: make test ARGS="-V"

#Includes
include_directories(
    ${CMAKE_SOURCE_DIR}/src/CppZip
    ${Boost_INCLUDE_DIRS}
    ${CPPUNIT_INCLUDE_DIRS}
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
    ${CPPUNIT_LIBRARIES}
    cppzip
)

#Resources for executing the tests
#add dir for testdata as a define
add_definitions(-DCPPZIP_TEST_WORKING_DIR="${CPPZIP_TEST_WORKING_DIR}")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CPPZIP_TEST_WORKING_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testZips DESTINATION ${CPPZIP_TEST_WORKING_DIR})
