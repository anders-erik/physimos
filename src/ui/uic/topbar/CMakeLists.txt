
add_library(uic_topbar
    uic_topbar.cc

    uic_grid_toggle.cc
    uic_main_states.cc
)


target_include_directories( uic_topbar PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/ui/
    ${CMAKE_SOURCE_DIR}/src/ui/uic/

    ${CMAKE_SOURCE_DIR}/extern/glad/include
)

