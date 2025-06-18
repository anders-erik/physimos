
set(SCENE_RENDER_3D

    renderer3D.cc

    program_vector.cc
    program_axes.cc
    program_model_texture.cc
    program_model_color.cc
    program_mesh.cc
    program_object_ids.cc
)


add_library(scene_render_3D ${SCENE_RENDER_3D})

target_include_directories(scene_render_3D PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/ 
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(scene_render_3D
    scene
    opengl
    math
)

