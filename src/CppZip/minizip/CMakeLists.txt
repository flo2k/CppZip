#####################################
# CMake file for Minizip
#####################################

#Sources
set(SOURCES
    ioapi.c
    mztools.c  
    unzip.c
    zip.c
)

if(WIN32)
    set(SOURCES
        ${SOURCES}
        iowin32.c
    )
endif(WIN32)

#Includes
include_directories(
    ${ZLIB_INCLUDE_DIR}
)

#Library
add_library(minizip STATIC ${SOURCES})

