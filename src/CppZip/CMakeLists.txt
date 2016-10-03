#####################################
# CMake file for CppZip
#####################################

#Documentation with Doxygen
set(DOXYFILE_OUTPUT_DIR
    ${CMAKE_BINARY_DIR}/doc
)
include(UseDoxygen OPTIONAL)

#Includes
include_directories(
    ${Boost_INCLUDE_DIRS}
 
    ${ZLIB_INCLUDE_DIR} #minizip
)

#Sources
set(SOURCES

    #minizip
    minizip/ioapi.c
    minizip/mztools.c  
    minizip/unzip.c
    minizip/zip.c

    #cppzip
    Unzip.cpp
    Zip.cpp
    ZipPrivate.cpp
)

#####################################
#Library
#####################################

#static version
#add_library(cppzip ${SOURCES})

#shared version
add_library(cppzip SHARED ${SOURCES})

#add_dependencies(cppzip minizip)

#addidtional dependencies
target_link_libraries(cppzip
    ${ZLIB_LIBRARY}
    ${Boost_LIBRARIES}
    #minizip
)

set(VERSION_STRING "${CPPZIP_VERSION_MAJOR}.${CPPZIP_VERSION_MINOR}.${CPPZIP_VERSION_PATCH}")

set_target_properties(cppzip PROPERTIES
    VERSION ${VERSION_STRING}
    SOVERSION ${VERSION_STRING}
    FOLDER "library"
)


#####################################
#INSTALL
#####################################

set(LIB_DIR lib${LIB_SUFFIX})

if(WIN32)
    install (TARGETS cppzip 
        RUNTIME DESTINATION .
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
else()
    install (TARGETS cppzip
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_DIR}
        ARCHIVE DESTINATION ${LIB_DIR}
    )
endif()

install(FILES CppZip.h  Unzip.h Zip.h ZipCommon.h
    DESTINATION include/cppzip
)

#copy to build include dir
set(CPPZIP_BUILD_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include/cppzip)
file(COPY CppZip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY Unzip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY Zip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY ZipCommon.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})

