
if(LINUX)
    add_subdirectory(wayland)
endif()

set(WINDOW 
    auxwin.cc
    glfw_key.cc
    coordinate_transform.cc
)



add_library(window ${WINDOW})

target_include_directories(window PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)

target_link_libraries( window
    lib
    scene
)

