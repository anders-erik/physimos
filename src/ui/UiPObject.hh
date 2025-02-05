#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "WorldObject.hpp"

#include "ui/ui_primitive.hh"


namespace UI::PObject {

    // Primitive with a WorldObject pointer. 
    struct Base : public UI::Primitive {
        ::WorldObject* pObject = nullptr;
        virtual void setPObject(::WorldObject* _pObject) {};
        virtual void reload() {};

        Base(){};
    };


    struct Container : public UI::PObject::Base {
        Container(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
    };


    struct NameLabel : public UI::PObject::Base {
        NameLabel(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
    };


    struct XPosition : public UI::PObject::Base {
        XPosition(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
        void reload();
    };
    struct XPosIncrease : public UI::PObject::Base {
        XPosIncrease(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
        UI::Action click();
    };
    struct XPosDecrease : public UI::PObject::Base {
        XPosDecrease(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
        UI::Action click();
    };


    struct YPosition : public UI::PObject::Base {
        YPosition(::WorldObject* _pObject);
    };
    struct YPosIncrease : public UI::PObject::Base {
        YPosIncrease(::WorldObject* _pObject);
        UI::Action click();
    };
    struct YPosDecrease : public UI::PObject::Base {
        YPosDecrease(::WorldObject* _pObject);
        UI::Action click();
    };


    struct ZPosition : public UI::PObject::Base {
        ZPosition(::WorldObject* _pObject);
    };
    struct ZPosIncrease : public UI::PObject::Base {
        ZPosIncrease(::WorldObject* _pObject);
        UI::Action click();
    };
    struct ZPosDecrease : public UI::PObject::Base {
        ZPosDecrease(::WorldObject* _pObject);
        UI::Action click();
    };



    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::PObject::Base {
    public:
        ToggleWireframe(::WorldObject* _pObject);
        void setPObject(::WorldObject* _pObject);
        UI::Action click();
    };





    // Primitive* newComponent(::WorldObject* _pObject);


    // Full context ui component for PObjects (formerly WorldObject)
    class Context {
    public:
        Container* container = nullptr;
        NameLabel* nameLabel = nullptr;
        XPosition* xPosition = nullptr;
        XPosIncrease* xPosIncrease = nullptr;
        XPosDecrease* xPosDecrease = nullptr;
        YPosition* yPosition = nullptr;
        YPosIncrease* yPosIncrease = nullptr;
        YPosDecrease* yPosDecrease = nullptr;
        ZPosition* zPosition = nullptr;
        ZPosIncrease* zPosIncrease = nullptr;
        ZPosDecrease* zPosDecrease = nullptr;
        ToggleWireframe* toggleWireframe = nullptr;

        Context() {};
        
        void populateContext(::WorldObject* _pObject);
        void newPObject(::WorldObject* _pObject);

        void reloadComponent();

    };

    extern Context* uiPObjectContext;
}


#endif