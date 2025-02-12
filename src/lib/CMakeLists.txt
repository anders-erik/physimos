
add_library(lib

    string.cc
    result.cc

)


# Currently lib should be self contained, with the exception of the standard library
target_include_directories(lib PUBLIC 
    .
)