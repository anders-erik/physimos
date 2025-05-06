#ifndef UIC_DRAW_TRANSFORM_HH
#define UIC_DRAW_TRANSFORM_HH


#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"
#include "ui/uic/uic_label.hh"

#include "draw/draw.hh"


namespace UI::component {


struct UIC_Draw_Transform : public ::UI::Primitive {

    draw::TransformContext& boundObject;
    draw::TransformContext transform_context_cache;

    Primitive title;

    UIC_Label_float zoom;
    UIC_Label_f2 pan;
    UIC_Label_f2 scene_pos;
    UIC_Label_f2 scene_pos_px;
    UIC_Label_f2 main_view_px;


    void update_component();
    void render_component();

    UIC_Draw_Transform();
};


}


#endif