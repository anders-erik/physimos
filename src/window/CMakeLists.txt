
if(LINUX)
    add_subdirectory(wayland)
endif()



set(WINDOW_SRC 
    auxwin.cc
    glfw.cc
    glfw_key.cc
    coordinate_transform.cc
)

# window lib definition

# Defaults to GLFW

# check PHY_NATWIN=1
if(PHY_BUILD_NATWIN EQUAL 1)
    add_definitions(-DPHY_NATWIN=1)
endif()


# if(DEFINED PHY_WINDOW_LIB)
#     set(PHY_WINDOW_LIB GLFW)
#     add_definitions(-DPHY_WINDOW_LIB=GLFW)
# elseif(PHY_WINDOW_LIB STREQUAL GLFW)
#     add_definitions(-DPHY_WINDOW_LIB=GLFW)
# elseif(PHY_WINDOW_LIB STREQUAL NATWIN)
#     add_definitions(-DPHY_WINDOW_LIB=NATWIN)
# else()
#     message(FATAL_ERROR "PHY_WINDOW_LIB must be either GLFW or NATWIN")
#     # stop processing
#     return()
# endif()



add_library(window ${WINDOW_SRC})

target_include_directories(window PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)

target_link_libraries( window
    lib
    scene
)

