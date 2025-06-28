
add_subdirectory(json)
add_subdirectory(bmp)
add_subdirectory(obj)

add_library(io 
    bmp_loader.cpp 
    png_loader.cpp 
    obj_loader.cpp
    pso_loader.cc
    resources.cc
    texture_gen.cc

    pretty_str.cc
)

target_link_libraries(io 
    bmp
    lib
)

target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_include_directories(io PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(io PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)

# for texture_gen access to gl api
target_include_directories(io PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)


