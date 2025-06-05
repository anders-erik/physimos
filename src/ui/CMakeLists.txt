
add_subdirectory(uic)
add_subdirectory(render)

add_library(ui

    ui.cc
    font_bitmap.cc
    ui_primitive.cc
    ui_globals.cc
    ui_texture.cc
    base.cc
    texture.cc
    pui.cc
    
    # dependencies removed
    ui_font.cc
    ui_shader_texture.cc
    ui_shader_color.cc
    ui_shader_string.cc

)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)

target_link_libraries(ui 
    ui_render

    image
)