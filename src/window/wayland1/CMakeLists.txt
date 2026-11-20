

# set(WL_CALLBACKS "WL-callbacks")
# set(WL_CALLBACKS "WL-callbacks")
# set(WL_CALLBACKS "WL-callbacks")
# set(WL_CALLBACKS "WL-callbacks")



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

# NOTE: This is note currently being used!! / AE, 2026-11-13
add_library(wl_cursor_extension_v1
    wl_cursor_extension_v1/cursor-shape-v1-protocol.c
)
target_link_libraries(wl_cursor_extension_v1 
    wayland-client
)
target_compile_options(wl_cursor_extension_v1 PRIVATE -g)

unset(CMAKE_C_COMPILER)


add_library(WL-egl wl-egl.cc)
target_link_libraries(WL-egl
    lib
    math

    opengl_loader

    wayland-client

    EGL # eglInitialize, eglGetDisplay, ...
    wayland-egl # wl_egl_window_create
)

add_library(WL-pool wl-pool.cc)
target_link_libraries(WL-pool
    lib
    math
    swrend
    
    wayland-client
)

add_library(WL-surfaces wl-surfaces.cc)
target_link_libraries(WL-surfaces
    lib
    math
    swrend
    
    WL-pool
    WL-callbacks
    WL-egl
)


add_library(WL-objects wl-objects.cc)
target_link_libraries(WL-objects
    lib
    math

    wayland-client

    WL-callbacks
)

add_library(WL-callbacks wl-callbacks.cc)
target_link_libraries(WL-callbacks
    lib
    math
    wayland-client
)


add_library(WL wayland.cc wl-callbacks.cc)
target_link_libraries(WL
    lib
    math
    swrend

    wayland-client # this was all it took to find wayland symbols...
    wayland-cursor
    xdg_wl_shell

    WL-callbacks
    WL-objects
    WL-surfaces
)

