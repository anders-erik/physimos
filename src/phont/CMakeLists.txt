
set(PHONT 
    phont.cc
)


add_library(phont ${PHONT})

target_include_directories(phont PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/glad/include/
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include/
)
target_link_libraries(phont
    lib
)


add_executable(main_phont
    main_phont.cc 
)
target_link_libraries(main_phont
    phont

    conductor
    ui
    image
    window
    math

    lib
    opengl

    glad 
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)



# target_include_directories(phont PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)
# target_include_directories(phont PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include/)



# target_link_libraries(phont )

