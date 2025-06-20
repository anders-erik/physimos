
set(SCENE_RENDER

    renderer2D.cc

    program_quad2D.cc

    shader_graph_point.cc
    shader_graph_line.cc
)


add_library(rend_scene2D ${SCENE_RENDER})

target_include_directories(rend_scene2D PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/ 
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(rend_scene2D
    scene
    opengl
    math
)

