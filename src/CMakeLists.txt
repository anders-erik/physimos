

add_subdirectory(loaders)
add_subdirectory(simulator)
add_subdirectory(math)
add_subdirectory(render)
add_subdirectory(objects)


set(SOURCES 
    Input.cpp 
    ui.cpp
    Simulation.cpp 
    Camera.cpp 
    Windowing.cpp 
    WorldObject.cpp 
    WorldRenderer.cpp 
    Timing.cpp 
    WorldScene.cpp 
    RigidBody.cpp 
    VectorMath.cpp 
    process_info.cpp 
    uiRenderer.cpp 
    SimWorldContainer.cpp
)

#   set(SOURCES )
add_library(source ${SOURCES})
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/loaders)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/simulator)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/render)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/objects)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)

target_link_libraries(source loaders simulator render objects)

