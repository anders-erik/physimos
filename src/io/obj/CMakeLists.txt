

add_library(obj
    obj.cc
)

target_include_directories(obj PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(obj

)





