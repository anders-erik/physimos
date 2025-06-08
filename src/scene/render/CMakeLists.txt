
set(SCENE_RENDER

    renderer2D.cc

    program_quad2D.cc

)


add_library(scene_render ${SCENE_RENDER})

target_include_directories(scene_render PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/ 
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(scene_render
    scene
    opengl
    math
)

