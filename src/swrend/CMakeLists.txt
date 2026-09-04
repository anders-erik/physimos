
add_executable(main_swrend main_swrend.cc)

target_link_libraries(main_swrend
    lib
    math
    wayland-client # this was all it took to find wayland symbols...
)
