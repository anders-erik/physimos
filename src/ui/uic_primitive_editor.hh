#ifndef UIC_PRIMITIVE_HH
#define UIC_PRIMITIVE_HH

#include "ui/ui_component.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

class Component_PrimitiveEditor;


struct BasePrimitive : public ::UI::Primitive {
    Component_PrimitiveEditor& primitiveComponent;

    virtual void reload() {};
    
    BasePrimitive(Component_PrimitiveEditor& _primitiveComponent) : 
        primitiveComponent{ _primitiveComponent } 
        {}
};



struct PrimitiveEditor_Container : public UI::component::BasePrimitive {
    PrimitiveEditor_Container(Component_PrimitiveEditor& _primitiveComponent);
};

struct PrimitiveEditor_Title : public UI::component::BasePrimitive {
    std::string title_text = "Primitive Editor";
    PrimitiveEditor_Title(Component_PrimitiveEditor& _primitiveComponent);
};

struct IdPrimitive : public UI::component::BasePrimitive {
    std::string id;
    void reload();

    IdPrimitive(Component_PrimitiveEditor& _primitiveComponent);
};

// struct IdPrimitive : public UI::component::BasePrimitive {
//     // IdPrimitive(PrimitiveComponent& primitiveComponent);
//     void reload();
// };




class Component_PrimitiveEditor {
// Linking error when trying to derive from Component
// class PrimitiveComponent : public ::UI::Component {

public:
    ::UI::Primitive& boundObject;

    PrimitiveEditor_Container container;
    PrimitiveEditor_Title title;
    IdPrimitive idPrimitive;

    
    void newPrimitive(::UI::Primitive& ui_primitive);

    Component_PrimitiveEditor(::UI::Primitive& ui_primitive);
};



}


#endif