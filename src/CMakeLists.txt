
add_subdirectory(conductor)
add_subdirectory(draw)
add_subdirectory(io)
add_subdirectory(logger)
add_subdirectory(lib)
add_subdirectory(math)
add_subdirectory(opengl)
add_subdirectory(phont)
add_subdirectory(rend)
add_subdirectory(scene)
add_subdirectory(scene2D)
add_subdirectory(simulator)
add_subdirectory(test)
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
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/io)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/scene)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/ui)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/simulator)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/rend)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/model)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src/draw)

target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include/)


target_link_libraries(source opengl uic_draw io simulator rend scene lib)



add_executable(main_main
    main.cc 
)
target_link_libraries(main_main

    rend
    scene
    conductor
  
    ui
    window
    math

    lib
    opengl

    io

    glad 
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)
