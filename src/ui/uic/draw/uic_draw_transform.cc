#include <iostream>


#include "lib/lib.hh"

#include "draw/draw.hh"

#include <random>

#include "uic_primitive_color.hh"
#include "ui/uic/draw/uic_draw_transform.hh"

namespace UI::component {


UIC_Draw_Transform::UIC_Draw_Transform()
    :   boundObject     { draw::ui_get_transform_context() },
        title           { Primitive("Transform") },
        zoom            { UIC_Label_float("Zoom: ", 0.0f, 5)},
        pan             { UIC_Label_f2("Pan: ", {0.0f, 0.0f}, 5)},
        scene_pos       { UIC_Label_f2("Scene Pos.: ", {0.0f, 0.0f}, 5)},
        scene_pos_px       { UIC_Label_f2("Sc. Pos. px: ", {0.0f, 0.0f}, 5)},
        main_view_px       { UIC_Label_f2("Main View px: ", {0.0f, 0.0f}, 5)}
{

    id = "UIC_Draw_Transform";
    set_w("90%");
    set_h("200x");

    set_x("|");
    set_y("_10x");
    
    set_color({0.0, 0.0, 1.0, 0.3});

    appendChild(&title);
    title.str_setFontSize(FontSize::f15);
    title.set_x("<10x");
    title.set_y("^5x");


    appendChild(&zoom);
    // zoom.max_chars = 5;
    zoom.str_fontSize = FontSize::f18;
    zoom.set_value(1.234);
    zoom.set_x("<0x");
    zoom.set_y("_140x");

    appendChild(&pan);
    // zoom.max_chars = 5;
    pan.str_fontSize = FontSize::f18;
    pan.set_value({0.0f, 0.0f});
    pan.set_x("<0x");
    pan.set_y("_110x");

    appendChild(&scene_pos);
    // zoom.max_chars = 5;
    scene_pos.str_fontSize = FontSize::f18;
    scene_pos.set_value({0.0f, 0.0f});
    scene_pos.set_x("<0x");
    scene_pos.set_y("_80x");

    appendChild(&scene_pos_px);
    // zoom.max_chars = 5;
    scene_pos_px.str_fontSize = FontSize::f18;
    scene_pos_px.set_value({0.0f, 0.0f});
    scene_pos_px.set_x("<0x");
    scene_pos_px.set_y("_50x");

    appendChild(&main_view_px);
    // zoom.max_chars = 5;
    main_view_px.str_fontSize = FontSize::f18;
    main_view_px.set_value({0.0f, 0.0f});
    main_view_px.set_x("<0x");
    main_view_px.set_y("_20x");

}

void UIC_Draw_Transform::render_component(){
    render();

    title.render();

    zoom.render_component();
    pan.render_component();
    scene_pos.render_component();
    scene_pos_px.render_component();
    main_view_px.render_component();

}

void UIC_Draw_Transform::update_component(){
    
    draw::TransformContext transform_context = boundObject;

    zoom.set_value(transform_context.zoom);
    pan.set_value(transform_context.pan_texture_pos);
    scene_pos.set_value(transform_context.cursor_pos_scene);
    scene_pos_px.set_value({(float)transform_context.cursor_pos_scene_px.x, (float)transform_context.cursor_pos_scene_px.y});
    main_view_px.set_value(transform_context.cursor_pos_main_view);

}



}
