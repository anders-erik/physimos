


set(MODEL_OBJS 
    model.cc
    obj.cc
)

#   set(SOURCES )
add_library(model ${MODEL_OBJS})
target_include_directories(model PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(model)


