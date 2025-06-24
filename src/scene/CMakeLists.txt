

set(SCENE_OBJS
    pscene.cc
    pobject.cc
    WorldSimulator.cc
    Camera.cpp
    model.cc

    camera.cc

    scene3D.cc
    object_manager.cc
    mesh.cc
    mesh_texture.cc

    manager_3D.cc
    scene_state.cc

    squad.cc
    object.cc
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
    rend
    source
    scene2D
)

