add_subdirectory(uic)

add_library(ui
    ui.cc
    ui_primitive.cc
    ui_globals.cc
    ui_component.cc 
    font.cc
    # components
    uic_transform.cc
    uic_pobject.cc
    uic_scene.cc
    # uic_primitive_editor.cc
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include)

target_link_libraries(ui uic)