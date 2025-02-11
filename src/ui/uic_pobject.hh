#ifndef UI_WORLDOBJECT_HH
#define UI_WORLDOBJECT_HH

#include "scene/pobject.hh"

#include "ui/ui_primitive.hh"
#include "ui/uic_transform.hh"


namespace UI::component {

    class PobjectComponent;

    // Primitive with a WorldObject pointer. 
    struct PobjectBase : public UI::Primitive {
        ::PObject* pObject = nullptr;
        // virtual void setPObject(::PObject* _pObject) {};
        virtual void reload() {};

        PobjectBase(){};
    };

    struct PobjectContainer : public UI::component::PobjectBase {
        PobjectContainer(PobjectComponent* _componentContext);
        // void setPObject(::PObject* _pObject);
    };

    struct PobjectNameLabel : public UI::component::PobjectBase {
        PobjectNameLabel(PobjectComponent* component);
        void reload();
        // void setPObject(::PObject* _pObject);
    };

    // Setting to public to be able to use primitive base pointer without cast
    class ToggleWireframe : public UI::component::PobjectBase {
    public:
        ToggleWireframe(PobjectComponent* _componentContext);
        // void setPObject(::PObject* _pObject);
        UI::Action click();
    };



    // Full context ui component for PObjects (formerly WorldObject)
    class PobjectComponent {
    public:
        ::PObject* boundObject = nullptr;

        PobjectContainer* container = nullptr;
        PobjectNameLabel* nameLabel = nullptr;
        ToggleWireframe* toggleWireframe = nullptr;

        ::UI::component::transform::TransformComponent* transformContext = nullptr;


        
        void populateContext(::PObject* _pObject);
        void newPObject(::PObject* _pObject);

        void reloadModule();

        PobjectComponent() {};
    };

    extern PobjectComponent* pobjectContext;
}


#endif