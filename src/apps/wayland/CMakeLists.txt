



add_executable(main_wayland main_wayland.cc)
target_link_libraries(main_wayland
    lib
    math
    swrend
    wayland
)
