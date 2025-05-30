#pragma once


#include <string>
#include <lib/str.hh>
#include <lib/file.hh>


namespace opengl {

struct Shader {
    enum Type {
        Vertex,
        Fragment,
    };
    unsigned int shader_id;
    Type type;
    Str path_base;
    Str path_core_full;
    File file;

    Shader() {};
    Shader(Str path_base);

    void set_full_core_path(Str& path);

    /** Compile single vertex and fragment shader, links program, and returns program name. */
    unsigned int compile();
    unsigned int get_shader_id();
    void delete_shader_id();
    void check_error();
};


}

