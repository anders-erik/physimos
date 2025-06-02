
add_library(ui_elements

    string.cc

)

target_include_directories(ui_elements PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/extern/glad/include
)


target_link_libraries(ui_elements
    ui
)