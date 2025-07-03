
set(REND_SCENE3D

    renderer3D.cc

    shader_vector.cc
    shader_axes.cc
    shader_model_texture.cc
    shader_model_color.cc
    shader_mesh.cc
    shader_object_ids.cc
    shader_quad.cc
    shader_color_light.cc
)


add_library(rend_scene3D ${REND_SCENE3D})

target_include_directories(rend_scene3D PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/ 
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(rend_scene3D
    scene
    opengl
    math
    lib
)

