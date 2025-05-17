
add_subdirectory(json)

add_library(lib

    string.cc
    fs.cc
    result.cc
    toml.cc

    log.cc
    process.cc
)


# Currently lib should be self contained, with the exception of the standard library
target_include_directories(lib PUBLIC 
    .
)