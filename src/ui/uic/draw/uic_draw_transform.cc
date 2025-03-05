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
        zoom            { Primitive("zoom")},
        pan_x           { Primitive("pan x")},
        pan_y           { Primitive("pan y")},
        texture_x       { Primitive("text x")},
        texture_y       { Primitive("text y")},
        texture_px_x    { Primitive("text px x")},
        texture_px_y    { Primitive("text px y")},
        main_view_x     { Primitive("x")},
        main_view_y     { Primitive("y")}
{

    id = "UIC_Draw_Transform_MainViewPos";
    set_w("90%");
    set_h("200x");

    set_x("|");
    set_y("_10x");
    
    set_color({0.0, 0.0, 1.0, 0.3});

    appendChild(&title);
    title.str_setFontSize(FontSize::f18);
    title.set_x("<10x");
    title.set_y("^5x");


    appendChild(&zoom);
    zoom.str_fontSize = FontSize::f18;
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
    main_view_x.str_fontSize = FontSize::f18;
    main_view_x.set_x("<50x");
    main_view_x.set_y("_20x");

    appendChild(&main_view_y);
    main_view_x.str_fontSize = FontSize::f15;
    main_view_y.set_x("<150x");
    main_view_y.set_y("_20x");

}

void UIC_Draw_Transform::render_component(){
    render();

    title.render();

    zoom.render();
    
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

    // Trigger rerender on main_view position change
    if( transform_context.texture_px_x != transform_context_cache.texture_px_x 
        || transform_context.texture_px_y != transform_context_cache.texture_px_y 
        || transform_context.zoom != transform_context_cache.zoom 
    ){
        transform_context_cache.texture_px_x    = transform_context.texture_px_x;
        transform_context_cache.texture_px_y    = transform_context.texture_px_y;
        transform_context_cache.zoom            = transform_context.zoom;

        zoom.update_str_double(transform_context.zoom, 4);

        pan_x.update_str_double(transform_context.pan_texture_coords_x, 5);
        pan_y.update_str_double(transform_context.pan_texture_coords_y, 5);


        texture_x.update_str_double(transform_context.texture_x, 5);
        texture_y.update_str_double(transform_context.texture_y, 5);
        
        texture_px_x.update_str_int(transform_context.texture_px_x);
        texture_px_y.update_str_int(transform_context.texture_px_y);

        
        main_view_x.update_str_double(transform_context.main_view_x, 5);
        main_view_y.update_str_double(transform_context.main_view_y, 5);
    }
}


// UIC_Draw_Transform::UIC_Draw_Transform()
//     :   boundObject { draw::ui_get_transform_context() },
//         title { Primitive("Transform") },
//         main_view_pos { UIC_Draw_Transform_MainViewPos() }
// {
//     id = "prim_color";
//     // set_w("180x");
//     set_w("96%");
//     set_x("<2%");
    
//     set_h("20%");
//     // set_color_texture(ColorTexture::DarkGray);
//     set_color(active_pallete.detail1);


//     appendChild(&title);
//     title.str_setFontSize(FontSize::f18);
//     title.set_x("<10x");
//     title.set_y("^5x");

//     appendChild(&main_view_pos);
//     main_view_pos.set_x("<0x");
//     main_view_pos.set_y("_10x");

    
// }

// void UIC_Draw_Transform::update_component() {
//     main_view_pos.update_component();
// }

// void UIC_Draw_Transform::render_component() {
//     render();

//     title.render();
//     main_view_pos.render_component();
// }

// UiResult UIC_Draw_Transform::try_find_target_component(double x, double y) {
//     // Return if point is not contain within the root component primitive
//     if (!containsPoint(x, y))
//         return UiResult();

//     if (title.containsPoint(x, y))
//         return UiResult(true, CAction::None, &title);

//     if (main_view_pos.containsPoint(x, y))
//         return UiResult(true, CAction::None, &main_view_pos);

//     return UiResult(true, CAction::None, this);

// }



}
