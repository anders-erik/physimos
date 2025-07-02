

set(SCRIPT_OBJS

    script.cc
)


add_library(script ${SCRIPT_OBJS})


target_include_directories(script PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/ 
    .
    #extern
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
)


# What to link against
target_link_libraries(script
    lib
    io
    scene
)

