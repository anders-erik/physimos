
add_executable(test_lib
    test_lib.cc
)

target_link_libraries(test_lib lib)

target_include_directories(test_lib PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)
