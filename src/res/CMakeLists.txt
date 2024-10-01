
add_library(res 
    bmp_loader.cpp 
    png_loader.cpp 
    obj_loader.cpp
    pso_loader.cc
    resources.cc
    texture_gen.cc
)

target_include_directories(res PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_include_directories(res PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)

# for texture_gen access to gl api
target_include_directories(res PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)