
add_library(uic_draw
    uic_draw_transform.cc
    uic_draw_brush.cc
)


target_include_directories( uic_draw PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui/
    ${CMAKE_SOURCE_DIR}/src/ui/uic/

    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

target_link_libraries(uic_draw
    uic
)
