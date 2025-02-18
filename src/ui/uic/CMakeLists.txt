
add_library(uic
    uic_primitive_editor.cc
    uic_primitive_color.cc
)


target_include_directories( uic PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}
)

