
add_subdirectory(json)
add_subdirectory(bmp)

add_library(file_format 
    bmp_loader.cpp 
    png_loader.cpp 
    obj_loader.cpp
    pso_loader.cc
    resources.cc
    texture_gen.cc

    test/test_file_format.cc
)

target_link_libraries(file_format 
    bmp
)

target_include_directories(file_format PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_include_directories(file_format PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(file_format PUBLIC ${CMAKE_SOURCE_DIR}/src/logger)

# for texture_gen access to gl api
target_include_directories(file_format PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)



add_executable(test_file_format
    test/test_file_format.cc
)
target_link_libraries(test_file_format
    file_format
    lib
    rend
)
target_include_directories(test_file_format PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)