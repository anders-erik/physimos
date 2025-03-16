#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "window.hh"
#include "gpu.hh"
#include "fs.hh"



int main(){

    bool fs_ok = fs_init();
    if(!fs_ok) exit(1);

    bool window_ok = window_init();
    if(!window_ok) return 1;


    
    bool build_ok = gpu_build_vert_frag_program(Shader::Model);
    if(!build_ok) exit(1);

    // std::cout << "Hello cube"  << std::endl;
    
    gpu_use_program(Shader::Model);
    // glUseProgram(program);

    
    

    while (!window_should_close())
    {   
        window_new_frame();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        InputState input_state = window_get_input();


        if(input_state.esc)
            window_set_close();

        window_end_of_frame();
    }
    


}


