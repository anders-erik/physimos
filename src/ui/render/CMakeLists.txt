
add_library(ui_render

    renderer_primitive.cc
    program_primitive_color.cc
    program_primitive_texture.cc
    program_primitive_string.cc

)

target_include_directories(ui_render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui_render PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui_render PUBLIC ${CMAKE_SOURCE_DIR}/src/ui/)
target_include_directories(ui_render PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)

target_link_libraries(ui_render
    ui
    opengl
)
