
add_library(ui
    ui.cc
    ui_list.cc
    ui_primitive.cc
    ui_globals.cc
    UiPObject.cc
    UiPScene.cc
    font.cc
)

target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)

