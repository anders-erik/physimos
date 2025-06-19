
add_library(opengl
    shader.cc
    program.cc
    texture.cc
    error.cc

    renderer_model_color.cc
    renderer_model_texture.cc
    renderer_axes.cc
    renderer_vector.cc

    texture_fb.cc
    texture_fbms.cc

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
    window
    rend
    glad
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)

add_executable(main_opengl
    main_opengl.cc
)
target_include_directories(main_opengl PUBLIC
    .
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)
target_link_libraries(main_opengl
    window

    lib
    opengl

    glad 
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)