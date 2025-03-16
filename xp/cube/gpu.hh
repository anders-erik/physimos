#pragma once

enum class Shader {
    Model,
};

unsigned int gpu_build_vert_frag_program(Shader shader_enum);

bool gpu_shader_link_check(unsigned int program);
bool gpu_shader_compile_check(unsigned int program);

void gpu_use_program(Shader shader_enum);