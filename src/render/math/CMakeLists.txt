
add_library(render_math

    renderer_graph.cc

)

target_include_directories(render_math PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(render_math 
    math
    lib
    render
)

