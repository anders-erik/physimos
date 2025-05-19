

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


set(WAYLAND 
    main.c
    protocols/xdg-shell-protocol.c
)

set(CMAKE_C_COMPILER gcc)

add_executable(wayland ${WAYLAND})

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


# target_link_libraries(wayland wayland-client)
target_link_libraries(wayland ${client_libs})

target_compile_options(wayland PRIVATE -g)

target_include_directories(wayland PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)

# target_link_libraries( window
#     lib
# )
# unset(CMAKE_C_COMPILER)
