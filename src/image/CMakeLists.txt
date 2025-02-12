
add_library(image
    bmp.cc
)

target_include_directories(image PUBLIC 
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/
    
    ${CMAKE_SOURCE_DIR}/src/
)