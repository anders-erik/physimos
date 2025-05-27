#include <iostream>

#include "window/auxwin.hh"
#include "scene/scene.hh"

int main(){

    window::Auxwin auxwin;
    auxwin.init(800, 600);

    std::cout << "Hello from editor!" << std::endl;


    scene::SceneModelEditor scene;
    
    while(auxwin.end_frame()){
        // auxwin.new_frame();

        std::vector<window::InputEvent> events = auxwin.new_frame();
        for(window::InputEvent& event : events){
            // window::InputEvent event = events.back();
            
            // if(event.type == window::EventType::Keystroke && event.key_stroke.key == window::Key::Esc)
            //     auxwin.close();

            scene.handle_input(event);
            
            // events.pop_back();
        }

        scene.update();
        scene.render();

        // auxwin.end_frame();
    }


    return 0;
}