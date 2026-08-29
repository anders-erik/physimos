
add_executable(main_electronics main_electronics.cc)

# add_library(electronics main_electronics.cc)

target_link_libraries( main_electronics
    lib
)

