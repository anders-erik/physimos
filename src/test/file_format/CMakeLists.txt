


add_executable(test_unit_file_format
    test_unit_file_format.cc
)

target_link_libraries(test_unit_file_format
    file_format
    lib
    rend
)

target_include_directories(test_unit_file_format PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)