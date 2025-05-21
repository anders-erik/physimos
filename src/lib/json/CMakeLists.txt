
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
)
