
add_library(json 
    json.cc
    json_lexer.cc
    json_parser.cc
    json_serialize.cc

    json_variant_playground.cc
)

target_link_libraries(json
    lib
)

target_include_directories(json PUBLIC 
    .
    ${CMAKE_SOURCE_DIR}/src
)


add_executable( main_json
    main_json.cc
)

include_directories( main_json
    .
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main_json
    lib
    math
    io
)
