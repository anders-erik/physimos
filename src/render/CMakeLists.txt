

add_subdirectory(math)

set(RENDER_OBJS 
    shader.cpp
    wo_renderer.cpp

    bitmap.cc
)

#   set(SOURCES )
add_library(render ${RENDER_OBJS})
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/)
# target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/render)
# target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/objects)
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)

target_link_libraries(render
    render_math
    lib
)




add_executable(main_render 
    main_render.cc
)

target_include_directories(main_render PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(main_render 
    render 
    math
    file_format
)

