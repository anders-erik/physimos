

# MODEL
set(MODEL_OBJS 
    model.cc
    obj.cc

    # XP EDITOR
    # scene.cc
    # camera.cc
    # renderer_model.cc
)
add_library(model ${MODEL_OBJS})
target_include_directories(model PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)
target_link_libraries(model
    glad
    scene
)





add_executable(main_editor 
    main_editor.cc
)
target_include_directories(main_editor PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/extern/glad/include
    ${CMAKE_SOURCE_DIR}/extern/GLFW/include
)
target_link_libraries(main_editor
    model

    window
    lib
    math
    opengl

    glad 
    ${CMAKE_SOURCE_DIR}/extern/GLFW/lib/libglfw3.a
)

