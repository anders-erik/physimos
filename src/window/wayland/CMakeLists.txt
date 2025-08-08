# set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBDECOR REQUIRED libdecor-0)
include_directories(${LIBDECOR_INCLUDE_DIRS})
link_libraries(${LIBDECOR_LIBRARIES})
execute_process (
    OUTPUT_STRIP_TRAILING_WHITESPACE

    COMMAND bash -c "pkg-config --cflags --libs libdecor-0"
    OUTPUT_VARIABLE libdecor_flags
)

add_executable(main_wayland_client
    main_wayland_client.cc
)
target_link_libraries(main_wayland_client PUBLIC 
    wayland-client
)

add_executable(main_wayland_server
    main_wayland_server.cc
)
target_link_libraries(main_wayland_server PUBLIC 
    wayland-server
)


add_executable(main_drm
    main_drm.cc
)
target_include_directories(main_drm PUBLIC 
    /usr/include/drm # libdrm-dev uses <drm.h> isntead of <drm/drm.h>
)
target_link_libraries(main_drm PUBLIC 
    drm
)
execute_process(
    COMMAND bash -c "dpkg -l | grep libdrm-dev"
    OUTPUT_VARIABLE libdrm_dev
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND bash -c "dpkg -l | grep libdrm2"
    OUTPUT_VARIABLE libdrm_runtime
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "libdrm_dev: _${libdrm_dev}_")
message(STATUS "libdrm_runtime: _${libdrm_runtime}_")
if("${libdrm_dev}" STREQUAL "")
    message(FATAL_ERROR "libdrm-dev not installed.")
elseif("${libdrm_runtime}" STREQUAL "")
    message(FATAL_ERROR "libdrm-runtime not installed.")
endif()




# set_target_properties(wayland PROPERTIES LINKER_LANGUAGE C)

# get lib name and remove trailing whitespace
execute_process (
    # COMMAND bash -c "pkg-config --cflags --libs wayland-client | sed 's/-l//g' | tr -d '\n' | tr -d ' '"
    # COMMAND bash -c "pkg-config --cflags --libs wayland-client | tr -d '\n' | tr -d ' '"
    OUTPUT_STRIP_TRAILING_WHITESPACE

    # COMMAND bash -c "pkg-config --libs wayland-client"
    COMMAND bash -c "pkg-config --cflags --libs wayland-client"
    OUTPUT_VARIABLE client_libs
)
# print client_libs
message(STATUS "client_libs: _${client_libs}_")

set(WAYLAND 
    main_wayland.c
    protocols/xdg-shell-protocol.c
)

set(CMAKE_C_COMPILER gcc)
add_executable(main_wayland ${WAYLAND})
target_link_libraries(main_wayland 
    ${client_libs}
    lib
)
target_compile_options(main_wayland PRIVATE -g)
target_include_directories(main_wayland PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src
)
unset(CMAKE_C_COMPILER)



# EGL
set(WAYLAND_EGL 
    main_wayland_egl.cc
    protocols/xdg-shell-protocol.c
    ${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c
)
add_executable(main_wayland_egl ${WAYLAND_EGL})
target_link_directories( main_wayland_egl PUBLIC
    /usr/lib/x86_64-linux-gnu/libdecor/plugins-1/
)
target_link_libraries(main_wayland_egl 
    ${client_libs}
    EGL # eglInitialize, eglGetDisplay, ...
    wayland-egl # wl_egl_window_create
    lib
    wayland-cursor
    # libdecor-0
    decor-0
    decor-cairo
)
target_compile_options(main_wayland_egl PRIVATE -g)
# target_compile_options(main_wayland_egl PRIVATE ${libdecor_flags})
target_include_directories(main_wayland_egl PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
    /usr/include/libdecor-0
)


# DECOR
set(WAYLAND_DECOR 
    main_wayland_decor.cc
    protocols/xdg-shell-protocol.c
    ${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c
)
add_executable(main_wayland_decor ${WAYLAND_DECOR})
target_link_directories( main_wayland_decor PUBLIC
    /usr/lib/x86_64-linux-gnu/libdecor/plugins-1/
)
target_link_libraries(main_wayland_decor 
    ${client_libs}
    EGL # eglInitialize, eglGetDisplay, ...
    wayland-egl # wl_egl_window_create
    lib
    # libdecor-0
    decor-0
    decor-cairo
)
target_compile_options(main_wayland_decor PRIVATE -g)
# target_compile_options(main_wayland_egl PRIVATE ${libdecor_flags})
target_include_directories(main_wayland_decor PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
    /usr/include/libdecor-0
)