


set(SCENE_OBJS 
    test.cc
    pscene.cc
    WorldObject.cpp
    WorldSimulator.cc
    Camera.cpp
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
)

