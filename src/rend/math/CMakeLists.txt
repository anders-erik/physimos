
add_library(rend_math

    renderer_graph.cc

)

target_include_directories(rend_math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(rend_math 
    math
    lib
    rend
)

