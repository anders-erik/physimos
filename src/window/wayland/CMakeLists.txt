

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
