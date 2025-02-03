
add_library(ui 
    ui_list.cc
    ui_element.cc
    ui_primitive.cc
    ui_globals.cc
    UiWorldObject.cc
    font.cc
)

target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)

