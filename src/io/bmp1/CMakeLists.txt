
add_library( bmp1
    bmp.cc
    bmp_header.cc
)

include_directories( bmp1
    .
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries( bmp1
    rend
)
