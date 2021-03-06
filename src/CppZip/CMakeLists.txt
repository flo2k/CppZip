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
    ${ZLIB_INCLUDE_DIRS}
    ../external

)

if(USE_BUILD_IN_ZLIB)
    #only add the internal zlib headers if the build-in zlib was build
    include_directories(${CMAKE_BINARY_DIR}/src/external/zlib)
endif()

MESSAGE(STATUS "cppzip lib: ZLIB_INCLUDE_DIRS: " ${ZLIB_INCLUDE_DIRS})

#set definition for import/export for windows dll
add_definitions(-DCPPZIP_LIBRARY)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
endif()

#Sources
set(SOURCES
    #minizip
    ../external/minizip/ioapi.c
    ../external/minizip/mztools.c
    ../external/minizip/unzip.c
    ../external/minizip/zip.c

    #cppzip
    Unzip.cpp
    Zip.cpp
    ZipCommon.cpp
    ZipPrivate.cpp
)

if(WIN32)
    set(SOURCES
        ${SOURCES}
        ../external/minizip/iowin32.c
    )
endif()

#####################################
#Library
#####################################

#static version
#add_library(cppzip ${SOURCES})

link_directories(${Boost_LIBRARY_DIRS})

#shared version
add_library(cppzip SHARED ${SOURCES})

if(TARGET zlib)
    add_dependencies(cppzip zlib)
endif()

#add_dependencies(cppzip minizip)
target_link_libraries(cppzip
    ${ZLIB_LIBRARIES}
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

#copy public headers
install(FILES CppZip.h CppZip_Global.h ZipDefines.h Unzip.h Zip.h
    DESTINATION include/cppzip
)

#copy public headers to build include dir
set(CPPZIP_BUILD_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include/cppzip)
file(COPY CppZip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY CppZip_Global.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY ZipDefines.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY Unzip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
file(COPY Zip.h DESTINATION ${CPPZIP_BUILD_INCLUDE_DIR})
