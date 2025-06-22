

set(SCENE2D_OBJS
    
    scene2D.cc
    camera2D.cc
    shapeS2D.cc
    quadS2D.cc

    manager_2D.cc
    quad_manager.cc
)


add_library(scene2D ${SCENE2D_OBJS})


target_include_directories(scene2D PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/ 
    .
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


target_link_libraries(scene2D
    rend
    source
)

