
add_subdirectory(res)
add_subdirectory(simulator)
add_subdirectory(math)
add_subdirectory(render)
add_subdirectory(logger)
add_subdirectory(ui)
add_subdirectory(model)
add_subdirectory(image)
add_subdirectory(lib)
add_subdirectory(scene)


set(SOURCES 
    conductor.cc
    conductor_viewport.cc
    
    Input.cpp
    Simulation.cpp
    Windowing.cpp
    Timing.cpp
    RigidBody.cpp
    Model.cpp
    transform.cc
)



add_library(source ${SOURCES})
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/res)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/scene)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/ui)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/simulator)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/render)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/model)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)

target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include/)


target_link_libraries(source res simulator render scene lib)

