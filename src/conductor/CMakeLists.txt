


set(CONDUCTOR_OBJS

    conductor2D.cc
)


add_library(conductor ${CONDUCTOR_OBJS})


target_include_directories(conductor PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/ 
    .
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(conductor 
    phont
    scene
    window
)

