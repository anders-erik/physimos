
add_executable(test_unit_lib
    test_unit_lib.cc
)

target_link_libraries(test_unit_lib 
    lib
    io
)

target_include_directories(test_unit_lib PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)
