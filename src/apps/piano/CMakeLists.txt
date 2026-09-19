
add_executable(main_piano main_piano.cc)

target_link_libraries(main_piano
    lib
    math
    swrend
    wayland
    audio
)
