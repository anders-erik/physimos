


set(OBJECT_OBJS 
    Model.cpp
    loaded_resources.cc
)

#   set(SOURCES )
add_library(objects ${OBJECT_OBJS})
target_include_directories(objects PUBLIC ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(objects PUBLIC ${CMAKE_SOURCE_DIR}/src/loaders)
# target_include_directories(objects PUBLIC ${CMAKE_SOURCE_DIR}/src/objects)
target_include_directories(objects PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include/)
target_include_directories(objects PUBLIC ${CMAKE_SOURCE_DIR}/extern/GLFW/include/)

target_link_libraries(objects loaders)


