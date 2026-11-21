

add_library(geometry_camera geo_camera.cc)
target_link_libraries(geometry_camera
    lib
    math
)

add_executable(main_geometry main_geometry.cc)

target_link_libraries(main_geometry
    lib
    math
    WL
    WL-egl

    geometry_camera
)