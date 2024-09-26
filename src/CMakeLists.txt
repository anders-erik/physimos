
set(SOURCES bmp_loader.cpp png_loader.cpp obj_loader.cpp Input.cpp ui.cpp shader.cpp Simulation.cpp Camera.cpp Windowing.cpp WorldObject.cpp WorldRenderer.cpp Model.cpp Timing.cpp WorldScene.cpp RigidBody.cpp VectorMath.cpp process_info.cpp uiRenderer.cpp SimWorldContainer.cpp wo_renderer.cpp Simulator.cpp Math.cpp)
#   set(SOURCES )
add_library(source ${SOURCES})
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)


# # src/loaders/CMakeLists.txt
# add_subdirectory(loaders)

# add_library(loaders bmp_loader.cpp png_loader.cpp obj_loader.cpp)
# target_include_directories(loaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)