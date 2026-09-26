

add_library(ui4_font_bitmap
    ui_font_bitmap.cc
)

target_link_libraries(ui4_font_bitmap
    lib
    bmp2
    math
)




add_library(ui4
    ui.cc
)

target_link_libraries(ui4
    lib
    bmp2
    math
    ui4_font_bitmap
)


