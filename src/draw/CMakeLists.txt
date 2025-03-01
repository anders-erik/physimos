
add_library(draw
    draw.cc
    draw_shader.cc
)

target_include_directories(draw PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/

    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)