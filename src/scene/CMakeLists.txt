
add_subdirectory(render)
add_subdirectory(render3D)


set(SCENE_OBJS
    pscene.cc
    pobject.cc
    WorldSimulator.cc
    Camera.cpp

    scene.cc
    camera.cc

    scene2D.cc
    camera2D.cc
    shapeS2D.cc
    quadS2D.cc

    scene3D.cc
    object_manager.cc
    mesh.cc

    manager.cc
    quad_manager.cc

)


add_library(scene ${SCENE_OBJS})


target_include_directories(scene PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/ 
    .
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(scene
    scene_render
    scene_render_3D
    source # this will be changed to specific libraries as refactoring progress allows, and not the whole physimos source
    phont
)

