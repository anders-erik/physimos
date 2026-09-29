

add_library(piano_ui
    piano_ui.cc
    uic/uic_piano.cc
    uic/uic_song.cc
)
target_link_libraries(piano_ui
    lib
    bmp2
    ui4
)
include_directories(piano_ui
    . # enable sane includes from uic directory
)




add_executable(main_piano main_piano.cc)

target_link_libraries(main_piano
    lib
    math
    swrend
    
    wayland
    audio
    bmp2
    ui4
    piano_ui
)
