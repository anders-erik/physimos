
add_library(geometry
    shape.cc
)

target_include_directories(geometry PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)