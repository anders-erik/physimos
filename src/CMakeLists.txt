
add_subdirectory(conductor)
add_subdirectory(draw)
add_subdirectory(file_format)
add_subdirectory(image)
add_subdirectory(logger)
add_subdirectory(lib)
add_subdirectory(math)
add_subdirectory(model)
add_subdirectory(opengl)
add_subdirectory(phont)
add_subdirectory(physics)
add_subdirectory(render)
add_subdirectory(scene)
add_subdirectory(simulator)
add_subdirectory(ui)
add_subdirectory(window)


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


if(MSVC)
  target_compile_options(source PRIVATE /W4 /WX)
else()
  target_compile_options(source PRIVATE -Wall )
endif()

target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/file_format)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/scene)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/ui)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/simulator)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/render)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/model)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/draw)

target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include/)


target_link_libraries(source opengl uic_draw file_format simulator render scene lib)

