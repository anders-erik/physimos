#pragma once

namespace xpcube {

enum class Shader {
    Model,
};

bool gpu_init();

void gpu_draw_triangle_moodel_shader();


unsigned int gpu_build_vert_frag_program(Shader shader_enum);

bool gpu_shader_link_check(unsigned int program);
bool gpu_shader_compile_check(unsigned int program);

void gpu_use_program(Shader shader_enum);


}