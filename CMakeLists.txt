cmake_minimum_required( VERSION "2.6.4" )

project( illustrations CXX C )


include_directories(
    include
)

set( ILLUSTRATIONS_SOURCES
    #src/png_boxes.cc
)

set( ILLUSTRATIONS_LIBS
    cairo
    png12
)



set( LOGPORT_COMPILE_OPTIONS
    -Wall
    -Wextra
    -O3
    -std=c++17
)

add_executable( build/illustrations ${ILLUSTRATIONS_SOURCES} src/png_boxes.cc )

target_link_libraries( build/illustrations ${ILLUSTRATIONS_LIBS} )

set_target_properties( build/illustrations
    PROPERTIES
    COMPILE_OPTIONS  "${LOGPORT_COMPILE_OPTIONS}"
)