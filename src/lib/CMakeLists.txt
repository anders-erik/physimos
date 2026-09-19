

add_library(lib

    alloc.cc
    bitmap.cc
    err.cc
    file.cc
    fs.cc
    log.cc
    print.cc
    process.cc
    result.cc
    str.cc
    string.cc
    toml.cc

    fs/linux.cc
    fs/windows.cc
)


# Currently lib should be self contained, with the exception of the standard library
target_include_directories(lib PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src/
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


