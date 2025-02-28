
add_library(uic_main_view
    uic_main_view.cc

    uic_main_view_ui_editor.cc
)


target_include_directories( uic_main_view PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui/
    ${CMAKE_SOURCE_DIR}/src/ui/uic/

    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

