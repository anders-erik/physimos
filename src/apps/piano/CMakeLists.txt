

add_library(piano_ui piano_ui.cc)
target_link_libraries(piano_ui
    lib
    ui
)



add_executable(main_piano main_piano.cc)

target_link_libraries(main_piano
    lib
    math
    swrend
    wayland
    audio

    piano_ui
)
