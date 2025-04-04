


set(SCENE_OBJS
    pscene.cc
    pobject.cc
    WorldSimulator.cc
    Camera.cpp

    scene.cc
    camera.cc
    scene_2d.cc
    camera_2d.cc
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
    source # this will be changed to specific libraries as refactoring progress allows, and not the whole physimos source
    phont
)

