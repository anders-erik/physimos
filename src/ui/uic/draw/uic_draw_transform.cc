#include <iostream>


#include "lib/lib.hh"

#include "draw/draw.hh"

#include <random>

#include "uic_primitive_color.hh"
#include "ui/uic/draw/uic_draw_transform.hh"

namespace UI::component {


UIC_Draw_Transform_MainViewPos::UIC_Draw_Transform_MainViewPos()
    :   main_view_x { Primitive("x")},
        main_view_y { Primitive("y")}
{
    id = "prim_color_set_red";
    set_w("20x");
    set_h("20x");
    set_color({1.0, 0.0, 0.0, 0.5});

    appendChild(&main_view_x);
    main_view_x.str_fontSize = FontSize::f18;
    main_view_x.set_x("<100x");
    main_view_x.set_y("_50x");

    appendChild(&main_view_y);
    main_view_x.str_fontSize = FontSize::f18;
    main_view_y.set_x("<100x");
    main_view_y.set_y("_80x");
}

void UIC_Draw_Transform_MainViewPos::render_component(){
    render();

    main_view_x.render();
    main_view_y.render();
}

void UIC_Draw_Transform_MainViewPos::update_component(){
    
    UIC_Draw_Transform* transform_component = (UIC_Draw_Transform*) parent;
    draw::TransformContext transform_context = transform_component->boundObject;

    if(transform_context.main_view_x != transform_context_cache.main_view_x || transform_context.main_view_y != transform_context_cache.main_view_y ){
        // std::cout << "new draw tranform" << std::endl;
        transform_context_cache.main_view_x = transform_context.main_view_x;
        transform_context_cache.main_view_y = transform_context.main_view_y;

        main_view_x.update_str_double(transform_context_cache.main_view_x, 7);
        main_view_y.update_str_double(transform_context_cache.main_view_y, 7);
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
