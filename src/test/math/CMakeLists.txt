

add_executable(test_math
    test_math.cc
)
target_link_libraries(test_math 
    math
    lib
)
target_include_directories(test_math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)