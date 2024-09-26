
add_library(loaders 
    bmp_loader.cpp 
    png_loader.cpp 
    obj_loader.cpp
)

target_include_directories(loaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)