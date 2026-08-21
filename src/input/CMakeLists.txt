
# set(CMAKE_C_COMPILER gcc)

# add_executable( main_input main_input_c.c)
add_executable( main_input main_input.cc)

target_link_libraries( main_input
    lib
)

# target_link_libraries()

# unset(CMAKE_C_COMPILER)