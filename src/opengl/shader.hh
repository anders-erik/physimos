#pragma once


#include <string>
#include <lib/str.hh>
#include <lib/file.hh>


namespace opengl {

class Shader {

    unsigned int shader_id;

    Str shaders_dir_path = "src/opengl/shaders/";
    Str program_name_str; // Path inside shaders directory

    Str path_core_full = "";
    
    File file;


public:

    enum Type {
        Vertex,
        Fragment,
    } type;

    

    Shader() {};
    Shader(Str program_name);

    void set_shader_type(Type type);
    void set_program_name(Str program_name);

    /** Compiles shader and returns shader id. */
    unsigned int compile();
    unsigned int get_shader_id();
    void delete_shader_id();


private:

    void assemble_core_path();

    void check_error();

};


}

