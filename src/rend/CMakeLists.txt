

add_subdirectory(math)
add_subdirectory(scene3D)
add_subdirectory(scene2D)
add_subdirectory(pui)


set(RENDER_OBJS 
    shader.cpp
    wo_renderer.cpp

    bitmap.cc

    rend_manager.cc
)

#   set(SOURCES )
add_library(rend ${RENDER_OBJS})
target_include_directories(rend PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(rend PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)

target_link_libraries(rend
    rend_math
    rend_scene3D
    rend_scene2D
    rend_pui
    lib
)




add_executable(main_rend 
    main_rend.cc
)

target_include_directories(main_rend PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(main_rend 
    rend 
    math
    file_format
)

