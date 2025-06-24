
set(REND_PUI

    renderer_base.cc

    shader_base.cc
    shader_string.cc
    shader_texture.cc
)


add_library(rend_pui ${REND_PUI})


target_include_directories(rend_pui PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/ 
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(rend_pui
    ui
    opengl
    math
    lib
)

