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

#Resources for executing the tests
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}" PATTERN "CppZipTest/*.zip")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/another_test.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/passwordProtected_linux_64bit.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/passwordProtected_win_64bit_7zip.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/täst.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/write_protected_file.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/passwordProtected_linux_32bit.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/passwordProtected_win_32bit.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/passwordProtected_win_64bit_winrar.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/zipFileFor_deleteAndReplaceTests.zip DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
