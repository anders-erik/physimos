

add_executable(test_unit_math
    test_unit_math.cc
)
target_link_libraries(test_unit_math 
    math
    lib
    io
)
target_include_directories(test_unit_math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)