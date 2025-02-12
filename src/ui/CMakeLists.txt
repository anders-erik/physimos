
add_library(ui
    ui.cc
    ui_primitive.cc
    ui_globals.cc
    font.cc
    # components
    uic_transform.cc
    uic_pobject.cc
    uic_scene.cc
)

target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include)

