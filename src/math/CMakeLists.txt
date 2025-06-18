
add_library(math
    vec3.cc
    Math.cc

    vecmat.cc
    transform.cc

    shape.cc
    box2D.cc
)

target_include_directories(math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/
)




add_executable(test_math
    test/test_math.cc
)
target_link_libraries(test_math 
    math
    lib
)
target_include_directories(test_math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)