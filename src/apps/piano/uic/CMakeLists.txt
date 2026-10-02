
# uics used for the ui4 library
add_library(uic4
    uic_piano.cc
    uic_song.cc
)
target_link_libraries(uic4
    lib
    ui4
)
include_directories(uic4
    .
    .. # enable sane includes from uic directory
    PUBLIC ${CMAKE_SOURCE_DIR}/src/
)


