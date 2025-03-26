
add_library(math
    vec3.cc
    Math.cc

    math.cc
)

target_include_directories(math PUBLIC 
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/
)