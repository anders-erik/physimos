#include <iostream>


#include "lib/lib.hh"

#include "draw/draw.hh"

#include <random>

#include "uic_primitive_color.hh"
#include "ui/uic/draw/uic_draw_transform.hh"

namespace UI::component {


UIC_Draw_Transform_MainViewPos::UIC_Draw_Transform_MainViewPos()
    :   main_view_x { Primitive("x")},
        main_view_y { Primitive("y")},
        texture_x   { Primitive("text x")},
        texture_y   { Primitive("text y")},
        pan_x       { Primitive("pan x")},
        pan_y       { Primitive("pan y")},
        zoom        { Primitive("zoom")}
{
    id = "prim_color_set_red";
    set_w("20x");
    set_h("20x");
    set_color({1.0, 0.0, 0.0, 0.5});

    appendChild(&main_view_x);
    main_view_x.str_fontSize = FontSize::f18;
    main_view_x.set_x("<50x");
    main_view_x.set_y("_30x");

    appendChild(&main_view_y);
    main_view_x.str_fontSize = FontSize::f18;
    main_view_y.set_x("<150x");
    main_view_y.set_y("_30x");

    appendChild(&texture_x);
    texture_x.str_fontSize = FontSize::f18;
    texture_x.set_x("<50x");
    texture_x.set_y("_70x");

    appendChild(&texture_y);
    texture_y.str_fontSize = FontSize::f18;
    texture_y.set_x("<150x");
    texture_y.set_y("_70x");

    appendChild(&pan_x);
    pan_x.str_fontSize = FontSize::f18;
    pan_x.set_x("<50x");
    pan_x.set_y("_110x");

    appendChild(&pan_y);
    pan_y.str_fontSize = FontSize::f18;
    pan_y.set_x("<150x");
    pan_y.set_y("_110x");


    appendChild(&zoom);
    zoom.str_fontSize = FontSize::f18;
    zoom.set_x("<100x");
    zoom.set_y("_140x");
}

void UIC_Draw_Transform_MainViewPos::render_component(){
    render();

    main_view_x.render();
    main_view_y.render();

    texture_x.render();
    texture_y.render();

    pan_x.render();
    pan_y.render();

    zoom.render();
}

void UIC_Draw_Transform_MainViewPos::update_component(){
    
    UIC_Draw_Transform* transform_component = (UIC_Draw_Transform*) parent;
    draw::TransformContext transform_context = transform_component->boundObject;

    // Trigger rerender on main_view position change
    if( transform_context.main_view_x != transform_context_cache.main_view_x 
        || transform_context.main_view_y != transform_context_cache.main_view_y 
        || transform_context.zoom != transform_context_cache.zoom 
    ){
        // std::cout << "new draw tranform" << std::endl;
        transform_context_cache.main_view_x = transform_context.main_view_x;
        transform_context_cache.main_view_y = transform_context.main_view_y;
        transform_context_cache.zoom = transform_context.zoom;

        main_view_x.update_str_double(transform_context.main_view_x, 7);
        main_view_y.update_str_double(transform_context.main_view_y, 7);

        texture_x.update_str_double(transform_context.texture_x, 7);
        texture_y.update_str_double(transform_context.texture_y, 7);

        pan_x.update_str_double(transform_context.pan_texture_coords_x, 7);
        pan_y.update_str_double(transform_context.pan_texture_coords_y, 7);

        zoom.update_str_double(transform_context.zoom, 7);
    }
}


UIC_Draw_Transform::UIC_Draw_Transform()
    :   boundObject { draw::ui_get_transform_context() },
        title { Primitive("Transform") },
        main_view_pos { UIC_Draw_Transform_MainViewPos() }
{
    id = "prim_color";
    // set_w("180x");
    set_w("96%");
    set_x("<2%");
    
    set_h("200x");
    // set_color_texture(ColorTexture::DarkGray);
    set_color(active_pallete.detail1);


    appendChild(&title);
    title.str_setFontSize(FontSize::f18);
    title.set_x("<10x");
    title.set_y("^5x");

    appendChild(&main_view_pos);
    main_view_pos.set_x("<0x");
    main_view_pos.set_y("_10x");

    
}

void UIC_Draw_Transform::update_component() {
    main_view_pos.update_component();
}

void UIC_Draw_Transform::render_component() {
    render();

    title.render();
    main_view_pos.render_component();
}

UiResult UIC_Draw_Transform::try_find_target_component(double x, double y) {
    // Return if point is not contain within the root component primitive
    if (!containsPoint(x, y))
        return UiResult();

    if (title.containsPoint(x, y))
        return UiResult(true, CAction::None, &title);

    if (main_view_pos.containsPoint(x, y))
        return UiResult(true, CAction::None, &main_view_pos);

    return UiResult(true, CAction::None, this);

}



}
