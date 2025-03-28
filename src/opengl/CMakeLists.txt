
add_library(opengl
    program.cc
    texture.cc
    error.cc

    renderer_model.cc
    renderer_model_texture.cc
    renderer_axes.cc
    renderer_vector.cc
)

target_include_directories(opengl PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/

    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)

target_link_libraries(opengl
    lib
    model
    math
)