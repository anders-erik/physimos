

add_library(lib

    string.cc
    fs.cc
    result.cc
    toml.cc

    log.cc
    process.cc

    str.cc
    file.cc

    alloc.cc
    print.cc

    err.cc

    fs/linux.cc
    fs/windows.cc
)


# Currently lib should be self contained, with the exception of the standard library
target_include_directories(lib PUBLIC 
    .
)


add_executable(main_lib
    main_lib.cc
)
add_executable(main_str
    main_str.cc
)
add_executable(main_file
    main_file.cc
)
target_link_libraries(main_lib  lib)
target_link_libraries(main_str  lib)
target_link_libraries(main_file lib)


