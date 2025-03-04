
add_library(uic_right_panel
    uic_right_panel.cc

    uic_right_panel_ui_editor.cc
    uic_right_panel_draw.cc
)


target_include_directories( uic_right_panel PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui/
    ${CMAKE_SOURCE_DIR}/src/ui/uic/

    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

