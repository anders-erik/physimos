
add_library(image
    bitmap.cc
    bmp.cc
)

target_include_directories(image PUBLIC 
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/
    
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(image 
    lib
)