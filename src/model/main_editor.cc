#include <iostream>

#include "window/auxwin.hh"

#include "scene.hh"

int main(){

    window::Auxwin auxwin(800, 600);

    std::cout << "Hello from editor!" << std::endl;

    // xpeditor::scene_init();
    xpeditor::Scene scene;
    
    while(auxwin.is_open()){
        auxwin.new_frame();

        std::queue<window::InputEvent> events = auxwin.get_input_events();
        while(events.size() > 0){
            window::InputEvent event = events.front();
            
            if(event.type == window::EventType::Keystroke && event.key_stroke.key == window::Key::Esc)
                auxwin.close();

            scene.handle_input(event);
            
            events.pop();
        }

        scene.update();
        scene.render();

        auxwin.end_frame();
    }


    return 0;
}