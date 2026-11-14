

add_executable(main_geometry main_geometry.cc)

target_link_libraries(main_geometry
    lib
    math
    WL
    WL-egl
)