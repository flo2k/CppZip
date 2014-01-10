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

install(FILES CppZip.h  Unzip.h ZipCommon.h  Zip.h  ZipPrivate.h 
    DESTINATION include/cppzip
)
