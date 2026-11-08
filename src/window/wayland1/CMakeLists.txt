

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

unset(CMAKE_C_COMPILER)



add_library(WL-egl wl-egl.cc)
target_link_libraries(WL-egl
    lib
    math
    wayland-client

    EGL # eglInitialize, eglGetDisplay, ...
    wayland-egl # wl_egl_window_create
)

add_library(WL-surfaces wl-surfaces.cc)
target_link_libraries(WL-surfaces
    lib
    math
    swrend
    
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

