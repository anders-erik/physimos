
add_library(opengl
    shader.cc
    program.cc
    texture.cc
    error.cc

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
    math
    window
    rend
    glad
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)
