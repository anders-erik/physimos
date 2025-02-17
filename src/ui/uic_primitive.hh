#ifndef UIC_PRIMITIVE_HH
#define UIC_PRIMITIVE_HH

#include "ui/ui_component.hh"

#include "ui/ui_primitive.hh"


namespace UI::component {

class PrimitiveComponent;


struct BasePrimitive : public ::UI::Primitive {
    PrimitiveComponent& primitiveComponent;

    virtual void reload() {};
    
    BasePrimitive(PrimitiveComponent& _primitiveComponent) : 
        primitiveComponent{ _primitiveComponent } 
        {}
};



struct ContainerPrimitive : public UI::component::BasePrimitive {

    ContainerPrimitive(PrimitiveComponent& _primitiveComponent) : 
        BasePrimitive(_primitiveComponent) 
        {}
};

struct IdPrimitive : public UI::component::BasePrimitive {
    IdPrimitive(PrimitiveComponent& _primitiveComponent) : 
        BasePrimitive(_primitiveComponent) 
        {}
};

// struct IdPrimitive : public UI::component::BasePrimitive {
//     // IdPrimitive(PrimitiveComponent& primitiveComponent);
//     void reload();
// };




class PrimitiveComponent {
// Linking error when trying to derive from Component
// class PrimitiveComponent : public ::UI::Component {

public:
    // 
    ::UI::Primitive& boundObject;

    ContainerPrimitive container;
    IdPrimitive idPrimitive;

    
    void newPrimitive(::UI::Primitive& ui_primitive);

    PrimitiveComponent(::UI::Primitive& ui_primitive) :     
        boundObject { ui_primitive },
        container   { *this },
        idPrimitive { *this }
        {};

};



}


#endif