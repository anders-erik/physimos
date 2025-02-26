
add_library(uic
    uic_root_topbar.cc
    uic_root_main_view.cc
    uic_root_left_panel.cc
    uic_root_right_panel.cc
    uic_root_workbench.cc

    uic_primitive_editor.cc
    uic_primitive_color.cc
    uic_primitive_position.cc
    uic_primitive_size.cc
    uic_primitive_list.cc
    uic_primitive_list_editor.cc
    uic_button.cc
)


target_include_directories( uic PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

