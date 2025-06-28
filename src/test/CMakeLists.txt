

add_subdirectory(io)
add_subdirectory(lib)
add_subdirectory(math)




add_executable(test_unit
    test_unit.cc
)
target_link_libraries(test_unit 
    math
    lib
)
target_include_directories(test_unit PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)