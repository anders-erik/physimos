


add_executable(test_unit_io
    test_unit_io.cc
)

target_link_libraries(test_unit_io
    io
    lib
    rend
)

target_include_directories(test_unit_io PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)