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
        pan_x           { Primitive("pan x")},
        pan_y           { Primitive("pan y")},
        texture_x       { Primitive("text x")},
        texture_y       { Primitive("text y")},
        texture_px_x    { Primitive("text px x")},
        texture_px_y    { Primitive("text px y")},
        main_view_x     { Primitive("x")},
        main_view_y     { Primitive("y")}
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
    zoom.set_x("<100x");
    zoom.set_y("_150x");

    appendChild(&pan_x);
    pan_x.str_fontSize = FontSize::f18;
    pan_x.set_x("<50x");
    pan_x.set_y("_120x");

    appendChild(&pan_y);
    pan_y.str_fontSize = FontSize::f18;
    pan_y.set_x("<150x");
    pan_y.set_y("_120x");


    appendChild(&texture_x);
    texture_x.str_fontSize = FontSize::f18;
    texture_x.set_x("<50x");
    texture_x.set_y("_80x");

    appendChild(&texture_y);
    texture_y.str_fontSize = FontSize::f18;
    texture_y.set_x("<150x");
    texture_y.set_y("_80x");

    appendChild(&texture_px_x);
    texture_px_x.str_fontSize = FontSize::f18;
    texture_px_x.set_x("<50x");
    texture_px_x.set_y("_50x");

    appendChild(&texture_px_y);
    texture_px_y.str_fontSize = FontSize::f18;
    texture_px_y.set_x("<150x");
    texture_px_y.set_y("_50x");


    appendChild(&main_view_x);
    main_view_x.str_fontSize = FontSize::f15;
    main_view_x.set_x("<50x");
    main_view_x.set_y("_20x");

    appendChild(&main_view_y);
    main_view_y.str_fontSize = FontSize::f15;
    main_view_y.set_x("<150x");
    main_view_y.set_y("_20x");

}

void UIC_Draw_Transform::render_component(){
    render();

    title.render();

    // zoom.render();
    zoom.render_component();
    
    pan_x.render();
    pan_y.render();
    
    texture_x.render();
    texture_y.render();
    
    texture_px_y.render();
    texture_px_x.render();

    main_view_x.render();
    main_view_y.render();
}

void UIC_Draw_Transform::update_component(){
    
    draw::TransformContext transform_context = boundObject;

    zoom.set_value(transform_context.zoom);



    // Trigger rerender on main_view position change
    if( transform_context.cursor_pos_scene_px.x != transform_context_cache.cursor_pos_scene_px.x 
        || transform_context.cursor_pos_scene_px.y != transform_context_cache.cursor_pos_scene_px.y 
        || transform_context.zoom != transform_context_cache.zoom 
    ){
        transform_context_cache.cursor_pos_scene_px.x    = transform_context.cursor_pos_scene_px.x;
        transform_context_cache.cursor_pos_scene_px.y    = transform_context.cursor_pos_scene_px.y;
        transform_context_cache.zoom            = transform_context.zoom;

        // zoom.update_str_double(transform_context.zoom, 4);
        

        pan_x.update_str_double(transform_context.pan_texture_pos.x, 5);
        pan_y.update_str_double(transform_context.pan_texture_pos.y, 5);


        texture_x.update_str_double(transform_context.cursor_pos_scene.x, 5);
        texture_y.update_str_double(transform_context.cursor_pos_scene.y, 5);
        
        texture_px_x.update_str_int(transform_context.cursor_pos_scene_px.x);
        texture_px_y.update_str_int(transform_context.cursor_pos_scene_px.y);

        
        main_view_x.update_str_double(transform_context.cursor_pos_main_view.x, 5);
        main_view_y.update_str_double(transform_context.cursor_pos_main_view.y, 5);
    }
}



}
