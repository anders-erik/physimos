
add_library( bmp
    bmp.cc
    bmp_header.cc
)

include_directories( bmp
    .
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries( bmp
    render
)
