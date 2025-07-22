
# add_library(gltf 
# )

# target_link_libraries(gltf
#     lib
# )

# target_include_directories(gltf PUBLIC 
#     .
#     ${CMAKE_SOURCE_DIR}/src
# )


add_executable( main_gltf
    main_gltf.cc
)

include_directories( main_gltf
    .
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main_gltf
    lib
    math
    io
)
