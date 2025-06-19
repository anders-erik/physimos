
set(REND_SCENE3D

    renderer3D.cc

    program_vector.cc
    program_axes.cc
    program_model_texture.cc
    program_model_color.cc
    program_mesh.cc
    program_object_ids.cc
    program_quad.cc
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

