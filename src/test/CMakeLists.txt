

add_subdirectory(file_format)
add_subdirectory(lib)
add_subdirectory(math)




add_executable(test
    test.cc
)
target_link_libraries(test 
    math
    lib
)
target_include_directories(test PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)