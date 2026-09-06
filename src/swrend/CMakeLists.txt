


# 
# XDG Shell wayland extension
# 
set(CMAKE_C_COMPILER gcc)

add_library(xdg_wl_shell
    xdg_wl_extension/xdg-shell-protocol.c
)
target_link_libraries(xdg_wl_shell 
    wayland-client
)
target_compile_options(xdg_wl_shell PRIVATE -g)

unset(CMAKE_C_COMPILER)


add_library(wayland wayland.cc)
target_link_libraries(wayland
    lib
    math
)

add_executable(main_swrend main_swrend.cc)
target_link_libraries(main_swrend
    lib
    math
    wayland
    wayland-client # this was all it took to find wayland symbols...
    xdg_wl_shell
)
