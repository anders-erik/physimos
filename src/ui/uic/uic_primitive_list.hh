#ifndef UIC_PRIMITIVE_LIST_HH
#define UIC_PRIMITIVE_LIST_HH

#include <vector>
#include <list>

#include "ui/ui_globals.hh"
#include "ui/ui_primitive.hh"

#include "uic/uic_primitive_color.hh"
#include "uic/uic_primitive_list.hh"


namespace UI::component {

// extern ::UI::component::UIC_PrimitiveColor color_editor_1;

/** Returns a */
void store(Primitive *_parent, ::UI::Primitive& _primitive);

struct UIC_PrimitiveList : public ::UI::Primitive {

    ::UI::Primitive& boundObject; // Primitive to edit

    Primitive title;

    int list_item_stride_px = 40;
    
    std::vector<::UI::component::UIC_PrimitiveColor> listObjects; // owns these list omponents

    // std::list<::UI::component::UIC_PrimitiveColor> listObjects; // owns these list omponents

    // std::list<::UI::component::UIC_PrimitiveColor*> listObjectPtnrs;
    
    void new_list_object();

    UiResult try_find_target_component(double x, double y);
    void render_component();
    
    UIC_PrimitiveList(::UI::Primitive& _primitive);
};



}


#endif