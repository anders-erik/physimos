
#include "window.hh"
#include "gpu.hh"
#include "fs.hh"
#include "scene.hh"

using namespace xprubik;


int main(){

    bool fs_ok = fs_init();
    if(!fs_ok) exit(1);

    bool window_ok = window_init();
    if(!window_ok) return 1;

    bool gpu_ok = gpu_init();
    if(!gpu_ok) return 1;

    bool scene_ok = scene_init();
    if(!scene_ok) return 1;

    
    // gpu_use_program(Shader::Model);

    
    

    while (!window_should_close())
    {

        window_new_frame();

        InputState input_state = window_get_input();
        if(input_state.esc)
            window_set_close();


        // gpu_draw_triangle_moodel_shader();

        scene_render();

        window_end_of_frame();
    }
    


}

