
add_library(opengl
    program.cc
)

target_include_directories(opengl PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/

    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)

target_link_libraries(opengl
    lib
)