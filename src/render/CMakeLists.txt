


set(RENDER_OBJS 
    shader.cpp
    wo_renderer.cpp
)

#   set(SOURCES )
add_library(render ${RENDER_OBJS})
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/)
# target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/render)
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/src/objects)
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)

target_link_libraries(render objects)


